#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include<iostream>
#include<vector>
#include<string>
#include "SDL.hpp"


class Menu: public SDL
{
private:
    Renderer* render;
    std::vector<Rect> textRects;
    std::vector<std::string> menuItems= {"SINGLE PLAYER", "MULTIPLAYER", "QUIT"};
    std::vector<std::string> pauseItems= {"RESUME","RESTART","MAIN MENU","QUIT"};
    std::vector<std::string> gameOverItems= {"SINGLE PLAYER", "MULTIPLAYER", "MAIN MENU", "QUIT"};
    std::vector<std::string> tempItems;
    int selectedItemIndex;
    void renderMenu();
    bool paused, gameOver;
    Font* font, *scoreFont;
    Color textColor = {255,255,255,255};
    Color selectedColor = {255,155,100,150};
    Surface *imageSurface, *textSurface;
    Texture *imageTexture, *textTexture;
    Rect textRect, imageRect;

    void renderImage(const char* path, bool);
    Texture* logoTexture = NULL;
    void renderText(int size, std::string text,  int y);
    Point mousePosition; // Store mouse position

    bool isPointInRect(const Point &point, const Rect &rect)
    {
        return point.x >= rect.x && point.x <= rect.x + rect.w &&
               point.y >= rect.y && point.y <= rect.y + rect.h;
    }
    enum {ScreenW = 1920, ScreenH = 1080 };
    int score1, score2, player, winner;

public:
    Menu(Renderer* render):render{render},selectedItemIndex{0} {}
    ~Menu() {}
    int showMenu(bool,bool,int score1=0, int score2=0, int player=1, int winner=0);
    void startScreen();
};
#endif // MENU_H_INCLUDED
