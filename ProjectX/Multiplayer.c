/*
 * The X Men, October 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Header: /PcProjectX/Multiplayer.c 290   17/09/98 15:27 Phillipd $
 *
 * $Log: /PcProjectX/Multiplayer.c $
 * 
 * 290   17/09/98 15:27 Phillipd
 * 
 * 289   14/09/98 16:33 Philipy
 * 
 * 288   14/09/98 15:35 Philipy
 * added facility for server based collisions
 * 
 * 287   3/09/98 17:34 Philipy
 * fixed server lag tolerance bug
 * 
 * 286   3/09/98 15:42 Philipy
 * re-allowed lag tolerance for server games
 * 
 * 285   3/09/98 9:31 Philipy
 * somw Gamespy fixes
 * added Session ( GUID ) and TCP command line flags
 * added TRACKER_NAME facility
 * 
 * 284   28/08/98 9:19 Collinsd
 * 
 * 283   27/08/98 20:12 Philipy
 * manual / auto server mode now selectable from menus
 * text now displayed when server in game & not rendering
 * dynamic PPS setting re-enabled for server
 * 
 * 282   26/08/98 17:06 Philipy
 * tracker now migrates along with host
 * 
 * 281   20/08/98 15:28 Philipy
 * You can now join server based games after being launched by a lobby.
 * Level name is updated after pseudohost selects level
 * AVI can now play if no sound hw exists
 * started gamespy support
 * 
 * 280   18/08/98 16:19 Philipy
 * 
 * 279   17/08/98 18:00 Philipy
 * removed loads of unreferenced local variables
 * 
 * 278   17/08/98 17:13 Philipy
 * added -ServerChoosesGame & ServerAutoStart command line options
 * 
 * 277   17/08/98 4:59p Oliverc
 * Changed the DirectPlay check to DX6
 * 
 * 276   17/08/98 11:16 Philipy
 * prevented \n appearing in session desc
 * 
 * 275   14/08/98 15:25 Philipy
 * added trilinear option to menus
 * fixed level name / shutdown packet in heartbeat
 * 
 * 
 * 274   14/08/98 9:13 Phillipd
 * DirectX6 is in effect.......
 * 
 * 273   12/08/98 12:51 Philipy
 * status request now treated as type 0 if no type specified.
 * responses now sent to requester address, not tracker address.
 * correct max players value now sent in heartbeat packet.
 * 
 * 272   7/08/98 12:45 Philipy
 * heartbeat now enabled for non tcp games ( if heartbeat.txt exists )
 * status type 0 now treated as status type 254
 * if lobby launched with max players set to 0, host is able to set max
 * players
 * shutdown udp packet now sent ( if specified in heartbeat.txt )
 * 
 * 271   5/08/98 11:04 Philipy
 * added AutoStart facility ( joins game based on GUID in registery )
 * upped patch version to 1.02
 * 
 * 270   31/07/98 12:17 Philipy
 * 
 * 269   30/07/98 10:43a Oliverc
 * Fixed TCP/IP address saving
 * 
 * 268   28/07/98 14:44 Philipy
 * all server timeouts now configurable
 * 
 * 267   28/07/98 10:39 Philipy
 * Max players now works properly for server games
 * 
 * 266   23/07/98 15:51 Philipy
 * CTF subtype now set b4 calling SetUpGameType
 * 
 * 265   22/07/98 16:53 Philipy
 * changed rejoining check so that dynamically updated user fields in SD
 * are not compared
 * 
 * 264   21/07/98 17:31 Philipy
 * added timeout stuff for titles
 * 
 * 263   21/07/98 14:21 Phillipd
 * 
 * 262   21/07/98 12:10 Philipy
 * 
 * 261   21/07/98 12:09 Collinsd
 * MAX_PLAYERS increased to 24 and version number incremented.
 * 
 * 260   21/07/98 11:39 Philipy
 * heartbeat packets now only sent if frequency specified
 * modified PlayersList to use 16 rather than MAX_PLAYERS
 * 
 * 259   20/07/98 12:27 Philipy
 * nicer handling of uninstalled levels
 * 
 * 258   7/16/98 10:01a Phillipd
 * 
 * 257   7/15/98 9:37a Phillipd
 * 
 * 256   7/15/98 9:30a Phillipd
 * 
 * 255   7/14/98 11:26a Phillipd
 * 
 * 254   14/07/98 11:15 Philipy
 * various patch bugs
 * pseudohost quitting in titles
 * 
 * 253   7/14/98 10:25a Phillipd
 * 
 * 252   7/10/98 5:20p Phillipd
 * 
 * 251   7/10/98 11:15a Phillipd
 * 
 * 250   7/09/98 11:50a Phillipd
 * Polytext now works being turned off for the Server.....HooRay...
 * 
 * 249   8/07/98 10:13 Philipy
 * 
 * 248   7/07/98 18:05 Philipy
 * added lobby autostart code ( when all players have recieved init msg )
 * added num primary weapons menu option ( propergated to other players &|
 * server )
 * extracted new title text for localisation
 * 
 * 247   3/07/98 17:46 Philipy
 * added quit option when using quickstart
 * 
 * 246   3/07/98 11:54 Philipy
 * heartbeat & quickstart stuff
 * 
 * 245   7/01/98 3:26p Phillipd
 * 
 * 244   6/26/98 4:13p Phillipd
 * 
 * 243   6/24/98 10:44a Phillipd
 * 
 * 242   6/24/98 10:31a Phillipd
 * 
 * 241   24/06/98 10:04 Philipy
 * heartbeat stuff ( not currently active )
 * 
 * 240   23/06/98 15:13 Philipy
 * 
 * 239   23/06/98 14:49 Philipy
 * 
 * 238   23/06/98 12:46 Philipy
 * 
 * 237   6/22/98 2:16p Phillipd
 * Option to reset the score for every level..............
 * 
 * 236   17/06/98 19:33 Philipy
 * more win98 stuff
 * 
 * 235   16/06/98 16:32 Philipy
 * more lobby / join game stuff
 * 
 * 234   13/06/98 20:46 Philipy
 * improved lobby support:
 * host now migrates properly
 * you can quit b4 starting game without screwing up session
 * 
 * 233   11/06/98 17:28 Collinsd
 * Fixed some warnings.
 * 
 * 232   11/06/98 16:57 Philipy
 * loads of win98 shareware version stuff
 * 
 * 231   6/11/98 12:43p Phillipd
 * 
 * 230   11/06/98 9:54 Philipy
 * files checked in prior to starting Win98 shareware version
 * 
 * 229   6/09/98 3:49p Phillipd
 * 
 * 228   6/09/98 12:14p Phillipd
 * 
 * 227   6/09/98 12:09p Phillipd
 * 
 * 226   9/06/98 11:23 Philipy
 * server now reset if pseudohost does not have any valid levels
 * fixed timed game bug for server based games
 * 
 * 225   8/06/98 12:28 Philipy
 * server levels now sent across to pseudohost. Pseudohost can only pick
 * valid levels
 * 
 * 224   22/05/98 17:51 Philipy
 * more work on session info
 * 
 * 223   20/05/98 16:42 Philipy
 * stoped server team games starting straight away
 * 
 * 222   20/05/98 9:38 Philipy
 * implemented front end server menus
 * removed ( invalid ) ping from sessions menu
 * changed EnumPlayers so that it solely uses MSG_NAME
 * 
 * 221   5/12/98 2:59p Phillipd
 * 
 * 220   5/12/98 2:54p Phillipd
 * 
 * 219   5/12/98 11:02a Phillipd
 * 
 * 218   11/05/98 15:15 Philipy
 * added session info stuff ( game type, ping etc )
 * 
 * 217   5/07/98 9:37a Phillipd
 * 
 * 216   4/29/98 4:41p Phillipd
 * 
 * 215   4/29/98 11:43a Phillipd
 * 
 * 214   4/27/98 4:01p Phillipd
 * 
 * 213   4/14/98 3:54p Phillipd
 * 
 * 212   11/04/98 12:17 Oliverc
 * Added -AnyDPlayVersion command line switch to disable Direct Play DLL
 * version checking
 * 
 * 211   4/10/98 12:43p Phillipd
 * 
 * 210   8/04/98 20:47 Philipy
 * title text messages now properly initialised
 * holo-glaire removed for sw version
 * compound buffer size can now be set in command line and opt file
 * prevented "level select disabled" from appearing at start of
 * multiplayer game
 * 
 * 209   8/04/98 20:35 Collinsd
 * 
 * 208   8/04/98 20:33 Collinsd
 * 
 * 207   8/04/98 17:01 Oliverc
 * Last service provider now stored in registry
 * 
 * 206   4/08/98 2:45p Phillipd
 * 
 * 205   8/04/98 11:06 Philipy
 * fixed host not displaying player names bug
 * 
 * 204   7/04/98 12:09 Philipy
 * Olly fixed CTF flag now reset when going into single player game
 * 
 * 203   7/04/98 9:10 Philipy
 * fixed mem leak in FileGetVersion
 * 
 * 202   6/04/98 21:19 Oliverc
 * Increased line spacing for network service providers to prevent TCP/IP
 * from being truncated
 * 
 * 201   4/06/98 7:06p Phillipd
 * 
 * 200   5/04/98 17:40 Collinsd
 * Updated all demo functions.
 * 
 * 199   5/04/98 17:10 Collinsd
 * Hacked demos to work.
 * 
 * 198   4/05/98 2:33p Phillipd
 * 
 * 197   4/04/98 11:21 Oliverc
 * Demos now play from CD if not installed
 * 
 * 196   3/04/98 17:58 Collinsd
 * 
 * 195   30/03/98 23:38 Oliverc
 * Fixed minor glitch in PreferedMaxPlayers for modem vs network multiplay
 * 
 * 194   30/03/98 21:28 Oliverc
 * Fixed demo naming bug
 * 
 * 193   3/30/98 7:35p Phillipd
 * 
 * 192   3/30/98 7:11p Phillipd
 * 
 * 191   3/27/98 3:10p Phillipd
 * 
 * 190   25/03/98 15:06 Oliverc
 * Only standard service providers are DirectPlay version checked now
 * 
 * 189   3/21/98 2:34p Phillipd
 * 
 * 188   16/03/98 16:40 Philipy
 * fixed buffered key problem
 * added AVI to splash screens
 * 
 * 187   16/03/98 16:39 Collinsd
 * Random pickups now work ( honest ).
 * 
 * 186   16/03/98 10:52 Philipy
 * removed 'No splash levels' msg
 * 
 * 185   15/03/98 18:40 Philipy
 * added water effect splash screen
 * fixed bug with end game sequence
 * implemented attract mode
 * text macros now saved in config
 * 
 * 184   11/03/98 10:55 Philipy
 * all sfx now stopped for pause mode
 * correct level info now displayed in briefing screen
 * correct level now entered after completing a previously saved level
 * level timer now accurate
 * 
 * 183   8/03/98 16:50 Philipy
 * prevent MenuBack from some menus when lobby launched
 * added team game support for lobby
 * 
 * 182   7/03/98 14:26 Oliverc
 * Clean demo no longer needs different output name
 * 
 * 181   6/03/98 17:36 Philipy
 * implemented ability to run when launched by lobby
 * 
 * 180   3/06/98 5:13p Phillipd
 * 
 * 178   4/03/98 12:33 Oliverc
 * CTF mode fully enabled
 * 
 * 177   3/03/98 16:59 Oliverc
 * New multiplayer CTF mode stuff (1st attempt)
 * 
 * 176   3/02/98 12:29p Phillipd
 * 
 * 175   2/03/98 12:10 Oliverc
 * Hidden ".DMO" extension from demo file names
 * and added extra info to .DMO file to specify game type etc
 * 
 * 174   27/02/98 16:30 Oliverc
 * Multiplayer game options always restored from registry on entering
 * "create multiplayer game" menu, and only saved back to registry when
 * multiplayer game started
 * 
 * 173   25/02/98 16:19 Oliverc
 * More multiplayer tweaks
 * 
 * 172   24/02/98 22:00 Oliverc
 * Tweaks to multiplayer games
 * 
 * 171   24/02/98 16:54 Oliverc
 * 1st attempt at bounty hunt multiplayer game
 * 
 * 170   2/23/98 2:02p Phillipd
 * Load Save now works.
 * 
 * 169   20/02/98 19:41 Oliverc
 * 2nd prototype of capture the flag game
 * 
 * 168   20/02/98 15:28 Philipy
 * re-implented AVI
 * splash screens can now play demos and AVIs
 * 
 * 167   10/02/98 16:08 Collinsd
 * Toggle for descent/forsaken collision added
 * 
 * 166   5/02/98 12:51 Collinsd
 * Bomb tag works again and invulnerable enemies are no longer effected by
 * shockwaves
 * 
 * 165   2/02/98 16:15 Oliverc
 * Added check for Direct Play 5.0a update when creating or joining
 * multiplayer games
 * 
 * 164   1/29/98 2:27p Phillipd
 * Demo can now be saved to ram....and can be saved if your not the
 * host...
 * 
 * 163   27/01/98 12:11 Philipy
 * demo settings now always restored b4 playing demo
 * drop primary / drop secondary removed
 * 
 * 162   27/01/98 11:05 Philipy
 * fixed team game stuff
 * 
 * 161   24/01/98 18:38 Philipy
 * time limit max now 15 mins
 * impossible to have unlimited time
 * 
 * 160   24/01/98 17:38 Philipy
 * fixed multiplayer join-quit-join bug
 * fixed attract mode loading wrong level
 * 
 * 159   22/01/98 19:13 Philipy
 * fixed re-loading looping sfx while in level
 * biker speech now switchable
 * 
 * 158   22/01/98 10:07 Collinsd
 * Names dont appear in demo playback if teamgame played before.
 * 
 * 157   22/01/98 8:45 Collinsd
 * Deallocate CountDownDigits in release level.
 * Turned off CoundDown when playing Demos
 * 
 * 156   21/01/98 12:19 Philipy
 * Added attract mode for shareware
 * fixed looping sfx volume bug
 * 
 * 155   1/19/98 4:21p Phillipd
 * Bright Ships flag added.....
 * 
 * 154   1/19/98 9:41a Phillipd
 * 
 * 153   17/01/98 16:16 Philipy
 * fixed demo playback bug
 * 
 * 152   1/16/98 2:54p Phillipd
 * 
 * 151   1/16/98 8:46a Phillipd
 * 
 * 150   15/01/98 23:19 Oliverc
 * Moved new Probe IP from EXTRA_IP to default CheckLegalIP()
 * 
 * 149   1/15/98 1:59p Phillipd
 * 
 * 148   13/01/98 11:48 Oliverc
 * TCPIP disabled for SHAREWARE
 * 
 * 147   13/01/98 10:58 Philipy
 * lives now carry over between levels as before
 * put #ifdefs around splash screens for shareware
 * 
 * 146   12/01/98 18:05 Oliverc
 * 
 * 145   1/12/98 10:04a Phillipd
 * 
 * 144   11/01/98 17:54 Philipy
 * inter level mission screen
 * 
 * 143   9/01/98 17:25 Philipy
 * player is now forced to start on level 0
 * 
 * 142   9/01/98 11:13 Philipy
 * CD nows plays last track
 * CD now replays current track from seperate  ( low priority ) thread -
 * but still causes pause
 * loading bar now displayed when loading
 * 
 * 141   9/01/98 10:35 Oliverc
 * Re-enabled MULTIPLAYER_VERSION check when joining game
 * 
 * 140   1/09/98 10:31a Phillipd
 * Bugs fixed......
 * 
 * 139   7/01/98 15:03 Oliverc
 * SHAREWARE versions now use built-in level lists instead of external
 * mission.dat and battle.dat files
 * 
 * 138   1/07/98 11:07a Phillipd
 * 
 * 137   1/07/98 10:43a Phillipd
 * 
 * 136   12/19/97 2:35p Phillipd
 * 
 * 135   12/13/97 1:35p Phillipd
 * 
 * 134   12/11/97 4:04p Phillipd
 * 
 * 133   12/11/97 12:00p Phillipd
 * 
 * 132   11/12/97 11:55 Oliverc
 * Fixed CRASH_BUG in multiplayer joining then aborting (pls check this
 * Phil!)
 * 
 * 131   12/03/97 2:50p Phillipd
 * Packets per second functioning.....
 * 
 * 130   3/12/97 14:15 Oliverc
 * Prevented demo playback from showing Current_Camera_View until that
 * player's position has been updated from the DMO file
 * 
 * 
 * 128   11/21/97 10:56a Phillipd
 * Max Kills to end a level....
 * 
 * 127   11/17/97 4:34p Phillipd
 * 
 * 126   4/11/97 16:26 Philipy
 * AVI now plays for stats screens
 * implemented scrolling messages (not enabled)
 * 
 * 125   10/30/97 12:40p Phillipd
 * 
 * 124   28/10/97 9:41 Philipy
 * Team member names now flash on team select screen when not ready
 * 
 * 123   10/22/97 12:14p Phillipd
 * 
 * 122   8/10/97 20:30 Philipy
 * 
 * 121   7/10/97 12:37 Collinsd
 * Added spotfx for a level and point and vector stuff for MX and MXA
 * models.
 * 
 * 120   1-10-97 8:26p Philipy
 * IP address now only displayed when hosting IP game
 * 
 * 119   10/01/97 2:48p Phillipd
 * 
 * 118   9/30/97 4:20p Phillipd
 * 
 * 117   30-09-97 10:46a Philipy
 * "Create Game" screen now shows host IP Address
 * 
 * 116   9/30/97 8:46a Phillipd
 * 
 * 115   9/29/97 11:55a Phillipd
 * Dawn of a new age with directplay 3
 * 
 * 114   9/29/97 9:08a Phillipd
 * 
 * 113   9/18/97 3:56p Phillipd
 * Fixed stats bug....
 * 
 * 112   9/18/97 12:16p Phillipd
 * 
 * 111   9/18/97 9:42a Phillipd
 * Proper single player started..
 * 
 * 110   15-09-97 3:23p Philipy
 * message displayed, then correct menu displayed on new host machine if
 * host migrates.
 * Host can now start another game, having quit 'waiting to start' menu
 * 
 * 109   14-09-97 6:51p Philipy
 * joining existing team game - you can now select which team to go into,
 * and view current scores
 * 
 * 108   5-09-97 11:12a Philipy
 * demo playback stuff from new menus
 * 
 * 106   1-09-97 5:47p Philipy
 * removed warnings....oops!
 * 
 * 105   1-09-97 5:35p Philipy
 * changes made for 3 & 4 player team game
 * 
 * 104   26-08-97 4:16p Philipy
 * various changes to enable host/join a game from new menus
 * 
 * 103   31-07-97 5:26p Philipy
 * switched off buffering for demo playback
 * 
 * 102   28/07/97 16:44 Collinsd
 * Tracker uses ships to target, demo mode works again?
 * 
 * 101   25/07/97 12:10 Collinsd
 * Changed ships bike index, and changed skin for olly.
 * 
 * 100   22-07-97 4:32p Philipy
 * removed some E3 demo stuff
 * 
 * 99    17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 98    14/07/97 12:44 Oliverc
 * Changed MICROSOFT_DEBUG flag to DISABLE_IP_CHECKING
 * 
 * 97    8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 96    6/24/97 11:12a Phillipd
 * 
 * 95    17/06/97 9:30 Oliverc
 * Disabled IP protection for MICROSOFT_DEBUG version
 * 
 * 94    6/16/97 4:15p Phillipd
 * 
 * 93    6/14/97 1:11p Phillipd
 * 
 * 92    6/12/97 11:15a Phillipd
 * 
 * 91    10/06/97 17:35 Oliverc
 * Changed way fingerprint and IP addresses are handled for special demo
 * versions
 * 
 * 90    6/10/97 9:01a Phillipd
 * 
 * 89    9-06-97 9:03p Philipy
 * removed flip to GDI for E3 demo, when joining a game
 * 
 * 88    7-06-97 9:13p Philipy
 * auto join a game
 * 
 * 87    6/07/97 3:15p Phillipd
 * 
 * 86    6/07/97 10:52a Phillipd
 * 
 * 85    6/06/97 19:31 Oliverc
 * 
 * 84    6/06/97 4:00p Phillipd
 * 
 * 83    6/06/97 3:32p Phillipd
 * 
 * 82    6/06/97 2:24p Phillipd
 * 
 * 81    6/06/97 2:23p Phillipd
 * 
 * 80    6-06-97 2:20p Philipy
 * join a game with new menus
 * 
 * 79    6/05/97 4:11p Phillipd
 * 
 * 78    6/03/97 10:46a Phillipd
 * 
 * 77    5/27/97 5:40p Phillipd
 * 
 * 76    5/19/97 4:48p Phillipd
 * Disabled PowerVR demo fingerprint/IP address
 * 
 * 75    5/19/97 4:19p Phillipd
 * Demo for PowerVR debugging
 * 
 * 74    5/08/97 11:23a Phillipd
 * Demos now contain a name for the level not a number.....
 * 
 * 73    5/01/97 12:46p Phillipd
 * 
 * 72    30/04/97 10:58 Oliverc
 * Moved FingerPrint[] to lastcomp.c
 * 
 * 70    29/04/97 17:49 Oliverc
 * 
 * 69    4/28/97 4:14p Phillipd
 * 
 * 68    4/26/97 2:01p Phillipd
 * 
 * 67    4/25/97 5:12p Phillipd
 * 
 * 66    4/25/97 10:36a Phillipd
 * Better demo playback...which player eyes works again
 * 
 * 65    4/24/97 9:09a Phillipd
 * 
 * 64    4/22/97 1:51p Phillipd
 * 
 * 63    4/16/97 4:41p Phillipd
 * 
 * 62    4/10/97 4:29p Phillipd
 * DirectPlay3 is here...
 * 
 * 61    4/10/97 10:02a Phillipd
 * 
 * 60    4/08/97 10:41a Phillipd
 * 12 players added...
 * 
 * 59    4/07/97 12:27p Phillipd
 * 
 * 58    4/05/97 12:08p Phillipd
 * 
 * 57    27/03/97 12:32 Oliverc
 * Added Brian Bruning's IP address (on unused #ifdef)
 * 
 * 56    20-03-97 5:46p Collinsd
 * Countdown timer now works in multiplayer.
 * 
 * 55    3/13/97 8:56a Phillipd
 * 
 * 54    3/12/97 6:01p Phillipd
 * 
 * 53    3/12/97 4:42p Phillipd
 * 
 * 52    3/11/97 5:41p Phillipd
 * 
 * 51    3/11/97 4:55p Phillipd
 * Demo Interpolate stuff is now working but not finished...
 * 
 * 50    3/11/97 9:16a Phillipd
 * Texture animation added to models..not finished..
 * 
 * 49    3/10/97 4:03p Phillipd
 * 
 * 48    3/01/97 12:57p Phillipd
 * handles multiple join requests from 1 person,,,,
 * 
 * 47    2/27/97 8:58a Phillipd
 * 
 * 46    26/02/97 20:00 Oliverc
 * Added the following to the set of allowed IP addresses: Sculptured US,
 * Acclaim UK, Iguana UK, Acclaim Germany, Acclaim France.
 * Removed Shawn's dynamic IP address.
 * ...and deleted much out-of-date "Amy" fingerprint.
 * 
 * 45    2/26/97 4:05p Phillipd
 * 
 * 44    2/25/97 4:05p Phillipd
 * 
 * 43    2/25/97 2:51p Phillipd
 * 
 * 42    2/24/97 4:47p Phillipd
 * GameElapsedTime is now relative to the demo not the
 * real world....so dynamic speed change is possible..
 * 
 * 41    2/24/97 10:00a Phillipd
 * Demo mode with multispeed has been added..
 * 
 * 40    2/20/97 11:10a Phillipd
 * Changed the way a Player Get his Player Number...
 * Much more stable and friendly...
 * 
 * 39    19-02-97 5:08p Collinsd
 * Added shawn's IP Address
 * 
 * 38    13/02/97 17:49 Oliverc
 * 
 * 37    13-02-97 3:36p Collinsd
 * 
 * 36    2/11/97 5:11p Phillipd
 * 
 * 35    11-02-97 5:08p Collinsd
 * Triggers/RegenPoints and pickups now are sent across correctly.
 * 
 * 34    5-02-97 3:00p Collinsd
 * Just got rid of warnings.
 * 
 * 33    2/04/97 5:11p Phillipd
 * new player joining brute force reject..
 * 
 * 32    2/03/97 5:16p Phillipd
 * Translusceny is now controlled by global execute buffers.... which is
 * much better...
 * 
 * 31    20-01-97 11:21a Collinsd
 * Added time limit to multiplayer games.
 * 
 * 30    18-01-97 3:01p Collinsd
 * Added countdown code. ( Dissabled for now ).
 * 
 * 29    2-01-97 12:19p Collinsd
 * Network game now works again.
 * Orbit pulsar now works over network game.
 * 
 * 28    12/27/96 12:34p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 27    12/23/96 6:04p Phillipd
 * 
 * 26    12/23/96 8:50a Phillipd
 * 
 * 25    12/19/96 4:02p Phillipd
 * 
 * 24    12/18/96 9:50a Phillipd
 * 
 * 23    12/17/96 4:57p Phillipd
 * Version Control Added..
 * 
 * 22    12/17/96 2:56p Phillipd
 * 
 * 21    12/17/96 2:47p Phillipd
 * 
 * 20    12/17/96 2:32p Phillipd
 * 
 * 19    12/17/96 12:55p Phillipd
 * 
 * 18    12/17/96 9:20a Phillipd
 * 
 * 17    12/14/96 5:04p Phillipd
 * 
 * 16    12/13/96 12:52p Phillipd
 * 
 * 15    12/12/96 12:46p Phillipd
 * 
 * 14    12/12/96 10:56a Phillipd
 * 
 * 13    12/06/96 12:22p Phillipd
 * 
 * 12    12/05/96 4:17p Collinsd
 * GDI flip added for tcpip
 * 
 * 11    12/03/96 5:00p Phillipd
 * Quting if not host bug fixed...
 * game doesnt start if ya just quit the title screen...
 * 
 * 10    12/03/96 11:14a Phillipd
 * 
 * 9     12/03/96 10:40a Phillipd
 * Multiplay can now back out
 * 
 * 8     11/28/96 11:28a Phillipd
 * You can see who is in a game and how many players
 * there should be...
 * 
 * 7     11/28/96 10:00a Phillipd
 * Multiplayer join changed beyond all recognition,,,
 * 
 * 6     11/27/96 11:05a Phillipd
 * Fixed Mouse speed on fast machines.....
 * 
 * 5     11/25/96 11:59a Phillipd
 * 
 * 4     11/20/96 2:42p Phillipd
 * players can now restart and keep there score...
 * 
 * 3     11/19/96 9:31a Phillipd
 * 
 * 2     11/18/96 3:26p Phillipd
 * 
 * 1     11/18/96 12:23p Phillipd
 * New menu multiplayer stuff..
 */
#define IDIRECTPLAY2_OR_GREATER

#include <stdio.h>
#include "windows.h"
#include "typedefs.h"
#include "main.h"
#include <dplay.h>
#include <dplobby.h>
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

#include "registry.h"
#include "Local.h"

#include "text.h"
#include "ships.h"
#include "LoadSave.h"
#include "pickups.h"
#include "primary.h"
#include "controls.h"
#include "xmem.h"
#include "dpthread.h"

// required version of Direct Play is 6.0 (4.6.0.318)
#define DPLAY_VERSION_HI	(4)
#define DPLAY_VERSION_LO	(6)
#define DPLAY_REVISION_HI	(0)
#define DPLAY_REVISION_LO	(318)

/*
 * Externals
 */
extern BOOL OKToProcessKeys;
extern SLIDER	MaxServerPlayersSlider;
extern BOOL	OKToJoinSession;
extern int OldPPSValue;

extern char host_ip[];

#ifdef MANUAL_SESSIONDESC_PROPAGATE
extern LPDPSESSIONDESC2                    glpdpSD_copy;
#endif

BOOL CheckDirectPlayVersion = TRUE;
BOOL IsServerGame = FALSE;
BOOL ResetKillsPerLevel = FALSE;
BOOL bTCP = FALSE;

extern uint8 QuickStart;

extern	BOOL	HarmTeamMates;
extern	BOOL ServerRendering;
extern BOOL	BrightShips;
extern BOOL	BikeExhausts;
extern int32 ColPerspective;

extern HKEY ghCondemnedKey;     // Condemned registry key handle
extern LONG RegGet(LPCTSTR lptszName, LPBYTE lpData, LPDWORD lpdwDataSize);
extern LONG RegSet(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);
extern LONG RegSetA(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize);

extern	uint32	BIGPACKETBUFFERSIZE;
extern	uint32	SERVERPACKETBUFFERSIZE;

extern BOOL NoMenuBack;

extern void DebugLastError( void );

extern BOOL NeedFlagAtHome;
extern BOOL OwnFlagTeleportsHome;
extern BOOL CanCarryOwnFlag;
extern SLIDER CTFSlider;

extern BOOL IsLobbyLaunched;

extern	BOOL	UseShortPackets;
extern	BOOL	BigPackets;

extern char *DemoFileName( char *demoname );
extern char *DemoName( char *demofilename );
extern void SetMultiplayerPrefs( void );

extern BOOL LimitedLoad;
extern LIST	MySessionsList;
extern BOOL	Panel;
extern BOOL PreAttractModePanel;
extern	BOOL	RecordDemoToRam;

extern MENUITEM TeamGameHostMenuItem;
extern BYTE	PreSynchupStatus;
extern int CurrentLoadingStep;
extern BOOL DemoShipInit[];
extern	float DPlayUpdateInterval;
extern	BOOL	BrightShips;
extern	BOOL	MyBrightShips;

extern float Pulse;
extern char *EmptyString;

extern	BOOL                    IsServer;
extern	BOOL					IsHost;   // is the user hosting/joining a game
extern	DPID					dcoID;    // player id
extern	int16					Lives;

extern	BOOL	BountyHunt;
extern	BOOL	CaptureTheFlag;
extern	BOOL	CTF;
extern	BOOL	BombTag;
extern	TEXT	DemoGameName;
extern	BOOL	RecordDemo;
extern	LIST	ServiceProvidersList;
extern	LIST	PlayersList;
extern	LIST	LevelList;
extern	LIST	TeamList[MAX_TEAMS];
extern	MENUITEM	TeamItem;
extern	MENUITEM	NewTeamItem;
extern	BOOL	TeamGame;
extern	BYTE	TeamNumber[MAX_PLAYERS];
extern	SLIDER	TimeLimit;
extern	SLIDER	DemoSpeed;
extern	SLIDER	MaxPlayersSlider;
extern	SLIDER	MaxKillsSlider;
extern	int16	MaxKills;

extern	TEXT	MultiPlayerGameName;
extern	char	biker_name[256];
extern	float	framelag;
extern char MyName[];
extern char NickName[];
extern	BYTE					Current_Camera_View;		// which object is currently using the camera view....
extern	BYTE                    WhoIAm;
extern	GLOBALSHIP              Ships[MAX_PLAYERS];
extern	int16					StatsStatus;
extern	int16					Stats[MAX_PLAYERS+1][MAX_PLAYERS+1];
extern	int16					StatsCount;
extern	SHORTNAMETYPE			Names;	// all the players short Names....
extern	int16	SelectedBike;
extern	SLIDER	NumOfPlayersSlider;
extern	int MenuStackLevel;
extern	BYTE	GameStatus[];	// Game Status for every Ship...
extern	SLIDER  PacketsSlider;

extern	LPDPSESSIONDESC2                    glpdpSD;

extern	LPDIRECTPLAY4A                       glpDP;				// directplay object pointer
extern	LPDIRECTPLAYLOBBY2A					lpDPlayLobby;		//Lobby stuff...
extern	LPDPLCONNECTION						glpdplConnection;	// connection settings
extern	TEXT TCPAddress;

extern	DPID	dcoID;

extern	WORD	Version;
extern	BOOL	CountDownOn;

extern	float	GetPlayerNumCount1;
extern	float	GetPlayerNumCount2;
extern	int		GetPlayerNumCount;
extern	FILE	*	DemoFp;
extern	FILE	*	DemoFpClean;
BOOL ChangeLevel( void );
BOOL InitLevels( char *levels_list );
extern	int16		LevelNum;
extern	int16		NewLevelNum;
extern	BOOL	PlayDemo;
extern	LONGLONG	GameStartedTime;		// when the game started
extern	LONGLONG	GameElapsedTime;		// when the game started
extern	LONGLONG	TempGameElapsedTime;		// when the game started
extern	LIST	DemoList;
extern	float Demoframelag;
extern	LONGLONG	DemoTimeSoFar;
extern	BOOL	AutoDetail;
void SetOurRenderStates( MENUITEM *item );
extern	MENUITEM	JoinItem;
extern	MENUITEM	NewJoinItem;
extern  MENUITEM	WatchTeamSelectionItem;
extern int CameraStatus;
extern	BYTE					OverallGameStatus;
extern char *CurrentLevelsList;
extern	int16	PreferedMaxPlayers;

extern	LIST	SessionsList;
extern	BOOL AutoSelectConnection;
extern	LONGLONG	Freq;
extern  MENUSTATE MenuState;
extern  char TeamCurrentScore[MAX_TEAMS][64];

extern	LONGLONG	DemoStartedTime;		// when the game started
extern	LONGLONG	DemoEndedTime;		// when the game started
extern	int32		DemoGameLoops;

extern BOOL PreventFlips;
extern	int		GameType;
extern	uint16		Seed1;
extern	uint16		Seed2;
extern	uint16		CopyOfSeed1;
extern	uint16		CopyOfSeed2;
extern	BOOL		RandomPickups;
extern	BOOL	PolygonText;
extern	BOOL	NoSFX;

/*
 * Globals to this module
 */

BOOL ServiceProviderSet = FALSE;

BOOL IPAddressExists = FALSE;

uint32	IPAddress = 0;
char    IPAddressText[16];
char    ServiceProviderShortName[16];

DPSESSIONDESC2			Old_Session;
 
DPCAPS					ServiceProviderCaps;
DWORD					SugestedEnumSessionsTimeout = 0;


DWORD                   Old_WhoIAm = (DWORD) -1;
DWORD					Old_Kills = 0;
DWORD					Old_Deaths = 0;
DWORD					Old_TeamNumber = 0;
char					Old_Name[256] = { 0 };
BOOL					Rejoining = FALSE;

DPID					PlayerIDs[MAX_PLAYERS];
DPID					LobbyPlayerIDs[MAX_PLAYERS];
uint16					NumLobbyPlayers;

DPID					TeamIDs[MAX_TEAMS][MAX_PLAYERS];
int						TeamMembers[MAX_TEAMS];

MENU  *				GetPlayerNumMenu;
float	Bodge	= 1.0F;

GUID	ServiceProvidersGuids[MAXSERVICEPROVIDERS];

DPSESSIONDESC2	Sessions[MAXSESSIONS];
char SessionNames[ MAXSESSIONS ][ 128 ];

DPSESSIONDESC2	SessionsCopy[MAXSESSIONS];
LIST	SessionsListCopy;


BOOL	SessionsRefresh[MAXSESSIONS];
BOOL	SessionsRefreshActive = FALSE;
GUID	gSPGuid;            // currently selected service provider guid

BOOL	Modem2Modem = FALSE;

uint16	RandomStartPosModify = 0;							

void DebugPrintf( const char * format, ... );
void DrawLoadingBox( int current_loading_step, int current_substep, int total_substeps );
void InitMySessionsList(void);
static int DirectPlayOK( LPGUID lpServiceProvider_guid );
void DrawFlatMenuItem( MENUITEM *Item );
void GetLevelName( char *buf, int bufsize, int level );


/********************************************\
|
|  CurrentServiceProviderNameShort
|
\********************************************/

void GetServiceProviderShortName( void )
{

	int size = 16;

	memset( &ServiceProviderShortName, 0, size );

	if ( IsEqualGuid( &gSPGuid , (GUID*)&DPSPGUID_TCPIP  ) )

		_snprintf ( ServiceProviderShortName, size, "TCPIP" );
		
	else if ( IsEqualGuid( &gSPGuid , (GUID*)&DPSPGUID_IPX    ) )

		_snprintf ( ServiceProviderShortName, size, "IPX" );

	else if ( IsEqualGuid( &gSPGuid , (GUID*)&DPSPGUID_SERIAL ) )
		
		_snprintf ( ServiceProviderShortName, size, "SERIAL" );

	else if ( IsEqualGuid( &gSPGuid , (GUID*)&DPSPGUID_MODEM  ) )
		
		_snprintf ( ServiceProviderShortName, size, "MODEM" );

}



/*
 * IsEqualGuid
 *
 * Determines if two guids are equal
 */
BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2)
{
   return (
      ((PLONG) lpguid1)[0] == ((PLONG) lpguid2)[0] &&
      ((PLONG) lpguid1)[1] == ((PLONG) lpguid2)[1] &&
      ((PLONG) lpguid1)[2] == ((PLONG) lpguid2)[2] &&
      ((PLONG) lpguid1)[3] == ((PLONG) lpguid2)[3]);
}

void InitDemoList( MENU * Menu );
void RestoreDemoSettings( void );
void GetMultiplayerPrefs( void );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get a List of Service Providers
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void GetServiceProviders( MENU *Menu )
{
	DWORD size;
	static GUID last_service_provider;
	LPVOID *ptr;
	MENUITEM *item;
	char sp_guidtext[ 256 ];

	if ( Menu )
	{
		for ( item = Menu->Item; item->x >= 0; item++ )
		{
			if ( item->FuncSelect == SelectQuit )
			{
				if ( QuickStart )
				{
					item->FuncDraw = DrawFlatMenuItem;
					item->highlightflags &= ~TEXTFLAG_Unselectable;
				}else
				{
					item->FuncDraw = NULL;
					item->highlightflags |= TEXTFLAG_Unselectable;
				}
			}
		}
	}

	SetOurRenderStates( NULL );
	
	GetLastGameInfo();
	ServiceProvidersList.items = 0;

	ServiceProvidersList.top_item = 0;
	ServiceProviderSet = FALSE;

	if (CameraStatus != CAMERA_AtStart)
		ServiceProvidersList.display_items = 8;
	else
		ServiceProvidersList.display_items = 8;

	ServiceProvidersList.FuncInfo = NULL;

	DPlayCreateLobby();

	ptr = NULL;
	if ( ghCondemnedKey )
	{
		if ( !bTCP )	// if we are not forcing TCP service provider...
		{
			size = sizeof( sp_guidtext );
			if ( RegGet( "ServiceProvider", (LPBYTE)&sp_guidtext, &size ) == ERROR_SUCCESS)
			{
		 		// convert to GUID
				if ( GUIDFromString( sp_guidtext, &last_service_provider ) != S_OK )
				{
					DebugPrintf("unable to convert session guid from string\n");
				}else
				{
					ptr = (LPVOID) &last_service_provider;
				}
			}
		}else
		{
			ptr = (LPVOID) &DPSPGUID_TCPIP;
		}

		if ( !bTCP )
		{
			size = sizeof( TCPAddress.text );
			RegGet("TCPIP", (LPBYTE)&TCPAddress.text[0],&size);
		}
	}
	ServiceProvidersList.selected_item = 0;
	DirectPlayEnumerate( EnumServiceProviders, ptr );
	GetMultiplayerPrefs();
	GetServerPrefs();
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	DirectPlayEnumerate callback. Stores the service provider information.
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL WINAPI EnumServiceProviders(LPGUID lpGuid, LPTSTR lpSpName, DWORD dwMajorVersion,
						         DWORD dwMinorVersion, LPVOID lpv)
{
	
	if( ServiceProvidersList.items < MAXSERVICEPROVIDERS )
	{
		strncpy( ServiceProvidersList.item[ServiceProvidersList.items] , lpSpName , sizeof(ServiceProvidersList.item[0])  );
		ServiceProvidersGuids[ServiceProvidersList.items] = *lpGuid;
		if ( lpv && IsEqualGUID( lpGuid, (LPGUID) lpv ) )
		{
			ServiceProvidersList.selected_item = ServiceProvidersList.items;
			ServiceProviderSet = TRUE;
		}

		if( strlen(lpSpName ) >= sizeof(ServiceProvidersList.item[0]) && (CameraStatus == CAMERA_AtStart))
		{
			strcpy( ServiceProvidersList.item[ServiceProvidersList.items] + sizeof(ServiceProvidersList.item[0]) - 4 , "..." );
		}
		ServiceProvidersList.items++;
	}
    return(TRUE);
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	A Provider has been chosen create a directplay object
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

BOOL ExitProviderChosen ( MENUITEM * Item )
{
	HRESULT hr;
    LPGUID  lpGuid;
	char sp_guidtext[ 256 ];

	// release any previously created Dplay Objects...
	DPlayRelease();

	// used to determine whether to display IP address on VDU;
	IPAddressExists = FALSE;

	// get a pointer to the guid
	lpGuid = (LPGUID ) &ServiceProvidersGuids[ServiceProvidersList.selected_item];


	// Check for modem
	Modem2Modem = FALSE;
	if (IsEqualGUID( lpGuid, &DPSPGUID_MODEM) )
		Modem2Modem = TRUE;

	// Check for acceptable version of Direct Play
	if ( CheckDirectPlayVersion && !DirectPlayOK( lpGuid ) )
	{
		PrintErrorMessage ( YOU_NEED_TO_INSATLL_THE_DIRECT_PLAY_50A_UPDATE, 2, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}
	
	
	// remember the selection
	gSPGuid = *lpGuid;

	if ((hr = OnceServiceProviderChosen( &ServiceProvidersGuids[ServiceProvidersList.selected_item] , lpDPlayLobby, &glpDP , &TCPAddress.text[0])) != DP_OK)
	{
		PrintErrorMessage ( CONNECTION_INITIALIZATION_ERROR, 2, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}

	if (IsEqualGUID( lpGuid, &DPSPGUID_TCPIP) )
	{
		if (GetIPAdd())
			IPAddressExists = TRUE;
	}

//	if ( !CheckLegalIP() )
//	{
//		DPlayRelease();
//		MenuBack();
//		return FALSE;
//	}

	ZeroMemory(&ServiceProviderCaps,sizeof(ServiceProviderCaps));
    ServiceProviderCaps.dwSize = sizeof(DPCAPS);

	hr = IDirectPlayX_GetCaps( glpDP , &ServiceProviderCaps , 0);
	if( hr != DP_OK )
	{
		PrintErrorMessage ( COULDNT_GET_SERVICE_PROVIDER_CAPS, 2, NULL,
			                ERROR_USE_MENUFUNCS );
		return FALSE;
	}
	SugestedEnumSessionsTimeout = ServiceProviderCaps.dwTimeout * 2;


	if( ServiceProviderCaps.dwMaxBufferSize < MAXBIGPACKETBUFFERSIZE )
	{
		BIGPACKETBUFFERSIZE = ServiceProviderCaps.dwMaxBufferSize;
	}else{
		BIGPACKETBUFFERSIZE = MAXBIGPACKETBUFFERSIZE;
	}
	if( ServiceProviderCaps.dwMaxBufferSize < MAXBIGPACKETBUFFERSIZE )
	{
		SERVERPACKETBUFFERSIZE = ServiceProviderCaps.dwMaxBufferSize;
	}else{
		SERVERPACKETBUFFERSIZE = MAXBIGPACKETBUFFERSIZE;
	}

	
	
//	if (!AutoSelectConnection)
//		MenuChange ( Item );

	if ( (CameraStatus == CAMERA_AtStart) && Item )
		MenuChange ( Item );


	if( Modem2Modem )
	{
		MaxPlayersSlider.value = 2;
		MaxPlayersSlider.max = 2;
	}else{
		MaxPlayersSlider.max = MAX_PLAYERS;
		MaxPlayersSlider.value = PreferedMaxPlayers;
	}

	// convert guid to string...
	if ( StringFromGUID( &gSPGuid, sp_guidtext ) )
	{
		RegSetA( "ServiceProvider",  (LPBYTE)&sp_guidtext,  sizeof( sp_guidtext ) );
	}

    RegSetA("TCPIP", (CONST BYTE *)&TCPAddress.text[0], sizeof(TCPAddress.text));

/* hack */
	GetServiceProviderShortName();
/* end hack */

	return TRUE;
}

BOOL RefreshDPlay ( void )
{
	HRESULT hr;
    LPGUID  lpGuid;

	// release any previously created Dplay Objects...
	DPlayRelease();

	// get a pointer to the guid
	lpGuid = (LPGUID ) &ServiceProvidersGuids[ServiceProvidersList.selected_item];


	if ((hr = OnceServiceProviderChosen( &ServiceProvidersGuids[ServiceProvidersList.selected_item] , lpDPlayLobby, &glpDP , &TCPAddress.text[0])) != DP_OK)
	{
		PrintErrorMessage ( CONNECTION_INITIALIZATION_ERROR, 2, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}

	ZeroMemory(&ServiceProviderCaps,sizeof(ServiceProviderCaps));
    ServiceProviderCaps.dwSize = sizeof(DPCAPS);

	hr = IDirectPlayX_GetCaps( glpDP , &ServiceProviderCaps , 0);
	if( hr != DP_OK )
	{
		PrintErrorMessage ( COULDNT_GET_SERVICE_PROVIDER_CAPS, 2, NULL, ERROR_USE_MENUFUNCS );
		return FALSE;
	}
	SugestedEnumSessionsTimeout = ServiceProviderCaps.dwTimeout * 2;


	if( ServiceProviderCaps.dwMaxBufferSize < MAXBIGPACKETBUFFERSIZE )
	{
		BIGPACKETBUFFERSIZE = ServiceProviderCaps.dwMaxBufferSize;
	}else{
		BIGPACKETBUFFERSIZE = MAXBIGPACKETBUFFERSIZE;
	}
	if( ServiceProviderCaps.dwMaxBufferSize < MAXBIGPACKETBUFFERSIZE )
	{
		SERVERPACKETBUFFERSIZE = ServiceProviderCaps.dwMaxBufferSize;
	}else{
		SERVERPACKETBUFFERSIZE = MAXBIGPACKETBUFFERSIZE;
	}

	return TRUE;
}

void SetUpGameSubType( int type )
{
	switch ( CTFSlider.value )
	{
	case CTF_MUST_CARRY:
		NeedFlagAtHome			= TRUE;
		OwnFlagTeleportsHome	= FALSE;
		CanCarryOwnFlag			= TRUE;
		break;
	case CTF_CAN_CARRY:
		NeedFlagAtHome			= FALSE;
		OwnFlagTeleportsHome	= FALSE;
		CanCarryOwnFlag			= TRUE;
		break;
	case CTF_ONE_TOUCH:
		NeedFlagAtHome			= FALSE;
		OwnFlagTeleportsHome	= TRUE;
		CanCarryOwnFlag			= FALSE;
		break;
	case CTF_NO_RETURN:
		NeedFlagAtHome			= FALSE;
		OwnFlagTeleportsHome	= FALSE;
		CanCarryOwnFlag			= FALSE;
		break;
	case CTF_STANDARD:
	default:
		NeedFlagAtHome			= TRUE;
		OwnFlagTeleportsHome	= TRUE;
		CanCarryOwnFlag			= FALSE;
	}
}

void SetUpGameType( int type ) 
{
	TeamGame = FALSE;
	BombTag = FALSE;
	CTF = FALSE;
	NeedFlagAtHome			= FALSE;
	OwnFlagTeleportsHome	= FALSE;
	CanCarryOwnFlag			= FALSE;
	CaptureTheFlag = FALSE;
	BountyHunt = FALSE;
	
	switch ( type )
	{
	case GAME_Normal:
		break;
	case GAME_Team:
		TeamGame = TRUE;
		break;
	case GAME_Tag:
		BombTag = TRUE;
		break;
	case GAME_CTF:
		CTF = TRUE;
		TeamGame = TRUE;

		SetUpGameSubType( CTFSlider.value );

		break;
	case GAME_CaptureFlag:
		CaptureTheFlag = TRUE;
		TeamGame = TRUE;
		break;
	case GAME_BountyHunt:
		BountyHunt = TRUE;
		break;
	case GAME_TeamBounty:
		BountyHunt = TRUE;
		TeamGame = TRUE;
		break;
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Hosting a session...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL StartAHostSession ( MENUITEM * Item )
{
	HRESULT hr;
	int i;
	DWORD		Temp;
	BYTE		msg;
	int			size;
	LONGLONG	TempTime;
	uint32		Seed;
	uint32		PackedInfo[ MAX_PICKUPFLAGS ];

	IsServer = FALSE;
	IsServerGame = FALSE;

	if ( MaxPlayersSlider.max != 2 )
		PreferedMaxPlayers = MaxPlayersSlider.value;
	SetMultiplayerPrefs();

	Seed = timeGetTime();
	Seed1 = (uint16) ( ( Seed >> 16 ) & 0xffff );
	Seed2 = (uint16) ( Seed & 0xffff );
	CopyOfSeed1 = Seed1;
	CopyOfSeed2 = Seed2;

	PlayDemo = FALSE;
	IsHost = TRUE;

	TeamGame = FALSE;
	BombTag = FALSE;
	CaptureTheFlag = FALSE;
	BountyHunt = FALSE;
	CTF = FALSE;

	SetUpGameType( GameType );

	SetBikeMods( 0 );

	MaxKills = MaxKillsSlider.value;

	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	RandomStartPosModify = (uint16) ( ( TempTime * 71.42857143 ) / Freq );

	if ( !IsLobbyLaunched )
	{
			d3dappi.lpDD->lpVtbl->FlipToGDISurface(d3dappi.lpDD);

		// create session
		if ((hr = DPlayCreateSession( &MultiPlayerGameName.text[0])) != DP_OK)
		{
			return FALSE;
		}
	
		// create player
		if ((hr = DPlayCreatePlayer(&dcoID, &biker_name[0], NULL, NULL, 0)) != DP_OK)
		{
		    return FALSE;
		}
	}else
	{
		// ammend session name to include level name...
		DPlayGetSessionDesc();
		glpdpSD->dwMaxPlayers = MaxPlayersSlider.value;
		DPlaySetSessionDesc( 1 );

		UpdateSessionName( MultiPlayerGameName.text );
	}

	DPlayUpdateInterval	= 60.0F / PacketsSlider.value;
	OldPPSValue = PacketsSlider.value;
	
	SetupDplayGame();
	
	DPlayGetSessionDesc();
	

	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		GameStatus[i] = STATUS_Null;
	}
	
	WhoIAm = 0;								// I was the first to join...
	Ships[WhoIAm].dcoID = dcoID;

	if ( TeamGame )
		MenuChange( &NewTeamItem );
	else
		MenuChange( Item );

	MyGameStatus = STATUS_StartingMultiplayer;
	
	Current_Camera_View = 0;				// set camera to that view
//	Ships[WhoIAm].enable = 1;
	Ships[WhoIAm].Pickups = 0;
	Ships[WhoIAm].RegenSlots = 0;
	Ships[WhoIAm].Mines = 0;
	Ships[WhoIAm].Triggers = 0;
	Ships[WhoIAm].TrigVars = 0;
	StatsStatus = 1;						// I started it so the Stats Are Valid...
	
	memset(&Names, 0, sizeof(SHORTNAMETYPE) );
    strncpy( (char*) &Names[WhoIAm][0] , &biker_name[0] , 7 );
	Names[WhoIAm][7] = 0;
	Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	
	NewLevelNum = LevelList.selected_item;	// I Select Which Level We Start on...

	if( TimeLimit.value )
	{
		CountDownOn = TRUE;
	}
	else
	{
		CountDownOn = FALSE;
	}


	if( RecordDemo || RecordDemoToRam )
	{
		uint32 mp_version = MULTIPLAYER_VERSION;
		uint32 flags;
		time_t now_time;
		struct tm *now;

		RecordDemo = TRUE;
		time( &now_time );
		now = localtime( &now_time );
#ifndef HOST_CHOOSES_DEMO_NAME
		if ( now )
		{
			sprintf( DemoGameName.text, "%s's Demo %d.%02d %d-%d-%d",
				biker_name,
				now->tm_hour, now->tm_min,
				1 + now->tm_mon, now->tm_mday,
				1900 + now->tm_year );
		}
		else
		{
			sprintf( DemoGameName.text, "%s's Demo",
				biker_name );
		}
#endif
		DemoFp = fopen( DemoFileName( DemoGameName.text ) , "wb" );
		setvbuf( DemoFp, NULL, _IONBF , 0 );		// size of stream buffer...

		Demo_fwrite( &mp_version, sizeof( mp_version ), 1, DemoFp );
		flags = 0;
		if( TeamGame )
			flags |= TeamGameBit;
		if( BombTag )
			flags |= BombGameBit;
		if( CTF )
			flags |= CTFGameBit;
		if( CaptureTheFlag )
			flags |= FlagGameBit;
		if ( BountyHunt )
			flags |= BountyGameBit;

		Demo_fwrite( &CopyOfSeed1, sizeof( CopyOfSeed1 ), 1, DemoFp );
		Demo_fwrite( &CopyOfSeed2, sizeof( CopyOfSeed2 ), 1, DemoFp );
		Demo_fwrite( &RandomPickups, sizeof( RandomPickups ), 1, DemoFp );
		PackPickupInfo( &PackedInfo[ 0 ] );
		Demo_fwrite( &PackedInfo[ 0 ], sizeof( PackedInfo ), 1, DemoFp );

		Demo_fwrite( &flags, sizeof( flags ), 1, DemoFp );
		Demo_fwrite( &RandomStartPosModify, sizeof( RandomStartPosModify ), 1, DemoFp );
		for( i = 0 ; i < 256 ; i++ )
		{
			Demo_fwrite( &ShortLevelNames[NewLevelNum][i], sizeof(char), 1, DemoFp );
			if( ShortLevelNames[NewLevelNum][i] == 0 )
			{
				break;
			}
		}

		// Best way I can Think of to send the Host Name to the demo file...
		Temp = 1;
		TempTime = 1;
		Demo_fwrite( &TempTime, sizeof(LONGLONG), 1, DemoFp );
		size = sizeof( NAMEMSG );
		Demo_fwrite( &size, sizeof(int), 1, DemoFp );
		Demo_fwrite( &Temp, sizeof(DWORD), 1, DemoFp );
		msg = MSG_NAME;
		Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );
		msg = 0;
		Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );				// Whos Name it is..
		Demo_fwrite( &biker_name[0], 7, 1, DemoFp );
		msg = 0;
		Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );				// terminator for name..
	}
	
	BrightShips = MyBrightShips;

	
	if ( IsLobbyLaunched )
	{
		ContinueLobbyLaunch();
	}

	tracker_addr = 0;
	DPStartThread();

	return TRUE;
}

void StartAPseudoHostSession( MENUITEM *Item )
{
	SetMultiplayerPrefs();
	
	SetUpGameType( GameType );

	SendGameMessage(MSG_GAMEPARAMETERS, 0, 0, 0, 0);

	MyGameStatus = STATUS_PseudoHostWaitingForAck;
	MenuChangeEx( &MENU_NEW_PseudoHostWaitingForAck );
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get a List of Current Sessions
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void GetCurrentSessions( MENU *Menu )
{
	int i;
	IsHost = FALSE;
	IsPseudoHost = FALSE;
	SessionsList.items = 0;
	SessionsList.top_item = 0;
	SessionsList.display_items = 8;
	SessionsList.selected_item = -1;

	PlayersList.items = 0;
	PlayersList.top_item = 0;
	PlayersList.display_items = 16;
	PlayersList.selected_item = -1;


	SessionsRefreshActive = TRUE;

	for( i = 0 ; i < MAXSESSIONS ; i++ )
	{
		SessionsRefresh[i] = FALSE;
	}
	
		d3dappi.lpDD->lpVtbl->FlipToGDISurface(d3dappi.lpDD);

	// enum sessions and we will decide the timeout
	DPlayEnumSessions(SugestedEnumSessionsTimeout, EnumSessions, (LPVOID) NULL, DPENUMSESSIONS_AVAILABLE | DPENUMSESSIONS_ASYNC);
//	DPlayEnumSessions(SugestedEnumSessionsTimeout, EnumSessions, (LPVOID) NULL, DPENUMSESSIONS_ALL | DPENUMSESSIONS_ASYNC);

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get a List of Current Sessions	Again..
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

void GetCurrentSessions_ReScan( MENUITEM *Item )
{
	int i;
	if( !SessionsRefreshActive )
	{
		SessionsRefreshActive = TRUE;
		
		for( i = 0 ; i < MAXSESSIONS ; i++ )
		{
			SessionsRefresh[i] = FALSE;
		}
 
		// enum sessions and we will decide the timeout
		DPlayEnumSessions(SugestedEnumSessionsTimeout, EnumSessions, (LPVOID) NULL, DPENUMSESSIONS_AVAILABLE | DPENUMSESSIONS_ASYNC);
//		DPlayEnumSessions(SugestedEnumSessionsTimeout, EnumSessions, (LPVOID) NULL, DPENUMSESSIONS_ALL | DPENUMSESSIONS_ASYNC);
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	EnumSessions callback. Inserts session description information in the Sessions List
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/


BOOL WINAPI EnumSessions(LPCDPSESSIONDESC2 lpDPSessionDesc, LPDWORD lpdwTimeOut, DWORD dwFlags, 
                        LPVOID lpContext)
{
	int i;
	GUID	tempGuid;
    if(dwFlags & DPESC_TIMEDOUT)
	{
		SessionsRefreshActive = FALSE;

		SessionsListCopy.items = 0;
		SessionsListCopy.top_item = 0;
		SessionsListCopy.display_items = 8;
		SessionsListCopy.selected_item = -1;

		for( i = 0 ; i < SessionsList.items ; i ++ )
		{
			if( SessionsRefresh[i] )
			{
				memcpy(&SessionsCopy[SessionsListCopy.items] ,&Sessions[i] , sizeof(DPSESSIONDESC2) );
				memcpy(&SessionsListCopy.item[SessionsListCopy.items] ,&SessionsList.item[i] , sizeof(SessionsList.item[i]) );

				if( i == SessionsList.selected_item )
				{
					SessionsListCopy.selected_item = SessionsListCopy.items;
				}
				SessionsListCopy.items++;
			}
		}
		memcpy(&Sessions ,&SessionsCopy , sizeof(Sessions) );
		memcpy(&SessionsList ,&SessionsListCopy , sizeof(SessionsList) );
		for( i = 0 ; i < SessionsList.items ; i ++ )
		{
			SessionsRefresh[i] = TRUE;
		}
		return FALSE;       // don't try again
	}

	tempGuid = lpDPSessionDesc->guidInstance;
	for( i = 0 ; i < SessionsList.items ; i++ )
	{
		if(	IsEqualGuid( &tempGuid, &Sessions[i].guidInstance) )
		{
			SessionsRefresh[ i ] = TRUE;
			// allready in the list...
			return TRUE;
		}
	}

	if( ( lpDPSessionDesc->dwUser4 & 0xffff ) == Version )
	{
		if( SessionsList.items < MAXSESSIONS )
		{
	
			// store away its guid...
			Sessions[SessionsList.items] = *lpDPSessionDesc;
			strncpy( SessionNames[ SessionsList.items ], lpDPSessionDesc->lpszSessionNameA, sizeof( SessionNames[ SessionsList.items ] ) );
	
#ifdef UNICODE
			strncpy( SessionsList.item[SessionsList.items] , lpDPSessionDesc->lpszSessionName , sizeof(SessionsList.item[0])  );
#else
			strncpy( SessionsList.item[SessionsList.items] , lpDPSessionDesc->lpszSessionNameA , sizeof(SessionsList.item[0])  );
#endif
	
#ifdef UNICODE
			if( strlen(lpDPSessionDesc->lpszSessionName ) >= sizeof(SessionsList.item[0]) )
#else
			if( strlen(lpDPSessionDesc->lpszSessionNameA ) >= sizeof(SessionsList.item[0]) )
#endif
			{
				strcpy( SessionsList.item[SessionsList.items] + sizeof(SessionsList.item[0]) - 4 , "..." );
			}
			SessionsRefresh[SessionsList.items] = TRUE;

			SessionsList.items++;
		}
	}

    return(TRUE);
}

void GetSessionInfo ( LPDPSESSIONDESC2 sd )
{
	int32	Time;

	// new additions ( previously in MSG_INIT )
	if( sd->dwUser3 & HarmTeamMatesBit )
	{
		HarmTeamMates = TRUE;
	}else{
		HarmTeamMates = FALSE;
	}
	if( sd->dwUser3 & BrightShipsBit )
	{
		BrightShips = TRUE;
	}else{
		BrightShips = FALSE;
	}
	if( sd->dwUser3 & ResetKillsPerLevelBit )
	{
		ResetKillsPerLevel = TRUE;
	}else{
		ResetKillsPerLevel = FALSE;
	}
	
	if( sd->dwUser3 & BikeExhaustBit )
	{
		BikeExhausts = TRUE;
	}else{
		BikeExhausts = FALSE;
	}
	   
	// if we are going to be the pseudohost, we must be responsible for setting collision type
	if ( ( sd->dwUser3 & ServerGameStateBits ) != SERVER_STATE_NeedHost )
	{
		ColPerspective = ( sd->dwUser3 & CollisionTypeBits ) >> Collision_Type_BitShift;
	}

	Time = ( ( sd->dwUser3 & GameTimeBit ) >> GameTimeBit_Shift );

	if( Time )
	{
		TimeLimit.value = Time;
		CountDownOn = TRUE;
	}
	else
	{
		CountDownOn = FALSE;
	}

	RandomStartPosModify = (WORD) (sd->dwUser2 & 0xffff);
	
	if( sd->dwUser3 & TeamGameBit )
	{
		TeamGame = TRUE;
	}else{
		TeamGame = FALSE;
	}
	if( sd->dwUser3 & BombGameBit )
	{
		BombTag = TRUE;
	}else{
		BombTag = FALSE;
	}
	if( sd->dwUser3 & FlagGameBit )
	{
		CaptureTheFlag = TRUE;
	}else{
		CaptureTheFlag = FALSE;
	}

	// check if any server bits are set
	// server could be in one of 3 states ( see main.h )
	if( sd->dwUser3 & ServerGameStateBits )
	{
		IsServerGame = TRUE;
	}else{
		IsServerGame = FALSE;
	}

	CTF = ( sd->dwUser3 & CTFGameBit ) ? TRUE : FALSE;
	
	// new addition ( previously in MSG_INIT )
	if ( CTF )
	{
		SetUpGameSubType( CTF_Type_Decode( sd->dwUser3 ) );
	}

	BountyHunt = ( sd->dwUser3 & BountyGameBit ) ? TRUE : FALSE;

	UseShortPackets = ( sd->dwUser3 & ShortPacketsBit ) ? TRUE : FALSE;
	BigPackets = ( sd->dwUser3 & BigPacketsBit ) ? TRUE : FALSE;

	MaxKills = (int16)( ( sd->dwUser2 & MaxKillsBits ) >> MaxKills_Shift );
}

BOOL JoinASessionEx ( MENU *Menu )
{
	MENUITEM item;

	memset( &item, 0, sizeof( MENUITEM ) );
	item.Value = Menu;
	return JoinASession( &item );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Join a Session...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL JoinASession ( MENUITEM * Item )
{
	HRESULT hr;
    LPGUID  lpGuid;
	DPSESSIONDESC2 temp_sd;

	PlayDemo = FALSE;
	IsServer = FALSE;
	IsServerGame = FALSE;
	TrackerOveride = FALSE;

	SetBikeMods( 0 );

#ifdef MANUAL_SESSIONDESC_PROPAGATE
	// ensure any SD copy is cleared, so that the first SD we get is the actual one ( not a manually propagated version )
	if ( glpdpSD_copy )
	{
		free( glpdpSD_copy );
		glpdpSD_copy = NULL;
	}
#endif

	if ( !IsLobbyLaunched )
	{
		switch( Sessions[ SessionsList.selected_item ].dwUser3 & ServerGameStateBits )
		{
		case SERVER_STATE_HostChoosing:	
			return TRUE;
		default:
			break;
		}
		
		// get a pointer to the guid
		lpGuid = (LPGUID ) &Sessions[SessionsList.selected_item].guidInstance;

		// open session
		if ((hr = DPlayOpenSession( lpGuid)) != DP_OK)
		{
			PrintErrorMessage ( COULDNT_OPEN_SESSION, 1, NULL, ERROR_USE_MENUFUNCS );
			return FALSE;
		}
		// create player
		if ((hr = DPlayCreatePlayer(&dcoID, &biker_name[0], NULL, NULL, 0)) != DP_OK)
		{
			PrintErrorMessage ( COULDNT_CREATE_PLAYER, 1, NULL, ERROR_USE_MENUFUNCS );
			return FALSE;
		}
	}


	DPlayGetSessionDesc();

	GetSessionInfo( glpdpSD );

	SetupDplayGame();

	Rejoining = FALSE;

	temp_sd = Sessions[SessionsList.selected_item];

	// zero Some stuff cos they might have changed..or they might be pointers...
	temp_sd.dwCurrentPlayers = 0;
	Old_Session.dwCurrentPlayers = 0;
	temp_sd.lpszSessionNameA = 0;
	Old_Session.lpszSessionNameA = 0;
	temp_sd.lpszPasswordA = 0;
	Old_Session.lpszPasswordA = 0;

	temp_sd.dwFlags = 0;
	Old_Session.dwFlags = 0;
	temp_sd.dwSize = 0;
	Old_Session.dwSize = 0;
	temp_sd.dwMaxPlayers = 0;
	Old_Session.dwMaxPlayers = 0;
	temp_sd.dwReserved1 = 0;
	Old_Session.dwReserved1 = 0;
	temp_sd.dwReserved2 = 0;
	Old_Session.dwReserved2 = 0;

	temp_sd.dwUser2 = 0;
	Old_Session.dwUser2 = 0;
	temp_sd.dwUser3 = 0;
	Old_Session.dwUser3 = 0;

	if ( !IsServerGame )
	{	
		if( ( Mymemcmp( (BYTE*) &Old_Session , (BYTE*) &temp_sd , sizeof(DPSESSIONDESC2 ) ) == TRUE ) &&
			( _stricmp( (char*) &biker_name[0], (char*) &Old_Name[0] ) == 0 ) )
		{
		   	Rejoining = TRUE;
		}
	}
	
	WhoIAm = 0xff;

	MyGameStatus = STATUS_GetPlayerNum;

	GetPlayerNumCount1 = 0.0F;
	GetPlayerNumCount2 = 60.0F * 30.0F;	// 30 Seconds...
	GetPlayerNumCount = 0;

	if ( TeamGame )
		GetPlayerNumMenu = &MENU_NEW_WatchTeamSelect;
	else
		GetPlayerNumMenu = (MENU*) Item->Value;

	// Stops us going straight in to the game....
	OverallGameStatus = STATUS_Null;

	return TRUE;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Select a Session and show players in it..
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SelectSession( MENUITEM *Item )
{
	
	if ((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU))
	{
		JoinASession (&NewJoinItem);
	}
	else
	{
		JoinASession ( &JoinItem );
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	EnumeratePlayer callback. Inserts player information into the PlayersList
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL WINAPI EnumPlayers(DPID pidID, DWORD dwPlayerType, LPCDPNAME lpName,
    DWORD dwFlags, LPVOID lpContext)
{

	if( PlayersList.items < MAX_PLAYERS )
	{
		PlayerIDs[PlayersList.items] = pidID;

#ifdef UNICODE
		strncpy( PlayersList.item[PlayersList.items] , lpName->lpszShortName , sizeof(PlayersList.item[0])  );
#else
		strncpy( PlayersList.item[PlayersList.items] , lpName->lpszShortNameA , sizeof(PlayersList.item[0])  );
#endif

#ifdef UNICODE
		if( strlen(lpName->lpszShortName ) >= sizeof(PlayersList.item[0]) )
#else
		if( strlen(lpName->lpszShortNameA ) >= sizeof(PlayersList.item[0]) )
#endif
		{
			strcpy( PlayersList.item[PlayersList.items] + sizeof(PlayersList.item[0]) - 4 , "..." );
		}
		PlayersList.items++;
	}
    return(TRUE);
}

BOOL WINAPI EnumLobbyPlayers(DPID pidID, DWORD dwPlayerType, LPCDPNAME lpName,
    DWORD dwFlags, LPVOID lpContext)
{

	if( NumLobbyPlayers < MAX_PLAYERS )
	{
		LobbyPlayerIDs[ NumLobbyPlayers++ ] = pidID;
	}
    return(TRUE);
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get The Players In the Current Session...
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void GetPlayersInCurrentSession( MENUITEM *Item )
{
	int i;
#ifdef DEBUG_ENUM_PLAYERS
	char buf[ 128 ];
#endif
	
	//Bodge -= framelag;
	//if( Bodge <= 0.0F )
	{
#ifdef ENUM_PLAYERS_ON_TITLES
		DPlayGetSessionDesc();

		NumOfPlayersSlider.value = (int) glpdpSD->dwCurrentPlayers;

		if (NumOfPlayersSlider.value != NumOfPlayersSlider.oldvalue)
		{	
//		 	NumOfPlayersSlider.oldvalue = NumOfPlayersSlider.value;
			NumOfPlayersSlider.redraw_req = TRUE;
		}
		
		PlayersList.items = 0;
		PlayersList.top_item = 0;
		PlayersList.selected_item = -1;
		DPlayEnumPlayers( NULL, EnumPlayers, (LPVOID) NULL, 0);
#else
		PlayersList.items = 0;
		PlayersList.top_item = 0;
		PlayersList.selected_item = -1;
		
		NumOfPlayersSlider.value = 0;

		for ( i = 0; i < MAX_PLAYERS; i++ )
		{
#ifdef DEBUG_ENUM_PLAYERS
			sprintf( buf, "player %d status %x", i, GameStatus[ i ] );
			Print4x5Text( buf, 10, 10 * i + 50, 2 );
#endif
			if ( IsServerGame )
			{
				if ( !i )
				{
					continue;	// don't add server name to list of players
				}
			}
			
			if( ( GameStatus[ i ] == MyGameStatus ) || ( i == WhoIAm ) )
			{
				NumOfPlayersSlider.value++;
				strcpy( PlayersList.item[ PlayersList.items++ ], Names[ i ] );
			}
		}

		if (NumOfPlayersSlider.value != NumOfPlayersSlider.oldvalue)
		{	
			NumOfPlayersSlider.redraw_req = TRUE;
		}
#endif
		Bodge = 30.0F;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	The host has waited for everyone now the game will init and start..
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void GoToSynchup ( MENUITEM * Item )
{

	if( TeamGame )
	{
		if (CameraStatus == CAMERA_AtStart)
		{
			MenuChange( &TeamItem );
		}else
		{
			MenuChange( &NewTeamItem );
		}
	}else{

		if (CameraStatus != CAMERA_AtStart)
		{	
			LastMenu = CurrentMenu;
			VduClear();
		}

		CurrentMenu = NULL;
		CurrentMenuItem = NULL;
		MenuStackLevel = 0;
	}

}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	The host has waited for everyone now the game will init and start..
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void TeamGoToSynchup ( MENUITEM * Item )
{
	/*
	// put there team numbers backin..
	//ReAssembleTeamNumbers();
	LastMenu = CurrentMenu;
	VduClear();
	MenuAbort();
	*/
	PreSynchupStatus = MyGameStatus;
	MyGameStatus = STATUS_WaitingToStartTeamGame;
	MenuChangeEx( &MENU_NEW_GeneralLoading );
	MenuDraw( CurrentMenu );	// because menu processing will not be done when MyGameStatus changes

}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Bail out of a multiplayer game before it starts...
	Input		:	MENU * Menu
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void BailMultiplayer( MENU * Menu )
{
	MyGameStatus = STATUS_Left;
    if ( ( glpDP != NULL ) && ( dcoID != 0 ) && ( WhoIAm < MAX_PLAYERS ) )
    {
		SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
	}

	ChangeServiceProvider( Menu );
}

void BailMultiplayerFrontEnd( MENU *Menu )
{
	int selected_item;

	// if launched from lobby, player has option to quit - service provider etc. is all pre chosen
	if ( IsLobbyLaunched )
		return;
	
	selected_item = ServiceProvidersList.selected_item;

	BailMultiplayer( Menu );

	GetServiceProviders( NULL );
	ServiceProvidersList.selected_item = selected_item;

	ExitProviderChosen( NULL );

	SessionsList.selected_item = -1;
	InitMySessionsList();
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Changeing Service Provider...
	Input		:	MENU * Menu
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ChangeServiceProvider( MENU * Menu )
{
	// cannot change service provider if lobby launched
	if ( IsLobbyLaunched )
		return;
					 
	if( dcoID )
	{
		DPlayDestroyPlayer(dcoID);
		dcoID = 0;
	}
	DPlayRelease();
	MyGameStatus = STATUS_Title;

	ServiceProvidersList.selected_item = -1;
}

void ExitServerSetup( MENU *Menu )
{
	MenuBackSpecific( &MENU_NEW_ChooseServerGameType, FALSE );
	ChangeServiceProvider( NULL );
}

void ChangeServiceProviderPseudoHost( MENU * Menu )
{
	MenuBackSpecific( &MENU_NEW_CreateGamePseudoHost, FALSE );	// ignore exit menu funcs
	
	if( dcoID )
	{
		DPlayDestroyPlayer(dcoID);
		dcoID = 0;
	}

	DPlayRelease();
	MyGameStatus = STATUS_Title;

	RefreshDPlay();

	OKToJoinSession = FALSE;
}

void InitTeamLists( MENU *Menu )
{
	int i;

	TeamList[0] = PlayersList;
	TeamList[0].selected_item = -1;
	TeamList[0].display_items = 12;
	TeamMembers[0] = PlayersList.items;
	
	for (i = 1; i < MAX_TEAMS; i++)
	{
		TeamList[i].items = 0;
		TeamList[i].top_item = 0;
		TeamList[i].display_items = 12;			// hard wired in because MAX_PLAYERS will increase
		TeamList[i].selected_item = -1;

		TeamMembers[i] = 0;
	}

	for ( i = 0; i < MAX_PLAYERS; i++ )
		PlayerReady[ i ] = FALSE;
}

void InitExistingGameJoin( MENU *Menu )
{
	int i;

	AllowQuitForLobby( Menu );
	
	InitTeamLists( NULL );

	for (i = 0; i < MAX_TEAMS; i++)
	{
		strcpy(TeamCurrentScore[i], ": 0");
	}

	InitTitleMessaging();
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Set the Team player lists..
	Input		:	MENU * Menu
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitTeamSelection( MENU *Menu )
{
	int i, j;
	MENUITEM *item;

	AllowQuitForLobby( Menu );

	if ( IsLobbyLaunched )
		NoMenuBack = TRUE;

	InitTeamLists( NULL );

	for( item = Menu->Item; item->x >=0; item++ )
	{
		if ( item->FuncDraw == DrawReadyButton )
		{
			
			if ( IsServer )
			{
				item->FuncSelect = NULL;
				PlayerReady[ WhoIAm ] = TRUE;
			}else
			{
				item->FuncSelect = PlayerConfirm;
			}
			
		}

		if 	( ( item->Variable == &TeamList[0] ) ||
		    ( item->Variable == &TeamList[1] ) ||
		    ( item->Variable == &TeamList[2] ) ||
		    ( item->Variable == &TeamList[3] ) ) 
		{
			if( IsServer )
			{
				item->FuncSelect = NULL;
			}else
			{
				item->FuncSelect = SelectTeamList;
			}
		}
	}

	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		TeamIDs[0][i] = PlayerIDs[i];

		for (j = 1; j < MAX_TEAMS; j++) 
		{
			TeamIDs[j][i] = 0;
		}
		if( IsHost )
			TeamNumber[i] = 255;
	}

//	for (i = 0; i < TeamList[0].items; i++)
//		TeamNumber[i] = 0;

	TeamNumber[WhoIAm] = 0;		// start myself off in team 0

	// if we went into this menu automatically (ie. we are not the host), Menustate
	// will still be in whatever state it was in in the last menu, so reset...
	MenuState = MENUSTATE_Select;

	InitTitleMessaging();
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Swap a list item from 1 list to another....
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SwapListItem( LIST * Source , LIST * Dest )
{
	int i;
	char TempItem[32];
	LIST	TempList;

	if( ( Source->selected_item != -1) && ( Source->items != 0 ) )
	{
		TempList.items = 0;
		TempList.top_item = 0;
		TempList.display_items = MAX_PLAYERS;
		TempList.selected_item = -1;
	

		strcpy( &TempItem[0] , Source->item[Source->selected_item] );
		
		for( i = 0 ; i < Source->items ; i++ )
		{
			if( i != Source->selected_item )
			{
				strcpy( &TempList.item[TempList.items][0] , Source->item[i]);
				TempList.items++;
			}
		}
		for( i = 0 ; i < TempList.items ; i++ )
		{
			strcpy( Source->item[i] , &TempList.item[i][0]);
		}
		Source->items = TempList.items;
		Source->top_item = 0;
		Source->display_items = MAX_PLAYERS;
		Source->selected_item = -1;

		//strcpy( Dest->item[Dest->items] , &TempItem[0]);
		//Dest->items++;

		// *** changed to put new item at top of list, so it is always visible ***

		TempList = *Dest;

		Dest->items = 0;
		Dest->top_item = 0;
		Dest->selected_item = 0;

		strcpy( Dest->item[Dest->items++], &TempItem[0]);

		for (i = 0; i < TempList.items; i++ )
		{
			strcpy( Dest->item[Dest->items++], TempList.item[i]);
		}
	}

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Swap a Teasm id from 1 list to another..
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SwapTeamIDs( LIST * Source , LIST * Dest )
{
	int i,e, sourceteam, destteam;
	DPID	TempID;
	DPID	TempIDs[MAX_PLAYERS];
	DPID	*SourceIDs;
	DPID	*DestIDs;

	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		TempIDs[i] = 0;
	}

	sourceteam = 0;
	destteam = 0;
	
	for (i = 0; i < MAX_TEAMS; i++)
	{
		if (Source == &TeamList[i])
		{
			SourceIDs = TeamIDs[i];
			sourceteam = i;
		}
		if (Dest == &TeamList[i])
		{
			DestIDs = TeamIDs[i];
			destteam = i;
		}
	}

	TempID = SourceIDs[Source->selected_item];

	e = 0;
	for( i = 0 ; i < TeamMembers[sourceteam]; i++ )
	{
		if( i != Source->selected_item )
		{
			TempIDs[e] = SourceIDs[i];
			e++;
		}
	}
	for( i = 0 ; i < e ; i++ )
	{
		SourceIDs[i] = TempIDs[i];
	}

	e = 0;
	DestIDs[TeamMembers[destteam]] = TempID;

	TeamMembers[sourceteam]--;
	TeamMembers[destteam]++;

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Re Assemble Team Numbers...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ReAssembleTeamNumbers(int *dummy)
{
	int i,e, team;

	UpdateSessions( NULL );

	for (team = 0; team < MAX_TEAMS; team++)
	{
 		for( i = 0 ; i < TeamMembers[team] ; i++ )
		{
			for( e = 0 ; e < MAX_PLAYERS ; e ++ )
			{
				if (e != WhoIAm)
				{
					if( PlayerIDs[e] == TeamIDs[team][i] )
					{
						TeamNumber[e] = team;
					}
				}
			}
		}
	}

	DistributeTeamsToLists( NULL );
}

void CheckForMenuChange(int *dummy)
{
	if (TeamNumber[0] != 255)
		MenuChange( &WatchTeamSelectionItem );

	GetPlayersInCurrentSession ( NULL );
}

void DistributeTeamsToLists(int *dummy)
{
	int team, player, num_players;
	BOOL OkayToStart = TRUE;
	int16 TeamScore[MAX_TEAMS];

	UpdateSessions( NULL );
	memset(&TeamScore, 0, sizeof(int16) * MAX_TEAMS);

	for (team = 0; team < MAX_TEAMS; team++)
	{
		TeamList[team].items = 0;
		TeamList[team].top_item = 0;
		TeamList[team].display_items = 12;
	}

	num_players = 0;
	
	for ( player = ( IsServerGame ? 1 : 0 ); player < MAX_PLAYERS; player++ )	// server dosn't count as player
	{
		// player has left, therefore reset their team number...
		if ((GameStatus[player] == STATUS_Left) || (GameStatus[player] == STATUS_LeftCrashed))
		{
			TeamNumber[player] = 255;
		}

		team = TeamNumber[player];
		if (team < MAX_TEAMS)
		{
			if ( ( player == WhoIAm ) || ( GameStatus[ player ] == STATUS_StartingMultiplayer ) || ( GameStatus[ player ] == STATUS_Normal ) )
			{
				num_players++;
				
				if ( !PlayerReady[player] && ( GameStatus[ player ] != STATUS_Normal ) )
				{
					if (Pulse >= 0.5F)			 
					{
						TeamList[team].item[TeamList[team].items++][0] = 0;
					}
					else
					{
						strcpy(&TeamList[team].item[TeamList[team].items++][0], &Names[player][0]);
					}

					OkayToStart = FALSE;
				}else
				{
					strcpy(&TeamList[team].item[TeamList[team].items++][0], &Names[player][0]);
				}
			}
		}

		if (TeamNumber[player] < MAX_TEAMS)
		{
			TeamScore[TeamNumber[player]] += Ships[player].Kills;
  		}
	}

	if ( IsHost && OkayToStart && num_players )
	{
		TeamGoToSynchup(NULL);
	}

	for (team = 0; team < MAX_TEAMS; team++)
	{
		sprintf(TeamCurrentScore[team], ": %d",TeamScore[team]);
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	My Memory Compare...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL Mymemcmp( BYTE * buf1 , BYTE * buf2 , int size )
{
	while( size--)
	{
		if( *buf1++ != *buf2++ )
		{
			return FALSE;
		}
	}
	return TRUE;
}

int GetIPAdd( void )
{
	int error;
	static char hname[1024];
	struct hostent *hp;
	uint32 LoBytes = 0x000000FF;
	uint32 prefIPAddress;
	uint32 ip1, ip2, ip3, ip4;

	if ( error = gethostname( hname, sizeof( hname ) ) )
	{

		error = WSAGetLastError();
		switch( error )
		{
		case WSAEFAULT:
			DebugPrintf("WSAEFAULT\n");
			break;
		case WSANOTINITIALISED:
			DebugPrintf("WSANOTINITIALISED\n");
			break;
		case WSAENETDOWN:
			DebugPrintf("WSAENETDOWN\n");
			break;
		case WSAEINPROGRESS:
			DebugPrintf("WSAEINPROGRESS\n");
			break;
		default:
			DebugPrintf("default\n");
			break;
		}
		
		return 0;
	}

	hp = gethostbyname ( hname );	
	if ( !hp )
		return 0;

	// if preferred address was specified in heartbeat.txt, verify
	if ( sscanf( host_ip, " %d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4 ) == 4 )
	{
		char FAR * FAR *add; 

		prefIPAddress = ip1 | ( ip2 << 8 ) | ( ip3 << 16 ) | ( ip4 << 24 );

		add = hp->h_addr_list;
		while ( *add )
		{
			IPAddress = *(uint32 *) *add;
			if ( IPAddress == prefIPAddress )
			{
				sprintf(IPAddressText, "%d.%d.%d.%d", IPAddress & LoBytes, (IPAddress >> 8) & LoBytes, (IPAddress >> 16) & LoBytes, (IPAddress >> 24) & LoBytes);
				return 1;
			}
			add++;
		}
	}

	// if we get here, just use 1st IP address in list
	IPAddress = *(uint32 *) *hp->h_addr_list;
	sprintf(IPAddressText, "%d.%d.%d.%d", IPAddress & LoBytes, (IPAddress >> 8) & LoBytes, (IPAddress >> 16) & LoBytes, (IPAddress >> 24) & LoBytes);

	return 1;
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	We Are About to Start a Demo Playback...
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void StartDemoPlayback( MENUITEM * Item )
{
	char buf[256];
	int i;
	uint32 mp_version;
	uint32 flags;
	uint32 PackedInfo[ MAX_PICKUPFLAGS ];

	TeamGame = FALSE;
	CountDownOn = FALSE;
	IsServer = FALSE;
	IsServerGame = FALSE;


	if( DemoList.item[0][0] == 0 )
	{
		// there are no demos...
		return;
	}
	LevelNum = -1;
	NewLevelNum = -1;

	for ( i = 0; i < MAX_PLAYERS; i++ )
		DemoShipInit[ i ] = FALSE;
	DemoShipInit[ MAX_PLAYERS ] = TRUE;
	memset (TeamNumber, 255, sizeof(BYTE) * MAX_PLAYERS);

	DemoFp = fopen( DemoFileName( DemoList.item[DemoList.selected_item] ) , "rb" );

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
	TeamGame = ( flags & TeamGameBit ) ? TRUE : FALSE;
	BombTag = ( flags & BombGameBit ) ? TRUE : FALSE;
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
	MenuAbort();
	ReleaseView();
	DestroySound( DESTROYSOUND_All );	// ReleaseView will not do a DestroySound if MyGameStatus == STATUS_Title

	PlayDemo = TRUE;

	MyGameStatus = STATUS_ChangeLevelPostPlayingDemo;
	WhoIAm = MAX_PLAYERS;

//	RandomStartPosModify = 0;
	SetupDplayGame();

	ChangeLevel();
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Clean up A demo File...
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
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
	BombTag = ( flags & BombGameBit ) ? TRUE : FALSE;
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

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	We Are About to Start a Demo Playback In Attract Mode...
	Input		:	MENUITEM * Item
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

// demo playback as splash screen...
BOOL StartSplashDemo( char *demofile, char *levels )
{
	char buf[256];
	int i;
	char *old_levels_list;
	uint32 mp_version;
	uint32 flags;
	uint32 PackedInfo[ MAX_PICKUPFLAGS ];

	RestoreDemoSettings();

	TeamGame = FALSE;
	CountDownOn = FALSE;
	IsServer = FALSE;
	IsServerGame = FALSE;


	old_levels_list = CurrentLevelsList;

	if ( !InitLevels( levels ) )
	{
		//Msg( "No splash levels" );
		return FALSE;
	}

	LevelNum = -1;
	NewLevelNum = -1;

	CurrentLevelsList = old_levels_list;	// CurrentLevelsList was changed by InitLevels

	for ( i = 0; i < MAX_PLAYERS; i++ )
		DemoShipInit[ i ] = FALSE;
	DemoShipInit[ MAX_PLAYERS ] = TRUE;
	memset (TeamNumber, 255, sizeof(BYTE) * MAX_PLAYERS);
	
	// Choose the appropriate demo...
	DemoFp = fopen( demofile , "rb" );
	if ( !DemoFp )
	{
		return FALSE;
	}

//	setvbuf( DemoFp, NULL, _IOFBF , 32768 );		// size of stream buffer...
	setvbuf( DemoFp, NULL, _IONBF , 0 );		// size of stream buffer...

	fread( &mp_version, sizeof( mp_version ), 1, DemoFp );
	if ( (mp_version > MULTIPLAYER_VERSION) || (mp_version < DEMO_MULTIPLAYER_VERSION) )
	{
		return FALSE;
	}

	fread( &CopyOfSeed1, sizeof( CopyOfSeed1 ), 1, DemoFp );
	fread( &CopyOfSeed2, sizeof( CopyOfSeed2 ), 1, DemoFp );
	fread( &RandomPickups, sizeof( RandomPickups ), 1, DemoFp );
	fread( &PackedInfo[ 0 ], sizeof( PackedInfo ), 1, DemoFp );
	UnpackPickupInfo( &PackedInfo[ 0 ] );

	fread( &flags, sizeof( flags ), 1, DemoFp );
	TeamGame = ( flags & TeamGameBit ) ? TRUE : FALSE;
	BombTag = ( flags & BombGameBit ) ? TRUE : FALSE;
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
		return FALSE;
	}
	CurrentMenu = NULL;
	CurrentMenuItem = NULL;
	MenuStackLevel = 0;

	PlayDemo = TRUE;
 
	MyGameStatus = STATUS_ChangeLevelPostAttractMode;
	WhoIAm = MAX_PLAYERS;
	
//	RandomStartPosModify = 0;
	SetupDplayGame();
	ChangeLevel();

	PreAttractModePanel = Panel;
	Panel = FALSE;

	return TRUE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Starting a Single Player Game...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL StartASinglePlayerGame( MENUITEM * Item )
{
	int i;
	LONGLONG	TempTime;

	PlayDemo = FALSE;
	IsHost = TRUE;
	AutoDetail = TRUE;
	// reset all bollocks...
	BombTag = FALSE;
	TeamGame = FALSE;
	CaptureTheFlag = FALSE;
	CTF = FALSE;
	IsServer = FALSE;
	IsServerGame = FALSE;

	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	RandomStartPosModify = 0;

	SetBikeMods( (uint16) (SelectedBike+2) );

	SetupDplayGame();
	
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		GameStatus[i] = STATUS_Null;
	}
	
	WhoIAm = 0;								// I was the first to join...

	Ships[WhoIAm].dcoID = 0;
	Current_Camera_View = 0;				// set camera to that view
	Ships[WhoIAm].enable = 1;
	StatsStatus = 1;						// I started it so the Stats Are Valid...
	
	memset(&Names, 0, sizeof(SHORTNAMETYPE) );
    strncpy( (char*) &Names[WhoIAm][0] , &biker_name[0] , 7 );
	Names[WhoIAm][7] = 0;																
	Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	
	CountDownOn = FALSE;

	MyGameStatus = STATUS_StartingSinglePlayer;

	return TRUE;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Starting a Single Player Game...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL LoadASinglePlayerGame( MENUITEM * Item )
{
	int i;
	LONGLONG	TempTime;

	IsServer = FALSE;
	IsServerGame = FALSE;
	PlayDemo = FALSE;
	IsHost = TRUE;
	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	RandomStartPosModify = 0;
	SetBikeMods( (uint16) (SelectedBike+2) );
	SetupDplayGame();
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		GameStatus[i] = STATUS_Null;
	}
	WhoIAm = 0;								// I was the first to join...
	Ships[WhoIAm].dcoID = 0;
	Current_Camera_View = 0;				// set camera to that view
	Ships[WhoIAm].enable = 1;
	StatsStatus = 1;						// I started it so the Stats Are Valid...
	memset(&Names, 0, sizeof(SHORTNAMETYPE) );
    strncpy( (char*) &Names[WhoIAm][0] , &biker_name[0] , 7 );
	Names[WhoIAm][7] = 0;																
	Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	

	NewLevelNum = -1;
	PreInGameLoad( Item );

	if( NewLevelNum == -1 )
		return FALSE;

	CountDownOn = FALSE;

	MyGameStatus = STATUS_TitleLoadGameStartingSinglePlayer;

	return TRUE;
}


int16 InGameLoadGameLevelNum;

BOOL InGameLoadASinglePlayerGame( MENUITEM * Item )
{
	int16 OldNewLevelNum;

	IsServer = FALSE;
	IsServerGame = FALSE;
	OldNewLevelNum = NewLevelNum;
	WhoIAm = 0;								// I was the first to join...
	NewLevelNum = -1;
	PreInGameLoad( Item );

	if( NewLevelNum == -1 )
	{
		NewLevelNum = OldNewLevelNum;
		return FALSE;
	}
	InGameLoadGameLevelNum = NewLevelNum;
	NewLevelNum = OldNewLevelNum;

	CountDownOn = FALSE;
	MyGameStatus = STATUS_InGameLoadGameStartingSinglePlayer;
	return TRUE;
}




#define VERSION_INFO			(TEXT( "\\" ))

static LONGLONG FileGetVersion( char *fname )
{
	DWORD infosize, dummy;
	LPVOID info;
	VS_FIXEDFILEINFO *inf;
	int len;
	union {
		struct {
			DWORD revision;
			DWORD version;
		};
		LONGLONG version_revision;
	} fileversion = { 0, 0 };

	infosize = GetFileVersionInfoSize( fname, &dummy );
	if ( infosize )
	{
		info = malloc( infosize );
		if ( GetFileVersionInfo( fname, 0, infosize, info ) )
		{
			if ( VerQueryValue( info, VERSION_INFO, &inf, &len ) )
			{
				fileversion.version = inf->dwFileVersionMS;
				fileversion.revision = inf->dwFileVersionLS;
			}
			else
			{
				DebugPrintf( "VerQueryValue( %s ) failed\n", fname );
			}
		}
		else
		{
			DebugPrintf( "GetFileVersioInfo( %s ) failed\n", fname );
		}
		free ( info );
	}
	else
	{
		DebugPrintf( "GetFileVersioInfoSize( %s ) failed\n", fname );
	}

	return fileversion.version_revision;
}



//////////////////////////////////////////////////////////////////////
// GetDigit(), ConvertField(), and GUIDFromString() all lifted from
// c:\dxsdk\sdk\samples\duel\util.c
// with only minor modifications to GUIDFromString() to avoid WCHAR
//////////////////////////////////////////////////////////////////////

// convert a hex char to an int - used by str to guid conversion
// we wrote our own, since the ole one is slow, and requires ole32.dll
// we use ansi strings here, since guids won't get internationalized
static int GetDigit(LPSTR lpstr)
{
	char ch = *lpstr;
    
    if (ch >= '0' && ch <= '9')
        return(ch - '0');
    if (ch >= 'a' && ch <= 'f')
        return(ch - 'a' + 10);
    if (ch >= 'A' && ch <= 'F')
        return(ch - 'A' + 10);
    return(0);
}
// walk the string, writing pairs of bytes into the byte stream (guid)
// we need to write the bytes into the byte stream from right to left
// or left to right as indicated by fRightToLeft
static void ConvertField(LPBYTE lpByte,LPSTR * ppStr,int iFieldSize,BOOL fRightToLeft)
{
	int i;

	for (i=0;i<iFieldSize ;i++ )
	{
		// don't barf on the field separators
		if ('-' == **ppStr) (*ppStr)++; 
		if (fRightToLeft == TRUE)
		{
			// work from right to left within the byte stream
			*(lpByte + iFieldSize - (i+1)) = 16*GetDigit(*ppStr) + GetDigit((*ppStr)+1);
		} 
		else 
		{
			// work from  left to right within the byte stream
			*(lpByte + i) = 16*GetDigit(*ppStr) + GetDigit((*ppStr)+1);
		}
		*ppStr+=2; // get next two digit pair
	}
} // ConvertField


// convert the passed in string to a real GUID
// walk the guid, setting each byte in the guid to the two digit hex pair in the
// passed string
HRESULT GUIDFromString( char *lpStr, GUID * pGuid)
{
	BYTE * lpByte; // byte index into guid
	int iFieldSize; // size of current field we're converting
	// since its a guid, we can do a "brute force" conversion
	
	// make sure we have a {xxxx-...} type guid
	if ('{' !=  *lpStr) return E_FAIL;
	lpStr++;
	
	lpByte = (BYTE *)pGuid;
	// data 1
	iFieldSize = sizeof(unsigned long);
	ConvertField(lpByte,&lpStr,iFieldSize,TRUE);
	lpByte += iFieldSize;

	// data 2
	iFieldSize = sizeof(unsigned short);
	ConvertField(lpByte,&lpStr,iFieldSize,TRUE);
	lpByte += iFieldSize;

	// data 3
	iFieldSize = sizeof(unsigned short);
	ConvertField(lpByte,&lpStr,iFieldSize,TRUE);
	lpByte += iFieldSize;

	// data 4
	iFieldSize = 8*sizeof(unsigned char);
	ConvertField(lpByte,&lpStr,iFieldSize,FALSE);
	lpByte += iFieldSize;

	// make sure we ended in the right place
	if ('}' != *lpStr) 
	{
		memset(pGuid,0,sizeof(GUID));
		return E_FAIL;
	}

	return S_OK;
}// GUIDFromString


#define REGISTRY_DIRECTPLAY_SERVICEPROVIDERS_KEY	(TEXT("Software\\Microsoft\\DirectPlay\\Service Providers"))

#define	GUID_NAME				(TEXT( "Guid" ))
#define	PATH_NAME				(TEXT( "Path" ))

#define MAX_SERVICEPROVIDER_NAME	(256)
#define MAX_GUIDTEXT				(256)


static int DirectPlayOK( LPGUID lpServiceProvider_guid )
{
	HKEY DP_ServiceProviders, DP_ServiceProvider;
	int errs = 0;
	int service_providers;
	FILETIME filetime;
	static char sp_name[ MAX_SERVICEPROVIDER_NAME ];
	static char sp_fname[ MAX_SERVICEPROVIDER_NAME ];
	static char sp_guidtext[ MAX_GUIDTEXT ];
	DWORD sp_len;
	GUID sp_guid;
	DWORD len;
	int old = 0;
	union {
		uint16 ver[ 4 ];
		LONGLONG version;
	} current, required = { DPLAY_REVISION_LO, DPLAY_REVISION_HI, DPLAY_VERSION_LO, DPLAY_VERSION_HI }, comp;

	if (   !IsEqualGUID( &DPSPGUID_MODEM,	lpServiceProvider_guid )
		&& !IsEqualGUID( &DPSPGUID_SERIAL,	lpServiceProvider_guid )
		&& !IsEqualGUID( &DPSPGUID_TCPIP,	lpServiceProvider_guid )
		&& !IsEqualGUID( &DPSPGUID_IPX,		lpServiceProvider_guid ) )
	{
		// not a recognised standard DirectPlay serviceprovider -- have to assume it's OK...
		return 1;
	}

	if ( RegOpenKeyEx( REGISTRY_ROOT_KEY, REGISTRY_DIRECTPLAY_SERVICEPROVIDERS_KEY,
			0, KEY_ENUMERATE_SUB_KEYS, &DP_ServiceProviders )
		== ERROR_SUCCESS )
	{
		service_providers = 0;
		sp_len = sizeof( sp_name );
		while ( RegEnumKeyEx( DP_ServiceProviders, service_providers, sp_name, &sp_len, NULL, NULL, NULL, &filetime ) 
			== ERROR_SUCCESS )
		{
			if ( RegOpenKeyEx( DP_ServiceProviders, sp_name,
				0, KEY_QUERY_VALUE, &DP_ServiceProvider )
				== ERROR_SUCCESS )
			{
				len = sizeof( sp_guidtext );
				if ( RegQueryValueEx( DP_ServiceProvider, GUID_NAME,
					NULL, NULL, sp_guidtext, &len )
					== ERROR_SUCCESS )
				{
					if ( GUIDFromString( sp_guidtext, &sp_guid ) == S_OK )
					{
						if ( IsEqualGuid( &sp_guid, lpServiceProvider_guid ) )
						{
							len = sizeof( sp_fname );
							if ( RegQueryValueEx( DP_ServiceProvider, PATH_NAME,
								NULL, NULL, sp_fname, &len )
								== ERROR_SUCCESS )
							{
								if ( current.version = FileGetVersion( sp_fname ) )
								{
									comp.version = current.version - required.version;
									DebugPrintf( "%s (%s) version is %hd.%hd.%hd.%hd %s\n",
										sp_name, sp_fname,
										current.ver[ 3 ], current.ver[ 2 ], current.ver[ 1 ], current.ver[ 0 ],
										( comp.version >= 0 ) ? "OK" : "out of date" );
									if ( comp.version < 0 )
										old++;
								}
								else
								{
									DebugPrintf( "FileGetVersion( %s ) failed\n", sp_fname );
									errs++;
								}
							}
							else
							{
								DebugPrintf( "RegQueryValueEx( %s\\%s ) failed\n", sp_name, PATH_NAME );
								errs++;
							}
						}
					}
					else
					{
						DebugPrintf( "GUIDFromString( %s ) failed for %s\n", sp_guidtext, sp_name);
						errs++;
					}
				}
				else
				{
					DebugPrintf( "RegQueryValueEx( %s\\%s ) failed\n", sp_name, GUID_NAME );
					errs++;
				}
				RegCloseKey( DP_ServiceProvider );
			}
			else
			{
					DebugPrintf( "RegOpenKeyEx( %s ) failed\n", sp_name );
				errs++;
			}
			sp_len = sizeof( sp_name );
			service_providers++;
		}
		RegCloseKey( DP_ServiceProviders );
	}
	else
	{
		errs++;
	}

	return !old;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Hosting a Server session ...
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL StartAHostSessionServer( MENUITEM * Item )
{
	HRESULT hr;
	int i;
	DWORD		Temp;
	BYTE		msg;
	int			size;
	LONGLONG	TempTime;
	uint32		Seed;
	uint32		PackedInfo[ MAX_PICKUPFLAGS ];
	char		ServerName[256];
	char		*pCh;

	IsServer = TRUE;
	IsServerGame = TRUE;

	NoSFX = TRUE;
//	d3dappi.Driver[d3dappi.CurrDriver].bDoesTextures = FALSE;
	PolygonText = FALSE;
	ServerRendering = FALSE;
	
	PreferedMaxPlayers = MaxServerPlayersSlider.value;
	MaxPlayersSlider.value = MaxServerPlayersSlider.value;
	SetMultiplayerPrefs();
	SetServerPrefs();

	Seed = timeGetTime();
	Seed1 = (uint16) ( ( Seed >> 16 ) & 0xffff );
	Seed2 = (uint16) ( Seed & 0xffff );
	CopyOfSeed1 = Seed1;
	CopyOfSeed2 = Seed2;

	PlayDemo = FALSE;
	IsHost = TRUE;

	TeamGame = FALSE;
	BombTag = FALSE;
	CaptureTheFlag = FALSE;
	BountyHunt = FALSE;
	CTF = FALSE;

	if ( ServerChoosesGameType )
		SetUpGameType( GameType );

	SetBikeMods( 0 );

	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	RandomStartPosModify = (uint16) ( ( TempTime * 71.42857143 ) / Freq );

	d3dappi.lpDD->lpVtbl->FlipToGDISurface(d3dappi.lpDD);

	pCh = ServerName;

	if( IPAddressExists )
	{
		pCh += sprintf( pCh , " %s's Server At %s~" , &biker_name[0],&IPAddressText );
	}else{
		pCh += sprintf( pCh , " %s's Server~" , &biker_name[0]);
	}

	GetLevelName( pCh, MAX_LEVEL_NAME_LENGTH, LevelList.selected_item );

	// create session
//	if ((hr = DPlayCreateSessionServer( &MultiPlayerGameName.text[0])) != DP_OK)
	if ((hr = DPlayCreateSessionServer( &ServerName[0])) != DP_OK)
	{
		return FALSE;
	}

	// create player
	if ((hr = DPlayCreatePlayerServer(&dcoID, "Server", NULL, NULL, 0)) != DP_OK)
	{
		return FALSE;
	}
	sprintf( &biker_name[0] , "Server" );

	DPlayUpdateInterval	= 60.0F / PacketsSlider.value;
	OldPPSValue = PacketsSlider.value;
	
	SetupDplayGame();

	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		GameStatus[i] = STATUS_Null;
	}
	
	WhoIAm = 0;								// I was the first to join...
	Ships[WhoIAm].dcoID = dcoID;

	if ( !ServerChoosesGameType )
	{
		MenuChangeEx( &MENU_NEW_HostWaitingToStartServer );
	}else
	{
		OKToProcessKeys = TRUE;
 		DPlayGetSessionDesc();
		if ( !glpdpSD )
		{
			return FALSE;
		}

		glpdpSD->dwMaxPlayers = MaxServerPlayersSlider.value + 1;

		glpdpSD->dwUser3 &= ~ServerGameStateBits;	// mask out old server state
		glpdpSD->dwUser3 |=	SERVER_STATE_Joinable;

		DPlaySetSessionDesc( 0 );

		MenuAbort();
	}

	MyGameStatus = STATUS_StartingMultiplayer;
	
	Current_Camera_View = 0;				// set camera to that view
	Ships[WhoIAm].Pickups = 0;
	Ships[WhoIAm].RegenSlots = 0;
	Ships[WhoIAm].Mines = 0;
	Ships[WhoIAm].Triggers = 0;
	Ships[WhoIAm].TrigVars = 0;
	
	memset(&Names, 0, sizeof(SHORTNAMETYPE) );
    strncpy( (char*) &Names[WhoIAm][0] , &biker_name[0] , 7 );
	Names[WhoIAm][7] = 0;
	Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
	NewLevelNum = LevelList.selected_item;	// I Select Which Level We Start on...

	if( TimeLimit.value )
	{
		CountDownOn = TRUE;
	}
	else
	{
		CountDownOn = FALSE;
	}
	if( RecordDemo || RecordDemoToRam )
	{
		uint32 mp_version = MULTIPLAYER_VERSION;
		uint32 flags;
		time_t now_time;
		struct tm *now;

		RecordDemo = TRUE;
		time( &now_time );
		now = localtime( &now_time );
#ifndef HOST_CHOOSES_DEMO_NAME
		if ( now )
		{
			sprintf( DemoGameName.text, "%s's Demo %d.%02d %d-%d-%d",
				biker_name,
				now->tm_hour, now->tm_min,
				1 + now->tm_mon, now->tm_mday,
				1900 + now->tm_year );
		}
		else
		{
			sprintf( DemoGameName.text, "%s's Demo",
				biker_name );
		}
#endif
		DemoFp = fopen( DemoFileName( DemoGameName.text ) , "wb" );
		setvbuf( DemoFp, NULL, _IONBF , 0 );		// size of stream buffer...

		Demo_fwrite( &mp_version, sizeof( mp_version ), 1, DemoFp );
		flags = 0;
		if( TeamGame )
			flags |= TeamGameBit;
		if( BombTag )
			flags |= BombGameBit;
		if( CTF )
			flags |= CTFGameBit;
		if( CaptureTheFlag )
			flags |= FlagGameBit;
		if ( BountyHunt )
			flags |= BountyGameBit;

		Demo_fwrite( &CopyOfSeed1, sizeof( CopyOfSeed1 ), 1, DemoFp );
		Demo_fwrite( &CopyOfSeed2, sizeof( CopyOfSeed2 ), 1, DemoFp );
		Demo_fwrite( &RandomPickups, sizeof( RandomPickups ), 1, DemoFp );
		PackPickupInfo( &PackedInfo[ 0 ] );
		Demo_fwrite( &PackedInfo[ 0 ], sizeof( PackedInfo ), 1, DemoFp );

		Demo_fwrite( &flags, sizeof( flags ), 1, DemoFp );
		Demo_fwrite( &RandomStartPosModify, sizeof( RandomStartPosModify ), 1, DemoFp );
		for( i = 0 ; i < 256 ; i++ )
		{
			Demo_fwrite( &ShortLevelNames[NewLevelNum][i], sizeof(char), 1, DemoFp );
			if( ShortLevelNames[NewLevelNum][i] == 0 )
			{
				break;
			}
		}

		// Best way I can Think of to send the Host Name to the demo file...
		Temp = 1;
		TempTime = 1;
		Demo_fwrite( &TempTime, sizeof(LONGLONG), 1, DemoFp );
		size = sizeof( NAMEMSG );
		Demo_fwrite( &size, sizeof(int), 1, DemoFp );
		Demo_fwrite( &Temp, sizeof(DWORD), 1, DemoFp );
		msg = MSG_NAME;
		Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );
		msg = 0;
		Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );				// Whos Name it is..
		Demo_fwrite( &biker_name[0], 7, 1, DemoFp );
		msg = 0;
		Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );				// terminator for name..
	}
	
	BrightShips = MyBrightShips;

	tracker_addr = 0;
	DPStartThread();

	return TRUE;
}


