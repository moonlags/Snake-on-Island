#include "window.h"

Window::~Window(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Window::DrawTexture(SDL_Texture* texture,SDL_Rect* src,SDL_Rect* dst){
	SDL_RenderCopy(renderer,texture,src,dst);
}

void Window::Present() {
    SDL_RenderPresent(renderer);
}

SDL_Texture* Window::LoadTexture(const char *file) {
    return IMG_LoadTexture(renderer,file);
}

SDL_Texture* Window::CreateScreenTexture() {
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, 600, 600, 32, SDL_PIXELFORMAT_ARGB8888);
    SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    return SDL_CreateTextureFromSurface(renderer,surface);
}

void Window::Clear() {
    SDL_RenderClear(renderer);
}

SDL_Event Window::GetEvent(){
	SDL_Event event;
    SDL_PollEvent(&event);
    return event;
}

void Window::PlaySound(Mix_Chunk* sound) {
    Mix_PlayChannel(-1,sound,0);
}

void Window::Init(const char* title,int x,int y,int w,int h) {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
    Mix_Volume(-1,MIX_MAX_VOLUME);

    window= SDL_CreateWindow(title,x,y,w,h,SDL_WINDOW_SHOWN);
    renderer= SDL_CreateRenderer(window,-1,0);
}

