#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class SDL {
public:
    using Texture = SDL_Texture;
    using SoundChunk = Mix_Chunk;
    using Music = Mix_Music;
    using Event = SDL_Event;
    using Rect = SDL_Rect;
    using Window = SDL_Window;
    using Font = TTF_Font;
    using Color = SDL_Color;
    using Surface = SDL_Surface;
    using Renderer = SDL_Renderer;
    using Point = SDL_Point;

    static constexpr int QUIT = SDL_QUIT;
    static constexpr int KEY_UP = SDL_KEYUP;
    static constexpr int KEY_DOWN = SDL_KEYDOWN;
    static constexpr int MOUSE_BUTTON_DOWN = SDL_MOUSEBUTTONDOWN;
    static constexpr int MOUSE_BUTTON_UP = SDL_MOUSEBUTTONUP;
    static constexpr int MOUSE_MOTION = SDL_MOUSEMOTION;

    static constexpr int UP = SDLK_UP;
    static constexpr int RSHIFT = SDLK_RSHIFT;
    static constexpr int LSHIFT = SDLK_LSHIFT;
    static constexpr int DOWN = SDLK_DOWN;
    static constexpr int LEFT = SDLK_LEFT;
    static constexpr int RIGHT = SDLK_RIGHT;
    static constexpr int ESC = SDLK_ESCAPE;
    static constexpr int A = SDLK_a;
    static constexpr int S = SDLK_s;
    static constexpr int D = SDLK_d;
    static constexpr int W = SDLK_w;
    static constexpr int M = SDLK_m;
    static constexpr int BUTTON_LEFT = SDL_BUTTON_LEFT;
    static constexpr int ENTER = SDLK_RETURN;
    static constexpr int WINDOWSHOWN = SDL_WINDOW_SHOWN;
    static constexpr int SCANCODE_DOWN = SDL_SCANCODE_DOWN;
    static constexpr int SCANCODE_S = SDL_SCANCODE_S;

    static const int WindowPosUndefined = SDL_WINDOWPOS_UNDEFINED;
    static const int InitPNG = IMG_INIT_PNG;
    static const int RendererAccelerated = SDL_RENDERER_ACCELERATED;


    SDL_Window* createWindow(const char* title, int x, int y, int width, int height, Uint32 flags);
    SDL_Renderer* createRenderer(SDL_Window* window, int index, Uint32 flags);
    Texture* loadTexture(const char* filePath, SDL_Renderer* renderer);
    SoundChunk* loadSOUND(const char* filePath);
    Music* loadMusic(const char* filePath);
    Surface* loadImage(const char* filePath);
    int pollEvent(Event& event);
    int playSoundChannel(Mix_Chunk* chunk, int channel, int loops);
    int TTFInit();
    int initImage(int flags);
    int initMusic();
    void setVolume(int volume);
    void playMUS(Mix_Music* music, int loops);
    void pauseMusic();
    void rewindMusic();
    void resumeMusic();
    void delay(unsigned int milliseconds);
    void getMouseState(int* mouseX, int* mouseY);
    Uint8* getKeyboardState(int* key);
    Uint32 getTicks() const;

    int playSoundChunk(Mix_Chunk* chunk, int channel, int loops);
    int getMusicVolume(Mix_Music *music);
    void setMusicVolume(int volume);
    int setRenderDrawColor(Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void renderCopy(Renderer* renderer, Texture* texture, const Rect* srcRect, const Rect* dstRect);
    void renderClear(Renderer* renderer);
    void destroyWindow(Window* window);
    void destroyRenderer(Renderer* renderer);
    void renderPresent(Renderer* renderer);
    void freeSurface(Surface* surface);
    void freeChunk(SoundChunk* sound);
    void destroyTexture(Texture* texture);
    void setTextureAlphaMod(Texture* texture, int alpha);
    void imageQuit();
    void TTFQuit();
    void MixerQuit();
    void SDLQuit();
    void closeFont(Font* font);
    int initialize();
    Texture* loadTexture(Renderer* renderer, const char* path);
    Font* openFont(const char* file, int size);
    Surface* renderTextSolid(Font* font, const char* text, Color color);
    Texture* createTextureFromSurface(Renderer* renderer, Surface* surface);

private:
    // Any internal state or resources can be managed here
};

