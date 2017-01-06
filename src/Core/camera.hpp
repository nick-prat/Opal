#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera();
    ~Camera();

    bool initCamera();
    void destroy();

    glm::mat4 getViewMatrix() const;
    void moveCamera(glm::vec3 delta);
    void rotateCamera(glm::vec3 rotation);

    void setPosition(glm::vec3 position);

private:
    glm::mat4 m_viewMatrix;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;
};

#endif // _CAMERA_H
