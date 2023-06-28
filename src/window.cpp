#include "window.h"

Window::~Window(){
    Mix_Quit();
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

void Window::Clear() {
    SDL_RenderClear(renderer);
}

SDL_Event Window::GetEvent(){
	SDL_Event event;
    SDL_PollEvent(&event);
    return event;
}

void Window::PlaySound(Mix_Chunk* sound,int loops) {
    Mix_PlayChannel(-1,sound,loops);
}

void Window::Init(const char* title,int x,int y,int w,int h) {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
    Mix_Volume(-1,MIX_MAX_VOLUME);

    TTF_Init();

    window= SDL_CreateWindow(title,x,y,w,h,SDL_WINDOW_SHOWN);
    renderer= SDL_CreateRenderer(window,-1,0);
}

void Window::RenderText(TTF_Font * font,const char * text,SDL_Rect* pos) {
    SDL_Surface* surface= TTF_RenderText_Solid(font,text,SDL_Color{0,0,0});

    SDL_Texture* texture= SDL_CreateTextureFromSurface(renderer,surface);

    SDL_RenderCopy(renderer,texture,nullptr,pos);
}