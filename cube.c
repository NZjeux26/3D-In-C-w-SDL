#include <stdio.h>
#include <math.h>
#include "include/SDL2/SDL.h"

int mains(int argc, char* argv[]) {
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

    // Main loop
    int is_running = 1;
    float angle = 0;
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //Rendering can be done here
        SDL_FPoint vertices[8] = { //where in the window the verts go
            {300,300},  //front top-left     0
            {500,300},  //front top-right    1
            {500,500},  //front bottom-right 2
            {300,500},  //front bottom-left  3
            {280,280},    //back TL 4
            {480,280},   //back TR 5
            {480,480},  //back BR 6 
            {280,480}    //back BL 7
        };
            
        SDL_Point cubeCentre = { 
            (vertices[6].x + vertices[0].x) / 2, 
            (vertices[6].y + vertices[0].y) / 2
            }; 

        int edges[12][2] = {
            {0,1}, {1,2}, {2,3}, {3,0},   //conenct front
            {4,5}, {5,6}, {6,7}, {7,4},   //conenct back
            {0,4}, {1,5}, {2,6}, {3,7}     //connect rear and front
        }; 

        angle +=0.01;
        for(int i = 0; i < 8; ++i){
            //translate vertices to origin
            float transX = vertices[i].x - cubeCentre.x;
            float transY = vertices[i].y - cubeCentre.y;
            //rotate around Z axis
            float newY = transX * cos(angle) - transY * sin(angle);
            float newX = transX * sin(angle) + transY * cos(angle);

            vertices[i].x = newX + cubeCentre.x;
            vertices[i].y = newY + cubeCentre.y;
        }
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);

        for(int i = 0; i < 12; i++){
         SDL_RenderDrawLine(renderer, vertices[edges[i][0]].x, vertices[edges[i][0]].y, vertices[edges[i][1]].x,vertices[edges[i][1]].y);
        }

       SDL_RenderPresent(renderer);
       SDL_Delay(16); // Cap the frame rate
    }
    // Clean up and quit
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}