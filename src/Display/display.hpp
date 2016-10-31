#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <GL/freeglut.h>
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GlutDisplay {
public:
    GlutDisplay();
    GlutDisplay(int width, int height);
    ~GlutDisplay();

    class InputModule {
    public:

        InputModule();
        ~InputModule();

        bool IsKeyPressed(unsigned char key) const;
        glm::vec2 GetMouseLocation() const;
        void UpdateKey(unsigned char key, bool pressed);

    private:
        bool m_keys[256];
    };

    class CameraModule {
    public:

        CameraModule();
        ~CameraModule();

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

    bool InitDisplay(int width, int height);
    void Destroy();

    std::shared_ptr<InputModule> GetInputModule();
    std::shared_ptr<CameraModule> GetCameraModule();

    glm::mat4 GetProjectionMatrix();

private:
    std::shared_ptr<InputModule> m_inputModule;
    std::shared_ptr<CameraModule> m_cameraModule;
    glm::mat4 m_projMatrix;
};

#endif // _DISPLAY_H
