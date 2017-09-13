#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/glm.hpp>

namespace Opal {

    class Camera {
    public:
        Camera();
        ~Camera();

        void update(const float scale);

        void moveCamera(const glm::vec3 delta);
        void rotateCamera(const glm::vec3 rotation);
        void setPosition(const glm::vec3 position);
        void setRotationClamp(const glm::vec2 clamp);

        glm::mat4 getViewMatrix() const;
        glm::vec3 getPosition() const;
        glm::vec3 getDirection() const;
        glm::vec3 getDirection2D() const;
        glm::vec3 getUpDirection() const;

    private:
        glm::vec3 m_position;
        glm::vec3 m_direction;
        glm::vec3 m_rotation;
        glm::vec3 m_up;
        glm::vec2 m_rotationClamp;
        glm::mat4 m_viewMatrix;
    };

}

#endif // _CAMERA_H
