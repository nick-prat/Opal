#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <GL/freeglut.h>
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum InputKey
{
    Key_W = 119,
    Key_A = 97,
    Key_S = 115,
    Key_D = 100,
    Key_Space = 32
};

class GlutDisplay
{

public:
    GlutDisplay();
    GlutDisplay(int width, int height, std::string title);
    ~GlutDisplay();

    class InputModule
    {
    public:

        InputModule();
        ~InputModule();

        bool IsKeyPressed(InputKey key) const;
        glm::vec2 GetMouseLocation() const;
        void UpdateKey(int key, bool pressed);

    private:
        bool m_keys[256];
    };

    class CameraModule
    {
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

    bool InitDisplay(int width, int height, std::string title);
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
