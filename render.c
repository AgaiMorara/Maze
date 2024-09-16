#include "main.h"
#include <math.h>
#include <stdio.h>

int grid[GRID_ROWS][GRID_COLS] = {
    {4, 4, 4, 1, 4, 4, 4, 4, 1, 4},
    {0, 0, 0, 0, 0, 1, 1, 0, 1, 4},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 1, 0, 0, 1, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 4},
    {1, 1, 1, 0, 1, 0, 0, 0, 1, 8},
    {1, 0, 0, 0, 0, 0, 1, 0, 1, 12},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 12},
    {1, 28, 28, 64, 64, 64, 32, 1, 1, 12}
};

void draw_circle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

float cast_ray(float ray_angle, float x_pos, float y_pos, SDL_Renderer* rend, float* wall_x, float* wall_y) {
    float ray_x = x_pos;
    float ray_y = y_pos;
    float step_x = cos(ray_angle);
    float step_y = sin(ray_angle);

    while (1) {
        int grid_x = (int)(ray_x / CELL_SIZE);
        int grid_y = (int)(ray_y / CELL_SIZE);

        if (grid_x >= 0 && grid_x < GRID_COLS && grid_y >= 0 && grid_y < GRID_ROWS) {
            if (grid[grid_y][grid_x] == 1) {
                *wall_x = ray_x;
                *wall_y = ray_y;
                return sqrt((ray_x - x_pos) * (ray_x - x_pos) + (ray_y - y_pos) * (ray_y - y_pos));
            }
        }

        ray_x += step_x;
        ray_y += step_y;

        if (ray_x < 0 || ray_x >= WINDOW_WIDTH || ray_y < 0 || ray_y >= WINDOW_HEIGHT)
            break;
    }
    return -1;
}

float fix_fish_eye(float distance, float ray_angle, float sprite_angle) {
    return distance * cos(ray_angle - sprite_angle);
}

void render_2d(SDL_Renderer* rend_2d, float x_pos, float y_pos, float sprite_angle) {
    SDL_SetRenderDrawColor(rend_2d, 255, 255, 255, 255);
    SDL_RenderClear(rend_2d);

    SDL_SetRenderDrawColor(rend_2d, 0, 0, 0, 255);
    SDL_Rect wall;
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            if (grid[row][col] == 1) {
                wall.x = col * CELL_SIZE;
                wall.y = row * CELL_SIZE;
                wall.w = CELL_SIZE;
                wall.h = CELL_SIZE;
                SDL_RenderFillRect(rend_2d, &wall);
            }
        }
    }

    float half_fov = FOV / 2;
    float start_angle = sprite_angle - half_fov * M_PI / 180;
    float angle_step = FOV * M_PI / 180 / NUM_RAYS;

    for (int i = 0; i < NUM_RAYS; i++) {
        float ray_angle = start_angle + i * angle_step;
        float wall_x, wall_y;
        float ray_distance = cast_ray(ray_angle, x_pos + SPRITE_RADIUS, y_pos + SPRITE_RADIUS, rend_2d, &wall_x, &wall_y);

        if (ray_distance != -1) {
            SDL_SetRenderDrawColor(rend_2d, 255, 0, 0, 255);
            SDL_RenderDrawLine(rend_2d, x_pos + SPRITE_RADIUS, y_pos + SPRITE_RADIUS, wall_x, wall_y);
        }
    }

    draw_circle(rend_2d, (int)x_pos + SPRITE_RADIUS, (int)y_pos + SPRITE_RADIUS, SPRITE_RADIUS);
    SDL_RenderDrawLine(rend_2d, x_pos + SPRITE_RADIUS, y_pos + SPRITE_RADIUS,
                       x_pos + SPRITE_RADIUS + LINE_LENGTH * cos(sprite_angle),
                       y_pos + SPRITE_RADIUS + LINE_LENGTH * sin(sprite_angle));

    SDL_RenderPresent(rend_2d);
}

void render_3d(SDL_Renderer* rend_3d, float x_pos, float y_pos, float sprite_angle) {
    SDL_SetRenderDrawColor(rend_3d, 0, 0, 0, 255);
    SDL_RenderClear(rend_3d);

    float half_fov = FOV / 2;
    float start_angle = sprite_angle - half_fov * M_PI / 180;
    float angle_step = FOV * M_PI / 180 / NUM_RAYS;

    for (int i = 0; i < NUM_RAYS; i++) {
        float ray_angle = start_angle + i * angle_step;
        float wall_x, wall_y;
        float ray_distance = cast_ray(ray_angle, x_pos + SPRITE_RADIUS, y_pos + SPRITE_RADIUS, rend_3d, &wall_x, &wall_y);

        if (ray_distance != -1) {
            ray_distance = fix_fish_eye(ray_distance, ray_angle, sprite_angle);
            int wall_height = (int)((WINDOW_HEIGHT / ray_distance) * (CELL_SIZE / 2));
            SDL_SetRenderDrawColor(rend_3d, 0, 255, 0, 255);
            SDL_RenderDrawLine(rend_3d, i * (WINDOW_WIDTH / NUM_RAYS), (WINDOW_HEIGHT - wall_height) / 2,
                               i * (WINDOW_WIDTH / NUM_RAYS), (WINDOW_HEIGHT + wall_height) / 2);
        }
    }

    SDL_RenderPresent(rend_3d);
}
