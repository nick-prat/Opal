#ifndef _INPUT_H
#define _INPUT_H

#include <glm/glm.hpp>
#include <functional>
#include <mutex>
#include <unordered_map>

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

    void ClearWhileKeyPressed();
    void DeregisterWhileKeyPressed(const InputKey key);
    void RegisterWhileKeyPressed(const InputKey key, const std::function<void(void)>& lambda);

    void ClearOnKeyPressed();
    void DeregisterOnKeyPressed(const InputKey key);
    void RegisterOnKeyPressed(const InputKey key, const std::function<void(void)>& lambda);

    void CallKeyLambdas();

    void UpdateMousePosition(const double xpos, const double ypos);
    std::pair<double, double> GetMousePosition() const;

    bool IsKeyPressed(const InputKey key) const;
    glm::vec2 GetMouseLocation() const;
    void UpdateKey(const int key, const bool pressed);

private:
    std::unordered_map<InputKey, bool> m_pressedKeys;
    std::unordered_map<InputKey, std::function<void(void)>> m_whileKeyPressed;
    std::unordered_map<InputKey, std::function<void(void)>> m_onKeyPressed;
    double m_mouseX;
    double m_mouseY;
};

#endif // _INPUT_H
