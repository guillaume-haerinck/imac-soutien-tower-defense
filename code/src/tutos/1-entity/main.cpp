#ifdef _WIN32
    #include <windows.h>
#endif
#define _USE_MATH_DEFINES
#include <cmath>
#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>

#include "core/gl-log-handler.hpp"
#include "core/init.hpp"
#include "entity.hpp"

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

int main(int argc, char **argv) {
    SDL_Window* window = imac::init();
    if (window == nullptr) {
        spdlog::critical("[INIT] Init not achieved !");
        debug_break();
    }

    /* Creation d'une entité au runtime */
    Entity* myEntity1 = new Entity(0.0f, 0.0f);

    /* Tableau dynamique des entités vivantes du jeu */
    std::vector<Entity*> entities;
    entities.push_back(myEntity1);

    bool loop = true;
    while (loop) {
        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        /* Update des entités */
        for (Entity* entity : entities) {
            entity->update();
        }

        SDL_GL_SwapWindow(window);
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                loop = false;
                break;
            }

            switch (e.type) {
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    {
                        /* Création d'une nouvelle entité au clic */
                        Entity* myNewEntity = new Entity(0.5f, 0.5f);
                        entities.push_back(myNewEntity);
                    }
                    break;

                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    break;

                default:
                    break;
            }
        }

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Cleanup */
    SDL_DestroyWindow(window);
    SDL_Quit();
    for (Entity* entity : entities) {
        delete entity;
    }

    return EXIT_SUCCESS;
}
