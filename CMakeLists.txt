cmake_minimum_required(VERSION 3.5)

project(opal)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra -pedantic -std=c++1z -fcolor-diagnostics")
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DURECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

# Applications source files
set(SOURCE_FILES
    # OpenGL
    src/GL/gl3w.c

    # Render
    src/Render/light.cc
    src/Render/shader.cc
    src/Render/sampler.cc

    # Systems
    src/Systems/movementsystem.cc
    src/Systems/boundingboxrendersystem.cc
    src/Systems/modelrendersystem.cc
    src/Systems/normalrendersystem.cc

    # Components
    src/Components/components.cc

    # Scene
    src/Scene/scene.cc

    # Resources
    src/Resources/scenehandler.cc
    src/Resources/assetstore.cc
    src/Resources/model3d.cc
    src/Resources/resources.cc
    src/Resources/texture.cc

    # Utilities
    src/Util/exceptions.cc
    src/Util/util.cc
    src/Util/log.cc

    # Core
    src/Core/glcore.cc
    src/Core/display.cc
    src/Core/camera.cc

)

add_library(opal SHARED ${SOURCE_FILES})
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(Lua51 REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${OPENGL_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} ${GLFW_INCLUDE_DIRECTORIES} "./include/")
target_link_libraries(opal -ldl -llua5.1 -lpthread /usr/lib/x86_64-linux-gnu/libfreeimage.so ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${ASSIMP_LIBRARIES})
