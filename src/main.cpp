#include <Core/openGL.hpp>

int main(int argc, char **args)
{
    const char* title = "OpenGL Project";
    int width = 1024;
    int height = 576;

    // Create GLUT window
    glutInit(&argc, args);
    glutInitContextVersion(3,3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title);

    // Create GLUT callbacks
    glutDisplayFunc([]() -> void {
        OpenGL::GetInstance()->DisplayFunc();
    });

    glutIdleFunc([]() -> void {
        OpenGL::GetInstance()->DisplayFunc();
    });

    glutKeyboardFunc([](unsigned char key, int x, int y) -> void {
        OpenGL::GetInstance()->KeyboardFunc(key, true, x, y);
    });

    glutKeyboardUpFunc([](unsigned char key, int x, int y) -> void {
        OpenGL::GetInstance()->KeyboardFunc(key, false, x, y);
    });

    // Create singleton instance of OpenGL
    if(!OpenGL::CreateInstance(width, height))
    {
        std::cout << "Couldn't Create Instance of OpenGL" << std::endl;
        return -1;
    }

    // Enter GLUT main loop
    glutMainLoop();

    // Destroy singletons
    OpenGL::DeleteInstance();
    RenderChain::DeleteInstance();
    return 0;
}
