#ifndef _INPUT_H
#define _INPUT_H

#include <glm/glm.hpp>
#include <functional>
#include <unordered_map>

// NOTE Shoudl i have different mouse input modes?

// TODO Implement all keys
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

class InputController {
public:
    InputController();
    ~InputController();

    void clearWhileKeyPressed();
    void deregisterWhileKeyPressed(const InputKey key);
    void registerWhileKeyPressed(const InputKey key, const std::function<void(InputKey)>& lambda);

    void clearOnKeyPressed();
    void deregisterOnKeyPressed(const InputKey key);
    void registerOnKeyPressed(const InputKey key, const std::function<void(InputKey)>& lambda);

    void callKeyLambdas();

    void updateMousePosition(const double xpos, const double ypos);
    std::pair<double, double> getMousePosition() const;

    bool isKeyPressed(const InputKey key) const;
    glm::vec2 getMouseLocation() const;
    void updateKey(const int key, const bool pressed);

private:
    std::unordered_map<InputKey, bool> m_pressedKeys;
    std::unordered_map<InputKey, std::function<void(InputKey)>> m_whileKeyPressed;
    std::unordered_map<InputKey, std::function<void(InputKey)>> m_onKeyPressed;
    double m_mouseX;
    double m_mouseY;
};

#endif // _INPUT_H
