project(UnitTestRunner VERSION 1.0)

#set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
#include(CTest)

#find_package(catch2 REQUIRED)

juce_add_console_app(UnitTestRunner PRODUCT_NAME "Unit Test Runner")

target_sources(UnitTestRunner PRIVATE PolymorphicReadonlyArrayTest.cpp)

target_compile_definitions(UnitTestRunner PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_UNIT_TESTS=1)

target_link_libraries(UnitTestRunner PRIVATE
        juce_recommended_config_flags
        juce_recommended_lto_flags
        juce_recommended_warning_flags
        juce_core)

#catch_discover_tests(UnitTestRunner)