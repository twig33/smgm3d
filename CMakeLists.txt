# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.7)

# Project's name

project(smgm3d)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(GLFW3 REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include" "${GLFW3_INCLUDE_DIR}")
link_directories("${PROJECT_SOURCE_DIR}/lib")

file(GLOB core_sources ${PROJECT_SOURCE_DIR}/src/core/*.cpp
     ${PROJECT_SOURCE_DIR}/src/core/graphics/*.cpp
     ${PROJECT_SOURCE_DIR}/src/core/util/*.cpp)
file(GLOB game_sources ${PROJECT_SOURCE_DIR}/src/game/*.cpp)
file(GLOB main_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB physics_sources ${PROJECT_SOURCE_DIR}/src/physics/*.cpp)
file(GLOB thirdparty_sources ${PROJECT_SOURCE_DIR}/src/third-party/*.c)

file(GLOB resources ${PROJECT_SOURCE_DIR}/res/*)

set(sources ${thirdparty_sources} ${core_sources} ${physics_sources} ${game_sources} ${main_sources})

set(libraries ${GLFW3_LIBRARY})
if (UNIX)
   set(libraries ${libraries} dl)
endif()

set (CMAKE_CXX_STANDARD 11)
add_executable(smgm3d ${sources})
#set_target_properties(smgm3d PROPERTIES COMPILE_OPTIONS "-m32" LINK_OPTIONS "-m32")

target_link_libraries(smgm3d ${libraries})

file(COPY ${resources} DESTINATION ${PROJECT_BINARY_DIR})

