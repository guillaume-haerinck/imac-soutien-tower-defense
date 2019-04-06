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

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

int main(int argc, char **argv)
{
    /* Init SDL, OpenGL, Debug and Glad */
    SDL_Window* window = imac::init();
    if (window == nullptr)
    {
        spdlog::critical("[INIT] Init not achieved !");
        debug_break();
    }

    /* Boucle d'affichage */
    bool loop = true;
    while (loop)
    {
        /* Recuperation du temps au debut de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /* Vide l'image */
        glClear(GL_COLOR_BUFFER_BIT);

        /* ------------- DESSINER ICI ---------------- */

        /* Echange du front et du back buffer : mise a jour de la fenetre */
        SDL_GL_SwapWindow(window);

        /* Boucle traitant les evenements */
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            /* L'utilisateur ferme la fenetre : */
            if (e.type == SDL_QUIT)
            {
                loop = false;
                break;
            }

            /* Quelques exemples de traitement d'evenements : */
            switch (e.type)
            {
                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    break;

                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    break;

                default:
                    break;
            }
        }

        /* Calcul du temps ecoule */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* Si trop peu de temps s'est ecoule, on met en pause le programme */
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associees a la SDL */
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
