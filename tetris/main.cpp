#include "tetris.h"
#include<iostream>
using namespace std;
int main(int argc, char *argv[] )
{
Tetris* tetris = new Tetris();

const char* title = "Tetris";
if(tetris ->init(title))
    {
    while (tetris ->isRunning())
    {
        tetris-> setCurrentTime(SDL_GetTicks());
        tetris->handleEvents();
        tetris->gameplay();
        tetris ->updateRender();
    }
}
else{
    cout << "Failed initialization";
}
tetris ->clean();
}
