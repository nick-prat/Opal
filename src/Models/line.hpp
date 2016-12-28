#ifndef _LINE_H
#define _LINE_H

#include <glm/glm.hpp>
#include <memory>

#include <Core/display.hpp>
#include <Render/Shader/shader.hpp>
#include <Render/renderobject.hpp>

class Line : public IRenderObject {
public:
    Line(glm::vec3 tail, glm::vec3 head, glm::vec3 color);
    ~Line();

    void render(const Display* const display) const override;

private:
    uint m_indexCount;
    GLuint m_VAO, m_VBO, m_IBO;
    glm::vec3 m_tail;
    glm::vec3 m_head;
    glm::vec3 m_color;
};

#endif
