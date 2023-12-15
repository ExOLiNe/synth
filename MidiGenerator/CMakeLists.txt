cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME MidiGenerator)
set(CMAKE_CXX_STANDARD 20)

project(${PROJECT_NAME} VERSION 0.0.1)

#add_subdirectory(${JUCE_SRC} ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

juce_add_plugin("${PROJECT_NAME}"
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "LoudPug"
        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT TRUE
        IS_MIDI_EFFECT TRUE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE LPug
        PLUGIN_CODE LPUG
        FORMATS VST3
        PRODUCT_NAME "LoudPug${PROJECT_NAME}${CMAKE_BUILD_TYPE}"
)

target_sources(${PROJECT_NAME} PRIVATE
    Source/PluginProcessor.cpp
)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall -Wextra #[[-Werror]] #[[-Wno-old-style-cast]]
            -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-undef
            -Wno-documentation-unknown-command
            )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
endif()

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        juce::juce_core
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)


option(BUILD_UNIT_TESTS "Build tests" ON)

if (BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()