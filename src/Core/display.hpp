#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <memory>
#include <glm/glm.hpp>

#include <Core/camera.hpp>
#include <Core/inputcontroller.hpp>

class GlutDisplay {
public:
    GlutDisplay();
    GlutDisplay(uint width, uint height);
    ~GlutDisplay();

    bool InitDisplay(uint width, uint height);
    void Destroy();

    std::shared_ptr<InputController> GetInputController() const;
    std::shared_ptr<Camera> GetCamera() const;
    glm::mat4 GetProjectionMatrix() const;
    uint GetWidth() const;
    uint GetHeight() const;

    void PassiveMotionFunc(int x, int y);

    void SetCursorPosition(float x, float y) const;
    void SetCursorVisible(bool visible) const;

private:
    uint m_width, m_height;
    std::shared_ptr<InputController> m_inputController;
    std::shared_ptr<Camera> m_camera;
    glm::mat4 m_projMatrix;
};

#endif // _DISPLAY_H
