#ifndef MIXER_H_INCLUDED
#define MIXER_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>

class mixer{
private:
    std::vector<Mix_Chunk*>sounds;
Mix_Music *m;
    public:


int initMixer() {
	Mix_Init(MIX_INIT_MP3);

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_Mixer couldnt init\n", Mix_GetError());

	}
return 0;

}


void playMusic(const char* filename)
{
    Mix_Music *m = NULL;
    m = Mix_LoadMUS(filename);
    if(m == NULL) {
		printf("Failed to load music\n", Mix_GetError());

	}
	Mix_VolumeMusic(50);
	Mix_PlayMusic(m, -1);

}

int loadSound(const char* filename) {
	Mix_Chunk *m = NULL;
	m = Mix_LoadWAV(filename);
	if(m == NULL) {
		printf("Failed to load music\n", Mix_GetError());
		return -1;
	}
	sounds.push_back(m);
	return sounds.size()-1;
}

void toggleVolume() {
	 int volume = Mix_GetMusicVolume(m);
          if(volume>0)
          Mix_VolumeMusic(0);
          else if(volume==0)
            Mix_VolumeMusic(50);
}
void rewindMusic()
{
    Mix_RewindMusic();
    Mix_PauseMusic();
}
int playSound(int s) {

        Mix_PlayChannel(-1, sounds[s], 0);
	return 0;
}

void load_AllSound(){
int sound0 = loadSound("sound/move.ogg"); //0 = move sound
int sound1 = loadSound("sound/harddrop.mp3"); //1= hard drop
int sound2 = loadSound("sound/clearline.ogg");//2= line clear
int sound3 = loadSound("sound/impact.ogg"); // 3= invalid move
int sound4 = loadSound("sound/powerup.ogg"); // 4= powerup
}

void quitMixer() {
	for(int i=0; i<sounds.size(); i++) {
		Mix_FreeChunk(sounds[i]);
		sounds[i]=NULL;
	}
	m = NULL;
	Mix_Quit();
}


};
#endif // MIXER_H_INCLUDED
