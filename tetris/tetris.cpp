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
    1,2,5,8,9,1,2,5,

};

bool Tetris::init(const char* title)
{

    //Initialize everything

    if(initialize()==0)
    {
        window = createWindow("tetris", WindowPosUndefined, WindowPosUndefined, ScreenW, ScreenH, WINDOWSHOWN);
        if (window!= NULL)
        {
            render = createRenderer(window, -1, RendererAccelerated);
            if(render!=NULL)
            {
                setRenderDrawColor(render,255, 255, 255, 255);
                int imgFlags = InitPNG;

                int initted = initImage(imgFlags);
                if((initted & imgFlags) != imgFlags)
                {
                    std::cout<<"Failed to init required png support\n"<<std::endl;
                }
                if (TTFInit() == -1)
                {
                    std::cerr << "SDL_ttf could not be initialized: "  << std::endl;
                }
                 initMusic();
                Surface* loadSurf = loadImage("images/Background.png");
                background = createTextureFromSurface(render, loadSurf);
                freeSurface(loadSurf);
                loadSurf = loadImage("images/blocks.png");
                blocks = createTextureFromSurface(render, loadSurf);
                freeSurface(loadSurf);
                loadSurf = loadImage("images/small_blocks.png");
                small_blocks = createTextureFromSurface(render, loadSurf);
                freeSurface(loadSurf);

                font = openFont("Font/Gagalin.ttf", 50);
                if (!font)
                {
                    std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
                }

                loadSurf = loadImage("images/powerup.png");
                powerup_img = createTextureFromSurface(render, loadSurf);
                freeSurface(loadSurf);

                m1.playMusic("sound/Tetris.mp3");
                 pauseMusic();
                 m1.load_AllSound();

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
    int selectedOption = pauseMenu.showMenu(paused,isGameOver[1]||isGameOver[2]);
    paused = false;
    return selectedOption;
}

void Tetris::nextTetrimino(int player, bool differentShape)
{

    if(differentShape == true)
    {
        n[player]= 7 + rand()%3;
        color[player] = 8;
    }
    else
    {
        n[player] = nextT[player];
        color[player] = 1 + n[player];
        nextT[player]=rand()%7;
    }



    for (int i= 0; i<8; i++)
    {

        if(player==2)
        {
            items[2][i].x = 37 + figures[n[2]][i]% 4;
            items[2][i].y = 2+int(figures[n[2]][i]/4);

        }
        else
        {

            items[1][i].x = 8 + figures[n[1]][i]% 4;
            items[1][i].y =2+ int(figures[n[1]][i]/4);

        }

    }
}

void Tetris::showNextpiece(int player)
{

for(int i=0; i<4; i++)
{
    if(player==1){
    nextItem[1][i].x = figures[nextT[1]][i] % 4;
    nextItem[1][i].y = int(figures[nextT[1]][i]/4);
    }
    else if(player==2){
        nextItem[2][i].x = figures[nextT[2]][i] % 4;
    nextItem[2][i].y =int(figures[nextT[2]][i]/4);
    }
}

}

void Tetris::handleEvents()
{

    Event e;

    //Get the players key input
    while(pollEvent(e))
    {
        switch (e.type)
        {
        case QUIT:
            running = false;
            break;
        case KEY_DOWN:

                int x;
                if(total_player==1)
                    x = 1;
                else
                   x=2;
            switch(e.key.keysym.sym)
            {
            case UP:
                rotate[x] = true;
                m1.playSound(0);
                break;
            case LEFT:
                dx[x] = -1;
                m1.playSound(0);
                break;
            case RIGHT:
                dx[x] = 1;
                m1.playSound(0);
                break;
            case DOWN:
                delay[x] = 50;
                m1.playSound(0);
                break;
            case RSHIFT:
                hardDrop(x);
                m1.playSound(1);
                break;
            case W:
                rotate[1] = true;
                m1.playSound(0);
                break;
            case A:
                dx[1] = -1;
                m1.playSound(0);
                break;
            case D:
                dx[1] = 1;
                m1.playSound(0);
                break;
            case S:
                delay[1] = 50;
                m1.playSound(0);
                break;
            case LSHIFT:
                hardDrop(1);
                m1.playSound(1);
                break;
            case ESC:
                pauseMusic();
                paused = true;
                break;
            case M:
                m1.toggleVolume();
                break;
            }
        }
    }
    const Uint8* state =getKeyboardState(NULL);
    if(state[SCANCODE_DOWN])
        delay[2]=50;

    if(state[SCANCODE_S])
        delay[1]=50;
}
void Tetris::setPosRect(Rect& rect, int x, int y, int w, int h)
{
    rect= {x,y,w,h};
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


}


bool Tetris::isShadowValid(int player, const Point shadowItems[3][8])
{
    for (int i = 0; i < 8; i++)
    {
        if(player==1)
        {
            if (shadowItems[1][i].x < 5 || shadowItems[1][i].x >= 15 || shadowItems[1][i].y >= Lines)
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
            if (shadowItems[2][i].x < 33 || shadowItems[2][i].x >= 43 || shadowItems[2][i].y >= Lines)
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
            if(items[1][i].x < 5 || items[1][i].x >=15 || items[1][i].y >= Lines){

                m1.playSound(3);
                return false;
            }

            else if (field[items[1][i].y][items[1][i].x])
            {
                m1.playSound(3);
              return false;
            }
        }
        return true;
    }
    else
    {
        for(int i=0; i<8; i++)
        {
            if(items[2][i].x < 33 || items[2][i].x >=43 || items[2][i].y >= Lines){
                    m1.playSound(3);
                return false;
            }

            else if (field[items[2][i].y][items[2][i].x]){
                    m1.playSound(3);
                return false;
            }
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

    if(shadowItems[1][1].y == 3)
    {
        isGameOver[1] = true;
        SDL_Delay(500);
    }
    if(shadowItems[2][1].y == 3)
    {
        isGameOver[2] = true;
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
    if(isGameOver[1])
    {
        if(total_player==2)
            selectedOption = game.showMenu(paused,isGameOver[1],playerScore[1],playerScore[2],total_player,2);
        else
            selectedOption = game.showMenu(paused,isGameOver[1],playerScore[1]);
        isGameOver[1]=false;
        resetGame();
    }
    if(isGameOver[2])
    {
        selectedOption = game.showMenu(paused,isGameOver[2], playerScore[1],playerScore[2],total_player,1);
        isGameOver[2]=false;
        resetGame();
    }

    //resetGame();
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
        total_player=mainMenu.showMenu(paused, isGameOver[1]||isGameOver[2])+1;
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
        //resetGame();
        total_player=mainMenu.showMenu(paused, isGameOver[1]||isGameOver[2])+1;
       if(total_player==1||total_player==2)
        {
            resetGame();
        }
        else
       {
           running=false;
            exit(0);
            break;
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
    for (int player = 1; player <= 2; player++)
    {
        playerScore[player] = 0;
        playerLevel[player] = 1;
        isGameOver[player]=false;
        rewindMusic();
        linesCleared[player]=0;
        nextTetrimino(player, false); // Reset the next tetriminos
        // Clear the field and initialize it again
        clearField(player);
        isPowerupActive[player] = false;
        tempDelay[player]=1000;
        pDelay[player]= 0;
        startPowerup[player] = true;
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
    renderCopy(render, background, NULL, NULL);
    if(paused && (!isGameOver[1] || !isGameOver[2]))
    {
        int selectedOption = showPauseMenu();
        pauseMenuOptions(selectedOption);
    }
    if(isGameOver[1]||isGameOver[2])
    {
        int selectedOption = gameOver();
        gameOverMenuOptions(selectedOption);
    }
    else
    {
        for(int player=1; player<=total_player; player++)
        {
            showShadow(player);
            showNextpiece(player);

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

            if(rotate[player] && n[player]!=3 && n[player]!=7 && n[player]!=8 && n[player]!=9)
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
                {
                    for(int i=0; i<8; i++)
                        items[player][i]=backup[player][i];
                }
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
                    m1.playSound(1);
                    nextTetrimino(player);
                }


                startTime[player] = currentTime[player];
            }


            //Check Lines
           linesCleared[player]=clearLines(player);

            // Update Player 1's score based on the number of lines cleared.
            if (tempDelay[player] > 100)
            {
                increasePlayerLevel(player, linesCleared[player]);
               if(linesCleared[player]/4>=1)
                   linesCleared[player]-=4;
           }

            //Reset the Tetriminos and update speed of tetriminos according to level
            dx[player]=0;
            rotate[player] = false;
            delay[player]=tempDelay[player] - pDelay[player];

        }
        field[0][0]=0;
    }
}


int Tetris::clearLines(int player)
{
   // int linesCleared[player]={0};

    if(player==1)
    {
        for (int i = 0; i < Lines; i++)
        {

            bool lineComplete = true;
            for (int j = 5; j < 15; j++)
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
                m1.playSound(2);
                // Clear the line and shift other lines down.
                for (int k = i; k > 0; k--)
                {
                    for (int j = 5; j < 15; j++)
                    {
                        field[k][j] = field[k - 1][j];
                    }
                }
                // Clear the top line.
                for (int j = 5; j < 15; j++)
                {
                    field[0][j] = 0;
                }
              increasePlayerScore(1);
            }

        }
        return linesCleared[player];
    }

    else
    {
        for (int i = 2; i < Lines; i++)
        {
            bool lineComplete = true;
            for (int j = 33; j < 43; j++)
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
                 m1.playSound(2);
                // Clear the line and shift other lines down.
                for (int k = i; k > 0; k--)
                {
                    for (int j = 33; j < 43; j++)
                    {
                        field[k][j] = field[k - 1][j];
                    }
                }
                // Clear the top line.
                for (int j = 33; j < 43; j++)
                {
                    field[0][j] = 0;
                }
                increasePlayerScore(2);
            }
        }
        return linesCleared[player];
    }

}


void Tetris::increasePlayerScore(int player)
{
    // Increase score for each line cleared of player
    playerScore[player] +=  100;
}


void Tetris::increasePlayerLevel(int player, int Cleared)
{
    // Update Player Score only if the max level is not reached
    if(Cleared/4 >=1)
    {
        playerLevel[player]++;
        tempDelay[player]-=50;
        if(tempDelay[player]<100)
        tempDelay[player]=100;
        isPowerupActive[player]=true;
        startPowerup[player]= true;

    }
     if(playerLevel[player]>1 && isPowerupActive[player])  //powerup enable at even level no.
    {
        //int powerup_no[3];
        if(startPowerup[player] )  // runs only one time per powerup
        {


            if(player ==1 )
            {
                powerup_x[1] = 5+ rand()% 10; //randomize power up spawn point
                powerup_y[1] = 7+rand() % 10;

                while(field[powerup_x[1]][powerup_y[1]]!=0) //check vacant field
                {
                    powerup_x[1] = 5+ rand()% 10;
                    powerup_y[1] = 7+rand() % 10;
                }
            }
            else if(player ==2)
            {
                powerup_x[2] = 33+ rand()% 10;
                powerup_y[2] = 7+rand() % 10;

                while(field[powerup_x[2]][powerup_y[2]]!=0)
                {
                    powerup_x[2] = 33+ rand()% 10;
                    powerup_y[2] = 7+rand() % 10;
                }
            }
            powerupStartTime[player] = currentTime[player];
            startPowerup[player] =false;

        }

        powerup(player, 1+rand()%3);

    }

}
void Tetris::renderText(std::string text, int x, int y)
{

    Surface* playerSurface = renderTextSolid(font, text.c_str(), textColor);
    Texture* playerTexture = createTextureFromSurface(render, playerSurface);

    //Set the position for text
    playerRect.x = x;
    playerRect.y = y;
    playerRect.w = playerSurface ->w;
    playerRect.h = playerSurface ->h;

    //Render Text On Screen
    renderCopy(render, playerTexture, NULL, &playerRect);

    //Clean Up Resources after rendering text
    freeSurface(playerSurface);
    destroyTexture(playerTexture);
}


void Tetris::drawScores()
{
    // Render the score for Player 1
    std::string player1ScoreText =  std::to_string(playerScore[1]);
    renderText(player1ScoreText,775,140);

    //Render level for player 1
    std::string player1LevelText =  std::to_string(playerLevel[1]);
    renderText(player1LevelText,775,375);
    if(total_player==1){
            std::string offlineText = "PLAYER 2 OFFLINE";

            renderText(offlineText,1330, 500 );
    }

    if(total_player==2)
    {
        // Render the score for Player 2
        std::string player2ScoreText =  std::to_string(playerScore[2]);
        renderText(player2ScoreText,1080,140);

        //Render level for player 2
        std::string player2LevelText = std::to_string(playerLevel[2]);
        renderText(player2LevelText,1080,375);
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


            setTextureAlphaMod(blocks, 25); //
            renderCopy(render, blocks, &srcR, &destR);

            // Restore full alpha for subsequent rendering
            setTextureAlphaMod(blocks, 255); // Set alpha to 255 (fully opaque)
        }
        for(int i=0; i<Lines ; i++)
            for(int j=0; j < Cols; j++)
                if(field[i][j])
                {
                    setPosRect(srcR, field[i][j] * BlockW);
                    setPosRect(destR, j*BlockW, i*BlockH);
                    renderCopy(render, blocks, &srcR, &destR );
                }

        for(int i=0; i<8; i++)
        {
            setPosRect(srcR, color[player] *  BlockW);
            setPosRect(destR, items[player][i].x* BlockW, items[player][i].y*BlockH);

            renderCopy(render,blocks, &srcR, &destR);
        }

       for(int i=0; i<4; i++)
        {
            setPosRect(srcR, (1 + nextT[player])*30, 0, 30,30 );
            if(player==1)
            setPosRect(destR, 30 + nextItem[player][i].x*30, 160 + nextItem[player][i].y*30, 30,30);
            if(player==2)
                setPosRect(destR, 1790 + nextItem[player][i].x*30, 160 + nextItem[player][i].y*30, 30,30);
            SDL_RenderCopy(render, small_blocks, &srcR, &destR);
        }
        drawScores();

    }
    renderPresent(render);

}

void Tetris::clean()
{
    resetGame();
    destroyTexture(blocks);
    destroyTexture(background);
    destroyTexture(powerup_img);

    // Close the font after use
    closeFont(font);


    destroyRenderer(render);
    destroyWindow(window);

    imageQuit();
    TTFQuit();
    m1.quitMixer();

    SDLQuit();
}
