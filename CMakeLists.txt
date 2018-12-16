cmake_minimum_required (VERSION 3.6)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(OpenGL)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

include_directories(/usr/local/Cellar/eigen/3.3.5/include/eigen3)

set(CMAKE_CXX_FLAGS "-g -Wall")

add_library(lib objloader.cpp motion.cpp motion.h mtlparser.cpp mtlparser.h tgaparser.h tgaparser.cpp antimotion.cpp antimotion.h android.cpp android.h camera.cpp camera.h)


link_libraries(lib)


add_executable(OpenGL main.cpp)
target_link_libraries(OpenGL ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
