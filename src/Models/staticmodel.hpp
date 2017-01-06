#ifndef _STATIC_MODEL_H
#define _STATIC_MODEL_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <GL/gl3w.h>

#include <Render/renderobject.hpp>

class Model3D;
class Display;
class Sampler;

class StaticModel : public IRenderObject {
public:
    StaticModel(const Model3D* const model, const glm::mat4& world);
    virtual ~StaticModel();

    virtual void render(const Display* const display) const override;
    virtual glm::mat4 generateMVP(const Display* const display) const;

    const Model3D* getModel() const;

protected:
    const Model3D* const m_model;

private:
    const glm::mat4 m_world;
    std::unique_ptr<Sampler> m_sampler;
    std::vector<GLsizei> m_indexCount;
    std::vector<GLuint> m_VAO, m_VBO, m_IBO;

    uint m_meshCount;
};

#endif // _STATIC_MODEL_H
