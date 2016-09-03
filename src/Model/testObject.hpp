#ifndef OPENGL_SHITTYOBJECT_H
#define OPENGL_SHITTYOBJECT_H

#include <Utilities/utilities.hpp>

#include "renderobject.hpp"

class TestObject : public IRenderObject
{
public:
    TestObject();
    TestObject(std::shared_ptr<GlutDisplay> display);
    ~TestObject();

    bool InitObject(std::shared_ptr<GlutDisplay> display);

    void Render();

private:
    GLuint m_VBO[2], m_VAO, m_IBO;
    std::vector<glm::vec3> m_colors;
    std::vector<glm::vec3> m_verts;
    std::vector<uint> m_indices;
    std::unique_ptr<Shader> m_shader;
};


#endif //OPENGL_SHITTYOBJECT_H
