#include "menu.h"

int Menu::showMenu(bool p){
    paused = p;
    bool menuRunning = true;
    SDL_Event e;

    while(menuRunning){
        while(SDL_PollEvent(&e)){
            switch(e.type){
            case SDL_QUIT:
                return -1;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
            case SDLK_UP:
                if(!paused){
                selectedItemIndex=(selectedItemIndex-1+menuItems.size())%menuItems.size();
                }
                else{
                  selectedItemIndex=(selectedItemIndex-1+pauseItems.size())%pauseItems.size();
                }
                break;
            case SDLK_DOWN:
                if(!paused){
                selectedItemIndex=(selectedItemIndex+1)%menuItems.size();
                }
                else{
                  selectedItemIndex=(selectedItemIndex+1)%pauseItems.size();
                }
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
                switch (e.button.button){
                    case SDL_BUTTON_LEFT:
                        //Check if mouse is over menu item
                    if(!paused){
                        for(size_t i =0;i<menuItems.size();i++){
                            if(isPointInRect(mousePosition, textRects[i]))
                            {
                                selectedItemIndex = i;
                                return selectedItemIndex;
                            }
                        }
                    }
                    else{
                        for(size_t i =0;i<pauseItems.size();i++){
                            if(isPointInRect(mousePosition, textRects[i]))
                            {
                                selectedItemIndex = i;
                                return selectedItemIndex;
                            }
                        }
                    }
                }
            }
        }

    renderMenu();

    }

    return -1;

}

void Menu::renderMenu(){
font = TTF_OpenFont("Font/Tetris.ttf", 36);
if(!font){
    std::cout<<TTF_GetError();
    return;
}
SDL_SetRenderDrawColor(render, 0,0,0,255);
SDL_RenderClear(render);
textRects.clear();
if(!paused){
for(size_t i=0;i<menuItems.size();i++){
    menuSurface = TTF_RenderText_Solid(font, menuItems[i].c_str(), i== selectedItemIndex ? selectedColor:textColor);
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
}
else{
        for(size_t i=0;i<pauseItems.size();i++){
    menuSurface = TTF_RenderText_Solid(font, pauseItems[i].c_str(), i== selectedItemIndex ? selectedColor:textColor);
    menuTexture = SDL_CreateTextureFromSurface(render, menuSurface);

    int textWidth = menuSurface->w;
    int textHeight = menuSurface->h;
    int centerX = (ScreenW - textWidth)/2;
    int centerY = (ScreenH - pauseItems.size() * 50) / 2 + i * 50;

    textRect.x = centerX;
    textRect.y = centerY;
    textRect.w = textWidth;
    textRect.h = textHeight;

    textRects.push_back(textRect);

    SDL_RenderCopy(render, menuTexture, NULL, &textRect);

    SDL_FreeSurface(menuSurface);
    SDL_DestroyTexture(menuTexture);
}



}
SDL_RenderPresent(render);
TTF_CloseFont(font);}
