cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(todoas VERSION 1.0.0 LANGUAGES C)
set(C_STANDARD 23)

add_executable(${PROJECT_NAME} src/main.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)

add_subdirectory(cmdio)
include_directories(${CMAKE_SOURCE_DIR}/cmdio)
target_link_libraries(${PROJECT_NAME} cmdio)

