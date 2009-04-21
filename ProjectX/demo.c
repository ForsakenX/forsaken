
#include <stdio.h>
#include "windows.h"
#include "typedefs.h"
#include "main.h"
#include <dplay.h>
#include "comm.h"
#include "d3dappi.h"
#include "title.h"
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
#include "winsock.h"
#include "MultiPlayer.h"
#include "mxload.h"
#include "mxaload.h"
#include <stdio.h>
#include "Local.h"
#include "text.h"
#include "ships.h"
#include "LoadSave.h"
#include "pickups.h"
#include "primary.h"
#include "controls.h"
#include "xmem.h"
#include "util.h"
#include "demo.h"
#include "file.h"


extern BYTE TeamNumber[MAX_PLAYERS];
extern LIST DemoList;
extern BOOL TeamGame;
extern BOOL CTF;
extern BOOL CaptureTheFlag;
extern BOOL BountyHunt;
extern	uint16	RandomStartPosModify;
extern int CameraStatus;  


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
	_strupr( filename );
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
	_strupr( demoname );
	dmo = strstr( demoname, DEMOFILE_EXTENSION );
	if ( dmo )
		*dmo = 0;

	return demoname;
}

void StartDemoCleaning( MENUITEM * Item )
{
	char buf[256];
	int i;
	uint32 mp_version;
	uint32 flags;
	char *clean_name;
	uint16	TempSeed1, TempSeed2;
	BOOL	TempRandomPickups;
	uint32	TempPackedInfo[ MAX_PICKUPFLAGS ];

	NewLevelNum = -1;

	memset (TeamNumber, 255, sizeof(BYTE) * MAX_PLAYERS);
	DemoFp = fopen( DemoFileName( DemoList.item[DemoList.selected_item] ) , "rb" );
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
	TeamGame = ( flags & TeamGameBit ) ? TRUE : FALSE;
	CTF = ( flags & CTFGameBit ) ? TRUE : FALSE;
	CaptureTheFlag = ( flags & FlagGameBit ) ? TRUE : FALSE;
	BountyHunt = ( flags & BountyGameBit ) ? TRUE : FALSE;

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

		if( _stricmp( (char*) &ShortLevelNames[i][0] , (char*) &buf[0] ) == 0 )
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
//	DemoFpClean = fopen( DemoFileName( DemoGameName.text ) , "wbc" );
	DemoFpClean = fopen( clean_name , "wbc" );
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
	if ( !DeleteFile( DemoFileName( DemoList.item[DemoList.selected_item] ) ) )
	{
		DebugPrintf( "DeleteFile( %s ) failed\n", DemoFileName( DemoList.item[DemoList.selected_item] ) );
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
}
