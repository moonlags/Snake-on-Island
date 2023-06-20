//
// Created by Konstantin on 18/06/2023.
//

#include "Apple.h"


Apple::Apple(int x, int y, int w, int h) {
    posrect={x,y,w,h};
}

Apple::~Apple() {
    SDL_RectEmpty(&posrect);
}

SDL_Rect* Apple::GetRect() {
    return &posrect;
}