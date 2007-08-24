/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 365 $
 *
 * $Header: /PcProjectX/models.c 365   11/11/98 16:00 Philipy $
 *
 * $Log: /PcProjectX/models.c $
 * 
 * 365   11/11/98 16:00 Philipy
 * various fixes for warnings / errors when compiling under VC6
 * 
 * 364   15/09/98 10:21 Phillipd
 * 
 * 363   15/09/98 9:31 Phillipd
 * 
 * 362   14/09/98 15:16 Phillipd
 * 
 * 361   14/09/98 12:42 Phillipd
 * 
 * 360   30/07/98 10:33 Collinsd
 * 
 * 359   14/07/98 10:26 Collinsd
 * Shadow code added under SHADOWTEST
 * 
 * 358   2/07/98 16:32 Collinsd
 * Orbitals killed when player is killed
 * 
 * 357   24/06/98 11:04 Philipy
 * added gateway flag
 * 
 * 356   15/06/98 11:04 Oliverc
 * 
 * 355   15/06/98 10:53 Oliverc
 * Added cerbero models back for WIN98SHAREWARE (as used for boss in
 * abandoned subway)
 * 
 * 354   15/06/98 10:48 Oliverc
 * Disabled more models for WIN98SHAREWARE version
 * 
 * 353   11/06/98 16:57 Philipy
 * loads of win98 shareware version stuff
 * 
 * 352   11/06/98 9:48 Collinsd
 * Orbitals now work in server game.
 * 
 * 351   13/05/98 16:13 Collinsd
 * Gravgon Range Check altered
 * 
 * 350   4/17/98 10:40a Phillipd
 * 
 * 349   16/04/98 12:59 Oliverc
 * More literal text converted to defined strings for localisation
 * 
 * 348   15/04/98 11:45 Collinsd
 * Flygirl now stealths correctly
 * 
 * 347   11/04/98 17:18 Collinsd
 * Taken out lines
 * 
 * 346   10/04/98 21:14 Collinsd
 * 
 * 345   10/04/98 20:32 Collinsd
 * 
 * 344   10/04/98 17:07 Collinsd
 * 
 * 343   10/04/98 12:54 Collinsd
 * Names appearing in demo playback ( Needs option on menu )
 * Bikes now clip against camera
 * 
 * 342   9/04/98 16:12 Philipy
 * engine frequency now clamped to maximum
 * 
 * 341   4/09/98 11:30a Phillipd
 * 
 * 340   8/04/98 16:08 Collinsd
 * 
 * 339   8/04/98 13:19 Collinsd
 * Bike engines not toggle on/off
 * 
 * 338   7/04/98 21:38 Collinsd
 * Stealthed shade plays engine sound
 * 
 * 337   7/04/98 10:54 Collinsd
 * Fixed corruption bug.
 * 
 * 336   4/07/98 10:29a Phillipd
 * 
 * 335   5/04/98 15:00 Philipy
 * started pre AVI CD accesss ( not yet implemented )
 * bike engine freq now done on 5 frame average
 * prevented CD track from playing in titles if set to off
 * NoDynamic SFX does not bike bike computer static anymore
 * water detail slider now only has two levels
 * 
 * 334   3/04/98 14:54 Collinsd
 * 
 * 333   2/04/98 21:06 Philipy
 * Added taunts ( single & multiplayer, plus enemy biker taunts )
 * added flygirl to front end.
 * sliders for biker, bike computer and taunt speech volume
 * added new sfx for title
 * 
 * 332   2/04/98 19:27 Collinsd
 * 
 * 331   2/04/98 19:07 Collinsd
 * 
 * 330   2/04/98 14:51 Collinsd
 * Stealthed ships now work
 * 
 * 329   1/04/98 15:30 Collinsd
 * Auto and bike details now work again.
 * 
 * 328   1/04/98 14:12 Collinsd
 * Component SFX
 * 
 * 327   31/03/98 16:23 Collinsd
 * Added spotfx check for 0 0 
 * 
 * 326   28/03/98 20:50 Collinsd
 * BGObjects now use collision zone bounding box.
 * Also fixed spotfx playing when paused.
 * 
 * 325   27/03/98 12:58 Philipy
 * changed cheat mode stuff
 * fixed bug that prevented individual variants of variant sfx to be
 * mapped
 * correct menutv now displayed between levels
 * 
 * 324   26/03/98 12:28 Philipy
 * added water detail & gamma sliders
 * fixed holo object swapping stuff
 * fixed crash bug when selecting room in debug mode
 * 
 * 323   20/03/98 17:23 Collinsd
 * Hopefully fixed crashbug on reactor on nps-sp01
 * 
 * 322   19/03/98 20:48 Collinsd
 * Added Model and Texture loading optimisation. ( Doesn't load
 * textures/models not used in level/mode ).
 * 
 * 321   19/03/98 14:22 Collinsd
 * Invulnerability effect no longer visible in demo.
 * 
 * 320   19/03/98 11:30 Collinsd
 * 
 * 319   19/03/98 11:29 Philipy
 * implemented new acclaim splash screen plus AVI
 * 
 * 318   3/19/98 10:20a Phillipd
 * 
 * 317   15/03/98 18:40 Philipy
 * added water effect splash screen
 * fixed bug with end game sequence
 * implemented attract mode
 * text macros now saved in config
 * 
 * 316   14/03/98 19:02 Oliverc
 * Added FAST_RGB_CLAMP code for lighting in assembler
 * 
 * 315   14/03/98 18:58 Collinsd
 * Added godmode and made debug mode work even when you change level.
 * 
 * 314   11/03/98 21:01 Collinsd
 * Added naked bikers.
 * 
 * 313   7/03/98 20:05 Collinsd
 * Bike Glows now on option and debug rays no longer loaded.
 * 
 * 312   6/03/98 15:27 Oliverc
 * Fixed bug in speed calculation when paused in demo playback
 * 
 * 311   3/03/98 18:20 Collinsd
 * CTF Team game
 * 
 * 310   2/03/98 21:13 Collinsd
 * No longer use multiple bit or secfire.
 * 
 * 309   2/03/98 12:02 Collinsd
 * Added ability to allocate comp objects from front end.
 * 
 * 308   28/02/98 16:18 Philipy
 * fixed memory leak
 * sfx loader does not now try to load sfx into hw if it knows that there
 * are insufficient resources
 * added buffer valid check b4 modifying looping sfx
 * 
 * 307   28/02/98 13:36 Collinsd
 * 
 * 306   27/02/98 19:11 Philipy
 * fixed load game sfx bug
 * added pseudo dithering for 8 bit saved game pic
 * flygirl now selectable from front end ( no model displayed )
 * 
 * 305   27/02/98 16:30 Collinsd
 * Added relinit sfx code.
 * 
 * 304   27/02/98 15:42 Collinsd
 * Added error check for overwriting sfxid...
 * Also added lives to save game. Ooops
 * 
 * 303   26/02/98 20:41 Philipy
 * added front end for load game
 * 
 * 302   25/02/98 16:04 Collinsd
 * Save Picture for save game...
 * 
 * 301   23/02/98 21:20 Collinsd
 * Optimised Load/Save Functions.
 * 
 * 300   23/02/98 13:00 Philipy
 * added scan lines for inter-level objects
 * added speech for bike selection in front end
 * 
 * 299   23/02/98 10:37 Philipy
 * added inter level stuff
 * 
 * 298   19/02/98 22:00 Collinsd
 * Added flygirl biker.
 * 
 * 297   18/02/98 11:53 Oliverc
 * First feature-complete version of force feedback joystick code
 * 
 * 296   17/02/98 9:17 Philipy
 * added frequency update for bike engine loop
 * 
 * 295   16/02/98 16:53 Collinsd
 * Added Chris's new code.
 * 
 * 294   16/02/98 14:33 Collinsd
 * Started LoadSave
 * 
 * 293   13/02/98 13:00 Collinsd
 * Added SoundFX on Enemies.
 * 
 * 292   12/02/98 14:09 Collinsd
 * Fixed targeting of bosses by homing missiles.
 * 
 * 291   11/02/98 16:29 Collinsd
 * Added soundfx for bikes/enemies
 * 
 * 290   11/02/98 12:57 Philipy
 * Changed PlaySfx calls to use Vol instead of Dist
 * 
 * 289   10/02/98 17:02 Collinsd
 * Fixed shockwaves giving credit for death to you.
 * 
 * 288   10/02/98 16:08 Collinsd
 * Toggle for descent/forsaken collision added
 * 
 * 287   10/02/98 11:40 Collinsd
 * Added code for descent/forsaken collision
 * 
 * 286   5/02/98 12:51 Collinsd
 * Bomb tag works again and invulnerable enemies are no longer effected by
 * shockwaves
 * 
 * 285   5/02/98 10:03 Oliverc
 * Monochrome ambient lights for PowerVR
 * 
 * 284   3/02/98 15:30 Collinsd
 * Added Fireballs for fleshmorph and ability to disable spotfx on models,
 * from within the animation
 * 
 * 283   2/02/98 10:07 Philipy
 * played around with model fading functions
 * 
 * 282   30/01/98 17:13 Collinsd
 * 
 * 281   30/01/98 14:58 Collinsd
 * 
 * 280   30/01/98 14:45 Collinsd
 * Shockwave doesn't damage enemies who's shockwave it is.
 * 
 * 279   29/01/98 20:20 Oliverc
 * Disabled Tom Cruise (flaming death biker) for both SHAREWARE and
 * SELF_PLAY versions
 * 
 * 278   28/01/98 9:04 Collinsd
 * Done more work on Afterburners.
 * 
 * 277   22/01/98 22:41 Oliverc
 * Disabled non-SHAREWARE bikes from title models in SHAREWARE version
 * 
 * 276   22/01/98 15:01 Collinsd
 * Added Optimisations.
 * 
 * 275   1/22/98 12:06p Phillipd
 * 
 * 274   22/01/98 11:57 Collinsd
 * Added DoDamage override invul.
 * 
 * 273   20/01/98 16:59 Collinsd
 * 
 * 272   20/01/98 16:34 Collinsd
 * Added SpotFX to bikes.
 * 
 * 271   19/01/98 12:54 Collinsd
 * Added Golden Idol.
 * 
 * 270   1/19/98 12:19p Phillipd
 * Lights now have a visible link list.....
 * 
 * 269   17/01/98 16:11 Collinsd
 * 
 * 268   1/17/98 4:02p Phillipd
 * 
 * 267   17/01/98 12:46 Collinsd
 * Hopefully fixed seconday link list corrupt error.
 * 
 * 266   1/16/98 2:54p Phillipd
 * 
 * 265   15/01/98 17:02 Philipy
 * changed PlayPannedSfx calls to use float for freq
 * 
 * 264   14/01/98 10:12 Collinsd
 * Add logfile and batchfile works again.
 * 
 * 263   13/01/98 17:37 Collinsd
 * Added back pyrolite fuel,  Changes secondary to try to get rid of any
 * chance of any link list corruption.
 * 
 * 262   13/01/98 14:16 Collinsd
 * 
 * 261   13/01/98 14:14 Collinsd
 * 
 * 260   12/01/98 16:51 Collinsd
 * 
 * 259   12/01/98 16:50 Collinsd
 * Added Sfx
 * 
 * 258   10/01/98 13:03 Collinsd
 * Added bomb pickup and change collectpickup.
 * 
 * 257   6/01/98 15:39 Collinsd
 * Taken out bike stuff for shareware
 * 
 * 256   5/01/98 16:41 Collinsd
 * 
 * 255   1/05/98 3:18p Phillipd
 * More sfx...
 * 
 * 254   5/01/98 10:37 Philipy
 * speech sfx implemented - currently defaults to 1 biker & computer only,
 * none selectable
 * 
 * 253   31/12/97 11:53 Collinsd
 * Added destroyatend explosion, and did a crap burn for enemies (
 * disabled )
 * 
 * 252   30/12/97 10:45 Collinsd
 * Added enemy damage modifier as variable.
 * 
 * 251   29/12/97 20:19 Collinsd
 * Added enemy damage modifier.
 * 
 * 250   12/19/97 11:14a Phillipd
 * 
 * 249   19/12/97 9:39 Collinsd
 * Fixed model trans colours. ( orig verts ).
 * 
 * 248   12/18/97 5:37p Phillipd
 * 
 * 247   18/12/97 15:34 Collinsd
 * Fixed Restart Points
 * 
 * 246   12/18/97 2:46p Phillipd
 * 
 * 245   12/18/97 12:08p Phillipd
 * 
 * 244   18/12/97 11:31 Collinsd
 * Added Restart Points, Changed Oneoff anim to only activate if not
 * already animating.
 * 
 * 243   15/12/97 18:01 Collinsd
 * Added stealth and changes speed of legz.
 * 
 * 242   13/12/97 19:01 Collinsd
 * More stuff on bike spotfx.
 * 
 * 241   12/12/97 17:43 Collinsd
 * Started Beard afterburner.
 * 
 * 240   11/12/97 15:53 Collinsd
 * Fixed enemy mines that didn't hurt you.  Also fixed enemy homing
 * missile.
 * 
 * 239   9/12/97 10:45 Collinsd
 * Done ship burning and change body parts and pickups now explode in
 * direction of ship movement.
 * 
 * 238   12/06/97 2:53p Phillipd
 * Fixed Phils Sfx Crash Bug....Doh
 * 
 * 237   5/12/97 16:35 Collinsd
 * Laser now updated to work with new nme code.
 * 
 * 236   5/12/97 14:11 Collinsd
 * Allocated ships fixed.
 * 
 * 235   4/12/97 15:11 Collinsd
 * Ships are now allocated models.
 * 
 * 234   3/12/97 16:28 Philipy
 * removed org_color from MX header group structure
 * 
 * 233   1/12/97 11:45 Collinsd
 * Fixed model loading.
 * 
 * 232   28/11/97 19:01 Collinsd
 * Fixed nmelightning, fixed mines not owned by player, fixed targeting of
 * non player missiles.
 * 
 * 231   27/11/97 12:32 Collinsd
 * All spotfx timing now work again. Rock and Glass models added.
 * 
 * 230   26/11/97 11:02 Collinsd
 * Enemies firing primary weapons now works again.
 * 
 * 229   25/11/97 21:29 Oliverc
 * Dave fixed bug in model freeing
 * 
 * 228   25/11/97 10:36 Collinsd
 * SpotFX on models modifications.
 * 
 * 227   24/11/97 20:04 Collinsd
 * Lasers on models now work properly.  New Primary weapon code.  Looping
 * Anims dont reset when shot.
 * 
 * 226   21/11/97 12:26 Collinsd
 * Added invulnerability to enemies and -1 on spotfx now works.
 * 
 * 225   21/11/97 9:09 Collinsd
 * Taken out all models not used.
 * 
 * 224   19/11/97 9:42 Collinsd
 * Added new level specific loading.
 * 
 * 223   18/11/97 11:50 Collinsd
 * Added Drip and Fixed Enemy damage calculation.
 * 
 * 222   15/11/97 15:20 Collinsd
 * Added Bubbles
 * 
 * 221   14/11/97 8:35 Collinsd
 * Added goldbar model. ( Collect 10 and get extra life. )
 * 
 * 220   13/11/97 17:30 Collinsd
 * Used ANIM_SECOND instead of 60.0F
 * 
 * 219   11/11/97 8:27 Collinsd
 * Gravgon's now effect enemies.
 * 
 * 218   7/11/97 17:56 Collinsd
 * Done some changes to clipgroups.
 * 
 * 217   7/11/97 9:24 Collinsd
 * Added point in group bounding box function
 * 
 * 216   6/11/97 19:24 Collinsd
 * Fix for spotfx moving through groups.
 * 
 * 215   6/11/97 19:03 Collinsd
 * Find Overlapping Visible Groups function Added.
 * 
 * 214   6/11/97 15:26 Collinsd
 * SpotFX on models now have initial delay.  
 * 
 * 213   6/11/97 11:37 Collinsd
 * Added BGObject Lighting, Stopped primary weapons creating lines when
 * outside group and made secondary weapons create debug lines when no
 * collision.
 * 
 * 212   5/11/97 10:52 Collinsd
 * Started work on crushing bgobjects.
 * 
 * 211   3/11/97 18:27 Collinsd
 * Added flames and shrapnel explosions.  ( Also effect Water ).
 * 
 * 210   30/10/96 10:07 Collinsd
 * Clipping of large objects better.
 * 
 * 209   29/10/97 9:26 Collinsd
 * Added new spotfx and pointdir stuff.
 * 
 * 208   25/10/97 11:18 Collinsd
 * You can now pickup ammo when you have supernasram weapon
 * 
 * 207   24/10/97 17:07 Collinsd
 * Pickups no longer dissapear in single player.
 * 
 * 206   24/10/97 10:01 Collinsd
 * Added display of all zones/forces.
 * 
 * 205   23/10/97 13:52 Collinsd
 * Added code to enable/disable compilation of software version.
 * SOFTWARE_ENABLE & softblit.lib.
 * 
 * 204   16/10/97 17:55 Collinsd
 * Added new pointdir effect for models.
 * 
 * 203   15/10/97 19:06 Collinsd
 * Fixed guts comming out bug. and fixed mutually visible group bug.
 * 
 * 202   15/10/97 9:37 Collinsd
 * Added logfile/batchfile creation code.
 * 
 * 201   11/10/97 17:02 Collinsd
 * Added more point/Vector for model stuff. Changed NME Lightning and
 * added fire primary to spotfx.
 * 
 * 200   9/10/97 11:33 Collinsd
 * Added new spotfx and enemies have ambient lights.
 * 
 * 199   4/10/97 15:58 Collinsd
 * Added new spiraling missile and took out old morph code.
 * 
 * 198   4/10/97 12:53 Collinsd
 * Fixed level change. ( Preload Enemies ).
 * 
 * 197   29/09/97 11:35 Collinsd
 * New bgobject stuff enabled. Pickups/Secondary and Enemies now have
 * group link lists.  Flags for secondary bullets being targeted and hit
 * now added.
 * 
 * 196   24/09/97 16:55 Collinsd
 * Added new bgobject stuff.  Added bounding box calc and display code.
 * and changes bgobject state changes.
 * 
 * 195   9/18/97 3:56p Phillipd
 * Fixed stats bug....
 * 
 * 194   17-09-97 4:07p Philipy
 * enabled dynamic loading of t-pages for mxa models
 * 
 * 193   9/05/97 4:45p Phillipd
 * 
 * 192   2/09/97 10:15 Collinsd
 * Fixed killed by enemy messages.
 * 
 * 191   2/09/97 8:54 Collinsd
 * Triggered enemies now works again.  Rotated hunter enemy
 * 
 * 190   19/08/97 11:57 Collinsd
 * Updated flame guy.
 * 
 * 189   18/08/97 10:01 Collinsd
 * Shockwaves now hurt enemies.
 * 
 * 188   15/08/97 16:12 Collinsd
 * Started bgobjects moving ships.  External Forces bug now fixed.
 * 
 * 187   13/08/97 15:09 Collinsd
 * Changed framelag trails.
 * 
 * 186   13/08/97 10:30 Oliverc
 * Disabled "Tom Cruise" from SELF_PLAY mode
 * 
 * 185   12/08/97 18:11 Collinsd
 * Conditional tom cruise
 * 
 * 184   12/08/97 15:52 Collinsd
 * Changed a few more things to do with visible groups.
 * 
 * 183   11/08/97 17:21 Collinsd
 * Added visible group checks.
 * 
 * 182   11/08/97 10:12 Collinsd
 * Added override data directory option. ( SFX don't work yet! )
 * 
 * 181   7/08/97 21:25 Collinsd
 * Change FmPolys to use tpage link list.
 * 
 * 180   6/08/97 19:21 Collinsd
 * Changed external/internal forces. Commented out some more A3D Sfx stuff
 * 
 * 179   4/08/97 15:36 Collinsd
 * Fixed trans for models.
 * 
 * 178   31/07/97 10:59 Collinsd
 * Changed model modify buffer.
 * 
 * 177   24/07/97 17:08 Collinsd
 * Added slick
 * 
 * 176   22-07-97 3:55p Philipy
 * change MODFUNC_ScaleXYZ to include rotation vector
 * 
 * 175   21/07/97 15:39 Collinsd
 * Changed primary/Secondary bullets to work from enemies.
 * 
 * 174   17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 173   16/07/97 20:45 Collinsd
 * CompObjects now use Olly's UV/RGB interpolation
 * 
 * 172   16-07-97 4:36p Philipy
 * added cerbero to title models
 * 
 * 171   16-07-97 4:10p Philipy
 * added bikes to title screen models
 * 
 * 170   16/07/97 12:34 Collinsd
 * Adder cerbero bike
 * 
 * 169   14-07-97 2:56p Philipy
 * fixed call to ExecuteMxaloadHeader, inside ModelDisp so that mxa
 * translucencies work.
 * 
 * 168   7/12/97 3:34p Phillipd
 * 
 * 167   10-07-97 6:04p Philipy
 * changed order of title models
 * 
 * 166   9/07/97 16:59 Collinsd
 * Fixed MXA models in titles
 * 
 * 165   7/09/97 3:13p Phillipd
 * Ambient Light version 1
 * 
 * 164   7/09/97 2:31p Phillipd
 * 
 * 163   9-07-97 2:20p Philipy
 * added StoreTriangles flag to MODELNAME structure
 * 
 * 162   9-07-97 11:38a Philipy
 * added test bike to title models
 * 
 * 161   8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 160   8/07/97 14:06 Collinsd
 * Added componented objects to enemies.
 * 
 * 159   5/07/97 16:31 Collinsd
 * Put OPT_ON's around opimisations off
 * 
 * 158   3/07/97 10:49 Collinsd
 * Merged MXA and MX models and added morphing objects.
 * 
 * 157   1/07/97 16:05 Collinsd
 * Added mofisto bike
 * 
 * 156   6/24/97 5:10p Phillipd
 * 
 * 155   6/24/97 11:12a Phillipd
 * 
 * 154   20/06/97 8:50 Collinsd
 * Added more error messages.
 * 
 * 153   19-06-97 3:49p Philipy
 * added extra models for title room
 * 
 * 152   18/06/97 17:35 Collinsd
 * Added PowerVR Trans
 * 
 * 151   10/06/97 14:28 Collinsd
 * Added ACTUAL_TRANS
 * 
 * 150   8/06/97 17:07 Collinsd
 * Done more work on BGObjects/Doors
 * 
 * 149   5/06/97 12:40 Collinsd
 * Reduced texture pages.
 * 
 * 148   5/23/97 9:18a Phillipd
 * sfx now distance and connected atten
 * 
 * 147   19-05-97 8:20p Philipy
 * Changed order of title screen models
 * 
 * 146   18/05/97 21:48 Collinsd
 * 
 * 145   9-05-97 6:25p Philipy
 * changed order of title screen models (again)
 * 
 * 144   9-05-97 5:46p Philipy
 * changed order of .mx files for title screen models
 * 
 * 143   5/09/97 5:44p Phillipd
 * LOD now stored with mxloadheader..
 * 
 * 142   8-05-97 12:02p Philipy
 * changed names of .mx files for title room
 * 
 * 141   7/05/97 9:55 Collinsd
 * Fixed reloading of title models.
 * 
 * 140   6-05-97 7:12p Philipy
 * 
 * 139   6-05-97 11:58a Philipy
 * Added remaining title room models
 * 
 * 138   1/05/97 15:22 Collinsd
 * Taken out man flying of bike.
 * 
 * 137   4/30/97 5:39p Phillipd
 * 
 * 136   4/30/97 4:34p Phillipd
 * 
 * 135   30-04-97 3:22p Philipy
 * 
 * 134   4/30/97 11:23a Phillipd
 * 
 * 133   26/04/97 14:49 Collinsd
 * Optimisations now on def.
 * 
 * 132   25/04/97 17:13 Collinsd
 * Added new bike
 * 
 * 131   23/04/97 10:37 Collinsd
 * taken out tom cruise
 * 
 * 130   22-04-97 3:25p Collinsd
 * Added player flying out of bilke.
 * 
 * 129   18-04-97 9:03a Collinsd
 * Not checking debug mode, using invul
 * 
 * 128   17-04-97 6:00p Collinsd
 * Models now use processexecmodel()
 * 
 * 127   4/07/97 12:27p Phillipd
 * 
 * 126   24-03-97 12:54p Collinsd
 * changed detail level of explosion.  Changed process model function
 * 
 * 125   20-03-97 10:00a Collinsd
 * new sfx
 * 
 * 124   19-03-97 4:12p Collinsd
 * added invul
 * 
 * 123   19-03-97 12:45p Collinsd
 * 
 * 122   19-03-97 11:30a Collinsd
 * 
 * 121   13-03-97 5:45p Collinsd
 * Added auto detail level to trails.
 * 
 * 120   13-03-97 11:47a Collinsd
 * polys/fmpolys only generated when needed.
 * 
 * 119   3/13/97 9:50a Phillipd
 * 
 * 118   12-03-97 4:47p Collinsd
 * Added recoil and new shrapnel model.
 * 
 * 117   12-03-97 12:40p Collinsd
 * Added new body parts.
 * 
 * 116   11-03-97 5:41p Collinsd
 * 
 * 115   5-03-97 5:00p Collinsd
 * 
 * 114   5-03-97 10:49a Collinsd
 * 
 * 113   4-03-97 6:01p Collinsd
 * Added fog to tracker.
 * 
 * 112   4-03-97 4:50p Collinsd
 * Added Tracker.
 * 
 * 111   27-02-97 2:08p Collinsd
 * Added optimisation to various files.
 * 
 * 110   25-02-97 11:08a Collinsd
 * Added group bounding box check for body parts.
 * 
 * 109   25-02-97 10:20a Collinsd
 * Corrected framelag on various bits.
 * 
 * 108   24-02-97 12:07p Collinsd
 * Gravgons no longer effect camera in demo mode.
 * 
 * 107   24-02-97 11:21a Collinsd
 * Added gravgon check for restart. and added bounding on object selection
 * depending on mode.
 * 
 * 106   24-02-97 10:23a Collinsd
 * Added component bgobjects.
 * 
 * 105   11-02-97 5:08p Collinsd
 * Triggers/RegenPoints and pickups now are sent across correctly.
 * 
 * 104   4-02-97 4:29p Collinsd
 * 
 * 103   31-01-97 4:08p Collinsd
 * Body Parts/Blood now use gravity vectors.
 * 
 * 102   21-01-97 5:34p Collinsd
 * Added the ability to delete bgobjects.
 * 
 * 101   13-01-97 12:19p Collinsd
 * Doors now cannot be opened by various rays.
 * 
 * 100   10-01-97 3:54p Collinsd
 * Missile/Primary weapons no longer open doors before collision.
 * 
 * 99    10-01-97 12:34p Collinsd
 * Added Doors, Chanded pickups/door/mine dropping.
 * 
 * 98    6-01-97 9:06a Collinsd
 * Added drop ammo/shield options.
 * Started working on titan start missile.
 * 
 * 97    3-01-97 3:28p Collinsd
 * Added xxx quantum/pine/purged mined xxx messages.
 * Trojax charging sfx on others pc's now fixed.
 * 
 * 96    2-01-97 5:25p Collinsd
 * Added extra model for stealthed orbit pulsars.
 * 
 * 95    2-01-97 4:22p Collinsd
 * Stealthed orbit pulsars now works.  Orbit pulsar and stealth mantle
 * regeneration bug fixed.
 * 
 * 94    2-01-97 12:19p Collinsd
 * Network game now works again.
 * Orbit pulsar now works over network game.
 * 
 * 93    31-12-96 12:35p Collinsd
 * Added multiple multiples.
 * 
 * 92    30-12-96 3:50p Collinsd
 * Added orbit pulsar.
 * 
 * 91    12/27/96 3:38p Phillipd
 * Primary.h Secondary.h pickups.h are now clean....
 * Still Lots to do though.....
 * 
 * 90    12/27/96 12:33p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 89    22-12-96 8:53p Collinsd
 * Added invisible shockwave to all missiles. hitting walls.
 * 
 * 88    21-12-96 7:44p Collinsd
 * Added code to enable lensflare on missiles. ( used scatter as test ).
 * Also fixed bug in shockwaves.
 * 
 * 87    21-12-96 5:24p Collinsd
 * Added drop primary/drop secondary weapons.
 * 
 * 86    19-12-96 3:19p Collinsd
 * Changed sfx funtion to allow frequency changing.
 * Added Trojax Charging SFX.
 * 
 * 85    12/14/96 9:18p Collinsd
 * Gravgon now finishes neatly.
 * 
 * 84    12/07/96 8:42p Collinsd
 * Added Jap Bird bike, Fixed mines being dropped and firing missile at
 * same time bug.  Added rotations when hit ( depending on damage ).
 * 
 * 83    12/04/96 5:02p Collinsd
 * Fixed bug in body part bounce
 * 
 * 82    12/04/96 4:55p Collinsd
 * Fix if body parts get out of map.
 * 
 * 81    12/02/96 1:26p Collinsd
 * No longer use quatfromvector routines, now use quatfrom2vectors.
 * 
 * 80    20/11/96 14:05 Collinsd
 * Added lower detail version of golden powerpod.
 * 
 * 79    18/11/96 15:10 Collinsd
 * Added new borg girl models. Changed dissapper time for scattered
 * weapons.
 * 
 * 78    14/11/96 17:35 Collinsd
 * added borg girl as pickup. ( TEMP )
 * 
 * 77    13/11/96 15:46 Collinsd
 * Added golden powerpod and ljay.
 * 
 * 76    6/11/96 18:42 Collinsd
 * Tidied up secondary weapons.
 * 
 * 75    4/11/96 16:35 Collinsd
 * Fixed fmpolys/polys/lines exec lists.
 * 
 * 74    4/11/96 10:45 Oliverc
 * Changed display routines to clip to visible portal boundaries of each
 * group
 * 
 * 73    31/10/96 20:57 Collinsd
 * fixed gravgon/shockwave.  Stealth mode no longer displays name.
 * 
 * 72    30/10/96 14:34 Collinsd
 * Added stealthmode.
 * 
 * 71    29/10/96 16:00 Collinsd
 * changed over to panned sfx.
 * 
 * 70    27/10/96 18:38 Collinsd
 * Optimised loads of shit.
 * 
 * 69    26/10/96 21:09 Collinsd
 * Optimised shockwave->mine collision.
 * 
 * 68    26/10/96 20:55 Collinsd
 * Shock/Gravity waves now scale model by matrix.
 * 
 * 67    26/10/96 18:04 Collinsd
 * Optimised/Fixed shockwave/gravity waves.
 * 
 * 66    10/25/96 2:07p Phillipd
 * 3d panel tested, test for lost packets
 * 
 * 65    23/10/96 9:59 Collinsd
 * Fixed gravgon bumping.
 * 
 * 64    22/10/96 12:08 Collinsd
 * Added body parts and blood splats.
 * 
 * 63    19/10/96 20:50 Collinsd
 * Changed scatter/thief to give more chance for enemy to pickup up
 * scattered weapons.
 * Reduced thief homing.  Increased Heatseaker.
 * Lights now can be enabled and disabled properly.
 * started on flying body parts.  Plus lots more..... 
 * 
 * 62    18/10/96 17:46 Collinsd
 * Changed all file loading to load from tidy directories.
 * 
 * 61    17/10/96 16:43 Collinsd
 * Taken out redundent models.
 * 
 * 60    17/10/96 14:44 Collinsd
 * Explosion and gravgon effects now work properly.
 * 
 * 59    16/10/96 18:05 Collinsd
 * Gravgon/Explosion now work in other camera views.
 * 
 * 58    16/10/96 12:38 Collinsd
 * Shock waves now affect mines.
 * Fixed bug in ship/mine collision
 * 
 * 57    15/10/96 15:30 Collinsd
 * Added missile/mine view toggled on/off with delete
 * 
 * 56    11/10/96 11:38 Collinsd
 * Added new models
 * 
 * 55    9/10/96 16:12 Collinsd
 * Added regeneration sparkle. And model display flag.
 * 
 * 54    9/10/96 12:28 Collinsd
 * Added code to handle low poly, single direction sphere.
 * 
 * 53    8/10/96 10:22 Collinsd
 * Added lower detail transpulse. and new low detail timer.
 * 
 * 52    7/10/96 16:31 Collinsd
 * Added execsize to mx and mxafiles. Also fixed colour keying on models.
 * 
 * 51    10/04/96 10:32a Phillipd
 * 
 * 50    3/10/96 12:01 Collinsd
 * Added the beard and borg body bits into models.
 * 
 * 49    2/10/96 12:33 Collinsd
 * You can no longer intersect models.
 * 
 * 48    30/09/96 14:48 Collinsd
 * Added simplified missiles.
 * Fixed distance 2dpoly's moved out by...
 * 
 * 47    9/26/96 11:21a Phillipd
 * MX load is now operational...
 * 
 * 46    9/25/96 5:53p Phillipd
 * added Simplified Models......And Bike Detail Level now
 * works..
 * 
 * 45    20/09/96 17:13 Collinsd
 * Changed 2Dpolys and Polys to display only polys in visible groups.
 * Changed secondary ammo to use array.
 * Pickups now use correct index's.  You can now drop a mine using 'B'.
 * 
 * 44    19/09/96 8:51 Collinsd
 * Added simplified new models for missiles
 * 
 * 43    18/09/96 17:36 Collinsd
 * Changed lines to be solid. added neds new models
 * 
 * 42    18/09/96 14:36 Collinsd
 * Fixed primary weapon select. and rotate pickups in all axis
 * 
 * 41    17/09/96 16:30 Collinsd
 * No damage when in limbo mode.
 * 
 * 40    13/09/96 17:04 Collinsd
 * Added quantun fireball mine.
 * 
 * 39    12/09/96 17:52 Collinsd
 * You can now shoot mines ( Primary weapons only though
 * at the moment ).
 * 
 * 38    11/09/96 19:00 Collinsd
 * Added tests for RAMP emulation ( Although may not work)
 * Also fixed gravity effect to be dependent on GLOBAL_SCALE
 * 
 * 37    11/09/96 14:19 Collinsd
 * Added global scale to hopefully everything that needs it.
 * 
 * 36    11/09/96 10:25 Collinsd
 * Added global_scale to typedefs.h
 * 
 * 35    10/09/96 16:33 Oliverc
 * Added external/internal force vector to ship movement
 * 
 * 34    10/09/96 16:07 Collinsd
 * Added gravgon effect. (Not finished)
 * 
 * 33    9/09/96 20:42 Collinsd
 * No longer used GroupPolyCol or WhichGroupImIn.
 * 
 * 32    6/09/96 16:20 Collinsd
 * Added extra life pickup, and started pine mine.
 * 
 * 31    6/09/96 9:13 Collinsd
 * Timer missile works now.
 * 
 * 30    4/09/96 20:01 Collinsd
 * Scatter missile and thief missile now work!
 * 
 * 29    2/09/96 14:18 Collinsd
 * Finished shield effect, started on secondary weapons.
 * 
 * 28    30/08/96 17:30 Collinsd
 * Fixed bug in rgba colours ( Cheers Dan ).
 * 
 * 27    29/08/96 21:05 Collinsd
 * Inproved shield effect.
 * 
 * 26    29/08/96 17:53 Collinsd
 * Lots more pickups and shield working a little better.
 * 
 * 25    28/08/96 15:03 Collinsd
 * Added 5 more pickups and fixed bug model group update.
 * 
 * 24    28/08/96 11:15 Collinsd
 * 
 * 23    23/08/96 17:22 Collinsd
 * Only Display Models in visible groups.
 * 
 * 22    8/23/96 2:54p Phillipd
 * 
 * 21    23/08/96 14:42 Collinsd
 * Added bikes as models
 * 
 * 20    23/08/96 14:20 Collinsd
 * All finished models in.
 * 
 * 19    23/08/96 9:18 Collinsd
 * All weapons have limits. New pickups in but not correct.
 * 
 * 18    16/08/96 11:01 Collinsd
 * You now see explosions close to walls and when hit. Pickups and bullets
 * don't dissapear when changing mode.
 * 
 * 17    15/08/96 9:54 Collinsd
 * Added glow to pickups and started primary wepon powerups
 * 
 * 16    8/13/96 2:43p Phillipd
 * 
 * 15    13/08/96 12:49 Collinsd
 * Working on primary weapons
 * 
 * 14    9/08/96 10:18 Collinsd
 * Added powerlevel to primary weapons.
 * Added a few more pickup models.
 * 
 * 13    7/08/96 10:32 Collinsd
 * Added pickup collision with background
 * 
 * 12    6/08/96 12:39 Collinsd
 * You can now pickup pickups. added 2 new models
 * 
 * 11    8/03/96 4:39p Phillipd
 * all Models can be env mapped..
 * Alpha texture formats are detected and reported...
 * But cant load a system Alpha texture into a Video Alpha Texture!!
 * 
 * 10    31/07/96 16:07 Collinsd
 * Added new pyrolite weapon, added type into bullets/missiles. Fixed bug
 * with 2d polys
 * 
 * 9     7/22/96 5:49p Phillipd
 * 
 * 8     7/21/96 2:29p Phillipd
 * 
 * 7     2/07/96 15:20 Collinsd
 * 
 * 6     28/06/96 17:35 Collinsd
 * 
 * 5     28/06/96 12:48 Collinsd
 * 
 * 4     28/06/96 10:05 Collinsd
 * 
 * 3     6/27/96 4:32p Phillipd
 * 
 * 2     6/27/96 4:04p Phillipd
 * 
 * 1     6/27/96 3:52p Phillipd
 * 
 */

#define FAST_RGB_CLAMP


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
*	m o d e l s . c
*	All routines to do with 3d Model Objects
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#include <stdio.h>
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
#include "triggers.h"
#include "pickups.h"

#include "models.h"

#include "Secondary.h"
#include "Primary.h"
#include "enemies.h"

#include "title.h"
#include "text.h"
#include "lines.h"
#include "2dpolys.h"
#include "polys.h"
#include "spotfx.h"
#include "water.h"
#include "main.h"
#include "controls.h"

#include "feedback.h"
#include "local.h"
#include "xmem.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		Externals ...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
extern VECTOR HoloRot;
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	CAMERA			CurrentCamera;
extern	MXALOADHEADER	MxaModelHeaders[ MAXMXAMODELHEADERS ];

extern	BYTE			Current_Camera_View;		// which object is currently using the camera view....
extern	BYTE			WhoIAm;
extern	GLOBALSHIP		Ships[MAX_PLAYERS];
extern	float			framelag;
extern	XLIGHT * FirstLightVisible;

extern	D3DMATRIXHANDLE	hView;
extern	D3DMATRIX		view;
extern	D3DMATRIX		identity;
extern	D3DMATRIXHANDLE	hWorld;
extern	BOOL			UsedStippledAlpha;
extern	int16			Stats[MAX_PLAYERS+1][MAX_PLAYERS+1];
extern	uint16			IsGroupVisible[MAXGROUPS];
extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;

extern	uint16			FirstSecBullUsed;
extern	SECONDARYWEAPONBULLET SecBulls[MAXSECONDARYWEAPONBULLETS];
extern	int16			SecondaryAmmo[ MAXSECONDARYWEAPONS ];
extern	int16			SecAmmoUsed[ MAXSECONDARYWEAPONS ];

extern	MATRIX			MATRIX_Identity;

extern	XLIGHT			XLights[ MAXXLIGHTS ];
extern	uint16			FirstXLightUsed;
extern	uint16			FirstXLightFree;
extern	PICKUP			Pickups[ MAXPICKUPS ];

extern	int16			NumPickupsPerGroup[ MAXGROUPS ];
extern	SLIDER			TrailDetailSlider;
extern	BOOL			AutoDetail;
extern	float			avgframelag;
extern	MODELNAME		TitleModelNames[MAXMODELHEADERS];
extern	BOOL			PowerVR;
extern	float			SoundInfo[MAXGROUPS][MAXGROUPS];
extern	ENEMY	*		FirstEnemyUsed;
extern	LINE			Lines[ MAXLINES ];
extern	FMPOLY			FmPolys[MAXNUMOF2DPOLYS];
extern	POLY			Polys[MAXPOLYS];
extern	FRAME_INFO	*	Flare_Header;
extern	PRIMARYWEAPONBULLET	PrimBulls[MAXPRIMARYWEAPONBULLETS];
extern	uint16			GlobalPrimBullsID;
extern	uint16			GlobalSecBullsID;
extern	int16			BikeModels[ MAXBIKETYPES ];
extern	BYTE			GameStatus[MAX_PLAYERS];	// Game Status for every Ship...
extern	BOOL			IsHost;
extern	float			NmeDamageModifier;
extern	int32			ColPerspective;
extern	BOOL			TeamGame;
extern	BYTE			TeamNumber[MAX_PLAYERS];
extern	SHORTNAMETYPE	Names;	// all the players short Names....
extern BOOL BikeExpanded;
extern	ENEMY			Enemies[ MAXENEMIES ];
extern	BIKEINFO		BikeCompFiles[ MAXBIKETYPES ];
extern	BOOL			BikeExhausts;
extern	int16			CameraRendering;
extern	BOOL			PlayDemo;
extern	BYTE			ChangeLevel_MyGameStatus;
extern	BOOL			CaptureTheFlag;
extern	BOOL			CTF;
extern	BOOL			BikeEnginesOn;
extern	BOOL                    IsServer;
extern	BYTE	Server_WhoHitYou;

void DebugPrintf( const char * format, ... );
void RefreshModel( uint16 model );

#define BALL_RADIUS	( 256 * GLOBAL_SCALE )
#define	NUMBIKEDETAILS	4

#ifdef SOFTWARE_ENABLE
extern	BOOL	SoftwareVersion;
#endif

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Globals
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void CheckModelLinkList( void );
void UpdateTracker( void );
void CreateTracker( void );
uint16	Tracker = (uint16) -1;
uint16	TrackerTarget = (uint16) -1;
MODEL	Models[MAXNUMOFMODELS];
uint16	FirstModelUsed;
uint16	FirstModelFree;
int16	NextNewModel = -1;
BOOL	ShowBoundingBoxes = FALSE;

//int8	*	ModelPath = "data\\models\\";
int8	*	ModelPath = "models\\";

MXLOADHEADER ModelHeaders[MAXMODELHEADERS];

#define	RETAIN_POLYS		TRUE
#define	DISCARD_POLYS		FALSE
void GetRealLightAmbient( VECTOR * Pos , float * Red , float * Green , float * Blue );
BOOL	ENVMxa( MXALOADHEADER * Mxaloadheader , MATRIX * Mat ,VECTOR * Pos);

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	In game models
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
MODELNAME	ModelNames[MAXMODELHEADERS] ={	

	{ "heat.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Heatseaker missile
	{ "fheat.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
	{ "grav.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Gravgon
	{ "fgrav.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
	{ "troj.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Trojax
	{ "stroj.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
	{ "prlt.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Pyrolyte
	{ "sprlt.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
	{ "trans.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Transpulse
	{ "strans.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
	{ "sus.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Suss Gun
	{ "ssus.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// LOD 1..
	{ "beam.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Beam Laser
	{ "sbeam.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
	{ "pod.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// powerpod..
	{ "spod.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// LOD 1..
	{ "fuel.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Fuel				
//	{ "sfuel.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..				
	{ "shld.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Shield
	{ "sshld.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
	{ "ammo.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Suss Gun
	{ "sammo.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
	{ "nrg.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Energy
	{ "snrg.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// LOD 1..
	{ "mug.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Mug Missile
	{ "fmug.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// LOD 1..
	{ "mugs.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Mug pickup
	{ "smugs.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "sctr.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Scatter Missile
	{ "fsctr.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
//	{ "thef.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Thief Missile
//	{ "fthef.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "titan.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Timer Missile
	{ "titan.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "lnch.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Rocket Launcher
	{ "slnch.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "comp.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Computer Targetting
	{ "scomp.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "pin.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Pine Mine
	{ "spin.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// LOD 1 ..
	{ "pine.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Pine Pickup
	{ "spine.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "qpod.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Quantum Pickup
	{ "sqpod.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "qpik.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Quantum Mine
	{ "sqpik.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "prge.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Purge Mine
	{ "sprge.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
//	{ "spid.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Spider Mine
//	{ "sspid.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "heatpk.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Heatseaker
	{ "sheatpk.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
//	{ "smok.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Smoke Streamer
//	{ "ssmok.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "prgpod.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Purge Pickup
	{ "sprgpod.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
//	{ "para.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Parasite Mine
//	{ "spara.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "ntro.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Nitro
	{ "sntro.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1
//	{ "gogl.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Goggles
//	{ "sgogl.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "vuln.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Invulnerability
	{ "svuln.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "gold.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "mant.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Stealth Mantle
	{ "smant.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
//	{ "flar.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Flare Pod
//	{ "sflar.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
//	{ "spdpod.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Spider Pod
//	{ "sspdpod.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "crys.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "orb.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Orbital Pulsar
	{ "sorb.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// LOD 1 ..
	{ "xtra.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Extra Life
	{ "sxtra.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1 ..
	{ "fpin.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "ball1.mx" , 2 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Ball
	{ "ball2.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
	{ "ball3.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 2..
//	{ "chaf.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Chaff
//	{ "schaf.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
//	{ "pnlmesh.mx" , 0 , TRUE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// the Panel....
	{ "gpod.mx" , 1 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Golden Powerpod
	{ "sgldpod.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1..
	{ "orb2.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Orbit pulsar
	{ "orb3.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Orbit pulsar ( Light Model )
	{ "gren.mx" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Titan Star Shrapnel
	{ "tracker.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Ian's Tracker
	{ "ping.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Ian's ping
	{ "brain.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Daniel's Brain
	{ "ear.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Daniel's Ear
	{ "eyeball.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Daniel's Eyeball
	{ "heart.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Daniel's Heart
	{ "kidney.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Daniel's Kidney
	{ "leg.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Daniel's Leg
	{ "shockwave.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Explosion Donut
	{ "nmemisil.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// NME Missile
	{ "dna.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// DNA Pickup
	{ "key.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Skeleton Key Pickup
	{ "shrap1.mx", 1, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Shrapnel
	{ "lshrap1.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// LOD 1
	{ "shrap2.mx", 1, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Shrapnel2
	{ "lshrap2.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// LOD 1
	{ "glass.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Shrapnel
	{ "rock1.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// LOD 1
	{ "rock2.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Shrapnel2
	{ "bomb.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Bomb Pickup
	{ "goldfig.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Gold Figure
	{ "flagmrphwave000.mxa" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD }, //Flag
	{ "redflagwave000.mxa" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD }, // Red Flag
	{ "greenflagwave000.mxa" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD }, //Green Flag
	{ "blueflagwave000.mxa" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD }, //Blue Flag
	{ "yellowflagwave000.mxa" , 0 , FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD }, //Yellow Flag

#if !defined( SELF_PLAY ) && !defined( SHAREWARE ) && !defined ( WIN98SHAREWARE ) && !defined ( GATEWAY )
	{ "tc-01.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Tom-Cruise Anim
	{ "tc-02.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-03.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-04.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-05.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-06.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-07.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-08.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-09.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-10.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-11.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-12.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-13.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-14.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-15.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-16.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-17.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-18.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-19.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-20.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-21.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-22.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-23.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-24.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-25.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-26.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "tc-27.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif

#ifdef SHADOWTEST

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	{ "borg400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "borg300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "borg200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "borg100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "borg050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sborg400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sborg300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sborg200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sborg100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sborg050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif
	{ "brd400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "brd300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "brd200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "brd100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "brd050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sbrd400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sbrd300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sbrd200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sbrd100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sbrd050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	{ "laj400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "laj300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "laj200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "laj100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "laj050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slaj400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "slaj300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slaj200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slaj100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slaj050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif

	{ "xcop400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "xcop300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "xcop200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "xcop100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "xcop050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sxcop400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sxcop300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sxcop200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sxcop100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sxcop050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "truk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "truk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "truk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "truk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "truk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "struk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "struk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "struk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "struk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "struk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	{ "foet400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "foet300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "foet200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "foet100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "foet050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sfoet400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sfoet300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sfoet200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sfoet100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sfoet050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "jap400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "jap300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jap200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jap100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jap050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjap400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sjap300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjap200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjap100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjap050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "nut400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "nut300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nut200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nut100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nut050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snut400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "snut300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snut200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snut100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snut050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "rhes400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "rhes300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "rhes200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "rhes100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "rhes050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "srhes400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "srhes300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "srhes200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "srhes100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "srhes050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "jo400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Unstealthed
	{ "jo300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jo200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jo100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jo050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjo400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sjo300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjo200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjo100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjo050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "shrk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "shrk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shrk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shrk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shrk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sshrk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sshrk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sshrk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sshrk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sshrk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "hk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Unstealthed
	{ "hk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "hk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "hk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "hk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "shk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif
	{ "nbia400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "nbia300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nbia200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nbia100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nbia050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snbia400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "snbia300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snbia200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snbia100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snbia050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	{ "mofi400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "mofi300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "mofi200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "mofi100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "mofi050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "smofi400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "smofi300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "smofi200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "smofi100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "smofi050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif

	{ "cerb400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "cerb300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "cerb200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "cerb100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "cerb050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "scerb400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "scerb300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "scerb200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "scerb100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "scerb050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	{ "slick400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "slick300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slick200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slick100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slick050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sslick400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sslick300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sslick200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sslick100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sslick050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif

#else

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	{ "borg400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "borg300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "borg200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "borg100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "borg050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sborg400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sborg300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sborg200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sborg100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sborg050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif
	{ "brd400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "brd300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "brd200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "brd100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "brd050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sbrd400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sbrd300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sbrd200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sbrd100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sbrd050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	{ "laj400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "laj300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "laj200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "laj100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "laj050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slaj400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "slaj300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slaj200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slaj100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slaj050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif

	{ "xcop400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "xcop300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "xcop200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "xcop100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "xcop050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sxcop400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sxcop300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sxcop200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sxcop100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sxcop050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "truk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "truk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "truk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "truk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "truk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "struk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "struk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "struk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "struk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "struk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	{ "foet400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "foet300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "foet200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "foet100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "foet050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sfoet400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sfoet300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sfoet200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sfoet100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sfoet050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "jap400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "jap300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jap200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jap100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jap050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjap400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sjap300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjap200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjap100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjap050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "nut400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "nut300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nut200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nut100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nut050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snut400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "snut300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snut200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snut100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snut050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "rhes400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "rhes300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "rhes200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "rhes100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "rhes050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "srhes400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "srhes300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "srhes200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "srhes100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "srhes050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "jo400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Unstealthed
	{ "jo300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jo200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jo100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "jo050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjo400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sjo300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjo200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjo100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sjo050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "shrk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "shrk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shrk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shrk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shrk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sshrk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sshrk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sshrk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sshrk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sshrk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

	{ "hk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Unstealthed
	{ "hk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "hk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "hk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "hk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shk400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "shk300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shk200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shk100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "shk050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif
	{ "nbia400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "nbia300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nbia200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nbia100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "nbia050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snbia400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "snbia300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snbia200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snbia100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "snbia050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	{ "mofi400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "mofi300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "mofi200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "mofi100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "mofi050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "smofi400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "smofi300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "smofi200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "smofi100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "smofi050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif

	{ "cerb400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "cerb300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "cerb200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "cerb100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "cerb050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "scerb400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "scerb300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "scerb200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "scerb100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "scerb050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },

#if !defined ( SHAREWARE ) && !defined ( WIN98SHAREWARE )
	{ "slick400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Unstealthed
	{ "slick300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slick200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slick100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slick050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sslick400.mxa", 4, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Stealthed
	{ "sslick300.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sslick200.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sslick100.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "sslick050.mxa", 0, FALSE, MODELTYPE_Morphing, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif

#endif

	{ "" , 0 , FALSE, MODELTYPE_Static, 0, FALSE, DONTLOAD_TPAGES, NOT_LEVEL_SPECIFIC, DO_LOAD }
};

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Title screen models
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
MODELNAME *TitleModelSet;

MODELNAME	TitleModelNames[ MAXMODELHEADERS ] ={	
	{ "disc1.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 1
	{ "disc2.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 2
	{ "disc3.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 3
	{ "disc4.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 4
	{ "disc5.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 5
	{ "disc6.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 6
	{ "mbot.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Bot. of mech. device
	{ "menuroom.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// main title screen room
	{ "vdu.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// VDU
	{ "holopad.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Holopad
	{ "mtop.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Top of mech. device
	{ "menutvdummy.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// TV screen
	{ "menutv.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// TV screen
	{ "hglaire.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// translucent glare effect for holopad
	{ "bracket.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// VDU bracket

#if defined( SHAREWARE ) || defined( WIN98SHAREWARE )
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM | LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "xcop400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "truk400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "nbia400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
#else
	{ "borg400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM | LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "brd400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "laj400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "xcop400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "truk400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "foet400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "jap400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "nut400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "rhes400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "jo400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			
	{ "shrk400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "hk400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			
	{ "nbia400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "mofi400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		
	{ "cerb400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "slick400.mxa", 0, FALSE, MODELTYPE_Morphing, 0, RETAIN_POLYS,  LOAD_TPAGES_SYSMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
#endif
	{ "" , 0 , FALSE, MODELTYPE_Static, 0, FALSE, DONTLOAD_TPAGES, NOT_LEVEL_SPECIFIC, DO_LOAD }
};

MODELNAME	InterLevelModelNames[ NUM_INTERLEVEL_MODELS + 1 ] ={
	{ "disc1.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 1
	{ "disc2.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 2
	{ "disc3.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 3
	{ "disc4.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 4
	{ "disc5.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 5
	{ "disc6.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Disc 6
	{ "mbot.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Bot. of mech. device
	{ "menuroom.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// main title screen room
	{ "vdu.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// VDU
	{ "holopad.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// Holopad
	{ "mtop.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// Top of mech. device
	{ "menutvdummy.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// TV screen
	{ "menutv.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },			// TV screen
	{ "hglaire.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// translucent glare effect for holopad
	{ "bracket.mx", 0, FALSE, MODELTYPE_Static, 0, DISCARD_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },		// VDU bracket

	{ "crys.mx" , 0 , FALSE, MODELTYPE_Static, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "gold.mx" , 0 , FALSE, MODELTYPE_Static, 0, RETAIN_POLYS,  LOAD_TPAGES_VIDMEM, NOT_LEVEL_SPECIFIC, DO_LOAD },
	{ "" , 0 , FALSE, MODELTYPE_Static, 0, FALSE, DONTLOAD_TPAGES, NOT_LEVEL_SPECIFIC, DO_LOAD }
};

MODELNAME	SplashModelNames[ MAXMODELHEADERS ] ={
	{ "" , 0 , FALSE, MODELTYPE_Static, 0, FALSE, DONTLOAD_TPAGES, NOT_LEVEL_SPECIFIC, DO_LOAD },
};



/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
*		Set up 2d exec buff etc...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void OnceOnlyInitModel( void )
{
	int i;
	int	Count;

	FirstModelUsed = (uint16) -1;
	FirstModelFree = 0;
	
	for( i=0;i<MAXNUMOFMODELS;i++)
	{
		memset( &Models[i], 0, sizeof( MODEL ) );
		Models[i].Func = MODFUNC_Nothing;
		Models[i].LifeCount = -1.0F;
		Models[i].Scale = 1.0F;
		Models[i].Visible = TRUE;
		Models[i].TimeInterval = (float) ( 11 - TrailDetailSlider.value );

		for( Count = 0; Count < 12; Count++ ) Models[i].TempLines[ Count ] = (uint16) -1;

		Models[i].Next = i + 1;
		Models[i].Prev = (uint16) -1;

		if( AutoDetail )
		{
			if( avgframelag > 2.0F ) Models[i].TimeInterval +=  ( avgframelag - 1.0F );
		}

	}

	Models[MAXNUMOFMODELS-1].Next = (uint16) -1;

	NextNewModel = MODEL_ExtraModels;

	for( i = MODEL_ExtraModels; i < MAXMODELHEADERS ; i++ )
	{
		memset( &ModelNames[ i ], 0, sizeof( MODELNAME ) );
	}

	for( i = TITLE_MODEL_ExtraModels; i < MAXMODELHEADERS ; i++ )
	{
		memset( &TitleModelNames[ i ], 0, sizeof( MODELNAME ) );
	}

	for( i = 0; i < MAXMODELHEADERS ; i++ )
	{
		memset( &SplashModelNames[ i ], 0, sizeof( MODELNAME ) );
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Pre-Init models
	Input		:	LPDIRECT3DDEVICE	Direct 3D Device
				:	MODELNAME	*		Current Model Name
	Output		:	BOOL				TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL PreInitModel( LPDIRECT3DDEVICE lpDev , MODELNAME *	NamePnt )
{
	int			i;
	int8		TempFilename[ 256 ];
	int8		Ext[ 32 ];

	AddCommentToBat( "Models" );

	for( i = 0 ; i < MAXMODELHEADERS ; i++ )
	{
		if( i == MODEL_ExtraModels ) AddCommentToBat( "Level Models" );

		if( NamePnt->Name[0] )
		{
			if( NamePnt->LoadEnable )
			{
				if( NamePnt->LevelSpecific )
				{
					sprintf( &TempFilename[0], "levels\\%s\\models\\%s", &ShortLevelNames[ LevelNum ][ 0 ], NamePnt->Name );
				}
				else
				{
					sprintf( &TempFilename[0], "models\\%s", NamePnt->Name );
				}

				Get_Ext( NamePnt->Name, &Ext[ 0 ] );

				if( !_stricmp( &Ext[ 0 ], "MX" ) )
				{
					NamePnt->DoIMorph = FALSE;

//					DebugPrintf( "PreLoading MX File %s\n", &TempFilename[ 0 ] );

					if( !PreMxload( &TempFilename[0], &ModelHeaders[i] , NamePnt->Panel, NamePnt->LevelSpecific ) ) return FALSE;	// the model and visipoly data
				}
				else
				{
					if( !_stricmp( &Ext[ 0 ], "MXA" ) )
					{
						NamePnt->DoIMorph = TRUE;

//						DebugPrintf( "PreLoading MXA File %s\n", &TempFilename[ 0 ] );

						MxaModelHeaders[i].AllocateTPage = NamePnt->AllocateTpage;

						if( !PreMxaload( &TempFilename[0], MxaModelHeaders, i, NamePnt->LevelSpecific ) ) return FALSE;	// the model and visipoly data
					}
					else
					{
						DebugPrintf( "Unknown format for file %s\n", &TempFilename[ 0 ] );
						return FALSE;
					}
				}
			}

			NamePnt++;

		}
		else break;
	}
	return	TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Init models
	Input		:	LPDIRECT3DDEVICE	Direct 3D Device
				:	MODELNAME	*		Current Model Name
	Output		:	BOOL				TRUE/FALSE
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL InitModel( LPDIRECT3DDEVICE lpDev , MODELNAME * NamePnt)
{
	int i;
	int8		TempFilename[ 256 ];
	VECTOR		TempVector = { 0.0F, 0.0F, 0.0F };
	VECTOR		TopLeft;
	VECTOR		BottomRight;

	for( i = 0 ; i < MAXMODELHEADERS ; i++ )
	{
		if( NamePnt->Name[0] != 0 )
		{
			if( NamePnt->LoadEnable )
			{
				strcpy( &TempFilename[0], NamePnt->Name );

				if( NamePnt->DoIMorph )
				{
//					DebugPrintf( "Loading MXA File %s\n", &TempFilename[ 0 ] );

					if( !Mxaload( &TempFilename[0] , &MxaModelHeaders[i], NamePnt->StoreTriangles ) ) return FALSE;	// the model and visipoly data
					ModelHeaders[i].LOD = NamePnt->LOD;

					TopLeft = TempVector;
					BottomRight = TempVector;
					GetMXABoundingBox( &MxaModelHeaders[i], &MATRIX_Identity, &TempVector, &TopLeft, &BottomRight );
					ModelHeaders[i].Center.x = ( TopLeft.x + ( ( BottomRight.x - TopLeft.x ) * 0.5F ) );
					ModelHeaders[i].Center.y = ( TopLeft.y + ( ( BottomRight.y - TopLeft.y ) * 0.5F ) );
					ModelHeaders[i].Center.z = ( TopLeft.z + ( ( BottomRight.z - TopLeft.z ) * 0.5F ) );
					ModelHeaders[i].Sizes.x = ( BottomRight.x - TopLeft.x );
					ModelHeaders[i].Sizes.y = ( BottomRight.y - TopLeft.y );
					ModelHeaders[i].Sizes.z = ( BottomRight.z - TopLeft.z );
				}
				else
				{
//					DebugPrintf( "Loading MX File %s\n", &TempFilename[ 0 ] );

					if( !Mxload( &TempFilename[0] , &ModelHeaders[i] , NamePnt->Panel, NamePnt->StoreTriangles) ) return FALSE;	// the model and visipoly data
					ModelHeaders[i].LOD = NamePnt->LOD;

					TopLeft = TempVector;
					BottomRight = TempVector;
					GetMXBoundingBox( &ModelHeaders[i], &MATRIX_Identity, &TempVector, &TopLeft, &BottomRight );
					ModelHeaders[i].Center.x = ( TopLeft.x + ( ( BottomRight.x - TopLeft.x ) * 0.5F ) );
					ModelHeaders[i].Center.y = ( TopLeft.y + ( ( BottomRight.y - TopLeft.y ) * 0.5F ) );
					ModelHeaders[i].Center.z = ( TopLeft.z + ( ( BottomRight.z - TopLeft.z ) * 0.5F ) );
					ModelHeaders[i].Sizes.x = ( BottomRight.x - TopLeft.x );
					ModelHeaders[i].Sizes.y = ( BottomRight.y - TopLeft.y );
					ModelHeaders[i].Sizes.z = ( BottomRight.z - TopLeft.z );
				}
			}

			NamePnt++;
		}
		else break;
	}
	return	TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Find a free Model and move it from the free list to
					the used list
	Input		:	nothing
	Output		:	uint16 number of Model free....
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
uint16	FindFreeModel()
{
	uint16 i;

#ifdef DEBUG_ON
	CheckModelLinkList();
#endif

	i = FirstModelFree;
	
	if ( i == (uint16) -1)
		return i;
 
	Models[i].Prev = FirstModelUsed;
	if ( FirstModelUsed != (uint16) -1)
	{
		Models[FirstModelUsed].Next = i;
	}

	Models[i].Flags = MODFLAG_Nothing;
	Models[i].Func = MODFUNC_Nothing;
	Models[i].LifeCount = -1.0F;
	Models[i].Scale = 1.0F;
	Models[i].Visible = TRUE;
	Models[i].TimeInterval = (float) ( 11 - TrailDetailSlider.value );
	Models[i].TimeCount = 0.0F;

	Models[i].Red = 255;
	Models[i].Green = 255;
	Models[i].Blue = 255;

   	if( AutoDetail )
   	{
   		if( avgframelag > 2.0F ) Models[i].TimeInterval +=  ( avgframelag - 1.0F );
   	}

	FirstModelUsed = i;
	FirstModelFree = Models[i].Next;

#ifdef DEBUG_ON
	CheckModelLinkList();
#endif
	return i ;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Kill a used Model and move it from the used list to
					the free list
	Input		:	uint16 number of Model free....
	Output		:	nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void	KillUsedModel( uint16 i )
{
	uint16		its_prev;
	uint16		its_next;
	int16		Count;
	
#ifdef DEBUG_ON
	CheckModelLinkList();
#endif
	its_prev = Models[i].Prev;
	its_next = Models[i].Next;

	if ( i == FirstModelUsed )
		FirstModelUsed = Models[i].Prev;

	if( its_prev != (uint16) -1)
		Models[its_prev].Next = its_next;

	if( its_next != (uint16) -1)
		Models[its_next].Prev = its_prev;

	for( Count = 0; Count < 12; Count++ )
	{
		if( Models[i].TempLines[ Count ] != (uint16) -1 )
		{
			KillUsedLine( Models[i].TempLines[ Count ] );
			Models[i].TempLines[ Count ] = (uint16) -1;
		}
	}

	KillAttachedSpotFX( i );
	KillAttachedSoundFX( i );

	Models[i].Func = MODFUNC_Nothing;
	Models[i].Prev = (uint16) -1;
	Models[i].Next = FirstModelFree;
	Models[i].LifeCount = -1.0F;
	Models[i].Scale = 1.0F;
	FirstModelFree	= i;

#ifdef DEBUG_ON
	CheckModelLinkList();
#endif
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Kill Attached SFX still playing
	Input		:	uint16	number of Model
	Output		:	nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void KillAttachedSoundFX( uint16 i )
{
	int16		Count;
	uint16		NumSpotFX = 0;
	PVSPOTFX *	SpotFXPtr = NULL;

	switch( Models[i].Func )
	{
		case MODFUNC_ProcessSpotFX:
			if( ModelNames[ Models[i].ModelNum ].DoIMorph )
			{
				if( MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
					MxaModelHeaders[ Models[i].ModelNum ].SpotFX )
				{
					SpotFXPtr = MxaModelHeaders[ Models[i].ModelNum ].SpotFX;
					NumSpotFX = MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX;
				}
			}
			else
			{
				if( ModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
					ModelHeaders[ Models[i].ModelNum ].SpotFX )
				{
					SpotFXPtr = ModelHeaders[ Models[i].ModelNum ].SpotFX;
					NumSpotFX = ModelHeaders[ Models[i].ModelNum ].NumSpotFX;
				}
			}

			if( SpotFXPtr && NumSpotFX )
			{
				for( Count = 0; Count < NumSpotFX; Count++ )
				{
					switch( SpotFXPtr->Type )
					{
						case SPOTFX_SoundFX:
							if( SpotFXPtr->SoundFX != -1 )
							{
								if( Models[i].SpotFXSFX_ID[ Count ] )
								{
									StopSfx( Models[i].SpotFXSFX_ID[ Count ] );
									Models[i].SpotFXSFX_ID[ Count ] = 0;
									Models[i].SpotFXFlags[ Count ] &= ~SPOTFX_FLAGS_DoneOnce;
								}
							}
							break;
						default:
							break;
					}
					SpotFXPtr++;
				}
			}
			break;

		default:
			break;
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Kill Attached Spot still playing
	Input		:	uint16	number of Model
	Output		:	nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void KillAttachedSpotFX( uint16 i )
{
	int16		Count;
	uint16		NumSpotFX = 0;
	PVSPOTFX *	SpotFXPtr = NULL;

	switch( Models[i].Func )
	{
		case MODFUNC_ProcessSpotFX:
			if( ModelNames[ Models[i].ModelNum ].DoIMorph )
			{
				if( MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
					MxaModelHeaders[ Models[i].ModelNum ].SpotFX )
				{
					SpotFXPtr = MxaModelHeaders[ Models[i].ModelNum ].SpotFX;
					NumSpotFX = MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX;
				}
			}
			else
			{
				if( ModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
					ModelHeaders[ Models[i].ModelNum ].SpotFX )
				{
					SpotFXPtr = ModelHeaders[ Models[i].ModelNum ].SpotFX;
					NumSpotFX = ModelHeaders[ Models[i].ModelNum ].NumSpotFX;
				}
			}

			if( SpotFXPtr && NumSpotFX )
			{
				for( Count = 0; Count < NumSpotFX; Count++ )
				{
					switch( SpotFXPtr->Type )
					{
						case SPOTFX_FirePrimary:
							KillPrimBullsByOwner( OWNER_MODELSPOTFX, i );
							break;
						default:
							break;
					}
					SpotFXPtr++;
				}
			}
			break;

		default:
			break;
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
*		display all active Models...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ModelDisp( uint16 group, LPDIRECT3DDEVICE lpDev, MODELNAME * NamePnt  )
{
	D3DMATRIX TempWorld;
	uint16	i;
	uint16	nextmodel;
	uint16	ModelNum;
	float	temp, temp2;
	VECTOR	TempVector;
	QUAT	TempQuat;
	MATRIX	TempMatrix, TempRotMatrix;
	VECTOR	x1, x2;
	uint16	ClipGroup;
	float r , g , b;
	BOOL	InTitle;
	BOOL	DoDisplay;
#ifdef SOFTWARE_ENABLE
	float	Trans;
#endif

	if( NamePnt == &ModelNames[0] ) InTitle = FALSE;
	else InTitle = TRUE;

	i =  FirstModelUsed;
	while( i != (uint16) -1 )
	{
		nextmodel = Models[i].Prev;

		if( Models[ i ].Visible )
		{
			if( InTitle || ( IsGroupVisible[ Models[ i ].Group ] || VisibleOverlap( Ships[ Current_Camera_View ].Object.Group, Models[ i ].Group, NULL ) ) )
			{
				if( Models[ i ].Flags & MODFLAG_UseClipGroup ) ClipGroup = Models[i].ClipGroup;
				else ClipGroup = Models[i].Group;
				
				if( ClipGroup == group )
				{
					switch( Models[i].Func )
					{
						case MODFUNC_Fade:
#if 0
							// is this mxa?
							/*
							if ( NamePnt[ i ].DoIMorph )
							{
								if( !FadeMxaModel( &MxaModelHeaders[ i ] ))
									DebugPrintf("FadeMxaModel failed\n");
							}
							*/
							if (!FadeMxaModel( &MxaModelHeaders[ Models[ i ].ModelNum ] ))
								Msg("MakeTranslucent() returned FALSE");

							TempMatrix = MATRIX_Identity;
							TempMatrix._11 *= Models[i].Xscale;
							TempMatrix._22 *= Models[i].Yscale;
							TempMatrix._33 *= Models[i].Zscale;

							BuildRotMatrix(Models[i].Rot.x, Models[i].Rot.y, Models[i].Rot.z, &TempRotMatrix);
							

							MatrixMultiply( &TempMatrix, &TempRotMatrix, &Models[i].Mat );
							MatrixTranspose( &Models[ i ].Mat, &Models[ i ].InvMat );

							
							//RefreshVertex( &MxaModelHeaders[ Models[ i ].ModelNum ],
							//	CurrentGroup, CurrentExecBuf, CurrentVert, 1.0F );

							if ( BikeExpanded )
								RefreshModel( i ); 
#endif							
							break;
						case MODFUNC_ReScale:
							break;

						case MODFUNC_Explode:
						case MODFUNC_Regen:
						case MODFUNC_Scale:
						case MODFUNC_Scale2:
						case MODFUNC_SphereZone:
							ApplyMatrix( &CurrentCamera.Mat, &Forward, &TempVector );
							QuatFromVector2( &TempVector, &TempQuat );
							QuatToMatrix( &TempQuat, &Models[ i ].Mat );
							TempMatrix = MATRIX_Identity;
							TempMatrix._11 = Models[i].Scale;
							TempMatrix._22 = Models[i].Scale;
							TempMatrix._33 = Models[i].Scale;
							MatrixMultiply( &TempMatrix, &Models[i].Mat, &Models[i].Mat );
							MatrixTranspose( &Models[ i ].Mat, &Models[ i ].InvMat );
							break;
			
						case MODFUNC_RealScale:
							Models[ i ].Mat = MATRIX_Identity;
							Models[ i ].Mat._11 = Models[i].Scale;
							Models[ i ].Mat._22 = 1.0F; //Models[i].Scale;
							Models[ i ].Mat._33 = Models[i].Scale;
							MatrixTranspose( &Models[ i ].Mat, &Models[ i ].InvMat );
							break;

						case MODFUNC_ScaleXYZandRotate:
							TempMatrix = MATRIX_Identity;
							TempMatrix._11 *= Models[i].Xscale;
							TempMatrix._22 *= Models[i].Yscale;
							TempMatrix._33 *= Models[i].Zscale;

							BuildRotMatrix(HoloRot.x, HoloRot.y, HoloRot.z, &TempRotMatrix);
							

							MatrixMultiply( &TempMatrix, &TempRotMatrix, &Models[i].Mat );
							MatrixTranspose( &Models[ i ].Mat, &Models[ i ].InvMat );
							break;
						
						case MODFUNC_ScaleDonut:
							Models[ i ].Mat = MATRIX_Identity;
							Models[ i ].Mat._11 = Models[i].Scale;
							Models[ i ].Mat._22 = Models[i].Scale;
							Models[ i ].Mat._33 = Models[i].Scale;
							MatrixTranspose( &Models[ i ].Mat, &Models[ i ].InvMat );
							break;

						case MODFUNC_Nothing:
						default:
							break;

					}

					ENV( &ModelHeaders[Models[i].ModelNum] , &Models[i].Mat , &Models[i].Pos);
				
					TempWorld._11 = Models[i].InvMat._11;
					TempWorld._12 = Models[i].InvMat._12;
					TempWorld._13 = Models[i].InvMat._13;
					TempWorld._14 = Models[i].InvMat._14;
					TempWorld._21 = Models[i].InvMat._21;
					TempWorld._22 = Models[i].InvMat._22;
					TempWorld._23 = Models[i].InvMat._23;
					TempWorld._24 = Models[i].InvMat._24;
					TempWorld._31 = Models[i].InvMat._31;
					TempWorld._32 = Models[i].InvMat._32;
					TempWorld._33 = Models[i].InvMat._33;
					TempWorld._34 = Models[i].InvMat._34;
					TempWorld._41 = Models[i].Pos.x;
					TempWorld._42 = Models[i].Pos.y;
					TempWorld._43 = Models[i].Pos.z;
					TempWorld._44 = Models[i].InvMat._44;
					
					if (lpDev->lpVtbl->SetMatrix(lpDev, hWorld, &TempWorld) != D3D_OK)
					{
						Msg( "ModelDisp() SetMatrix1 Failed\n" );
						return FALSE;
					}
					
					/*	Execute it	*/
					temp = DistanceVector2Vector( &Models[i].Pos, &CurrentCamera.Pos ); 
		
	 				if( ModelHeaders[Models[i].ModelNum].LOD != 0 )
					{
						if( CurrentCamera.UseLowestLOD )
						{
							ModelNum = ModelHeaders[Models[i].ModelNum].LOD;
						}
						else
						{
							if( AutoDetail && ( avgframelag >= 2.0F ) )
							{
								// auto detail level....
								ModelNum = (uint16) (avgframelag - 1.0F);

								if( ModelNum > ModelHeaders[Models[i].ModelNum].LOD )
								{
									ModelNum = ModelHeaders[Models[i].ModelNum].LOD;
								}
							}
							else
							{
								temp2 = ( temp + ( ( (float) NumPickupsPerGroup[ group ] ) * ( 128.0F * GLOBAL_SCALE ) ) );
									
								if( temp2 >= ( ( 3072.0F * GLOBAL_SCALE )  * ( (float) ModelHeaders[Models[i].ModelNum].LOD ) )  )
								{
									ModelNum = ModelHeaders[Models[i].ModelNum].LOD;
								}
								else
								{
									ModelNum = (uint16) ( temp2 / (3072.0F*GLOBAL_SCALE )  );
									if( ModelNum > ModelHeaders[ Models[i].ModelNum ].LOD )
									{
										ModelNum = ModelHeaders[Models[i].ModelNum].LOD;
									}
								}
							}

							if( ModelHeaders[ Models[i].ModelNum ].LOD == NUMBIKEDETAILS )		// Hack for bike detail
							{
								if( ModelNum < BikeDetail )	ModelNum = BikeDetail;

								if( ModelNum > ModelHeaders[ Models[i].ModelNum ].LOD )
								{
									ModelNum = ModelHeaders[Models[i].ModelNum].LOD;
								}
							}
						}
					}
					else
					{
						ModelNum = 0;
					}

					ModelNum += Models[i].ModelNum;
		
//					if( ( temp > ( PICKUP_RADIUS * 0.75F ) ) || !( Models[i].Flags & MODFLAG_Clip ) )
					if( ( temp > ( PICKUP_RADIUS * 0.75F ) ) || ( Models[i].Type == MODTYPE_Field ) )
					{
						DoDisplay = TRUE;

						if( NamePnt[ ModelNum ].DoIMorph )
						{
							if( ( Models[i].Flags & MODFLAG_StealthOffset ) && ( Models[i].Flags & MODFLAG_Stealth ) )
							{
								if( !InterpFrames( &MxaModelHeaders[ ModelNum + Models[i].StealthOffset ], Models[i].Frame, Models[i].InterpFrame, Models[i].InterpTime ) )
								{
									Msg( "ModelDisp() InterpFrames() Failed\n" );
									return FALSE;
								}
							}
							else
							{
								if( !InterpFrames( &MxaModelHeaders[ ModelNum ], Models[i].Frame, Models[i].InterpFrame, Models[i].InterpTime ) )
								{
									Msg( "ModelDisp() InterpFrames() Failed\n" );
									return FALSE;
								}
							}

							x1.x = 0.0F;
							x1.y = 0.0F;
							x1.z = 0.0F;
							x2 = x1;

#ifndef FINAL_RELEASE
							CreateMXABoundingBox( &MxaModelHeaders[ ModelNum ],	&Models[i].Mat, &Models[i].Pos,
												  &Models[i].TempLines[ 0 ], Models[i].Group );
#endif

							ENVMxa( &MxaModelHeaders[ ModelNum ] , &Models[i].Mat , &Models[i].Pos);


							if( Models[i].Flags & MODFLAG_AmbientLight )
							{
								GetRealLightAmbient( &Models[i].Pos , &r , &g , &b );
								AmbientLightMxaModel( &MxaModelHeaders[ ModelNum ],
												Models[i].Red, Models[i].Green, Models[i].Blue, 0 , r , g , b);

							}else if( Models[i].Flags & MODFLAG_RealLight )
							{
								TempMatrix = Models[i].Mat;
								TempMatrix._41 = Models[i].Pos.x;
								TempMatrix._42 = Models[i].Pos.y;
								TempMatrix._43 = Models[i].Pos.z;
								XLightMxaloadHeader( &MxaModelHeaders[ ModelNum ], &Models[i].Pos, Models[i].Radius, &TempMatrix );
							}

							if( Models[i].Flags & MODFLAG_Stealth )
							{
								if( Models[i].Flags & MODFLAG_StealthOffset )
								{
									ModelNum += Models[i].StealthOffset;
								}
								else
								{
									ModelNum += ( ModelHeaders[Models[i].ModelNum].LOD + 1 );
								}
#ifdef SOFTWARE_ENABLE
								if( SoftwareVersion )
								{
//									if( !LightMxaModel( ModelNum, &Models[i].Pos, 0.0F, 0.0F, 0.0F, 0.0F ) )
									Trans = (float) ( floor( ( (float) Models[i].Red ) + ( (float) Models[i].Green ) + ( (float) Models[i].Blue ) ) / 3.0F );

									if( !LightMxaModel( ModelNum, &Models[i].Pos, (float) Models[i].Red, (float) Models[i].Green, (float) Models[i].Blue, Trans ) )
									{
										DoDisplay = FALSE;
									}
								}
								else
#endif
								{
									if( !LightMxaModel( ModelNum, &Models[i].Pos, (float) Models[i].Red, (float) Models[i].Green, (float) Models[i].Blue, 255.0F ) )
									{
										DoDisplay = FALSE;
									}
								}
							}

							if( !( Models[i].Flags & MODFLAG_RealLight ) &&
								!( Models[i].Flags & MODFLAG_Stealth ) &&
								!( Models[i].Flags & MODFLAG_AmbientLight ) &&
								!( Models[i].Flags & MODFLAG_Original ) )
							{
								UpdateMxaModel( &MxaModelHeaders[ ModelNum ] );
							}

							if( DoDisplay )
							{
								if( ExecuteMxaloadHeader( &MxaModelHeaders[ModelNum], ClipGroup ) != TRUE)
								{
									Msg( "ModelDisp() ExecuteMxaloadHeader for %s Failed\n", &ModelNames[ Models[i].ModelNum ].Name[ 0 ] );
									return FALSE;
								}
							}
						}
						else
						{
							x1.x = 0.0F;
							x1.y = 0.0F;
							x1.z = 0.0F;
							x2 = x1;

#ifndef FINAL_RELEASE
							CreateMXBoundingBox( &ModelHeaders[ ModelNum ],	&Models[i].Mat, &Models[i].Pos,
												 &Models[i].TempLines[ 0 ], Models[i].Group );
#endif
							if( Models[i].Flags & MODFLAG_AmbientLight )
							{
								GetRealLightAmbient( &Models[i].Pos , &r , &g , &b );
								AmbientLightMxModel( &ModelHeaders[ ModelNum ],
												Models[i].Red, Models[i].Green, Models[i].Blue, 0 , r , g , b);
							}else if( Models[i].Flags & MODFLAG_RealLight )
							{
								TempMatrix = Models[i].Mat;
								TempMatrix._41 = Models[i].Pos.x;
								TempMatrix._42 = Models[i].Pos.y;
								TempMatrix._43 = Models[i].Pos.z;
								XLightMxloadHeader( &ModelHeaders[ ModelNum ], &Models[i].Pos, Models[i].Radius, &TempMatrix );
							}

							if( Models[i].Flags & MODFLAG_Stealth )
							{
								if( Models[i].Flags & MODFLAG_StealthOffset )
								{
									ModelNum += Models[i].StealthOffset;
								}
								else
								{
									ModelNum += ( ModelHeaders[Models[i].ModelNum].LOD + 1 );
								}

#ifdef SOFTWARE_ENABLE
								if( SoftwareVersion )
								{
//									if( !LightMxModel( ModelNum, &Models[i].Pos, 0.0F, 0.0F, 0.0F, 0.0F ) )
									Trans = (float) ( floor( ( (float) Models[i].Red ) + ( (float) Models[i].Green ) + ( (float) Models[i].Blue ) ) / 3.0F );

									if( !LightMxModel( ModelNum, &Models[i].Pos, (float) Models[i].Red, (float) Models[i].Green, (float) Models[i].Blue, Trans ) )
									{
										DoDisplay = FALSE;
									}
								}
								else
#endif
								{
									if( !LightMxModel( ModelNum, &Models[i].Pos, (float) Models[i].Red, (float) Models[i].Green, (float) Models[i].Blue, 255.0F ) )
									{
										DoDisplay = FALSE;
									}
								}
							}

							if( DoDisplay )
							{
								if( ExecuteMxloadHeader( &ModelHeaders[ ModelNum ], i ) != TRUE)
								{
									Msg( "ModelDisp() ExecuteMxloadHeader for %s Failed\n", &ModelNames[ Models[i].ModelNum + ModelNum ].Name[ 0 ] );
									return FALSE;
								}
							}
						}
					}
				}
			}
		}

		/*	go on to the next	*/
		i = nextmodel;
	}
	if (lpDev->lpVtbl->SetMatrix(lpDev, hWorld, &identity) != D3D_OK)
	{
		Msg( "ModelDisp() SetMatrix2 Failed\n" );
		return FALSE;
	}
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
*		Set up 2d exec buff etc...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ReleaseTitleModels( )
{
	int i;

	for( i = 0 ; i < MAXMODELHEADERS; i++ )
	{
		if( TitleModelSet[i].Name[0] != 0 )
		{
			if( TitleModelSet[i].LoadEnable )
			{
				if( TitleModelSet[i].DoIMorph )
				{
					ReleaseMxaloadheader( &MxaModelHeaders[i] );
				}
				else
				{
					ReleaseMxloadheader( &ModelHeaders[i] );
				}
			}
		}
		else break;
	}
	return	TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
*		Set up 2d exec buff etc...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ReleaseModels( )
{
	int i;

	for( i = 0 ; i < MAXMODELHEADERS ; i++ )
	{
		if( ModelNames[i].Name[0] != 0 )
		{
			if( ModelNames[i].LoadEnable )
			{
				if( ModelNames[i].DoIMorph )
				{
					ReleaseMxaloadheader( &MxaModelHeaders[i] );
				}
				else
				{
					ReleaseMxloadheader( &ModelHeaders[i] );
				}
			}
		}
		else break;
	}
	return	TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Process Models
	Input		:	Nothing
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
VECTOR BloodOffsets[] = {

	{ 0.0F, -1.0F, 0.0F }, { 0.0F, -25.0F, 0.0F },					// Head Dir, Offsets
	{ -0.276F, 0.92F, -0.276F },	{ -30.0F, 100.0F, -30.0F },		// Leg Dir, Offsets
	{ 0.0F, 0.0F, 1.0F }, { 0.0F, 35.0F, 40.0F },					// Arm Dir, Offsets

};

float TomFrame = 0.0F;

void ProcessModels( void )
{
	uint16		i;
	uint16		nextmodel;
	VECTOR		Int_Point;
	NORMAL		Int_Normal;
	VECTOR		TempVector;
	float		Speed;
	float		Cos;
	QUAT		TempQuat;
	VECTOR		FrameLag_Rot;
	VECTOR		Pos;
	VECTOR		TempDir;
	VECTOR		OffVector;
	VECTOR	*	BloodPtr;
	VECTOR		Rotation;
	VECTOR		UpVector;
	VECTOR		DirVector;
	MATRIX		UpMatrix;
	MATRIX		InvUpMatrix;
	uint16		NumSpotFX;
	PVSPOTFX *	SpotFXPtr;
	int16		Count;
	float		WaterDamage;
	uint16		SpotFXGroup;
	int16		VisNum;
	uint16		VisGroups[ MAXGROUPS ];
	int16		Count2;
	float		ShipSpeed;

	i =  FirstModelUsed;

	while( i != (uint16) -1 )
	{
		nextmodel = Models[i].Prev;

		if( Models[ i ].Visible )
		{
			switch( Models[i].Func )
			{
				case MODFUNC_Nothing:
					break;

				case MODFUNC_Explode:
					Models[i].Scale += ( ( Models[i].MaxScale / 40.0F ) * framelag );
					if( Models[i].Scale <= Models[i].MaxScale )			// 10.0F
					{
						ShockWave( &Models[i].Pos, ( Models[i].Scale * BALL_RADIUS ), Models[i].OwnerType, Models[i].Owner, 16.0F, Models[i].Group, Models[i].SecWeapon, i );
					}
					else
					{
						KillUsedModel( i );
					}
					break;

				case MODFUNC_Regen:
					Models[i].Pos = Ships[Models[i].Owner].Object.Pos;
					Models[i].Scale += ( framelag * 0.01F );
					if( Models[i].Scale > Models[i].MaxScale )
					{
						KillUsedModel( i );
						break;
					}
					break;

				case MODFUNC_SphereZone:
					break;

				case MODFUNC_Scale:
					Models[i].LifeCount -= framelag;
					if( Models[i].LifeCount <= 0.0F )
					{
						Models[i].LifeCount = 0.0F;
					}

					Models[i].Scale -= ( framelag * GRAVGON_SCALE_DEC );
					if( Models[i].Scale < 0.0F )
					{
						Models[i].Scale = Models[i].MaxScale;
						if( Models[i].LifeCount == 0.0F )
						{
							KillUsedModel( i );
							break;
						}
					}
					GravityWave( &Models[i].Pos, ( BALL_RADIUS * Models[i].MaxScale ), Models[i].Owner, 10.0F, Models[i].Group );
					break;

				case MODFUNC_Scale2:
					Models[i].LifeCount -= framelag;
					if( Models[i].LifeCount <= 0.0F )
					{
						Models[i].LifeCount = 0.0F;
					}

					Models[i].Scale -= ( framelag * GRAVGON_SCALE_DEC );
					if( Models[i].Scale < 0.0F )
					{
						Models[i].Scale = Models[i].MaxScale;
						if( Models[i].LifeCount == 0.0F )
						{
							KillUsedModel( i );
							break;
						}
					}
					GravityWave( &Models[i].Pos, ( BALL_RADIUS * Models[i].MaxScale ), Models[i].Owner, 10.0F, Models[i].Group );
					break;

				case MODFUNC_BodyPart:
					if( Models[i].Group != (uint16) -1 )
					{
						QuatFrom2Vectors( &TempQuat, &Mloadheader.Group[ Models[i].Group ].up, &SlideUp );
						QuatToMatrix( &TempQuat, &UpMatrix );
						MatrixTranspose( &UpMatrix, &InvUpMatrix );
					}
					else
					{
						QuatFrom2Vectors( &TempQuat, &SlideUp, &SlideUp );
						QuatToMatrix( &TempQuat, &UpMatrix );
						MatrixTranspose( &UpMatrix, &InvUpMatrix );
					}

					Models[i].LifeCount -= framelag;
					if( Models[i].LifeCount < 0.0F )
					{
						Models[i].LifeCount = 0.0F;
						KillUsedModel( i );
						break;
					}
					else
					{
						if( AmIOutsideGroup( &Mloadheader, &Models[i].Pos, Models[i].Group ) )
						{
							Models[i].LifeCount = 0.0F;
							KillUsedModel( i );
							break;
						}

	     				TempDir.x = ( Models[i].Dir.x * framelag );
	     				TempDir.y = ( Models[i].Dir.y * framelag );
	     				TempDir.z = ( Models[i].Dir.z * framelag );
		  
	     				if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, &Models[i].Pos,
	     									  Models[i].Group, &TempDir, (VECTOR *) &Int_Point,
	     									  &Models[i].Group, &Int_Normal, &TempVector, TRUE, NULL ) == TRUE )
	     				{
							Models[i].Pos = Int_Point;
	     					CreateBloodSplat( &Int_Point, (VECTOR *) &Int_Normal, Models[i].Group );
		
	     					TempVector = Models[i].Dir;
	     					NormaliseVector( &TempVector );
	     					Speed = VectorLength( &Models[i].Dir );
	     					ReflectVector( &TempVector, &Int_Normal, &Models[i].Dir );
	     					Speed *= 0.5F;
		  
							PlayPannedSfx( SFX_Squish, Models[i].Group , &Models[i].Pos, 0.0F );
		
	     					if( Speed < ( 5.0F * GLOBAL_SCALE ) )
	     					{
		  						KillUsedModel( i );
	     						break;
	     					}
		  
	     					Models[i].Dir.x *= Speed;
	     					Models[i].Dir.y *= Speed;
	     					Models[i].Dir.z *= Speed;
	     					Models[i].Rot.x = ( ( (float) Random_Range( 10000 ) ) / 10000.0F );
	     					Models[i].Rot.y = ( ( (float) Random_Range( 10000 ) ) / 10000.0F );
	     					Models[i].Rot.z = ( ( (float) Random_Range( 10000 ) ) / 10000.0F );
	     					Models[i].Rot.x *= Speed;
	     					Models[i].Rot.y *= Speed;
	     					Models[i].Rot.z *= Speed;
	     				}
						else
						{
		     				Models[i].Pos.x += ( Models[i].Dir.x * framelag );
		     				Models[i].Pos.y += ( Models[i].Dir.y * framelag );
		     				Models[i].Pos.z += ( Models[i].Dir.z * framelag );
						}

						ApplyMatrix( &UpMatrix, &Models[i].Dir, &TempDir );

	     				if( TempDir.x )
	     				{
	     					if( TempDir.x > 0.0F )
	     					{
	     						TempDir.x -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x < 0.0F ) TempDir.x = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.x += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x > 0.0F ) TempDir.x = 0.0F;
	     					}
	     				}
	     				if( TempDir.z )
	     				{
	     					if( TempDir.z > 0.0F )
	     					{
	     						TempDir.z -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z < 0.0F ) TempDir.z = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.z += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z > 0.0F ) TempDir.z = 0.0F;
	     					}
	     				}
	     				TempDir.y -= ( ( 0.5F * GLOBAL_SCALE ) * framelag );

						ApplyMatrix( &InvUpMatrix, &TempDir, &Models[i].Dir );
		  
	     	   			FrameLag_Rot.x = ( Models[i].Rot.x * framelag );
	     	   			FrameLag_Rot.y = ( Models[i].Rot.y * framelag );
	     	   			FrameLag_Rot.z = ( Models[i].Rot.z * framelag );
	     	   			MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
	     	   			QuatMultiply( &TempQuat, &Models[i].Quat, &Models[i].Quat );
	     	   			QuatToMatrix( &Models[i].Quat, &Models[i].Mat );							// Create Dir Matrix
	     	   			MatrixTranspose( &Models[i].Mat, &Models[i].InvMat );						// Create Dir Matrix
		
//						BloodPtr = &BloodOffsets[ ( ( Models[i].ModelNum - MODEL_BorgHead ) * 2 ) ];
						BloodPtr = &BloodOffsets[ 0 ];
						
						ApplyMatrix( &Models[i].Mat, BloodPtr, &TempVector );
	     				NormaliseVector( &TempVector );
						BloodPtr++;
						ApplyMatrix( &Models[i].Mat, BloodPtr, &OffVector );
	     				Cos = DotProduct( &TempVector, &SlideUp );
	     				if( Cos < 0.0F )
	     				{
							if( IsGroupVisible[ Models[i].Group ] )
							{
								Models[i].TimeCount += framelag;
		
								if( Models[i].TimeCount >= Models[i].TimeInterval )
								{
//									Models[i].TimeCount -= Models[i].TimeInterval;
									Models[i].TimeCount = 0.0F;
									Pos.x = ( Models[i].Pos.x + ( OffVector.x * GLOBAL_SCALE ) );
									Pos.y = ( Models[i].Pos.y + ( OffVector.y * GLOBAL_SCALE ) );
									Pos.z = ( Models[i].Pos.z + ( OffVector.z * GLOBAL_SCALE ) );
			     					CreateBloodTrail( &Pos, &TempVector, Models[i].Group );
								}
							}
	     				}
						else
						{
							Models[i].TimeCount = 0.0F;
						}
					}
	 				break;
#if !defined( SELF_PLAY ) && !defined( SHAREWARE ) && !defined ( WIN98SHAREWARE ) && !defined ( GATEWAY )
				case MODFUNC_TomCruise:
					if( Models[i].Group != (uint16) -1 )
					{
						QuatFrom2Vectors( &TempQuat, &Mloadheader.Group[ Models[i].Group ].up, &SlideUp );
						QuatToMatrix( &TempQuat, &UpMatrix );
						MatrixTranspose( &UpMatrix, &InvUpMatrix );
					}
					else
					{
						QuatFrom2Vectors( &TempQuat, &SlideUp, &SlideUp );
						QuatToMatrix( &TempQuat, &UpMatrix );
						MatrixTranspose( &UpMatrix, &InvUpMatrix );
					}

					TomFrame += ( framelag * 0.2F );
					while( TomFrame >= 27.0F ) TomFrame -= 27.0F;

					Models[ i ].ModelNum = (uint16) ( MODEL_Tom0 + TomFrame );

					Models[i].LifeCount -= framelag;
					if( Models[i].LifeCount < 0.0F )
					{
						Models[i].LifeCount = 0.0F;
						KillUsedModel( i );
						break;
					}
					else
					{
						if( AmIOutsideGroup( &Mloadheader, &Models[i].Pos, Models[i].Group ) )
						{
							Models[i].LifeCount = 0.0F;
							KillUsedModel( i );
							break;
						}

	     				TempDir.x = ( Models[i].Dir.x * framelag );
	     				TempDir.y = ( Models[i].Dir.y * framelag );
	     				TempDir.z = ( Models[i].Dir.z * framelag );
		  
	     				if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, &Models[i].Pos,
	     									  Models[i].Group, &TempDir, (VECTOR *) &Int_Point,
	     									  &Models[i].Group, &Int_Normal, &TempVector, TRUE, NULL ) == TRUE )
	     				{
							PlayPannedSfx( SFX_Squish, Models[i].Group , &Models[i].Pos, 0.0F );
							NormaliseVector( &Models[i].Dir );
							ExplodeBody( &Models[i].Pos, &Models[i].Dir, Models[i].Group );
	  						KillUsedModel( i );
     						break;
	     				}
						else
						{
		     				Models[i].Pos.x += ( Models[i].Dir.x * framelag );
		     				Models[i].Pos.y += ( Models[i].Dir.y * framelag );
		     				Models[i].Pos.z += ( Models[i].Dir.z * framelag );
						}

						ApplyMatrix( &UpMatrix, &Models[i].Dir, &TempDir );

	     				if( TempDir.x )
	     				{
	     					if( TempDir.x > 0.0F )
	     					{
	     						TempDir.x -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x < 0.0F ) TempDir.x = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.x += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x > 0.0F ) TempDir.x = 0.0F;
	     					}
	     				}
	     				if( TempDir.z )
	     				{
	     					if( TempDir.z > 0.0F )
	     					{
	     						TempDir.z -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z < 0.0F ) TempDir.z = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.z += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z > 0.0F ) TempDir.z = 0.0F;
	     					}
	     				}
	     				TempDir.y -= ( ( 0.5F * GLOBAL_SCALE ) * framelag );

						ApplyMatrix( &InvUpMatrix, &TempDir, &Models[i].Dir );
					}
	 				break;
#endif // !SELF_PLAY
				case MODFUNC_OrbitPulsar:
//					if( Ships[ Models[ i ].Ship ].enable && ( Ships[ Models[ i ].Ship ].NumMultiples ) )
					if( ( ( GameStatus[ Models[ i ].Ship ] == STATUS_SinglePlayer ) || ( GameStatus[ Models[ i ].Ship ] == STATUS_Normal ) ) &&
						( Ships[ Models[ i ].Ship ].NumMultiples ) )
					{
						if( ( Ships[ Models[ i ].Ship ].Object.Flags & SHIP_Stealth ) ) Models[ i ].ModelNum = MODEL_OrbitPulsar_Light;
						else Models[ i ].ModelNum = MODEL_OrbitPulsar;

						Rotation.x = (float) sin( D2R( Models[ i ].AxisRot ) );
						Rotation.y = (float) cos( D2R( Models[ i ].AxisRot ) );
						Rotation.z = 0.0F;
						ApplyMatrix( &Ships[ Models[ i ].Ship ].Object.FinalMat, &Rotation, &UpVector );	// Calc Up Vector
						ApplyMatrix( &Ships[ Models[ i ].Ship ].Object.FinalMat, &Forward, &DirVector );	// Calc Dir Vector
						QuatFromDirAndUp( &DirVector, &UpVector, &Models[i].Quat );					// Create Quat
	     	   			QuatToMatrix( &Models[i].Quat, &Models[i].Mat );							// Create Matrix
	     	   			MatrixTranspose( &Models[i].Mat, &Models[i].InvMat );						// Create Inv Matrix
						Models[ i ].Pos.x = ( Ships[ Models[ i ].Ship ].Object.Pos.x + ( UpVector.x * ORBIT_RADIUS ) ); //+ ( DirVector.x * ( ORBIT_RADIUS * 0.85F ) ) );
						Models[ i ].Pos.y = ( Ships[ Models[ i ].Ship ].Object.Pos.y + ( UpVector.y * ORBIT_RADIUS ) ); //+ ( DirVector.y * ( ORBIT_RADIUS * 0.85F ) ) );
						Models[ i ].Pos.z = ( Ships[ Models[ i ].Ship ].Object.Pos.z + ( UpVector.z * ORBIT_RADIUS ) ); //+ ( DirVector.z * ( ORBIT_RADIUS * 0.85F ) ) );
						Models[ i ].AxisRot += ( 6.0F * framelag );
						Models[ i ].Group = Ships[ Models[ i ].Ship ].Object.Group;
					}
					else
					{
						KillUsedModel( i );
					}
					break;

				case MODFUNC_ReScale:
					break;

				case MODFUNC_RealScale:
					break;

				case MODFUNC_ScaleDonut:
					Models[i].Scale += ( ( Models[i].MaxScale / 60.0F ) * framelag );
					if( Models[i].Scale > Models[i].MaxScale ) KillUsedModel( i );
					break;

				case MODFUNC_LargeShrapnel:
					if( Models[i].Group != (uint16) -1 )
					{
						QuatFrom2Vectors( &TempQuat, &Mloadheader.Group[ Models[i].Group ].up, &SlideUp );
						QuatToMatrix( &TempQuat, &UpMatrix );
						MatrixTranspose( &UpMatrix, &InvUpMatrix );
					}
					else
					{
						QuatFrom2Vectors( &TempQuat, &SlideUp, &SlideUp );
						QuatToMatrix( &TempQuat, &UpMatrix );
						MatrixTranspose( &UpMatrix, &InvUpMatrix );
					}

					Models[i].LifeCount -= framelag;
					if( Models[i].LifeCount < 0.0F )
					{
						Models[i].LifeCount = 0.0F;
						KillUsedModel( i );
						break;
					}
					else
					{
	     				TempDir.x = ( Models[i].Dir.x * framelag );
	     				TempDir.y = ( Models[i].Dir.y * framelag );
	     				TempDir.z = ( Models[i].Dir.z * framelag );
		  
						WaterObjectCollide( Models[i].Group, &Models[i].Pos, &TempDir, &Int_Point, 255 );

	     				if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, &Models[i].Pos,
	     									  Models[i].Group, &TempDir, (VECTOR *) &Int_Point,
	     									  &Models[i].Group, &Int_Normal, &TempVector, TRUE, NULL ) == TRUE )
	     				{
							if( Random_Range( 3 ) )
							{
	     						TempVector = Models[i].Dir;
	     						NormaliseVector( &TempVector );
	     						Speed = VectorLength( &Models[i].Dir );
	     						ReflectVector( &TempVector, &Int_Normal, &Models[i].Dir );
	     						Speed *= 0.8F;

								CreateSpotFXSmallShrapnel( &Int_Point, &Models[i].Dir, Speed, Models[i].Group );
	  							KillUsedModel( i );
								break;
							}
							else
							{
								Models[i].Pos = Int_Point;

	     						TempVector = Models[i].Dir;
	     						NormaliseVector( &TempVector );
	     						Speed = VectorLength( &Models[i].Dir );
	     						ReflectVector( &TempVector, &Int_Normal, &Models[i].Dir );
	     						Speed *= 0.5F;
			  
	     						if( Speed < ( 5.0F * GLOBAL_SCALE ) )
	     						{
		  							KillUsedModel( i );
	     							break;
	     						}
			  
	     						Models[i].Dir.x *= Speed;
	     						Models[i].Dir.y *= Speed;
	     						Models[i].Dir.z *= Speed;
	     						Models[i].Rot.x = ( ( (float) Random_Range( 10000 ) ) / 10000.0F );
	     						Models[i].Rot.y = ( ( (float) Random_Range( 10000 ) ) / 10000.0F );
	     						Models[i].Rot.z = ( ( (float) Random_Range( 10000 ) ) / 10000.0F );
	     						Models[i].Rot.x *= Speed;
	     						Models[i].Rot.y *= Speed;
	     						Models[i].Rot.z *= Speed;
							}
	     				}
						else
						{
		     				Models[i].Pos.x += ( Models[i].Dir.x * framelag );
		     				Models[i].Pos.y += ( Models[i].Dir.y * framelag );
		     				Models[i].Pos.z += ( Models[i].Dir.z * framelag );
						}

						ApplyMatrix( &UpMatrix, &Models[i].Dir, &TempDir );

	     				if( TempDir.x )
	     				{
	     					if( TempDir.x > 0.0F )
	     					{
	     						TempDir.x -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x < 0.0F ) TempDir.x = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.x += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x > 0.0F ) TempDir.x = 0.0F;
	     					}
	     				}
	     				if( TempDir.z )
	     				{
	     					if( TempDir.z > 0.0F )
	     					{
	     						TempDir.z -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z < 0.0F ) TempDir.z = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.z += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z > 0.0F ) TempDir.z = 0.0F;
	     					}
	     				}
	     				TempDir.y -= ( ( 0.5F * GLOBAL_SCALE ) * framelag );

						ApplyMatrix( &InvUpMatrix, &TempDir, &Models[i].Dir );
		  
	     	   			FrameLag_Rot.x = ( Models[i].Rot.x * framelag );
	     	   			FrameLag_Rot.y = ( Models[i].Rot.y * framelag );
	     	   			FrameLag_Rot.z = ( Models[i].Rot.z * framelag );
	     	   			MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
	     	   			QuatMultiply( &TempQuat, &Models[i].Quat, &Models[i].Quat );
	     	   			QuatToMatrix( &Models[i].Quat, &Models[i].Mat );							// Create Dir Matrix
	     	   			MatrixTranspose( &Models[i].Mat, &Models[i].InvMat );						// Create Dir Matrix

						Models[i].TimeInterval = (float) ( 11 - TrailDetailSlider.value );
   						if( AutoDetail )
   						{
   							Models[i].TimeInterval += avgframelag;
   						}

						if( IsGroupVisible[ Models[i].Group ] )
						{
							Models[i].TimeCount += framelag;

							if( Models[i].TimeCount >= Models[i].TimeInterval )
							{
								Models[i].TimeCount = FMOD( Models[i].TimeCount, Models[i].TimeInterval );

								TempDir = Models[i].Dir;
								NormaliseVector( &TempDir );

								if( !InWater( Models[i].Group, &Models[i].Pos, &WaterDamage ) )
								{
									CreateShrapnelTrail( &Models[i].Pos, &TempDir, ( 8.0F * GLOBAL_SCALE ), Models[i].Group, 200, 100, 80 );
								}
								else
								{
									CreateBubbleTrail( &Models[i].Pos, &TempDir, Models[i].Group, 64, 64, 64 );
								}
							}
						}

					}
	 				break;

				case MODFUNC_SmallShrapnel:
					if( Models[i].Group != (uint16) -1 )
					{
						QuatFrom2Vectors( &TempQuat, &Mloadheader.Group[ Models[i].Group ].up, &SlideUp );
						QuatToMatrix( &TempQuat, &UpMatrix );
						MatrixTranspose( &UpMatrix, &InvUpMatrix );
					}
					else
					{
						QuatFrom2Vectors( &TempQuat, &SlideUp, &SlideUp );
						QuatToMatrix( &TempQuat, &UpMatrix );
						MatrixTranspose( &UpMatrix, &InvUpMatrix );
					}

					Models[i].LifeCount -= framelag;
					if( Models[i].LifeCount < 0.0F )
					{
						Models[i].LifeCount = 0.0F;
						KillUsedModel( i );
						break;
					}
					else
					{
	     				TempDir.x = ( Models[i].Dir.x * framelag );
	     				TempDir.y = ( Models[i].Dir.y * framelag );
	     				TempDir.z = ( Models[i].Dir.z * framelag );
		  
						WaterObjectCollide( Models[i].Group, &Models[i].Pos, &TempDir, &Int_Point, 128 );

	     				if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, &Models[i].Pos,
	     									  Models[i].Group, &TempDir, (VECTOR *) &Int_Point,
	     									  &Models[i].Group, &Int_Normal, &TempVector, TRUE, NULL ) == TRUE )
	     				{
							Models[i].Pos = Int_Point;

	     					TempVector = Models[i].Dir;
	     					NormaliseVector( &TempVector );
	     					Speed = VectorLength( &Models[i].Dir );
	     					ReflectVector( &TempVector, &Int_Normal, &Models[i].Dir );
	     					Speed *= 0.5F;
		  
	     					if( Speed < ( 5.0F * GLOBAL_SCALE ) )
	     					{
		  						KillUsedModel( i );
	     						break;
	     					}
		  
	     					Models[i].Dir.x *= Speed;
	     					Models[i].Dir.y *= Speed;
	     					Models[i].Dir.z *= Speed;
	     					Models[i].Rot.x = ( ( (float) Random_Range( 10000 ) ) / 10000.0F );
	     					Models[i].Rot.y = ( ( (float) Random_Range( 10000 ) ) / 10000.0F );
	     					Models[i].Rot.z = ( ( (float) Random_Range( 10000 ) ) / 10000.0F );
	     					Models[i].Rot.x *= Speed;
	     					Models[i].Rot.y *= Speed;
	     					Models[i].Rot.z *= Speed;
	     				}
						else
						{
		     				Models[i].Pos.x += ( Models[i].Dir.x * framelag );
		     				Models[i].Pos.y += ( Models[i].Dir.y * framelag );
		     				Models[i].Pos.z += ( Models[i].Dir.z * framelag );
						}

						ApplyMatrix( &UpMatrix, &Models[i].Dir, &TempDir );

	     				if( TempDir.x )
	     				{
	     					if( TempDir.x > 0.0F )
	     					{
	     						TempDir.x -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x < 0.0F ) TempDir.x = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.x += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x > 0.0F ) TempDir.x = 0.0F;
	     					}
	     				}
	     				if( TempDir.z )
	     				{
	     					if( TempDir.z > 0.0F )
	     					{
	     						TempDir.z -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z < 0.0F ) TempDir.z = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.z += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z > 0.0F ) TempDir.z = 0.0F;
	     					}
	     				}
	     				TempDir.y -= ( ( 0.5F * GLOBAL_SCALE ) * framelag );

						ApplyMatrix( &InvUpMatrix, &TempDir, &Models[i].Dir );
		  
	     	   			FrameLag_Rot.x = ( Models[i].Rot.x * framelag );
	     	   			FrameLag_Rot.y = ( Models[i].Rot.y * framelag );
	     	   			FrameLag_Rot.z = ( Models[i].Rot.z * framelag );
	     	   			MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
	     	   			QuatMultiply( &TempQuat, &Models[i].Quat, &Models[i].Quat );
	     	   			QuatToMatrix( &Models[i].Quat, &Models[i].Mat );							// Create Dir Matrix
	     	   			MatrixTranspose( &Models[i].Mat, &Models[i].InvMat );						// Create Dir Matrix

						if( IsGroupVisible[ Models[i].Group ] )
						{
							Models[i].TimeCount += framelag;

							if( Models[i].TimeCount >= Models[i].TimeInterval )
							{
								Models[i].TimeCount = FMOD( Models[i].TimeCount, Models[i].TimeInterval );

								TempDir = Models[i].Dir;
								NormaliseVector( &TempDir );

								if( !InWater( Models[i].Group, &Models[i].Pos, &WaterDamage ) )
								{
									CreateShrapnelTrail( &Models[i].Pos, &TempDir, ( 4.0F * GLOBAL_SCALE ), Models[i].Group, 200, 100, 80 );
								}
								else
								{
									CreateBubbleTrail( &Models[i].Pos, &TempDir, Models[i].Group, 64, 64, 64 );
								}
							}
						}
					}
	 				break;

				case MODFUNC_ProcessSpotFX:
//					if( Models[i].Flags & MODFLAG_Stealth ) break;
					if( Models[i].Flags & MODFLAG_SpotFXDisabled ) break;
					if( framelag == 0.0F ) break;

					SpotFXPtr = NULL;
					NumSpotFX = 0;

					if( ModelNames[ Models[i].ModelNum ].DoIMorph )
					{
						if( MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
							MxaModelHeaders[ Models[i].ModelNum ].SpotFX )
						{
							SpotFXPtr = MxaModelHeaders[ Models[i].ModelNum ].SpotFX;
							NumSpotFX = MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX;
						}
					}
					else
					{
						if( ModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
							ModelHeaders[ Models[i].ModelNum ].SpotFX )
						{
							SpotFXPtr = ModelHeaders[ Models[i].ModelNum ].SpotFX;
							NumSpotFX = ModelHeaders[ Models[i].ModelNum ].NumSpotFX;
						}
					}

					if( !SpotFXPtr || !NumSpotFX ) break;

					TempDir.x = ( Models[i].Pos.x - Models[i].OldPos.x );
					TempDir.y = ( Models[i].Pos.y - Models[i].OldPos.y );
					TempDir.z = ( Models[i].Pos.z - Models[i].OldPos.z );
					ApplyMatrix( &Models[i].InvMat, &TempDir, &TempDir );
					Speed = TempDir.z;
					ShipSpeed = VectorLength( &Models[i].Dir );

					for( Count = 0; Count < NumSpotFX; Count++ )
					{
						if( !( Models[i].Flags & MODFLAG_Stealth ) || ( SpotFXPtr->Type == SPOTFX_SoundFX ) )
						{
							Models[i].SpotFXTimeInterval[ Count ] = (float) ( 11 - TrailDetailSlider.value );

							if( AutoDetail )
							{
								Models[i].SpotFXTimeInterval[ Count ] += avgframelag;
							}

							VisNum = VisibleOverlap( Ships[ Current_Camera_View ].Object.Group, Models[i].Group, &VisGroups[ 0 ] );

							switch( Models[i].SpotFXState[ Count ] )
							{
								case SPOTFX_STATE_WaitingForDelay:
									Models[i].SpotFXDelay[ Count ] -= framelag;

									if( Models[i].SpotFXDelay[ Count ] < 0.0F )
									{
										Models[i].SpotFXDelay[ Count ] += SpotFXPtr->ActiveDelay;
										Models[i].SpotFXState[ Count ] = SPOTFX_STATE_Active;
					 					Models[i].SpotFXTimeCount[ Count ] = 0.0F;
									}
									break;

								case SPOTFX_STATE_Active:
									if( Models[i].Visible )
									{
										switch( SpotFXPtr->Type )
										{
											case SPOTFX_Flame:
											case SPOTFX_Smoke:
											case SPOTFX_Steam:
											case SPOTFX_Sparks:
											case SPOTFX_ElectricBeams:
											case SPOTFX_GravgonTrail:
											case SPOTFX_NmeTrail:
											case SPOTFX_NmeVapourTrail:
											case SPOTFX_FireWall:
											case SPOTFX_GravitySparks:
											case SPOTFX_Bubbles:
											case SPOTFX_BeardAfterburner:
											case SPOTFX_Hk5Afterburner:
											case SPOTFX_SoundFX:
											default:
												Models[i].SpotFXTimeCount[ Count ] += framelag;

												if( Models[i].SpotFXTimeCount[ Count ] >= Models[i].SpotFXTimeInterval[ Count ] )
												{
					 								Models[i].SpotFXTimeCount[ Count ] = (float) FMOD( Models[i].SpotFXTimeCount[ Count ], Models[i].SpotFXTimeInterval[ Count ] );

													ApplyMatrix( &Models[i].Mat, &SpotFXPtr->Pos, &Pos );
													ApplyMatrix( &Models[i].Mat, &SpotFXPtr->DirVector, &DirVector );
													ApplyMatrix( &Models[i].Mat, &SpotFXPtr->UpVector, &UpVector );

													Pos.x += Models[i].Pos.x;
													Pos.y += Models[i].Pos.y;
													Pos.z += Models[i].Pos.z;

													SpotFXGroup = Models[i].Group;

													for( Count2 = 0; Count2 < VisNum; Count2++ )
													{
														if( Bsp_Header[ 0 ].State )
														{
															if( PointInsideSkin( &Pos, VisGroups[ Count2 ] ) )
															{
																SpotFXGroup = VisGroups[ Count2 ];
																break;
															}
														}
														else
														{
															if( PointInGroupBoundingBox( &Mloadheader, &Pos, VisGroups[ Count2 ] ) )
															{
																SpotFXGroup = VisGroups[ Count2 ];
																break;
															}
														}
													}

													if( GroupsAreVisible( SpotFXGroup, Ships[ Current_Camera_View ].Object.Group ) )
													{
														switch( SpotFXPtr->Type )
														{
															case SPOTFX_Flame:
																CreateSpotFXFlame( &Pos, &DirVector, SpotFXGroup );
																break;

															case SPOTFX_Smoke:
																CreateSpotFXSmoke( &Pos, &DirVector, SpotFXGroup, SpotFXPtr->Red, SpotFXPtr->Green, SpotFXPtr->Blue );
																break;

															case SPOTFX_Steam:
																CreateSpotFXSteam( &Pos, &DirVector, SpotFXGroup, SpotFXPtr->Red, SpotFXPtr->Green, SpotFXPtr->Blue );
																break;

															case SPOTFX_Sparks:
																CreateSpotFXSparks( &Pos, &DirVector, SpotFXGroup, SpotFXPtr->Red, SpotFXPtr->Green, SpotFXPtr->Blue );
																break;

															case SPOTFX_ElectricBeams:
																CreateSpotFXElectricBeams( &Pos, &DirVector, &UpVector, SpotFXGroup );
																break;

															case SPOTFX_GravgonTrail:
																CreateSpotFXGravgonTrail( &Pos, &DirVector, SpotFXGroup );
																break;

															case SPOTFX_NmeTrail:
																CreateSpotFXNmeTrail( &Pos, &DirVector, SpotFXGroup, SpotFXPtr->Red, SpotFXPtr->Green, SpotFXPtr->Blue );
																break;

															case SPOTFX_NmeVapourTrail:
																CreateSpotFXNmeVapourTrail( &Pos, &DirVector, SpotFXGroup, SpotFXPtr->Red, SpotFXPtr->Green, SpotFXPtr->Blue );
																break;

															case SPOTFX_FireWall:
																CreateSpotFXFireWall( &Pos, &DirVector, &UpVector, SpotFXGroup );
																break;

															case SPOTFX_GravitySparks:
																CreateSpotFXRealSparks(  &Pos, &DirVector, SpotFXGroup, SpotFXPtr->Red, SpotFXPtr->Green, SpotFXPtr->Blue );
																break;

															case SPOTFX_Bubbles:
																CreateSpotFXBubbles( &Pos, &DirVector, SpotFXGroup, SpotFXPtr->Red, SpotFXPtr->Green, SpotFXPtr->Blue, ( 2.0F * BLOCKSIZE ) );
																break;

															case SPOTFX_BeardAfterburner:
																if( BikeExhausts ) CreateSpotFXBeardAfterburner( &Pos, &DirVector, SpotFXGroup, Speed );
																break;

															case SPOTFX_Hk5Afterburner:
																if( BikeExhausts ) CreateSpotFXHk5Afterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed, 32, 32, 128 );
																break;

															case SPOTFX_SoundFX:
																if( !( Models[ i ].SpotFXFlags[ Count ] & SPOTFX_FLAGS_DoneOnce ) )
																{
																	if( SpotFXPtr->SoundFX != -1 )
																	{
																		if( EngineEnabled( Models[i].OwnerType, Models[i].Owner ) )
																		{
																			if( !Models[ i ].SpotFXSFX_ID[ Count ] )
																			{
																				Models[ i ].SpotFXSFX_ID[ Count ] = PlaySpotSfx( SpotFXPtr->SoundFX, &Models[i].Group, &Models[i].Pos,
																												  SpotFXPtr->SoundFXSpeed, SpotFXPtr->SoundFXVolume, SPOT_SFX_TYPE_Normal );
																				Models[ i ].SpotFXFlags[ Count ] |= SPOTFX_FLAGS_DoneOnce;
																			}
																			else
																			{
																				StopSfx( Models[i].SpotFXSFX_ID[ Count ] );
																				Models[i].SpotFXSFX_ID[ Count ] = 0;

																				Models[ i ].SpotFXSFX_ID[ Count ] = PlaySpotSfx( SpotFXPtr->SoundFX, &Models[i].Group, &Models[i].Pos,
																												  SpotFXPtr->SoundFXSpeed, SpotFXPtr->SoundFXVolume, SPOT_SFX_TYPE_Normal );
																				Models[ i ].SpotFXFlags[ Count ] |= SPOTFX_FLAGS_DoneOnce;
#ifdef DEBUG_SPOTFX_SOUND
																				Msg( "Model:SpotFX_SoundFX Overwriting SFX ID %d\n", Models[i].ModelNum );
#endif
																			}
																		}
																		else
																		{
																			if( Models[ i ].SpotFXSFX_ID[ Count ] )
																			{
																				StopSfx( Models[i].SpotFXSFX_ID[ Count ] );
																				Models[i].SpotFXSFX_ID[ Count ] = 0;
																			}
																		}
																	}

																}
																break;

															default:
																break;
														}
													}
												}
												break;

											case SPOTFX_Explosion:
											case SPOTFX_FireSecondary:
											case SPOTFX_FirePrimary:
											case SPOTFX_NmeGlow:
											case SPOTFX_Shrapnel:
											case SPOTFX_Drip:
											case SPOTFX_BorgAfterburner:
											case SPOTFX_ExcopAfterburner:
											case SPOTFX_TruckerAfterburner:
											case SPOTFX_NubiaAfterburner:
											case SPOTFX_CerberoAfterburner:
											case SPOTFX_FoetoidAfterburner:
											case SPOTFX_FoetoidSmallAfterburner:
											case SPOTFX_JapBirdAfterburner:
											case SPOTFX_JoAfterburner:
											case SPOTFX_JoSmallAfterburner:
											case SPOTFX_LaJayAfterburner:
											case SPOTFX_MofistoAfterburner:
											case SPOTFX_NutterAfterburner:
											case SPOTFX_RhesusAfterburner:
											case SPOTFX_SharkAfterburner:
											case SPOTFX_SlickAfterburner:
												ApplyMatrix( &Models[i].Mat, &SpotFXPtr->Pos, &Pos );
												ApplyMatrix( &Models[i].Mat, &SpotFXPtr->DirVector, &DirVector );
												ApplyMatrix( &Models[i].Mat, &SpotFXPtr->UpVector, &UpVector );

												Pos.x += Models[i].Pos.x;
												Pos.y += Models[i].Pos.y;
												Pos.z += Models[i].Pos.z;

												SpotFXGroup = Models[i].Group;

												for( Count2 = 0; Count2 < VisNum; Count2++ )
												{
													if( Bsp_Header[ 0 ].State )
													{
														if( PointInsideSkin( &Pos, VisGroups[ Count2 ] ) )
														{
															SpotFXGroup = VisGroups[ Count2 ];
															break;
														}
													}
													else
													{
														if( PointInGroupBoundingBox( &Mloadheader, &Pos, VisGroups[ Count2 ] ) )
														{
															SpotFXGroup = VisGroups[ Count2 ];
															break;
														}
													}
												}

												if( GroupsAreVisible( SpotFXGroup, Ships[ Current_Camera_View ].Object.Group ) )
												{
													switch( SpotFXPtr->Type )
													{
														case SPOTFX_Explosion:
															if( !( Models[ i ].SpotFXFlags[ Count ] & SPOTFX_FLAGS_DoneOnce ) )
															{
																CreateSpotFXExplosion( &Pos, &DirVector, SpotFXGroup );
																Models[ i ].SpotFXFlags[ Count ] |= SPOTFX_FLAGS_DoneOnce;
															}
															break;

														case SPOTFX_FireSecondary:
															if( !( Models[ i ].SpotFXFlags[ Count ] & SPOTFX_FLAGS_DoneOnce ) )
															{
																CreateSpotFXFireSecondary( &Pos, &DirVector, &UpVector, SpotFXGroup, SpotFXPtr->Secondary, Models[ i ].OwnerType, Models[ i ].Owner );
																Models[ i ].SpotFXFlags[ Count ] |= SPOTFX_FLAGS_DoneOnce;
															}
															break;

														case SPOTFX_FirePrimary:
															if( !( Models[ i ].SpotFXFlags[ Count ] & SPOTFX_FLAGS_DoneOnce ) )
															{
																CreateModelSpotFXFirePrimary( &Pos, &DirVector, &UpVector, SpotFXGroup, SpotFXPtr->Primary, i, Count );
																Models[ i ].SpotFXFlags[ Count ] |= SPOTFX_FLAGS_DoneOnce;
															}
															break;

														case SPOTFX_NmeGlow:
															CreateSpotFXNmeGlow( &Pos, &DirVector, SpotFXGroup, SpotFXPtr->Red, SpotFXPtr->Green, SpotFXPtr->Blue );
															break;

														case SPOTFX_Shrapnel:
															if( !( Models[ i ].SpotFXFlags[ Count ] & SPOTFX_FLAGS_DoneOnce ) )
															{
																CreateSpotFXShrapnel( &Pos, &DirVector, SpotFXGroup );
																Models[ i ].SpotFXFlags[ Count ] |= SPOTFX_FLAGS_DoneOnce;
															}
															break;

														case SPOTFX_Drip:
															if( !( Models[ i ].SpotFXFlags[ Count ] & SPOTFX_FLAGS_DoneOnce ) )
															{
																CreateSpotFXDrip( &Pos, &DirVector, SpotFXGroup, SpotFXPtr->Red, SpotFXPtr->Green, SpotFXPtr->Blue );
																Models[ i ].SpotFXFlags[ Count ] |= SPOTFX_FLAGS_DoneOnce;
															}
															break;

														case SPOTFX_BorgAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_ExcopAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_TruckerAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_NubiaAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_CerberoAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_FoetoidAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_FoetoidSmallAfterburner:
															if( BikeExhausts ) CreateSpotFXSmallAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_JapBirdAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_JoAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_JoSmallAfterburner:
															if( BikeExhausts ) CreateSpotFXSmallAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_LaJayAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_MofistoAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_NutterAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_RhesusAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_SharkAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														case SPOTFX_SlickAfterburner:
															if( BikeExhausts ) CreateSpotFXBorgAfterburner( &Pos, &DirVector, SpotFXGroup, ShipSpeed );
															break;

														default:
															break;
													}
												}
												break;
										}
									}

									Models[i].SpotFXDelay[ Count ] -= framelag;

									if( Models[i].SpotFXDelay[ Count ] < 0.0F )
									{
										if( SpotFXPtr->InactiveDelay < 0.0F )
										{
											Models[i].SpotFXDelay[ Count ] = -1.0F;

											switch( SpotFXPtr->Type )
											{
												case SPOTFX_SoundFX:
													if( ( SpotFXPtr->SoundFX != -1 ) &&
														( Models[ i ].SpotFXSFX_ID[ Count ] ) )
													{
														if( EngineEnabled( Models[i].OwnerType, Models[i].Owner ) )
														{
															Models[i].SpotFXSFX_ID[ Count ] = EngineCode( Models[i].OwnerType, Models[i].Owner, Models[i].SpotFXSFX_ID[ Count ], Speed );
														}
														else
														{
															StopSfx( Models[i].SpotFXSFX_ID[ Count ] );
															Models[i].SpotFXSFX_ID[ Count ] = 0;
															Models[i].SpotFXFlags[ Count ] &= ~SPOTFX_FLAGS_DoneOnce;
														}
													}
													break;

												default:
													break;
											}
										}
										else
										{
											if( SpotFXPtr->InactiveDelay == 0.0F )
											{
												Models[i].SpotFXDelay[ Count ] += SpotFXPtr->ActiveDelay;
											}
											else
											{
												if( SpotFXPtr->SoundFX != -1 )
												{
													if( Models[i].SpotFXSFX_ID[ Count ] )
													{
														StopSfx( Models[i].SpotFXSFX_ID[ Count ] );
														Models[i].SpotFXSFX_ID[ Count ] = 0;
													}
												}

												Models[i].SpotFXDelay[ Count ] += SpotFXPtr->InactiveDelay;
												Models[i].SpotFXState[ Count ] = SPOTFX_STATE_Inactive;
											}
										}
									}
									break;

								case SPOTFX_STATE_Inactive:
									Models[i].SpotFXDelay[ Count ] -= framelag;

									if( Models[i].SpotFXDelay[ Count ] < 0.0F )
									{
										Models[i].SpotFXFlags[ Count ] &= ~SPOTFX_FLAGS_DoneOnce;
										Models[i].SpotFXDelay[ Count ] += SpotFXPtr->ActiveDelay;
										Models[i].SpotFXState[ Count ] = SPOTFX_STATE_Active;
										Models[i].SpotFXTimeCount[ Count ] = 0.0F;
									}
									break;
							}
						}

						SpotFXPtr++;
					}
					break;
			}

			Models[i].OldPos = Models[i].Pos;
		}
		else
		{
			switch( Models[i].Func )
			{
				case MODFUNC_ProcessSpotFX:
					SpotFXPtr = NULL;
					NumSpotFX = 0;

					if( ModelNames[ Models[i].ModelNum ].DoIMorph )
					{
						if( MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
							MxaModelHeaders[ Models[i].ModelNum ].SpotFX )
						{
							SpotFXPtr = MxaModelHeaders[ Models[i].ModelNum ].SpotFX;
							NumSpotFX = MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX;
						}
					}
					else
					{
						if( ModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
							ModelHeaders[ Models[i].ModelNum ].SpotFX )
						{
							SpotFXPtr = ModelHeaders[ Models[i].ModelNum ].SpotFX;
							NumSpotFX = ModelHeaders[ Models[i].ModelNum ].NumSpotFX;
						}
					}

					if( !SpotFXPtr || !NumSpotFX ) break;

					for( Count = 0; Count < NumSpotFX; Count++ )
					{
						switch( SpotFXPtr->Type )
						{
							case SPOTFX_SoundFX:
								if( SpotFXPtr->SoundFX != -1 )
								{
									if( Models[i].SpotFXSFX_ID[ Count ] )
									{
										StopSfx( Models[i].SpotFXSFX_ID[ Count ] );
										Models[i].SpotFXSFX_ID[ Count ] = 0;
										Models[i].SpotFXFlags[ Count ] &= ~SPOTFX_FLAGS_DoneOnce;
									}
								}
								break;

							default:
								break;
						}
						SpotFXPtr++;
					}
					break;
			}
		}

		i = nextmodel;
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Create SpotFX Fire Primary
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	VECTOR	*	Up
				:	uint16		Group
				:	uint16		Weapon
				:	uint16		ModelIndex
				:	uint16		SpotFXIndex
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void CreateModelSpotFXFirePrimary( VECTOR * Pos, VECTOR * Dir, VECTOR * Up,
							    uint16 Group, uint8 Weapon, uint16 ModelIndex, uint16 SpotFXIndex )
{
	VECTOR	TempVector = { 0.0F, 0.0F, 0.0F };
	uint16	i;

	if( Weapon != -1 )
	{
		i = EnemyFirePrimary( OWNER_MODELSPOTFX, ModelIndex, 0, Weapon, Group, Pos, &TempVector, Dir, Up, 2, 0.0F, FALSE, NULL );

		if( i != (uint16) -1 )
		{
			PrimBulls[i].SpotFX = SpotFXIndex;
			GetLaserLocalVector( i, &PrimBulls[i].LocalDir );
		}
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Process model
	Input		:	uint16	Model Number
				:	float	Scale
				:	float	MaxScale
				:	int8	R
				:	int8	G
				:	int8	B
	Output		:	BOOL	True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ProcessModel( MXLOADHEADER * DstMloadheader, float Scale, float MaxScale, int8 R, int8 G, int8 B )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	D3DCOLOR				Colour;
	float					Col2;
	uint8					Col;
	uint8					Red;
	uint8					Green;
	uint8					Blue;

	Col2 = ( ( Scale * ( 128.0F / MaxScale ) ) + 128.0F );
	if( Col2 > 255.0F ) Col2 = 255.0F;
	Col = (uint8) ( 255.0F - Col2 );

	if( R ) Red = Col;
	else Red = 0;
	if( G ) Green = Col;
	else Green = 0;
	if( B ) Blue = Col;
	else Blue = 0;

	if( PowerVR )
	{
		Colour = RGBA_MAKE( Red, Green, Blue, 128 );
	}
	else
	{
		if( UsedStippledAlpha != TRUE )
		{
#if ACTUAL_TRANS
			Colour = RGBA_MAKE( Red, Green, Blue, Col );
#else
			Colour = RGBA_MAKE( Red, Green, Blue, 255 );
#endif
		}
		else
		{
			Colour = RGBA_MAKE( Red, Green, Blue, Col );
		}
	}

	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{
			memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
			DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

			DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

			Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

			while( Vert-- )
			{
				DstlpD3DLVERTEX->color = Colour;
				DstlpD3DLVERTEX++;
			}

			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
		}
	}
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Process model scaling original gouraud
	Input		:	uint16	Model Number
				:	float	Scale
				:	float	MaxScale
				:	int8	R
				:	int8	G
				:	int8	B
	Output		:	BOOL	True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ProcessModel2( MXLOADHEADER * DstMloadheader, float Scale, float MaxScale, int8 R, int8 G, int8 B )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	float					Col2;
	uint16					Col;
	uint16					Red;
	uint16					Green;
	uint16					Blue;
	
	//D3DCOLOR	*			OrigColours;
	LPD3DLVERTEX			VertPtr;

	//D3DCOLOR_RGBA *			OrigRGBA;
	D3DCOLOR_RGBA *			DestRGBA;

	Col2 = ( ( Scale / MaxScale ) * 255.0F );
	if( Col2 > 255.0F ) Col2 = 255.0F;
	Col = (uint16) ( 255.0F - Col2 );

	if( R ) Red = Col;
	else Red = 0;
	if( G ) Green = Col;
	else Green = 0;
	if( B ) Blue = Col;
	else Blue = 0;

	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{
			memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
			DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

			DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

			Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];
//			OrigColours = DstMloadheader->Group[ Group ].org_colors[ ExecBuf ];
			VertPtr = DstMloadheader->Group[ Group ].org_vertpnt[ ExecBuf ];

			while( Vert-- )
			{
#if 0
				OrigRGBA = (D3DCOLOR_RGBA *) OrigColours;
				DestRGBA = (D3DCOLOR_RGBA *) &DstlpD3DLVERTEX->color;
				DestRGBA->r = (uint8) ( ( (uint16) OrigRGBA->r * Red ) >> 8 );
				DestRGBA->g = (uint8) ( ( (uint16) OrigRGBA->g * Green ) >> 8 );
				DestRGBA->b = (uint8) ( ( (uint16) OrigRGBA->b * Blue ) >> 8 );
				OrigColours++;
				DstlpD3DLVERTEX++;
#endif
				DestRGBA = (D3DCOLOR_RGBA *) &DstlpD3DLVERTEX->color;

				DestRGBA->r = (uint8) ( ( (uint16) (RGBA_GETRED(VertPtr->color) * Red) ) >> 8 );
				DestRGBA->g = (uint8) ( ( (uint16) (RGBA_GETGREEN(VertPtr->color) * Green) ) >> 8 );
				DestRGBA->b = (uint8) ( ( (uint16) (RGBA_GETBLUE(VertPtr->color) * Blue) ) >> 8 );

				VertPtr++;
				DstlpD3DLVERTEX++;
				
			}

			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
		}
	}
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Process model
	Input		:	LPDIRECT3DEXECUTEBUFFER	lpExBuf
				:	int16	NumVerts
				:	float	Scale
				:	float	MaxScale
				:	int8	R
				:	int8	G
				:	int8	B
	Output		:	BOOL	True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ProcessModelExec( LPDIRECT3DEXECUTEBUFFER lpExBuf, int16 NumVerts, float Scale, float MaxScale, int8 R, int8 G, int8 B )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	D3DCOLOR				Colour;
	float					Col2;
	uint8					Col;
	uint8					Red;
	uint8					Green;
	uint8					Blue;

	Col2 = ( ( Scale * ( 128.0F / MaxScale ) ) + 128.0F );
	if( Col2 > 255.0F ) Col2 = 255.0F;
	Col = (uint8) ( 255.0F - Col2 );

	if( R ) Red = Col;
	else Red = 0;
	if( G ) Green = Col;
	else Green = 0;
	if( B ) Blue = Col;
	else Blue = 0;

	if( PowerVR )
	{
		Colour = RGBA_MAKE( Red, Green, Blue, 128 );
	}
	else
	{
		if( UsedStippledAlpha != TRUE )
		{
#if ACTUAL_TRANS
			Colour = RGBA_MAKE( Red, Green, Blue, Col );
#else
			Colour = RGBA_MAKE( Red, Green, Blue, 255 );
#endif
		}
		else
		{
			Colour = RGBA_MAKE( Red, Green, Blue, Col );
		}
	}

	memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	if( lpExBuf->lpVtbl->Lock( lpExBuf, &DstDebDesc ) != D3D_OK ) return FALSE;
	
	DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;
	
	while( NumVerts-- )
	{
		DstlpD3DLVERTEX->color = Colour;
		DstlpD3DLVERTEX++;
	}
	
	if( lpExBuf->lpVtbl->Unlock( lpExBuf ) != D3D_OK )	return FALSE;

	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Process Sphere Zone model
	Input		:	LPDIRECT3DEXECUTEBUFFER	lpExBuf
				:	int16	NumVerts
				:	uint8	R
				:	uint8	G
				:	uint8	B
	Output		:	BOOL	True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ProcessSphereZoneModelExec( LPDIRECT3DEXECUTEBUFFER lpExBuf, int16 NumVerts, uint8 R, uint8 G, uint8 B )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	D3DCOLOR				Colour;

	if( PowerVR )
	{
		Colour = RGBA_MAKE( R, G, B, 128 );
	}
	else
	{
		if( UsedStippledAlpha != TRUE )
		{
#if ACTUAL_TRANS
			Colour = RGBA_MAKE( R, G, B, 255 );
#else
			Colour = RGBA_MAKE( R, G, B, 255 );
#endif
		}
		else
		{
			Colour = RGBA_MAKE( R, G, B, 255 );
		}
	}

	memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	if( lpExBuf->lpVtbl->Lock( lpExBuf, &DstDebDesc ) != D3D_OK ) return FALSE;
	
	DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;
	
	while( NumVerts-- )
	{
		DstlpD3DLVERTEX->color = Colour;
		DstlpD3DLVERTEX++;
	}
	
	if( lpExBuf->lpVtbl->Unlock( lpExBuf ) != D3D_OK )	return FALSE;

	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Create Tracker
	Input		:	Nothing
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void UpdateTracker( void )
{
	VECTOR	Pos;
	VECTOR	Offset;
	uint16	i;

	if( Tracker == (uint16 ) -1 ) CreateTracker();

	if( Tracker != (uint16 ) -1 )
	{
		Offset.x = ( -400.0F * GLOBAL_SCALE );
		Offset.y = ( +300.0F * GLOBAL_SCALE );
		Offset.z = ( +640.0F * GLOBAL_SCALE );
		ApplyMatrix( &Ships[ WhoIAm ].Object.FinalMat, &Offset, &Pos );
		Pos.x += Ships[ WhoIAm ].Object.Pos.x;
		Pos.y += Ships[ WhoIAm ].Object.Pos.y;
		Pos.z += Ships[ WhoIAm ].Object.Pos.z;
		Models[ Tracker ].Pos = Pos;
		Models[ Tracker ].Group = Ships[ WhoIAm ].Object.Group;
	}

	if( TrackerTarget != (uint16 ) -1 )
	{
		i = FindClosestPickup();

		Offset.x = ( -400.0F * GLOBAL_SCALE );
		Offset.y = ( +300.0F * GLOBAL_SCALE );
		Offset.z = ( +640.0F * GLOBAL_SCALE );
		ApplyMatrix( &Ships[ WhoIAm ].Object.FinalMat, &Offset, &Offset );
		Offset.x += Ships[ WhoIAm ].Object.Pos.x;
		Offset.y += Ships[ WhoIAm ].Object.Pos.y;
		Offset.z += Ships[ WhoIAm ].Object.Pos.z;

		Pos.x = ( ( Pickups[ i ].Pos.x - Ships[ WhoIAm ].Object.Pos.x ) / 50.0F );
		Pos.y = ( ( Pickups[ i ].Pos.y - Ships[ WhoIAm ].Object.Pos.y ) / 50.0F );
		Pos.z = ( ( Pickups[ i ].Pos.z - Ships[ WhoIAm ].Object.Pos.z ) / 50.0F );

		Models[ TrackerTarget ].Mat = Ships[ WhoIAm ].Object.FinalMat;
		MatrixTranspose( &Models[ TrackerTarget ].Mat, &Models[ TrackerTarget ].InvMat );

		Models[ TrackerTarget ].Pos.x = Pos.x + Offset.x;
		Models[ TrackerTarget ].Pos.y = Pos.y + Offset.y;
		Models[ TrackerTarget ].Pos.z = Pos.z + Offset.z;
		Models[ TrackerTarget ].Group = Ships[ WhoIAm ].Object.Group;
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Create Tracker
	Input		:	Nothing
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void CreateTracker( void )
{
	uint16	Model;

	Model =	FindFreeModel();

	if( Model != (uint16 ) -1 )
	{
		Models[ Model ].OwnerType = OWNER_NOBODY;
		Models[ Model ].Owner = 0;
		Models[ Model ].ModelNum = MODEL_Tracker;
		Models[ Model ].Type = MODTYPE_Missile;
		Models[ Model ].Flags = MODFLAG_Clip;
		Models[ Model ].Visible = TRUE;
		Models[ Model ].Pos = Ships[ WhoIAm ].Object.Pos;
		Models[ Model ].Dir.x = 0.0F;
		Models[ Model ].Dir.y = 0.0F;
		Models[ Model ].Dir.z = 0.0F;
		Models[ Model ].Rot.x = 0.0F;
		Models[ Model ].Rot.y = 0.0F;
		Models[ Model ].Rot.z = 0.0F;
		Models[ Model ].Quat.w = 1.0F;
		Models[ Model ].Quat.x = 0.0F;
		Models[ Model ].Quat.y = 0.0F;
		Models[ Model ].Quat.z = 0.0F;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_Nothing;
		Models[ Model ].Scale = 1.0F;
		Models[ Model ].MaxScale = 1.0F;
		Models[ Model ].Group = Ships[ WhoIAm ].Object.Group;
		Models[ Model ].LifeCount = (float) -0.0F;
	}

	Tracker = Model;

	Model =	FindFreeModel();

	if( Model != (uint16 ) -1 )
	{
		Models[ Model ].OwnerType = OWNER_NOBODY;
		Models[ Model ].Owner = 0;
		Models[ Model ].ModelNum = MODEL_Ping;
		Models[ Model ].Type = MODTYPE_Missile;
		Models[ Model ].Flags = MODFLAG_Clip;
		Models[ Model ].Visible = TRUE;
		Models[ Model ].Pos = Ships[ WhoIAm ].Object.Pos;
		Models[ Model ].Dir.x = 0.0F;
		Models[ Model ].Dir.y = 0.0F;
		Models[ Model ].Dir.z = 0.0F;
		Models[ Model ].Rot.x = 0.0F;
		Models[ Model ].Rot.y = 0.0F;
		Models[ Model ].Rot.z = 0.0F;
		Models[ Model ].Quat.w = 1.0F;
		Models[ Model ].Quat.x = 0.0F;
		Models[ Model ].Quat.y = 0.0F;
		Models[ Model ].Quat.z = 0.0F;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_RealScale;
		Models[ Model ].Scale = 1.0F; //0.125F;
		Models[ Model ].MaxScale = 1.0F;
		Models[ Model ].Group = Ships[ WhoIAm ].Object.Group;
		Models[ Model ].LifeCount = (float) -0.0F;
	}

	TrackerTarget = Model;

}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Light model
	Input		:	uint16		Model Number
				:	float		R,G,B,Trans
	Output		:	BOOL		True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL TintModel( uint16 Model, float RF, float GF, float BF, float TF )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	MXLOADHEADER	*		DstMloadheader;
	D3DCOLOR				Colour;

	DstMloadheader = &ModelHeaders[ Model ];

	Colour = RGBA_MAKE( (uint8) RF, (uint8) GF, (uint8) BF, (uint8) TF );
	if( !Colour ) return FALSE;

	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{
			memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
			DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

			DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

			Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

			while( Vert-- )
			{
				DstlpD3DLVERTEX->color = Colour;
				DstlpD3DLVERTEX++;
			}

			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
		}
	}
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Light model
	Input		:	uint16		Model Number
				:	float		R,G,B,Trans
	Output		:	BOOL		True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL TintMxaModel( MXALOADHEADER * DstMloadheader, float RF, float GF, float BF, float TF )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	D3DCOLOR				Colour;

	Colour = RGBA_MAKE( (uint8) RF, (uint8) GF, (uint8) BF, (uint8) TF );
	if( !Colour ) return FALSE;

	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{
			memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
			DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

			DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

			Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

			while( Vert-- )
			{
				DstlpD3DLVERTEX->color = Colour;
				DstlpD3DLVERTEX++;
			}

			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
		}
	}
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Update Colours
	Input		:	MXALOADHEADER	ModelHeader
	Output		:	BOOL	True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL UpdateMxaModel( MXALOADHEADER * MXAloadheader )
{
    LPD3DLVERTEX	lpPointer;
	LPD3DLVERTEX	lpD3DLVERTEX;
    LPD3DLVERTEX	lpD3DLVERTEX2;
	D3DEXECUTEBUFFERDESC	debDesc;
	int		group;
	int		execbuf;
	int		vert;

	group = MXAloadheader->num_groups;

	while( group--)
	{
		execbuf = MXAloadheader->Group[group].num_execbufs;
		while( execbuf--)
		{
			memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
			debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			/*	lock the execute buffer	*/
			if ( MXAloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Lock( MXAloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
				return FALSE ;
			lpPointer = (LPD3DLVERTEX) debDesc.lpData;

			lpD3DLVERTEX2 = MXAloadheader->Group[group].org_vertpnt[execbuf];
			
			lpD3DLVERTEX = lpPointer;
			vert = MXAloadheader->Group[group].num_verts_per_execbuf[execbuf];
			while( vert --)
			{
				lpD3DLVERTEX->color = lpD3DLVERTEX2->color;
				lpD3DLVERTEX->color |= 0xFF000000;
				lpD3DLVERTEX++;		
				lpD3DLVERTEX2++;		
			}
			/*	unlock the execute buffer	*/
			if ( MXAloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( MXAloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
				return FALSE;
		}
	}
	return TRUE;
}


/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Ambient Light model
	Input		:	uint16		Model Number
				:	float		R,G,B,Trans
	Output		:	BOOL		True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL AmbientLightMxaModel( MXALOADHEADER * DstMloadheader, int R, int G, int B, int A , float rp , float gp , float bp)
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	LPD3DLVERTEX			SrclpD3DLVERTEX;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	D3DCOLOR				Colour;
#ifdef FAST_RGB_CLAMP
#ifndef USEASM
	uint32					tempiA;
	uint32					carry, clamp;
#endif
	uint32					tempiR, tempiG, tempiB;
	uint32					col_inc, inc_inv, inc_carry;
#else
	int r ,g ,b ,a;
#endif

	R -= (int) rp;
	G -= (int) gp;
	B -= (int) bp;

	if( R < 0 )
		R = 0;
	if( G < 0 )
		G = 0;
	if( B < 0 )
		B = 0;

#ifdef FAST_RGB_CLAMP
	tempiR = ( (int) -( R ) ) & 0xFF;
	tempiG = ( (int) -( G ) ) & 0xFF;
	tempiB = ( (int) -( B ) ) & 0xFF;
	// so far so much nearly the same...
	col_inc = ( tempiR << 16 ) + ( tempiG << 8 ) + tempiB;
	inc_inv = col_inc ^ 0x00FFFFFF;
	inc_carry = ( ( inc_inv + 0x00010101) ^ ( inc_inv ^ 0x00010101 ) ) & 0x01010100;
#endif
	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{
			memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
			DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

			DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;
			SrclpD3DLVERTEX = DstMloadheader->Group[ Group ].org_vertpnt[ExecBuf];
			Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

			while( Vert-- )
			{
				Colour = SrclpD3DLVERTEX->color;
#ifdef FAST_RGB_CLAMP
#ifdef	USEASM
__asm
{
				mov	ecx , Colour		;int ecx = col   get the color

				mov edi , ecx			;edi = col..
				and ecx , 0x00ffffff	;and out the alpha
				and edi , 0xff000000	;keep the alpha for later
				
				mov ebx , col_inc		;ebx = col_inc
				mov edx , ecx			;edx = col

				add ecx , ebx			;ecx = col + col_inc
				xor edx , ebx			;edx = col ^ col_inc
				xor edx , ecx			;edx = ( col + col_inc ) ^ ( col ^ col_inc )
				and edx	, 0x01010100	;edx = carry = ( ( col + col_inc ) ^ ( col ^ col_inc ) ) & 0x01010100
				sub ecx , edx			;ecx = (col + col_inc) - carry
				mov ebx , inc_carry		;ebx = inc_carry
				or  edx , ebx			;edx = carry = carry | inc_carry
				mov eax , edx			;eax = carry
				shr	edx , 8				;edx = ( carry >> 8 )
				sub eax , edx			;eax = clamp = carry - ( carry >> 8 )
				and ecx , eax			;col = ecx & clamp

				or  ecx , edi			;or back in the alpha..

				mov Colour, ecx			;int put the color back
}
#else	//USEASM
				tempiA = Colour & 0xff000000;
				// for subtraction, simply add the negative
				carry = ( ( Colour + col_inc ) ^ ( Colour ^ col_inc ) ) & 0x01010100;
				Colour = Colour + col_inc - carry;
				// set the carry also for those channels where we are subtracting 0 (this seems wrong!)
				carry |= inc_carry;
				// ...and now it's nearly the same as for additive saturation
				clamp = carry - ( carry >> 8 );
				Colour = ( Colour & clamp ) & 0x00ffffff;
				Colour |= tempiA;
#endif // USEASM
#else
				r = RGBA_GETRED(Colour);
				g = RGBA_GETGREEN(Colour);
				b = RGBA_GETBLUE(Colour);
				a = RGBA_GETALPHA(Colour);
				r -= R;
				g -= G;
				b -= B;
//				a -= A;
				if( r < 0 )
					r = 0;
				if( g < 0 )
					g = 0;
				if( b < 0 )
					b = 0;
//				if( a < 0 )
//					a = 0;
				Colour = RGBA_MAKE( r, g, b, a);
#endif
				DstlpD3DLVERTEX->color = Colour;
				DstlpD3DLVERTEX++;
				SrclpD3DLVERTEX++;
			}

			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
		}
	}
	return TRUE;
}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Ambient Light MX model
	Input		:	uint16		Model Number
				:	float		R,G,B,Trans
	Output		:	BOOL		True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL AmbientLightMxModel( MXLOADHEADER * DstMloadheader, int R, int G, int B, int A , float rp , float gp , float bp)
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	LPD3DLVERTEX			SrclpD3DLVERTEX;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	D3DCOLOR				Colour;
#ifdef FAST_RGB_CLAMP
#ifndef USEASM
	uint32					tempiA;
	uint32					carry, clamp;
#endif
	uint32					tempiR, tempiG, tempiB;
	uint32					col_inc, inc_inv, inc_carry;
#else
	int r ,g ,b ,a;
#endif

	R -= (int) rp;
	G -= (int) gp;
	B -= (int) bp;

	if( R < 0 )
		R = 0;
	if( G < 0 )
		G = 0;
	if( B < 0 )
		B = 0;


#ifdef FAST_RGB_CLAMP
	tempiR = ( (int) -( R ) ) & 0xFF;
	tempiG = ( (int) -( G ) ) & 0xFF;
	tempiB = ( (int) -( B ) ) & 0xFF;
	// so far so much nearly the same...
	col_inc = ( tempiR << 16 ) + ( tempiG << 8 ) + tempiB;
	inc_inv = col_inc ^ 0x00FFFFFF;
	inc_carry = ( ( inc_inv + 0x00010101) ^ ( inc_inv ^ 0x00010101 ) ) & 0x01010100;
#endif
	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{
			memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
			DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

			DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;
			SrclpD3DLVERTEX = DstMloadheader->Group[ Group ].org_vertpnt[ExecBuf];
			Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

			while( Vert-- )
			{
				Colour = SrclpD3DLVERTEX->color;
#ifdef FAST_RGB_CLAMP
#ifdef	USEASM
__asm
{
				mov	ecx , Colour		;int ecx = col   get the color

				mov edi , ecx			;edi = col..
				and ecx , 0x00ffffff	;and out the alpha
				and edi , 0xff000000	;keep the alpha for later
				
				mov ebx , col_inc		;ebx = col_inc
				mov edx , ecx			;edx = col

				add ecx , ebx			;ecx = col + col_inc
				xor edx , ebx			;edx = col ^ col_inc
				xor edx , ecx			;edx = ( col + col_inc ) ^ ( col ^ col_inc )
				and edx	, 0x01010100	;edx = carry = ( ( col + col_inc ) ^ ( col ^ col_inc ) ) & 0x01010100
				sub ecx , edx			;ecx = (col + col_inc) - carry
				mov ebx , inc_carry		;ebx = inc_carry
				or  edx , ebx			;edx = carry = carry | inc_carry
				mov eax , edx			;eax = carry
				shr	edx , 8				;edx = ( carry >> 8 )
				sub eax , edx			;eax = clamp = carry - ( carry >> 8 )
				and ecx , eax			;col = ecx & clamp

				or  ecx , edi			;or back in the alpha..

				mov Colour, ecx			;int put the color back
}
#else	//USEASM
				tempiA = Colour & 0xff000000;
				// for subtraction, simply add the negative
				carry = ( ( Colour + col_inc ) ^ ( Colour ^ col_inc ) ) & 0x01010100;
				Colour = Colour + col_inc - carry;
				// set the carry also for those channels where we are subtracting 0 (this seems wrong!)
				carry |= inc_carry;
				// ...and now it's nearly the same as for additive saturation
				clamp = carry - ( carry >> 8 );
				Colour = ( Colour & clamp ) & 0x00ffffff;
				Colour |= tempiA;
#endif // USEASM
#else
				r = RGBA_GETRED(Colour);
				g = RGBA_GETGREEN(Colour);
				b = RGBA_GETBLUE(Colour);
				a = RGBA_GETALPHA(Colour);
				r -= R;
				g -= G;
				b -= B;
//				a -= A;
				if( r < 0 )
					r = 0;
				if( g < 0 )
					g = 0;
				if( b < 0 )
					b = 0;
//				if( a < 0 )
//					a = 0;
				Colour = RGBA_MAKE( r, g, b, a);
#endif
				DstlpD3DLVERTEX->color = Colour;
				DstlpD3DLVERTEX++;
				SrclpD3DLVERTEX++;
			}

			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
		}
	}
	return TRUE;
}




/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Light model
	Input		:	uint16		Model Number
				:	MATRIX	*	Matrix
				:	float		Z Translation
				:	float		Range
	Output		:	BOOL		True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ShadeModel( uint16 Model, MATRIX * Matrix, float ZTrans, float Range )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	MXLOADHEADER	*		DstMloadheader;
	D3DCOLOR				Colour;
	float					RF, GF, BF, TF;
	VECTOR					TempPoint;

	DstMloadheader = &ModelHeaders[ Model ];

	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{
			memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
			DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

			DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

			Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

			while( Vert-- )
			{
				ApplyMatrix( Matrix, (VECTOR*) DstlpD3DLVERTEX, &TempPoint );
				TempPoint.z += ZTrans;
				if( TempPoint.z > Range )
				{
					RF = 0.0F; GF = 0.0F; BF = 0.0F; TF = 0.0F;
				}
				else
				{
					if( TempPoint.z < 0.0F ) 
					{
						RF = 255.0F; GF = 255.0F; BF = 255.0F; TF = 255.0F;
					}
					else
					{
						RF = ( 255.0F - ( ( TempPoint.z / Range ) * 255.0F ) );
						GF = RF; BF = RF;
						TF = 255.0F;
					}
				}

				Colour = RGBA_MAKE( (uint8) RF, (uint8) GF, (uint8) BF, (uint8) TF );

				DstlpD3DLVERTEX->color = Colour;
				DstlpD3DLVERTEX++;
			}

			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
		}
	}
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Light model
	Input		:	uint16		Model Number
				:	MATRIX	*	Matrix
				:	VECTOR	*	Pos
	Output		:	BOOL	True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL LightModel( uint16 Model, VECTOR * Pos )
{
	XLIGHT * LightPnt;
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	VECTOR					DistVector;
	float					Dist;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	MXLOADHEADER	*		DstMloadheader;
	D3DCOLOR				Colour;
    float					Val;
	float					TF = 0.0F;
	float					RF = 0.0F;
	float					GF = 0.0F;
	float					BF = 0.0F;

	DstMloadheader = &ModelHeaders[ Model ];

	LightPnt = FirstLightVisible;
	
	while( LightPnt )
	{
		DistVector.x = ( Pos->x - LightPnt->Pos.x );
		DistVector.y = ( Pos->y - LightPnt->Pos.y );
		DistVector.z = ( Pos->z - LightPnt->Pos.z );
		Dist = VectorLength( &DistVector );
		
		if( Dist < LightPnt->Size )
		{
			Val = ( 1.0F - ( Dist / LightPnt->Size ) );
	
			RF += ( LightPnt->r * Val );
			GF += ( LightPnt->g * Val );
			BF += ( LightPnt->b * Val );
			TF += ( 255.0F * Val );
		}
		LightPnt = LightPnt->NextVisible;
	}
		
	if( RF > 255.0F ) RF = 255.0F;
	if( GF > 255.0F ) GF = 255.0F;
	if( BF > 255.0F ) BF = 255.0F;
	if( TF > 255.0F ) TF = 255.0F;

	Colour = RGBA_MAKE( (uint8) RF, (uint8) GF, (uint8) BF, (uint8) TF );
	if( !Colour ) return FALSE;

	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{
			memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
			DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

			DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

			Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

			while( Vert-- )
			{
				DstlpD3DLVERTEX->color = Colour;
				DstlpD3DLVERTEX++;
			}

			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
		}
	}
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Light model
	Input		:	uint16		Model Number
				:	MATRIX	*	Matrix
				:	VECTOR	*	Pos
	Output		:	BOOL	True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL LightModel2( uint16 Model, VECTOR * Pos )
{
	XLIGHT * LightPnt;
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	VECTOR					DistVector;
	float					Dist;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	MXLOADHEADER	*		DstMloadheader;
	D3DCOLOR				Colour;
    float					Val;
	float					TF = 255.0F;
	float					RF = 128.0F;
	float					GF = 128.0F;
	float					BF = 128.0F;

	DstMloadheader = &ModelHeaders[ Model ];

	LightPnt = FirstLightVisible;
	
	while( LightPnt )
	{
		DistVector.x = ( Pos->x - LightPnt->Pos.x );
		DistVector.y = ( Pos->y - LightPnt->Pos.y );
		DistVector.z = ( Pos->z - LightPnt->Pos.z );
		Dist = VectorLength( &DistVector );
		
		if( Dist < LightPnt->Size )
		{
			Val = ( 1.0F - ( Dist / LightPnt->Size ) );
	
			RF += ( LightPnt->r * Val );
			GF += ( LightPnt->g * Val );
			BF += ( LightPnt->b * Val );
			TF += ( 255.0F * Val );
		}
		LightPnt = LightPnt->NextVisible;
	}
		
	if( RF > 255.0F ) RF = 255.0F;
	if( GF > 255.0F ) GF = 255.0F;
	if( BF > 255.0F ) BF = 255.0F;
	if( TF > 255.0F ) TF = 255.0F;

	Colour = RGBA_MAKE( (uint8) RF, (uint8) GF, (uint8) BF, (uint8) TF );
	if( !Colour ) return FALSE;

	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{
			memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
			DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

			DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

			Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];

			while( Vert-- )
			{
				DstlpD3DLVERTEX->color = Colour;
				DstlpD3DLVERTEX++;
			}

			if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
							DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
		}
	}
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Light MX model
	Input		:	uint16		Model Number
				:	MATRIX	*	Matrix
				:	VECTOR	*	Pos
				:	float		Starting RED ( 0-255 )
				:	float		Starting GREEN ( 0-255 )
				:	float		Starting BLUE ( 0-255 )
				:	float		Starting TRANSPARANCY ( 0-255 )
	Output		:	BOOL	True/False ( Visible/Not )
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL LightMxModel( uint16 Model, VECTOR * Pos, float RF, float GF, float BF, float TF )
{
	XLIGHT * LightPnt;
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	VECTOR					DistVector;
	float					Dist;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	MXLOADHEADER	*		DstMloadheader;
	D3DCOLOR				Colour;
    float					Val;

	DstMloadheader = &ModelHeaders[ Model ];

	if( DstMloadheader->state == TRUE )
	{
		LightPnt = FirstLightVisible;
		while( LightPnt )
		{
			DistVector.x = ( Pos->x - LightPnt->Pos.x );
			DistVector.y = ( Pos->y - LightPnt->Pos.y );
			DistVector.z = ( Pos->z - LightPnt->Pos.z );
			Dist = VectorLength( &DistVector );
			
			if( Dist < LightPnt->Size )
			{
				Val = ( 1.0F - ( Dist / LightPnt->Size ) );
		
				RF += ( LightPnt->r * Val );
				GF += ( LightPnt->g * Val );
				BF += ( LightPnt->b * Val );
				TF += ( 255.0F * Val );
			}
			LightPnt = LightPnt->NextVisible;
		}
		
		if( RF > 255.0F ) RF = 255.0F;
		if( GF > 255.0F ) GF = 255.0F;
		if( BF > 255.0F ) BF = 255.0F;
		if( TF > 255.0F ) TF = 255.0F;

		Colour = RGBA_MAKE( (uint8) RF, (uint8) GF, (uint8) BF, (uint8) TF );
		if( !Colour ) return FALSE;

		for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
		{
			for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
			{
				memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
				DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
				if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
								DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;
	
				DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;
	
				Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];
	
				while( Vert-- )
				{
					DstlpD3DLVERTEX->color = Colour;
					DstlpD3DLVERTEX++;
				}
	
				if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
								DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
			}
		}
	}
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Light MXA model
	Input		:	uint16		Model Number
				:	MATRIX	*	Matrix
				:	VECTOR	*	Pos
				:	float		Starting RED ( 0-255 )
				:	float		Starting GREEN ( 0-255 )
				:	float		Starting BLUE ( 0-255 )
				:	float		Starting TRANSPARANCY ( 0-255 )
	Output		:	BOOL	True/False ( Visible/Not )
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL LightMxaModel( uint16 Model, VECTOR * Pos, float RF, float GF, float BF, float TF )
{
	XLIGHT * LightPnt;
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	VECTOR					DistVector;
	float					Dist;
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	MXALOADHEADER	*		DstMloadheader;
	D3DCOLOR				Colour;
    float					Val;

	DstMloadheader = &MxaModelHeaders[ Model ];

	if( DstMloadheader->state == TRUE )
	{
		LightPnt = FirstLightVisible;
	
		while( LightPnt )
		{
			DistVector.x = ( Pos->x - LightPnt->Pos.x );
			DistVector.y = ( Pos->y - LightPnt->Pos.y );
			DistVector.z = ( Pos->z - LightPnt->Pos.z );
			Dist = VectorLength( &DistVector );
			
			if( Dist < LightPnt->Size )
			{
				Val = ( 1.0F - ( Dist / LightPnt->Size ) );
		
				RF += ( LightPnt->r * Val );
				GF += ( LightPnt->g * Val );
				BF += ( LightPnt->b * Val );
				TF += ( 255.0F * Val );
			}
			LightPnt = LightPnt->NextVisible;
		}
		
		if( RF > 255.0F ) RF = 255.0F;
		if( GF > 255.0F ) GF = 255.0F;
		if( BF > 255.0F ) BF = 255.0F;
		if( TF > 255.0F ) TF = 255.0F;

		Colour = RGBA_MAKE( (uint8) RF, (uint8) GF, (uint8) BF, (uint8) TF );
		if( !Colour ) return FALSE;

		for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
		{
			for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
			{
				memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
				DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
				if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
								DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;
	
				DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;
	
				Vert = DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ];
	
				while( Vert-- )
				{
					DstlpD3DLVERTEX->color = Colour;
					DstlpD3DLVERTEX++;
				}
	
				if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
								DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL DoesLightEffectGroup( MLOADHEADER * Mloadheader , VECTOR * Pos , float size , uint16 group )
{
	VECTOR	Temp;
	Temp.x = Pos->x - Mloadheader->Group[group].center.x;
	if( Temp.x < 0.0F )	Temp.x *= -1.0F;
	Temp.y = Pos->y - Mloadheader->Group[group].center.y;
	if( Temp.y < 0.0F )	Temp.y *= -1.0F;
	Temp.z = Pos->z - Mloadheader->Group[group].center.z;
	if( Temp.z < 0.0F )	Temp.z *= -1.0F;
	if ( (Temp.x <= ( Mloadheader->Group[group].half_size.x + size ) ) &&
		 (Temp.y <= ( Mloadheader->Group[group].half_size.y + size ) ) &&
		 (Temp.z <= ( Mloadheader->Group[group].half_size.z + size ) ) )
	{
		return TRUE;
	}
	return FALSE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Shock Wave Damage to Ships and Mines.
	Input		:	VECTOR	*	Position
				:	float		Radius
				:	uint16		OwnerType
				:	uint16		Owner
				:	float		Damage at center
				:	uint16		Group
				:	BYTE		Weapon
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void ShockWave( VECTOR * Pos, float Radius, uint16 OwnerType, uint16 Owner, float Center_Damage, uint16 Group, BYTE Weapon, uint16 model )
{
//	int16		Count;
	float		Damage;
	uint16		EndGroup;
	VECTOR		DistVector;
	float		DistFromCenter;
	VERT		Int_Point;
	NORMAL		Int_Normal;
	VECTOR		TempVector;
	VECTOR		Recoil;
	uint16		i, Next;
//	char		methodstr[256];
//	char		tempstr[256];
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;

	switch( ColPerspective )
	{
#if 0
		case COLPERS_Server:
			if( IsServer )
			{
				if( OwnerType == OWNER_SHIP || ( ( OwnerType != OWNER_SHIP ) && IsHost ) )
				{
					Server_WhoHitYou = (BYTE)Owner;
					for( Count = 0; Count < MAX_PLAYERS; Count++ )
					{
						if( ( Ships[ Count ].enable ) && ( Ships[ Count ].Object.Mode != LIMBO_MODE ) )
						{
							if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
							{
								DistVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
								DistVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
								DistVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
								DistFromCenter = VectorLength( &DistVector );
						
								if( DistFromCenter < ( Radius + SHIP_RADIUS ) )
								{
									if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
												  Group, &DistVector, (VECTOR *) &Int_Point,
												  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
									{
										Damage = ( Center_Damage - ( DistFromCenter / ( ( Radius + SHIP_RADIUS ) / Center_Damage ) ) );
					
										Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 4.0F ) );
										Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 4.0F ) );
										Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 4.0F ) );
				
										Damage = ( Damage * framelag );
				
										if( Count != WhoIAm )
										{
											if( Weapon != (BYTE) -1 )
											{
												IHitYou( (BYTE) Count, Damage, &Recoil, &TempVector, &TempVector, 0.0F, WEPTYPE_Secondary, Weapon, FRAMELAGED_RECOIL );
											}
											else
											{
												IHitYou( (BYTE) Count, Damage, &Recoil, &TempVector, &TempVector, 0.0F, WEPTYPE_Various, Weapon, FRAMELAGED_RECOIL );
											}
										}
										else
										{
											if( !Ships[ WhoIAm ].Invul )
											{
												if( OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

												Ships[ WhoIAm ].Damage = Damage;
									
												Ships[WhoIAm].ShipThatLastHitMe = WhoIAm;
												if( DoDamage( DONT_OVERRIDE_INVUL ) == 1 )		// Did I Die?
												{
													GetDeathString( WEPTYPE_Secondary, Weapon, &methodstr[0] );
				
													Ships[ WhoIAm ].ShipThatLastKilledMe = WhoIAm;
													Ships[ WhoIAm ].Object.Mode = DEATH_MODE;
													Ships[ WhoIAm ].Timer = 0.0F;
													Stats[ WhoIAm ][ WhoIAm ]++;
													if( OwnerType != OWNER_SHIP ) sprintf( &tempstr[0], AN_ENEMY_KILLED_YOU, &methodstr[0] );
													else{
														PlaySfx( SFX_BIKECOMP_DY, 1.0F );
														sprintf( &tempstr[0], YOU_KILLED_YOURSELF_HOW, &methodstr[0] );
													}
													AddMessageToQue( &tempstr[0] );
													ShipDiedSend( WEPTYPE_Secondary, Weapon );
												}
											}
											ForceExternal( WhoIAm, &Recoil );
											if ( !( Models[ model ].Flags & MODFLAG_ShockwaveHitMe ) )
											{
												Models[ model ].Flags |= MODFLAG_ShockwaveHitMe;
												Damage = ( Center_Damage - ( DistFromCenter / ( ( ( Models[ model ].MaxScale * BALL_RADIUS ) + SHIP_RADIUS ) / Center_Damage ) ) );
												Damage = ( Damage * framelag );
												Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 4.0F ) );
												Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 4.0F ) );
												Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 4.0F ) );
												FB_JoltForce( &Recoil );
											}
										}
									}
								}
							}
						}
					}
				}
			}
			break;

		case COLPERS_Forsaken:
			if( ( ( OwnerType == OWNER_SHIP ) && ( Owner == WhoIAm ) ) ||
				( ( OwnerType != OWNER_SHIP ) && IsHost ) )
			{
				for( Count = 0; Count < MAX_PLAYERS; Count++ )
				{
					if( ( Ships[ Count ].enable ) && ( Ships[ Count ].Object.Mode != LIMBO_MODE ) )
					{
						if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
						{
							DistVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
							DistVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
							DistVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
							DistFromCenter = VectorLength( &DistVector );
					
							if( DistFromCenter < ( Radius + SHIP_RADIUS ) )
							{
								if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
											  Group, &DistVector, (VECTOR *) &Int_Point,
											  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
								{
									Damage = ( Center_Damage - ( DistFromCenter / ( ( Radius + SHIP_RADIUS ) / Center_Damage ) ) );
				
									Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 4.0F ) );
									Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 4.0F ) );
									Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 4.0F ) );
			
									Damage = ( Damage * framelag );
			
									if( Count != WhoIAm )
									{
										if( Weapon != (BYTE) -1 )
										{
											IHitYou( (BYTE) Count, Damage, &Recoil, &TempVector, &TempVector, 0.0F, WEPTYPE_Secondary, Weapon, FRAMELAGED_RECOIL );
										}
										else
										{
											IHitYou( (BYTE) Count, Damage, &Recoil, &TempVector, &TempVector, 0.0F, WEPTYPE_Various, Weapon, FRAMELAGED_RECOIL );
										}
									}
									else
									{
										if( !Ships[ WhoIAm ].Invul )
										{
											if( OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

											Ships[ WhoIAm ].Damage = Damage;
								
											Ships[WhoIAm].ShipThatLastHitMe = WhoIAm;
											if( DoDamage( DONT_OVERRIDE_INVUL ) == 1 )		// Did I Die?
											{
												GetDeathString( WEPTYPE_Secondary, Weapon, &methodstr[0] );
			
												Ships[ WhoIAm ].ShipThatLastKilledMe = WhoIAm;
												Ships[ WhoIAm ].Object.Mode = DEATH_MODE;
												Ships[ WhoIAm ].Timer = 0.0F;
												Stats[ WhoIAm ][ WhoIAm ]++;
												if( OwnerType != OWNER_SHIP ) sprintf( &tempstr[0], AN_ENEMY_KILLED_YOU, &methodstr[0] );
												else{
													PlaySfx( SFX_BIKECOMP_DY, 1.0F );
													sprintf( &tempstr[0], YOU_KILLED_YOURSELF_HOW, &methodstr[0] );
												}
												AddMessageToQue( &tempstr[0] );
												ShipDiedSend( WEPTYPE_Secondary, Weapon );
											}
										}
										ForceExternal( WhoIAm, &Recoil );
										if ( !( Models[ model ].Flags & MODFLAG_ShockwaveHitMe ) )
										{
											Models[ model ].Flags |= MODFLAG_ShockwaveHitMe;
											Damage = ( Center_Damage - ( DistFromCenter / ( ( ( Models[ model ].MaxScale * BALL_RADIUS ) + SHIP_RADIUS ) / Center_Damage ) ) );
											Damage = ( Damage * framelag );
											Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 4.0F ) );
											Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 4.0F ) );
											Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 4.0F ) );
											FB_JoltForce( &Recoil );
										}
									}
								}
							}
						}
					}
				}
			}
			break;

#endif
		case COLPERS_Forsaken:
		case COLPERS_Server:
		case COLPERS_Descent:
			if( ( Ships[ WhoIAm ].enable ) && ( Ships[ WhoIAm ].Object.Mode != LIMBO_MODE ) )
			{
				if( !SoundInfo[ Ships[ WhoIAm ].Object.Group ][ Group ] )
				{
					DistVector.x = ( Ships[ WhoIAm ].Object.Pos.x - Pos->x );
					DistVector.y = ( Ships[ WhoIAm ].Object.Pos.y - Pos->y );
					DistVector.z = ( Ships[ WhoIAm ].Object.Pos.z - Pos->z );
					DistFromCenter = VectorLength( &DistVector );
			
					if( DistFromCenter < ( Radius + SHIP_RADIUS ) )
					{
						if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
									  Group, &DistVector, (VECTOR *) &Int_Point,
									  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
						{
							Damage = ( Center_Damage - ( DistFromCenter / ( ( Radius + SHIP_RADIUS ) / Center_Damage ) ) );
			
							Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 4.0F ) );
							Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 4.0F ) );
							Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 4.0F ) );
			
							Damage = ( Damage * framelag );
							if( OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;
			
#if 0
							if( !Ships[ WhoIAm ].Invul )
							{
								Ships[ WhoIAm ].Damage = Damage;
						
								Ships[WhoIAm].ShipThatLastHitMe = WhoIAm;
								if( DoDamage( DONT_OVERRIDE_INVUL ) == 1 )		// Did I Die?
								{
									GetDeathString( WEPTYPE_Secondary, Weapon, &methodstr[0] );
			
									Ships[ WhoIAm ].ShipThatLastKilledMe = WhoIAm;
									Ships[ WhoIAm ].Object.Mode = DEATH_MODE;
									Ships[ WhoIAm ].Timer = 0.0F;
									Stats[ WhoIAm ][ WhoIAm ]++;
									if( OwnerType != OWNER_SHIP ) sprintf( &tempstr[0], AN_ENEMY_KILLED_YOU, &methodstr[0] );
									else{
										PlaySfx( SFX_BIKECOMP_DY, 1.0F );
										sprintf( &tempstr[0], YOU_KILLED_YOURSELF_HOW, &methodstr[0] );
									}
									AddMessageToQue( &tempstr[0] );
									ShipDiedSend( WEPTYPE_Secondary, Weapon );
								}
							}
#endif

							HitMe( OwnerType, Owner, Damage, WEPTYPE_Secondary, Weapon );
							ForceExternal( WhoIAm, &Recoil );
							if ( !( Models[ model ].Flags & MODFLAG_ShockwaveHitMe ) )
							{
								Models[ model ].Flags |= MODFLAG_ShockwaveHitMe;
								Damage = ( Center_Damage - ( DistFromCenter / ( ( ( Models[ model ].MaxScale * BALL_RADIUS ) + SHIP_RADIUS ) / Center_Damage ) ) );
								Damage = ( Damage * framelag );
								Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 4.0F ) );
								Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 4.0F ) );
								Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 4.0F ) );
								FB_JoltForce( &Recoil );
							}
						}
					}
				}
			}
			break;
	}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Shockwave hitting any mines?
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( SecBulls[ i ].Owner == WhoIAm )
		{
	 		if( SecBulls[ i ].SecType == SEC_MINE )
			{
				if( !SoundInfo[ SecBulls[i].GroupImIn ][ Group ] )
				{
		   			DistVector.x = ( SecBulls[ i ].Pos.x - Pos->x );
		   			DistVector.y = ( SecBulls[ i ].Pos.y - Pos->y );
		   			DistVector.z = ( SecBulls[ i ].Pos.z - Pos->z );
		   			DistFromCenter = VectorLength( &DistVector );
		   
		   			if( DistFromCenter < ( Radius + MINE_RADIUS ) )
		   			{
		   				if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
		   							  Group, &DistVector, (VECTOR *) &Int_Point,
		   							  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
		   				{
							Damage = ( ( Center_Damage - ( DistFromCenter / ( ( Radius + MINE_RADIUS ) / Center_Damage ) ) ) * framelag );

							if( OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

							SecBulls[ i ].Shield -= Damage;
							if( SecBulls[ i ].Shield < 0.0F ) SecBulls[ i ].Shield = 0.0F;
		
							if( SecBulls[ i ].Shield == 0.0F )
							{
								DestroySecondary( i, &SecBulls[i].Pos );
							}
						}
					}
				}
			}
		}

		i = Next;
	}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Shockwave hitting any Enemies?
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( ( Enemy->Status & ENEMY_STATUS_Enable ) && !( Enemy->Object.Flags & SHIP_Invul ) )
		{
//			if( ( OwnerType == OWNER_ENEMY ) && ( Enemy->Index != Owner ) || ( OwnerType != OWNER_ENEMY ) )
			if( OwnerType != OWNER_ENEMY )
			{
				if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
				{
					DistVector.x = ( Enemy->Object.Pos.x - Pos->x );
					DistVector.y = ( Enemy->Object.Pos.y - Pos->y );
					DistVector.z = ( Enemy->Object.Pos.z - Pos->z );
					DistFromCenter = VectorLength( &DistVector );
				   
					if( DistFromCenter < ( Radius + SHIP_RADIUS ) )
					{
						if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
									  Group, &DistVector, (VECTOR *) &Int_Point,
									  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
						{
							Damage = ( ( Center_Damage - ( DistFromCenter / ( ( Radius + SHIP_RADIUS ) / Center_Damage ) ) ) * framelag );
				
							Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 4.0F ) );
							Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 4.0F ) );
							Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 4.0F ) );
				
							Enemy->Object.ExternalForce.x = ( Recoil.x * framelag );
							Enemy->Object.ExternalForce.y = ( Recoil.y * framelag );
							Enemy->Object.ExternalForce.z = ( Recoil.z * framelag );
				
							if( OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

							Enemy->Object.Shield -= Damage;
							if( Enemy->Object.Shield < 0 ) Enemy->Object.Shield = 0;
							if( Enemy->Object.Shield == 0 )
							{
								CreateNewExplosion( &Enemy->Object.Pos, &Enemy->Object.DirVector, Enemy->Object.Group );
								KillUsedEnemy( Enemy );
							}
						}
					}
				}
			}
		}

		Enemy = NextEnemy;
	}

}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Gravity Wave Damage to me
	Input		:	VECTOR	*	Position
				:	float		Radius
				:	uint16		Owner
				:	float		Damage at center
				:	uint16		Group
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void GravityWave( VECTOR * Pos, float Radius, uint16 Owner, float Center_Grav, uint16 Group )
{
	int16	Gravity;
	uint16	EndGroup;
	VECTOR	DistVector;
	float	DistFromCenter;
	VERT	Int_Point;
	NORMAL	Int_Normal;
	VECTOR	TempVector;
	VECTOR	force;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;

  	if( ( Ships[ WhoIAm ].enable ) && ( Ships[ WhoIAm ].Object.Mode != LIMBO_MODE ) && ( WhoIAm < MAX_PLAYERS ) )
   	{
		if( !SoundInfo[ Ships[ WhoIAm ].Object.Group ][ Group ] )
		{
	   		DistVector.x = ( Ships[ WhoIAm ].Object.Pos.x - Pos->x );
	   		DistVector.y = ( Ships[ WhoIAm ].Object.Pos.y - Pos->y );
	   		DistVector.z = ( Ships[ WhoIAm ].Object.Pos.z - Pos->z );
	   		DistFromCenter = VectorLength( &DistVector );
	   
	   		if( ( DistFromCenter < ( Radius + SHIP_RADIUS ) ) && ( DistFromCenter > ( ( Radius / 8.0F ) + SHIP_RADIUS ) ) )
	   		{
	   			if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
	   						  Group, &DistVector, (VECTOR *) &Int_Point,
	   						  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
	   			{
	   				Gravity = (int16) ( Center_Grav - ( DistFromCenter / ( ( Radius + SHIP_RADIUS ) / Center_Grav ) ) );
	   
	   				force.x = -( ( ( DistVector.x / DistFromCenter ) * Gravity ) * GLOBAL_SCALE ) * 0.5F;
	   				force.y = -( ( ( DistVector.y / DistFromCenter ) * Gravity ) * GLOBAL_SCALE ) * 0.5F;
	   				force.z = -( ( ( DistVector.z / DistFromCenter ) * Gravity ) * GLOBAL_SCALE ) * 0.5F;
	   				ForceExternal( WhoIAm, &force );
	   			}
	   		}
		}
   	}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	GravityWave effecting any Enemies?
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
		{
			if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
			{
				DistVector.x = ( Enemy->Object.Pos.x - Pos->x );
				DistVector.y = ( Enemy->Object.Pos.y - Pos->y );
				DistVector.z = ( Enemy->Object.Pos.z - Pos->z );
				DistFromCenter = VectorLength( &DistVector );
			   
		   		if( ( DistFromCenter < ( Radius + SHIP_RADIUS ) ) && ( DistFromCenter > ( ( Radius / 8.0F ) + SHIP_RADIUS ) ) )
				{
					if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
								  Group, &DistVector, (VECTOR *) &Int_Point,
								  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
					{
	   					Gravity = (int16) ( Center_Grav - ( DistFromCenter / ( ( Radius + SHIP_RADIUS ) / Center_Grav ) ) );
		   
	   					force.x = -( ( ( DistVector.x / DistFromCenter ) * Gravity ) * GLOBAL_SCALE ) * 0.5F;
	   					force.y = -( ( ( DistVector.y / DistFromCenter ) * Gravity ) * GLOBAL_SCALE ) * 0.5F;
	   					force.z = -( ( ( DistVector.z / DistFromCenter ) * Gravity ) * GLOBAL_SCALE ) * 0.5F;

						Enemy->Object.ExternalForce.x += ( force.x * framelag );
						Enemy->Object.ExternalForce.y += ( force.y * framelag );
						Enemy->Object.ExternalForce.z += ( force.z * framelag );
					}
				}
			}
		}

		Enemy = NextEnemy;
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Check if within range of gravgon
	Input		:	VECTOR	*	Pos
	Output		:	BOOL		True/False ( True if collision )
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL CheckForGravgons( VECTOR * Pos )
{
	uint16		i;
	uint16		nextmodel;
	VECTOR		DistVector;
	float		DistToGravgon;

	i =  FirstModelUsed;

	while( i != (uint16) -1 )
	{
		nextmodel = Models[i].Prev;

		switch( Models[i].Func )
		{
			case MODFUNC_Scale:
				DistVector.x = ( Pos->x - Models[ i ].Pos.x );
				DistVector.y = ( Pos->y - Models[ i ].Pos.y );
				DistVector.z = ( Pos->z - Models[ i ].Pos.z );
				DistToGravgon = VectorLength( &DistVector );
	
				if( DistToGravgon <= ( ( BALL_RADIUS * Models[i].MaxScale ) + ( SHIP_RADIUS * 8.0F ) ) )
				{
					return TRUE;
				}
				break;

			default:
				break;
		}

		i = nextmodel;
	}

	return FALSE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Throw Rider toward all players
	Input		:	uint16		Ship
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void ThrowOutRider( uint16 Ship )
{
	VECTOR	Dir;

	if( !SoundInfo[ Ships[ Ship ].Object.Group ][ Ships[ Current_Camera_View ].Object.Group ] )
	{
		Dir = Ships[ Ship ].LastMove;
		NormaliseVector( &Dir );

		ExplodeBody( &Ships[ Ship ].Object.Pos, &Dir, Ships[ Ship ].Object.Group );
//		CreateTomCruise( &Ships[ Ship ].Object.Pos, Ships[ Ship ].Object.Group );
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Explode Body into Bits
	Input		:	uint16		Ship
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void ExplodeBody( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	CreateBodyPart( Pos, Dir, Group, MODEL_Brain );
	CreateBodyPart( Pos, Dir, Group, MODEL_Ear );
	CreateBodyPart( Pos, Dir, Group, MODEL_Eyeball );
	CreateBodyPart( Pos, Dir, Group, MODEL_Heart );
	CreateBodyPart( Pos, Dir, Group, MODEL_Kidney );
	CreateBodyPart( Pos, Dir, Group, MODEL_Leg );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Create Body Part
	Input		:	VECTOR	*	Pos
				:	uint16		Group
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void CreateBodyPart( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 ModelNum )
{
	uint16	Model;

	Model =	FindFreeModel();
	if( Model != (uint16 ) -1 )
	{
		Models[ Model ].OwnerType = OWNER_NOBODY;
		Models[ Model ].Owner = 0;
		Models[ Model ].ModelNum = ModelNum;
		Models[ Model ].Type = MODTYPE_Missile;
		Models[ Model ].Flags = MODFLAG_Clip;
		Models[ Model ].Visible = TRUE;
		Models[ Model ].Pos = *Pos;

		Models[ Model ].Dir = *Dir;
		Models[ Model ].Dir.x += ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
		Models[ Model ].Dir.y += ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
		Models[ Model ].Dir.z += ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
		NormaliseVector( &Models[ Model ].Dir );
		Models[ Model ].Dir.x *= 5.0F;
		Models[ Model ].Dir.y *= 5.0F;
		Models[ Model ].Dir.y += 5.0F;
		Models[ Model ].Dir.z *= 5.0F;

//		Models[ Model ].Dir.x = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
//		Models[ Model ].Dir.y = ( ( (float) Random_Range( 8000 ) ) + 2000.0F ) / 1000.0F;
//		Models[ Model ].Dir.z = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
		Models[ Model ].Rot.x = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
		Models[ Model ].Rot.y = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
		Models[ Model ].Rot.z = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
		Models[ Model ].Quat.w = 1.0F;
		Models[ Model ].Quat.x = 0.0F;
		Models[ Model ].Quat.y = 0.0F;
		Models[ Model ].Quat.z = 0.0F;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_BodyPart;
		Models[ Model ].Scale = 1.0F;
		Models[ Model ].MaxScale = 1.0F;
		Models[ Model ].Group = Group;
		Models[ Model ].LifeCount = (float) ( 10.0F * ANIM_SECOND );
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Create Tom Cruise
	Input		:	VECTOR	*	Pos
				:	uint16		Group
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void CreateTomCruise( VECTOR * Pos, uint16 Group )
{
#if !defined( SELF_PLAY ) && !defined( SHAREWARE ) && !defined ( WIN98SHAREWARE ) && !defined ( GATEWAY )
	uint16	Model;
	VECTOR	TempDir;
	VECTOR	TempPos;
	float	Time;
	float	Distance;
	float	Height;

	Model =	FindFreeModel();
	if( Model != (uint16 ) -1 )
	{
		Models[ Model ].OwnerType = OWNER_NOBODY;
		Models[ Model ].Owner = 0;
		Models[ Model ].ModelNum = MODEL_Tom0;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Clip;
		Models[ Model ].Visible = TRUE;
		Models[ Model ].Pos = *Pos;

		TempDir.x = ( CurrentCamera.Pos.x - Pos->x );
		TempDir.y = ( CurrentCamera.Pos.y - Pos->y );
		TempDir.z = ( CurrentCamera.Pos.z - Pos->z );
		Distance = VectorLength( &TempDir );
		Time = ( Distance / ( 20.0F * GLOBAL_SCALE ) );
		Height = ( Time * ( 5.0F * GLOBAL_SCALE ) );

		TempPos.x = CurrentCamera.Pos.x + ( Mloadheader.Group[ Group ].up.x * Height );
		TempPos.y = CurrentCamera.Pos.y + ( Mloadheader.Group[ Group ].up.y * Height );
		TempPos.z = CurrentCamera.Pos.z + ( Mloadheader.Group[ Group ].up.z * Height );

		TempDir.x = ( TempPos.x - Pos->x );
		TempDir.y = ( TempPos.y - Pos->y );
		TempDir.z = ( TempPos.z - Pos->z );
		NormaliseVector( &TempDir );
		TempDir.x = ( TempDir.x * 20.0F );
		TempDir.y = ( TempDir.y * 20.0F );
		TempDir.z = ( TempDir.z * 20.0F );
		Models[ Model ].Dir = TempDir;

		TempDir.x = ( CurrentCamera.Pos.x - Pos->x );
		TempDir.y = 0.0F;
		TempDir.z = ( CurrentCamera.Pos.z - Pos->z );
		NormaliseVector( &TempDir );
		QuatFrom2Vectors( &Models[ Model ].Quat, &Forward, &TempDir );
		QuatToMatrix( &Models[ Model ].Quat, &Models[ Model ].Mat );
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );

		Models[ Model ].Rot.x = 0.0F;
		Models[ Model ].Rot.y = 0.0F;
		Models[ Model ].Rot.z = 0.0F;
		Models[ Model ].Func = MODFUNC_TomCruise;
		Models[ Model ].Scale = 1.0F;
		Models[ Model ].MaxScale = 1.0F;
		Models[ Model ].Group = Group;
		Models[ Model ].LifeCount = (float) ( 10.0F * ANIM_SECOND );
	}
#endif
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Missile Shock Wave Damage to Ships and Mines.
	Input		:	VECTOR	*	Position
				:	float		Radius
				:	uint16		Owner
				:	float		Damage at center
				:	uint16		Group
				:	BYTE		Weapon
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void MissileShockWave( VECTOR * Pos, float Radius, uint16 Owner, float Center_Damage, uint16 Group, BYTE Weapon )
{
	int16		Count;
	float		Damage;
	uint16		EndGroup;
	VECTOR		DistVector;
	float		DistFromCenter;
	VERT		Int_Point;
	NORMAL		Int_Normal;
	VECTOR		TempVector;
	VECTOR		Recoil;
//	uint16		i, Next;
	char		methodstr[256];
	char		tempstr[256];
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;

	switch( ColPerspective )
	{
		case COLPERS_Server:
			if( IsServer )
			{
				Server_WhoHitYou = (BYTE)Owner;
				{
					for( Count = 0; Count < MAX_PLAYERS; Count++ )
					{
						if( ( Ships[ Count ].enable ) && ( Ships[ Count ].Object.Mode != LIMBO_MODE ) )
						{
							if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
							{
								DistVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
								DistVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
								DistVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
								DistFromCenter = VectorLength( &DistVector );
								
								if( DistFromCenter < ( Radius + SHIP_RADIUS ) )
								{
									if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
												  Group, &DistVector, (VECTOR *) &Int_Point,
												  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
									{
										Damage = ( Center_Damage - ( DistFromCenter / ( ( Radius + SHIP_RADIUS ) / Center_Damage ) ) );
								
										Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 10.0F ) );
										Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 10.0F ) );
										Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 10.0F ) );
								
										Damage = ( Damage * framelag );
								
										if( Count != WhoIAm )
										{
											IHitYou( (BYTE) Count, Damage, &Recoil, &TempVector, &TempVector, 0.0F, WEPTYPE_Secondary, Weapon, FRAMELAGED_RECOIL );
										}
										else
										{
											if( !Ships[ WhoIAm ].Invul )
											{
												Ships[ WhoIAm ].Damage = Damage;
												Ships[WhoIAm].ShipThatLastHitMe = WhoIAm;
									
												if( DoDamage( DONT_OVERRIDE_INVUL ) == 1 )						// Did I Die?
												{
													GetDeathString( WEPTYPE_Secondary, Weapon, &methodstr[0] );
								
													Ships[ WhoIAm ].ShipThatLastKilledMe = WhoIAm;
													Ships[ WhoIAm ].Object.Mode = DEATH_MODE;
													Ships[ WhoIAm ].Timer = 0.0F;
													Stats[ WhoIAm ][ WhoIAm ]++;
													PlaySfx( SFX_BIKECOMP_DY, 1.0F );
													sprintf( &tempstr[0], YOU_KILLED_YOURSELF_HOW, &methodstr[0] );
													AddMessageToQue( &tempstr[0] );
													ShipDiedSend( WEPTYPE_Secondary, Weapon );
												}

												ForceExternalOneOff( WhoIAm, &Recoil );
											}
										}
									}
								}
							}
						}
					}
				}
			}
			break;

		case COLPERS_Forsaken:
			if( Owner == WhoIAm )
			{
				for( Count = 0; Count < MAX_PLAYERS; Count++ )
				{
					if( ( Ships[ Count ].enable ) && ( Ships[ Count ].Object.Mode != LIMBO_MODE ) )
					{
						if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
						{
							DistVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
							DistVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
							DistVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
							DistFromCenter = VectorLength( &DistVector );
							
							if( DistFromCenter < ( Radius + SHIP_RADIUS ) )
							{
								if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
											  Group, &DistVector, (VECTOR *) &Int_Point,
											  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
								{
									Damage = ( Center_Damage - ( DistFromCenter / ( ( Radius + SHIP_RADIUS ) / Center_Damage ) ) );
							
									Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 10.0F ) );
									Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 10.0F ) );
									Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 10.0F ) );
							
									Damage = ( Damage * framelag );
							
									if( Count != WhoIAm )
									{
										IHitYou( (BYTE) Count, Damage, &Recoil, &TempVector, &TempVector, 0.0F, WEPTYPE_Secondary, Weapon, FRAMELAGED_RECOIL );
									}
									else
									{
										if( !Ships[ WhoIAm ].Invul )
										{
											Ships[ WhoIAm ].Damage = Damage;
											Ships[WhoIAm].ShipThatLastHitMe = WhoIAm;
								
											if( DoDamage( DONT_OVERRIDE_INVUL ) == 1 )						// Did I Die?
											{
												GetDeathString( WEPTYPE_Secondary, Weapon, &methodstr[0] );
							
												Ships[ WhoIAm ].ShipThatLastKilledMe = WhoIAm;
												Ships[ WhoIAm ].Object.Mode = DEATH_MODE;
												Ships[ WhoIAm ].Timer = 0.0F;
												Stats[ WhoIAm ][ WhoIAm ]++;
												PlaySfx( SFX_BIKECOMP_DY, 1.0F );
												sprintf( &tempstr[0], YOU_KILLED_YOURSELF_HOW, &methodstr[0] );
												AddMessageToQue( &tempstr[0] );
												ShipDiedSend( WEPTYPE_Secondary, Weapon );
											}

											ForceExternalOneOff( WhoIAm, &Recoil );
										}
									}
								}
							}
						}
					}
				}
			}
			break;

		case COLPERS_Descent:
			if( ( Ships[ WhoIAm ].enable ) && ( Ships[ WhoIAm ].Object.Mode != LIMBO_MODE ) )
			{
				if( !SoundInfo[ Ships[ WhoIAm ].Object.Group ][ Group ] )
				{
					DistVector.x = ( Ships[ WhoIAm ].Object.Pos.x - Pos->x );
					DistVector.y = ( Ships[ WhoIAm ].Object.Pos.y - Pos->y );
					DistVector.z = ( Ships[ WhoIAm ].Object.Pos.z - Pos->z );
					DistFromCenter = VectorLength( &DistVector );
					
					if( DistFromCenter < ( Radius + SHIP_RADIUS ) )
					{
						if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
									  Group, &DistVector, (VECTOR *) &Int_Point,
									  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
						{
							Damage = ( Center_Damage - ( DistFromCenter / ( ( Radius + SHIP_RADIUS ) / Center_Damage ) ) );
					
							Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 10.0F ) );
							Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 10.0F ) );
							Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 10.0F ) );
					
							Damage = ( Damage * framelag );
#if 0					
							if( !Ships[ WhoIAm ].Invul )
							{
								Ships[ WhoIAm ].Damage = Damage;
								Ships[WhoIAm].ShipThatLastHitMe = WhoIAm;
						
								if( DoDamage( DONT_OVERRIDE_INVUL ) == 1 )						// Did I Die?
								{
									GetDeathString( WEPTYPE_Secondary, Weapon, &methodstr[0] );
					
									Ships[ WhoIAm ].ShipThatLastKilledMe = WhoIAm;
									Ships[ WhoIAm ].Object.Mode = DEATH_MODE;
									Ships[ WhoIAm ].Timer = 0.0F;
									Stats[ WhoIAm ][ WhoIAm ]++;
									PlaySfx( SFX_BIKECOMP_DY, 1.0F );
									sprintf( &tempstr[0], YOU_KILLED_YOURSELF_HOW, &methodstr[0] );
									AddMessageToQue( &tempstr[0] );
									ShipDiedSend( WEPTYPE_Secondary, Weapon );
								}
							}
#endif
							HitMe( OWNER_SHIP, Owner, Damage, WEPTYPE_Secondary, Weapon );
							ForceExternalOneOff( WhoIAm, &Recoil );
						}
					}
				}
			}
			break;
	}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Shockwave hitting any mines?
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
#if 0
	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( SecBulls[ i ].Owner == WhoIAm )
		{
	 		if( SecBulls[ i ].SecType == SEC_MINE )
			{
				if( !SoundInfo[ SecBulls[ i ].GroupImIn ][ Group ] )
				{
	   				DistVector.x = ( SecBulls[ i ].Pos.x - Pos->x );
	   				DistVector.y = ( SecBulls[ i ].Pos.y - Pos->y );
	   				DistVector.z = ( SecBulls[ i ].Pos.z - Pos->z );
	   				DistFromCenter = VectorLength( &DistVector );
					
	   				if( DistFromCenter < ( Radius + MINE_RADIUS ) )
	   				{
	   					if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
	   								  Group, &DistVector, (VECTOR *) &Int_Point,
	   								  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
	   					{
							Damage = ( ( Center_Damage - ( DistFromCenter / ( ( Radius + MINE_RADIUS ) / Center_Damage ) ) ) * framelag );
							SecBulls[ i ].Shield -= Damage;
							if( SecBulls[ i ].Shield < 0.0F ) SecBulls[ i ].Shield = 0.0F;
					
							if( SecBulls[ i ].Shield == 0.0F )
							{
								DestroySecondary( i, &SecBulls[i].Pos );
							}
						}
					}
				}
			}
		}

		i = Next;
	}
#endif
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Shockwave hitting any Enemies?
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( ( Enemy->Status & ENEMY_STATUS_Enable ) && !( Enemy->Object.Flags & SHIP_Invul ) )
		{
			if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
			{
				DistVector.x = ( Enemy->Object.Pos.x - Pos->x );
				DistVector.y = ( Enemy->Object.Pos.y - Pos->y );
				DistVector.z = ( Enemy->Object.Pos.z - Pos->z );
				DistFromCenter = VectorLength( &DistVector );
			   
				if( DistFromCenter < ( Radius + SHIP_RADIUS ) )
				{
					if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, Pos,	 	/* Hit Background? */
								  Group, &DistVector, (VECTOR *) &Int_Point,
								  &EndGroup, &Int_Normal, &TempVector, TRUE, NULL ) != TRUE )
					{
						Damage = ( ( Center_Damage - ( DistFromCenter / ( ( Radius + SHIP_RADIUS ) / Center_Damage ) ) ) * framelag );
			
						Recoil.x = ( ( DistVector.x / DistFromCenter ) * ( Damage / 4.0F ) );
						Recoil.y = ( ( DistVector.y / DistFromCenter ) * ( Damage / 4.0F ) );
						Recoil.z = ( ( DistVector.z / DistFromCenter ) * ( Damage / 4.0F ) );
			
						Enemy->Object.ExternalForce.x = ( Recoil.x * framelag );
						Enemy->Object.ExternalForce.y = ( Recoil.y * framelag );
						Enemy->Object.ExternalForce.z = ( Recoil.z * framelag );
			
						Enemy->Object.Shield -= Damage;
						if( Enemy->Object.Shield < 0 ) Enemy->Object.Shield = 0;
						if( Enemy->Object.Shield == 0 )
						{
							CreateNewExplosion( &Enemy->Object.Pos, &Enemy->Object.DirVector, Enemy->Object.Group );
							KillUsedEnemy( Enemy );
						}
					}
				}
			}
		}

		Enemy = NextEnemy;
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Hit me
	Input		:	uint16		OwnerType
				:	uint16		OwnerID
				:	float		Damage
				:	uint16		WeaponType
				:	uint16		Weapon
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void HitMe( uint16 OwnerType, uint16 OwnerID, float Damage, uint8 WeaponType, uint8 Weapon )
{
	char	methodstr[256];
	char	tempstr[256];
	char	teamstr[256];

	if( !Ships[ WhoIAm ].Invul )
	{
		Ships[ WhoIAm ].Damage = Damage;
		if( OwnerType != OWNER_SHIP ) Ships[ WhoIAm ].ShipThatLastHitMe = MAX_PLAYERS;
		else Ships[ WhoIAm ].ShipThatLastHitMe = (uint8) OwnerID;

		if( DoDamage( DONT_OVERRIDE_INVUL ) == 1 )		// Did I Die?
		{
			GetDeathString( WeaponType, Weapon, &methodstr[0] );

			if( OwnerType != OWNER_SHIP ) Ships[ WhoIAm ].ShipThatLastKilledMe = MAX_PLAYERS;
			else Ships[ WhoIAm ].ShipThatLastKilledMe = (uint8) OwnerID;
			Ships[ WhoIAm ].Object.Mode = DEATH_MODE;
			Ships[ WhoIAm ].Timer = 0.0F;

			if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
			{
				PlaySfx( SFX_BIKECOMP_DY, 1.0F );
				sprintf( &tempstr[0], YOU_KILLED_YOURSELF_HOW, &methodstr[0] );
			}
			else
			{
				if( OwnerType == OWNER_ENEMY )
				{
					sprintf( &tempstr[0], ENEMY_KILLED_YOU, &methodstr[0] );
				}
				else
				{
					if( OwnerType == OWNER_SHIP )
					{
						if (TeamGame && ( TeamNumber[Ships[WhoIAm].ShipThatLastKilledMe] == TeamNumber[WhoIAm]))
						{
							strcpy(&teamstr[0], FROM_YOUR_OWN_TEAM );
						}
						else
						{
							strcpy(&teamstr[0], "");
						}

   						sprintf( (char*)&tempstr[0] , SOMEONE_KILLED_YOU, &Names[Ships[WhoIAm].ShipThatLastKilledMe][0], &methodstr[0], &teamstr );
					}
					else
					{
						sprintf( &tempstr[0], YOU_DIED );
					}
				}
			}
			AddMessageToQue( &tempstr[0] );
			ShipDiedSend( WeaponType, Weapon );
		}
	}
}
	
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Create Orbit Pulsar
	Input		:	uint16		Ship
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void CreateOrbitPulsar( uint16 Ship )
{
	float	Rot;
	float	Inc;
	int16	Count;
	uint16	Model;

	Rot = 0.0F;
	Inc = ( 360.0F / Ships[ Ship ].NumMultiples );

	for( Count = 0; Count < Ships[ Ship ].NumMultiples; Count++ )
	{
		if( Ships[ Ship ].OrbModels[ Count ] == (uint16) -1 )
		{
			Model =	FindFreeModel();

			if( Model != (uint16 ) -1 )
			{
				Ships[ Ship ].OrbModels[ Count ] = Model;
				Models[ Model ].OwnerType = OWNER_SHIP;
				Models[ Model ].Owner = Ship;
				Models[ Model ].Ship = Ship;
				Models[ Model ].ModelNum = MODEL_OrbitPulsar;
				Models[ Model ].Type = MODTYPE_Field; //Missile;
				Models[ Model ].Flags = MODFLAG_Nothing;
				Models[ Model ].Visible = TRUE;
				Models[ Model ].Pos = Ships[ Ship ].Object.Pos;
				Models[ Model ].Dir.x = 0.0F;
				Models[ Model ].Dir.y = 0.0F;
				Models[ Model ].Dir.z = 0.0F;
				Models[ Model ].Rot.x = 0.0F;
				Models[ Model ].Rot.y = 0.0F;
				Models[ Model ].Rot.z = 0.0F;
				Models[ Model ].Quat.w = 1.0F;
				Models[ Model ].Quat.x = 0.0F;
				Models[ Model ].Quat.y = 0.0F;
				Models[ Model ].Quat.z = 0.0F;
				Models[ Model ].Mat = MATRIX_Identity;
				MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
				Models[ Model ].Func = MODFUNC_OrbitPulsar;
				Models[ Model ].AxisRot = Rot;
				Models[ Model ].Scale = 1.0F;
				Models[ Model ].MaxScale = 1.0F;
				Models[ Model ].Group = Ships[ Ship ].Object.Group;
				Models[ Model ].LifeCount = (float) ( 10.0F * ANIM_SECOND );
			}
		}
		else
		{
			Model = Ships[ Ship ].OrbModels[ Count ];
			Models[ Model ].AxisRot = Rot;
		}
		Rot += Inc;
	}

	for( Count = Ships[ Ship ].NumMultiples; Count < MAXMULTIPLES; Count++ )
	{
		if( Ships[ Ship ].OrbModels[ Count ] != (uint16) -1 )
		{
			KillUsedModel( Ships[ Ship ].OrbModels[ Count ] );
			Ships[ Ship ].OrbModels[ Count ] = (uint16) -1;
		}
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Create Body Part
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	UpVector
				:	uint16		Group
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void CreateExplosionDonut( VECTOR * Pos, VECTOR * Up, uint16 Group )
{
	uint16	Model;

	Model =	FindFreeModel();
	if( Model != (uint16 ) -1 )
	{
		Models[ Model ].OwnerType = OWNER_NOBODY;
		Models[ Model ].Owner = 0;
		Models[ Model ].ModelNum = MODEL_ExplosionDonut;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].Visible = TRUE;
		Models[ Model ].Pos.x = ( Pos->x + ( Up->x * SHIP_RADIUS ) );
		Models[ Model ].Pos.y = ( Pos->y + ( Up->y * SHIP_RADIUS ) );
		Models[ Model ].Pos.z = ( Pos->z + ( Up->z * SHIP_RADIUS ) );
		Models[ Model ].Dir.x = 0.0F;
		Models[ Model ].Dir.y = 0.0F;
		Models[ Model ].Dir.z = 0.0F;
		Models[ Model ].Rot.x = 0.0F;
		Models[ Model ].Rot.y = 0.0F;
		Models[ Model ].Rot.z = 0.0F;
		QuatFromVector( Up, &Models[ Model ].Quat );
		QuatToMatrix( &Models[ Model ].Quat, &Models[ Model ].Mat );
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_ScaleDonut;
		Models[ Model ].Scale = 0.1F;
		Models[ Model ].MaxScale = 10.0F;
		Models[ Model ].Group = Group;
		Models[ Model ].LifeCount = (float) ( 10.0F * ANIM_SECOND );
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Create Line
	Input		:	float		x1
				:	float		y1
				:	float		z1
				:	float		x2
				:	float		y2
				:	float		z2
				:	uint16		Group
	Output		:	uint16		Line Index
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
uint16 CreateLine( float x1, float y1, float z1, float x2, float y2, float z2, uint16 Group )
{
	uint16	line;

	line = FindFreeLine();						// Line attached

	if( line != (uint16 ) -1 )
	{
		Lines[ line ].StartPos.x = x1;
		Lines[ line ].StartPos.y = y1;
		Lines[ line ].StartPos.z = z1;
		Lines[ line ].EndPos.x = x2;
		Lines[ line ].EndPos.y = y2;
		Lines[ line ].EndPos.z = z2;
		Lines[ line ].StartCol.R = 255;
		Lines[ line ].StartCol.G = 255;
		Lines[ line ].StartCol.B = 255;
		Lines[ line ].EndCol.R = 255;
		Lines[ line ].EndCol.G = 255;
		Lines[ line ].EndCol.B = 255;
		Lines[ line ].StartTrans = 255;
		Lines[ line ].EndTrans = 255;
		Lines[ line ].Group = Group;
	}
	return( line );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Get .MX File Bouning Box
	Input		:	MXLOADHEADER * DstMloadheader
				:	MATRIX	*	Matrix
				:	VECTOR	*	Pos
				:	VECTOR	*	TopLeft
				:	VECTOR	*	BottomRight
	Output		:	BOOL		True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL GetMXBoundingBox( MXLOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft,
					   VECTOR * BottomRight )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	uint16					Group;
	uint16					Count;
	uint16					ExecBuf;
	VECTOR					TempVert;

	Group = 0;
	ExecBuf = 0;

	memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
					DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

	DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

	for( Count = 0; Count < 8; Count++ )
	{
		TempVert.x = DstlpD3DLVERTEX->x;
		TempVert.y = DstlpD3DLVERTEX->y;
		TempVert.z = DstlpD3DLVERTEX->z;
		ApplyMatrix( Matrix, &TempVert, &TempVert );
		TempVert.x += Pos->x;
		TempVert.y += Pos->y;
		TempVert.z += Pos->z;

		if( ( TopLeft->x == BottomRight->x ) && ( TopLeft->y == BottomRight->y ) && ( TopLeft->z == BottomRight->z ) &&
			( TopLeft->x == 0.0F ) && ( TopLeft->y == 0.0F ) && ( TopLeft->z == 0.0F ) )
		{
			TopLeft->x = TempVert.x;
			TopLeft->y = TempVert.y;
			TopLeft->z = TempVert.z;
			BottomRight->x = TempVert.x;
			BottomRight->y = TempVert.y;
			BottomRight->z = TempVert.z;
		}
		else
		{
			if( TempVert.x < TopLeft->x ) TopLeft->x = TempVert.x;
			if( TempVert.x > BottomRight->x ) BottomRight->x = TempVert.x;
			if( TempVert.y < TopLeft->y ) TopLeft->y = TempVert.y;
			if( TempVert.y > BottomRight->y ) BottomRight->y = TempVert.y;
			if( TempVert.z < TopLeft->z ) TopLeft->z = TempVert.z;
			if( TempVert.z > BottomRight->z ) BottomRight->z = TempVert.z;
		}

		DstlpD3DLVERTEX++;
	}

	if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
					DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Get .MXA File Bouning Box
	Input		:	MXALOADHEADER * DstMloadheader
				:	MATRIX	*	Matrix
				:	VECTOR	*	Pos
				:	VECTOR	*	TopLeft
				:	VECTOR	*	BottomRight
	Output		:	BOOL	True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL GetMXABoundingBox( MXALOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft,
					    VECTOR * BottomRight )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	uint16					Group;
	uint16					Count;
	uint16					ExecBuf;
	VECTOR					TempVert;

	Group = 0;
	ExecBuf = 0;
	
	memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
					DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

	DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

	for( Count = 0; Count < 8; Count++ )
	{
		TempVert.x = DstlpD3DLVERTEX->x;
		TempVert.y = DstlpD3DLVERTEX->y;
		TempVert.z = DstlpD3DLVERTEX->z;
		ApplyMatrix( Matrix, &TempVert, &TempVert );
		TempVert.x += Pos->x;
		TempVert.y += Pos->y;
		TempVert.z += Pos->z;

		if( ( TopLeft->x == BottomRight->x ) && ( TopLeft->y == BottomRight->y ) && ( TopLeft->z == BottomRight->z ) &&
			( TopLeft->x == 0.0F ) && ( TopLeft->y == 0.0F ) && ( TopLeft->z == 0.0F ) )
		{
			TopLeft->x = TempVert.x;
			TopLeft->y = TempVert.y;
			TopLeft->z = TempVert.z;
			BottomRight->x = TempVert.x;
			BottomRight->y = TempVert.y;
			BottomRight->z = TempVert.z;
		}
		else
		{
			if( TempVert.x < TopLeft->x ) TopLeft->x = TempVert.x;
			if( TempVert.x > BottomRight->x ) BottomRight->x = TempVert.x;
			if( TempVert.y < TopLeft->y ) TopLeft->y = TempVert.y;
			if( TempVert.y > BottomRight->y ) BottomRight->y = TempVert.y;
			if( TempVert.z < TopLeft->z ) TopLeft->z = TempVert.z;
			if( TempVert.z > BottomRight->z ) BottomRight->z = TempVert.z;
		}

		DstlpD3DLVERTEX++;
	}

	if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
					DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
	
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Create .MX File Bouning Box ( Lines )
	Input		:	MXLOADHEADER * DstMloadheader
				:	MATRIX	*	Matrix
				:	VECTOR	*	Pos
				:	uint16	*	Line Array
				:	uint16		Group
	Output		:	BOOL		True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL CreateMXBoundingBox( MXLOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, uint16 * LineArray, uint16 Group2 )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	uint16					Group;
	uint16					Count;
	uint16					ExecBuf;
	VECTOR					Verts[ 8 ];

	for( Count = 0; Count < 12; Count++ )
	{
		if( LineArray[ Count ] != (uint16) -1 )
		{
			KillUsedLine( LineArray[ Count ] );
			LineArray[ Count ] = (uint16) -1;
		}

	}
	if( !ShowBoundingBoxes ) return TRUE;

	Group = 0;
	ExecBuf = 0;

	memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
					DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

	DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

	for( Count = 0; Count < 8; Count++ )
	{
		Verts[ Count ].x = DstlpD3DLVERTEX->x;
		Verts[ Count ].y = DstlpD3DLVERTEX->y;
		Verts[ Count ].z = DstlpD3DLVERTEX->z;
		ApplyMatrix( Matrix, &Verts[ Count ], &Verts[ Count ] );
		Verts[ Count ].x += Pos->x;
		Verts[ Count ].y += Pos->y;
		Verts[ Count ].z += Pos->z;

		DstlpD3DLVERTEX++;
	}

	LineArray[ 0 ] = CreateLine( Verts[ 0 ].x, Verts[ 0 ].y, Verts[ 0 ].z, Verts[ 1 ].x, Verts[ 1 ].y, Verts[ 1 ].z, Group2 );
	LineArray[ 1 ] = CreateLine( Verts[ 1 ].x, Verts[ 1 ].y, Verts[ 1 ].z, Verts[ 3 ].x, Verts[ 3 ].y, Verts[ 3 ].z, Group2 );
	LineArray[ 2 ] = CreateLine( Verts[ 3 ].x, Verts[ 3 ].y, Verts[ 3 ].z, Verts[ 2 ].x, Verts[ 2 ].y, Verts[ 2 ].z, Group2 );
	LineArray[ 3 ] = CreateLine( Verts[ 2 ].x, Verts[ 2 ].y, Verts[ 2 ].z, Verts[ 0 ].x, Verts[ 0 ].y, Verts[ 0 ].z, Group2 );

	LineArray[ 4 ] = CreateLine( Verts[ 4 ].x, Verts[ 4 ].y, Verts[ 4 ].z, Verts[ 5 ].x, Verts[ 5 ].y, Verts[ 5 ].z, Group2 );
	LineArray[ 5 ] = CreateLine( Verts[ 5 ].x, Verts[ 5 ].y, Verts[ 5 ].z, Verts[ 7 ].x, Verts[ 7 ].y, Verts[ 7 ].z, Group2 );
	LineArray[ 6 ] = CreateLine( Verts[ 7 ].x, Verts[ 7 ].y, Verts[ 7 ].z, Verts[ 6 ].x, Verts[ 6 ].y, Verts[ 6 ].z, Group2 );
	LineArray[ 7 ] = CreateLine( Verts[ 6 ].x, Verts[ 6 ].y, Verts[ 6 ].z, Verts[ 4 ].x, Verts[ 4 ].y, Verts[ 4 ].z, Group2 );
			   
	LineArray[ 8 ] = CreateLine( Verts[ 0 ].x, Verts[ 0 ].y, Verts[ 0 ].z, Verts[ 4 ].x, Verts[ 4 ].y, Verts[ 4 ].z, Group2 );
	LineArray[ 9 ] = CreateLine( Verts[ 1 ].x, Verts[ 1 ].y, Verts[ 1 ].z, Verts[ 5 ].x, Verts[ 5 ].y, Verts[ 5 ].z, Group2 );
	LineArray[ 10 ] = CreateLine( Verts[ 2 ].x, Verts[ 2 ].y, Verts[ 2 ].z, Verts[ 6 ].x, Verts[ 6 ].y, Verts[ 6 ].z, Group2 );
	LineArray[ 11 ] = CreateLine( Verts[ 3 ].x, Verts[ 3 ].y, Verts[ 3 ].z, Verts[ 7 ].x, Verts[ 7 ].y, Verts[ 7 ].z, Group2 );

	if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
					DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Create .MXA File Bouning Box ( Lines )
	Input		:	MXALOADHEADER * DstMloadheader
				:	MATRIX	*	Matrix
				:	VECTOR	*	Pos
				:	uint16	*	Line Array
				:	uint16		Group
	Output		:	BOOL	True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL CreateMXABoundingBox( MXALOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, uint16 * LineArray, uint16 Group2 )
{
	D3DEXECUTEBUFFERDESC	DstDebDesc;
	LPD3DLVERTEX			DstlpD3DLVERTEX;
	uint16					Group;
	uint16					Count;
	uint16					ExecBuf;
	VECTOR					Verts[ 8 ];

	for( Count = 0; Count < 12; Count++ )
	{
		if( LineArray[ Count ] != (uint16) -1 )
		{
			KillUsedLine( LineArray[ Count ] );
			LineArray[ Count ] = (uint16) -1;
		}

	}
	if( !ShowBoundingBoxes ) return TRUE;

	Group = 0;
	ExecBuf = 0;
	
	memset( &DstDebDesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	DstDebDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Lock(
					DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ], &DstDebDesc ) != D3D_OK ) return FALSE;

	DstlpD3DLVERTEX = (LPD3DLVERTEX) DstDebDesc.lpData;

	for( Count = 0; Count < 8; Count++ )
	{
		Verts[ Count ].x = DstlpD3DLVERTEX->x;
		Verts[ Count ].y = DstlpD3DLVERTEX->y;
		Verts[ Count ].z = DstlpD3DLVERTEX->z;
		ApplyMatrix( Matrix, &Verts[ Count ], &Verts[ Count ] );
		Verts[ Count ].x += Pos->x;
		Verts[ Count ].y += Pos->y;
		Verts[ Count ].z += Pos->z;

		DstlpD3DLVERTEX++;
	}

	LineArray[ 0 ] = CreateLine( Verts[ 0 ].x, Verts[ 0 ].y, Verts[ 0 ].z, Verts[ 1 ].x, Verts[ 1 ].y, Verts[ 1 ].z, Group2 );
	LineArray[ 1 ] = CreateLine( Verts[ 1 ].x, Verts[ 1 ].y, Verts[ 1 ].z, Verts[ 3 ].x, Verts[ 3 ].y, Verts[ 3 ].z, Group2 );
	LineArray[ 2 ] = CreateLine( Verts[ 3 ].x, Verts[ 3 ].y, Verts[ 3 ].z, Verts[ 2 ].x, Verts[ 2 ].y, Verts[ 2 ].z, Group2 );
	LineArray[ 3 ] = CreateLine( Verts[ 2 ].x, Verts[ 2 ].y, Verts[ 2 ].z, Verts[ 0 ].x, Verts[ 0 ].y, Verts[ 0 ].z, Group2 );

	LineArray[ 4 ] = CreateLine( Verts[ 4 ].x, Verts[ 4 ].y, Verts[ 4 ].z, Verts[ 5 ].x, Verts[ 5 ].y, Verts[ 5 ].z, Group2 );
	LineArray[ 5 ] = CreateLine( Verts[ 5 ].x, Verts[ 5 ].y, Verts[ 5 ].z, Verts[ 7 ].x, Verts[ 7 ].y, Verts[ 7 ].z, Group2 );
	LineArray[ 6 ] = CreateLine( Verts[ 7 ].x, Verts[ 7 ].y, Verts[ 7 ].z, Verts[ 6 ].x, Verts[ 6 ].y, Verts[ 6 ].z, Group2 );
	LineArray[ 7 ] = CreateLine( Verts[ 6 ].x, Verts[ 6 ].y, Verts[ 6 ].z, Verts[ 4 ].x, Verts[ 4 ].y, Verts[ 4 ].z, Group2 );
			   
	LineArray[ 8 ] = CreateLine( Verts[ 0 ].x, Verts[ 0 ].y, Verts[ 0 ].z, Verts[ 4 ].x, Verts[ 4 ].y, Verts[ 4 ].z, Group2 );
	LineArray[ 9 ] = CreateLine( Verts[ 1 ].x, Verts[ 1 ].y, Verts[ 1 ].z, Verts[ 5 ].x, Verts[ 5 ].y, Verts[ 5 ].z, Group2 );
	LineArray[ 10 ] = CreateLine( Verts[ 2 ].x, Verts[ 2 ].y, Verts[ 2 ].z, Verts[ 6 ].x, Verts[ 6 ].y, Verts[ 6 ].z, Group2 );
	LineArray[ 11 ] = CreateLine( Verts[ 3 ].x, Verts[ 3 ].y, Verts[ 3 ].z, Verts[ 7 ].x, Verts[ 7 ].y, Verts[ 7 ].z, Group2 );

	if( DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf ]->lpVtbl->Unlock(
					DstMloadheader->Group[ Group ].lpExBuf[ ExecBuf] ) != D3D_OK )	return FALSE;
	
	return TRUE;
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Setup Model SpotFX
	Input		:	uint16		ModelIndex
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void SetupModelSpotFX( uint16 i )
{
	int16		Count;
	int16		NumSpotFX;
	PVSPOTFX *	SpotFXPtr;

	if( ModelNames[ Models[i].ModelNum ].DoIMorph )
	{
		if( MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
			MxaModelHeaders[ Models[i].ModelNum ].SpotFX )
		{
			SpotFXPtr = MxaModelHeaders[ Models[i].ModelNum ].SpotFX;
			NumSpotFX = MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX;
		}
	}
	else
	{
		if( ModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
			ModelHeaders[ Models[i].ModelNum ].SpotFX )
		{
			SpotFXPtr = ModelHeaders[ Models[i].ModelNum ].SpotFX;
			NumSpotFX = ModelHeaders[ Models[i].ModelNum ].NumSpotFX;
		}
	}

	for( Count = 0; Count < NumSpotFX; Count++ )
	{
		Models[i].SpotFXSFX_ID[ Count ] = 0;

		if( SpotFXPtr->StartDelay )
		{
			Models[i].SpotFXFlags[ Count ] = SPOTFX_FLAGS_Nothing;
			Models[i].SpotFXState[ Count ] = SPOTFX_STATE_WaitingForDelay;
			Models[i].SpotFXDelay[ Count ] = SpotFXPtr->StartDelay;

		}
		else
		{
			Models[i].SpotFXFlags[ Count ] = SPOTFX_FLAGS_Nothing;
			Models[i].SpotFXState[ Count ] = SPOTFX_STATE_Active;
			Models[i].SpotFXDelay[ Count ] = SpotFXPtr->ActiveDelay;
		}

		SpotFXPtr++;
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Update Ship Model
	Input		:	uint16		Ship
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
//#define	PHIL_TESTSFX

void UpdateShipModel( uint16 Ship )
{
	uint16	Model;

#ifdef PHIL_TESTSFX
	int16	SoundFX;
#endif

	if( Ship == MAX_PLAYERS ) return;

	if( Ships[ Ship ].enable )
	{
		if( BikeCompFiles[ ( Ships[ Ship ].BikeNum % MAXBIKETYPES ) ].Filename[ 0 ] )
		{
			if( Ships[ Ship ].ModelNum != (uint16) -1 )
			{
//				DebugPrintf( "Deallocated Model %d for ship %d\n", Ships[ Ship ].ModelNum, Ship );

				KillUsedModel( Ships[ Ship ].ModelNum );
				Ships[ Ship ].ModelNum = (uint16) -1;
			}

			if( !Ships[ Ship ].Object.Components )
			{
//				DebugPrintf( "Allocated Componented Model for ship %d\n", Ship );
				AllocateCompShip( Ship );
			}

			if( Ships[ Ship ].Object.Components )
			{
				Ships[ Ship ].Object.Time += ( framelag * Ships[ Ship ].Object.AnimSpeed );

				if( Ships[ Ship ].Object.Time > Ships[ Ship ].Object.OverallTime )
				{
					Ships[ Ship ].Object.Time = FMOD( Ships[ Ship ].Object.Time, Ships[ Ship ].Object.OverallTime );
				}

				if( ( Ships[ Ship ].Object.Flags & SHIP_Stealth ) )
				{
					SetCompObjStealth( Ships[ Ship ].Object.Components, 1 );

					if( Ships[ Ship ].Object.Flags & SHIP_Charging )
					{
						Ships[ Ship ].Object.Red = 0;
						Ships[ Ship ].Object.Green = 0;
						Ships[ Ship ].Object.Blue = 128;
					}
					else
					{
						Ships[ Ship ].Object.Red = 0;
						Ships[ Ship ].Object.Green = 0;
						Ships[ Ship ].Object.Blue = 0;
					}
				}
				else
				{
					SetCompObjNonStealth( Ships[ Ship ].Object.Components, 1 );
					UpdateObjectAmbientColour( &Ships[ Ship ].Object );
				}


				UpdateCompObjColours( Ships[ Ship ].Object.Components, 1, (int) Ships[ Ship ].Object.Red,
									  (int) Ships[ Ship ].Object.Green, (int) Ships[ Ship ].Object.Blue );
				UpdateCompObjChildren( Ships[ Ship ].Object.Components, 1, &Ships[ Ship ].Object.FinalMat,
									   &Ships[ Ship ].Object.Pos, Ships[ Ship ].Object.Time,
									   Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos );

				if( ( Ships[ Ship ].enable != 0 ) &&
					( Ships[ Ship ].Object.Mode != GAMEOVER_MODE ) &&
					( Ships[ Ship ].Object.Mode != LIMBO_MODE ) &&
					( (GameStatus[ Ship ] == STATUS_Normal ) || (GameStatus[ Ship ] == STATUS_SinglePlayer ) ) &&
					( Current_Camera_View != Ship ) )
				{
					SetCompObjModelsState( Ships[ Ship ].Object.Components, 1, TRUE );
				}
				else
				{
					SetCompObjModelsState( Ships[ Ship ].Object.Components, 1, FALSE );
				}
			}
		}
		else
		{
			if( Ships[ Ship ].Object.Components )
			{
//				DebugPrintf( "Deallocated Componented Model for ship %d\n", Ship );

				FreeCompObjChildren( Ships[ Ship ].Object.Components, 1 );
				free( Ships[ Ship ].Object.Components );
				Ships[ Ship ].Object.Components = NULL;
			}

			if( Ships[ Ship ].ModelNum == (uint16) -1 )
			{
#ifdef DEBUG_ON
				PickupModelValid();
#endif
				Model =	FindFreeModel();

				if( Model != (uint16 ) -1 )
				{
					Models[ Model ].OwnerType = OWNER_SHIP;
					Models[ Model ].Owner = WhoIAm;
					Models[ Model ].ModelNum = BikeModels[ ( Ships[ Ship ].BikeNum % MAXBIKETYPES ) ];
					Models[ Model ].Type = MODTYPE_Pickup;
					Models[ Model ].Flags = MODFLAG_AmbientLight;
					Models[ Model ].Visible = TRUE;
					Models[ Model ].Pos = Ships[ Ship ].Object.Pos;
					Models[ Model ].Dir = Ships[ Ship ].Move_Off;
					Models[ Model ].Rot.x = 0.0F;
					Models[ Model ].Rot.y = 0.0F;
					Models[ Model ].Rot.z = 0.0F;
					Models[ Model ].Mat = Ships[ Ship ].Object.FinalMat;
					MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
					Models[ Model ].Func = MODFUNC_ProcessSpotFX; //MODFUNC_Nothing;
					Models[ Model ].Scale = 1.0F;
					Models[ Model ].MaxScale = 1.0F;
					Models[ Model ].Group = Ships[ Ship ].Object.Group;
					Models[ Model ].LifeCount = (float) 10000.0F;
					Ships[ Ship ].ModelNum = Model;

//					DebugPrintf( "Allocated Model %d for ship %d\n", Model, Ship );

#ifdef PHIL_TESTSFX
					if( !Ship )
					{
						SoundFX = ReturnSFXIndex( "tankeng" );

						if( SoundFX != -1 )
						{
							PlayPannedSfxVariable( SoundFX, &Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, 0.0F );
						}
					}
#endif

#ifdef DEBUG_ON
					PickupModelValid();
#endif
				}
			}

			if( Ships[ Ship ].ModelNum != (uint16) -1 )
			{
				Model = Ships[ Ship ].ModelNum;

				if( ( Ships[ Ship ].Object.Flags & SHIP_Stealth ) )
				{
					Models[ Model ].Flags &= ~MODFLAG_AmbientLight;
					Models[ Model ].Flags |= MODFLAG_Stealth;

					if( Ships[ Ship ].Object.Flags & SHIP_Charging )
					{
						Models[ Model ].Red = 0;
						Models[ Model ].Green = 0;
						Models[ Model ].Blue = 128;
					}
					else
					{
						Models[ Model ].Red = 0;
						Models[ Model ].Green = 0;
						Models[ Model ].Blue = 0;
					}
				}
				else
				{
					Models[ Model ].Flags |= MODFLAG_AmbientLight;
					Models[ Model ].Flags &= ~MODFLAG_Stealth;
					UpdateObjectAmbientColour( &Ships[Ship].Object );
					Models[ Model ].Red = (int) Ships[ Ship ].Object.Red;
					Models[ Model ].Green = (int) Ships[ Ship ].Object.Green;
					Models[ Model ].Blue = (int) Ships[ Ship ].Object.Blue;
				}

				Models[ Model ].ModelNum = 	BikeModels[ ( Ships[ Ship ].BikeNum % MAXBIKETYPES ) ];
				Models[ Model ].Pos = Ships[ Ship ].Object.Pos;
				Models[ Model ].Dir = Ships[ Ship ].Move_Off;
				Models[ Model ].Mat = Ships[ Ship ].Object.FinalMat;
				MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
				Models[ Model ].Group = Ships[ Ship ].Object.Group;

				if( ( Ships[ Ship ].enable != 0 ) &&
					( Ships[ Ship ].Object.Mode != GAMEOVER_MODE ) &&
					( Ships[ Ship ].Object.Mode != LIMBO_MODE ) &&
					( (GameStatus[ Ship ] == STATUS_Normal ) || (GameStatus[ Ship ] == STATUS_SinglePlayer ) ) &&
					( Current_Camera_View != Ship ) )
				{
					Models[ Model ].Visible = TRUE;
				}
				else
				{
					Models[ Model ].Visible = FALSE;
				}
			}
		}
	}
	else
	{
		Ships[ Ship ].NumMultiples = 0;
		CreateOrbitPulsar( Ship );

		if( Ships[ Ship ].ModelNum != (uint16) -1 )
		{
//			DebugPrintf( "Deallocated Model %d for ship %d\n", Ships[ Ship ].ModelNum, Ship );

			KillUsedModel( Ships[ Ship ].ModelNum );
			Ships[ Ship ].ModelNum = (uint16) -1;
		}

		if( Ships[ Ship ].Object.Components )
		{
//			DebugPrintf( "Deallocated Componented Model for ship %d\n", Ship );

			FreeCompObjChildren( Ships[ Ship ].Object.Components, 1 );
			free( Ships[ Ship ].Object.Components );
			Ships[ Ship ].Object.Components = NULL;
		}
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Get Ambient Colour for Object
	Input		:	OBJECT * Object
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void UpdateObjectAmbientColour( OBJECT * Object )
{
	D3DCOLOR	Col;
	float		Red,Green,Blue,Speed;

	Col = FindNearestCellColour( &Mloadheader, &Object->Pos, Object->Group );
	Red = (float)RGBA_GETRED(Col);
	Green = (float)RGBA_GETGREEN(Col);
	Blue = (float)RGBA_GETBLUE(Col);

	Speed = VectorLength( &Object->Speed );
	Speed += VectorLength( &Object->ExternalForce );
	Speed *= framelag;
	Speed *= 0.1F;

	if( Object->Red < Red )
	{
		Object->Red+= 1.0F + Speed;
		if( Object->Red > Red )
			Object->Red = Red;
	}
	else if( Object->Red > Red )
	{
		Object->Red-= 1.0F + Speed;
		if( Object->Red < Red )
			Object->Red = Red;
	}
	if( Object->Green < Green )
	{
		Object->Green+= 1.0F + Speed;
		if( Object->Green > Green )
			Object->Green = Green;
	}
	else if( Object->Green > Green )
	{
		Object->Green-= 1.0F + Speed;
		if( Object->Green < Green )
			Object->Green = Green;
	}
	if( Object->Blue < Blue )
	{
		Object->Blue+= 1.0F + Speed;
		if( Object->Blue > Blue )
			Object->Blue = Blue;
	}
	else if( Object->Blue > Blue )
	{
		Object->Blue-= 1.0F + Speed;
		if( Object->Blue < Blue )
			Object->Blue = Blue;
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Get An Overall Light level for a Position...Based on
				:	Real lights...
	Input		:	VECTOR * Pos , float * Red , float * Green , float * Blue
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void GetRealLightAmbient( VECTOR * Pos , float * Red , float * Green , float * Blue )
{
	VECTOR					DistVector;
	float					Dist;
    float					Val;
	XLIGHT * LightPnt;
	float	RF,GF,BF;
	float Size2;

	RF = GF = BF = 0.0F;
	
	LightPnt = FirstLightVisible;
	
	while( LightPnt )
	{
		DistVector.x = ( Pos->x - LightPnt->Pos.x );
		DistVector.y = ( Pos->y - LightPnt->Pos.y );
		DistVector.z = ( Pos->z - LightPnt->Pos.z );

		Size2 = LightPnt->Size;
		Size2 *= Size2;
		
		Dist = ( DistVector.x * DistVector.x ) + ( DistVector.y * DistVector.y ) + ( DistVector.z * DistVector.z );
	
		if( Dist < Size2 )
		{
			Val = ( 1.0F - ( Dist / Size2 ) );
	
			RF += ( LightPnt->r * Val );
			GF += ( LightPnt->g * Val );
			BF += ( LightPnt->b * Val );
		}
		LightPnt = LightPnt->NextVisible;
	}
	if ( !d3dapp->CurrDriver || PowerVR )
	{
#ifdef SOFTWARE_ENABLE
		if ( !SoftwareVersion )
#endif
		{
			RF = ( RF+GF+BF ) * 0.33333F;
			GF = RF;
			BF = RF;
		}
	}
	if( RF > 255.0F ) RF = 255.0F;
	if( GF > 255.0F ) GF = 255.0F;
	if( BF > 255.0F ) BF = 255.0F;

	*Red = RF;
	*Green = GF;
	*Blue = BF;

}

#define POINT_TO_PLANE( P, N ) ( (P)->x * (N)->Normal.x + (P)->y * (N)->Normal.y + (P)->z * (N)->Normal.z + ( (N)->Offset) )

void RefreshModel( uint16 model )
{
	uint16					Group;
	uint16					Vert;
	uint16					ExecBuf;
	MXALOADHEADER			*DstMloadheader;
	float distance, refresh_value;
	VECTOR point;
	MATRIX Mat_R;

	DstMloadheader = &MxaModelHeaders[ Models[ model ].ModelNum ];

	BuildRotMatrix( Models[ model ].Rot.x, Models[ model ].Rot.y, Models[ model ].Rot.z, &Mat_R);
	
	for( Group = 0; Group < DstMloadheader->num_groups; Group++ )
	{
		for( ExecBuf = 0; ExecBuf < DstMloadheader->Group[ Group ].num_execbufs; ExecBuf++ )
		{
			for ( Vert = 0; Vert < DstMloadheader->Group[ Group ].num_verts_per_execbuf[ ExecBuf ]; Vert++ )
			{
				point.x = DstMloadheader->Group[ Group ].org_vertpnt[ ExecBuf ][ Vert ].x;
				point.y = DstMloadheader->Group[ Group ].org_vertpnt[ ExecBuf ][ Vert ].y;
				point.z = DstMloadheader->Group[ Group ].org_vertpnt[ ExecBuf ][ Vert ].z;

				ApplyMatrix( &Mat_R, &point, &point );
				distance = POINT_TO_PLANE( &point, &Models[ model ].IntersectionPlane );
				distance = 0.0F - distance;
				
				//distance = (float)fabs((double)distance);
				if ( ( distance > 0.0F ) &&  ( distance < SCAN_WINDOW_WIDTH ) )
				{
					//refresh_value = 1.0F;

					refresh_value = 1.0F - ( distance / SCAN_WINDOW_WIDTH );
										
				}
			}
		}
	}

}

#define SHIP_MIN_FREQ 0.5F
#define SHIP_MAX_FREQ 1.0F
#define ENGINEUPDATEPAUSEFRAMES 5

float FrameLagStore[ MAX_PLAYERS ];
int CurrentStoredSpeed[ MAX_PLAYERS ];
float SpeedStore[ MAX_PLAYERS ][ ENGINEUPDATEPAUSEFRAMES ];


void InitShipSpeeds( void )
{
	memset( FrameLagStore, 0, sizeof( FrameLagStore ) );
	memset( CurrentStoredSpeed, 0, sizeof( CurrentStoredSpeed ) );
	memset( SpeedStore, 0, sizeof( SpeedStore ) );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Update SFX for Ships/Enemies
	Input		:	uint16		OwnerType
				:	uint16		OwenID
				:	uint32		SoundFX ID
				:	float		Speed
	Output		:	uint32		New SoundFX ID
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
uint32 EngineCode( uint16 OwnerType, uint16 OwnerID, uint32 SoundFX_ID, float Speed )
{
	float fracspeed, newfreq;
	int i;

	if ( OwnerType != OWNER_SHIP )
		return SoundFX_ID;

	if( OwnerID >= MAX_PLAYERS )
		return SoundFX_ID;
	
	if ( framelag )
	  	Speed /= framelag;
	else
		Speed = 0.0F;

	FrameLagStore[ OwnerID ] += framelag;

	while( ( FrameLagStore[ OwnerID ] >= 1.0F ) && ( CurrentStoredSpeed[ OwnerID ] < ENGINEUPDATEPAUSEFRAMES ) )
	{
		FrameLagStore[ OwnerID ] -= 1.0F;
		SpeedStore[ OwnerID ][ CurrentStoredSpeed[ OwnerID ]++ ] = Speed;
	}

	if ( CurrentStoredSpeed[ OwnerID ] >= ENGINEUPDATEPAUSEFRAMES )
	{
		CurrentStoredSpeed[ OwnerID ] = 0;
	}else
	{
		return SoundFX_ID;
	}

	Speed = 0;
	for( i = 0; i < ENGINEUPDATEPAUSEFRAMES; i++ )
	{
		Speed += SpeedStore[ OwnerID ][ i ];
	}
	
	Speed /= (float)ENGINEUPDATEPAUSEFRAMES;
	if ( Speed > MAXMOVESPEED )
		Speed = MAXMOVESPEED;
	
	//DebugPrintf("speed for owner type %d id %d = %d\n", OwnerType, OwnerID, Speed);

	switch( OwnerType )
	{
		case OWNER_SHIP:
			fracspeed = (float)fabs( (double) ( Speed / MAXMOVESPEED ) );
			newfreq = fracspeed * ( SHIP_MAX_FREQ - SHIP_MIN_FREQ ) + SHIP_MIN_FREQ;
			//DebugPrintf("using frequency of %f\n", newfreq );
			ModifyLoopingSfx( SoundFX_ID, newfreq, 0.0F );
			break;
		case OWNER_ENEMY:
			switch( Enemies[ OwnerID ].Type )
			{
				case ENEMY_BeamTurret:			// 0
				case ENEMY_DuelTurret:			// 1
				case ENEMY_PulseTurret:			// 2
				case ENEMY_MissileTurret:		// 3
				case ENEMY_SnubTurret:			// 4
				case ENEMY_LazerBot:			// 5
				case ENEMY_SnubBot:				// 6
				case ENEMY_AirMoble:			// 7
				case ENEMY_AmmoDump:			// 8
				case ENEMY_LeviTank:			// 9
				case ENEMY_Max:					// 10
				case ENEMY_Mekton:				// 11
				case ENEMY_Boss_Nukecrane:		// 12
				case ENEMY_Supressor:			// 13
				case ENEMY_MineSweeper:			// 14
				case ENEMY_Swarm:				// 15
				case ENEMY_Shade:				// 16
				case ENEMY_MineLayer:			// 17
				case ENEMY_Hunter:				// 18
				case ENEMY_Legz:				// 19
				case ENEMY_GuardBot:			// 20
				case ENEMY_Boss_Metatank:		// 21
				case ENEMY_Boss_BigGeek:		// 22
				case ENEMY_Boss_Avatar:			// 23
				case ENEMY_Boss_FleshMorph:		// 24
				case ENEMY_Boss_Exogenon:		// 25
				case ENEMY_Bike_Lokasenna:  	// 26
				case ENEMY_Bike_Beard:       	// 27
				case ENEMY_Bike_LAJay:    		// 28
				case ENEMY_Bike_ExCop:     		// 29
				case ENEMY_Bike_RexHardy:  		// 30
				case ENEMY_Bike_Foetoid:    	// 31
				case ENEMY_Bike_NimSoo:    		// 32
				case ENEMY_Bike_Nutta:      	// 33
				case ENEMY_Bike_Sceptre:    	// 34
				case ENEMY_Bike_Jo:         	// 35
				case ENEMY_Bike_CuvelClark:		// 36
				case ENEMY_Bike_HK5:       		// 37
				case ENEMY_Bike_Nubia:      	// 38
				case ENEMY_Bike_Mofisto:    	// 39
				case ENEMY_Bike_Cerbero:    	// 40
				case ENEMY_Bike_Slick:      	// 41
				case ENEMY_LEADER_Swarm:		// 42
				case ENEMY_LEADER_Hunter:		// 43
				case ENEMY_LEADER_Mekton:		// 44
				case ENEMY_LEADER_SnubBot:		// 45
				case ENEMY_LEADER_Legz:			// 46
				case ENEMY_LEADER_Shade:		// 47
				case ENEMY_LEADER_Supressor:	// 48
				case ENEMY_LEADER_LeviTank:		// 49
				case ENEMY_LEADER_LazerBot:		// 50
				case ENEMY_LEADER_Max:			// 51
				case ENEMY_LEADER_AirMoble:		// 52
				case ENEMY_Fodder1:				// 53
				case ENEMY_Boss_LittleGeek:		// 54
					break;

				default:
					break;
			}
			break;

		default:
			break;
	}

	return( SoundFX_ID );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Save Models Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
FILE * SaveModels( FILE * fp )
{
	uint16 i;

	if( fp )
	{
		fwrite( &FirstModelUsed, sizeof( FirstModelUsed ), 1, fp );
		fwrite( &FirstModelFree, sizeof( FirstModelFree ), 1, fp );
		
		i = FirstModelUsed;

		while( i != (uint16) -1 )
		{
			fwrite( &Models[ i ].Next, sizeof( uint16 ), 1, fp );
			fwrite( &Models[ i ].Prev, sizeof( uint16 ), 1, fp );
			fwrite( &Models[ i ].Type, sizeof( int16 ), 1, fp );
			fwrite( &Models[ i ].Flags, sizeof( uint16 ), 1, fp );
			fwrite( &Models[ i ].SecWeapon, sizeof( BYTE ), 1, fp );
			fwrite( &Models[ i ].Visible, sizeof( BOOL ), 1, fp );
			fwrite( &Models[ i ].ModelNum, sizeof( uint16 ), 1, fp );
			fwrite( &Models[ i ].Group, sizeof( uint16 ), 1, fp );
			fwrite( &Models[ i ].OwnerType, sizeof( uint16 ), 1, fp );
			fwrite( &Models[ i ].Owner, sizeof( uint16 ), 1, fp );
			fwrite( &Models[ i ].Func, sizeof( int16 ), 1, fp );
			fwrite( &Models[ i ].Scale, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].Xscale, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].Yscale, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].Zscale, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].MaxScale, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].LifeCount, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].AxisRot, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].Speed, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].TimeInterval, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].TimeCount, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].Frame, sizeof( int16 ), 1, fp );
			fwrite( &Models[ i ].InterpFrame, sizeof( int16 ), 1, fp );
			fwrite( &Models[ i ].InterpTime, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].Qlerp, sizeof( QUATLERP ), 1, fp );
			fwrite( &Models[ i ].Quat, sizeof( QUAT ), 1, fp );
			fwrite( &Models[ i ].Ship, sizeof( uint16 ), 1, fp );
			fwrite( &Models[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &Models[ i ].OldPos, sizeof( VECTOR ), 1, fp );
			fwrite( &Models[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fwrite( &Models[ i ].Rot, sizeof( VECTOR ), 1, fp );
			fwrite( &Models[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fwrite( &Models[ i ].Radius, sizeof( float ), 1, fp );
			fwrite( &Models[ i ].Red, sizeof( int ), 1, fp );
			fwrite( &Models[ i ].Green, sizeof( int ), 1, fp );
			fwrite( &Models[ i ].Blue, sizeof( int ), 1, fp );
			fwrite( &Models[ i ].SpotFXFlags[ 0 ], sizeof( Models[i].SpotFXFlags ), 1, fp );
			fwrite( &Models[ i ].SpotFXState[ 0 ], sizeof( Models[i].SpotFXState ), 1, fp );
			fwrite( &Models[ i ].SpotFXDelay[ 0 ], sizeof( Models[i].SpotFXDelay ), 1, fp );
			fwrite( &Models[ i ].SpotFXTimeInterval[ 0 ], sizeof( Models[i].SpotFXTimeInterval ), 1, fp );
			fwrite( &Models[ i ].SpotFXTimeCount[ 0 ], sizeof( Models[i].SpotFXTimeCount ), 1, fp );
			fwrite( &Models[ i ].SpotFXSFX_ID[ 0 ], sizeof( Models[i].SpotFXSFX_ID ), 1, fp );
			fwrite( &Models[ i ].ClipGroup, sizeof( uint16 ), 1, fp );
			i = Models[ i ].Prev;
		}

		i = FirstModelFree;

		while( i != (uint16) -1 )
		{
			fwrite( &Models[ i ].Next, sizeof( uint16 ), 1, fp );
			i = Models[ i ].Next;
		}

	}

	return( fp );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Load Models Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
FILE * LoadModels( FILE * fp )
{
	uint16	i;
	int16	Count;

	if( fp )
	{
		fread( &FirstModelUsed, sizeof( FirstModelUsed ), 1, fp );
		fread( &FirstModelFree, sizeof( FirstModelFree ), 1, fp );
		
		i = FirstModelUsed;

		while( i != (uint16) -1 )
		{
			fread( &Models[ i ].Next, sizeof( uint16 ), 1, fp );
			fread( &Models[ i ].Prev, sizeof( uint16 ), 1, fp );
			fread( &Models[ i ].Type, sizeof( int16 ), 1, fp );
			fread( &Models[ i ].Flags, sizeof( uint16 ), 1, fp );
			fread( &Models[ i ].SecWeapon, sizeof( BYTE ), 1, fp );
			fread( &Models[ i ].Visible, sizeof( BOOL ), 1, fp );
			fread( &Models[ i ].ModelNum, sizeof( uint16 ), 1, fp );
			fread( &Models[ i ].Group, sizeof( uint16 ), 1, fp );
			fread( &Models[ i ].OwnerType, sizeof( uint16 ), 1, fp );
			fread( &Models[ i ].Owner, sizeof( uint16 ), 1, fp );
			fread( &Models[ i ].Func, sizeof( int16 ), 1, fp );
			fread( &Models[ i ].Scale, sizeof( float ), 1, fp );
			fread( &Models[ i ].Xscale, sizeof( float ), 1, fp );
			fread( &Models[ i ].Yscale, sizeof( float ), 1, fp );
			fread( &Models[ i ].Zscale, sizeof( float ), 1, fp );
			fread( &Models[ i ].MaxScale, sizeof( float ), 1, fp );
			fread( &Models[ i ].LifeCount, sizeof( float ), 1, fp );
			fread( &Models[ i ].AxisRot, sizeof( float ), 1, fp );
			fread( &Models[ i ].Speed, sizeof( float ), 1, fp );
			fread( &Models[ i ].TimeInterval, sizeof( float ), 1, fp );
			fread( &Models[ i ].TimeCount, sizeof( float ), 1, fp );
			fread( &Models[ i ].Frame, sizeof( int16 ), 1, fp );
			fread( &Models[ i ].InterpFrame, sizeof( int16 ), 1, fp );
			fread( &Models[ i ].InterpTime, sizeof( float ), 1, fp );
			fread( &Models[ i ].Qlerp, sizeof( QUATLERP ), 1, fp );
			Models[ i ].Qlerp.crnt = &Models[ i ].Quat;
			fread( &Models[ i ].Quat, sizeof( QUAT ), 1, fp );
			fread( &Models[ i ].Ship, sizeof( uint16 ), 1, fp );
			fread( &Models[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &Models[ i ].OldPos, sizeof( VECTOR ), 1, fp );
			fread( &Models[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fread( &Models[ i ].Rot, sizeof( VECTOR ), 1, fp );
			fread( &Models[ i ].Mat, sizeof( MATRIX ), 1, fp );
			MatrixTranspose( &Models[ i ].Mat, &Models[ i ].InvMat );
			fread( &Models[ i ].Radius, sizeof( float ), 1, fp );
			fread( &Models[ i ].Red, sizeof( int ), 1, fp );
			fread( &Models[ i ].Green, sizeof( int ), 1, fp );
			fread( &Models[ i ].Blue, sizeof( int ), 1, fp );
			fread( &Models[ i ].SpotFXFlags[ 0 ], sizeof( Models[i].SpotFXFlags ), 1, fp );
			fread( &Models[ i ].SpotFXState[ 0 ], sizeof( Models[i].SpotFXState ), 1, fp );
			fread( &Models[ i ].SpotFXDelay[ 0 ], sizeof( Models[i].SpotFXDelay ), 1, fp );
			fread( &Models[ i ].SpotFXTimeInterval[ 0 ], sizeof( Models[i].SpotFXTimeInterval ), 1, fp );
			fread( &Models[ i ].SpotFXTimeCount[ 0 ], sizeof( Models[i].SpotFXTimeCount ), 1, fp );
			fread( &Models[ i ].SpotFXSFX_ID[ 0 ], sizeof( Models[i].SpotFXSFX_ID ), 1, fp );
			fread( &Models[ i ].ClipGroup, sizeof( uint16 ), 1, fp );

			ReinitSpotFXSFX( i );

			i = Models[ i ].Prev;
		}

		i = FirstModelFree;

		while( i != (uint16) -1 )
		{
			memset( &Models[ i ], 0, sizeof( MODEL ) );
			Models[i].Func = MODFUNC_Nothing;
			Models[i].LifeCount = -1.0F;
			Models[i].Scale = 1.0F;
			Models[i].Visible = TRUE;
			Models[i].TimeInterval = (float) ( 11 - TrailDetailSlider.value );
			for( Count = 0; Count < 12; Count++ ) Models[i].TempLines[ Count ] = (uint16) -1;
			Models[i].Prev = (uint16) -1;

			fread( &Models[ i ].Next, sizeof( uint16 ), 1, fp );
			i = Models[ i ].Next;
		}
	}

	return( fp );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Reinitialise Sound FX on Models
	Input		:	uint16		Model Index
	Output		:	BOOL		True/False
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL ReinitSpotFXSFX( uint16 i )
{
	int16		Count;
	uint16		NumSpotFX = 0;
	PVSPOTFX *	SpotFXPtr = NULL;

	if( ModelNames[ Models[i].ModelNum ].DoIMorph )
	{
		if( MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
			MxaModelHeaders[ Models[i].ModelNum ].SpotFX )
		{
			SpotFXPtr = MxaModelHeaders[ Models[i].ModelNum ].SpotFX;
			NumSpotFX = MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX;
		}
	}
	else
	{
		if( ModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
			ModelHeaders[ Models[i].ModelNum ].SpotFX )
		{
			SpotFXPtr = ModelHeaders[ Models[i].ModelNum ].SpotFX;
			NumSpotFX = ModelHeaders[ Models[i].ModelNum ].NumSpotFX;
		}
	}

	if( SpotFXPtr && NumSpotFX )
	{
		for( Count = 0; Count < NumSpotFX; Count++ )
		{
			switch( SpotFXPtr->Type )
			{
				case SPOTFX_SoundFX:
					if( SpotFXPtr->SoundFX != -1 )
					{
						if( Models[i].SpotFXSFX_ID[ Count ] )
						{
							RestoreSfxData( Models[i].SpotFXSFX_ID[ Count ], &Models[i].Pos, &Models[i].Group );
						}
					}
					break;
				default:
					break;
			}
			SpotFXPtr++;
		}
	}
	return( TRUE );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Set Ship Display/Not depending on camera
	Input		:	Nothing
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void SetShipsVisibleFlag( void )
{
	int16	Ship;
	uint16	Model;
	BOOL	ShipVisible;

	for( Ship = 0; Ship < MAX_PLAYERS; Ship++ )
	{
		ShipVisible = FALSE;

		if( ( Ships[ Ship ].enable ) &&
			( Ships[ Ship ].Object.Mode != GAMEOVER_MODE ) &&
			( Ships[ Ship ].Object.Mode != LIMBO_MODE ) &&
			( (GameStatus[ Ship ] == STATUS_Normal ) || (GameStatus[ Ship ] == STATUS_SinglePlayer ) ) )
		{
			if( Ship != WhoIAm )
			{
				if( PlayDemo && ( Current_Camera_View == Ship ) )
				{
					ShipVisible = FALSE;
				}
				else
				{
					ShipVisible = TRUE;
				}
			}
			else
			{
				if( ( CameraRendering != CAMRENDERING_Main ) &&
					( CameraRendering != CAMRENDERING_Rear ) )
				{
					ShipVisible = TRUE;
				}
			}
		}
			
		if( BikeCompFiles[ ( Ships[ Ship ].BikeNum % MAXBIKETYPES ) ].Filename[ 0 ] )
		{
			if( Ships[ Ship ].Object.Components )
			{
				SetCompObjModelsState( Ships[ Ship ].Object.Components, 1, ShipVisible );
			}
		}
		else
		{
			if( Ships[ Ship ].ModelNum != (uint16) -1 )
			{
				Model = Ships[ Ship ].ModelNum;
				Models[ Model ].Visible = ShipVisible;
			}
		}
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Enable and Disable Models dependant on
				:	Level/Game mode
	Input		:	Nothing
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void EnableRelavantModels( MODELNAME * ModelNamesPtr )
{
	if( !ModelNamesPtr ) return;
	if( ModelNamesPtr != &ModelNames[0] ) return;

	if( ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
		( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
		( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) )
	{
		ModelNamesPtr[ MODEL_Tracker ].LoadEnable = FALSE;
		ModelNamesPtr[ MODEL_Ping ].LoadEnable = FALSE;
		ModelNamesPtr[ MODEL_Flag ].LoadEnable = FALSE;
		ModelNamesPtr[ MODEL_RedFlag ].LoadEnable = FALSE;
		ModelNamesPtr[ MODEL_GreenFlag ].LoadEnable = FALSE;
		ModelNamesPtr[ MODEL_BlueFlag ].LoadEnable = FALSE;
		ModelNamesPtr[ MODEL_YellowFlag ].LoadEnable = FALSE;
	}
	else
	{
		ModelNamesPtr[ MODEL_Tracker ].LoadEnable = FALSE;
		ModelNamesPtr[ MODEL_Ping ].LoadEnable = FALSE;

		if( CaptureTheFlag ) ModelNamesPtr[ MODEL_Flag ].LoadEnable = TRUE;
		else ModelNamesPtr[ MODEL_Flag ].LoadEnable = FALSE;

		if( CTF )
		{
			ModelNamesPtr[ MODEL_RedFlag ].LoadEnable = TRUE;
			ModelNamesPtr[ MODEL_GreenFlag ].LoadEnable = TRUE;
			ModelNamesPtr[ MODEL_BlueFlag ].LoadEnable = TRUE;
			ModelNamesPtr[ MODEL_YellowFlag ].LoadEnable = TRUE;
		}
		else
		{
			ModelNamesPtr[ MODEL_RedFlag ].LoadEnable = FALSE;
			ModelNamesPtr[ MODEL_GreenFlag ].LoadEnable = FALSE;
			ModelNamesPtr[ MODEL_BlueFlag ].LoadEnable = FALSE;
			ModelNamesPtr[ MODEL_YellowFlag ].LoadEnable = FALSE;
		}
	}
}




void CheckModelLinkList( void )
{
	uint16 i;
	uint16 Count;


	
	i = 0;
	Count = FirstModelFree;
	while( ( Count != (uint16)-1 ) && i < MAXNUMOFMODELS*2 )
	{
		Count = Models[Count].Next;
		i++;
	}
	if( i == MAXNUMOFMODELS*2 )
	{
		// oh shit
        Msg( "Model Free link list Gone up its ass\n" );
		i = 0;
	}

	i = 0;

	Count = FirstModelUsed;
	while( ( Count != (uint16)-1 ) && i < MAXNUMOFMODELS*2 )
	{
		if( Models[Count].Prev == Models[Count].Next )
		{
			i = MAXNUMOFMODELS*2;
			break;
		}
		Count = Models[Count].Prev;
		i++;
	}
	if( i == MAXNUMOFMODELS*2 )
	{
		// oh shit
        Msg( "Model Used link list Gone up its ass\n" );
		i = 0;
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Kill all bike engine sound
	Input		:	Nothing
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
void KillAllBikeEngines( void )
{
	uint16		NumSpotFX;
	PVSPOTFX *	SpotFXPtr;
	int16		Count;
	uint16		i;

	i = FirstModelUsed;

	while( i != (uint16) -1 )
	{
		switch( Models[i].Func )
		{
			case MODFUNC_ProcessSpotFX:
				if( ModelNames[ Models[i].ModelNum ].DoIMorph )
				{
					if( MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
						MxaModelHeaders[ Models[i].ModelNum ].SpotFX )
					{
						SpotFXPtr = MxaModelHeaders[ Models[i].ModelNum ].SpotFX;
						NumSpotFX = MxaModelHeaders[ Models[i].ModelNum ].NumSpotFX;
					}
				}
				else
				{
					if( ModelHeaders[ Models[i].ModelNum ].NumSpotFX &&
						ModelHeaders[ Models[i].ModelNum ].SpotFX )
					{
						SpotFXPtr = ModelHeaders[ Models[i].ModelNum ].SpotFX;
						NumSpotFX = ModelHeaders[ Models[i].ModelNum ].NumSpotFX;
					}
				}

				if( SpotFXPtr && NumSpotFX )
				{
					for( Count = 0; Count < NumSpotFX; Count++ )
					{
						switch( SpotFXPtr->Type )
						{
							case SPOTFX_SoundFX:
								if( SpotFXPtr->SoundFX != -1 )
								{
									if( ( Models[i].OwnerType == OWNER_SHIP ) &&
										( Models[i].Owner < MAX_PLAYERS ) )
									{
										if( Models[i].SpotFXSFX_ID[ Count ] )
										{
											StopSfx( Models[i].SpotFXSFX_ID[ Count ] );
											Models[i].SpotFXSFX_ID[ Count ] = 0;
											Models[i].SpotFXFlags[ Count ] &= ~SPOTFX_FLAGS_DoneOnce;
										}
									}
								}
								break;
							default:
								break;
						}
						SpotFXPtr++;
					}
				}
				break;

			default:
				break;
		}

		i = Models[ i ].Prev;
	}
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Procedure	:	Check if bike engine enabled and is ship if so then
				:	return TRUE else return FALSE
	Output		:	Nothing
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL EngineEnabled( uint16 OwnerType, uint16 Owner )
{
	if( BikeEnginesOn ) return( TRUE );
	if( ( OwnerType != OWNER_SHIP ) || ( Owner >= MAX_PLAYERS ) ) return( TRUE );
	return( FALSE );
}

/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
		True EnviroMent Mapping for an Mxaloadheader...
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
BOOL	ENVMxa( MXALOADHEADER * Mxloadheader , MATRIX * Mat ,VECTOR * Pos)
{
	VECTOR Temp;
	D3DEXECUTEBUFFERDESC	debDesc;
	uint16 group;
	uint16 vert;
	uint16 execbuf;
	LPD3DLVERTEX	lpD3DLVERTEX;
	float	u,v;

	for( group = 0 ; group < Mxloadheader->num_groups ; group ++ )
	{
		for( execbuf = 0 ; execbuf < Mxloadheader->Group[group].num_execbufs ; execbuf++ )
		{
			if( (Mxloadheader->Group[group].exec_type[execbuf]&HASENVIROMENTMAP) != 0 )
			{
				/*	lock the execute buffer	*/
				memset(&debDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
				debDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
				if ( Mxloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Lock( Mxloadheader->Group[group].lpExBuf[execbuf], &debDesc ) != D3D_OK)
					return FALSE ;
				lpD3DLVERTEX = ( LPD3DLVERTEX ) debDesc.lpData;
				for( vert = 0 ; vert < 	Mxloadheader->Group[group].num_verts_per_execbuf[execbuf] ; vert ++ )
				{
					ApplyMatrix( Mat , (VECTOR *) lpD3DLVERTEX,  &Temp);

					NormaliseVector( &Temp );

					u = (0.25f + (-0.25f * Temp.x) );
					v = (0.5f + (-0.5f * Temp.y) );

					lpD3DLVERTEX->tu = u;//+(Pos->x*0.001F)+(Pos->z*0.001F);
					lpD3DLVERTEX->tv = v;//+(Pos->y*0.001F);
					lpD3DLVERTEX++;
				}
				/*	unlock the execute buffer	*/
				if ( Mxloadheader->Group[group].lpExBuf[execbuf]->lpVtbl->Unlock( Mxloadheader->Group[group].lpExBuf[execbuf] ) != D3D_OK)
					return FALSE ;
			}
		}
	}
	return TRUE;
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

