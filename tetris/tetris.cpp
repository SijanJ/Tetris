#include "tetris.h"
#include <iostream>
const int Tetris::figures[][4]=
{
    0,1,2,3,
    0,4,5,6,
    2,6,5,4,
    1,2,5,6,
    2,1,5,4,
    1,4,5,6,
    0,1,5,6

};

bool Tetris::init(const char* title)
{

    //Initialize everything
    if(SDL_Init(SDL_INIT_EVERYTHING)==0)
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
        window = SDL_CreateWindow("tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenW, ScreenH, SDL_WINDOW_SHOWN);
        if (window != NULL)
        {
            render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(render!=NULL)
            {
                SDL_SetRenderDrawColor(render,255, 255, 255, 255);
                int imgFlags = IMG_INIT_PNG;

                int initted = IMG_Init(imgFlags);
                if((initted & imgFlags) != imgFlags)
                {
                    std::cout<<"Failed to init required png support\n"<<"IMG_Init()Error: "<<IMG_GetError()<<std::endl;
                }
                if (TTF_Init() == -1)
                {
                    std::cerr << "SDL_ttf could not be initialized: " << TTF_GetError() << std::endl;
                    // Handle the error or return false.
                }

                SDL_Surface* loadSurf = IMG_Load("images/twobox.png");
                background = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("images/blocks.png");
                blocks = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);
                font = TTF_OpenFont("Font/Tetris.ttf", 24);
                if (!font)
                {
                    std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
                    // Handle the error or return.
                }

                nextTetrimino();

            }
        }
        else
            return false;
    }
    else
        return true;

    running = true;
    return true;
}
void Tetris::nextTetrimino()
{
    int n = rand() % 7;

    for (int i= 0; i<4; i++)
    {

        if(nextT==2)
        {
            items2[i].x = 16 + figures[n][i]% 4;
            items2[i].y = 1+int(figures[n][i]/4);
        }
        else
        {
            items[i].x = 4 + figures[n][i]% 4;
            items[i].y = int(figures[n][i]/4);
        }

    }
}

void Tetris::handleEvents()
{

    SDL_Event e;

    //Get the players key input
    while(SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            switch(e.key.keysym.sym)
            {
            case SDLK_UP:
                rotate2 = true;
                break;
            case SDLK_LEFT:
                dx2 = -1;
                break;
            case SDLK_RIGHT:
                dx2 = 1;
                break;
            case SDLK_DOWN:
                delay2 = 50;
                break;
            case SDLK_RSHIFT:
                hardDrop2();
                break;
            case SDLK_w:
                rotate = true;
                break;
            case SDLK_a:
                dx = -1;
                break;
            case SDLK_d:
                dx = 1;
                break;
            case SDLK_s:
                delay = 50;
                break;
            case SDLK_LSHIFT:
                hardDrop();
                break;
            }
        }
    }
    const Uint8* state =SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_DOWN])
        delay2=50;

    if(state[SDL_SCANCODE_S])
        delay=50;
}
void Tetris::setPosRect(SDL_Rect& rect, int x, int y, int w, int h)
{
    rect= {x,y,w,h};
}

void Tetris::moveRectPos(SDL_Rect& rect, int x, int y)
{
    rect.x+=x;
    rect.y+=y;
}

void Tetris::hardDrop()
{
    while(isvalid())
    {
        for(int i=0; i<4; i++)
        {
            items[i].y++;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        items[i].y--;
    }

    // Place the tetrimino in its final position
    for (int i = 0; i < 4; i++)
    {
        field[items[i].y][items[i].x] = color;
    }
    // Generate the next tetrimino for Player 1
    nextT = 1;
    color = 1 + rand() % 7;
    nextTetrimino();

    int linesCleared1 = clearLines();
    increasePlayer1Score(linesCleared1);
    if (tempDelay > 100)
    {
        increasePlayer1Level(linesCleared1);
    }

}

void Tetris::hardDrop2()
{
// Move the tetrimino down until it collides with another block or the bottom
    while (isvalid2())
    {
        for (int i = 0; i < 4; i++)
        {
            items2[i].y++;
        }
    }

    // Move the tetrimino back up to its last valid position
    for (int i = 0; i < 4; i++)
    {
        items2[i].y--;
    }

    // Place the tetrimino in its final position
    for (int i = 0; i < 4; i++)
    {
        field[items2[i].y][items2[i].x] = color2;
    }

    // Generate the next tetrimino for Player 2
    nextT = 2;
    color2 = 1 + rand() % 7;
    nextTetrimino();

    // Update the score and level for Player 2
    linesCleared2 = clearLines2(); // You need to implement the clearLines2 function
    increasePlayer2Score(linesCleared2);
    if (tempDelay2 > 100)
    {
        increasePlayer2Level(linesCleared2);
    }
}

bool Tetris::isvalid()
{

    for(int i=0; i<4; i++)
    {


        if(items[i].x < 1 || items[i].x >=11 || items[i].y >= Lines)
            return false;
        else if (field[items[i].y][items[i].x])
            return false;
    }
    return true;
}

bool Tetris::isvalid2()
{
    for(int i=0; i<4; i++)
    {
        if(items2[i].x < 13 || items2[i].x >=23 || items2[i].y >= Lines)
            return false;
        else if (field[items2[i].y][items2[i].x])
            return false;
    }
    return true;

}



void Tetris::gameplay()
{
    //backup
    for(int i=0; i<4; i++)
    {
        backup[i]=items[i];
        backup2[i]=items2[i];
    }


    //Move the tetriminos
    if(dx)
    {
        for(int i=0; i<4; i++)
        {
            items[i].x += dx;
        }
        if(!isvalid())
            for(int i=0; i<4; i++)
                items[i]=backup[i];
    }
    if(dx2)
    {

        for(int i=0; i<4; i++)
        {

            items2[i].x +=dx2;
        }


        if(!isvalid2())
            for(int i=0; i<4; i++)
                items2[i]=backup2[i];
    }

    //Rotate the tetriminos
    if(rotate)
    {
        Point p = items[2]; //center of rotation
        for(int i=0; i<4; i++)
        {
            int x= items[i].y - p.y;
            int y = items[i].x - p.x;
            items[i].x = p.x -x;
            items[i].y = p.y + y;
        }
        if(!isvalid())
            for(int i=0; i<4; i++)
                items[i]=backup[i];
    }
    if(rotate2)
    {
        Point p = items2[2]; //center of rotation
        for(int i=0; i<4; i++)
        {
            int x= items2[i].y - p.y;
            int y = items2[i].x - p.x;
            items2[i].x = p.x -x;
            items2[i].y = p.y + y;
        }
        if(!isvalid2())
            for(int i=0; i<4; i++)
                items2[i]=backup2[i];
    }

    nextT =1;

    //Ticks
    if(currentTime - startTime > delay)
    {

        for(int i=0; i<4; i++)
        {
            backup[i]=items[i];


        }
        for (int i=0; i<4 ; i++)
        {
            items[i].y++;

        }

        if(!isvalid())
        {
            for(int i=0; i<4; i++)
            {
                field[backup[i].y][backup[i].x]=color;
            }
            nextT=1;
            color = 1 + rand()%7;
            nextTetrimino();
        }


        startTime = currentTime;
    }

    if(currentTime2 - startTime2 > delay2)
    {

        for(int i=0; i<4; i++)
        {
            backup2[i]=items2[i];
        }
        for (int i=0; i<4 ; i++)
        {
            items2[i].y++;
        }


        if(!isvalid2())
        {
            for(int i=0; i<4; i++)
            {
                field[backup2[i].y][backup2[i].x]=color2;
            }
            nextT=2;
            color2 = 1 + rand()%7;
            nextTetrimino();

        }

        startTime2 = currentTime2;
    }

    //Check Lines


    linesCleared=0;

    linesCleared=clearLines();
    // Update Player 1's score based on the number of lines cleared.
    increasePlayer1Score(linesCleared);
    if (tempDelay > 100)
    {
        increasePlayer1Level(linesCleared);
    }


    // Check if any lines are completed and update the scores accordingly for Player 2.
    linesCleared2 = 0;

    linesCleared2 = clearLines2();
    // Update Player 2's score based on the number of lines cleared.
    increasePlayer2Score(linesCleared2);
    if (tempDelay2 > 100)
    {
        increasePlayer2Level(linesCleared2);
    }

    //Reset the Tetriminos and update speed of tetriminos according to level
    dx=0;
    rotate = false;
    delay=tempDelay;

    dx2=0;
    rotate2= false;
    delay2=tempDelay2;




    updateRender();
}



int Tetris::clearLines()
{
    linesCleared=0;
    for (int i = 0; i < Lines; i++)
    {

        bool lineComplete = true;
        for (int j = 1; j < 11; j++)
        {
            if (field[i][j] == 0)
            {
                lineComplete = false;
                break;
            }
        }

        if (lineComplete)
        {
            linesCleared++;
            // Clear the line and shift other lines down.
            for (int k = i; k > 0; k--)
            {
                for (int j = 1; j < 11; j++)
                {
                    field[k][j] = field[k - 1][j];
                }
            }
            // Clear the top line.
            for (int j = 1; j < 11; j++)
            {
                field[0][j] = 0;
            }
        }
    }
    return linesCleared;
}

int Tetris::clearLines2()
{
    for (int i = 2; i < Lines; i++)
    {
        bool lineComplete = true;
        for (int j = 13; j < 23; j++)
        {
            if (field[i][j] == 0)
            {
                lineComplete = false;
                break;
            }
        }

        if (lineComplete)
        {
            linesCleared2++;
            // Clear the line and shift other lines down.
            for (int k = i; k > 0; k--)
            {
                for (int j = 13; j < 23; j++)
                {
                    field[k][j] = field[k - 1][j];
                }
            }
            // Clear the top line.
            for (int j = 13; j < 23; j++)
            {
                field[0][j] = 0;
            }
        }
    }
    return linesCleared2;
}

void Tetris::increasePlayer1Score(int Cleared)
{
    // Increase score for each line cleared of player 1
    player1Score += Cleared * 100;
}

void Tetris::increasePlayer2Score(int Cleared)
{
    // Increase score for each line cleared of player 2
    player2Score += Cleared * 100;
}

void Tetris::increasePlayer1Level(int Cleared)
{

    // Increase speed for each line cleared of player 1
    tempDelay -= Cleared*50;

    // Update Player 1 Score only if the max level is not reached
    if(tempDelay>100)
    {
        player1Level+=Cleared;
    }
}

void Tetris::increasePlayer2Level(int Cleared)
{

    // Increase speed for each line cleared of player 2
    tempDelay2 -= Cleared*50;

    // Update Player 2 Score only if the max level is not reached
    if(tempDelay2>100)
    {
        player2Level+=Cleared;
    }
}


void Tetris::drawScores()
{


    // Render the score for Player 1
    std::string player1ScoreText = "Player 1 Score: " + std::to_string(player1Score);
    SDL_Surface* player1ScoreSurface = TTF_RenderText_Solid(font, player1ScoreText.c_str(), textColor);
    SDL_Texture* player1ScoreTexture = SDL_CreateTextureFromSurface(render, player1ScoreSurface);

    // Set the position of Player 1's score text
    player1ScoreRect.x = 10;
    player1ScoreRect.y = 10;
    player1ScoreRect.w = player1ScoreSurface->w;
    player1ScoreRect.h = player1ScoreSurface->h;

    // Render Player 1's score text on the screen
    SDL_RenderCopy(render, player1ScoreTexture, NULL, &player1ScoreRect);

    // Clean up resources after rendering Player 1's score
    SDL_FreeSurface(player1ScoreSurface);
    SDL_DestroyTexture(player1ScoreTexture);

    // Render the score for Player 2
    std::string player2ScoreText = "Player 2 Score: " + std::to_string(player2Score);
    SDL_Surface* player2ScoreSurface = TTF_RenderText_Solid(font, player2ScoreText.c_str(), textColor);
    SDL_Texture* player2ScoreTexture = SDL_CreateTextureFromSurface(render, player2ScoreSurface);

    // Set the position of Player 2's score text

    player2ScoreRect.x = 500;
    player2ScoreRect.y = 10;
    player2ScoreRect.w = player2ScoreSurface->w;
    player2ScoreRect.h = player2ScoreSurface->h;

    // Render Player 2's score text on the screen
    SDL_RenderCopy(render, player2ScoreTexture, NULL, &player2ScoreRect);

    // Clean up resources after rendering Player 2's score
    SDL_FreeSurface(player2ScoreSurface);
    SDL_DestroyTexture(player2ScoreTexture);

    //Render level for player 1
    std::string player1LevelText = "Player 1 Level: " + std::to_string(player1Level);
    SDL_Surface* player1LevelSurface = TTF_RenderText_Solid(font, player1LevelText.c_str(), textColor);
    SDL_Texture* player1LevelTexture = SDL_CreateTextureFromSurface(render, player1LevelSurface);

    // Set the position of player 1's Level Text

    player1LevelRect.x = 10;
    player1LevelRect.y = 50;
    player1LevelRect.w = player1LevelSurface->w;
    player1LevelRect.h = player1LevelSurface->h;

    // Render Player 1's score text on screen
    SDL_RenderCopy(render, player1LevelTexture, NULL, &player1LevelRect);

    // Clean up resources after rendering Player 1's level
    SDL_FreeSurface(player1LevelSurface);
    SDL_DestroyTexture(player1LevelTexture);

    //Render level for player 2
    std::string player2LevelText = "Player 2 Level: " + std::to_string(player2Level);
    SDL_Surface* player2LevelSurface = TTF_RenderText_Solid(font, player2LevelText.c_str(), textColor);
    SDL_Texture* player2LevelTexture = SDL_CreateTextureFromSurface(render, player2LevelSurface);

    // Set the position of player 2's Level Text

    player2LevelRect.x = 500;
    player2LevelRect.y = 50;
    player2LevelRect.w = player2LevelSurface->w;
    player2LevelRect.h = player2LevelSurface->h;

    // Render Player 2's score text on screen
    SDL_RenderCopy(render, player2LevelTexture, NULL, &player2LevelRect);

    // Clean up resources after rendering Player 2's level
    SDL_FreeSurface(player2LevelSurface);
    SDL_DestroyTexture(player2LevelTexture);
}

void Tetris::updateRender()
{
    SDL_RenderCopy(render, background, NULL, NULL);
    for(int i=0; i<Lines ; i++)
        for(int j=0; j < Cols; j++)
            if(field[i][j])
            {
                setPosRect(srcR, field[i][j] * BlockW);
                setPosRect(destR, j*BlockW, i*BlockH);
                SDL_RenderCopy(render, blocks, &srcR, &destR );
            }

    for(int i=0; i<4; i++)
    {
        setPosRect(srcR, color *  BlockW);
        setPosRect(destR, items[i].x* BlockW, items[i].y*BlockH);

        SDL_RenderCopy(render,blocks, &srcR, &destR);
    }



    field[0][0]=0;
    for(int i=0; i<Lines ; i++)
        for(int j=0; j < Cols; j++)
            if(field[i][j])
            {
                setPosRect(srcR, field[i][j] * BlockW);
                setPosRect(destR, j*BlockW, i*BlockH);
                SDL_RenderCopy(render, blocks, &srcR, &destR );
            }

    for(int j=0; j<4; j++)
    {
        setPosRect(srcR, color2 *  BlockW);
        setPosRect(destR, items2[j].x* BlockH, items2[j].y*BlockH);
        SDL_RenderCopy(render,blocks, &srcR, &destR);
    }


    drawScores();


    SDL_RenderPresent(render);

}

void Tetris::clean()
{
    SDL_DestroyTexture(blocks);
    SDL_DestroyTexture(background);

    // Close the font after use
    TTF_CloseFont(font);


    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    IMG_Quit();
    TTF_Quit();
}
