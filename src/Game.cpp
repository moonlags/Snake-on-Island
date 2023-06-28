//
// Created by Konstantin on 17/06/2023.
//

#include "Game.h"
#include <ctime>
#include <iostream>
#include <chrono>
#include <cmath>

Game::Game(int applesMax, int snakeLenght,int map_size):applesMax(applesMax),map_size(map_size),inMenu(true),running(true),snakeLenght(snakeLenght) {
    window.Init("Snake",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,600,600);
    window.Clear();

    if(map_size<600){
        std::cout << "Map size is too small!"<<std::endl;
        running=false;
        return;
    }

    ground_texture=window.LoadTexture("res/grass.png");
    water_texture=window.LoadTexture("res/water.png");
    apple_texture=window.LoadTexture("res/apple.png");
    snake_head_texture=window.LoadTexture("res/snake_head.png");
    snake_body_texture=window.LoadTexture("res/snake_body.png");
    text_texture=window.LoadTexture("res/text.png");

    music_sfx= Mix_LoadWAV("res/music.wav");
    eat_sfx= Mix_LoadWAV("res/eat.wav");

    font= TTF_OpenFont("res/JetbrainsMono.ttf",24);

    std::srand(std::time(nullptr));

    vx=0;
    vy=0;
    Window::PlaySound(music_sfx,10);
}

auto start=std::chrono::system_clock::now();

void Game::GenerateMap(int groundCount) {
    int size=map_size/75*(map_size/75);
    if(groundCount>size){
        std::cout << "Too much ground for "<<size<<" map tiles!"<<std::endl;
        running=false;
        return;
    }
    std::vector<char> map_temp(size,'w');
    map_temp[std::rand()%size]='g';
    for(int i=0;i<groundCount-1;){
        int pos=std::rand()%size;
        if(((pos+8<size&&map_temp[pos+8]=='g')||(pos-8>=0&&map_temp[pos-8]=='g')||map_temp[pos+1]=='g'||map_temp[pos-1]=='g')&&map_temp[pos]!='g'){
            map_temp[pos]='g';
            ++i;
        }
    }

    int x = 0;
    int y = 0;
    for (int i = 1; i <= size; ++i)
    {
        if(map_temp[i-1]=='g'){
            SDL_Rect dst={x,y,75,75};
            map.push_back(Tile{'g',dst});
        }else{
            SDL_Rect dst={x,y,75,75};
            map.push_back(Tile{'w',dst});
        }
        if (y != int((i / (map_size/75) * 75)))
        {
            x = 0;
            y = (i / (map_size/75)) * 75;
            continue;
        }
        x += 75;
    }
}

Game::~Game() {
    SDL_DestroyTexture(ground_texture);
    SDL_DestroyTexture(water_texture);
    SDL_DestroyTexture(apple_texture);
    SDL_DestroyTexture(snake_body_texture);
    SDL_DestroyTexture(snake_head_texture);
    Mix_FreeChunk(music_sfx);
    Mix_FreeChunk(eat_sfx);
    apples.clear();
    snake.clear();
}

bool Game::isRunning() const {
    return running;
}

void Game::Update() {
    if (snake.empty()) {
        for(auto t:map){
            if(t.type=='g'){
                snake.emplace_back(t.pos.x, t.pos.y, 20, 20, true);
                break;
            }
        }
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
            if (!inMenu&&event.key.keysym.sym == SDLK_UP&&vy!=20) {
                vx = 0;
                vy = -20;
            } else if (!inMenu&&event.key.keysym.sym == SDLK_DOWN&&vy!=-20) {
                vx = 0;
                vy = 20;
            } else if (!inMenu&&event.key.keysym.sym == SDLK_LEFT&&vx!=20) {
                vy = 0;
                vx = -20;
            } else if (!inMenu&&event.key.keysym.sym == SDLK_RIGHT&&vx!=-20) {
                vy = 0;
                vx = 20;
            } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }else if(inMenu&&event.key.keysym.sym==SDLK_RETURN){
                inMenu=false;
            }
            break;
    }

    if(inMenu)return;

    if (apples.size() < applesMax) {
        SDL_Rect rect = {std::rand() % (map_size-37), std::rand() % (map_size-37), 37, 37};
        for (auto t: map) {
            if (t.type=='g'&&SDL_HasIntersection(&rect, &t.pos)) {
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
            Window::PlaySound(eat_sfx,0);
        }
    }


    {
        SDL_Rect * origrect=snake[0].GetRect();
        if(origrect->x>map_size){
            snake[0].SetRect(1,origrect->y,20,20);
        }else if(origrect->x+19<0){
            snake[0].SetRect(map_size-1,origrect->y,20,20);
        }else if(origrect->y+1>map_size){
            snake[0].SetRect(origrect->x,1,20,20);
        }else if(origrect->y+19<0){
            snake[0].SetRect(origrect->x,map_size-1,20,20);
        }
    }


    for(auto s:snake){
        if(!s.Head()&& SDL_HasIntersection(snake[0].GetRect(),s.GetRect())&&(vx!=0||vy!=0)){
            running=false;
            break;
        }
    }

    bool res=false;
    for(auto  t:map){
        if(t.type=='g'&&SDL_HasIntersection(snake[0].GetRect(),&t.pos)){
            res=true;
            break;
        }
    }
    if(!res){
        running=false;
    }

    SDL_Rect new_camera_rect={snake[0].GetRect()->x-300,snake[0].GetRect()->y-300,600,600};
    if(new_camera_rect.x<0){
        new_camera_rect.x=0;
    }if(new_camera_rect.x>map_size-600){
        new_camera_rect.x=map_size-600;
    }if(new_camera_rect.y<0){
        new_camera_rect.y=0;
    }if(new_camera_rect.y>map_size-600){
        new_camera_rect.y=map_size-600;
    }
    camera.SetRect(new_camera_rect.x,new_camera_rect.y,600,600);
}

void Game::Render() {
    window.Clear();

    for(auto t:map){
        if(!SDL_HasIntersection(&t.pos,camera.GetRect()))continue;

        SDL_Rect pos=t.pos;
        pos.x=t.pos.x-camera.GetRect()->x;
        pos.y=t.pos.y-camera.GetRect()->y;

        if(t.type=='w'){
            window.DrawTexture(water_texture,nullptr,&pos);
        }else if(t.type=='g'){
            window.DrawTexture(ground_texture,nullptr,&pos);
        }
    }

    if(inMenu) {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed=end-start;
        SDL_Rect rect={40,static_cast<int>(20+std::sin(elapsed.count()*5)*5),534,480};
        window.DrawTexture(text_texture,nullptr,&rect);
        window.Present();
        return;
    }

    for(Apple apple:apples){
        SDL_Rect pos={apple.GetRect()->x-camera.GetRect()->x,apple.GetRect()->y-camera.GetRect()->y,37,37};

        window.DrawTexture(apple_texture, nullptr,&pos);
    }

    for (auto tile:snake){
        SDL_Rect pos={tile.GetRect()->x-camera.GetRect()->x,tile.GetRect()->y-camera.GetRect()->y,20,20};
        if(tile.Head()){
            window.DrawTexture(snake_head_texture,nullptr,&pos);
        }else{
            window.DrawTexture(snake_body_texture,nullptr,&pos);
        }
    }

    SDL_Rect pos ={0,0,200,50};

    char tmp[32];
    sprintf(tmp,"%d",snakeLenght);

    char text[20]="Snake Length: ";
    window.RenderText(font, strcat(text,tmp),&pos);

    window.Present();
}