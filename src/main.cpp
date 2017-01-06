#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>
#include <Core/glcore.hpp>

// TODO Ensure const correctness is follwed in whole project
// TODO Implement some sort of collsion detection (bullet physics library?)

int main(int argc, char **args) {
    if(argc != 2) {
        Log::getErrorLog() << "Arguments are invalid, use:\n\t" << args[0] << " [scene_name]\n";
        exit(-1);
    }

    std::string scene = args[1];
    const char* title = "OpenGL Project";
    const int width = 1024;
    const int height = 576;
    const int major = 3;
    const int minor = 3;

    glfwSetErrorCallback([] (int error, const char* desc) {
        Log::getErrorLog() << "ERROR: " << "(" << error << ")" << " " << desc << '\n';
    });

    if(!glfwInit()) {
        Log::error("Couldn't initialize GLFW3\n");
        exit(-1);
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!window) {
        Log::error("Couldn't create window\n");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    if(gl3wInit() == -1) {
        Log::error("Couldn't initialize GL3W\n");
        glfwTerminate();
        exit(-1);
    }

    if(!gl3wIsSupported(major, minor)) {
        Log::getErrorLog() << "Open GL " << major << "." << minor << " is unsupported\n";
        glfwTerminate();
        exit(-1);
    }

    GLCore* glCore;
    try {
        glCore = new GLCore(width, height, scene);
    } catch(GenericException& error) {
        error.printError();
        glfwTerminate();
        exit(-1);
    }

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
        GLCore* glCore = reinterpret_cast<GLCore*>(glfwGetWindowUserPointer(window));
        if(action == GLFW_PRESS) {
            glCore->inputFunc(key, true);
        } else if(action == GLFW_RELEASE) {
            glCore->inputFunc(key, false);
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int /*mods*/) {
        GLCore* glCore = reinterpret_cast<GLCore*>(glfwGetWindowUserPointer(window));
        if(action == GLFW_PRESS) {
            glCore->inputFunc(button, true);
        } else if(action == GLFW_RELEASE) {
            glCore->inputFunc(button, false);
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        GLCore* glCore = reinterpret_cast<GLCore*>(glfwGetWindowUserPointer(window));
        glCore->mouseFunc(xpos, ypos);
    });

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, glCore);
    glfwSwapInterval(1);
    double timer = glfwGetTime();
    unsigned long frames = 0;

    while(!glfwWindowShouldClose(window)) {
        glCore->displayFunc();
        glfwSwapBuffers(window);
        glfwPollEvents();
        frames++;
    }

    timer = glfwGetTime() - timer;
    Log::getLog() << "Average FPS: " << frames / timer << '\n';

    delete glCore;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
