#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <memory>
#include <glm/glm.hpp>

class Camera;
class InputController;

class Display {
public:
    Display();
    Display(unsigned int width, unsigned int height);
    Display(const Display&) = delete;
    Display(Display&& display);
    ~Display();

    Display& operator=(const Display&) = delete;
    Display& operator=(Display&& display);

    InputController* getInputController() const;
    Camera* getCamera() const;
    glm::mat4 getProjectionMatrix() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void setCursorPosition(float x, float y) const;
    void setCursorVisible(bool visible) const;
    void setWireFrame(bool wireframe) const;

private:
    unsigned int m_width, m_height;
    std::unique_ptr<InputController> m_inputController;
    std::unique_ptr<Camera> m_camera;
    glm::mat4 m_projMatrix;
};

#endif // _DISPLAY_H
