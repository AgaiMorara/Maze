#include "render.h"
#include <SDL2/SDL.h>
#include <math.h>

extern int texture[200 * 200 * 3];  /* Texture array from bamboo.c */

/* Function to cast a single ray and find the distance to the nearest wall */
float cast_ray(float ray_angle, float x_pos, float y_pos, SDL_Renderer *rend, int is_3d, int ray_index) {
    int wall_height;
    float ray_x = cos(ray_angle);
    float ray_y = sin(ray_angle);
    float distance = 0;
    int hit_wall = 0;
    int grid_x;
    int grid_y;
    int texture_x, texture_y,tex_index, r, g, b, y ;

    float corrected_dist;
    float step_size = 0.1;
    float hit_x = x_pos, wall_hit_x;
    float hit_y = y_pos;

    /* Normalize the ray angle */
    ray_angle = fmod(ray_angle, 2 * M_PI);
    if (ray_angle < 0) ray_angle += 2 * M_PI;

    while (!hit_wall) {
        hit_x += ray_x * step_size;
        hit_y += ray_y * step_size;
        distance += step_size;

        /* Check if we hit a wall */
        grid_x = (int)(hit_x / CELL_SIZE);
        grid_y = (int)(hit_y / CELL_SIZE);

        if (grid_x < 0 || grid_x >= 16 || grid_y < 0 || grid_y >= 16 || grid[grid_y][grid_x] == 1) {
            hit_wall = 1;
        }
    }

    /* Apply fisheye correction */
    corrected_dist = distance * cos(ray_angle - player_angle);

    /* Calculate wall height (2/3 of the screen max) */
    wall_height = (int)((WINDOW_HEIGHT * 2 / 3) * CELL_SIZE / corrected_dist);

    /* Calculate texture offset based on where the ray hit */
     wall_hit_x = fmod(hit_x, CELL_SIZE) / CELL_SIZE;  /* Horizontal hit position within the wall */
    texture_x = (int)(wall_hit_x * 200);  /* Map to texture coordinates */

    /* Render the wall slice with texture */
    for (y = 0; y < wall_height; y++) {
      texture_y = (int)((float)y / wall_height * 200);  /* Map to texture coordinates */
      tex_index = (texture_y * 200 + texture_x) * 3;    /* Texture index (RGB) */

      /* Fetch RGB values from the texture array */
        r = texture[tex_index];
        g = texture[tex_index + 1];
        b = texture[tex_index + 2];

        /* Set the color for the wall slice pixel */
        SDL_SetRenderDrawColor(rend, r, g, b, 255);

        /* Draw the pixel (column of the wall slice) */
        SDL_RenderDrawPoint(rend, WINDOW_WIDTH + ray_index, (WINDOW_HEIGHT - wall_height) / 2 + y);
    }

    /* Return the calculated distance */
    return distance;
}
