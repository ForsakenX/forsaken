/*
 * The X Men, November 1997
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 39 $
 *
 * $Header: /PcProjectX/LoadSave.c 39    8/04/98 10:22 Collinsd $
 *
 * $Log: /PcProjectX/LoadSave.c $
 * 
 * 39    8/04/98 10:22 Collinsd
 * Screenshots folder created if doesn't exist.
 * 
 * 38    27/03/98 21:06 Philipy
 * added end game sequence for completing the game with cheating
 * 
 * 37    13/03/98 16:14 Collinsd
 * Added Magic and Version number to save game.
 * 
 * 36    11/03/98 10:55 Philipy
 * all sfx now stopped for pause mode
 * correct level info now displayed in briefing screen
 * correct level now entered after completing a previously saved level
 * level timer now accurate
 * 
 * 35    10/03/98 19:31 Philipy
 * 
 * 34    7/03/98 19:38 Oliverc
 * Added BikeExhausts display option to multiplayer start menu
 * Automatically advance selected game whenever game saved
 * 
 * 33    3/06/98 4:27p Phillipd
 * 
 * 32    6/03/98 15:28 Oliverc
 * Added GetMissionName() routine to convert short level name to full
 * mission name for use in load/save game info display (both in-game in in
 * front end)
 * 
 * 31    5/03/98 21:46 Oliverc
 * Changed save games to use fixed number of slots
 * Added option to delete items from any list (currently enabled only on
 * demos)
 * 
 * 30    2/03/98 17:29 Oliverc
 * Added optional validation function to TEXT structure to filter
 * permitted characters for demo filenames
 * All .DMO files now kept in Demos folder
 * 
 * 29    28/02/98 20:16 Oliverc
 * Saving a game now creates the "savegame" folder if it doesn't exist
 * already before saving
 * 
 * 28    27/02/98 16:30 Collinsd
 * Added relinit sfx code.
 * 
 * 27    27/02/98 15:42 Collinsd
 * Added error check for overwriting sfxid...
 * Also added lives to save game. Ooops
 * 
 * 26    25/02/98 16:04 Collinsd
 * Save Picture for save game...
 * 
 * 25    23/02/98 21:20 Collinsd
 * Optimised Load/Save Functions.
 * 
 * 24    23/02/98 10:35 Collinsd
 * Added Ship Load/Save Functions
 * 
 * 23    21/02/98 20:06 Collinsd
 * Added Enemy Save/Load
 * 
 * 22    21/02/98 13:04 Philipy
 * added in game load / save for sfx
 * 
 * 21    19/02/98 14:19 Oliverc
 * Added load/save for background texture animations
 * 
 * 20    18/02/98 11:50 Collinsd
 * Added load/save for restart points.
 * 
 * 19    18/02/98 10:42 Collinsd
 * Added Save/Load for RemoteCameras.
 * 
 * 18    18/02/98 9:53 Collinsd
 * Added Save/load functions for Timer/Temp guage and trigger areas
 * 
 * 17    17/02/98 16:35 Collinsd
 * Added ExternalForce and Teleports load/save function.
 * 
 * 16    17/02/98 15:41 Collinsd
 * Added Save/Load Functions for RealTime Lights.
 * 
 * 15    17/02/98 15:00 Collinsd
 * Added load/save functions for triggers and xlights..
 * 
 * 14    17/02/98 12:09 Collinsd
 * Added Save/Load BGObjects/CompObjects.
 * 
 * 13    16/02/98 14:33 Collinsd
 * Started LoadSave
 * 
 * 12    1/22/98 10:28a Phillipd
 * 
 * 11    12/20/97 12:43p Phillipd
 * Levelname is now sent across for a level change in Multiplayer....
 * Enemies dont appear in Multiplayer Games....
 * 
 * 10    12/19/97 11:14a Phillipd
 * 
 * 9     12/18/97 5:37p Phillipd
 * 
 * 8     12/18/97 2:46p Phillipd
 * 
 * 7     12/18/97 9:21a Phillipd
 * 
 * 6     12/17/97 5:19p Phillipd
 * 
 * 5     12/16/97 3:24p Phillipd
 * 
 * 4     12/15/97 2:43p Phillipd
 * 
 * 3     11/29/97 4:35p Phillipd
 * Xmem is now in effect...use it allways....
 * 
 * 2     11/26/97 10:44a Phillipd
 * 
 * 1     11/26/97 9:56a Phillipd
 */

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Include Files...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <direct.h>
#include "typedefs.h"
#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "main.h"
#include <dplay.h>
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
#include "file.h"
#include "Triggers.h"
#include "pickups.h"
#include "enemies.h"
#include "mload.h"
#include "magic.h"
#include "Ships.h"
#include "title.h"
#include "2dpolys.h"
#include "polys.h"
#include "screenpolys.h"
#include "primary.h"
#include "secondary.h"
#include "spotfx.h"
#include "pickups.h"
#include "LoadSave.h"
#include "rtlight.h"
#include "teleport.h"
#include "extforce.h"
#include "camera.h"
#include "restart.h"
#include "sfx.h"
#include "text.h"
#include "XMem.h"

/*			Stuff that needs to be saved/loaded.....

		BackGroundObjects...	1				Dave	( DONE )
		Triggers...				2				Olly	( DONE )
		Cameras...				3				Dan		( DONE )
		RealTimeLights...		4				Olly	( DONE )
		XLights					5				Dave	( DONE )
		Current Time...			6				Dan		( DONE )
		Pickups...				7				Dave	( DONE )
		Restart Points...		8				Dave	( DONE )
		Start Points...			9				Dave	( DONE )
		Enemies...				10				Dan
		Trigger Areas...		11				Dan		( DONE )
		External Forces...		12				Dan		( DONE )
		Teleports...			13				Dan		( DONE )
		SpotFx...				14				Dave	( DONE )
		Primary Bullets...		15				Dave	( DONE )
		Secondary Bullets...	16				Dave	( DONE )
		Models ??				17				Dave	( DONE )
		Ships					18				Dan
		ScreenPolys				19				Dave	( DONE )
		2dPolys					20				Dave	( DONE )
		2dTextures				21				Dave	( DONE )
		SFX						22				Phil
		Temperature..			23				Dave	( DONE )
		TextureAnims			24				Olly
 */




/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Defines
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define MAX_SAVEGAME_FILENAME		128


BOOL SaveSnapShot( int8 * Filename );
																   
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Externals ...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern BOOL Cheated;
extern	char biker_name[];
extern	GLOBALSHIP		Ships[ MAX_PLAYERS ];
extern	float framelag;
extern	int CrystalsFound;
extern	int16 Lives;
extern	int16		LevelNum;
extern	int16		NewLevelNum;
extern	char		ShortLevelNames[MAXLEVELS][32];
extern	char		MissionTextNames[MAXLEVELS][128];
extern	float		LevelTimeTaken;
extern	int16		NumInitEnemies;
extern	int16		NumKilledEnemies;
extern	LIST		LoadSavedGameList;
extern float Old_LevelTime_Float;
extern float Old_Time_Float;


void DebugPrintf( const char * format, ... );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Globals ...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int folder_exists( char *pathspec, ... )
{
	static char pathname[ 256 ];
	static struct _stat stat;
	va_list args;

	va_start( args, pathspec );
	vsprintf( pathname, pathspec, args );
	va_end( args );
	if ( _stat( pathname, &stat ) )
	{
		// no such path exists...attempt to create a directory with that name
		return !_mkdir( pathname );
	}
	else if ( stat.st_mode & _S_IFDIR )
	{
		// path exists and is a directory
		return 1;
	}
	else
	{
		// path exists but is not a directory
		return 0;
	}
}


char *SaveGameFileName( int slot )
{
	static char name[ MAX_SAVEGAME_FILENAME ];

	sprintf( name, SAVEGAME_FOLDER "\\save%02d" SAVEGAME_EXTENSION, slot + 1 );
	_strupr( name );
	return name;
}


char *SaveGamePicFileName( int slot )
{
	static char name[ MAX_SAVEGAME_FILENAME ];

	sprintf( name, SAVEGAME_FOLDER "\\save%02d" SAVEGAMEPIC_EXTENSION, slot + 1 );
	_strupr( name );
	return name;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	PreInGame Load
	Input		:	MENUITEM * MenuItem
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int8	LoadGameFilename[ 256 ];

BOOL PreInGameLoad( MENUITEM * MenuItem )
{
	FILE	*	fp;
	int16		i;
	int8		buf[ 256 ];
	uint32		MagicNumber;
	uint32		VersionNumber;

#ifdef SAVEGAME_SLOTS
	fp = fopen( SaveGameFileName( LoadSavedGameList.selected_item ), "rb" );
#else
	sprintf( &LoadGameFilename[ 0 ], "savegame\\%s", &LoadSavedGameList.item[ LoadSavedGameList.selected_item ][ 0 ] );
	fp = fopen( &LoadGameFilename[ 0 ], "rb" );
#endif

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( uint32 ), 1, fp );
		fread( &VersionNumber, sizeof( uint32 ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != LOADSAVE_VERSION_NUMBER  ) )
		{
			fclose( fp );
#ifdef SAVEGAME_SLOTS
			Msg( "PreInGameLoad() Incompatible save game file %s", SaveGameFileName( LoadSavedGameList.selected_item ) );
#else
			Msg( "PreInGameLoad() Incompatible save game file %s", &LoadGameFilename[ 0 ] );
#endif
			return( FALSE );
		}

		i = 0;
		do
		{
			fread( &buf[i], sizeof(char), 1, fp );
		}
		while( buf[ i++ ] != 0 );
		
		for (i = 0; i < NumLevels; i++)
		{

			if( _stricmp( (char*) &ShortLevelNames[i][0] , (char*) &buf[0] ) == 0 ) 
			{
				NewLevelNum = i;
				break;
			}
		}
		
		if( ( NewLevelNum == -1 ) || ( i == 256 ) )
		{
			fclose( fp );
			return FALSE;
		}
		fclose( fp );
		return TRUE;
	}
	return FALSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	InGame Load
	Input		:	MENUITEM * MenuItem
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InGameLoad( MENUITEM * MenuItem )
{
	FILE	*	fp;
	int8		buf[ 256 ];
	int8		biker[ 256 ];
	int16		i;
	int16		Hours, Minutes, Seconds;
	int16		KilledEnemiesNum, InitEnemiesNum;
	LONGLONG	Time_Freq;
	LONGLONG	Time_Value;
	uint32		MagicNumber;
	uint32		VersionNumber;

#ifdef SAVEGAME_SLOTS
	fp = fopen( SaveGameFileName( LoadSavedGameList.selected_item ), "rb" );
#else
	fp = fopen( &LoadGameFilename[ 0 ], "rb" );
#endif

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( uint32 ), 1, fp );
		fread( &VersionNumber, sizeof( uint32 ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != LOADSAVE_VERSION_NUMBER  ) )
		{
			fclose( fp );
#ifdef SAVEGAME_SLOTS
			Msg( "PreInGameLoad() Incompatible save game file %s", SaveGameFileName( LoadSavedGameList.selected_item ) );
#else
			Msg( "PreInGameLoad() Incompatible save game file %s", &LoadGameFilename[ 0 ] );
#endif
			return;
		}

		i = 0;
		do
		{
			fread( &buf[i], sizeof(char), 1, fp );
		}
		while( buf[ i++ ] != 0 );
		i = 0;
		do
		{
			fread( &biker[i], sizeof(char), 1, fp );
		}
		while( biker[ i++ ] != 0 );

		fread( &Hours, sizeof( Hours ), 1, fp );
		fread( &Minutes, sizeof( Minutes ), 1, fp );
		fread( &Seconds, sizeof( Seconds ), 1, fp );
		fread( &InitEnemiesNum, sizeof( NumInitEnemies ), 1, fp );
		fread( &KilledEnemiesNum, sizeof( NumKilledEnemies ), 1, fp );
		fread( &Lives, sizeof( Lives ), 1, fp );

		DebugPrintf( "Loading Ships\n" );

		fp = LoadShips( fp );
		if( !fp ) return;

		DebugPrintf( "Loading Enemies\n" );

		if( !Enemy_Load( fp ) )
			return;

		DebugPrintf( "Loading LoadSFX\n" );

		fp = LoadAllSfx( fp );
		if( !fp ) return;

		DebugPrintf( "Loading Texture Animations\n" );

		fp = LoadTextureAnimations( fp );
		if( !fp ) return;

		DebugPrintf( "Loading RestartPoints\n" );

		fp = LoadStartRestartPoints( fp );
		if( !fp ) return;

		DebugPrintf( "Loading RemoteCameras\n" );

		fp = LoadRemoteCameras( fp );
		if( !fp ) return;

		DebugPrintf( "Loading ScreePolys\n" );

		fp = LoadScreenPolys( fp );
		if( !fp ) return;

		DebugPrintf( "Loading TriggerAreas\n" );

		fp = LoadTriggerAreas( fp );
		if( !fp ) return;

		DebugPrintf( "Loading ExternalForces\n" );

		fp = LoadExternalForces( fp );
		if( !fp ) return;

		DebugPrintf( "Loading Teleports\n" );

		fp = LoadTeleports( fp );
		if( !fp ) return;

		DebugPrintf( "Loading Realtime Lights\n" );

		fp = LoadRealTimeLights( fp );
		if( !fp ) return;

		DebugPrintf( "Loading XLights\n" );

		fp = LoadXLights( fp );
		if( !fp ) return;

		DebugPrintf( "Loading Triggers\n" );

		fp = LoadAllTriggers( fp );
		if( !fp ) return;

		DebugPrintf( "Loading BGObjects\n" );

		fp = LoadBGObjects( fp );
		if( !fp ) return;

		DebugPrintf( "Loading Pickups\n" );

		fp = LoadAllPickups( fp );
		if( !fp ) return;

		DebugPrintf( "Loading PrimBulls\n" );

		fp = LoadPrimBulls( fp );
		if( !fp ) return;

		DebugPrintf( "Loading SecBulls\n" );

		fp = LoadSecBulls( fp );
		if( !fp ) return;

		DebugPrintf( "Loading Models\n" );

		fp = LoadModels( fp );
		if( !fp ) return;

		DebugPrintf( "Loading Polys\n" );

		fp = LoadPolys( fp );
		if( !fp ) return;

		DebugPrintf( "Loading FmPolys\n" );

		fp = LoadFmPolys( fp );
		if( !fp ) return;

		DebugPrintf( "Loading SpotFX\n" );

		fp = LoadAllSpotFX( fp );
		if( !fp ) return;

		DebugPrintf( "Loading Text\n" );

		fp = LoadAllText( fp );
		if( !fp ) return;

		fread( &Cheated, sizeof( BOOL ), 1, fp );

		DebugPrintf( "Loaded OK\n" );
		fclose( fp );

		// init timers...
		QueryPerformanceCounter((LARGE_INTEGER *) &Time_Value);
		QueryPerformanceFrequency((LARGE_INTEGER *) &Time_Freq);
		Old_LevelTime_Float = ( ( Time_Value * 100.0F ) / Time_Freq );
		Old_Time_Float = Old_LevelTime_Float;
	}
}


char *GetMissionName( char *levelname )
{
	static char RealLevelName[ 256 ];
	FILE *f;
	int16 i;

	for (i = 0; i < NumLevels; i++)
	{
		
		if( _stricmp( (char*) &ShortLevelNames[i][0] , levelname ) == 0 ) 
		{
			break;
		}
	}
	if ( i >= NumLevels )
		return levelname;

	f = fopen( MissionTextNames[ i ], "r" );

	RealLevelName[ 0 ] = 0;
	if( f )
	{
		i = 0;
		do
		{
			RealLevelName[ i ] = fgetc( f );
			i++;
		}
		while( ( ( RealLevelName[ i - 1 ] != 13 ) && ( RealLevelName[ i - 1 ] != 10 ) ) );

		if( i )
		{
			RealLevelName[ i - 1 ] = 0;
		}

		fclose( f );
	}
	return RealLevelName;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	InGame Save
	Input		:	MENUITEM * MenuItem
	Output		:	BOOL	True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL InGameSave( MENUITEM * MenuItem )
{
	FILE	*	fp;
	FILE	*	f;
	int16		i;
	int8		RealLevelName[ 128 + 1 ];
	int8		Filename[ 256 ];
	int8		SFilename[ 256 ];
	int16		Hours, Minutes, Seconds;
	int8		sep = 0;
	uint32 IntLevelTimeTaken;
	uint32		MagicNumber = MAGIC_NUMBER;
	uint32		VersionNumber = LOADSAVE_VERSION_NUMBER;

	f = fopen( MissionTextNames[ NewLevelNum ], "r" );

	if( f )
	{
		i = 0;
		do
		{
			RealLevelName[ i ] = fgetc( f );
			i++;
		}
		while( ( ( RealLevelName[ i - 1 ] != 13 ) && ( RealLevelName[ i - 1 ] != 10 ) ) );

		if( i )
		{
			RealLevelName[ i - 1 ] = 0;
		}

		fclose( f );

		IntLevelTimeTaken = ( uint32 ) ( LevelTimeTaken / 100.0F );
		Minutes = ( uint16 ) ( IntLevelTimeTaken / 60 );
		Seconds = ( uint16 ) ( IntLevelTimeTaken % 60 );
		Hours = ( uint16 ) ( Minutes / 60 );
		Minutes = ( uint16 ) ( Minutes % 60 );

#ifdef SAVEGAME_SLOTS
		strcpy( Filename, SaveGameFileName( LoadSavedGameList.selected_item ) );
		strcpy( SFilename, SaveGamePicFileName( LoadSavedGameList.selected_item ) );
#else
		sprintf( &Filename[ 0 ], "savegame\\%s - %hd-%hd-%hd - %hd of %hd Enemies killed.sav", &RealLevelName[ 0 ], Hours, Minutes, Seconds, NumKilledEnemies, NumInitEnemies );
		sprintf( &SFilename[ 0 ], "savegame\\%s - %hd-%hd-%hd - %hd of %hd Enemies killed.ppm", &RealLevelName[ 0 ], Hours, Minutes, Seconds, NumKilledEnemies, NumInitEnemies );
#endif

		DebugPrintf( "%s\n", &Filename[ 0 ] );
	}
	else
	{
		return FALSE;
	}

	if ( !folder_exists( SAVEGAME_FOLDER ) )
	{
		return FALSE;
	}

	if( !SaveSnapShot( &SFilename[ 0 ] ) )
	{
		return FALSE;
	}

	fp = fopen( &Filename[ 0 ], "wb" );

	if( fp != NULL )
	{
		fwrite( &MagicNumber, sizeof( uint32 ), 1, fp );
		fwrite( &VersionNumber, sizeof( uint32 ), 1, fp );

		i = 0;
		do
		{
			fwrite( &ShortLevelNames[NewLevelNum][i], sizeof(char), 1, fp );
		}
		while( ShortLevelNames[NewLevelNum][i++] != 0 );

		fprintf( fp, "%s", biker_name );
		fwrite( &sep, sizeof( sep ), 1, fp );
		fwrite( &Hours, sizeof( Hours ), 1, fp );
		fwrite( &Minutes, sizeof( Minutes ), 1, fp );
		fwrite( &Seconds, sizeof( Seconds ), 1, fp );
		fwrite( &NumInitEnemies, sizeof( NumInitEnemies ), 1, fp );
		fwrite( &NumKilledEnemies, sizeof( NumKilledEnemies ), 1, fp );
		fwrite( &Lives, sizeof( Lives ), 1, fp );

		fp = SaveShips( fp );
		if( !fp ) return FALSE;
		if( !Enemy_Save( fp ) )
			return FALSE;
		fp = SaveAllSfx( fp );
		if( !fp ) return FALSE;
		fp = SaveTextureAnimations( fp );
		if( !fp ) return FALSE;
		fp = SaveStartRestartPoints( fp );
		if( !fp ) return FALSE;
		fp = SaveRemoteCameras( fp );
		if( !fp ) return FALSE;
		fp = SaveScreenPolys( fp );
		if( !fp ) return FALSE;
		fp = SaveTriggerAreas( fp );
		if( !fp ) return FALSE;
		fp = SaveExternalForces( fp );
		if( !fp ) return FALSE;
		fp = SaveTeleports( fp );
		if( !fp ) return FALSE;
		fp = SaveRealTimeLights( fp );
		if( !fp ) return FALSE;
		fp = SaveXLights( fp );
		if( !fp ) return FALSE;
		fp = SaveAllTriggers( fp );
		if( !fp ) return FALSE;
		fp = SaveBGObjects( fp );
		if( !fp ) return FALSE;
		fp = SaveAllPickups( fp );
		if( !fp ) return FALSE;
		fp = SavePrimBulls( fp );
		if( !fp ) return FALSE;
		fp = SaveSecBulls( fp );
		if( !fp ) return FALSE;
		fp = SaveModels( fp );
		if( !fp ) return FALSE;
		fp = SavePolys( fp );
		if( !fp ) return FALSE;
		fp = SaveFmPolys( fp );
		if( !fp ) return FALSE;
		fp = SaveAllSpotFX( fp );
		if( !fp ) return FALSE;
		fp = SaveAllText( fp );
		if( !fp ) return FALSE;
		
		fwrite( &Cheated, sizeof( BOOL ), 1, fp );

		fclose( fp );
		return TRUE;
	}
	return FALSE;
}


char *SavedGameInfo( int slot )
{
	static char info[ 256 ];
	FILE	*	fp;
	int8		buf[ 256 ];
	int8		biker[ 256 ];
	int16		i;
	int16		Hours, Minutes, Seconds;
	int16		KilledEnemiesNum, InitEnemiesNum;
	int16		Lives;
	char	*	LevelName;
	uint32		MagicNumber;
	uint32		VersionNumber;

	fp = fopen( SaveGameFileName( slot ), "rb" );

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( uint32 ), 1, fp );
		fread( &VersionNumber, sizeof( uint32 ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != LOADSAVE_VERSION_NUMBER  ) )
		{
			fclose( fp );
			sprintf( info, "unused slot %d", slot + 1 );
			return info;
		}

		i = 0;
		do
		{
			fread( &buf[i], sizeof(char), 1, fp );
		}
		while( buf[ i++ ] != 0 );
		i = 0;
		do
		{
			fread( &biker[i], sizeof(char), 1, fp );
		}
		while( biker[ i++ ] != 0 );

		LevelName = GetMissionName( buf );

		fread( &Hours, sizeof( Hours ), 1, fp );
		fread( &Minutes, sizeof( Minutes ), 1, fp );
		fread( &Seconds, sizeof( Seconds ), 1, fp );
		fread( &InitEnemiesNum, sizeof( NumInitEnemies ), 1, fp );
		fread( &KilledEnemiesNum, sizeof( NumKilledEnemies ), 1, fp );
		fread( &Lives, sizeof( Lives ), 1, fp );
		if ( _stricmp( biker, DEFAULT_PLAYER_NAME ) )
			sprintf( info, "%s in %s %hd'%02hd %hd/%hd",
				biker,
				LevelName,
				Hours * 60 + Minutes, Seconds,
				KilledEnemiesNum, InitEnemiesNum );
		else
			sprintf( info, "%s %hd'%02hd %hd/%hd",
				LevelName,
				Hours * 60 + Minutes, Seconds,
				KilledEnemiesNum, InitEnemiesNum );
		fclose( fp );
	}
	else
	{
		sprintf( info, "unused slot %d", slot + 1 );
	}

	return info;
}


BOOL SaveGameSlotUsed( int slot )
{
	FILE	*	fp;
	uint32		MagicNumber;
	uint32		VersionNumber;

	fp = fopen( SaveGameFileName( slot ), "rb" );
	if ( fp )
	{
		fread( &MagicNumber, sizeof( uint32 ), 1, fp );
		fread( &VersionNumber, sizeof( uint32 ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != LOADSAVE_VERSION_NUMBER  ) )
		{
			fclose( fp );
			return FALSE;
		}

		fclose( fp );
		return TRUE;
	}
	return FALSE;
}
