#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <memory>
#include <glm/glm.hpp>

class Camera;
class InputController;

class Display {
public:
    Display();
    Display(uint width, uint height);
    ~Display();

    InputController* getInputController() const;
    Camera* getCamera() const;
    glm::mat4 getProjectionMatrix() const;
    uint getWidth() const;
    uint getHeight() const;

    void passiveMotionFunc(int x, int y);

    void setCursorPosition(float x, float y) const;
    void setCursorVisible(bool visible) const;

private:
    uint m_width, m_height;
    std::unique_ptr<InputController> m_inputController;
    std::unique_ptr<Camera> m_camera;
    glm::mat4 m_projMatrix;
};

#endif // _DISPLAY_H
