#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera();
    ~Camera();

    bool InitCamera();
    void Destroy();

    glm::mat4 GetViewMatrix();
    void MoveCamera(glm::vec3 delta);
    void RotateCamera(glm::vec3 rotation);

    protected:

private:
    glm::mat4 m_viewMatrix;

    glm::vec3 m_translation;
    glm::vec3 m_rotation;
};

#endif // _CAMERA_H
