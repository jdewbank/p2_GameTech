#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(void)
{
	SDL_Init( SDL_INIT_AUDIO );
	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
	sfx_hit = Mix_LoadWAV("sfx_hit.wav");
	sfx_wall= Mix_LoadWAV("sfx_wall.wav");
	sfx_miss= Mix_LoadWAV("sfx_miss.wav");
	mute = 0;
}

SoundPlayer::~SoundPlayer(void)
{
	 Mix_FreeChunk( sfx_hit ); 
	 Mix_FreeChunk( sfx_wall); 
	 Mix_FreeChunk( sfx_miss); 
	 Mix_CloseAudio(); 
	 SDL_Quit();
}

void SoundPlayer::playSound(int sfx_id)
{
	Mix_Chunk* sfx = 
		(sfx_id == 0)? sfx_hit:
		(sfx_id == 1)? sfx_wall:
		(sfx_id == 2)? sfx_miss: NULL;
	if(!mute) Mix_PlayChannel( -1, sfx, 0 );
}

void SoundPlayer::toggleSound() {mute = !mute;}
