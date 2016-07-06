//
// Created by nprat on 7/4/16.
//

#ifndef OPENGL_SHITTYOBJECT_H
#define OPENGL_SHITTYOBJECT_H

#include <Utilities/utilities.hpp>

#include "renderobject.h"

class ShittyObject : public RenderObject
{
public:
    ShittyObject();
    ShittyObject(std::shared_ptr<GlutDisplay> display);
    ~ShittyObject();

    bool InitObject(std::shared_ptr<GlutDisplay> display);
    void Destroy();

    void Render();

private:
    GLuint m_VBO;
    GLuint m_IBO;
    std::vector<glm::vec3> m_verts;
    std::vector<uint> m_indices;
    std::unique_ptr<Shader> m_shader;
};


#endif //OPENGL_SHITTYOBJECT_H
