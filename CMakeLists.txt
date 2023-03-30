cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME Synth)

project(${PROJECT_NAME} VERSION 0.0.1)

function(add_images_from_directory target assetsSubFolder)
    set (AssetsDir ${CMAKE_CURRENT_LIST_DIR}/${assetsSubFolder})
    file(GLOB_RECURSE images
            "${AssetsDir}/*.svg")

    if(NOT images STREQUAL "")
        set (ImagesTarget "${target}-Images")
        juce_add_binary_data(${ImagesTarget}
                HEADER_NAME BinaryData.h
                SOURCES ${images})
        target_link_libraries(${target} PRIVATE ${ImagesTarget})
    endif()
endfunction()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#Compile commands, useful for some IDEs like VS-Code
#set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_subdirectory(JUCE)

juce_add_plugin("${PROJECT_NAME}"
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "LoudPug"
        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE LPug
        PLUGIN_CODE LPUG
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "LoadPug${PROJECT_NAME}")

add_images_from_directory(${PROJECT_NAME} Assets)

target_sources(${PROJECT_NAME}
        PRIVATE
        Source/Constants.cpp
        Source/other/Grid.cpp
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/ui/TopPanel.cpp
        Source/ui/BodyPanel.cpp
        Source/ui/MainPanel.cpp
        Source/ui/MatrixPanel.cpp
        Source/ui/MasterPanel.cpp
        Source/ui/main/LeftPanel.cpp
        Source/ui/main/RightPanel.cpp
        Source/ui/main/right/FmOscPanel.cpp
        Source/ui/main/right/FilterPanel.cpp
        Source/ui/main/right/EnvPanel.cpp
        Source/ui/main/right/env/adsr/ADSRWave.cpp
        Source/ui/main/right/env/adsr/ADSRPanel.cpp
        Source/ui/main/right/env/lfo/LFOWave.cpp
        Source/ui/main/right/env/lfo/LFOPanel.cpp
        Source/ui/main/right/env/Body.cpp
        Source/ui/top/LogoComponent.cpp
        Source/ui/top/MenuComponent.cpp
        Source/ui/top/PresetComponent.cpp
        Source/ui/main/left/OscPanel.cpp
        Source/ui/main/left/osc/WaveComponent.cpp
        Source/ui/main/left/osc/Controls.cpp

        Source/audio/osc/SynthVoice.cpp
        Source/audio/osc/SynthSound.cpp
        Source/audio/wave/WaveTables.cpp
        Source/audio/lfo/LFO.cpp
        )

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

#if(MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
#else()
#    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
#endif()


target_link_libraries(${PROJECT_NAME}
        PRIVATE
        juce::juce_core
        juce::juce_gui_extra
        juce::juce_graphics
        juce::juce_opengl
        juce::juce_audio_utils
        juce::juce_audio_devices
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

option(BUILD_UNIT_TESTS "Build tests" ON)
#add_compile_definitions(DEBUG_BUFFERS)

if (BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()