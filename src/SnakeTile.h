//
// Created by Konstantin on 18/06/2023.
//

#ifndef SNAKE_SNAKETILE_H
#define SNAKE_SNAKETILE_H
#include <SDL2/SDL.h>

class SnakeTile {
public:
    SnakeTile(int x,int y,int w,int h,bool isHead);
    virtual ~SnakeTile();
    SDL_Rect* GetRect();
    void SetRect(int x,int y,int w,int h);
    bool Head() const;
private:
    SDL_Rect posrect{};
    bool isHead;
};


#endif //SNAKE_SNAKETILE_H
