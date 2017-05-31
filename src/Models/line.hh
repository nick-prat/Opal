#ifndef _LINE_H
#define _LINE_H

#include <glm/glm.hpp>
#include <GL/gl3w.h>

#include <Render/renderobject.hh>

class Display;

// NOTE Are lines actually very useful?

class Line : public IRenderObject {
public:
    Line(glm::vec3 tail, glm::vec3 head, glm::vec3 color);
    ~Line();

protected:
    void render(const Shader& shader, const Display& display) const override;

private:
    unsigned int m_indexCount;
    GLuint m_VAO, m_VBO, m_IBO;
    glm::vec3 m_tail;
    glm::vec3 m_head;
    glm::vec3 m_color;
};

#endif
