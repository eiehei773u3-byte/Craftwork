#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <string>

class Root {
public:
    Root(const std::string& title, int width, int height);
    ~Root();

    bool isClosed() const { return m_Closed; }
    void pollEvents();
    void clear(float r, float g, float b, float a);
    void swapBuffers();

private:
    std::string m_Title;
    int m_Width;
    int m_Height;
    bool m_Closed = false;

    SDL_Window* m_Window = nullptr;
    SDL_GLContext m_Context = nullptr;

    bool init();
};
