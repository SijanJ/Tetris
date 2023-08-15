#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<vector>
#include<string>

class Menu{
private:
    SDL_Renderer* render;
    std::vector<SDL_Rect> textRects;
    std::vector<std::string> menuItems={"Single Player", "Multiplayer", "Quit"};
    std::vector<std::string> pauseItems={"RESUME","RESTART","MAINMENU","EXIT"};
    int selectedItemIndex;
    void renderMenu();
    bool paused;
    TTF_Font* font;
    SDL_Color textColor = {255,255,255,255};
    SDL_Color selectedColor ={255,155,100,150};
    SDL_Surface* menuSurface;
    SDL_Texture* menuTexture;
    SDL_Rect textRect;
    SDL_Point mousePosition; // Store mouse position

    bool isPointInRect(const SDL_Point &point, const SDL_Rect &rect)
    {
        return point.x >= rect.x && point.x <= rect.x + rect.w &&
               point.y >= rect.y && point.y <= rect.y + rect.h;
    }
    enum {ScreenW = 864, ScreenH = 800 };

public:
    Menu(SDL_Renderer* render):render{render},selectedItemIndex{0}{}
    ~Menu(){}
    int showMenu(bool);
};
#endif // MENU_H_INCLUDED
