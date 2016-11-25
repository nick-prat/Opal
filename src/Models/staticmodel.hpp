#ifndef _STATIC_MODEL_H
#define _STATIC_MODEL_H

#include <Core/display.hpp>
#include <Resources/model3d.hpp>
#include <Render/renderobject.hpp>
#include <Render/Shader/shader.hpp>
#include <Render/Sampler/sampler.hpp>

class StaticModel : public IRenderObject
{
public:
    StaticModel(std::shared_ptr<Model3D> model);
    ~StaticModel();

    void Render(const Display* const display) override;

    std::shared_ptr<Model3D> GetModel();

private:
    Sampler m_sampler;
    uint m_meshCount;
    std::vector<GLsizei> m_indexCount;
    std::vector<GLuint> m_VAO, m_VBO, m_IBO;
    const std::shared_ptr<Model3D> m_model;
    std::shared_ptr<Shader> m_shader;
};

#endif // _STATIC_MODEL_H
