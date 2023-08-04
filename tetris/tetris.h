#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Tetris
{

public:
    Tetris()
    {

    }
    ~Tetris()
    {

    }

    void setCurrentTime(Uint32 t)
    {
        currentTime =t;
        currentTime2=t;
    }

    bool isRunning()
    {
        return running;
    }

    bool isvalid();
    bool isvalid2();

    bool init(const char* title);
    void nextTetrimino();
    void nextTetrimino2();
    void handleEvents();
    void setPosRect(SDL_Rect& rect, int x=0, int y=0, int w=BlockW, int h = BlockH);
    void moveRectPos(SDL_Rect& rect,int x, int y);
    void gameplay();
    void updateRender();
    void clean();
    void updateScore();
    int nextT;

private:
    enum {ScreenW = 864, ScreenH = 800 };
    enum {BlockW = 36, BlockH = 36};
    enum {Lines = 20, Cols = 24};
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    SDL_Texture* background = NULL, * blocks = NULL, *text=NULL;
    SDL_Rect srcR = {0,0, BlockW, BlockH}, destR= {0,0, BlockW, BlockH};
    SDL_Rect textRect;
    TTF_Font *font = TTF_OpenFont("Font/Tetris.ttf",20);
    SDL_Color textColor = {0,0,0};
    bool running = false;
    int field[Lines][Cols]= {0};
    static const int figures[][4];
    struct Point
    {
        int x,y;
    } items[4],backup[4], items2[4], backup2[4];
    int color =1,color2=2;
    int dx =0;
    int dx2=0;
    bool rotate =false;
    bool rotate2 = false;
    unsigned int delay=1000,delay2=1000;
    unsigned int tempDelay=1000, tempDelay2=1000;
    int player1Score = 0; // Score for player 1
    int player2Score = 0; // Score for player 2
    int linesCleared = 0;
    int linesCleared2 = 0;
    int player1Level=0;
    int player2Level=0;

    void increasePlayer1Score(int linesCleared);
    void increasePlayer2Score(int linesCleared);
    void increasePlayer1Level(int linesCleared);
    void increasePlayer2Level(int linesCleared);
    void drawScores();
    Uint32 startTime=0, currentTime =0,currentTime2=0,startTime2=0;

};



#endif // TETRIS_H_INCLUDED
