#include <stdio.h>
#include <math.h>
#include "include/SDL2/SDL.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
//custom struct for 3d vectors
typedef struct{
    float x;
    float y;
    float z;
} Vector3;

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
//intionlises the 3d points of the cube with cords
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
void matmul(float matrix[3][3], Vector3 *vector) {
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
        "3D Cube",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
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
        //defines 3x3 matrixes for each axis, see https://en.wikipedia.org/wiki/Rotation_matrix under basic 3d Rotations, note matching rotations to the code here
        float roX[3][3] = {{1,0,0}, {0,cos(angle), -sin(angle)}, {0, sin(angle), cos(angle)}};
        float roY[3][3] = {{cos(angle), 0, sin(angle)}, {0,1,0}, {-sin(angle), 0, cos(angle)}};
        float roZ[3][3] = {{cos(angle), -sin(angle), 0},{sin(angle), cos(angle), 0}, {0,0,1}};

        Vector3 projected[8];
        
        //apply rotations and projections to the 3d Points
        for (int i = 0; i < 8; i++){
            //the matmuls applies the rotations to the matreices
            Vector3 rotated = points[i];
            matmul(roY, &rotated);
            matmul(roX, &rotated);
            matmul(roZ, &rotated);
            //Everything below calculates the 3d projection onto 2d plane (perspective)
            float distance = 2;
            float z = 1 / (distance - rotated.z);

            float projection[2][3] = {{z, 0, 0}, {0,z,0}};
            Vector3 projected2D = {0,0,0};

            projected2D.x = projection[0][0] * rotated.x + projection[0][1] * rotated.y + projection[0][2] * rotated.z;
            projected2D.y = projection[1][0] * rotated.x + projection[1][1] * rotated.y + projection[1][2] * rotated.z;

            projected2D.x *= 200;
            projected2D.y *= 200;

            projected[i] = projected2D;

        }
        //draw the points to shape the cube
        for(int i = 0; i < 8; i++){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, projected[i].x + WINDOW_WIDTH / 2, projected[i].y + WINDOW_HEIGHT / 2);
        }
        //Draw the lines between the points to form the cube
        for(int i = 0; i<4; i++){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer, projected[i].x + WINDOW_WIDTH / 2, projected[i].y + WINDOW_HEIGHT / 2, projected[(i+1) % 4].x + WINDOW_WIDTH / 2, projected[(i + 1) % 4].y + WINDOW_HEIGHT / 2);
            SDL_RenderDrawLine(renderer, projected[i + 4].x + WINDOW_WIDTH / 2, projected[i + 4].y + WINDOW_HEIGHT / 2, projected[((i+1) %4) + 4].x + WINDOW_WIDTH / 2, projected[((i+1) % 4) + 4].y + WINDOW_HEIGHT / 2);
            SDL_RenderDrawLine(renderer, projected[i].x + WINDOW_WIDTH / 2, projected[i].y + WINDOW_HEIGHT / 2, projected[i+ 4].x + WINDOW_WIDTH / 2, projected[i + 4].y + WINDOW_HEIGHT / 2);
        }

       SDL_RenderPresent(renderer);
       angle+= 0.03;
       SDL_Delay(16); // Cap the frame rate
    }
    // Clean up and quit
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}