#ifndef _DYNAMIC_MODEL_H
#define _DYNAMIC_MODEL_H

#include <Models/staticmodel.hpp>

class DynamicModel : public StaticModel {
public:
    DynamicModel(const Model3D* const model3D);
    ~DynamicModel();

    virtual void render(const Display* const display) const override;
    glm::mat4 generateMVP(const Display* const display) const override;

    void translate(const glm::vec3& trans);
    void rotate(const float& degrees, const glm::vec3& rotate);
    void scale(const glm::vec3& scale);

    void setVisible(const bool visible);

    bool isVisible() const;
    glm::mat4 getWorld() const;

private:
    bool m_visible;
    glm::mat4 m_translate;
    glm::mat4 m_scale;
    glm::mat4 m_rotate;
};

#endif // _DYNAMIC_MODEL_H
