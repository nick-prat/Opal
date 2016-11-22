#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <memory>
#include <glm/glm.hpp>

#include <Core/camera.hpp>
#include <Core/inputcontroller.hpp>

class Display {
public:
    Display();
    Display(uint width, uint height);
    ~Display();

    bool InitDisplay(uint width, uint height);
    void Destroy();

    std::shared_ptr<InputController> GetInputController() const;
    std::shared_ptr<Camera> GetCamera() const;
    glm::mat4 GetProjectionMatrix() const;
    uint GetWidth() const;
    uint GetHeight() const;

    void SetCursorPosition(float x, float y) const;
    void SetCursorVisible(bool visible) const;

    void SwapBuffers() const;

private:
    uint m_width, m_height;
    std::shared_ptr<InputController> m_inputController;
    std::shared_ptr<Camera> m_camera;
    glm::mat4 m_projMatrix;
};

#endif // _DISPLAY_H
