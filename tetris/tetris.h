#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Tetris
{

    public:
    Tetris(){

    }
    ~Tetris(){

    }

    void setCurrentTime(Uint32 t)
    {
        currentTime =t;
    }

    bool isRunning()
    {
    return running;
    }

    bool isvalid();
    bool isvalid2();

    bool init(const char* title);
    void nextTetrimino();
    void handleEvents();
    void setPosRect(SDL_Rect& rect, int x=0, int y=0, int w=BlockW, int h = BlockH);
    void gameplay();
    void updateRender();
    void clean();
    int nextT;

    private:
        enum { ScreenW = 720, ScreenH = 800 };
        enum {BlockW = 36, BlockH = 36};
        enum { Lines = 20, Cols = 22};
        SDL_Window* window = NULL;
	SDL_Renderer* render = NULL;
	SDL_Texture* background = NULL, * blocks = NULL;
     SDL_Rect srcR = {0,0, BlockW, BlockH}, destR={0,0, BlockW, BlockH};
     bool running = false;
     int field[Lines][Cols]={0};
     static const int figures[7][4];
     struct Point
     {
         int x,y;
     } items[4],backup[4], items2[4], backup2[4];
     int color =1;
     int dx =0;
     int dx2=0;
     bool rotate =false;
     bool rotate2 = false;
     unsigned int delay = 300;
     Uint32 startTime=0, currentTime =0;

};


#endif // TETRIS_H_INCLUDED
