cmake_minimum_required(VERSION 3.5)
project(OpenGL)
add_definitions(-DVERSION_MIN)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories("./src")
# Applications source files
set(SOURCE_FILES
        src/main.cpp
        src/Display/display.cpp src/Display/inputmodule.cpp src/Display/cameramodule.cpp
        src/Model/Shader/shader.cpp src/Model/Sprite/sprite.cpp src/Model/testObject.cpp src/Model/renderchain.cpp src/Model/renderobject.cpp src/Model/Assimp/assimploader.cpp
        src/Utilities/utilities.hpp
        src/Core/openGL.cpp
        src/Core/glapi.h src/Core/glapi.cpp )
add_executable(OpenGL ${SOURCE_FILES})
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(ASSIMP REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIR})
target_link_libraries(OpenGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${ASSIMP_LIBRARIES})
