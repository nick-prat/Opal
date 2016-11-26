#ifndef _DYNAMIC_MODEL_H
#define _DYNAMIC_MODEL_H

#include <Models/staticmodel.hpp>

class DynamicModel : public StaticModel {
public:
    DynamicModel(Model3D* model3D);
    ~DynamicModel();

    void Translate(const glm::vec3& trans);
    void Rotate(const float& degrees, const glm::vec3& rotate);
    void Scale(const glm::vec3& scale);

    const Model3D* const GetModel();
    glm::mat4 GetWorld() const;

private:
    glm::mat4 m_translate;
    glm::mat4 m_scale;
    glm::mat4 m_rotate;
};

#endif // _DYNAMIC_MODEL_H
