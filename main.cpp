#include <OpenGL/OpenGL.h>

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
    glutDisplayFunc([]() { OpenGL::getInstance()->DisplayFunc(); });
    glutIdleFunc([]() { OpenGL::getInstance()->DisplayFunc(); });

    // Create instance of game
    if(!OpenGL::CreateInstance(width, height, title))
    {
        return -1;
    }

    // Enter GLUT main loop
    glutMainLoop();

    OpenGL::DestroyInstance();

    return 0;
}

#pragma clang diagnostic pop