#include <OpenGL/openGL.h>

int main(int argc, char **args)
{
    const char* title = "OpenGL Project";
    int width = 1024;
    int height = 576;

    // Create GLUT window
    glutInit(&argc, args);

    //#ifdef VERSION_MIN
    glutInitContextVersion(3,3);
    glutInitContextProfile( GLUT_CORE_PROFILE);
    //#endif

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title);

    // Create GLUT callbacks
    glutDisplayFunc([]() { OpenGL::GetInstance()->DisplayFunc(); });
    glutIdleFunc([]() { OpenGL::GetInstance()->DisplayFunc(); });

    // Create singleton instance of OpenGL
    if(!OpenGL::CreateInstance(width, height, title))
    {
        std::cout << "Couldn't Create Instance of OpenGL" << std::endl;
        return -1;
    }

    // Create singleton instance of RenderChain (Capability of 10 objects)
    if(!RenderChain::CreateInstance(10, true))
    {
        std::cout << "Couldn't Create Instance of RenderChain" << std::endl;
        return -1;
    }

    // Enter GLUT main loop
    glutMainLoop();

    // Destroy singletons
    OpenGL::DeleteInstance();
    RenderChain::DeleteInstance();
    return 0;
}

#pragma clang diagnostic pop