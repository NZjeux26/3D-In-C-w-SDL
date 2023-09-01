#include <stdio.h>
#include <math.h>
#include "include/SDL2/SDL.h"

typedef struct{
    float x;
    float y;
    float z;
} Vector3;

typedef struct{
    float m[3][3];
} Matrix3x3;

Vector3 rotated;
Vector3 projected2D;
Vector3 projected[8];

Vector3 points[8];
float angle = 0;

Vector3 createVector3(float x, float y, float z){
    Vector3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

void createPoints(){
    points[0] = createVector3(-0.5, -0.5, -0.5);
    points[1] = createVector3(0.5, -0.5, -0.5);
    points[2] = createVector3(0.5, 0.5, -0.5);
    points[3] = createVector3(-0.5, 0.5, -0.5);
    points[4] = createVector3(-0.5, -0.5, 0.5);
    points[5] = createVector3(0.5, -0.5, 0.5);
    points[6] = createVector3(0.5, 0.5, 0.5);
    points[7] = createVector3(-0.5, 0.5, 0.5);
}

//matrix-vector multiplication
Vector3 matmul(float matrix[3][3], Vector3 *vector) {
   float x = vector->x;
   float y = vector->y;
   float z = vector->z;

   vector->x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
   vector->y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
   vector->z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;

}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1200, 800,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        SDL_Log("Failed to create ERROR: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    createPoints();

    // Main loop
    int is_running = 1;
   
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = 0;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        float roZ[3][3] = {{cos(angle), -sin(angle), 0},{sin(angle), cos(angle), 0}, {0,0,1}};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < 8; i++){
         
        }

       SDL_RenderPresent(renderer);
       SDL_Delay(16); // Cap the frame rate
    }
    // Clean up and quit
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}