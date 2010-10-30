// TODO: this code loads pointers !!!!!!!!!!
// apparrently this is somehow possible if you save/load from save binary

/*===================================================================
		Include Files...	
===================================================================*/
#define SAVEGAME_SLOTS
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "main.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "file.h"
#include "triggers.h"
#include "pickups.h"
#include "enemies.h"
#include "mload.h"

#include "ships.h"
#include "title.h"
#include "2dpolys.h"
#include "polys.h"
#include "screenpolys.h"
#include "primary.h"
#include "secondary.h"
#include "spotfx.h"
#include "pickups.h"
#include "loadsave.h"
#include "rtlight.h"
#include "teleport.h"
#include "extforce.h"
#include "camera.h"
#include "restart.h"
#include "sfx.h"
#include "text.h"
#include "xmem.h"
#include "file.h"
#include "util.h"
#include "timer.h"

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




/*===================================================================
		Defines
===================================================================*/
#define MAX_SAVEGAME_FILENAME		128


BOOL SaveSnapShot( int8 * Filename );
																   
/*===================================================================
		Externals ...
===================================================================*/
extern BOOL Cheated;
extern	char biker_name[];
extern	GLOBALSHIP		Ships[ MAX_PLAYERS+1 ];
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

/*===================================================================
		Globals ...
===================================================================*/


char *SaveGameFileName( int slot )
{
	static char name[ MAX_SAVEGAME_FILENAME ];

	sprintf( name, SAVEGAME_FOLDER "\\save%02d" SAVEGAME_EXTENSION, slot + 1 );
	strtoupper( name );
	return name;
}


char *SaveGamePicFileName( int slot )
{
	static char name[ MAX_SAVEGAME_FILENAME ];

	sprintf( name, SAVEGAME_FOLDER "\\save%02d" SAVEGAMEPIC_EXTENSION, slot + 1 );
	strtoupper( name );
	return name;
}

/*===================================================================
	Procedure	:	PreInGame Load
	Input		:	MENUITEM * MenuItem
	Output		:	nothing
===================================================================*/
int8	LoadGameFilename[ 256 ];
extern	int16			NumLevels;

BOOL PreInGameLoad( MENUITEM * MenuItem )
{
	FILE	*	fp;
	int16		i;
	int8		buf[ 256 ];
	uint32		MagicNumber;
	uint32		VersionNumber;

#ifdef SAVEGAME_SLOTS
	fp = file_open( SaveGameFileName( LoadSavedGameList.selected_item ), "rb" );
#else
	sprintf( &LoadGameFilename[ 0 ], "savegame\\%s", &LoadSavedGameList.item[ LoadSavedGameList.selected_item ][ 0 ] );
	fp = file_open( &LoadGameFilename[ 0 ], "rb" );
#endif

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( uint32 ), 1, fp );
		fread( &VersionNumber, sizeof( uint32 ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != LOADSAVE_VERSION_NUMBER  ) )
			Msg("Save file is in an old format.\nYou will most likely crash...\n");
/*
		{
			fclose( fp );
#ifdef SAVEGAME_SLOTS
			Msg( "PreInGameLoad() Incompatible save game file %s", SaveGameFileName( LoadSavedGameList.selected_item ) );
#else
			Msg( "PreInGameLoad() Incompatible save game file %s", &LoadGameFilename[ 0 ] );
#endif
			return( FALSE );
		}
*/

		i = 0;
		do
		{
			fread( &buf[i], sizeof(char), 1, fp );
		}
		while( buf[ i++ ] != 0 );
		
		for (i = 0; i < NumLevels; i++)
		{

			if( strcasecmp( (char*) &ShortLevelNames[i][0] , (char*) &buf[0] ) == 0 ) 
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

/*===================================================================
	Procedure	:	InGame Load
	Input		:	MENUITEM * MenuItem
	Output		:	nothing
===================================================================*/
extern px_timer_t countdown_timer;
void InGameLoad( MENUITEM * MenuItem )
{
	FILE	*	fp;
	int8		buf[ 256 ];
	int8		biker[ 256 ];
	int16		i;
	int16		Hours, Minutes, Seconds;
	int16		KilledEnemiesNum, InitEnemiesNum;
	uint32		MagicNumber;
	uint32		VersionNumber;

#ifdef SAVEGAME_SLOTS
	fp = file_open( SaveGameFileName( LoadSavedGameList.selected_item ), "rb" );
#else
	fp = file_open( &LoadGameFilename[ 0 ], "rb" );
#endif

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( uint32 ), 1, fp );
		fread( &VersionNumber, sizeof( uint32 ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != LOADSAVE_VERSION_NUMBER  ) )
			Msg("Save file is in an old format.\nYou will most likely crash...\n");
/*
		{
			fclose( fp );
#ifdef SAVEGAME_SLOTS
			Msg( "PreInGameLoad() Incompatible save game file %s", SaveGameFileName( LoadSavedGameList.selected_item ) );
#else
			Msg( "PreInGameLoad() Incompatible save game file %s", &LoadGameFilename[ 0 ] );
#endif
			return;
		}
*/

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

		timer_clear( &countdown_timer );
	}
}


char *GetMissionName( char *levelname )
{
	static char RealLevelName[ 256 ];
	FILE *f;
	int16 i;

	for (i = 0; i < NumLevels; i++)
	{
		
		if( strcasecmp( (char*) &ShortLevelNames[i][0] , levelname ) == 0 ) 
		{
			break;
		}
	}
	if ( i >= NumLevels )
		return levelname;

	f = file_open( MissionTextNames[ i ], "r" );

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


/*===================================================================
	Procedure	:	InGame Save
	Input		:	MENUITEM * MenuItem
	Output		:	BOOL	True/False
===================================================================*/
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
	uint32		MagicNumber = MAGIC_NUMBER;
	uint32		VersionNumber = LOADSAVE_VERSION_NUMBER;

	f = file_open( MissionTextNames[ NewLevelNum ], "r" );

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

		Minutes = ( uint16 ) ( LevelTimeTaken / 60 );
		Seconds = ( uint16 ) LevelTimeTaken;
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

	fp = file_open( &Filename[ 0 ], "wb" );

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

	fp = file_open( SaveGameFileName( slot ), "rb" );

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( uint32 ), 1, fp );
		fread( &VersionNumber, sizeof( uint32 ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != LOADSAVE_VERSION_NUMBER  ) )
			Msg("Save file is in an old format.\nYou will most likely crash...\n");
		/*
		{
			fclose( fp );
			sprintf( info, "unused slot %d", slot + 1 );
			return info;
		}
		*/

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
		if ( strcasecmp( biker, DEFAULT_PLAYER_NAME ) )
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

	fp = file_open( SaveGameFileName( slot ), "rb" );
	if ( fp )
	{
		fread( &MagicNumber, sizeof( uint32 ), 1, fp );
		fread( &VersionNumber, sizeof( uint32 ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != LOADSAVE_VERSION_NUMBER  ) )
			Msg("Save file is in an old format.\nYou will most likely crash...\n");
		/*
		{
			fclose( fp );
			return FALSE;
		}
		*/

		fclose( fp );
		return TRUE;
	}
	return FALSE;
}
