#include "menu.h"
#include<iostream>

void Menu::renderImage(const char* path, bool background)
{
    imageSurface = loadImage(path);
    imageTexture = createTextureFromSurface(render, imageSurface);
    //imageTexture = loadTexture(path, render);
    if(!background)
    {
        imageRect.x = (ScreenW - imageSurface->w) / 2;
        imageRect.y = 50;
        imageRect.w = imageSurface->w;
        imageRect.h = imageSurface->h;
        renderCopy(render, imageTexture, NULL, &imageRect);
    }
    else
    {
        renderCopy(render, imageTexture, NULL, NULL);
    }
    freeSurface(imageSurface);
    destroyTexture(imageTexture);
}

void Menu::startScreen()
{
    bool showText = true;
    Uint32 lastTextChangeTime = getTicks();
    bool startScreenRunning = true;
    setRenderDrawColor(render, 150, 150, 150, 255);
    renderClear(render);

    while (startScreenRunning)
    {
        Event e;
        while (pollEvent(e))
        {
            if (e.type == QUIT)
            {
                exit(0);
            }
            if (e.type == KEY_DOWN)
            {
                if (e.key.keysym.sym == ENTER)
                {
                    return;
                }
            }
        }

        // Clear the renderer
        setRenderDrawColor(render, 150, 150, 150, 255);
        renderClear(render);

        renderImage("images/logo.png",false);


        // Blink the text every 500ms
        Uint32 currentTime = getTicks();
        if (currentTime - lastTextChangeTime >= 500)
        {
            lastTextChangeTime = currentTime;
            showText = !showText;
        }

        if (showText)
        {
            renderText(48, "Press Enter To Continue", 500);
        }

        renderPresent(render);

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
    Event e;
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
        if(menuRunning)
            pauseMusic();
        while(pollEvent(e))
        {
            switch(e.type)
            {
            case QUIT:
                exit(0);
                break;
            case KEY_DOWN:
                switch(e.key.keysym.sym)
                {
                case UP:
                    selectedItemIndex=(selectedItemIndex-1+tempItems.size())%tempItems.size();
                    break;
                case DOWN:
                    selectedItemIndex=(selectedItemIndex+1)%tempItems.size();
                    break;
                case ENTER:
                    if((selectedItemIndex == 1 && paused)||(((selectedItemIndex==0&&!paused)||selectedItemIndex==1)))
                          {
                              rewindMusic();
                              resumeMusic();
                          }
                    if(selectedItemIndex == 0 && paused)
                            resumeMusic();
                        if(selectedItemIndex == 2 && paused)
                                rewindMusic();

                    menuRunning=false;
                    return selectedItemIndex;

                default:
                    break;
                }
            case MOUSE_MOTION:
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

            case MOUSE_BUTTON_DOWN:
                switch (e.button.button)
                {
                case BUTTON_LEFT:
                    //Check if mouse is over menu item
                    for(size_t i =0; i<tempItems.size(); i++)
                    {
                        if(isPointInRect(mousePosition, textRects[i]))
                        {
                            menuRunning=false;
                            selectedItemIndex = i;
                            if(selectedItemIndex==0)
                                resumeMusic();
                            if(selectedItemIndex==1)
                            {
                                rewindMusic();
                                resumeMusic();
                            }
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
    font = openFont("Font/Gagalin.ttf", fsize);
    if (!font)
    {
        std::cout << "Failed to load font: " << std::endl;
        return;
    }

    textSurface = renderTextSolid(font,text.c_str(), textColor);
    textTexture = createTextureFromSurface(render, textSurface);

    textRect.x = (ScreenW-textSurface->w)/2;;
    textRect.y = y;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    renderCopy(render, textTexture,NULL, &textRect);

    freeSurface(textSurface);
    destroyTexture(textTexture);

    closeFont(font);
}


void Menu::renderMenu()
{

    setRenderDrawColor(render, 0,0,0,255);
    renderClear(render);
    textRects.clear();
    renderImage("images/BG.png",true);
    if(!paused && !gameOver)
    {
        //renderImage("images/Space.gif",true);
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

    font = openFont("Font/Gagalin.ttf", 36);
    for(size_t i=0; i<tempItems.size(); i++)
    {

        textSurface = renderTextSolid(font, tempItems[i].c_str(), int(i) == selectedItemIndex ? selectedColor:textColor);
        textTexture = createTextureFromSurface(render, textSurface);

        int textWidth = textSurface->w;
        int textHeight = textSurface->h;
        int centerX = (ScreenW - textWidth)/2;
        int centerY = (ScreenH - menuItems.size() * 50) / 2 + i * 50;

        textRect.x = centerX;
        textRect.y = centerY;
        textRect.w = textWidth;
        textRect.h = textHeight;

        textRects.push_back(textRect);

        renderCopy(render, textTexture, NULL, &textRect);

        freeSurface(textSurface);
        destroyTexture(textTexture);

    }


    renderPresent(render);
    closeFont(font);
}
