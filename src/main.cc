#include <GL/gl3w.h>

#include <iostream>
#include <memory>

#include <Utilities/exceptions.hh>
#include <Utilities/log.hh>
#include <Core/glcore.hh>
#include <Scene/scene.hh>

// TODO Ensure const correctness is follwed in whole project
// TODO Implement some sort of collsion detection (bullet physics library?)

int main(int argc, char* argv[]) {
    if(argc != 2) {
        Log::getErrorLog() << "Arguments are invalid, use:\n\t" << argv[0] << " [scene_name]\n";
        return -1;
    }

    const std::string scenename = argv[1];
    const std::string title = "OpenGL Project";
    constexpr int width = 1024;
    constexpr int height = 576;

    GLCore::initAPI();

    GLCore glCore;
    try {
        glCore = GLCore(width, height, title);
    } catch(GenericException& error) {
        error.printError();
        return 1;
    }

    glCore.setClearColor(glm::vec4(0.0f));
    glCore.setVsync(true);

    glCore.loadScene(scenename);
    glCore.start();

    glCore.destroy();
    //GLCore::closeAPI();
    return 0;
}
