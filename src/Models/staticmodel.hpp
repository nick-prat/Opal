#ifndef _STATIC_MODEL_H
#define _STATIC_MODEL_H

#include <Core/display.hpp>
#include <Resources/model3d.hpp>
#include <Render/renderobject.hpp>
#include <Render/Shader/shader.hpp>
#include <Render/Sampler/sampler.hpp>

class StaticModel : public IRenderObject {
public:
    StaticModel(const Model3D* const model);
    virtual ~StaticModel();

    virtual void render(const Display* const display) override;
    virtual glm::mat4 GenerateMVP(const Display* const display) const;

    const Model3D* GetModel() const;

protected:
    const Model3D* const m_model;

private:
    Sampler m_sampler;
    uint m_meshCount;
    std::vector<GLsizei> m_indexCount;
    std::vector<GLuint> m_VAO, m_VBO, m_IBO;
    std::shared_ptr<Shader> m_shader;
};

#endif // _STATIC_MODEL_H
