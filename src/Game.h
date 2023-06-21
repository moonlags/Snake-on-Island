//
// Created by Konstantin on 17/06/2023.
//

#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H
#pragma once

#include "window.h"
#include "Apple.h"
#include "SnakeTile.h"
#include <vector>

class Game {
public:
    Game(int applesMax,int snakeLenght);
    bool isRunning() const;
    virtual ~Game();
    void GenerateMap(int groundCount);
    void Update();
    void Render();
private:
    bool inMenu;
    bool running;
    Window window;
    SDL_Texture* text_texture;
    SDL_Texture* map_texture{};
    SDL_Texture* apple_texture;
    SDL_Texture* snake_head_texture;
    SDL_Texture* snake_body_texture;
    SDL_Texture* water_texture;
    SDL_Texture* ground_texture;
    int applesMax;
    std::vector<Apple> apples;
    std::vector<SnakeTile> snake;
    int snakeLenght;
    std::vector<SDL_Rect> ground;
    int vx,vy;
};


#endif //SNAKE_GAME_H
