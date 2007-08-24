/*
 * The X Men, November 1997
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 14 $
 *
 * $Header: /PcProjectX/Include/LoadSave.h 14    27/08/98 10:20 Collinsd $
 *
 * $Log: /PcProjectX/Include/LoadSave.h $
 * 
 * 14    27/08/98 10:20 Collinsd
 * Hack to fix invisible enemies colliding with background objects.
 * 
 * 13    8/04/98 10:22 Collinsd
 * Screenshots folder created if doesn't exist.
 * 
 * 12    28/03/98 20:50 Collinsd
 * BGObjects now use collision zone bounding box.
 * Also fixed spotfx playing when paused.
 * 
 * 11    27/03/98 12:58 Oliverc
 * Doubled size of trigmodque (BiaS)
 * 
 * 10    26/03/98 9:23 Collinsd
 * Updated LoadSave Version
 * 
 * 9     13/03/98 16:15 Collinsd
 * Added Magic and Version number to save game.
 * 
 * 8     7/03/98 19:39 Oliverc
 * Added BikeExhausts display option to multiplayer start menu
 * Automatically advance selected game whenever game saved
 * 
 * 7     6/03/98 15:29 Oliverc
 * Added GetMissionName() routine to convert short level name to full
 * mission name for use in load/save game info display (both in-game in in
 * front end)
 * 
 * 6     5/03/98 21:46 Oliverc
 * Changed save games to use fixed number of slots
 * Added option to delete items from any list (currently enabled only on
 * demos)
 * 
 * 5     23/02/98 10:35 Collinsd
 * Added Ship Load/Save Functions
 * 
 * 4     16/02/98 14:34 Collinsd
 * Started LoadSave
 * 
 * 3     12/17/97 5:19p Phillipd
 * 
 * 2     11/26/97 10:44a Phillipd
 * 
 * 1     11/26/97 9:57a Phillipd
 * 
 * 
 */
#ifndef LOADSAVE_INCLUDED
#define LOADSAVE_INCLUDED
/*
 * Defines
 */
#define SAVEGAME_SLOTS

#define SAVEGAME_FOLDER			"Savegame"
#define SNAPSHOT_FOLDER			"Screenshots"
#define FMVSNAPSHOT_FOLDER		"Screens"
#define SAVEGAME_EXTENSION		".SAV"
#define SAVEGAME_FILESPEC		"save??"
#define SAVEGAMEPIC_EXTENSION	".PPM"

#define	LOADSAVE_VERSION_NUMBER	5

/*
 * fn prototypes
 */
void InGameLoad( MENUITEM * MenuItem );
BOOL InGameSave( MENUITEM * MenuItem );
BOOL PreInGameLoad( MENUITEM * MenuItem );
char *SaveGameFileName( int slot );
char *SaveGamePicFileName( int slot );
char *SavedGameInfo( int slot );
char *GetMissionName( char *levelname );
BOOL SaveGameSlotUsed( int slot );
#endif	// LOADSAVE_INCLUDED

