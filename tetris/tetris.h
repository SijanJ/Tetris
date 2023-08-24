#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED

#include <string>
#include "mixer.h"
#include "SDL.hpp"

class Tetris:public SDL
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
        currentTime[1] =t;
        currentTime[2]=t;
    }

    bool isRunning()
    {
        return running;
    }

    bool isvalid(int player);

    SDL_Renderer* getRenderer()
    {
        return render;
    }
    bool init(const char* title);
    void nextTetrimino(int player_no, bool differentShape = false);
    void handleEvents();
    void setPosRect(Rect& rect, int x=0, int y=0, int w=BlockW, int h = BlockH);
    void gameplay();
    void pauseMenuOptions(int);
    void updateRender();
    void renderText(std::string, int, int);
    void clean();
    void hardDrop(int player);
    int clearLines(int player);
    int total_player;
    int player[3];
    mixer m1;

private:
    enum {ScreenW = 1920, ScreenH = 1080 };
    enum {BlockW = 40, BlockH = 40};
    enum {Lines = 20 +3, Cols = 48};
    Window* window = NULL;
    Renderer* render = NULL;
    Texture* background = NULL, * blocks = NULL, *powerup_img = NULL, *small_blocks = NULL;
    Texture* player1LevelTexture=NULL, * player2LevelTexture=NULL;
    Surface* player1LevelSurface=NULL, *player2LevelSurface=NULL;
    Rect srcR = {0,0, BlockW, BlockH}, destR= {0,0, BlockW, BlockH};
    Rect playerRect;
    Rect gameOverRect;
    Font *font;
    Color textColor = {0,0,0};
    bool running = false;
    bool paused = false;
    int showPauseMenu();
    void clearField(int);
    void resetGame();
    int gameOver();
    void gameOverMenuOptions(int);
    bool isGameOver[3]= {false,false,false};

    int field[Lines][Cols]= {0};

    static const int figures[][8];
    struct Point
    {
        int x,y;
    } items[3][8],backup[3][8],shadowItems[3][8], nextItem[3][4];

    int color[3] = {1,3,4};
    int dx[3] = {0};
    int n[3];
    int nextT[3]={0,rand()%7,rand()%7};
    bool rotate[3] = {false};
    unsigned int delay[3] , pDelay[3]={ 0 };

    unsigned int tempDelay[3]= {1000,1000,1000};

    int playerScore[3] = {0}; // Score for players
   int linesCleared[3] = {0};
    int playerLevel[3]= {1,1,1};
    std::string player2LevelText;
    std::string player1LevelText;

    int powerup_x[3],powerup_y[3];
    int powerupStartTime[3];
    bool isPowerupActive[3] = {true, true, true},startPowerup[3] = {true, true, true};

    void increasePlayerScore(int player);
    void increasePlayerLevel(int player, int linesCleared);
    void drawScores();
    void showShadow(int player);
    bool isShadowValid(int player, const Point shadowItems[3][8]);
    void showNextpiece(int player);
    void powerup(int player_no, int powerup_no);
    Uint32 startTime[3]= {0}, currentTime[3] = {0};


};



#endif // TETRIS_H_INCLUDED
