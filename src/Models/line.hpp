#ifndef _LINE_H
#define _LINE_H

#include <glm/glm.hpp>
#include <memory>

#include <Display/display.hpp>
#include <Render/renderobject.hpp>

class Line : public IRenderObject {
public:
    Line(std::shared_ptr<GlutDisplay> display, glm::vec3 tail, glm::vec3 head, glm::vec3 color);
    ~Line();

    void Render();

private:
    uint m_indexCount;
    GLuint m_VAO, m_VBO, m_IBO;
    glm::vec3 m_tail;
    glm::vec3 m_head;
    glm::vec3 m_color;
    std::shared_ptr<GlutDisplay> m_display;
};

#endif
