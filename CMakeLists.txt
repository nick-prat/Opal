cmake_minimum_required(VERSION 3.5)
project(OpenGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories("./")
set(SOURCE_FILES main.cpp Model/renderchain.cpp Model/renderobject.cpp Display/display.cpp Display/inputmodule.cpp
        Model/Shader/shader.cpp Model/Sprite/sprite.cpp Utilities/utilities.hpp Display/cameramodule.cpp)
add_executable(OpenGL ${SOURCE_FILES})
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(OpenGL ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} "-L/usr/lib -lSDL2")