#include "Game.h"

int main(int argc,char** argv){
    Game game=Game(3,3);
    game.GenerateMap(40);
    while(game.isRunning()){
        game.Update();
        game.Render();

        SDL_Delay(100);
    }
    //SDL_Delay(5000);
    return 0;
}