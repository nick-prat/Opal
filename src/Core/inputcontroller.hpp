#ifndef _INPUT_H
#define _INPUT_H

#include <glm/glm.hpp>
#include <functional>
#include <unordered_set>
#include <unordered_map>

enum class InputKey : int {
    A = 65,
    S = 83,
    D = 68,
    W = 87,
    E = 69,
    Q = 81,
    LSHIFT = 340,
    SPACE = 32
};

class InputController {
public:
    InputController();
    ~InputController();

    void ClearWhileKeyPressed();
    void DeregisterWhileKeyPressed(InputKey key);
    void RegisterWhileKeyPressed(InputKey key, const std::function<void(void)>& lambda);
    void CallKeyLambdas();

    bool IsKeyPressed(InputKey key) const;
    glm::vec2 GetMouseLocation() const;
    void UpdateKey(int key, bool pressed);

private:
    std::unordered_set<InputKey> m_pressedKeys;
    std::unordered_map<InputKey, std::function<void(void)>> m_keyLambdas;
};

#endif // _INPUT_H
