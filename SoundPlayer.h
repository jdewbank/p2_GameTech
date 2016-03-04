#ifndef __SoundPlayer_h_
#define __SoundPlayer_h_

#include <SDL.h>
#include <SDL_mixer.h>

class SoundPlayer
{
public:
	bool mute;
	Mix_Chunk *sfx_hit; 
	Mix_Chunk *sfx_wall;
	Mix_Chunk *sfx_miss; 

    SoundPlayer(void);
    ~SoundPlayer(void);

	void playSound(int);
	void toggleSound();
};


#endif // #ifndef __SoundPlayer_h_