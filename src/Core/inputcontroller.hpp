#ifndef _INPUT_H
#define _INPUT_H

#include <glm/glm.hpp>
#include <unordered_set>

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

    bool IsKeyPressed(InputKey key) const;
    glm::vec2 GetMouseLocation() const;
    void UpdateKey(int key, bool pressed);

private:
    std::unordered_set<InputKey> m_pressedKeys;
};

#endif // _INPUT_H
