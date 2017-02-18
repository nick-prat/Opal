#include <GL/gl3w.h>

#include <iostream>
#include <memory>

#include <Utilities/exceptions.hpp>
#include <Utilities/log.hpp>
#include <Core/glcore.hpp>
#include <Scene/scene.hpp>
#include <Models/staticmodel.hpp>
#include <Models/dynamicmodel.hpp>

// TODO Ensure const correctness is follwed in whole project
// TODO Implement some sort of collsion detection (bullet physics library?)

int main(int argc, char **args) {
    if(argc != 2) {
        Log::getErrorLog() << "Arguments are invalid, use:\n\t" << args[0] << " [scene_name]\n";
        return -1;
    }

    const std::string scenename = args[1];
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

    Scene scene = glCore.createScene(scenename);

    auto& rh = scene.getResourceHandler();
    const auto* m3d = rh.getResource<Model3D>("m3d_bear");
    auto smodel = StaticModel(*m3d);

    auto dynmodel1 = DynamicModel(smodel);
    dynmodel1.scale(glm::vec3(0.1f, 0.1f, 0.1f));
    dynmodel1.translate(glm::vec3(1.5f, -2.1f, 0.0f));
    auto dynmodel2 = DynamicModel(std::move(smodel));
    dynmodel2.scale(glm::vec3(0.1f, 0.1f, 0.1f));
    dynmodel2.translate(glm::vec3(3.0f, -2.1f, 0.0f));

    auto sh = rh.getShader("shader_staticmodel");
    sh->attachRenderObject(&dynmodel1);
    sh->attachRenderObject(&dynmodel2);

    glCore.startScene(&scene);

    double timer = glfwGetTime();
    unsigned long frames = 0;

    while(!glCore.shouldClose()) {
        glCore.displayFunc();
        frames++;
    }

    timer = glfwGetTime() - timer;
    Log::getLog() << "Average FPS: " << frames / timer << '\n';

    scene.destroy();
    glCore.destroy();
    GLCore::closeAPI();
    return 0;
}
