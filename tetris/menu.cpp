#include "menu.h"
#include<iostream>

int Menu::showMenu(bool p, bool g, int s1,int s2, int pl,int w)
{
    paused = p;
    score1=s1;
    score2=s2;
    player=pl;
    winner=w;
    gameOver = g;
    bool menuRunning = true;
    SDL_Event e;
    if(!paused && !gameOver)
    {
        tempItems=menuItems;
    }
    else if(paused && !gameOver)
    {
        tempItems=pauseItems;
    }
    else if(gameOver)
    {
        tempItems=gameOverItems;
    }

    while(menuRunning)
    {
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
            case SDL_QUIT:
                exit(0);
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym)
                {
                case SDLK_UP:
                    selectedItemIndex=(selectedItemIndex-1+tempItems.size())%tempItems.size();
                    break;
                case SDLK_DOWN:
                    selectedItemIndex=(selectedItemIndex+1)%tempItems.size();
                    break;
                case SDLK_RETURN:
                    return selectedItemIndex;
                default:
                    break;
                }
            case SDL_MOUSEMOTION:
                mousePosition.x = e.motion.x;
                mousePosition.y = e.motion.y;
                // Check if the mouse is over any of the menu items
                for (size_t i = 0; i < textRects.size(); i++)
                {
                    if (isPointInRect(mousePosition,textRects[i]))
                    {
                        selectedItemIndex = i;
                        break;
                    }
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                switch (e.button.button)
                {
                case SDL_BUTTON_LEFT:
                    //Check if mouse is over menu item
                    for(size_t i =0; i<tempItems.size(); i++)
                    {
                        if(isPointInRect(mousePosition, textRects[i]))
                        {
                            selectedItemIndex = i;
                            return selectedItemIndex;
                        }
                    }

                }
            }
        }

        renderMenu();

    }

    return -1;

}

void Menu::renderMenu()
{
    font = TTF_OpenFont("Font/Gagalin.ttf", 36);
    if(!font)
    {
        std::cout<<TTF_GetError();
        return;
    }
    SDL_SetRenderDrawColor(render, 0,0,0,255);
    SDL_RenderClear(render);
    textRects.clear();
    if(!paused && !gameOver)
    {
        backSurface = IMG_Load("images/");
        backTexture = SDL_CreateTextureFromSurface(render, backSurface);

        SDL_RenderCopy(render, backTexture, NULL, NULL);

        SDL_FreeSurface(backSurface);
        SDL_DestroyTexture(backTexture);

        logoSurface = IMG_Load("images/logo.png");
        logoTexture = SDL_CreateTextureFromSurface(render, logoSurface);

        logoRect.x =(ScreenW-logoSurface->w)/2;
        logoRect.y = 50;
        logoRect.w = logoSurface->w;
        logoRect.h = logoSurface->h;

        SDL_RenderCopy(render, logoTexture,NULL, &logoRect);

        SDL_FreeSurface(logoSurface);
        SDL_DestroyTexture(logoTexture);

    }

    if(gameOver)
    {
        scoreFont = TTF_OpenFont("Font/Gagalin.ttf",96);
        std::string gameOverText;
        if(winner==1)
        {
            gameOverText="PLAYER 1 WINS";
        }
        else if(winner==2)
        {
            gameOverText="PLAYER 2 WINS";
        }
        else
        {
            gameOverText="GAME OVER";
        }


        logoSurface = TTF_RenderText_Solid(scoreFont,gameOverText.c_str(), textColor);
        logoTexture = SDL_CreateTextureFromSurface(render, logoSurface);

        logoRect.x =(ScreenW-logoSurface->w)/2;
        logoRect.y = 50;
        logoRect.w = logoSurface->w;
        logoRect.h = logoSurface->h;

        SDL_RenderCopy(render, logoTexture,NULL, &logoRect);

        SDL_FreeSurface(logoSurface);
        SDL_DestroyTexture(logoTexture);

        if(player==1)
        {
            std::string scoreText = "YOUR SCORE: " + std::to_string(score1);
            logoSurface = TTF_RenderText_Solid(scoreFont,scoreText.c_str(), textColor);
            logoTexture = SDL_CreateTextureFromSurface(render, logoSurface);

            logoRect.x =(ScreenW-logoSurface->w)/2;
            logoRect.y = 150;
            logoRect.w = logoSurface->w;
            logoRect.h = logoSurface->h;

            SDL_RenderCopy(render, logoTexture,NULL, &logoRect);

            SDL_FreeSurface(logoSurface);
            SDL_DestroyTexture(logoTexture);
        }
        TTF_CloseFont(scoreFont);

        if(player==2)
        {

            scoreFont=TTF_OpenFont("Font/Gagalin.ttf",48);
            std::string score1Text = "Player 1 SCORE: " + std::to_string(score1);
            logoSurface = TTF_RenderText_Solid(scoreFont,score1Text.c_str(), textColor);
            logoTexture = SDL_CreateTextureFromSurface(render, logoSurface);

            logoRect.x = (ScreenW-logoSurface->w)/2 ;
            logoRect.y = 150;
            logoRect.w = logoSurface->w;
            logoRect.h = logoSurface->h;

            SDL_RenderCopy(render, logoTexture,NULL, &logoRect);

            SDL_FreeSurface(logoSurface);
            SDL_DestroyTexture(logoTexture);

            std::string score2Text = "Player 2 SCORE: " + std::to_string(score2);
            logoSurface = TTF_RenderText_Solid(scoreFont,score2Text.c_str(), textColor);
            logoTexture = SDL_CreateTextureFromSurface(render, logoSurface);

            logoRect.x =(ScreenW-logoSurface->w)/2;
            logoRect.y = 200;
            logoRect.w = logoSurface->w;
            logoRect.h = logoSurface->h;

            SDL_RenderCopy(render, logoTexture,NULL, &logoRect);

            SDL_FreeSurface(logoSurface);
            SDL_DestroyTexture(logoTexture);
            TTF_CloseFont(scoreFont);
        }
    }

    if(paused)
    {

        scoreFont = TTF_OpenFont("Font/Gagalin.ttf",96);
        logoSurface = TTF_RenderText_Solid(scoreFont,"PAUSED", textColor);
        logoTexture = SDL_CreateTextureFromSurface(render, logoSurface);

        logoRect.x =(ScreenW-logoSurface->w)/2;
        logoRect.y = 50;
        logoRect.w = logoSurface->w;
        logoRect.h = logoSurface->h;

        SDL_RenderCopy(render, logoTexture,NULL, &logoRect);

        SDL_FreeSurface(logoSurface);
        SDL_DestroyTexture(logoTexture);

        TTF_CloseFont(scoreFont);

    }
    for(size_t i=0; i<tempItems.size(); i++)
    {

        menuSurface = TTF_RenderText_Solid(font, tempItems[i].c_str(), int(i) == selectedItemIndex ? selectedColor:textColor);
        menuTexture = SDL_CreateTextureFromSurface(render, menuSurface);

        int textWidth = menuSurface->w;
        int textHeight = menuSurface->h;
        int centerX = (ScreenW - textWidth)/2;
        int centerY = (ScreenH - menuItems.size() * 50) / 2 + i * 50;

        textRect.x = centerX;
        textRect.y = centerY;
        textRect.w = textWidth;
        textRect.h = textHeight;

        textRects.push_back(textRect);

        SDL_RenderCopy(render, menuTexture, NULL, &textRect);


        SDL_FreeSurface(menuSurface);
        SDL_DestroyTexture(menuTexture);

    }



    SDL_RenderPresent(render);
    TTF_CloseFont(font);
}
