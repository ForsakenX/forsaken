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
#include "file.h"
#include "util.h"
#include "timer.h"
#include "oct2.h"

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


bool SaveSnapShot( int8_t * Filename );
																   
/*===================================================================
		Externals ...
===================================================================*/
extern bool Cheated;
extern	char biker_name[];
extern	float framelag;
extern	int CrystalsFound;
extern	int16_t Lives;
extern	int16_t		LevelNum;
extern	char		MissionTextNames[MAXLEVELS][128];
extern	float		LevelTimeTaken;
extern	int16_t		NumInitEnemies;
extern	int16_t		NumKilledEnemies;
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
int8_t	LoadGameFilename[ 256 ];
extern	int16_t			NumLevels;

bool PreInGameLoad( MENUITEM * MenuItem )
{
	FILE	*	fp;
	int16_t		i;
	int8_t		buf[ 256 ];
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;

#ifdef SAVEGAME_SLOTS
	fp = file_open( SaveGameFileName( LoadSavedGameList.selected_item ), "rb" );
#else
	sprintf( &LoadGameFilename[ 0 ], "savegame\\%s", &LoadSavedGameList.item[ LoadSavedGameList.selected_item ][ 0 ] );
	fp = file_open( &LoadGameFilename[ 0 ], "rb" );
#endif

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

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
			return( false );
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
			return false;
		}
		fclose( fp );
		return true;
	}
	return false;
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
	int8_t		buf[ 256 ];
	int8_t		biker[ 256 ];
	int16_t		i;
	int16_t		Hours, Minutes, Seconds;
	int16_t		KilledEnemiesNum, InitEnemiesNum;
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;

#ifdef SAVEGAME_SLOTS
	fp = file_open( SaveGameFileName( LoadSavedGameList.selected_item ), "rb" );
#else
	fp = file_open( &LoadGameFilename[ 0 ], "rb" );
#endif

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

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

		fread( &Cheated, sizeof( bool ), 1, fp );

		DebugPrintf( "Loaded OK\n" );
		fclose( fp );

		timer_clear( &countdown_timer );
	}
}


char *GetMissionName( char *levelname )
{
	static char RealLevelName[ 256 ];
	FILE *f;
	int16_t i;

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
	Output		:	bool	True/False
===================================================================*/
bool InGameSave( MENUITEM * MenuItem )
{
	FILE	*	fp;
	FILE	*	f;
	int16_t		i;
	int8_t		RealLevelName[ 128 + 1 ];
	int8_t		Filename[ 256 ];
	int8_t		SFilename[ 256 ];
	int16_t		Hours, Minutes, Seconds;
	int8_t		sep = 0;
	u_int32_t		MagicNumber = MAGIC_NUMBER;
	u_int32_t		VersionNumber = LOADSAVE_VERSION_NUMBER;

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

		Minutes = ( u_int16_t ) ( LevelTimeTaken / 60 );
		Seconds = ( u_int16_t ) LevelTimeTaken;
		Hours = ( u_int16_t ) ( Minutes / 60 );
		Minutes = ( u_int16_t ) ( Minutes % 60 );

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
		return false;
	}

	if ( !folder_exists( SAVEGAME_FOLDER ) )
	{
		return false;
	}

	if( !SaveSnapShot( &SFilename[ 0 ] ) )
	{
		return false;
	}

	fp = file_open( &Filename[ 0 ], "wb" );

	if( fp != NULL )
	{
		fwrite( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fwrite( &VersionNumber, sizeof( u_int32_t ), 1, fp );

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
		if( !fp ) return false;
		if( !Enemy_Save( fp ) )
			return false;
		fp = SaveAllSfx( fp );
		if( !fp ) return false;
		fp = SaveTextureAnimations( fp );
		if( !fp ) return false;
		fp = SaveStartRestartPoints( fp );
		if( !fp ) return false;
		fp = SaveRemoteCameras( fp );
		if( !fp ) return false;
		fp = SaveScreenPolys( fp );
		if( !fp ) return false;
		fp = SaveTriggerAreas( fp );
		if( !fp ) return false;
		fp = SaveExternalForces( fp );
		if( !fp ) return false;
		fp = SaveTeleports( fp );
		if( !fp ) return false;
		fp = SaveRealTimeLights( fp );
		if( !fp ) return false;
		fp = SaveXLights( fp );
		if( !fp ) return false;
		fp = SaveAllTriggers( fp );
		if( !fp ) return false;
		fp = SaveBGObjects( fp );
		if( !fp ) return false;
		fp = SaveAllPickups( fp );
		if( !fp ) return false;
		fp = SavePrimBulls( fp );
		if( !fp ) return false;
		fp = SaveSecBulls( fp );
		if( !fp ) return false;
		fp = SaveModels( fp );
		if( !fp ) return false;
		fp = SavePolys( fp );
		if( !fp ) return false;
		fp = SaveFmPolys( fp );
		if( !fp ) return false;
		fp = SaveAllSpotFX( fp );
		if( !fp ) return false;
		fp = SaveAllText( fp );
		if( !fp ) return false;
		
		fwrite( &Cheated, sizeof( bool ), 1, fp );

		fclose( fp );
		return true;
	}
	return false;
}


char *SavedGameInfo( int slot )
{
	static char info[ 256 ];
	FILE	*	fp;
	int8_t		buf[ 256 ];
	int8_t		biker[ 256 ];
	int16_t		i;
	int16_t		Hours, Minutes, Seconds;
	int16_t		KilledEnemiesNum, InitEnemiesNum;
	int16_t		Lives;
	char	*	LevelName;
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;

	fp = file_open( SaveGameFileName( slot ), "rb" );

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

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


bool SaveGameSlotUsed( int slot )
{
	FILE	*	fp;
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;

	fp = file_open( SaveGameFileName( slot ), "rb" );
	if ( fp )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != LOADSAVE_VERSION_NUMBER  ) )
			Msg("Save file is in an old format.\nYou will most likely crash...\n");
		/*
		{
			fclose( fp );
			return false;
		}
		*/

		fclose( fp );
		return true;
	}
	return false;
}
