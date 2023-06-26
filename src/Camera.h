//
// Created by Konstantin on 26/06/2023.
//

#ifndef SNAKE_CAMERA_H
#define SNAKE_CAMERA_H
#include <SDL2/SDL.h>


class Camera {
public:
    Camera(int x,int y,int w,int h);
    virtual ~Camera();
    SDL_Rect* GetRect();
    void SetRect(int x,int y,int w,int h);
private:
    SDL_Rect posrect{};
};


#endif //SNAKE_CAMERA_H
