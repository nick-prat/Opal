#ifndef _STATIC_MODEL_H
#define _STATIC_MODEL_H

#include <memory>

#include <Display/display.h>
#include <Model/Assimp/assimpmodel.h>
#include <Model/renderobject.h>

class StaticModel : public IRenderObject
{
public:
    StaticModel(std::shared_ptr<GlutDisplay> display, std::shared_ptr<AssimpModel> model);
    ~StaticModel();

    void Render();

private:
    std::vector<std::vector<GLuint>> m_VBO;
    std::vector<GLuint> m_VAO, m_IBO;
    std::shared_ptr<GlutDisplay> m_display;
    std::shared_ptr<AssimpModel> m_model;
};

#endif // _STATIC_MODEL_H
