
#ifndef LOADSAVE_INCLUDED
#define LOADSAVE_INCLUDED
/*
 * Defines
 */
//#define SAVEGAME_SLOTS
#include "main.h"

#define SAVEGAME_FOLDER			"Savegame"
#define SNAPSHOT_FOLDER			"ScreenShots"
#define FMVSNAPSHOT_FOLDER		"ScreenShots"
#define SAVEGAME_EXTENSION		".SAV"
#define SAVEGAME_FILESPEC		"save??"
#define SAVEGAMEPIC_EXTENSION	".PPM"

#define	LOADSAVE_VERSION_NUMBER	5

/*
 * fn prototypes
 */
void InGameLoad( MENUITEM * MenuItem );
bool InGameSave( MENUITEM * MenuItem );
bool PreInGameLoad( MENUITEM * MenuItem );
char *SaveGameFileName( int slot );
char *SaveGamePicFileName( int slot );
char *SavedGameInfo( int slot );
char *GetMissionName( char *levelname );
bool SaveGameSlotUsed( int slot );
#endif	// LOADSAVE_INCLUDED

