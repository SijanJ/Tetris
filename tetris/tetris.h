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
        currentTime[1] =t;
        currentTime[2]=t;
    }

    bool isRunning()
    {
        return running;
    }

    bool isvalid(int player);


    bool init(const char* title);
    void nextTetrimino(int player_no, bool differentShape = false);
    void handleEvents();
    void setPosRect(SDL_Rect& rect, int x=0, int y=0, int w=BlockW, int h = BlockH);
    void moveRectPos(SDL_Rect& rect,int x, int y);
    void gameplay();
    void updateRender();
    void clean();
    void hardDrop(int player);
    int clearLines(int player);
    int total_player;
    int player[3];

private:
    enum {ScreenW = 864, ScreenH = 800 };
    enum {BlockW = 36, BlockH = 36};
    enum {Lines = 20, Cols = 24};
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    SDL_Texture* background = NULL, * blocks = NULL, *powerup_img = NULL;
    SDL_Texture* player1LevelTexture=NULL, * player2LevelTexture=NULL;
    SDL_Surface* player1LevelSurface=NULL, *player2LevelSurface=NULL;
    SDL_Rect srcR = {0,0, BlockW, BlockH}, destR= {0,0, BlockW, BlockH};
    SDL_Rect player1ScoreRect;
    SDL_Rect player2ScoreRect;
    SDL_Rect player1LevelRect;
    SDL_Rect player2LevelRect;
    TTF_Font *font ;
    SDL_Color textColor = {0,0,0};
    bool running = false;

    int field[Lines][Cols]= {0};

    static const int figures[][8];
    struct Point
    {
        int x,y;
    } items[3][8],backup[3][8],shadowItems[3][8];

    int color[3] = {1,3,4};
    int dx[3] ={0};
    int n[3];
    bool rotate[3] ={false};
    unsigned int delay[3] = {1000,1000,1000};

    unsigned int tempDelay[3]={1000,1000,1000};

    int playerScore[3] = {0,0,0}; // Score for player 1
    int linesCleared[3] = {0};
    int playerLevel[3]={0};
    std::string player2LevelText;
    std::string player1LevelText;

    int powerup_x[3],powerup_y[3];
    int powerupStartTime[3];
    bool isPowerupActive[3] = {true, true, true},startPowerup[3] = {true, true, true};

    void increasePlayerScore(int player, int linesCleared);
    void increasePlayerLevel(int player, int linesCleared);
    void drawScores();
    void showShadow(int player);
    bool isShadowValid(int player, const Point shadowItems[3][8]);
    void powerup(int player_no, int powerup_no);
    Uint32 startTime[3]={0}, currentTime[3] ={0};

};



#endif // TETRIS_H_INCLUDED
