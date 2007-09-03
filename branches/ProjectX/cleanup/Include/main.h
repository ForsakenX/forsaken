/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 58 $
 *
 * $Header: /PcProjectX/include/main.h 58    14/09/98 15:35 Philipy $
 *
 * $Log: /PcProjectX/include/main.h $
 * 
 * 58    14/09/98 15:35 Philipy
 * added facility for server based collisions
 * 
 * 57    5/08/98 11:04 Philipy
 * added AutoStart facility ( joins game based on GUID in registery )
 * upped patch version to 1.02
 * 
 * 56    31/07/98 16:17 Philipy
 * added PPS to session desc
 * 
 * 55    28/07/98 14:44 Philipy
 * all server timeouts now configurable
 * 
 * 54    28/07/98 10:39 Philipy
 * Max players now works properly for server games
 * 
 * 53    7/08/98 10:10a Phillipd
 * 
 * 52    7/03/98 4:26p Phillipd
 * 
 * 51    6/22/98 2:16p Phillipd
 * 
 * 50    16/06/98 16:33 Philipy
 * more lobby / join game stuff
 * 
 * 49    9/06/98 11:23 Philipy
 * server now reset if pseudohost does not have any valid levels
 * fixed timed game bug for server based games
 * 
 * 48    8/06/98 12:28 Philipy
 * server levels now sent across to pseudohost. Pseudohost can only pick
 * valid levels
 * 
 * 47    22/05/98 17:51 Philipy
 * more work on session info
 * 
 * 46    20/05/98 9:39 Philipy
 * implemented front end server menus
 * removed ( invalid ) ping from sessions menu
 * changed EnumPlayers so that it solely uses MSG_NAME
 * 
 * 45    11/05/98 15:15 Philipy
 * added session info stuff ( game type, ping etc )
 * 
 * 44    5/07/98 10:30a Phillipd
 * 
 * 43    4/27/98 4:02p Phillipd
 * 
 * 42    4/06/98 7:06p Phillipd
 * 
 * 41    4/04/98 14:23 Philipy
 * mode scaling stuff is now calculated rather than based on fixed values
 * added -NoBlitTextScaling option to ReadIni and command line options
 * 
 * 40    16/03/98 19:36 Collinsd
 * Changed Clipping Distance when nitro used. ( Fixes Laser )
 * 
 * 39    6/03/98 17:37 Philipy
 * implemented ability to run when launched by lobby
 * 
 * 38    4/03/98 12:33 Oliverc
 * CTF mode fully enabled
 * 
 * 37    3/02/98 12:30p Phillipd
 * 
 * 36    24/02/98 16:56 Oliverc
 * 1st attempt at bounty hunt multiplayer game
 * 
 * 35    2/23/98 2:02p Phillipd
 * Load Save now works.
 * 
 * 34    23/02/98 10:37 Philipy
 * added inter level stuff
 * 
 * 33    21/02/98 16:25 Philipy
 * added text messages for capture flag
 * 
 * 32    20/02/98 15:30 Philipy
 * re-implented AVI
 * splash screens can now play demos and AVIs
 * 
 * 31    27/01/98 14:08 Oliverc
 * Added MindSpring splash screen
 * 
 * 30    26/01/98 18:23 Philipy
 * fixed video memory leaks
 * splash screens now display after release view, and call InitScene,
 * InitView after  completion
 * 
 * 29    21/01/98 12:19 Philipy
 * Added attract mode for shareware
 * fixed looping sfx volume bug
 * 
 * 28    15/01/98 18:55 Philipy
 * added new splash screens
 * 
 * 27    12/01/98 0:08 Philipy
 * bug fixes
 * added inter-level mission briefing
 * changed splash screen code, + added splash screen on exit
 * 
 * 26    9/01/98 11:14 Philipy
 * loading bar now displayed when loading
 * 
 * 25    11/27/97 12:25p Phillipd
 * 
 * 24    4/11/97 16:26 Philipy
 * AVI now plays for stats screens
 * implemented scrolling messages (not enabled)
 * 
 * 23    21/10/97 13:14 Philipy
 * MAXLEVELS now defined here
 * 
 * 22    16/10/97 18:14 Philipy
 * added STATUS_PlayingAVI
 * 
 * 21    10/03/97 4:24p Phillipd
 * 
 * 20    9/18/97 10:48a Phillipd
 * 
 * 19    9/18/97 9:42a Phillipd
 * Proper Single Player Started
 * 
 * 18    6/10/97 9:01a Phillipd
 * 
 * 17    6/07/97 3:12p Phillipd
 * 
 * 16    6/03/97 10:47a Phillipd
 * 
 * 15    5/01/97 2:48p Phillipd
 * 
 * 14    4/25/97 2:51p Phillipd
 * 
 * 13    4/24/97 5:23p Phillipd
 * 
 * 12    20-03-97 5:46p Collinsd
 * Countdown timer now works in multiplayer.
 * 
 * 11    2/24/97 10:00a Phillipd
 * Demo mode with multispeed has been added..
 * 
 * 10    2/20/97 11:10a Phillipd
 * Changed the way a Player Get his Player Number...
 * Much more stable and friendly...
 * 
 * 9     12/18/96 12:41p Phillipd
 * Copyright screen added..
 * 
 * 8     12/17/96 12:55p Phillipd
 * 
 * 7     12/14/96 5:04p Phillipd
 * 
 * 6     12/01/96 1:21p Phillipd
 * You can now quit a game go back to the title screen and start or join
 * another....
 * 
 * 5     11/28/96 12:37p Phillipd
 * clear screen when starting multiplayer
 * 
 * 4     11/28/96 10:00a Phillipd
 * Multiplayer join changed beyond all recognition,,,
 * 
 * 3     11/25/96 11:59a Phillipd
 * 
 * 2     10/14/96 12:41p Phillipd
 * 
 * 1     9/27/96 9:14a Phillipd
 */

#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Game Status Defines...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define STATUS_PlayingAVI					0x01	// Playing AVI
#define	STATUS_Title						0x02	// On the Title Screen...
#define	STATUS_Copyright					0x03	// On the Copyright Screen...
#define	STATUS_StartingMultiplayer			0x04	// On the Multiplayer Starting Screen...
#define	STATUS_PreStartingMultiplayerSynch	0x05	// Waiting for everyone to have loaded the right level and be ready
#define	STATUS_StartingMultiplayerSynch		0x06	// Waiting for everyone to have loaded the right level and be ready
													// to Start...
#define	STATUS_QuitCurrentGame				0x07	// About to quit a game and return to the title screen..
#define	STATUS_GetPlayerNum					0x08	// Trying to aquire a Player Number...
#define	STATUS_PlayingDemo					0x09	// Playing Back a Demo...
#define	STATUS_Normal 						0x0a	// normal in game...
#define	STATUS_LevelEnd						0x0b	// the level has ended tell the host I know the level has ended...
#define	STATUS_BetweenLevels    			0x0c	// in title room between levels

#define	STATUS_ViewingScore					0x0d	// tell the host I am looking at the Score....
#define	STATUS_WaitingAfterScore			0x0e	// waiting for other players or waitng for the host to start...
#define	STATUS_Joining						0x0f	// I Want to join your game....
#define	STATUS_Left							0x10	// I have left your game...
#define	STATUS_ChangeLevelPostInitView		0x11	// after a change level init view
#define	STATUS_ChangeLevelPostPlayingDemo	0x12	// after a change level Playing Demo...
#define	STATUS_ChangeLevelPostAttractMode	0x13	// after a change level Attract Mode...
#define	STATUS_AttractMode					0x14	// Playing Back a Demo In Attract Mode...
											
#define	STATUS_StartingSinglePlayer			0x20	// Starting Single Player in game...
#define	STATUS_PostStartingSinglePlayer		0x21	// Starting Single Player in game after the level has been loaded...
#define	STATUS_SinglePlayer					0x22	// Single Player in game...
#define	STATUS_ViewingStats					0x23	// Viewing Stats Single Player...

#define	STATUS_TitleLoadGameStartingSinglePlayer		0x24
#define	STATUS_TitleLoadGamePostStartingSinglePlayer	0x25
#define	STATUS_ChangeLevelPostLoadGame	0x26
#define	STATUS_InGameLoadGameStartingSinglePlayer		0x27

#define	STATUS_WaitingToStartSinglePlayer		0x30	// waiting to start single player game...
#define	STATUS_WaitingToStartMultiPlayerHost	0x31	// waiting to start multi player game for host...
#define	STATUS_WaitingToStartMultiPlayerClient	0x32	// waiting to start multi player game for client...
#define	STATUS_WaitingToStartTeamGame			0x33	// waiting to start multi player game for client...
#define STATUS_QuittingFromTitles				0x34	// quitting game from title screen
#define STATUS_WaitingToStartDemo				0x35	// waiting to start demo
#define STATUS_WaitingToStartEndGameSequence	0x36	// waiting to start end of game demo playback sequence

#define STATUS_WaitingForLobbyConnect			0x37	// waiting for host to select game info in lobby launched game
#define STATUS_QuittingFromInGame				0x38	// quitting completely after stats ( ie. when playing lobby launched game )

#define	STATUS_SplashScreen					0x40	// showing splash screen or AVI...
#define STATUS_FinishedShowingSplashScreen	0x41	// finished showing current splash screen;


#define	STATUS_DisabledByServer				0x50	// Disabled by the Server


#define	STATUS_InitView_0					0x80	// 0 of Preping for a level..
#define	STATUS_InitView_1					0x81	// 1 of Preping for a level..
#define	STATUS_InitView_2					0x82	// 2 of Preping for a level..
#define	STATUS_InitView_3					0x83	// 3 of Preping for a level..
#define	STATUS_InitView_4					0x84	// 4 of Preping for a level..
#define	STATUS_InitView_5					0x85	// 5 of Preping for a level..
#define	STATUS_InitView_6					0x86	// 6 of Preping for a level..
#define	STATUS_InitView_7					0x87	// 7 of Preping for a level..
#define	STATUS_InitView_8					0x88	// 8 of Preping for a level..
#define	STATUS_InitView_9					0x89	// 9 of Preping for a level..
#define	STATUS_InitView_10					0x8a	// 10 of Preping for a level..
											
#define STATUS_WaitingToSendMessages		0xfa	// waiting until all guaranteed messages sent before continuing
#define STATUS_GetLevelNames				0xfb	// Pseudo host is recieving level names from server
#define STATUS_PseudoHostWaitingForAck		0xfc	// Pseudo host waiting for acknowledgement of game parameters from server
#define	STATUS_TurnedOffByServer			0xfd	// The Server has decided I dont need to know about this player...
#define	STATUS_LeftCrashed					0xfe	// I have left and crashed...
#define	STATUS_Null							0xff	// Im not doing anything yet...
#define STATUS_TryingToFindSession			0xf9
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	MultiPlayer Bit Defines...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

/*
dwUser3
*/

#define	TeamGameBit					0x0001	// bit 0: used in session description to denote a team game...
#define	GameTimeBit					0x01fe	// bits 1 - 8: Timed game uses bits 1-8 for time 0-30
#define	BombGameBit					0x0200	// bit 9: used in session description to denote a Bomb Tag game...
#define	FlagGameBit					0x0400	// bit 10: used in session description to denote a Capture the Flag Game...
#define	BountyGameBit				0x0800	// bit 11: used in session description to denote a Bounty Hunt Game...
#define	ShortPacketsBit				0x1000	// bit 12: used in session description to denote Use Short Packets...
#define	CTFGameBit					0x2000	// bit 13: used in session description to denote a "traditional" CTF (Capture the Flag) Game...
#define	BigPacketsBit				0x4000	// bit 14: used in session description to denote Use Group Packets...
#define	ServerGameStateBits			0x18000	// bit 15 & 16: used in session description to denote its a Server client based game..., and state of server
#define HarmTeamMatesBit			0x20000	// bit 17
#define BrightShipsBit				0x40000 // bit 18
#define BikeExhaustBit				0x80000 // bit 19
#define EnableMaxPlayersChangeBit	0x100000 // bit 20
#define CTF_TypeMask				0xe00000 // bits 21-23
#define CurrentGameTimeBits			0x1f000000 // bits 24-28 ( 0 to 30 mins )
#define CurrentGameTime_Shift		0x18

#define ResetKillsPerLevelBit		0x20000000 // bit 29
#define CollisionTypeBits			0xC0000000 // bits 30 & 31

#define Collision_Type_BitShift		0x1e		

#define CTF_Type_BitShift			0x15
#define CTF_Type_Decode( DW ) ( ( DW & CTF_TypeMask ) >> CTF_Type_BitShift )
#define CTF_Type_Encode( DW ) ( DW << CTF_Type_BitShift )

#define Server_State_BitShift		0xf

#define SERVER_STATE_NoServer		0x0									// not a server session
#define SERVER_STATE_NeedHost		( 0x1 << Server_State_BitShift )	// server session, waiting to allocate pseudo host ( joinable )
#define SERVER_STATE_HostChoosing	( 0x2 << Server_State_BitShift )	// server session, pseudohost is choosing game parameters ( not joinable )}
#define SERVER_STATE_Joinable		( 0x3 << Server_State_BitShift )	// server session, pseudohost has choosen game parameters ( joinable )

#define Server_State_Value( DW ) ( ( DW & ServerGameStateMask ) >> Server_State_BitShift )

#define	GameTimeBit_Shift			0x0001	// Timed game starts at bit 1

/*
dwUser2
*/

// low word used for RandomStartPosModify
#define MaxKillsBits 				0x00ff0000 // bits 16 - 23
#define CurrentMaxKillsBits			0xff000000 // bits 24 - 31

#define MaxKills_Shift				0x10
#define CurrentMaxKills_Shift		0x18

/*
dwUser4
*/

#define VersionNoBits	0x0000ffff	// bits 0 - 15
#define MaxPlayerBits	0x00ff0000 // bits 16 - 23
#define MaxPlayers_Shift	0x10

#define PacketsPerSecondBits	0xff000000	// bits 24-31
#define PacketsPerSecond_Shift	0x18


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Screen Mode Defines...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define Mode320X200 0
#define Mode320X240 1
#define Mode320X400 2
#define Mode512X384 3
#define Mode640X400 4
#define Mode640X480 5
#define Mode800X600 6
#define Mode1024X768 7

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Some Global Defines...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define START_FOV			(90.0F) // initial horizontal field-of-view (in degrees)
#define MAX_FOV				(120.0F) // max horizontal field-of-view (in degrees)

#define	MAXBOMBS			8		// Maximum number of bombs......
#define MAXLEVELS			(64)

#endif	// MAIN_INCLUDED