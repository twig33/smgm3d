# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.7)

# Project's name

project(smgm3d)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_SOURCE_DIR}/lib")

file(GLOB core_sources ${PROJECT_SOURCE_DIR}/src/core/*.cpp)
file(GLOB main_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB thirdparty_sources ${PROJECT_SOURCE_DIR}/src/third-party/*.c)

file(GLOB resources ${PROJECT_SOURCE_DIR}/res/*)

set(sources ${thirdparty_sources} ${core_sources} ${main_sources})

set (CMAKE_CXX_STANDARD 11)
add_executable(smgm3d ${sources})
#set_target_properties(smgm3d PROPERTIES COMPILE_OPTIONS "-m32" LINK_OPTIONS "-m32")

target_link_libraries(smgm3d glfw GL dl)

file(COPY ${resources} DESTINATION ${PROJECT_BINARY_DIR})

