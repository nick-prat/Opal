#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Core/gl.hh>

#include <memory>
#include <glm/glm.hpp>

class Camera;
class InputController;

class Display {
public:
    Display();
    Display(unsigned int width, unsigned int height, unsigned int major, unsigned int minor, const std::string& title);
    Display(const Display&) = delete;
    Display(Display&& display);
    ~Display();

    Display& operator=(const Display&) = delete;
    Display& operator=(Display&& display);

    void resize();
    void update();

    InputController* getInputController() const;
    Camera* getCamera() const;
    glm::mat4 getProjectionMatrix() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    bool windowShouldClose() const;

    void setMouseCapture(bool capture);
    void setClearColor(const glm::vec4& color);
    void setVsync(bool enabled);
    void setCursorPosition(float x, float y);
    void setCursorVisible(bool visible);
    void setWireFrame(bool wireframe);

private:
    GLFWwindow* m_window;
    unsigned int m_width, m_height;
    std::unique_ptr<InputController> m_inputController;
    std::unique_ptr<Camera> m_camera;
    glm::mat4 m_projMatrix;
};

#endif // _DISPLAY_H
