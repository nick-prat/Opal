cmake_minimum_required(VERSION 3.5)
project(OpenGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra -pedantic -std=c++1z")
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DURECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

# Applications source files
set(SOURCE_FILES
    # Root
    src/main.cpp

    # OpenGL
    src/GL/gl3w.c

    # Render
    src/Render/light.cpp
    src/Render/renderobject.cpp
    src/Render/shader.cpp
    src/Render/sampler.cpp

    # ECS
    src/ECS/components.cpp

    # Scene
    src/Scene/scene.cpp

    # Models
    src/Models/dynamicmodel.cpp
    src/Models/staticmodel.cpp
	src/Models/line.cpp

    # Resources
    src/Resources/texture.cpp
    src/Resources/resourcehandler.cpp
    src/Resources/model3d.cpp

    # Utilities
    src/Utilities/exceptions.cpp
    src/Utilities/utilities.cpp
    src/Utilities/log.cpp

    # Core
    src/Core/glcore.cpp
    src/Core/display.cpp
	src/Core/inputcontroller.cpp
    src/Core/camera.cpp

)

add_executable(OpenGL ${SOURCE_FILES})
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(ASSIMP REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${OPENGL_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} ${GLFW_INCLUDE_DIRECTORIES} "src/")
target_link_libraries(OpenGL -ldl -llua /usr/lib/libfreeimage.so ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${ASSIMP_LIBRARIES})
