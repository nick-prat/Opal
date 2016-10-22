cmake_minimum_required(VERSION 3.5)
project(OpenGL)
add_definitions(-DVERSION_MIN)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -std=c++14")
include_directories("./src")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DURECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

# Applications source files
set(SOURCE_FILES
        src/main.cpp
        src/Display/display.cpp 
		src/Display/Modules/inputmodule.cpp src/Display/Modules/cameramodule.cpp
	src/Model/Render/renderchain.cpp src/Model/Render/renderobject.cpp
        src/Model/Shader/shader.cpp 
	src/Model/Textures/texture.cpp
	src/Model/Sampler/sampler.cpp
		src/Model/Models/StaticModel/staticmodel.cpp  
		src/Model/Models/Sprite/sprite.cpp
	src/Assimp/assimploader.cpp src/Assimp/assimpmodel.cpp
        src/Utilities/utilities.cpp src/Utilities/log.cpp
        src/Core/openGL.cpp src/Core/glapi.cpp )
add_executable(OpenGL ${SOURCE_FILES})
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(ASSIMP REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIR})
target_link_libraries(OpenGL /usr/lib/libfreeimage.so ${OPENGL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${GLUT_LIBRARIES} ${ASSIMP_LIBRARIES})
