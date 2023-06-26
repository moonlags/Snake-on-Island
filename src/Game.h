//
// Created by Konstantin on 17/06/2023.
//

#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H
#pragma once

#include "window.h"
#include "Apple.h"
#include "SnakeTile.h"
#include "Camera.h"
#include <vector>

struct Tile{
public:
    char type;
    SDL_Rect pos;
};

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
    SDL_Texture* apple_texture;
    SDL_Texture* snake_head_texture;
    SDL_Texture* snake_body_texture;
    SDL_Texture* water_texture;
    SDL_Texture* ground_texture;
    Mix_Chunk* music_sfx;
    Mix_Chunk* eat_sfx;
    int applesMax;
    std::vector<Apple> apples;
    std::vector<SnakeTile> snake;
    Camera camera{0,0,600,600};
    int snakeLenght;
    std::vector<Tile> map;
    int vx,vy;
};


#endif //SNAKE_GAME_H
