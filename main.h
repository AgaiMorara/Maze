#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define GRID_ROWS 10
#define GRID_COLS 10
#define CELL_SIZE 64
#define M_PI 3.14159265358979323846

#define SPEED (300)
#define FOV 60
#define NUM_RAYS 300
#define SPRITE_RADIUS 15
#define LINE_LENGTH 30

// Global grid data
extern int grid[GRID_ROWS][GRID_COLS];

// Function declarations
void draw_circle(SDL_Renderer* renderer, int x, int y, int radius);
float cast_ray(float ray_angle, float x_pos, float y_pos, SDL_Renderer* rend, float* wall_x, float* wall_y);
float fix_fish_eye(float distance, float ray_angle, float sprite_angle);
void render_2d(SDL_Renderer* rend_2d, float x_pos, float y_pos, float sprite_angle);
void render_3d(SDL_Renderer* rend_3d, float x_pos, float y_pos, float sprite_angle);

#endif
