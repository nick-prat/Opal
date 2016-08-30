#ifndef _STATIC_MODEL_H
#define _STATIC_MODEL_H

#include <memory>

#include <Display/display.h>
#include <Model/Shader/shader.h>
#include <Model/Assimp/assimpmodel.h>
#include <Model/renderobject.h>

class StaticModel : public IRenderObject
{
public:
    StaticModel(std::shared_ptr<GlutDisplay> display, std::shared_ptr<AssimpModel> model);
    ~StaticModel();

    void Render();

private:
    uint m_meshCount;
    std::vector<GLsizei> m_indexCount;
    std::vector<std::vector<GLuint>> m_VBO;
    std::vector<GLuint> m_VAO, m_IBO;
    const std::shared_ptr<GlutDisplay> m_display;
    const std::shared_ptr<AssimpModel> m_model;
    std::shared_ptr<Shader> m_shader;
};

#endif // _STATIC_MODEL_H
