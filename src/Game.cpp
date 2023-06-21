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
    if (snake.empty()) {
        snake.emplace_back(ground[1].x, ground[1].y, 20, 20, true);
    } else if (snake.size() < snakeLenght) {
        SDL_Rect *last_tile_rect = snake[snake.size() - 1].GetRect();
        snake.emplace_back(last_tile_rect->x, last_tile_rect->y, 20, 20, false);
    }

    auto event = Window::GetEvent();
    switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_UP&&vy!=20) {
                vx = 0;
                vy = -20;
            } else if (event.key.keysym.sym == SDLK_DOWN&&vy!=-20) {
                vx = 0;
                vy = 20;
            } else if (event.key.keysym.sym == SDLK_LEFT&&vx!=20) {
                vy = 0;
                vx = -20;
            } else if (event.key.keysym.sym == SDLK_RIGHT&&vx!=-20) {
                vy = 0;
                vx = 20;
            } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
            break;
    }
    if (apples.size() < applesMax) {
        SDL_Rect rect = {std::rand() % 570, std::rand() % 570, 37, 37};
        for (auto g: ground) {
            if (SDL_HasIntersection(&rect, &g)) {
                apples.emplace_back(rect.x, rect.y, 37, 37);
                break;
            }
        }
    }

    for (int i = snake.size() - 1; i >= 0; --i) {
        if (!snake[i].Head() && !SDL_HasIntersection(snake[i].GetRect(), snake[i - 1].GetRect())) {
            snake[i].SetRect(snake[i - 1].GetRect()->x, snake[i - 1].GetRect()->y, 20, 20);
        } else if (snake[i].Head()) {
            snake[i].SetRect(snake[i].GetRect()->x + vx, snake[i].GetRect()->y + vy, 20, 20);
        }
    }

    for (int i = 0; i < apples.size(); ++i) {
        if (SDL_HasIntersection(apples[i].GetRect(), snake[0].GetRect())) {
            ++snakeLenght;
            apples.erase(std::next(apples.begin(), i));
        }
    }

    {
        SDL_Rect * origrect=snake[0].GetRect();
        if(origrect->x+1>600){
            snake[0].SetRect(1,origrect->y,20,20);
        }else if(origrect->x<-10){
            snake[0].SetRect(599,origrect->y,20,20);
        }else if(origrect->y+1>600){
            snake[0].SetRect(origrect->x,1,20,20);
        }else if(origrect->y-1<0){
            snake[0].SetRect(origrect->x,599,20,20);
        }
    }

    for(auto s:snake){
        if(!s.Head()&& SDL_HasIntersection(snake[0].GetRect(),s.GetRect())&&(vx!=0||vy!=0)){
            running=false;
            break;
        }
    }

    bool res=false;
    for(auto  g:ground){
        if(SDL_HasIntersection(snake[0].GetRect(),&g)){
            res=true;
            break;
        }
    }
    if(!res){
        running=false;
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