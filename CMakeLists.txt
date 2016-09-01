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
        src/Display/display.cpp src/Display/inputmodule.cpp src/Display/cameramodule.cpp
	src/Model/testObject.cpp src/Model/renderchain.cpp src/Model/renderobject.cpp
        	src/Model/Shader/shader.cpp 
		src/Model/Sprite/sprite.cpp
		src/Model/Models/staticmodel.cpp  
		src/Model/Assimp/assimploader.cpp src/Model/Assimp/assimpmodel.cpp
        src/Utilities/utilities.cpp src/Utilities/log.cpp
        src/Core/openGL.cpp src/Core/glapi.cpp )
add_executable(OpenGL ${SOURCE_FILES})
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(ASSIMP REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIR})
target_link_libraries(OpenGL ${OPENGL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${GLUT_LIBRARIES} ${ASSIMP_LIBRARIES})
