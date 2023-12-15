set(PROJECT_NAME MidiGeneratorUnitTestRunner)
project(${PROJECT_NAME} VERSION 1.0)

find_package(catch2 REQUIRED)

juce_add_console_app(${PROJECT_NAME} PRODUCT_NAME "Unit Test Runner")

target_sources(${PROJECT_NAME} PRIVATE RingIndexTest.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_UNIT_TESTS=1)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Catch2WithMain
        MidiGenerator
        juce_recommended_config_flags
        juce_recommended_lto_flags
        juce_recommended_warning_flags
        juce_core)

catch_discover_tests(${PROJECT_NAME})