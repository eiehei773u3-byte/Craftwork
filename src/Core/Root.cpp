#include "../Classes/Root.hpp"
#include <iostream>

Root::Root(const std::string& title, int width, int height)
    : m_Title(title), m_Width(width), m_Height(height) 
{
    if (!init()) {
        m_Closed = true;
    }
}

Root::~Root() {
    if (m_Context) SDL_GL_DeleteContext(m_Context);
    if (m_Window) SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

bool Root::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to init SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_Window = SDL_CreateWindow(
        m_Title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_Width,
        m_Height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!m_Window) {
        std::cerr << "Failed to create SDL Window: " << SDL_GetError() << std::endl;
        return false;
    }

    m_Context = SDL_GL_CreateContext(m_Window);
    if (!m_Context) {
        std::cerr << "Failed to create OpenGL Context: " << SDL_GetError() << std::endl;
        return false;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return false;
    }

    SDL_GL_SetSwapInterval(1);
    return true;
}

void Root::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_Closed = true;
        }
    }
}

void Root::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Root::swapBuffers() {
    SDL_GL_SwapWindow(m_Window);
}
