cmake_minimum_required(VERSION 3.3)
project(stones)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lglut -lGL")

set(SOURCE_FILES main.cpp Triangulation.cpp Triangulation.hpp Point.cpp Point.hpp DoubleComparations.hpp)
add_executable(stones ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(stones ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )