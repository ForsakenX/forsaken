/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Includes
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include	"cdaudio.h"
#include	"main.h"	// for definition of MAX_LEVELS

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Global Variables
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	CDSound		sound;
	CDInfo		cd_info;
	int16		actual_volume;
	BOOL		CD_Present = FALSE;
	int			CDTrack[MAXLEVELS];
	DWORD CDFinishTime;

	// thread stuff...
#ifdef CD_LOOP_THREAD
	DWORD	CDThreadID;
	HANDLE	CDThread;
	CRITICAL_SECTION	CDKey;
	BOOL RestartCDTrack = FALSE;
#endif

	extern BOOL CD_OK;


void DebugPrintf( const char * format, ... );

#ifdef CD_LOOP_THREAD

void CDThreadProc (void * pParm)
{
	while( 1 )
	{
		EnterCriticalSection (&CDKey);

		if ( RestartCDTrack )
		{
			RestartCDTrack = FALSE;
			PlayAudioTrack();
		}

		LeaveCriticalSection (&CDKey);

		Sleep( 0 );
	}
}

#endif
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Initialise Sound Devices
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SoundInit( void )
{
	long numDevs, c, found;
	
	/*
	 * set up the volume information
	 */

#ifdef WIN98SHAREWARE
	return;
#endif

	found = FALSE;

	numDevs = auxGetNumDevs();

	for (c = 0; !found && c < numDevs; c++)
	{
		AUXCAPS		auxCaps;

		auxGetDevCaps(c, &auxCaps, sizeof(AUXCAPS));

		if( auxCaps.wTechnology & AUXCAPS_CDAUDIO)
		{
			cd_info.volume_id = c;
			found = TRUE;
		}
	}

	
	auxGetVolume( cd_info.volume_id, &sound.OriginalMusicVolume );
	actual_volume = (int16) sound.OriginalMusicVolume;


}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Close CD Device
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CloseCD( void )
{
	if( cd_info.DeviceID )
	{
	    mciSendCommand(cd_info.DeviceID, MCI_CLOSE, 0, (DWORD)NULL);
		cd_info.DeviceID = 0;
		CD_Present = FALSE;
#ifdef CD_LOOP_THREAD
		TerminateThread (&CDThread, 0);
#endif
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Open CD Device
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void OpenCD( void )
{
    DWORD dwReturn;
    MCI_OPEN_PARMS mciOpenParams;
    MCI_SET_PARMS mciSetParams;
	char ErrorText[128];
	MCI_STATUS_PARMS mci_status;
	uint8 i;

#ifdef CD_LOOP_THREAD
	InitializeCriticalSection (&CDKey);
#endif

#ifdef WIN98SHAREWARE
	return;
#endif
		
	if( cd_info.DeviceID ) 
		return;

	memset(&mciOpenParams, sizeof(MCI_OPEN_PARMS), 0);
    mciOpenParams.lpstrDeviceType = "cdaudio";
    
	if( dwReturn = mciSendCommand( 0, MCI_OPEN, MCI_OPEN_TYPE, (DWORD)(LPVOID) &mciOpenParams ) ) 
	{
	        /*
	         * Failed to open device;
	         * don't close it, just return error.
	         */
		mciGetErrorString( dwReturn, ErrorText, 128 ); 
		DebugPrintf("PlayAudioTrack() - unable to open CD - error: %s\n", ErrorText );

		return;
    }

    /* Device opened successfully, get the device ID. */
    
	cd_info.DeviceID = mciOpenParams.wDeviceID;

    /* Set the time format to minute/second/frame. */
    
	mciSetParams.dwTimeFormat = MCI_FORMAT_TMSF;
    
	if( dwReturn = mciSendCommand(cd_info.DeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID) 	&mciSetParams)) 
	{
		CloseCD();
		return;
    }

	// find out number of tracks on CD...
	mci_status.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
	if (dwReturn = mciSendCommand(cd_info.DeviceID, MCI_STATUS, MCI_STATUS_ITEM|MCI_WAIT, (DWORD)(LPVOID)&mci_status))
	{
		mciGetErrorString( dwReturn, ErrorText, 128 ); 
		DebugPrintf("PlayAudioTrack() - unable to get num tracks - error: %s\n", ErrorText );
		CloseCD();
		return;
	}

	cd_info.num_tracks = mci_status.dwReturn;

	if ( cd_info.num_tracks > MAX_AUDIO_TRACKS )
		cd_info.num_tracks = MAX_AUDIO_TRACKS;

	mci_status.dwItem = MCI_STATUS_LENGTH;
	for (i=0; i<cd_info.num_tracks; i++)
	{
		mci_status.dwTrack = i+1;
		mciSendCommand(cd_info.DeviceID, MCI_STATUS, MCI_TRACK|MCI_STATUS_ITEM|MCI_WAIT,	(DWORD)(LPVOID)&mci_status);
		//DebugPrintf("mins: %d, secs: %d\n", MCI_MSF_MINUTE(mci_status.dwReturn), MCI_MSF_SECOND(mci_status.dwReturn));
		cd_info.track_length[ i ] = (MCI_MSF_MINUTE(mci_status.dwReturn)*60 + MCI_MSF_SECOND(mci_status.dwReturn)) * 1000;
	}
#ifdef CD_LOOP_THREAD
	CDThread =
	CreateThread (NULL, 0,
	              (LPTHREAD_START_ROUTINE) CDThreadProc,
	              NULL, 0, &CDThreadID);

	if (!(SetThreadPriority( CDThread, THREAD_PRIORITY_LOWEST )) )
		DebugPrintf("Unable to set thread priority\n");
#endif

	CD_Present = TRUE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Set Volume of CD
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CdSetVolume( void )
{
	auxSetVolume( cd_info.volume_id, ( actual_volume | ( actual_volume << 16 ) ) );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Play Audio Track
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void PlayAudioTrack( void )
{
    DWORD dwReturn;
    MCI_PLAY_PARMS mciPlay;
	char ErrorText[128];
	
    /*
     * Begin playback from the given track and play
     * until the beginning of the next track. The window
     * procedure function for the parent window will be
     * notified with an MM_MCINOTIFY message when playback
     * is complete. Unless the play command fails, the
     * window procedure closes the device.
     */

#ifdef WIN98SHAREWARE
	return;
#endif

	if( !CD_OK )
		return;
    
    mciPlay.dwCallback = (DWORD) myglobs.hWndMain;
	mciPlay.dwFrom = MCI_MAKE_TMSF(cd_info.current_track, 0, 0, 0);
	if ( cd_info.current_track == cd_info.num_tracks )
	{
		if (dwReturn = mciSendCommand(cd_info.DeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY , (DWORD)(LPVOID) &mciPlay))
		{
			mciGetErrorString( dwReturn, ErrorText, 128 ); 
			DebugPrintf("PlayAudioTrack() - unable to play track %d - error: %s\n", cd_info.current_track, ErrorText );
		    return;
		}
	}else
	{
		mciPlay.dwTo = MCI_MAKE_TMSF(cd_info.current_track + 1, 0, 0, 0);
		if (dwReturn = mciSendCommand(cd_info.DeviceID, MCI_PLAY, MCI_FROM | MCI_TO | MCI_NOTIFY , (DWORD)(LPVOID) &mciPlay))
		{
			mciGetErrorString( dwReturn, ErrorText, 128 ); 
			DebugPrintf("PlayAudioTrack() - unable to play track %d - error: %s\n", cd_info.current_track, ErrorText );
		    return;
		}
	}

#ifdef CD_LOOP_THREAD
	CDFinishTime = timeGetTime() + cd_info.track_length[ cd_info.current_track - 1 ];
	DebugPrintf("CD started at %d", timeGetTime() );
	DebugPrintf("due to finish at %d", CDFinishTime );
#endif

	cd_info.IsPlaying = TRUE;

    return;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Stop Playing Audio Track
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CdStop( void )
{
    DWORD dwReturn;
    MCI_GENERIC_PARMS mciGenericParms;

	cd_info.IsPlaying = FALSE;

	if( !cd_info.DeviceID ) return;

    mciGenericParms.dwCallback = (DWORD) myglobs.hWndMain;
    
	if (dwReturn = mciSendCommand(cd_info.DeviceID, MCI_STOP, MCI_WAIT, (DWORD)(LPVOID) 	&mciGenericParms)) 
	{
		CloseCD();
	}

	cd_info.loop = FALSE;
	cd_info.paused = FALSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Unpause CD Audio
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CdUnPause( void )
{
    DWORD dwReturn;
    MCI_PLAY_PARMS mciPlay;
	char ErrorText[128];

	cd_info.IsPlaying = TRUE;

	if( !cd_info.DeviceID ) return;

	mciPlay.dwFrom = cd_info.current_position;
    mciPlay.dwTo = MCI_MAKE_TMSF(cd_info.current_track + 1, 0, 0, 0);
    mciPlay.dwCallback = (DWORD) myglobs.hWndMain;

	if ( cd_info.current_track == cd_info.num_tracks )
	{
		if (dwReturn = mciSendCommand(cd_info.DeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY , (DWORD)(LPVOID) &mciPlay))
		{
			mciGetErrorString( dwReturn, ErrorText, 128 ); 
			DebugPrintf("PlayAudioTrack() - unable to play track %d - error: %s\n", cd_info.current_track, ErrorText );
		    return;
		}
	}else
	{
		mciPlay.dwTo = MCI_MAKE_TMSF(cd_info.current_track + 1, 0, 0, 0);
		if (dwReturn = mciSendCommand(cd_info.DeviceID, MCI_PLAY, MCI_FROM | MCI_TO | MCI_NOTIFY , (DWORD)(LPVOID) &mciPlay))
		{
			mciGetErrorString( dwReturn, ErrorText, 128 ); 
			DebugPrintf("PlayAudioTrack() - unable to play track %d - error: %s\n", cd_info.current_track, ErrorText );
		    return;
		}
	}
    
	cd_info.paused = FALSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Pause CD Audio
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CdPause( void )
{
    DWORD dwReturn;
    MCI_GENERIC_PARMS mciGeneric;
	MCI_STATUS_PARMS mciStatus;

#ifdef WIN98SHAREWARE
	return;
#endif

	cd_info.IsPlaying = FALSE;

	if( !cd_info.DeviceID )	return;

    mciGeneric.dwCallback = (DWORD) myglobs.hWndMain;
    mciStatus.dwCallback = (DWORD) myglobs.hWndMain;

	mciStatus.dwItem = MCI_STATUS_POSITION;
	
	if (dwReturn = mciSendCommand(cd_info.DeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID) &mciStatus)) 
	{
		CloseCD();
	}
    
	if (dwReturn = mciSendCommand(cd_info.DeviceID, MCI_STOP, MCI_WAIT, (DWORD)(LPVOID) &mciGeneric)) 
	{
		CloseCD();
	}

	cd_info.current_position = mciStatus.dwReturn;
	cd_info.paused = TRUE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if CD is still playing
	Input		:	Nothing
	Output		:	BOOL	True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL CdIsPlaying( void )
{
    DWORD dwReturn;
	MCI_STATUS_PARMS mciStatus;

#ifdef WIN98SHAREWARE
	return FALSE;
#endif
  
	if( !cd_info.DeviceID ) return TRUE;
	if( !cd_info.loop ) return TRUE;

	mciStatus.dwItem = MCI_STATUS_MODE; 
 
	dwReturn = mciSendCommand(cd_info.DeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID) &mciStatus); 

	return( mciStatus.dwReturn == MCI_MODE_PLAY );
}

