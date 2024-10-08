#ifndef MAIN_H
#define MAIN_H


#include <SDL2/SDL.h>
#include <stdint.h>
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define CELL_SIZE (36)
#define FOV (60)  /* Field of view in degrees */
#define NUM_RAYS (WINDOW_WIDTH)  /* One ray per column */
#define SPEED (300)
#define TURN_SPEED (5 * M_PI / 180)  /* 5 degrees per frame */

extern int grid[16][16];
extern int rayangle;
extern float player_angle;
extern int texture[200 * 200 * 3];
extern int grass_texture[200 * 200 * 3];
extern int sky_texture[200 * 200 * 3];

/* Function declarations */
void handle_movement(const uint8_t *keystate, float *x_vel, float *y_vel, float *angle);
void update_player_position(float *x_pos, float *y_pos, float x_vel, float y_vel);
float cast_ray(float ray_angle, float x_pos, float y_pos, SDL_Renderer *rend,int is_3d, int ray_index);
void draw_grid(SDL_Renderer *rend);
void draw_player(SDL_Renderer *rend, float x_pos, float y_pos, float player_angle);
void draw_ceiling(SDL_Renderer *rend, int *ceiling_texture, int ceiling_width, int ceiling_height);
void draw_floor(SDL_Renderer *rend, int *floor_texture, int floor_width, int floor_height);


#endif
