#pragma once

#ifdef _WIN32
    #include <windows.h>
#endif
#define _USE_MATH_DEFINES
#include <cmath>
#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "core/gl-log-handler.hpp"

/* Dimensions initiales et titre de la fenetre */
static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 600;

namespace imac {
SDL_Window* init()
{
    /* Init logger */
    spdlog::set_pattern("[%l] %^ %v %$");
    
    /* Init SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        spdlog::critical("[SDL2] Unable to initialize SDL: {}", SDL_GetError());
        debug_break();
        return nullptr;
    }

    /* Init OpenGL */
    SDL_GL_LoadLibrary(NULL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_GL_SetSwapInterval(1);

    /* Create Window */
    SDL_Window* window = SDL_CreateWindow(
        "IMAC Soutien tower defense",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        spdlog::critical("[SDL2] Window is null: {}", SDL_GetError());
        debug_break();
        return nullptr;
    }

    /* Create OpenGl context */
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr)
    {
        spdlog::critical("[SDL2] OpenGL context is null: {}", SDL_GetError());
        debug_break();
        return nullptr;
    }

    /* Init Glad once the OpenGL context is valid */
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        spdlog::critical("[Glad] Glad not init");
        debug_break();
        return nullptr;
    }

    /* Enable OpenGL debugging */
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gllog::messageCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    return window;
}
}
