#ifndef MIXER_H_INCLUDED
#define MIXER_H_INCLUDED


#include <iostream>
#include <vector>
#include "SDL.hpp"

class mixer:private SDL{
private:
    std::vector<SoundChunk*>sounds;
Music *m;
int volume = 25;
    public:

void playMusic(const char* filename)
{
    Music *m = NULL;
    m = loadMusic(filename);
    if(m == NULL) {
		printf("Failed to load music\n");

	}
	setVolume(volume);
	playMUS(m, -1);

}

int loadSound(const char* filename) {
	SoundChunk *m = NULL;
	m = loadSOUND(filename);
	if(m == NULL) {
		printf("Failed to load music\n");
		return -1;
	}
	sounds.push_back(m);
	return sounds.size()-1;
}

void toggleVolume() {
	 int volume = getMusicVolume(m);
          if(volume>0)
          setMusicVolume(0);
          else if(volume==0)
            setMusicVolume(25);
}
void rewindMusic()
{
    rewindMusic();
    pauseMusic();
}
int playSound(int s) {

        playSoundChunk(sounds[s],-1, 0);
	return 0;
}

void load_AllSound(){
loadSound("sound/move.ogg"); //0 = move sound
loadSound("sound/harddrop.mp3"); //1= hard drop
loadSound("sound/clearline.ogg");//2= line clear
loadSound("sound/impact.ogg"); // 3= invalid move
loadSound("sound/powerup.ogg"); // 4= powerup
}

void quitMixer() {
	for(int i=0; i<sounds.size(); i++) {
		freeChunk(sounds[i]);
		sounds[i]=NULL;
	}
	m = NULL;
	MixerQuit();
}


};
#endif // MIXER_H_INCLUDED
