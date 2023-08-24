#include "SDL.hpp"
#include<iostream>

int SDL::initImage(int flags){
    return IMG_Init(flags);
}

int SDL::initMusic(){
    Mix_Init(MIX_INIT_MP3);

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "SDL_Mixer couldnt init\n" <<Mix_GetError();

	}
return 0;
}

void SDL::setVolume(int volume)
{
    Mix_VolumeMusic(volume);
}

int SDL::initialize(){
    return SDL_Init(SDL_INIT_EVERYTHING);

}


int SDL::TTFInit(){
    return TTF_Init();
}
SDL_Window* SDL::createWindow(const char* title, int x, int y, int width, int height, Uint32 flags) {
    return SDL_CreateWindow(title, x, y, width, height, flags);
}

SDL_Renderer* SDL::createRenderer(SDL_Window* window, int index, Uint32 flags) {
    return SDL_CreateRenderer(window, index, flags);
}

SDL::Texture* SDL::loadTexture(const char* filePath, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        // Handle error
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL::SoundChunk* SDL::loadSOUND(const char* filePath) {
    SDL::SoundChunk* sound = Mix_LoadWAV(filePath);
    if (!sound) {
        // Handle error
    }
    return sound;
}

SDL::Music* SDL::loadMusic(const char* filePath) {
    SDL::Music* music = Mix_LoadMUS(filePath);
    if (!music) {
        // Handle error
    }
    return music;
}

int SDL::getMusicVolume(Mix_Music *music){
    return Mix_GetMusicVolume(music);
}

void SDL::setMusicVolume(int volume){
Mix_VolumeMusic(volume);
}

int SDL::pollEvent(SDL_Event& event) {
    return SDL_PollEvent(&event);
}


int SDL::playSoundChannel(SDL::SoundChunk* chunk, int channel, int loops) {
    return Mix_PlayChannel(channel, chunk, loops);
}

void SDL::playMUS(SDL::Music* music, int loops) {
    Mix_PlayMusic(music, loops);
}

void SDL::pauseMusic() {
    Mix_PauseMusic();
}

void SDL::rewindMusic()
{
    Mix_RewindMusic();
}

void SDL::resumeMusic()
{
   Mix_ResumeMusic();
}



void SDL::delay(unsigned int milliseconds) {
    SDL_Delay(milliseconds);
}

void SDL::getMouseState(int* mouseX, int* mouseY) {
    SDL_GetMouseState(mouseX, mouseY);
}
Uint8* SDL::getKeyboardState(int* key){
    SDL_GetKeyboardState(key);
}


Uint32 SDL::getTicks() const {
    return SDL_GetTicks();
}



int SDL::playSoundChunk(SDL::SoundChunk* chunk, int channel, int loops) {
    return Mix_PlayChannel(channel, chunk, loops);
}


int SDL::setRenderDrawColor(Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
    return SDL_SetRenderDrawColor(renderer,r, g, b, a);
}
void SDL::renderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect) {
    SDL_RenderCopy(renderer, texture, srcRect, dstRect);
}

void SDL::renderClear(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);
}

void SDL::destroyWindow(SDL_Window* window) {
    SDL_DestroyWindow(window);
}

void SDL::destroyTexture(SDL_Texture* texture){
    SDL_DestroyTexture(texture);
}


void SDL::freeSurface(SDL_Surface* surface){
    SDL_FreeSurface(surface);
}

void SDL::freeChunk(SoundChunk* sound){
   Mix_FreeChunk(sound);
}

void SDL::renderPresent(SDL_Renderer* renderer) {
    SDL_RenderPresent(renderer);
}
void SDL::setTextureAlphaMod(SDL_Texture* texture, int alpha){
    SDL_SetTextureAlphaMod(texture, alpha);
}
void SDL::destroyRenderer(SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
}

void SDL::imageQuit(){
    IMG_Quit();
}

void SDL::TTFQuit()
{
    TTF_Quit();
}

void SDL::MixerQuit()
{
    Mix_Quit();
}

void SDL::SDLQuit(){
    SDL_Quit();
}

void SDL::closeFont(TTF_Font* font){
    TTF_CloseFont(font);
}
SDL_Texture* SDL::loadTexture(SDL_Renderer* renderer, const char* path) {
    return IMG_LoadTexture(renderer, path);
}

TTF_Font* SDL::openFont(const char* file, int size) {
    return TTF_OpenFont(file, size);
}

SDL_Surface* SDL::renderTextSolid(TTF_Font* font, const char* text, SDL_Color color) {
    return TTF_RenderText_Solid(font, text, color);
}

SDL_Texture* SDL::createTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(renderer, surface);
}

SDL_Surface* SDL::loadImage(const char* filePath){
    return IMG_Load(filePath);
}

// Other member function implementations...

