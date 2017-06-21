cmake_minimum_required(VERSION 3.5)
project(Opal)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra -pedantic -std=c++1z")
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DURECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

# Applications source files
set(SOURCE_FILES
    # Root
    src/main.cc

    # OpenGL
    src/GL/gl3w.c

    # Render
    src/Render/light.cc
    src/Render/shader.cc
    src/Render/sampler.cc

    # Components
    src/Components/components.cc

    # Scene
    src/Scene/scene.cc

    # Resources
    src/Resources/texture.cc
    src/Resources/resourcehandler.cc
    src/Resources/model3d.cc

    # Utilities
    src/Utilities/exceptions.cc
    src/Utilities/utilities.cc
    src/Utilities/log.cc

    # Core
    src/Core/glcore.cc
    src/Core/display.cc
	src/Core/inputcontroller.cc
    src/Core/camera.cc

)

add_executable(Opal ${SOURCE_FILES})
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(ASSIMP REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${OPENGL_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} ${GLFW_INCLUDE_DIRECTORIES} "src/")
target_link_libraries(Opal -ldl -llua /usr/lib/libfreeimage.so ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${ASSIMP_LIBRARIES})
