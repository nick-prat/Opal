#ifndef _STATIC_MODEL_H
#define _STATIC_MODEL_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <GL/gl3w.h>

#include <Render/Sampler/sampler.hpp>
#include <Render/renderobject.hpp>

class Model3D;
class Display;

class StaticModel : public IRenderObject {
public:
    StaticModel() = delete;
    StaticModel(const Model3D* const model, const glm::mat4& world);
    StaticModel(const StaticModel&) = delete;
    StaticModel(StaticModel&& model);
    virtual ~StaticModel();

    StaticModel& operator=(const StaticModel&) = delete;
    StaticModel& operator=(StaticModel&& model) = delete;

    virtual glm::mat4 generateMVP(const Display* const display) const;

    const Model3D* getModel() const;

protected:
    virtual void render(const Shader* const shader, const Display* const display) const override;

protected:
    const Model3D* m_model;

private:
    const glm::mat4 m_world;
    Sampler m_sampler;
    std::vector<GLsizei> m_indexCount;
    std::vector<GLuint> m_VAO, m_VBO, m_IBO;

    const unsigned int m_meshCount;
};

#endif // _STATIC_MODEL_H
