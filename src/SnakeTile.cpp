//
// Created by Konstantin on 18/06/2023.
//

#include "SnakeTile.h"

SnakeTile::SnakeTile(int x, int y, int w, int h, bool isHead):isHead(isHead) {
    posrect={x,y,w,h};
}

SnakeTile::~SnakeTile() {
    SDL_RectEmpty(&posrect);
}

SDL_Rect* SnakeTile::GetRect() {
    return &posrect;
}

void SnakeTile::SetRect(int x,int y,int w, int h) {
    posrect={x,y,w,h};
}

bool SnakeTile::Head() const {
    return isHead;
}