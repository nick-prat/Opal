#ifndef _STATIC_MODEL_H
#define _STATIC_MODEL_H

#include <vector>
#include <glm/glm.hpp>

#include <Render/sampler.hpp>
#include <Render/renderobject.hpp>

class Model3D;
class Display;

class StaticModel : public IRenderObject {
public:
    StaticModel() = delete;
    StaticModel(const Model3D& model, const glm::mat4& world = glm::mat4(1.0f));
    StaticModel(const StaticModel& model);
    StaticModel(StaticModel&& model);
    virtual ~StaticModel();

    StaticModel& operator=(const StaticModel&) = delete;
    StaticModel& operator=(StaticModel&& model) = delete;

    void destroy();

    virtual glm::mat4 generateMVP(const Display& display) const;

    const Model3D& getModel() const;

protected:
    void generateBuffers();
    virtual void render(const Shader& shader, const Display& display) const override;

protected:
    const Model3D& m_model;

private:
    const glm::mat4 m_world;
    const unsigned int m_meshCount;

    Sampler m_sampler;
    std::vector<GLsizei> m_indexCount;
    std::vector<GLuint> m_VAO, m_VBO, m_IBO;
};

#endif // _STATIC_MODEL_H
