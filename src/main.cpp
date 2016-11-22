#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Utilities/exceptions.hpp>
#include <Core/glcore.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    GLCore* glCore = reinterpret_cast<GLCore*>(glfwGetWindowUserPointer(window));
    if(glCore == nullptr) {
        std::cout << "glCore was null in key_callback" << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if(action == GLFW_PRESS) {
        glCore->KeyboardFunc(key, true);
    } else if(action == GLFW_RELEASE) {
        glCore->KeyboardFunc(key, false);
    }
}

void error_callback(int error, const char* desc) {
    std::cout << "(" << error << ")" << " " << desc << std::endl;
}

int main(int argc, char **args)
{
    if(argc != 2) {
        std::cout << "Arguments are invalid, use:\n\t" << args[0] << " [scene_name]" << std::endl;
        exit(-1);
    }

    std::string scene = args[1];
    const char* title = "OpenGL Project";
    const int width = 1024;
    const int height = 576;
    const int major = 3;
    const int minor = 3;

    glfwSetErrorCallback(error_callback);

    if(!glfwInit()) {
        std::cout << "Couldn't initialize GLFW3" << std::endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!window) {
        std::cout << "Couldn't create window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    if(gl3wInit() == -1) {
        std::cout << "Couldn't initialize GL3W" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    if(!gl3wIsSupported(major, minor)) {
        std::cout << "Open GL " << major << "." << minor << " is unsupported" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    GLCore* glCore;
    try {
        glCore = new GLCore(width, height, scene);
    } catch(generic_exception& error) {
        error.PrintError();
        glfwTerminate();
        exit(-1);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowUserPointer(window, glCore);

    while(!glfwWindowShouldClose(window)) {
        glCore->DisplayFunc();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
