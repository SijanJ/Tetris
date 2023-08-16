#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<vector>
#include<string>

class Menu
{
private:
    SDL_Renderer* render;
    std::vector<SDL_Rect> textRects;
    std::vector<std::string> menuItems= {"SINGLE PLAYER", "MULTIPLAYER", "QUIT"};
    std::vector<std::string> pauseItems= {"RESUME","RESTART","MAIN MENU","QUIT"};
    std::vector<std::string> gameOverItems= {"SINGLE PLAYER", "MULTIPLAYER", "MAIN MENU", "QUIT"};
    std::vector<std::string> tempItems;
    int selectedItemIndex;
    void renderMenu();
    bool paused, gameOver;
    TTF_Font* font, *scoreFont;
    SDL_Color textColor = {255,255,255,255};
    SDL_Color selectedColor = {255,155,100,150};
    SDL_Surface* menuSurface, *backSurface;
    SDL_Texture* menuTexture, *backTexture;
    SDL_Rect textRect, logoRect;
    SDL_Surface* logoSurface;
    SDL_Texture* logoTexture;
    SDL_Point mousePosition; // Store mouse position

    bool isPointInRect(const SDL_Point &point, const SDL_Rect &rect)
    {
        return point.x >= rect.x && point.x <= rect.x + rect.w &&
               point.y >= rect.y && point.y <= rect.y + rect.h;
    }
    enum {ScreenW = 864, ScreenH = 800 };
    int score1, score2, player, winner;

public:
    Menu(SDL_Renderer* render):render{render},selectedItemIndex{0} {}
    ~Menu() {}
    int showMenu(bool,bool,int score1=0, int score2=0, int player=1, int winner=0);
};
#endif // MENU_H_INCLUDED
