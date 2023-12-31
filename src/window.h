#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

class Window{
public:
	void Init(const char* title,int x,int y,int w,int h);
	virtual ~Window();

	void DrawTexture(SDL_Texture* texture,SDL_Rect* src,SDL_Rect* dst);
    SDL_Texture* LoadTexture(const char* file);
	void Clear();
	static SDL_Event GetEvent();
    void Present();
    static void PlaySound(Mix_Chunk* sound,int loops);
    void RenderText(TTF_Font* font,const char* text,SDL_Rect* pos);
private:
	SDL_Window* window=nullptr;
	SDL_Renderer* renderer=nullptr;
};