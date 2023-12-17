project(MemoryUnitTestRunner VERSION 1.0)

find_package(catch2 REQUIRED)

juce_add_console_app(${PROJECT_NAME} PRODUCT_NAME "Unit Test Runner")

target_sources(${PROJECT_NAME} PRIVATE PolymorphicReadonlyArrayTest.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_UNIT_TESTS=1)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Memory
        Catch2WithMain)

catch_discover_tests(${PROJECT_NAME})