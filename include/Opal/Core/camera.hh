#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/glm.hpp>

namespace Opal {

    class Camera {
    public:
        Camera();
        ~Camera();

        void update(float scale);

        glm::mat4 getViewMatrix() const;
        void moveCamera(glm::vec3 delta);
        void rotateCamera(glm::vec3 rotation);

        void setPosition(glm::vec3 position);
        glm::vec3 getPosition() const;
        glm::vec3 getDirection() const;
        glm::vec3 getDirection2D() const;
        glm::vec3 getUpDirection() const;

    private:
        glm::mat4 m_viewMatrix;
        glm::vec3 m_position;
        glm::vec3 m_direction;
        glm::vec3 m_rotation;
        glm::vec3 m_up;
    };

}

#endif // _CAMERA_H
