/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 257 $
 *
 * $Header: /PcProjectX/Include/Mydplay.h 257   22/10/98 17:33 Phillipd $
 *
 * $Log: /PcProjectX/Include/Mydplay.h $
 * 
 * 257   22/10/98 17:33 Phillipd
 * 
 * 256   22/10/98 15:21 Phillipd
 * 
 * 255   22/10/98 11:35 Phillipd
 * 
 * 254   21/10/98 17:27 Phillipd
 * 
 * 253   20/10/98 17:17 Phillipd
 * 
 * 252   17/09/98 15:27 Phillipd
 * 
 * 251   17/09/98 10:39 Phillipd
 * 
 * 250   17/09/98 9:17 Phillipd
 * 
 * 248   14/09/98 15:35 Philipy
 * added facility for server based collisions
 * 
 * 247   3/09/98 17:34 Philipy
 * fixed server lag tolerance bug
 * 
 * 246   3/09/98 9:31 Philipy
 * somw Gamespy fixes
 * added Session ( GUID ) and TCP command line flags
 * added TRACKER_NAME facility
 * 
 * 245   28/08/98 9:19 Collinsd
 * 
 * 244   27/08/98 17:26 Philipy
 * Pseudohost can select level, & migrates on quitting
 * players booted to titles due to not having new level are now informed
 * 
 * 243   26/08/98 17:06 Philipy
 * tracker now migrates along with host
 * 
 * 242   25/08/98 15:44 Phillipd
 * 
 * 241   18/08/98 18:14 Philipy
 * all pings in server game are now from server perspective
 * windowed mode re-enabled ( still needs some work )
 * 
 * 240   17/08/98 17:13 Philipy
 * added -ServerChoosesGame & ServerAutoStart command line options
 * 
 * 239   14/08/98 9:13 Phillipd
 * DirectX6 is in effect....
 * 
 * 238   5/08/98 11:04 Philipy
 * added AutoStart facility ( joins game based on GUID in registery )
 * upped patch version to 1.02
 * 
 * 237   3/08/98 14:10 Phillipd
 * 
 * 236   3/08/98 12:28 Philipy
 * upped max players to 32
 * 
 * 235   30/07/98 14:18 Phillipd
 * 
 * 234   28/07/98 10:39 Philipy
 * Max players now works properly for server games
 * 
 * 233   23/07/98 15:38 Philipy
 * server now resets if no packets recieved for 5 mins
 * 
 * 232   21/07/98 12:10 Collinsd
 * MAX_PLAYERS increased to 24 and version number incremented.
 * 
 * 231   7/16/98 10:17a Phillipd
 * 
 * 230   15/07/98 16:25 Philipy
 * now handles pseudohost quitting in titles & server quitting ( in
 * titiles or in game )
 * 
 * 229   7/15/98 9:53a Phillipd
 * 
 * 228   7/08/98 2:46p Phillipd
 * 
 * 227   8/07/98 9:32 Oliverc
 * Converted multiplayer bounty and flag games to server operation for
 * patch
 * 
 * 226   7/07/98 18:05 Philipy
 * added lobby autostart code ( when all players have recieved init msg )
 * added num primary weapons menu option ( propergated to other players &|
 * server )
 * extracted new title text for localisation
 * 
 * 225   7/07/98 14:14 Collinsd
 * Fixed missed serverkillpickup messages when dynamically joining.
 * 
 * 224   7/03/98 11:23a Phillipd
 * 
 * 223   6/30/98 3:12p Phillipd
 * 
 * 222   6/30/98 10:38a Phillipd
 * 
 * 221   6/30/98 10:26a Phillipd
 * 
 * 220   6/26/98 2:07p Phillipd
 * 
 * 219   6/26/98 10:52a Phillipd
 * 
 * 218   22/06/98 9:39 Collinsd
 * Added new message for ship exploding.
 * 
 * 217   6/18/98 12:06p Phillipd
 * 
 * 216   6/10/98 9:55a Phillipd
 * 
 * 215   9/06/98 11:23 Philipy
 * server now reset if pseudohost does not have any valid levels
 * fixed timed game bug for server based games
 * 
 * 214   8/06/98 15:46 Collinsd
 * Done more on Server mode and Fixed trojax ammo usage
 * 
 * 213   8/06/98 12:28 Philipy
 * server levels now sent across to pseudohost. Pseudohost can only pick
 * valid levels
 * 
 * 212   3/06/98 15:30 Philipy
 * added server in game options for rendering on/off and selecting players
 * pseudohost can now select if server does collisions. This gets
 * propagated in MSG_GameParams and MSG_Init
 * 
 * 211   6/03/98 2:29p Phillipd
 * 
 * 210   22/05/98 17:51 Philipy
 * more work on session info
 * 
 * 209   21/05/98 16:52 Collinsd
 * Pickups controlled by host ( Sort of ).
 * 
 * 208   21/05/98 10:08 Philipy
 * fix to MSG_INIT for server game
 * 
 * 207   21/05/98 9:20 Collinsd
 * 
 * 206   21/05/98 9:10 Collinsd
 * Pickup server stuff started
 * 
 * 205   20/05/98 9:39 Philipy
 * implemented front end server menus
 * removed ( invalid ) ping from sessions menu
 * changed EnumPlayers so that it solely uses MSG_NAME
 * 
 * 204   5/13/98 3:59p Phillipd
 * 
 * 203   11/05/98 15:15 Philipy
 * added session info stuff ( game type, ping etc )
 * 
 * 202   5/11/98 12:29p Phillipd
 * 
 * 201   5/11/98 10:16a Phillipd
 * 
 * 200   4/09/98 4:37p Phillipd
 * 
 * 199   9/04/98 16:11 Philipy
 * team scores now propagate
 * 
 * 198   5/04/98 17:10 Collinsd
 * Hacked demos to work.
 * 
 * 197   4/05/98 2:33p Phillipd
 * 
 * 196   4/04/98 10:19p Phillipd
 * 
 * 195   4/04/98 5:38p Phillipd
 * 
 * 194   4/04/98 1:06p Phillipd
 * 
 * 193   3/04/98 17:06 Collinsd
 * 
 * 192   3/04/98 16:37 Collinsd
 * Done titan shrapnel as one message,
 * 
 * 191   4/03/98 3:33p Phillipd
 * 
 * 190   2/04/98 21:07 Philipy
 * Added taunts ( single & multiplayer, plus enemy biker taunts )
 * added flygirl to front end.
 * sliders for biker, bike computer and taunt speech volume
 * added new sfx for title
 * 
 * 189   4/02/98 12:53p Phillipd
 * 
 * 188   3/28/98 2:06p Phillipd
 * 
 * 187   27/03/98 17:33 Collinsd
 * HarmTeamMates now sent accross.
 * 
 * 186   3/18/98 4:32p Phillipd
 * 
 * 185   3/18/98 3:32p Phillipd
 * 
 * 184   16/03/98 12:19 Collinsd
 * Random Pickups option added.  Sphere dislpay of trigger areas
 * 
 * 183   13/03/98 11:36 Collinsd
 * Increased Max players to 16
 * 
 * 182   10/03/98 15:55 Collinsd
 * Qued Mines now work.
 * 
 * 181   3/09/98 4:32p Phillipd
 * 
 * 180   7/03/98 19:37 Oliverc
 * Added BikeExhausts display option to multiplayer INITMSG
 * 
 * 179   7/03/98 19:10 Collinsd
 * Added new message for olly ( TeamGoals ) Also added pos and group to
 * explodesecondary.
 * 
 * 178   7/03/98 16:32 Oliverc
 * Fixed bug: CTF type now sent to joining players in INITMSG
 * 
 * 177   3/07/98 2:01p Phillipd
 * 
 * 176   3/06/98 4:28p Phillipd
 * 
 * 175   3/05/98 3:42p Phillipd
 * 
 * 174   3/05/98 12:47p Phillipd
 * 
 * 173   4/03/98 12:33 Oliverc
 * CTF mode fully enabled
 * 
 * 172   3/03/98 11:32a Phillipd
 * 
 * 171   3/03/98 10:33 Oliverc
 * Reworked SHIP flag bit fields
 * 
 * 170   2/03/98 21:14 Collinsd
 * No longer use multiple bit or secfire.
 * 
 * 169   3/02/98 5:17p Phillipd
 * 
 * 168   3/02/98 11:56a Phillipd
 * 
 * 167   2/28/98 11:26a Phillipd
 * 
 * 166   2/26/98 4:50p Phillipd
 * 
 * 165   26/02/98 9:30 Oliverc
 * Disabled Bombtag
 * Fixed pickup flags bug
 * Disabled logos for EXTERNAL_DEMOs
 * 
 * 164   2/25/98 3:18p Phillipd
 * 
 * 163   2/25/98 2:34p Phillipd
 * 
 * 162   2/25/98 10:51a Phillipd
 * 
 * 161   24/02/98 16:56 Oliverc
 * 1st attempt at bounty hunt multiplayer game
 * 
 * 160   21/02/98 16:25 Philipy
 * added text messages for capture flag
 * 
 * 159   20/02/98 19:41 Oliverc
 * 2nd prototype of capture the flag game
 * 
 * 158   10/02/98 17:02 Collinsd
 * Fixed shockwaves giving credit for death to you.
 * 
 * 157   10/02/98 12:13 Philipy
 * added collision type to MSG_INIT
 * 
 * 156   9/02/98 10:51 Collinsd
 * Changed version number of game
 * 
 * 155   2/02/98 4:11p Phillipd
 * 
 * 154   1/29/98 2:27p Phillipd
 * 
 * 153   1/27/98 10:09a Phillipd
 * 
 * 152   1/19/98 4:21p Phillipd
 * 
 * 151   15/01/98 12:51 Collinsd
 * Time now works when people join an existing timed game.  Crystals
 * reinitialised.  Damage Flash only in Normal_Mode.
 * 
 * 150   15/01/98 10:15 Collinsd
 * Hopefully fixed link list fuckup.  Pickups can only be generated once.
 * Started of fixing mine bug.  Updated MX, MXA and Spotfx formats.
 * 
 * 149   9/01/98 10:35 Oliverc
 * Re-enabled MULTIPLAYER_VERSION check when joining game
 * 
 * 148   12/20/97 5:03p Phillipd
 * 
 * 147   12/20/97 12:43p Phillipd
 * 
 * 146   12/17/97 5:19p Phillipd
 * 
 * 145   12/12/97 10:22a Phillipd
 * Packets per second now dynamic
 * 
 * 144   11/12/97 15:53 Collinsd
 * Fixed enemy mines that didn't hurt you.  Also fixed enemy homing
 * missile.
 * 
 * 143   4/12/97 15:12 Collinsd
 * Ships are now allocated models.
 * 
 * 142   12/03/97 4:37p Phillipd
 * 
 * 141   12/03/97 3:21p Phillipd
 * 
 * 140   11/21/97 10:57a Phillipd
 * 
 * 139   11/11/97 9:20 Collinsd
 * Added scattering of enemies.
 * 
 * 138   11/10/97 7:29p Phillipd
 * 
 * 137   4/11/97 16:26 Philipy
 * AVI now plays for stats screens
 * implemented scrolling messages (not enabled)
 * 
 * 136   10/24/97 5:54p Phillipd
 * 
 * 135   10/21/97 5:07p Phillipd
 * 
 * 134   10/14/97 4:49p Phillipd
 * 
 * 133   10/09/97 3:30p Phillipd
 * External Forces coming...
 * Shield and hull are now floats
 * 
 * 132   10/07/97 11:33a Phillipd
 * 
 * 131   10/01/97 3:53p Phillipd
 * 
 * 130   9/29/97 9:08a Phillipd
 * 
 * 129   24/09/97 16:55 Collinsd
 * Added new bgobject stuff.  Added bounding box calc and display code.
 * and changes bgobject state changes.
 * 
 * 128   14-09-97 6:57p Philipy
 * joining existing team game - you can now select which team to go into,
 * and view current scores
 * 
 * 127   27/08/97 15:38 Collinsd
 * Taken out old secondary weapon stuff
 * 
 * 126   27/08/97 15:02 Collinsd
 * 
 * 125   8/19/97 12:59p Phillipd
 * 
 * 124   15/08/97 16:13 Collinsd
 * Started bgobjects moving ships.  External Forces bug now fixed.
 * 
 * 123   25/07/97 12:11 Collinsd
 * Changed ships bike index, and changed skin for olly.
 * 
 * 122   21/07/97 15:40 Collinsd
 * Changed primary/Secondary bullets to work from enemies.
 * 
 * 121   6/30/97 10:28a Phillipd
 * 
 * 120   6/24/97 5:11p Phillipd
 * 
 * 119   6/10/97 9:01a Phillipd
 * 
 * 118   4/06/97 11:11 Collinsd
 * Message sending added for doors.
 * 
 * 117   6/03/97 10:47a Phillipd
 * 
 * 116   5/07/97 5:44p Phillipd
 * 
 * 115   4/29/97 5:15p Phillipd
 * changed the status update....
 * 
 * 114   4/08/97 10:41a Phillipd
 * 12 players added...
 * 
 * 113   7-04-97 3:34p Collinsd
 * Added supernashram powerup.
 * 
 * 112   3/19/97 3:34p Phillipd
 * Added Invulnerable flag and timer....gets sent across network...
 * 
 * 111   3/13/97 9:50a Phillipd
 * 
 * 110   3/11/97 5:41p Phillipd
 * 
 * 109   10-03-97 3:53p Collinsd
 * Added fire held bit.
 * 
 * 108   5-03-97 4:59p Collinsd
 * 
 * 107   2/25/97 12:32p Phillipd
 * rotation interpilation...
 * 
 * 106   2/24/97 10:00a Phillipd
 * Demo mode with multispeed has been added..
 * 
 * 105   12-02-97 12:03p Collinsd
 * Added error checking to readfiles();  Also added triggers to enemies.
 * 
 * 104   2/12/97 9:31a Phillipd
 * 
 * 103   11-02-97 5:08p Collinsd
 * Triggers/RegenPoints and pickups now are sent across correctly.
 * 
 * 102   2/04/97 5:11p Phillipd
 * new player joining brute force reject..
 * 
 * 101   1/13/97 5:40p Phillipd
 * added message macors to multiplayer....
 * 
 * 100   3-01-97 3:28p Collinsd
 * Added xxx quantum/pine/purged mined xxx messages.
 * Trojax charging sfx on others pc's now fixed.
 * 
 * 99    31-12-96 12:35p Collinsd
 * Added multiple multiples.
 * 
 * 98    30-12-96 3:50p Collinsd
 * Added orbit pulsar.
 * 
 * 97    12/27/96 12:33p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 96    12/23/96 8:50a Phillipd
 * 
 * 95    19-12-96 4:17p Collinsd
 * Added lighting when charging thr trojax.
 * 
 * 94    12/19/96 12:02p Phillipd
 * got rid of dpid from short ship packet
 * added location names to team members...
 * 
 * 93    12/17/96 9:20a Phillipd
 * 
 * 92    12/16/96 10:07a Collinsd
 * Full shield and full now transfered in update.
 * 
 * 91    12/07/96 8:42p Collinsd
 * Added Jap Bird bike, Fixed mines being dropped and firing missile at
 * same time bug.  Added rotations when hit ( depending on damage ).
 * 
 * 90    12/04/96 2:22p Collinsd
 * Pine Mine missile now works.
 * 
 * 89    29/11/96 14:20 Oliverc
 * Added ship rotation force functions
 * 
 * 88    11/28/96 3:31p Phillipd
 * Better Multiplayer Starting...
 * 
 * 87    11/28/96 11:19a Collinsd
 * Added shield and hull sending across network
 * 
 * 86    11/26/96 12:35p Collinsd
 * Should hopefully fix problem.
 * 
 * 85    11/22/96 3:01p Phillipd
 * Fire packet cut right down
 * 
 * 84    11/20/96 2:42p Phillipd
 * players can now restart and keep there score...
 * 
 * 83    18/11/96 11:39 Collinsd
 * Added scattered weapons dissapearing and regenerating elsewhere after
 * 5-10 seconds.
 * 
 * 82    15/11/96 16:49 Collinsd
 * Mines now sent across when joining. Also fixed bug in pickup sending.
 * 
 * 81    7/11/96 11:11 Collinsd
 * Got rid of quat in ship structures.
 * 
 * 80    7/11/96 10:47 Collinsd
 * Shortened short ships structures.
 * 
 * 79    7/11/96 9:03 Collinsd
 * Changed over to using new network firing.
 * 
 * 78    1/11/96 17:52 Collinsd
 * Reduced ship/shortship structure size by using bits.
 * 
 * 77    31/10/96 16:30 Collinsd
 * Primary/Secondary now included ship updates.  Stealth regeneration
 * fixed
 * 
 * 76    30/10/96 16:21 Collinsd
 * stealth sfx and regeneration
 * 
 * 75    30/10/96 14:34 Collinsd
 * Added stealthmode.
 * 
 * 74    26/10/96 18:04 Collinsd
 * Added recoil vector to ihityou.
 * 
 * 73    10/25/96 10:15a Phillipd
 * 
 * 72    10/24/96 3:01p Phillipd
 * 
 * 71    10/15/96 10:42a Phillipd
 * Added Spot Lights....
 * 
 * 70    10/14/96 12:12p Phillipd
 * 
 * 69    10/11/96 5:30p Phillipd
 * New low Player...
 * detsroy message now looks for a clean break rather than
 * treating a crash leaving the same as a quit game..
 * 
 * 68    10/11/96 11:40a Phillipd
 * Now player names are gotten rather than given..
 * 
 * 67    10/10/96 3:10p Phillipd
 * 
 * 66    9/10/96 16:12 Collinsd
 * Added regeneration sparkle. And model display flag.
 * 
 * 65    8/10/96 20:26 Collinsd
 * fixed pickup transfer for new player.
 * 
 * 64    8/10/96 14:34 Oliverc
 * New bob mechanism implemented (with anti-drift compensation...)
 * 
 * 63    5/10/96 13:22 Collinsd
 * Fixed bug that caused pickups to do collision processing
 * even when stationary for the first few seconds after initialisation.
 * 
 * 62    10/04/96 3:35p Phillipd
 * ships now dont slip through the floor if no packet gets through
 * 
 * 61    4/10/96 10:23 Oliverc
 * Added LastMove vector to ship
 * 
 * 60    10/04/96 10:16a Phillipd
 * 
 * 59    29/09/96 18:47 Collinsd
 * Added turbo to ship structure. Fixed pine mine firing pos.
 * 
 * 58    9/18/96 3:59p Phillipd
 * 
 * 57    9/17/96 4:18p Phillipd
 * Limbo Mode added.....Dont do anything to the ship...
 * 
 * 56    9/17/96 3:37p Phillipd
 * 
 * 55    17/09/96 15:34 Collinsd
 * Shockwaves added. and fixed primary previous.
 * 
 * 54    16/09/96 15:20 Oliverc
 * Changed autolevelling to be properly framelagged
 * and added ship bobbing
 * 
 * 53    9/16/96 11:51a Phillipd
 * 
 * 52    16/09/96 11:24 Collinsd
 * Added joining regeneration of pickups code.
 * 
 * 51    9/16/96 10:16a Phillipd
 * Big changes to Full screen display...
 * And Pickups being set to joining player
 * 
 * 50    14/09/96 21:27 Oliverc
 * Added basic ship auto-levelling (incl config file setting) but not made
 * it frame lagged properly
 * 
 * 49    12/09/96 17:52 Collinsd
 * You can now shoot mines ( Primary weapons only though
 * at the moment ).
 * 
 * 48    10/09/96 16:33 Oliverc
 * Added external/internal force vector to ship movement
 * 
 * 47    10/09/96 12:52 Collinsd
 * Pickups now save and load groups
 * 
 * 46    9/10/96 9:56a Phillipd
 * 
 * 45    9/09/96 20:42 Collinsd
 * No longer used GroupPolyCol or WhichGroupImIn.
 * 
 * 44    7/09/96 20:21 Collinsd
 * Collision with pine mine now added.  Trojax power
 * fixed, now does proper damage.  Transpulse speeded
 * up and made more lethal.
 * 
 * 43    9/06/96 12:56p Phillipd
 * 
 * 42    5/09/96 15:08 Collinsd
 * Added timer missile.
 * 
 * 41    9/05/96 1:59p Phillipd
 * 
 * 40    8/28/96 11:13a Phillipd
 * Game now works under a GameStatus flag...
 * So you can view scores load a new level and restart..
 * 
 * 39    8/23/96 2:41p Phillipd
 * 
 * 38    8/23/96 11:10a Phillipd
 * 
 * 37    8/22/96 5:13p Phillipd
 * The Stats Are coming
 * 
 * 36    16/08/96 17:29 Collinsd
 * Damage variables now in..
 * 
 * 35    8/15/96 5:18p Phillipd
 * 
 * 34    8/14/96 5:48p Phillipd
 * 
 * 33    8/14/96 4:30p Phillipd
 * 
 * 32    14/08/96 10:49 Collinsd
 * 
 * 31    8/13/96 10:15a Phillipd
 * Player name and score now given to everyone for printing....4x5 print
 * routine and uint16 printing
 * 
 * 29    8/11/96 5:35p Phillipd
 * 
 * 28    9/08/96 10:18 Collinsd
 * Added powerlevel to primary weapons.
 * Added a few more pickup models.
 * 
 * 27    8/08/96 8:48a Phillipd
 * 
 * 26    8/07/96 10:49a Phillipd
 * 
 * 25    5/08/96 20:04 Collinsd
 * Network for pickups now works,
 * 
 * 24    8/02/96 9:31a Phillipd
 * 
 * 23    7/31/96 5:14p Phillipd
 * 
 * 22    31/07/96 16:07 Collinsd
 * Added new pyrolite weapon, added type into bullets/missiles. Fixed bug
 * with 2d polys
 * 
 * 21    7/29/96 4:42p Phillipd
 * 
 * 20    7/26/96 4:27p Phillipd
 * 
 * 19    7/26/96 2:41p Phillipd
 * 
 * 18    7/23/96 9:41a Phillipd
 * 
 * 17    21/07/96 14:21 Collinsd
 * Added drop vector to dplay and secondary weapons.
 * 
 * 16    7/16/96 9:36a Phillipd
 * 
 * 15    7/15/96 10:36a Phillipd
 * 
 * 14    7/12/96 12:29p Phillipd
 * 
 * 13    10/07/96 17:29 Collinsd
 * 
 * 12    7/08/96 10:12a Phillipd
 * 
 * 11    7/08/96 9:43a Phillipd
 * 
 * 10    7/06/96 2:58p Phillipd
 * Ships now use Quats...
 * 
 * 9     7/05/96 3:21p Phillipd
 * 
 * 8     7/04/96 11:15a Phillipd
 * 
 * 7     7/04/96 9:49a Phillipd
 * 
 * 6     7/02/96 11:05a Phillipd
 * 
 * 5     26/06/96 11:03 Collinsd
 * 
 * 4     25/06/96 15:15 Collinsd
 * 
 * 3     6/25/96 3:00p Phillipd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*==========================================================================
 *
 *  Copyright (C) 1995 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       mydplay.h
 *  Content:    main include file
 *
 *
 ***************************************************************************/

#ifndef MYDPLAY_INCLUDED
#define MYDPLAY_INCLUDED

#define IDIRECTPLAY2_OR_GREATER
#define WIN32_EXTRA_LEAN

 
 /*
 * defines
 */
#define FIND_SESSION_TIMEOUT	3600.0F

#define MULTIPLAYER_VERSION		(0x010d)
#define DEMO_MULTIPLAYER_VERSION (0x0109)

#define	GUARANTEEDMESSAGES

#define MAXPACKETSPERBIGPACKET 64
#define MAXBIGPACKETBUFFERSIZE 1024
#define MAXPACKETSPERSERVERPACKET 64


#define	SHORTBANK
#define	VERYSHORTPACKETS

#define	NOISEFALLOFFFACTOR ( 1.0F / 120.0F )	// approx 2 seconds....

#define NORMAL_MODE 0
#define DEATH_MODE 1
#define LIMBO_MODE 2		// this is what the ship is in while viewing its own death....but has blown up...
#define DEMO_MODE  3		// this is the mode the camera goes into when Playing back a Demo
#define GAMEOVER_MODE  4	// this is the mode the player goes into when single player has finished...


#define MAX_PLAYERS			32 //24 //16 //12
BOOL PlayerReady[MAX_PLAYERS];
#define MAX_SHIELD			255.0F
#define START_SHIELD		128.0F
#define MAX_HULL			255.0F
#define START_HULL			128.0F

#define	MAXTEXTMSG			128		// the number of chars in a message you can send.....

#define	MAXGENPICKUPCOUNT	5
#define	MAXGENREGENSLOTCOUNT 6
#define	MAXGENMINECOUNT		3 //4
#define	MAXGENTRIGGERCOUNT	60
#define	MAXGENTRIGVARCOUNT	60

#define	MAXMULTIPLES		8

#define MAX_PICKUPFLAGS		2

#define MAX_BUFFER_SIZE    1024

#define	FRAMELAGED_RECOIL	TRUE
#define	ONEOFF_RECOIL		FALSE

#define MAX_TEAMS 4

#define	NUMTITANBITS		10 

/*
 * structures
 */
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Bodge Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
typedef struct SHORTMINE{

	uint16	OwnerType;
	uint16	Owner;
	uint16	BulletID;
	uint16	Group;
	VECTOR	Pos;
	VECTOR	Offset;
	VECTOR	Dir;
	VECTOR	UpVector;
	VECTOR	DropDir;
	uint16	Weapon;
	float	LifeCount;

}SHORTMINE;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Bodge Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
typedef struct SHORTKILLMINE{
	uint16	OwnerType;			// Owner ID
	uint16	Owner;				// Owner ID
	uint16	ID;					// ID
	float	ShockwaveSize;		// Size
}SHORTKILLMINE;
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Bodge Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

typedef struct MISSEDMINEMSG{
 	int16	Type;
	void *	Struct;
}MISSEDMINEMSG;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Bodge Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
typedef struct SHORTPICKUP{
 	int16	Type;
	int16	Group;
	uint16	Owner;				// Owner ID
	uint16	ID;					// ID
	VECTOR	Pos;
	VECTOR	Dir;
	int16	RegenSlot;
	float	Speed;
	BOOL	Sparkle;
	float	LifeCount;
	uint16	TriggerMod;
}SHORTPICKUP;

typedef struct SHORTTRIGVAR{
	int		State;
}SHORTTRIGVAR;

typedef struct SHORTTRIGGER{
	int		Active;
}SHORTTRIGGER;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Bodge Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
typedef struct SHORTREGENSLOT{

	uint16	GenType;			// Generation Type
	uint16	RegenType;			// Regeneration Type
	float	GenDelay;			// Generation Delay ( 60hz )
	float	Lifespan;			// Lifespan of pickup ( 60hz )
	float	Wait;				// Global count
	int16	Status;				// Status of slot
	int16	Group;				// Group to generate pickup
	VECTOR	Pos;				// Position to generate pickup
	uint16	Type;				// Type of pickup
	uint16	TriggerMod;			// Trigger Mod Index
	uint16	PickupIndex;		// Pickup Index
	uint16	PickupID;			// Pickup ID

}SHORTREGENSLOT;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Bodge Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
typedef struct SHORTKILLPICKUP{
	uint16	Owner;				// Owner ID
	uint16	ID;					// ID
	int16	Style;				// Style
}SHORTKILLPICKUP;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Bodge Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
typedef struct SHORTSERVERKILLPICKUP{
	uint16	Owner;				// Owner ID
	uint16	ID;					// ID
	int16	Style;				// Style
	uint16	NewOwner;

}SHORTSERVERKILLPICKUP;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Bodge Structures
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
typedef struct MISSEDPICKUPMSG{
 	int16	Type;
	void *	Struct;
}MISSEDPICKUPMSG;


typedef	char		SHORTNAMETYPE[MAX_PLAYERS][8];

typedef struct _SHIPDIEDINFO
{
	BYTE		WeaponType;
	BYTE		Weapon;

} SHIPDIEDINFO;

typedef struct _PICKUPINFO
{
	uint16		IDCount;
	int16		Type;
	uint16		Group;
	VECTOR		Pos;
	VECTOR		Dir;
	float		Speed;
	int16		RegenSlot;
	BOOL		Sparkle;
	float		LifeCount;
	uint16		TriggerMod;
}PICKUPINFO; // 46

typedef struct _VERYSHORTPICKUPINFO
{
	uint16		IDCount;
	BYTE		Type;
	BYTE		Group;
	BYTE		RegenSlot;
	BYTE		Sparkle;
	SHORTVECTOR	Pos;
	SHORTVECTOR	Dir;
	float		Speed;
	float		LifeCount;
	uint16		TriggerMod;
}VERYSHORTPICKUPINFO; // 28



typedef struct _KILLPICKUPINFO
{
	uint16		Owner;
	uint16		IDCount;
	int16		Style;
}KILLPICKUPINFO;

typedef struct _SERVERKILLPICKUPINFO
{
	uint16		Owner;
	uint16		IDCount;
	int16		Style;
	uint16		NewOwner;
}SERVERKILLPICKUPINFO;

typedef struct _EXPLODESHIPINFO
{
	uint16		Ship;
}EXPLODESHIPINFO;

typedef struct _EXPSECONDARYINFO
{
	uint16		OwnerType;
	uint16		Owner;
	uint16		IDCount;
	float		ShockwaveSize;
	VECTOR		Pos;
	uint16		Group;

}EXPSECONDARYINFO;

typedef struct _TEAMGOALSINFO
{
	uint16		TeamGoals[ MAX_TEAMS ];

}TEAMGOALSINFO;

typedef struct _SHOCKWAVEINFO
{
	uint16		Owner;
	uint16		Group;
	VECTOR		Pos;
	BYTE		Weapon;
	float		ShockwaveSize;
}SHOCKWAVEINFO;

typedef struct _BGOUPDATEINFO
{
	uint16		BGObject;
	int16		State;
	float		Time;
}BGOUPDATEINFO;

typedef struct _PRIMBULLPOSDIR
{
	uint16		OwnerType;
	uint16		OwnerID;
	uint16		BulletID;
	int8		Weapon;
	uint16		Group;
	VECTOR		Pos;
	VECTOR		Offset;
	VECTOR		Dir;
	VECTOR		Up;
	int16		PowerLevel;
	float		PLevel;

}PRIMBULLPOSDIR;

typedef struct _SECBULLPOSDIR
{
	uint16		OwnerType;
	uint16		Owner;
	uint16		BulletID;
	uint16		Group;
	VECTOR		Pos;
	VECTOR		Offset;
	VECTOR		Dir;
	VECTOR		UpVector;
	VECTOR		DropDir;
	int8		Weapon;

}SECBULLPOSDIR;

typedef struct _TITANBITS
{
	uint16		OwnerType;
	uint16		Owner;
	uint16		BulletID;
	uint16		Group;
	VECTOR		Pos;
	VECTOR		Offset;
	VECTOR		UpVector;
	VECTOR		DropDir;
	VECTOR		Directions[ NUMTITANBITS ];
	int8		Weapon;

}TITANBITS;

typedef struct _SHIPHIT
{
	float		Damage;
	VECTOR		Recoil;
	VECTOR		Point;
	VECTOR		Dir;
	BYTE		WeaponType;
	BYTE		Weapon;
	float		Force;
	BOOL		OneOffExternalForce;

}SHIPHIT;


typedef struct _SHORTSHIPHIT
{
	float		Damage;
	float		Force;
	SHORTVECTOR	Recoil;
	SHORTVECTOR	Point;
	SHORTVECTOR	Dir;
	BYTE		WeaponType;
	BYTE		Weapon;
	uint16		OneOffExternalForce;
	uint16		Recoil_Scalar;
}SHORTSHIPHIT;



typedef struct _SETTIME
{
	float		Time;

}SETTIME;

typedef struct _GLOBALSHIP
{
	OBJECT				Object;		//
	
	BYTE				enable;     // is this ship active?
	BYTE				ShipThatLastKilledMe;		// the ship who killed me last...
	BYTE				ShipThatLastHitMe;		// the ship who killed me last...
	BYTE				NumMultiples;	// Number of multiples
	float				StealthTime; // Stealth Mode Time Limit
	float				Timer;		// General Purpose Mode Timer....How Long to stay dead ...etc..
	float				InvulTimer;	// HowLong am I Invulnerable
	BOOL				Invul;		// Am I Invulnerable...
	VECTOR				LastAngle;	// what my last Step Turn Angles Were...
	int16				Kills;		// number of kills
	int16				Deaths;		// number of deaths
	uint16				PrimBullIdCount;// Id count for every bullet I fire....
	uint16				SecBullIdCount;// Id count for every bullet I fire....
	uint16				PickupIdCount;// Id count for every pickup I generate.....
	float				Damage;		// how much Damege I Have Taken...
	BYTE				Primary;	// which weapon is my primary....
	BYTE				Secondary;	// which wepaon is my secondary....
	uint16				ModelNum;	// which model to display for them...
	int16				BikeNum;	// which model to display for them...
	BYTE				Pickups;
	BYTE				RegenSlots;
	BYTE				Triggers;
	BYTE				TrigVars;
	BYTE				Mines;
	BOOL				JustRecievedPacket;//
	VECTOR				LastMove;	// last movement vector (framelagged)
	VECTOR				Move_Off;	// Last MoveMent...x , y , z
	DPID                dcoID;
	uint16				OrbModels[ MAXMULTIPLES ];	// Orbit Pulsar Model
	float				OrbAmmo[ MAXMULTIPLES ];	// Orbit Pulsar Ammo
	int8				LastPacketID;
	BOOL				FirstPacketRecieved;

	float				PrimPowerLevel; // Trojax Powerlevel
	uint16				PrimID;		// Bullet ID
	uint16				SecID;		// Missile ID
	BYTE				SecWeapon;	// Weapon


	BOOL				DemoInterpolate;

	VECTOR		OldPos;				//
	VECTOR		NextPos;			// what my next position will be
	QUAT		OldQuat;			//
	QUAT		NextQuat;			// what my next view angle will be
	float		OldBank;			//
	float		NextBank;			// what my next bank angle will be..
	LONGLONG	OldTime;			// how long before I am at those positions
	LONGLONG	NextTime;			// how long before I am at those positions

	float		SuperNashramTimer;	// HowLong have i left with the super nashram?
	uint16		TempLines[ 12 ];

	float		ShakeTimer;
	float		ShakeDirTimer;
	float		ShakeForce;

	float		PacketDelay;

	uint8		ShieldHullCount;

	VECTOR		RealPos;
	uint16		RealGroup;

} GLOBALSHIP, *LPGLOBALSHIP;



/*		this is all that really needs to be sent	*/
typedef struct _SHORTGLOBALSHIP
{
	uint32				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Status;			// tells us constantly what this ship is doing...
	BYTE				GroupImIn;		// Group Im In

	VECTOR				Pos;			// x , y , z position
	VECTOR				Move_Off;		// Last MoveMent...x , y , z
	VECTOR				Angle;			// Last Rotation..
#ifdef	SHORTBANK
	int16				Bank;		// How much Am I banked....
#else
	float				Bank;		// How much Am I banked....
#endif
	QUAT				Quat;		// Final Quat...
//	int8				LastPacketID;	// should go in order.....
} SHORTGLOBALSHIP, *LPSHORTGLOBALSHIP;

typedef struct _VERYSHORTGLOBALSHIP
{
	uint32				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Status;			// tells us constantly what this ship is doing...
	BYTE				GroupImIn;		// Group Im In
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTVECTOR			Move_Off;		// Last MoveMent...x , y , z
	SHORTVECTOR			Angle;			// Last Rotation..
	int16				Bank;		// How much Am I banked....
	uint16				Move_Off_Scalar;
	SHORTQUAT			Quat;		// Final Quat...
} VERYSHORTGLOBALSHIP, *LPVERYSHORTGLOBALSHIP;


typedef struct _FSHORTGLOBALSHIP
{
	uint32				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Primary;		// Primary weapon
	BYTE				Secondary;		// Secondary weapon
	BYTE				GroupImIn;		// Group Im In
	BYTE				PrimPowerLevel;	// Trojax Powerlevel
	VECTOR				Pos;			// x , y , z position
	VECTOR				Move_Off;		// Last MoveMent...x , y , z
	VECTOR				Angle;			// Last Rotation..
#ifdef	SHORTBANK
	int16				Bank;		// How much Am I banked....
#else
	float				Bank;		// How much Am I banked....
#endif
	QUAT				Quat;		// Final Quat...
} FSHORTGLOBALSHIP, *LPFSHORTGLOBALSHIP;

typedef struct _FVERYSHORTGLOBALSHIP
{
	uint32				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Primary;		// Primary weapon
	BYTE				Secondary;		// Secondary weapon
	BYTE				GroupImIn;		// Group Im In
	BYTE				PrimPowerLevel;	// Trojax Powerlevel
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTVECTOR			Move_Off;		// Last MoveMent...x , y , z
	SHORTVECTOR			Angle;			// Last Rotation..
	int16				Bank;		// How much Am I banked....
	uint16				Move_Off_Scalar;
	SHORTQUAT			Quat;		// Final Quat...
} FVERYSHORTGLOBALSHIP, *LPFVERYSHORTGLOBALSHIP;

typedef struct _GROUPONLY_FVERYSHORTGLOBALSHIP
{
	uint32				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Primary;		// Primary weapon
	BYTE				Secondary;		// Secondary weapon
	BYTE				GroupImIn;		// Group Im In
	BYTE				PrimPowerLevel;	// Trojax Powerlevel
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTQUAT			Quat;		// Final Quat...
	int16				Bank;		// How much Am I banked....
} GROUPONLY_FVERYSHORTGLOBALSHIP, *LPGROUPONLY_FVERYSHORTGLOBALSHIP;


//----------------------------------------------------------
// Mask Defines for .Flags uint32
//----------------------------------------------------------

#define	SHIP_Light_Bit			30
#define	SHIP_CarryingFlag4_Bit	29
#define	SHIP_CarryingFlag3_Bit	28
#define	SHIP_CarryingFlag2_Bit	27
#define	SHIP_CarryingFlag1_Bit	26
#define	SHIP_CarryingFlag_Bit	26
#define	SHIP_CarryingBounty_Bit	26
#define	SHIP_Shield_Bit			25
#define	SHIP_Scattered_Bit		24
#define	SHIP_Litup_Bit			23
#define	SHIP_InWater_Bit		22
#define	SHIP_SuperNashram_Bit	21
#define	SHIP_Invul_Bit			20
#define	SHIP_MulFire_Bit		19
#define	SHIP_Charging_Bit		18
#define	SHIP_Turbo_Bit			17
#define	SHIP_Stealth_Bit		16
#define	SHIP_PrimFire_Bit		15
#define	SHIP_PrimToggle_Bit		14
#define	SHIP_SecFire_Bit		13
#define	SHIP_SecToggle_Bit		12
#define	SHIP_IsHost_Bit			11
#define	SHIP_Enable_Bit			10
#define	SHIP_Hull_Bit2			9
#define	SHIP_Hull_Bit1			8
#define	SHIP_Mode_Bit2			7
#define	SHIP_Mode_Bit1			6
#define	SHIP_PowerLevel_Bit2	5
#define	SHIP_PowerLevel_Bit1	4
#define	SHIP_NumMultiples_Bit4	3
#define	SHIP_NumMultiples_Bit3	2
#define	SHIP_NumMultiples_Bit2	1
#define	SHIP_NumMultiples_Bit1	0

#define	SHIP_Light			( 1 << SHIP_Light_Bit )
#define	SHIP_CarryingFlag4	( 1 << SHIP_CarryingFlag4_Bit )
#define	SHIP_CarryingFlag3	( 1 << SHIP_CarryingFlag3_Bit )
#define	SHIP_CarryingFlag2	( 1 << SHIP_CarryingFlag2_Bit )
#define	SHIP_CarryingFlag1	( 1 << SHIP_CarryingFlag1_Bit )
#define	SHIP_CarryingFlag	( 1 << SHIP_CarryingFlag_Bit )
#define	SHIP_CarryingFlags	( SHIP_CarryingFlag1 \
							| SHIP_CarryingFlag2 \
							| SHIP_CarryingFlag3 \
							| SHIP_CarryingFlag4 )
#define	SHIP_NumMultiplesBit4	( 1 << SHIP_NumMultiples_Bit4 )
#define	SHIP_NumMultiplesBit3	( 1 << SHIP_NumMultiples_Bit3 )
#define	SHIP_NumMultiplesBit2	( 1 << SHIP_NumMultiples_Bit2 )
#define	SHIP_NumMultiplesBit1	( 1 << SHIP_NumMultiples_Bit1 )
#define	SHIP_Shield			( 1 << SHIP_Shield_Bit )
#define	SHIP_HullBit2		( 1 << SHIP_Hull_Bit2 )
#define	SHIP_HullBit1		( 1 << SHIP_Hull_Bit1 )
#define	SHIP_ModeBit2		( 1 << SHIP_Mode_Bit2 )
#define	SHIP_ModeBit1		( 1 << SHIP_Mode_Bit1 )
#define	SHIP_PowerLevelBit2	( 1 << SHIP_PowerLevel_Bit2 )
#define	SHIP_PowerLevelBit1	( 1 << SHIP_PowerLevel_Bit1 )
#define	SHIP_CarryingBounty	( 1 << SHIP_CarryingBounty_Bit )
#define	SHIP_Scattered		( 1 << SHIP_Scattered_Bit)
#define	SHIP_Litup			( 1 << SHIP_Litup_Bit )
#define	SHIP_InWater		( 1 << SHIP_InWater_Bit )
#define	SHIP_SuperNashram	( 1 << SHIP_SuperNashram_Bit )
#define	SHIP_Invul			( 1 << SHIP_Invul_Bit )
#define	SHIP_MulFire		( 1 << SHIP_MulFire_Bit )
#define	SHIP_Multiple		( ( 1 << SHIP_NumMultiples_Bit4 ) \
							| ( 1 << SHIP_NumMultiples_Bit3 ) \
							| ( 1 << SHIP_NumMultiples_Bit2 ) \
							| ( 1 << SHIP_NumMultiples_Bit1 ) )
#define	SHIP_Charging		( 1 << SHIP_Charging_Bit )
#define	SHIP_Turbo			( 1 << SHIP_Turbo_Bit )
#define	SHIP_Stealth		( 1 << SHIP_Stealth_Bit )
#define	SHIP_PrimFire		( 1 << SHIP_PrimFire_Bit )
#define	SHIP_PrimToggle		( 1 << SHIP_PrimToggle_Bit )
#define	SHIP_SecFire		( 1 << SHIP_SecFire_Bit )
#define	SHIP_SecToggle		( 1 << SHIP_SecToggle_Bit )
#define	SHIP_IsHost			( 1 << SHIP_IsHost_Bit )
#define	SHIP_Enable			( 1 << SHIP_Enable_Bit )

//----------------------------------------------------------
// communication packet structures
//----------------------------------------------------------
#define MSG_UPDATE					0x11
#define MSG_HEREIAM					0x22
#define MSG_INIT					0x33
#define MSG_SHIPHIT					0x44
#define MSG_PRIMBULLPOSDIR			0x55
#define MSG_SECBULLPOSDIR			0x66
#define MSG_SHIPDIED				0x77
#define MSG_DROPPICKUP				0x88
#define MSG_KILLPICKUP				0x99
#define MSG_SHORTSTATS				0xaa
#define MSG_STATUS					0xbb
#define MSG_EXPSECONDARY			0xcc
#define MSG_SHORTPICKUP				0xee
#define MSG_SHOCKWAVE				0xff
#define MSG_FUPDATE					0x1f
#define MSG_SHORTMINE				0x2f
#define MSG_TEXTMSG					0x3f
#define MSG_SHORTREGENSLOT			0x4f
#define MSG_SHORTTRIGGER			0x5f
#define MSG_SHORTTRIGVAR			0x6f
#define MSG_NAME					0x7f
#define MSG_INTERPOLATE				0x8f
#define MSG_BOMB					0x9f
#define MSG_BGOUPDATE				0xaf
#define MSG_PINGREQUEST				0xbf
#define MSG_PLAYERPINGS				0xbe
#define MSG_PINGREPLY				0xcf
#define MSG_LONGSTATUS				0xdf
#define MSG_SETTIME					0xef
#define MSG_REQTIME					0xf1
#define MSG_ACKMSG					0xf2
#define MSG_GUARANTEEDMSG			0xf3
#define MSG_KILLSDEATHSBIKENUM		0xf4
#define MSG_VERYSHORTUPDATE			0xf5
#define MSG_VERYSHORTFUPDATE		0xf6
#define MSG_ALIVE					0xf7		// short message to make sure people know im alive and still in the session....
#define MSG_VERYSHORTINTERPOLATE	0xf8
#define MSG_TEAMGOALS				0xf9
#define MSG_YOUQUIT					0xfa
#define MSG_SENDKILLSDEATHSBIKENUM	0xfb
#define MSG_SHORTSHIPHIT			0xfc
#define MSG_TITANBITS				0xfd
#define MSG_BIGPACKET				0xfe
#define MSG_SERVERUPDATE			0xe1
#define MSG_GAMEPARAMETERS			0xe2		// pseudohost sends this to server to set game info
#ifdef MANUAL_SESSIONDESC_PROPAGATE
#define MSG_SESSIONDESC				0xe3		// message to maually propagate session desc
#endif
#define MSG_TOSERVER				0xe4		// client uses to send messages to server ( ie. start game, reset etc )
#define MSG_SERVERKILLPICKUP		0xe5
#define MSG_SERVERSAYSSHIPDIED		0xe6
#define MSG_LEVELNAMES				0xe7		// msg contining names of levels on server
#define MSG_EXPLODESHIP				0xe8
#define MSG_SHIELDHULL				0xe9
#define MSG_SERVERSCORED			0xea

#define MSG_TRACKERINFO				0xe0		// tracker info for if host migrates in peer-peer game
#define MSG_TOCLIENT				0xeb		// message to client ( server game )

#define MSG_GROUPONLY_VERYSHORTFUPDATE		0xec
#define MSG_VERYSHORTDROPPICKUP				0xed

typedef struct _SERVERSCOREDMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		WhoScored;
	BYTE		Score;
}SERVERSCOREDMSG, *LPSERVERSCOREDMSG;


typedef struct _SHIELDHULLMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		Shield;
	BYTE		Hull;
	BYTE		ValidChange;
	uint8		Count;
}SHIELDHULLMSG, *LPSHIELDHULLMSG;


typedef struct _ALIVEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
}ALIVEMSG, *LPALIVEMSG;

typedef struct _SENDKILLSDEATHSBIKENUMMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
}SENDKILLSDEATHSBIKENUMMSG, *LPSENDKILLSDEATHSBIKENUMMSG;


typedef struct _YOUQUITMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		You;
}YOUQUITMSG, *LPYOUQUITMSG;



typedef struct GUARANTEEDMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	uint32		Ack;
	uint32		ID;
	BOOL		OverideOlderMessage;
	BOOL		AllPlayers;
	BYTE		Count;
	BYTE		StartOfMessage;
}GUARANTEEDMSG, *LPGUARANTEEDMSG;

typedef struct _ACKMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	uint32		ID;
	BYTE		AckTo;
}ACKMSG, *LPACKMSG;

typedef struct _KILLSDEATHSBIKENUMMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	int16		Kills;
	int16		Deaths;
	BYTE		BikeNum;		// which model to display for them...
}KILLSDEATHSBIKENUMMSG, *LPKILLSDEATHSBIKENUMMSG;

typedef struct _PINGMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		ToYou;		// 0 non-guarenteed...1 guarenteed..
	BYTE		Pad;
	LONGLONG	Time;
}PINGMSG, *LPPINGMSG;

typedef struct _PLAYERPINGSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	uint16		Ping[ MAX_PLAYERS ];
}PLAYERPINGSMSG, *LPPLAYERPINGSMSG;

typedef struct _BOMBMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    BYTE        WhoGotHit;
	BYTE		BombNum;
	float		BombTime;
} BOMBMSG, *LPBOMBMSG;


typedef struct _SHORTPICKUPMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		HowManyPickups;
	BYTE		Pickups;
    struct SHORTPICKUP  ShortPickup[MAXGENPICKUPCOUNT];
} SHORTPICKUPMSG, *LPSHORTPICKUPMSG;

typedef struct _SHORTREGENSLOTMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		HowManyRegenSlots;
	BYTE		RegenSlots;
    struct SHORTREGENSLOT  ShortRegenSlot[MAXGENREGENSLOTCOUNT];
} SHORTREGENSLOTMSG, *LPSHORTREGENSLOTMSG;

typedef struct _SHORTTRIGGERMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		HowManyTriggers;
	BYTE		Triggers;
    struct SHORTTRIGGER  ShortTrigger[MAXGENTRIGGERCOUNT];
} SHORTTRIGGERMSG, *LPSHORTTRIGGERMSG;

typedef struct _SHORTTRIGVARMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		HowManyTrigVars;
	BYTE		TrigVars;
    struct SHORTTRIGVAR  ShortTrigVar[MAXGENTRIGVARCOUNT];
} SHORTTRIGVARMSG, *LPSHORTTRIGVARMSG;

typedef struct _SHORTMINEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		HowManyMines;
	BYTE		Mines;
    struct SHORTMINE  ShortMine[MAXGENMINECOUNT];
} SHORTMINEMSG, *LPSHORTMINEMSG;

typedef struct _UPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    SHORTGLOBALSHIP  ShortGlobalShip;
} UPDATEMSG, *LPUPDATEMSG;

typedef struct _VERYSHORTUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    VERYSHORTGLOBALSHIP  ShortGlobalShip;
} VERYSHORTUPDATEMSG, *LPVERYSHORTUPDATEMSG;

typedef struct _FUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    FSHORTGLOBALSHIP  ShortGlobalShip;
} FUPDATEMSG, *LPFUPDATEMSG;

typedef struct _VERYSHORTFUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    FVERYSHORTGLOBALSHIP  ShortGlobalShip;
} VERYSHORTFUPDATEMSG, *LPVERYSHORTFUPDATEMSG;

typedef struct _GROUPONLY_VERYSHORTFUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    GROUPONLY_FVERYSHORTGLOBALSHIP  ShortGlobalShip;
} GROUPONLY_VERYSHORTFUPDATEMSG, *LPGROUPONLY_VERYSHORTFUPDATEMSG;


typedef struct _HEREIAMMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE        Old_WhoIAm;
	BYTE        Old_TeamNumber;
	DPID		ID;
	BOOL		Rejoining;
} HEREIAMMSG, *LPHEREIAMMSG;

typedef struct _INITMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    BYTE        YouAre;
	BYTE		Status;
	BOOL		PlayerReady[MAX_PLAYERS];
	BYTE		TeamNumber[MAX_PLAYERS];
	//int16		MaxKills;
	float		PacketsPerSecond;
	char		LevelName[32];
	//BOOL		HarmTeamMates;
	//BOOL		BrightShips;
	//BOOL		BikeExhausts;
	//int32		Collisions;
	uint32		PickupFlags[ MAX_PICKUPFLAGS ];
	int16		GoalScore;
	//int16		CTF_Type;
	int16		BountyBonusInterval;
	BOOL		RandomPickups;
	uint16		Seed1;
	uint16		Seed2;
	DWORD		dwUser3;	// user field 3 of session desc ( used for server state )
	BOOL		ServerCollisions;
	int16		PrimaryPickups;
	DPID		FromDpid;
	BYTE		GameStatus[MAX_PLAYERS];
} INITMSG, *LPINITMSG;

typedef struct _SHIPHITMSG
{
    BYTE        MsgCode;
	BYTE		WhoHitYou;
    BYTE        You;
	SHIPHIT		ShipHit;
	int16		Deaths;			// number of deaths
} SHIPHITMSG, *LPSHIPHITMSG;

typedef struct _SHORTSHIPHITMSG
{
    BYTE        MsgCode;
	BYTE		WhoHitYou;
    BYTE        You;
	SHORTSHIPHIT	ShipHit;
	int16		Deaths;			// number of deaths
} SHORTSHIPHITMSG, *LPSHORTSHIPHITMSG;


typedef struct _SHIPDIEDMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    BYTE        WhoKilledMe;
	BYTE		Type;
	BYTE		WeaponType;
	BYTE		Weapon;
	VECTOR		Pos;
} SHIPDIEDMSG, *LPSHIPDIEDMSG;

typedef struct _SERVERSAYSSHIPDIEDMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		WhoWasKilled;
    BYTE        WhoKilledThem;
	BYTE		Type;
	BYTE		WeaponType;
	BYTE		Weapon;
	VECTOR		Pos;
	int16		Deaths;
}SERVERSAYSSHIPDIEDMSG, *LPSERVERSAYSSHIPDIEDMSG;

typedef struct _KILLPICKUPMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	KILLPICKUPINFO	KillPickupInfo;

} KILLPICKUPMSG, *LPKILLPICKUPMSG;

typedef struct _SERVERKILLPICKUPMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	SERVERKILLPICKUPINFO	ServerKillPickupInfo;

} SERVERKILLPICKUPMSG, *LPSERVERKILLPICKUPMSG;

typedef struct _EXPLODESHIPMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	EXPLODESHIPINFO	ExplodeShipInfo;

} EXPLODESHIPMSG, *LPEXPLODESHIPMSG;

typedef struct _EXPSECONDARYMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	EXPSECONDARYINFO	ExplodeSecondaryInfo;

} EXPSECONDARYMSG, *LPEXPSECONDARYMSG;

typedef struct _TEAMGOALSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	TEAMGOALSINFO	TeamGoalsInfo;

} TEAMGOALSMSG, *LPTEAMGOALSMSG;

typedef struct _SHOCKWAVEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	SHOCKWAVEINFO	ShockwaveInfo;

} SHOCKWAVEMSG, *LPSHOCKWAVEMSG;

typedef struct _BGOUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BGOUPDATEINFO	BGOUpdateInfo;

} BGOUPDATEMSG, *LPBGOUPDATEMSG;

typedef struct _DROPPICKUPMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	PICKUPINFO	PickupInfo;

} DROPPICKUPMSG, *LPDROPPICKUPMSG;

typedef struct _VERYSHORTDROPPICKUPMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	VERYSHORTPICKUPINFO	PickupInfo;

} VERYSHORTDROPPICKUPMSG, *LPVERYSHORTDROPPICKUPMSG;


typedef struct _PRIMBULLPOSDIRMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	PRIMBULLPOSDIR	PrimBullPosDir;
} PRIMBULLPOSDIRMSG, *LPPRIMBULLPOSDIRMSG;

typedef struct _SECBULLPOSDIRMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	SECBULLPOSDIR	SecBullPosDir;
} SECBULLPOSDIRMSG, *LPSECBULLPOSDIRMSG;

typedef struct _TITANBITSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	TITANBITS	TitanBits;
} TITANBITSMSG, *LPTITANBITSMSG;

typedef	int16		SHORTSTATSTYPE[MAX_PLAYERS];

typedef struct _SHORTSTATSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    BYTE        WhosStats;
	SHORTSTATSTYPE		ShortStats;
} SHORTSTATSMSG, *LPSHORTSTATSMSG;

typedef struct _STATUSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BOOL		IsHost;			// from host ???
	BYTE		Status;
	BYTE		TeamNumber;
	uint16		TeamScore;		// if leaving game, used to propagate my score to another team member
	BOOL		IAmReady;		// used for team game - game cannot start until everyone is ready
	BYTE		Pickups;		// tells how much of the pickup list I have recieved..
	BYTE		RegenSlots;		// tells how much of the pickup regen slots list I have recieved..
	BYTE		Mines;			// tells how much of the mine list I have recieved..
	BYTE		Triggers;		// tells how much of the mine list I have recieved..
	BYTE		TrigVars;		// tells how much of the mine list I have recieved..
	float		PacketsPerSecond;
	LONGLONG	LevelCheckSum;
} STATUSMSG, *LPSTATUSMSG;

typedef struct _LONGSTATUSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	STATUSMSG	Status;
	char		LevelName[32];
	uint16		RandomStartPosModify;
} LONGSTATUSMSG, *LPLONGSTATUSMSG;

enum{
	TEXTMSGTYPE_Taunt1,
	TEXTMSGTYPE_Taunt2,
	TEXTMSGTYPE_Taunt3,
	TEXTMSGTYPE_TitleMessage,
	TEXTMSGTYPE_JoiningTeamGame,
	TEXTMSGTYPE_QuickTaunt,
	TEXTMSGTYPE_CaptureFlagMessage,
	TEXTMSGTYPE_ScoredWithFlag,
	TEXTMSGTYPE_ReturningFlag,
	TEXTMSGTYPE_ReturnedFlag,
	TEXTMSGTYPE_FlagDriftedIn,
	TEXTMSGTYPE_FlagEscaped,
	TEXTMSGTYPE_BountyMessage,
	TEXTMSGTYPE_QuickTauntWhisper,
	TEXTMSGTYPE_SpeechTaunt,
};

typedef struct _TEXTMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	char		Text[MAXTEXTMSG];
	BYTE		TextMsgType;
} TEXTMSG, *LPTEXTMSG;

typedef struct _NAMEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	char		Name[8];
} NAMEMSG, *LPNAMEMSG;

typedef struct _INTERPOLATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	VECTOR		NextPos;			// what my next position will be
	QUAT		NextQuat;			// what my next view angle will be
	float		NextBank;			// what my next bank angle will be..
	LONGLONG	NextTime;			// how long before I am at thos positions
} INTERPOLATEMSG, *LPINTERPOLATEMSG;

typedef struct _VERYSHORTINTERPOLATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	SHORTVECTOR	NextPos;			// what my next position will be
	SHORTQUAT	NextQuat;			// what my next view angle will be
	int16		NextBank;			// what my next bank angle will be..
	LONGLONG	NextTime;			// how long before I am at thos positions
} VERYSHORTINTERPOLATEMSG, *LPVERYSHORTINTERPOLATEMSG;


typedef struct _SETTIMEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	SETTIME		TimeInfo;

} SETTIMEMSG, *LPSETTIMEMSG;


typedef struct _REQTIMEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;

} REQTIMEMSG, *LPREQTIMEMSG;


typedef struct _SERVERUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	DWORD		Players;
	BYTE		Group[MAX_PLAYERS];
	BYTE		Status[MAX_PLAYERS];
}SERVERUPDATEMSG, *LPSERVERUPDATEMSG;


#define GAMEPARAM_GameTypeBits				0x7		// bits 0-2
#define GAMEPARAM_GameTypeBitShift			0x0
#define GAMEPARAM_CTFTypeBits				0x78	// bits 3-6
#define GAMEPARAM_CTFTypeBitShift			0x3
#define GAMEPARAM_BikeExhaustBit			0x80	// bit 7
#define GAMEPARAM_BrightBikesBit			0x100	// bit 8
#define GAMEPARAM_RandomisePickupsBit		0x200	// bit 9
#define GAMEPARAM_BountyBonusBit			0x400	// bit 10
#define GAMEPARAM_BountyBonusValueBits		0x3800	// bits 11 - 13 ( 5 second units )
#define GAMEPARAM_BountyBonusValueBitShift	0xb
#define GAMEPARAM_HarmTeamMatesBit			0x4000	// bit 14
#define GAMEPARAM_FlagCaptureScoreBits		0x78000	// bits 15 - 18
#define GAMEPARAM_FlagCaptureScoreBitShift	0xf
#define GAMEPARAM_ServerCollisionBit		0x80000	// bit 19
#define	GAMEPARAM_ResetKillsPerLevelBit		0x100000 // bit 20 
#define	GAMEPARAM_LagCompensationBits		0x600000 // bit 21 & 22
#define	GAMEPARAM_LagCompensationBitShift	0x15 

typedef struct _GAMEPARAMETERSMSG
{
	BYTE		        MsgCode;
    BYTE				WhoIAm;
	uint8				Level[ 8 ];		// level ( limited to 8 chars )
	uint8				ScoreLimit;			// score limit ( in units of 5 ) 
	uint8				TimeLimit;			// time limit ( 5 minute units )
	uint32				GameInfo;		// see above
	uint32				AllowedPickups[ MAX_PICKUPFLAGS ]; // allowed pickups
	int16				PrimaryPickups;
	uint16				MaxPlayers;
} GAMEPARAMETERSMSG, *LPGAMEPARAMETERSMSG;

#ifdef MANUAL_SESSIONDESC_PROPAGATE

typedef struct _SESSIONDESCMSG
{
	BYTE		        MsgCode;
    BYTE				WhoIAm;
	DWORD				dwUser1;
	DWORD				dwUser2;
	DWORD				dwUser3;
	DWORD				dwUser4;
} SESSIONDESCMSG, *LPSESSIONDESCMSG;

#endif

#define TOSERVERMSG_StartGame 0
#define TOSERVERMSG_Reset 1
#define TOSERVERMSG_ChangeLevel 2
#define TOSERVERMSG_IAmPseudoHost 3

typedef struct _TOSERVERMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		Type;
	BYTE		Data;
}TOSERVERMSG, *LPTOSERVERMSG;

#define TOCLIENTMSG_YouArePseudoHost 0

typedef struct _TOCLIENTMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		Type;
	BYTE		Data;
}TOCLIENTMSG, *LPTOCLIENTMSG;


#define MAXLEVELSPERBATCH 8

typedef struct _LEVELNAMESMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		TotalLevels;
	BYTE		ThisBatch;
	BYTE		FirstLevel;
	char		Level[ MAXLEVELSPERBATCH ][ 8 ];	// only first 8 chars of leve names are used
}LEVELNAMESMSG, *LPLEVELNAMESMSG;

typedef struct _TRACKERINFOMSG
{
    BYTE    MsgCode;
    BYTE    WhoIAm;
	uint32	addr;
	u_short	port;
	DWORD	freq;
	int		type;
	BOOL	shutdown;
	BOOL	gamespy;
}TRACKERINFOMSG, *LPTRACKERINFOMSG;

// globals needed in other modules
extern BOOL	IsPseudoHost;
extern int GuaranteedMessagesActive;
extern GUID autojoin_session_guid;
extern float FindSessionTimeout;
extern BOOL	ServerChoosesGameType;

#define SERVERLEVELS_Waiting	0
#define SERVERLEVELS_None		1
#define SERVERLEVELS_Got		2

/*
 * fn prototypes
 */
void    DestroyGame( void );
void    SendGameMessage( BYTE msg, DWORD to, BYTE row, BYTE col, BYTE mask );
void    EvaluateMessage( DWORD len , BYTE * MsgPnt );
void    ReceiveGameMessages( void );
void    initShip( uint16 i );
void	DplayGameUpdate();
void	SetupDplayGame();
void	PrimBullPosDirSend( uint16 OwnerType, uint16 OwnerID, uint16 BulletID, int8 Weapon,
						uint16 Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16 PowerLevel, float PLevel );
void SecBullPosDirSend( uint16 OwnerType, uint16 Owner, uint16 BulletID, uint16 Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * UpVector,
						VECTOR * DropDir, int8 Weapon );
void TitanBitsSend( uint16 OwnerType, uint16 Owner, uint16 BulletID, uint16 Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * UpVector,
						VECTOR * DropDir, int8 Weapon, VECTOR * Directions );
void	DropPickupSend( VECTOR * Pos, uint16 Group, VECTOR * Dir, float Speed, int16 Type, uint16 IDCount, int16 RegenSlot, BOOL Sparkle, float LifeCount, uint16 TriggerMod);
void	KillPickupSend( uint16 Owner, uint16 IDCount, int16 Style );
void	ServerKillPickupSend( uint16 Owner, uint16 IDCount, int16 Style, uint16 NewOwner );
void	ExplodeShip( uint16 Ship );
void	ExplodeSecondarySend( VECTOR * Pos, uint16 Group, uint16 OwnerType, uint16 Owner, uint16 IDCount, float ShockwaveSize );
void	CreateShockwaveSend( uint16 OwnerType, uint16 Owner, VECTOR * Pos, uint16 Group, float ShockwaveSize, BYTE Weapon );
void	IHitYou( BYTE you, float Damage, VECTOR * Recoil, VECTOR * Point, VECTOR * Dir, float Force, BYTE WeaponType, BYTE Weapon, BOOL FramelagRecoil );
void	EvalSysMessage( DWORD len , BYTE * MsgPnt );
void	ShipDiedSend( BYTE WeaponType, BYTE Weapon );
void	UpdateBGObjectSend( uint16 BGObject, int16 State, float Time );

void	smallinitShip( uint16 i );
void DemoPlayingDplayGameUpdate(void);
void DemoClean( void );
void PingGuarenteed(void);
void PingNonGuarenteed(void);
int FindSameLevel( char * Name );
void	RequestTime( void  );
void	SetTime( float Time );
void Demo_fwrite( const void *buffer, size_t size, size_t count , FILE *stream );
void StopDemoRecording( void );

BOOL AddGuaranteedMessage( int MessageLength , void * Message , BYTE MsgType, BOOL OverideOlderMessage, BOOL AllPlayers );
void ProcessGuaranteedMessages( BOOL ReleaseMessages , BOOL IgnoreTime , BOOL SendGuaranteed );
void AcknowledgeMessage( uint32 ID , uint32 Player , BYTE PlayerNum );


void InitAcknowledgeMessageQue( void );
void FreeAllPlayersAcknowledgeMessageQue( BYTE Player );
void ProcessAcknowledgeMessageQue( void );
BOOL CompareAcknowledgeMessageQue( BYTE Player , uint32 ID);
BOOL AddAcknowledgeMessageQue( BYTE Player , uint32 ID );

void SendBigPacket( BOOL SendGuaranteed );
void ServiceBigPacket( BOOL OverideTime );
void AddToBigPacket( int MessageLength , void * Message , BYTE MsgType );


BOOL AddMessageToSeverQue( void * Message );
void ServiceServer( BOOL OverideTime );
void InitServerMessageQue( void );
void FreeServerMessageQue( void );

void ServerIHitYou( BYTE Owner , BYTE you, float Damage, VECTOR * Recoil, VECTOR * Point, VECTOR * Dir, float Force, BYTE WeaponType, BYTE Weapon, BOOL FramelagRecoil );
BOOL UpdateAmmoAndValidateMessage( void * Message );
BOOL CheckPlayersActive( void );
BOOL AutoJoinSession( void );
void AllocatePseudoHost( void );

#endif


