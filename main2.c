#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "render.h"
#include "grass.c"
#include "sky.c"

float player_angle;

int main(void) {
  SDL_Window *win;
  SDL_Renderer *rend;
  int close_requested = 0, i;
  float half_fov = FOV *M_PI / 180.0 / 2;
  
  SDL_Event event;

 /* Player's initial position and angle */
    float x_pos = 2 * CELL_SIZE;
    float y_pos = 2 * CELL_SIZE;
    float ray_angle;

const uint8_t *keystate;
 float x_vel = 0, y_vel = 0; /*player starts facing right 0 rad */


  /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    /* Create window */
    win = SDL_CreateWindow("AgaiMorar Raycasting in  2D and 3D",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH * 2, WINDOW_HEIGHT, 0);
    if (!win) {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* Create renderer */
    rend  = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!rend) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    
   
    /* Main loop */
    while (!close_requested) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                close_requested = 1;
            }
        }
	keystate = SDL_GetKeyboardState(NULL);

        /* Handle movement input */
        handle_movement(keystate, &x_vel, &y_vel, &player_angle);

        /* Check for collision and update position */
        update_player_position(&x_pos, &y_pos, x_vel, y_vel);

        /* Clear screen */
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderClear(rend);

        /* Draw grid and player */
	draw_ceiling(rend, sky_texture, 200, 200)
        draw_grid(rend);
        draw_player(rend, x_pos, y_pos, player_angle);

        /* Cast rays in both 2D and 3D views */
	for(i = 0; i < NUM_RAYS; i++)
	  {
	    ray_angle = player_angle - half_fov + i * (FOV * M_PI /180 )/ NUM_RAYS;
	    cast_ray(ray_angle, x_pos, y_pos, rend, 0, i);
            cast_ray(ray_angle, x_pos, y_pos, rend, 1, i);
            }
	    SDL_RenderPresent(rend);  /* Present everything */
	    SDL_Delay(1000 / 60);  /* Cap at 60 FPS */
	  }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
