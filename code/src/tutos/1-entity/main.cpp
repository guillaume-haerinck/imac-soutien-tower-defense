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
    unsigned int loopCount = 0;
    bool prepareWave = false;
    while (loop) {
        loopCount++;
        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        /* Update des entités */
        for (Entity* entity : entities) {
            entity->update();
        }

        /* Création de 2 entités toutes les 3 secondes */
        if (loopCount == 60 * 1 && prepareWave) {
            printf("1 seconde...\n");
        } else if (loopCount == 60 * 2 && prepareWave) {
            printf("2 secondes ...\n");
        } else if (loopCount == 60 * 3 && prepareWave) {
            printf("3 secondes ecoulees\n");
            Entity* myNewEntity = new Entity(50., 50.);
            Entity* myNewEntity2 = new Entity(60., 60.);
            entities.push_back(myNewEntity);
            entities.push_back(myNewEntity2);
            loopCount = 0;
            prepareWave = false;
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
                    if (entities.size() > 0 && e.key.keysym.sym == 'd') {
                        /* Delete the last entity on the list */
                        Entity* myEntityToDelete = entities.at(entities.size() - 1);
                        delete myEntityToDelete;
                        entities.erase(entities.end() - 1);
                    }

                    if (e.key.keysym.sym == 'w') {
                        printf("New wave in 3 seconds\n");
                        prepareWave = true;
                        loopCount = 0;
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
