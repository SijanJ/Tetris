#include "tetris.h"
#include <iostream>
#include "powerup.h"
#include "menu.h"

const int Tetris::figures[][8]=
{
    4,5,6,7,4,5,6,7,
    0,4,5,6,0,4,5,6,
    2,6,5,4,2,6,5,4,
    1,2,5,6,1,2,5,6,
    2,1,5,4,2,1,5,4,
    1,4,5,6,1,4,5,6,
    0,1,5,6,0,1,5,6,
    0,1,2,4,6,8,9,10,
    1,4,5,6,9,1,4,5,

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

                font = TTF_OpenFont("Font/Gagalin.ttf", 24);
                if (!font)
                {
                    std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
                    // Handle the error or return.
                }

                loadSurf = IMG_Load("images/powerup.png");
                powerup_img = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);


                nextTetrimino(1);
                nextTetrimino(2);


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

int Tetris::showPauseMenu()
{
    Menu pauseMenu(render);
    paused = true;
    int selectedOption = pauseMenu.showMenu(paused,isGameOver[0]||isGameOver[1]);
    paused = false;

    return selectedOption;
}

void Tetris::nextTetrimino(int player, bool differentShape)
{
    if(differentShape == true)
    {
        n[player]= 7 + rand()%2;
        color[player] = 8;
    }
    else
    {
        n[player] = rand() % 7;
        color[player] = 1 + n[player];
    }



    for (int i= 0; i<8; i++)
    {

        if(player==2)
        {
            items[2][i].x = 16 + figures[n[2]][i]% 4;
            items[2][i].y = int(figures[n[2]][i]/4);

        }
        else
        {

            items[1][i].x = 4 + figures[n[1]][i]% 4;
            items[1][i].y = int(figures[n[1]][i]/4);

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
                rotate[2] = true;
                break;
            case SDLK_LEFT:
                dx[2] = -1;
                break;
            case SDLK_RIGHT:
                dx[2] = 1;
                break;
            case SDLK_DOWN:
                delay[2] = 50;
                break;
            case SDLK_RSHIFT:
                hardDrop(2);
                break;
            case SDLK_w:
                rotate[1] = true;
                break;
            case SDLK_a:
                dx[1] = -1;
                break;
            case SDLK_d:
                dx[1] = 1;
                break;
            case SDLK_s:
                delay[1] = 50;
                break;
            case SDLK_LSHIFT:
                hardDrop(1);
                break;
            case SDLK_ESCAPE:
                paused = true;
            }
        }
    }
    const Uint8* state =SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_DOWN])
        delay[2]=50;

    if(state[SDL_SCANCODE_S])
        delay[1]=50;
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

void Tetris::hardDrop(int player)
{
    while(isvalid(player))
    {
        for(int i=0; i<8; i++)
        {
            items[player][i].y++;
        }
    }
    for (int i = 0; i < 8; i++)
    {
        items[player][i].y--;
    }

    // Place the tetrimino in its final position
    for (int i = 0; i < 8; i++)
    {
        field[items[player][i].y][items[player][i].x] = color[player];
    }
    // Generate the next tetrimino for Player 1

    color[player] = 1 + rand() % 7;
    nextTetrimino(player);
    int linesCleared[3];
    linesCleared[player] = clearLines(player);
    increasePlayerScore(player, linesCleared[player]);
    if (tempDelay[player] > 100)
    {
        increasePlayerLevel(player, linesCleared[player]);
    }

    showShadow(player);


}


bool Tetris::isShadowValid(int player, const Point shadowItems[3][8])
{
    for (int i = 0; i < 8; i++)
    {
        if(player==1)
        {
            if (shadowItems[1][i].x < 1 || shadowItems[1][i].x >= 11 || shadowItems[1][i].y >= Lines)
            {
                return false;
            }
            else if (field[shadowItems[1][i].y][shadowItems[1][i].x])
            {
                return false;
            }
        }

        else
        {
            if (shadowItems[2][i].x < 13 || shadowItems[2][i].x >= 23 || shadowItems[2][i].y >= Lines)
            {
                return false;
            }
            else if (field[shadowItems[2][i].y][shadowItems[2][i].x])
            {
                return false;
            }
        }

    }
    return true;
}

bool Tetris::isvalid(int player)
{
    if(player==1)
    {
        for(int i=0; i<8; i++)
        {
            if(items[1][i].x < 1 || items[1][i].x >=11 || items[1][i].y >= Lines)
                return false;

            else if (field[items[1][i].y][items[1][i].x])
                return false;
        }
        return true;
    }
    else
    {
        for(int i=0; i<8; i++)
        {
            if(items[2][i].x < 13 || items[2][i].x >=23 || items[2][i].y >= Lines)
                return false;

            else if (field[items[2][i].y][items[2][i].x])
                return false;
        }
        return true;
    }

}



void Tetris::showShadow(int player)
{
// Calculate the shadow positions
    for (int i = 0; i < 8; i++)
    {
        shadowItems[player][i] = items[player][i];
    }
    while (isShadowValid(player, shadowItems))
    {
        for (int i = 0; i < 8; i++)
        {
            shadowItems[player][i].y++;
        }
    }

    if(shadowItems[1][1].y == 1)
    {
        isGameOver[0] = true;
        SDL_Delay(500);
    }
    if(shadowItems[2][1].y == 1)
    {
        isGameOver[1] = true;
        SDL_Delay(500);
    }
    for (int i = 0; i < 8; i++)
    {
        shadowItems[player][i].y--;
    }
}

int Tetris::gameOver()
{

    int selectedOption;
    Menu game(render);
    if(isGameOver[0])
    {
        if(total_player==2)
            selectedOption = game.showMenu(paused,isGameOver[0],playerScore[0],playerScore[2],total_player,2);
        else
            selectedOption = game.showMenu(paused,isGameOver[0],playerScore[1]);
        isGameOver[0]=false;
    }
    else
    {
        selectedOption = game.showMenu(paused,isGameOver[1], playerScore[1],playerScore[2],total_player,1);
        isGameOver[1]=false;
    }


    return selectedOption;
}
void Tetris::pauseMenuOptions(int option)
{
    Menu mainMenu(render);
    switch (option)
    {
    case 0:
        break;
    case 1:
        resetGame();
        break;
    case 2:
        total_player=mainMenu.showMenu(paused, isGameOver[0]||isGameOver[1])+1;
        if(total_player==1||total_player==2)
        {
            resetGame();
        }
        else
        {
            running=false;
            exit(0);
        }
        break;
    case 3:
        running=false;
        exit(0);
        break;
    default:
        break;
    }
}

void Tetris::gameOverMenuOptions(int option)
{
    Menu mainMenu(render);
    switch (option)
    {
    case 0:
        resetGame();
        total_player=1;
        break;
    case 1:
        resetGame();
        total_player=2;
        break;
    case 2:
        total_player=mainMenu.showMenu(paused, isGameOver[0]||isGameOver[1])+1;
        if(total_player==1||total_player==2)
        {
            resetGame();
        }
        else
        {
            running=false;
            exit(0);
        }
        break;
    case 3:
        running=false;
        exit(0);
        break;
    default:
        break;
    }
}

void Tetris::resetGame()
{
    // Reset game-related variables, fields, scores, levels, etc.
    // For example:
    for (int player = 1; player <= total_player; player++)
    {
        playerScore[player] = 0;
        playerLevel[player] = 0;
        // Reset other relevant variables
        nextTetrimino(player, false); // Reset the next tetriminos
        // Clear the field and initialize it again
        clearField(player);
    }
}

void Tetris::clearField(int player)
{
    for (int i = 0; i < Lines; i++)
    {
        for (int j = 0; j < Cols; j++)
        {
            field[i][j] = 0;
        }
    }
}

void Tetris::gameplay()
{
    SDL_RenderCopy(render, background, NULL, NULL);
    if(paused && (!isGameOver[0] || !isGameOver[1]))
    {
        int selectedOption = showPauseMenu();
        pauseMenuOptions(selectedOption);
    }
    if(isGameOver[0]||isGameOver[1])
    {
        int selectedOption = gameOver();
        gameOverMenuOptions(selectedOption);
    }
    else
    {
        for(int player=1; player<=total_player; player++)
        {
            showShadow(player);

            //backup
            for(int i=0; i<8; i++)
                backup[player][i]=items[player][i];

            //Move the tetriminos
            if(dx[player])
            {
                for(int i=0; i<8; i++)
                {
                    items[player][i].x += dx[player];
                }
                if(!isvalid(player))
                    for(int i=0; i<8; i++)
                        items[player][i]=backup[player][i];

            }


            //Rotate the tetriminos
            if(rotate[player] && n[player]!=3 && n[player]!=7 && n[player]!=8)
            {
                Point p = items[player][2]; //center of rotation
                for(int i=0; i<8; i++)
                {
                    int x= items[player][i].y - p.y;
                    int y = items[player][i].x - p.x;
                    items[player][i].x = p.x -x;
                    items[player][i].y = p.y + y;
                }
                if(!isvalid(player))
                    for(int i=0; i<8; i++)
                        items[player][i]=backup[player][i];

            }

            //Ticks
            if(currentTime[player] - startTime[player] > delay[player])
            {

                for(int i=0; i<8; i++)
                    backup[player][i]=items[player][i];

                for (int i=0; i<8 ; i++)
                    items[player][i].y++;

                if(!isvalid(player))
                {
                    for(int i=0; i<8; i++)
                    {
                        field[backup[player][i].y][backup[player][i].x]=color[player];
                    }

                    //color[player] = 1 + rand()%7;

                    nextTetrimino(player);
                }


                startTime[player] = currentTime[player];
            }


            //Check Lines
            linesCleared[player]=0;
            linesCleared[player]=clearLines(player);

            // Update Player 1's score based on the number of lines cleared.
            increasePlayerScore(player, linesCleared[player]);
            if (tempDelay[player] > 100)
            {
                increasePlayerLevel(player, linesCleared[player]);
            }

            //Reset the Tetriminos and update speed of tetriminos according to level
            dx[player]=0;
            rotate[player] = false;
            delay[player]=tempDelay[player];

        }
        field[0][0]=0;
    }
}


int Tetris::clearLines(int player)
{
    linesCleared[player]=0;

    if(player==1)
    {
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

                linesCleared[player]++;
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
        return linesCleared[player];
    }

    else
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
                linesCleared[player]++;
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
        return linesCleared[player];
    }

}


void Tetris::increasePlayerScore(int player, int Cleared)
{
    // Increase score for each line cleared of player
    playerScore[player] += Cleared * 100;
}


void Tetris::increasePlayerLevel(int player, int Cleared)
{
    // Increase speed for each line cleared of player
    tempDelay[player] -= Cleared*50;
    // Update Player Score only if the max level is not reached
    if(tempDelay[player]>100)
    {
        playerLevel[player]+=Cleared;
    }
    if(playerLevel[player]%2 == 0 && isPowerupActive[player])  //powerup enable at even level no.
    {
        int powerup_no;
        if(startPowerup[player] )  // runs only one time per powerup
        {
            powerup_no = 1; //randomize powerup no(only 1 for now)

            if(player ==1 )
            {
                powerup_x[1] = 1+ rand()% 10; //randomize power up spawn point
                powerup_y[1] = 4+rand() % 10;

                if(field[powerup_x[1]][powerup_y[1]]!=0) //check vacant field
                {
                    powerup_x[1] = 1+ rand()% 10;
                    powerup_y[1] = 4+rand() % 10;
                }
            }
            else if(player ==2)
            {
                powerup_x[2] = 13+ rand()% 10;
                powerup_y[2] = 4+rand() % 10;

                if(field[powerup_x[2]][powerup_y[2]]!=0)
                {
                    powerup_x[2] = 13+ rand()% 10;
                    powerup_y[2] = 4+rand() % 10;
                }
            }
            powerupStartTime[player] = currentTime[player];
            startPowerup[player] =false;

        }
        powerup(player, powerup_no);

    }
    if(playerLevel[player]%2 ==1)   //reset for next level powerup
    {
        isPowerupActive[player] = true;
        startPowerup[player] = true;


    }
}
void Tetris::renderText(std::string text, int x, int y)
{

    SDL_Surface* playerSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(render, playerSurface);

    //Set the position for text
    playerRect.x = x;
    playerRect.y = y;
    playerRect.w = playerSurface ->w;
    playerRect.h = playerSurface ->h;

    //Render Text On Screen
    SDL_RenderCopy(render, playerTexture, NULL, &playerRect);

    //Clean Up Resources after rendering text
    SDL_FreeSurface(playerSurface);
    SDL_DestroyTexture(playerTexture);
}


void Tetris::drawScores()
{
    // Render the score for Player 1
    std::string player1ScoreText = "Player 1 Score: " + std::to_string(playerScore[1]);
    renderText(player1ScoreText,10,10);

    //Render level for player 1
    std::string player1LevelText = "Player 1 Level: " + std::to_string(playerLevel[1]);
    renderText(player1LevelText,10,50);

    if(total_player==2)
    {
        // Render the score for Player 2
        std::string player2ScoreText = "Player 2 Score: " + std::to_string(playerScore[2]);
        renderText(player2ScoreText,500,10);

        //Render level for player 2
        std::string player2LevelText = "Player 2 Level: " + std::to_string(playerLevel[2]);
        renderText(player2LevelText,500,50);
    }

}

void Tetris::updateRender()
{
    for(int player=1; player<=total_player; player++)
    {
        for (int j = 0; j < 8; j++)
        {
            setPosRect(srcR, color[player] * BlockW);
            setPosRect(destR, shadowItems[player][j].x * BlockW, shadowItems[player][j].y * BlockH);


            SDL_SetTextureAlphaMod(blocks, 25); //
            SDL_RenderCopy(render, blocks, &srcR, &destR);

            // Restore full alpha for subsequent rendering
            SDL_SetTextureAlphaMod(blocks, 255); // Set alpha to 255 (fully opaque)
        }
        for(int i=0; i<Lines ; i++)
            for(int j=0; j < Cols; j++)
                if(field[i][j])
                {
                    setPosRect(srcR, field[i][j] * BlockW);
                    setPosRect(destR, j*BlockW, i*BlockH);
                    SDL_RenderCopy(render, blocks, &srcR, &destR );
                }

        for(int i=0; i<8; i++)
        {
            setPosRect(srcR, color[player] *  BlockW);
            setPosRect(destR, items[player][i].x* BlockW, items[player][i].y*BlockH);

            SDL_RenderCopy(render,blocks, &srcR, &destR);
        }

        drawScores();

    }
    SDL_RenderPresent(render);

}

void Tetris::clean()
{
    SDL_DestroyTexture(blocks);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(powerup_img);

    // Close the font after use
    TTF_CloseFont(font);


    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
