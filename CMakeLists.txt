cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME Synth_root)
set(CMAKE_CXX_STANDARD 20)

project(${PROJECT_NAME} VERSION 0.0.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(JUCE_BUILD_EXTRAS ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if (MSVC)
            message(STATUS "Clang MSVC")
            add_compile_options(/EHsc -Werror)
        else()
            message(STATUS "Clang but not MSVC")
            add_compile_options(-v)
        endif()
    else()
        message(STATUS "MSVC")
        add_compile_options(/W4 /WX)
    endif()
else()
    message(STATUS "Something else ${CMAKE_BUILD_TYPE}")
endif()

set(CPM_JUCE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../JUCE)

set(CPM_TRACY_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../Tracy)

find_package(JUCE REQUIRED)
find_package(Tracy REQUIRED)

#add_subdirectory(tracy)
add_subdirectory(MidiGenerator)
add_subdirectory(Synth)
add_subdirectory(Memory)