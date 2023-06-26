//
// Created by Konstantin on 26/06/2023.
//

#include "Camera.h"


SDL_Rect *Camera::GetRect() {
    return &posrect;
}

Camera::Camera(int x, int y, int w, int h) {
    posrect={x,y,w,h};
}

Camera::~Camera() {
    SDL_RectEmpty(&posrect);
}

void Camera::SetRect(int x, int y, int w, int h) {
    posrect={x,y,w,h};
}