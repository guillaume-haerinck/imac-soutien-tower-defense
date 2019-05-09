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
#include "map.hpp"

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

int main(int argc, char **argv) {
    SDL_Window* window = imac::init();
    if (window == nullptr) {
        spdlog::critical("[INIT] Init not achieved !");
        debug_break();
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    // Entities
    std::vector<Entity*> entities;

    /* Map */
    Map level1;

    bool loop = true;
    while (loop) {
        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        level1.draw();
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
                    {
                        printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                        glm::vec2 gridPos = level1.windowToGrid((float) e.button.x, (float) e.button.y);
                        spdlog::info("Grid x: {} y: {}", gridPos.x, gridPos.y);
                        MapTile tile = level1.getTile(gridPos.x, gridPos.y);
                        if (tile == MapTile::constructible) {
                            spdlog::info("I can construct here");
                            glm::vec2 winPos = level1.gridToWindow(gridPos.x, gridPos.y);
                            Entity* myNewEntity = new Entity(winPos.x, winPos.y);
                            entities.push_back(myNewEntity);
                        } else {
                            spdlog::warn("can't construct here");
                        }
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
