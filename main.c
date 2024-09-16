#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include "main.h"

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win_2d = SDL_CreateWindow("2D Top-Down View",
                                          SDL_WINDOWPOS_CENTERED_DISPLAY(0) - WINDOW_WIDTH / 2 - 320,
                                          SDL_WINDOWPOS_CENTERED_DISPLAY(0),
                                          WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Window* win_3d = SDL_CreateWindow("3D Raycast View",
                                          SDL_WINDOWPOS_CENTERED_DISPLAY(0) + WINDOW_WIDTH / 2 + 320,
                                          SDL_WINDOWPOS_CENTERED_DISPLAY(0),
                                          WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!win_2d || !win_3d) {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* rend_2d = SDL_CreateRenderer(win_2d, -1, SDL_RENDERER_ACCELERATED);
    SDL_Renderer* rend_3d = SDL_CreateRenderer(win_3d, -1, SDL_RENDERER_ACCELERATED);

    float x_pos = WINDOW_WIDTH / 4;
    float y_pos = WINDOW_HEIGHT / 4;
    float sprite_angle = 0;

    int close_requested = 0;
    while (!close_requested) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                close_requested = 1;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                        x_pos += SPEED * cos(sprite_angle) / 60;
                        y_pos += SPEED * sin(sprite_angle) / 60;
                        break;
                    case SDL_SCANCODE_S:
                        x_pos -= SPEED * cos(sprite_angle) / 60;
                        y_pos -= SPEED * sin(sprite_angle) / 60;
                        break;
                    case SDL_SCANCODE_A:
                        sprite_angle -= SPEED * M_PI / 180 / 60;
                        break;
                    case SDL_SCANCODE_D:
                        sprite_angle += SPEED * M_PI / 180 / 60;
                        break;
                }
            }
        }

        render_2d(rend_2d, x_pos, y_pos, sprite_angle);
        render_3d(rend_3d, x_pos, y_pos, sprite_angle);

        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(rend_2d);
    SDL_DestroyRenderer(rend_3d);
    SDL_DestroyWindow(win_2d);
    SDL_DestroyWindow(win_3d);
    SDL_Quit();

    return 0;
}
