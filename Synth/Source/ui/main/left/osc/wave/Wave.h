#pragma once

#include <juce_core.h>
#include <juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>
#include <sstream>
#include <mutex>
#include <map>
#include "../../../../../Constants.h"
#include <Tracy.hpp>

#define DEFAULT_COLOUR 0.2f
#define HIGHLIGHTED_COLOUR 1.0f

static void gLClearError() {
    using namespace juce::gl;
    while(glGetError() != GL_NO_ERROR);
}

static bool glLogCall(const char* function, const char* file, int line) {
#include <iostream>
    using namespace juce::gl;
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

#define GL_CALL(x) gLClearError();\
    x;\
    jassert(glLogCall(#x, __FILE__, __LINE__))

class Wave : public juce::OpenGLAppComponent/*, public juce::AudioProcessorValueTreeState::Listener*/ {
public:
    Wave(juce::AudioProcessorValueTreeState& treeState, juce::String oscId) :
    zIndex(treeState.getRawParameterValue(oscId + params::osc::wtPos.name)) {
        openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::OpenGLVersion::openGL4_3);

        draggableOrientation.reset(juce::Vector3D<float>(0.0, 1.0, 0.0));
    }

    /*void parameterChanged (const juce::String& parameterID, float newValue) override {
        setZHighlight((int) (newValue / 100 * zSize) - 1);
    }*/

    void mouseDown(const juce::MouseEvent &event) override {
        draggableOrientation.mouseDrag(event.getPosition());
    }

    void mouseDrag(const juce::MouseEvent &event) override {
        draggableOrientation.mouseDrag(event.getPosition());
    }

    ~Wave() override { shutdownOpenGL(); }

    void initialise() override {
        createShaders();
        draggableOrientation.mouseDrag(juce::Point{260.0f, 160.0f});
    }

    void shutdown() override {
        prepare = false;
        shader.reset();
        uniforms.reset();
    }

    juce::Matrix3D<float> getProjectionMatrix() const {
        auto w = 1.2;
        auto h = w * getLocalBounds().toFloat().getAspectRatio(false); // [2]

        return juce::Matrix3D<float>::fromFrustum(-w, w, -h, h, 4.0f, 30.0f); // [3]
    }

    juce::Matrix3D<float> getViewMatrix() const {
        auto viewMatrix =
                juce::Matrix3D<float>::fromTranslation({0.0f, 0.0f, -10.0f}); // [4]
        auto rotationMatrix = draggableOrientation.getRotationMatrix();

        return viewMatrix * rotationMatrix;
    }

    template <typename T>
    using zVector = std::vector<T>;
    using xVector = std::vector<float>;

    void setData(const zVector<xVector>& data) {
        renderMutex.lock();

        if (!indices.empty()) {
            indices.erase(indices.begin(), indices.end());
        }
        if (!vertices.empty()) {
            vertices.erase(vertices.begin(), vertices.end());
        }

        zSize = data.size();

        xSize = data.at(0).size();

        float maxValueByAbs = 0;

        for (size_t z = 0; z < zSize; ++z) {
            float zPos = normalizeIndex(z, zSize);

            jassert(data.at(z).size() == xSize);

            for (size_t x = 0; x < xSize; ++x) {
                float xPos = normalizeIndex(x, xSize);
                float y = data.at(z).at(x);
                float yAbs = std::abs(y);
                if (yAbs > maxValueByAbs) {
                    maxValueByAbs = yAbs;
                }
                vertices.push_back({xPos, y, zPos, (float)z});
            }
        }

        std::for_each(vertices.begin(), vertices.end(), [&](std::array<float, 4>& vertex) {
            vertex[1] /= maxValueByAbs;
        });

        for (size_t z = 0; z < zSize - 1; ++z) {
            for (unsigned int x = 0; x < xSize - 1; ++x) {
                indices.push_back((xSize * z) + x);
                indices.push_back((xSize * z) + x + 1);
                indices.push_back((xSize * (z + 1)) + x);

                indices.push_back((xSize * z) + x + 1);
                indices.push_back((xSize * (z + 1)) + x + 1);
                indices.push_back((xSize * (z + 1)) + x);
            }
        }
        prepare = false;
        renderMutex.unlock();
    }

    void render() override {
#ifdef PROFILING_ENABLED
        ZoneScoped;
#endif
        unsigned int zHighlighted = ((static_cast<unsigned int>(zIndex->load() / 100 * zSize) - 1));
        using namespace ::juce::gl;

        jassert(juce::OpenGLHelpers::isContextActive());

        auto desktopScale = (float) openGLContext.getRenderingScale(); // [1]
        juce::OpenGLHelpers::clear(getLookAndFeel().findColour(
                juce::ResizableWindow::backgroundColourId)); // [2]

        glEnable(GL_BLEND);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPointSize(3.0f);

        glViewport(0, 0, (float) getWidth() * desktopScale,
                   (float) getHeight() * desktopScale);

        shader->use();

        if (uniforms->projectionMatrix.get() != nullptr)
            uniforms->projectionMatrix->setMatrix4(getProjectionMatrix().mat, 1,
                                                   false);

        if (uniforms->viewMatrix.get() != nullptr)
            uniforms->viewMatrix->setMatrix4(getViewMatrix().mat, 1, false);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*if (prepare) {
            draw();
            return;
        } else {
            draw();
            prepare = true;
        }*/

        if (!prepare) {
            GL_CALL(glGenVertexArrays(1, &vao));

            GL_CALL(glBindVertexArray(vao));

            // Create a vertex buffer
            GL_CALL(glGenBuffers(1, &vertexBuffer));
            GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));

            // Upload the point data to the vertex vertexBuffer
            GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(std::array<float, 4>),
                                 vertices.data(), GL_STATIC_DRAW));

            // Attributes for z index
            GL_CALL(GLint zIndexAttrib = glGetAttribLocation(shader->getProgramID(), "zIndex"));
            GL_CALL(glVertexAttribPointer(zIndexAttrib, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3*sizeof(float))));
            GL_CALL(glEnableVertexAttribArray(1));

            // Attributes for vertices
            GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0));
            GL_CALL(glEnableVertexAttribArray(0));


            // Attributes for highlighted
            GL_CALL(highlightedIndex = glGetUniformLocation(shader->getProgramID(), "highlightedIndex"));
            GL_CALL(glUniform1f(highlightedIndex, zHighlighted));

            // Create an index buffer
            GL_CALL(glGenBuffers(1, &indexBuffer));
            GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));


            // Upload the index data to the index buffer
            GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                                 indices.data(), GL_STREAM_DRAW));
            GL_CALL(glBindVertexArray(0));
            prepare = true;
            return;
        }

        GL_CALL(glUniform1f(highlightedIndex, zHighlighted));
        GL_CALL(glBindVertexArray(vao));

        draw();

        GL_CALL(glBindVertexArray(0));

        return;
        // Delete the vertex vertexBuffer
        glDisableVertexAttribArray(highlightedIndex);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &indexBuffer);
        openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
        openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //renderMutex.unlock();
    }

    void draw() {
        using namespace juce::gl;
        // Draw the points
        GL_CALL(glDrawElements(GL_TRIANGLES, indices.size() * sizeof(GLuint),
                               GL_UNSIGNED_INT, 0));
    }

    //! [createShaders]
    void createShaders() {
        std::stringstream vertexShader;
        vertexShader << R"(
            attribute vec3 position;
            attribute float zIndex;

            uniform float highlightedIndex;
            uniform mat4 projectionMatrix;
            uniform mat4 viewMatrix;

            out vec4 pos;
            out float zInd;

            void main()
            {
                pos = vec4(position, 1.0);
                zInd = zIndex;
                gl_Position = projectionMatrix * viewMatrix * pos;
            })";

        std::stringstream fragmentShader;
        fragmentShader <<
                       R"(
            in vec4 pos;
            in float zInd;
            uniform float highlightedIndex;
            void main()
            {)"
                       #if JUCE_OPENGL_ES
                       R"(    lowp vec4 colour = vec4(0.95, 0.57, 0.03, 0.7);)"
                       #else
                       R"(    vec4 colour = vec4(0.99, 0.99, 0.00, 1.0);)"
                       #endif
                       R"(
                float alpha = )" << DEFAULT_COLOUR << R"(;
                if (int(highlightedIndex) == int(zInd)) {
                    alpha = )" << HIGHLIGHTED_COLOUR << R"(;
                }
                gl_FragColor = vec4(0.0, pos.x, 1.0 - pos.x, alpha);
            })";

        std::unique_ptr<juce::OpenGLShaderProgram> newShader(
                new juce::OpenGLShaderProgram(openGLContext));

        if (newShader->addVertexShader(
                juce::OpenGLHelpers::translateVertexShaderToV3(vertexShader.str()))
            &&
            newShader->addFragmentShader(
                    juce::OpenGLHelpers::translateFragmentShaderToV3(fragmentShader.str())) &&
            newShader->link()) {
            uniforms.reset();

            shader.reset(newShader.release());
            shader->use();

            uniforms.reset(new Uniforms(*shader));
        } else {
            juce::Logger::writeToLog(newShader->getLastError());
        }
    }

    void paint(juce::Graphics &) override {

    }

    void resized() override {
        draggableOrientation.setViewport(getLocalBounds());
    }

private:

    // This class just manages the uniform values that the demo shaders use.
    struct Uniforms {
        explicit Uniforms(juce::OpenGLShaderProgram &shaderProgram) {
            projectionMatrix.reset(createUniform(shaderProgram, "projectionMatrix"));
            viewMatrix.reset(createUniform(shaderProgram, "viewMatrix"));
        }
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> projectionMatrix,
                viewMatrix;
    private:
        static juce::OpenGLShaderProgram::Uniform *
        createUniform(juce::OpenGLShaderProgram &shaderProgram,
                      const juce::String &uniformName) {
            using namespace ::juce::gl;

            if (glGetUniformLocation(shaderProgram.getProgramID(),
                                     uniformName.toRawUTF8()) < 0)
                return nullptr;

            return new juce::OpenGLShaderProgram::Uniform(shaderProgram,
                                                          uniformName.toRawUTF8());
        }
    };

    static inline float normalizeIndex(const size_t index, const size_t total) {
        return -1 + (float(index) / ((float)total - 1)) * 2;
    }

    std::vector<std::array<float, 4>> vertices;
    std::vector<GLuint> indices;

    //unsigned int zHighlighted = 0;
    size_t xSize;
    size_t zSize;

    bool prepare = false;

    std::mutex renderMutex;

    juce::Draggable3DOrientation draggableOrientation;

    std::unique_ptr<juce::OpenGLShaderProgram> shader;
    std::unique_ptr<Uniforms> uniforms;

    GLuint highlightedIndex;
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLuint vao;

    std::atomic<float> *zIndex;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Wave)
};