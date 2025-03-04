
#include <stdio.h>

#include "main.h"
#include "net.h"
#include "render.h"
#include "title.h"

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "multiplayer.h"
#include "mxload.h"
#include "mxaload.h"
#include <stdio.h>
#include "local.h"
#include "text.h"
#include "ships.h"
#include "loadsave.h"
#include "pickups.h"
#include "primary.h"
#include "controls.h"
#include "util.h"
#include "demo.h"
#include "file.h"


extern BYTE TeamNumber[MAX_PLAYERS];
extern LIST DemoList;
extern bool TeamGame;
extern bool CTF;
extern bool CaptureTheFlag;
extern bool BountyHunt;
extern u_int16_t RandomStartPosModify;
extern int CameraStatus;  
extern bool CountDownOn;
extern bool DemoShipInit[];
extern u_int16_t CopyOfSeed1;
extern u_int16_t CopyOfSeed2;
extern bool RandomPickups;
extern bool PlayDemo;
extern void DebugLastError( void );
extern bool ChangeLevel( void );

FILE	*	DemoFp = NULL;
FILE	*	DemoFpClean = NULL;

char *DemoFileName( char *demoname )
{
	static char filename[ MAX_DEMOFILENAME_LENGTH  ];
	char *dmo;

	folder_exists( DEMOFOLDER );
	strcpy( filename, DEMOFOLDER );
	strncat( filename, "\\", MAX_DEMONAME_LENGTH );
	strncat( filename, demoname, MAX_DEMONAME_LENGTH );
	filename[ strlen( DEMOFOLDER ) + 1 + MAX_DEMONAME_LENGTH ] = 0;
	strtoupper( filename );
	dmo = strstr( filename, DEMOFILE_EXTENSION );
	if ( !dmo )
		strcat( filename, DEMOFILE_EXTENSION );

	return filename;
}

char *DemoName( char *demofilename )
{
	static char demoname[ MAX_DEMONAME_LENGTH + 4 ];
	char *dmo;
	int offset;

	offset = strstr( demofilename, DEMOFOLDER ) ? strlen( DEMOFOLDER ) + 1 : 0;
	strncpy( demoname, demofilename + offset, sizeof( demoname ) - 1 );
	demoname[ sizeof( demoname ) - 1 ] = 0;
	strtoupper( demoname );
	dmo = strstr( demoname, DEMOFILE_EXTENSION );
	if ( dmo )
		*dmo = 0;

	return demoname;
}

void StartDemoCleaning( MENUITEM * Item )
{
#ifdef DEMO_SUPPORT
	char buf[256];
	int i;
	u_int32_t mp_version;
	u_int32_t flags;
	char *clean_name;
	u_int16_t	TempSeed1, TempSeed2;
	bool	TempRandomPickups;
	u_int32_t	TempPackedInfo[ MAX_PICKUPFLAGS ];

	NewLevelNum = -1;

	memset (TeamNumber, 255, sizeof(BYTE) * MAX_PLAYERS);
	DemoFp = file_open( DemoFileName( DemoList.item[DemoList.selected_item] ) , "rb" );
	if ( !DemoFp )
	{
		// can't open file
		return;
	}

	setvbuf( DemoFp, NULL, _IONBF , 0 );		// size of stream buffer...
	
	fread( &mp_version, sizeof( mp_version ), 1, DemoFp );
	if ( (mp_version > MULTIPLAYER_VERSION) || (mp_version < DEMO_MULTIPLAYER_VERSION) )
	{
		// incompatible multiplayer version
		fclose( DemoFp );
		return;
	}

	fread( &TempSeed1, sizeof( TempSeed1 ), 1, DemoFp );
	fread( &TempSeed2, sizeof( TempSeed2 ), 1, DemoFp );
	fread( &TempRandomPickups, sizeof( TempRandomPickups ), 1, DemoFp );
	fread( &TempPackedInfo[ 0 ], sizeof( TempPackedInfo ), 1, DemoFp );

	fread( &flags, sizeof( flags ), 1, DemoFp );
	TeamGame = ( flags & TeamGameBit ) ? true : false;
	CTF = ( flags & CTFGameBit ) ? true : false;
	CaptureTheFlag = ( flags & FlagGameBit ) ? true : false;
	BountyHunt = ( flags & BountyGameBit ) ? true : false;

	fread( &RandomStartPosModify, sizeof( RandomStartPosModify ), 1, DemoFp );

	for( i = 0 ; i < 256 ; i++ )
	{
		fread( &buf[i], sizeof(char), 1, DemoFp );
		if( buf[i] == 0 )
		{
			break;
		}
	}
	
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
		fclose( DemoFp );
		return;
	}

	clean_name = _tempnam( ".", "dmo" );
	if ( !clean_name )
	{
		// unable to create unique temporary filename
		return;
	}
	DebugPrintf( "temp demo clean name = %s\n", clean_name );
//	DemoFpClean = file_open( DemoFileName( DemoGameName.text ) , "wbc" );
	DemoFpClean = file_open( clean_name , "wbc" );
	setvbuf( DemoFpClean, NULL, _IONBF , 0 );		// size of stream buffer...

	fwrite( &mp_version, sizeof( mp_version ), 1, DemoFpClean );

	fwrite( &TempSeed1, sizeof( TempSeed1 ), 1, DemoFpClean );
	fwrite( &TempSeed2, sizeof( TempSeed2 ), 1, DemoFpClean );
	fwrite( &TempRandomPickups, sizeof( TempRandomPickups ), 1, DemoFpClean );
	fwrite( &TempPackedInfo[ 0 ], sizeof( TempPackedInfo ), 1, DemoFpClean );

	fwrite( &flags, sizeof( flags ), 1, DemoFpClean );
	fwrite( &RandomStartPosModify, sizeof( RandomStartPosModify ), 1, DemoFpClean );
	for( i = 0 ; i < 256 ; i++ )
	{
		fwrite( &buf[i], sizeof(char), 1, DemoFpClean );
		if( buf[i] == 0 )
		{
			break;
		}
	}

	DemoClean();
	
	fclose( DemoFp );
	fclose( DemoFpClean );
	if ( !delete_file( DemoFileName( DemoList.item[DemoList.selected_item] ) ) )
	{
		DebugPrintf( "delete_file( %s ) failed\n", DemoFileName( DemoList.item[DemoList.selected_item] ) );
		DebugLastError();
	}
	if ( !MoveFile( clean_name, DemoFileName( DemoList.item[DemoList.selected_item] ) ) )
	{
		DebugPrintf( "MoveFile( %s, %s ) failed\n",
			clean_name, DemoFileName( DemoList.item[DemoList.selected_item] ) );
		DebugLastError();
	}
	free( clean_name );

	if (CameraStatus != CAMERA_AtStart)
		MenuBack();
	else
		MenuExit();
#endif
}

void StartDemoPlayback( MENUITEM * Item )
{
#ifdef DEMO_SUPPORT
	char buf[256];
	int i;
	u_int32_t mp_version;
	u_int32_t flags;
	u_int32_t PackedInfo[ MAX_PICKUPFLAGS ];

	TeamGame = false;
	CountDownOn = false;

	if( DemoList.item[0][0] == 0 )
	{
		// there are no demos...
		return;
	}
	LevelNum = -1;
	NewLevelNum = -1;

	for ( i = 0; i < MAX_PLAYERS; i++ )
		DemoShipInit[ i ] = false;
	DemoShipInit[ MAX_PLAYERS ] = true;
	memset (TeamNumber, 255, sizeof(BYTE) * MAX_PLAYERS);

	DemoFp = file_open( DemoFileName( DemoList.item[DemoList.selected_item] ) , "rb" );

	if( !DemoFp )
	{
		// Couldnt find the selected demo...
		return;
	}

	setvbuf( DemoFp, NULL, _IONBF , 0 );		// size of stream buffer...
	

	fread( &mp_version, sizeof( mp_version ), 1, DemoFp );
	if ( (mp_version > MULTIPLAYER_VERSION) || (mp_version < DEMO_MULTIPLAYER_VERSION) )
	{
		// incompatible multiplayer version
		fclose( DemoFp );
		return;
	}

	fread( &CopyOfSeed1, sizeof( CopyOfSeed1 ), 1, DemoFp );
	fread( &CopyOfSeed2, sizeof( CopyOfSeed2 ), 1, DemoFp );
	fread( &RandomPickups, sizeof( RandomPickups ), 1, DemoFp );
	fread( &PackedInfo[ 0 ], sizeof( PackedInfo ), 1, DemoFp );
	UnpackPickupInfo( &PackedInfo[ 0 ] );

	fread( &flags, sizeof( flags ), 1, DemoFp );
	TeamGame = ( flags & TeamGameBit ) ? true : false;
	CTF = ( flags & CTFGameBit ) ? true : false;
	CaptureTheFlag = ( flags & FlagGameBit ) ? true : false;
	BountyHunt = ( flags & BountyGameBit ) ? true : false;

	fread( &RandomStartPosModify, sizeof( RandomStartPosModify ), 1, DemoFp );

	for( i = 0 ; i < 256 ; i++ )
	{
		fread( &buf[i], sizeof(char), 1, DemoFp );
		if( buf[i] == 0 )
		{
			break;
		}
	}
	
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
		fclose( DemoFp );
		return;
	}
	MenuAbort();
	ReleaseView();
	DestroySound( DESTROYSOUND_All );	// ReleaseView will not do a DestroySound if MyGameStatus == STATUS_Title

	PlayDemo = true;

	MyGameStatus = STATUS_ChangeLevelPostPlayingDemo;
	WhoIAm = MAX_PLAYERS;

//	RandomStartPosModify = 0;
	SetupNetworkGame();

	ChangeLevel();
#endif
}
