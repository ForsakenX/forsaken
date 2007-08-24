#ifndef CDAUDIO_INCLUDED
#define CDAUDIO_INCLUDED

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Includes
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include "typedefs.h"
#include "d3dmain.h"

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Structures
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define MAX_AUDIO_TRACKS 16

typedef struct CDInfo {

    UINT			DeviceID;
	unsigned long	current_track; 
	DWORD			current_position;
	long			paused;
	long			loop;
	long			volume_id;
	long			IsPlaying;
	DWORD			num_tracks;
	DWORD			track_length[ MAX_AUDIO_TRACKS ];	// milliseconds
} CDInfo;

typedef struct CDSound {

	uint32			OriginalMusicVolume;

} CDSound;


#define CD_TITLE_TRACK 7

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Prototypes
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SoundInit( void );
void CloseCD( void );
void OpenCD( void );
void CdSetVolume( void );
void PlayAudioTrack( void );
void CdStop( void );
void CdUnPause( void );
void CdPause( void );
BOOL CdIsPlaying( void );

#endif
