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

    // Passage des coordonnées normalisés d'OpenGL aux coordonées de la fenetre (0,0) est maintenant en haut à gauche
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h, 0);

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
                        /* Création d'une nouvelle entité au clic. Transformation des coordonées de SDL en coordonées normalisées pour OpenGL */
                        Entity* myNewEntity = new Entity((float) e.button.x, (float) e.button.y);
                        entities.push_back(myNewEntity);
                    }
                    break;

                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    if (entities.size() > 0) {
                        /* Delete the last entity on the list */
                        Entity* myEntityToDelete = entities.at(entities.size() - 1);
                        delete myEntityToDelete;
                        entities.erase(entities.end() - 1);
                    }
                        
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
