//
//  main.c
//  First_SDL2_Code
//
//  Created by Gregoire Mahon & Armand Lelong on 12/01/2023.
//
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

int windowWidth = 1920;
int windowHeight = 1080;

// Def window limits function
void handle_window_limits(SDL_Rect *characterRect, int windowWidth, int windowHeight) {
    // Check if the character is outside of the left or right limits
    if (characterRect->x < 0) {
        characterRect->x = 0;
    } else if (characterRect->x + characterRect->w > windowWidth) {
        characterRect->x = windowWidth - characterRect->w;
    }

    // Check if the character is outside of the top or bottom limits
    if (characterRect->y < 0) {
        characterRect->y = 0;
    } else if (characterRect->y + characterRect->h > windowHeight) {
        characterRect->y = windowHeight - characterRect->h;
    }
}

int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { // Return 0 if initialization is OK
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    // Create the window
    SDL_Window *window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, 0);
    if (window == NULL) {
        printf("Failed to create the window: %s\n", SDL_GetError());
        return 1;
    }

    // Create the renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Failed to create the renderer: %s\n", SDL_GetError());
        return 1;
    }
    // Load the background image
    SDL_Surface *backgroundSurface = SDL_LoadBMP("background.bmp");
    if (backgroundSurface == NULL) {
        printf("Failed to load the background image: %s\n", SDL_GetError());
        return 1;
    }

    // Create the background texture
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (backgroundTexture == NULL) {
        printf("Failed to create the background texture: %s\n", SDL_GetError());
        return 1;
    }

    // Set the initial position of the background
    SDL_Rect backgroundRect = {0, 0, windowWidth, windowHeight};
    
    // Load the character image
    SDL_Surface *characterSurface = SDL_LoadBMP("character.bmp");
    if (characterSurface == NULL) {
        printf("Failed to load the character image: %s\n", SDL_GetError());
        return 1;
    }

    // Create the character texture
    SDL_Texture *characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    SDL_FreeSurface(characterSurface);
    if (characterTexture == NULL) {
        printf("Failed to create the character texture: %s\n", SDL_GetError());
        return 1;
    }

    // Set the initial position of the character
    SDL_Rect characterRect = {0, 0, 192, 108};

    // Run the main game loop
    int frameCounter = 0; // Initialize the frame counter
    int jumpHeight = 50; // Set the jump height
    int jumpDuration = 30; // Set the duration of the jump in frames
    int jumping = 0; // Initialize the jumping flag
    int running = 1;
    
                while (running) {
                    // Handle events
                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            running = 0;
                        } else if (event.type == SDL_KEYDOWN) {
                            switch (event.key.keysym.sym) {
                                case SDLK_UP:
                                    characterRect.y -= 50;
                                    break;
                                case SDLK_DOWN:
                                    characterRect.y += 50;
                                    break;
                                case SDLK_LEFT:
                                    characterRect.x -= 50;
                                    break;
                                case SDLK_RIGHT:
                                    characterRect.x += 50;
                                    break;
                                case SDLK_SPACE:
                                    if (jumping == 0) { // Only jump if the character is not already jumping
                                        jumping = 1;
                                        frameCounter = 0;
                                    }
                                    break;
                            }
                        }
                    }
                    
                    // Update the character position
                    if (jumping) {
                        if (frameCounter < jumpDuration / 2) { // If only jumps up
                            characterRect.y -= jumpHeight / (jumpDuration / 2);
                        } else {
                            characterRect.y += jumpHeight / (jumpDuration / 2); // if jumped up, then character goes down
                        }
                        frameCounter++;
                        if (frameCounter == jumpDuration) {
                            jumping = 0;
                        }
                    }
                    // Avoid window limits
                    handle_window_limits(&characterRect, windowWidth, windowHeight);
                    
                    // Clear the renderer
                    SDL_RenderClear(renderer);

                    // Draw the character
                    SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
                    SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);

                    // Update the renderer
                    SDL_RenderPresent(renderer);
                    // Wait for a bit
                    SDL_Delay(10);
                }

                // Clean up
                SDL_DestroyTexture(characterTexture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();

                return 0;
            }
