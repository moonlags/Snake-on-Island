#include <iostream>
#include "Game.h"

int main(int argc,char** argv){
    Game game=Game(3,3,1200);
    game.GenerateMap((1200/75*(1200/75))*0.6);
    while(game.isRunning()){
        game.Update();
        game.Render();

        SDL_Delay(100);
    }
    std::cout <<"Game over!"<<std::endl;
    SDL_Delay(2000);
    return 0;
}