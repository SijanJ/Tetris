#include "menu.h"
#include<iostream>

void Menu::renderImage(const char* path, bool background)
{
    imageSurface = IMG_Load(path);
    imageTexture = SDL_CreateTextureFromSurface(render, imageSurface);
    if(!background)
    {
        imageRect.x = (ScreenW - imageSurface->w) / 2;
        imageRect.y = 50;
        imageRect.w = imageSurface->w;
        imageRect.h = imageSurface->h;
        SDL_RenderCopy(render, imageTexture, NULL, &imageRect);
    }
    else
    {
        SDL_RenderCopy(render, imageTexture, NULL, NULL);
    }
    SDL_FreeSurface(imageSurface);
    SDL_DestroyTexture(imageTexture);
}

void Menu::startScreen()
{
    bool showText = true;
    Uint32 lastTextChangeTime = SDL_GetTicks();
    bool startScreenRunning = true;
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);

    while (startScreenRunning)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                exit(0);
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    return;
                }
            }
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);

        renderImage("images/logo.png",false);


        // Blink the text every 500ms
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastTextChangeTime >= 500)
        {
            lastTextChangeTime = currentTime;
            showText = !showText;
        }

        if (showText)
        {
            renderText(48, "Press Enter To Continue", 500);
        }

        SDL_RenderPresent(render);

    }
}


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
                break;
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
                    menuRunning=false;
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
                            menuRunning=false;
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

void Menu::renderText(int fsize, std::string text, int y)
{
    font = TTF_OpenFont("Font/Gagalin.ttf", fsize);
    if (!font)
    {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    textSurface = TTF_RenderText_Solid(font,text.c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(render, textSurface);

    textRect.x = (ScreenW-textSurface->w)/2;;
    textRect.y = y;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_RenderCopy(render, textTexture,NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    TTF_CloseFont(font);
}


void Menu::renderMenu()
{

    SDL_SetRenderDrawColor(render, 0,0,0,255);
    SDL_RenderClear(render);
    textRects.clear();
    if(!paused && !gameOver)
    {
        renderImage("images/Space.gif",true);
        renderImage("images/logo.png",false);

    }

    if(gameOver)
    {
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



        renderText(96,gameOverText,50);

        if(player==1)
        {
            std::string scoreText = "YOUR SCORE: " + std::to_string(score1);

            renderText(96, scoreText,  150);
        }

        if(player==2)
        {

            std::string score1Text = "Player 1 SCORE: " + std::to_string(score1);
            renderText(48, score1Text,  150);

            std::string score2Text = "Player 2 SCORE: " + std::to_string(score2);
            renderText(48, score2Text,  200);
        }
    }

    if(paused)
    {
        renderText(96, "PAUSED",  50);
    }

    font = TTF_OpenFont("Font/Gagalin.ttf", 36);
    for(size_t i=0; i<tempItems.size(); i++)
    {

        textSurface = TTF_RenderText_Solid(font, tempItems[i].c_str(), int(i) == selectedItemIndex ? selectedColor:textColor);
        textTexture = SDL_CreateTextureFromSurface(render, textSurface);

        int textWidth = textSurface->w;
        int textHeight = textSurface->h;
        int centerX = (ScreenW - textWidth)/2;
        int centerY = (ScreenH - menuItems.size() * 50) / 2 + i * 50;

        textRect.x = centerX;
        textRect.y = centerY;
        textRect.w = textWidth;
        textRect.h = textHeight;

        textRects.push_back(textRect);

        SDL_RenderCopy(render, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

    }


    SDL_RenderPresent(render);
    TTF_CloseFont(font);
}
