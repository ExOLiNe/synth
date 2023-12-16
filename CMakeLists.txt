cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME Synth_root)
set(CMAKE_CXX_STANDARD 20)

project(${PROJECT_NAME} VERSION 0.0.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(JUCE_BUILD_EXTRAS ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (MSVC)
        message(STATUS "Clang MSVC")
        add_compile_options(/EHsc -Werror)
    else()
        add_compile_options(-v)
        message(STATUS "Clang but not MSVC")
    endif()
else()
    add_compile_options(/W4 /WX)
endif()

set(DCPM_JUCE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../JUCE)

find_package(JUCE REQUIRED)

add_subdirectory(tracy)
add_subdirectory(MidiGenerator)
add_subdirectory(Synth)