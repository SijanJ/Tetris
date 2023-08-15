#include "menu.h"
#include "tetris.h"
#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;



int main(int argc, char *argv[] )
{
    srand(time(0));
    Tetris* tetris = new Tetris();

    const char* title = "Tetris";
    if(tetris ->init(title))
    {
        Menu mainMenu(tetris->getRenderer());

        int selectedItemIndex = mainMenu.showMenu(false);

        if(selectedItemIndex == 0 || selectedItemIndex == 1){
                tetris->total_player = selectedItemIndex+1;
        while (tetris ->isRunning())
        {
            tetris-> setCurrentTime(SDL_GetTicks());
            tetris->handleEvents();
            tetris->gameplay();
            tetris ->updateRender();
        }
    }
    }
    else
    {
        cout << "Failed initialization";
    }
    tetris ->clean();
    return 0;

}
