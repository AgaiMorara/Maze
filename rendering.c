#include "render.h"
#include <SDL2/SDL.h>

/* Grid definition */
int grid[16][16] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
int line_x, line_y;

/* Function to draw the 2D grid */
void draw_grid(SDL_Renderer *rend) {
  SDL_Rect wall_rect;
  int i, j;
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);  /* Black for walls */
    for ( i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            if (grid[i][j] == 1) {
                wall_rect.x = j * CELL_SIZE;
		wall_rect.y = i * CELL_SIZE;
		wall_rect.w = CELL_SIZE;
		wall_rect.h = CELL_SIZE;
                SDL_RenderFillRect(rend, &wall_rect);
            }
        }
    }
}

/* Function to draw the player */
void draw_player(SDL_Renderer *rend, float x_pos, float y_pos, float angle) {
  int radius = 5, dy, dx, w, h;
    SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);  /* Blue for player */
    for (w = 0; w < radius * 2; w++) {
        for (h = 0; h < radius * 2; h++) {
             dx = radius - w;
	     dy = radius - h;
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(rend, x_pos + dx, y_pos + dy);
            }
        }
    }
    /* Draw direction line */
     line_x = x_pos + cos(angle) * 20;
     line_y = y_pos + sin(angle) * 20;
    SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);  /* Green for direction */
    SDL_RenderDrawLine(rend, x_pos, y_pos, line_x, line_y);
}

/* Handle player movement*/
void handle_movement(const Uint8 *keystate, float *x_vel, float *y_vel, float *angle) {
    if (keystate[SDL_SCANCODE_W]) {
        *x_vel = cos(*angle) * SPEED / 60;
        *y_vel = sin(*angle) * SPEED / 60;
    } else if (keystate[SDL_SCANCODE_S]) {
        *x_vel = -cos(*angle) * SPEED / 60;
        *y_vel = -sin(*angle) * SPEED / 60;
    } else {
        *x_vel = 0;
        *y_vel = 0;
    }
    if (keystate[SDL_SCANCODE_A]) {
        *angle -= TURN_SPEED;
    } else if (keystate[SDL_SCANCODE_D]) {
        *angle += TURN_SPEED;
    }
}

/* Update player position */
void update_player_position(float *x_pos, float *y_pos, float x_vel, float y_vel) {
    float new_x = *x_pos + x_vel;
    float new_y = *y_pos + y_vel;

    int grid_x = (int)(new_x / CELL_SIZE);
    int grid_y = (int)(new_y / CELL_SIZE);

    if (grid[grid_y][grid_x] == 0) {
        *x_pos = new_x;
        *y_pos = new_y;
    }
}
