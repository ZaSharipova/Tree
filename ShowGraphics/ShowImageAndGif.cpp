#include "SDL.h"
#include "SDL_image.h"
#include "gifdec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int ShowImage(const char *filename) {
    assert(filename);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    const char *ext = strrchr(filename, '.');
    if (!ext) {
        fprintf(stderr, "File has no extension\n");
        SDL_Quit();
        return 1;
    }

    if (strcmp(ext, ".gif") != 0 && strcmp(ext, ".GIF") != 0) {
        if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
            fprintf(stderr, "IMG_Init error: %s\n", IMG_GetError());
            SDL_Quit();
            return 1;
        }

        SDL_Surface *img = IMG_Load(filename);
        if (!img) {
            fprintf(stderr, "IMG_Load error: %s\n", IMG_GetError());
            IMG_Quit();
            SDL_Quit();
            return 1;
        }

        SDL_Window* window = SDL_CreateWindow("Image", 0, 0,
            img->w, img->h, SDL_WINDOW_SHOWN);
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, img);
        SDL_FreeSurface(img);

        SDL_Event e = {};
        int running = 1;
        while (running) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) running = 0;
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) running = 0;
                if (e.type == SDL_MOUSEBUTTONDOWN) running = 0; 
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, tex, NULL, NULL);
            SDL_RenderPresent(renderer);
        }

        SDL_DestroyTexture(tex);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        IMG_Quit();
        SDL_Quit();
        return 0;
    }


    gd_GIF *gif = gd_open_gif(filename);
    if (!gif) {
        fprintf(stderr, "Failed to open GIF\n");
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("GIF Viewer", 0, 0,
        gif->width, gif->height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        gif->width, gif->height);

    uint8_t *frame = (uint8_t *)malloc(gif->width * gif->height * 3);
    if (!frame) {
        fprintf(stderr, "Failed to allocate memory for GIF frame\n");
        gd_close_gif(gif);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    gd_get_frame(gif);
    gd_render_frame(gif, frame);

    SDL_Event e = {};
    int running = 1;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&e) && running) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) running = 0;
            if (e.type == SDL_MOUSEBUTTONDOWN) running = 0; 
        }

        if (!running) break;

        gd_render_frame(gif, frame);
        SDL_UpdateTexture(tex, NULL, frame, gif->width * 3);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, tex, NULL, NULL);
        SDL_RenderPresent(renderer);

        int delay = gif->gce.delay * 10;
        if (delay < 20) {
            delay = 20;
        }
        Uint32 now = SDL_GetTicks();
        if (now - last_time < (Uint32)delay) {
            SDL_Delay((Uint32)delay - (now - last_time));
        }
        last_time = SDL_GetTicks();

        if (!gd_get_frame(gif)) {
            gd_rewind(gif);
            gd_get_frame(gif);
        }
    }

    free(frame);
    gd_close_gif(gif);
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0; 
}
