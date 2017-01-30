#include <iostream>
#include <memory>

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

    glfwSetErrorCallback([] (int error, const char* desc) {
        Log::getErrorLog() << "ERROR: " << "(" << error << ")" << " " << desc << '\n';
    });

    std::unique_ptr<GLCore> glCore;
    try {
        glCore = std::unique_ptr<GLCore>(new GLCore(width, height, title));
    } catch(GenericException& error) {
        error.printError();
        glfwTerminate();
        exit(-1);
    }

    glCore->startScene(scene);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    auto window = glCore->getWindow();
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

    return 0;
}
