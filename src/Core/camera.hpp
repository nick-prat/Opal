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

    void SetPosition(glm::vec3 position);

    protected:

private:
    glm::mat4 m_viewMatrix;

    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;
};

#endif // _CAMERA_H
