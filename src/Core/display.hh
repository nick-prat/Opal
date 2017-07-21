#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Core/gl.hh>

#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>

#include <Core/camera.hh>

enum class InputKey : int {
    A = 65,
    S = 83,
    D = 68,
    W = 87,
    E = 69,
    Q = 81,
    LSHIFT = 340,
    SPACE = 32,
    LCLICK = 0,
    RCLICK = 1,
    MCLICK = 2
};

class Display {
public:
    Display();
    Display(unsigned int width, unsigned int height, unsigned int major, unsigned int minor, const std::string &title);
    Display(const Display&) = delete;
    Display(Display &&display);
    ~Display();

    Display &operator=(const Display&) = delete;
    Display &operator=(Display &&display);

    void resize(int width, int height);
    void update();

    Camera &getCamera();
    const Camera &getCamera() const;
    glm::mat4 getProjectionMatrix() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    bool windowShouldClose() const;

    void setMouseCapture(bool capture);
    void setClearColor(const glm::vec4 &color);
    void setVsync(bool enabled);
    void setCursorPosition(const glm::vec2 &pos);
    void setCursorVisible(bool visible);
    void setWireFrame(bool wireframe);

    void clearWhileKeyPressed();
    void deregisterWhileKeyPressed(const InputKey key);
    void registerWhileKeyPressed(const InputKey key, const std::function<void(InputKey)> &lambda);

    void clearOnKeyPressed();
    void deregisterOnKeyPressed(const InputKey key);
    void registerOnKeyPressed(const InputKey key, const std::function<void(InputKey)> &lambda);

    void centerCursor();
    void bindCursorUpdate(std::function<void(int, int)> func);
    void callKeyLambdas();
    const std::pair<int, int> &getCursorPosition() const;
    bool isKeyPressed(const InputKey key) const;

private:
    void updateKey(const int key, const bool pressed);
    void updateCursorPosition(const int xpos, const int ypos);

private:
    GLFWwindow* m_window;
    Camera m_camera;
    unsigned int m_width, m_height;
    double m_prevTime, m_timeScale;
    glm::mat4 m_projMatrix;
    std::unordered_map<InputKey, bool> m_pressedKeys;
    std::unordered_map<InputKey, std::function<void(InputKey)>> m_whileKeyPressed;
    std::unordered_map<InputKey, std::function<void(InputKey)>> m_onKeyPressed;
    std::function<void(int, int)> m_cursorFunc;
    std::pair<int, int> m_cursorPos;
};

#endif // _DISPLAY_H
