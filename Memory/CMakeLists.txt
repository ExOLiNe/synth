project(Memory VERSION 1.0.0)

add_library(${PROJECT_NAME} STATIC Source/empty.cpp)
target_include_directories(${PROJECT_NAME} INTERFACE Source)

add_subdirectory(Tests)