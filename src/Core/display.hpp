#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <memory>
#include <glm/glm.hpp>

#include <Core/camera.hpp>
#include <Core/inputcontroller.hpp>

class GlutDisplay {
public:
    GlutDisplay();
    GlutDisplay(int width, int height);
    ~GlutDisplay();

    bool InitDisplay(int width, int height);
    void Destroy();

    std::shared_ptr<InputController> GetInputController();
    std::shared_ptr<Camera> GetCamera();

    glm::mat4 GetProjectionMatrix();

private:
    std::shared_ptr<InputController> m_inputController;
    std::shared_ptr<Camera> m_camera;
    glm::mat4 m_projMatrix;
};

#endif // _DISPLAY_H
