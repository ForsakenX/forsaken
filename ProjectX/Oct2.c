/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 * $Header: /PcProjectX/Oct2.c 1062  11/11/98 16:00 Philipy $
 *
 * $Log: /PcProjectX/Oct2.c $
 * 
 * 1062  11/11/98 16:00 Philipy
 * various fixes for warnings / errors when compiling under VC6
 * 
 * 1061  5/11/98 3:34p Oliverc
 * Made Z_TRICK work as it should (but left disabled for patch beta 4 as
 * it messes up on translucent external views in certain levels)
 * 
 * 1060  3/11/98 16:06 Philipy
 * z trick stuff
 * 
 * 1059  23/10/98 12:29 Phillipd
 * 
 * 1058  22/10/98 8:09p Oliverc
 * Added support for international versions of Direct Input key labels
 * -- may be enabled on USE_DINPUT_NAMES define
 * 
 * 1057  22/10/98 14:35 Phillipd
 * 
 * 1056  22/10/98 12:31 Phillipd
 * 
 * 1055  22/10/98 12:02p Oliverc
 * Added BSP corruption check code (temporary)
 * 
 * 1054  22/10/98 11:35 Phillipd
 * 
 * 1053  21/10/98 17:27 Phillipd
 * 
 * 1052  21/10/98 12:39 Phillipd
 * 
 * 1051  17/09/98 15:27 Phillipd
 * 
 * 1050  7/09/98 12:36 Philipy
 * added DX version check
 * 
 * 1049  3/09/98 10:43 Phillipd
 * 
 * 1048  3/09/98 10:41 Phillipd
 * Fixed Viewport error......
 * 
 * 1047  27/08/98 20:12 Philipy
 * manual / auto server mode now selectable from menus
 * text now displayed when server in game & not rendering
 * dynamic PPS setting re-enabled for server
 * 
 * 1046  27/08/98 19:17 Philipy
 * no need to keep pressing return when trying to quit
 * 'waiting to quit...' now displayed until automatically quitting
 * 
 * 1045  27/08/98 17:26 Philipy
 * Pseudohost can select level, & migrates on quitting
 * players booted to titles due to not having new level are now informed
 * 
 * 1044  26/08/98 17:06 Philipy
 * tracker now migrates along with host
 * 
 * 1043  25/08/98 17:38 Philipy
 * added gamespy support
 * tracker config now selectable from start server game menu, & stored in
 * reg
 * 
 * 1042  25/08/98 15:44 Phillipd
 * 
 * 1041  20/08/98 4:03p Oliverc
 * Added SetViewportError() debug function (enabled on DEBUG_VIEWPORT
 * build switch)
 * 
 * 1040  20/08/98 15:28 Philipy
 * You can now join server based games after being launched by a lobby.
 * Level name is updated after pseudohost selects level
 * AVI can now play if no sound hw exists
 * started gamespy support
 * 
 * 1039  17/08/98 18:00 Philipy
 * removed loads of unreferenced local variables
 * 
 * 1038  17/08/98 17:13 Philipy
 * added -ServerChoosesGame & ServerAutoStart command line options
 * 
 * 1037  17/08/98 11:16 Philipy
 * prevented \n appearing in session desc
 * 
 * 1036  14/08/98 16:15 Philipy
 * heartbeat now migrates with host
 * 
 * 1035  14/08/98 15:25 Philipy
 * added trilinear option to menus
 * fixed level name / shutdown packet in heartbeat
 * 
 * 
 * 1034  14/08/98 9:13 Phillipd
 * DirectX6 is in effect.......
 * 
 * 1033  12/08/98 6:12p Oliverc
 * Changed colorkey from pink to blank for panel.bmp to fix 32 bit bug
 * 
 * 1032  10/08/98 17:33 Philipy
 * rewrote AVI player
 * 
 * 1031  7/08/98 12:45 Philipy
 * heartbeat now enabled for non tcp games ( if heartbeat.txt exists )
 * status type 0 now treated as status type 254
 * if lobby launched with max players set to 0, host is able to set max
 * players
 * shutdown udp packet now sent ( if specified in heartbeat.txt )
 * 
 * 1030  6/08/98 1:09p Oliverc
 * Levels not on HD now looked for on CD as well for patch
 * 
 * 1029  5/08/98 11:04 Philipy
 * added AutoStart facility ( joins game based on GUID in registery )
 * upped patch version to 1.02
 * 
 * 1028  4/08/98 9:57 Philipy
 * fixed 'server state reset to xxx' bug
 * 
 * 1027  28/07/98 10:39 Philipy
 * Max players now works properly for server games
 * 
 * 1026  24/07/98 12:40 Phillipd
 * 
 * 1025  24/07/98 10:02 Phillipd
 * 
 * 1024  23/07/98 18:40 Philipy
 * server now properly resets after timing out ( no msgs recieved ) while
 * in game
 * 
 * 1023  23/07/98 15:38 Philipy
 * server now resets if no packets recieved for 5 mins
 * 
 * 1022  23/07/98 15:20 Phillipd
 * 
 * 1021  23/07/98 15:16 Phillipd
 * 
 * 1020  22/07/98 16:29 Phillipd
 * 
 * 1019  22/07/98 16:24 Phillipd
 * 
 * 1018  22/07/98 16:23 Phillipd
 * 
 * 1017  22/07/98 14:57 Phillipd
 * 
 * 1016  22/07/98 12:39 Phillipd
 * 
 * 1015  21/07/98 17:31 Philipy
 * added timeout stuff for titles
 * 
 * 1014  21/07/98 15:25 Phillipd
 * Changed score display to cope with more than 16 players.....
 * 
 * 1013  17/07/98 5:39p Oliverc
 * Added level MXV file existance check to InitLevels()
 * 
 * 1012  7/16/98 12:25p Phillipd
 * 
 * 1011  7/16/98 11:37a Phillipd
 * 
 * 1010  7/16/98 11:27a Phillipd
 * 
 * 1009  16/07/98 10:53 Philipy
 * fixed dissapearing session after level change on server based game
 * 
 * 1008  7/15/98 11:44a Phillipd
 * 
 * 1007  7/14/98 11:27a Phillipd
 * 
 * 1006  14/07/98 11:15 Philipy
 * various patch bugs
 * pseudohost quitting in titles
 * 
 * 1005  7/14/98 10:38a Phillipd
 * 
 * 1004  14/07/98 10:27 Collinsd
 * Shadow code added under SHADOWTEST
 * 
 * 1003  7/14/98 10:07a Phillipd
 * 
 * 1002  7/13/98 11:47a Phillipd
 * 
 * 1001  7/13/98 11:43a Phillipd
 * 
 * 1000  7/13/98 11:22a Phillipd
 * 
 * 999   7/13/98 11:20a Phillipd
 * 
 * 998   7/09/98 11:50a Phillipd
 * Polytext now works being turned off for the Server.....HooRay...
 * 
 * 997   7/08/98 2:48p Phillipd
 * 
 * 996   7/08/98 2:46p Phillipd
 * 
 * 995   8/07/98 9:31 Oliverc
 * Converted multiplayer bounty and flag games to server operation for
 * patch
 * 
 * 994   7/07/98 18:05 Philipy
 * added lobby autostart code ( when all players have recieved init msg )
 * added num primary weapons menu option ( propergated to other players &|
 * server )
 * extracted new title text for localisation
 * 
 * 993   7/07/98 9:31a Phillipd
 * 
 * 992   7/06/98 5:09p Phillipd
 * 
 * 991   7/06/98 5:02p Phillipd
 * 
 * 990   7/03/98 5:03p Phillipd
 * 
 * 989   7/03/98 4:51p Phillipd
 * 
 * 988   7/03/98 4:27p Phillipd
 * 
 * 987   7/03/98 4:26p Phillipd
 * 
 * 986   7/03/98 4:20p Phillipd
 * 
 * 985   7/03/98 3:25p Phillipd
 * 
 * 984   3/07/98 11:54 Philipy
 * heartbeat & quickstart stuff
 * 
 * 983   1/07/98 15:29 Collinsd
 * Hopefully fixed bug that lost pickups.
 * 
 * 982   30/06/98 10:38 Collinsd
 * Better
 * 
 * 981   26/06/98 17:45 Collinsd
 * Added server debug info.
 * 
 * 980   6/24/98 10:22a Phillipd
 * 
 * 979   24/06/98 10:04 Philipy
 * heartbeat stuff ( not currently active )
 * 
 * 978   22/06/98 17:38 Oliverc
 * Removed CD checks in multiplayer games for patch
 * 
 * 977   6/22/98 2:16p Phillipd
 * Option to reset the score for every level..............
 * 
 * 976   6/22/98 9:56a Phillipd
 * 
 * 975   19/06/98 11:36 Collinsd
 * Fixed software Fullscreen Rearview
 * 
 * 974   17/06/98 19:33 Philipy
 * more win98 stuff
 * 
 * 973   6/17/98 12:49p Phillipd
 * 
 * 972   16/06/98 16:32 Philipy
 * more lobby / join game stuff
 * 
 * 971   13/06/98 20:46 Philipy
 * improved lobby support:
 * host now migrates properly
 * you can quit b4 starting game without screwing up session
 * 
 * 970   11/06/98 17:28 Collinsd
 * Fixed some warnings.
 * 
 * 969   11/06/98 16:57 Philipy
 * loads of win98 shareware version stuff
 * 
 * 968   11/06/98 9:54 Philipy
 * files checked in prior to starting Win98 shareware version
 * 
 * 967   6/11/98 9:29a Phillipd
 * 
 * 966   6/10/98 11:12a Phillipd
 * 
 * 965   6/09/98 3:49p Phillipd
 * 
 * 964   6/09/98 12:09p Phillipd
 * 
 * 963   9/06/98 11:23 Philipy
 * server now reset if pseudohost does not have any valid levels
 * fixed timed game bug for server based games
 * 
 * 962   8/06/98 12:28 Philipy
 * server levels now sent across to pseudohost. Pseudohost can only pick
 * valid levels
 * 
 * 961   6/04/98 2:44p Phillipd
 * 
 * 960   3/06/98 15:30 Philipy
 * added server in game options for rendering on/off and selecting players
 * pseudohost can now select if server does collisions. This gets
 * propagated in MSG_GameParams and MSG_Init
 * 
 * 959   22/05/98 17:51 Philipy
 * more work on session info
 * 
 * 958   5/22/98 10:29a Phillipd
 * 
 * 957   5/20/98 4:50p Phillipd
 * Full Screen Rearview Functional....
 * 
 * 956   5/20/98 4:46p Phillipd
 * 
 * 955   20/05/98 16:42 Philipy
 * stoped server team games starting straight away
 * 
 * 954   5/20/98 12:19p Phillipd
 * 
 * 953   20/05/98 9:39 Philipy
 * implemented front end server menus
 * removed ( invalid ) ping from sessions menu
 * changed EnumPlayers so that it solely uses MSG_NAME
 * 
 * 952   5/11/98 10:16a Phillipd
 * 
 * 951   4/29/98 4:41p Phillipd
 * 
 * 950   28/04/98 14:53 Oliverc
 * Disabled most of extra info on specific compile switches
 * 
 * 949   4/27/98 4:01p Phillipd
 * 
 * 948   21/04/98 10:20 Collinsd
 * Added french and italian.
 * 
 * 947   4/17/98 11:51a Phillipd
 * 
 * 946   17/04/98 11:46 Collinsd
 * 
 * 945   4/17/98 10:51a Phillipd
 * 
 * 944   4/17/98 10:40a Phillipd
 * 
 * 943   4/17/98 9:28a Phillipd
 * 
 * 942   15/04/98 12:28 Oliverc
 * In-game text substituted for localised definitions
 * 
 * 941   4/13/98 9:47p Phillipd
 * 
 * 940   11/04/98 17:09 Collinsd
 * 
 * 939   11/04/98 16:34 Oliverc
 * Disabled DebugPrintf() for FINAL_RELEASE
 * along with log file & batch file deletion
 * 
 * 938   4/11/98 11:53a Phillipd
 * 
 * 937   4/10/98 2:21p Phillipd
 * 
 * 936   4/10/98 1:21p Phillipd
 * 
 * 935   4/10/98 12:43p Phillipd
 * 
 * 934   4/09/98 4:37p Phillipd
 * 
 * 933   4/09/98 12:05p Phillipd
 * 
 * 932   9/04/98 11:52 Philipy
 * added facility to enable individual cheats for multiplayer
 * 
 * 931   8/04/98 20:47 Philipy
 * title text messages now properly initialised
 * holo-glaire removed for sw version
 * compound buffer size can now be set in command line and opt file
 * prevented "level select disabled" from appearing at start of
 * multiplayer game
 * 
 * 930   8/04/98 15:09 Oliverc
 * 
 * 929   4/08/98 2:12p Phillipd
 * 
 * 928   8/04/98 13:34 Oliverc
 * Changed CD file check to work with compressed/uncompressed install
 * version
 * 
 * 927   8/04/98 12:36 Oliverc
 * Changed internal multipayer level order to match T:\Gamedata
 * 
 * 926   8/04/98 10:22 Collinsd
 * Screenshots folder created if doesn't exist.
 * 
 * 925   8/04/98 10:15 Philipy
 * Title CD track now only started after valid CD check
 * 
 * 924   8/04/98 9:41 Collinsd
 * 
 * 923   8/04/98 9:39 Collinsd
 * 
 * 922   7/04/98 17:50 Philipy
 * removed multiplayer taunts
 * AVI thread now allowed to exit nicely rather than being terminated
 * fixed inter-level bug
 * fixed bug in enemy taunts
 * 
 * 921   4/07/98 3:02p Phillipd
 * 
 * 920   4/07/98 2:45p Phillipd
 * 
 * 919   7/04/98 11:00 Philipy
 * potentially fixed crash when going from AVI to titles
 * fixed CD audio looping
 * no CD audio in front end unless full install
 * bike features sliders now give correct values
 * 
 * 918   4/07/98 10:59a Phillipd
 * 
 * 917   4/07/98 10:57a Phillipd
 * 
 * 916   6/04/98 21:18 Philipy
 * flips now reenabled when joining existing game
 * 
 * 915   6/04/98 19:33 Oliverc
 * Fixed unnecessary D3D Zbuffer clearing for software version
 * 
 * 914   6/04/98 17:06 Philipy
 * modified various sfx
 * 
 * 913   6/04/98 12:33 Oliverc
 * Dropped "flag" level (earlier version of "midpand") from internal
 * multiplayer level table
 * 
 * 912   6/04/98 11:28 Philipy
 * added big packets option
 * upped frequency of some speech sfx
 * re-implemented holo-scanline
 * 
 * 911   4/06/98 9:39a Phillipd
 * 
 * 910   5/04/98 17:16 Oliverc
 * Added OriginalLevels flag (TRUE iff original levels are being used...)
 * 
 * 909   5/04/98 16:58 Philipy
 * cheats now disabled for multiplayer
 * sfx now paused for shortcut single player menus
 * 
 * 908   5/04/98 15:01 Philipy
 * started pre AVI CD accesss ( not yet implemented )
 * bike engine freq now done on 5 frame average
 * prevented CD track from playing in titles if set to off
 * NoDynamic SFX does not bike bike computer static anymore
 * water detail slider now only has two levels
 * 
 * 907   4/04/98 23:04 Oliverc
 * Changed InitLevels() for the FINAL_RELEASE to use an internal level
 * list if no levels.dat file found installed
 * 
 * 906   4/04/98 7:59p Phillipd
 * 
 * 905   4/04/98 7:53p Phillipd
 * If host quits viewing score everyone does....
 * 
 * 904   4/04/98 7:27p Phillipd
 * 
 * 903   4/04/98 18:31 Collinsd
 * 
 * 902   4/04/98 14:22 Philipy
 * mode scaling stuff is now calculated rather than based on fixed values
 * added -NoBlitTextScaling option to ReadIni and command line options
 * 
 * 901   4/04/98 2:03p Phillipd
 * 
 * 900   4/04/98 11:52 Oliverc
 * Added default joystick axis & buttons config (including SpaceOrb)
 * Added extra CD check
 * 
 * 899   4/03/98 8:36p Phillipd
 * 
 * 898   3/04/98 18:23 Philipy
 * fixed placeholder memory problem
 * remove existing team scores from team join menu
 * 
 * 897   3/04/98 18:04 Collinsd
 * 
 * 896   3/04/98 17:19 Collinsd
 * 
 * 895   3/04/98 17:06 Collinsd
 * Change for software version
 * 
 * 894   3/04/98 16:03 Philipy
 * fixed CD audio stuff
 * 
 * 893   4/03/98 3:35p Phillipd
 * 
 * 892   3/04/98 14:04 Philipy
 * fixed credit toggle stuff
 * 
 * 891   3/04/98 12:25 Collinsd
 * Fix for attract mode if no dmo.
 * 
 * 890   2/04/98 21:06 Philipy
 * Added taunts ( single & multiplayer, plus enemy biker taunts )
 * added flygirl to front end.
 * sliders for biker, bike computer and taunt speech volume
 * added new sfx for title
 * 
 * 889   2/04/98 16:53 Oliverc
 * Added Z-buffer clear override (independent of background clears) for
 * software version
 * 
 * 888   2/04/98 14:29 Oliverc
 * Added default settings for joystick axes & buttons
 * 
 * 887   2/04/98 12:29 Collinsd
 * Fixed bug in bgobjects in multiplayer
 * 
 * 886   4/02/98 8:49a Phillipd
 * 
 * 
 * 884   1/04/98 16:39 Collinsd
 * Background transparancies now work, zbuffer clear called when clear
 * screen called.
 * 
 * 883   1/04/98 11:44 Collinsd
 * Invulnerability effect now no longer appears in demos. and god mode
 * works properly over multiple levels.
 * 
 * 882   1/04/98 9:15 Oliverc
 * Changed in-game function key assignments
 * 
 * 881   31/03/98 20:01 Collinsd
 * Colour keying working.
 * 
 * 879   31/03/98 14:04 Collinsd
 * Game complete credits done.
 * 
 * 878   31/03/98 10:30 Collinsd
 * Temp clear zbuffer added
 * 
 * 877   30/03/98 23:40 Oliverc
 * Added shortcut keys F2, F3 & F4 for in-game options, load & save game
 * menus
 * 
 * 876   3/30/98 6:57p Phillipd
 * 
 * 875   30/03/98 17:30 Philipy
 * added cd specific path stuff
 * added new bike computers
 * prevented File_Exists being called every time a dynamic sfx is played
 * 
 * 874   3/30/98 2:25p Phillipd
 * 
 * 873   30/03/98 12:05 Oliverc
 * Added valid installation check for FINAL_RELEASE
 * Added code to re-check CD whenever a DEVICECHANGE message is received
 * to prevent users removing CD during play (for FINAL_RELEASE)
 * Added code to try to cancel CD autoplay from disturbing the game (but
 * didn't seem to work)
 * 
 * 872   29/03/98 20:27 Oliverc
 * 
 * 871   29/03/98 20:05 Philipy
 * LOBBY_DEBUG is now no longer defined - no Direct Play registery
 * settings are written out
 * 
 * 870   29/03/98 19:59 Philipy
 * cd path now verified earlier
 * sfx no longer reloaded when changing biker / bike computer
 * mouse sensitivity rounding error fixed
 * 
 * 869   3/28/98 2:06p Phillipd
 * 
 * 868   27/03/98 21:06 Philipy
 * added end game sequence for completing the game with cheating
 * 
 * 867   27/03/98 19:58 Philipy
 * changed TloadReloadPlaceHolder() to use MovePPMtoVidMem()
 * fixed level pic display
 * 
 * 866   27/03/98 17:09 Oliverc
 * replaced old "outside_map => bomb killed you" code with new version
 * that puts you back into your last valid restart position
 * 
 * 865   27/03/98 15:57 Oliverc
 * Fixed bug in default joystick config for Spaceorb
 * 
 * 864   27/03/98 15:52 Collinsd
 * Software version update.
 * 
 * 863   27/03/98 12:58 Philipy
 * changed cheat mode stuff
 * fixed bug that prevented individual variants of variant sfx to be
 * mapped
 * correct menutv now displayed between levels
 * 
 * 862   26/03/98 15:29 Oliverc
 * Added LIMITED_LEVELS build option to allow hardcoding of allowed levels
 * 
 * 861   26/03/98 15:16 Collinsd
 * Mods for Chris
 * 
 * 860   24/03/98 21:36 Oliverc
 * Fixed memory leak in force feedback info
 * 
 * 859   24/03/98 21:07 Philipy
 * fixed quicktext stuff
 * sfx do not pause when in multiplayer mode
 * rear camera not shown for splash demos
 * 
 * 858   24/03/98 18:01 Collinsd
 * Added ZBuffer clear for title/splash
 * 
 * 857   24/03/98 16:20 Philipy
 * added new sfx
 * 
 * 856   24/03/98 12:14 Collinsd
 * Added credits as splash screen.
 * 
 * 855   23/03/98 10:21 Oliverc
 * Merged all button and keyboard control configurations into one
 * 
 * 854   21/03/98 14:52 Philipy
 * only relevent sfx are loaded between levels
 * 
 * 853   3/21/98 2:48p Phillipd
 * 
 * 852   3/21/98 2:34p Phillipd
 * 
 * 851   3/21/98 1:43p Phillipd
 * 
 * 850   21/03/98 11:07 Collinsd
 * FIX UV's included again in 2dtextures.c
 * 
 * 849   3/20/98 11:08a Phillipd
 * 
 * 848   3/20/98 10:36a Phillipd
 * 
 * 847   19/03/98 20:49 Collinsd
 * Added Model and Texture loading optimisation. ( Doesn't load
 * textures/models not used in level/mode ).
 * 
 * 846   19/03/98 20:32 Philipy
 * added different end of game scenarios
 * code now written to config to indicate if secret biker is available
 * 
 * 845   19/03/98 14:43 Collinsd
 * 
 * 844   19/03/98 14:22 Collinsd
 * Invulnerability effect no longer visible in demo.
 * 
 * 843   19/03/98 11:29 Philipy
 * implemented new acclaim splash screen plus AVI
 * 
 * 842   19/03/98 11:29 Collinsd
 * Bike appears in missile and pip cameras
 * 
 * 841   3/18/98 4:03p Phillipd
 * re jigged stats screen......
 * 
 * 840   3/18/98 3:31p Phillipd
 * 
 * 839   3/17/98 10:40a Phillipd
 * 
 * 838   16/03/98 19:36 Collinsd
 * Changed Clipping Distance when nitro used. ( Fixes Laser )
 * 
 * 837   16/03/98 16:40 Philipy
 * fixed buffered key problem
 * added AVI to splash screens
 * 
 * 836   15/03/98 18:40 Philipy
 * added water effect splash screen
 * fixed bug with end game sequence
 * implemented attract mode
 * text macros now saved in config
 * 
 * 835   14/03/98 18:58 Collinsd
 * Added godmode and made debug mode work even when you change level.
 * 
 * 834   14/03/98 15:55 Collinsd
 * Taken out frame clamp.
 * 
 * 833   14/03/98 14:06 Collinsd
 * Scatter works with Collision Perspective.
 * 
 * 832   12/03/98 10:36 Collinsd
 * 
 * 831   11/03/98 21:01 Collinsd
 * Added naked bikers.
 * 
 * 830   11/03/98 12:40 Philipy
 * allowed debug msgs
 * 
 * 829   11/03/98 10:55 Philipy
 * all sfx now stopped for pause mode
 * correct level info now displayed in briefing screen
 * correct level now entered after completing a previously saved level
 * level timer now accurate
 * 
 * 828   3/10/98 12:17p Phillipd
 * 
 * 827   3/10/98 11:18a Phillipd
 * 
 * 826   3/10/98 10:45a Phillipd
 * 
 * 825   9/03/98 16:57 Philipy
 * only one 'player is joining the game' message is broadcast when joining
 * team game
 * 
 * 824   3/09/98 4:30p Phillipd
 * Check for secret level..16 and secret biker put in....
 * if you dont collect 15 crystals and complete level 15 then it goes to
 * the end seq...
 * otherwise you go to the secret level...if you collect the crystal there
 * and complete
 * that level then the secret biker should be enabled....
 * 
 * 823   8/03/98 17:08 Philipy
 * correct command line information now stored in registery for lobby
 * session
 * 
 * 822   8/03/98 16:50 Philipy
 * prevent MenuBack from some menus when lobby launched
 * added team game support for lobby
 * 
 * 821   6/03/98 17:37 Philipy
 * implemented ability to run when launched by lobby
 * 
 * 820   3/06/98 5:13p Phillipd
 * 
 * 819   6/03/98 14:43 Collinsd
 * 25fps
 * 
 * 818   6/03/98 14:25 Collinsd
 * 
 * 817   6/03/98 12:09 Collinsd
 * Taken out drop pickup code.
 * 
 * 816   6/03/98 10:29 Collinsd
 * Screen Saving now using my routine... and is on f8
 * 
 * 815   3/05/98 11:52a Phillipd
 * Previously triggered text message are now accessable in single player
 * by pressing
 * f9-10 previous and last
 * 
 * 814   5/03/98 10:50 Collinsd
 * Fixed glitch and text overlay on save game.
 * 
 * 813   3/05/98 8:47a Phillipd
 * 
 * 812   3/03/98 16:59 Oliverc
 * New multiplayer CTF mode stuff (1st attempt)
 * 
 * 811   2/03/98 21:13 Collinsd
 * No longer use multiple bit or secfire.
 * 
 * 810   2/28/98 11:26a Phillipd
 * 
 * 809   28/02/98 11:06 Collinsd
 * Fixed Crystal and Gold bars tally over multiple levels.
 * 
 * 808   27/02/98 19:11 Philipy
 * fixed load game sfx bug
 * added pseudo dithering for 8 bit saved game pic
 * flygirl now selectable from front end ( no model displayed )
 * 
 * 807   2/26/98 10:00p Oliverc
 * Fixed bug in force feedback joystick config reading (failed when no
 * joystick connected if one had been previously defined)
 * 
 * 806   26/02/98 20:41 Philipy
 * added front end for load game
 * 
 * 805   2/26/98 4:50p Phillipd
 * 
 * 804   26/02/98 9:30 Oliverc
 * Disabled Bombtag
 * Fixed pickup flags bug
 * Disabled logos for EXTERNAL_DEMOs
 * 
 * 803   25/02/98 21:08 Collinsd
 * Fixed Stats memsets
 * 
 * 802   25/02/98 18:38 Collinsd
 * Added solid poly option.
 * 
 * 801   25/02/98 16:19 Oliverc
 * More multiplayer tweaks
 * 
 * 800   25/02/98 16:06 Collinsd
 * Save Picture for save game...
 * 
 * 799   2/25/98 10:50a Phillipd
 * 
 * 798   24/02/98 16:54 Oliverc
 * 1st attempt at bounty hunt multiplayer game
 * 
 * 797   23/02/98 15:31 Philipy
 * implemented single player level timer
 * 
 * 796   2/23/98 2:02p Phillipd
 * Load Save now works.
 * 
 * 795   23/02/98 10:37 Philipy
 * added inter level stuff
 * 
 * 794   21/02/98 16:24 Philipy
 * added text messages for capture flag
 * 
 * 793   21/02/98 13:04 Philipy
 * added in game load / save for sfx
 * 
 * 792   21/02/98 12:48 Oliverc
 * Active goals now re-checked every game loop
 * as well as number of flags held by each team
 * 
 * 791   20/02/98 19:41 Oliverc
 * 2nd prototype of capture the flag game
 * 
 * 790   20/02/98 15:29 Philipy
 * re-implented AVI
 * splash screens can now play demos and AVIs
 * 
 * 789   20/02/98 12:30 Oliverc
 * Prototype goal load/release/check/display for capture the flag
 * multiplayer
 * 
 * 788   20/02/98 11:53 Collinsd
 * Fixed small bug in bgobject collision.  And updated software code.
 * 
 * 787   19/02/98 22:00 Collinsd
 * Added flygirl biker.
 * 
 * 786   18/02/98 19:41 Oliverc
 * 
 * 785   18/02/98 11:53 Oliverc
 * First feature-complete version of force feedback joystick code
 * 
 * 784   17/02/98 17:15 Philipy
 * level.mis now used to store level name as well as mission briefing
 * if file not there, reverts back to old level name
 * 
 * 783   17/02/98 9:16 Philipy
 * added support for placeholder textures, which can be dynamically
 * updated
 * implemented mission briefing screens
 * 
 * 782   11/02/98 12:59 Oliverc
 * Added basic force feedback support for joysticks
 * 
 * 781   11/02/98 12:54 Philipy
 * changed calls to PlaySfx to call with volume rather than distance
 * 
 * 780   9/02/98 12:21 Philipy
 * added sound buffer memory managment
 * only one piece of bike computer speech can now play at a time
 * 
 * 779   7/02/98 18:58 Collinsd
 * Release of Thermal gauge now in.
 * 
 * 778   7/02/98 18:42 Collinsd
 * Added Temperature Gauege.
 * 
 * 777   2/07/98 1:39p Phillipd
 * Int the bombs now in......
 * 
 * 776   5/02/98 10:06 Oliverc
 * PowerVR hacks (texture blend mode now MODULATEALPHA, COLORKEY_ENABLE
 * switched on)
 * 
 * 775   2/02/98 20:08 Philipy
 * added configurable quick text message buttons
 * 
 * 774   2/02/98 14:20 Philipy
 * Harm Teammates, Disable Pickups front end stuff done
 * 
 * 773   2/02/98 9:19 Philipy
 * fixed splashscreen bug
 * 
 * 772   30/01/98 9:12 Collinsd
 * Added Laser for exogenon and added corruption checking for componented
 * objects.
 * 
 * 770   1/29/98 2:35p Phillipd
 * 
 * 769   1/29/98 2:27p Phillipd
 * Demo can now be saved to ram....and can be saved if your not the
 * host...
 * 
 * 768   29/01/98 14:19 Oliverc
 * 
 * 767   29/01/98 11:30 Philipy
 * fixed loading bar
 * 
 * 766   28/01/98 21:43 Oliverc
 * Fixed bugs in team tally screen display
 * 
 * 765   1/28/98 6:45p Phillipd
 * 
 * 764   1/28/98 4:55p Phillipd
 * 
 * 763   1/28/98 2:35p Phillipd
 * 
 * 762   1/28/98 2:29p Phillipd
 * 
 * 761   28/01/98 14:25 Oliverc
 * Changed splash screens for different markets
 * 
 * 760   28/01/98 10:03 Philipy
 * added SPLASH_Dummy
 * 
 * 759   27/01/98 14:08 Oliverc
 * Added MindSpring splash screen
 * 
 * 758   27/01/98 12:30 Oliverc
 * 
 * 757   27/01/98 11:13 Oliverc
 * Fixed demo playback speeds so only a single 100% appears
 * Eliminated intermediate "restore game" menu between "play demo" on
 * start screen and actual "play demo" menu
 * 
 * 756   27/01/98 11:05 Philipy
 * fixed team game stuff
 * 
 * 755   1/27/98 10:09a Phillipd
 * 
 * 754   26/01/98 18:34 Philipy
 * Opps! - fixed bug with new splashscreen stuff
 * 
 * 753   26/01/98 18:23 Philipy
 * fixed video memory leaks
 * splash screens now display after release view, and call InitScene,
 * InitView after  completion
 * 
 * 752   26/01/98 10:20 Collinsd
 * 
 * 751   24/01/98 17:38 Philipy
 * fixed multiplayer join-quit-join bug
 * fixed attract mode loading wrong level
 * 
 * 750   1/24/98 5:10p Phillipd
 * 
 * 749   24/01/98 15:27 Collinsd
 * Updated Code for SFX on Animations ( Disabled )
 * 
 * 748   24/01/98 12:47 Collinsd
 * Fixed Bug where on machine with blitted text.  Font page was
 * deallocated on loading rolling demo.
 * 
 * 747   23/01/98 17:36 Philipy
 * sorted out release view for client joining game
 * 
 * 746   23/01/98 16:35 Collinsd
 * ReleaseLevel() now done between single player levels.
 * 
 * 745   1/23/98 12:33p Phillipd
 * 
 * 744   23/01/98 11:24 Collinsd
 * Added override for solid screen poly bilinear
 * 
 * 743   22/01/98 19:14 Philipy
 * fixed re-loading looping sfx while in level
 * biker speech now switchable
 * 
 * 741   22/01/98 15:06 Oliverc
 * Added loading of pre-calculated group connectivity, visibility,
 * indirect visibility, and sound attentuation tables
 * 
 * 740   22/01/98 11:57 Collinsd
 * Added DoDamage override invul.
 * 
 * 739   22/01/98 9:46 Philipy
 * team game stats
 * 
 * 738   22/01/98 9:38 Collinsd
 * Single player timelimit does not effect multiplayer limit any more.
 * 
 * 737   22/01/98 8:45 Collinsd
 * Deallocate CountDownDigits in release level.
 * Turned off CoundDown when playing Demos
 * 
 * 736   21/01/98 17:27 Philipy
 * attract mode splash screen stuff
 * 
 * 735   21/01/98 16:30 Philipy
 * fixed some spotsfx bugs
 * 
 * 734   21/01/98 12:19 Philipy
 * Added attract mode for shareware
 * fixed looping sfx volume bug
 * 
 * 733   1/20/98 12:25p Phillipd
 * 
 * 732   20/01/98 12:25 Philipy
 * Fixed level starting bug
 * 
 * 731   1/20/98 11:40a Phillipd
 * 
 * 730   1/19/98 4:21p Phillipd
 * Bright Ships flag added.....
 * 
 * 729   19/01/98 13:01 Philipy
 * fixed mission text not being displayed
 * possibly fixed stats bug ?
 * 
 * 728   1/19/98 12:19p Phillipd
 * Lights now have a visible link list.....
 * 
 * 727   1/19/98 10:00a Phillipd
 * 
 * 726   19/01/98 9:50 Philipy
 * more changes to critical sections
 * 
 * 725   18/01/98 23:39 Philipy
 * Rogue sfx killing code changed slightly
 * 
 * 724   1/16/98 5:43p Phillipd
 * 
 * 723   16/01/98 12:11 Philipy
 * added spaceorb auto configure stuff
 * splash screen now displays in highest res it can find
 * 
 * 722   15/01/98 18:55 Philipy
 * added new splash screens
 * 
 * 720   15/01/98 17:04 Philipy
 * changed PlayPannedSfx calls to use float for freq
 * 
 * 719   1/15/98 3:52p Phillipd
 * 
 * 718   15/01/98 12:51 Collinsd
 * Time now works when people join an existing timed game.  Crystals
 * reinitialised.  Damage Flash only in Normal_Mode.
 * 
 * 716   14/01/98 12:25 Oliverc
 * Changed level format MXV to include pre-calculated ambient lighting
 * info
 * Added UVFix option to command line options and OPT file to allow
 * texture coordinates to be adjusted at runtime (currently only for level
 * backgrounds)
 * 
 * 715   14/01/98 11:34 Collinsd
 * Dont scale textures.
 * 
 * 714   14/01/98 10:12 Collinsd
 * Add logfile and batchfile works again.
 * 
 * 713   13/01/98 12:06 Philipy
 * added looping spot sfx support, and changed looping sfx to use static
 * list rather than dynamic linked list
 * 
 * 712   13/01/98 9:39 Collinsd
 * Added debug to show untriggered enemies.  Started Enemy Poly Colisions.
 * 
 * 711   12/01/98 16:57 Philipy
 * now properly releasing splash screen surfaces
 * 
 * 710   12/01/98 16:47 Philipy
 * fixed deallocation errors
 * 
 * 709   12/01/98 16:02 Collinsd
 * 
 * 708   12/01/98 14:29 Philipy
 * fixed bug that prevented loading with no soundcard
 * 
 * 707   12/01/98 11:23 Philipy
 * fixed leveloading bug
 * 
 * 706   12/01/98 0:08 Philipy
 * bug fixes
 * added inter-level mission briefing
 * changed splash screen code, + added splash screen on exit
 * 
 * 705   11/01/98 17:54 Philipy
 * inter level mission screen
 * 
 * 704   10/01/98 20:35 Philipy
 * fixed no speech option
 * 
 * 703   10/01/98 19:30 Philipy
 * bug fixes
 * 
 * 702   10/01/98 15:12 Oliverc
 * Fixed triggerload bug for multiplayer
 * 
 * 701   1/10/98 1:44p Phillipd
 * Quick Fix
 * 
 * 700   1/10/98 11:52a Phillipd
 * 
 * 699   9/01/98 17:25 Philipy
 * player is now forced to start on level 0
 * 
 * 698   9/01/98 11:27 Collinsd
 * 
 * 697   9/01/98 11:14 Philipy
 * CD nows plays last track
 * CD now replays current track from seperate  ( low priority ) thread -
 * but still causes pause
 * loading bar now displayed when loading
 * 
 * 696   9/01/98 11:01 Collinsd
 * Stuff back to normal and goldbars not get carried over levels.
 * 
 * 695   8/01/98 15:23 Collinsd
 * Added Timer for single player levels.
 * 
 * 694   7/01/98 16:06 Collinsd
 * Changed pickups you already got not to play sfx constantly.
 * Changed Blend mode code to check for trans first
 * 
 * 693   1/07/98 3:17p Phillipd
 * 
 * 691   7/01/98 14:51 Collinsd
 * Fixed ScreenPolys crashing game when changing screen mode.
 * 
 * 690   7/01/98 9:34 Philipy
 * added title room sfx
 * added ability to select bike computer, biker with sfx loaded
 * 
 * 689   6/01/98 18:29 Oliverc
 * Enabled bike controls menu on F1 key in game for external demo
 * 
 * 688   6/01/98 12:06 Oliverc
 * Fixed "no cursor after ALT+TAB/Close" bug by changing un-malloc'ed
 * Msg() to DebugPrintf()
 * 
 * 687   5/01/98 16:41 Collinsd
 * 
 * 686   1/05/98 3:29p Phillipd
 * Sfx for score....
 * 
 * 685   1/05/98 3:18p Phillipd
 * More sfx...
 * 
 * 684   5/01/98 14:27 Collinsd
 * Moved Spotfx so the fireprimary is in sync.
 * 
 * 683   5/01/98 11:36 Philipy
 * physically removed some comented out lines
 * 
 * 682   3/01/98 15:01 Oliverc
 * Pressing F1 key in-game now goes straight to bike controls menu
 * 
 * 681   1/02/98 12:38p Phillipd
 * Pause mode added for single player.....
 * 
 * 680   1/02/98 9:50a Phillipd
 * 
 * 679   31/12/97 12:51 Collinsd
 * 
 * 678   31/12/97 11:53 Collinsd
 * 
 * 677   31/12/97 11:41 Philipy
 * CD no longer plays in title room
 * 
 * 676   31/12/97 11:34 Oliverc
 * Disabled POLYGONAL_COLLISIONS
 * 
 * 675   12/31/97 9:19a Phillipd
 * 
 * 674   30/12/97 12:53 Philipy
 * removed redundant function
 * 
 * 673   12/30/97 12:09p Phillipd
 * 
 * 672   12/29/97 11:18a Phillipd
 * 
 * 671   24/12/97 9:20 Philipy
 * fixed dynamic sound stuff by making X_Malloc, X_Free etc. atomic
 * 
 * 670   12/22/97 10:19a Phillipd
 * Texture memory is now got from directdraw2 interface....
 * Modecase is set to nearest resolution that is smaller.....
 * 
 * 669   12/20/97 5:03p Phillipd
 * 
 * 668   12/20/97 12:43p Phillipd
 * Levelname is now sent across for a level change in Multiplayer....
 * Enemies dont appear in Multiplayer Games....
 * 
 * 666   19/12/97 14:41 Collinsd
 * Gold bars not carried over levels.
 * 
 * 665   19/12/97 9:39 Collinsd
 * Fixed model trans colours. ( orig verts ).
 * 
 * 664   18/12/97 21:16 Collinsd
 * Start pos loading from .stp first then mxv
 * 
 * 663   18/12/97 11:31 Collinsd
 * Added Restart Points, Changed Oneoff anim to only activate if not
 * already animating.
 * 
 * 662   12/18/97 11:25a Phillipd
 * 
 * 661   12/18/97 9:21a Phillipd
 * 
 * 660   12/17/97 12:06p Phillipd
 * 
 * 659   16/12/97 16:36 Oliverc
 * Level list now loaded from arbitrary file in Levels folder (mission.dat
 * for single player and battle.dat for multiplayer games), with existing
 * levels.dat used as a fallback for compatibility with existing levels
 * 
 * 658   12/16/97 2:44p Phillipd
 * 
 * 657   12/15/97 2:43p Phillipd
 * 
 * 656   12/15/97 12:00p Phillipd
 * 
 * 655   12/13/97 1:35p Phillipd
 * 
 * 653   12/10/97 10:39a Phillipd
 * Players names are now propergated across the network when changed...
 * Difficulty levels are functional....
 * 
 * 652   6/12/97 19:31 Oliverc
 * Level loading now fails if no BSP found in BSP_ONLY mode
 * 
 * 651   12/06/97 2:53p Phillipd
 * Fixed Phils Sfx Crash Bug....Doh
 * 
 * 650   5/12/97 14:11 Collinsd
 * Allocated ships fixed.
 * 
 * 649   12/05/97 9:36a Phillipd
 * 
 * 648   4/12/97 15:11 Collinsd
 * Ships are now allocated models.
 * 
 * 647   3/12/97 20:01 Oliverc
 * Added NULL pointer checks before misc free() calls
 * 
 * 646   3/12/97 18:51 Philipy
 * changed some sfx timer stuff
 * 
 * 645   12/03/97 5:18p Phillipd
 * 
 * 644   12/03/97 12:05p Phillipd
 * 
 * 643   2/12/97 18:05 Oliverc
 * 
 * 642   2/12/97 16:46 Philipy
 * compound sfx buffer is now stopped when quitting game
 * 
 * 641   12/02/97 4:20p Phillipd
 * 
 * 640   2/12/97 11:52 Philipy
 * boot demo stuff
 * 
 * 639   12/01/97 3:27p Phillipd
 * 
 * 638   12/01/97 11:16a Phillipd
 * 
 * 637   1/12/97 11:06 Philipy
 * fixed de-allocation bug with axis / button names of unconnected
 * joysticks
 * 
 * 636   1/12/97 10:51 Oliverc
 * Fixed memory alloced but not freed bugs
 * 
 * 635   1/12/97 10:19 Collinsd
 * Fixed Memory Allocation Bugs.
 * 
 * 634   12/01/97 10:11a Phillipd
 * 
 * 633   1/12/97 9:48 Philipy
 * checked in with changes commented out in order to get Xmem stuff
 * 
 * 632   12/01/97 9:46a Phillipd
 * 
 * 631   11/29/97 4:35p Phillipd
 * Xmem is now in effect...use it allways....
 * 
 * 630   27/11/97 12:34 Philipy
 * fixed sound bug on self play demo
 * demo playback speed now given as true percentage
 * 
 * 629   27/11/97 12:34 Oliverc
 * Changed ship inside/outside map check to use PointInsideSkin()
 * 
 * 628   11/27/97 12:25p Phillipd
 * 
 * 627   26/11/97 11:48 Philipy
 * implemented dynamic loading of SFX, dynamic allocation of mixing
 * channels.
 * 3D sound currently disabled.
 * 
 * 626   11/21/97 2:47p Phillipd
 * 
 * 625   21/11/97 11:21 Philipy
 * fxed releasing of splash screen
 * 
 * 624   11/21/97 10:56a Phillipd
 * Max Kills to end a level....
 * 
 * 623   21/11/97 9:09 Collinsd
 * Taken out all models not used.
 * 
 * 622   11/21/97 9:06a Phillipd
 * 
 * 621   11/19/97 12:37p Phillipd
 * Trigger bug fixed...
 * Teleports now trigger on offable....
 * num of Secrets Are calculated
 * 
 * 620   11/18/97 3:44p Phillipd
 * 
 * 619   18/11/97 14:54 Oliverc
 * Prevented remote camera from inadvertently using player's main FOV (eg
 * when nitroing)
 * 
 * 618   11/18/97 2:44p Phillipd
 * 
 * 617   11/17/97 4:34p Phillipd
 * 
 * 616   11/17/97 10:13a Phillipd
 * Trig area V2...
 * 
 * 615   13/11/97 15:04 Philipy
 * fixed crash bug - unable to restart game second time
 * 
 * 614   11/11/97 9:41a Phillipd
 * 
 * 613   11/11/97 8:49 Collinsd
 * 
 * 612   7/11/97 14:25 Philipy
 * fixed bug when changing mode from title room: InitTitle was not being
 * called
 * 
 * 611   6/11/97 15:23 Philipy
 * JoystickInfo structure now dynamically allocates space for Joystick /
 * Button / POV names.
 * All deallocated in OnceOnlyRelease().
 * 
 * 610   6/11/97 10:25 Oliverc
 * 
 * 609   11/05/97 3:35p Phillipd
 * 3d panel disabled...
 * 
 * 608   4/11/97 17:34 Philipy
 * now skips stats AVI with -NoAVI flag
 * 
 * 607   4/11/97 16:26 Philipy
 * AVI now plays for stats screens
 * implemented scrolling messages (not enabled)
 * 
 * 606   11/03/97 6:20p Phillipd
 * 
 * 605   30/10/97 19:21 Collinsd
 * Enemies clip properly to groups now.
 * 
 * 604   30/10/97 17:00 Collinsd
 * Clipping of BGObjects now works.
 * 
 * 603   10/30/97 9:30a Phillipd
 * Bikes with different mods started...
 * 
 * 602   27/10/97 15:38 Philipy
 * added in-game cd track playing
 * 
 * 601   24/10/97 12:33 Philipy
 * more debug menu stuff
 * 
 * 600   24/10/97 12:07 Collinsd
 * zone display now works of menus.
 * 
 * 599   24/10/97 10:36 Philipy
 * Updated debug menus
 * 
 * 598   24/10/97 10:03 Collinsd
 * 
 * 597   24/10/97 10:01 Collinsd
 * 
 * 596   10/24/97 9:40a Phillipd
 * 
 * 595   10/24/97 9:18a Phillipd
 * 
 * 594   23/10/97 16:49 Philipy
 * added tggle (number key 1) for playing AVI on texture.
 * (no texture conversion yet, could appear corrupted)
 * 
 * 593   23/10/97 13:52 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 592   21/10/97 13:10 Philipy
 * levels.dat can now (optionally) store CD track num after each level
 * 
 * 591   20/10/97 11:58 Oliverc
 * Trigger info now dynamically allocated on loading
 * 
 * 590   17/10/97 15:13 Philipy
 * 
 * 589   17/10/97 14:23 Philipy
 * 
 * 588   16/10/97 18:18 Philipy
 * Implemented pre-game AVI player
 * 
 * 587   16/10/97 18:17 Philipy
 * 
 * 586   16/10/97 17:56 Oliverc
 * Changed BSP loading so no *z.bsp files loaded any more
 * 
 * 585   10/16/97 11:34a Phillipd
 * 
 * 584   10/16/97 10:06a Phillipd
 * 
 * 583   10/16/97 10:00a Phillipd
 * 
 * 582   10/16/97 9:56a Phillipd
 * 
 * 580   15/10/97 17:00 Collinsd
 * 
 * 579   15/10/97 9:37 Collinsd
 * Added logfile/batchfile creation code.
 * 
 * 578   10/14/97 4:48p Phillipd
 * 
 * 577   11/10/97 19:07 Oliverc
 * Added real-time mapped lights in level
 * 
 * 576   10/09/97 3:29p Phillipd
 * External forces coming...
 * Shield and Hull now floats
 * 
 * 575   8/10/97 20:31 Philipy
 * 
 * 574   8/10/97 16:32 Collinsd
 * Added spotfx and started lightning bolt.
 * 
 * 573   10/08/97 2:05p Phillipd
 * 
 * 572   7/10/97 12:37 Collinsd
 * Added spotfx for a level and point and vector stuff for MX and MXA
 * models.
 * 
 * 571   10/07/97 12:00p Phillipd
 * 
 * 570   10/07/97 10:18a Phillipd
 * 
 * 569   7/10/97 10:16 Philipy
 * fixed mouse look-up bug when starting subsequent games.
 * DInput() was being called every restart, with no corresponding
 * TermDInput().
 * DInput is now only ever done once (TitleOnceOnly)
 * 
 * 568   10/06/97 7:19p Phillipd
 * water stuff...more
 * 
 * 567   6-10-97 3:40p Philipy
 * Joystick info now loaded/saved for unconnected joysticks (prevents
 * information being lost from config file)
 * 
 * 566   4/10/97 12:53 Collinsd
 * Fixed level change. ( Preload Enemies ).
 * 
 * 565   3/10/97 17:08 Collinsd
 * Fixed mode change/level change bug.
 * 
 * 564   10/03/97 4:24p Phillipd
 * 
 * 563   3-10-97 12:17p Philipy
 * Joystick stuff almost done
 * 
 * 562   1-10-97 7:21p Philipy
 * more joystick stuff - writing to config, changed JOYSTICKINFO
 * structure, user interface for assigning buttons & axis.
 * Fixed bug that prevented you from going into wireframe mode - keyboard
 * buffer is now flushed in InitView, and TermDInput is not called from
 * ReleaseView (something needs to be done about releasing DirectX objects
 * when quitting from titles!)
 * 
 * 561   1/10/97 17:29 Collinsd
 * Fixed model init.
 * 
 * 560   10/01/97 2:48p Phillipd
 * 
 * 559   9/29/97 11:55a Phillipd
 * Dawn of a new age with directplay 3
 * 
 * 558   29/09/97 11:35 Collinsd
 * New bgobject stuff enabled. Pickups/Secondary and Enemies now have
 * group link lists.  Flags for secondary bullets being targeted and hit
 * now added.
 * 
 * 557   9/29/97 9:08a Phillipd
 * 
 * 556   26-09-97 3:27p Philipy
 * config file can now store joystick info.
 * now only does InitDInput() at start of title screen, and TermDInput()
 * when quitting game
 * 
 * 555   24/09/97 16:55 Collinsd
 * Added new bgobject stuff.  Added bounding box calc and display code.
 * and changes bgobject state changes.
 * 
 * 554   9/23/97 4:24p Phillipd
 * 
 * 553   22/09/97 10:40 Collinsd
 * Software version works again. ( Now with trasnsluecency )
 * 
 * 552   9/19/97 2:40p Phillipd
 * 
 * 551   9/18/97 3:56p Phillipd
 * Fixed stats bug....
 * 
 * 550   9/18/97 12:16p Phillipd
 * 
 * 549   9/18/97 10:48a Phillipd
 * 
 * 548   17-09-97 4:08p Philipy
 * enabled dynamic loading of textures, plus splash screen now released
 * before othe textures are loaded
 * 
 * 547   17/09/97 15:58 Collinsd
 * Fixed blit's in software.
 * 
 * 546   17/09/97 9:55 Collinsd
 * Blitting now works in software versions in 320x240 mode.
 * 
 * 545   16/09/97 17:52 Collinsd
 * More of Chris's stuff works.
 * 
 * 544   16/09/97 10:59 Collinsd
 * Added Chris's code
 * 
 * 543   15-09-97 3:23p Philipy
 * message displayed, then correct menu displayed on new host machine if
 * host migrates.
 * Host can now start another game, having quit 'waiting to start' menu
 * 
 * 541   14-09-97 6:54p Philipy
 * joining existing team game - you can now select which team to go into,
 * and view current scores
 * 
 * 540   12/09/97 10:52 Collinsd
 * Oops f6
 * 
 * 539   12/09/97 10:50 Collinsd
 * Added global portal clipping on/off using shift f7
 * 
 * 538   10/09/97 11:27 Collinsd
 * Fixed clipping of models to portals
 * 
 * 537   5-09-97 5:47p Philipy
 * Menus now use buffered keys
 * 
 * 536   5-09-97 4:12p Philipy
 * Changed menus to use buffered keys
 * 
 * 535   5/09/97 11:57 Collinsd
 * Fixed Scores bug.
 * 
 * 534   9/04/97 3:28p Phillipd
 * 
 * 529   3-09-97 12:30p Philipy
 * game now returns to copyright screen
 * 
 * 528   3/09/97 12:20 Oliverc
 * Added command-line options to set normal FOV and screen aspect ratio
 * for widescreen TVs
 * 
 * 527   3/09/97 11:10 Collinsd
 * Added ColourMode Variable
 * 
 * 526   3-09-97 11:09a Philipy
 * moved lpDDSSplash definition to Title.c
 * changed ATTRACTMODECOUNTVALUE to 1.0F 
 * 
 * 525   2/09/97 15:06 Oliverc
 * Disabled portal clipping of lines
 * 
 * 524   31/08/97 13:24 Collinsd
 * Added aiming pos, firing pos and laser works with enemies now.
 * 
 * 523   8/31/97 12:40p Phillipd
 * 
 * 522   26-08-97 4:18p Philipy
 * VDU screen is cleared before starting multiplayer game from new menus
 * 
 * 521   20/08/97 12:18 Oliverc
 * Triangle visibility statistics now kept (min, max, avg) for all
 * background groups and written to .VIS file in level folder iff player
 * using vanilla '-data' command line option to indicate he wants to use
 * PROJXDATA environment variable
 * 
 * 520   8/19/97 12:59p Phillipd
 * 
 * 519   17/08/97 14:58 Collinsd
 * Taken out old code.
 * 
 * 518   8/17/97 1:17p Phillipd
 * Enemies coming along....
 * 
 * 517   8/15/97 4:24p Phillipd
 * enemies coming along
 * 
 * 516   14-08-97 5:42p Philipy
 * added direct draw surface for splash screens
 * 
 * 515   14/08/97 12:38 Collinsd
 * Targeting computer only in debug mode.
 * 
 * 514   13/08/97 10:16 Oliverc
 * (Dave) Fixed bug in call to InitLogos() due changes to screenpolys
 * 
 * 513   12/08/97 16:50 Collinsd
 * Added warp5 mode for blending. ( Modulate alpha, srcalpha, invsrcalpha
 * )
 * 
 * 
 * 511   11/08/97 11:59 Collinsd
 * Added data override for levels as well
 * 
 * 510   11/08/97 10:12 Collinsd
 * Added override data directory option. ( SFX don't work yet! )
 * 
 * 509   9/08/97 14:56 Collinsd
 * Added ScreenPoly TPage Link list and flat shaded screen polys for phil
 * 
 * 508   8/08/97 14:18 Collinsd
 * Display of Polys has now been updated to use TPage link list.
 * 
 * 507   8/08/97 9:33a Phillipd
 * 
 * 506   7/08/97 21:25 Collinsd
 * Change FmPolys to use tpage link list.
 * 
 * 505   8/04/97 3:48p Phillipd
 * Water effects..Temporary and not finished..currently disabled
 * 
 * 504   2/08/97 12:34 Oliverc
 * Added wireframe mode option for debugging group/portal visibility
 * 
 * 501   28/07/97 16:44 Collinsd
 * Tracker uses ships to target, demo mode works again?
 * 
 * 500   25/07/97 12:10 Collinsd
 * Changed ships bike index, and changed skin for olly.
 * 
 * 499   21/07/97 17:31 Collinsd
 * 
 * 498   17/07/97 16:06 Oliverc
 * Added ship cruise control keys
 * 
 * 497   17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 496   16/07/97 11:57 Collinsd
 * Made lines more flexible
 * 
 * 495   16/07/97 11:31 Collinsd
 * Added collision zone display.
 * 
 * 494   15/07/97 10:10 Collinsd
 * Added display of triggerzones on Shift F10 in debug mode.
 * 
 * 493   7/14/97 5:07p Phillipd
 * 
 * 492   7/12/97 3:34p Phillipd
 * 
 * 491   9/07/97 16:59 Collinsd
 * Fixed MXA models in titles
 * 
 * 490   8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 489   8/07/97 14:06 Collinsd
 * Added componented objects to enemies.
 * 
 * 488   7/07/97 4:31p Phillipd
 * Slght change to the autodetail calculation....
 * 
 * 487   4/07/97 16:35 Collinsd
 * Started user control componented objects
 * 
 * 486   7/03/97 5:27p Phillipd
 * 
 * 485   7/03/97 4:49p Phillipd
 * 
 * 484   7/03/97 10:11a Phillipd
 * More Ai and node
 * 
 * 483   6/30/97 10:27a Phillipd
 * enemy ai started....
 * 
 * 482   6/24/97 5:10p Phillipd
 * 
 * 481   6/24/97 11:12a Phillipd
 * 
 * 480   6/23/97 10:51a Phillipd
 * 
 * 479   6/23/97 9:56a Phillipd
 * 
 * 478   6/23/97 9:53a Phillipd
 * 
 * 477   6/23/97 9:43a Phillipd
 * 
 * 476   6/19/97 3:01p Phillipd
 * 
 * 475   6/19/97 2:24p Phillipd
 * 
 * 474   6/19/97 2:17p Phillipd
 * 
 * 473   6/19/97 10:24a Phillipd
 * 
 * 472   19/06/97 9:00 Collinsd
 * Added error checking to displaytitle()
 * 
 * 471   6/18/97 6:02p Phillipd
 * 
 * 470   6/18/97 5:47p Phillipd
 * 
 * 469   6/18/97 4:26p Phillipd
 * 
 * 468   6/18/97 3:44p Phillipd
 * 
 * 467   6/17/97 5:06p Phillipd
 * 
 * 466   6/17/97 4:05p Phillipd
 * 
 * 465   6/17/97 4:03p Phillipd
 * 
 * 464   6/17/97 12:13p Phillipd
 * 
 * 463   16/06/97 16:22 Collinsd
 * Changed blend modes again
 * 
 * 462   6/16/97 10:16a Phillipd
 * 
 * 461   14/06/97 14:01 Collinsd
 * Changed death blend mode
 * 
 * 460   6/14/97 1:51p Phillipd
 * 
 * 459   6/14/97 1:11p Phillipd
 * 
 * 458   6/14/97 11:03a Phillipd
 * Made all on screen info for 512x384 !bPrimary to use Polys.....
 * 
 * 457   13/06/97 16:04 Collinsd
 * Taken out old bgobjects
 * 
 * 456   13/06/97 14:24 Collinsd
 * Added more error information
 * 
 * 455   13/06/97 14:22 Collinsd
 * Added more error information
 * 
 * 454   13/06/97 12:37 Collinsd
 * Changed blend mode checking.
 * 
 * 453   12/06/97 17:39 Oliverc
 * Added check to kill off player if he manages to get outside the map
 * 
 * 452   6/12/97 4:33p Phillipd
 * 
 * 451   6/12/97 2:27p Phillipd
 * 
 * 450   6/12/97 11:15a Phillipd
 * 
 * 449   6/11/97 3:48p Phillipd
 * 
 * 448   6/11/97 11:16a Phillipd
 * 
 * 447   6/11/97 10:17a Phillipd
 * 
 * 446   6/10/97 4:00p Phillipd
 * 
 * 445   10/06/97 14:50 Collinsd
 * Added more error checking again for mload
 * 
 * 444   6/10/97 11:54a Phillipd
 * 
 * 443   6/10/97 11:51a Phillipd
 * 
 * 442   6/10/97 11:11a Phillipd
 * 
 * 441   6/10/97 10:17a Phillipd
 * 
 * 440   6/10/97 9:41a Phillipd
 * 
 * 439   6/10/97 9:01a Phillipd
 * 
 * 438   6/07/97 4:46p Phillipd
 * 
 * 437   6/07/97 4:11p Phillipd
 * 
 * 436   6/07/97 4:10p Phillipd
 * 
 * 435   6/07/97 10:52a Phillipd
 * 
 * 434   6/06/97 17:01 Oliverc
 * 
 * 433   6/06/97 4:54p Phillipd
 * 
 * 432   6/06/97 3:32p Phillipd
 * 
 * 431   6-06-97 10:15a Philipy
 * CameraStatus is reset at default case of InitView - prevents in game
 * menus from being suppressed
 * 
 * 430   6/05/97 4:09p Phillipd
 * 
 * 429   6/05/97 10:45a Phillipd
 * Joystick now works with directinput2.....
 * 
 * 428   5/06/97 9:59 Collinsd
 * Changed texture blend recognition
 * 
 * 427   6/04/97 4:56p Phillipd
 * 
 * 426   6/04/97 3:39p Phillipd
 * 
 * 425   6/04/97 9:40a Phillipd
 * 
 * 424   3/06/97 14:17 Collinsd
 * 
 * 423   3/06/97 14:03 Collinsd
 * Added more error checking.
 * 
 * 422   6/03/97 11:47a Phillipd
 * 
 * 421   6/03/97 10:46a Phillipd
 * 
 * 420   6/02/97 9:09a Phillipd
 * 
 * 419   5/31/97 12:35p Phillipd
 * Sfx bug fixed...Now check distance for an overide rather than just
 * checking to see if its the same sfx num...You can quit out of the menus
 * again.....
 * 
 * 418   30/05/97 16:49 Collinsd
 * added logos
 * 
 * 417   5/30/97 11:41a Phillipd
 * 
 * 416   5/30/97 11:08a Phillipd
 * 
 * 415   5/28/97 2:53p Phillipd
 * 
 * 414   5/27/97 5:40p Phillipd
 * 
 * 413   22-05-97 4:32p Philipy
 * Fixed merge fuckup
 * 
 * 412   22-05-97 4:04p Philipy
 * 
 * 411   22/05/97 9:18 Collinsd
 * Fixed loading of BGObjects.
 * 
 * 410   5/21/97 5:00p Phillipd
 * 
 * 409   5/21/97 3:44p Phillipd
 * All Game processes are now in a seperate routine to MainGame so they
 * can be called before or after rendering
 * 
 * 408   5/21/97 10:25a Phillipd
 * 
 * 407   5/21/97 10:17a Phillipd
 * 
 * 406   5/21/97 9:56a Phillipd
 * 
 * 405   5/20/97 2:20p Phillipd
 * 
 * 404   20/05/97 10:17 Collinsd
 * Made texture loading for polys/fmpolys and screenpolys flexible.
 * 
 * 403   18/05/97 22:27 Collinsd
 * Done more work on BGObjects
 * 
 * 402   17/05/97 20:15 Collinsd
 * Add PowerVR code for polys and screenpolys as well.......
 * 
 * 401   5/17/97 5:17p Phillipd
 * 
 * 400   17/05/97 16:01 Collinsd
 * 
 * 399   5/17/97 3:45p Phillipd
 * 
 * 398   5/17/97 2:15p Phillipd
 * 
 * 397   16/05/97 15:55 Oliverc
 * New BSP stuff
 * 
 * 396   5/08/97 3:48p Phillipd
 * Background clear to non black is now done if the affected group is
 * visible not just if you are in it....
 * 
 * 395   5/08/97 3:48p Phillipd
 * 
 * 394   5/07/97 5:44p Phillipd
 * 
 * 393   5/07/97 10:47a Phillipd
 * 
 * 392   7/05/97 9:57 Collinsd
 * Fixed reloading of title models.
 * 
 * 391   6/05/97 14:52 Oliverc
 * Changed demograb path (for TimB)
 * 
 * 390   6-05-97 12:28p Philipy
 * 
 * 389   6-05-97 12:25p Philipy
 * 
 * 388   5/01/97 3:12p Phillipd
 * Mxamodels now take a file list..
 * 
 * 387   5/01/97 11:00a Phillipd
 * Added Quit.bat optout....Just put Quit.bat in the same dir as
 * Projectx.exe and it will quit.....
 * 
 * 386   4/30/97 5:35p Phillipd
 * 
 * 385   4/30/97 4:37p Phillipd
 * 
 * 384   30-04-97 3:22p Philipy
 * 
 * 383   4/30/97 12:27p Phillipd
 * 
 * 382   4/30/97 11:23a Phillipd
 * 
 * 381   29/04/97 17:49 Oliverc
 * Moved FingerPrint[] to Multiplayer.c
 * 
 * 380   29/04/97 15:05 Collinsd
 * Fixing bug in screenpolys.
 * 
 * 379   26/04/97 20:29 Collinsd
 * Added more checks and fixed one small bug in screenpolys.c
 * 
 * 378   4/25/97 2:51p Phillipd
 * 
 * 377   4/25/97 10:36a Phillipd
 * Better demo playback...which player eyes works again
 * 
 * 376   4/24/97 5:22p Phillipd
 * 
 * 375   4/19/97 3:44p Phillipd
 * 
 * 374   4/19/97 1:23p Phillipd
 * Only Ships Dont collide when no_collisions is set...
 * Trigger areas work better.....
 * 
 * 373   4/15/97 3:32p Phillipd
 * Simple Panel is now only loaded when you need it..
 * 
 * 372   4/09/97 10:00a Phillipd
 * 
 * 371   4/07/97 12:27p Phillipd
 * 
 * 370   4/05/97 12:08p Phillipd
 * 
 * 369   4/02/97 4:08p Phillipd
 * Added Weapon kill Stats..
 * 
 * 368   4/02/97 11:44a Phillipd
 * 
 * 367   4/02/97 8:51a Phillipd
 * Bsps enabled...
 * 
 * 366   1-04-97 5:01p Collinsd
 * Added clear override for groups.
 * 
 * 365   1-04-97 3:41p Collinsd
 * Added colour key for background and clear colour per group.
 * 
 * 364   27/03/97 14:56 Oliverc
 * New fingerprint
 * 
 * 363   25/03/97 17:37 Oliverc
 * 
 * 362   3/22/97 11:19a Phillipd
 * made all check for currdriver >= 2 into
 * d3dappi.thisdriver.bishardware....more reliable..
 * 
 * 361   3/21/97 12:23p Phillipd
 * 
 * 360   20-03-97 5:46p Collinsd
 * Countdown timer now works in multiplayer.
 * 
 * 359   3/20/97 3:41p Phillipd
 * removed c&c backdrop from scores...
 * 
 * 358   3/20/97 12:23p Phillipd
 * Init Sound is only called once as is destroysound...
 * 
 * 357   3/19/97 3:34p Phillipd
 * Added Invulnerable flag and timer....gets sent across network...
 * 
 * 356   17-03-97 8:57a Collinsd
 * Poly's, Screen Polys, FmPolys now can create up to 1000 each.
 * 
 * 355   3/14/97 5:32p Phillipd
 * 
 * 354   3/13/97 4:35p Phillipd
 * 
 * 353   3/13/97 11:28a Phillipd
 * Auto Detail level added..
 * Text all one colour unless printing own name...
 * 
 * 352   13-03-97 9:02a Collinsd
 * 
 * 351   12-03-97 4:47p Collinsd
 * Added recoil and new shrapnel model.
 * 
 * 350   12-03-97 2:13p Collinsd
 * Added more error handling.
 * 
 * 349   3/12/97 12:39p Phillipd
 * 
 * 348   3/11/97 4:55p Phillipd
 * Demo Interpolate stuff is now working but not finished...
 * 
 * 347   3/07/97 9:51a Phillipd
 * 
 * 346   5-03-97 5:00p Collinsd
 * 
 * 345   5-03-97 9:44a Collinsd
 * ZBuffer clipped.
 * 
 * 344   4-03-97 6:01p Collinsd
 * Added fog to tracker.
 * 
 * 343   4-03-97 4:50p Collinsd
 * Added Tracker.
 * 
 * 342   3/01/97 12:57p Phillipd
 * handles multiple join requests from 1 person,,,,
 * 
 * 341   27-02-97 2:08p Collinsd
 * Added optimisation to various files.
 * 
 * 340   2/27/97 8:58a Phillipd
 * 
 * 339   2/26/97 4:05p Phillipd
 * 
 * 338   2/26/97 3:10p Phillipd
 * 
 * 337   26-02-97 12:02p Collinsd
 * 
 * 336   26-02-97 12:01p Collinsd
 * Added screen poly text.
 * 
 * 335   2/25/97 5:14p Phillipd
 * 
 * 334   2/25/97 4:05p Phillipd
 * 
 * 333   2/24/97 4:47p Phillipd
 * GameElapsedTime is now relative to the demo not the
 * real world....so dynamic speed change is possible..
 * 
 * 332   2/24/97 11:55a Phillipd
 * 
 * 331   24-02-97 11:55a Collinsd
 * Pickups not generated in demo mode.
 * 
 * 330   24-02-97 11:21a Collinsd
 * Added gravgon check for restart. and added bounding on object selection
 * depending on mode.
 * 
 * 329   2/24/97 10:00a Phillipd
 * Demo mode with multispeed has been added..
 * 
 * 328   2/20/97 11:10a Phillipd
 * Changed the way a Player Get his Player Number...
 * Much more stable and friendly...
 * 
 * 327   15-02-97 9:32p Collinsd
 * Portals now use variable execute buffers.  They also
 * allocate/deallocate themselves properly now.
 * 
 * 326   15-02-97 2:45p Collinsd
 * Added error checking for most file loading.
 * 
 * 325   2/14/97 12:47p Phillipd
 * 
 * 324   2/13/97 9:42a Phillipd
 * 
 * 323   12-02-97 12:03p Collinsd
 * Added error checking to readfiles();  Also added triggers to enemies.
 * 
 * 322   11-02-97 5:08p Collinsd
 * Triggers/RegenPoints and pickups now are sent across correctly.
 * 
 * 321   2/10/97 12:10p Phillipd
 * 
 * 320   2/08/97 12:33p Phillipd
 * 
 * 319   2/07/97 4:59p Phillipd
 * 
 * 318   6/02/97 16:18 Oliverc
 * Fixed bug in level name reading for user-supplied levels file
 * 
 * 317   6/02/97 9:35 Oliverc
 * Added alternate level spec file command line option
 * 
 * 316   2/05/97 4:16p Phillipd
 * 
 * 315   2/05/97 12:01p Phillipd
 * nicer directplay bug handle..
 * 
 * 314   5/02/97 9:01 Oliverc
 * Added trigger loading
 * 
 * 313   2/04/97 5:11p Phillipd
 * new player joining brute force reject..
 * 
 * 312   4-02-97 4:11p Collinsd
 * 
 * 311   2/03/97 5:16p Phillipd
 * Translusceny is now controlled by global execute buffers.... which is
 * much better...
 * 
 * 310   31-01-97 9:24a Collinsd
 * Added detail level for missile trails.
 * 
 * 309   29-01-97 5:55p Collinsd
 * fixed a couple of bugs.
 * 
 * 308   27-01-97 2:27p Collinsd
 * Added bsp volume display
 * 
 * 307   1/27/97 10:58a Phillipd
 * Textures enabled flag added to debug menu
 * 
 * 306   1/25/97 5:32p Phillipd
 * 
 * 305   1/23/97 3:55p Phillipd
 * 
 * 304   22-01-97 6:00p Collinsd
 * Added debug node cube.
 * 
 * 303   20-01-97 2:45p Collinsd
 * Temp, added test for Dave Hodgeson.
 * 
 * 302   20-01-97 11:21a Collinsd
 * Added time limit to multiplayer games.
 * 
 * 301   1/17/97 6:04p Phillipd
 * 
 * 300   1/16/97 12:16p Phillipd
 * 
 * 299   1/15/97 12:15p Phillipd
 * started work on the server mode
 * 
 * 298   1/13/97 5:40p Phillipd
 * added message macors to multiplayer....
 * 
 * 297   13-01-97 4:28p Collinsd
 * Added 6 Door Types.
 * 
 * 296   10-01-97 12:34p Collinsd
 * Added Doors, Chanded pickups/door/mine dropping.
 * 
 * 295   1/10/97 11:30a Phillipd
 * movies are now doable
 * 
 * 294   12/27/96 3:38p Phillipd
 * Primary.h Secondary.h pickups.h are now clean....
 * Still Lots to do though.....
 * 
 * 293   12/27/96 12:34p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 292   12/23/96 6:04p Phillipd
 * 
 * 291   22-12-96 7:27p Collinsd
 * Increased model count.  Decreased scatter viewcone.  Lensflare now
 * works properly on scatter missiles.  Names are printed after the
 * endscene.
 * 
 * 290   20/12/96 15:19 Oliverc
 * Changed debug menu to be on SHIFT-F1
 * 
 * 289   19/12/96 16:16 Oliverc
 * Added debug options menu with "go to room" function
 * 
 * 288   12/19/96 2:52p Phillipd
 * new 512x384/640x400 font added
 * 
 * 287   12/19/96 11:47a Phillipd
 * 
 * 286   18-12-96 2:23p Collinsd
 * Put back in Enhanced crosshair
 * 
 * 285   12/18/96 12:41p Phillipd
 * Copyright screen added..
 * 
 * 284   12/17/96 5:28p Collinsd
 * Added team stuff.
 * 
 * 283   12/17/96 12:55p Phillipd
 * 
 * 282   12/17/96 9:20a Phillipd
 * 
 * 281   12/16/96 5:48p Collinsd
 * Uses various flags set in menus
 * 
 * 280   12/13/96 7:17p Collinsd
 * Added target for targeting computer. ( Disabled at the moment though ).
 * 
 * 279   13/12/96 10:56 Oliverc
 * Added in-game help screen
 * 
 * 278   12/12/96 16:23 Oliverc
 * Changed game key reading to use DirectInput
 * 
 * 277   12/12/96 4:16p Collinsd
 * Changed the way screen polys works.
 * 
 * 276   12/12/96 12:46p Phillipd
 * 
 * 275   12/12/96 10:56a Phillipd
 * 
 * 274   12/12/96 10:15a Phillipd
 * 
 * 273   12/10/96 2:06p Phillipd
 * 
 * 272   12/09/96 4:08p Phillipd
 * 
 * 271   12/08/96 2:30p Phillipd
 * 
 * 270   6/12/96 16:42 Oliverc
 * Fixed bug with group transition in portal/background collision
 * 
 * 269   12/06/96 4:05p Collinsd
 * fixed warning
 * 
 * 268   12/06/96 4:03p Collinsd
 * Changed timer
 * 
 * 267   12/06/96 4:03p Phillipd
 * 
 * 266   12/06/96 12:22p Phillipd
 * 
 * 265   12/06/96 9:13a Phillipd
 * More simplified panel stuff
 * 
 * 264   12/05/96 4:49p Collinsd
 * New fingerprint
 * 
 * 263   12/04/96 3:15p Phillipd
 * 
 * 262   12/04/96 2:21p Phillipd
 * 
 * 261   12/04/96 11:20a Phillipd
 * Simplified Panel Started
 * 
 * 260   12/03/96 5:00p Phillipd
 * Quting if not host bug fixed...
 * game doesnt start if ya just quit the title screen...
 * 
 * 259   12/03/96 10:38a Collinsd
 * Fixed dropped mine orientation
 * 
 * 258   12/02/96 4:26p Phillipd
 * 
 * 257   12/02/96 1:26p Collinsd
 * No longer use quatfromvector routines, now use quatfrom2vectors.
 * 
 * 256   2/12/96 12:02 Oliverc
 * Added code to detect invalid levels and fail gracefully
 * 
 * 255   2/12/96 11:19 Oliverc
 * 
 * 254   12/01/96 1:21p Phillipd
 * You can now quit a game go back to the title screen and start or join
 * another....
 * 
 * 253   11/29/96 3:10p Phillipd
 * Full screen from menu toggle...
 * Fixed the mouse lag problem again....
 * 
 * 252   11/28/96 4:26p Phillipd
 * 
 * 251   11/28/96 3:32p Phillipd
 * Better Multiplayer Starting...
 * 
 * 250   11/28/96 12:37p Phillipd
 * clear screen when starting multiplayer
 * 
 * 249   11/28/96 11:43a Collinsd
 * Taken out debug info on shields and hull
 * 
 * 248   28/11/96 11:42 Oliverc
 * 
 * 247   11/28/96 11:19a Collinsd
 * Added shield and hull sending across network
 * 
 * 246   11/28/96 10:00a Phillipd
 * Multiplayer join changed beyond all recognition,,,
 * 
 * 245   11/27/96 2:05p Phillipd
 * 
 * 244   11/27/96 11:05a Phillipd
 * Fixed Mouse speed on fast machines.....
 * 
 * 243   11/26/96 4:33p Phillipd
 * 
 * 242   11/25/96 5:14p Phillipd
 * 
 * 241   11/22/96 5:10p Phillipd
 * 
 * 240   11/22/96 11:17a Phillipd
 * New frequency dplay stuff added..only sends a packet every n frames
 * unless fireing..
 * 
 * 239   11/22/96 9:20a Phillipd
 * Put in c for asm flag and made a bit more watcom compatible
 * 
 * 238   11/21/96 2:37p Phillipd
 * 
 * 237   21/11/96 11:45 Collinsd
 * Added f6-f12 controlled by main menu dubug option.
 * 
 * 236   21/11/96 8:55 Collinsd
 * New NY-21-11-96 fingerprint
 * 
 * 235   11/19/96 9:32a Phillipd
 * 
 * 234   18/11/96 19:30 Oliverc
 * Changed menu toggle/back out key to ESC and stopped ESC from quitting
 * the program
 * 
 * 233   16/11/96 20:30 Collinsd
 * Added pickups per group variables/display.  Also fixed gravgon hitting
 * mine position problem.
 * 
 * 232   16/11/96 13:50 Oliverc
 * Made user-selected FOV compatible with "nitro-rush" effect
 * 
 * 231   15/11/96 18:03 Collinsd
 * Added all levels of detail for LAJ
 * 
 * 230   15/11/96 16:49 Collinsd
 * Mines now sent across when joining. Also fixed bug in pickup sending.
 * 
 * 229   11/15/96 3:40p Phillipd
 * 
 * 228   14/11/96 17:37 Oliverc
 * Added in-window in-game menus
 * 
 * 227   11/14/96 11:28a Phillipd
 * mines are displayed..fov grows if speed > max
 * 
 * 226   11/13/96 5:04p Phillipd
 * Changed blitting slightly...
 * Offscreen surface is now smaller...50%
 * 
 * 225   11/13/96 12:57p Phillipd
 * space now works when you change level...
 * and there is a bar for the nitro..
 * 
 * 223   11/13/96 9:08a Phillipd
 * All the Menus in the world....And then Some
 * 
 * 222   11/07/96 4:28p Phillipd
 * 
 * 221   11/07/96 4:12p Phillipd
 * 
 * 220   11/07/96 4:00p Phillipd
 * 
 * 219   11/07/96 2:33p Phillipd
 * Another bug...
 * 
 * 218   11/07/96 2:15p Phillipd
 * bug fixes....changing level...
 * 
 * 217   11/07/96 2:00p Phillipd
 * Change Level multiplayer bug fixed
 * 
 * 216   7/11/96 10:47 Collinsd
 * Shortened short ships structures.
 * 
 * 215   11/07/96 10:47a Phillipd
 * 
 * 214   7/11/96 10:13 Oliverc
 * Fixed viewport resizing
 * 
 * 213   11/05/96 5:26p Phillipd
 * 
 * 212   4/11/96 16:35 Collinsd
 * Fixed fmpolys/polys/lines exec lists.
 * 
 * 211   4/11/96 11:18 Oliverc
 * 
 * 210   4/11/96 10:45 Oliverc
 * Changed display routines to clip to visible portal boundaries of each
 * group
 * 
 * 209   10/31/96 4:31p Phillipd
 * light float to int in assembler....
 * 
 * 208   10/30/96 2:55p Phillipd
 * 
 * 207   29/10/96 16:00 Collinsd
 * changed over to panned sfx.
 * 
 * 206   10/29/96 2:49p Phillipd
 * panning sfx and new panel....with lights...
 * 
 * 205   10/28/96 8:50a Phillipd
 * 
 * 204   25/10/96 16:40 Collinsd
 * Added audio cd playing code.
 * 
 * 203   10/25/96 2:23p Phillipd
 * 
 * 202   10/25/96 2:07p Phillipd
 * 3d panel tested, test for lost packets
 * 
 * 201   10/24/96 3:01p Phillipd
 * 
 * 200   10/24/96 12:45p Phillipd
 * Added CanCullFlag for turning on/off cull mode so ya dont have to
 * process double sided stuff so much..
 * 
 * 199   24/10/96 11:47 Collinsd
 * Added total verts/poly for POLY's and added player
 * name display.
 * 
 * 198   10/23/96 4:24p Phillipd
 * Lots of crap taken out of D3dapp and its associated functions and
 * files....
 * 
 * 197   18/10/96 17:46 Collinsd
 * Changed all file loading to load from tidy directories.
 * 
 * 196   10/18/96 11:57a Phillipd
 * 
 * 195   10/17/96 4:43p Phillipd
 * proper score sort....dont work on stats yet...
 * 
 * 194   17/10/96 14:33 Oliverc
 * Fixed alpha blending function detection
 * 
 * 193   10/17/96 12:23p Phillipd
 * 
 * 192   10/17/96 10:37a Phillipd
 * 
 * 191   16/10/96 15:11 Oliverc
 * Player's control inputs now flushed between levels & lives
 * 
 * 190   10/16/96 3:11p Phillipd
 * 
 * 189   10/16/96 9:06a Phillipd
 * 
 * 188   15/10/96 15:30 Collinsd
 * Added missile/mine view toggled on/off with delete
 * 
 * 187   10/15/96 2:21p Phillipd
 * 
 * 186   14/10/96 9:48 Oliverc
 * Increased MAXLEVELS from 8 to 16
 * 
 * 185   10/10/96 4:37p Phillipd
 * 
 * 184   10/09/96 2:33p Phillipd
 * 
 * 183   8/10/96 20:26 Collinsd
 * fixed pickup transfer for new player.
 * 
 * 182   10/08/96 4:43p Phillipd
 * 
 * 181   10/08/96 10:43a Phillipd
 * 
 * 180   10/08/96 10:24a Phillipd
 * 
 * 179   8/10/96 9:24 Collinsd
 * added loading of pickups when changing level.
 * 
 * 178   10/08/96 8:59a Phillipd
 * 
 * 177   10/07/96 11:41a Phillipd
 * 
 * 176   10/07/96 11:29a Phillipd
 * 
 * 175   10/06/96 5:04p Phillipd
 * We now have our own text debug print info thing...
 * which speeds up the game by 10%.....
 * 
 * 174   5/10/96 21:34 Collinsd
 * Added drop pickup mode. speeded up pickups even more now 0.9% of
 * processing
 * 
 * 173   10/05/96 7:00p Phillipd
 * Got rid of the extents management from RenderLoop()
 * Removed clearing back buffers from d3dmain.cpp..
 * now done every time a camera view needs to do it..
 * Removed load of / 2.0f from quat.c and made some of the functions
 * __inline
 * 
 * 172   10/05/96 3:12p Phillipd
 * 
 * 171   10/05/96 2:02p Phillipd
 * 
 * 170   10/03/96 3:06p Phillipd
 * 
 * 169   3/10/96 14:04 Collinsd
 * Added nitro limit.
 * 
 * 168   10/03/96 11:56a Phillipd
 * 
 * 167   10/03/96 9:14a Phillipd
 * 
 * 166   2/10/96 12:21 Collinsd
 * Now display pickup i want to drop in full screen as well.
 * 
 * 165   10/02/96 10:36a Phillipd
 * 
 * 164   10/02/96 10:35a Phillipd
 * 
 * 163   1/10/96 17:43 Collinsd
 * Reduced pulsar to half.  Tidied up primary weapons.
 * deleted redundant code in 2dpolys/primary weapons.
 * 
 * 162   10/01/96 4:58p Phillipd
 * added more panel position correct stuff..
 * 
 * 161   10/01/96 10:34a Phillipd
 * 
 * 160   29/09/96 18:47 Collinsd
 * Added turbo to ship structure. Fixed pine mine firing pos.
 * 
 * 159   29/09/96 16:02 Collinsd
 * Lensflare effect in ( NO CLIPPING YET )
 * 
 * 158   9/27/96 10:26a Phillipd
 * 
 * 157   9/27/96 8:54a Phillipd
 * put alpha blit test in Testblt....Doesnt work on our hardware
 * 
 * 156   9/26/96 5:03p Phillipd
 * New Font Sizes..
 * 
 * 155   25/09/96 11:11 Collinsd
 * 
 * 154   23/09/96 14:20 Collinsd
 * Started spider mine.
 * 
 * 153   23/09/96 12:32 Collinsd
 * Random  rotations for pickups
 * 
 * 152   9/23/96 11:49a Phillipd
 * Added Level Select to the Menus..
 * 
 * 151   23/09/96 10:22 Oliverc
 * Set NumLevels = number of levels in levels.dat (up to a maximum of
 * MAXLEVELS)
 * 
 * 150   23/09/96 10:17 Oliverc
 * Level names now read in from levels.dat at startup
 * 
 * 149   20/09/96 17:13 Collinsd
 * Changed 2Dpolys and Polys to display only polys in visible groups.
 * Changed secondary ammo to use array.
 * Pickups now use correct index's.  You can now drop a mine using 'B'.
 * 
 * 148   9/20/96 3:16p Phillipd
 * 
 * 147   9/19/96 2:33p Phillipd
 * 
 * 146   19/09/96 9:20 Oliverc
 * Converted mouse input to use DirectInput instead of dmouse.vxd
 * 
 * 145   9/18/96 5:35p Phillipd
 * 
 * 144   9/18/96 3:59p Phillipd
 * 
 * 143   9/17/96 3:28p Phillipd
 * new 320x400 panel..
 * 
 * 142   17/09/96 14:13 Oliverc
 * Changed FOV setting to take current screen mode (and thus pixel aspect
 * ratio) into account so odd sizes (eg 320x400) should now work without
 * distortion
 * 
 * 141   17/09/96 13:58 Collinsd
 * Added autoselection of primary and secondary weapons.
 * Also you can pickup anything if F12 enabled.
 * 
 * 140   9/17/96 11:35a Phillipd
 * 
 * 139   9/17/96 11:26a Phillipd
 * 
 * 138   17/09/96 11:23 Oliverc
 * Made aztec level the default and removed plain level
 * 
 * 137   9/16/96 3:10p Phillipd
 * 
 * 136   16/09/96 15:05 Collinsd
 * 
 * 135   9/16/96 10:16a Phillipd
 * Big changes to Full screen display...
 * And Pickups being set to joining player
 * 
 * 134   15/09/96 18:14 Collinsd
 * Changed scatter weapons, now scatters even more. (ooooooohhhhh!)
 * 
 * 133   9/15/96 3:43p Phillipd
 * 
 * 132   15/09/96 14:48 Collinsd
 * Scaled primary weapons, changed defauls ammo levels.
 * 
 * 131   13/09/96 17:04 Collinsd
 * Added quantun fireball mine.
 * 
 * 130   9/13/96 4:46p Phillipd
 * 
 * 129   11/09/96 14:19 Collinsd
 * Added global scale to hopefully everything that needs it.
 * 
 * 128   9/11/96 10:25a Phillipd
 * 
 * 127   7/09/96 20:21 Collinsd
 * Collision with pine mine now added.  Trojax power
 * fixed, now does proper damage.  Transpulse speeded
 * up and made more lethal.
 * 
 * 126   9/06/96 12:56p Phillipd
 * 
 * 125   9/05/96 4:47p Phillipd
 * 
 * 124   5/09/96 10:45 Collinsd
 * Fixed next prim/sec weapons selection. 
 * 
 * 123   5/09/96 9:29 Oliverc
 * Added "toggle ship-to-background collisions" key F12
 * 
 * 122   9/04/96 5:54p Phillipd
 * 
 * 121   9/03/96 8:57a Phillipd
 * 
 * 120   9/02/96 2:39p Phillipd
 * 
 * 119   9/02/96 2:19p Phillipd
 * 
 * 118   8/30/96 2:32p Phillipd
 * 
 * 117   8/29/96 5:52p Phillipd
 * 
 * 116   28/08/96 11:55 Collinsd
 * Added show skin code + more options for show visi.
 * 
 * 115   8/28/96 11:13a Phillipd
 * Game now works under a GameStatus flag...
 * So you can view scores load a new level and restart..
 * 
 * 114   8/23/96 2:41p Phillipd
 * 
 * 113   23/08/96 9:18 Collinsd
 * All weapons have limits. New pickups in but not correct.
 * 
 * 112   21/08/96 15:26 Collinsd
 * Changed the way the timer works.....
 * 
 * 111   21/08/96 11:25 Collinsd
 * Added saving and loading of pickups positions.
 * 
 * 110   21/08/96 8:46 Collinsd
 * Added descent level, and powerlevel bar for trojax.
 * 
 * 109   8/21/96 8:44a Phillipd
 * 
 * 108   8/20/96 12:06p Phillipd
 * lots of new panel / text stuff...new stats array...
 * 
 * 107   8/19/96 8:51a Phillipd
 * 
 * 106   8/16/96 5:53p Phillipd
 * 
 * 105   8/16/96 4:15p Phillipd
 * 
 * 104   8/16/96 2:43p Phillipd
 * 
 * 103   8/16/96 11:04a Phillipd
 * 
 * 102   16/08/96 11:01 Collinsd
 * You now see explosions close to walls and when hit. Pickups and bullets
 * don't dissapear when changing mode.
 * 
 * 101   8/15/96 4:46p Phillipd
 * 
 * 100   15/08/96 9:54 Collinsd
 * Added glow to pickups and started primary wepon powerups
 * 
 * 99    8/14/96 5:48p Phillipd
 * 
 * 98    8/14/96 4:30p Phillipd
 * 
 * 97    8/14/96 9:41a Phillipd
 * 
 * 96    14/08/96 9:27 Collinsd
 * 
 * 95    13/08/96 15:52 Collinsd
 * Added powerup pods
 * 
 * 94    13/08/96 14:57 Collinsd
 * Now prints strings to tell you what you've  picked up.
 * 
 * 93    8/13/96 12:50p Phillipd
 * 
 * 92    8/13/96 10:15a Phillipd
 * Player name and score now given to everyone for printing....4x5 print
 * routine and uint16 printing
 * 
 * 91    8/11/96 5:35p Phillipd
 * 
 * 90    8/09/96 2:02p Phillipd
 * 
 * 89    8/08/96 9:18 Oliverc
 * 
 * 88    8/07/96 10:49a Phillipd
 * 
 * 87    8/06/96 3:19p Phillipd
 * different panels for each resolotion...
 * 
 * 86    8/06/96 3:10p Phillipd
 * 
 * 85    5/08/96 20:04 Collinsd
 * Network for pickups now works,
 * 
 * 84    8/05/96 5:53p Phillipd
 * 
 * 83    5/08/96 9:06 Collinsd
 * 
 * 82    4/08/96 17:04 Collinsd
 * All  primary weapons are now in. ( no power ups )
 * 
 * 81    2/08/96 18:59 Collinsd
 * Added lines into game.
 * 
 * 80    2/08/96 15:29 Collinsd
 * Added pickups
 * 
 * 79    8/02/96 3:26p Phillipd
 * 
 * 78    7/31/96 4:05p Phillipd
 * 
 * 77    7/30/96 5:45p Phillipd
 * 
 * 76    7/29/96 5:24p Phillipd
 * 
 * 75    7/29/96 4:43p Phillipd
 * 
 * 74    7/29/96 12:16p Phillipd
 * 
 * 73    7/26/96 4:27p Phillipd
 * 
 * 72    7/25/96 5:44p Phillipd
 * 
 * 71    7/25/96 5:38p Phillipd
 * 
 * 70    24/07/96 15:05 Oliverc
 * 
 * 69    7/24/96 2:42p Phillipd
 * 
 * 68    7/24/96 9:52a Phillipd
 * 
 * 67    23/07/96 18:01 Collinsd
 * Added visipoly line mode and group in mode.
 * 
 * 66    7/23/96 4:50p Phillipd
 * 
 * 65    7/23/96 4:10p Phillipd
 * 
 * 64    7/23/96 3:37p Phillipd
 * 
 * 63    23/07/96 15:26 Collinsd
 * Added new offset file loading/format.
 * 
 * 62    23/07/96 14:50 Oliverc
 * 
 * 61    7/23/96 9:41a Phillipd
 * 
 * 60    7/22/96 5:49p Phillipd
 * 
 * 59    7/22/96 3:51p Phillipd
 * 
 * 58    22/07/96 14:47 Collinsd
 * 
 * 57    22/07/96 14:08 Collinsd
 * Added visipoly display.
 * 
 * 56    7/22/96 11:16a Phillipd
 * 
 * 55    7/21/96 4:27p Phillipd
 * added asynchrinus(??) execution ..so lights happen at the same time as
 * the last group is being displayed...
 * 
 * 54    7/21/96 2:36p Phillipd
 * 
 * 53    7/21/96 2:29p Phillipd
 * 
 * 52    21/07/96 12:52 Collinsd
 * Now all works with Dan's new tloader
 * 
 * 51    21/07/96 12:28 Collinsd
 * Primary Weapon Selection added.  Missiles now have multiple fireing
 * methods/directions.  Now uses tload.
 * 
 * 50    7/21/96 12:25p Phillipd
 * 
 * 49    19/07/96 16:56 Collinsd
 * Now have seperate explosion for Ship and Background.
 * Increased Max 2DPolys, and have framelag compensated fireing of the
 * primary and secondary weapons.
 * 
 * 48    19/07/96 12:36 Collinsd
 * New Explosions and better smoke trails.
 * 
 * 47    19/07/96 12:34 Oliverc
 * Changed ship <-> background collision routine
 * to track movement of ship through portals
 * 
 * 46    7/17/96 5:46p Phillipd
 * 
 * 45    7/16/96 11:11a Phillipd
 * moved all visipoly stuff to visi.c and visi.h..
 * 
 * 44    15/07/96 14:30 Collinsd
 * Transparency & Translucency work (With hardware)
 * 
 * 43    11/07/96 17:47 Oliverc
 * 
 * 42    11/07/96 17:35 Oliverc
 * 
 * 41    7/11/96 4:27p Phillipd
 * 
 * 40    7/11/96 2:55p Phillipd
 * added D3DAppFreeVideoMemory...returns total free..
 * 
 * 39    11/07/96 12:50 Collinsd
 * Drop missiles started
 * 
 * 38    7/10/96 5:10p Phillipd
 * 
 * 37    7/09/96 12:55p Phillipd
 * 
 * 36    7/09/96 12:52p Phillipd
 * 
 * 35    7/08/96 5:35p Phillipd
 * 
 * 34    7/08/96 10:20a Phillipd
 * 
 * 33    6/07/96 16:24 Oliverc
 * 
 * 32    7/06/96 1:40p Phillipd
 * 
 * 31    7/06/96 1:08p Phillipd
 * 
 * 30    5/07/96 17:59 Collinsd
 * Faceme poly's now use boxtoppm data
 * 
 * 29    7/05/96 5:44p Phillipd
 * 
 * 28    4/07/96 16:54 Collinsd
 * 
 * 27    4/07/96 16:09 Collinsd
 * 
 * 26    4/07/96 11:57 Oliverc
 * 
 * 25    4/07/96 10:34 Collinsd
 * 
 * 24    3/07/96 10:45 Oliverc
 * 
 * 23    2/07/96 17:57 Oliverc
 * 
 * 22    2/07/96 16:50 Oliverc
 * 
 * 21    2/07/96 15:45 Oliverc
 * 
 * 20    2/07/96 15:20 Collinsd
 * 
 * 19    28/06/96 17:35 Collinsd
 * 
 * 18    28/06/96 14:10 Oliverc
 * 
 * 17    28/06/96 12:48 Collinsd
 * 
 * 16    28/06/96 10:05 Collinsd
 * 
 * 15    6/27/96 3:13p Phillipd
 * 
 * 14    6/27/96 12:29p Phillipd
 * 
 * 13    6/27/96 9:40a Phillipd
 * Major rearange of ships and oct2....
 * 
 * 12    26/06/96 17:23 Oliverc
 * 
 * 11    6/26/96 12:42p Phillipd
 * 
 * 10    25/06/96 16:10 Collinsd
 * 
 * 9     25/06/96 14:41 Collinsd
 * 
 * 8     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

 /*==========================================================================
 *
 *  Copyright (C) 1995 Microsoft Corporation. All Rights Reserved.
 *
 *  File: oct2.c
 *
 ***************************************************************************/

//#define REFLECTION

#define INSIDE_BSP // disable to use bounding box inside check instead
#define BSP_ONLY

//#define LOBBY_DEBUG

#ifdef LOBBY_DEBUG
#include <direct.h>
#include <stdlib.h>
#endif

#include <stdio.h>
#include "main.h"
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "Node.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
#include "ddsurfhand.h"
#include "getdxver.h"



#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "d3dmain.h"

#include "2dtextures.h"
#include "mload.h"

#include "primary.h"
#include "secondary.h"
#include "triggers.h"
#include "pickups.h"

#include "Ships.h"
#include "exechand.h"
#include "collision.h"
#include "ddutil.h"
#include "2dpolys.h"
#include "lines.h"
#include "polys.h"
#include "lights.h"
#include "models.h"
#include "2dtextures.h"
#include "visi.h"
#include "tload.h"
#include "sfx.h"
#include "transexe.h"
#include "Text.h"
#include "Mxaload.h"
#include "dinput.h"
#include "camera.h"
#include "title.h"
#include "screenpolys.h"
#include "controls.h"
#include "comm.h"
#include "triggers.h"
#include "enemies.h"

#include "tasker.h"
#include "bsp.h"
#include "trigarea.h"
#include "multiplayer.h"

#include  <fcntl.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <malloc.h>
#include  <string.h>
#include  "file.h"
#include  "pvrd3d.h"

#include  "PolySort.h"

#include  "Ai.h"
#include  "Water.h"
#include  "spotfx.h"

#include <process.h>
#include  "ExtForce.h"
#include  "Teleport.h"

#include  "rtlight.h"
#include  "avi.h"
#include  "restart.h"


#include "registry.h"
#include "Local.h"

#include  "force.h"
#include  "feedback.h"
#include  "goal.h"
#include  "splash.h"
#include  "LoadSave.h"
#include  "Credits.h"
#include  "XMem.h"
#include "dpthread.h"

#ifdef SHADOWTEST
#include "triangles.h"
#include "shadows.h"
#endif


extern BOOL Bsp_Duplicate( BSP_HEADER *src, BSP_HEADER *dup );
extern BOOL Bsp_Identical( BSP_HEADER *b1, BSP_HEADER *b2 );
BSP_HEADER Bsp_Original;


extern BOOL OKToJoinSession;
extern DPID PseudoHostDPID;

extern int    BountyInLevel;
extern int    FlagsInLevel;
extern int    TeamFlagsInLevel[ MAX_TEAMS ];
extern int    TeamFlagsInShips[ MAX_TEAMS ];

extern BOOL NeedFlagAtHome;
extern BOOL OwnFlagTeleportsHome;
extern BOOL CanCarryOwnFlag;
extern LONGLONG LevelCheckSum;

BOOL OriginalLevels = FALSE;
BOOL CheatsDisabled = FALSE;
BOOL WaitingToQuit;

void ConfigureSpaceorbAxis( int joystick );
void DefaultJoystickSettings( USERCONFIG *u );

int folder_exists( char *pathspec, ... );

extern const char last_compiled[];
extern  float Countdown_Float;

#ifdef MANUAL_SESSIONDESC_PROPAGATE
extern LPDPSESSIONDESC2                    glpdpSD_copy;
#endif
extern  uint32 BigPacketSize;
extern  uint32 MaxBigPacketSize;
extern  uint32 RecPacketSize;
extern  uint32 MaxRecPacketSize;

extern  uint32 CurrentBytesPerSecRec;
extern  uint32 CurrentBytesPerSecSent;
extern  uint32 MaxCurrentBytesPerSecRec;
extern  uint32 MaxCurrentBytesPerSecSent;


extern char *JoystickPOVDirections[];
extern VECTOR BikePos;

extern BOOL CTF;

extern BOOL IPAddressExists;
extern uint8 QuickStart;

extern char *TeamName[ MAX_TEAMS ];
extern BOOL IsLobbyLaunched;
extern int16 DummyTextureIndex;

extern void Printint16( int16 num , int x , int y , int col );

extern void InitIndirectVisible( uint16 g );

extern  BOOL IsServerGame;

extern void AddIndirectVisible( uint16 g );
extern  BOOL  RearCameraDisable;
extern  MODELNAME   SplashModelNames[MAXMODELHEADERS];
extern USERCONFIG *player_config;
extern char biker_config[];

/********************
splash externs
*******************/
extern NEWSPLASHSCREENS NewSplashScreens[];
extern int NewCurrentSplashScreen;
extern DWORD SplashFinishTime;
extern DWORD SplashStartTime;
extern BYTE PreSplash_MyGameStatus;
extern MENU *PreSplash_Menu;
extern MENUITEM *PreSplash_MenuItem;
extern LPDIRECTDRAWSURFACE lpDDS_NewSplash;

extern BOOL LimitedLoad;
extern BOOL InSplashDemo;

extern MODELNAME *TitleModelSet;
extern float LevelTimeTaken;

extern BYTE PreDemoEndMyGameStatus;
extern CRITICAL_SECTION CompoundSfxKey;
extern CRITICAL_SECTION SfxHolderKey; 
extern int NumDupCompoundBuffers;
extern COMPOUND_SFX_INFO CompoundSfxBuffer[MAX_COMPOUND_BUFFERS];
extern  BOOL  RecordDemoToRam;
extern  TEXT  DemoGameName;

extern  BOOL FullRearView;

extern  int AckMsgsActiveMax;
extern  int AckMsgsActive;

extern MENU MENU_Controls;
extern MENU MENU_Options;
extern MENU MENU_LoadSavedGame;
extern MENU MENU_SaveGame;
extern MENU MENU_NEW_BetweenLevels;
extern MENU MENU_NEW_BetweenLevels;
extern MENU MENU_NEW_StartAttractMode;
extern MENU MENU_NEW_NumberOfCrystals;
extern MENU MENU_NEW_WatchTeamSelect;

extern float VDUoffsetX;
extern float VDUoffsetY;
extern  int16 NextworkOldKills;
extern  int16 NextworkOldDeaths;
extern  int16 NextworkOldBikeNum;
extern  BOOL  DS;

extern  size_t  MemUsed;
extern  size_t  ExecMemUsed;

#ifdef DEBUG_ON
extern size_t SBufferMemUsedSW;
extern size_t SBufferMemUsedHW;
extern size_t MaxSBufferMemUsedHW;
extern size_t MaxSBufferMemUsedSW;
extern uint32 CurrentBikeCompSpeech;
#endif

BOOL PreventFlips = FALSE;
BOOL Cheated = FALSE;

BOOL ServerRendering = TRUE;
BOOL ServerCollisions = TRUE;

uint16 Current_Max_Score;

void InitSoundInfo( MLOADHEADER * Mloadheader );
void InitShipSpeeds( void );

#define DEFAULT_SCREEN_ASPECT_RATIO ( 4.0F / 3.0F )

#define SAVESCREEN_3DFX

uint32        AnimOncePerFrame = 0;         // used for stuff that is displayed more than once in a single frame..
int CurrentLoadingStep = 0;

extern BOOL bSoundEnabled;

extern LPDIRECTDRAWSURFACE    Lp_AVI_DDSurface;

extern TeamCol[];
extern int Num_StatsMessage_Parts;
extern LIST BikeList;
extern LIST LevelList;
extern DWORD BufferedKey[];

extern  BOOL ResetKillsPerLevel;

extern BOOL DebugVisible;

extern int  outside_map;

extern  int16 NextNewModel;

BOOL  bPolyText = FALSE;
BOOL  PolygonText = FALSE;
extern  BOOL  PolyText[255];
extern MENUSTATE MenuState;
extern JOYSTICKINFO JoystickInfo[MAX_JOYSTICKS]; 
extern  char * LogFilename;
extern  char * BatchFilename;
extern  BOOL  ShowEFZones;
extern  BOOL  ShowTeleports;
extern BOOL MenuFrozen;
extern  int16 MaxKills;
extern SPOT_SFX_LIST LoopingSfxListStart[];
extern  int16 NumGoldBars;

extern CRITICAL_SECTION SfxKey;
extern BOOL ReloadSfx;
extern float ReloadSfxPause;
extern  BOOL  IllegalTime;
extern  int   SinglePlayerTimeLimit;
extern  BOOL  BilinearSolidScrPolys;
extern  uint16  RandomStartPosModify;             
extern  int BigPacketsSent;
extern  LONGLONG  LastPacketTime[MAX_PLAYERS+1];
extern  char * StatusTab[256];

BYTE  PreSynchupStatus;
char *CurrentLevelsList;

float Old_LevelTime_Float;
void InitFontTransTable( BOOL BlitText );

#ifdef SOFTWARE_ENABLE
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
    Chris's Code
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void  CWExecute2( LPDIRECT3DDEVICE lpDev,
          LPDIRECT3DEXECUTEBUFFER execbuf,
          LPDIRECT3DVIEWPORT lpView,
          WORD cwFlags);
void  SWRendView( void );
void  SetVTables( void );
void CWClearZBuffer( void );
BOOL CWRenderSmallCamera( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );

extern  BOOL  SoftwareVersion;
extern  long  RenderingSmall;
extern  long  RenderingSmallXOff;
extern  long  RenderingSmallYOff;
extern  long  zRW;
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#endif

void InitModeCase(void);
BOOL  IsEqualGuid(GUID *lpguid1, GUID *lpguid2);
void InitPolyText( void );


// Ai Modes
char * AiModes[] = {
  "AIMODE_PLAYER",          // Controlled by PLAYER
  "AIMODE_FOLLOWPATH",        // Follow node path
  "AIMODE_IDLE",          // Do nothing for a while
  "AIMODE_SCAN",          // Look around for a target
  "AIMODE_MOVETOTARGET",      // Move towards selected target   
  "AIMODE_FIRE",      // Move towards selected target   
  "AIMODE_DOGFIGHT",        // Dogfight with the selected target   
  "AIMODE_KILLMINE",        // Attack And kill a Mine....
  "AIMODE_RETREAT",         // Try and get away from PLAYER
  "AIMODE_FORMATION",       // Stay in Formation and attack whan necessary...

  "AIMODE_DEATH_CRASHNBURN",    // Fall to the ground and Shatter...
  "AIMODE_DEATH_PINGOFF",     // Ping off in the dir of the bullet that killed me spinning as I go...
  "AIMODE_DEATH_PSYCHO",      // Twist and turn and shoot and explode...
  "",
  "",
  "",
};
extern  int EnemiesActive;

// Bomb Stuff
void  InitBombs( void );
BOOL  BombTag = FALSE;//TRUE;
float NewBombTime = ( 100.0F * 30.0F ); // approx 30 seconds...
int   LowestBombTime = -1;
int   OldLowestBombTime = -1;
int   LastPersonISentABomb = 0;
int   NumOfBombToStart = 1;
BOOL  BombActive[MAXBOMBS];
float BombTime[MAXBOMBS];
extern  int BombNumToSend;
extern  float BombTimeToSend;
void PassOnNewBomb( int i );
void UpdateBombs( void );
// End of Bomb Stuff
void CheckTimeLimit( void );

extern  int16 InGameLoadGameLevelNum;
#ifdef REFLECTION
void WierdShit( void );
#endif


BOOL  IMustQuit = FALSE;

// Capture The Flag Stuff
BOOL  CaptureTheFlag = FALSE;
BOOL  IHaveAFlag = FALSE;

// End of Capture The Flag


// Bounty Hunt
BOOL  BountyHunt = FALSE;

static char fname[256];
static int fnum = 0;
extern int CameraStatus;  


char *  InitViewMessages[] = {
                 "Loading... 1 of 10" ,
                 "Loading... 2 of 10" ,
                 "Loading... 3 of 10" ,
                 "Loading... 4 of 10" ,
                 "Loading... 5 of 10" ,
                 "Loading... 6 of 10" ,
                 "Loading... 7 of 10" ,
                 "Loading... 8 of 10" ,
                 "Loading... 9 of 10" ,
                 "Loading... 10 of 10" ,
                 /*
                 "Loading OffScreen Surfaces" ,
                 "Texture Load Prep         " ,
                 "Loading Textures          " ,
                 "Loading Models            " ,
                 "Loading World Mesh        " ,
                 "Loading Collision Skins   " ,
                 "Init Sound Info           " ,
                 "Init Ambient Lighting     " ,
                 "Loading Sfx               " ,
                 "Everything Else...Nodes   " ,
                 */
                 "" 
};

extern  float MaxMoveSpeed;
extern  float MoveAccell;
extern  float MoveDecell;
extern  float MaxTurboSpeed;
extern  float TurboAccell;
extern  float TurboDecell;
extern  float MaxTurnSpeed;
extern  float TurnAccell;
extern  float TurnDecell;
extern  float MaxRollSpeed;
extern  float RollAccell;
extern  float RollDecell;
extern  float MaxBankAngle;
extern  float BankAccell;
extern  float BankDecell;

extern BOOL BrightShips;

extern  float LastDistance[MAX_SFX];

BOOL  Is3Dfx;
BOOL  Is3Dfx2;

BOOL  IsPowerVRD3DDevice(void);
BOOL  PowerVR = FALSE;
BOOL  PowerVR_Overide = FALSE;
BOOL  bPolySort = FALSE;

void  Test( void )
{
  while( 1 )
  {
    TaskSleep(1);
  }
}
extern int GameCompleted;
extern  OFF_FILES OffsetFiles[];
extern  OFF_FILES Title_OffsetFiles[];
extern  OFF_FILES Splash_OffsetFiles[];

extern  int   DifficultyLevel;

extern int outside_group;

extern  BOOL  ScreenSaving;
extern  float WhiteOut;
extern float mouse_dx;
extern float mouse_dy;
BOOL  InsideResizeViewport = FALSE;

DWORD CurrentSrcBlend;
DWORD CurrentDestBlend;
DWORD CurrentTextureBlend;
 
LONGLONG  GameStartedTime;    // when the game started
LONGLONG  GameElapsedTime;    // Real how long the game has been going in game time not real..
LONGLONG  TempGameElapsedTime;  // Real how long the game has been going in game time not real..
LONGLONG  GameCurrentTime;    // How long the game has been going...
LONGLONG  LargeTime;
LONGLONG  LastTime;
LONGLONG  TimeDiff;
LONGLONG  Freq;
BOOL  JustExitedMenu =FALSE;

BOOL  Inside;
int   Depth;

void CalculateFramelag( void );
void SetGamePrefs( void );

MENU  MENU_EditMacro1;
MENU  MENU_EditMacro2;
MENU  MENU_EditMacro3;

extern  BOOL  ServerMode;
int   ServerCount = 0;

extern  DPID                    dcoID;

extern  FILE  * DemoFp;
extern  FILE  * DemoFpClean;

extern  BOOL  PlayDemo;
extern  BOOL  PauseDemo;
extern  BOOL  RecordDemo;
extern  SLIDER  DemoSpeed;
extern  SLIDER  DemoEyesSelect;
extern  BOOL  DemoScreenGrab;
extern  BOOL  ShowWeaponKills;

LPDIRECT3DEXECUTEBUFFER lpD3DNormCmdBuf;
LPDIRECT3DEXECUTEBUFFER lpD3DTransCmdBuf;
LPDIRECT3DEXECUTEBUFFER lpD3DSpcFxTransCmdBuf;
BOOL InitSpecialExecBufs( void );
void ReleaseSpecialExecBufs( void );

extern  BOOL  flush_input; // tells control routines to ignore next player input

extern  uint16  IsGroupVisible[MAXGROUPS];

extern  MXLOADHEADER ModelHeaders[MAXMODELHEADERS];
extern  MODELNAME TitleModelNames[MAXMODELHEADERS]; 

extern  char  TitleNames[8][64];  

extern  ENEMY * FirstEnemyUsed;

extern  CAMERA  CurrentCamera;
extern  CAMERA  MainCamera;     // the main viewing screen...

float FPS = 0.0F;         // Frames Per Second...
double  TPS = 0.0;          // Textures Per Second...     
int Our_TrianglesDrawn = 0; // Textures Per Second...     

#define MYTIMER
#undef MYTIMER

#define LEVELSFILE      "levels.dat"

extern  MENU  *       GetPlayerNumMenu;


#define FOV_GROW(A)     ((A) + 10.0F)
#define FOV_SHRINK(A)   ((A) - 10.0F)

LONGLONG  DemoStartedTime;    // when the game started
LONGLONG  DemoEndedTime;      // when the game started
float   DemoTotalTime = 0.0F; // total game time (in seconds)
int32   DemoGameLoops = 0;
float DemoAvgFps = 0.0F;

#define MIN_VIEWPORT_WIDTH  (64)
#define MIN_VIEWPORT_HEIGHT (64)

#define VIEWPORT_GROW   (1.1F)
#define VIEWPORT_SHRINK   (1.0F/1.1F)

extern  TLOADHEADER Tloadheader;
extern  int16   SecondaryAmmo[ MAXSECONDARYWEAPONS ];
BOOL LockOutWindows = FALSE;

extern uint16 OnceOnly;
extern char         MyName[ 32 ];
extern  BOOL                    IsHost;
extern  BOOL                    IsServer;
extern float  MessageTime[MAX_MESSAGES];
extern  char MessageBank[MAX_MESSAGES][200];
extern  int CurrentMessage;
extern  BYTE          MyGameStatus;
BYTE PreWaitingToSendMessagesStatus;
extern  BYTE          GameStatus[]; // Game Status for every Ship...
extern  BYTE          OverallGameStatus;      // what the host says he is doing...
extern  LPDIRECT3DEXECUTEBUFFER Skin_Execs[ MAXGROUPS ];
extern  LPDIRECT3DEXECUTEBUFFER Portal_Execs[ MAXGROUPS ];
extern  float PyroliteAmmo;
extern  float SussGunAmmo;
extern  float GeneralAmmo;
extern  SHIPCONTROL control;
extern  REGENPOINT  * RegenPoints;
extern  int16     NumRegenPoints;
extern  float PowerLevel;
extern  float LaserTemperature;
extern  float NitroFuel;
extern  SECONDARYWEAPONBULLET SecBulls[MAXSECONDARYWEAPONBULLETS];
extern  ENEMY * TestEnemy;

extern  DWORD                   Old_WhoIAm;
extern  DWORD         Old_Kills;
extern  DWORD         Old_Deaths;
extern  DWORD         Old_TeamNumber;
extern  char          Old_Name[256];
extern  BOOL          Rejoining;
extern  char  biker_name[256];
extern  int16 SelectedBike;
extern  DPSESSIONDESC2      Old_Session;
extern  LPDPSESSIONDESC2                    glpdpSD;
extern  LPDIRECTPLAY4A                       glpDP;     // directplay object pointer
extern  BOOL  AutoDetail;
extern  LONGLONG  DemoTimeSoFar;

extern  float cral;
extern  int   HullHit;
extern  int   ShieldHit;
extern  BOOL  TargetComputerOn;

extern  int ScoreSortTab[MAX_PLAYERS];
extern  int16 NumPickupsPerGroup[ MAXGROUPS ];
extern  PICKUP  Pickups[ MAXPICKUPS ];

extern  BOOL  DebugInfo;
extern  BOOL  GodMode;
extern  BOOL  GodModeOnceOnly;
extern  BOOL  EnhancedXHair;

extern  BOOL  TeamGame;
extern  BYTE  TeamNumber[MAX_PLAYERS];

extern  int   no_collision;   // disables player ship-to-background collisions
extern  SLIDER  TimeLimit;
extern  BOOL  CountDownOn;
extern  REGENPOINT  * RegenSlotsCopy[ MAX_PLAYERS ];

extern  BOOL  ShowTrigZones;
extern  BOOL  ShowColZones;
extern  BOOL  NodeCube;
extern  BOOL  OldNodeCube;
extern  MATRIX  MATRIX_Identity;

extern  uint32  TotalFmPolysInUse;
extern  uint32  TotalPolysInUse;
extern  uint32  TotalScrPolysInUse;
extern  MODELNAME ModelNames[MAXMODELHEADERS];
extern  BOOL  quitting; 
extern  int16 MakeColourMode;
extern  BOOL  ShowBoundingBoxes;

BOOL InitViewport( float scale );


BYTE  InitView_MyGameStatus;
BYTE  ChangeLevel_MyGameStatus;

LPDIRECTDRAWPALETTE ddpal;

void ProcessGameKeys( void );


float GetPlayerNumCount1 = 0.0F;
float GetPlayerNumCount2 = 0.0F;
int   GetPlayerNumCount = 0;

int RearCameraActive = 0;
int MissileCameraActive = 0;
uint16  CameraMissile = (uint16) -1;
int MissileCameraEnable = 0;
BYTE  TempMissileCam;

extern  uint16    PrimaryStats[TOTALPRIMARYWEAPONS*2];
extern  uint16    SecondaryStats[TOTALSECONDARYWEAPONS*2];

extern  REMOTECAMERA * ActiveRemoteCamera;

BOOL
Our_CalculateFrameRate(void);
BOOL VduFinished( MENU *Menu );
BOOL WriteMessage(const char *format, ...); // printf-a-like for bottom line of window

BOOL ScoreDisplay();
BOOL FreeScoreDisplay();
BOOL InitScoreDisplay();
BOOL StatsDisplay();
BOOL FreeStatsDisplay();
BOOL InitStatsDisplay();

int Secrets = 0;
int TotalSecrets = 0;
int CrystalsFound = 0;
extern  int16   NumInitEnemies;
extern  int16   NumKilledEnemies;

BOOL  ResizeViewport( float scale );
float CurrentViewportScale;
BOOL  FullScreenViewport();

BOOL InitDInput(void);
BOOL TermDInput( void );
BOOL  ClearBuffers( BOOL ClearScreen, BOOL ClearZBuffer );
BOOL  ClearZBuffer();

BOOL  RenderCurrentCamera( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );
void DrawLoadingBox( int current_loading_step, int current_substep, int total_substeps );
void FreeSfxHolder( int index ) ;
void ShowSplashScreen( int num );
void InitValidPickups();

void  PlotSimplePanel( void );

BOOL cursorclipped = FALSE;
RECT cursorclip;

LPDIRECT3DDEVICE lpD3Ddev = NULL;
D3DVIEWPORT viewport;
D3DVIEWPORT oldviewport;
HRESULT hresult;
int initfov = 0;
float viewplane_distance;
float hfov = START_FOV;
float chosen_fov = START_FOV;
float normal_fov = START_FOV;
float screen_aspect_ratio = DEFAULT_SCREEN_ASPECT_RATIO;
uint16 PanelHeight = 48;

BOOL  DrawPanel = FALSE;
BOOL  DrawSimplePanel = TRUE;
BOOL  ReMakeSimplePanel = TRUE;

BOOL  OldDrawPanel = TRUE;
BOOL  DrawCrosshair = TRUE;
BOOL  Panel = TRUE;
BOOL PreAttractModePanel = TRUE;

BOOL  UsedStippledAlpha = FALSE;
BOOL  CanCullFlag = FALSE;


BOOL ChangeLevel( void );
void SelectQuitCurrentGame( MENUITEM *Item );

BOOL ForsakenAnyKey = FALSE;

float pixel_aspect_ratio;
float ticksperframe = 14.0F;  
float Oldframelag;  
float framelag = 0.0F;  
float framelag2 = 0.0F; 
float avgframelag = 0.0F; 
float Demoframelag = 0.5F;

extern int FontWidth;
extern int FontHeight;
extern int FontSourceWidth;
extern int FontSourceHeight;

extern  int PlayerSort[MAX_PLAYERS];
extern  int16 Stats[MAX_PLAYERS+1][MAX_PLAYERS+1];
extern int16 NumOfActivePlayers;

void GeneralBltFast( int srcx, int srcy , int w , int h  , int dstx , int dsty , LPDIRECTDRAWSURFACE Surface ,  char * FileName , DWORD flags , LPDIRECTDRAWSURFACE DestSurface);
void GeneralBlt( int srcx, int srcy , int w , int h  , int dstx , int dsty , int dstw , int dsth , LPDIRECTDRAWSURFACE SrcSurface ,   char * FileName , DWORD flags , LPDIRECTDRAWSURFACE DestSurface);

void CALLBACK TimerProc( UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 );

int16   LevelNum = 0 ;
int16   NewLevelNum = 0 ;
int16   NumLevels = 0;
char  ShortLevelNames[MAXLEVELS][32];
char  LevelNames[MAXLEVELS][128];
char  ServerLevelNames[MAXLEVELS][ 8 ];
uint8 ServerLevelsListState = SERVERLEVELS_Waiting;
int16 NumServerLevels = 0;
char  CollisionNames[MAXLEVELS][128];
char  CollisionZNames[MAXLEVELS][128];
char  BspNames[MAXLEVELS][128];
char  BspZNames[MAXLEVELS][128];
char  ZoneNames[MAXLEVELS][128];
char  WaterNames[MAXLEVELS][128];
char  TextNames[MAXLEVELS][128];
char  MsgNames[MAXLEVELS][128];
char  ExternalForceNames[MAXLEVELS][128];
char  TeleportNames[MAXLEVELS][128];

char MissionTextNames[MAXLEVELS][128];
char MissionTextPics[MAXLEVELS][128];

int use_level_path = 0;
char level_path[128];
char level_names[MAXLEVELS][128];
int use_data_path = 0;
int use_local_data = 0;
char data_path[ 128 ] = "";
char normdata_path[128] = "data\\";
int16 CameraRendering = CAMRENDERING_None;

BOOL SeriousError = FALSE;
BOOL DoClipping = TRUE;
BOOL OnceOnlyChangeLevel = FALSE;

LPDIRECT3DEXECUTEBUFFER RenderBufs[ 2 ] = { NULL, NULL };
void InitRenderBufs( LPDIRECT3DDEVICE lpDev );
void ReleaseRenderBufs( void );
BOOL ChangeBackgroundColour( float R, float G, float B );
BOOL SetMatrixViewPort( void );
void PrintInitViewStatus( BYTE Status );
void GetHardwareCaps( void );

void UpdateBGObjectsClipGroup( CAMERA * Camera );
void UpdateEnemiesClipGroup( CAMERA * Camera  );
void SpecialDestroyGame( void );
BOOL SaveFullScreenSnapShot( int8 * Filename );
void InitMySessionsList(void);

extern  int16 PrimaryInLevel[ MAXPRIMARYWEAPONS ];
extern  int16 SecondaryInLevel[ MAXSECONDARYWEAPONS ];
extern  int16 PrimaryInPlayers[ MAXPRIMARYWEAPONS ];
extern  int16 SecondaryInPlayers[ MAXSECONDARYWEAPONS ];
extern  int16 PrimaryToGenerate[ MAXPRIMARYWEAPONS ];
extern  int16 SecondaryToGenerate[ MAXSECONDARYWEAPONS ];
extern  int16 MinesInLevel[ MAXSECONDARYWEAPONS ];
extern  int16 OrbsInLevel;
extern  int16 OrbsToGenerate;
extern  int16 OrbsInPlayers;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
    Weapon Names...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
char PrimaryNames[7][16] = {
            { "Pulsar" } ,
            { "Trojax" } ,
            { "Pyrolite" } ,
            { "Transpulse" } ,
            { "Suss Gun" } ,
            { "Laser" } ,
            { "" }  
};
char SecondaryNames[12][16] = {
            { "Mug" } ,
            { "Solaris" } ,
            { "Thief" } ,
            { "Scatter" } ,
            { "Gravgon" } , 
            { "MFRL" } , 
            { "Titan" } ,
            { "Purge Mine" } ,
            { "Pine Mine" } ,
            { "Quantum Mine" } ,
            { "Spider Mine" } ,
            { "" }  

};
int PrimaryLengths[7] ={ 
             sizeof("Puls")-1  ,
             sizeof("Troj")-1  ,
             sizeof("Pyro")-1  ,
             sizeof("Tran")-1  ,
             sizeof("Suss")-1  ,
             sizeof("Beam")-1 };
int SecondaryLengths[12] = {
             sizeof("Mug ")-1 ,
             sizeof("Sola")-1 ,
             sizeof("Thei")-1 ,
             sizeof("Scat")-1 ,
             sizeof("Grav")-1 , 
             sizeof("MFRL")-1 , 
             sizeof("Tita"  )-1 ,
             sizeof("Purg")-1 ,
             sizeof("Pine")-1 ,
             sizeof("Quan")-1,
             sizeof("Spid")-1 };
#if 0
int PrimaryLengths[7] ={ 
             sizeof("Pulsar")-1  ,
             sizeof("Trojax")-1  ,
             sizeof("Pyrolite")-1  ,
             sizeof("Transpulse")-1  ,
             sizeof("Suss Gun")-1  ,
             sizeof("Beam Laser")-1 };
int SecondaryLengths[12] = {
             sizeof("Mug")-1 ,
             sizeof("Solaris")-1 ,
             sizeof("Thief")-1 ,
             sizeof("Scatter")-1 ,
             sizeof("Gravgon")-1 , 
             sizeof("MFRL")-1 , 
             sizeof("Titan")-1 ,
             sizeof("Purge Mine")-1 ,
             sizeof("Pine Mine")-1 ,
             sizeof("Quantum Mine")-1,
             sizeof("Spider Mine")-1 };
#endif


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
    Panel Description Stuff...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
char  PanelNames[8][32] ={  
               { "data\\pictures\\p320X50.bmp" } ,
               { "data\\pictures\\p320X60.bmp" } ,
               { "data\\pictures\\p320X100.bmp" } ,
               { "data\\pictures\\p512X96.bmp" } ,
               { "data\\pictures\\p640X100.bmp" } ,
               { "data\\pictures\\p640X120.bmp" } ,
               { "data\\pictures\\p800X150.bmp" } ,
               { "" }
};

//  How much can we get away with not rendering... and how much do we have to blit???
int16 PanelVisibleY[8] = { 50,      //320X200
               60,      //320X240
               100,     //320X400
               96,      //512X384
               100,     //640X400
               120,     //640X480
               150 };     //800X600

int16 PanelVisibleX[8] = { 320,     //320X200
               320,     //320X240
               320,     //320X400
               512,     //512X384
               640,     //640X400
               640,     //640X480
               800 };     //800X600
              
int16 CrossHairX[10] = { 192 ,192 ,160 , 128, 96, 224, 192, 160, 128, 96 };
int16 CrossHairY[10] = {  64, 64, 64, 64, 64, 32, 32, 32, 32, 32 };

int16 PrimaryX[6] = { 64+(48*0), 64+(48*1) , 64+(48*2) , 64+(48*3) , 64+(48*0), 64+(48*1) } ;
int16 PrimaryY[6] = { 96 , 96 , 96 , 96 , 96+(32*1) , 96+(32*1) };


int16 ModesX[8] = { 320,      //320X200
            320,      //320X240
            320,      //320X400
            512,      //512X384
            640,      //640X400
            640,      //640X480
            800 };      //800X600
int16 ModesY[8] = { 200,      //320X200
            240,      //320X240
            400,      //320X400
            384,      //512X384
            400,      //640X400
            480,      //640X480
            600 };      //800X600

float ModeScaleX[8] ={ 1.0F ,     //320X200
             1.0F ,     //320X240
             1.0F ,     //320X400
             1.6F ,     //512X384
             2.0F ,     //640X400
             2.0F ,     //640X480
             2.5F  };     //800X600
    
float ModeScaleY[8] ={ 1.0F  ,      //320X200
             1.2F  ,      //320X240
             2.0F  ,      //320X400
             1.92F ,      //512X384
             2.0F  ,      //640X400
             2.4F  ,      //640X480
             3.0F   };      //800X600


char  FontNames[8][64] = { { "data\\pictures\\font.bmp" } , //320X200
               { "data\\pictures\\font.bmp" } , //320X240
               { "data\\pictures\\font.bmp" } , //320X400
               { "data\\pictures\\font512.bmp" } ,  //512X384
               { "data\\pictures\\font512.bmp" } ,  //640X400
               { "data\\pictures\\font800.bmp" } ,  //640X480
               { "data\\pictures\\font800.bmp" } }; //800X600

char  DynamicFontNames[ 64 ];

char  ScoreNames[8][64] ={  
               { "data\\pictures\\S320X200.bmp" } ,
               { "data\\pictures\\S320X240.bmp" } ,
               { "data\\pictures\\S320X400.bmp" } ,
               { "data\\pictures\\S512X384.bmp" } ,
               { "data\\pictures\\S640X400.bmp" } ,
               { "data\\pictures\\S640X480.bmp" } ,
               { "data\\pictures\\S800X600.bmp" } ,
               { "" }
};

char  DynamicScoreNames[ 64 ];

char  StatsNames[8][64] ={  
               { "data\\pictures\\T320X200.bmp" } ,
               { "data\\pictures\\T320X240.bmp" } ,
               { "data\\pictures\\T320X400.bmp" } ,
               { "data\\pictures\\T512X384.bmp" } ,
               { "data\\pictures\\T640X400.bmp" } ,
               { "data\\pictures\\T640X480.bmp" } ,
               { "data\\pictures\\T800X600.bmp" } ,
               { "" }
};




/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
    Positions of stuff to go on the panel....
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
int16 ModeCase = 0;
uint16  PrimaryChanged;
uint16  SecondaryChanged;
uint16  ShieldChanged;
uint16  HullChanged;
uint16  PowerChanged;
uint16  PrimaryNumChanged;
uint16  SecondaryNumChanged;

int   PrimaryWeaponShowX;
int   PrimaryWeaponShowY;
int   PrimaryWeaponTextX;
int   PrimaryWeaponTextY;
int   SecondaryWeaponShowX;
int   SecondaryWeaponShowY;
int   SecondaryWeaponTextX;
int   SecondaryWeaponTextY;
int   PanelShieldPosX;
int   PanelShieldPosY;
int   PanelHullPosX;
int   PanelHullPosY;
int   PanelShieldTextPosX;
int   PanelShieldTextPosY;
int   PanelHullTextPosX;
int   PanelHullTextPosY;
int   PanelShieldBarPosX;
int   PanelShieldBarPosY;
int   PanelHullBarPosX;
int   PanelHullBarPosY;
int   PrimaryWeaponNumX;
int   PrimaryWeaponNumY;
int   SecondaryWeaponNumX;
int   SecondaryWeaponNumY;
int   PanelPowerPosX;
int   PanelPowerPosY;  

int   WeaponSizeX;
int   WeaponSizeY;

void FillInPanelPositions()
{
  if( ModeCase != -1 )
  {
    PrimaryChanged = (uint16) -1;
    HullChanged = (uint16) -1;
    ShieldChanged = (uint16) -1;
    PowerChanged = (uint16) -1;   
    PrimaryNumChanged = (uint16) -1;    
    SecondaryNumChanged = (uint16) -1;    
    
    WeaponSizeX = (int) ceil(36.0F * ModeScaleX[ModeCase]);
    WeaponSizeY = (int) ceil(37.0F * ModeScaleY[ModeCase]);
    PrimaryWeaponShowX = (int) floor( ( ModesX[ModeCase] * 0.5F ) - ( 149.0F * ModeScaleX[ModeCase] ) );
    PrimaryWeaponShowY = (int) floor( ( 6.0F * ModeScaleY[ModeCase] ) );
    SecondaryChanged = (uint16) -1;
    SecondaryWeaponShowX = (int) floor( ( ModesX[ModeCase] * 0.5F ) + ( 113.0F * ModeScaleX[ModeCase] ) );
    SecondaryWeaponShowY = (int) floor( ( 6.0F * ModeScaleY[ModeCase] ) );

    PanelShieldBarPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( 80.0F * ModeScaleX[ModeCase] ) );
    PanelShieldBarPosY = (int) ( PanelVisibleY[ModeCase] - ( 16.0F * ModeScaleY[ModeCase] ) );
    PanelShieldPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( 28.0F * ModeScaleX[ModeCase] ) );
    PanelShieldPosY = (int) ( PanelVisibleY[ModeCase] - ( 17.0F * ModeScaleY[ModeCase] ) );
    PanelShieldTextPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( 98.0F * ModeScaleX[ModeCase] ) );
    PanelShieldTextPosY = (int) ( PanelVisibleY[ModeCase] - ( 17.0F * ModeScaleY[ModeCase] ) );

    PanelHullBarPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) + ( 28.0F * ModeScaleX[ModeCase] ) );
    PanelHullBarPosY = (int) ( PanelVisibleY[ModeCase] - ( 16.0F * ModeScaleY[ModeCase] ) );
    PanelHullPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) + ( 12.0F * ModeScaleX[ModeCase] ) );
    PanelHullPosY = (int) ( PanelVisibleY[ModeCase] - ( 17.0F * ModeScaleY[ModeCase] ) );
    PanelHullTextPosX = ( int ) ( ( PanelVisibleX[ModeCase] * 0.5F ) + ( 86.0F * ModeScaleX[ModeCase] ) );
    PanelHullTextPosY = (int) ( PanelVisibleY[ModeCase] - ( 17.0F * ModeScaleY[ModeCase] ) );

    PrimaryWeaponTextX = (int) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( 44.0F * ModeScaleX[ModeCase] ) );
    PrimaryWeaponTextY = (int) ( PanelVisibleY[ModeCase] - ( 45.0F * ModeScaleY[ModeCase] ) );
    SecondaryWeaponTextX = (int) ( ( PanelVisibleX[ModeCase] * 0.5F ) + ( 29.0F * ModeScaleX[ModeCase] ) );
    SecondaryWeaponTextY = (int) ( PanelVisibleY[ModeCase] - ( 45.0F * ModeScaleY[ModeCase] ) );

    PrimaryWeaponNumX = (int) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( 44.0F * ModeScaleX[ModeCase] ) );
    PrimaryWeaponNumY = (int) ( PanelVisibleY[ModeCase] - ( 39.0F * ModeScaleY[ModeCase] ) );
    SecondaryWeaponNumX = (int) ( ( PanelVisibleX[ModeCase] * 0.5F ) + ( 29.0F * ModeScaleX[ModeCase] ) );
    SecondaryWeaponNumY = (int) ( PanelVisibleY[ModeCase] - ( 39.0F * ModeScaleY[ModeCase] ) );

    
    PanelPowerPosX = (int) ( ( PanelVisibleX[ModeCase] * 0.5F ) - ( (FontWidth*7)>>1 ) );
    PanelPowerPosY = (int) ( PanelVisibleY[ModeCase] - ( 38 *ModeScaleY[ModeCase] ) );
  
  }
}
  
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
    Off Screen Sufaces...Used to Blit to screen...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
LPDIRECTDRAWSURFACE     lpDDSOne;       // crosshair
LPDIRECTDRAWSURFACE     lpDDSTwo = NULL;       // Font Bitmap
LPDIRECTDRAWSURFACE     lpDDSThree;     // Panel
LPDIRECTDRAWSURFACE     lpDDSFour;     // Panel Contents

//LPDIRECTDRAWSURFACE     lpDDSOverlay;     // Panel
DDCOLORKEY ddcolorkey;

LPDIRECT3DMATERIAL lpBmat;    // a Material for the Background clearing

MLOADHEADER Mloadheader;
BOOL TestTransformClip(LPDIRECT3DVIEWPORT lpView);

MCLOADHEADER MCloadheader;          //  inner skin collision map...
MCLOADHEADER MCloadheadert0;        //  0 thickness collision map...

extern  LPDIRECT3DEXECUTEBUFFER Portal_lpD3DExBuf;

extern  uint16      FirstFmPolyUsed;
extern  uint16      FirstPolyUsed;
extern  uint16      FirstLineUsed;
extern  uint16      FirstScrPolyUsed;
extern  GLOBALSHIP    Ships[MAX_PLAYERS+1];
extern  BYTE      WhoIAm;
extern  BYTE      Current_Camera_View;    // which object is currently using the camera view....
extern  BOOL      RemoteCameraActive;

extern  int16     ShowPortal;
extern  int16     ShowSkin;
extern  int16         NamesAreLegal;
extern  SHORTNAMETYPE     Names;  // all the players short Names....

BOOL ServerMainGame(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );

BOOL MainGame(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );

void Build_View();
BOOL  Disp3dPanel( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );
BOOL DispTracker( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );

/**************************************************************************
  DirectInput Globals
 **************************************************************************/
LPDIRECTINPUT                   lpdi = NULL;
LPDIRECTINPUTDEVICE             lpdiMouse = NULL;
LPDIRECTINPUTDEVICE       lpdiKeyboard = NULL;
LPDIRECTINPUTDEVICE       lpdiBufferedKeyboard = NULL;
LPDIRECTINPUTDEVICE2      lpdiJoystick[MAX_JOYSTICKS];
DIDEVCAPS           diJoystickCaps[MAX_JOYSTICKS];
int               Num_Joysticks;



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Global Variables
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/   
#define Near  ( 100.0F * GLOBAL_SCALE )
//#define Far   ( 65535.0F * GLOBAL_SCALE )
#define Far   ( 49152.0F * GLOBAL_SCALE )
#define MinNear ( 50.0F * GLOBAL_SCALE )

D3DMATRIX proj = {
    D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(Far/(Far-Near)), D3DVAL(1.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(-Far*Near/(Far-Near)), D3DVAL(0.0) };

D3DMATRIX panelproj = {
    D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(Far/(Far-Near)), D3DVAL(1.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(-Far*Near/(Far-Near)), D3DVAL(0.0) };

D3DMATRIX trackerproj = {
    D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(Far/(Far-Near)), D3DVAL(1.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(-Far*Near/(Far-Near)), D3DVAL(0.0) };

MATRIX  ProjMatrix = {
    D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(2.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(Far/(Far-Near)), D3DVAL(1.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(-Far*Near/(Far-Near)), D3DVAL(0.0) };


  VECTOR  SlideLeft = { -1.0F, 0.0F, 0.0F };
  VECTOR  SlideRight  = { 1.0F, 0.0F, 0.0F };
  VECTOR  SlideUp   = { 0.0F, 1.0F, 0.0F };
  VECTOR  SlideDown = { 0.0F, -1.0F, 0.0F };
  VECTOR  Forward   = { 0.0F, 0.0F, 1.0F };
  VECTOR  Backward  = { 0.0F, 0.0F,-1.0F };
  VECTOR  NullVector  = { 0.0F, 0.0F, 0.0F };

  VECTOR  Camera_Dir = { 0.0F, 0.0F,0.0F };


  D3DLIGHT light;


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
static D3DEXECUTEDATA d3dExData;
static D3DEXECUTEBUFFERDESC debDesc;

/*
 * Global projection, view, world and identity matricies
 */
D3DMATRIXHANDLE hProj;
D3DMATRIXHANDLE hView;
D3DMATRIXHANDLE hWorld;
D3DMATRIX view;
D3DMATRIX identity = {
    D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0)
};
MATRIX Identity = {
    D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0)
};


D3DMATRIX world = {
    D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0),
    D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0), D3DVAL(1.0)
};

BOOL ZClearsOn = TRUE;
BOOL g_OddFrame = FALSE;


BOOL SetZProj( void )
{
  float Scale, NewNear;

  if( hfov > START_FOV )
  {
    Scale = ( ( MAX_FOV - hfov ) / ( MAX_FOV - START_FOV ) );
    NewNear = ( MinNear + ( ( Near - MinNear ) * Scale ) );
    proj._33 = D3DVAL(Far/(Far-NewNear));
    proj._43 = D3DVAL(-Far*NewNear/(Far-NewNear));
  }
  else
  {
    proj._33 = D3DVAL(Far/(Far-Near));
    proj._43 = D3DVAL(-Far*Near/(Far-Near));
  }

#ifdef Z_TRICK
  if( g_OddFrame )
  {
    proj._33 = ( 1.0f - ( 0.5f * proj._33 ) );
    proj._43 *= -0.5f;
  }else
  {
    proj._33 *= 0.5f;
    proj._43 *= 0.5f;
  }
  ProjMatrix._33 = proj._33;
  ProjMatrix._43 = proj._43;
  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &proj) != D3D_OK)
  {
    return FALSE;
  }
#endif
  return TRUE;
}


SetFOV( float fov )
{
    HRESULT rval;
  float screen_width, screen_height;
  float Scale, NewNear;

  if ( fov <= 1.0F || fov >= 150.0F )
    fov = hfov;
    memset(&viewport, 0, sizeof(D3DVIEWPORT));
    viewport.dwSize = sizeof(D3DVIEWPORT);
    rval = d3dapp->lpD3DViewport->lpVtbl->GetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
        Msg( "GetViewport failed.\n%s", D3DAppErrorToString(rval) );
        return FALSE;
    }
  if ( d3dapp->bFullscreen )
  {
    screen_width = (float) d3dapp->ThisMode.w;
    screen_height = (float) d3dapp->ThisMode.h;
  }
  else
  {
    screen_width = (float) d3dapp->WindowsDisplay.w;
    screen_height = (float) d3dapp->WindowsDisplay.h;
  }
  pixel_aspect_ratio = screen_aspect_ratio * screen_height / screen_width;
  viewplane_distance = (float) ( viewport.dwWidth / ( 2 * tan( DEG2RAD( fov ) * 0.5 ) ) );
  proj._11 = 2 * viewplane_distance / viewport.dwWidth;
  proj._22 = 2 * viewplane_distance / ( viewport.dwHeight / pixel_aspect_ratio );

  if( fov > START_FOV )
  {
    Scale = ( ( MAX_FOV - fov ) / ( MAX_FOV - START_FOV ) );
    NewNear = ( MinNear + ( ( Near - MinNear ) * Scale ) );
    proj._33 = D3DVAL(Far/(Far-NewNear));
    proj._34 = D3DVAL( 1.0 );
    proj._43 = D3DVAL(-Far*NewNear/(Far-NewNear));
    proj._44 = D3DVAL( 0.0 );
  }
  else
  {
    proj._33 = D3DVAL(Far/(Far-Near));
    proj._34 = D3DVAL( 1.0 );
    proj._43 = D3DVAL(-Far*Near/(Far-Near));
    proj._44 = D3DVAL( 0.0 );
  }

#ifdef Z_TRICK
  if ( !ZClearsOn )
  {
    if( g_OddFrame )
    {
      proj._33 = ( 1.0f - ( 0.5f * proj._33 ) );
      proj._43 *= -0.5f;
    }else
    {
      proj._33 *= 0.5f;
      proj._43 *= 0.5f;
    }
  }
#endif

  ProjMatrix._11 = proj._11;
  ProjMatrix._22 = proj._22;
  ProjMatrix._33 = proj._33;
  ProjMatrix._34 = proj._34;
  ProjMatrix._43 = proj._43;
  ProjMatrix._44 = proj._44;

  hfov = fov;
  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &proj) != D3D_OK)
  {
      return FALSE;
  }
  return TRUE;
}


void SetViewportError( char *where, D3DVIEWPORT *vp, HRESULT rval )
{
  static char msg[1024];

  sprintf( msg, "SetViewport failed in %s.\n"
    "%s\n"
    "size=%d(=sizeof()%d?)\n"
    "xpos=%d ypos=%d\n"
    "width=%d height=%d\n"
    "xscale=%f yscale=%f\n"
    "xmax=%f ymax=%f\n"
    "zmin=%f zmax=%f\n",
    where,
    D3DAppErrorToString( rval ),
    vp->dwSize, sizeof( *vp ),
    vp->dwX, vp->dwY,
    vp->dwWidth, vp->dwHeight,
    vp->dvScaleX, vp->dvScaleY,
    vp->dvMaxX, vp->dvMaxY,
    vp->dvMinZ, vp->dvMaxZ );
  Msg( msg );
}

BOOL
ResizeViewport( float scale )
{
    HRESULT rval;
  int left, top;
  int width, height;
  int maxwidth, maxheight;
  BOOL  NewDrawPanel;
  BOOL  NewDrawSimplePanel;
  
  InsideResizeViewport = TRUE;

  scale = 1.0F;

  /*
     * Setup the viewport for specified viewing area
     */
    memset(&viewport, 0, sizeof(D3DVIEWPORT));
    viewport.dwSize = sizeof(D3DVIEWPORT);
    rval = d3dapp->lpD3DViewport->lpVtbl->GetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
        Msg( "GetViewport failed.\n%s", D3DAppErrorToString(rval) );
        return FALSE;
    }
  maxwidth = d3dapp->szClient.cx;


//  DrawSimplePanel = FALSE;
  if( scale < 1.0F )
  {
    NewDrawSimplePanel = TRUE;
  }
  if( scale >= 1.0F )
  {
    NewDrawSimplePanel = FALSE;
  }
  
  if( NewDrawSimplePanel )
  {
    maxheight = d3dapp->szClient.cy - PanelVisibleY[ModeCase];
  }else{
    maxheight = d3dapp->szClient.cy;
  }
  if ( scale < 1.01F )
  {
    width = (int) floor( maxwidth * scale );
    if ( width < MIN_VIEWPORT_WIDTH )
    {
      width = MIN_VIEWPORT_WIDTH;
      scale = (float) width / maxwidth;
    }
    height = (int) floor( maxheight * scale );

    NewDrawPanel = FALSE;

    left = ( ( maxwidth - width ) >> 1 ) & ~1;
    top = ( ( maxheight - height ) >> 1 ) & ~1;
  }
  else
  {
    width = maxwidth;
    height = maxheight;
    if ( scale > 1.11F )
    {
      NewDrawPanel = TRUE;
      scale = 1.2F;
    }
    else
    {
      NewDrawPanel = FALSE;
    }
    left = 0;
    top = 0;
  }
  CurrentViewportScale = scale;

  // This disables the 3d panel...
  NewDrawPanel = DrawPanel;

    
  if( (NewDrawPanel != DrawPanel) || (NewDrawSimplePanel != DrawSimplePanel) )
  {
//    ReleaseView();
//    DrawPanel = NewDrawPanel;
//    DrawSimplePanel = NewDrawSimplePanel;
//    InitView();
  }
  
  
  viewport.dwX = left;
  viewport.dwY = top;
    viewport.dwWidth = width;
    viewport.dwHeight = height;
    viewport.dvScaleX = viewport.dwWidth / (float)2.0;
    viewport.dvScaleY = viewport.dwHeight / (float)2.0;
    viewport.dvMaxX = (float)D3DDivide(D3DVAL(viewport.dwWidth),
                                       D3DVAL(2 * viewport.dvScaleX));
    viewport.dvMaxY = (float)D3DDivide(D3DVAL(viewport.dwHeight),
                                       D3DVAL(2 * viewport.dvScaleY));
    rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "ResizeViewport", &viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }
  SetFOV( hfov );

  // clear viewport
  D3DAppIClearBuffers();

  return TRUE;
}
BOOL
FullScreenViewport()
{
    HRESULT rval;
  int left, top;
  int width, height;
  int maxwidth, maxheight;

    /*
     * Setup the viewport for specified viewing area
     */
    memset(&viewport, 0, sizeof(D3DVIEWPORT));
    viewport.dwSize = sizeof(D3DVIEWPORT);
    rval = d3dapp->lpD3DViewport->lpVtbl->GetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
        Msg( "GetViewport failed.\n%s", D3DAppErrorToString(rval) );
        return FALSE;
    }
  maxwidth = d3dapp->szClient.cx;
  maxheight = d3dapp->szClient.cy;
  width = maxwidth;
  height = maxheight;
  left = 0;
  top = 0;

    viewport.dwX = left;
  viewport.dwY = top;
    viewport.dwWidth = width;
    viewport.dwHeight = height;
    viewport.dvScaleX = viewport.dwWidth / (float)2.0;
    viewport.dvScaleY = viewport.dwHeight / (float)2.0;
    viewport.dvMaxX = (float)D3DDivide(D3DVAL(viewport.dwWidth),
                                       D3DVAL(2 * viewport.dvScaleX));
    viewport.dvMaxY = (float)D3DDivide(D3DVAL(viewport.dwHeight),
                                       D3DVAL(2 * viewport.dvScaleY));
    rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "FullScreenViewport", &viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }
  SetFOV( hfov );
  // clear viewport
  D3DAppIClearBuffers();
  D3DAppIClearBuffers();
  return TRUE;
}


void ReallyShowCursor( BOOL show )
{
  if ( show )
  {
    while ( ShowCursor( TRUE ) < 0 )
      ; // try again
  }
  else
  {
    while ( ShowCursor( FALSE ) >= 0 )
      ; // try again
  }
}

void
SetCursorClip( void )
{
    HRESULT         err;

  if ( !d3dapp )
  {
    DebugPrintf( "SetCursorClip: d3dapp is NULL\n" );
    ClipCursor( NULL );
    ReallyShowCursor( TRUE );
    return;
  }
  cursorclip.left = d3dapp->pClientOnPrimary.x + d3dapp->szClient.cx / 2;
  cursorclip.top = d3dapp->pClientOnPrimary.y + d3dapp->szClient.cy / 2;
  cursorclip.right = cursorclip.left + 1;
  cursorclip.bottom = cursorclip.top + 1;
  if ( cursorclipped )
  {
    if ( lpdiMouse )
    {
      err = IDirectInputDevice_Acquire(lpdiMouse);
    }
    if ( lpdiKeyboard )
    {
      err = IDirectInputDevice_Acquire(lpdiKeyboard);
    }
    if ( lpdiBufferedKeyboard )
    {
      err = IDirectInputDevice_Acquire(lpdiBufferedKeyboard);
    }
    DebugPrintf( "SetCursorClip: cursor is clipped\n" );
    ClipCursor( &cursorclip );
    SetCursorPos( cursorclip.left, cursorclip.top );
    ReallyShowCursor( FALSE );
  }
  else
  {
    if ( lpdiMouse )
    {
      err = IDirectInputDevice_Unacquire(lpdiMouse);
    }
    if ( lpdiKeyboard )
    {
      err = IDirectInputDevice_Unacquire(lpdiKeyboard);
    }
    if ( lpdiBufferedKeyboard )
    {
      err = IDirectInputDevice_Unacquire(lpdiBufferedKeyboard);
    }
    DebugPrintf( "SetCursorClip: cursor is not clipped\n" );
    ClipCursor( NULL );
    ReallyShowCursor( TRUE );
  }
}

void
OverrideDefaults(Defaults* defaults)
{
    strcpy(defaults->Name, "Forsaken");
}

static BOOL LevelValid( char *level )
{
  uint16 i;
  
  if ( !IsPseudoHost )
    return TRUE;

  for ( i = 0; i < NumServerLevels; i++ )
  {
    if ( !_strnicmp( level, ServerLevelNames[ i ], 7 ) )  // 8th char could be null terminator
      return TRUE;
  }

  return FALSE;
}

BOOL InitLevels( char * levels_list )
{

  int j;
  FILE *f;
  char levels_file[ 256 ];
  char * DataPath;

  if ( !levels_list ){
    levels_list = LEVELSFILE;
  }

  // path to Data
  DataPath = normdata_path;
  if ( use_data_path ) {
    DataPath = data_path;
  }
  
  // path to Levels\\<type>.bat
    sprintf( levels_file, "%s\\levels\\%s", DataPath, levels_list );
  
  // set some globals
    CurrentLevelsList = levels_list;
    OriginalLevels = TRUE;

  /////
    // Read Levels From File
  /////

    f = fopen( levels_file, "r" );
  if (!f ){
    return FALSE;
  }

  // scan each level name into ShortLevelNames[j]
    j=0;
  while ( j < MAXLEVELS && fscanf( f, " %s", ShortLevelNames[ j ] ) == 1 )
  {

        // if the level is a valid hard coded server level
    // or if we are the psuedo host
    if ( LevelValid( ShortLevelNames[ j ] ) )
    {

      // get the path to the current mxv file
      sprintf( LevelNames[ j ], "%slevels\\%s\\%s.mxv", DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );

      // check to see if the level mxv file exists
      if ( !File_Exists( LevelNames[ j ] ) ){
         continue;
      }

      // add paths for a level to the lists
      sprintf( CollisionNames[ j ],   "%slevels\\%s\\%s.mc",    DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( CollisionZNames[ j ],    "%slevels\\%s\\%sz.mc",   DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( BspNames[ j ],       "%slevels\\%s\\%s.bsp",   DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( BspZNames[ j ],      "%slevels\\%s\\%sz.bsp",  DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( ZoneNames[ j ],      "%slevels\\%s\\%s.zon",   DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( WaterNames[ j ],     "%slevels\\%s\\%s.wat",   DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( TextNames[ j ],      "%slevels\\%s\\%s.txt",   DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( MsgNames[ j ],       "%slevels\\%s\\%s.msg",   DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( ExternalForceNames[ j ], "%slevels\\%s\\%s.gf",    DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( TeleportNames[ j ],    "%slevels\\%s\\%s.tel",   DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( MissionTextNames[ j ],   "%slevels\\%s\\%s.mis",   DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );
      sprintf( MissionTextPics[ j ],    "%slevels\\%s\\%s.ppm",   DataPath, ShortLevelNames[ j ], ShortLevelNames[ j ] );

      // increment counter
      j++;

    }
  }

  NumLevels = j;
  fclose( f );

  // no levels were found
  // this should never happen
  // unless you delete all the level directories
  // or the level file doesn't exist
  if( !NumLevels ) return FALSE;

  // select default level
  LevelList.items = 0;
  LevelList.top_item = 0;
  LevelList.display_items = 8;
  LevelList.selected_item = 0;  // you can only ever start game on first level!
  NewLevelNum = 0;

  // add each level to the master list
    for (j = 0; j < NumLevels; j++)
  {
    strcpy( (char*) &LevelList.item[LevelList.items][0] , (char*) &ShortLevelNames[j][0] );
    LevelList.items++;
    }

  // Read TXT in of default level
  LoadLevelText( NULL );

  // success !
  return TRUE;

}


BOOL SetMatrixViewPort( void )
{
    size_t size;
    LPVOID lpBufStart, lpInsStart, lpPointer;
    LPDIRECT3DEXECUTEBUFFER lpD3DExCmdBuf;
    D3DMATERIAL bmat;
    D3DMATERIALHANDLE hBmat;
  LPDIRECTDRAW lpDD = d3dapp->lpDD;
  LPDIRECT3D lpD3D = d3dapp->lpD3D;
  LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
    LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;

    memset(&bmat, 0, sizeof(D3DMATERIAL));
  bmat.dwSize = sizeof(D3DMATERIAL);
  bmat.dwRampSize = 1;
  if (lpD3D->lpVtbl->CreateMaterial(lpD3D, &lpBmat, NULL) != D3D_OK) {
      return FALSE;
  }
  if (lpBmat->lpVtbl->SetMaterial(lpBmat, &bmat) != D3D_OK) {
      return FALSE;
  }
  if (lpBmat->lpVtbl->GetHandle(lpBmat, lpDev, &hBmat) != D3D_OK) {
      return FALSE;
  }
  if (lpView->lpVtbl->SetBackground(lpView, hBmat) != D3D_OK) {
      return FALSE;
  }
  
   
    ticksperframe = 14.0F;
       
  /*
   * Set the view, world and projection matrices
   * Create a buffer for matrix set commands etc.
   */
  MAKE_MATRIX(lpDev, hView, identity);
  MAKE_MATRIX(lpDev, hProj, proj);
  MAKE_MATRIX(lpDev, hWorld, world);
  world = identity;
  size = 0;
  size += sizeof(D3DINSTRUCTION) * 3;
  size += sizeof(D3DSTATE) * 5;
   
       
    if (MakeExecuteBuffer( &debDesc, lpDev , &lpD3DExCmdBuf , size ) != TRUE ) return FALSE;
   
   
  /*
   * lock it so it can be filled
   */
  if (lpD3DExCmdBuf->lpVtbl->Lock(lpD3DExCmdBuf, &debDesc) != D3D_OK) return FALSE;
  
  lpBufStart = debDesc.lpData;
  memset(lpBufStart, 0, size);
  lpPointer = lpBufStart;
  
  lpInsStart = lpPointer;
  OP_STATE_TRANSFORM(3, lpPointer);
      STATE_DATA(D3DTRANSFORMSTATE_PROJECTION, hProj, lpPointer);
      STATE_DATA(D3DTRANSFORMSTATE_VIEW, hView, lpPointer);
      STATE_DATA(D3DTRANSFORMSTATE_WORLD, hWorld, lpPointer);
  OP_STATE_LIGHT(1, lpPointer);
      STATE_DATA(D3DLIGHTSTATE_AMBIENT, RGBA_MAKE(255, 255, 255, 0), lpPointer);
  OP_EXIT(lpPointer);
  /*
   * Setup the execute data describing the buffer
   */
  lpD3DExCmdBuf->lpVtbl->Unlock(lpD3DExCmdBuf);
  memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
  d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
  d3dExData.dwInstructionOffset = (ULONG) 0;
  d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
  lpD3DExCmdBuf->lpVtbl->SetExecuteData(lpD3DExCmdBuf, &d3dExData);
  lpDev->lpVtbl->BeginScene(lpDev);
  lpDev->lpVtbl->Execute(lpDev, lpD3DExCmdBuf, lpView , D3DEXECUTE_CLIPPED);
  lpDev->lpVtbl->EndScene(lpDev);
  /*
   * We are done with the command buffer.
   */
  XRELEASE(lpD3DExCmdBuf);
   
    viewport.dwSize = sizeof(D3DVIEWPORT);
    if ((hresult = lpView->lpVtbl->GetViewport(lpView, (LPD3DVIEWPORT)&viewport)) != D3D_OK)
    {
      viewport.dwWidth = 320;
      viewport.dwHeight = 200;
    }
    lpD3Ddev = lpDev;
    if ( !initfov )
    {
      SetFOV( chosen_fov = normal_fov );
      initfov = 1;
    }

    if( InsideResizeViewport == FALSE )
    {
      InitViewport( 1.1F );
    }else{
    InsideResizeViewport = FALSE;
  }

  return TRUE;
}

struct {

  DWORD ModeMask;
  DWORD SourceMask;
  DWORD DestMask;
  DWORD Mode;
  DWORD Source;
  DWORD Dest;

} PreferredCaps[] = {

  { D3DPTBLENDCAPS_MODULATE,  D3DPBLENDCAPS_ONE,  D3DPBLENDCAPS_ONE,          // Mask
    D3DTBLEND_MODULATE,   D3DBLEND_ONE,   D3DBLEND_ONE },           // Value

  { D3DPTBLENDCAPS_MODULATEALPHA, D3DPBLENDCAPS_SRCALPHA, D3DPBLENDCAPS_SRCALPHA,   // Mask
    D3DTBLEND_MODULATEALPHA,      D3DBLEND_SRCALPHA,      D3DBLEND_SRCALPHA },    // Value

  { D3DPTBLENDCAPS_ADD, D3DPBLENDCAPS_INVSRCALPHA,  D3DPBLENDCAPS_SRCALPHA,     // Mask
    D3DTBLEND_ADD,      D3DBLEND_INVSRCALPHA,     D3DBLEND_SRCALPHA },      // Value

  { D3DPTBLENDCAPS_MODULATEALPHA, D3DPBLENDCAPS_SRCALPHA, D3DPBLENDCAPS_INVSRCALPHA,  // Mask
    D3DTBLEND_MODULATEALPHA,      D3DBLEND_SRCALPHA,      D3DBLEND_INVSRCALPHA },   // Value

};

struct {

  DWORD ModeMask;
  DWORD SourceMask;
  DWORD DestMask;
  DWORD Mode;
  DWORD Source;
  DWORD Dest;

} NonPrimPreferredCaps[] = {

  { D3DPTBLENDCAPS_MODULATEALPHA, D3DPBLENDCAPS_SRCALPHA, D3DPBLENDCAPS_SRCALPHA,   // Mask
    D3DTBLEND_MODULATEALPHA,      D3DBLEND_SRCALPHA,      D3DBLEND_SRCALPHA },    // Value

  { D3DPTBLENDCAPS_MODULATE,  D3DPBLENDCAPS_ONE,  D3DPBLENDCAPS_ONE,          // Mask
    D3DTBLEND_MODULATE,   D3DBLEND_ONE,   D3DBLEND_ONE },           // Value

  { D3DPTBLENDCAPS_ADD, D3DPBLENDCAPS_INVSRCALPHA,  D3DPBLENDCAPS_SRCALPHA,     // Mask
    D3DTBLEND_ADD,      D3DBLEND_INVSRCALPHA,     D3DBLEND_SRCALPHA },      // Value

  { D3DPTBLENDCAPS_MODULATEALPHA, D3DPBLENDCAPS_SRCALPHA, D3DPBLENDCAPS_INVSRCALPHA,  // Mask
    D3DTBLEND_MODULATEALPHA,      D3DBLEND_SRCALPHA,      D3DBLEND_INVSRCALPHA },   // Value
};

void  GetHardwareCaps( void )
{   
  int16         Count;
  int16         NumPreferredCaps;
  struct  _D3DPrimCaps *  TriCapsPtr;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Check for Culling Caps
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  CanCullFlag = FALSE;
  if( ( d3dappi.ThisDriver.Desc.dpcTriCaps.dwMiscCaps & D3DPMISCCAPS_CULLCCW ) &&
    ( d3dappi.ThisDriver.Desc.dpcTriCaps.dwMiscCaps & D3DPMISCCAPS_CULLNONE ) )
    CanCullFlag = TRUE;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Check for Translucency Caps
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  UsedStippledAlpha = FALSE;

  if( !Is3Dfx && !Is3Dfx2 )
  {
    TriCapsPtr = &d3dappi.ThisDriver.Desc.dpcTriCaps;
    NumPreferredCaps = ( sizeof( PreferredCaps ) / sizeof( PreferredCaps[ 0 ] ) );
    
    for( Count = 0; Count < NumPreferredCaps; Count++ )
    {
      if( ( TriCapsPtr->dwDestBlendCaps & PreferredCaps[ Count ].ModeMask ) &&
        ( TriCapsPtr->dwSrcBlendCaps & PreferredCaps[ Count ].SourceMask ) &&
        ( TriCapsPtr->dwDestBlendCaps & PreferredCaps[ Count ].DestMask ) )
      {
        CurrentTextureBlend = PreferredCaps[ Count ].Mode;
        CurrentSrcBlend = PreferredCaps[ Count ].Source;
        CurrentDestBlend = PreferredCaps[ Count ].Dest;
        break;
      }
    }
  }
  else
  {
    TriCapsPtr = &d3dappi.ThisDriver.Desc.dpcTriCaps;
    NumPreferredCaps = ( sizeof( NonPrimPreferredCaps ) / sizeof( NonPrimPreferredCaps[ 0 ] ) );
    
    for( Count = 0; Count < NumPreferredCaps; Count++ )
    {
      if( ( TriCapsPtr->dwDestBlendCaps & NonPrimPreferredCaps[ Count ].ModeMask ) &&
        ( TriCapsPtr->dwSrcBlendCaps & NonPrimPreferredCaps[ Count ].SourceMask ) &&
        ( TriCapsPtr->dwDestBlendCaps & NonPrimPreferredCaps[ Count ].DestMask ) )
      {
        CurrentTextureBlend = NonPrimPreferredCaps[ Count ].Mode;
        CurrentSrcBlend = NonPrimPreferredCaps[ Count ].Source;
        CurrentDestBlend = NonPrimPreferredCaps[ Count ].Dest;
        break;
      }
    }
  }

  if( Count == NumPreferredCaps )
  {
    UsedStippledAlpha = TRUE;
    if( d3dappi.ThisDriver.Desc.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAFLATBLEND )
      UsedStippledAlpha = FALSE;
    if( d3dappi.ThisDriver.Desc.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAGOURAUDBLEND )
      UsedStippledAlpha = FALSE;
  }

  bPolySort = FALSE;
//  if( PowerVR_Overide || (PowerVR = IsPowerVRD3DDevice() ) )
  if( PowerVR_Overide )
  {
    PowerVR = TRUE;
    //PowerVr
    CurrentTextureBlend = D3DTBLEND_MODULATEALPHA;
    CurrentDestBlend = D3DBLEND_INVSRCALPHA;
    CurrentSrcBlend = D3DBLEND_SRCALPHA;
  }
}



float HealthCount = 0.0F;
int PowerSizes[6] = { 0 , 4 , 16 , 24 , 40 , 56 };
void TestBlt()
{
#if 0
    RECT    src;
    HRESULT ddrval;
  RECT    dest;
  DDBLTFX fx;
  int x;
  int y;
#endif
  int energy;

//  while( 1 )
  {
    if( DrawCrosshair && WhoIAm == Current_Camera_View)
    {
      if( bPolyText && PolyText[MyGameStatus])
      {
        // Add Crosshair Polygon..
        AddScreenPolyText( (uint16) 63 , (float) (viewport.dwX + (viewport.dwWidth>>1)) , (float) (viewport.dwY + (viewport.dwHeight>>1)) , 64, 255, 64, 255 );

        // Blt trojax PowerLevel / LaserTemperature
        energy = (int) ( ( PowerLevel * 0.01F ) * 9.0F );
        if( !energy )
        {
          energy = (int) ( ( LaserTemperature *0.01F ) * 9.0F );
          if( energy > 8 ) energy = 8;
        }
        if( energy )
        {
          AddScreenPolyText( (uint16) (64+8-energy) , (float) (viewport.dwX + (viewport.dwWidth>>1))-16 , (float) (viewport.dwY + (viewport.dwHeight>>1))+4 , 64, 255, 64, 255 );
        }

        if ( ( control.turbo || Ships[WhoIAm].Object.CruiseControl == CRUISE_NITRO ) && NitroFuel )
        {
          AddScreenPolyTextScale( 72, (float) ( (d3dappi.szClient.cx>>1) - (NitroFuel - 8) ), (float) (viewport.dwY + (viewport.dwHeight>>1)-7 ) ,
                      (float) ( ( ( 1.0F / 100.0F ) * ( NitroFuel * 0.5F) ) * ( (32.0F-0.125F) + 0.125F ) ) , 1.0F,
                      64, 255, 64, 255 );
        }

      
      }else{
        //  Blt Crosshair
        if( !EnhancedXHair )
        {
          GeneralBltFast( 0 , 0 , 16 , 16  , (viewport.dwX + (viewport.dwWidth>>1))-8 , (viewport.dwY + (viewport.dwHeight>>1))-8 ,
                  lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
        }
        //  Blt Nitro...
        if ( ( control.turbo || Ships[WhoIAm].Object.CruiseControl == CRUISE_NITRO ) && NitroFuel )
        {
          Printuint16( (uint16) NitroFuel , (d3dappi.szClient.cx>>1)-(1*FontWidth), (viewport.dwY + (viewport.dwHeight>>1))+8 , 2 );
          GeneralBltFast( 0 , 24 , (int)NitroFuel , 3  , (d3dappi.szClient.cx>>1)-(int)(NitroFuel*0.5F) , (viewport.dwY + (viewport.dwHeight>>1))-8 ,
                  lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
        }
        // Blt trojax PowerLevel / LaserTemperature
        energy = (int) ( ( PowerLevel * 0.01F ) * 9.0F );
        if( !energy )
        {
          energy = (int) ( ( LaserTemperature *0.01F ) * 9.0F );
          if( energy > 8 ) energy = 8;
        }
        if( energy )
        {
          GeneralBltFast( 0 , 24-energy , 4 , energy  , (viewport.dwX + (viewport.dwWidth>>1))-16 , (viewport.dwY + (viewport.dwHeight>>1))+4-energy ,
                  lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
        }
      }

      if( Ships[WhoIAm].Invul )
      {
        Print4x5Text( "I" , FontWidth , d3dappi.szClient.cy-((FontHeight*4)+8) , 2 );
        Printuint16( (uint16) (Ships[WhoIAm].InvulTimer / 60.0F) , FontWidth*4 , d3dappi.szClient.cy-((FontHeight*4)+8) , 2 );
      }
      if( Ships[WhoIAm].Object.Flags & SHIP_SuperNashram )
      {
        Print4x5Text( "S" , FontWidth , d3dappi.szClient.cy-((FontHeight*5)+10) , 2 );
        Printuint16( (uint16) (Ships[WhoIAm].SuperNashramTimer / 60.0F) , FontWidth*4 , d3dappi.szClient.cy-((FontHeight*5)+10) , 2 );
      }
      if( Ships[WhoIAm].Object.Flags & SHIP_Stealth )
      {
        Print4x5Text( "C" , FontWidth , d3dappi.szClient.cy-((FontHeight*6)+12) , 2 );
        Printuint16( (uint16) (Ships[WhoIAm].StealthTime / 60.0F) , FontWidth*4 , d3dappi.szClient.cy-((FontHeight*6)+12) , 2 );
      }

    }
    
    if( Panel && !PlayDemo)
    {
      if( DrawSimplePanel )
      {
        PlotSimplePanel();
      }else{
        if( !DrawPanel && (WhoIAm == Current_Camera_View ) ) 
        {
          // Full Screen Minimum Stats...

          if( bPolyText && PolyText[MyGameStatus])
          {
            // blt hull
            AddScreenPolyText( (uint16) 56 , (float) FontWidth , (float) (d3dappi.szClient.cy-((FontHeight*1)+2) ), 32, 255, 32, 255 );
            // blt shld
            AddScreenPolyText( (uint16) 55 , (float) FontWidth , (float) (d3dappi.szClient.cy-((FontHeight*2)+4) ), 32, 255, 32, 255 );
            // Blt Primary
            AddScreenPolyText( (uint16)( Ships[WhoIAm].Primary + 38 ) , (float) (d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] ) ), (float) (d3dappi.szClient.cy-((FontHeight*2)+4) ), 32, 255, 32, 255 );
            // Blt Secondary
            AddScreenPolyText( (uint16)( Ships[WhoIAm].Secondary + 44 ) , (float) (d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] ) ) , (float) (d3dappi.szClient.cy-((FontHeight*1)+2)), 32, 255, 32, 255 );
            // Blt Power Pods
            AddScreenPolyText( (uint16) (Ships[WhoIAm].Object.PowerLevel + 57 ), (float) (d3dappi.szClient.cx >> 1) - ( ( FontWidth * 7) >>1 ) , (float) (d3dappi.szClient.cy-((FontHeight*1)+2)), 32, 255, 32, 255 );
            // Blt Mine..
            energy = (int) GetBestMine();
            if( energy != 65535 )
            {
              AddScreenPolyText( (uint16)( energy + 44 ) , (float) (d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[energy] ) ), (float) (FontHeight), 32, 255, 32, 255 );
              Printuint16( (uint16) SecondaryAmmo[energy] , d3dappi.szClient.cx - ( FontWidth*5) , FontHeight , 2 );
            }


            
            // poly shld bar
            AddScreenPolyTextScale( 72, (float) (FontWidth*10)-4, (float) (d3dappi.szClient.cy-(FontHeight*2)-2) ,
                        (float) ( ( ( 1.0F / 256.0F ) * ( Ships[WhoIAm].Object.Shield *0.25F ) ) * ( (32.0F-0.125F) + 0.125F ) ) , 1.0F,
                        (uint8)(63+(ShieldHit * (192/24) )), (uint8)(255-(ShieldHit * (192/24) )), 64, 255 );
            if( ShieldHit )
              ShieldHit -=1;
            
            // poly hull bar
            AddScreenPolyTextScale( 72, (float) (FontWidth*10)-4, (float) (d3dappi.szClient.cy-(FontHeight*1)-2) ,
                        (float) ( ( ( 1.0F / 256.0F ) * ( Ships[WhoIAm].Object.Hull * 0.25F) ) * ( (32.0F-0.125F) + 0.125F ) ) , 1.0F,
                        (uint8)(63+(HullHit * (192/24) )), (uint8)(255-(HullHit * (192/24) )), 64, 255 );
            if( HullHit )
              HullHit -=1;
            

          }else{
            // blt hull
            DoFontBlt( FontWidth*28 , FontHeight , FontWidth*4 , FontHeight , FontWidth , d3dappi.szClient.cy-((FontHeight*1)+2) );
            // blt shld
            DoFontBlt( FontWidth*28 , 0 , FontWidth*4 , FontHeight , FontWidth , d3dappi.szClient.cy-((FontHeight*2)+4) );
            // Blt Primary
            DoFontBlt( FontWidth*16 , FontHeight*Ships[WhoIAm].Primary , FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] , FontHeight , d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] ) , d3dappi.szClient.cy-((FontHeight*2)+4)  );
            // Blt Secondary
            DoFontBlt( FontWidth*16 , (FontHeight*6)+FontHeight*Ships[WhoIAm].Secondary , FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] , FontHeight , d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] ) , d3dappi.szClient.cy-((FontHeight*1)+2)  );
            // Blt Power Pods
            DoFontBlt( FontWidth*(32-7) , FontHeight*(7+Ships[WhoIAm].Object.PowerLevel) , FontWidth*7 , FontHeight , (d3dappi.szClient.cx >> 1) - ( ( FontWidth * 7) >>1 ) , d3dappi.szClient.cy-((FontHeight*1)+2) );
            // Blt Mine..
            energy = (int) GetBestMine();
            if( energy != 65535 )
            {
              DoFontBlt( FontWidth*16 , (FontHeight*6)+FontHeight*energy , FontWidth*SecondaryLengths[energy] , FontHeight , d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[energy] ) , FontHeight  );
              Printuint16( (uint16) SecondaryAmmo[energy] , d3dappi.szClient.cx - ( FontWidth*5) , FontHeight , 2 );
            }
            // blt shld bar
            if( ShieldHit == 0 )
            {
              GeneralBltFast( 0 , 24 , (int)(Ships[WhoIAm].Object.Shield * 0.25F) , 3  , FontWidth*10 , d3dappi.szClient.cy-((FontHeight*2)+3) ,
                      lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT, d3dapp->lpBackBuffer);
            }else{
              ShieldHit -=1;
              GeneralBltFast( 0 , 24 + ((ShieldHit>>2)*8) , (int)(Ships[WhoIAm].Object.Shield * 0.25F) , 3  , FontWidth*10 , d3dappi.szClient.cy-((FontHeight*2)+3) ,
                      lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
            }
            Printuint16( (uint16) Ships[WhoIAm].Object.Shield , FontWidth*6 , d3dappi.szClient.cy-((FontHeight*2)+4) , 2 );
            // blt hull bar
            if( HullHit == 0 )
            {
              GeneralBltFast( 0 , 24 ,(int) (Ships[WhoIAm].Object.Hull *0.25F) , 3  , FontWidth*10 , d3dappi.szClient.cy-((FontHeight*1)+1) ,
                      lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
            }else{
              HullHit -=1;
              GeneralBltFast( 0 , 24 + ((HullHit>>2)*8) , (int)(Ships[WhoIAm].Object.Hull *0.25F) , 3  , FontWidth*10 , d3dappi.szClient.cy-((FontHeight*1)+1) ,
                      lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
            }

          }

          // blt shield num
          Printuint16( (uint16) Ships[WhoIAm].Object.Shield , FontWidth*6 , d3dappi.szClient.cy-((FontHeight*2)+4) , 2 );
          // blt hull num
          if( Ships[WhoIAm].Object.Hull > 0.0F && Ships[WhoIAm].Object.Hull < 1.0F )
          {
            Printuint16( (uint16) 1 , FontWidth*6 , d3dappi.szClient.cy-((FontHeight*1)+2) , 2 );
          }else{
            Printuint16( (uint16) Ships[WhoIAm].Object.Hull , FontWidth*6 , d3dappi.szClient.cy-((FontHeight*1)+2) , 2 );
          }
          // Blt Primary ammo
          if( Ships[WhoIAm].Primary == PYROLITE_RIFLE )
          {
            energy =  (int) PyroliteAmmo;
          }else{
            if( Ships[WhoIAm].Primary == SUSS_GUN )
            {
              energy = (int) SussGunAmmo;
            }else{
              energy = (int) GeneralAmmo;
            }
          }
          Printuint16( (uint16) energy , d3dappi.szClient.cx - ( FontWidth*5) , d3dappi.szClient.cy-((FontHeight*2)+4) , 2 );
          
          // Blt Secondary ammo
          Printuint16( (uint16) GetCurSecAmmo() , d3dappi.szClient.cx - ( FontWidth*5) , d3dappi.szClient.cy-((FontHeight*1)+2) , 2 );
          
        }
      }
    
      if( (NamesAreLegal != 0) || IsHost )
      {
        ScoreSort();
        PrintScoreSort();
      }
      MessageQuePrint();
    }else{
      if( Panel )
      {
        ScoreSort();
        PrintScoreSort();
        MessageQuePrint();

        if( DemoEyesSelect.value != MAX_PLAYERS )
        {
          Print4x5Text( Names[DemoEyesSelect.value] ,d3dappi.szClient.cx - (FontWidth*9), FontHeight , 0 );
        }
      }
    }
    if( Ships[WhoIAm].Object.Mode == GAMEOVER_MODE )
    {
      CenterPrint4x5Text( "Game Over" , (d3dappi.szClient.cy >> 1) - (FontHeight*2) , 2 );
    }
  }
      
//    TaskSleep( 1 );
}

void
ReleaseLevel(void)
{
  switch( MyGameStatus )
  {
  case  STATUS_Copyright:
  case  STATUS_Title:
  case  STATUS_BetweenLevels:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
    break;
  
  default:
    if( GodMode ) LoseAllWeapons();

    ReleaseComponentedShips();
    KillThermo();
    NodeRelease();
    CameraRelease();
    ReleaseExternalForces();
    ReleaseTeleports();
    ReleaseTriggers();
    ReleaseTriggerArea();
    ReleaseRTLights();
    ReleaseGoal();

    if( CountDownOn || BombTag )
    {
      DeleteCountdownDigits();
    }

    KillAllBGObjects();
    ReleaseAllEnemies();
    ReleaseAllRestartPoints();
    DestroySound( DESTROYSOUND_All );
    ProcessGuaranteedMessages( TRUE , FALSE , FALSE );
    FreeServerMessageQue();


#ifdef SOFTWARE_ENABLE
    if( SoftwareVersion )
    {
      SetVTables();
      SWRendView();
      d3dappi.lpBackBuffer->lpVtbl->Unlock( d3dappi.lpBackBuffer, NULL );
    }
#endif
    break;
  }

}



void
ReleaseScene(void)
{
  int16 Count;

  if( RegenPoints != NULL )
  {
    free( RegenPoints );
    RegenPoints = NULL;
    NumRegenPoints = 0;

    for( Count = 0; Count < MAX_PLAYERS; Count++ )
    {
      if( RegenSlotsCopy[ Count ] != NULL )
      {
        free( RegenSlotsCopy[ Count ] );
        RegenSlotsCopy[ Count ] = NULL;
      }
    }
  }

  switch( MyGameStatus )
  {
  case  STATUS_Copyright:
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
    break;
  
  default:
    DestroyGame();
    break;
  }

}

void
ReleaseView(void)
{
  switch( MyGameStatus )
  {
  case  STATUS_PlayingAVI:
    ReleaseDDSurf(lpDDSTwo);  //font
    lpDDSTwo = NULL;
    break;
  case  STATUS_Copyright:
    ReleaseTitle();
    break;
  case  STATUS_StartingMultiplayer:
  case  STATUS_GetPlayerNum:
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_QuittingFromTitles:
  case  STATUS_WaitingToStartMultiPlayerClient:
  case  STATUS_WaitingToStartDemo:        // Added by DC 24/1/98

    ReleaseFlyGirl();

    FreeAllLastAFrameScrPolys();
    ReleaseTitle();
    ReleaseSpecialExecBufs();
    ReleaseTloadheader( &Tloadheader );
    ReleaseSysTload( SystemMemTPages, &CurrentSysTexture );
    ReleaseTitleModels();
    Free_All_Off_Files( &Title_OffsetFiles[ 0 ] );
    ReleaseRenderBufs();
    ReleasePolySort();
    break;

  case STATUS_ViewingScore:
#if 0 // used to release solid scr poly stuff
    ReleaseSpecialExecBufs();
    ReleaseTloadheader( &Tloadheader );
    ReleaseSysTload( SystemMemTPages, &CurrentSysTexture );
    Free_All_Off_Files( &Title_OffsetFiles[ 0 ] );
    ReleaseRenderBufs();
    ReleasePolySort();
#endif
    FreeScoreDisplay();
    break;

  case STATUS_ViewingStats:
    FreeStatsDisplay();
    break;

  default:
    ReleaseSpecialExecBufs();

    Bspfree();

    
    FreeAllLastAFrameScrPolys();
    ReleaseMloadheader(&Mloadheader);
    ReleaseTloadheader( &Tloadheader );
    ReleaseSysTload( SystemMemTPages, &CurrentSysTexture );
    ReleaseModels();
    
    if ( MCloadheader.Buffer )
    {
      free( MCloadheader.Buffer );
      MCloadheader.Buffer = NULL;
    }
    if ( MCloadheadert0.Buffer )
    {
      free( MCloadheadert0.Buffer );
      MCloadheadert0.Buffer = NULL;
    }
  
    Free_All_Off_Files( &OffsetFiles[ 0 ] );
    ReleaseSkinExecs();
    ReleasePortalExecs();
    ReleaseRenderBufs();
  
    WaterRelease();
    
    FreeTxtFile();
    FreeMsgFile();

    if( lpDDSOne != NULL ) {
      ReleaseDDSurf(lpDDSOne);
      lpDDSOne = NULL;
    }
 
    if( lpDDSTwo != NULL ) {
      ReleaseDDSurf(lpDDSTwo);
      lpDDSTwo = NULL;
    }

    if( !DrawPanel && DrawSimplePanel ) {
      if( lpDDSThree != NULL ) {
        ReleaseDDSurf(lpDDSThree);
        lpDDSTwo = NULL;
      }
      if( lpDDSFour != NULL ) {
        ReleaseDDSurf(lpDDSFour);
        lpDDSTwo = NULL;
      }
    }

  RELEASE(lpBmat);


//    TaskerInit();

  }
}
//extern  uint16  BackgroundModel;

BOOL
InitScene(void)
{
  int i;
//  BackgroundModel = (uint16) -1;

  OnceOnlyInitModel();
  InitXLights();
  InitPrimBulls();
  InitSecBulls();
  InitFmPoly();

#ifdef SHADOWTEST
  InitTriangles();
#endif

  InitPolys();
  InitScrPolys();
  InitLines();
  SetupSpotFX();
  InitPickups();
  InitAmmoUsed();
  InitAfterBurners();
  InitBGObjects();
  InitRestartPoints();
  InitEnemies();

  for( i = 0 ; i < MAX_PLAYERS ; i++ )
  {
    Ships[i].ModelNum = (uint16) -1;
  }


  QueryPerformanceFrequency((LARGE_INTEGER *) &Freq);

  switch( MyGameStatus )
  {
  case  STATUS_Copyright:
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
  case  STATUS_StartingMultiplayer:
  case  STATUS_GetPlayerNum:
    if (!LoadTitleModels())
    { Msg( "LoadTitleModels failed.\n" );
      return FALSE;
    }
    break;
  default:
    break;
  }
  
  return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Init the view...This might be title or score
            or main game....
  Input   :   LPDIRECTDRAW lpDD, LPDIRECT3D lpD3D, LPDIRECT3DDEVICE lpDev,
            LPDIRECT3DVIEWPORT lpView
  Output    :   BOOL TRUE/FLASE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

BOOL
InitView( void )
{
  LPDIRECTDRAW lpDD = d3dapp->lpDD;
  LPDIRECT3D lpD3D = d3dapp->lpD3D;
  LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
    LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;
  DWORD dwItems = INFINITE;

  CheatsDisabled = FALSE;

  // if viewing splash screens, initview will be called after viewing final screen
  if (( MyGameStatus == STATUS_SplashScreen ) || ( MyGameStatus == STATUS_FinishedShowingSplashScreen ))
    return TRUE;

  // flush keyboard buffer...
  if (lpdiBufferedKeyboard)
  {
    IDirectInputDevice_GetDeviceData( lpdiBufferedKeyboard, sizeof(DIDEVICEOBJECTDATA), NULL, &dwItems, 0); 
  }

  if( PowerVR )
  {
    MakeColourMode = MCM_PowerVR;
  }
  else
  {
    if(d3dapp->CurrDriver != 0)
    {
      if( UsedStippledAlpha != TRUE ) MakeColourMode = MCM_Normal;
      else MakeColourMode = MCM_Stipple;
    }
    else
    {
      MakeColourMode = MCM_Software;
    }
  }


  GetHardwareCaps();
  InitModeCase();

  switch( MyGameStatus )
  {
  case  STATUS_PlayingAVI:
    if ( bSoundEnabled )
      DestroySound( DESTROYSOUND_All );
    break;
  case  STATUS_Copyright:
    AddCommandToBat( "mkdir %%2\\data\\BGObjects\n" );
    AddCommandToBat( "mkdir %%2\\data\\Models\n" );
    AddCommandToBat( "mkdir %%2\\data\\Textures\n" );
    AddCommandToBat( "mkdir %%2\\data\\Pictures\n" );
    AddCommandToBat( "mkdir %%2\\data\\Offsets\n" );
    AddCommandToBat( "mkdir %%2\\data\\Sfx\n" );

    InitFont( FALSE );
    
    AddCommentToBat( "Started Loading Titles" );

    if( InitTitle( lpDD, lpD3D, lpDev, lpView ) != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }


    break;
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
  case  STATUS_StartingMultiplayer:
  case  STATUS_GetPlayerNum:
    if( InitTitle( lpDD, lpD3D, lpDev, lpView ) != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    if ( !bSoundEnabled )
    {
      if (! InitializeSound( DESTROYSOUND_All ))
      {
        DebugPrintf("unable to initialise sound in initview\n");
        Msg("unable to initialise sound in initview\n");
        exit(1);
      }
    }

    CurrentSysTexture = 0;
        
    InitPolySort();

    InitRenderBufs(lpDev );

    InitSpecialExecBufs();
    
    if( !SetMatrixViewPort() )
    {
      SeriousError = TRUE;
      Msg( "SetMatrixViewPort() Failed\n" );
      return FALSE;
    }
    // Init the Texture Handler
    InitTload( &Tloadheader );

    if( !Load_All_Off_Files( &Title_OffsetFiles[ 0 ] ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    
    if( !PreLoadFlyGirl() )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    

    if( !PreInitModel( lpDev , TitleModelSet ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    
    //  Load in And if nescessary ReScale Textures...
    if( !Tload( &Tloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    // Load all system memory textures...
    if( !SysTload( SystemMemTPages, CurrentSysTexture ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    if( !InitModel( lpDev , TitleModelSet ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }
      
    if ( !AllocateCompFlyGirl() )
    {
      SeriousError = TRUE;
      return FALSE;
    }else
    {
      UpdateFlyGirl( &BikePos, &MATRIX_Identity, FALSE );
    }
    

    QueryPerformanceCounter((LARGE_INTEGER *) &LastTime);

    //DummyTextureIndex = FindTexture( &Tloadheader, "textures\\dummy.ppm" );
    DummyTextureIndex = FindTexture( &Tloadheader, "textures\\dummy.bmp" );
    if ( DummyTextureIndex != -1 )
    {
      Tloadheader.PlaceHolderFile[ DummyTextureIndex ] = ( char * )malloc( sizeof( char ) * 256 );
      Tloadheader.PlaceHolder[ DummyTextureIndex ] = TRUE;
    }

    if ( !CurrentMenu )
      MenuRestart( &MENU_Start );

    break;

  case STATUS_ViewingScore:
    InitScoreDisplay();
    break;

  case STATUS_ViewingStats:
    InitStatsDisplay();
    break;
  
  default:
    InitView_MyGameStatus = MyGameStatus;
    MyGameStatus = STATUS_InitView_0;
    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    D3DAppClearScreenOnly();
    CameraStatus = CAMERA_AtStart;  //prevents on screen menus from being suppressed
    break;
  }


  return TRUE;
}

BOOL FAR PASCAL InitJoystickInput(LPCDIDEVICEINSTANCE pdinst, 
                                  LPVOID pvRef) 
{ 
   LPDIRECTINPUT pdi = pvRef; 
   LPDIRECTINPUTDEVICE pdev;
   HRESULT hr;
   char tempstr[MAX_PATH];

   // create the DirectInput joystick device 
   if (pdi->lpVtbl->CreateDevice(pdi, &pdinst->guidInstance, 
                                 &pdev, NULL) != DI_OK) 
   { 
      return DIENUM_CONTINUE; 
   } 

   strcpy (tempstr, &pdinst->tszProductName[0]);
   JoystickInfo[Num_Joysticks].Name = (char *) malloc ( strlen( tempstr ) + 1 );
   strcpy (JoystickInfo[Num_Joysticks].Name, tempstr);

   hr = pdev->lpVtbl->QueryInterface( pdev, &IID_IDirectInputDevice2,
                       (LPVOID *)&lpdiJoystick[Num_Joysticks]);
   pdev->lpVtbl->Release(pdev);

   if (hr != S_OK)
     return DIENUM_CONTINUE;

   diJoystickCaps[ Num_Joysticks ].dwSize = sizeof( DIDEVCAPS );
   hr = IDirectInputDevice2_GetCapabilities( lpdiJoystick[ Num_Joysticks ], &diJoystickCaps[ Num_Joysticks ] );

   if (hr != S_OK)
     return DIENUM_CONTINUE;

   Num_Joysticks++;

   if (Num_Joysticks > MAX_JOYSTICKS)
     Num_Joysticks = MAX_JOYSTICKS;

   return DIENUM_CONTINUE;
}

BOOL CALLBACK DIEnumDeviceObjectsProc( 
                      LPCDIDEVICEOBJECTINSTANCE lpddoi,
                      LPVOID pvRef) 
{ 
  GUID   AxisGUID[MAX_JOYSTICK_AXIS - 1];
  GUID   ButtonGUID = GUID_Button;
//  GUID   KeyGUID = GUID_Key;
  GUID   POVGUID = GUID_POV;
//  GUID   UnknownGUID = GUID_Unknown;
  GUID   CompareGUID;
  int *joysticknumptr;
  int joysticknum, axis;
  char tempstr[MAX_JOYNAME];
#if 1
   AxisGUID[0] = GUID_XAxis;
   AxisGUID[1] = GUID_YAxis;
   AxisGUID[2] = GUID_ZAxis;
   AxisGUID[3] = GUID_RxAxis;
   AxisGUID[4] = GUID_RyAxis;
   AxisGUID[5] = GUID_RzAxis;
   AxisGUID[6] = GUID_Slider; // js can have up to 2 sliders

  joysticknumptr = (int *)(pvRef);
  joysticknum = *joysticknumptr;

  CompareGUID = lpddoi->guidType;

  for (axis = 0; axis < MAX_JOYSTICK_AXIS - 1; axis++)
  {
    if (IsEqualGuid( &AxisGUID[axis], &CompareGUID ))
    {
      if ((axis == AXIS_SliderAxis0) && (JoystickInfo[joysticknum].Axis[AXIS_SliderAxis0].exists))
        axis++;

      JoystickInfo[joysticknum].Axis[axis].exists = TRUE;

      if (lpddoi->tszName && *lpddoi->tszName)
        strncpy( tempstr, lpddoi->tszName, MAX_JOYNAME );
      else
        sprintf( tempstr, "Axis %d", axis );

      JoystickInfo[joysticknum].Axis[axis].name = 
        (char *) malloc ( strlen ( tempstr ) + 1 );

      strcpy ( JoystickInfo[joysticknum].Axis[axis].name, tempstr );

      JoystickInfo[joysticknum].Axis[axis].feedback =
        ( lpddoi->dwType & DIDFT_FFACTUATOR ) ? TRUE : FALSE;

      if ( JoystickInfo[joysticknum].Axis[axis].feedback )
        DebugPrintf( "Joystick %s Axis %s supports force feedback\n",
          JoystickInfo[ joysticknum ].Name,
          JoystickInfo[ joysticknum ].Axis[ axis ].name );

/*
      if (lpddoi->tszName && *lpddoi->tszName)
        strncpy( JoystickInfo[joysticknum].Axis[axis].name, lpddoi->tszName, MAX_JOYNAME );
      else
        // no name given back by joystick for axis, therefore assign arbitrary name
        sprintf( JoystickInfo[joysticknum].Axis[axis].name, "Axis %d", axis );
*/

      JoystickInfo[joysticknum].NumAxis++;
      return DIENUM_CONTINUE; 
    }
  }

  if (IsEqualGuid( &ButtonGUID, &CompareGUID ))
  {

#ifdef USE_JOYSTICK_NAMES
    if (lpddoi->tszName && *lpddoi->tszName)
      strncpy( tempstr, lpddoi->tszName, MAX_JOYNAME );
    else
#endif
      sprintf( tempstr, "Button %d", JoystickInfo[joysticknum].NumButtons );

    JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name = 
      (char *) malloc ( strlen ( tempstr ) + 1 );

    strcpy ( JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name, tempstr );

    JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].feedback_trigger =
      ( lpddoi->dwType & DIDFT_FFEFFECTTRIGGER ) ? TRUE : FALSE;

    JoystickInfo[joysticknum].NumButtons++;

/*    
    if (lpddoi->tszName && *lpddoi->tszName)
      strncpy( JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons++].name, lpddoi->tszName, MAX_JOYNAME );
    else
    {
      // no name given back by joystick for button, therefore assign arbitrary name
      sprintf( JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name, "Button %d", JoystickInfo[joysticknum].NumButtons );
      JoystickInfo[joysticknum].NumButtons++;
    }
*/

    return DIENUM_CONTINUE; 
  }

  if (IsEqualGuid( &POVGUID, &CompareGUID ))
  {
    int dir;

#ifdef USE_JOYSTICK_NAMES
    if (lpddoi->tszName && *lpddoi->tszName)
      strncpy( tempstr, lpddoi->tszName, MAX_JOYNAME );
    else
#endif
      if ( JoystickInfo[ joysticknum ].NumPOVs )
        sprintf( tempstr, "Hat %d", JoystickInfo[joysticknum].NumPOVs + 1 );
      else
        sprintf( tempstr, "Hat" );

    JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name = 
      (char *) malloc ( strlen ( tempstr ) + 1 );

    strcpy ( JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name, tempstr );

    for ( dir = 0; dir < MAX_POV_DIRECTIONS; dir++ )
    {
      JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].dirname[ dir ] = 
        (char *) malloc ( strlen ( tempstr ) + strlen( JoystickPOVDirections[ dir ] ) + 2 );
      
      strcpy ( JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].dirname[ dir ], tempstr );
      strcat ( JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].dirname[ dir ], " " );
      strcat ( JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].dirname[ dir ], JoystickPOVDirections[ dir ] );
    }

    JoystickInfo[joysticknum].NumPOVs++;
/*    
    if (lpddoi->tszName && *lpddoi->tszName)
      strncpy( JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs++].name, lpddoi->tszName, MAX_JOYNAME );
    else
    {
      // no name given back by joystick for POV, therefore assign arbitrary name
      sprintf( JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name, "POV %d", JoystickInfo[joysticknum].NumPOVs );
      JoystickInfo[joysticknum].NumPOVs++;
    }
*/

    return DIENUM_CONTINUE; 
  }
#endif
  return DIENUM_CONTINUE; 
}


#ifdef USE_DINPUT_KEYNAMES

#define MAX_KEYS  (256)
DWORD KeyMax;
DIDEVICEOBJECTINSTANCE KeyInfo[ MAX_KEYS ];

BOOL CALLBACK DIEnumKeyboardObjectsProc( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef )
{
  DWORD key = lpddoi->dwOfs;
  if ( key < MAX_KEYS )
    KeyInfo[ key ] = *lpddoi;
  if ( KeyMax < key )
    KeyMax = key;
  return DIENUM_CONTINUE;
}


char *DI_KeyName( DWORD key )
{
  return ( key < MAX_KEYS && KeyInfo[ key ].dwSize ) ? KeyInfo[ key ].tszName : NULL;
}

#endif // USE_DINPUT_KEYNAMES


/**************************************************************************
  InitDInput
 **************************************************************************/
BOOL InitDInput(void)
{
  HRESULT         err;
  GUID     guid_mouse = GUID_SysMouse;
  GUID     guid_keyboard = GUID_SysKeyboard;
//  GUID     guid_joystick = GUID_Joystick;
  DIPROPDWORD dipdw =
        {
            {
                sizeof(DIPROPDWORD),        // diph.dwSize
                sizeof(DIPROPHEADER),       // diph.dwHeaderSize
                0,                          // diph.dwObj
                DIPH_DEVICE,                // diph.dwHow
            },
            DINPUT_BUFFERSIZE,              // dwData
        };
  LPDIRECTINPUTDEVICE     tempJoystick = NULL;
  LPVOID joysticknumptr;
  int i, j, k;
  BOOL failjoystick;

    err = DirectInputCreateA(myglobs.hInstApp, DIRECTINPUT_VERSION, &lpdi, NULL);

    if(err != DI_OK)
    {
            return FALSE;
    }


    // Create a mouse.
    err = IDirectInput_CreateDevice(lpdi, &guid_mouse, &lpdiMouse, NULL);
  if ( err != DI_OK )
  {
    goto fail;
  }

    // Tell DirectInput that we want to receive data in mouse format
    err = IDirectInputDevice_SetDataFormat(lpdiMouse, &c_dfDIMouse);

    if(err != DI_OK)
    {
            goto fail;
    }

    // set desired access mode
    err = IDirectInputDevice_SetCooperativeLevel(lpdiMouse, myglobs.hWndMain,
    DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    if(err != DI_OK)
    {
            goto fail;
    }

  err = IDirectInputDevice_Acquire(lpdiMouse);

    // try to create keyboard device
    if(IDirectInput_CreateDevice(lpdi, &guid_keyboard, &lpdiKeyboard, NULL) !=DI_OK)
    {
            goto fail;
    }

    // Tell DirectInput that we want to receive data in keyboard format
    if (IDirectInputDevice_SetDataFormat(lpdiKeyboard, &c_dfDIKeyboard) != DI_OK)
    {
            goto fail;
    }

    // set cooperative level
    if(IDirectInputDevice_SetCooperativeLevel(lpdiKeyboard, myglobs.hWndMain,
                     DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
    {
            goto fail;
    }

#ifdef USE_DINPUT_KEYNAMES
  // get key info
  KeyMax = 0;
  memset( KeyInfo, 0, sizeof( KeyInfo ) );
  if (IDirectInputDevice_EnumObjects(lpdiKeyboard, DIEnumKeyboardObjectsProc, NULL, DIDFT_ALL ) != DI_OK)
  {
    // don't care -- fall back on internal key name info
  }
#endif // USE_DINPUT_KEYNAMES

    // try to acquire the keyboard
    err = IDirectInputDevice_Acquire(lpdiKeyboard);

    if(err != DI_OK)
    {
//            goto fail;
    }

    // try to create another keyboard device (which will be buffered, for menu screens)
    if(IDirectInput_CreateDevice(lpdi, &guid_keyboard, &lpdiBufferedKeyboard, NULL) !=DI_OK)
    {
            goto fail;
    }

    // Tell DirectInput that we want to receive data in keyboard format
    if (IDirectInputDevice_SetDataFormat(lpdiBufferedKeyboard, &c_dfDIKeyboard) != DI_OK)
    {
            goto fail;
    }

    // set cooperative level
    if(IDirectInputDevice_SetCooperativeLevel(lpdiBufferedKeyboard, myglobs.hWndMain,
                     DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
    {
            goto fail;
    }

    // set the buffer size...
  err = lpdiBufferedKeyboard->lpVtbl->SetProperty(lpdiBufferedKeyboard, DIPROP_BUFFERSIZE, &dipdw.diph);

    if(err != DI_OK)
    {
            goto fail;
    }

    // try to acquire the keyboard
    err = IDirectInputDevice_Acquire(lpdiBufferedKeyboard);

    if(err != DI_OK)
    {
//            goto fail;
    }

  FF_Init();
  // try to create Joystick devices
  for ( i = 0; i < MAX_JOYSTICKS; i++ )
    lpdiJoystick[i] = NULL;
  Num_Joysticks = 0;

//  lpdi->lpVtbl->EnumDevices(lpdi, DIDEVTYPE_JOYSTICK, 
//                         InitJoystickInput, lpdi, DIEDFL_ATTACHEDONLY); 

  failjoystick = FALSE;
  for (i = 0; i < Num_Joysticks; i++)
  {
    JoystickInfo[i].assigned = FALSE;
    JoystickInfo[i].connected = TRUE;
    JoystickInfo[i].NumAxis = 0;
    JoystickInfo[i].NumButtons = 0;
    JoystickInfo[i].NumPOVs = 0;

    joysticknumptr = (LPVOID)&i;
    for (j = AXIS_Start; j <= AXIS_End; j++)
    {
      JoystickInfo[i].Axis[j].exists = FALSE;
    }
    lpdiJoystick[i]->lpVtbl->EnumObjects(lpdiJoystick[i], DIEnumDeviceObjectsProc, 
                     joysticknumptr, DIDFT_ALL); 

    for (j = AXIS_Start; j <= AXIS_End; j++)
    {
      JoystickInfo[i].Axis[j].action = SHIPACTION_Nothing;
      JoystickInfo[i].Axis[j].inverted = FALSE;
      JoystickInfo[i].Axis[j].deadzone = 20;
      JoystickInfo[i].Axis[j].fine = TRUE;
    }

    for (j = 0; j < JoystickInfo[i].NumButtons; j++)
      JoystickInfo[i].Button[j].action = SHIPACTION_Nothing;

    for (j = 0; j < JoystickInfo[i].NumPOVs; j++)
    {
      for (k = 0; k < MAX_POV_DIRECTIONS; k++)
        JoystickInfo[i].POV[j].action[k] = SHIPACTION_Nothing;
    }

    if ( diJoystickCaps[ i ].dwFlags & DIDC_FORCEFEEDBACK )
    {
      DIPROPDWORD DIPropAutoCenter;
      
      DIPropAutoCenter.diph.dwSize = sizeof(DIPropAutoCenter);
      DIPropAutoCenter.diph.dwHeaderSize = sizeof(DIPROPHEADER);
      DIPropAutoCenter.diph.dwObj = 0;
      DIPropAutoCenter.diph.dwHow = DIPH_DEVICE;
      DIPropAutoCenter.dwData = 0;
      
      err = IDirectInputDevice2_SetProperty( lpdiJoystick[ i ], DIPROP_AUTOCENTER, &DIPropAutoCenter.diph );
      if ( err != DI_OK )
      {
        DebugPrintf( "Unable to turn autocentering off for forcefeedback joystick %s\n",
          JoystickInfo[ i ].Name );
        goto fail;
      }

      if ( !FF_InitJoystick( i ) )
      {
        DebugPrintf( "Unable to initialise force feedback info\n" );
        goto fail;
      }
    }
    else
    {
      JoystickInfo[ i ].feedback = NULL;
    }

    // Tell DirectInput that we want to receive data in joystick format
    if (IDirectInputDevice2_SetDataFormat(lpdiJoystick[i], &c_dfDIJoystick2) == DI_OK)
    {                          
      // set cooperative level
      if(IDirectInputDevice2_SetCooperativeLevel(lpdiJoystick[i], myglobs.hWndMain,
//                 DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) == DI_OK)
                 DISCL_EXCLUSIVE | DISCL_FOREGROUND) == DI_OK) // need exclusive mode for force-feedback joysticks
      {
        // try to acquire the Joystick
        err = IDirectInputDevice2_Acquire(lpdiJoystick[i]);
        if (err != DI_OK)
        {
          failjoystick = TRUE;
        }
      }else
      {
        failjoystick = TRUE;
      }
    }else
    {
      failjoystick = TRUE;
    }

    
    if (failjoystick)
    {
      failjoystick = FALSE;
      IDirectInputDevice2_Release(lpdiJoystick[i]);
      lpdiJoystick[i] = NULL;
    }
#if NOW_DONE_INSIDE_DEFAULTJOYSTICKSETTINGS
    SetUpJoystickAxis(i);

     // if spaceorb...
    if ( !_stricmp( JoystickInfo[i].Name, "Spacetec SpaceOrb 360" ) )
    {
      DebugPrintf("Spaceorb detected - auto configuring axis...\n");
      ConfigureSpaceorbAxis( i );
    }
#endif
  }
  DebugPrintf( "InitDInput: %d joysticks connected\n", Num_Joysticks );
  DefaultJoystickSettings( &default_config );
  *player_config = default_config;
  DebugPrintf( "InitDInput: default configs initialised\n" );

  // if we get here, all DirectInput objects were created ok
    return TRUE;

fail:
    if (lpdiMouse)  IDirectInputDevice_Release(lpdiMouse), lpdiMouse  = NULL;
    if (lpdiKeyboard)  IDirectInputDevice_Release(lpdiKeyboard), lpdiKeyboard  = NULL;
    if (lpdiBufferedKeyboard)  IDirectInputDevice_Release(lpdiBufferedKeyboard), lpdiBufferedKeyboard  = NULL;
  for (i = 0; i < Num_Joysticks; i++)
  {
      if (lpdiJoystick[i])
    {
      FF_ReleaseJoystick( i );
      IDirectInputDevice2_Release(lpdiJoystick[i]);
      lpdiJoystick[i] = NULL;
    }
    if ( JoystickInfo[ i ].feedback )
    {
      free( JoystickInfo[ i ].feedback );
      JoystickInfo[ i ].feedback = NULL;
    }
  }
    if (lpdi)   IDirectInputDevice_Release(lpdi), lpdi     = NULL;
    return FALSE;               
}

BOOL TermDInput( void )
{
    int i;

  if (lpdiMouse)
  {
    IDirectInputDevice_Unacquire(lpdiMouse);
    IDirectInputDevice_Release(lpdiMouse);
    lpdiMouse  = NULL;
  }
    if (lpdiKeyboard)
  {
    IDirectInputDevice_Unacquire(lpdiKeyboard);
    IDirectInputDevice_Release(lpdiKeyboard);
    lpdiKeyboard  = NULL;
  }
    if (lpdiBufferedKeyboard)
  {
    IDirectInputDevice_Unacquire(lpdiBufferedKeyboard);
    IDirectInputDevice_Release(lpdiBufferedKeyboard);
    lpdiBufferedKeyboard  = NULL;
  }

  for (i = 0; i < Num_Joysticks; i++)
  {
      if (lpdiJoystick[i])
    {
      FF_ReleaseJoystick( i );
      IDirectInputDevice2_Unacquire(lpdiJoystick[i]);
      IDirectInputDevice2_Release(lpdiJoystick[i]);
      lpdiJoystick[i]  = NULL;
    }
    if ( JoystickInfo[ i ].feedback )
    {
      free( JoystickInfo[ i ].feedback );
      JoystickInfo[ i ].feedback = NULL;
    }
  }

    if (lpdi)
  {
    IDirectInputDevice_Release(lpdi);
    lpdi     = NULL;
  }

  return TRUE;
}


void ProcessGameKeys( void )
{

  static int16 startpos = 0;

  // if were not (!) in (normal,single,attract or demo) modes
  // then don't listen to keys...
  if ( ! (
           ( MyGameStatus == STATUS_Normal       ) ||
           ( MyGameStatus == STATUS_SinglePlayer ) ||
           ( MyGameStatus == STATUS_PlayingDemo  ) ||
           ( MyGameStatus == STATUS_AttractMode  )
     )   )
  {
    return;
  }

  // ESCAPE
  if ( IsKeyPressed( DIK_ESCAPE ) )
    // if were not currently in a menu
    //   ignore keystroke if we just exited the menu
    //   stops going back itno menu from hitting to many times
    if ( !CurrentMenu && !JustExitedMenu )
    {
      // if were playing a demo
      if( PlayDemo )
        // show the demo menu
        MenuRestart( &MENU_DemoPlaying );
      // if were playing the game
      else
        // show the game menu
        MenuRestart( &MENU_InGame );
    // if were not in the menu
    // and the above check failed
    // reset variable so 3rd escape takes us back in
    }else
      JustExitedMenu = FALSE;


  // debuggin keys
  if ( DebugInfo ) {

    // Shift Modifier
    if ( IsKeyHeld( DIK_LSHIFT ) || IsKeyHeld( DIK_RSHIFT ) )
    {

      // Shift + F1
      if ( IsKeyPressed( DIK_F1 ) )
        if ( !CurrentMenu )
          MenuRestart( &MENU_DebugHelp );
        else if ( CurrentMenu == &MENU_DebugHelp )
          MenuExit();

      // Shift + Ctrl Modifier
      if ( IsKeyHeld( DIK_LCONTROL ) || IsKeyHeld( DIK_RCONTROL ) )
      {

        // Shift + Ctrl + F2
        if ( IsKeyPressed( DIK_F2 ) )
          SetFOV( chosen_fov = normal_fov );
        
        // Shift + Ctrl + F3
        if ( IsKeyPressed( DIK_F3 ) )
          SetFOV( chosen_fov = normal_fov );

      } // end shift + ctrl modifier
      else // shift only modifier
      {

        // Shift + F2
        if ( IsKeyPressed( DIK_F2 ) )
          SetFOV( chosen_fov = FOV_GROW( hfov ) );

        // Shift + F3
        if ( IsKeyPressed( DIK_F3 ) )
          SetFOV( chosen_fov = FOV_SHRINK( hfov ) );

      } // end shift only modifier

      // Shift + F4
      if ( IsKeyPressed( DIK_F4 ) )
        if ( !CurrentMenu )
          MenuRestart( &MENU_GotoRoom );

      // Shift + F5
      if ( IsKeyPressed( DIK_F5 ) )
      {
        InitShipStartPos( WhoIAm, ++startpos );
        AddMessageToQue(
          "You are now in %s\n",
          Mloadheader.Group[ Ships[ WhoIAm ].Object.Group ].name
        );
      }

      // Shift + F6
      if ( IsKeyPressed( DIK_F6 ) )
        DoClipping = !DoClipping;

      // Shift + F7
      if ( IsKeyPressed( DIK_F7 ) )
        ShowBoundingBoxes = !ShowBoundingBoxes;

      // Shift + F8
      if ( IsKeyPressed( DIK_F8 ) )
        ShowEFZones = !ShowEFZones;

      // Shift + F9
      if ( IsKeyPressed( DIK_F9 ) )
        if( ShowPortal == 4 )
          ShowPortal = 0;
        else
          ShowPortal++;

      // Shift + F10
      if ( IsKeyPressed( DIK_F10 ) )
        ShowColZones = !ShowColZones;

#ifdef POLYGONAL_COLLISIONS

      // Shift + F11
      if ( IsKeyPressed( DIK_F11 ) )
        if( ShowSkin == 6 )
          ShowSkin = 0;
        else
          ShowSkin++;

#endif

      // Shift + F12
      if( IsKeyPressed( DIK_F12 ) )
        no_collision = !no_collision;

    } // end shift modifier
    // CTRL Modifiers
    else if ( IsKeyHeld( DIK_LCONTROL ) || IsKeyHeld( DIK_RCONTROL ) )
    {

      // Ctrl + F1
      if ( IsKeyPressed( DIK_F1 ) )
        ShowTeleports = !ShowTeleports;
    
      // Ctrl + F2
      if ( IsKeyPressed( DIK_F2 ) )
        ShowTrigZones = !ShowTrigZones;

    } // end ctrl modifier
    else // no modifiers
    {

      // F8
      if ( IsKeyPressed( DIK_F8 ) )
        // wtf is this ?
        if( TargetComputerOn )
          TargetComputerOn = FALSE;
        else
          TargetComputerOn = TRUE;
    
    }

  } // end debug keys
  else // non debugging mode keys
  {

    // F1
    if ( IsKeyPressed( DIK_F1 )  )
      if ( !CurrentMenu )
        MenuRestart( &MENU_Controls );
      else if ( CurrentMenu == &MENU_Controls )
        MenuExit();

    // F2
    if ( IsKeyPressed( DIK_F2 )  )
      if ( !CurrentMenu )
        MenuRestart( &MENU_Options );
      else if ( CurrentMenu == &MENU_Options )
        MenuExit();

    // single player
    if ( MyGameStatus == STATUS_SinglePlayer )
    {

      // F3
      if ( IsKeyPressed( DIK_F3 ) ) 
        // check for quick load/save only in single player
        if ( !CurrentMenu )
          MenuRestart( &MENU_LoadSavedGame );
        else if ( CurrentMenu == &MENU_LoadSavedGame )
          MenuExit();
  
      // F4        
      if ( IsKeyPressed( DIK_F4 ) )
        // quick save
        if ( !CurrentMenu )
          MenuRestart( &MENU_SaveGame );
        else if ( CurrentMenu == &MENU_SaveGame )
          MenuExit();

    } // end single player

    // F5
    if ( IsKeyPressed( DIK_F5 ) )
      MissileCameraEnable ^=1;

    // not single player mode
    // and Shift Modifier
    if ( ( MyGameStatus != STATUS_SinglePlayer ) &&
         ( IsKeyHeld( DIK_LSHIFT ) || IsKeyHeld( DIK_RSHIFT ) ) )
    {

      // Shift + F6
      if ( IsKeyPressed( DIK_F6 ) )
        PingNonGuarenteed();

      // Shift + F7
      if ( IsKeyPressed( DIK_F7 ) )
        PingGuarenteed();
      
    } // not single player and shift modifier end
    else // single player or shift modifier
    {

      // F6
      if ( IsKeyPressed( DIK_F6 ) )
        RearCameraActive ^=1;
  
      // F7
      if ( IsKeyPressed( DIK_F7 ) )
        // toggle panel text
        Panel = !Panel;

    } // single player or shift modifier end

#ifdef SAVESCREEN_3DFX
    // capture a screen shot
    if ( IsKeyPressed( DIK_F8 ) && ScreenSaving )
    {
      sprintf( fname, ".\\ScreenShots\\screen%d.ppm", fnum++ );
      // wtf is this ?
      // _spawnl( _P_WAIT, "grab.exe", "grab.exe", "-fp" , "-w 512" , "-h 384" , fname, NULL );
      folder_exists( SNAPSHOT_FOLDER );
      SaveFullScreenSnapShot( fname );
    }
#endif

    // single player mode
    if( MyGameStatus == STATUS_SinglePlayer )
    {

      // F9
      if ( IsKeyPressed( DIK_F9 ) )
        LastTextMessage();

      // F10
      if ( IsKeyPressed( DIK_F10 ) )
        NextTextMessage();

    } // end single player mode
    else // not single player mode
    {

      // Shift Modifier
      if ( ( IsKeyHeld( DIK_LSHIFT ) || IsKeyHeld( DIK_RSHIFT ) ) )
      {

        // Shift + F9
        if ( IsKeyPressed( DIK_F9 ) )
          if ( !CurrentMenu )
            MenuRestart( &MENU_EditMacro1 );
          else if ( CurrentMenu == &MENU_EditMacro1 )
            MenuExit();

        // Shift + F10
        if ( IsKeyPressed( DIK_F10 ) )
          if ( !CurrentMenu )
            MenuRestart( &MENU_EditMacro2 );
          else if ( CurrentMenu == &MENU_EditMacro2 )
            MenuExit();

        // Shift + F11
        if ( IsKeyPressed( DIK_F11 ) )
          if ( !CurrentMenu )
            MenuRestart( &MENU_EditMacro3 );
          else if ( CurrentMenu == &MENU_EditMacro3 )
            MenuExit();

      } // Shift Modifier
      else // no modifier
      {

        // F9
        if ( IsKeyPressed( DIK_F9 ) )
          SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt1, 0);

        // F10
        if ( IsKeyPressed( DIK_F10 ) )
          SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt2, 0);

        // F11
        if ( IsKeyPressed( DIK_F11 ) )
          SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt3, 0);

      } // end (not) shift modifier
    } // end not single player

#ifndef POLYGONAL_COLLISIONS
#ifdef REMOTE_CAMERA_ENABLED

	// F12
	if ( IsKeyPressed( DIK_F12 ) )
	  if( !RemoteCameraActive )
	  {
		RemoteCameraActive = TRUE;
		Ships[MAX_PLAYERS].Object.Pos = Ships[WhoIAm].Object.Pos;
		Ships[MAX_PLAYERS].Object.Group = Ships[WhoIAm].Object.Group;
		Ships[MAX_PLAYERS].Object.Mode = NORMAL_MODE;
		Ships[MAX_PLAYERS].enable = 1;
		Current_Camera_View = MAX_PLAYERS;    // which object is currently using the camera view....
	  }else{
		RemoteCameraActive = FALSE;
		Ships[MAX_PLAYERS].enable = 0;
		Current_Camera_View = WhoIAm;   // which object is currently using the camera view....
	  }

#endif
#endif

  } // end debug/normal keys
} // ProcessGameKeys



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Handle all things keys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
int My_Key_Up ( WPARAM wParam , LPARAM lParam )
{
  switch( wParam )
  {
  case VK_PAUSE:
      return 2; // signal pause
      break;
  }
  return 0;
}


void My_Key_Down ( WPARAM wParam , LPARAM lParam )
{
  switch( wParam )
  {
    case VK_ESCAPE:
    case VK_F12:
      break;
    }
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Set the View Matrix
  Input   :   nothing...  Current_Camera_View must be set
                  to a legal ship number...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

void Build_View()
{
  VECTOR  Trans;
  VECTOR  Trans2;

  
  Trans.x = -CurrentCamera.Pos.x;
  Trans.y = -CurrentCamera.Pos.y;
  Trans.z = -CurrentCamera.Pos.z;

  ApplyMatrix( &CurrentCamera.InvMat, &Trans, &Trans2 );      /* Set Translation */

  view._11 = CurrentCamera.Mat._11; 
  view._12 = CurrentCamera.Mat._12;
  view._13 = CurrentCamera.Mat._13;
  view._14 = CurrentCamera.Mat._14;
             
  view._21 = CurrentCamera.Mat._21;
  view._22 = CurrentCamera.Mat._22;
  view._23 = CurrentCamera.Mat._23;
  view._24 = CurrentCamera.Mat._24;
             
  view._31 = CurrentCamera.Mat._31;
  view._32 = CurrentCamera.Mat._32;
  view._33 = CurrentCamera.Mat._33;
  view._34 = CurrentCamera.Mat._34;
                                        
  view._41 = Trans2.x;
  view._42 = Trans2.y;
  view._43 = Trans2.z;
  view._44 = CurrentCamera.Mat._44;
}


char TriggerName[256];

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Inits And Loads New Level..
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ChangeLevel( void )
{
  //if( NewLevelNum != LevelNum )
  //{
    LevelNum = NewLevelNum;

//    NumGoldBars = 0;
    
    ReleaseLevel();
    KillNodeCubeLines();
    KillBoxLines();

    NextworkOldKills = -1;
    NextworkOldDeaths = -1;
    NextworkOldBikeNum = -1;

    ActiveRemoteCamera = NULL;
    MissileCameraActive = 0;
    CameraMissile = (uint16) -1;

    DPlayGetSessionDesc();

    if (glpdpSD && glpDP )
    {
      // call LevelCheckSum init
      InitCheckSum( &glpdpSD->guidInstance );
    }else{
      LevelCheckSum = 0;
    }

    InitScene();
    InitTextMessages();
    InitMultiSfxHandle();

    OnceOnlyChangeLevel = TRUE;
    ChangeLevel_MyGameStatus = MyGameStatus;
    Change_Ext( &LevelNames[ LevelNum ][ 0 ], &TriggerName[ 0 ], ".TRG" );
    if( !Triggerload( TriggerName ) )
    {
      SeriousError = TRUE;
      return( FALSE );
    }

    MyGameStatus = STATUS_ChangeLevelPostInitView;
    InitView();
  //}
  return( TRUE );
}

void DrawLoadingBox( int current_loading_step, int current_substep, int total_substeps )
{
  HRESULT hr;
  DDBLTFX fx;
  RECT    dest, darkgreen, lightgreen;
  float xmin, xmax, ymin, ymax, loaded, one_step;
  int total_loading_steps = 11;
  int BarXMin = 20;
  int BarXMax = 180;
  int BarYMin = 155;
  int BarYMax = 160;
  float BorderX, BorderY;

  // if not in mission loading screen ( buffer flips disabled ), do not display loading status
  if ( !PreventFlips )
    return;

//  BorderX = (float)floor(ModeScaleX[ModeCase]);
//  BorderY = (float)floor(ModeScaleY[ModeCase]);
  BorderX = 1.0F;
  BorderY = 1.0F;

  if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
  {
    //coords for high res font...
    darkgreen.left = 10;
    darkgreen.right = 11;
    darkgreen.top = 194;
    darkgreen.bottom = 195;

    lightgreen.left = 50;
    lightgreen.right = 51;
    lightgreen.top = 194;
    lightgreen.bottom = 195;

  }else
  {
    //coords for low res font...
    darkgreen.left = 2;
    darkgreen.right = 3;
    darkgreen.top = 122;
    darkgreen.bottom = 123;

    lightgreen.left = 10;
    lightgreen.right = 11;
    lightgreen.top = 122;
    lightgreen.bottom = 123;
  }


  if ( !BorderX)
    BorderX = 1.0F;
  if ( !BorderY)
    BorderY = 1.0F;

  memset( &fx, 0, sizeof( DDBLTFX ) );
  fx.dwSize = sizeof( DDBLTFX );

  xmin = (BarXMin + VDUoffsetX) * ModeScaleX[ModeCase];
  xmax = (BarXMax + VDUoffsetX) * ModeScaleX[ModeCase];
  ymin = (BarYMin + VDUoffsetY) * ModeScaleY[ModeCase];
  ymax = (BarYMax + VDUoffsetY) * ModeScaleY[ModeCase];

  loaded = (float)current_loading_step / (float)total_loading_steps;
  xmax = ( ( xmax - xmin ) * loaded ) + xmin;

  loaded = (float)current_substep / (float)total_substeps;

  one_step = (float)( xmax - xmin ) / (float)total_loading_steps;
  one_step *= loaded;
  xmax += one_step;

  // specify co-ordinates to plot to...
  dest.left = (LONG)xmin;
  dest.right = (LONG)xmax;
  dest.top = (LONG)ymin;
  dest.bottom = (LONG)ymax;

  //DebugPrintf("blitted to l:%d r:%d t:%d b:%d\n", dest.left, dest.right, dest.top, dest.bottom );
  
  hr = d3dapp->lpFrontBuffer->lpVtbl->Blt( d3dapp->lpFrontBuffer, &dest, lpDDSTwo, &darkgreen, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
  if ( hr != DD_OK )
  {
    DebugPrintf("%s", D3DAppErrorToString(hr));
  }

  // top...
  dest.left = (LONG) ( (BarXMin + VDUoffsetX) * ModeScaleX[ModeCase]) ;
  dest.right = (LONG)((BarXMax + VDUoffsetX) * ModeScaleX[ModeCase]);
  dest.top = (LONG)((BarYMin - BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  dest.bottom = (LONG)((BarYMin + VDUoffsetY) * ModeScaleY[ModeCase]);
  hr = d3dapp->lpFrontBuffer->lpVtbl->Blt( d3dapp->lpFrontBuffer, &dest, lpDDSTwo, &lightgreen, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
  // bottom...
  dest.left = (LONG) ( (BarXMin + VDUoffsetX) * ModeScaleX[ModeCase]) ;
  dest.right = (LONG)((BarXMax + VDUoffsetX) * ModeScaleX[ModeCase]);
  dest.top = (LONG)((BarYMax + VDUoffsetY) * ModeScaleY[ModeCase]);
  dest.bottom = (LONG)((BarYMax + BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  hr = d3dapp->lpFrontBuffer->lpVtbl->Blt( d3dapp->lpFrontBuffer, &dest, lpDDSTwo, &lightgreen, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
  
  // left...
  dest.left = (LONG) ( (BarXMin - BorderX + VDUoffsetX) * ModeScaleX[ModeCase]) ;
  dest.right = (LONG)((BarXMin + VDUoffsetX) * ModeScaleX[ModeCase]);
  dest.top = (LONG)((BarYMin - BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  dest.bottom = (LONG)((BarYMax + BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  hr = d3dapp->lpFrontBuffer->lpVtbl->Blt( d3dapp->lpFrontBuffer, &dest, lpDDSTwo, &lightgreen, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
  // right...
  dest.left = (LONG) ( (BarXMax + VDUoffsetX) * ModeScaleX[ModeCase]) ;
  dest.right = (LONG)((BarXMax + BorderX + VDUoffsetX) * ModeScaleX[ModeCase]);
  dest.top = (LONG)((BarYMin - BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  dest.bottom = (LONG)((BarYMax + BorderY + VDUoffsetY) * ModeScaleY[ModeCase]);
  hr = d3dapp->lpFrontBuffer->lpVtbl->Blt( d3dapp->lpFrontBuffer, &dest, lpDDSTwo, &lightgreen, DDBLT_KEYSRC  | DDBLT_WAIT, &fx );
}

static void UpdateKillsTime( void )
{ 
  int i;

  // update session description with current leading kills & elapsed time...
  if ( !glpdpSD )
  {
    DPlayGetSessionDesc();
  }

  if ( glpdpSD )
  {
    uint16 highest_score = 0;
    uint16 minutes_left, old_minutes_left, old_highest_score;
    
    // if current level elapsed time is more than 1 min than stored elapsed time, store in SD
    minutes_left = ( uint16 ) ( ( Countdown_Float / 100.0F ) / 60.0F );
    if ( minutes_left > 30 )
      minutes_left = 30;
    old_minutes_left = ( uint16 ) ( ( glpdpSD->dwUser3 & CurrentGameTimeBits ) >> CurrentGameTime_Shift );

    if ( minutes_left != old_minutes_left )
    {
      glpdpSD->dwUser3 &= ~CurrentGameTimeBits;
      glpdpSD->dwUser3 |=  ( minutes_left << CurrentGameTime_Shift );

      if ( DPlaySetSessionDesc( 1 ) != DP_OK )
      {
        DebugPrintf("RenderScene(): Unable to set session desc!!\n");
      }
    }

    // find highest score
    if ( !TeamGame )
    {
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if ( ( i == WhoIAm ) || ( GameStatus[ i ] == STATUS_Normal ) )
        {
          if ( Ships[ i ].Kills > highest_score )
          {
            highest_score = Ships[ i ].Kills;
          }
        }
      }
    }else
    {
      uint16 current_team_score[ MAX_TEAMS ];

      memset( current_team_score, 0, sizeof( uint16 ) * MAX_TEAMS );

      for ( i = 0; i < MAX_PLAYERS; i++ )
      {
        if ( ( i == WhoIAm ) || ( GameStatus[ i ] == STATUS_Normal ) )
        {
          current_team_score[ TeamNumber[ i ] ] += Ships[ i ].Kills;
        }
      }

      for ( i = 0; i < MAX_TEAMS; i++ )
      {
        if ( current_team_score[ i ] > highest_score )
        {
          highest_score = current_team_score[ i ];
        }
      }
    }

    if ( highest_score > 255 )
      highest_score = 255;

    old_highest_score = ( uint16 ) ( ( glpdpSD->dwUser2 & CurrentMaxKillsBits ) >> CurrentMaxKills_Shift );

    if ( old_highest_score != highest_score )
    {
      glpdpSD->dwUser2 &= ~CurrentMaxKillsBits;
      glpdpSD->dwUser2 |= ( highest_score << CurrentMaxKills_Shift );

      if ( DPlaySetSessionDesc( 1 ) != DP_OK )
      {
        DebugPrintf("RenderScene(): Unable to set session desc!!\n");
      }
    }
  }
}

void GetLevelName( char *buf, int bufsize, int level )
{
  FILE *f;
  char *pCh;
  
  buf[ 0 ] = 0;

  if ( level < 0 )
    return;
  
  f = fopen( MissionTextNames[ level ], "r" );
  if ( f )
  {
    // read level name
    if ( !fgets( buf, bufsize, f ) )
    {
        // if error ( as oposed to end of file )
      if ( ferror( f ) != 0 )
      {
        buf[ 0 ] = 0;
      }
    }
  }
  if ( !buf[ 0 ] )
  {
    strncpy( buf, LevelList.item[ level ], bufsize );
  }

  pCh = strchr( buf, '\n' );
  if ( pCh )
    *pCh = 0;
  
  buf[ bufsize - 1 ] = 0; // ensure null terminated 
}

void StoreLevelNameInSessionDesc( char *str )
{
  char buf[ MAX_LEVEL_NAME_LENGTH + MAX_SESSION_DESC_LENGTH + 1 ];  // extra 1 for '~' char
  char *pCh;
  
  DPlayGetSessionDesc();
  if ( glpdpSD )
  {
    strncpy( buf, glpdpSD->lpszSessionNameA, MAX_SESSION_DESC_LENGTH );
    buf[ MAX_SESSION_DESC_LENGTH - 1 ] = 0; // ensure null terminated 

    pCh = strchr( buf, '~' );
    if ( pCh )
    {
      pCh++;
    }else
    {
      pCh = buf + strlen( buf );
      *pCh++ = '~';
    }

    strncpy( pCh, str, MAX_LEVEL_NAME_LENGTH );

    buf[ MAX_LEVEL_NAME_LENGTH + MAX_SESSION_DESC_LENGTH ] = 0; // ensure null terminated 

    UpdateSessionName( buf );
  }
}

float Browl = 0.0F;
float QuitTime = 0.0F;
float HostMultiPlayerTimeout;

int colourflash = 0;
char NodeName[256];
BOOL CreditsToggle = FALSE;

extern LPDPLCONNECTION glpdplConnection;
BOOL SetZCompare( void );

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Game Status Control...
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
RenderScene(LPDIRECT3DDEVICE Null1, LPDIRECT3DVIEWPORT Null2 )
{
  uint16  i,e,o;
  char  buf[256];
  char full_level_name[ MAX_LEVEL_NAME_LENGTH ];
    LPDIRECTDRAWPALETTE ddpal;
  LPDIRECTDRAW lpDD = d3dapp->lpDD;
  LPDIRECT3D lpD3D = d3dapp->lpD3D;
  LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
  LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;
//  struct _stat stat_buf;
//  int result;
  static int WaitFrames = 2;
  BOOL done;
  DWORD   Temp;
  BYTE    msg;
  int     size;
  LONGLONG  TempTime;
  HRESULT hr;
  LONGLONG  Time_Freq;
  LONGLONG  Time_Value;
  float   Time_Float;
  float   time_diff;

  if ( SeriousError )
    return FALSE;

//  if ( !Bsp_Identical( &Bsp_Header[ 0 ], &Bsp_Original ) )
//  {
//    hr = 0;
//  }

  CalculateFramelag();
  AnimOncePerFrame++;

#if 0
  QuitTime -= framelag;
  if( QuitTime <= 0.0F )
  {
    QuitTime = 60.0F * 5.0F;
    result = _stat( "quit.bat", &stat_buf );
    if( result == 0 )
    {
      // file exists..
      quitting = TRUE;
      return FALSE;
    }
  }
#endif

  if ( bSoundEnabled )
  {
    EnterCriticalSection (&SfxKey);
  
    CheckSBufferList();
  
    LeaveCriticalSection (&SfxKey);
  }

#ifndef MULTIPLE_READINPUTS
  ReadInput();
#endif

  switch( MyGameStatus )
  {
  case STATUS_TryingToFindSession:
    D3DAppClearScreenOnly();

      if ( GeneralTimeout( &FindSessionTimeout ) )
    {
      PrintErrorMessage( CANNOT_FIND_SESSION, 0, NULL, ERROR_QUIT );
      MyGameStatus = STATUS_Title;
      break;
    }

    // search for matching session
    SessionsList.selected_item = -1;
    for ( i = 0; i < SessionsList.items; i++ )
    {
      if ( IsEqualGuid( &Sessions[ i ].guidInstance, &autojoin_session_guid ) )
      {
        SessionsList.selected_item = i;
        break;
      }
    }
    if( (SessionsList.selected_item != -1) && SessionsRefresh[SessionsList.selected_item] )
    {
      SelectSession( NULL );
    }
    else
    {
      CenterPrint4x5Text( SEARCHING_FOR_GAME , (d3dappi.szClient.cy>>1) , 2 );
      GetCurrentSessions_ReScan( NULL );
    }
    break;
  case STATUS_QuittingFromTitles:
    quitting = TRUE;
    if ( IsLobbyLaunched )
    {
      if ( IsHost )
      {
        free( glpdplConnection );
        glpdplConnection = NULL;
        free( glpdpSD );
        glpdpSD = NULL;
      }
    }
    break;
  case STATUS_QuittingFromInGame:
    StopCompoundSfx();
    OutputVisiStats( &Mloadheader, LevelNames[ LevelNum ] );
    MenuAbort();
    DestroyGame();  // DestroyGame changes MyGameStatus..So Set it back
    MyGameStatus = STATUS_QuittingFromInGame;
    ReleaseLevel();
    quitting = TRUE;
    break;
  case STATUS_SplashScreen:
    
    if ( NewSplashScreens[ NewCurrentSplashScreen ].flags & SPLASH_Timed )
    {
      DWORD currenttime = timeGetTime();
      
      if ( currenttime > SplashFinishTime )
      {
        MyGameStatus = STATUS_FinishedShowingSplashScreen;
        break;
      }
    }

    if ( NewSplashScreens[ NewCurrentSplashScreen ].flags & SPLASH_Return )
    {
      if ( AnyKeyPressed() )
      {
        MyGameStatus = STATUS_FinishedShowingSplashScreen;
        break;
      }
    }

    if ( NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->SplashDisplayFunc )
    {
      if ( !NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->SplashDisplayFunc( NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->SplashDisplayVar ) )
      {
        MyGameStatus = STATUS_FinishedShowingSplashScreen;
      }
    }
      
    break;
    
  case STATUS_FinishedShowingSplashScreen:
       
    if ( NewSplashScreens[ NewCurrentSplashScreen ].flags & SPLASH_ShowNext )
    {
      // call post splash function
      if ( NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PostSplashFunc )
        NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PostSplashFunc( NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PostSplashVar );

      // increment splash screen
      NewCurrentSplashScreen++;

      MyGameStatus = STATUS_SplashScreen;

      // set finish time if required
      if ( NewSplashScreens[ NewCurrentSplashScreen ].flags & SPLASH_Timed )
      {
        SplashFinishTime = timeGetTime() + NewSplashScreens[ NewCurrentSplashScreen ].time;
      }

      // call initialization function
      if ( NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PreSplashFunc )
      {
        if ( !NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PreSplashFunc( NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PreSplashVar ) )
        {
          MyGameStatus = STATUS_FinishedShowingSplashScreen;
        }
      }
    }
    else
    {
      // finished current batch of splash screens...

      // restore old game status
      MyGameStatus = PreSplash_MyGameStatus; 

      // restore old menu status
      CurrentMenu = PreSplash_Menu;
      CurrentMenuItem = PreSplash_MenuItem;

      // call post splash function  ( now that game status has been changed back )
      if ( NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PostSplashFunc )
        NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PostSplashFunc( NewSplashScreens[ NewCurrentSplashScreen ].splashinfo->PostSplashVar );

    }
    
    break;
  case STATUS_WaitingToStartSinglePlayer:
    
    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    
    done = VduFinished( &MENU_NEW_BetweenLevels );
      
    if ( done )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
          PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

      StartASinglePlayerGame( NULL );
    }
    
    break;
  case STATUS_WaitingToStartDemo:
    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    done = VduFinished( &MENU_NEW_StartAttractMode );
      
    if ( done )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
          PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      LastMenu = CurrentMenu;
      VduClear();
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
      ReleaseView();
      MyGameStatus = STATUS_Title;

      if( !CreditsToggle )
      {
        ShowSplashScreen( SPLASHSCREEN_AttractMode );
      }
      else
      {
        ShowSplashScreen( SPLASHSCREEN_Credits );
        SetupCredits();
      }
    }
    break;
  case STATUS_WaitingToStartEndGameSequence:
    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    done = VduFinished( &MENU_NEW_GeneralLoading );
      
    if ( done )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
          PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

      MyGameStatus = STATUS_Title;
      ReleaseView();

      switch( GameCompleted )
      {
      case GAMECOMPLETE_WithoutAllCrystals:
      case GAMECOMPLETE_WithAllCrystalsExceptLast:
        SetupGameCompleteCredits();
        ShowSplashScreen( SPLASHSCREEN_GameComplete );
        break;
      case GAMECOMPLETE_WithAllCrystals:
        SetupGameCompleteCredits();
        ShowSplashScreen( SPLASHSCREEN_SpecialGameComplete );
        break;
      default:
        SetupGameCompleteCredits();
        ShowSplashScreen( SPLASHSCREEN_GameComplete );
      }
    }
    break;
  case STATUS_WaitingToStartMultiPlayerHost:
    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      if( dcoID )
      {
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      }
    }
    ReceiveGameMessages();

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    if ( VduFinished( NULL ) )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
      MyGameStatus = STATUS_StartingMultiplayer;
      GoToSynchup( NULL );
    }
    break;
  case STATUS_WaitingToStartMultiPlayerClient:
    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      if( dcoID )
      {
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      }
    }
    ReceiveGameMessages();

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    if ( VduFinished( NULL ) )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      LastMenu = CurrentMenu;
      VduClear();
      
      PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

      MenuAbort();
      ReleaseView();
      LevelNum = -1;

      MyGameStatus = STATUS_PreStartingMultiplayerSynch;
      if( !ChangeLevel() ) 
        return( FALSE );
    }
    break;
  case STATUS_WaitingToStartTeamGame:
    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      if( dcoID )
      {
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      }
    }
    ReceiveGameMessages();

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    if ( VduFinished( NULL ) )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      LastMenu = CurrentMenu;
      VduClear();
      
      MenuAbort();

      PreventFlips = TRUE;
      CurrentLoadingStep = 1;
      DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

      MyGameStatus = PreSynchupStatus;
    }

    if( IMustQuit )
      SpecialDestroyGame();

    break;
  case STATUS_Title:
  case STATUS_BetweenLevels:
  case STATUS_Copyright:
    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    break;

  case STATUS_Normal:
    
    PreventFlips = FALSE;




    if ( !CheatsDisabled )
      DisableCheats();
  
    if( IMustQuit )
      SpecialDestroyGame();

    if( IsServer )
    {
      if( ServerMainGame( lpDev , lpView ) != TRUE )
        return FALSE;
    }else{
      if( MainGame( lpDev , lpView ) != TRUE )
        return FALSE;
    }

    if ( IsHost )
    {
      UpdateKillsTime();
      UpdatePlayerInfo();
    }

    
    if ( IsServer )
    {
      if ( !CheckPlayersActive() )
      {
        // no players active....reset back to titles
        QuickStart = QUICKSTART_Server;
        SelectQuitCurrentGame( NULL );
        PreventFlips = FALSE;
      }
    }


    if ( bSoundEnabled )
    {
      ProcessLoopingSfx();
#ifdef PLAYER_SPEECH_TAUNTS
      ProcessTaunt();
#endif
    }

    LevelTimeTaken += framelag;

    // if player is quiting nothing should stop him.....
    if( ( MyGameStatus != STATUS_QuitCurrentGame ) && ( MyGameStatus != STATUS_QuittingFromInGame ) )
    {
      if( IsHost )
      {
        if( NewLevelNum != LevelNum )
        {
          // disable players joining...
          DPlayGetSessionDesc();
          
          //glpdpSD->dwFlags = DPSESSION_MIGRATEHOST | DPSESSION_KEEPALIVE | DPSESSION_NEWPLAYERSDISABLED | DPSESSION_JOINDISABLED;
          glpdpSD->dwFlags |= DPSESSION_NEWPLAYERSDISABLED | DPSESSION_JOINDISABLED;

          DPlaySetSessionDesc(0);

          for( i = 0 ; i < MAX_PLAYERS ; i++ )
          {
            if( (GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && (GameStatus[i] != STATUS_Null ) && i != WhoIAm )
            {
              if( GameStatus[i] != STATUS_Normal )
              {
                SendGameMessage(MSG_YOUQUIT, 0, (BYTE) i , 0, 0);
              }
            }
          }
          // the level has ended or changed ..tell everyone to go onto new level
          MyGameStatus = STATUS_LevelEnd;
          GameStatus[WhoIAm] = MyGameStatus;
          RandomStartPosModify += 1;
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
          ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
          ServiceBigPacket(TRUE);
          ServiceServer( TRUE );

        }
      }else{
        if( OverallGameStatus == STATUS_LevelEnd )
        {
          // tell the host that I am now in the same state as him....
          MyGameStatus = OverallGameStatus;
          GameStatus[WhoIAm] = MyGameStatus;
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
        }
      }
    }

    if ( WaitingToQuit )
    {
      D3DAppClearScreenOnly();
      CenterPrint4x5Text( PLEASE_WAIT, (d3dappi.szClient.cy>>1)-(FontHeight>>1) , 2 );
      SelectQuitCurrentGame( NULL );
    }
    break;
  case STATUS_LevelEnd:
    D3DAppClearScreenOnly();

    ReceiveGameMessages();
    Browl -= framelag;

    StopDemoRecording();

    if( Browl <= 0.0F )
    {
      if( IsHost )
      {
        if( dcoID )
        {
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
        }
      }else{
        if( dcoID )
        {
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
        }
      }
      Browl = 30.0F;
    }
    if( IsHost )
    {

      // have all the enabled players switched over to STATUS_LevelEnd??
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( ( Ships[i].enable != 0 ) && ( GameStatus[i] != STATUS_LevelEnd ) && ( i != WhoIAm ) )
          break;
      }
      if( i != MAX_PLAYERS )
        break;  // not yet..
      OutputVisiStats( &Mloadheader, LevelNames[ LevelNum ] );
      // un load all graphics
      ReleaseView();
      ReleaseLevel();
      // tell them all they can now look at the scores...
      MyGameStatus = STATUS_ViewingScore;
      GameStatus[WhoIAm] = MyGameStatus;
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
      ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
      ServiceBigPacket(TRUE);
      ServiceServer( TRUE );

      InitScene();  // STATSTEST
      InitView();
      NextworkOldKills = -1;
      NextworkOldDeaths = -1;
      NextworkOldBikeNum = -1;
      HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F * 2.0F;
    }else{
      if( MyGameStatus != OverallGameStatus )
      {
        OutputVisiStats( &Mloadheader, LevelNames[ LevelNum ] );
        // un load all graphics
        ReleaseView();
        ReleaseLevel();
        // tell the host that I am now in the same state as him....
        MyGameStatus = STATUS_ViewingScore;

        GameStatus[WhoIAm] = MyGameStatus;
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

        InitScene();  // STATSTEST
        InitView();
        NextworkOldKills = -1;
        NextworkOldDeaths = -1;
        NextworkOldBikeNum = -1;
        HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F * 2.0F;
      }
    }
    break;
  case STATUS_ViewingScore:

    ReceiveGameMessages();

    Browl -= framelag;
    if( Browl <= 0.0F )
    {
      if( IsHost )
      {
        if( dcoID )
        {
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
        }
      }else{
        if( dcoID )
        {
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
        }
      }
      Browl = 30.0F;
    }

    ScoreDisplay();
#ifdef MULTIPLE_READINPUTS
    ReadInput();
#endif

    HostMultiPlayerTimeout -= framelag;

    if( IsKeyPressed( DIK_SPACE ) ||
      ( OverallGameStatus == STATUS_WaitingAfterScore ) ||
      ( IsServer && ( HostMultiPlayerTimeout < ( (60.0F * 60.0F * 2.0F * 2.0F) - ( 30.0F * 60.0F ) ) ) )
      )
    {
      D3DAppIClearBuffers();
      if( !IsServer )
        HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F;
      else
        HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F * 2.0F;

      if( IsHost )
      {
        // store full level name in session desc
        GetLevelName( full_level_name, MAX_LEVEL_NAME_LENGTH, NewLevelNum );
        StoreLevelNameInSessionDesc( full_level_name );
        RegSetA( "LevelName",  (LPBYTE)LevelList.item[ NewLevelNum ] , sizeof( LevelList.item[ NewLevelNum ] ) );

//        if ( IsLobbyLaunched )
//        {
//          MyGameStatus = STATUS_QuittingFromInGame;
//        }else
        {
          // tell them all I Am waiting
          ReleaseView();
          MyGameStatus = STATUS_WaitingAfterScore;
          GameStatus[WhoIAm] = MyGameStatus;
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
          ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
          ServiceBigPacket(TRUE);
          ServiceServer( TRUE );
          if( !ChangeLevel() ) return( FALSE );
        }
      }else{
//          if ( IsLobbyLaunched )
//          {
//            MyGameStatus = STATUS_QuittingFromInGame;
//          }else
        {
          // tell the host that I am now Waiting for him to finish viewing the score....
          ReleaseView();
          MyGameStatus = STATUS_WaitingAfterScore;
          GameStatus[WhoIAm] = MyGameStatus;

          if( !ChangeLevel() ) return( FALSE );
        }
      }
    }
        break;
  case STATUS_WaitingAfterScore:

    InitFontTransTable( TRUE );
    D3DAppClearScreenOnly();
    e = 0;

    if( IsHost )
    {
      Printuint16( (uint16) (HostMultiPlayerTimeout / 60.0F) , 0 , 0 , 2 );
      HostMultiPlayerTimeout -= framelag;
    }

    if( DS )
    {
      DisplayStatusMessages();
    }else{

      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( MaxKills || ResetKillsPerLevel )
        {
          Ships[i].Kills = 0;
          Ships[i].Deaths = 0;
          for( o = 0 ; o < MAX_PLAYERS ; o++ )
          {
            Stats[i][o] = 0;
          }
        }
        
        if( (GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && (GameStatus[i] != STATUS_Null ) )
        {

  //        if( GameStatus[i] == STATUS_ViewingScore )
  //        {
  //          wsprintf( buf, "%-8s status %8s\n", &Names[i][0] , "viewing score" );
  //          CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy>>1)-( ( (FontHeight+2) * MAX_PLAYERS ) >> 1 )+ (e * (FontHeight+(FontHeight>>1)) ) , GameStatus[i] == STATUS_StartingMultiplayerSynch ? 2 : 1 );
  //        }
  //        else
          {
            wsprintf( buf, "%-8s status %14s\n", &Names[i][0] , &StatusTab[ GameStatus[i] ][0] );
            CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy>>1)-( ( (FontHeight+2) * MAX_PLAYERS ) >> 1 )+ (e * (FontHeight+(FontHeight>>1)) ) , GameStatus[i] == STATUS_WaitingAfterScore ? 2 : 1 );
          }
          
          e++;
        }
      }
    }
    CenterPrint4x5Text( "waiting for all other players" , (d3dappi.szClient.cy>>1)-(FontHeight>>1) + ( ( FontHeight+2) * (MAX_PLAYERS+1)) , (colourflash>>3) &3);

    Browl -= framelag;
    if( Browl < 0.0F )
    {
      colourflash++;
      if( IsHost )
      {
        if( dcoID )
        {
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
        }
      }else{
        if( dcoID )
        {
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
        }
      }
      Browl = 30.0F;
    }
    ReceiveGameMessages();

    if( IsHost )
    {

      // have all the enabled players switched over to STATUS_LevelEnd??
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( ( ( ( GameStatus[ i ] >= STATUS_InitView_0 ) && ( GameStatus[ i ] <= STATUS_InitView_9 ) ) || ( GameStatus[i] == STATUS_ViewingScore ) ) && ( i != WhoIAm ) ) break;
      }
      if( HostMultiPlayerTimeout >= 0.0F )
      {
        if( i != MAX_PLAYERS )
          break;  // not yet..
      }
      // tell them all they can now restart a new level...
      if( CountDownOn || BombTag )
      {
        InitBombs();
        CreateCountdownDigits();
        StartCountDown( (int16) TimeLimit.value, 0 );
      }

      DebugPrintf("STATUS_WaitingAfterScore setting MyGameStatus to STATUS_Normal\n");
      MyGameStatus = STATUS_Normal;
      InitFontTransTable( !bPolyText );
      GameStatus[WhoIAm] = MyGameStatus;
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
      ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
      ServiceBigPacket(TRUE);
      ServiceServer( TRUE );

      DPlayGetSessionDesc();

      if ( glpdpSD )
      {
        // re-enable joining.....
        glpdpSD->dwFlags &= ~( DPSESSION_NEWPLAYERSDISABLED | DPSESSION_JOINDISABLED );
        DPlaySetSessionDesc( 1 );
      }

    }else{
      if( OverallGameStatus == STATUS_Normal )
      {
        InitFontTransTable( !bPolyText );

        if( CountDownOn || BombTag )
        {
          InitBombs();
          CreateCountdownDigits();
          StartCountDown( (int16) TimeLimit.value, 0 );
        }
        // tell the host that I am now in the same state as him....
        MyGameStatus = OverallGameStatus;
        GameStatus[WhoIAm] = MyGameStatus;
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      }
    }
    break;

  case STATUS_Joining:
    PreventFlips = FALSE;
    D3DAppClearScreenOnly();

    ReceiveGameMessages();
    if( IMustQuit || IsHost || ( OverallGameStatus != STATUS_Normal ) )
    {
      SpecialDestroyGame();
      break;
    }
    Browl -= framelag;
    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      if( dcoID )
      {
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      }
    }

    CenterPrint4x5Text( "Pickups Left   " , (d3dappi.szClient.cy>>1)-(FontHeight<<2), 8 );
    Printuint16( (uint16) Ships[WhoIAm].Pickups , (d3dappi.szClient.cx>>1)+((17*FontWidth>>1)), (d3dappi.szClient.cy>>1)-(FontHeight<<2), 2 );

    CenterPrint4x5Text( "RegenSlots Left" , (d3dappi.szClient.cy>>1)-(FontHeight<<1), 8 );
    Printuint16( (uint16) Ships[WhoIAm].RegenSlots , (d3dappi.szClient.cx>>1)+((17*FontWidth>>1)), (d3dappi.szClient.cy>>1)-(FontHeight<<1), 2 );

    CenterPrint4x5Text( "Mines Left     " , (d3dappi.szClient.cy>>1), 8 );
    Printuint16( (uint16) Ships[WhoIAm].Mines , (d3dappi.szClient.cx>>1)+((17*FontWidth>>1)), (d3dappi.szClient.cy>>1), 2 );

    CenterPrint4x5Text( "Triggers Left  " , (d3dappi.szClient.cy>>1)+(FontHeight<<1), 8 );
    Printuint16( (uint16) Ships[WhoIAm].Triggers , (d3dappi.szClient.cx>>1)+((17*FontWidth>>1)), (d3dappi.szClient.cy>>1)+(FontHeight<<1), 2 );

    CenterPrint4x5Text( "TrigVars Left  " , (d3dappi.szClient.cy>>1)+(FontHeight<<2), 8 );
    Printuint16( (uint16) Ships[WhoIAm].TrigVars , (d3dappi.szClient.cx>>1)+((17*FontWidth>>1)), (d3dappi.szClient.cy>>1)+(FontHeight<<2), 2 );
    
    // wait for all the pickup and mine stuff to be sent to me.....
    if( ( Ships[WhoIAm].Pickups == 0 ) && ( Ships[WhoIAm].Mines == 0 ) && ( Ships[WhoIAm].RegenSlots == 0 ) &&
      ( Ships[WhoIAm].Triggers == 0 ) && ( Ships[WhoIAm].TrigVars == 0 ) && OverallGameStatus == STATUS_Normal)
    {
      // broadcast "x is joining the game" to all players...
      if (TeamGame)
        SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_JoiningTeamGame, 0);

      MyGameStatus = OverallGameStatus;
      QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
      GameStatus[WhoIAm] = OverallGameStatus;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      SyncMines();
      InitShipRandomPos( WhoIAm );
      if( Rejoining && !TeamGame )
      {
        Ships[WhoIAm].Kills = (int16) Old_Kills;
        Ships[WhoIAm].Deaths = (int16) Old_Deaths;
        TeamNumber[WhoIAm] = (BYTE) Old_TeamNumber;
      }
      NextworkOldKills = -1;
      NextworkOldDeaths = -1;
      NextworkOldBikeNum = -1;
      SendGameMessage(MSG_NAME, 0, 0, 0, 0);
      SendGameMessage(MSG_SENDKILLSDEATHSBIKENUM, 0, 0, 0, 0);
      if( CountDownOn || BombTag )
      {
        InitBombs();
        CreateCountdownDigits();
        StartCountDown( (int16) TimeLimit.value, 0 );
        IllegalTime = TRUE;
      }
    }
    break;

  case STATUS_StartingMultiplayer:

    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      if( dcoID )
      {
        if( TeamGame &&  (OverallGameStatus == STATUS_Normal) )
        {
//          SendGameMessage(MSG_SENDKILLSDEATHSBIKENUM, 0, 0, 0, 0);
        }
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

        if ( IsServer )
        {
          static int first_of_batch = 0;
          
          DPlayGetSessionDesc();
          if ( ( glpdpSD->dwUser3 & ServerGameStateBits ) == SERVER_STATE_HostChoosing )
          {
            SendGameMessage( MSG_LEVELNAMES, 0, 0, (BYTE)first_of_batch, 0 );
            first_of_batch += MAXLEVELSPERBATCH;
            if ( first_of_batch >= LevelList.items )
            {
              first_of_batch = 0;
            }
          }
        }
      }
    }

    ReceiveGameMessages();

    if( ( MyGameStatus == STATUS_QuitCurrentGame ) || ( MyGameStatus == STATUS_Title ) )
    {
      break;
    }
    
    if( ( CurrentMenu ) && ( CurrentMenuItem ) )
    {
      if( DisplayTitle() != TRUE )
        return FALSE;
    }

    // if status has been changed from titles, should exit this state straight away
    if ( MyGameStatus != STATUS_StartingMultiplayer )
      break;

    if( ( IsHost ) && ( !CurrentMenu ) && ( !CurrentMenuItem ) )
    {
      // disable players joining...
      DPlayGetSessionDesc();
      
      //glpdpSD->dwFlags = DPSESSION_MIGRATEHOST | DPSESSION_KEEPALIVE | DPSESSION_NEWPLAYERSDISABLED | DPSESSION_JOINDISABLED;
//      if( !IsServer )
      glpdpSD->dwFlags |= DPSESSION_NEWPLAYERSDISABLED | DPSESSION_JOINDISABLED;

      DPlaySetSessionDesc(0);
      D3DAppIClearBuffers();
      ReleaseView();
      // tell them all to load up a level
      MyGameStatus = STATUS_StartingMultiplayerSynch;
      GameStatus[WhoIAm] = MyGameStatus;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      ServiceBigPacket(TRUE);
      ServiceServer( TRUE );

      LevelNum = -1;
      if( !ChangeLevel() ) return( FALSE );
      if( !IsServer )
        HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F;
      else
        HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F * 2.0F;
    }
    else
    {
      if( !IsHost )
      {
        if( ( OverallGameStatus == STATUS_StartingMultiplayerSynch ) || 
          ( (OverallGameStatus == STATUS_Normal) && (!TeamGame || PlayerReady[WhoIAm]) ) )
        {

          if ((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU))
          { 
            //LastMenu = CurrentMenu;
            //VduClear();
            MenuChangeEx( &MENU_NEW_GeneralLoading );
            MenuDraw( CurrentMenu );  // because menu processing will not be done when MyGameStatus changes
          }else
            D3DAppIClearBuffers();


          PreSynchupStatus = MyGameStatus;
          MyGameStatus = STATUS_WaitingToStartMultiPlayerClient;
        }
      }else
      {
        UpdatePlayerInfo();
      }
    }
    break;
  case  STATUS_PreStartingMultiplayerSynch:
    PreventFlips = FALSE;

    ReceiveGameMessages();
    if( IMustQuit )
    {
      SpecialDestroyGame();
      break;
    }
    MyGameStatus = STATUS_StartingMultiplayerSynch;
    if( OverallGameStatus == STATUS_Normal )
    {
      initShip( WhoIAm );
      MyGameStatus = STATUS_Joining;
      Browl = 0.0F;
    }else{
      smallinitShip( WhoIAm );
      MyGameStatus = STATUS_StartingMultiplayerSynch;
    }
    SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    GameStatus[WhoIAm] = MyGameStatus;
    break;

  case  STATUS_StartingMultiplayerSynch:
    PreventFlips = FALSE;
    InitFontTransTable( TRUE );

    ReceiveGameMessages();

    if( IMustQuit )
    {
      SpecialDestroyGame();
      break;
    }

    D3DAppClearScreenOnly();
    e = 0;

    if( DS )
    {
      DisplayStatusMessages();
    }else{
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( ( GameStatus[i] != STATUS_GetPlayerNum )&& (GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && (GameStatus[i] != STATUS_Null ) )
        {
          wsprintf( buf, "%-8s status %14s\n", &Names[i][0] , &StatusTab[ GameStatus[i] ][0] );
          CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy>>1)-( ( (FontHeight+2) * MAX_PLAYERS ) >> 1 )+ (e * (FontHeight+(FontHeight>>1)) ) , GameStatus[i] == STATUS_StartingMultiplayerSynch ? 2 : 1 );
          e++;
        }
      }
    }
    CenterPrint4x5Text( "all players synching" , (d3dappi.szClient.cy>>1)-(FontHeight>>1) + ( ( FontHeight+2) * (MAX_PLAYERS+1)) , (colourflash>>3) &3);
    Browl -= framelag;
    if( Browl <= 0.0F )
    {
      colourflash++;
      Browl = 30.0F;
      if( dcoID )
      {
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      }
    }

    if( CountDownOn || BombTag )
    {
      InitBombs();
      CreateCountdownDigits();
      StartCountDown( (int16) TimeLimit.value, 0 );
      IllegalTime = FALSE;
    }

    if( IsHost )
    {
      Printuint16( (uint16) (HostMultiPlayerTimeout / 60.0F) , 0 , 0 , 2 );
      HostMultiPlayerTimeout -= framelag;

      // have all the enabled players switched over to STATUS_StartingMultiplayer??
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( (GameStatus[i] != STATUS_GetPlayerNum ) &&(GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && ( (GameStatus[i] != STATUS_Null ) && ( GameStatus[i] != STATUS_StartingMultiplayerSynch ) ) && ( i != WhoIAm ) )
          break;
      }
      if( HostMultiPlayerTimeout >= 0.0F )
      {
        if( i != MAX_PLAYERS )
          break;  // not yet..
      }
      // tell them all they can now restart a new level...
      QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
      DebugPrintf("STATUS_StartingMultiplayerSynch setting MyGameStatus to STATUS_Normal\n");
      MyGameStatus = STATUS_Normal;
      GameStatus[WhoIAm] = STATUS_Normal;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      ServiceBigPacket(TRUE);
      ServiceServer( TRUE );

      NextworkOldKills = -1;
      NextworkOldDeaths = -1;
      NextworkOldBikeNum = -1;
      InitFontTransTable( !bPolyText );

    }else{
      if( OverallGameStatus == STATUS_Normal )
      {
        QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
        MyGameStatus = OverallGameStatus;
        GameStatus[WhoIAm] = MyGameStatus;
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
        NextworkOldKills = -1;
        NextworkOldDeaths = -1;
        NextworkOldBikeNum = -1;
        InitFontTransTable( !bPolyText );
      }
    }
    break;
  
  case STATUS_QuitCurrentGame:
    StopCompoundSfx();
    OutputVisiStats( &Mloadheader, LevelNames[ LevelNum ] );
    MenuAbort();
    MenuRestart( &MENU_Start );
    DestroyGame();  // DestroyGame changes MyGameStatus..So Set it back
    MyGameStatus = STATUS_QuitCurrentGame;
    ReleaseView();
    ReleaseLevel();


    if ( PreDemoEndMyGameStatus == STATUS_AttractMode )
    {
      PreDemoEndMyGameStatus = 0;

      MyGameStatus = STATUS_Title;
    }
    else
    {
      MyGameStatus = STATUS_Title;

      InitScene();
      InitView();
    }
    break;
  case STATUS_WaitingForLobbyConnect:

    ReceiveGameMessages();

    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;

      // reget session info..
      hr = DPlayGetSessionDesc();
      if ( hr != DP_OK )
      {
        DebugPrintf("DPlayGetSessionDesc not OK\n");
      }

      if ( glpdpSD )
      {
        if ( glpdpSD->dwUser4 != 0 )
        {
          switch ( glpdpSD->dwUser3 & ServerGameStateBits )
          {
          case SERVER_STATE_NoServer:   
          case SERVER_STATE_Joinable:   
            if( TeamGame )
            {
              JoinASessionEx( &MENU_NEW_WatchTeamSelect );
            }else
            {
              JoinASessionEx( &MENU_NEW_JoinWaitingToStart );
            }
            break;
          case SERVER_STATE_NeedHost:   
            JoinASessionEx( &MENU_NEW_CreateGamePseudoHost );
            break;
          case SERVER_STATE_HostChoosing: 
            D3DAppClearScreenOnly();
            CenterPrint4x5Text( HOST_IS_SETTING_UP_GAME, (d3dappi.szClient.cy>>1)-(FontHeight>>1) , 2 );
            break;
          }
        }
      }else
      {
        DebugPrintf("no DPlay ptr!\n");
      }
    }

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    DebugPrintf("STATUS_WaitingForLobbyConnect 3\n");

    break;
  case STATUS_GetPlayerNum:
    D3DAppClearScreenOnly();
    ReceiveGameMessages();
    
    CenterPrint4x5Text( "Requesting Player Number" , (d3dappi.szClient.cy>>1)-(FontHeight>>1) + ( ( FontHeight+2) * (MAX_PLAYERS+1)) , 2 );
                                          
    sprintf( &buf[0] , "Attempt %d\n" , GetPlayerNumCount );
    CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy>>1)-(FontHeight>>1) , 2 );

    
    GetPlayerNumCount1 -= framelag;
    GetPlayerNumCount2 -= framelag;

    if( WhoIAm != 0xff )
    {
      if( WhoIAm == MAX_PLAYERS )
      {
        PrintErrorMessage ( "The Game is Currently Un-Joinable", 3, NULL, ERROR_USE_MENUFUNCS );
        MyGameStatus = STATUS_Title;
        break;
      }
      if( WhoIAm == MAX_PLAYERS+1 )
      {
        PrintErrorMessage ( "You Dont have the level they are playing", 3, NULL, ERROR_USE_MENUFUNCS );
        MyGameStatus = STATUS_Title;
        break;
      }
      if( WhoIAm == MAX_PLAYERS+2 )
      {
        PrintErrorMessage ( "The Game is Full", 3, NULL, ERROR_USE_MENUFUNCS );
        MyGameStatus = STATUS_Title;
        break;
      }

      if( Rejoining )
      {
        Ships[WhoIAm].Kills = (int16) Old_Kills;
        Ships[WhoIAm].Deaths = (int16) Old_Deaths;
        TeamNumber[WhoIAm] = (BYTE) Old_TeamNumber;
      }
      memset(&Names, 0, sizeof(SHORTNAMETYPE) );
        strncpy( (char*) &Names[WhoIAm][0] , &biker_name[0] , 7 );
      Names[WhoIAm][7] = 0;
      Ships[ WhoIAm ].BikeNum = ( SelectedBike % MAXBIKETYPES );
          
      Old_Session = *glpdpSD;
      strcpy( (char*) &Old_Name[0] , (char*) &biker_name[0] );
    
      MyGameStatus = STATUS_StartingMultiplayer;
      MenuState = MENUSTATE_Select;

      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      SendGameMessage(MSG_NAME, 0, 0, 0, 0);
      SendGameMessage(MSG_SENDKILLSDEATHSBIKENUM, 0, 0, 0, 0);
      ServiceBigPacket(TRUE);
      ServiceServer( TRUE );

// hack for ramdemo...
      if( RecordDemoToRam )
        RecordDemo = TRUE;

      if( RecordDemo )
      {
        DemoFp = fopen( "Test.dmo" , "wb" );
        setvbuf( DemoFp, NULL, _IONBF , 0 );    // size of stream buffer...
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
        msg = WhoIAm;
        Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );       // Whos Name it is..
        Demo_fwrite( &biker_name[0], 7, 1, DemoFp );
        msg = 0;
        Demo_fwrite( &msg, sizeof(BYTE), 1, DemoFp );       // terminator for name..
      }

      // menu change is done when session description is changed for lobby session
      if ( IsPseudoHost )
      {
        MyGameStatus = STATUS_GetLevelNames;
        PseudoHostAck_Timeout = PSEUDOHOST_WAIT_TIME;
      }else
      {
        MenuChangeEx( GetPlayerNumMenu );
      }
      break;
    }

    if( GetPlayerNumCount2 <= 0.0F )
    {
      // Time is Up No Player Number was got so quit back to the title screen...
      PrintErrorMessage ( "Couldnt Get a Player Number", 3, NULL, ERROR_USE_MENUFUNCS );
      MyGameStatus = STATUS_Title;
      break;
//      BailMultiplayer( (MENU*) NULL );
//      MenuRestart( &MENU_Start );
//      MyGameStatus = STATUS_Title;
    }

    if( GetPlayerNumCount1 <= 0.0F )
    {
      GetPlayerNumCount++;

      // Time to send another request for a Player Number...
      GetPlayerNumCount1 = 60.0F;   // approx 1 Second...
      SendGameMessage(MSG_HEREIAM, 0, 0, 0, 0); // can I join your game...
      DebugPrintf("sending MSG_HEREIAM\n");
    }

    break;
  case STATUS_GetLevelNames:
    D3DAppClearScreenOnly();

    if ( GeneralTimeout( &PseudoHostAck_Timeout ) )
    {
      if( dcoID )
      {
        DPlayDestroyPlayer(dcoID);
        dcoID = 0;
      }

      DPlayRelease();
      MyGameStatus = STATUS_Title;
      RefreshDPlay();

      OKToJoinSession = FALSE;
      break;
    }

    ReceiveGameMessages();
    Browl -= framelag;
    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      if( dcoID )
      {
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      }
    }

    CenterPrint4x5Text( QUERYING_SERVER_FOR_LEVELS, (d3dappi.szClient.cy>>1)-(FontHeight>>1) , 2 );

    switch( ServerLevelsListState )
    {
    case SERVERLEVELS_None:
      SendGameMessage(MSG_TOSERVER, 0, 0, TOSERVERMSG_Reset, 0);
      PrintErrorMessage("you do not have any of the levels on this server", 0, NULL, ERROR_DONTUSE_MENUFUNCS );
      PreWaitingToSendMessagesStatus = STATUS_Title;
      MyGameStatus = STATUS_WaitingToSendMessages;  // to ensure all guaranteed messages get sent
      SessionsList.selected_item = -1;
      MySessionsList.selected_item = -1;

      break;
    case SERVERLEVELS_Got:
      MenuChangeEx( &MENU_NEW_CreateGamePseudoHost );
      MyGameStatus = STATUS_StartingMultiplayer;
      MenuState = MENUSTATE_Select;
      break;
    }

    break;
  case STATUS_PlayingDemo:

    DemoGameLoops++;

    if( DemoSpeed.value > 8 )
    {
      // slower or normal playback speed...
      Demoframelag = 1.0F / (float) ( DemoSpeed.value - 7 );
    }else{
      Demoframelag = 1.0F * (float) ( 9 - DemoSpeed.value );
    }
    
    Oldframelag = framelag;

    if( PauseDemo )
    {
      framelag = 0.0F;
    }else{
      framelag *= Demoframelag;
    }

    if( DemoScreenGrab && !CurrentMenu )
    {
//      framelag = 2.24F;   // 30 fps
      framelag = 2.613333F; // 25 fps
      Demoframelag = 1.0F;
      Oldframelag = framelag;
    }

    if( DemoScreenGrab && !CurrentMenu )
    {
      sprintf( fname, "ScreenS\\scr%04d.ppm", fnum );
//      _spawnl( _P_WAIT, "grab.exe", "grab.exe", "-fp" , "-w 512" , "-h 384" , fname, NULL );
      folder_exists( FMVSNAPSHOT_FOLDER );
      SaveFullScreenSnapShot( fname );
      fnum++;
      if( fnum > 9999 )
      {
        DemoScreenGrab = FALSE;
      }
    }

    if( MainGame( lpDev , lpView ) != TRUE )
      return FALSE;

    break;


  case STATUS_InitView_0:
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
    ServiceBigPacket(TRUE);
    ServiceServer( TRUE );


    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

    D3DAppClearScreenOnly();


    MenuFrozen = FALSE; // ensure that menus are OK to use once in game
    JustExitedMenu = FALSE; 

    QuickStart = QUICKSTART_None;
    WaitingToQuit = FALSE;
  
    ReceiveGameMessages();

    if (bPrimaryPalettized )
    {
      lpPalette = DDLoadPalette( lpDD , "data\\pictures\\pal.bmp");
      ddpal =  DDLoadPalette( lpDD , "data\\pictures\\pal.bmp");
      d3dappi.lpFrontBuffer->lpVtbl->SetPalette( d3dappi.lpFrontBuffer, ddpal );
      d3dappi.lpBackBuffer->lpVtbl->SetPalette( d3dappi.lpBackBuffer, ddpal );
    }
  

    
    if( !SetMatrixViewPort() )
    {
      SeriousError = TRUE;
      Msg( "SetMatrixViewPort() failed\n" );
      return FALSE;
    }
    
    // Init the font....
    if( !IsServer )
    {
      InitFont(FALSE);
    }else{
      InitFont(TRUE);
    }
  
    MyGameStatus = STATUS_InitView_1;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_1:
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
    ServiceBigPacket(TRUE);
    ServiceServer( TRUE );

    
    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

    AddCommentToBat( "Started Loading Level" );

    SetOurRenderStates( NULL );
    D3DAppClearScreenOnly();

    ReceiveGameMessages();

    // Create the offscreen surface, by loading our bitmap
      lpDDSOne = DDLoadBitmap( lpDD, "data\\pictures\\panel.bmp" , 0, 0 );
    ddpal =  DDLoadPalette( lpDD , "data\\pictures\\panel.bmp");
    lpDDSOne->lpVtbl->SetPalette( lpDDSOne , ddpal );
    DDSetColorKey( lpDDSOne, RGB_MAKE( 0 , 0 , 0 ) );
    // Create the offscreen surface, by loading our bitmap.
    if( ( ModeCase != -1 ) && ( DrawPanel == FALSE ) && DrawSimplePanel )
    {
      lpDDSThree = DDLoadBitmap( lpDD, (char*) &PanelNames[ModeCase] , 0, 0 );
      ddpal =  DDLoadPalette( lpDD , (char*) &PanelNames[ModeCase]);
      lpDDSThree->lpVtbl->SetPalette( lpDDSThree , ddpal );
      DDSetColorKey( lpDDSThree, RGB_MAKE( 255 , 0 , 255 ) );
      PanelHeight = PanelVisibleY[ModeCase];

      lpDDSFour = DDLoadBitmap( lpDD, "data\\pictures\\pcontent.bmp" , 0, 0 );
      ddpal =  DDLoadPalette( lpDD , "data\\pictures\\pcontent.bmp" );
      lpDDSFour->lpVtbl->SetPalette( lpDDSFour , ddpal );
    }
  

    FillInPanelPositions();
    ReMakeSimplePanel = TRUE;
    
//    InitVisiExecList( lpDev );
    InitSkinExecs();
    InitPortalExecs();
    InitRenderBufs( lpDev );

    // Init the Texture Handler
    if( !InitTload( &Tloadheader ) )
    {
      SeriousError = TRUE;
      Msg( "InitTLoad failed\n" );
      return FALSE;
    }
  
    //  Prep the Texture Handler.....
    if( !PreMload( (char*) &LevelNames[LevelNum][0] , &Mloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE; // the model and visipoly data
    }


    // Can Cope with no .Wat file!!!
    PreWaterLoad( (char*) &WaterNames[LevelNum][0] );
    
    
    if( OnceOnlyChangeLevel )
    {
      OnceOnlyChangeLevel = FALSE;

      if( !PreLoadShips() )
      {
        SeriousError = TRUE;
        return FALSE;
      }

      if( !PreLoadBGOFiles() )
      {
        SeriousError = TRUE;
        return FALSE;
      }

      if( !PreLoadRestartPoints() )
      {
        SeriousError = TRUE;
        return FALSE;
      }

      if( !PreLoadEnemies() )
      {
        SeriousError = TRUE;
        return FALSE;
      }
    }

    if ( !LimitedLoad )
    {
      EnableRelavantModels( &ModelNames[0] );

      if( !PreInitModel( lpDev , &ModelNames[0] ) )
      {
        SeriousError = TRUE;
        return FALSE;
      }
    }else
    {
      if( !PreInitModel( lpDev , &SplashModelNames[0] ) )
      {
        SeriousError = TRUE;
        return FALSE;
      }
    }

    if ( !LimitedLoad )
    {
      if( !Load_All_Off_Files( &OffsetFiles[ 0 ] ) )
      {
        SeriousError = TRUE;
        return FALSE;
      }
    }else
    {
      if( !Load_All_Off_Files( &Splash_OffsetFiles[ 0 ] ) )
      {
        SeriousError = TRUE;
        return FALSE;
      }

    }

    InitLogos();

    MyGameStatus = STATUS_InitView_2;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_2:
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
    ServiceBigPacket(TRUE);
    ServiceServer( TRUE );

    
    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );

    D3DAppClearScreenOnly();

    ReceiveGameMessages();


    //  Load in And if nescessary ReScale Textures... 
    if( !Tload( &Tloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }
  
    if ( !LimitedLoad ) FixTextureUVs( &OffsetFiles[ 0 ] );
    else FixTextureUVs( &Splash_OffsetFiles[ 0 ] );

    // Load all system memory textures...
    if( !SysTload( SystemMemTPages, CurrentSysTexture ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    MyGameStatus = STATUS_InitView_3;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_3:
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
    ServiceBigPacket(TRUE);
    ServiceServer( TRUE );


    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    
    D3DAppClearScreenOnly();

    ReceiveGameMessages();

    if ( !LimitedLoad )
    {
      if( !InitModel( lpDev , &ModelNames[0] ) )
        {
        SeriousError = TRUE;
        return FALSE;               // all 3d models....
      }
    }else
    {
      if( !InitModel( lpDev , &SplashModelNames[0] ) )
        {
        SeriousError = TRUE;
        return FALSE;               // all 3d models....
      }
    }

    MyGameStatus = STATUS_InitView_4;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_4:
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
    ServiceBigPacket(TRUE);
    ServiceServer( TRUE );


    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    
    D3DAppClearScreenOnly();

    ReceiveGameMessages();

    if( !Mload( (char*) &LevelNames[LevelNum][0] , &Mloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE; // the model and visipoly data
    }

    InitVisiStats( &Mloadheader );

    MyGameStatus = STATUS_InitView_5;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_5:
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
    ServiceBigPacket(TRUE);
    ServiceServer( TRUE );


    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    
    D3DAppClearScreenOnly();
    ReceiveGameMessages();

    // Can Cope with no Bsp file!!!
#ifdef LOAD_ZBSP
    Bspload( (char*) &BspZNames[LevelNum][0], &Bsp_Header[ 0 ] );
    Bspload( (char*) &BspNames[LevelNum][0], &Bsp_Header[ 1 ] );
#else
#ifdef BSP_ONLY
    if ( !Bspload( (char*) &BspNames[LevelNum][0], &Bsp_Header[ 0 ] ) )
    {
      SeriousError = TRUE;
      Msg( "Bspload failed\n" );
      return FALSE;   // the collision data
    }
#else
    Bspload( (char*) &BspNames[LevelNum][0], &Bsp_Header[ 0 ] ); // load .BSP file into 0 skin
#endif
    Bsp_Header[ 1 ].State = FALSE; // no non-zero .BSP any more
#endif
//    if ( !Bsp_Duplicate( &Bsp_Header[ 0 ], &Bsp_Original ) )
//    {
//      SeriousError = TRUE;
//      Msg( "Bsp_Duplicate failed\n" );
//      return FALSE;
//    }
    
    // might not be any water...
    WaterLoad();

    ReadTxtFile( (char*) &TextNames[LevelNum][0] );
    
    ReadMsgFile( (char*) &MsgNames[LevelNum][0] );
    
    if( !MCload( (char*) &CollisionNames[LevelNum][0] , &MCloadheader ) )
    {
      SeriousError = TRUE;
      Msg( "MCload non zero failed\n" );
      return FALSE;   // the collision data
    }
    if( !MCload( (char*) &CollisionZNames[LevelNum][0] , &MCloadheadert0 ) )
    {
      SeriousError = TRUE;
      Msg( "MCload zero failed\n" );
      return FALSE; // the collision data skin thickness 0
    }
  
    SetUpShips();
  
    cursorclipped = TRUE;
    SetCursorClip();

    if( !InitSpecialExecBufs() )
    {
      SeriousError = TRUE;
      Msg( "InitSpecialExecBufs() failed\n" );
      return FALSE;
    }
    
//    TaskerInit();
//    TaskCreate( (TaskFunc) Test , 0 );

    MyGameStatus = STATUS_InitView_6;
    PrintInitViewStatus( MyGameStatus );

    break;

  case STATUS_InitView_6:
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
    ServiceBigPacket(TRUE);
    ServiceServer( TRUE );


    DrawLoadingBox( CurrentLoadingStep++, 0, 1 );
    
    D3DAppClearScreenOnly();
    ReceiveGameMessages();

    InitSoundInfo( &Mloadheader );
    
    MyGameStatus = STATUS_InitView_7;
    PrintInitViewStatus( MyGameStatus );
    break;
  case STATUS_InitView_7:
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
    ServiceBigPacket(TRUE);
    ServiceServer( TRUE );


    D3DAppClearScreenOnly();
    ReceiveGameMessages();


#ifdef NO_PRECALCULATED_CELL_COLOURS
    CreateCellColours( &Mloadheader );
#endif

    MyGameStatus = STATUS_InitView_8;
    PrintInitViewStatus( MyGameStatus );

    break;
  case STATUS_InitView_8:
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ProcessGuaranteedMessages( FALSE , TRUE , FALSE );
    ServiceBigPacket(TRUE);
    ServiceServer( TRUE );


    D3DAppClearScreenOnly();
    ReceiveGameMessages();


    MyGameStatus = STATUS_InitView_9;
    PrintInitViewStatus( MyGameStatus );
    QueryPerformanceCounter((LARGE_INTEGER *) &LastTime);
    // dummy call to timer ensures no pauses later...
    timeSetEvent( 10, 10, TimerProc, (DWORD)-1, TIME_ONESHOT ); 

    InitShipSpeeds();

    MyGameStatus = InitView_MyGameStatus;

    Current_Max_Score = 0;  // used by host to store highest score in session desc

    break;
  case STATUS_ChangeLevelPostInitView:
    Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NodeName[ 0 ], ".NOD" );

    if( !Nodeload( NodeName ) )
    {
      SeriousError = TRUE;
      return( FALSE );
    }

    if( !LoadBGOFiles() )
    {
      SeriousError = TRUE;
      return( FALSE );
    }

    if( !LoadEnemies() )
    {
      SeriousError = TRUE;
      Msg( "LoadEnemies() failed\n" );
      return( FALSE );
    }

    if( !LoadSpotFX() )
    {
      SeriousError = TRUE;
      Msg( "LoadSpotFX() failed\n" );
      return( FALSE );
    }

    if( !LoadStartPoints() )
    {
      SeriousError = TRUE;
      Msg( "LoadStartPoints() failed\n" );
      return( FALSE );
    }

    if( !LoadRestartPoints() )
    {
      SeriousError = TRUE;
      Msg( "LoadRestartPoints() failed\n" );
      return( FALSE );
    }

    if ( !LimitedLoad )
    {
      if ( !InitializeSound( DESTROYSOUND_All ))
      {
        Msg("InitializeSound() failed\n");
        return FALSE;
      }
    }


    Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NodeName[ 0 ], ".CAM" );

    if( !Cameraload( NodeName ) )
    {
      SeriousError = TRUE;
      return( FALSE );
    }

    // might not be any real-time lights
    LoadRTLights();

    if ( CaptureTheFlag || CTF )
      GoalLoad();

    InitThermo();

    if( ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) )
    {
      LoadTimeForLevel();
    }

    if( !LoadPickupsPositions() )
    {
      SeriousError = TRUE;
      Msg( "LoadPickupsPositions() failed\n" );
      return( FALSE );
    }

    // might not be any External Forces...
    ExternalForcesLoad( (char*) &ExternalForceNames[LevelNum][0] );
    // might not be any Teleports...
    TeleportsLoad( (char*) &TeleportNames[LevelNum][0] );
    // Can Cope with no Zone file!!!
    TriggerAreaload( (char*) &ZoneNames[LevelNum][0] );



    InitShipsChangeLevel(&Mloadheader);

    if( ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) )
    {
      if( CountDownOn || BombTag )
      {
        InitBombs();
        CreateCountdownDigits();
        StartCountDown( (int16) SinglePlayerTimeLimit, 0 );
      }
      else
      {
        DeleteCountdownDigits();
      }
    }

    GodModeOnceOnly = TRUE;

    QueryPerformanceCounter((LARGE_INTEGER *) &LastTime);

    MyGameStatus = ChangeLevel_MyGameStatus;
    break;
  case STATUS_ChangeLevelPostPlayingDemo:
    WhoIAm = MAX_PLAYERS;
    Ships[WhoIAm].Object.Mode = DEMO_MODE;
    Ships[WhoIAm].enable = 1;
    dcoID = 0;
    glpDP = NULL;
    IsHost = TRUE;
    Current_Camera_View = MAX_PLAYERS;    // which object is currently using the camera view....
    AutoDetail = FALSE;
  
    for( i = 0 ; i < MAX_PLAYERS ; i++ )
    {
      GameStatus[i] = STATUS_Null;
    }
  
    DemoTimeSoFar = 0;
    if( DemoSpeed.value > 8 )
    {
      // slower or normal playback speed...
      Demoframelag = 1.0F / (float) ( DemoSpeed.value - 7 );
    }else{
      Demoframelag = 1.0F * (float) ( 9 - DemoSpeed.value );
    }
  
    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
    QueryPerformanceCounter((LARGE_INTEGER *) &DemoStartedTime);
    GameElapsedTime = 0;
    DemoGameLoops = 0;
    TempGameElapsedTime = GameStartedTime;
    MyGameStatus = STATUS_PlayingDemo;
    break;
  case STATUS_ChangeLevelPostAttractMode:
    WhoIAm = MAX_PLAYERS;
    Ships[WhoIAm].Object.Mode = DEMO_MODE;
    Ships[WhoIAm].enable = 1;
    dcoID = 0;
    glpDP = NULL;
    IsHost = TRUE;
    Current_Camera_View = 0;    // which object is currently using the camera view....
    AutoDetail = FALSE;
  
    for( i = 0 ; i < MAX_PLAYERS ; i++ )
    {
      GameStatus[i] = STATUS_Null;
    }
    DemoTimeSoFar = 0;
    Demoframelag = 1.0F;
    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
    GameElapsedTime = 0;
    TempGameElapsedTime = GameStartedTime;
    
    if ( InSplashDemo )
    {
      MyGameStatus = STATUS_SplashScreen;

      // if this demo is being used for splash screen, timer to start from here...
      SplashStartTime = timeGetTime();
      SplashFinishTime = SplashStartTime + NewSplashScreens[ NewCurrentSplashScreen ].time;
    }else
    {
      MyGameStatus = STATUS_AttractMode;
    }

    PreventFlips = FALSE;

    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
    QueryPerformanceCounter((LARGE_INTEGER *) &DemoStartedTime);
    GameElapsedTime = 0;
    DemoGameLoops = 0;

    break;
  case STATUS_AttractMode:

    DemoGameLoops++;

    if( DemoSpeed.value > 8 )
    {
      // slower or normal playback speed...
      Demoframelag = 1.0F / (float) ( DemoSpeed.value - 7 );
    }else{
      Demoframelag = 1.0F * (float) ( 9 - DemoSpeed.value );
    }
    
//    Demoframelag = 10.0F;
    Oldframelag = framelag;

    if( PauseDemo )
    {
      framelag = 0.0F;
    }else{
      framelag *= Demoframelag;
    }

#if 0
    DemoEyesSelect.value = 0;
#endif
    Current_Camera_View = DemoEyesSelect.value;   // which object is currently using the camera view....

    framelag *= Demoframelag;

    if( MainGame( lpDev , lpView ) != TRUE )
      return FALSE;
    break;

//  *********************** Single Player Game Stuff **********************************
  case  STATUS_StartingSinglePlayer:
    //D3DAppIClearBuffers();
    MenuAbort();
    ReleaseView();
    // tell them all to load up a level
    MyGameStatus = STATUS_PostStartingSinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    LevelNum = -1;
    if( !NewLevelNum )
    {
      NumGoldBars = 0;
      CrystalsFound = 0;
    }
    GameCompleted = GAMECOMPLETE_NotComplete;
    if( !ChangeLevel() ) return( FALSE );
    break;

  case  STATUS_PostStartingSinglePlayer:
    smallinitShip( WhoIAm );
//    if( CountDownOn )
//    {
//      CreateCountdownDigits();
//      StartCountDown( (int16) TimeLimit.value, 0 );
//    }
    
    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
    MyGameStatus = STATUS_SinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    LevelTimeTaken = 0.0F;

    QueryPerformanceCounter((LARGE_INTEGER *) &Time_Value);
    QueryPerformanceFrequency((LARGE_INTEGER *) &Time_Freq);
    Old_LevelTime_Float = ( ( Time_Value * 100.0F ) / Time_Freq );

    break;
  case STATUS_SinglePlayer:

    PreventFlips = FALSE;
      
    if( MainGame( lpDev , lpView ) != TRUE )
      return FALSE;

    if ( bSoundEnabled && !CurrentMenu )
    {
      ProcessLoopingSfx();
      ProcessEnemyBikerTaunt();
    }

    //LevelTimeTaken += framelag;

    QueryPerformanceCounter((LARGE_INTEGER *) &Time_Value);
    QueryPerformanceFrequency((LARGE_INTEGER *) &Time_Freq);
    Time_Float = ( ( Time_Value * 100.0F ) / Time_Freq );
    time_diff = ( Time_Float - Old_LevelTime_Float );
    Old_LevelTime_Float = Time_Float;

    if ( framelag )
    {
      LevelTimeTaken += time_diff;
    }

    /*
    sprintf( buf, "time so far %d", ((uint32)LevelTimeTaken) / 100 );
    Print4x5Text( buf, 10, 30, 2 );
    */

    // if player is quiting nothing should stop him.....
    if( MyGameStatus != STATUS_QuitCurrentGame )
    {
      if( IsHost )
      {
        if( NewLevelNum != LevelNum )
        {
          D3DAppIClearBuffers();
          ReleaseView();
          // the level has ended or changed...
          MyGameStatus = STATUS_ViewingStats;
          GameStatus[WhoIAm] = MyGameStatus;
          InitView();
        }
      }
    }
    break;
  case STATUS_ViewingStats:
    D3DAppIClearBuffers();

    ReleaseLevel();

    ReleaseView();
      
    MyGameStatus = STATUS_BetweenLevels;
    InitScene();
    InitView();
    MenuAbort();
    MenuRestart( &MENU_NEW_NumberOfCrystals );
    CameraStatus = CAMERA_AtRightVDU;
        break;


  case  STATUS_TitleLoadGameStartingSinglePlayer:
    //D3DAppIClearBuffers();
    MenuAbort();
    ReleaseView();
    // tell them all to load up a level
    MyGameStatus = STATUS_TitleLoadGamePostStartingSinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    LevelNum = -1;
    NumGoldBars = 0;
    CrystalsFound = 0;
    GameCompleted = GAMECOMPLETE_NotComplete;
    if( !ChangeLevel() ) return( FALSE );
    break;
  case  STATUS_InGameLoadGameStartingSinglePlayer:
    //D3DAppIClearBuffers();

    NewLevelNum = InGameLoadGameLevelNum;
    MenuAbort();
    ReleaseView();
    // tell them all to load up a level
    MyGameStatus = STATUS_TitleLoadGamePostStartingSinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    LevelNum = -1;
    NumGoldBars = 0;
    CrystalsFound = 0;
    GameCompleted = GAMECOMPLETE_NotComplete;
    if( !ChangeLevel() ) return( FALSE );
    break;
  case  STATUS_TitleLoadGamePostStartingSinglePlayer:

    smallinitShip( WhoIAm );
    InGameLoad( NULL );
    
    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
    QueryPerformanceCounter((LARGE_INTEGER *) &LastTime);

    MyGameStatus = STATUS_SinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    break;

//  ******************** End of Single Player Game Stuff *******************************

  case STATUS_PseudoHostWaitingForAck:
    ReceiveGameMessages();

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      if( dcoID )
      {
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      }

      DPlayGetSessionDesc();
      switch( glpdpSD->dwUser3 & ServerGameStateBits )
      {
      case SERVER_STATE_NoServer:
      case SERVER_STATE_NeedHost:
        // server is in wrong state.
        // probably because we have quit & rejoined the session and are recieving first ack msg.
        // therefore just ignore message
        break;
      case SERVER_STATE_HostChoosing:
        // server state has not changed
        break;
      case SERVER_STATE_Joinable:
        // server has made session joinable, therefore he must have recieved MSG_GAMEPARAMETERS
        MyGameStatus = STATUS_StartingMultiplayer;
        if ( TeamGame )
        {
          MenuChangeEx( &MENU_NEW_WatchTeamSelect );
        }else
        {
          MenuChangeEx( &MENU_NEW_PseudoHostWaitingToStart );
        }
        break;
      }
    }
    break;
  case STATUS_WaitingToSendMessages:
#ifdef  GUARANTEEDMESSAGES
    if ( GuaranteedMessagesActive )
    {
      D3DAppClearScreenOnly();
      CenterPrint4x5Text( "please wait...", (d3dappi.szClient.cy>>1)-(FontHeight>>1) , 2 );
      ReceiveGameMessages();  // this also ensures that guaranteed messages are sent
    }
    else
#endif
    {
      if ( !RefreshDPlay() )
        MyGameStatus = STATUS_Title;  // because we will have been thrown back a menu

      InitMySessionsList();
      GetCurrentSessions( NULL );

      MyGameStatus = PreWaitingToSendMessagesStatus;
    }
  default:
    break;
  }
  return TRUE;
}


float Hdiv[MAX_PLAYERS] = { 1.0F ,
                1.0F ,
                1.0F ,
                0.5F ,
                0.5F ,
                0.5F ,
                1.0F ,
                0.5F };
float Vdiv[MAX_PLAYERS] = { 1.0F ,
                0.5F ,
                0.33333333F ,
                0.5F ,
                0.2F ,
                0.33333333F ,
                0.142857142F ,
                0.25F  };
float Hpos[MAX_PLAYERS][MAX_PLAYERS] = { { 0.0F } ,
                       { 0.0F , 0.0F } ,
                       { 0.0F , 0.0F , 0.0F } ,
                       { 0.0F , 1.0F , 0.0F , 1.0F } ,
                       { 0.0F , 0.0F , 0.0F , 0.0F , 0.0F } ,
                       { 0.0F , 1.0F , 0.0F , 1.0F , 0.0F , 1.0F} ,
                       { 0.0F , 0.0F , 0.0F , 0.0F , 0.0F , 0.0F , 0.0F} ,
                       { 0.0F , 1.0F , 0.0F , 1.0F , 0.0F , 1.0F , 0.0F , 1.0F } };
float Vpos[MAX_PLAYERS][MAX_PLAYERS] = { { 0.0F } ,
                       { 0.0F , 1.0F } ,
                       { 0.0F , 1.0F , 2.0F } ,
                       { 0.0F , 0.0F , 1.0F , 1.0F } ,
                       { 0.0F , 1.0F , 2.0F , 3.0F , 4.0F } ,
                       { 0.0F , 0.0F , 1.0F , 1.0F , 2.0F , 2.0F} ,
                       { 0.0F , 1.0F , 2.0F , 3.0F , 4.0F , 5.0F , 6.0F} ,
                       { 0.0F , 0.0F , 1.0F , 1.0F , 2.0F , 2.0F , 3.0F , 3.0F } };
                       

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Main Routines...
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void MainRoutines( void )
{
  if( PlayDemo )
  {
    DemoPlayingDplayGameUpdate();
  }

  ProcessShips();

#ifdef SHADOWTEST
//  CreateSpotLight( (uint16) WhoIAm, SHIP_RADIUS, &Mloadheader );
//  CreateShadowsForShips();
#endif

  FirePrimary();
  if( !BombTag )
    FireSecondary();
  ProcessEnemies();
  ProcessSpotFX();
  ProcessPrimaryBullets();
  ProcessSecondaryBullets();
  if( !PlayDemo ) RegeneratePickups();
  ProcessPickups();
  ProcessBGObjects( TRUE );
  ProcessRestartPoints();
  ProcessModels();
  ProcessPolys();
  ProcessXLights( &Mloadheader );
  DoAfterBurnerEffects();
  FmPolyProcess();
  CheckTimeLimit();
  if( CountDownOn ) UpdateCountdownDigits();
  if( ForsakenAnyKey ) DisplayForsakenAnyKey();
  ShowScreenMultiples();
  ProcessActiveConditions();
  ProcessTriggerAreas();
  UpdateBombs();
  ProcessGoals();
#ifdef DOESNT_WORK_AND_A_BAD_IDEA_ANYWAY
  if ( outside_map && !DebugInfo && ( Ships[WhoIAm].Object.Mode != DEATH_MODE ) && ( Ships[WhoIAm].Object.Mode != LIMBO_MODE ) )
  {
    // bugger -- the player has managed to get outside the map, so kill him off...
    Ships[WhoIAm].ShipThatLastHitMe = MAX_PLAYERS;
    Ships[WhoIAm].Damage = 255 + 255; //make sure I Die....
    DoDamage( OVERRIDE_INVUL );
  
  
    Ships[WhoIAm].Object.Mode = DEATH_MODE;
    Ships[WhoIAm].Timer = 0.0F;
    AddMessageToQue( "A Bomb Killed You..." );
    ShipDiedSend( WEPTYPE_Primary, 0);
  }
#endif
  WaterProcess();
  ProcessRTLights();
}

void CheckForRogueSfx( void )
{ 
  DWORD current_time, dwStatus;
  int i;

  // Dirty hack to kill off any rogue sfx!! 
  if ( bSoundEnabled && NumDupCompoundBuffers )
  {
    EnterCriticalSection( &CompoundSfxKey );
    
    current_time = GetTickCount();

    for ( i = 0; i < NumDupCompoundBuffers; i++ )
    {
      IDirectSoundBuffer_GetStatus( CompoundSfxBuffer[ i ].buffer, &dwStatus );

      // if buffer is playing, check whether it should have stopped by now...
      if (dwStatus & DSBSTATUS_PLAYING)
      {
        if ( current_time > ( CompoundSfxBuffer[ i ].finish_time + 50 ) )
        {
          DebugPrintf("Rogue SFX killed off: SfxNum %d, SndObj index %d, start time %d finish time %d ( current time %d ), timerID %d  \n",
          CompoundSfxBuffer[ i ].current_sfx, CompoundSfxBuffer[ i ].compound_buffer_lookup_index, CompoundSfxBuffer[ i ].start_time,
          CompoundSfxBuffer[ i ].finish_time, current_time, CompoundSfxBuffer[ i ].timerID);

          EnterCriticalSection( &SfxHolderKey );
          KillCompoundSfxBuffer( i );
          LeaveCriticalSection( &SfxHolderKey );

          //SfxHolder[ CompoundSfxBuffer[ i ].SfxHolderIndex ].Used = FALSE;
          FreeSfxHolder( CompoundSfxBuffer[ i ].SfxHolderIndex );
  

        }
      }
    }

    LeaveCriticalSection( &CompoundSfxKey );
  }
}

void CheckLevelEnd ( void )
{

  if( HasLevelEnded() )
  {
    PauseAllSfx();
#ifdef FINAL_RELEASE
    if( OriginalLevels )
    {
#endif
      LevelNum = -1;
      if( NewLevelNum == ( LevelList.items - 2 ) )
      {
        // if they cheated at any point
        if ( Cheated )
        {
          GameCompleted = GAMECOMPLETE_Cheated;
        }else
        {
          // Check for Secret level...
          if( CrystalsFound == ( LevelList.items - 1 ) )
          {
            //They can go to the secret level
            NewLevelNum = ( LevelList.items - 1 );
            GameCompleted = GAMECOMPLETE_GotAllCrystalsSoFar;
          }else{
            // go straight to the end sequence
            GameCompleted = GAMECOMPLETE_WithoutAllCrystals;
          }
        }
      }else if( NewLevelNum == ( LevelList.items - 1 ) ){
        // They have completed the Secret level
        if( CrystalsFound == ( LevelList.items ) )
        {
          // they found the crystal on the secret level...Therefore they have access to the Secret Biker..
          BikeList.items = MAXBIKETYPES;
          // record this in their config
          write_config( player_config, biker_config );
          GameCompleted = GAMECOMPLETE_WithAllCrystals;
        }else
        {
          GameCompleted = GAMECOMPLETE_WithAllCrystalsExceptLast;
        }
      }else{
        NewLevelNum++;
        GameCompleted = GAMECOMPLETE_NotComplete;
      }
      
      return;
#ifdef FINAL_RELEASE
    }else
    {
      if( NewLevelNum == ( LevelList.items - 1 ) )
      {
        // completed all levels ( not original levels )
        GameCompleted = GAMECOMPLETE_AdditionalLevels;
        return;
      }else
      {
        NewLevelNum++;
        GameCompleted = GAMECOMPLETE_NotComplete;
      }
    }
#endif
  }

}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Main Render Loop...
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
MainGame(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView )
{
  int i, active;
  int enabled[MAX_PLAYERS];
  static float fov_inc = 0.0F;
    RECT    src;
    HRESULT ddrval;

  QueryPerformanceCounter((LARGE_INTEGER *) &GameCurrentTime);

  if( PlayDemo )
  {
    if( PauseDemo )
    {
      TempGameElapsedTime = GameCurrentTime;
    }else{
      if( DemoScreenGrab && !CurrentMenu )
      {
        GameElapsedTime += (LONGLONG) ( ( ( 2 * ticksperframe ) / 1000.0F) * Freq );  // approx every 2 frames...
      }else{
        GameElapsedTime += (LONGLONG) ( ( GameCurrentTime - TempGameElapsedTime ) * Demoframelag );
      }
      TempGameElapsedTime = GameCurrentTime;

      GameCurrentTime = GameCurrentTime - GameStartedTime;
      GameCurrentTime = (LONGLONG) ( GameCurrentTime * Demoframelag );
    }
  }else{
    GameCurrentTime = GameCurrentTime - GameStartedTime;
  }




#ifdef DEBUG
  if ( framelag > 10.0F ) // check framelag out of reasonable range -> probably debugging
  {
    framelag = 10.0F; // clamp framelag to something reasonable
  }
#endif // DEBUG

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Main Routines to be called before Rendering....  
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  // set up groups indirectly visible by all active cameras
  /*
  // removed by Phil 11/11/98 ( VC6 warning )
  if( ServerMode && !PowerVR )
  {
    ServerCount++;
    if( ServerCount >= active )
    {
      ServerCount = 0;
    }
    InitIndirectVisible( Ships[enabled[ServerCount]].Object.Group );
  }
  else
  */
  {
    InitIndirectVisible( Ships[Current_Camera_View].Object.Group );
  }
  if( !PowerVR && ( ActiveRemoteCamera || (MissileCameraActive && MissileCameraEnable) ) )
  {
    AddIndirectVisible( (uint16) ( ( ActiveRemoteCamera ) ? ActiveRemoteCamera->Group : SecBulls[ CameraMissile ].GroupImIn ) );
  }

  MainRoutines();

  if( MyGameStatus == STATUS_QuitCurrentGame )
    return TRUE;


  
  TloadCheckForLostSurfaces(&Tloadheader);
  memset( (void*) &IsGroupVisible[0] , 0 , MAXGROUPS * sizeof(uint16) );
  cral += (framelag*2.0F);

  for( i = 0 ; i < MAX_SFX ; i++ )
  {
    LastDistance[i] = 100000.0F;
  }

#ifdef SOFTWARE_ENABLE
  if( SoftwareVersion )
  {
//    CWClearZBuffer();
  }
#endif

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Now the Rendering can begin...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  if (lpDev->lpVtbl->BeginScene(lpDev) != D3D_OK)
    return FALSE;


  if( ServerMode && !PowerVR )
  {
    CameraRendering = CAMRENDERING_Server;
      
    active = 0;
    for ( i = 0 ; i < MAX_PLAYERS ; i++ )
    {
      if( Ships[i].enable != 0 )
      {
        enabled[active] = i;
        active++;
      }
    }
    if( active > 1 )
    {
      src.top = 0;
      src.left = 0;
      src.right = d3dappi.szClient.cx;
      src.bottom = d3dappi.szClient.cy;
      while( 1 )
      {
        ddrval = d3dapp->lpBackBuffer->lpVtbl->BltFast( d3dapp->lpBackBuffer, 0, 0, d3dapp->lpFrontBuffer, &src, DDBLTFAST_WAIT );
          if( ddrval == DD_OK )
              break;
          if( ddrval == DDERR_SURFACELOST )
        {
      
          d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
          d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
              break;
        }
          if( ddrval != DDERR_WASSTILLDRAWING )
              break;
      }
    }
    
    Current_Camera_View = enabled[ServerCount];

    MainCamera.enable = 1;
    MainCamera.GroupImIn = Ships[enabled[ServerCount]].Object.Group;  
    MainCamera.Mat = Ships[enabled[ServerCount]].Object.FinalMat; 
    MainCamera.InvMat = Ships[enabled[ServerCount]].Object.FinalInvMat; 
    MainCamera.Pos = Ships[enabled[ServerCount]].Object.Pos;  
    MainCamera.Viewport = viewport; 
    MainCamera.Proj = proj; 

    
    MainCamera.Viewport.dwX = (long) ( viewport.dwX + ( ( viewport.dwWidth * Hdiv[active-1] ) * (Hpos[active-1][ServerCount] ) ) );
    MainCamera.Viewport.dwY = (long) ( viewport.dwY + ( ( viewport.dwHeight * Vdiv[active-1] ) * (Vpos[active-1][ServerCount]) ) );
    MainCamera.Viewport.dwWidth = (long) ( viewport.dwWidth * Hdiv[active-1] );
    MainCamera.Viewport.dwHeight = (long) ( viewport.dwHeight * Vdiv[active-1] );
    MainCamera.Viewport.dvScaleX = MainCamera.Viewport.dwWidth / (float)2.0;
    MainCamera.Viewport.dvScaleY = MainCamera.Viewport.dwHeight / (float)2.0;
    MainCamera.Viewport.dvMaxX = (float)D3DDivide(D3DVAL(MainCamera.Viewport.dwWidth),
                       D3DVAL(2 * MainCamera.Viewport.dvScaleX));
    MainCamera.Viewport.dvMaxY = (float)D3DDivide(D3DVAL(MainCamera.Viewport.dwHeight),
                       D3DVAL(2 * MainCamera.Viewport.dvScaleY));
  
    CurrentCamera = MainCamera;
    CurrentCamera.UseLowestLOD = TRUE;
    if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
      return FALSE;

  }else{

    if( !FullRearView )
    {
      CameraRendering = CAMRENDERING_Main;
      MainCamera.enable = 1;
      MainCamera.GroupImIn = Ships[Current_Camera_View].Object.Group; 
      MainCamera.Mat = Ships[Current_Camera_View].Object.FinalMat;  
      MainCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat;  
      MainCamera.Pos = Ships[Current_Camera_View].Object.Pos; 
      MainCamera.Viewport = viewport; 
      MainCamera.Proj = proj; 

      
      CurrentCamera = MainCamera;

      HUDNames();
      DispEnhancedCrosshair();

      CurrentCamera.UseLowestLOD = FALSE;
      if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
        return FALSE;

#ifdef Z_TRICK
      if ( !ZClearsOn )
      {
        g_OddFrame = !g_OddFrame;
        SetZProj();
        if( !SetZCompare() )
        {
          Msg("unable to set z compare\n");
          return FALSE;
        }
      }
#endif
  
      if( RearCameraActive && !PowerVR && !RearCameraDisable && !InSplashDemo )
      {
        CameraRendering = CAMRENDERING_Rear;

        CurrentCamera.enable = 1;
        CurrentCamera.UseLowestLOD = TRUE;
        CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;  
        CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
        CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
        CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
        CurrentCamera.Viewport = viewport;  
        CurrentCamera.Proj = proj;  
        
        CurrentCamera.InvMat._31 *= -1.0F;
        CurrentCamera.InvMat._32 *= -1.0F;
        CurrentCamera.InvMat._33 *= -1.0F;
        CurrentCamera.InvMat._11 *= -1.0F;
        CurrentCamera.InvMat._12 *= -1.0F;
        CurrentCamera.InvMat._13 *= -1.0F;
        
        CurrentCamera.Mat._13 *= -1.0F;
        CurrentCamera.Mat._23 *= -1.0F;
        CurrentCamera.Mat._33 *= -1.0F;
        CurrentCamera.Mat._11 *= -1.0F;
        CurrentCamera.Mat._21 *= -1.0F;
        CurrentCamera.Mat._31 *= -1.0F;
        
        CurrentCamera.Viewport.dwX = ( viewport.dwX + viewport.dwWidth ) - ( (viewport.dwWidth >>4) + ( viewport.dwWidth >>2 ) );
        CurrentCamera.Viewport.dwY = viewport.dwY + (viewport.dwHeight >>4);
        CurrentCamera.Viewport.dwWidth = viewport.dwWidth >>2;
        CurrentCamera.Viewport.dwHeight = viewport.dwHeight >>2;
        CurrentCamera.Viewport.dvScaleX = CurrentCamera.Viewport.dwWidth / (float)2.0;
        CurrentCamera.Viewport.dvScaleY = CurrentCamera.Viewport.dwHeight / (float)2.0;
        CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwWidth),
                           D3DVAL(2 * CurrentCamera.Viewport.dvScaleX));
        CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwHeight),
                           D3DVAL(2 * CurrentCamera.Viewport.dvScaleY));
        
        
        CurrentCamera.UseLowestLOD = TRUE;

  #ifdef SOFTWARE_ENABLE
        if( SoftwareVersion )
        {
          if( !CWRenderSmallCamera( lpDev , lpView ) )
            return FALSE;
        }
        else
  #endif
        {
          if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
            return FALSE;
        }
      }

      if( !PowerVR && ( ActiveRemoteCamera || (MissileCameraActive && MissileCameraEnable) ) )
      {
        float main_fov;

        TempMissileCam = Current_Camera_View;
        Current_Camera_View = -1;
        if( ActiveRemoteCamera )
        {
          CameraRendering = CAMRENDERING_Pip;

          CurrentCamera.enable = 1;
          CurrentCamera.GroupImIn = ActiveRemoteCamera->Group;  
          CurrentCamera.Mat = ActiveRemoteCamera->Mat;  
          CurrentCamera.InvMat = ActiveRemoteCamera->InvMat;  
          CurrentCamera.Pos = ActiveRemoteCamera->Pos;
        }else{
          CameraRendering = CAMRENDERING_Missile;

          CurrentCamera.enable = 1;
          CurrentCamera.GroupImIn = SecBulls[ CameraMissile ].GroupImIn;  
          CurrentCamera.Mat = SecBulls[ CameraMissile ].Mat;  
          MatrixTranspose( &SecBulls[ CameraMissile ].Mat, &CurrentCamera.InvMat );
          
          CurrentCamera.Pos = SecBulls[ CameraMissile ].Pos;
        }
        CurrentCamera.Viewport = viewport;
        main_fov = hfov;
        SetFOV( normal_fov ); // was SetFOV( START_FOV ), but this doesn't work for wide angle fov
        CurrentCamera.Proj = proj;  
        
        CurrentCamera.Viewport.dwX = viewport.dwX + (viewport.dwWidth >>4);
        CurrentCamera.Viewport.dwY = viewport.dwY + (viewport.dwHeight >>4);
        CurrentCamera.Viewport.dwWidth = viewport.dwWidth >>2;
        CurrentCamera.Viewport.dwHeight = viewport.dwHeight >>2;
        CurrentCamera.Viewport.dvScaleX = CurrentCamera.Viewport.dwWidth / (float)2.0;
        CurrentCamera.Viewport.dvScaleY = CurrentCamera.Viewport.dwHeight / (float)2.0;
        CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwWidth),
                           D3DVAL(2 * CurrentCamera.Viewport.dvScaleX));
        CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwHeight),
                           D3DVAL(2 * CurrentCamera.Viewport.dvScaleY));
        
        
        CurrentCamera.UseLowestLOD = TRUE;

  #ifdef SOFTWARE_ENABLE
        if( SoftwareVersion )
        {
          if( !CWRenderSmallCamera( lpDev , lpView ) )
            return FALSE;
        }
        else
  #endif
        {
          if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
            return FALSE;
        }

        Current_Camera_View=TempMissileCam;
        SetFOV( main_fov );

      }
    }else{
      // Full Screen Rear View....
      CameraRendering = CAMRENDERING_Rear;
      CurrentCamera.enable = 1;
      CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;  
      CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
      CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
      CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
      CurrentCamera.Viewport = viewport;  
      CurrentCamera.Proj = proj;  
      
      CurrentCamera.InvMat._31 *= -1.0F;
      CurrentCamera.InvMat._32 *= -1.0F;
      CurrentCamera.InvMat._33 *= -1.0F;
      CurrentCamera.InvMat._11 *= -1.0F;
      CurrentCamera.InvMat._12 *= -1.0F;
      CurrentCamera.InvMat._13 *= -1.0F;
      
      CurrentCamera.Mat._13 *= -1.0F;
      CurrentCamera.Mat._23 *= -1.0F;
      CurrentCamera.Mat._33 *= -1.0F;
      CurrentCamera.Mat._11 *= -1.0F;
      CurrentCamera.Mat._21 *= -1.0F;
      CurrentCamera.Mat._31 *= -1.0F;
  
      CurrentCamera.UseLowestLOD = FALSE;

#if 0
#ifdef SOFTWARE_ENABLE
      if( SoftwareVersion )
      {
        if( !CWRenderSmallCamera( lpDev , lpView ) )
          return FALSE;
      }
      else
#endif
#endif
      {
        if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
          return FALSE;
      }
#ifdef Z_TRICK
      if ( !ZClearsOn )
      {
        g_OddFrame = !g_OddFrame;
        SetZProj();
        if( !SetZCompare() )
        {
          Msg("unable to set z compare\n");
          return FALSE;
        }
      }
#endif
    }
  }

  if( DrawPanel && (WhoIAm == Current_Camera_View ))
  {
    Disp3dPanel( lpDev, lpView );
  }
  
  if( TargetComputerOn )
  {
      lpDev->lpVtbl->Execute(lpDev, lpD3DTransCmdBuf, lpView , D3DEXECUTE_CLIPPED);
    DispTracker( lpDev, lpView );
  }


#ifdef REFLECTION
  if( !InSplashDemo )
  {
    VECTOR  TempUp;
    CameraRendering = CAMRENDERING_Missile;
//    CameraRendering = CAMRENDERING_Main;

    SetFOV( 90.0F );
    if( Ships[WhoIAm ^ 1].enable )
    {
      Current_Camera_View = WhoIAm ^ 1;

      CurrentCamera.enable = 1;
      CurrentCamera.UseLowestLOD = FALSE;
      CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;

      ApplyMatrix( &Ships[WhoIAm].Object.Mat, &SlideUp, &TempUp );      /* Calc Direction Vector */

      MakeViewMatrix( &Ships[Current_Camera_View].Object.Pos, &Ships[WhoIAm].Object.Pos, &TempUp, &CurrentCamera.Mat);
      MatrixTranspose( &CurrentCamera.Mat, &CurrentCamera.InvMat );

//      CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
//      CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
      CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
      CurrentCamera.Viewport = viewport;  
      CurrentCamera.Proj = proj;  

      CurrentCamera.Viewport.dwX = 0;
      CurrentCamera.Viewport.dwY = 0;
      CurrentCamera.Viewport.dwWidth = 128;
      CurrentCamera.Viewport.dwHeight = 128;
      CurrentCamera.Viewport.dvScaleX = CurrentCamera.Viewport.dwWidth / (float)2.0;
      CurrentCamera.Viewport.dvScaleY = CurrentCamera.Viewport.dwHeight / (float)2.0;
      CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwWidth),
                         D3DVAL(2 * CurrentCamera.Viewport.dvScaleX));
      CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwHeight),
                         D3DVAL(2 * CurrentCamera.Viewport.dvScaleY));
      
      {
        if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
          return FALSE;
      }
    }
    Current_Camera_View = WhoIAm;
    SetFOV( START_FOV );
  }
#endif

  if (lpDev->lpVtbl->EndScene(lpDev) != D3D_OK)
        return FALSE;

#ifdef REFLECTION
  WierdShit();
#endif

  ScreenPolyProcess();
  DispHUDNames();
  DispUntriggeredNMEs();

#ifdef INSIDE_BSP
  Inside = PointInsideSkin( &Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group );
#endif

#ifdef SOFTWARE_ENABLE
  if( !SoftwareVersion )
#endif
  {
    if( Our_CalculateFrameRate() != TRUE)
      return FALSE;
  }


/* Secondary routines called after rendering */
    
  if( !PlayDemo )
  {
    DplayGameUpdate();
  }


  if( (Ships[WhoIAm].Object.Speed.z) > (MaxMoveSpeed) )
  {
    float diff;
    diff = ( Ships[WhoIAm].Object.Speed.z - MaxMoveSpeed ) / (  MaxTurboSpeed - MaxMoveSpeed );

    if( diff > 0.0F )
    {
      fov_inc += diff * diff * framelag;
      if ( fov_inc > 30.0F )
        fov_inc = 30.0F;
    }
    else
    {
      fov_inc *= (float) pow( 0.95, framelag );
    }
  }
  else
  {
    fov_inc *= (float) pow( 0.95, framelag );
  }
  SetFOV( chosen_fov + fov_inc );

#ifdef SOFTWARE_ENABLE
  if( !SoftwareVersion )
#endif
  {
    if( CurrentMenu && CurrentMenuItem )
    {
      MenuDraw( CurrentMenu );

      MenuItemDrawCursor( CurrentMenuItem );

      if ( DrawSimplePanel )
        TestBlt();
// Just to make sure that another press of escape doesnt take you back into the menu you wanted to exit!!
      JustExitedMenu = TRUE;
      
      MenuProcess();
    }
    else
    {
      if( !ServerMode )
      {
#ifndef REFLECTION
        if( !FullRearView )
          TestBlt();
#endif
      }
//      TaskDispatch();
    }
  }

  ProcessGameKeys();

  CheckForRogueSfx();

  CheckLevelEnd();

  return TRUE;
}

BOOL  ScoreDisplaySfx = TRUE;
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Init Score Display Stuff...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  InitScoreDisplay()
{
#ifdef SCROLLING_MESSAGES
  int i;
#endif
   //Create the offscreen surface, by loading our bitmap.

  if( ModeCase != -1 )
  {
    lpDDSOne = DDLoadBitmap( d3dapp->lpDD,/* (char*) &ScoreNames[ModeCase]*/ DynamicScoreNames , 0, 0 );
    ddpal =  DDLoadPalette( d3dapp->lpDD , /*(char*) &ScoreNames[ModeCase]*/DynamicScoreNames);
    lpDDSOne->lpVtbl->SetPalette( lpDDSOne , ddpal );
  }

  InitFont(TRUE);
  ChangeBackgroundColour( 0, 0, 0 );

#ifdef SCROLLING_MESSAGES
  // get random phrases...
  for (i = 0; i < Num_StatsMessage_Parts; i++)
  {
    StatsMessages[i].current_variation = Random_Range(StatsMessages[i].num_variations);
  }
#endif
  // process scores...
  ScoreSort();

  ScoreDisplaySfx = TRUE;

  PreventFlips = FALSE;

  return TRUE;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Free Score Display Stuff...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  FreeScoreDisplay()
{
  ReleaseDDSurf(lpDDSOne);  
  ReleaseDDSurf(lpDDSTwo);
  lpDDSTwo = NULL;
  lpDDSOne = NULL;
  return TRUE;
}

void ScrollingTeamMessage(char **str, int num_strings, int *col)
{
  static uint16 StartChar = 0;
  static float TotalShift = 0;
  char *message;
  int MaxChars, i;
  uint16 CurrentChar;
  int TotalLength;
  uint16 CurrentMessagePos, ProcessedChars, CurrentCharOffset, CurrentString, CurrentStrLen;
  int xpos;

  TotalShift += 0.5F * framelag;

  TotalLength = 0;
  for (i = 0; i < num_strings; i++)
    TotalLength += strlen( str[i] );

  if (StartChar > (TotalLength - 1))
    StartChar = 0;

  while( TotalShift >= FontWidth )
  {
    StartChar++;
    if (StartChar == TotalLength)
      StartChar = 0;
    TotalShift -= FontWidth;
  }

  MaxChars = (d3dappi.szClient.cx / FontWidth) + 2;

  message = (char *)calloc( MaxChars, sizeof(char) );
  
  CurrentChar = StartChar;
  CurrentMessagePos = 0;
  ProcessedChars = 0;
  xpos = 0 - (int)TotalShift;

  while( ProcessedChars++ <= MaxChars )
  {
    // find out which string CurrentChar is in...
    CurrentCharOffset = 0;
    for (CurrentString = 0; CurrentString < num_strings; CurrentString++)
    {
      if ( CurrentChar < strlen( str[CurrentString] ) + CurrentCharOffset )
      {
        CurrentStrLen = strlen( str[CurrentString] );
        break;
      }
      else
        CurrentCharOffset += strlen( str[CurrentString] );
    }

    // get next character from string, and add to message...
    message[CurrentMessagePos++] = str[CurrentString][CurrentChar++ - CurrentCharOffset];

    if ( (CurrentChar - CurrentCharOffset) == CurrentStrLen )
    {
      // display text so far...
      message[CurrentMessagePos] = 0;
      PrintClipped4x5Text( message, xpos, d3dappi.szClient.cy - (FontHeight*2) , col[CurrentString] );
      xpos += FontWidth * strlen( message );
      message[0] = 0;
      CurrentMessagePos = 0;

      if (CurrentString == num_strings - 1)
        CurrentChar = 0;
    }
  }
  // display remaining text...
  message[CurrentMessagePos] = 0;
  PrintClipped4x5Text( message, xpos, d3dappi.szClient.cy - (FontHeight*2) , col[CurrentString] );

  free (message);
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Score Display...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ScoreDisplay()
{
    RECT    src;
  RECT    dest;
    HRESULT ddrval;
  DDBLTFX fx;
  int x;
  int y;
  int i;
#ifdef  PHILHASTIME
  int e;
  char  TempStr[256];
#endif
  int col;
  int num_teams;
//  BOOL TeamOK[MAX_TEAMS];
  int16 TeamScore[MAX_TEAMS];
  static float pulse = 0.0F;
  BOOL pulseon;
  int16 myteam;
  BOOL TeamOK[MAX_TEAMS];
  int active_players;
  int count = 0;

  int Names1X;
  int Kills1X;
  int Deaths1X;
  int Names2X;
  int Kills2X;
  int Deaths2X;
  int YSpaceing = (FontHeight+(FontHeight>>1));


  active_players = 0;
  for( i = 0 ; i < MAX_PLAYERS ; i++ )
  {
    if ( ( (GameStatus[ i ] == STATUS_ViewingScore) ||
      (GameStatus[ i ] == STATUS_WaitingAfterScore ) ||
      (GameStatus[ i ] == STATUS_Left) ||
      ( ( GameStatus[ i ] >= STATUS_InitView_0 ) && ( GameStatus[ i ] <= STATUS_InitView_9 ) ) ) 
      && !( IsServerGame && i == 0 ) )
      active_players++;
  }

//  active_players = MAX_PLAYERS;

  if( active_players <= 16 )
  {

    Names1X = ( d3dappi.szClient.cx >> 1 ) -  (FontWidth*10);
    Kills1X = ( d3dappi.szClient.cx >> 1 ) +  (FontWidth*2);
    Deaths1X = ( d3dappi.szClient.cx >> 1 ) +  (FontWidth*10);
  }else{
    Names1X = ( d3dappi.szClient.cx >> 1 ) -  (FontWidth*10) - (FontWidth*20);
    Kills1X = ( d3dappi.szClient.cx >> 1 ) +  (FontWidth*2) - (FontWidth*20);
    Deaths1X = ( d3dappi.szClient.cx >> 1 ) +  (FontWidth*10) - (FontWidth*20);
    Names2X = ( d3dappi.szClient.cx >> 1 ) -  (FontWidth*10) + (FontWidth*10);
    Kills2X = ( d3dappi.szClient.cx >> 1 ) +  (FontWidth*2) + (FontWidth*10);
    Deaths2X = ( d3dappi.szClient.cx >> 1 ) +  (FontWidth*10) + (FontWidth*10);
  }




  pulse += framelag/60.0F;
  if (pulse > 1.0F)
    pulse -= (float)floor((double)pulse);

  if (pulse <= 0.5F)
    pulseon = TRUE;
  else
    pulseon = FALSE;

  //  Blt Background
  src.top = 0;
  src.left = 0;
  src.right = d3dappi.szClient.cx;
  src.bottom = d3dappi.szClient.cy;
  x = 0;
  y = 0;
  memset(&fx, 0, sizeof(DDBLTFX));
  fx.dwSize = sizeof(DDBLTFX);
  dest.top = 0;
  dest.bottom = d3dappi.szClient.cy;
  dest.left = 0;
  dest.right = d3dappi.szClient.cx;
      
  while( 1 )
  {
    ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, NULL, lpDDSOne, NULL, DDBLT_WAIT, &fx );
    if( ddrval == DD_OK )
      break;
    if( ddrval == DDERR_SURFACELOST )
    {
      d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      DDReLoadBitmap( lpDDSOne,/* (char*) &ScoreNames[ModeCase] */DynamicScoreNames );
      break;
    }
    if( ddrval != DDERR_WASSTILLDRAWING )
      break;
  }

  num_teams = 0;

  if (TeamGame)
  {
    memset(&TeamScore, 0, sizeof(int16) * MAX_TEAMS);
    memset(&TeamOK, FALSE, sizeof(BOOL) * MAX_TEAMS);

    for (i = 0; i < MAX_PLAYERS; i++)
    {
      if ( (TeamNumber[i] < MAX_TEAMS) &&
         ( (GameStatus[ i ] == STATUS_ViewingScore) ||
         (GameStatus[ i ] == STATUS_WaitingAfterScore ) ||
         ( ( GameStatus[ i ] >= STATUS_InitView_0 ) && ( GameStatus[ i ] <= STATUS_InitView_9 ) ) ) &&
         (Ships[i].Kills != -32767) )
      {
        if ( WhoIAm == ScoreSortTab[i] )
          myteam = TeamNumber[ i ];
        TeamScore[TeamNumber[i]] += Ships[i].Kills;
        if (!TeamOK[TeamNumber[i]])
        {
          num_teams++;
          TeamOK[TeamNumber[i]] = TRUE;
        }

        
      }
    }
  }

#ifdef  PHILHASTIME
  Print4x5Text( "kills" , ( d3dappi.szClient.cx >> 1 ) +  ( ( (active_players*FontWidth*4) + (2*FontWidth) + (1*FontWidth) ) >> 1 )  ,  ( d3dappi.szClient.cy >> 1 ) - ( ( (active_players*(FontHeight+(FontHeight/2))) + (FontHeight*5) ) >> 1 ), 1 );
  Print4x5Text( "deaths" , ( d3dappi.szClient.cx >> 1 ) -  ( ( (active_players*FontWidth*4) + (8*FontWidth) + (4*FontWidth) ) >> 1 )  ,  ( d3dappi.szClient.cy >> 1 ) + ( ( (active_players*(FontHeight+(FontHeight/2))) + (FontHeight*2) ) >> 1 ), 1 );
#else
  if( active_players <= 16 )
  {
 
    if( CTF || CaptureTheFlag || BountyHunt )
    {
      Print4x5Text( "Score" , ( d3dappi.szClient.cx >> 1 ) - (2*FontWidth) ,  ( d3dappi.szClient.cy >> 1 ) - ( ( (active_players*YSpaceing ) >> 1 ) + FontHeight  ) , 2 );
    }else{
      Print4x5Text( "kills" , ( d3dappi.szClient.cx >> 1 ) - (2*FontWidth) ,  ( d3dappi.szClient.cy >> 1 ) - ( ( (active_players*YSpaceing ) >> 1 ) + FontHeight  ) , 2 );
      Print4x5Text( "deaths" , ( d3dappi.szClient.cx >> 1 ) + (5*FontWidth) ,  ( d3dappi.szClient.cy >> 1 ) - ( ( (active_players*YSpaceing) >> 1 ) + FontHeight  ) , 1 );
    }
  }else{
    if( CTF || CaptureTheFlag || BountyHunt )
    {
      Print4x5Text( "Score" , Kills1X ,  ( d3dappi.szClient.cy >> 1 ) - ( ( ((active_players >> 2)*YSpaceing ) ) + FontHeight  ) , 2 );
      Print4x5Text( "Score" , Kills2X ,  ( d3dappi.szClient.cy >> 1 ) - ( ( ((active_players >> 2)*YSpaceing ) ) + FontHeight  ) , 2 );
    }else{
      Print4x5Text( "kills" , Kills1X ,  ( d3dappi.szClient.cy >> 1 ) - ( ( ((active_players >> 2)*YSpaceing ) ) + FontHeight  ) , 2 );
      Print4x5Text( "kills" , Kills2X ,  ( d3dappi.szClient.cy >> 1 ) - ( ( ((active_players >> 2)*YSpaceing ) ) + FontHeight  ) , 2 );
      Print4x5Text( "deaths" , Deaths1X ,  ( d3dappi.szClient.cy >> 1 ) - ( ( ((active_players >> 2)*YSpaceing) ) + FontHeight  ) , 1 );
      Print4x5Text( "deaths" , Deaths2X ,  ( d3dappi.szClient.cy >> 1 ) - ( ( ((active_players >> 2)*YSpaceing) ) + FontHeight  ) , 1 );
    }
  }

#endif

  for( i = 0 ; i < MAX_PLAYERS ; i++ )
  {
    if( ( (GameStatus[ ScoreSortTab[i] ] == STATUS_ViewingScore) ||
      (GameStatus[ ScoreSortTab[i] ] == STATUS_WaitingAfterScore ) ||
      (GameStatus[ ScoreSortTab[i] ] == STATUS_Left) ||
      ( ( GameStatus[ ScoreSortTab[i] ] >= STATUS_InitView_0 ) && ( GameStatus[ ScoreSortTab[i] ] <= STATUS_InitView_9 ) ) )
      && !( IsServerGame && ScoreSortTab[i] == 0 ) )
    {
      if (TeamGame)
      {
        if (WhoIAm == ScoreSortTab[i])
        {
          if( ScoreDisplaySfx )
          {
            if( i == 0 )
            {
              // Gee im top of the table...
              PlaySfx( SFX_BIKER_VP, 1.0F );
            }else if( i == active_players-1)
            {
              // oow im at the bottom
              PlaySfx( SFX_BIKER_LP, 1.0F );
            }
            ScoreDisplaySfx = FALSE;
          }

          if (pulseon)
            col = 0;
          else
            col = TeamCol[TeamNumber[WhoIAm]];
        }else
          col = TeamCol[TeamNumber[ScoreSortTab[i]]];
      }else
      {
        if (WhoIAm == ScoreSortTab[i])
        {
          if( ScoreDisplaySfx )
          {
            if( i == 0 )
            {
              // Gee im top of the table...
              PlaySfx( SFX_BIKECOMP_GK, 1.0F );
            }else if( i == active_players-1)
            {
              // oow im at the bottom
              PlaySfx( SFX_BIKECOMP_PK, 1.0F );
            }
            ScoreDisplaySfx = FALSE;
          }
          col = 0;

        }
        else
          col = 2;
      }
#ifdef  PHILHASTIME
      TempStr[0] = Names[ScoreSortTab[i]][0];
      TempStr[1] = 0;
      // print up first letter of name..
      Print4x5Text( (char*) &TempStr ,  ( ( d3dappi.szClient.cx >> 1 ) -  ( (active_players*FontWidth*4) >> 1 ) )  +  ( i*(4*FontWidth) )  , ( d3dappi.szClient.cy >> 1 ) - ( ( (active_players*(FontHeight+(FontHeight/2))) + (FontHeight*5) ) >> 1 ), col  );
      // print up fiorst 5 chars of the name...
      for( e= 0 ; e < 5 ; e++ )
      {
        TempStr[e] = Names[ScoreSortTab[i]][e];
      }
      TempStr[5] = 0;
      Print4x5Text( (char*) &TempStr , ( d3dappi.szClient.cx >> 1 ) -  ( ( (active_players*FontWidth*4) + (8*FontWidth) + (4*FontWidth) ) >> 1 )  ,  ( d3dappi.szClient.cy >> 1 ) - ( ( (active_players*(FontHeight+(FontHeight/2))) + (FontHeight*2)  >> 1 ) ) + (i * (FontHeight+(FontHeight/2))), col);
      // print kills...
      Printint16( Ships[ScoreSortTab[i]].Kills , ( d3dappi.szClient.cx >> 1 ) +  ( ( (active_players*FontWidth*4) + (2*FontWidth) + (1*FontWidth) ) >> 1 )  ,  ( d3dappi.szClient.cy >> 1 ) - ( ( (active_players*(FontHeight+(FontHeight/2))) + (FontHeight*2) ) >> 1 ) + (i * (FontHeight+(FontHeight/2))), col);
      // print deaths...
      Printuint16( Ships[ScoreSortTab[i]].Deaths ,  ( ( d3dappi.szClient.cx >> 1 ) -  ( (active_players*FontWidth*4) >> 1 ) ) + ( ( i*(4*FontWidth) )  )  ,  ( d3dappi.szClient.cy >> 1 ) + ( ( (active_players*(FontHeight+(FontHeight/2))) + (FontHeight*2) ) >> 1 ), 1 );
      
      for( e = 0 ; e < MAX_PLAYERS ; e++ )
      {
        // print stats...
        if( ( GameStatus[ScoreSortTab[e]] == STATUS_Left) || ( Ships[ScoreSortTab[e]].enable)  )
        {
          
          if( i == e )
             Printuint16( Stats[ScoreSortTab[i]][ScoreSortTab[e]] ,  ( ( d3dappi.szClient.cx >> 1 ) -  ( (active_players*FontWidth*4) >> 1 ) ) + ( e*(4*FontWidth) )  ,  ( d3dappi.szClient.cy >> 1 ) - ( ( (active_players*(FontHeight+(FontHeight/2))) + (FontHeight*2) ) >> 1 ) + (i * (FontHeight+(FontHeight/2))), 1 );
          else Printuint16( Stats[ScoreSortTab[i]][ScoreSortTab[e]] ,  ( ( d3dappi.szClient.cx >> 1 ) -  ( (active_players*FontWidth*4) >> 1 ) ) + ( e*(4*FontWidth) )  ,  ( d3dappi.szClient.cy >> 1 ) - ( ( (active_players*(FontHeight+(FontHeight/2))) + (FontHeight*2) ) >> 1 ) + (i * (FontHeight+(FontHeight/2))), 0 );
        }
      }
#else
      if( active_players < 16 )
      {
        Print4x5Text( (char*) &Names[ScoreSortTab[i]][0] , Names1X ,  ( ( d3dappi.szClient.cy >> 1 ) - (active_players*YSpaceing>>1) ) + (count * YSpaceing) , col);
        // print kills...
        Printint16( Ships[ScoreSortTab[i]].Kills , Kills1X  ,  ( ( d3dappi.szClient.cy >> 1 ) - (active_players*YSpaceing>>1) ) + (count * YSpaceing), col);
        // print deaths...
        if( !CTF && !CaptureTheFlag && !BountyHunt )
        {
          Printuint16( Ships[ScoreSortTab[i]].Deaths , Deaths1X  ,  ( ( d3dappi.szClient.cy >> 1 ) - (active_players*YSpaceing>>1) ) + (count * YSpaceing), 1 );
        }
      }else{
        if( count < (active_players >> 1) )
        {
          Print4x5Text( (char*) &Names[ScoreSortTab[i]][0] , Names1X ,  ( ( d3dappi.szClient.cy >> 1 ) - ((active_players >> 2)*YSpaceing) ) + (count * YSpaceing) , col);
          // print kills...
          Printint16( Ships[ScoreSortTab[i]].Kills , Kills1X  ,  ( ( d3dappi.szClient.cy >> 1 ) - ((active_players >> 2)*YSpaceing) ) + (count * YSpaceing), col);
          // print deaths...
          if( !CTF && !CaptureTheFlag && !BountyHunt )
          {
            Printuint16( Ships[ScoreSortTab[i]].Deaths , Deaths1X  ,  ( ( d3dappi.szClient.cy >> 1 ) - ((active_players >> 2)*YSpaceing ) ) + (count * YSpaceing), 1 );
          }
        }else{
          Print4x5Text( (char*) &Names[ScoreSortTab[i]][0] , Names2X ,  ( ( d3dappi.szClient.cy >> 1 ) - ((active_players >> 2)*YSpaceing) ) + ((count-(active_players>>1)) * YSpaceing) , col);
          // print kills...
          Printint16( Ships[ScoreSortTab[i]].Kills , Kills2X  ,  ( ( d3dappi.szClient.cy >> 1 ) - ((active_players >> 2)*YSpaceing) ) + ((count-(active_players>>1)) * YSpaceing), col);
          // print deaths...
          if( !CTF && !CaptureTheFlag && !BountyHunt )
          {
            Printuint16( Ships[ScoreSortTab[i]].Deaths , Deaths2X  ,  ( ( d3dappi.szClient.cy >> 1 ) - ((active_players >> 2)*YSpaceing ) ) + ((count-(active_players>>1)) * YSpaceing), 1 );
          }

        }
      }

      count++;
#endif
    }
  }

  if ( TeamGame )
  {
    char buf[MAX_TEAMS][32];
    int buf_width[ MAX_TEAMS ];
    int buf_col[ MAX_TEAMS ];
    int num_bufs = 0;
    int total_width = 0;
    int gap_width = FontWidth * 3;
    int bufxpos[ MAX_TEAMS ];
    int total_screen_width = d3dappi.szClient.cx;

    for ( i = 0; i < MAX_TEAMS; i++ )
    {
      if ( TeamOK[ i ] )
      {
        sprintf( buf[ num_bufs ], "%-8s %d", TeamName[ i ], TeamScore[ i ] );
        buf_width[ num_bufs ] = strlen( buf[ num_bufs ] ) * FontWidth;
        total_width += buf_width[ num_bufs ];

        if ( i == TeamNumber[ WhoIAm ] )
          buf_col[ num_bufs ] = pulseon ? 0 : TeamCol[ i ];
        else
          buf_col[ num_bufs ] = TeamCol[ i ];

        num_bufs++;

      }
    }

    total_width += gap_width * ( num_bufs - 1 );

    for( i = 0; i < num_bufs; i++ )
    {
      if ( !i )
        bufxpos[ i ] = ( total_screen_width / 2 ) - ( total_width / 2 );
      else
        bufxpos[ i ] = bufxpos[ i - 1 ] + buf_width[ i - 1 ] + gap_width; 

      Print4x5Text( buf[ i ] ,bufxpos[ i ],  d3dappi.szClient.cy - (FontHeight*4) , 
         buf_col[ i ]);

    }
  }

  return TRUE;
}

#if 0
  RECT rectOverlaySource, rectOverlayDest;
  DDOVERLAYFX ddofx;

  lpDDSOverlay =  DDLoadBitmapOverlay( lpDD, (char*) &PanelNames[0] , 320, 80 );
  DDSetColorKey( lpDDSOverlay, RGB_MAKE( 255 , 0 , 255 ) );
  
    /* Set the overlay source rect */
    SetRect(&rectOverlaySource,0,0,320,80);
    /* Fill the entire display with the overlay */
    SetRect(&rectOverlayDest, 0 , 0 , 320 , 80 );

  memset(&ddofx, 0, sizeof(DDOVERLAYFX));
    ddofx.dwSize = sizeof(DDOVERLAYFX);
    /* Position, set colorkey values, and show the front overlay buffer */
    lpDDSOverlay->lpVtbl->UpdateOverlay( lpDDSOverlay , &rectOverlaySource,
                                      lpDDSOverlay,
                                      &rectOverlayDest,
                                      DDOVER_SHOW | DDOVER_KEYDESTOVERRIDE,
                                      &ddofx);
#endif

#if 0
      memset(&fx, 0, sizeof(DDBLTFX));
      fx.dwSize = sizeof(DDBLTFX);
      fx.dwAlphaSrcConstBitDepth = 8;
      fx.dwAlphaSrcConst = 128;
      fx.dwAlphaDestConstBitDepth = 8;
      fx.dwAlphaDestConst = 255;
      dest.top = d3dappi.szClient.cy-PanelVisibleY[ModeCase];
      dest.bottom = d3dappi.szClient.cy;
      dest.left = 0;
      dest.right = d3dappi.szClient.cx;
#endif
#if 0
        ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, &dest, lpDDSThree, &src, DDBLT_KEYSRC | DDBLT_WAIT | DDBLT_ALPHASRCCONSTOVERRIDE | DDBLT_ALPHADESTCONSTOVERRIDE | DDBLT_ALPHADEST | DDBLT_ALPHASRC , &fx );
#endif

#if defined( FINAL_RELEASE )
BOOL NoDebugMsgs = FALSE;
#else
BOOL NoDebugMsgs = FALSE;
#endif

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Check for a valid Crc in a Dplay Message..
  Input   :   int8  * String
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void DebugPrintf( const char * format, ... )
{
#ifndef FINAL_RELEASE
    static char buf1[256], buf2[512];
  va_list args;

  if ( NoDebugMsgs )
    return;

  va_start( args, format );
  vsprintf( buf1, format, args );
  wsprintf( buf2, "%hs", buf1 );

  OutputDebugString( buf2 );
  va_end( args );

  AddCommentToLog( buf2 );
#endif
}



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   General blit fast...
  Input   :   int   srcx,srcy , width , height , dstx , dsty
        :   LPDIRECTDRAWSURFACE Surface
        :   char * FileName , LPDIRECTDRAWSURFACE DestSurface
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#ifdef  USEINLINE
_inline
#endif
void GeneralBltFast( int srcx, int srcy , int w , int h  , int dstx , int dsty , LPDIRECTDRAWSURFACE SrcSurface ,   char * FileName , DWORD flags , LPDIRECTDRAWSURFACE DestSurface)
{
    RECT    src;
    HRESULT ddrval;
  src.top = srcy;
  src.left = srcx;
  src.right = src.left+w;
  src.bottom = src.top+h;
  while( 1 )
  {
    ddrval = DestSurface->lpVtbl->BltFast( DestSurface, dstx, dsty, SrcSurface, &src, flags );
      if( ddrval == DD_OK )
          break;
      if( ddrval == DDERR_SURFACELOST )
    {
  
      d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      DestSurface->lpVtbl->Restore(DestSurface);
      DDReLoadBitmap( SrcSurface, FileName );
          break;
    }
      if( ddrval != DDERR_WASSTILLDRAWING )
          break;
  }
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   General blit fast...
  Input   :   int   srcx,srcy , width , height , dstx , dsty ,dstw , dsth ,
        :   LPDIRECTDRAWSURFACE Surface
        :   char * FileName , LPDIRECTDRAWSURFACE DestSurface
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#ifdef  USEINLINE
//_inline
#endif
void GeneralBlt( int srcx, int srcy , int w , int h  , int dstx , int dsty , int dstw , int dsth , LPDIRECTDRAWSURFACE SrcSurface ,   char * FileName , DWORD flags , LPDIRECTDRAWSURFACE DestSurface)
{
  RECT  dst;
    RECT    src;
  DDBLTFX fx;
    HRESULT ddrval;
  src.top = srcy;
  src.left = srcx;
  src.right = src.left+w;
  src.bottom = src.top+h;

  dst.top = dsty;
  dst.left = dstx;
  dst.right = dst.left+dstw;
  dst.bottom = dst.top+dsth;

    memset(&fx, 0, sizeof(DDBLTFX));
    fx.dwSize = sizeof(DDBLTFX);

  
  while( 1 )
  {
    ddrval = DestSurface->lpVtbl->Blt( DestSurface, &dst, SrcSurface, &src, DDBLT_WAIT | DDBLT_ASYNC , &fx );
      if( ddrval == DD_OK )
          break;
      if( ddrval == DDERR_SURFACELOST )
    {
      d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      DestSurface->lpVtbl->Restore(DestSurface);
      DDReLoadBitmap( SrcSurface, FileName );
          break;
    }
      if( ddrval != DDERR_WASSTILLDRAWING )
          break;
  }
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Do font blt..
  Input   :   int sx , int sy , int sw , int sh , int x ,int y
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#ifdef  USEINLINE
_inline
#endif
void DoFontBlt(int sx , int sy , int sw , int sh , int x ,int y)
{
    RECT    src;
    HRESULT ddrval;
  src.left = sx;
  src.top = sy;
  src.right = sx+sw;
  src.bottom = sy+sh;
  while( 1 )
  {
    ddrval = d3dapp->lpBackBuffer->lpVtbl->BltFast( d3dapp->lpBackBuffer, x, y, lpDDSTwo, &src, DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT );
      if( ddrval == DD_OK )
          break;
      if( ddrval == DDERR_SURFACELOST )
    {
      d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      ReInitFont();
          break;
    }
      if( ddrval != DDERR_WASSTILLDRAWING )
          break;
  }
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Clear the Zbuffer / screen 
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  ClearBuffers( BOOL ClearScreen, BOOL ClearZBuffer )
{
    int clearflags;
    D3DRECT dummy;
    if (!d3dappi.bRenderingIsOK)
  {
        return FALSE;
    }
    /*
     * Decided wether to clear just back buffer or also z-buffer
     */

  clearflags = 0;


  if( myglobs.bClearsOn || ClearScreen )  // toggle clearing the screen...
  {
    clearflags |= D3DCLEAR_TARGET;
  }

#ifndef SOFTWARE_ENABLE
#ifdef Z_TRICK
  if ( ZClearsOn || ClearZBuffer )  // never clear Z buffer unless told to...
#else
  if (d3dapprs.bZBufferOn || ClearZBuffer ) // If a ZBuffer is enabled then always clear it..
#endif
  {
    clearflags |= D3DCLEAR_ZBUFFER;
    }
#endif

#ifdef SOFTWARE_ENABLE
  if ( ( myglobs.bClearsOn || ClearScreen || ClearZBuffer ) && SoftwareVersion )
  {
    CWClearZBuffer();
  }
#endif

  if( clearflags != 0 )
  {
#ifdef SOFTWARE_ENABLE
    if( SoftwareVersion )
    {
      if (RenderingSmall)
      {
        dummy.x1 = CurrentCamera.Viewport.dwX+RenderingSmallXOff;
        dummy.y1 = CurrentCamera.Viewport.dwY+RenderingSmallYOff;
        dummy.x2 = CurrentCamera.Viewport.dwX+CurrentCamera.Viewport.dwWidth+RenderingSmallXOff;
        dummy.y2 = CurrentCamera.Viewport.dwY+CurrentCamera.Viewport.dwHeight+RenderingSmallYOff;

        
      }
      else
      {
        dummy.x1 = CurrentCamera.Viewport.dwX;
        dummy.y1 = CurrentCamera.Viewport.dwY;
        dummy.x2 = CurrentCamera.Viewport.dwX+CurrentCamera.Viewport.dwWidth;
        dummy.y2 = CurrentCamera.Viewport.dwY+CurrentCamera.Viewport.dwHeight;
      }
    }
    else
#endif
    {
      dummy.x1 = CurrentCamera.Viewport.dwX;
      dummy.y1 = CurrentCamera.Viewport.dwY;
      dummy.x2 = CurrentCamera.Viewport.dwX+CurrentCamera.Viewport.dwWidth;
      dummy.y2 = CurrentCamera.Viewport.dwY+CurrentCamera.Viewport.dwHeight;
    }

    LastError =
              d3dappi.lpD3DViewport->lpVtbl->Clear(d3dappi.lpD3DViewport,
                                                   1, &dummy,
                                                   clearflags);
    if (LastError != D3D_OK)
    {
        return FALSE;
    }
  }
    return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Clear the Zbuffer
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  ClearZBuffer()
{
    int clearflags;
    D3DRECT dummy;

    if (!d3dappi.bRenderingIsOK) return FALSE;

  clearflags = D3DCLEAR_ZBUFFER;

  dummy.x1 = CurrentCamera.Viewport.dwX;
  dummy.y1 = CurrentCamera.Viewport.dwY;
  dummy.x2 = CurrentCamera.Viewport.dwX+CurrentCamera.Viewport.dwWidth;
  dummy.y2 = CurrentCamera.Viewport.dwY+CurrentCamera.Viewport.dwHeight;

  LastError = d3dappi.lpD3DViewport->lpVtbl->Clear( d3dappi.lpD3DViewport, 1, &dummy, clearflags );
  if (LastError != D3D_OK) return FALSE;
    return TRUE;
}


void InitRenderBufs( LPDIRECT3DDEVICE lpDev )
{
  int Count;
  D3DEXECUTEBUFFERDESC debdesc;

  for( Count = 0; Count < 2; Count++ )
  {
    if( RenderBufs[ Count ] != NULL )
    {
      XRELEASE( RenderBufs[ Count ] );
      RenderBufs[ Count ] = NULL;
    }
    MakeExecuteBuffer( &debdesc, lpDev, &RenderBufs[ Count ], 32767 );
  }
}


void ReleaseRenderBufs( void )
{
  int Count;

  for( Count = 0; Count < 2; Count++ )
  {
    if( RenderBufs[ Count ] != NULL )
    {
      XRELEASE( RenderBufs[ Count ] );
      RenderBufs[ Count ] = NULL;
    }
  }
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Render 1 Frame Using CurrentCamera...
  Input   :
  Output    : BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  BOOL  ClearScrOverride;
  BOOL  ClearZOverride;

BOOL  RenderCurrentCamera( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView )
{
    HRESULT rval;
  int16 Count;
  VISGROUP  *g;
  uint16  group;
  uint16  i;
  float R, G, B;
  NumOfTransExe = 0;

  Build_View();
  CurrentCamera.View = view;
  if (lpDev->lpVtbl->SetMatrix(lpDev, hView, &view) != D3D_OK)
    return FALSE;

    rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &CurrentCamera.Viewport);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "RenderCurrentCamera1", &CurrentCamera.Viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }


  // Ship Model Enable/Disable
  SetShipsVisibleFlag();

  // PowerVr Translucent PolySort....
  if( !InitPolySort() )
    return FALSE;

  
  // find visible groups
  FindVisible( &CurrentCamera, &Mloadheader );

  BuildVisibleLightList( CurrentCamera.GroupImIn );
  
  UpdateBGObjectsClipGroup( &CurrentCamera );
  UpdateEnemiesClipGroup( &CurrentCamera );

  ClearScrOverride = FALSE;
  ClearZOverride = FALSE;
  if( CurrentCamera.GroupImIn != (uint16) -1 && !DebugVisible )
  {
    for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
    {
      if( Mloadheader.Group[ g->group ].BGClear_Flag )
      {
        if ( Mloadheader.Group[ g->group ].BGClear_Flag & 0x80 )
        {
          R = Mloadheader.Group[ g->group ].BGClear_Red;
          G = Mloadheader.Group[ g->group ].BGClear_Green;
          B = Mloadheader.Group[ g->group ].BGClear_Blue;
          ChangeBackgroundColour( R, G, B );
          ClearScrOverride = TRUE;
        }
#ifdef SOFTWARE_ENABLE
        else
          ClearZOverride = TRUE; // this never had any effect before Z_TRICK, now it messes it up, so...get rid!
#endif
        break;
      }
    }
  }

  if (ClearBuffers( ClearScrOverride, ClearZOverride ) != TRUE )
    return FALSE;

  // reset all the normal execute status flags...
#ifdef SOFTWARE_ENABLE
  if( SoftwareVersion )
  {
    CWExecute2( lpDev, lpD3DNormCmdBuf, lpView , D3DEXECUTE_CLIPPED );
  }
  else
#endif
  {
      lpDev->lpVtbl->Execute(lpDev, lpD3DNormCmdBuf, lpView , D3DEXECUTE_CLIPPED);
  }

  // set all the Translucent execute status flags...
  if( WhiteOut != 0.0F)
  {
#ifdef SOFTWARE_ENABLE
    if( SoftwareVersion )
    {
      CWExecute2( lpDev, lpD3DSpcFxTransCmdBuf, lpView , D3DEXECUTE_CLIPPED );
    }
    else
#endif
    {
      lpDev->lpVtbl->Execute(lpDev, lpD3DSpcFxTransCmdBuf, lpView , D3DEXECUTE_CLIPPED);
    }
  }


  // display background
#ifdef SOFTWARE_ENABLE
/*-----------------------------------------------------------------------------
    Chris Walsh's Code
-----------------------------------------------------------------------------*/
  if( SoftwareVersion )
  {
    if ( !CWDisplayBackground( &Mloadheader, &CurrentCamera ) )
      return FALSE;
  }
  else
/*---------------------------------------------------------------------------*/
#endif
  {
    if ( !DisplayBackground( &Mloadheader, &CurrentCamera ) )
      return FALSE;
  }

  // reset all the normal execute status flags...
#ifdef SOFTWARE_ENABLE
  if( SoftwareVersion )
  {
    CWExecute2( lpDev, lpD3DNormCmdBuf, lpView , D3DEXECUTE_CLIPPED );
  }
  else
#endif
  {
    if( WhiteOut == 0.0F)
        lpDev->lpVtbl->Execute(lpDev, lpD3DNormCmdBuf, lpView , D3DEXECUTE_CLIPPED);
  }

  if( !bPolySort )
  {
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Non Group Clipped Non Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
    if( !DisplaySolidGroupUnclippedPolys( RenderBufs[ 0 ], lpDev, lpView ) )
        return FALSE;

#ifdef SHADOWTEST
    if( !DisplaySolidGroupUnclippedTriangles( RenderBufs[ 0 ], lpDev, lpView ) )
        return FALSE;
#endif
  }

  
  // display clipped opaque objects
  for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
  {
    group = g->group;

    // Do the Background animation for that group.....
    BackGroundTextureAnimation( &Mloadheader , group );

#ifdef CLIP_LINES
    ClipGroup( &CurrentCamera, group );
#else
    ClipGroup( &CurrentCamera, CurrentCamera.GroupImIn );
#endif

#ifndef FINAL_RELEASE
//    if( d3dapp->bIsPrimary )
    {
      i = FirstLineUsed;
      while( i != (uint16) -1 )
      {
        if( LinesDispGroup( group, RenderBufs[ 0 ], &i ) )
        {
          if( lpDev->lpVtbl->Execute(lpDev, RenderBufs[ 0 ], lpView, D3DEXECUTE_CLIPPED ) != D3D_OK )
            return FALSE;
        }
      }
    }
#endif

    ClipGroup( &CurrentCamera, group );


    if( MyGameStatus == STATUS_SplashScreen )
      GroupWaterProcessDisplay( group );

    if ( !LimitedLoad )
    {
      if( !ModelDisp( group, lpDev, &ModelNames[0] ) )
        return FALSE;
    }else
    {
      if( !ModelDisp( group, lpDev, &SplashModelNames[0] ) )
        return FALSE;
    }

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Group Clipped Non Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
    if( !bPolySort )
    {
      if( !DisplaySolidGroupClippedPolys( RenderBufs[ 1 ], group, lpDev, lpView ) )
        return FALSE;
#ifdef SHADOWTEST
      if( !DisplaySolidGroupClippedTriangles( RenderBufs[ 1 ], group, lpDev, lpView ) )
        return FALSE;
#endif
    }

//    if (Mod_Ship_Exec_Buffer( group, lpDev, lpView ) != TRUE)
//      return FALSE;
  }

  ClipGroup( &CurrentCamera, CurrentCamera.GroupImIn );
//  DisplayWaterMesh();

  // set all the Translucent execute status flags...
#ifdef SOFTWARE_ENABLE
  if( SoftwareVersion )
  {
    CWExecute2( lpDev, lpD3DTransCmdBuf, lpView , D3DEXECUTE_CLIPPED );
  }
  else
#endif
  {
      lpDev->lpVtbl->Execute(lpDev, lpD3DTransCmdBuf, lpView , D3DEXECUTE_CLIPPED);
  }


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Skin & BSP Node Info
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  if( ShowSkin || OldNodeCube || NodeCube || ShowTrigZones || ShowColZones || ShowEFZones || ShowTeleports )
  {
    if( OldNodeCube || NodeCube ) ClearZBuffer();

    for( Count = 0; Count < MAXGROUPS; Count++ )
    {
      if( Skin_Execs[ Count ] != NULL )
      {
#ifdef SOFTWARE_ENABLE
        if( SoftwareVersion )
        {
          CWExecute2( lpDev, Skin_Execs[ Count ], lpView , D3DEXECUTE_CLIPPED );
        }
        else
#endif
        {
          if (lpDev->lpVtbl->Execute(lpDev, Skin_Execs[ Count ], lpView , D3DEXECUTE_CLIPPED) != D3D_OK)
            return FALSE;
        }
      }
    }

#if 0
    for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
    {
      group = g->group;
      ClipGroup( &CurrentCamera, group );
  
//      if( d3dapp->bIsPrimary )
      {
        i = FirstLineUsed;
        while( i != (uint16) -1 )
        {
          if( LinesDispGroup( group, RenderBufs[ 0 ], &i ) )
          {
            if( lpDev->lpVtbl->Execute(lpDev, RenderBufs[ 0 ], lpView, D3DEXECUTE_CLIPPED ) != D3D_OK )
              return FALSE;
          }
        }
      }
    }
#endif

  }

#ifdef SOFTWARE_ENABLE
  if( SoftwareVersion )
  {
    CWExecute2( lpDev, lpD3DTransCmdBuf, lpView , D3DEXECUTE_CLIPPED);
  }
  else
#endif
  {
      lpDev->lpVtbl->Execute(lpDev, lpD3DTransCmdBuf, lpView , D3DEXECUTE_CLIPPED);
  }

  // display clipped translucencies
  for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
  {
    group = g->group;
    ClipGroup( &CurrentCamera, group );
    if( MyGameStatus != STATUS_SplashScreen )
      GroupWaterProcessDisplay( group );



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Group Clipped Non Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
    if( !bPolySort )
    {
      if( !DisplayGroupClippedPolys( RenderBufs[ 1 ], group, lpDev, lpView ) )
        return FALSE;

#ifdef SHADOWTEST
      if( !DisplayGroupClippedTriangles( RenderBufs[ 1 ], group, lpDev, lpView ) )
        return FALSE;
#endif

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Group Clipped Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
      if( !DisplayGroupClippedFmPolys( RenderBufs[ 0 ], group, lpDev, lpView ) )
          return FALSE;

      ExecuteTransExe( group );
    }
    else
    {
      i = FirstFmPolyUsed;
      while( i != (uint16) -1 )
      {
        PVR_FmPolyDispGroup( group, &i );
      }

      i = FirstPolyUsed;
      while( i != (uint16) -1 )
      {
        PVR_PolyDispGroup( group, &i );
      }

      ExecuteTransExe( group );
    }

  }


  ClipGroup( &CurrentCamera, CurrentCamera.GroupImIn );


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Non Group Clipped Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  if( !bPolySort )
  {
    if( !DisplayGroupUnclippedFmPolys( RenderBufs[ 0 ], lpDev, lpView ) )
        return FALSE;

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Non Group Clipped Non Faceme Transluecent Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
    if( !DisplayGroupUnclippedPolys( RenderBufs[ 0 ], lpDev, lpView ) )
        return FALSE;
#ifdef SHADOWTEST
    if( !DisplayGroupUnclippedTriangles( RenderBufs[ 0 ], lpDev, lpView ) )
      return FALSE;
#endif
  }
  
  // display unclipped translucencies
  for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
  {
    group = g->group;
    ExecuteTransExeUnclipped( group );
  }


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Portals
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  if( ShowPortal )
  {
    for( Count = 0; Count < MAXGROUPS; Count++ )
    {
      if( Portal_Execs[ Count ] != NULL )
      {
#ifdef SOFTWARE_ENABLE
        if( SoftwareVersion )
        {
          CWExecute2( lpDev, Portal_Execs[ Count ], lpView , D3DEXECUTE_CLIPPED );
        }
        else
#endif
        {
          if (lpDev->lpVtbl->Execute(lpDev, Portal_Execs[ Count ], lpView , D3DEXECUTE_CLIPPED) != D3D_OK)
            return FALSE;
        }
      }
    }
  }

  // PowerVR Translucent Poly Sorted Display...
  if( !PolyListExecute() )
    return FALSE;

  
  
  DoLensflareEffect();
  DoAllSecBullLensflare();

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Transluecent Screen Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  if( !bPolySort )
  {
    if( !DisplayNonSolidScrPolys( RenderBufs[ 1 ], lpDev, lpView ) )
      return FALSE;
  }
  else
  {
    while( i != (uint16) -1 )
    {
      PVR_ScreenPolysDisp( &i );
    }
  }

  // reset all the normal execute status flags...
#ifdef SOFTWARE_ENABLE
  if( SoftwareVersion )
  {
    CWExecute2( lpDev, lpD3DNormCmdBuf, lpView , D3DEXECUTE_CLIPPED);
  }
  else
#endif
  {
      lpDev->lpVtbl->Execute(lpDev, lpD3DNormCmdBuf, lpView , D3DEXECUTE_CLIPPED);
  }


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Display Solid Screen Polys
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  BilinearSolidScrPolys = FALSE;

  if( !DisplaySolidScrPolys( RenderBufs[ 1 ], lpDev, lpView ) )
    return FALSE;

  rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "RenderCurrentCamera2", &viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }

#ifdef SOFTWARE_ENABLE

  if( SoftwareVersion )
  {
    SetVTables();
    SWRendView();
    d3dappi.lpBackBuffer->lpVtbl->Unlock( d3dappi.lpBackBuffer, NULL );
  }

#endif

  return TRUE;
}
  

extern  int   NumOfVertsConsidered;
extern  int   NumOfVertsTouched;

int our_count = 0;
int our_this_frames = 0;
int our_last_polygons = 0;
time_t our_last_time = 0;
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Our Calculate frame rate...
  Input   :
  Output    : BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
Our_CalculateFrameRate(void)
{
  double t;
  int p, f , i;
  D3DSTATS stats;
  time_t our_this_time;
  char buf[256];
    /*
     * Calculate the frame rate and get other stats.
     */

    our_count++;
    our_this_frames++;
    if (our_count == 25)
  {
        our_count = 0;
        our_this_time = GetTickCount();
        t = (our_this_time - our_last_time) / (double)CLOCKS_PER_SEC;
        our_last_time = our_this_time;

        memset(&stats, 0, sizeof(D3DSTATS));
        stats.dwSize = sizeof(D3DSTATS);
        d3dapp->lpD3DDevice->lpVtbl->GetStats( d3dapp->lpD3DDevice, &stats);
        p = stats.dwTrianglesDrawn - our_last_polygons;
    Our_TrianglesDrawn = p;
        our_last_polygons = stats.dwTrianglesDrawn;


        f = our_this_frames;
        our_this_frames = 0;

#if 1
        FPS = (float)(f / t);
#else
        FPS = 60.0F / framelag;
#endif
        TPS = (long)(p / t);
  }

    
  if( ( MyGameStatus == STATUS_PlayingDemo ) || ( MyGameStatus == STATUS_AttractMode ) )
  {
    QueryPerformanceCounter((LARGE_INTEGER *) &DemoEndedTime);
    TimeDiff = DemoEndedTime - DemoStartedTime;
    DemoTotalTime = ( (float) TimeDiff / (float) Freq );
    DemoAvgFps = DemoGameLoops / DemoTotalTime;
  }
  
  if( myglobs.bShowFrameRate )
  {
//    sprintf(&buf[0], "%4.2f %4d %4d %4d %4d", FPS , BigPacketSize , MaxBigPacketSize ,RecPacketSize , MaxRecPacketSize );
      sprintf(&buf[0], "%4.2f", FPS );
    CenterPrint4x5Text( (char *) &buf[0] , FontHeight+viewport.dwY , 2 );
  }

  if( myglobs.bShowInfo )
  {
#if 0
    sprintf(&buf[0], "%d Gvis %d tri %d PPBP",
                        (int) NumGroupsVisible,
              (int) Our_TrianglesDrawn / 25,
              BigPacketsSent
              );
    

    CenterPrint4x5Text( (char *) &buf[0] , (FontHeight * 2) + (FontHeight>>1) + viewport.dwY , 2 );
#endif
    
#if 0
    sprintf(&buf[0], "%d Mem %d ExecMem %d SoftwareSfx",
              (int) MemUsed ,
              (int) ExecMemUsed,
              (int) SBufferMemUsedSW
              );
    

    CenterPrint4x5Text( (char *) &buf[0] , (FontHeight * 6) + (FontHeight>>1) + viewport.dwY , 2 );
#endif


#ifdef DEBUG_POLY_DETAILS
    sprintf(&buf[0], "FmPolys %d, Polys %d, ScrPolys %d",
                        (int) TotalFmPolysInUse,
                        (int) TotalPolysInUse,
                        (int) TotalScrPolysInUse
              );
    

    CenterPrint4x5Text( (char *) &buf[0] , (FontHeight * 6) + (FontHeight>>1) + viewport.dwY , 2 );
#endif

#if 1
    sprintf( &buf[0] , "In %s framelag%5.2f Clear=%c%c",
         (GroupImIn == (uint16) -1) ? "(outside)" : Mloadheader.Group[GroupImIn].name,
         framelag,
         ClearScrOverride ? 'S' : ' ',
         ClearZOverride ? 'Z' : ' '
         );
#ifdef INSIDE_BSP
         CenterPrint4x5Text( (char *) &buf[0] , (FontHeight * 4) + (FontHeight>>1) + viewport.dwY , (!Inside) ? 1 : 2 );
#else
         CenterPrint4x5Text( (char *) &buf[0] , (FontHeight * 4) + (FontHeight>>1) + viewport.dwY , (outside_group) ? 1 : 2 );
#endif
#else
    sprintf(&buf[0], "%d before %d after",
              Tloadheader.VidMemBefore ,Tloadheader.VidMemAfter , 
              NumOfVertsTouched
              );
    CenterPrint4x5Text( (char *) &buf[0] , (FontHeight*6) + (FontHeight) + viewport.dwY , 2 );

#endif
  

    /*
    sprintf( &buf[0], "NeedFlagAtHome %d", NeedFlagAtHome );
    Print4x5Text( (char *) &buf[0], 16, 50 + ( ( MAXPRIMARYWEAPONS + MAXSECONDARYWEAPONS + 9 + 0 ) * FontHeight ) , 2 );
      
    sprintf( &buf[0], "OwnFlagTeleportsHome %d", OwnFlagTeleportsHome );
    Print4x5Text( (char *) &buf[0], 16, 50 + ( ( MAXPRIMARYWEAPONS + MAXSECONDARYWEAPONS + 9 + 1 ) * FontHeight ) , 2 );

    sprintf( &buf[0], "CanCarryOwnFlag %d", CanCarryOwnFlag );
    Print4x5Text( (char *) &buf[0], 16, 50 + ( ( MAXPRIMARYWEAPONS + MAXSECONDARYWEAPONS + 9 + 2 ) * FontHeight ) , 2 );
    */

#if 0
    if( IsServerGame && IsServer )
    {
      CorrectForExtraOrMissingPickups();

      for( i = 1; i < MAXPRIMARYWEAPONS; i++ )
      {
        sprintf( &buf[0], "L %hd - P %hd - G %hd %s", PrimaryInLevel[ i ], PrimaryInPlayers[ i ], PrimaryToGenerate[ i ], PrimaryNames[ i ] );
        Print4x5Text( (char *) &buf[0], 16, ( ( i + 5 ) * FontHeight ) , 2 );
      }

      for( i = 1; i < MAXSECONDARYWEAPONS; i++ )
      {
        if( ( i >= PURGEMINE ) && ( i <= SPIDERMINE ) )
        {
          sprintf( &buf[0], "L %hd - P %hd - G %hd - D %hd %s", SecondaryInLevel[ i ], SecondaryInPlayers[ i ], SecondaryToGenerate[ i ], MinesInLevel[ i ], SecondaryNames[ i ] );
        }
        else
        {
          sprintf( &buf[0], "L %hd - P %hd - G %hd %s", SecondaryInLevel[ i ], SecondaryInPlayers[ i ], SecondaryToGenerate[ i ], SecondaryNames[ i ] );
        }

        Print4x5Text( (char *) &buf[0], 16, ( ( i + 6 + MAXPRIMARYWEAPONS ) * FontHeight ) , 2 );
      }

      sprintf( &buf[0], "L %hd - P %hd - G %hd Orbitals", OrbsInLevel, OrbsInPlayers, OrbsToGenerate );
      Print4x5Text( (char *) &buf[0], 16, ( ( MAXPRIMARYWEAPONS + MAXSECONDARYWEAPONS + 7 ) * FontHeight ) , 2 );

      if ( CTF )
      {
        for ( i = 0; i < MAX_TEAMS; i++ )
        {
          sprintf( &buf[0], "L %d - P %d Flag %s", TeamFlagsInLevel[ i ], TeamFlagsInShips[ i ], TeamName[ i ] );
          Print4x5Text( (char *) &buf[0], 16, ( ( MAXPRIMARYWEAPONS + MAXSECONDARYWEAPONS + 9 + i ) * FontHeight ) , 2 );
        }
      }
    }
#endif

    if( DS )
    {
      DisplayStatusMessages();
    }else if( ShowWeaponKills )
    {
      for( i = 0 ; i < MAXPRIMARYWEAPONS+1 ; i++ )
      {
        Printuint16( PrimaryStats[i] , (d3dappi.szClient.cx>>1)-(5*FontWidth), (viewport.dwY + (viewport.dwHeight>>2))+( i * ( FontHeight+(FontHeight>>1) ) ), 2 );
      }

      for( i = 0 ; i < TOTALSECONDARYWEAPONS ; i++ )
      {
        Printuint16( SecondaryStats[i] , (d3dappi.szClient.cx>>1)+(1*FontWidth), (viewport.dwY + (viewport.dwHeight>>2))+( i * ( FontHeight+(FontHeight>>1) ) ), 2 );
      }
        
    }else{
      sprintf( &buf[0], "CurRec %5d CurSent %5d MaxRec %5d MaxSent %5d", CurrentBytesPerSecRec , CurrentBytesPerSecSent , MaxCurrentBytesPerSecRec ,MaxCurrentBytesPerSecSent );
      CenterPrint4x5Text( (char *) &buf[0] , (FontHeight * 8) + (FontHeight>>1) + viewport.dwY , 2 );
      sprintf(&buf[0], "Rec %5d Sent %5d MaxRec %5d MaxSent %5d", RecPacketSize , BigPacketSize , MaxRecPacketSize , MaxBigPacketSize );
      CenterPrint4x5Text( (char *) &buf[0] , (FontHeight * 9) + (FontHeight>>1) + viewport.dwY , 2 );
    }



#ifdef DEBUG_SFX
    sprintf( buf, "sbuffer mem used hw %d ( max %d )", SBufferMemUsedHW, MaxSBufferMemUsedHW );
    CenterPrint4x5Text( buf , (FontHeight*8) + (FontHeight) + viewport.dwY , 2 );
    sprintf( buf, "sbuffer mem used sw %d ( max %d )", SBufferMemUsedSW, MaxSBufferMemUsedSW );
    CenterPrint4x5Text( buf , (FontHeight*10) + (FontHeight) + viewport.dwY , 2 );
    sprintf( buf, "currentbikecomp %d", CurrentBikeCompSpeech );
    CenterPrint4x5Text( buf , (FontHeight*12) + (FontHeight) + viewport.dwY , 2 );
#endif


  }
  return TRUE;
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Disp3d Panel..
  Input   :
  Output    : BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  Disp3dPanel( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView )
{
  VECTOR  Trans;
  MATRIX  Matrix;
  MATRIX  rotMatrix;
  MATRIX  finalMatrix;
  VECTOR  Pos;
  VECTOR  Temp;
  VECTOR  Scale;
    int clearflags;
    D3DRECT dummy;
  D3DVIEWPORT newviewport;
  float screen_width, screen_height;


//  return TRUE;

    newviewport.dwSize = sizeof(D3DVIEWPORT);
    newviewport.dwX = 0;
  newviewport.dwY = 0;
    newviewport.dwWidth = d3dapp->szClient.cx;
    newviewport.dwHeight = d3dapp->szClient.cy;
    newviewport.dvScaleX = newviewport.dwWidth / (float)2.0;
    newviewport.dvScaleY = newviewport.dwHeight / (float)2.0;
    newviewport.dvMaxX = (float)D3DDivide(D3DVAL(newviewport.dwWidth),
                                       D3DVAL(2 * newviewport.dvScaleX));
    newviewport.dvMaxY = (float)D3DDivide(D3DVAL(newviewport.dwHeight),
                                       D3DVAL(2 * newviewport.dvScaleY));
  
    if( d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &newviewport) != D3D_OK )
    return FALSE;

  if ( d3dapp->bFullscreen )
  {
    screen_width = (float) d3dapp->ThisMode.w;
    screen_height = (float) d3dapp->ThisMode.h;
  }
  else
  {
    screen_width = (float) d3dapp->WindowsDisplay.w;
    screen_height = (float) d3dapp->WindowsDisplay.h;
  }
  pixel_aspect_ratio = screen_aspect_ratio * screen_height / screen_width;
  viewplane_distance = (float) ( newviewport.dwWidth / ( 2 * tan( DEG2RAD( normal_fov ) * 0.5 ) ) );
  panelproj._11 = 2 * viewplane_distance / newviewport.dwWidth;
  panelproj._22 = 2 * viewplane_distance / ( newviewport.dwHeight / pixel_aspect_ratio );

  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &panelproj) != D3D_OK)
  {
      return FALSE;
  }

  
  
  clearflags = 0;
  if (d3dapprs.bZBufferOn != 0 )      // If a ZBuffer is enabled then always clear it..
  {
    clearflags |= D3DCLEAR_ZBUFFER;
    }
  if( clearflags != 0 )
  {
    dummy.x1 = newviewport.dwX;
    dummy.x2 = newviewport.dwX+newviewport.dwWidth;

    dummy.y1 = newviewport.dwY + ( newviewport.dwHeight >> 1 );
    dummy.y2 = dummy.y1+( newviewport.dwHeight >> 1);

    if( d3dappi.lpD3DViewport->lpVtbl->Clear(d3dappi.lpD3DViewport,
                                             1, &dummy,
                                             clearflags) != D3D_OK )
                         return FALSE;
  }

  Trans.x = 0.0F;
    Trans.y = -180.0F;
  Trans.z = 280.0F;

  Trans.x -= ( ( Ships[WhoIAm].Object.Speed.x * -4.0F ) + ( Ships[WhoIAm].Object.Angle.y * 8.0F ) );
  Trans.y -= ( ( Ships[WhoIAm].Object.Speed.y * -2.0F ) + ( Ships[WhoIAm].Object.Angle.x * 4.0F ) );
  Trans.z += ( Ships[WhoIAm].Object.Speed.z * 4.0F );
  

  Pos.x = -( ( Ships[WhoIAm].Object.Speed.x * -4.0F ) + ( Ships[WhoIAm].Object.Angle.y * 8.0F ) );
  Pos.y = -( ( Ships[WhoIAm].Object.Speed.y * -2.0F ) + ( Ships[WhoIAm].Object.Angle.x * 4.0F ) );
  Pos.z = +( Ships[WhoIAm].Object.Speed.z * 4.0F );

  ApplyMatrix( &MainCamera.Mat, &Pos, &Temp );

  Temp.x += MainCamera.Pos.x;
  Temp.y += MainCamera.Pos.y;
  Temp.z += MainCamera.Pos.z;

  Matrix = MainCamera.Mat;
  Matrix._41 = Temp.x;
  Matrix._42 = Temp.y;
  Matrix._43 = Temp.z;

  Scale.x = 0.5F;
  Scale.y = 0.5F;
  Scale.z = 0.5F;
  ScaleMatrix( &Matrix, &Scale );
  

  BuildRotMatrix( Ships[WhoIAm].Object.Angle.x, Ships[WhoIAm].Object.Angle.y, Ships[WhoIAm].Object.Angle.y+Ships[WhoIAm].Object.Angle.z, &rotMatrix);


  MatrixMultiply( &Matrix, &rotMatrix, &finalMatrix );

//  XLightMxloadHeader( &ModelHeaders[MODEL_Panel] , &Temp , SHIP_RADIUS*4.0F , &finalMatrix );
  XLightMxloadHeader( &ModelHeaders[MODEL_Eyeball] , &Temp , SHIP_RADIUS*4.0F , &finalMatrix );
  
  
  
//  view = identity;
  view._41 = Trans.x;
  view._42 = Trans.y;
  view._43 = Trans.z;
  view._11 = rotMatrix._11; 
  view._12 = rotMatrix._12;
  view._13 = rotMatrix._13;
  view._14 = rotMatrix._14;
             
  view._21 = rotMatrix._21;
  view._22 = rotMatrix._22;
  view._23 = rotMatrix._23;
  view._24 = rotMatrix._24;
             
  view._31 = rotMatrix._31;
  view._32 = rotMatrix._32;
  view._33 = rotMatrix._33;
  view._34 = rotMatrix._34;
                                        
  view._44 = rotMatrix._44;

  if (lpDev->lpVtbl->SetMatrix(lpDev, hView, &view) != D3D_OK)
    return FALSE;

//  if (ExecuteMxloadHeader( &ModelHeaders[MODEL_Panel], (uint16) -1 ) != TRUE )
  if (ExecuteMxloadHeader( &ModelHeaders[MODEL_Eyeball], (uint16) -1 ) != TRUE )
    return FALSE;

    if( d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport) != D3D_OK )
    return FALSE;

  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &proj) != D3D_OK)
  {
      return FALSE;
  }


  return TRUE;
}





#if 0
        if( ModeCase != -1 )
        {
          //  Blt Panel
          GeneralBltFast( 0 , 0 , PanelVisibleX[ModeCase] , PanelVisibleY[ModeCase]  , 0  , d3dappi.szClient.cy-PanelVisibleY[ModeCase] ,
                  lpDDSThree , (char*) &PanelNames[ModeCase] , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
          //  Primary one
          GeneralBltFast( PrimaryX[Ships[WhoIAm].Primary] , PrimaryY[Ships[WhoIAm].Primary] , 48 , 32  , PrimaryWeaponShowX , PrimaryWeaponShowY ,
                  lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);
          
          HealthCount+=  framelag;
          //  Health one
          src.top = ( ( (int) (HealthCount * 0.25F) ) & 15 ) * 8;
          src.left = 0;
          src.right = 32;
          src.bottom = src.top+8;
      
          memset(&fx, 0, sizeof(DDBLTFX));
          fx.dwSize = sizeof(DDBLTFX);
          dest.top = ( d3dappi.szClient.cy-10 )- ( Ships[WhoIAm].Shield >> 3 );
          dest.bottom = d3dappi.szClient.cy-10;
          dest.left = (d3dappi.szClient.cx >> 1) - 16 - 4;
          dest.right = (d3dappi.szClient.cx >> 1) - 4;
          
          while( 1 )
          {
            ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, &dest, lpDDSOne, &src, DDBLT_KEYSRC | DDBLT_WAIT , &fx );
            if( ddrval == DD_OK )
              break;
            if( ddrval == DDERR_SURFACELOST )
            {
              d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
              d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
              DDReLoadBitmap( lpDDSOne, "data\\pictures\\panel.bmp" );
              break;
            }
            if( ddrval != DDERR_WASSTILLDRAWING )
              break;
          }
      
          //  Hull one
          src.top = ( ( (int) ( (HealthCount * 0.25F) + 8 ) ^ 15 ) & 15 ) * 8;
          src.left = 0;
          src.right = 32;
          src.bottom = src.top+8;
      
          memset(&fx, 0, sizeof(DDBLTFX));
          fx.dwSize = sizeof(DDBLTFX);
          dest.top = ( d3dappi.szClient.cy-10 )- ( Ships[WhoIAm].Hull >> 3 );
          dest.bottom = d3dappi.szClient.cy-10;
          dest.left = (d3dappi.szClient.cx >> 1) + 4;
          dest.right = (d3dappi.szClient.cx >> 1) + 4 + 16;
          
          while( 1 )
          {
            ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, &dest, lpDDSOne, &src, DDBLT_KEYSRC | DDBLT_WAIT , &fx );
            if( ddrval == DD_OK )
              break;
            if( ddrval == DDERR_SURFACELOST )
            {
              d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
              d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
              DDReLoadBitmap( lpDDSOne, "data\\pictures\\panel.bmp" );
              break;
            }
            if( ddrval != DDERR_WASSTILLDRAWING )
              break;
          }
          //  Powerpods
          src.top = 0;
          src.left = 32+64;
          src.right = src.left+PowerSizes[Ships[WhoIAm].PowerLevel];
          src.bottom = src.top+8;
          y =  d3dappi.szClient.cy-74;
          x =  ( d3dappi.szClient.cx >> 1 ) - 28;
      
          while( 1 )
          {
            ddrval = d3dapp->lpBackBuffer->lpVtbl->BltFast( d3dapp->lpBackBuffer, x, y, lpDDSOne, &src, DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT );
            if( ddrval == DD_OK )
              break;
            if( ddrval == DDERR_SURFACELOST )
            {
              d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
              d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
              DDReLoadBitmap( lpDDSOne, "data\\pictures\\panel.bmp" );
              break;
            }
            if( ddrval != DDERR_WASSTILLDRAWING )
              break;
          }
        }
        
        //  Blt Score
        Printuint16( Ships[WhoIAm].Shield , PanelShieldPosX, PanelShieldPosY , 2 );
        Printuint16( Ships[WhoIAm].Hull , PanelHullPosX, PanelHullPosY , 2 );
        // Blt Primary
        if( Ships[WhoIAm].Primary == PYROLITE_RIFLE )
        {
          energy =  (int) PyroliteAmmo;
        }else{
          if( Ships[WhoIAm].Primary == SUSS_GUN )
          {
            energy = (int) SussGunAmmo;
          }else{
            energy = (int) GeneralAmmo;
          }
        }
        Printuint16( (uint16) energy , ( d3dappi.szClient.cx >> 1) - 64 , d3dappi.szClient.cy-( 48 - FontHeight - (FontHeight>>1) ), 2  );
      
        DoFontBlt( FontWidth*16 , FontHeight*Ships[WhoIAm].Primary , FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] , FontHeight , PrimaryWeaponTextX , PrimaryWeaponTextY   );
        // Blt Secondary
        DoFontBlt( FontWidth*16 , (FontHeight*6)+FontHeight*Ships[WhoIAm].Secondary , FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] , FontHeight ,SecondaryWeaponTextX - ( FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] ) , SecondaryWeaponTextY );
        Printuint16( (uint16) GetCurSecAmmo() , SecondaryWeaponNumX - ( 4*FontWidth)  , SecondaryWeaponNumY + (FontHeight + (FontHeight>>1) ) , 2 );
        // blt shld
        DoFontBlt( FontWidth*28 , 0 , FontWidth*4 , FontHeight , PanelShieldTextPosX , PanelShieldTextPosY );
        // blt hull
        DoFontBlt( FontWidth*28 , FontHeight , FontWidth*4 , FontHeight , PanelHullTextPosX , PanelHullTextPosY );
#endif
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Init View port without calling initview..
  Input   : Nothing
  Output    : Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
InitViewport( float scale )
{
    HRESULT rval;
  int left, top;
  int width, height;
  int maxwidth, maxheight;

    /*
     * Setup the viewport for specified viewing area
     */
    memset(&viewport, 0, sizeof(D3DVIEWPORT));
    viewport.dwSize = sizeof(D3DVIEWPORT);
    rval = d3dapp->lpD3DViewport->lpVtbl->GetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
        Msg( "GetViewport failed.\n%s", D3DAppErrorToString(rval) );
        return FALSE;
    }
  maxwidth = d3dapp->szClient.cx;

  DrawSimplePanel = FALSE;
  if( scale < 1.01F )
    DrawSimplePanel = TRUE;

  if( DrawSimplePanel )
  {
    maxheight = d3dapp->szClient.cy - PanelVisibleY[ModeCase];
  }else{
    maxheight = d3dapp->szClient.cy;
  }
  if ( scale < 1.01F )
  {
    width = (int) floor( maxwidth * scale );
    if ( width < MIN_VIEWPORT_WIDTH )
    {
      width = MIN_VIEWPORT_WIDTH;
      scale = (float) width / maxwidth;
    }
    height = (int) floor( maxheight * scale );
    DrawPanel = FALSE;
    left = ( ( maxwidth - width ) >> 1 ) & ~1;
    top = ( ( maxheight - height ) >> 1 ) & ~1;
  }
  else
  {
    width = maxwidth;
    height = maxheight;
    if ( scale > 1.11F )
    {
      DrawPanel = TRUE;
      scale = 1.2F;
    }
    else
    {
      DrawPanel = FALSE;
    }
    left = 0;
    top = 0;
  }
  CurrentViewportScale = scale;

    viewport.dwX = left;
  viewport.dwY = top;
    viewport.dwWidth = width;
    viewport.dwHeight = height;
    viewport.dvScaleX = viewport.dwWidth / (float)2.0;
    viewport.dvScaleY = viewport.dwHeight / (float)2.0;
    viewport.dvMaxX = (float)D3DDivide(D3DVAL(viewport.dwWidth),
                                       D3DVAL(2 * viewport.dvScaleX));
    viewport.dvMaxY = (float)D3DDivide(D3DVAL(viewport.dwHeight),
                                       D3DVAL(2 * viewport.dvScaleY));
    rval = d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport);
    if (rval != D3D_OK) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "InitViewport", &viewport, rval );
#else
        Msg("SetViewport failed.\n%s", D3DAppErrorToString(rval));
#endif
        return FALSE;
    }
  SetFOV( hfov );

  // clear viewport
  D3DAppIClearBuffers();

  return TRUE;
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Plot Simple Panel...
  Input   : Nothing
  Output    : Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void  PlotSimplePanel( void )
{
  int energy;

  // Blt Primary
  if( ( Ships[WhoIAm].Primary != PrimaryChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    PrimaryChanged = Ships[WhoIAm].Primary;
    GeneralBlt( ( Ships[WhoIAm].Primary & 3 ) * 64 ,
            ( ( Ships[WhoIAm].Primary >> 2 )& 3 ) * 64 ,
          64 , 64 , PrimaryWeaponShowX , PrimaryWeaponShowY ,
          WeaponSizeX , WeaponSizeY ,
          lpDDSFour , "data\\pictures\\pcontent.bmp" , 0 , lpDDSThree);

    GeneralBltFast( FontWidth*16 , FontHeight*Ships[WhoIAm].Primary  , FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] , FontHeight ,
              PrimaryWeaponTextX , PrimaryWeaponTextY , lpDDSTwo ,
            /*&FontNames[ModeCase][0]*/DynamicFontNames ,
             DDBLTFAST_WAIT, lpDDSThree);
  }
  
  

  // Blt Secondary
  if( ( Ships[WhoIAm].Secondary != SecondaryChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    SecondaryChanged = Ships[WhoIAm].Secondary;
    GeneralBlt( ( (Ships[WhoIAm].Secondary+6) & 3 ) * 64 ,
            ( ( (Ships[WhoIAm].Secondary+6) >> 2 )& 3 ) * 64 ,
          64 , 64 , SecondaryWeaponShowX , SecondaryWeaponShowY ,
          WeaponSizeX , WeaponSizeY ,
          lpDDSFour , "data\\pictures\\pcontent.bmp" , 0 , lpDDSThree);
    GeneralBltFast( FontWidth*16 , (FontHeight*6)+FontHeight*Ships[WhoIAm].Secondary  , FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] , FontHeight ,
              SecondaryWeaponTextX , SecondaryWeaponTextY , lpDDSTwo ,
            /*&FontNames[ModeCase][0]*/DynamicFontNames ,
            DDBLTFAST_WAIT, lpDDSThree);
  }


  // blt shld
  if( ReMakeSimplePanel == TRUE )
  {
    GeneralBltFast( FontWidth*28 , 0 , FontWidth*4 , FontHeight  , PanelShieldTextPosX , PanelShieldTextPosY ,
            lpDDSTwo ,
            /*&FontNames[ModeCase][0]*/DynamicFontNames ,
            DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT, lpDDSThree);
  }
  if( ( Ships[WhoIAm].Object.Shield != ShieldChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    ShieldChanged = (uint16) Ships[WhoIAm].Object.Shield;

    Printuint16AnySurface( (uint16) Ships[WhoIAm].Object.Shield , PanelShieldPosX, PanelShieldPosY , 2 , DDBLTFAST_WAIT, lpDDSThree );
    
    if( ShieldHit == 0 )
    {
      GeneralBltFast( 64 - (int)(Ships[WhoIAm].Object.Shield *0.5F) , 24 , 56 , 3 , PanelShieldBarPosX , PanelShieldBarPosY ,
              lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_WAIT, lpDDSThree);
    }else{
      ShieldHit -=1;
      GeneralBltFast( 64 - (int)(Ships[WhoIAm].Object.Shield *0.5F) , 24 + ((ShieldHit>>2)*8) , 56 , 3  , PanelShieldBarPosX , PanelShieldBarPosY ,
              lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_WAIT , lpDDSThree);
      ShieldChanged = (uint16)-1;
    }
  }
  // blt Hull
  if( ReMakeSimplePanel == TRUE )
  {
    GeneralBltFast( FontWidth*28 , FontHeight , FontWidth*4 , FontHeight  , PanelHullTextPosX , PanelHullTextPosY ,
            lpDDSTwo , 
            /*&FontNames[ModeCase][0]*/DynamicFontNames ,
            DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT, lpDDSThree);
  }

  if( ( Ships[WhoIAm].Object.Hull != HullChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    HullChanged = (uint16)Ships[WhoIAm].Object.Hull;
    Printuint16AnySurface( (uint16) Ships[WhoIAm].Object.Hull , PanelHullPosX, PanelHullPosY , 2 , DDBLTFAST_WAIT, lpDDSThree);
    if( HullHit == 0 )
    {
      GeneralBltFast( 64 - (int)(Ships[WhoIAm].Object.Hull *0.5F) , 24 , 56 , 3  , PanelHullBarPosX , PanelHullBarPosY ,
              lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_WAIT, lpDDSThree);
    }else{
      HullHit -=1;
      GeneralBltFast( 64 - (int)(Ships[WhoIAm].Object.Hull *0.5F) , 24 + ((HullHit>>2)*8) , 56 , 3  , PanelHullBarPosX , PanelHullBarPosY ,
              lpDDSOne ,  (char*) "data\\pictures\\panel.bmp" , DDBLTFAST_WAIT , lpDDSThree);
      HullChanged = (uint16)-1;
    }
  }


  // Blt Power Pods
  if( ( PowerChanged != Ships[WhoIAm].Object.PowerLevel ) || ( ReMakeSimplePanel == TRUE ) )
  {
    GeneralBltFast( FontWidth*(32-7) , FontHeight*(7+Ships[WhoIAm].Object.PowerLevel) , FontWidth*7 , FontHeight  , PanelPowerPosX , PanelPowerPosY ,
            lpDDSTwo , 
            /*&FontNames[ModeCase][0]*/DynamicFontNames ,
            DDBLTFAST_WAIT, lpDDSThree);

    PowerChanged = Ships[WhoIAm].Object.PowerLevel;
  }
  
  //  Blt Primary ammo
  if( Ships[WhoIAm].Primary == PYROLITE_RIFLE )
  {
    energy =  (int) PyroliteAmmo;
  }else{
    if( Ships[WhoIAm].Primary == SUSS_GUN )
    {
      energy = (int) SussGunAmmo;
    }else{
      energy = (int) GeneralAmmo;
    }
  }
  if( ( energy != PrimaryNumChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    PrimaryNumChanged = energy;
    Printuint16AnySurface( (uint16) energy , PrimaryWeaponNumX , PrimaryWeaponNumY , 2 , DDBLTFAST_WAIT, lpDDSThree);
  }

  // Blt Secondary ammo
  if( ( (uint16) GetCurSecAmmo() != SecondaryNumChanged ) || ( ReMakeSimplePanel == TRUE ) )
  {
    SecondaryNumChanged = (uint16) GetCurSecAmmo();
      Printuint16AnySurface( (uint16) GetCurSecAmmo() , SecondaryWeaponNumX , SecondaryWeaponNumY , 2 , DDBLTFAST_WAIT, lpDDSThree );
  }
  
  //  Blt Panel
  GeneralBltFast( 0 , 0 , PanelVisibleX[ModeCase] , PanelVisibleY[ModeCase]  , 0  , d3dappi.szClient.cy-PanelVisibleY[ModeCase] ,
          lpDDSThree , (char*) &PanelNames[ModeCase] , DDBLTFAST_SRCCOLORKEY  | DDBLTFAST_WAIT , d3dapp->lpBackBuffer);


  

  energy = (int) GetBestMine();
  // Blt Mine..
  if( energy != 65535 )
  {
    DoFontBlt( FontWidth*16 , (FontHeight*6)+FontHeight*energy , FontWidth*SecondaryLengths[energy] , FontHeight , d3dappi.szClient.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[energy] ) , FontHeight  );
    Printuint16( (uint16) SecondaryAmmo[energy] , d3dappi.szClient.cx - ( FontWidth*5) , FontHeight , 2 );
  }

  ReMakeSimplePanel = FALSE;
}
  



void  FlipToGDISurface()
{
  d3dappi.lpDD->lpVtbl->FlipToGDISurface(d3dappi.lpDD);
}




/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Make Specific Cmd Buffers...
  Input   :   nothing..
  Output    :   BOOL TRUE/FLASE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL
InitSpecialExecBufs( void )
{
    LPVOID lpBufStart, lpInsStart, lpPointer;
    size_t size;

  LPDIRECTDRAW lpDD = d3dapp->lpDD;
  LPDIRECT3D lpD3D = d3dapp->lpD3D;
  LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
    LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;


  size = 0;
  size += sizeof(D3DSTATE) * 9;
    size += sizeof(D3DINSTRUCTION) * 2;
  
  if (MakeExecuteBuffer( &debDesc, lpDev , &lpD3DNormCmdBuf , size ) != TRUE ) return FALSE;
  
  /*
   * lock it so it can be filled
   */
  if (lpD3DNormCmdBuf->lpVtbl->Lock(lpD3DNormCmdBuf, &debDesc) != D3D_OK) return FALSE;
  
  lpBufStart = debDesc.lpData;
  memset(lpBufStart, 0, size);
  lpPointer = lpBufStart;
  
  lpInsStart = lpPointer;


  //  this is for a normal non translucent buffers....
  if ( PowerVR )
  {
  OP_STATE_RENDER( 9, lpPointer);
  }
  else
  {
  OP_STATE_RENDER( 8, lpPointer);
  }
    STATE_DATA( D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE, lpPointer );
    STATE_DATA( D3DRENDERSTATE_BLENDENABLE, FALSE, lpPointer );
    STATE_DATA( D3DRENDERSTATE_ZWRITEENABLE, TRUE, lpPointer );
    STATE_DATA( D3DRENDERSTATE_STIPPLEDALPHA , FALSE, lpPointer );
    STATE_DATA( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE, lpPointer );
    STATE_DATA( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO, lpPointer );
      STATE_DATA( D3DRENDERSTATE_WRAPU, FALSE, lpPointer );
      STATE_DATA( D3DRENDERSTATE_WRAPV, FALSE, lpPointer );
    if ( PowerVR )
    {
      STATE_DATA( D3DRENDERSTATE_COLORKEYENABLE, TRUE, lpPointer );
    }
    OP_EXIT(lpPointer);
  
  /*
   * Setup the execute data describing the buffer
   */
  lpD3DNormCmdBuf->lpVtbl->Unlock(lpD3DNormCmdBuf);
  memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
  d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
  d3dExData.dwInstructionOffset = (ULONG) 0;
  d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
  lpD3DNormCmdBuf->lpVtbl->SetExecuteData(lpD3DNormCmdBuf, &d3dExData);


  
  // Start of Trans exec Buffer...
  
  size = 0;

  if(d3dappi.ThisDriver.bIsHardware)
  {
    if( !UsedStippledAlpha )
    {
      size += sizeof(D3DSTATE) * 6;
    }else{
      size += sizeof(D3DSTATE) * 1;
    }
  }else{
    size += sizeof(D3DSTATE) * 1;
  }
    size += sizeof(D3DINSTRUCTION) * 2;

  
  if (MakeExecuteBuffer( &debDesc, lpDev , &lpD3DTransCmdBuf , size ) != TRUE ) return FALSE;
  
  
  /*
   * lock it so it can be filled
   */
  if (lpD3DTransCmdBuf->lpVtbl->Lock(lpD3DTransCmdBuf, &debDesc) != D3D_OK) return FALSE;
  
  lpBufStart = debDesc.lpData;
  memset(lpBufStart, 0, size);
  lpPointer = lpBufStart;
  
  lpInsStart = lpPointer;

  //  this is for Translucent buffers....
  if(d3dappi.ThisDriver.bIsHardware)
  {
    if( UsedStippledAlpha != TRUE )
    {
      if ( PowerVR )
      {
      OP_STATE_RENDER( 6, lpPointer);
      }
      else
      {
      OP_STATE_RENDER( 5, lpPointer);
      }

#if ACTUAL_TRANS
      STATE_DATA( D3DRENDERSTATE_ZWRITEENABLE, TRUE, lpPointer );
#else
      if( PowerVR )
      {
        STATE_DATA( D3DRENDERSTATE_ZWRITEENABLE, TRUE, lpPointer );
      }else{
        STATE_DATA( D3DRENDERSTATE_ZWRITEENABLE, FALSE, lpPointer );
      }
#endif

      STATE_DATA( D3DRENDERSTATE_BLENDENABLE, TRUE, lpPointer );
      STATE_DATA( D3DRENDERSTATE_SRCBLEND, CurrentSrcBlend, lpPointer );
      STATE_DATA( D3DRENDERSTATE_DESTBLEND, CurrentDestBlend, lpPointer );
      STATE_DATA( D3DRENDERSTATE_TEXTUREMAPBLEND, CurrentTextureBlend, lpPointer );
      if ( PowerVR )
      {
        STATE_DATA( D3DRENDERSTATE_COLORKEYENABLE, TRUE, lpPointer );
      }
    }
    else
    {
      OP_STATE_RENDER( 1, lpPointer);
//      STATE_DATA( D3DRENDERSTATE_BLENDENABLE, FALSE, lpPointer );
//      STATE_DATA( D3DRENDERSTATE_ZWRITEENABLE, FALSE, lpPointer );
      STATE_DATA( D3DRENDERSTATE_STIPPLEDALPHA , TRUE, lpPointer );
//      STATE_DATA( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR, lpPointer );
//      STATE_DATA( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR, lpPointer );
    }
  }
  else
  {
    OP_STATE_RENDER( 1, lpPointer);
    STATE_DATA( D3DRENDERSTATE_BLENDENABLE, TRUE, lpPointer );
  }

    OP_EXIT(lpPointer);
  
  /*
   * Setup the execute data describing the buffer
   */
  lpD3DTransCmdBuf->lpVtbl->Unlock(lpD3DTransCmdBuf);
  memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
  d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
  d3dExData.dwInstructionOffset = (ULONG) 0;
  d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
  lpD3DTransCmdBuf->lpVtbl->SetExecuteData(lpD3DTransCmdBuf, &d3dExData);

  // Start of SpcFxTrans exec Buffer...
  
  size = 0;

  if(d3dappi.ThisDriver.bIsHardware)
  {
    if( !UsedStippledAlpha )
    {
      size += sizeof(D3DSTATE) * 5;
    }else{
      size += sizeof(D3DSTATE) * 1;
    }
  }else{
    size += sizeof(D3DSTATE) * 1;
  }

    size += sizeof(D3DINSTRUCTION) * 2;
  
  if (MakeExecuteBuffer( &debDesc, lpDev , &lpD3DSpcFxTransCmdBuf , size ) != TRUE ) return FALSE;
  
  /*
   * lock it so it can be filled
   */
  if (lpD3DSpcFxTransCmdBuf->lpVtbl->Lock(lpD3DSpcFxTransCmdBuf, &debDesc) != D3D_OK) return FALSE;
  
  lpBufStart = debDesc.lpData;
  memset(lpBufStart, 0, size);
  lpPointer = lpBufStart;
  
  lpInsStart = lpPointer;


  //  this is for SpcFxTranslucent buffers....
  if(d3dappi.ThisDriver.bIsHardware)
  {
    if( UsedStippledAlpha != TRUE )
    {
      OP_STATE_RENDER( 4, lpPointer);
      STATE_DATA( D3DRENDERSTATE_BLENDENABLE, TRUE, lpPointer );
      STATE_DATA( D3DRENDERSTATE_SRCBLEND, CurrentSrcBlend, lpPointer );
      STATE_DATA( D3DRENDERSTATE_DESTBLEND, CurrentDestBlend, lpPointer );
      STATE_DATA( D3DRENDERSTATE_TEXTUREMAPBLEND, CurrentTextureBlend, lpPointer );
    }
    else
    {
      OP_STATE_RENDER( 1, lpPointer);
      STATE_DATA( D3DRENDERSTATE_STIPPLEDALPHA , TRUE, lpPointer );
//      STATE_DATA( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR, lpPointer );
//      STATE_DATA( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR, lpPointer );
    }
  }
  else
  {
    OP_STATE_RENDER( 1, lpPointer);
    STATE_DATA( D3DRENDERSTATE_BLENDENABLE, TRUE, lpPointer );
  }

    OP_EXIT(lpPointer);
 
  /*
   * Setup the execute data describing the buffer
   */
  lpD3DSpcFxTransCmdBuf->lpVtbl->Unlock(lpD3DSpcFxTransCmdBuf);
  memset(&d3dExData, 0, sizeof(D3DEXECUTEDATA));
  d3dExData.dwSize = sizeof(D3DEXECUTEDATA);
  d3dExData.dwInstructionOffset = (ULONG) 0;
  d3dExData.dwInstructionLength = (ULONG) ((char *)lpPointer - (char*)lpInsStart);
  lpD3DSpcFxTransCmdBuf->lpVtbl->SetExecuteData(lpD3DSpcFxTransCmdBuf, &d3dExData);

  
  return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Release Specific Cmd Buffers...
  Input   :   nothing..
  Output    :   BOOL TRUE/FLASE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

void ReleaseSpecialExecBufs( void )
{
  if( lpD3DNormCmdBuf )
  {
    XRELEASE(lpD3DNormCmdBuf);
    lpD3DNormCmdBuf = NULL;
  }
  if( lpD3DTransCmdBuf )
  {
    XRELEASE(lpD3DTransCmdBuf);
    lpD3DTransCmdBuf = NULL;
  }
  if( lpD3DSpcFxTransCmdBuf )
  {
    XRELEASE(lpD3DSpcFxTransCmdBuf);
    lpD3DSpcFxTransCmdBuf = NULL;
  }
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Special Destroy game for dplay fuck up..
  Input   :   nothing..
  Output    :   nothing..
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void SpecialDestroyGame( void )
{
  IMustQuit = FALSE;
  
  if ( PlayDemo )
  {
    Panel = PreAttractModePanel;
    PlayDemo = FALSE;
  }

  switch( MyGameStatus )
  {
  case STATUS_WaitingToStartTeamGame:
  case STATUS_StartingMultiplayer:
  case STATUS_GetPlayerNum:

    DPlayDestroyPlayer(dcoID);
    dcoID = 0;
    DPlayRelease();
    MyGameStatus = STATUS_Title;
    MenuRestart( &MENU_ForceAbort );
    break;
  default:
    PreventFlips = FALSE;
    MyGameStatus = STATUS_QuitCurrentGame;
    break;
  }

}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Calculate the framelag..
  Input   :   nothing..
  Output    :   nothing..
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void CalculateFramelag( void )
{
  framelag = 0.0F;
  if( IsServer )
  {
    while( framelag < 0.1F )
    {
      QueryPerformanceCounter((LARGE_INTEGER *) &LargeTime);
      TimeDiff = LargeTime - LastTime;
      framelag = (float) ( ( TimeDiff * 71.0 ) / Freq );
      framelag2 = framelag;
    }
  }else{
    QueryPerformanceCounter((LARGE_INTEGER *) &LargeTime);
    TimeDiff = LargeTime - LastTime;
    framelag = (float) ( ( TimeDiff * 71.0 ) / Freq );
    framelag2 = framelag;
  }

  LastTime = LargeTime;

  if( avgframelag == 0.0F )
  {
    avgframelag = framelag;
  }else{
    avgframelag += framelag;
    avgframelag *= 0.5F;
    avgframelag = (float) floor(avgframelag);
  }

  if( CurrentMenu && (MyGameStatus == STATUS_SinglePlayer) )
  {
    framelag = 0.0F;
  }

}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Disp Tracker
  Input   : LPDIRECT3DDEVICE  lpDev
        : LPDIRECT3DVIEWPORT  lpView
  Output    : BOOL        TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL DispTracker( LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView )
{
  uint16    i;
    int     clearflags;
    D3DRECT   dummy;
  D3DVIEWPORT newviewport;
  float   screen_width, screen_height;
  VECTOR    TempVector;
  MATRIX    TempMatrix;
  float   TargetDistance;
  float   Red, Green, Blue, Trans;
  VECTOR    ShipDir, TargetDir;
  float   Cos;

  if ( d3dapp->bFullscreen )
  {
    screen_width = (float) d3dapp->ThisMode.w;
    screen_height = (float) d3dapp->ThisMode.h;
  }
  else
  {
    screen_width = (float) d3dapp->WindowsDisplay.w;
    screen_height = (float) d3dapp->WindowsDisplay.h;
  }
  pixel_aspect_ratio = screen_aspect_ratio * screen_height / screen_width;

    newviewport.dwSize = sizeof(D3DVIEWPORT);
    newviewport.dwX = 0;
  newviewport.dwY = 0;
    newviewport.dwWidth = ( d3dapp->szClient.cx / 3 ) & -2;
    newviewport.dwHeight = (uint32) ( (float) newviewport.dwWidth * pixel_aspect_ratio );
    newviewport.dvScaleX = newviewport.dwWidth / (float)2.0;
    newviewport.dvScaleY = newviewport.dwHeight / (float)2.0;
    newviewport.dvMaxX = (float)D3DDivide(D3DVAL(newviewport.dwWidth),
                                       D3DVAL(2 * newviewport.dvScaleX));
    newviewport.dvMaxY = (float)D3DDivide(D3DVAL(newviewport.dwHeight),
                                       D3DVAL(2 * newviewport.dvScaleY));
  
    if( d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &newviewport) != D3D_OK )
    return FALSE;

  viewplane_distance = (float) ( newviewport.dwWidth / ( 2 * tan( DEG2RAD( normal_fov ) * 0.5 ) ) );
  panelproj._11 = 2 * viewplane_distance / newviewport.dwWidth;
  panelproj._22 = 2 * viewplane_distance / ( newviewport.dwHeight / pixel_aspect_ratio );

  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &panelproj) != D3D_OK)
  {
      return FALSE;
  }

  clearflags = 0;
  if (d3dapprs.bZBufferOn != 0 )      // If a ZBuffer is enabled then always clear it..
  {
    clearflags |= D3DCLEAR_ZBUFFER;
    }
  if( clearflags != 0 )
  {
    dummy.x1 = newviewport.dwX;
    dummy.x2 = newviewport.dwX + newviewport.dwWidth;

    dummy.y1 = newviewport.dwY;
    dummy.y2 = newviewport.dwY + newviewport.dwHeight;

    if( d3dappi.lpD3DViewport->lpVtbl->Clear(d3dappi.lpD3DViewport,
                                             1, &dummy,
                                             clearflags) != D3D_OK )
                         return FALSE;
  }

  MatrixTranspose( &Ships[ WhoIAm ].Object.FinalMat, &TempMatrix );
  ShadeModel( MODEL_Tracker, &TempMatrix, ( 40.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ) );

  TempMatrix = MATRIX_Identity;
  TempMatrix._11 = 1.25F;
  TempMatrix._22 = 1.25F;
  TempMatrix._33 = 1.25F;
  MatrixMultiply( &TempMatrix, &Ships[ WhoIAm ].Object.FinalMat, &TempMatrix );

  view._11 = TempMatrix._11; 
  view._12 = TempMatrix._12;
  view._13 = TempMatrix._13;
  view._14 = TempMatrix._14;
             
  view._21 = TempMatrix._21;
  view._22 = TempMatrix._22;
  view._23 = TempMatrix._23;
  view._24 = TempMatrix._24;
             
  view._31 = TempMatrix._31;
  view._32 = TempMatrix._32;
  view._33 = TempMatrix._33;
  view._34 = TempMatrix._34;

  view._41 = 0.0F;
  view._42 = 0.0F;
  view._43 = ( 280.0F * GLOBAL_SCALE );
  view._44 = 1.0F;

  if (lpDev->lpVtbl->SetMatrix(lpDev, hView, &view) != D3D_OK)
    return FALSE;

  if (ReallyExecuteMxloadHeader( &ModelHeaders[MODEL_Tracker], (uint16) -1 ) != TRUE )
    return FALSE;

#if 0
  i = FindClosestPickup();
#else
  i = FindClosestShip();
#endif
  
  if( i != (uint16) -1 )
  {
    Red = 255.0F;
    Green = 255.0F;
    Blue = 255.0F;
    Trans = 255.0F;
#if 0
    TempVector.x = ( ( Pickups[ i ].Pos.x - Ships[ WhoIAm ].Object.Pos.x ) / 50.0F );
    TempVector.y = ( ( Pickups[ i ].Pos.y - Ships[ WhoIAm ].Object.Pos.y ) / 50.0F );
    TempVector.z = ( ( Pickups[ i ].Pos.z - Ships[ WhoIAm ].Object.Pos.z ) / 50.0F );
#else
    TempVector.x = ( ( Ships[ i ].Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x ) / 50.0F );
    TempVector.y = ( ( Ships[ i ].Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y ) / 50.0F );
    TempVector.z = ( ( Ships[ i ].Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z ) / 50.0F );
#endif
    TargetDistance = VectorLength( &TempVector );

    if( TargetDistance > ( 130.0F * GLOBAL_SCALE ) )
    {
      NormaliseVector( &TempVector );
      TempVector.x *= ( 130.0F * GLOBAL_SCALE );
      TempVector.y *= ( 130.0F * GLOBAL_SCALE );
      TempVector.z *= ( 130.0F * GLOBAL_SCALE );
      Green = 64.0F;
      Blue = 64.0F;
    }
    else
    {
      ApplyMatrix( &Ships[ WhoIAm ].Object.FinalMat, &Forward, &ShipDir );
      NormaliseVector( &ShipDir );
      TargetDir = TempVector;
      NormaliseVector( &TargetDir );
      Cos = DotProduct( &ShipDir, &TargetDir );
      if( Cos < 0.0F )
      {
        Red = 256.0F + ( Cos * 192.0F );
        Green = 256.0F + ( Cos * 192.0F );
      }
    }

    TintModel( MODEL_Ping, Red, Green, Blue, Trans );

    MatrixTranspose( &Ships[ WhoIAm ].Object.FinalMat, &TempMatrix );
    ApplyMatrix( &TempMatrix, &TempVector, &TempVector ); // Calc Up Vector

    view = identity;
    view._11 = 1.25F;
    view._22 = 1.25F;
    view._33 = 1.25F;
    view._41 = TempVector.x;
    view._42 = TempVector.y;
    view._43 = TempVector.z + ( 280.0F * GLOBAL_SCALE );
    view._44 = 1.0F;

    if (lpDev->lpVtbl->SetMatrix(lpDev, hView, &view) != D3D_OK)
      return FALSE;

    if (ReallyExecuteMxloadHeader( &ModelHeaders[MODEL_Ping], (uint16) -1 ) != TRUE )
      return FALSE;
  }

    if( d3dapp->lpD3DViewport->lpVtbl->SetViewport(d3dapp->lpD3DViewport, &viewport) != D3D_OK )
    return FALSE;

  if (lpD3Ddev->lpVtbl->SetMatrix(lpD3Ddev, hProj, &proj) != D3D_OK)
  {
      return FALSE;
  }

  return TRUE;
}

#ifdef LOBBY_DEBUG

#define REGISTRY_LOBBY_KEY    (TEXT("Software\\Microsoft\\DirectPlay\\Applications\\Forsaken"))

#define LOBBY_GUID        "{27E96003-B1C3-11d1-B00C-080009C042E7}"
//#define LOBBY_COMMANDLINE   "-xmen -LogFile -NoAVI"


HKEY ghLobbyKey = NULL;     // lobby registry key handle
static DWORD                    gdwLobbyDisp;        // key created or opened

static LONG RegSetLobby(LPCTSTR lptszName, CONST BYTE * lpData, DWORD dwSize)
{
    return RegSetValueEx(ghLobbyKey, lptszName, 0, REG_SZ, lpData, dwSize);
}

void SetLobbyRegistrySettings( void )
{
  char buf[ 256 ];
  char *appname;
  int i;
  
  // set up registry key
  if ( !ghLobbyKey )
  {
#ifdef FINAL_RELEASE
    RegOpenKeyEx(REGISTRY_ROOT_KEY,
      REGISTRY_LOBBY_KEY,
      0,
      KEY_ALL_ACCESS,
      &ghLobbyKey
      );
#else
    RegCreateKeyEx(REGISTRY_ROOT_KEY,
      REGISTRY_LOBBY_KEY,
      0,
      NULL,                   
      REG_OPTION_NON_VOLATILE,
      KEY_ALL_ACCESS,
      NULL,
      &ghLobbyKey,
      &gdwLobbyDisp
      );
#endif
  }

  // set individual items 
  strcpy( buf, LOBBY_GUID );
    RegSetLobby("Guid", (CONST BYTE *)buf, sizeof( char ) * strlen( buf ) );

  buf[ 0 ] = 0;
  for ( i = 1; i < __argc; i++ )
  {
    strcat( buf, __argv[ i ] );
    strcat( buf, " " );
  }

//  strcpy( buf, LOBBY_COMMANDLINE );
    RegSetLobby("CommandLine", (CONST BYTE *)buf, sizeof( char ) * strlen( buf ) );

  _getcwd( buf, 256 );
    RegSetLobby("CurrentDirectory", (CONST BYTE *)buf, sizeof( char ) * strlen( buf ) );
  
  strcpy( buf, __argv[ 0 ] );
  appname = strrchr( buf, 92 );
  if( appname )
  {
    *appname = 0;
      RegSetLobby("Path", (CONST BYTE *)buf, sizeof( char ) * strlen( buf ) );
    RegSetLobby("File", (CONST BYTE *)&appname[ 1 ], sizeof( char ) * strlen( &appname[ 1 ] ) );
  }
}

#endif


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Once only init Stuff
  Input   : Nothing
  Output    : Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void  OnceOnlyInit( void )
{
  DWORD dwPlatform, dwVersion;

  GetDXVersion( &dwVersion, &dwPlatform);

  if ( dwVersion < 0x600 )
  {
    Msg("You need to install Direct X 6 or later in order to run 'Forsaken'\n");
    exit( 1 );
  }
  
  
  InitPolyText();

#ifndef FINAL_RELEASE
  DeleteFile( LogFilename );
  DeleteFile( BatchFilename );
#endif
  if (!InitDInput())
  {
    DebugPrintf( "Oct2.c OnceOnlyInit() Failed on InitDInput()\n" );
    exit(1);
  }
  FB_Initialise();

  InitValidPickups();
  SetupCharTransTable();

  QueryPerformanceCounter((LARGE_INTEGER *) &LargeTime);
  LastTime = LargeTime;
#ifdef SCROLLING_MESSAGES
  InitStatsMessages();
#endif

#ifdef LOBBY_DEBUG
    SetLobbyRegistrySettings();
#endif

}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Once only Release Stuff
  Input   : Nothing
  Output    : Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void  OnceOnlyRelease( void )
{
  int joystick, i, j;


#ifdef MANUAL_SESSIONDESC_PROPAGATE
    if ( glpdpSD_copy )
    free ( glpdpSD_copy );
#endif
  
  DestroySound( DESTROYSOUND_All );
  TermDInput();

  for (joystick = 0; joystick < Num_Joysticks; joystick++)
  {
    free (JoystickInfo[joystick].Name);

    if ( JoystickInfo[ joystick ].connected )
    {
      for (i = 0; i < JoystickInfo[joystick].NumButtons; i++)
        free (JoystickInfo[joystick].Button[i].name);

      for (i = 0; i < JoystickInfo[joystick].NumPOVs; i++)
      {
        free (JoystickInfo[joystick].POV[i].name);
        for ( j = 0; j < MAX_POV_DIRECTIONS; j++ )
        {
          if ( JoystickInfo[ joystick ].POV[ i ].dirname[ j ] )
            free( JoystickInfo[ joystick ].POV[ i ].dirname[ j ] );
          JoystickInfo[ joystick ].POV[ i ].dirname[ j ] = NULL;
        }
      }

      for (i = 0; i < MAX_JOYSTICK_AXIS; i++)
      {
        if (JoystickInfo[joystick].Axis[i].exists)
          free (JoystickInfo[joystick].Axis[i].name);     
      }
    }
  }
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Change Background Colour
  Input   : float R ( 0.0F - 1.0F )
        : float G ( 0.0F - 1.0F )
        : float B ( 0.0F - 1.0F )
  Output    : BOOL  TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ChangeBackgroundColour( float R, float G, float B )
{
    D3DMATERIAL bmat;
    D3DMATERIALHANDLE hBmat;
  LPDIRECT3D lpD3D = d3dapp->lpD3D;

  BOOL  HadtoCreate = FALSE;


  memset(&bmat, 0, sizeof(D3DMATERIAL));
  bmat.diffuse.r = R;
  bmat.diffuse.g = G;
  bmat.diffuse.b = B;
    bmat.dwSize = sizeof(D3DMATERIAL);
    bmat.dwRampSize = 1;


  if( !lpBmat )
  {
    if (lpD3D->lpVtbl->CreateMaterial(lpD3D, &lpBmat, NULL) != D3D_OK) return FALSE;
    HadtoCreate = TRUE;

  }

  if( lpBmat->lpVtbl->SetMaterial( lpBmat, &bmat ) != D3D_OK) return FALSE;
    if( lpBmat->lpVtbl->GetHandle(lpBmat, d3dapp->lpD3DDevice, &hBmat) != D3D_OK) return FALSE;
    if( d3dapp->lpD3DViewport->lpVtbl->SetBackground( d3dapp->lpD3DViewport, hBmat ) != D3D_OK) return FALSE;

  if( HadtoCreate )
  {
    RELEASE(lpBmat);
  }

  return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Print up how far the init view has to go...
  Input   : BYTE GameStatus
  Output    : void
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void PrintInitViewStatus( BYTE Status )
{
  int i;

  for( i = 0 ; i < ( Status - STATUS_InitView_0 )+1 ; i ++ )
  {
    CenterPrint4x5Text( InitViewMessages[i], ( d3dappi.szClient.cy >> 2 ) + ( i * ( FontHeight + ( FontHeight>>1 ) ) ) , 2 );
  }
}




/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Detect if Power vr is being used...
  Input   : Nothing
  Output    : BOOL  FALSE/TRUE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL IsPowerVRD3DDevice(void)
{
  D3DTEXTUREHANDLE  hTex;
  DDSURFACEDESC     DDSurfDesc;
  LPDIRECTDRAWSURFACE lpDDSurface;
  LPDIRECT3DTEXTURE lpD3DTexture;
  PPVR_CTL      lpPVRCtl;
  LPDIRECTDRAW lpDD = d3dapp->lpDD;
  LPDIRECT3DDEVICE lpD3DDev = d3dapp->lpD3DDevice;

  memset (&DDSurfDesc, 0, sizeof (DDSurfDesc));
  //  Try creating surface...
  DDSurfDesc.dwSize = sizeof (DDSURFACEDESC);
    DDSurfDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    DDSurfDesc.dwHeight = 2;
    DDSurfDesc.dwWidth = 2;
    DDSurfDesc.ddsCaps.dwCaps = DDSCAPS_TEXTURE; 
    DDSurfDesc.ddpfPixelFormat.dwSize = sizeof (DDPIXELFORMAT); 
    DDSurfDesc.ddpfPixelFormat.dwFlags = DDPF_FOURCC; 
  DDSurfDesc.ddpfPixelFormat.dwFourCC = FOURCC_PVR_CTL; 
  //  If creation failed, PVR isn't present.
    if (lpDD->lpVtbl->CreateSurface(lpDD,&DDSurfDesc,&lpDDSurface,NULL) != DD_OK)
  {
    return FALSE;
  }
  //  Lock it to get pointer to our control structure.
  if (lpDDSurface->lpVtbl->Lock(lpDDSurface,NULL,&DDSurfDesc,DDLOCK_WAIT,NULL) != DD_OK)
  {
    RELEASE(lpDDSurface);
    return FALSE;
  }
  lpPVRCtl = (PPVR_CTL) DDSurfDesc.lpSurface;
  //  Get texturing interface
  if (lpDDSurface->lpVtbl->QueryInterface(lpDDSurface,&IID_IDirect3DTexture, &lpD3DTexture) !=DD_OK)
  {
    //  Have the pointer unlock surface now.
    lpDDSurface->lpVtbl->Unlock(lpDDSurface,&DDSurfDesc); 
    RELEASE(lpDDSurface);
    return FALSE;
  }
  //  Get a handle for the texture, if PowerVR is the supplied device we'll 
  //  see a non zero value in lpPVRCtl->pvReserved.
  lpD3DTexture->lpVtbl->GetHandle(lpD3DTexture,lpD3DDev,&hTex);
  //  Zero ? Yes, then its not PVR.
  if (!lpPVRCtl->pvReserved)
  {
    //  Have the pointer unlock surface now.
    lpDDSurface->lpVtbl->Unlock(lpDDSurface,&DDSurfDesc); 
    //  Release objects.
    RELEASE(lpD3DTexture);
      RELEASE(lpDDSurface);
    return FALSE;
  }
  //  Have the pointer unlock surface now.
  lpDDSurface->lpVtbl->Unlock(lpDDSurface,&DDSurfDesc); 
  //  Release objects.
  RELEASE(lpD3DTexture);
    RELEASE(lpDDSurface);
  //  Must be PVR !
  return TRUE;
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Pass On A New Bomb to SomeOne...
  Input   : int Bomb Number...
  Output    : Sets LowestBombTime...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void PassOnNewBomb( int i )
{
  int j , e;
  
  BombNumToSend = i;
  BombTimeToSend = NewBombTime;


  j = LastPersonISentABomb;

  for( e = 0 ; e < MAX_PLAYERS ; e++ )
  {
    j++;
    if( j >= MAX_PLAYERS )
      j = 0;
    if( ( j != WhoIAm ) && ( GameStatus[j] == STATUS_Normal ) )
    {
      SendGameMessage( MSG_BOMB, 0, (BYTE) j, 0, 0 );
      LastPersonISentABomb = j;
      return;
    }
  }
  // uh oh Im the only one playing
  BombActive[i] = TRUE;
  BombTime[i] = NewBombTime;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure : Finds The Bomb With the Shortest Fuse ...
  Input   : Nothing
  Output    : Sets LowestBombTime...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
extern  float   Time_Diff;
void UpdateBombs( void )
{
  int i;
  BOOL  BombHasBlownUp = FALSE;

  OldLowestBombTime = LowestBombTime;
  LowestBombTime = -1;
  if( !BombTag )
    return;

  for( i = 0 ; i < MAXBOMBS ; i++ )
  {
    if( BombActive[i] )
    {
      break;
    }
  }
  if( i == MAXBOMBS )
    ResetCountDownBombTag( 0.0F );

  if( !NumOfBombToStart || !IsHost )
  {
    for( i = 0 ; i < MAXBOMBS ; i++ )
    {
      if( BombActive[i] )
      {
        if( (BombTime[i] -= Time_Diff ) <= 0.0F )
        {
          // One of My Bombs Has Blown Up.... Doh....
          BombHasBlownUp = TRUE;
          break;
        }else if( ( LowestBombTime == -1 ) || (BombTime[i] < BombTime[LowestBombTime] ) )
        {
          LowestBombTime = i;
        }
      }
    }
    if( BombHasBlownUp )
    {
      Ships[WhoIAm].ShipThatLastHitMe = MAX_PLAYERS;
      Ships[WhoIAm].Damage = 255 + 255; //make sure I Die....
      DoDamage( OVERRIDE_INVUL );
    
    
      Ships[WhoIAm].Object.Mode = DEATH_MODE;
      Ships[WhoIAm].Timer = 0.0F;
      AddMessageToQue( A_BOMB_KILLED_YOU );
      ShipDiedSend( WEPTYPE_Primary, 0);
    
    
      for( i = 0 ; i < MAXBOMBS ; i++ )
      {
        if( BombActive[i] )
        {
          BombActive[i] = FALSE;
          PassOnNewBomb( i );
        }
      }
      LowestBombTime = -1;
    }

    if( LowestBombTime != -1 )
    {
      if( LowestBombTime != OldLowestBombTime )
      {
        ResetCountDownBombTag( BombTime[LowestBombTime] );
      }
    }
  }

  UpdateCountdownDigits();


  if( NumOfBombToStart && IsHost )
  {
    while( NumOfBombToStart )
    {
      NumOfBombToStart--;
      PassOnNewBomb( NumOfBombToStart );
    }
    Time_Diff = 0.0F;
  }



}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Init The Bombs...
  Input   :   nothing...
  Output    :   nothing..
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void InitBombs( void )
{
  int i;
  NewBombTime = ( 100.0F * 30.0F ); // approx 30 seconds...
  LowestBombTime = -1;
  OldLowestBombTime = -1;
  LastPersonISentABomb = 0;
  NumOfBombToStart = 1;

  for( i = 0 ; i < MAXBOMBS ; i++ )
  {
    BombActive[i]= FALSE;
    BombTime[i] = 0.0F;
  }
}




/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Init Stats Display Stuff...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  InitStatsDisplay()
{
  // Create the offscreen surface, by loading our bitmap.

//  if( ModeCase != -1 )
//  {
//    lpDDSOne = DDLoadBitmap( d3dapp->lpDD, (char*) &StatsNames[ModeCase] , 0, 0 );
//    ddpal =  DDLoadPalette( d3dapp->lpDD , (char*) &StatsNames[ModeCase]);
//    lpDDSOne->lpVtbl->SetPalette( lpDDSOne , ddpal );
//  }
  InitFont(FALSE);
  ChangeBackgroundColour( 0, 0, 0 );

  HowManySecrets( &TotalSecrets , &Secrets );

  return TRUE;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Free Stats Display Stuff...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL  FreeStatsDisplay()
{
//  ReleaseDDSurf(lpDDSOne);
  ReleaseDDSurf(lpDDSTwo);
  lpDDSTwo = NULL;
  return TRUE;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Stats Display...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL StatsDisplay()
{
#if 0
    HRESULT ddrval;
    RECT    src;
  RECT    dest;
  DDBLTFX fx;
  int x,y;
#endif
  char buf[256];

#if 0
  //  Blt Background
  src.top = 0;
  src.left = 0;
  src.right = d3dappi.szClient.cx;
  src.bottom = d3dappi.szClient.cy;
  x = 0;
  y = 0;
  memset(&fx, 0, sizeof(DDBLTFX));
  fx.dwSize = sizeof(DDBLTFX);
  dest.top = 0;
  dest.bottom = d3dappi.szClient.cy;
  dest.left = 0;
  dest.right = d3dappi.szClient.cx;
      
  while( 1 )
  {
    ddrval = d3dapp->lpBackBuffer->lpVtbl->Blt( d3dapp->lpBackBuffer, &dest, lpDDSOne, &src, DDBLT_WAIT , &fx );
    if( ddrval == DD_OK )
      break;
    if( ddrval == DDERR_SURFACELOST )
    {
      d3dapp->lpFrontBuffer->lpVtbl->Restore(d3dapp->lpFrontBuffer);
      d3dapp->lpBackBuffer->lpVtbl->Restore(d3dapp->lpBackBuffer);
      DDReLoadBitmap( lpDDSOne, (char*) &StatsNames[ModeCase] );
      break;
    }
    if( ddrval != DDERR_WASSTILLDRAWING )
      break;
  }
#endif
  if( !GameCompleted )
  {
    if( Secrets == 1 )
    {
      sprintf( (char*) &buf ,"%d Secret found out of %d", Secrets , TotalSecrets );
    }else{
      sprintf( (char*) &buf ,"%d Secrets found out of %d", Secrets , TotalSecrets );
    }
    CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy >> 1 ) - (FontHeight*2) , 2 );
    if( NumKilledEnemies == 1 )
    {
      sprintf( (char*) &buf ,"%d Enemy killed out of %d", NumKilledEnemies , NumInitEnemies );
    }else{
      sprintf( (char*) &buf ,"%d Enemies killed out of %d", NumKilledEnemies , NumInitEnemies );
    }
    CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy >> 1 ) + (FontHeight*0) , 2 );
    if( CrystalsFound == 1 )
    {
      sprintf( (char*) &buf ,"%d Crystal found so far", CrystalsFound );
    }else{
      sprintf( (char*) &buf ,"%d Crystals found so far", CrystalsFound );
    }
    CenterPrint4x5Text( &buf[0] , (d3dappi.szClient.cy >> 1 ) + (FontHeight*2) , 2 );
  }else{
    CenterPrint4x5Text( "Congratulations" , (d3dappi.szClient.cy >> 1 ) - (FontHeight*2) , 2 );
    CenterPrint4x5Text( "Demo Complete" , (d3dappi.szClient.cy >> 1 ) + (FontHeight*0) , 2 );
    if( DifficultyLevel != 3 )
    {
      CenterPrint4x5Text( "Now Try a Harder Difficulty Setting" , (d3dappi.szClient.cy >> 1 ) + (FontHeight*2) , 2 );
    }else{
      CenterPrint4x5Text( "Try the real Game for a harder challenge" , (d3dappi.szClient.cy >> 1 ) + (FontHeight*2) , 2 );
    }
  }
  
  CenterPrint4x5Text( "Press Space to continue" , d3dappi.szClient.cy - (FontHeight*2) , 2 );
//  Our_CalculateFrameRate();


  return TRUE;
}

void InitModeCase(void)
{
#if 0
  if( d3dappi.szClient.cx >= 1024 && d3dappi.szClient.cy >= 768 )
    ModeCase = Mode1024X768;
  else if( d3dappi.szClient.cx >= 800 && d3dappi.szClient.cy >= 600 )
    ModeCase = Mode800X600;
  else if( d3dappi.szClient.cx >= 640 && d3dappi.szClient.cy >= 480 )
    ModeCase = Mode640X480;
  else if( d3dappi.szClient.cx >= 640 && d3dappi.szClient.cy >= 400 )
    ModeCase = Mode640X400;
  else if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
    ModeCase = Mode512X384;
  else if( d3dappi.szClient.cx >= 320 && d3dappi.szClient.cy >= 400 )
    ModeCase = Mode320X400;
  else if( d3dappi.szClient.cx >= 320 && d3dappi.szClient.cy >= 240 )
    ModeCase = Mode320X240;
  else
    ModeCase = Mode320X200;
#endif

  ModeCase = 0; // now dynamic

  ModeScaleX[ 0 ] = (float)d3dappi.szClient.cx / 320.0F;
  ModeScaleY[ 0 ] = (float)d3dappi.szClient.cy / 200.0F;

  if ( ( d3dappi.szClient.cx >= 512 ) && ( d3dappi.szClient.cy >= 384 ) )
  {
    strcpy( DynamicFontNames, FontNames[ Mode512X384 ] );
  }else
  {
    strcpy( DynamicFontNames, FontNames[ Mode320X200 ] );
  }

  if( d3dappi.szClient.cx >= 800 && d3dappi.szClient.cy >= 600 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode800X600 ] );
  else if( d3dappi.szClient.cx >= 640 && d3dappi.szClient.cy >= 480 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode640X480 ] );
  else if( d3dappi.szClient.cx >= 640 && d3dappi.szClient.cy >= 400 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode640X400 ] );
  else if( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode512X384 ] );
  else if( d3dappi.szClient.cx >= 320 && d3dappi.szClient.cy >= 400 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode320X400 ] );
  else if( d3dappi.szClient.cx >= 320 && d3dappi.szClient.cy >= 240 )
    strcpy( DynamicScoreNames, ScoreNames[ Mode320X240 ] );
  else
    strcpy( DynamicScoreNames, ScoreNames[ Mode320X200 ] );

  // Check for Polygon Text.....
  bPolyText = FALSE;
  if(( d3dappi.szClient.cx >= 512 && d3dappi.szClient.cy >= 384 ) && PolygonText && !IsServer )
    bPolyText = TRUE;

}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Get Bit Shift required to make first bit of
        :   mask bit 0
  Input   :   int32 Mask
  Output    :   int16 Shift Value
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
int16 GetBitShift( int32 Mask )
{
  int16 Count;

  if( Mask )
  {
    Count = 0;

    while( !( Mask & 1 ) )
    {
      Count++;
      Mask >>= 1;
    }

    return( Count );
  }

  return( 0 );
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Render Snapshot
  Input   :   Nothing
  Output    :   Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void RenderSnapshot( void )
{
  LPDIRECT3DDEVICE lpDev = d3dapp->lpD3DDevice;
    LPDIRECT3DVIEWPORT lpView = d3dapp->lpD3DViewport;

  lpDev->lpVtbl->BeginScene(lpDev);

  CurrentCamera.enable = 1;
  CurrentCamera.UseLowestLOD = TRUE;
  CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;  
  CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
  CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
  CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
  CurrentCamera.Viewport = viewport;  
  CurrentCamera.Proj = proj;  
  
  CurrentCamera.Viewport.dwX = 0;
  CurrentCamera.Viewport.dwY = 0;
  CurrentCamera.Viewport.dwWidth = 128;
  CurrentCamera.Viewport.dwHeight = 128;
  CurrentCamera.Viewport.dvScaleX = CurrentCamera.Viewport.dwWidth / (float)2.0;
  CurrentCamera.Viewport.dvScaleY = CurrentCamera.Viewport.dwHeight / (float)2.0;
  CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwWidth),
                     D3DVAL(2 * CurrentCamera.Viewport.dvScaleX));
  CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(D3DVAL(CurrentCamera.Viewport.dwHeight),
                     D3DVAL(2 * CurrentCamera.Viewport.dvScaleY));
  
  CurrentCamera.UseLowestLOD = TRUE;
  if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
    return;

  lpDev->lpVtbl->EndScene(lpDev);
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Save PPM Bitmap
  Input   :   int8  * Filename
        :   int8  * Screen Pointer
        :   uint32    Width
        :   uint32    Height
        :   uint32    Bytes Per Pixel
        :   uint32    LineSize
        :   uint32    RedMask
        :   uint32    GreenMask
        :   uint32    BlueMask
        :   uint32    x1, y1, x2 ,y2 ( Portion of Screen )
  Output    :   BOOL    True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL SavePPM( uint8 * Filename, uint8 * ScreenPtr, uint32 Width, uint32 Height, uint32 BytesPerPixel,
        uint32 LineSize, uint32 RedMask, uint32 GreenMask, uint32 BlueMask,
        uint32 x1, uint32 y1, uint32 x2, uint32 y2 )
{
  uint32    XCount, YCount;
  uint32    RedMul, GreenMul, BlueMul;
  uint8   Red, Green, Blue;
  int16   RedShift, GreenShift, BlueShift;
  uint8 * LinePtr;
  int8    Header[ 256 ];
  FILE  * fp;
  int16   i;
  uint32    Val;
  uint32    Xoff, Yoff;
  uint32    Xsize, Ysize;

  if( ( x1 == x2 ) && ( y1 == y2 ) )
  {
    x1 = 0;
    y1 = 0;
    x2 = Width;
    y2 = Height;
  }

  Xoff = x1;
  Yoff = y1;
  Xsize = ( x2 - x1 );
  Ysize = ( y2 - y1 );

  fp = fopen( Filename, "wb" );

  if( fp != NULL )
  {
    sprintf( &Header[ 0 ], "P6%c#Forsaken (c)1998%c%d %d%c255%c", 10, 10, Xsize, Ysize, 10, 10 );

    i = 0;
    while( Header[ i ] )
    {
      fwrite( &Header[ i ], sizeof( int8 ), 1, fp );
      i++;
    }

    RedShift = GetBitShift( RedMask );
    GreenShift = GetBitShift( GreenMask );
    BlueShift = GetBitShift( BlueMask );

    RedMul = ( 256 / ( ( RedMask >> RedShift ) + 1 ) );
    GreenMul = ( 256 / ( ( GreenMask >> GreenShift ) + 1 ) );
    BlueMul = ( 256 / ( ( BlueMask >> BlueShift ) + 1 ) );

    ScreenPtr += ( Yoff * LineSize );

    for( YCount = 0; YCount < Ysize; YCount++ )
    {
      LinePtr = ( ScreenPtr + Xoff );

      for( XCount = 0; XCount < Xsize; XCount++ )
      {
        Val = 0;

        switch( BytesPerPixel )
        {
          case 1:
            Val = (uint32) LinePtr[0];
            break;
          case 2:
            Val = (uint32) ( (uint32) LinePtr[0] ) |
                   ( ( (uint32) LinePtr[1] ) << 8 );
            break;
          case 3:
            Val = (uint32) ( (uint32) LinePtr[0] ) |
                   ( ( (uint32) LinePtr[1] ) << 8 ) |
                   ( ( (uint32) LinePtr[2] ) << 16 );
            break;
        }
        LinePtr += BytesPerPixel;

        Red   = (uint8) ( ( ( Val & RedMask ) >> RedShift ) * RedMul );
        Green = (uint8) ( ( ( Val & GreenMask ) >> GreenShift ) * GreenMul );
        Blue  = (uint8) ( ( ( Val & BlueMask ) >> BlueShift ) * BlueMul );

        fwrite( &Red, sizeof( Red ), 1, fp );
        fwrite( &Green, sizeof( Green ), 1, fp );
        fwrite( &Blue, sizeof( Blue ), 1, fp );
      }

      ScreenPtr += LineSize;
    }

    fclose( fp );
  }
  else
  {
    DebugPrintf( "Failed to create %s\n", Filename );
    return( FALSE );
  }

  return( TRUE );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Save SnapShot screen for saved game
  Input   :   int8  * Filename
  Output    :   BOOL    True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL SaveSnapShot( int8 * Filename )
{
  HRESULT     hr;
  DDSURFACEDESC SurfaceDesc;

  FreeAllLastAFrameScrPolys();
  RenderSnapshot();

  memset( &SurfaceDesc, 0, sizeof( SurfaceDesc ) );
  SurfaceDesc.dwSize = sizeof( SurfaceDesc );

  hr = d3dapp->lpBackBuffer->lpVtbl->Lock( d3dapp->lpBackBuffer, NULL, &SurfaceDesc,
        DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_READONLY, NULL );

  if ( hr != DD_OK )
  {
    switch( hr )
    {
      case DDERR_INVALIDOBJECT:
        DebugPrintf( "Error Locking Surface ( Invalid Object )\n" );
        break;

      case DDERR_INVALIDPARAMS:
        DebugPrintf( "Error Locking Surface ( Invalid Params )\n" );
        break;

      case DDERR_OUTOFMEMORY:
        DebugPrintf( "Error Locking Surface ( Out of Memory )\n" );
        break;

      case DDERR_SURFACEBUSY:
        DebugPrintf( "Error Locking Surface ( Surface Busy )\n" );
        break;

      case DDERR_SURFACELOST:
        DebugPrintf( "Error Locking Surface ( Surface Lost )\n" );
        break;

      case DDERR_WASSTILLDRAWING:
        DebugPrintf( "Error Locking Surface ( Was Still Drawing )\n" );
        break;
    }
    return( FALSE );
  }

  SavePPM( Filename, SurfaceDesc.lpSurface, SurfaceDesc.dwWidth, SurfaceDesc.dwHeight,
        ( ( SurfaceDesc.ddpfPixelFormat.dwRGBBitCount + 7 )  / 8 ),
        SurfaceDesc.lPitch, SurfaceDesc.ddpfPixelFormat.dwRBitMask, SurfaceDesc.ddpfPixelFormat.dwGBitMask,
        SurfaceDesc.ddpfPixelFormat.dwBBitMask, 0, 0, 128, 128 );

  hr = d3dapp->lpBackBuffer->lpVtbl->Unlock( d3dapp->lpBackBuffer, NULL );
  if ( hr != DD_OK )
  {
    DebugPrintf( "Error Unlocking Surface\n" );
    return( FALSE );
  }

  if( CurrentMenu ) MenuDraw( CurrentMenu );
  MainGame( d3dapp->lpD3DDevice, d3dapp->lpD3DViewport );

  return( TRUE );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Save SnapShot screen
  Input   :   int8  * Filename
  Output    :   BOOL    True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL SaveFullScreenSnapShot( int8 * Filename )
{
  HRESULT     hr;
  DDSURFACEDESC SurfaceDesc;

  memset( &SurfaceDesc, 0, sizeof( SurfaceDesc ) );
  SurfaceDesc.dwSize = sizeof( SurfaceDesc );

  hr = d3dapp->lpFrontBuffer->lpVtbl->Lock( d3dapp->lpFrontBuffer, NULL, &SurfaceDesc,
        DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_READONLY, NULL );

  if ( hr != DD_OK )
  {
    switch( hr )
    {
      case DDERR_INVALIDOBJECT:
        DebugPrintf( "Error Locking Surface ( Invalid Object )\n" );
        break;

      case DDERR_INVALIDPARAMS:
        DebugPrintf( "Error Locking Surface ( Invalid Params )\n" );
        break;

      case DDERR_OUTOFMEMORY:
        DebugPrintf( "Error Locking Surface ( Out of Memory )\n" );
        break;

      case DDERR_SURFACEBUSY:
        DebugPrintf( "Error Locking Surface ( Surface Busy )\n" );
        break;

      case DDERR_SURFACELOST:
        DebugPrintf( "Error Locking Surface ( Surface Lost )\n" );
        break;

      case DDERR_WASSTILLDRAWING:
        DebugPrintf( "Error Locking Surface ( Was Still Drawing )\n" );
        break;
    }
    return( FALSE );
  }

  SavePPM( Filename, SurfaceDesc.lpSurface, SurfaceDesc.dwWidth, SurfaceDesc.dwHeight,
        ( ( SurfaceDesc.ddpfPixelFormat.dwRGBBitCount + 7 )  / 8 ),
        SurfaceDesc.lPitch, SurfaceDesc.ddpfPixelFormat.dwRBitMask, SurfaceDesc.ddpfPixelFormat.dwGBitMask,
        SurfaceDesc.ddpfPixelFormat.dwBBitMask, 0, 0, SurfaceDesc.dwWidth, SurfaceDesc.dwHeight );

  hr = d3dapp->lpFrontBuffer->lpVtbl->Unlock( d3dapp->lpFrontBuffer, NULL );
  if ( hr != DD_OK )
  {
    DebugPrintf( "Error Unlocking Surface\n" );
    return( FALSE );
  }
  return( TRUE );
}

#define DebugPrintf if ( 0 ) DebugPrintf

static int CheckFileWriteable( char *fname )
{
    HANDLE      fhandle;

    if ( ( fhandle = CreateFile( fname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ))
    == INVALID_HANDLE_VALUE )
  {
    DebugPrintf( "CreateFile( %s ) failed\n", fname );
    return 0;
  }
    else
  {
    DWORD attr;

        CloseHandle( fhandle );
    if ( ( attr = GetFileAttributes( fname ) ) == 0xFFFFFFFFL )
    {
      DebugPrintf( "GetFileAttributes( %s ) failed\n", fname );
      return -1;
    }
        if ( !SetFileAttributes( fname, FILE_ATTRIBUTE_NORMAL ) )
    {
      DebugPrintf( "SetFileAttributes( %s, 0x%x ) failed\n", fname, FILE_ATTRIBUTE_NORMAL );
      return -2;
    }
        if ( !SetFileAttributes( fname, attr ) )
    {
      DebugPrintf( "SetFileAttributes( %s, 0x%x ) failed\n", fname, attr );
      return -3;
    }
  }

  return 1;
}

#ifdef  REFLECTION
LPDIRECTDRAWSURFACE lpDestTextureSurf = NULL;
void WierdShit( void )
{
    HDC                 hdcPrimary;
    HDC                 hdcDest;
    HRESULT             hr;
  DDSURFACEDESC ddsd2;
    LPDIRECT3DTEXTURE lpDestTexture = NULL;

  if( MyGameStatus != STATUS_Normal )
    return;


  if( !lpDestTextureSurf )
  {
    memcpy(&ddsd2, &d3dappi.ThisTextureFormat.ddsd, sizeof(DDSURFACEDESC));
    ddsd2.dwSize = sizeof(DDSURFACEDESC);
    ddsd2.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd2.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;

    ddsd2.dwHeight = 128;
    ddsd2.dwWidth = 256;
    
    if (d3dappi.lpDD->lpVtbl->CreateSurface(d3dappi.lpDD , &ddsd2, &lpDestTextureSurf, NULL) != DD_OK)
      lpDestTextureSurf = NULL;
  }
  if( !lpDestTextureSurf )
    return;


    if ((hr = d3dapp->lpBackBuffer->lpVtbl->GetDC( d3dapp->lpBackBuffer, &hdcPrimary )) == DD_OK)
  {
      if ((hr = lpDestTextureSurf->lpVtbl->GetDC( lpDestTextureSurf, &hdcDest )) == DD_OK)
    {

        StretchBlt(hdcDest, 0, 0, 128, 128, hdcPrimary, 0, 0, 128, 128, SRCCOPY);

          lpDestTextureSurf->lpVtbl->ReleaseDC(lpDestTextureSurf, hdcDest);
    }
        d3dapp->lpBackBuffer->lpVtbl->ReleaseDC( d3dapp->lpBackBuffer, hdcPrimary);
  }
  LastError = lpDestTextureSurf->lpVtbl->QueryInterface(lpDestTextureSurf,
                       &IID_IDirect3DTexture,
                       (LPVOID*)&lpDestTexture);

    Tloadheader.lpTexture[12]->lpVtbl->Load(Tloadheader.lpTexture[12], lpDestTexture);

}
#endif



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   Server Main Routines...
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void ServerMainRoutines( void )
{
  ProcessShips();
//  FirePrimary();
//  ProcessEnemies();
  ProcessSpotFX();
  ProcessPrimaryBullets();
  ProcessSecondaryBullets();
  if( !PlayDemo ) RegeneratePickups();
  ProcessPickups();
  ProcessBGObjects( TRUE );
//  ProcessRestartPoints();
  ProcessModels();
  ProcessPolys();
  ProcessXLights( &Mloadheader );
  DoAfterBurnerEffects();
  FmPolyProcess();
  CheckTimeLimit();
  if( CountDownOn ) UpdateCountdownDigits();
//  if( ForsakenAnyKey ) DisplayForsakenAnyKey();
//  ShowScreenMultiples();
  ProcessActiveConditions();
  ProcessTriggerAreas();
//  UpdateBombs();
  ProcessGoals();
  WaterProcess();
  ProcessRTLights();
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :   ServerMain Render Loop...
  Input   :   nothing...
  Output    :   nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
float ServerDrawTimer = 0.0F;
#define PSEUDOHOST_CHECK_TIMER 1200.0F
float PseudoHostCheckTimer = PSEUDOHOST_CHECK_TIMER;

BOOL ServerMainGame(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView )
{
  int     i;
  LONGLONG  TempTime;
  VECTOR    ScatterDir;

  if ( !ServerChoosesGameType )
  {
    // check we have a pseudohost...
    PseudoHostCheckTimer -= framelag;
    if ( PseudoHostCheckTimer < 0.0F )
    {
      PseudoHostCheckTimer = PSEUDOHOST_CHECK_TIMER;
      if ( !PseudoHostDPID )
      {
        AllocatePseudoHost();
      }
    }
  }

  QueryPerformanceCounter((LARGE_INTEGER *) &GameCurrentTime);

  GameCurrentTime = GameCurrentTime - GameStartedTime;

//  Ships[WhoIAm].enable = FALSE;
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Main Routines to be called before Rendering....  
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  // set up groups indirectly visible by all active cameras
  InitIndirectVisible( Ships[Current_Camera_View].Object.Group );

  if( !PowerVR && ( ActiveRemoteCamera || (MissileCameraActive && MissileCameraEnable) ) )
  {
    AddIndirectVisible( (uint16) ( ( ActiveRemoteCamera ) ? ActiveRemoteCamera->Group : SecBulls[ CameraMissile ].GroupImIn ) );
  }

  ServerMainRoutines();

  if( MyGameStatus == STATUS_QuitCurrentGame )
    return TRUE;


  
  TloadCheckForLostSurfaces(&Tloadheader);
  memset( (void*) &IsGroupVisible[0] , 0 , MAXGROUPS * sizeof(uint16) );
  cral += (framelag*2.0F);

  for( i = 0 ; i < MAX_SFX ; i++ )
  {
    LastDistance[i] = 100000.0F;
  }

#ifdef SOFTWARE_ENABLE
  if( SoftwareVersion )
  {
//    CWClearZBuffer();
  }
#endif

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  Procedure :  Now the Rendering can begin...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
  if( ServerRendering )
  {
    if (lpDev->lpVtbl->BeginScene(lpDev) != D3D_OK)
      return FALSE;

    PreventFlips = FALSE;
    
    CameraRendering = CAMRENDERING_Main;
    MainCamera.enable = 1;
    MainCamera.GroupImIn = Ships[Current_Camera_View].Object.Group; 
    MainCamera.Mat = Ships[Current_Camera_View].Object.FinalMat;  
    MainCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat;  
    MainCamera.Pos = Ships[Current_Camera_View].Object.Pos; 
    MainCamera.Viewport = viewport; 
    MainCamera.Proj = proj; 
    CurrentCamera = MainCamera;
    CurrentCamera.UseLowestLOD = FALSE;
    if( RenderCurrentCamera( lpDev , lpView ) != TRUE ) 
      return FALSE;

    if (lpDev->lpVtbl->EndScene(lpDev) != D3D_OK)
      return FALSE;
  }else{
    ServerDrawTimer -= framelag;
    if( ServerDrawTimer > 0.0F )
    {
      PreventFlips = TRUE;
    }else{
      D3DAppClearScreenOnly();
      CenterPrint4x5Text( FORSAKEN_SERVER, (d3dappi.szClient.cy>>1)-(FontHeight>>1) , 2 );
      PreventFlips = FALSE;
      ServerDrawTimer = 60.0F;
    }
  }

  ScreenPolyProcess();


/* Secondary routines called after rendering */
    
  DplayGameUpdate();

  if( Our_CalculateFrameRate() != TRUE)
    return FALSE;


#ifdef SOFTWARE_ENABLE
  if( !SoftwareVersion )
#endif
  {
    if( CurrentMenu && CurrentMenuItem )
    {
      if( !ServerRendering )
      {
        D3DAppClearScreenOnly();
        PreventFlips = FALSE;
      }
      MenuDraw( CurrentMenu );

      MenuItemDrawCursor( CurrentMenuItem );

// Just to make sure that another press of escape doesnt take you back into the menu you wanted to exit!!
      JustExitedMenu = TRUE;
      
      MenuProcess();
    }else{
      ScoreSort();
      PrintScoreSort();
    }

  }

  ProcessGameKeys();

  CheckForRogueSfx();

  CheckLevelEnd();


  QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
  for( i = 1 ; i < MAX_PLAYERS ; i++ )
  {
    if( (GameStatus[i] != STATUS_GetPlayerNum ) && 
      (GameStatus[i] != STATUS_DisabledByServer ) &&
      (GameStatus[i] != STATUS_StartingMultiplayer ) &&
      (GameStatus[i] != STATUS_Joining ) &&
      (GameStatus[i] != STATUS_LeftCrashed ) &&
      (GameStatus[i] != STATUS_Left ) &&
      (GameStatus[i] != STATUS_Null ) )
    {
      if( ( (TempTime - LastPacketTime[i]) / Freq ) > 15 )
      {
//        SendGameMessage(MSG_YOUQUIT, 0, (BYTE) i , 0, 0);
        ScatterDir = Ships[ i ].LastMove;
        NormaliseVector( &ScatterDir );
        ScatterWeaponsForShip( (uint16) i, &ScatterDir, MAXPICKUPS ); // Scatter all weapons in direction.
        GameStatus[i] = STATUS_DisabledByServer;
        Ships[i].enable = FALSE;
        LastPacketTime[i] = TempTime;
      }
    }
  }
  return TRUE;
}



