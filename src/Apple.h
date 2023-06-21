//
// Created by Konstantin on 18/06/2023.
//

#ifndef SNAKE_APPLE_H
#define SNAKE_APPLE_H
#pragma once
#include <SDL2/SDL.h>

class Apple {
public:
    Apple(int x,int y,int w,int h);
    virtual ~Apple();
    SDL_Rect* GetRect();
private:
    SDL_Rect posrect{};
};


#endif //SNAKE_APPLE_H
