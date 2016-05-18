#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <string>
#include <SDL2/SDL.h>

class Display
{

public:
    Display();
    ~Display();

    bool InitDisplay(int width, int height, std::string title);
    void Destroy();

    void Update();
    bool IsClosed();

public:
    SDL_Window* m_window;
    SDL_GLContext m_glcontext;
    bool m_isClosed;
};

#endif // _DISPLAY_H
