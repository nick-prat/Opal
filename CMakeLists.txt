cmake_minimum_required(VERSION 3.5)
project(OpenGL)
add_definitions(-DVERSION_MIN)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g  -Wall -pedantic -Wextra -std=c++14")
include_directories("./src")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DURECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

# Applications source files
set(SOURCE_FILES
    # Root
    src/main.cpp

    # Display/
    src/Display/display.cpp
	src/Display/Modules/inputmodule.cpp
    src/Display/Modules/cameramodule.cpp

    # Render
    src/Render/renderchain.cpp
    src/Render/renderobject.cpp
    src/Render/Shader/shader.cpp
    src/Render/Textures/texture.cpp
    src/Render/Sampler/sampler.cpp

    # Models
    src/Models/staticmodel.cpp
	src/Models/sprite.cpp
	src/Models/line.cpp

    # Resources
    src/Resources/resourceloader.cpp
    src/Resources/model3d.cpp

    # Utilities
    src/Utilities/utilities.cpp
    src/Utilities/log.cpp

    # Core
    src/Core/openGL.cpp
    src/Core/glapi.cpp )

add_executable(OpenGL ${SOURCE_FILES})
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(ASSIMP REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIR})
target_link_libraries(OpenGL /usr/lib/libfreeimage.so ${OPENGL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${GLUT_LIBRARIES} ${ASSIMP_LIBRARIES})
