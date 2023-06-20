//
// Created by Konstantin on 17/06/2023.
//

#include "Game.h"
#include <ctime>
#include <iostream>

Game::Game(int applesMax, int snakeLenght):applesMax(applesMax),running(true),snakeLenght(snakeLenght) {
    window.Init("Snake",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,600,600);
    window.Clear();

    ground_texture=window.LoadTexture("res/grass.png");
    water_texture=window.LoadTexture("res/water.png");
    apple_texture=window.LoadTexture("res/apple.png");
    snake_head_texture=window.LoadTexture("res/snake_head.png");
    snake_body_texture=window.LoadTexture("res/snake_body.png");

    std::srand(std::time(nullptr));
    vx=0;
    vy=0;
}

void Game::GenerateMap(int groundCount) {
    std::vector<char> map(64,'w');
    map[std::rand()%64]='g';
    for(int i=0;i<groundCount-1;){
        int pos=std::rand()%64;
        if(((pos+8<=63&&map[pos+8]=='g')||(pos-8>=0&&map[pos-8]=='g')||map[pos+1]=='g'||map[pos-1]=='g')&&map[pos]!='g'){
            map[pos]='g';
            ++i;
        }
    }

    int x = 0;
    int y = 0;
    for (int i = 1; i <= 64; ++i)
    {
        if(map[i-1]=='g'){
            SDL_Rect dst={x,y,75,75};
            window.DrawTexture(ground_texture,nullptr,&dst);
            ground.push_back(dst);
        }else{
            SDL_Rect dst={x,y,75,75};
            window.DrawTexture(water_texture,nullptr,&dst);
        }
        if (y != int((i / 8) * 75))
        {
            x = 0;
            y = (i / 8) * 75;
            continue;
        }
        x += 75;
    }
    window.Present();

    map_texture=window.CreateScreenTexture();
}

Game::~Game() {
    SDL_DestroyTexture(ground_texture);
    SDL_DestroyTexture(water_texture);
    SDL_DestroyTexture(apple_texture);
    SDL_DestroyTexture(map_texture);
    SDL_DestroyTexture(snake_body_texture);
    SDL_DestroyTexture(snake_head_texture);
    apples.clear();
    snake.clear();
}

bool Game::isRunning() const {
    return running;
}

void Game::Update() {
    if(snake.empty()){
        snake.emplace_back(ground[0].x,ground[0].y,37,37,true);
    }else if(snake.size()<snakeLenght){
        SDL_Rect* last_tile_rect=snake[snake.size()-1].GetRect();
        snake.emplace_back(last_tile_rect->x,last_tile_rect->y,37,37,false);
    }

    auto event=Window::GetEvent();
    switch(event.type){
        case SDL_QUIT:
            running=false;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym==SDLK_UP){
                vx=0;
                vy=-37;
            }else if(event.key.keysym.sym==SDLK_DOWN){
                vx=0;
                vy=37;
            }else if(event.key.keysym.sym==SDLK_LEFT){
                vy=0;
                vx=-37;
            }else if(event.key.keysym.sym==SDLK_RIGHT){
                vy=0;
                vx=37;
            }
            break;
    }
    if (apples.size()<applesMax){
        SDL_Rect rect={std::rand()%570,std::rand()%570,37,37};
        for(auto g:ground){
            if(SDL_HasIntersection(&rect,&g)){
                apples.emplace_back(rect.x,rect.y,37,37);
                break;
            }
        }
    }
    //FROM OTHER SIDE OF VECTOR!!!!!
    for(int i=0;i<snake.size();++i){
        if(!snake[i].Head()&&!SDL_HasIntersection(snake[i].GetRect(),snake[i-1].GetRect())){
            snake[i].SetRect(snake[i-1].GetRect()->x,snake[i-1].GetRect()->y,37,37);
        }else{
            snake[i].SetRect(snake[i].GetRect()->x+vx,snake[i].GetRect()->y+vy,37,37);
        }
    }
}

void Game::Render() {
    window.Clear();
    window.DrawTexture(map_texture,nullptr,nullptr);

    for(Apple apple:apples){
        window.DrawTexture(apple_texture, nullptr,apple.GetRect());
    }

    //std::cout << snake[1].GetRect()->x<<" "<<snake[1].GetRect()->y<<std::endl;

    for (auto tile:snake){
        if(tile.Head()){
            window.DrawTexture(snake_head_texture,nullptr,tile.GetRect());
        }else{
            window.DrawTexture(snake_body_texture,nullptr,tile.GetRect());
        }
    }

    window.Present();
}