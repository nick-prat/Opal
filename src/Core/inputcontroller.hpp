#ifndef _INPUT_H
#define _INPUT_H

#include <glm/glm.hpp>

class InputController {
public:
    InputController();
    ~InputController();

    bool IsKeyPressed(unsigned char key) const;
    glm::vec2 GetMouseLocation() const;
    void UpdateKey(unsigned char key, bool pressed);

private:
    bool m_keys[256];
};

#endif // _INPUT_H
