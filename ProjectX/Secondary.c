/*
 * Second.c
 *
 * Secondary weapon routines
 *
 * Project X, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 501 $
 *
 * $Header: /PcProjectX/Secondary.c 501   11/11/98 16:00 Philipy $
 *
 * $Log: /PcProjectX/Secondary.c $
 * 
 * 501   11/11/98 16:00 Philipy
 * various fixes for warnings / errors when compiling under VC6
 * 
 * 500   21/10/98 16:09 Collinsd
 * 
 * 499   21/10/98 12:39 Phillipd
 * 
 * 498   21/10/98 12:07 Collinsd
 * 
 * 497   14/09/98 16:12 Phillipd
 * 
 * 496   14/09/98 16:00 Phillipd
 * 
 * 495   14/09/98 15:16 Phillipd
 * 
 * 494   14/09/98 12:42 Phillipd
 * 
 * 493   24/07/98 12:36 Collinsd
 * 
 * 492   24/07/98 11:34 Collinsd
 * Fixed dropping of pickups and scattering them again
 * 
 * 491   15/07/98 10:27 Collinsd
 * Prediction code added to pickups in server game.
 * 
 * 490   15/07/98 9:42 Collinsd
 * Fixed homing missiles targeting mines.
 * 
 * 489   13/07/98 9:36 Collinsd
 * 
 * 488   13/07/98 9:27 Collinsd
 * Fixed problem of not collecting pickup when thrown out.
 * 
 * 487   7/08/98 11:56a Phillipd
 * 
 * 486   8/07/98 9:31 Oliverc
 * Converted multiplayer bounty and flag games to server operation for
 * patch
 * 
 * 485   3/07/98 14:29 Collinsd
 * Moved HostRestoreAmmo and Weapons into scatterweaponsforship
 * 
 * 484   2/07/98 14:10 Collinsd
 * Temp Cheat to wind up peacefrom and sint.
 * 
 * 483   1/07/98 15:32 Collinsd
 * Got rid of warnings.
 * 
 * 482   1/07/98 15:30 Collinsd
 * Hopefully fixed bug that lost pickups.
 * 
 * 481   30/06/98 10:39 Collinsd
 * Better
 * 
 * 480   26/06/98 17:46 Collinsd
 * Added server debug info.
 * 
 * 479   26/06/98 10:55 Collinsd
 * Server Pickups and weapons works pretty well, death mode pickups thrown
 * out, as well as scattered.  Quitting/Crash players throw out all
 * pickups.
 * 
 * 478   8/06/98 15:45 Collinsd
 * Done more on Server mode and Fixed trojax ammo usage
 * 
 * 477   13/05/98 10:52 Collinsd
 * Updated collision with bgobjects and pickups.  Also changed radius of
 * mine detection for regeneration of bikes.
 * 
 * 476   24/04/98 1:37 Philipy
 * weapon order text now comes from local files
 * 
 * 475   15/04/98 12:31 Oliverc
 * In-game text substituted for localised definitions
 * 
 * 474   14/04/98 11:50 Philipy
 * changed purge mine sfx sound effect
 * 
 * 473   11/04/98 18:07 Collinsd
 * 
 * 472   10/04/98 12:54 Collinsd
 * Names appearing in demo playback ( Needs option on menu )
 * Bikes now clip against camera
 * 
 * 471   9/04/98 19:20 Collinsd
 * Can't fire in demo mode.
 * 
 * 470   8/04/98 10:16 Philipy
 * added weapon select sfx for changing weapon with no bike computer
 * 
 * 469   6/04/98 17:06 Philipy
 * modified various sfx
 * 
 * 468   6/04/98 11:31 Collinsd
 * Added show restart zone to debug menu.
 * 
 * 467   6/04/98 10:09 Collinsd
 * MFRL now framelagged properly
 * 
 * 466   5/04/98 19:49 Collinsd
 * Added GlobalFramelagAddition.
 * 
 * 465   3/04/98 17:02 Philipy
 * cd audio now only retriggered if enabled!
 * added generic pickup sound if bike computer speech is zero
 * 
 * 464   3/04/98 16:37 Collinsd
 * Done titan shrapnel as one message,
 * 
 * 463   3/04/98 11:55 Collinsd
 * Tidy
 * 
 * 462   2/04/98 21:07 Philipy
 * Added taunts ( single & multiplayer, plus enemy biker taunts )
 * added flygirl to front end.
 * sliders for biker, bike computer and taunt speech volume
 * added new sfx for title
 * 
 * 461   30/03/98 21:35 Collinsd
 * 
 * 460   30/03/98 15:54 Collinsd
 * 
 * 459   30/03/98 12:22 Collinsd
 * Added Titan Launch SFX
 * 
 * 458   29/03/98 19:10 Collinsd
 * Nitro thrown out in single player.
 * 
 * 457   3/28/98 10:06a Phillipd
 * 
 * 456   24/03/98 16:20 Philipy
 * added new sfx
 * 
 * 455   17/03/98 20:29 Collinsd
 * Fixed pinemines targeting own team mates mines.
 * 
 * 454   14/03/98 18:59 Collinsd
 * Added godmode and made debug mode work even when you change level.
 * 
 * 453   14/03/98 14:06 Collinsd
 * Scatter works with Collision Perspective.
 * 
 * 452   13/03/98 9:12 Collinsd
 * Upded damage
 * 
 * 451   12/03/98 21:52 Collinsd
 * Added cheatmodes for primary and seconadry weapons.
 * 
 * 450   10/03/98 22:25 Collinsd
 * Goals framelagged, flag and names appear if outside map again.
 * 
 * 449   10/03/98 15:54 Collinsd
 * Qued Mines now work.
 * 
 * 448   9/03/98 16:22 Collinsd
 * Mines selectable for dropping as well.
 * 
 * 447   7/03/98 19:10 Collinsd
 * Added new message for olly ( TeamGoals ) Also added pos and group to
 * explodesecondary.
 * 
 * 446   6/03/98 14:25 Collinsd
 * 
 * 445   6/03/98 12:09 Collinsd
 * Taken out drop pickup code.
 * 
 * 444   5/03/98 12:06 Collinsd
 * Fixed pickup crushing generating withing crushing object bounds.
 * 
 * 443   4/03/98 16:10 Collinsd
 * 
 * 442   4/03/98 12:33 Oliverc
 * CTF mode fully enabled
 * 
 * 441   3/03/98 18:20 Collinsd
 * CTF Team game
 * 
 * 440   3/03/98 17:43 Oliverc
 * Fixed bug: flags & bounty now only thwon out if in correct game mode
 * (needed when re-using Ships[].Object.Flags bits in different modes)
 * 
 * 439   3/03/98 16:59 Oliverc
 * New multiplayer CTF mode stuff (1st attempt)
 * 
 * 438   2/03/98 21:14 Collinsd
 * No longer use multiple bit or secfire.
 * 
 * 437   27/02/98 19:42 Collinsd
 * Fixed pickups sending message if init failed
 * 
 * 436   27/02/98 18:56 Collinsd
 * Added Sfx for next/prev secondary
 * 
 * 435   25/02/98 18:40 Collinsd
 * 
 * 434   25/02/98 18:38 Collinsd
 * Added solid poly option.
 * 
 * 433   24/02/98 16:55 Oliverc
 * 1st attempt at bounty hunt multiplayer game
 * 
 * 432   23/02/98 22:05 Collinsd
 * Added Bounty
 * 
 * 431   23/02/98 21:20 Collinsd
 * Optimised Load/Save Functions.
 * 
 * 430   21/02/98 14:29 Collinsd
 * Added dan's new enemy code and fixed flashing of team in capture the
 * flag.
 * 
 * 429   20/02/98 20:56 Collinsd
 * Added FlagDisplay Code.
 * 
 * 428   20/02/98 19:41 Oliverc
 * 2nd prototype of capture the flag game
 * 
 * 427   18/02/98 19:42 Oliverc
 * Enabled scatter, pine mine & quantum mine for non-SHAREWARE version
 * 
 * 425   16/02/98 21:38 Collinsd
 * Started Save/Load BGObjects. 
 * 
 * 424   16/02/98 14:34 Collinsd
 * Started LoadSave
 * 
 * 423   12/02/98 14:09 Collinsd
 * Fixed targeting of bosses by homing missiles.
 * 
 * 422   11/02/98 16:29 Collinsd
 * Added soundfx for bikes/enemies
 * 
 * 421   11/02/98 12:57 Philipy
 * Changed PlaySfx calls to use Vol instead of Dist
 * 
 * 420   10/02/98 11:40 Collinsd
 * Added code for descent/forsaken collision
 * 
 * 419   9/02/98 18:29 Collinsd
 * Fixed damage after death, spinning caused by damage, and passing
 * through mines.
 * 
 * 418   7/02/98 13:23 Collinsd
 * Added polygonal collision to enemies.
 * 
 * 417   4/02/98 19:22 Collinsd
 * Fixed pine mines targeting wrong enemies.  Also fixed pine mines not
 * shooting players in a multiplayer.
 * 
 * 416   3/02/98 15:30 Collinsd
 * Added Fireballs for fleshmorph and ability to disable spotfx on models,
 * from within the animation
 * 
 * 415   2/02/98 10:06 Collinsd
 * Reduced poly's on splash and splash does not effect water.
 * 
 * 414   30/01/98 17:13 Collinsd
 * 
 * 413   30/01/98 14:49 Collinsd
 * 
 * 412   30/01/98 13:53 Collinsd
 * 
 * 411   30/01/98 13:48 Collinsd
 * Added Depth Charge.
 * 
 * 410   30/01/98 10:59 Collinsd
 * 
 * 409   30/01/98 10:58 Collinsd
 * Added Correction code for laser direction
 * 
 * 408   29/01/98 12:06 Collinsd
 * Added fireball, blue photon, tentacle and changed spotfx laser.
 * 
 * 407   19/01/98 20:45 Collinsd
 * Fixed error checking for fire primary and fire secondary.
 * 
 * 406   19/01/98 20:10 Collinsd
 * Spotfx bug fixed. Added sparks sfx
 * 
 * 405   19/01/98 11:48 Collinsd
 * Fixed Mines dissapearing.
 * 
 * 404   19/01/98 9:36 Collinsd
 * Fixed heatseaker target.
 * 
 * 403   19/01/98 9:09 Collinsd
 * Crushed pickups now regernerate.
 * 
 * 402   17/01/98 12:46 Collinsd
 * Hopefully fixed seconday link list corrupt error.
 * 
 * 401   16/01/98 18:01 Oliverc
 * Only disabled selected weapons for SHAREWARE instead of always...
 * 
 * 400   16/01/98 16:04 Collinsd
 * Updated strings for secondary weapons.
 * 
 * 399   1/16/98 3:59p Phillipd
 * 
 * 398   16/01/98 14:09 Collinsd
 * Change reinit mine code.
 * 
 * 397   15/01/98 17:05 Philipy
 * changed PlayPannedSfx calls to use float for freq
 * 
 * 396   15/01/98 15:53 Collinsd
 * ????????
 * 
 * 395   15/01/98 12:51 Collinsd
 * Time now works when people join an existing timed game.  Crystals
 * reinitialised.  Damage Flash only in Normal_Mode.
 * 
 * 394   15/01/98 10:15 Collinsd
 * Hopefully fixed link list fuckup.  Pickups can only be generated once.
 * Started of fixing mine bug.  Updated MX, MXA and Spotfx formats.
 * 
 * 393   13/01/98 17:37 Collinsd
 * Added back pyrolite fuel,  Changes secondary to try to get rid of any
 * chance of any link list corruption.
 * 
 * 392   13/01/98 9:39 Collinsd
 * Added debug to show untriggered enemies.  Started Enemy Poly Colisions.
 * 
 * 391   10/01/98 20:35 Philipy
 * fixed no speech option
 * 
 * 390   10/01/98 19:31 Philipy
 * bug fixes
 * 
 * 389   8/01/98 9:27 Collinsd
 * Fixed Andy's Spotfx problems.
 * 
 * 388   5/01/98 14:08 Philipy
 * selecting a weapon now plays correct sfx
 * 
 * 387   5/01/98 10:38 Philipy
 * speech sfx implemented - currently defaults to 1 biker & computer only,
 * none selectable
 * 
 * 386   5/01/98 10:39 Collinsd
 * Water Splash is now tinted by water colour.
 * 
 * 385   3/01/98 19:14 Collinsd
 * Added Splash for secondary weapons
 * 
 * 384   2/01/98 17:52 Collinsd
 * Destroy at end better.
 * 
 * 383   1/02/98 11:13a Phillipd
 * 
 * 382   31/12/97 11:53 Collinsd
 * Added destroyatend explosion, and did a crap burn for enemies (
 * disabled )
 * 
 * 381   30/12/97 10:45 Collinsd
 * Added enemy damage modifier as variable.
 * 
 * 380   29/12/97 20:19 Collinsd
 * Added enemy damage modifier.
 * 
 * 379   29/12/97 19:53 Philipy
 * various bug fixes
 * 
 * 378   29/12/97 15:52 Collinsd
 * Spotfx fireprimary and firesecondary can now be seed in camera views.
 * 
 * 377   29/12/97 10:42 Collinsd
 * Pickups now use new collision
 * 
 * 376   19/12/97 16:16 Collinsd
 * Taken out burning effect  when in game over mode.
 * 
 * 375   12/19/97 11:14a Phillipd
 * 
 * 374   12/18/97 5:37p Phillipd
 * 
 * 373   12/18/97 2:47p Phillipd
 * 
 * 372   11/12/97 17:21 Collinsd
 * Titan crash fixed.
 * 
 * 371   11/12/97 15:53 Collinsd
 * Fixed enemy mines that didn't hurt you.  Also fixed enemy homing
 * missile.
 * 
 * 370   11/12/97 9:20 Collinsd
 * Fixed titan
 * 
 * 369   10/12/97 17:19 Collinsd
 * Fixed titan star exploding next to portals and generating non collision
 * lines.
 * 
 * 368   10/12/97 14:19 Collinsd
 * Fixed secondary collision.
 * 
 * 367   9/12/97 10:45 Collinsd
 * Done ship burning and change body parts and pickups now explode in
 * direction of ship movement.
 * 
 * 366   8/12/97 11:34 Collinsd
 * Death flames, and target info changed.
 * 
 * 365   6/12/97 19:22 Collinsd
 * Added burning effect on deathmode.
 * 
 * 364   5/12/97 17:33 Collinsd
 * Bullets/missiles generated if player outide map.
 * 
 * 363   2/12/97 17:41 Collinsd
 * Sfx when hit not played twice any more.
 * 
 * 362   2/12/97 14:55 Collinsd
 * Thief missile now normal missile ( Doesn't hurt you still )
 * 
 * 361   2/12/97 14:13 Collinsd
 * Changes to mine destruction.
 * 
 * 360   28/11/97 19:01 Collinsd
 * Fixed nmelightning, fixed mines not owned by player, fixed targeting of
 * non player missiles.
 * 
 * 359   25/11/97 12:56 Collinsd
 * Added Photon Missile
 * 
 * 358   22/11/97 13:34 Collinsd
 * Mines work again, scattering of enemies works properly. Pulsar double
 * shot only creates on sfx.
 * 
 * 357   11/22/97 1:23p Phillipd
 * 
 * 356   21/11/97 12:26 Collinsd
 * Added invulnerability to enemies and -1 on spotfx now works.
 * 
 * 355   21/11/97 9:09 Collinsd
 * Taken out all models not used.
 * 
 * 354   11/20/97 11:44a Phillipd
 * 
 * 353   18/11/97 15:24 Collinsd
 * Fixed scattering enemies.
 * 
 * 352   15/11/97 16:57 Collinsd
 * Secondary weapons now effect water, and bubbles hitting surface effect
 * water.
 * 
 * 351   15/11/97 15:20 Collinsd
 * Added Bubbles
 * 
 * 350   13/11/97 15:53 Collinsd
 * Fixed secbull link list fuck up.
 * 
 * 349   13/11/97 15:11 Collinsd
 * Tinted blood.
 * 
 * 348   13/11/97 9:54 Collinsd
 * Taken off !no collision as condition to create lines.
 * 
 * 347   12/11/97 18:17 Collinsd
 * Added what weapon activates trigger area code ( Disabled for now )
 * 
 * 346   11/11/97 12:56 Collinsd
 * Added slow fireing when no ammo and mxa for enemies.
 * 
 * 345   11/11/97 9:19 Collinsd
 * Added scattering of enemies.
 * 
 * 344   10/11/97 17:59 Collinsd
 * Added zone checks for when player or enemy shoots.
 * 
 * 343   6/11/97 11:37 Collinsd
 * Added BGObject Lighting, Stopped primary weapons creating lines when
 * outside group and made secondary weapons create debug lines when no
 * collision.
 * 
 * 342   10/30/97 9:30a Phillipd
 * Bikes with different mods started...
 * 
 * 341   29/10/97 19:42 Collinsd
 * Fixed scatter weapons in single player.
 * 
 * 340   25/10/97 11:19 Collinsd
 * You can now pickup ammo when you have supernasram weapon
 * 
 * 339   24/10/97 17:08 Collinsd
 * Pickups no longer dissapear in single player.
 * 
 * 338   22/10/97 16:57 Collinsd
 * Fixed group link list bug on secondary weapons
 * 
 * 337   15/10/97 19:06 Collinsd
 * Fixed guts comming out bug. and fixed mutually visible group bug.
 * 
 * 336   14/10/97 16:56 Collinsd
 * Added olly's visible group stuff
 * 
 * 335   10/09/97 3:29p Phillipd
 * External forces coming...
 * Shield and Hull now floats
 * 
 * 334   4/10/97 15:58 Collinsd
 * Added new spiraling missile and took out old morph code.
 * 
 * 333   4/10/97 12:53 Collinsd
 * Fixed level change. ( Preload Enemies ).
 * 
 * 332   1/10/97 17:49 Collinsd
 * Titan back to normal.
 * 
 * 331   30/09/97 17:49 Collinsd
 * Fixed small bug in group assignment for mines.
 * 
 * 330   30/09/97 8:46 Collinsd
 * Fixed host picking pickups immediatly after being scattered and fixed
 * mines not being destroyed bug Bliz found.
 * 
 * 329   29/09/97 11:35 Collinsd
 * New bgobject stuff enabled. Pickups/Secondary and Enemies now have
 * group link lists.  Flags for secondary bullets being targeted and hit
 * now added.
 * 
 * 328   25/09/97 16:58 Collinsd
 * Added group link list to pickups/secondary added more code for
 * bgobjects.
 * 
 * 327   24/09/97 16:55 Collinsd
 * Added new bgobject stuff.  Added bounding box calc and display code.
 * and changes bgobject state changes.
 * 
 * 326   22/09/97 10:41 Collinsd
 * Titan no longer targets players on your team.
 * 
 * 325   9/18/97 12:16p Phillipd
 * 
 * 324   15/09/97 10:10 Collinsd
 * Added secbull number for close mines.
 * 
 * 323   13/09/97 14:17 Collinsd
 * Less than or equal
 * 
 * 322   13/09/97 13:55 Collinsd
 * Again, and again, i'll just go home!
 * 
 * 321   13/09/97 13:39 Collinsd
 * Again!!!
 * 
 * 320   13/09/97 13:26 Collinsd
 * Titan code bug fixed
 * 
 * 319   13/09/97 13:02 Collinsd
 * Changed Enemy mine awarness code.
 * 
 * 318   9/12/97 4:49p Phillipd
 * 
 * 317   12/09/97 12:18 Collinsd
 * Mines work now..
 * 
 * 316   12/09/97 11:17 Collinsd
 * Mines now tell enemies where they are and collide when close enough
 * 
 * 315   4/09/97 15:26 Collinsd
 * Team colours now correct
 * 
 * 314   2/09/97 16:39 Collinsd
 * Cloaked team players now have names above them.
 * 
 * 313   2/09/97 15:26 Collinsd
 * Pine mines target enemies/mines.
 * 
 * 312   2/09/97 9:06 Collinsd
 * Added one off avoid into enemy flags
 * 
 * 311   2/09/97 8:54 Collinsd
 * Triggered enemies now works again.  Rotated hunter enemy
 * 
 * 310   1/09/97 14:18 Collinsd
 * Added enemy targeted pos, dir and flag.
 * 
 * 309   1/09/97 11:48 Collinsd
 * 
 * 308   1/09/97 11:37 Collinsd
 * Enemies target ships and ships target enemies/mines when homing.
 * 
 * 307   27/08/97 15:38 Collinsd
 * Taken out old secondary weapon stuff
 * 
 * 306   27/08/97 15:02 Collinsd
 * 
 * 305   15-08-97 4:13p Philipy
 * added description for weapons ( SecondaryDescription[] )
 * 
 * 304   15/08/97 16:13 Collinsd
 * Started bgobjects moving ships.  External Forces bug now fixed.
 * 
 * 303   13/08/97 15:09 Collinsd
 * Changed framelag trails.
 * 
 * 302   12/08/97 15:52 Collinsd
 * Changed a few more things to do with visible groups.
 * 
 * 301   11/08/97 17:21 Collinsd
 * Added visible group checks.
 * 
 * 300   9/08/97 14:56 Collinsd
 * Added ScreenPoly TPage Link list and flat shaded screen polys for phil
 * 
 * 299   8/08/97 14:18 Collinsd
 * Display of Polys has now been updated to use TPage link list.
 * 
 * 298   7/08/97 21:25 Collinsd
 * Change FmPolys to use tpage link list.
 * 
 * 297   31/07/97 10:59 Collinsd
 * Changed model modify buffer.
 * 
 * 296   24/07/97 16:09 Collinsd
 * Added enemy bullet and changed reflection of bullets
 * 
 * 295   21/07/97 15:40 Collinsd
 * Changed primary/Secondary bullets to work from enemies.
 * 
 * 294   17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 293   8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 292   8/07/97 14:06 Collinsd
 * Added componented objects to enemies.
 * 
 * 291   5/07/97 16:31 Collinsd
 * Put OPT_ON's around opimisations off
 * 
 * 290   6/30/97 10:27a Phillipd
 * enemy ai started....
 * 
 * 289   6/24/97 5:11p Phillipd
 * 
 * 288   6/24/97 11:12a Phillipd
 * 
 * 287   14/06/97 15:21 Collinsd
 * Fixed targeting bug in titan ( direction would be wrong, Not crash bug
 * )
 * 
 * 286   13/06/97 16:04 Collinsd
 * Taken out old bgobjects
 * 
 * 285   12/06/97 12:55 Collinsd
 * Changed how powerpods are calculated.
 * 
 * 284   10/06/97 14:29 Collinsd
 * Added ACTUAL_TRANS
 * 
 * 283   8/06/97 17:07 Collinsd
 * Done more work on BGObjects/Doors.
 * 
 * 282   5/06/97 12:40 Collinsd
 * Reduced texture pages.
 * 
 * 281   4/06/97 11:12 Collinsd
 * Added message sending for doors.
 * 
 * 280   3/06/97 11:42 Collinsd
 * Done more work on BGObjects
 * 
 * 279   27/05/97 17:40 Collinsd
 * Animating backgrounds now work (Ish)
 * 
 * 278   5/23/97 9:18a Phillipd
 * 
 * 277   7/05/97 11:28 Collinsd
 * Fixed scatter trail on screensaving.
 * 
 * 276   1/05/97 15:21 Collinsd
 * Fixed mine sending bug.
 * 
 * 275   30/04/97 13:09 Collinsd
 * Fixed bug in name/energy display.
 * 
 * 274   30/04/97 9:47 Collinsd
 * Fixed screenpolys crash bug.
 * 
 * 273   29/04/97 17:12 Collinsd
 * Fixed bug in screenpolys.
 * 
 * 272   29/04/97 15:05 Collinsd
 * Fixing bug in screenpolys.
 * 
 * 271   26/04/97 20:29 Collinsd
 * Added more checks and fixed one small bug in screenpolys.c
 * 
 * 270   26/04/97 14:49 Collinsd
 * Optimisations now on def.
 * 
 * 269   24/04/97 9:15 Collinsd
 * fixed unused code.
 * 
 * 268   22-04-97 3:25p Collinsd
 * Added player flying out of bilke.
 * 
 * 267   9-04-97 8:51a Collinsd
 * Added invulnerability
 * 
 * 266   7-04-97 4:49p Collinsd
 * fixed titan star
 * 
 * 265   7-04-97 3:34p Collinsd
 * Added supernashram powerup.
 * 
 * 264   4-04-97 4:18p Collinsd
 * Titan Star now spreads toward reflected angle.
 * 
 * 263   3-04-97 4:33p Collinsd
 * Power levels now 2, titanstar better!
 * 
 * 262   2-04-97 11:54a Collinsd
 * Added Jo bike
 * 
 * 261   19-03-97 11:30a Collinsd
 * 
 * 260   16-03-97 8:07p Collinsd
 * Changed missiles
 * 
 * 259   16-03-97 2:53p Collinsd
 * modified thief missile. ( but nobody likes it, boo hooo ).
 * 
 * 258   13-03-97 5:45p Collinsd
 * Added auto detail level to trails.
 * 
 * 257   13-03-97 11:47a Collinsd
 * polys/fmpolys only generated when needed.
 * 
 * 256   12-03-97 4:47p Collinsd
 * Added recoil and new shrapnel model.
 * 
 * 255   11-03-97 5:41p Collinsd
 * 
 * 254   11-03-97 4:57p Collinsd
 * Changed titan star missile.
 * 
 * 253   10-03-97 7:49p Collinsd
 * Cheat mode no longer buggers up pickups.
 * 
 * 252   9-03-97 9:55p Collinsd
 * Changed primary weapons, Added screen polys indicating how many orbit
 * pulsars,
 * 
 * 251   5-03-97 10:49a Collinsd
 * 
 * 250   27-02-97 2:43p Collinsd
 * PointToSphere better.
 * 
 * 249   27-02-97 2:08p Collinsd
 * Added optimisation to various files.
 * 
 * 248   25-02-97 10:21a Collinsd
 * Corrected framelag on various bits.
 * 
 * 247   24-02-97 10:23a Collinsd
 * Added component bgobjects.
 * 
 * 246   13-02-97 9:25a Collinsd
 * Added destructable enemies/triggers.
 * 
 * 245   11-02-97 5:09p Collinsd
 * Triggers/RegenPoints and pickups now are sent across correctly.
 * 
 * 244   4-02-97 4:29p Collinsd
 * 
 * 243   31-01-97 4:08p Collinsd
 * Body Parts/Blood now use gravity vectors.
 * 
 * 242   31-01-97 10:20a Collinsd
 * Light Details moved into detail menu.
 * 
 * 241   31-01-97 9:24a Collinsd
 * Added detail level for missile trails.
 * 
 * 240   30-01-97 3:58p Collinsd
 * 
 * 239   21-01-97 5:34p Collinsd
 * Added the ability to delete bgobjects.
 * 
 * 238   15-01-97 11:15a Collinsd
 * Crushing Doors now work properly.
 * 
 * 237   14-01-97 12:38p Collinsd
 * BGObject Type are now saved as well
 * 
 * 236   13-01-97 5:03p Collinsd
 * Added Temp Door SFX
 * 
 * 235   13-01-97 12:19p Collinsd
 * Doors now cannot be opened by various rays.
 * 
 * 234   10-01-97 3:54p Collinsd
 * Missile/Primary weapons no longer open doors before collision.
 * 
 * 233   10-01-97 12:34p Collinsd
 * Added Doors, Chanded pickups/door/mine dropping.
 * 
 * 232   6-01-97 12:36p Collinsd
 * Added debug message for missiles getting out of map.
 * 
 * 231   6-01-97 12:06p Collinsd
 * fixed titan star shrapnel getting out of map bug.
 * 
 * 230   6-01-97 11:09a Collinsd
 * All references to timer now replaced with titanstar.
 * 
 * 229   6-01-97 9:06a Collinsd
 * Added drop ammo/shield options.
 * Started working on titan start missile.
 * 
 * 228   3-01-97 3:28p Collinsd
 * Added xxx quantum/pine/purged mined xxx messages.
 * Trojax charging sfx on others pc's now fixed.
 * 
 * 227   2-01-97 4:22p Collinsd
 * Stealthed orbit pulsars now works.  Orbit pulsar and stealth mantle
 * regeneration bug fixed.
 * 
 * 226   31-12-96 12:35p Collinsd
 * Added multiple multiples.
 * 
 * 225   30-12-96 3:50p Collinsd
 * Added orbit pulsar.
 * 
 * 224   12/27/96 3:38p Phillipd
 * Primary.h Secondary.h pickups.h are now clean....
 * Still Lots to do though.....
 * 
 * 223   12/27/96 12:34p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 222   22-12-96 9:39p Collinsd
 * Heatseaker box no longer appears when you have a strait missile.
 * 
 * 221   22-12-96 8:53p Collinsd
 * Added invisible shockwave to all missiles. hitting walls.
 * 
 * 220   22-12-96 7:27p Collinsd
 * Increased model count.  Decreased scatter viewcone.  Lensflare now
 * works properly on scatter missiles.  Names are printed after the
 * endscene.
 * 
 * 219   21-12-96 7:44p Collinsd
 * Added code to enable lensflare on missiles. ( used scatter as test ).
 * Also fixed bug in shockwaves.
 * 
 * 218   21-12-96 5:24p Collinsd
 * Added drop primary/drop secondary weapons.
 * 
 * 217   20-12-96 5:06p Collinsd
 * Added debug weapon feature.
 * 
 * 216   19-12-96 3:19p Collinsd
 * Changed sfx funtion to allow frequency changing.
 * Added Trojax Charging SFX.
 * 
 * 215   19-12-96 11:45a Collinsd
 * Added better ship collision for Trojax and Transpulse.
 * 
 * 214   18-12-96 11:55a Collinsd
 * Fixed colours of names to match Dan's
 * 
 * 213   18-12-96 11:22a Collinsd
 * Fixed some problems with target computer.
 * 
 * 212   18-12-96 9:22a Collinsd
 * Fixed huds
 * 
 * 211   12/17/96 5:28p Collinsd
 * Added team stuff.
 * 
 * 210   12/17/96 12:56p Collinsd
 * Added team missile stuff. and fixed translucency on 3DFX.
 * 
 * 209   12/16/96 6:00p Collinsd
 * Started work on enhanced crosshair.
 * 
 * 208   12/16/96 2:26p Collinsd
 * Added look ahead box
 * 
 * 207   12/14/96 10:44p Collinsd
 * When someone quits the game all pickups scattered. and dissapear under
 * host control.
 * 
 * 206   12/14/96 8:47p Collinsd
 * Target dissapears if no ammo for missile selected.
 * 
 * 205   12/14/96 6:58p Collinsd
 * Added new flash graphics and rearranged texture pages.
 * 
 * 204   12/14/96 5:31p Collinsd
 * Tidied up screen polys and added tempory clipping.
 * 
 * 203   12/13/96 7:17p Collinsd
 * Added target for targeting computer. ( Disabled at the moment though ).
 * 
 * 202   12/12/96 4:17p Collinsd
 * Changed the way screen polys works.
 * 
 * 201   12/11/96 5:17p Collinsd
 * Changed scatterd weapons to spread out more.
 * 
 * 200   12/10/96 11:52a Collinsd
 * Added point in sphere to hitship and hitmine routines. Also gave
 * scatter 360 degree field of view.
 * 
 * 199   12/10/96 11:18a Collinsd
 * Added incoming message when no sound.  Also added code to compensate
 * for pickups using same regeneration slots.
 * 
 * 198   12/09/96 5:25p Collinsd
 * Started smoke when hull damaged ( still need positions )
 * 
 * 197   12/07/96 9:43p Collinsd
 * Pickups sending on joining a game has been speeded up.
 * 
 * 196   12/07/96 8:42p Collinsd
 * Added Jap Bird bike, Fixed mines being dropped and firing missile at
 * same time bug.  Added rotations when hit ( depending on damage ).
 * 
 * 195   12/06/96 4:03p Collinsd
 * Nothing
 * 
 * 194   12/04/96 2:22p Collinsd
 * Pine Mine missile now works.
 * 
 * 193   12/04/96 11:39a Collinsd
 * Ooopppps, fixed launch sfx.
 * 
 * 192   12/04/96 11:21a Collinsd
 * Added foetoid and new scaled bikes. Added sussgun richochet and sussgun
 * no ammo sfx, new load weapon sfx, and new transpulse/trojax sfx.
 * 
 * 191   12/03/96 10:38a Collinsd
 * Fixed dropped mine orientation
 * 
 * 190   12/02/96 1:25p Collinsd
 * No longer use quatfromvector routines.  now use
 * quatfrom2vectors.
 * 
 * 189   11/29/96 3:01p Collinsd
 * Ships hit now get buffeted about a lot more.  Missiles fired, now start
 * in the correct orientation.
 * 
 * 188   11/28/96 4:00p Collinsd
 * Started hull damage effect
 * 
 * 187   11/28/96 11:24a Collinsd
 * Shield effect only happens when shield there.
 * 
 * 186   11/27/96 10:21a Collinsd
 * Changed drop mine code.
 * 
 * 185   25/11/96 12:16 Collinsd
 * Missiles now collide less when moving straight.
 * 
 * 184   11/25/96 11:59a Phillipd
 * 
 * 183   22/11/96 14:18 Collinsd
 * Oooopppssss.
 * 
 * 182   22/11/96 14:01 Collinsd
 * Changed mine check range.
 * 
 * 181   22/11/96 12:44 Collinsd
 * Added function to check proximity of mines when regenerating.
 * 
 * 180   11/22/96 12:27p Phillipd
 * more stuff is checked to make sure someone joining cant be hurt..
 * 
 * 179   22/11/96 11:18 Collinsd
 * Added portal only collision for primary/secondary fire offsets.
 * 
 * 178   20/11/96 12:09 Collinsd
 * Added powerpod management.
 * 
 * 177   19/11/96 16:18 Collinsd
 * Blood moves in correct direction now.
 * 
 * 176   19/11/96 11:06 Collinsd
 * Added new blood graphics.
 * 
 * 175   18/11/96 15:10 Collinsd
 * Added new borg girl models. Changed dissapper time for scattered
 * weapons.
 * 
 * 174   18/11/96 11:39 Collinsd
 * Added scattered weapons dissapearing and regenerating elsewhere after
 * 5-10 seconds.
 * 
 * 173   16/11/96 20:30 Collinsd
 * Added pickups per group variables/display.  Also fixed gravgon hitting
 * mine position problem.
 * 
 * 172   15/11/96 16:49 Collinsd
 * Mines now sent across when joining. Also fixed bug in pickup sending.
 * 
 * 171   14/11/96 16:40 Collinsd
 * Added incoming sound effect and scattered message if no sound card
 * present.
 * 
 * 170   14/11/96 15:42 Collinsd
 * added targeted sound fx.
 * 
 * 169   14/11/96 12:22 Collinsd
 * Added new sfx ( Golden PowerPod, Scattered, Missile Launch )
 * 
 * 168   14/11/96 10:12 Collinsd
 * Added golden powerpod and best mine function.
 * 
 * 167   13/11/96 15:46 Collinsd
 * Added golden powerpod and ljay.
 * 
 * 166   7/11/96 15:17 Collinsd
 * fixed problem with mines.
 * 
 * 165   7/11/96 13:59 Collinsd
 * Possible fix to mine problem. ( I Hope ).
 * 
 * 164   7/11/96 13:01 Collinsd
 * fixed pos of mine explosion.
 * 
 * 163   7/11/96 11:11 Collinsd
 * Got rid of quat in ship structures.
 * 
 * 162   7/11/96 9:03 Collinsd
 * Changed over to using new network firing.
 * 
 * 161   6/11/96 18:42 Collinsd
 * Tidied up secondary weapons.
 * 
 * 160   6/11/96 14:40 Collinsd
 * purge mine now bobs.
 * 
 * 159   5/11/96 17:16 Collinsd
 * hopefully fixed stealth from generating too many times.
 * 
 * 158   4/11/96 16:35 Collinsd
 * Fixed fmpolys/polys/lines exec lists.
 * 
 * 157   1/11/96 17:52 Collinsd
 * Reduced ship/shortship structure size by using bits.
 * 
 * 156   31/10/96 20:57 Collinsd
 * fixed gravgon/shockwave.  Stealth mode no longer displays name.
 * 
 * 155   31/10/96 16:30 Collinsd
 * Primary/Secondary now included ship updates.  Stealth regeneration
 * fixed
 * 
 * 154   30/10/96 14:34 Collinsd
 * Added stealthmode.
 * 
 * 153   29/10/96 16:00 Collinsd
 * changed over to panned sfx.
 * 
 * 152   29/10/96 14:49 Collinsd
 * Added big explosion flash.
 * 
 * 151   29/10/96 12:01 Collinsd
 * Added white pulsing start to new trail. ( Looks better )
 * 
 * 150   29/10/96 11:59 Collinsd
 * Added new trail for scatter.
 * 
 * 149   27/10/96 18:38 Collinsd
 * Optimised loads of shit.
 * 
 * 148   27/10/96 16:25 Collinsd
 * Fixed pine mines
 * 
 * 147   27/10/96 12:30 Collinsd
 * Optimised light handling for pickups/primary/secondary.
 * 
 * 146   26/10/96 21:15 Collinsd
 * Various optimisations
 * 
 * 145   26/10/96 19:21 Collinsd
 * Added 'O' on ship for when homing missiles can see target.
 * 
 * 144   26/10/96 18:33 Collinsd
 * Centered and moved up name tags.
 * 
 * 143   26/10/96 18:04 Collinsd
 * Added recoil vector to ihityou.
 * 
 * 142   25/10/96 16:40 Collinsd
 * Added audio cd playing code.
 * 
 * 141   24/10/96 15:16 Collinsd
 * No message if sfx, mine now collide immediatly if not owner
 * 
 * 140   24/10/96 11:47 Collinsd
 * Added total verts/poly for POLY's and added player
 * name display.
 * 
 * 139   22/10/96 12:08 Collinsd
 * Added body parts and blood splats.
 * 
 * 138   19/10/96 20:50 Collinsd
 * Changed scatter/thief to give more chance for enemy to pickup up
 * scattered weapons.
 * Reduced thief homing.  Increased Heatseaker.
 * Lights now can be enabled and disabled properly.
 * started on flying body parts.  Plus lots more..... 
 * 
 * 137   17/10/96 17:33 Collinsd
 * Changed thief missile inteligence
 * 
 * 136   17/10/96 16:43 Collinsd
 * Taken out redundent models.
 * 
 * 135   15/10/96 18:00 Collinsd
 * Changed collision back.
 * 
 * 134   15/10/96 17:39 Collinsd
 * SCREEN SHOTS ONLY
 * 
 * 133   15/10/96 15:30 Collinsd
 * Added missile/mine view toggled on/off with delete
 * 
 * 132   15/10/96 12:31 Collinsd
 * Added directional light to quantum fireball.  Changed ammo, so none are
 * generated if there are no weapons that use them.
 * 
 * 131   14/10/96 19:10 Collinsd
 * Fixed MFRL & Heatseaker pickup problem.
 * 
 * 130   14/10/96 14:22 Collinsd
 * Added even more debug info in pickups.
 * Also speeded up target aquisition of homing missiles
 * 
 * 129   9/10/96 16:12 Collinsd
 * Added regeneration sparkle. And model display flag.
 * 
 * 128   9/10/96 12:28 Collinsd
 * Added code to handle low poly, single direction sphere.
 * 
 * 127   8/10/96 10:40 Collinsd
 * Added generic pickup sfx, and changed smoke trail.
 * 
 * 126   7/10/96 13:52 Collinsd
 * Added new smoke trails
 * 
 * 125   6/10/96 16:29 Collinsd
 * Nitro regenerates when you die.
 * 
 * 124   5/10/96 21:34 Collinsd
 * Added drop pickup mode. speeded up pickups even more now 0.9% of
 * processing
 * 
 * 123   5/10/96 20:42 Collinsd
 * Added ammo usage to primary weapons.  Cleaned up
 * primary/secondary weapon code.  Added primary lights detail variable.
 * 
 * 122   5/10/96 14:02 Collinsd
 * Added sfx to selection of primary and secondary weapons
 * 
 * 121   5/10/96 13:22 Collinsd
 * Fixed bug that caused pickups to do collision processing
 * even when stationary for the first few seconds after initialisation.
 * 
 * 120   4/10/96 16:05 Collinsd
 * Pickups now work properly when stationary.
 * 
 * 119   4/10/96 10:47 Collinsd
 * Added visible group pickup as well as using proper ray for movement of
 * ship picking up.
 * 
 * 118   3/10/96 15:49 Collinsd
 * Added new sfx
 * 
 * 117   1/10/96 17:44 Collinsd
 * Reduced pulsar to half.  Tidied up primary weapons.
 * deleted redundant code in 2dpolys/primary weapons.
 * 
 * 116   30/09/96 14:48 Collinsd
 * Added simplified missiles.
 * Fixed distance 2dpoly's moved out by...
 * 
 * 115   29/09/96 18:47 Collinsd
 * Added turbo to ship structure. Fixed pine mine firing pos.
 * 
 * 114   25/09/96 11:11 Collinsd
 * 
 * 113   23/09/96 14:20 Collinsd
 * Started spider mine.
 * 
 * 112   23/09/96 10:34 Collinsd
 * Pine mine fire pine missile ( Limited to 10 ).  Multiple rocket
 * launcher regeneration bug fixed
 * 
 * 111   23/09/96 8:50 Collinsd
 * Errrm, don't know, but it must have been important.
 * 
 * 110   20/09/96 20:11 Collinsd
 * Bug fixed in mine pod regeneration, and killing.
 * 
 * 109   20/09/96 18:15 Collinsd
 * Pine mines no longer fire at dieing ships.  Mines don't
 * regenerate unless destroyed.
 * 
 * 108   20/09/96 17:13 Collinsd
 * Changed 2Dpolys and Polys to display only polys in visible groups.
 * Changed secondary ammo to use array.
 * Pickups now use correct index's.  You can now drop a mine using 'B'.
 * 
 * 107   18/09/96 17:37 Collinsd
 * 
 * 106   18/09/96 14:36 Collinsd
 * Fixed primary weapon select. and rotate pickups in all axis
 * 
 * 105   18/09/96 9:02 Collinsd
 * Altered pickups.  You no longer pickup pickups in other groups.
 * 
 * 104   17/09/96 17:02 Collinsd
 * You now cannot pickup anything in death mode.
 * 
 * 103   17/09/96 16:14 Collinsd
 * Next/Prev Secondary now works better.
 * 
 * 102   17/09/96 15:34 Collinsd
 * Shockwaves added. and fixed primary previous.
 * 
 * 101   17/09/96 13:58 Collinsd
 * Added autoselection of primary and secondary weapons.
 * Also you can pickup anything if F12 enabled.
 * 
 * 100   16/09/96 17:04 Collinsd
 * 
 * 99    16/09/96 11:24 Collinsd
 * Added joining regeneration of pickups code.
 * 
 * 98    16/09/96 9:52 Collinsd
 * Fixed bug in regeneration of secondary weapon pickups.
 * 
 * 97    16/09/96 9:25 Collinsd
 * Added regenerating pickups.
 * 
 * 96    15/09/96 15:43 Collinsd
 * 
 * 95    15/09/96 14:48 Collinsd
 * Scaled primary weapons, changed defauls ammo levels.
 * 
 * 94    14/09/96 16:25 Collinsd
 * Added shockwave to ship death.
 * 
 * 93    14/09/96 15:40 Collinsd
 * Added shield to mines, and damage to missiles.
 * Also when you die your weapons and ammo are dropped
 * 
 * 92    13/09/96 17:04 Collinsd
 * Added quantun fireball mine.
 * 
 * 91    13/09/96 14:26 Collinsd
 * Secondary weapons can now hit mines.
 * 
 * 90    13/09/96 12:59 Collinsd
 * Added shield effect to external views.
 * 
 * 89    12/09/96 17:52 Collinsd
 * You can now shoot mines ( Primary weapons only though
 * at the moment ).
 * 
 * 88    11/09/96 19:00 Collinsd
 * Added tests for RAMP emulation ( Although may not work)
 * Also fixed gravity effect to be dependent on GLOBAL_SCALE
 * 
 * 87    11/09/96 14:19 Collinsd
 * Added global scale to hopefully everything that needs it.
 * 
 * 86    10/09/96 16:07 Collinsd
 * Added gravgon effect. (Not finished)
 * 
 * 85    10/09/96 12:54 Collinsd
 * Pickups now save and load groups
 * 
 * 84    10/09/96 9:00 Collinsd
 * Timer missile no longer explodes on contact with walls. It waits for
 * release.
 * 
 * 83    9/09/96 20:42 Collinsd
 * No longer used GroupPolyCol or WhichGroupImIn.
 * 
 * 82    9/09/96 11:23 Collinsd
 * 
 * 81    7/09/96 20:21 Collinsd
 * Collision with pine mine now added.  Trojax power
 * fixed, now does proper damage.  Transpulse speeded
 * up and made more lethal.
 * 
 * 80    7/09/96 17:08 Collinsd
 * Added purge mine and done more on pine mine.
 * 
 * 79    6/09/96 16:20 Collinsd
 * Added extra life pickup, and started pine mine.
 * 
 * 78    6/09/96 9:13 Collinsd
 * Timer missile works now.
 * 
 * 77    5/09/96 15:08 Collinsd
 * Added timer missile.
 * 
 * 76    5/09/96 10:45 Collinsd
 * Fixed next prim/sec weapons selection. 
 * 
 * 75    4/09/96 20:01 Collinsd
 * Scatter missile and thief missile now work!
 * 
 * 74    2/09/96 14:39 Collinsd
 * Fixed bug with missile ammo.
 * 
 * 73    2/09/96 14:18 Collinsd
 * Finished shield effect, started on secondary weapons.
 * 
 * 72    30/08/96 17:31 Collinsd
 * Fixed bug in rgba colours ( Cheers Dan ).
 * 
 * 71    29/08/96 17:53 Collinsd
 * Lots more pickups and shield working a little better.
 * 
 * 70    28/08/96 15:03 Collinsd
 * Added 5 more pickups and fixed bug model group update.
 * 
 * 69    23/08/96 9:18 Collinsd
 * All weapons have limits. New pickups in but not correct.
 * 
 * 68    21/08/96 8:46 Collinsd
 * Added descent level, and powerlevel bar for trojax.
 * 
 * 67    16/08/96 17:29 Collinsd
 * Damage variables now in..
 * 
 * 66    16/08/96 15:18 Collinsd
 * Pickups now have bubble
 * 
 * 65    15/08/96 17:50 Collinsd
 * 
 * 64    15/08/96 17:28 Collinsd
 * Laser now hits ships.  Bug with primary weapons hitting background
 * before ship fixed.
 * 
 * 63    13/08/96 12:49 Collinsd
 * Working on primary weapons
 * 
 * 62    9/08/96 15:10 Collinsd
 * Missiles don't go on forever if outside group.
 * 
 * 61    9/08/96 10:18 Collinsd
 * Added powerlevel to primary weapons.
 * Added a few more pickup models.
 * 
 * 60    8/08/96 9:13 Collinsd
 * Added Sfx and pickups
 * 
 * 59    7/08/96 10:58 Collinsd
 * 
 * 58    7/08/96 10:32 Collinsd
 * Added pickup collision with background
 * 
 * 57    2/08/96 16:13 Collinsd
 * Fixed primary weapon net loop bug.  Oooops...
 * 
 * 56    8/02/96 2:34p Phillipd
 * 
 * 55    2/08/96 9:28 Collinsd
 * Added double sided 2D polys, Static 2D polys.
 * Added transpulse cannon and pyrolite weapon.
 * Weapons should work correctly over network.
 * 
 * 54    31/07/96 16:07 Collinsd
 * Added new pyrolite weapon, added type into bullets/missiles. Fixed bug
 * with 2d polys
 * 
 * 53    29/07/96 16:43 Collinsd
 * Fixed bug in missile hit & changed transpulse.
 * 
 * 52    25/07/96 10:08 Collinsd
 * Added variable speed to trojax weapon.
 * 
 * 51    7/25/96 10:05a Phillipd
 * 
 * 50    7/24/96 2:42p Phillipd
 * 
 * 49    24/07/96 12:59 Collinsd
 * Started on primary weapons.
 * 
 * 48    23/07/96 18:01 Collinsd
 * Added visipoly line mode and group in mode.
 * 
 * 47    23/07/96 15:26 Collinsd
 * Added new offset file loading/format.
 * 
 * 46    22/07/96 17:46 Collinsd
 * 
 * 45    21/07/96 20:42 Collinsd
 * Added plasma sparks. ( Crap at the moment ! ).
 * 
 * 44    21/07/96 14:26 Collinsd
 * Added twin rocket launch.
 * 
 * 43    21/07/96 14:21 Collinsd
 * Added drop vector to dplay and secondary weapons.
 * 
 * 42    21/07/96 12:28 Collinsd
 * Primary Weapon Selection added.  Missiles now have multiple fireing
 * methods/directions.  Now uses tload.
 * 
 * 41    19/07/96 16:56 Collinsd
 * Now have seperate explosion for Ship and Background.
 * Increased Max 2DPolys, and have framelag compensated fireing of the
 * primary and secondary weapons.
 * 
 * 40    19/07/96 12:36 Collinsd
 * New Explosions and better smoke trails.
 * 
 * 39    17/07/96 19:15 Collinsd
 * Fixed bug in missiles/ improved 2D sprites handling
 * 
 * 38    17/07/96 15:28 Collinsd
 * Missiles/2DPolys altered to work better
 * 
 * 37    16/07/96 17:19 Collinsd
 * Frame lag on homing missiles turn added.
 * 
 * 36    16/07/96 16:34 Collinsd
 * 
 * 35    16/07/96 16:06 Collinsd
 * Missiles redone, and bug fixed with collision.
 * 
 * 34    7/16/96 11:11a Phillipd
 * moved all visipoly stuff to visi.c and visi.h..
 * 
 * 33    15/07/96 14:30 Collinsd
 * Transparency & Translucency work (With hardware)
 * 
 * 32    11/07/96 17:33 Collinsd
 * Target lost when behine wall fixed.
 * 
 * 31    11/07/96 12:50 Collinsd
 * Drop missiles started
 * 
 * 30    10/07/96 17:10 Collinsd
 * Smoke trails added to missiles, explosions improved.
 * 
 * 29    8/07/96 10:54 Collinsd
 * Added random rotation to explosion.
 * 
 * 28    6/07/96 22:25 Collinsd
 * Added pulse to 2dPoly's.
 * 
 * 27    6/07/96 20:49 Collinsd
 * FmPolys now caluate position of explosion constantly.
 * 
 * 26    6/07/96 16:36 Collinsd
 * Explosion moved away from wall, toward camera.
 * 
 * 25    5/07/96 17:59 Collinsd
 * Faceme poly's now use boxtoppm data
 * 
 * 24    4/07/96 16:54 Collinsd
 * 
 * 23    4/07/96 16:09 Collinsd
 * 
 * 22    4/07/96 10:34 Collinsd
 * 
 * 21    7/04/96 9:49a Phillipd
 * 
 * 20    2/07/96 16:37 Collinsd
 * Collision to ships now using ray to sphere
 * 
 * 19    2/07/96 16:26 Collinsd
 * Straight/Homing Missile to ship collision (Sphere to Point )
 * 
 * 18    2/07/96 15:20 Collinsd
 * 
 * 17    28/06/96 17:35 Collinsd
 * 
 * 16    28/06/96 12:47 Collinsd
 * Now uses missile mesh and turn speed reduced!
 * 
 * 15    28/06/96 10:05 Collinsd
 * 
 * 14    27/06/96 12:59 Collinsd
 * 
 * 13    27/06/96 12:40 Collinsd
 * 
 * 12    27/06/96 12:22 Collinsd
 * 
 * 11    27/06/96 11:53 Collinsd
 * 
 * 10    27/06/96 11:24 Collinsd
 * 
 * 9     26/06/96 16:31 Collinsd
 * 
 * 8     26/06/96 16:30 Collinsd
 * 
 * 7     26/06/96 15:55 Collinsd
 * Added homing missiles ( As 2D Sprites )
 * 
 * 6     26/06/96 12:14 Collinsd
 * 
 * 5     25/06/96 17:32 Collinsd
 * 
 * 4     25/06/96 16:27 Collinsd
 * Secondary bullet colour/keyboard fireing
 * 
 * 3     25/06/96 16:10 Collinsd
 * 
 * 2     25/06/96 15:15 Collinsd
 * 
 * 1     25/06/96 15:07 Collinsd
 * 
 */

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
*	s e c o n d . c
*	All routines to do with Secondary weapons...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <stdio.h>
#include "main.h"
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"

#include "screenpolys.h"
#include "polys.h"

#include "d3dmain.h"
#include "Ships.h"
#include "exechand.h"
#include "Mload.h"
#include "collision.h"
#include "lights.h"
#include "2dpolys.h"
#include "models.h"
#include "sphere.h"
#include "controls.h"
#include "visi.h"
#include "sfx.h"
#include "triggers.h"
#include "pickups.h"
#include "enemies.h"

#include "secondary.h"
#include "primary.h"

#include "title.h"
#include "text.h"
#include "spotfx.h"
#include "AI.h"
#include "water.h"

#include "local.h"

#define	SCATTER_TEST	0

#define	MYEXPLO			0
#define	MISSILE_SPEED	( MAXMOVESPEED * 1.6F )			// Ship Speed * 1.6

#if !defined( VERSION_SPANISH ) && !defined( VERSION_ITALIAN ) && !defined( VERSION_GERMAN ) && !defined( VERSION_FRENCH )
#include "localtitle.h"
#elif defined( VERSION_SPANISH )
#include "sp-localtitle.h"
#elif defined( VERSION_ITALIAN )
#include "it-localtitle.h"
#elif defined( VERSION_GERMAN )
#include "ge-localtitle.h"
#elif defined( VERSION_FRENCH )
#include "fr-localtitle.h"
#endif

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Externs
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern	BYTE	Server_WhoHitYou;
extern SLIDER BikeCompSpeechSlider;
extern BOOL CaptureTheFlag;
extern BOOL CTF;
extern BOOL BountyHunt;
extern int8 TeamFlagPickup[ MAX_TEAMS ];
extern uint32 TeamFlagMask[ MAX_TEAMS ];
extern char *TeamName[ MAX_TEAMS ];

extern int16			PickupsGot[ MAXPICKUPTYPES ];
extern	FRAME_INFO	*	Flare_Header;
extern	FRAME_INFO	*	Trojax_Exp_Header;
extern	FRAME_INFO	*	Pulsar_Exp_Header;
extern	FRAME_INFO	*	Bits_Header;
extern	SHIPCONTROL		control;
extern	D3DMATRIX		view;
extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
extern	D3DMATERIALHANDLE hMat2;
extern	D3DTEXTUREHANDLE bTex;
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	XLIGHT			XLights[MAXXLIGHTS];
extern	FMPOLY			FmPolys[MAXNUMOF2DPOLYS];
extern	POLY			Polys[MAXPOLYS];
extern	MODEL			Models[MAXNUMOFMODELS];
extern	float			framelag;
extern	GLOBALSHIP      Ships[MAX_PLAYERS+1];
extern	BYTE			WhoIAm;
extern	BYTE			Current_Camera_View;		// which object is currently using the camera view....
extern	BOOL            bSoundEnabled;

extern	FRAME_INFO	*	Exp_Header;
extern	FRAME_INFO	*	Exp2_Header;
extern	FRAME_INFO	*	GravgonTrail_Header;
extern	FRAME_INFO	*	SolarisTrail_Header;
extern	FRAME_INFO	*	NewTrail_Header;
extern	FRAME_INFO	*	Blood_Header;
extern	FRAME_INFO	*	BloodSplat_Header;
extern	FRAME_INFO	*	BloodDribble_Header;
extern	FRAME_INFO	*	Laser_Header;
extern	FRAME_INFO	*	ScatterTrail_Header;
extern	FRAME_INFO	*	GreyFlare_Header;
extern	FRAME_INFO	*	BigFlash_Header;
extern	FRAME_INFO	*	Target_Header;
extern	FRAME_INFO	*	Hud_Header;
extern	FRAME_INFO	*	LensFlare_Header;
extern	FRAME_INFO	*	BlueSpark_Header;
extern	FRAME_INFO	*	Circle_Header;
extern	FRAME_INFO	*	Tentacle_Header;
extern	MATRIX			MATRIX_Identity;
extern	int16			PrimaryWeaponsGot[ MAXPRIMARYWEAPONS ];
extern	USERCONFIG	*	player_config;
extern	BOOL            IsHost;
extern	float			NitroFuel;
extern	float			NitroFuelUsed;
extern	int16			NumStealths;
extern	int16			NumOrbs;

extern	BOOL			SecondaryLightDetail;
extern	uint16			CameraMissile;
extern	int				MissileCameraActive;
extern	MATRIX			ProjMatrix;
extern	SHORTNAMETYPE	Names;
extern	CAMERA			CurrentCamera;

extern	MISSEDMINEMSG	MissedMines[ MAXMISSEDMINES ];
extern	SHORTMINE		MissedInitMines[ MAXMISSEDMINES ];
extern	SHORTKILLMINE	MissedKillMines[ MAXMISSEDMINES ];
extern	int16			NumMissedMines;
extern	int16			NumMissedInitMines;
extern	int16			NumMissedKillMines;

extern	int16			NumPowerPods;

extern	BYTE			GameStatus[MAX_PLAYERS];	// Game Status for every Ship...
													// this tells the drones what status the host thinks hes in..
extern	float			ticksperframe;

extern	SCRPOLY			ScrPolys[ MAXNUMOFSCRPOLYS ];

extern	PRIMARYWEAPONATTRIB PrimaryWeaponAttribs[ TOTALPRIMARYWEAPONS ];
extern	BOOL			EnhancedXHair;

extern	BOOL			TeamGame;
extern	BYTE			TeamNumber[MAX_PLAYERS];
extern	BOOL			DebugInfo;
extern	BOOL			GodMode;
extern	SLIDER			TrailDetailSlider;

extern	uint16			IsGroupVisible[MAXGROUPS];
extern	BOOL			AutoDetail;
extern	float			avgframelag;
extern	BOOL			PickupInvulnerability;
extern	int16			NumInvuls;
extern	float			SoundInfo[MAXGROUPS][MAXGROUPS];
extern	ENEMY	*		FirstEnemyUsed;
extern	ENEMY			Enemies[ MAXENEMIES ];
extern	ENEMY_TYPES		EnemyTypes[ MAX_ENEMY_TYPES ];
extern	int				no_collision;		// disables player ship-to-background collisions
extern	int				outside_map;
extern	float			NmeDamageModifier;
extern	int32			ColPerspective;

extern	BOOL			CaptureTheFlag;
extern	BOOL			BountyHunt;

extern	float			GlobalFramelagAddition;
extern	BOOL			PlayDemo;

extern	BOOL	IsServerGame;
extern	BOOL	IsServer;
extern	int16	Host_PrimaryWeaponsGot[ MAX_PLAYERS ][ MAXPRIMARYWEAPONS ];
extern	int16	Host_SecondaryWeaponsGot[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
extern	float	Host_GeneralAmmo[ MAX_PLAYERS ];
extern	float	Host_PyroliteAmmo[ MAX_PLAYERS ];
extern	float	Host_SussGunAmmo[ MAX_PLAYERS ];
extern	float	Host_GeneralAmmoUsed;
extern	float	Host_PyroliteAmmoUsed;
extern	float	Host_SussGunAmmoUsed;
extern	int16	Host_SecondaryAmmo[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
extern	int16	Host_SecAmmoUsed[ MAXSECONDARYWEAPONS ];
extern	int8	Host_SecWeaponsGot[ MAX_PLAYERS ][ MAXPICKUPS ];
extern	float	Host_NitroFuel[ MAX_PLAYERS ];
extern	float	Host_NitroFuelUsed;
extern	int16	Host_PowerLevel[ MAX_PLAYERS ];
extern	BYTE	Host_NumOfOrbitals[ MAX_PLAYERS ];
extern	BYTE	Host_CopyOfNumOfOrbitals[ MAX_PLAYERS ];
extern	float	Host_CopyOfOrbAmmo[ MAX_PLAYERS ][ MAXMULTIPLES ];
extern	int16	Host_CopyOfPowerLevel[ MAX_PLAYERS ];
extern	float	Host_CopyOfNitroFuel[ MAX_PLAYERS ];
extern	float	Host_CopyOfGeneralAmmo[ MAX_PLAYERS ];
extern	float	Host_CopyOfSussGunAmmo[ MAX_PLAYERS ];
extern	float	Host_CopyOfPyroliteAmmo[ MAX_PLAYERS ];
extern	int16	Host_PickupsGot[ MAX_PLAYERS ][ MAXPICKUPTYPES ];
extern  uint32  Host_Flags[ MAX_PLAYERS ];

extern	int16	NumSuperNashrams;

extern	char PrimaryNames[7][16];
extern	PICKUP	Pickups[ MAXPICKUPS ];
extern	float	RegenDelay;

void DebugPrintf( const char * format, ... );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Globals
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/*
char *SecondaryDescription[MAXSECONDARYWEAPONS] = {

	"Mug",
	"Solaris",
	"Thief",
	"Scatter",
	"Gravgon",
	"MFRL",
	"Titan",
	"Purge Mine",
	"Pine Mine",
	"Quantum Mine",
	"Spider Mine"

};
*/
char *SecondaryDescription[MAXSECONDARYWEAPONS] = {
	LT_MENU_NEW_SecondaryWeaponKeys17 /*"mug missile:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys18 /*"solaris missile:"*/,
	"Thief",
	LT_MENU_NEW_SecondaryWeaponKeys5 /*"scatter missile"*/,
	LT_MENU_NEW_SecondaryWeaponKeys6 /*"gravgon missile"*/,
	LT_MENU_NEW_SecondaryWeaponKeys21 /*"mfrl:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys22 /*"Titan Missile:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys23 /*"purge mine:"*/,
	LT_MENU_NEW_SecondaryWeaponKeys24 /*"pine mine:"*/,
	 LT_MENU_NEW_SecondaryWeaponKeys25 /*"quantum mine:"*/,
	 "spider mine",
};

// is weapon valid as far as order processing & keydefs concerned?
BOOL SecondaryValid[ MAXSECONDARYWEAPONS ] =
{

	TRUE,	// mug
	TRUE,	// solaris
	TRUE,	// thief
	TRUE,	// scatter
	TRUE,	// gravgon
	TRUE,	// mfrl
	TRUE,	// titan
	TRUE,	// purge
	TRUE,	// pine
	TRUE,	// quantum
	FALSE,	// spider

};

ENTRY		EntryList[ MAX_ENTRYS ];
ENTRY	*	FirstFree = &EntryList[ 0 ];
ENTRY	*	FirstUsed = NULL;

SECONDARYWEAPONBULLET	SecBulls[MAXSECONDARYWEAPONBULLETS];
SHORTMINE	MinesCopy[ MAX_PLAYERS ][ MAXSECONDARYWEAPONBULLETS ];
uint16		FirstSecBullUsed;
uint16		FirstSecBullFree;
float		SecondaryFireDelay = 0.0F;

int16		SecondaryWeaponsGot[ MAXSECONDARYWEAPONS ];
int8		SecWeaponsGot[ MAXPICKUPS ];
int16		SecAmmoUsed[ MAXSECONDARYWEAPONS ];
int16		SecondaryAmmo[ MAXSECONDARYWEAPONS ];
BOOL		TargetComputerOn = FALSE;

int16		NumSecBullsPerGroup[ MAXGROUPS ];
SECONDARYWEAPONBULLET *	SecBullGroups[ MAXGROUPS ];

BOOL		ImTargeted = FALSE;
float		TargetedDelay = 0.0F;
float		MissileDistance = 0.0F;
uint16		MissileNum;

BOOL		ShowNamesAnyway = FALSE;

LONGLONG	Time_LastValue;

uint16		GlobalSecBullsID = 0;

int8	*	DebugSecStrings[] = {

			"Mug Missile",					// Secondary Weapons
			"Heatseeker Missile",
			"Thief Missile",
			"Scatter Missile",
			"Gravgon Missile",
			"Rocket Launcher",
			"TitanStar Missile",
			"Purge Mine",
			"Pine Mine",
			"Quantum Mine",
			"Spider Mine",
			"Pine Mine Missile",
			"TitanStar Shrapnel",
			"Enemy Spiral",
			"Photon Missile",
			"Blue Photon Missile",
			"FireBall",
};

int16 SecondarySfxLookup [ MAXSECONDARYWEAPONS ] = 
{
	SFX_Select_MugMissile,
	SFX_Select_SolarisMissile,
	SFX_DontHaveThat, // no sfx for thief missile
	SFX_Select_ScatterMissile,
	SFX_Select_GravgonMissile,
	SFX_Select_MFRL,
	SFX_Select_TitanStarMissile,
	SFX_Select_PurgeMine,
	SFX_Select_PineMine,
	SFX_Select_QuantumMine,
	SFX_BIKECOMP_SP,
};

CAMERA		TempCamera;

int8 SecondaryToFireLookup[ MAXSECONDARYWEAPONS ] = {

	MUGMISSILE,
	SOLARISMISSILE,
	THIEFMISSILE,
	SCATTERMISSILE,
	GRAVGONMISSILE,
	MULTIPLEMISSILE,
	TITANSTARMISSILE,
	PURGEMINE,
	PINEMINE,
	QUANTUMMINE,
	SPIDERMINE,
};

BOOL	SecondaryWeaponCheat = FALSE;

#ifdef SINT_PEACEFROG_CHEAT
int16	TargetingWeaponCheat = -1;
#endif

SECONDARYWEAPONATTRIB	SecondaryWeaponAttribs[ TOTALSECONDARYWEAPONS ] = {

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	0 Mug Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	// uint16	State				// Starting state
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fast can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		0,				// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_Mug,		// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		255.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // uint16 Flags				// Initial Flags
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	1 Solaris Heatseeker Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	// uint16	State				// Starting state
		MISMOVE_HOMING,	// uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		1.5F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		120,			// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_Heatseaker, // uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // uint16 Flags				// Initial Flags
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	2 Thief Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	// uint16	State				// Starting state
		MISMOVE_HOMING, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		3.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		360,			// uint16	ViewCone			// View Cone ( Degrees )
//		MODEL_Thief,	// uint16	ModelNum;			// if 3D model which one....
		MODEL_Eyeball,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		255.0F,			// float	g;					// what color does it emit....
		255.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		0.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // uint16 Flags				// Initial Flags
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	3 Scatter Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	//uint16	State				// Starting state
		MISMOVE_HOMING, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
#if SCATTER_TEST
		3.0F,			// float	TurnSpeed			// how fase can I turn
#else
		2.0F,			// float	TurnSpeed			// how fase can I turn
#endif
		30,				// uint16	FireDelay			// how fast can I fire
#if SCATTER_TEST
		360,			// uint16	ViewCone			// View Cone ( Degrees )
#else
		180,			// uint16	ViewCone			// View Cone ( Degrees )
#endif
		MODEL_Scatter,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		TRUE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // uint16 Flags				// Initial Flags
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	4 Gravgon Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	// uint16	State				// Starting state
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F ,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		0,				// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_Gravgon,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		255.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // uint16 Flags				// Initial Flags
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	5 Multiple Missile Launcher
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_DROP,		//uint16	State				// Starting state
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		MISSILE_SPEED,	// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 2.0F ),	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fase can I turn
		1,				// uint16	FireDelay			// how fast can I fire
		0,				// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_PineMissile,	// uint16	ModelNum;			// if 3D model which one....
		0.0F,			// float	lightsize;	// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		192.0F,			// float	g;					// what color does it emit....
		0.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		32.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // uint16 Flags				// Initial Flags
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	6 TitanStar Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	//uint16	State				// Starting state
#ifdef SINT_PEACEFROG_CHEAT
		MISMOVE_HOMING,	// uint16	Move Type			// Straight/Homing
#else
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
#endif
		MISSILE_SPEED * 0.75F,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED * 0.75F,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
#ifdef SINT_PEACEFROG_CHEAT
		2.0F,			// float	TurnSpeed			// how fase can I turn
#else
		0.0F,			// float	TurnSpeed			// how fase can I turn
#endif
		30,				// uint16	FireDelay			// how fast can I fire
#ifdef SINT_PEACEFROG_CHEAT
		360,			// uint16	ViewCone			// View Cone ( Degrees )
#else
		0,				// uint16	ViewCone			// View Cone ( Degrees )
#endif
		MODEL_TitanStar,// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE * 1.5F,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		256.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // uint16 Flags				// Initial Flags
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	7 Purge Mine
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MINE,		// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	//uint16	State				// Starting state
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		0,				// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_Purge,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		1.0F,			// float	Shield				// Shield
		0.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot | SECFLAGS_CanTarget, 		// Initial Flags
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	8 Pine Mine
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MINE,		// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	//uint16	State				// Starting state
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.75F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		360,			// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_Pine,		// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		400.0F,			// float	Shield				// Shield
		0.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot | SECFLAGS_CanTarget, 		// Initial Flags
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	9 Quantum Mine
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MINE,		// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	//uint16	State				// Starting state
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		0,				// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_Quantum,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		32.0F,			// float	r;					// what color does it emit....
		32.0F,			// float	g;					// what color does it emit....
		255.0F,			// float	b;					// what color does it emit....
		40.0F,			// float	Shield				// Shield
		0.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot | SECFLAGS_CanTarget, 		// Initial Flags
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	10 Spider Mine
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MINE,		// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	//uint16	State				// Starting state
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		0,				// uint16	ViewCone			// View Cone ( Degrees )
//		MODEL_Spider,	// uint16	ModelNum;			// if 3D model which one....
		MODEL_Eyeball,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		32.0F,			// float	r;					// what color does it emit....
		32.0F,			// float	g;					// what color does it emit....
		255.0F,			// float	b;					// what color does it emit....
		40.0F,			// float	Shield				// Shield
		0.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot | SECFLAGS_CanTarget, 		// Initial Flags
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	11 Pine Mine Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	// uint16	State				// Starting state
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		MISSILE_SPEED,	// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 2.0F ),	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fast can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		0,				// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_PineMissile,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		255.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		20.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // uint16 Flags				// Initial Flags
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	12 Titan Star Shapnel Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	// uint16	State				// Starting state
#ifdef SINT_PEACEFROG_CHEAT
		MISMOVE_HOMING,	// uint16	Move Type			// Straight/Homing
#else
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
#endif
		( MISSILE_SPEED * 1.25F ),	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 1.25F ),	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
#ifdef SINT_PEACEFROG_CHEAT
		3.5F,			// float	TurnSpeed			// how fast can I turn
#else
		0.0F,			// float	TurnSpeed			// how fast can I turn
#endif
		30,				// uint16	FireDelay			// how fast can I fire
#ifdef SINT_PEACEFROG_CHEAT
		360,			// uint16	ViewCone			// View Cone ( Degrees )
#else
		0,				// uint16	ViewCone			// View Cone ( Degrees )
#endif
		MODEL_TitanStarShrapnel,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		255.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		20.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		SHIP_RADIUS,	// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // uint16 Flags				// Initial Flags
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	13 Enemy Spiraling Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_SPIRALING,	// uint16	State				// Starting state
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
		( MISSILE_SPEED * 0.75F ),	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 0.75F ),	// float	StartSpeed;			// how fast do I move to start with
		256.0F,			// float	Size;				// how big is my collide sphere
		0.0F,			// float	TurnSpeed			// how fast can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		0,				// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_EnemySpiralMissile, // uint16	ModelNum;	// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		255.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		1.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot,						 		// Initial Flags
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	14 Enemy Homing Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	// uint16	State				// Starting state
		MISMOVE_HOMING,	// uint16	Move Type			// Straight/Homing
		( MISSILE_SPEED * 0.5F ), // float	StartSpeed;	// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 0.5F ), // float	Speed;		// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		2.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		360,			// uint16	ViewCone			// View Cone ( Degrees )
		(uint16) -1,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot, // uint16 Flags				// Initial Flags
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	15 Enemy Blue Homing Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	// uint16	State				// Starting state
		MISMOVE_HOMING,	// uint16	Move Type			// Straight/Homing
		( MISSILE_SPEED * 0.5F ), // float	StartSpeed;	// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 0.5F ), // float	Speed;		// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		2.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		360,			// uint16	ViewCone			// View Cone ( Degrees )
		(uint16) -1,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		64.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		255.0F,			// float	b;					// what color does it emit....
		128.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_CanShoot, // uint16 Flags				// Initial Flags
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	16 Enemy FireBall
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	// uint16	State				// Starting state
		MISMOVE_STRAIGHT, // uint16	Move Type			// Straight/Homing
		( MISSILE_SPEED * 1.5F ), // float	StartSpeed;	// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 1.5F ), // float	Speed;		// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		2.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		360,			// uint16	ViewCone			// View Cone ( Degrees )
		(uint16) -1,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		85.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_Nothing, // uint16 Flags				// Initial Flags
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	17 Enemy Tentacle
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_STRAIGHT,	//uint16	State				// Starting state
		MISMOVE_HOMING, // uint16	Move Type			// Straight/Homing
		MISSILE_SPEED,	// float	StartSpeed;			// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		MISSILE_SPEED,	// float	Speed;				// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		2.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		360,			// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_Eyeball,	// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		0.0F,			// float	ColRadius			// Collision Radius
		SECFLAGS_NoIncoming, // uint16 Flags				// Initial Flags
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	18 DepthCharge
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		SEC_MISSILE,	// uint16	SecType				// Missile/Mine
		MIS_GRAVITY,	//uint16	State				// Starting state
		MISMOVE_HOMING, // uint16	Move Type			// Straight/Homing
		( MISSILE_SPEED * 0.16F ),	// float	StartSpeed;		// how fast do I move to start with
		0.0F,			// float	SpeedInc;			// how fast do I Accelerate
		( MISSILE_SPEED * 0.16F ),	// float	Speed;	// how fast do I move
		256.0F,			// float	Size;				// how big is my collide sphere
		2.0F,			// float	TurnSpeed			// how fase can I turn
		30,				// uint16	FireDelay			// how fast can I fire
		360,			// uint16	ViewCone			// View Cone ( Degrees )
		MODEL_Purge,		// uint16	ModelNum;			// if 3D model which one....
		SEC_LIGHTSIZE,	// float	lightsize;			// how big is the light
		255.0F,			// float	r;					// what color does it emit....
		64.0F,			// float	g;					// what color does it emit....
		64.0F,			// float	b;					// what color does it emit....
		0.0F,			// float	Shield				// Shield
		64.0F,			// float	Damage				// Damage
		FALSE,			// BOOL		Lensflare			// Lensflare
		( SHIP_RADIUS * 0.5F ),	// float	ColRadius	// Collision Radius
		SECFLAGS_NoIncoming, // uint16 Flags			// Initial Flags
	},
};

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Set up And Init all SecBulls
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	InitSecBulls(void)
{
	uint16	i;

	FirstSecBullUsed = (uint16) -1;
	FirstSecBullFree = 0;

	SetupSecBullGroups();

	for( i = 0 ; i < MAXSECONDARYWEAPONBULLETS ; i++ )
	{
		SecBulls[ i ].Used = FALSE;
		SecBulls[ i ].Next = i + 1;
		SecBulls[ i ].Prev = (uint16) -1;
		SecBulls[ i ].NextInGroup = NULL;
		SecBulls[ i ].PrevInGroup = NULL;
		SecBulls[ i ].State = MIS_STRAIGHT;
		SecBulls[ i ].Flags = SECFLAGS_Nothing;
		SecBulls[ i ].Index = i;
		SecBulls[ i ].Type = (uint16) -1;
		SecBulls[ i ].SecType = SEC_MISSILE;
		SecBulls[ i ].DropCount = 0.0F;
		SecBulls[ i ].MoveType = MISMOVE_STRAIGHT;
		SecBulls[ i ].Owner = (uint16) -1;
		SecBulls[ i ].LifeCount = 0.0F;
		SecBulls[ i ].ColFlag = 0;
		SecBulls[ i ].GroupImIn = (uint16) -1;
		SecBulls[ i ].ModelNum = (uint16) -1;
		SecBulls[ i ].ModelIndex = (uint16) -1;
		SecBulls[ i ].fmpoly = (uint16) -1;
		SecBulls[ i ].numfmpolys = 0;
		SecBulls[ i ].poly = (uint16) -1;
		SecBulls[ i ].numpolys = 0;
		SecBulls[ i ].light = (uint16) -1;
		SecBulls[ i ].Target = (uint16) -1;
		SecBulls[ i ].TargetType = (uint16) -1;
		SecBulls[ i ].SpeedWanted = 32.0F;
		SecBulls[ i ].SpeedInc = 32.0F;
		SecBulls[ i ].Speed = 32.0F;
		SecBulls[ i ].TurnSpeed = 0.0F;
		SecBulls[ i ].ViewCone = 0.0F;
		SecBulls[ i ].DirVector = Forward;
		SecBulls[ i ].UpVector = SlideUp;
		SecBulls[ i ].DropVector = SlideDown;
		SecBulls[ i ].NumOldPos = 0;
		QuatFrom2Vectors( &SecBulls[ i ].DirQuat, &Forward, &SecBulls[ i ].DirVector );
		QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );
	}
	SecBulls[MAXSECONDARYWEAPONBULLETS-1].Next = (uint16) -1;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Find a free SecBull and move it from the free list to
					the used list
	Input		:	nothing
	Output		:	uint16 number of SecBull free....
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16	FindFreeSecBull(void)
{
	uint16 i;

	i = FirstSecBullFree;
	
	if ( i == (uint16) -1)
		return i;

	if( SecBulls[i].Used )
	{
		// This Secondary Bullet has been Used before....
		Msg( "%s Bullet has been Used more than once\n", DebugSecStrings[SecBulls[i].Type] );
	}
	
	
	SecBulls[i].Prev = FirstSecBullUsed;
	if ( FirstSecBullUsed != (uint16) -1)
	{
		SecBulls[FirstSecBullUsed].Next = i;
	}
	FirstSecBullUsed = i;
	FirstSecBullFree = SecBulls[i].Next;
	SecBulls[i].Used = TRUE;
	return i ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Kill a used SecBull and move it from the used list to
					the free list
	Input		:	uint16 number of SecBull free....
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	KillUsedSecBull( uint16 i )
{
	uint16	its_prev;
	uint16	its_next;
	

	if( !SecBulls[i].Used )
	{
		// This Secondary Bullet has been Freed before....
		Msg( "%s Bullet has been Freed more than once\n", DebugSecStrings[SecBulls[i].Type] );
	}
	
	its_prev = SecBulls[i].Prev;
	its_next = SecBulls[i].Next;

	if ( i == FirstSecBullUsed )
		FirstSecBullUsed = SecBulls[i].Prev;

	if( its_prev != (uint16) -1)
		SecBulls[its_prev].Next = its_next;

	if( its_next != (uint16) -1)
		SecBulls[its_next].Prev = its_prev;

	SecBulls[i].Prev = (uint16) -1;
	SecBulls[i].Next = FirstSecBullFree;
	FirstSecBullFree	= i;
	SecBulls[i].Used = FALSE;

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if a new bullet is due....if it is init it
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
static	int16	Toggle = 0;

void FireSecondary( void )
{
static int16	OnceOnlyFlag = 0;
	VECTOR	UpVector;
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	DropVector;
	VECTOR	recoil;
	int		firing;

	firing = 0;

#if SCATTER_TEST
	int16	Count;
#endif
	RegenerateQuedMines();

	if( WhoIAm == MAX_PLAYERS ) return;

	if( control.drop_secondary )
	{
		DropSecondary();
	}

	if( control.select_secondary )
	{
		if( SecondaryWeaponsGot[ ( control.select_secondary - 1 ) ] )
		{
			if( SecondaryAmmo[ ( control.select_secondary - 1 ) ] )
			{
				if( !OnceOnlyFlag )
				{
					if( SecondaryWeaponCheat )
					{
						if( Ships[ WhoIAm ].Secondary == ( control.select_secondary - 1 ) )
						{
							ToggleSecondaryWeapons( Ships[ WhoIAm ].Secondary );
						}
					}

					OnceOnlyFlag = 1;
					Ships[ WhoIAm ].Secondary = ( control.select_secondary - 1 );
					if( BikeCompSpeechSlider.value )
					{
						PlaySfx( SecondarySfxLookup[ Ships[ WhoIAm ].Secondary ], 1.0F );
					}else
					{
						PlaySfx( SFX_WeaponSelect, 1.0F );
					}
				}
			}
		}
		else
		{
			if( !OnceOnlyFlag )
			{
				OnceOnlyFlag = 1;
				if( !bSoundEnabled ) AddMessageToQue( YOU_DONT_HAVE_THAT_WEAPON );
				PlaySfx( SFX_DontHaveThat, 1.0F );
			}
		}
	}
	else
	{
		OnceOnlyFlag = 0;
	}

	if( control.select_next_secondary )
	{
		Ships[ WhoIAm ].Secondary = GetNextValidSeconadaryWeapon();
		if( BikeCompSpeechSlider.value )
		{
			PlaySfx( SecondarySfxLookup[ Ships[ WhoIAm ].Secondary ], 1.0F );
		}else
		{
			PlaySfx( SFX_WeaponSelect, 1.0F );
		}
	}

	if( control.select_prev_secondary )
	{
		Ships[ WhoIAm ].Secondary = GetPrevValidSecondaryWeapon();
		if( BikeCompSpeechSlider.value )
		{
			PlaySfx( SecondarySfxLookup[ Ships[ WhoIAm ].Secondary ], 1.0F );
		}else
		{
			PlaySfx( SFX_WeaponSelect, 1.0F );
		}
	}

	SecondarySelectControl();

	if( control.fire_mine )
	{
		DropMine();
		SecondaryFireDelay -= framelag;													// Added because mines could
		return;																			// be dropped the same time as
																						// firing secondary missiles
	}

	if ( SecondaryFireDelay <= 0.0F )
	{
		if( Ships[ WhoIAm ].Object.Mode == NORMAL_MODE )
		{
			if( control.fire_secondary )
			{
				SecondaryFireDelay += (float) SecondaryWeaponAttribs[ SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ] ].FireDelay;
	
				if( SecondaryAmmo[ Ships[ WhoIAm ].Secondary ] )
				{
					firing = 1;
					ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &Dir );
					ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideUp, &UpVector );		/* Calc Up Vector */
	
					switch( Ships[WhoIAm].Secondary )
					{
						case PURGEMINE:
						case PINEMINE:
						case SPIDERMINE:
						case QUANTUMMINE:
							break;

						default:
							Ships[ WhoIAm ].Object.Flags |= SHIP_SecFire;
							if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_SecToggle;
							else Ships[ WhoIAm ].Object.Flags &= ~SHIP_SecToggle;
							Ships[ WhoIAm ].SecID = Ships[ WhoIAm ].SecBullIdCount + 1;
							Ships[ WhoIAm ].SecWeapon = Ships[WhoIAm].Secondary;
							break;
					}

					switch( SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ] )
					{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire bottom Left, then Right, then Left, then...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case MUGMISSILE:
							if( Toggle == 0 )
							{
								Toggle = 1;
								DropVector.x = -0.7071F;
								DropVector.y = -0.7071F;
								DropVector.z = 0.0F;
							}
							else
							{
								Toggle = 0;
								DropVector.x = +0.7071F;
								DropVector.y = -0.7071F;
								DropVector.z = 0.0F;
							}
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, FALSE );
							break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire bottom Left and Right together
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case SOLARISMISSILE:
							if( Toggle == 0 )
							{
								Toggle = 1;
								DropVector.x = -0.7071F;
								DropVector.y = -0.7071F;
								DropVector.z = 0.0F;
							}
							else
							{
								Toggle = 0;
								DropVector.x = +0.7071F;
								DropVector.y = -0.7071F;
								DropVector.z = 0.0F;
							}
	
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, FALSE );
							break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire from bottom center at random trajectories
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case MULTIPLEMISSILE:
							DropVector.x = ( ( ( (float) Random_Range( 512 ) ) / 640.0F ) - 0.4F );
							DropVector.y = -( ( ( (float) Random_Range( 512 ) ) / 1024.0F ) + 1.0F );
							DropVector.z = ( ( (float) Random_Range( 512 ) ) / 512.0F );
							NormaliseVector( &DropVector );
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, FALSE );
							break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire from center below view
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case THIEFMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, FALSE );
							break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire from center below view
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case SCATTERMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
#if SCATTER_TEST
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos = Ships[ WhoIAm ].Object.Pos;

							for( Count = 0; Count < 8; Count++ )
							{
								Dir.x = (float) sin( D2R( ( 360.0F / 8.0F ) * Count ) );
								Dir.y = (float) cos( D2R( ( 360.0F / 8.0F ) * Count ) );
								Dir.z = 0.0F;
								UpVector.x = 0.0F;
								UpVector.y = 0.0F;
								UpVector.z = -1.0F;
								ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Dir, &Dir );
								ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &UpVector, &UpVector );
								InitOneSecBull( WhoIAm, Ships[WhoIAm].Object.Group, Pos, Dir, UpVector, DropVector, ++Ships[ WhoIAm ].SecBullIdCount, Ships[ WhoIAm ].Secondary, FALSE );
							}
#else
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, FALSE );
#endif
							break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire from center below view
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case GRAVGONMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, FALSE );
							break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire from center below view
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case TITANSTARMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
			
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, FALSE );
							recoil.x = 0.0F;
							recoil.y = 0.0F;
							recoil.z = -( 20.0F * GLOBAL_SCALE );
							ForceInternalOneOff( WhoIAm, &recoil );
							break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Drop Behind
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case PURGEMINE:
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = -1.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, TRUE );
							break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Drop Behind
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case PINEMINE:
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = -1.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, TRUE );
							break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Drop Behind
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case QUANTUMMINE:
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = -1.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, TRUE );
							break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Drop Behind
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case SPIDERMINE:
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = -1.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, Ships[ WhoIAm ].Secondary, TRUE );
							break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Spiral Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case ENEMYSPIRALMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ], FALSE );
							break;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Photon Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case ENEMYHOMINGMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ], FALSE );
							break;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Blue Photon Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case ENEMYBLUEHOMINGMISSILE:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ], FALSE );
							break;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Blue Photon Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
						case ENEMYFIREBALL:
							DropVector.x = 0.0F;
							DropVector.y = -1.0F;
							DropVector.z = 0.0F;
		
							ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
							Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
							Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
							Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
							InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
											Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
											&Pos, &Dir, &UpVector, &DropVector, SecondaryToFireLookup[ Ships[ WhoIAm ].Secondary ], FALSE );
							break;
					}
				}
			}
			else
			{
				if( SecondaryFireDelay < 0.0F ) SecondaryFireDelay = 0.0F;
			}
		}
   }
   else
   {
		SecondaryFireDelay -= framelag;
   }
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Clean up and kill a Sec Bull
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CleanUpSecBull( uint16 i )
{
	uint16	light;
	uint16	fmpoly;
	uint16	nextfmpoly;
	uint16	model;
	uint16	poly;
	uint16	nextpoly;
	int16	Count;

	light = SecBulls[i].light;
	fmpoly = SecBulls[i].fmpoly;
	poly = SecBulls[i].poly;
	model = SecBulls[i].ModelIndex;

	RemoveSecBullFromGroup( i, SecBulls[i].GroupImIn );

	if ( light != (uint16) -1 )
	{
		SetLightDie(SecBulls[i].light);					// Kill Light
		SecBulls[i].light = (uint16) -1;
	}

	for( Count = 0; Count < SecBulls[i].numfmpolys; Count++ )
	{
		nextfmpoly = FmPolys[ fmpoly ].Prev;
		if( fmpoly != (uint16) -1 ) KillUsedFmPoly( fmpoly );
		fmpoly = nextfmpoly;
	}
	SecBulls[i].numfmpolys = 0;
	SecBulls[i].fmpoly = (uint16) -1;

	if ( model != (uint16) -1 )
	{
		KillUsedModel( model );							// Kill Model
		SecBulls[i].ModelIndex = (uint16) -1;
	}

	for( Count = 0; Count < SecBulls[i].numpolys; Count++ )
	{
		nextpoly = Polys[ poly ].Prev;
		if( poly != (uint16) -1 ) KillUsedPoly( poly );
		poly = nextpoly;
	}
	SecBulls[i].numpolys = 0;
	SecBulls[i].poly = (uint16) -1;

	SecBulls[i].LifeCount = 0.0F;
	if( CameraMissile == i ) MissileCameraActive = 0;

	KillUsedSecBull( i );								// Kill Bullet
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Init 1 Secondary Bullet..Mainly for Dplay...
	Input		:	uint16		OwnerType
				:	uint16		Owner
				:	uint16		Bullet ID
				:	uint16		Group
				:	VECTOR	*	Pos
				:	VECTOR	*	Offset
				:	VECTOR	*	Dir
				:	VECTOR	*	DropDir
				:	int8		Weapon
				:	BOOL		NetFlag ( Send to everyone? )
	Output		:	uint16		SecBull Index ( -1 nothing )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16	InitOneSecBull( uint16 OwnerType, uint16 Owner, uint16 BulletID, uint16 Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * UpVector,
						VECTOR * DropDir, int8 Weapon, BOOL NetFlag )
{
	uint16	i;
	uint16	light;
	uint16	fmpoly;
	uint16	model;
	int16	Count;

	if( !VectorLength( Dir ) )
	{
		return( (uint16) -1 );
	}

	switch( Weapon )
	{
		case MUGMISSILE:
		case ENEMYSPIRALMISSILE:
		case ENEMYHOMINGMISSILE:
		case ENEMYBLUEHOMINGMISSILE:
		case ENEMYFIREBALL:
		case ENEMYDEPTHCHARGE:
		case TITANSTARSHRAPNEL:
		case THIEFMISSILE:
		case MULTIPLEMISSILE:
		case ENEMYTENTACLE:
			if( MyGameStatus != STATUS_SinglePlayer )
			{
				if( !IsGroupVisible[ Group ] && !VisibleOverlap( Ships[ Current_Camera_View ].Object.Group, Group, NULL ) && !outside_map )
					return( (uint16) -1 );
			}
			break;

		case SOLARISMISSILE:
		case SCATTERMISSILE:
		case TITANSTARMISSILE:
		case GRAVGONMISSILE:
		case PURGEMINE:
		case PINEMINE:
		case QUANTUMMINE:
		case SPIDERMINE:
		case PINEMISSILE:
			break;

		default:
			Weapon = -1;
			break;
	}

	i = FindFreeSecBull();

	if( i != (uint16) -1 )
	{
		if( ( OwnerType == OWNER_SHIP ) && ( Owner == WhoIAm ) )
		{
			CameraMissile = i;
			MissileCameraActive = 1;
		}

		SecBulls[i].DropVector = *DropDir;
		SecBulls[i].DirVector = *Dir;
		SecBulls[i].UpVector = *UpVector;

		QuatFromDirAndUp( &SecBulls[ i ].DirVector, &SecBulls[ i ].UpVector, &SecBulls[ i ].DirQuat );
		QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );							// Create Dir Matrix

		SecBulls[i].Type = Weapon;
		SecBulls[i].Flags = SecondaryWeaponAttribs[ Weapon ].Flags;
		SecBulls[i].SecType = SecondaryWeaponAttribs[ Weapon ].SecType;
		SecBulls[i].MoveType = SecondaryWeaponAttribs[ Weapon ].MoveType;
		SecBulls[i].ColRadius = SecondaryWeaponAttribs[ Weapon ].ColRadius;
		SecBulls[i].DropCount = 2.0F;
		SecBulls[i].TargetType = (uint16) -1;
		SecBulls[i].Target = (uint16) -1;
		SecBulls[i].Pos.x = ( Pos->x + Offset->x );
		SecBulls[i].Pos.y = ( Pos->y + Offset->y );
		SecBulls[i].Pos.z = ( Pos->z + Offset->z );
		SecBulls[i].Offset = *Offset;
		SecBulls[i].StartPos = SecBulls[i].Pos;
		SecBulls[i].StartDir = SecBulls[i].DirVector;
		SecBulls[i].StartMat = SecBulls[i].Mat;
		SecBulls[i].OwnerType = OwnerType;
		SecBulls[i].Owner = Owner;
		SecBulls[i].Weapon = Weapon;
		SecBulls[i].ID = BulletID;
		SecBulls[i].State = SecondaryWeaponAttribs[ Weapon ].State;
		SecBulls[i].SpeedInc = SecondaryWeaponAttribs[ Weapon ].SpeedInc;
		SecBulls[i].SpeedWanted = SecondaryWeaponAttribs[ Weapon ].Speed;
		SecBulls[i].Speed = SecondaryWeaponAttribs[ Weapon ].StartSpeed;
		SecBulls[i].TurnSpeed = SecondaryWeaponAttribs[ Weapon ].TurnSpeed;
		SecBulls[i].ViewCone = (float) COSD( SecondaryWeaponAttribs[ Weapon ].ViewCone / 2 );
		SecBulls[i].LifeCount = 0.0F;
		SecBulls[i].LifeSpan = 0.0F;
		SecBulls[i].ModelNum = SecondaryWeaponAttribs[ Weapon ].ModelNum;
		SecBulls[i].xsize = 32.0F;
		SecBulls[i].ysize = 32.0F;
		SecBulls[i].ColFlag = 0;
		SecBulls[i].ColPoint.x = 0.0F;
		SecBulls[i].ColPoint.y = 0.0F;
		SecBulls[i].ColPoint.z = 0.0F;
		SecBulls[i].GroupImIn = Group;
		SecBulls[i].Shield = SecondaryWeaponAttribs[ Weapon ].Shield;
		SecBulls[i].Damage = SecondaryWeaponAttribs[ Weapon ].Damage;
		SecBulls[i].Ammo = 0;
		SecBulls[i].NumOldPos = 0;
		SecBulls[i].fmpoly = (uint16) -1;
		SecBulls[i].numfmpolys = 0;
		SecBulls[i].numpolys = 0;
		SecBulls[i].Time = 0.0F;
		SecBulls[i].Interval = (float) ( 12 - TrailDetailSlider.value ); //8.0F;
		SecBulls[i].Lensflare = SecondaryWeaponAttribs[ Weapon ].Lensflare;
		SecBulls[i].NumBounces = 0;
		SecBulls[i].RetractPos = 0.0F;
		SecBulls[i].FramelagAddition = 0.0F;

		if( AutoDetail )
		{
			if( avgframelag > 2.0F ) SecBulls[i].Interval +=  ( avgframelag - 1.0F );
		}

		if( ( OwnerType == OWNER_SHIP ) && ( Owner == WhoIAm ) )
		{
			if( SecBulls[i].SecType != SEC_XMISSILE )
			{
				if( !GodMode )
				{
					int8	TempWeapon;

					TempWeapon = ConvSecToNormWeapon( Weapon );

					if( SecBulls[i].SecType != SEC_MINE ) SecAmmoUsed[ TempWeapon ]++;
					if( SecondaryAmmo[ TempWeapon ] ) SecondaryAmmo[ TempWeapon ]--;
					if( !SecondaryAmmo[ TempWeapon ] ) SecondaryWeaponsGot[ TempWeapon ] = 0;
				}
			}
		}

		SecBulls[i].GroupImIn = MoveGroup( &Mloadheader, Pos, Group, Offset );

		AddSecBullToGroup( i, SecBulls[i].GroupImIn );

		switch( Weapon )
		{
			case TITANSTARSHRAPNEL:
				SecBulls[i].LifeSpan = (float) 600.0F;
				break;

			case THIEFMISSILE:
				SecBulls[i].LifeSpan = 600.0F;
				break;

			case ENEMYTENTACLE:
				SecBulls[i].LifeSpan = 200.0F;
				break;

			case ENEMYSPIRALMISSILE:
			case ENEMYHOMINGMISSILE:
			case ENEMYBLUEHOMINGMISSILE:
			case MUGMISSILE:
			case SOLARISMISSILE:
			case SCATTERMISSILE:
			case GRAVGONMISSILE:
				PlayPannedSfx( SFX_MissileLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case TITANSTARMISSILE:
				PlayPannedSfx( SFX_TitanLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 1.0F );
				break;

			case MULTIPLEMISSILE:
				//PlayPannedSfx( SFX_MFRLLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				PlayPannedSfxWithVolModify(SFX_MFRLLaunch, SecBulls[i].GroupImIn, &SecBulls[i].Pos, 0.0F, 0.85F );
				break;

			case PINEMISSILE:
				PlayPannedSfx( SFX_MissileLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case PURGEMINE:
				PlayPannedSfx( SFX_DropMine, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case PINEMINE:
				PlayPannedSfx( SFX_DropMine, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				SecBulls[i].Ammo = 10;
				break;

			case QUANTUMMINE:
				PlayPannedSfx( SFX_DropMine, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case SPIDERMINE:
				PlayPannedSfx( SFX_DropMine, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case ENEMYFIREBALL:
				SecBulls[i].NumBounces = 2;
				PlayPannedSfx( SFX_MissileLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;

			case ENEMYDEPTHCHARGE:
				QuatFromDirAndUp( &Forward, &SlideUp, &SecBulls[ i ].DirQuat );
				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );			// Create Dir Matrix

				SecBulls[i].LifeSpan = ( ( ( (float) Random_Range( 10000 ) ) / 5000.0F ) + 2.0F ) * 60.0F;
				SecBulls[i].DirVector.x *= SecBulls[i].Speed;
				SecBulls[i].DirVector.y *= SecBulls[i].Speed;
				SecBulls[i].DirVector.z *= SecBulls[i].Speed;
				PlayPannedSfx( SFX_MissileLaunch, SecBulls[i].GroupImIn , &SecBulls[i].Pos, 0.0F );
				break;
		}

		light = (uint16) -1;
		fmpoly = (uint16) -1;
		model = (uint16) -1;

		if( SecondaryWeaponAttribs[ Weapon ].lightsize != 0.0F )
		{
			light = FindFreeXLight();
			if( light != (uint16 ) -1 )
			{
				switch( Weapon )
				{
					case QUANTUMMINE:
						ApplyMatrix( &SecBulls[ i ].Mat, &Forward, &XLights[ light ].Dir ); /* Calc Direction Vector */
						NormaliseVector( &XLights[ light ].Dir );
						XLights[ light ].Type = SPOT_LIGHT;
						XLights[ light ].Pos = SecBulls[i].Pos;
						XLights[ light ].CosArc = (float)cos(D2R(30));
						XLights[ light ].Size = ( 4096.0F * GLOBAL_SCALE ); //PickupAttribs[ Type ].Lightsize;
						XLights[ light ].SizeCount = 0.0F;
						XLights[ light ].r = SecondaryWeaponAttribs[ Weapon ].r;
						XLights[ light ].g = SecondaryWeaponAttribs[ Weapon ].g;
						XLights[ light ].b = SecondaryWeaponAttribs[ Weapon ].b;
						XLights[ light ].Group = SecBulls[i].GroupImIn;
						break;

					default:
						XLights[light].Pos = SecBulls[i].Pos;
						XLights[light].Size = SecondaryWeaponAttribs[ Weapon ].lightsize;
						XLights[light].SizeCount = 0.0F;
						XLights[light].r = SecondaryWeaponAttribs[ Weapon ].r;
						XLights[light].g = SecondaryWeaponAttribs[ Weapon ].g;
						XLights[light].b = SecondaryWeaponAttribs[ Weapon ].b;
						XLights[light].Group = SecBulls[i].GroupImIn;
						break;
				}
				if( !SecondaryLightDetail ) XLights[ light ].Visible = FALSE;
			}
		}
		SecBulls[i].light = light;

		if( SecBulls[i].ModelNum != (uint16) -1 )
		{
			model =	FindFreeModel();

			if( model != (uint16 ) -1 )
			{
				Models[model].Owner = SecBulls[i].Owner;
				Models[model].OwnerType = SecBulls[i].OwnerType;
				Models[model].ModelNum = SecBulls[ i ].ModelNum;
				Models[model].Type = MODTYPE_Missile;
				Models[model].Flags = MODFLAG_Clip;
				Models[model].Visible = TRUE;
				Models[model].Pos = SecBulls[ i ].Pos;
				Models[model].Mat = SecBulls[ i ].Mat;
				MatrixTranspose( &SecBulls[ i ].Mat, &Models[model].InvMat );
				Models[model].Func = MODFUNC_Nothing;
			}
		}
		SecBulls[i].ModelIndex = model;

		switch( Weapon )
		{
			case ENEMYTENTACLE:
				SecBulls[i].Interval = 8.0F;
				break;

			case SCATTERMISSILE:
				SecBulls[i].Interval = 8.0F;

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
   				if( fmpoly != (uint16 ) -1 )
   				{
   					FmPolys[ fmpoly ].LifeCount = 1000.0F;
   					FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
   					FmPolys[ fmpoly ].SeqNum = FM_ZERO;
   					FmPolys[ fmpoly ].Frame = 0.0F;
#if ACTUAL_TRANS
					FmPolys[ fmpoly ].Trans = 128;
#else
					FmPolys[ fmpoly ].Trans = 255;
#endif
					FmPolys[ fmpoly ].R = 64;
					FmPolys[ fmpoly ].G = 64;
					FmPolys[ fmpoly ].B = 255;
   					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
   					FmPolys[ fmpoly ].xsize = ( 10.0F * GLOBAL_SCALE );
   					FmPolys[ fmpoly ].ysize = ( 10.0F * GLOBAL_SCALE );
   					FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
					FmPolys[ fmpoly ].Group = SecBulls[i].GroupImIn;
					SecBulls[i].numfmpolys++;
					SecBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

			case ENEMYHOMINGMISSILE:
				fmpoly = FindFreeFmPoly();					// Faceme polygon attached

				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 10000.0F;
					FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = FM_ZERO;
					FmPolys[ fmpoly ].Frame = 2.0F;
					FmPolys[ fmpoly ].AnimSpeed = 1.0F;
					FmPolys[ fmpoly ].R = 255;
					FmPolys[ fmpoly ].G = 128;
					FmPolys[ fmpoly ].B = 64;
   					FmPolys[ fmpoly ].Trans = 255;
					FmPolys[ fmpoly ].Frm_Info = &LensFlare_Header;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].Rot = 0.0F;
					FmPolys[ fmpoly ].xsize = ( 12.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 12.0F * GLOBAL_SCALE );
   					FmPolys[ fmpoly ].Dir.x = 0.0F;
   					FmPolys[ fmpoly ].Dir.y = 0.0F;
   					FmPolys[ fmpoly ].Dir.z = 0.0F;
					FmPolys[ fmpoly ].Speed = 0.0F;
					FmPolys[ fmpoly ].UpSpeed = 0.0F;
					FmPolys[ fmpoly ].Group = Group;
					SecBulls[i].numfmpolys++;
					SecBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}

				for( Count = 0; Count < 10; Count++ )
				{
					fmpoly = FindFreeFmPoly();					// Faceme polygon attached

					if( fmpoly != (uint16 ) -1 )
					{
						FmPolys[ fmpoly ].LifeCount = 10000.0F;
						FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
						FmPolys[ fmpoly ].SeqNum = FM_PHOTON;
						FmPolys[ fmpoly ].Frame = 0.0F;
						FmPolys[ fmpoly ].AnimSpeed = 1.0F;
						FmPolys[ fmpoly ].R = 64;
						FmPolys[ fmpoly ].G = 64;
						FmPolys[ fmpoly ].B = 64;
   						FmPolys[ fmpoly ].Trans = 255;
						FmPolys[ fmpoly ].Frm_Info = &BigFlash_Header;
						FmPolys[ fmpoly ].DirVector.x = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].DirVector.y = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].DirVector.z = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.x = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.y = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.z = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						NormaliseVector( &FmPolys[ fmpoly ].DirVector );
						NormaliseVector( &FmPolys[ fmpoly ].UpVector );
   						FmPolys[ fmpoly ].Flags = FM_FLAG_RELATIVE;
						FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
						FmPolys[ fmpoly ].RotSpeed = ( ( (float) Random_Range( 10000 ) ) / 2500.0F ) + 2.0F;
						FmPolys[ fmpoly ].xsize = ( 2.0F * GLOBAL_SCALE );
						FmPolys[ fmpoly ].ysize = ( 12.0F * GLOBAL_SCALE );
   						FmPolys[ fmpoly ].Dir.x = 0.0F;
   						FmPolys[ fmpoly ].Dir.y = 0.0F;
   						FmPolys[ fmpoly ].Dir.z = 0.0F;
						FmPolys[ fmpoly ].Speed = 0.0F;
						FmPolys[ fmpoly ].UpSpeed = 0.0F;
						FmPolys[ fmpoly ].Group = SecBulls[i].GroupImIn;
						SecBulls[i].numfmpolys++;
						SecBulls[i].fmpoly = fmpoly;
						AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
					}
				}
				break;

			case ENEMYBLUEHOMINGMISSILE:
				fmpoly = FindFreeFmPoly();					// Faceme polygon attached

				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 10000.0F;
					FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = FM_ZERO;
					FmPolys[ fmpoly ].Frame = 2.0F;
					FmPolys[ fmpoly ].AnimSpeed = 1.0F;
					FmPolys[ fmpoly ].R = 96;
					FmPolys[ fmpoly ].G = 96;
					FmPolys[ fmpoly ].B = 255;
   					FmPolys[ fmpoly ].Trans = 255;
					FmPolys[ fmpoly ].Frm_Info = &LensFlare_Header;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].Rot = 0.0F;
					FmPolys[ fmpoly ].xsize = ( 12.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 12.0F * GLOBAL_SCALE );
   					FmPolys[ fmpoly ].Dir.x = 0.0F;
   					FmPolys[ fmpoly ].Dir.y = 0.0F;
   					FmPolys[ fmpoly ].Dir.z = 0.0F;
					FmPolys[ fmpoly ].Speed = 0.0F;
					FmPolys[ fmpoly ].UpSpeed = 0.0F;
					FmPolys[ fmpoly ].Group = Group;
					SecBulls[i].numfmpolys++;
					SecBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}

				for( Count = 0; Count < 10; Count++ )
				{
					fmpoly = FindFreeFmPoly();					// Faceme polygon attached

					if( fmpoly != (uint16 ) -1 )
					{
						FmPolys[ fmpoly ].LifeCount = 10000.0F;
						FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
						FmPolys[ fmpoly ].SeqNum = FM_BLUEPHOTON;
						FmPolys[ fmpoly ].Frame = 0.0F;
						FmPolys[ fmpoly ].AnimSpeed = 1.0F;
						FmPolys[ fmpoly ].R = 128;
						FmPolys[ fmpoly ].G = 128;
						FmPolys[ fmpoly ].B = 128;
   						FmPolys[ fmpoly ].Trans = 255;
						FmPolys[ fmpoly ].Frm_Info = &BlueSpark_Header;
						FmPolys[ fmpoly ].DirVector.x = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].DirVector.y = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].DirVector.z = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.x = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.y = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						FmPolys[ fmpoly ].UpVector.z = ( ( ( (float) Random_Range( 10000 ) ) / 10000.0F ) - 0.5F );
						NormaliseVector( &FmPolys[ fmpoly ].DirVector );
						NormaliseVector( &FmPolys[ fmpoly ].UpVector );
   						FmPolys[ fmpoly ].Flags = FM_FLAG_RELATIVE;
						FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
						FmPolys[ fmpoly ].RotSpeed = ( ( (float) Random_Range( 10000 ) ) / 2500.0F ) + 2.0F;
						FmPolys[ fmpoly ].xsize = ( 4.0F * GLOBAL_SCALE );
						FmPolys[ fmpoly ].ysize = ( 12.0F * GLOBAL_SCALE );
   						FmPolys[ fmpoly ].Dir.x = 0.0F;
   						FmPolys[ fmpoly ].Dir.y = 0.0F;
   						FmPolys[ fmpoly ].Dir.z = 0.0F;
						FmPolys[ fmpoly ].Speed = 0.0F;
						FmPolys[ fmpoly ].UpSpeed = 0.0F;
						FmPolys[ fmpoly ].Group = SecBulls[i].GroupImIn;
						SecBulls[i].numfmpolys++;
						SecBulls[i].fmpoly = fmpoly;
						AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
					}
				}
				break;

			case ENEMYFIREBALL:
				fmpoly = FindFreeFmPoly();					// Faceme polygon attached

				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 10000.0F;
					FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = FM_ZERO;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].AnimSpeed = 1.0F;
					FmPolys[ fmpoly ].R = 255;
					FmPolys[ fmpoly ].G = 128;
					FmPolys[ fmpoly ].B = 128;
   					FmPolys[ fmpoly ].Trans = 255;
					FmPolys[ fmpoly ].Frm_Info = &Circle_Header;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].Rot = 0.0F;
					FmPolys[ fmpoly ].xsize = ( 8.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 8.0F * GLOBAL_SCALE );
   					FmPolys[ fmpoly ].Dir.x = 0.0F;
   					FmPolys[ fmpoly ].Dir.y = 0.0F;
   					FmPolys[ fmpoly ].Dir.z = 0.0F;
					FmPolys[ fmpoly ].Speed = 0.0F;
					FmPolys[ fmpoly ].UpSpeed = 0.0F;
					FmPolys[ fmpoly ].Group = Group;
					SecBulls[i].numfmpolys++;
					SecBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

			case TITANSTARSHRAPNEL:
			case THIEFMISSILE:
			case ENEMYSPIRALMISSILE:
			case MUGMISSILE:
			case SOLARISMISSILE:
			case MULTIPLEMISSILE:
			case GRAVGONMISSILE:
			case TITANSTARMISSILE:
			case PINEMISSILE:
			case PURGEMINE:
			case PINEMINE:
			case QUANTUMMINE:
			case SPIDERMINE:
			case ENEMYDEPTHCHARGE:
			default:
				break;
		}

		if( NetFlag )
		{
			SecBullPosDirSend( OwnerType, Owner, BulletID, Group, Pos, Offset, Dir, UpVector,
						DropDir, Weapon );
		}
	}
	return i;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check Point to Sphere collision
	Input		:	VERT	*	Target Position
				:	float		Target Radius
				:	VERT	*	Point
	Output		:	BOOL		TRUE/FALSE
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL PointToSphere(	VERT * TargPos, float TargRadius, VERT * BulPos )
{
	float	d;
	float	xd, yd, zd;

	xd = ( TargPos->x - BulPos->x );
	yd = ( TargPos->y - BulPos->y );
	zd = ( TargPos->z - BulPos->z );
	d = ( xd * xd ) + ( yd * yd ) + ( zd * zd );
	if( d < 0.0F ) d = -d;
	if( d <= ( TargRadius * TargRadius ) ) return TRUE;

	return FALSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Process Secondary Bullets
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ProcessSecondaryBullets( void )
{
	uint16		i;
	uint16		NextMissile;
	MATRIX		TempMat;
	uint16		fmpoly;
	uint16		nextfmpoly;
	int16		Count;
	float		NewFramelag;

	i = FirstSecBullUsed;

	if( ImTargeted )
	{
		TargetedDelay -= framelag;
		if( TargetedDelay <= 0.0F )
		{
			if( !( SecBulls[ MissileNum ].Flags & SECFLAGS_NoIncoming ) )
			{
				PlayPannedSfx( SFX_Lockon, SecBulls[ MissileNum ].GroupImIn , &SecBulls[ MissileNum ].Pos, 0.0F );
			}
			TargetedDelay += ( ( MissileDistance / ( 1024.0F * GLOBAL_SCALE ) ) * 2.0F );
		}
	}
	else
	{
		TargetedDelay = 0.0F;
	}

	ImTargeted = FALSE;
	MissileDistance = 32767.0F;

	while( i != (uint16) -1 )
	{
		NextMissile = SecBulls[ i ].Prev;											/* Next Missile */

		NewFramelag = ( framelag + SecBulls[i].FramelagAddition );

		SecBulls[ i ].LifeCount += NewFramelag; //framelag;

		switch( SecBulls[ i ].SecType )
		{
			case SEC_MISSILE:
			case SEC_XMISSILE:
				if( !ProcessMissiles( i, &NextMissile ) )
				{
					i = NextMissile;
					continue;
				}
				break;

			case SEC_MINE:
				if( !ProcessMines( i ) )
				{
					i = NextMissile;
					continue;
				}
				break;

			default:
				i = NextMissile;
				continue;
				break;
		}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Update Attached Objects
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		if( i != (uint16) -1 )
		{
			if( SecBulls[i].light != (uint16) -1 )									/* Light Attached? */
			{																		
				XLights[SecBulls[i].light].Pos = SecBulls[i].Pos;					/* Update Light Position */
				XLights[SecBulls[i].light].Group = SecBulls[i].GroupImIn;			/* Update Light Group */
				if( XLights[ SecBulls[i].light ].Type == SPOT_LIGHT )
				{
					MatrixTranspose( &SecBulls[i].Mat, &TempMat );
					ApplyMatrix( &TempMat, &Forward, &XLights[ SecBulls[i].light ].Dir );
					NormaliseVector( &XLights[ SecBulls[i].light ].Dir );
				}
			}																		

			if( SecBulls[i].fmpoly != (uint16) -1 )
			{
				fmpoly = SecBulls[i].fmpoly;

				for( Count = 0; Count < SecBulls[i].numfmpolys; Count++ )
				{
					nextfmpoly = FmPolys[ fmpoly ].Prev;
			
					if ( fmpoly != (uint16) -1 )
					{
						FmPolys[ fmpoly ].Pos = SecBulls[i].Pos;
						FmPolys[ fmpoly ].Group = SecBulls[i].GroupImIn;
						if( ( SecBulls[i].Weapon == SCATTERMISSILE ) ||
							( SecBulls[i].Weapon == ENEMYTENTACLE ) )
						{
							FmPolys[ fmpoly ].Pos.x += ( -SecBulls[i].DirVector.x * ( 190.0F * GLOBAL_SCALE ) );
							FmPolys[ fmpoly ].Pos.y += ( -SecBulls[i].DirVector.y * ( 190.0F * GLOBAL_SCALE ) );
							FmPolys[ fmpoly ].Pos.z += ( -SecBulls[i].DirVector.z * ( 190.0F * GLOBAL_SCALE ) );
						}
					}
					fmpoly = nextfmpoly;
				}
			}

			if ( SecBulls[i].ModelIndex != (uint16) -1 )							/* 3D Model Attached? */
			{																		
				Models[SecBulls[i].ModelIndex].Mat	  = SecBulls[i].Mat;			/* Update Model Matrix */
				MatrixTranspose( &SecBulls[i].Mat, &Models[SecBulls[i].ModelIndex].InvMat );
				Models[SecBulls[i].ModelIndex].Pos	  = SecBulls[i].Pos;			
				Models[SecBulls[i].ModelIndex].Group  = SecBulls[i].GroupImIn;
			}																		
		}

		i = NextMissile;														/* Next Missile */
	}																				
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Process All Mines
	Input		:	uint16		Secondary Weapon Bullet Index
	Output		:	BOOL		True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL ProcessMines( uint16 i )
{
	float		Cos;
	float		Angle;
	VECTOR		DirVector;
	QUATLERP	qlerp;
	VECTOR		DropVector;
	VECTOR		DropPos;
	VECTOR		FrameLag_Rot;
	QUAT		TempQuat;
	int16		Val;
	float		Height;
	float		ClosestCos = -1.0F;
	uint16		Ship, Enemy, Secondary;
	uint16		Owner;
	float		Radius;
	uint16		BulletID;
	float		NewFramelag;

	NewFramelag = ( framelag + SecBulls[i].FramelagAddition );
	if( SecBulls[i].FramelagAddition != 0.0F )
	{
		SecBulls[i].FramelagAddition = 0.0F;
	}

	if( SecBulls[i].LifeSpan )
	{
		if( SecBulls[i].LifeCount > SecBulls[i].LifeSpan )
		{
			DestroySecondary( i, &SecBulls[i].Pos );
			return( FALSE );
		}
	}

	switch( SecBulls[ i ].Type )
	{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		PURGE MINE, proximity detonation
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case PURGEMINE:
			FrameLag_Rot.x = 0.0F;
			FrameLag_Rot.y = ( 2.0F * framelag );
			FrameLag_Rot.z = 0.0F;
			MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
			QuatMultiply( &SecBulls[ i ].DirQuat, &TempQuat, &SecBulls[ i ].DirQuat );
			QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );				// Create Dir Matrix

			Val = (int16) ( SecBulls[i].LifeCount * 3.0F );
			Val %= 360;
			Height = (float) ( COSD( Val ) * ( 200.0F * GLOBAL_SCALE ) );
			SecBulls[i].Pos.x = SecBulls[i].StartPos.x + ( SecBulls[i].UpVector.x * Height );
			SecBulls[i].Pos.y = SecBulls[i].StartPos.y + ( SecBulls[i].UpVector.y * Height );
			SecBulls[i].Pos.z = SecBulls[i].StartPos.z + ( SecBulls[i].UpVector.z * Height );

			if( SecBulls[i].LifeCount >= 150.0F ) Owner = (uint16) -1;
			else Owner = SecBulls[i].Owner;
	
			if( CheckProximityToShips( &SecBulls[i].Pos, SecBulls[i].GroupImIn, MINE_TRIGGER_RADIUS,
									   SecBulls[i].OwnerType, Owner ) )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( FALSE );
			}
			else
			{
				Radius = MaxColDistance;
				Enemy = GetClosestEnemy( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &Radius, SecBulls[i].OwnerType, Owner );

				if( Enemy != (uint16) -1 )
				{
					if( Radius < ( ( EnemyTypes[ Enemies[ Enemy ].Type ].Radius * 2.0F ) + MINE_TRIGGER_RADIUS ) )
					{
						if( !( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) ||
							( ( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) && ( Radius <= Enemies[ Enemy ].MineDistance ) ) )
						{
							Enemies[ Enemy ].MinePos = SecBulls[i].Pos;
							Enemies[ Enemy ].MineDistance = Radius;
							Enemies[ Enemy ].AIFlags |= AI_MINEISCLOSE;
							Enemies[ Enemy ].MineNum = i;
						}
					}
					if( Radius < MINE_TRIGGER_RADIUS ) DestroySecondary( i, &SecBulls[i].Pos );
				}
			}
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		PINE MINE, Aim at nearest target and fire 1 missile per sec
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case PINEMINE:
			if( SecBulls[i].LifeCount >= 150.0F ) Owner = (uint16) -1;
			else Owner = SecBulls[i].Owner;
	
			if( CheckProximityToShips( &SecBulls[i].Pos, SecBulls[i].GroupImIn, MINE_TRIGGER_RADIUS,
									   SecBulls[i].OwnerType, Owner ) )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( FALSE );
			}
			else
			{
				Radius = MaxColDistance;
				Enemy = GetClosestEnemy( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &Radius, SecBulls[i].OwnerType, Owner );

				if( Enemy != (uint16) -1 )
				{
					if( Radius < ( ( EnemyTypes[ Enemies[ Enemy ].Type ].Radius * 2.0F ) + MINE_TRIGGER_RADIUS ) )
					{
						if( !( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) ||
							( ( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) && ( Radius <= Enemies[ Enemy ].MineDistance ) ) )
						{
							Enemies[ Enemy ].MinePos = SecBulls[i].Pos;
							Enemies[ Enemy ].MineDistance = Radius;
							Enemies[ Enemy ].AIFlags |= AI_MINEISCLOSE;
							Enemies[ Enemy ].MineNum = i;
						}
					}
					if( Radius < MINE_TRIGGER_RADIUS ) DestroySecondary( i, &SecBulls[i].Pos );
				}
			}

			Ship = (uint16) -1;
			Enemy = (uint16) -1;
			Secondary = (uint16) -1;

#ifdef SINT_PEACEFROG_CHEAT
			TargetingWeaponCheat = SecBulls[i].Weapon;
#endif
			switch( SecBulls[i].OwnerType )
			{
				case OWNER_SHIP:
					Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					Enemy = TargetClosestEnemy( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											    SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												&ClosestCos );
					if( Enemy != (uint16) -1 ) Ship = (uint16) -1;
					Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					if( Secondary != (uint16) -1 )
					{
						Ship = (uint16) -1;
						Enemy = (uint16) -1;
					}
					break;

				case OWNER_ENEMY:
					Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					if( Secondary != (uint16) -1 )
					{
						Ship = (uint16) -1;
					}
					break;

				case OWNER_NOBODY:
					Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
											  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
											  &ClosestCos );
					if( Secondary != (uint16) -1 )
					{
						Ship = (uint16) -1;
					}
					break;

				default:
					break;
			}

			if( Ship != (uint16) -1 )
			{
				DirVector.x = ( Ships[ Ship ].Object.Pos.x - SecBulls[ i ].Pos.x );
				DirVector.y = ( Ships[ Ship ].Object.Pos.y - SecBulls[ i ].Pos.y );
				DirVector.z = ( Ships[ Ship ].Object.Pos.z - SecBulls[ i ].Pos.z );
			}
			else
			{
				if( Enemy != (uint16) -1 )
				{
					DirVector.x = ( Enemies[ Enemy ].Object.Pos.x - SecBulls[ i ].Pos.x );
					DirVector.y = ( Enemies[ Enemy ].Object.Pos.y - SecBulls[ i ].Pos.y );
					DirVector.z = ( Enemies[ Enemy ].Object.Pos.z - SecBulls[ i ].Pos.z );
				}
				else
				{
					if( Secondary != (uint16) -1 )
					{
						DirVector.x = ( SecBulls[ Secondary ].Pos.x - SecBulls[ i ].Pos.x );
						DirVector.y = ( SecBulls[ Secondary ].Pos.y - SecBulls[ i ].Pos.y );
						DirVector.z = ( SecBulls[ Secondary ].Pos.z - SecBulls[ i ].Pos.z );
					}
				}
			}

			if( ( Ship != (uint16) -1 ) || ( Enemy != (uint16) -1 ) || ( Secondary != (uint16) -1 ) )
			{
				NormaliseVector( &DirVector );

				Cos = DotProduct( &DirVector, &SecBulls[ i ].DirVector );

				QuatFrom2Vectors( &qlerp.end, &Forward, &DirVector );
   				
				qlerp.start	= SecBulls[ i ].DirQuat;				/* Current Quaternion */
   				qlerp.crnt	= &SecBulls[ i ].DirQuat;				/* Address to store new Quaternion */
				qlerp.dir	= QuatDotProduct( &qlerp.start, &qlerp.end );
   	
				if( Cos < -1.0F ) Cos = -1.0F;
				else if ( Cos > 1.0F ) Cos = 1.0F;
				Angle = (float) acos( Cos );
				if( Angle ) qlerp.time = ( ( D2R( SecBulls[ i ].TurnSpeed ) * framelag ) / Angle );
				else qlerp.time = 1.0F;
				if( qlerp.time > 1.0F ) qlerp.time = 1.0F;
   				
				QuatInterpolate( &qlerp );
				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );
				ApplyMatrix( &SecBulls[ i ].Mat, &Forward, &SecBulls[ i ].DirVector );
				ApplyMatrix( &SecBulls[ i ].Mat, &SlideUp, &SecBulls[ i ].UpVector );

				if( R2D( Angle ) < 1.0F )
				{
					if( ( IsServerGame && IsServer ) ||
						( !IsServerGame && ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( SecBulls[ i ].Owner == WhoIAm ) ) ) ||
						( !IsServerGame && ( ( SecBulls[i].OwnerType != OWNER_SHIP ) && IsHost ) ) )
					{
						BulletID = GetSecondaryBulletID( SecBulls[i].OwnerType, SecBulls[i].Owner );

						if( SecBulls[i].LifeCount > 60.0F )
						{
							DropPos.x = ( SecBulls[i].UpVector.x * ( -128.0F * GLOBAL_SCALE ) );
							DropPos.y = ( SecBulls[i].UpVector.y * ( -128.0F * GLOBAL_SCALE ) );
							DropPos.z = ( SecBulls[i].UpVector.z * ( -128.0F * GLOBAL_SCALE ) );
							DropVector.x = 0.0F;
							DropVector.y = 0.0F;
							DropVector.z = 0.0F;
							InitOneSecBull( SecBulls[i].OwnerType, SecBulls[i].Owner, BulletID,
											SecBulls[i].GroupImIn, &SecBulls[i].Pos, &DropPos,
											&SecBulls[i].DirVector, &SecBulls[i].UpVector, &DropVector, PINEMISSILE, TRUE );
							SecBulls[i].LifeCount = 0.0F;
						}
					}
				}
			}
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		QUANTUM MINE, proximity detonation
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case QUANTUMMINE:
			FrameLag_Rot.x = 0.0F;
			FrameLag_Rot.y = ( 2.0F * framelag );
			FrameLag_Rot.z = 0.0F;
			MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
			QuatMultiply( &SecBulls[ i ].DirQuat, &TempQuat, &SecBulls[ i ].DirQuat );
			QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );				// Create Dir Matrix

			Val = (int16) ( SecBulls[i].LifeCount * 2.5F );
			Val %= 360;
			Height = (float) ( COSD( Val ) * ( 75.0F * GLOBAL_SCALE ) );
			SecBulls[i].Pos.x = SecBulls[i].StartPos.x + ( SecBulls[i].UpVector.x * Height );
			SecBulls[i].Pos.y = SecBulls[i].StartPos.y + ( SecBulls[i].UpVector.y * Height );
			SecBulls[i].Pos.z = SecBulls[i].StartPos.z + ( SecBulls[i].UpVector.z * Height );

			if( SecBulls[i].LifeCount >= 150.0F ) Owner = (uint16) -1;
			else Owner = SecBulls[i].Owner;
	
			if( CheckProximityToShips( &SecBulls[i].Pos, SecBulls[i].GroupImIn, MINE_TRIGGER_RADIUS,
									   SecBulls[i].OwnerType, Owner ) )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( FALSE );
			}
			else
			{
				Radius = MaxColDistance;
				Enemy = GetClosestEnemy( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &Radius, SecBulls[i].OwnerType, Owner );

				if( Enemy != (uint16) -1 )
				{
					if( Radius < ( ( EnemyTypes[ Enemies[ Enemy ].Type ].Radius * 2.0F ) + MINE_TRIGGER_RADIUS ) )
					{
						if( !( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) ||
							( ( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) && ( Radius <= Enemies[ Enemy ].MineDistance ) ) )
						{
							Enemies[ Enemy ].MinePos = SecBulls[i].Pos;
							Enemies[ Enemy ].MineDistance = Radius;
							Enemies[ Enemy ].AIFlags |= AI_MINEISCLOSE;
							Enemies[ Enemy ].MineNum = i;
						}
					}
					if( Radius < MINE_TRIGGER_RADIUS ) DestroySecondary( i, &SecBulls[i].Pos );
				}
			}
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		SPIDER MINE, proximity detonation, Moving
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case SPIDERMINE:
			FrameLag_Rot.x = 0.0F;
			FrameLag_Rot.y = ( 2.0F * framelag );
			FrameLag_Rot.z = 0.0F;
			MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
			QuatMultiply( &SecBulls[ i ].DirQuat, &TempQuat, &SecBulls[ i ].DirQuat );
			QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );				// Create Dir Matrix

			if( SecBulls[i].LifeCount >= 150.0F ) Owner = (uint16) -1;
			else Owner = SecBulls[i].Owner;
	
			if( CheckProximityToShips( &SecBulls[i].Pos, SecBulls[i].GroupImIn, MINE_TRIGGER_RADIUS,
									   SecBulls[i].OwnerType, Owner ) )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( FALSE );
			}
			else
			{
				Radius = MaxColDistance;
				Enemy = GetClosestEnemy( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &Radius, SecBulls[i].OwnerType, Owner );

				if( Enemy != (uint16) -1 )
				{
					if( Radius < ( ( EnemyTypes[ Enemies[ Enemy ].Type ].Radius * 2.0F ) + MINE_TRIGGER_RADIUS ) )
					{
						if( !( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) ||
							( ( Enemies[ Enemy ].AIFlags & AI_MINEISCLOSE ) && ( Radius <= Enemies[ Enemy ].MineDistance ) ) )
						{
							Enemies[ Enemy ].MinePos = SecBulls[i].Pos;
							Enemies[ Enemy ].MineDistance = Radius;
							Enemies[ Enemy ].AIFlags |= AI_MINEISCLOSE;
							Enemies[ Enemy ].MineNum = i;
						}
					}
					if( Radius < MINE_TRIGGER_RADIUS ) DestroySecondary( i, &SecBulls[i].Pos );
				}
			}
			break;
	}
	return( TRUE );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Process All Missiles
	Input		:	uint16		Secondary Weapon Bullet Index
	Output		:	BOOL		NotDestroyed/Destroyed?  ( TRUE/FALSE )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL ProcessMissiles( uint16 i, uint16 * NextMissile )
{
	QUATLERP		qlerp;
	VECTOR			NewPos;
	VECTOR			DirVector;
	VECTOR			TempVector;
	VECTOR			NormVector;
	VECTOR			temp;
	float			Length;
	float			Speed;
	float			Cos;
	float			Angle;
	float			DistToInt;
	float			DistToTarget;
	float			DistFromStart;
	uint16			Light;
	uint16			HitTarget;
	uint16			HitWall;
	uint16			HitSecondary;
	VECTOR			Int_Point;
	VECTOR			Int_Point2;
	VECTOR			MInt_Point;
	VECTOR			MInt_Point2;
	VECTOR			Recoil;
	BGOBJECT	*	BGObject;
	ENEMY		*	HitEnemy;
	VECTOR			EInt_Point;
	VECTOR			EInt_Point2;
	NORMAL			ENormal;
	MATRIX			UpMatrix;
	MATRIX			InvUpMatrix;
	QUAT			TempQuat;
	VECTOR			FrameLag_Rot;
	VECTOR			NewDir;
	float			Damage;
	float			ClosestCos = -1.0F;
	uint16			Ship;
	uint16			Enemy;
	uint16			Secondary;
	uint16			OldGroup;
	float			LifeCount;
	VECTOR			Rotation;
	VECTOR			UpVector;
	VECTOR			TempPoint1;
	VECTOR			TempPoint2;
	uint16			TempGroup;
	uint16			TempEnemyIndex;
	float			NewFramelag;

	NewFramelag = ( framelag + SecBulls[i].FramelagAddition );
	if( SecBulls[i].FramelagAddition != 0.0F )
	{
		SecBulls[i].FramelagAddition = 0.0F;
	}

	*NextMissile = SecBulls[ i ].Prev;

	OldGroup = SecBulls[ i ].GroupImIn;

	switch( SecBulls[ i ].Weapon )
	{
		case ENEMYTENTACLE:
			if( ( SecBulls[i].NumOldPos == MAXTENTOLDPOS ) || ( SecBulls[i].State == MIS_RETRACTTENTACLE ) )
				break;

			SecBulls[i].Time += NewFramelag; //framelag;
			if( SecBulls[i].Time >= SecBulls[i].Interval )
			{
				SecBulls[i].Time -= ( SecBulls[i].Interval * (float) ( (int32) ( SecBulls[i].Time / SecBulls[i].Interval ) ) );
				memcpy( &SecBulls[ i ].OldPos[ 1 ], &SecBulls[ i ].OldPos[ 0 ], ( ( MAXTENTOLDPOS - 1 ) * sizeof( OLDPOS ) ) );
				SecBulls[ i ].OldPos[ 0 ].Pos = SecBulls[ i ].Pos;
				SecBulls[ i ].OldPos[ 0 ].Dir = SecBulls[ i ].DirVector;
				SecBulls[ i ].OldPos[ 0 ].R = 128; //32;
				SecBulls[ i ].OldPos[ 0 ].G = 128; //32;
				SecBulls[ i ].OldPos[ 0 ].B = 128;
				if( SecBulls[ i ].NumOldPos != MAXTENTOLDPOS )
				{
					SecBulls[ i ].NumOldPos++;
					if( SecBulls[ i ].NumOldPos == MAXTENTOLDPOS )
					{
						SecBulls[ i ].State = MIS_RETRACTTENTACLE;
						SecBulls[ i ].RetractPos = 0.0F;

						CreateShockwave( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, SecBulls[i].GroupImIn, TENTACLE_SHOCKWAVE, SecBulls[i].Weapon );
					}
				}
			}
			break;

		case SCATTERMISSILE:
			SecBulls[i].Time += NewFramelag; //framelag;
			if( SecBulls[i].Time >= SecBulls[i].Interval )
			{
				SecBulls[i].Time -= ( SecBulls[i].Interval * (float) ( (int32) ( SecBulls[i].Time / SecBulls[i].Interval ) ) );
				memcpy( &SecBulls[ i ].OldPos[ 1 ], &SecBulls[ i ].OldPos[ 0 ], ( ( MAXOLDPOS - 1 ) * sizeof( OLDPOS ) ) );
				SecBulls[ i ].OldPos[ 0 ].Pos = SecBulls[ i ].Pos;
				SecBulls[ i ].OldPos[ 0 ].Dir = SecBulls[ i ].DirVector;
				SecBulls[ i ].OldPos[ 0 ].R = 128; //32;
				SecBulls[ i ].OldPos[ 0 ].G = 128; //32;
				SecBulls[ i ].OldPos[ 0 ].B = 128;
				if( SecBulls[ i ].NumOldPos != MAXOLDPOS ) SecBulls[ i ].NumOldPos++;
			}
			break;

		case ENEMYSPIRALMISSILE:
		case ENEMYHOMINGMISSILE:
		case ENEMYBLUEHOMINGMISSILE:
		case ENEMYFIREBALL:
		case MUGMISSILE:
		case SOLARISMISSILE:
		case GRAVGONMISSILE:
		case MULTIPLEMISSILE:
		case TITANSTARMISSILE:
		case PINEMISSILE:
			break;

		case THIEFMISSILE:
			if( SecBulls[i].LifeCount > SecBulls[i].LifeSpan )
			{
	 			CreateExplosion( &SecBulls[i].Pos, SecBulls[i].GroupImIn );
	 			CleanUpSecBull( i );
				return( FALSE );
			}
			break;

		case TITANSTARSHRAPNEL:
			if( SecBulls[i].LifeCount > SecBulls[i].LifeSpan )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( FALSE );
			}
			else
			{
				NormVector = SecBulls[i].DirVector;
				NormaliseVector( &NormVector );
				QuatFrom2Vectors( &SecBulls[ i ].DirQuat, &Forward, &NormVector );
				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );
			}
			break;

		case ENEMYDEPTHCHARGE:
			if( SecBulls[i].LifeCount > SecBulls[i].LifeSpan )
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( FALSE );
			}
			else
			{
				FrameLag_Rot.x = 0.0F;
				FrameLag_Rot.y = ( 5.0F * NewFramelag ); //framelag );
				FrameLag_Rot.z = 0.0F;
				MakeQuat( FrameLag_Rot.x, FrameLag_Rot.y, FrameLag_Rot.z, &TempQuat );
				QuatMultiply( &SecBulls[ i ].DirQuat, &TempQuat, &SecBulls[ i ].DirQuat );
				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );				// Create Dir Matrix
			}
			break;

	}

   	switch( SecBulls[ i ].State )
   	{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Missile Dropping ( MFRL )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
   		case MIS_DROP:														/* Dropping */
   			if( SecBulls[ i ].DropCount == 0.0F )
   			{
				SecBulls[ i ].State = MIS_ACCELERATE;						/* Start to accelerate */
				SecBulls[ i ].ColFlag = 0;

   				Light = FindFreeXLight();									/* Generate Light */
   				if( Light != (uint16 ) -1 )
   				{
					if( !SecondaryLightDetail ) XLights[ Light ].Visible = FALSE;
   					XLights[ Light ].Pos = SecBulls[i].Pos;
   					XLights[ Light ].Size = SecondaryWeaponAttribs[ SecBulls[i].Type ].lightsize;
   					XLights[ Light ].SizeCount = 0.0F;
   					XLights[ Light ].r = SecondaryWeaponAttribs[ SecBulls[i].Type ].r;
   					XLights[ Light ].g = SecondaryWeaponAttribs[ SecBulls[i].Type ].g;
   					XLights[ Light ].b = SecondaryWeaponAttribs[ SecBulls[i].Type ].b;
 					XLights[ Light ].Group = SecBulls[i].GroupImIn;
				}
				SecBulls[i].light = Light;
   			}
   			else
   			{
   				SecBulls[ i ].DropCount -= NewFramelag; //framelag;
   				if( SecBulls[ i ].DropCount < 0.0F )
				{
					SecBulls[ i ].FramelagAddition = (float) fabs( SecBulls[i].DropCount );
					SecBulls[ i ].DropCount = 0.0F;
				}
   			}
   			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Missile Accelerating ( MFRL )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
   		case MIS_ACCELERATE:												/* Accelerating */
   			if( SecBulls[ i ].Speed < SecBulls[ i ].SpeedWanted )
   			{
   				SecBulls[ i ].Speed += SecBulls[ i ].SpeedInc;
   				if( SecBulls[ i ].Speed > SecBulls[ i ].SpeedWanted )
   				{
   					SecBulls[ i ].Speed = SecBulls[ i ].SpeedWanted;
					SecBulls[ i ].State = MIS_STRAIGHT;
   				}
   			}
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Missile Flying Staight ( All )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
   		case MIS_STRAIGHT:													/* Straight ( possibly homing missile looking for target ) */
   			if( SecBulls[ i ].MoveType == MISMOVE_HOMING )
   			{
				ClosestCos = -1.0F;
				
#ifdef SINT_PEACEFROG_CHEAT
				TargetingWeaponCheat = SecBulls[i].Weapon;
#endif
				switch( SecBulls[i].OwnerType )
				{
					case OWNER_SHIP:
						Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
						Enemy = TargetClosestEnemy( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												    SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
													&ClosestCos );
						if( Enemy != (uint16) -1 ) Ship = (uint16) -1;
#if 0
						Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
						if( Secondary != (uint16) -1 )
						{
							Ship = (uint16) -1;
							Enemy = (uint16) -1;
						}
#else
						Secondary = (uint16) -1;
#endif
						if( Ship != (uint16) -1 )
						{
							if( !( SecBulls[ i ].Flags & SECFLAGS_NoIncoming ) )
							{
								if( Ship == WhoIAm )
								{
									if( !bSoundEnabled || !BikeCompSpeechSlider.value ) 
										AddMessageToQue( INCOMING_MISSILE );
									else
										PlayPannedSfx( SFX_Incoming, Ships[ WhoIAm ].Object.Group , &Ships[ WhoIAm ].Object.Pos, 0.0F );
								}
							}

							SecBulls[ i ].State = MIS_HOMING;
							SecBulls[ i ].TargetType = OWNER_SHIP;
							SecBulls[ i ].Target = Ship;
						}
						else
						{
							if( Enemy != (uint16) -1 )
							{
								SecBulls[ i ].State = MIS_HOMING;
								SecBulls[ i ].TargetType = OWNER_ENEMY;
								SecBulls[ i ].Target = Enemy;
								Enemies[ Enemy ].AIFlags |= AI_TARGETED;
								Enemies[ Enemy ].TargetingPos = SecBulls[i].Pos;
								Enemies[ Enemy ].TargetingDir = SecBulls[i].DirVector;
							}
							else
							{
								if( Secondary != (uint16) -1 )
								{
									SecBulls[ i ].State = MIS_HOMING;
									SecBulls[ i ].TargetType = OWNER_MINE;
									SecBulls[ i ].Target = Secondary;
								}
							}
						}
						break;

					case OWNER_ENEMY:
						Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
#if 0
						Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
						if( Secondary != (uint16) -1 )
						{
							Ship = (uint16) -1;
						}
#else
						Secondary = (uint16) -1;
#endif
					 
						if( Ship != (uint16) -1 )
						{
							if( !( SecBulls[ i ].Flags & SECFLAGS_NoIncoming ) )
							{
								if( Ship == WhoIAm )
								{
									if( !bSoundEnabled || !BikeCompSpeechSlider.value ) 
										AddMessageToQue( INCOMING_MISSILE );
									else
										PlayPannedSfx( SFX_Incoming, Ships[ WhoIAm ].Object.Group , &Ships[ WhoIAm ].Object.Pos, 0.0F );
								}
							}

							SecBulls[ i ].State = MIS_HOMING;
							SecBulls[ i ].TargetType = OWNER_SHIP;
							SecBulls[ i ].Target = Ship;
						}
						else
						{
							if( Secondary != (uint16) -1 )
							{
								SecBulls[ i ].State = MIS_HOMING;
								SecBulls[ i ].TargetType = OWNER_MINE;
								SecBulls[ i ].Target = Secondary;
							}
						}
						break;

					case OWNER_NOBODY:
						Ship = TargetClosestShip( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
#if 0
						Secondary = TargetClosestSecondary( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn,
												  SecBulls[i].OwnerType, SecBulls[i].Owner, SecBulls[i].ViewCone,
												  &ClosestCos );
						if( Secondary != (uint16) -1 )
						{
							Ship = (uint16) -1;
						}
#else
						Secondary = (uint16) -1;
#endif
					 
						if( Ship != (uint16) -1 )
						{
							if( !( SecBulls[ i ].Flags & SECFLAGS_NoIncoming ) )
							{
								if( Ship == WhoIAm )
								{
									if( !bSoundEnabled || !BikeCompSpeechSlider.value ) 
										AddMessageToQue( INCOMING_MISSILE );
									else
										PlayPannedSfx( SFX_Incoming, Ships[ WhoIAm ].Object.Group , &Ships[ WhoIAm ].Object.Pos, 0.0F );
								}
							}

							SecBulls[ i ].State = MIS_HOMING;
							SecBulls[ i ].TargetType = OWNER_SHIP;
							SecBulls[ i ].Target = Ship;
						}
						else
						{
							if( Secondary != (uint16) -1 )
							{
								SecBulls[ i ].State = MIS_HOMING;
								SecBulls[ i ].TargetType = OWNER_MINE;
								SecBulls[ i ].Target = Secondary;
							}
						}
						break;

					default:
						break;
				}
			}
   			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Missile Homing ( Heatseaker/Scatter/Thief )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
   		case MIS_HOMING:													/* Homing toward target ( could lose target ) */
			if( StillGotTarget( &SecBulls[ i ].Pos, &SecBulls[ i ].DirVector,
								SecBulls[ i ].GroupImIn, SecBulls[i].TargetType, SecBulls[ i ].Target,
								SecBulls[ i ].ViewCone ) )
			{
				switch( SecBulls[i].TargetType )
				{
					case OWNER_SHIP:
		   				NormVector.x = ( Ships[ SecBulls[i].Target ].Object.Pos.x - SecBulls[i].Pos.x );
		   				NormVector.y = ( Ships[ SecBulls[i].Target ].Object.Pos.y - SecBulls[i].Pos.y );
		   				NormVector.z = ( Ships[ SecBulls[i].Target ].Object.Pos.z - SecBulls[i].Pos.z );
						break;

					case OWNER_ENEMY:
						Enemies[ SecBulls[i].Target ].AIFlags |= AI_TARGETED;
						Enemies[ SecBulls[i].Target ].TargetingPos = SecBulls[ i ].Pos;
						Enemies[ SecBulls[i].Target ].TargetingDir = SecBulls[ i ].DirVector;

		   				NormVector.x = ( Enemies[ SecBulls[i].Target ].Object.Pos.x - SecBulls[i].Pos.x );
		   				NormVector.y = ( Enemies[ SecBulls[i].Target ].Object.Pos.y - SecBulls[i].Pos.y );
		   				NormVector.z = ( Enemies[ SecBulls[i].Target ].Object.Pos.z - SecBulls[i].Pos.z );
						break;

					case OWNER_MINE:
		   				NormVector.x = ( SecBulls[ SecBulls[i].Target ].Pos.x - SecBulls[i].Pos.x );
		   				NormVector.y = ( SecBulls[ SecBulls[i].Target ].Pos.y - SecBulls[i].Pos.y );
		   				NormVector.z = ( SecBulls[ SecBulls[i].Target ].Pos.z - SecBulls[i].Pos.z );
						break;
				}

				if( ( SecBulls[i].TargetType == OWNER_SHIP ) && ( SecBulls[i].Target == WhoIAm ) )
				{
					DistToTarget = VectorLength( &NormVector );
					if( DistToTarget < MissileDistance )
					{
						MissileDistance = DistToTarget;
						MissileNum = i;
						ImTargeted = TRUE;
					}
				}

				NormaliseVector( &NormVector );

				Cos = DotProduct( &NormVector, &SecBulls[ i ].DirVector );

				QuatFrom2Vectors( &qlerp.end, &Forward, &NormVector );
   		
   				qlerp.start	= SecBulls[ i ].DirQuat;				/* Current Quaternion */
   				qlerp.crnt	= &SecBulls[ i ].DirQuat;				/* Address to store new Quaternion */
   				qlerp.dir	= QuatDotProduct( &qlerp.start, &qlerp.end );

				if( Cos < -1.0F ) Cos = -1.0F;
				else if ( Cos > 1.0F ) Cos = 1.0F;
   				Angle = (float) acos( Cos );
   				if( Angle ) qlerp.time = ( ( D2R( SecBulls[ i ].TurnSpeed ) * NewFramelag ) / Angle ); //framelag ) / Angle );
   				else qlerp.time = 1.0F;
   				if( qlerp.time > 1.0F ) qlerp.time = 1.0F;
   		
   				QuatInterpolate( &qlerp );
   				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );
				ApplyMatrix( &SecBulls[ i ].Mat, &Forward, &SecBulls[ i ].DirVector );
				ApplyMatrix( &SecBulls[ i ].Mat, &SlideUp, &SecBulls[ i ].UpVector );
			}
			else
			{
   				SecBulls[ i ].State = MIS_STRAIGHT;
   				SecBulls[ i ].Target = (uint16) -1;
				SecBulls[ i ].ColFlag = 0;
			}
   			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Missile gravity
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case MIS_GRAVITY:
			QuatFrom2Vectors( &TempQuat, &Mloadheader.Group[ Models[i].Group ].up, &SlideUp );
			QuatToMatrix( &TempQuat, &UpMatrix );
			MatrixTranspose( &UpMatrix, &InvUpMatrix );

			ApplyMatrix( &UpMatrix, &SecBulls[i].DirVector, &DirVector );

			NormVector.x = DirVector.x;
			NormVector.y = 0.0F;
			NormVector.z = DirVector.z;
			Speed = ( VectorLength( &NormVector ) * 0.995F );
			NormaliseVector( &NormVector );
			DirVector.x = ( NormVector.x * Speed );
			DirVector.z = ( NormVector.z * Speed );
	     	DirVector.y -= ( ( 0.08F * GLOBAL_SCALE ) * NewFramelag ); //framelag );

			ApplyMatrix( &InvUpMatrix, &DirVector, &SecBulls[i].DirVector );
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Should never be in here
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
   		case MIS_SPIRALING:												/* Spiraling missile */
			Angle = (float) D2R( ( ( SecBulls[i].LifeCount * 6.0F ) - ( floor( ( SecBulls[i].LifeCount * 6.0F ) / 360.0F ) * 360.0F ) ) );

			Rotation.x = (float) sin( Angle );
			Rotation.y = (float) cos( Angle );
			Rotation.z = 0.0F;
			ApplyMatrix( &SecBulls[i].StartMat, &Rotation, &UpVector );	// Calc Up Vector

			TempPoint1.x = ( UpVector.x * SPIRAL_RADIUS );
			TempPoint1.y = ( UpVector.y * SPIRAL_RADIUS );
			TempPoint1.z = ( UpVector.z * SPIRAL_RADIUS );

			Angle = (float) D2R( ( ( SecBulls[i].LifeCount * 6.0F ) + 30.0F ) - ( floor( ( ( SecBulls[i].LifeCount * 6.0F ) + 30.0F ) / 360.0F ) * 360.0F ) );

			Rotation.x = (float) sin( Angle );
			Rotation.y = (float) cos( Angle );
			Rotation.z = 0.0F;
			ApplyMatrix( &SecBulls[i].StartMat, &Rotation, &UpVector );	// Calc Up Vector

			TempPoint2.x = ( UpVector.x * SPIRAL_RADIUS ) + ( SecBulls[i].StartDir.x * ( 100.0F * GLOBAL_SCALE ) );
			TempPoint2.y = ( UpVector.y * SPIRAL_RADIUS ) + ( SecBulls[i].StartDir.y * ( 100.0F * GLOBAL_SCALE ) );
			TempPoint2.z = ( UpVector.z * SPIRAL_RADIUS ) + ( SecBulls[i].StartDir.z * ( 100.0F * GLOBAL_SCALE ) );

			TempVector.x = ( TempPoint1.x - TempPoint2.x );
			TempVector.y = ( TempPoint1.y - TempPoint2.y );
			TempVector.z = ( TempPoint1.z - TempPoint2.z );
			NormaliseVector( &TempVector );

			QuatFromVector2( &TempVector, &TempQuat );
			QuatToMatrix( &TempQuat, &SecBulls[i].Mat );
			ApplyMatrix( &SecBulls[ i ].Mat, &Forward, &SecBulls[ i ].DirVector );
			ApplyMatrix( &SecBulls[ i ].Mat, &SlideUp, &SecBulls[ i ].UpVector );
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Retract Tentacle 
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case MIS_RETRACTTENTACLE:
			SecBulls[i].Speed = 0.0F;

			if( SecBulls[ i ].NumOldPos )
			{
				TempVector.x = ( SecBulls[i].OldPos[ (int16) SecBulls[i].RetractPos ].Pos.x - SecBulls[i].Pos.x );
				TempVector.y = ( SecBulls[i].OldPos[ (int16) SecBulls[i].RetractPos ].Pos.y - SecBulls[i].Pos.y );
				TempVector.z = ( SecBulls[i].OldPos[ (int16) SecBulls[i].RetractPos ].Pos.z - SecBulls[i].Pos.z );
				SecBulls[i].GroupImIn = MoveGroup( &Mloadheader, &SecBulls[i].Pos, SecBulls[i].GroupImIn, &TempVector );

				if( OldGroup != SecBulls[ i ].GroupImIn )
				{
					MoveSecBullToGroup( i, OldGroup, SecBulls[ i ].GroupImIn );
					OldGroup = SecBulls[i].GroupImIn;
				}

				SecBulls[i].Pos = SecBulls[i].OldPos[ (int16) SecBulls[i].RetractPos ].Pos;
				SecBulls[i].DirVector = SecBulls[i].OldPos[ (int16) SecBulls[i].RetractPos ].Dir;
				QuatFromDirAndUp( &SecBulls[ i ].DirVector, &SecBulls[ i ].UpVector, &SecBulls[ i ].DirQuat );
				QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );							// Create Dir Matrix
	
				if( (int16) SecBulls[i].RetractPos != ( MAXTENTOLDPOS - 1 ) )
				{
					SecBulls[i].RetractPos += ( 0.5F * NewFramelag ); //framelag );
				}

				if( SecBulls[i].RetractPos > (float) ( SecBulls[ i ].NumOldPos - 1 ) )
				{
					SecBulls[i].RetractPos = (float) ( SecBulls[ i ].NumOldPos - 1 );
					DestroySecondary( i, &SecBulls[i].Pos );
					return( FALSE );
				}
			}
			else
			{
				DestroySecondary( i, &SecBulls[i].Pos );
				return( FALSE );
			}
   			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Should never be in here
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
   		default:															/* Unknown/Undefined Missile */
   			break;
   	}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Do Collision Routines
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	Speed = ( SecBulls[ i ].Speed * NewFramelag ); //framelag );
	
	if( SecBulls[i].State == MIS_DROP )
	{																		/* NewPos when dropping */
		if( Speed > ( SecBulls[ i ].Speed * 2.0F ) ) Speed = ( SecBulls[ i ].Speed * 2.0F );
		NewPos.x = SecBulls[ i ].Pos.x + ( SecBulls[ i ].DropVector.x * Speed );
		NewPos.y = SecBulls[ i ].Pos.y + ( SecBulls[ i ].DropVector.y * Speed );
		NewPos.z = SecBulls[ i ].Pos.z + ( SecBulls[ i ].DropVector.z * Speed );
	}
	else
	{
		NewPos.x = SecBulls[ i ].Pos.x + ( SecBulls[ i ].DirVector.x * Speed );
		NewPos.y = SecBulls[ i ].Pos.y + ( SecBulls[ i ].DirVector.y * Speed );
		NewPos.z = SecBulls[ i ].Pos.z + ( SecBulls[ i ].DirVector.z * Speed );
	}

	DirVector.x = ( NewPos.x - SecBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
	DirVector.y = ( NewPos.y - SecBulls[ i ].Pos.y );
	DirVector.z = ( NewPos.z - SecBulls[ i ].Pos.z );
	
	Length = VectorLength( &DirVector );									/* Length of Vector */ 

	switch( SecBulls[i].OwnerType )
	{
		case OWNER_SHIP:
			TriggerAreaPlayerShootsCheck( &SecBulls[i].Pos, &NewPos, SecBulls[i].GroupImIn, WEPTYPE_Secondary, SecBulls[i].Weapon );
			break;

		case OWNER_ENEMY:
			TriggerAreaEnemyShootsCheck( &SecBulls[i].Pos, &NewPos, SecBulls[i].GroupImIn, WEPTYPE_Secondary, SecBulls[i].Weapon );
			break;
	}

	DistToInt = 10000.0F;									/* Distance to Intersection */
	HitWall = (uint16) -1;

	if( Length )
	{
		if( ( SecBulls[ i ].State == MIS_STRAIGHT ) || ( SecBulls[ i ].State == MIS_ACCELERATE ) )
		{
			if( !SecBulls[i].ColFlag )
			{
				temp.x = ( SecBulls[ i ].DirVector.x * MaxColDistance );
				temp.y = ( SecBulls[ i ].DirVector.y * MaxColDistance );
				temp.z = ( SecBulls[ i ].DirVector.z * MaxColDistance );
		
				if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &SecBulls[i].Pos,
									SecBulls[i].GroupImIn, &temp, (VECTOR *) &SecBulls[i].ColPoint,
									&SecBulls[i].ColGroup, &SecBulls[i].ColPointNormal, &TempVector, FALSE, NULL ) )
				{
					DebugPrintf( "Secondary weapon %d didn't collide with backgroup in group %d\n", SecBulls[i].Weapon, SecBulls[i].GroupImIn );
					if( DebugInfo ) CreateDebugLine( &SecBulls[i].Pos, &temp, SecBulls[i].GroupImIn, 255, 64, 64 );
					CleanUpSecBull( i );
					return( FALSE );
				}
				else
				{
					SecBulls[i].ColFlag = 1;
					SecBulls[i].ColStart = SecBulls[i].Pos;
					SecBulls[i].ColDist = (float) fabs( DistanceVert2Vector( (VERT*) &SecBulls[i].ColPoint, &SecBulls[i].Pos ) );
				}
			}

			DistFromStart = (float) fabs( DistanceVert2Vector( (VERT*) &NewPos, &SecBulls[i].ColStart ) );
	
			if( SecBulls[i].ColDist < DistFromStart )
	   		{
				HitWall = 0;
				DistToInt = (float) fabs( DistanceVert2Vector( (VERT*) &SecBulls[i].ColPoint, &SecBulls[i].Pos ) );
			}

			if( HitWall == (uint16) -1 )
			{
				if( OneGroupBGObjectCol( 0.0F, 0, SecBulls[i].GroupImIn, &SecBulls[i].Pos, &DirVector,
					 (VECTOR *) &SecBulls[i].ColPoint, &SecBulls[i].ColPointNormal, &TempVector, &BGObject, 0.0F ) )
				{
					HitWall = 0;
					DistToInt = (float) fabs( DistanceVert2Vector( (VERT*) &SecBulls[i].ColPoint, &SecBulls[i].Pos ) );

					if( BGObject )
					{
						ChangeBGState( BGObject, SecBulls[i].OwnerType, SecBulls[i].Owner, SHOT, SecBulls[i].Damage );
					}
				}
			}

		}
		else
		{
			if( BackgroundCollide( &MCloadheadert0, &Mloadheader, &SecBulls[i].Pos, SecBulls[i].GroupImIn,
									&DirVector, (VECTOR *) &SecBulls[i].ColPoint, &SecBulls[i].ColGroup, &SecBulls[i].ColPointNormal,
									&TempVector, TRUE, NULL ) == TRUE )
			{
				TempVector.x = ( SecBulls[i].ColPoint.x - SecBulls[i].Pos.x );
				TempVector.y = ( SecBulls[i].ColPoint.y - SecBulls[i].Pos.y );
				TempVector.z = ( SecBulls[i].ColPoint.z - SecBulls[i].Pos.z );
				DistToInt = VectorLength( &TempVector );
				HitWall = 0;
			}
		}

		if ( WaterObjectCollide( SecBulls[i].GroupImIn, &SecBulls[i].Pos, &DirVector, &Int_Point, SecBulls[i].Damage ) )
		{
			CreateSplash( (VECTOR *) &Int_Point, &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
		}
	}

	if( SecBulls[i].ColRadius ) HitTarget = CheckHitShip( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, SecBulls[i].GroupImIn, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &Int_Point, &Int_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Sphere );
	else HitTarget = CheckHitShip( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, SecBulls[i].GroupImIn, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &Int_Point, &Int_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Sphere );

	if( HitTarget != (uint16) -1 ) HitWall = (uint16) -1;

	if( SecBulls[i].ColRadius ) HitSecondary = CheckHitSecondary( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &MInt_Point, &MInt_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Sphere );
	else HitSecondary = CheckHitSecondary( &SecBulls[i].Pos, SecBulls[i].GroupImIn, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &MInt_Point, &MInt_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Point );

	if( HitSecondary != (uint16) -1 )
	{
		HitWall = (uint16) -1;
		HitTarget = (uint16) -1;
	}
	
	if( SecBulls[i].ColRadius )	HitEnemy = CheckHitEnemy( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &EInt_Point, &EInt_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Sphere );
	else HitEnemy = CheckHitEnemy( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, &SecBulls[i].DirVector, &SecBulls[i].UpVector, Length, &EInt_Point, &EInt_Point2, &DistToInt, SecBulls[i].ColRadius, COLTYPE_Point );

	if( HitEnemy != NULL )
	{
		HitWall = (uint16) -1;
		HitTarget = (uint16) -1;
		HitSecondary = (uint16) -1;
	}
	else
	{
		TempEnemyIndex = GetComEnemyHitInfo( &EInt_Point, &ENormal, &DistToInt );

		if( TempEnemyIndex != (uint16) -1 )
		{
			HitEnemy = &Enemies[ TempEnemyIndex ];
			EInt_Point2 = EInt_Point;
			HitWall = (uint16) -1;
			HitTarget = (uint16) -1;
			HitSecondary = (uint16) -1;
		}
	}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Hit Secondary
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	if( HitSecondary != (uint16) -1 )							/* Hit Secondary? */
	{
		SecBulls[ HitSecondary ].Shield -= SecBulls[i].Damage;
		if( SecBulls[ HitSecondary ].Shield < 0.0F ) SecBulls[ HitSecondary ].Shield = 0.0F;

		if( SecBulls[ HitSecondary ].Shield == 0.0F )
		{
			if( *NextMissile == HitSecondary )
			{
				*NextMissile = SecBulls[ HitSecondary ].Prev;
			}

			switch( SecBulls[ HitSecondary ].Weapon )
			{
				case ENEMYSPIRALMISSILE:
				case ENEMYHOMINGMISSILE:
				case ENEMYBLUEHOMINGMISSILE:
				case GRAVGONMISSILE:
				case MUGMISSILE:
				case MULTIPLEMISSILE:
				case SOLARISMISSILE:
				case ENEMYTENTACLE:
				case SCATTERMISSILE:
				default:
					MissileShockWave( (VECTOR *) &MInt_Point, MISSILE_SHOCKWAVE, SecBulls[ HitSecondary ].Owner,
									( SecBulls[ HitSecondary ].Damage / 2.5F ), SecBulls[ HitSecondary ].GroupImIn,
									SecBulls[ HitSecondary ].Weapon );
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case ENEMYFIREBALL:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case ENEMYDEPTHCHARGE:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case THIEFMISSILE:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case PINEMISSILE:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case TITANSTARSHRAPNEL:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case TITANSTARMISSILE:
					TempVector.x = 0.0F;
					TempVector.y = 1.0F;
					TempVector.z = 0.0F;
					CreateExplosionDonut( &SecBulls[ HitSecondary ].Pos, (VECTOR *) &TempVector, SecBulls[ HitSecondary ].GroupImIn );
					if( ( ( SecBulls[ HitSecondary ].OwnerType == OWNER_SHIP ) && ( SecBulls[ HitSecondary ].Owner == WhoIAm ) ) ||
						( ( SecBulls[ HitSecondary ].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
						( ( SecBulls[ HitSecondary ].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
					{
						NewDir.x = ( MInt_Point.x - SecBulls[ HitSecondary ].Pos.x );
						NewDir.y = ( MInt_Point.y - SecBulls[ HitSecondary ].Pos.y );
						NewDir.z = ( MInt_Point.z - SecBulls[ HitSecondary ].Pos.z );
						TempGroup = MoveGroup( &Mloadheader, &SecBulls[ HitSecondary ].Pos, SecBulls[ HitSecondary ].GroupImIn, &NewDir );
						CreateTitanStarShrapnel( HitSecondary, (VECTOR *) &MInt_Point, &SecBulls[ HitSecondary ].DirVector, TempGroup );
					}
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;

				case PURGEMINE:
				case PINEMINE:
				case QUANTUMMINE:
				case SPIDERMINE:
					DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
					break;
			}
		}

		switch( SecBulls[ i ].Weapon )
		{
			case ENEMYSPIRALMISSILE:
			case ENEMYHOMINGMISSILE:
			case ENEMYBLUEHOMINGMISSILE:
			case GRAVGONMISSILE:
			case MUGMISSILE:
			case MULTIPLEMISSILE:
			case SOLARISMISSILE:
			case ENEMYTENTACLE:
			case SCATTERMISSILE:
			default:
				MissileShockWave( (VECTOR *) &MInt_Point, MISSILE_SHOCKWAVE, SecBulls[i].Owner, ( SecBulls[i].Damage / 2.5F ), SecBulls[i].GroupImIn, SecBulls[i].Weapon );
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;

			case ENEMYFIREBALL:
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;

			case ENEMYDEPTHCHARGE:
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;

			case THIEFMISSILE:
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;

			case PURGEMINE:
			case PINEMINE:
			case QUANTUMMINE:
			case SPIDERMINE:
			case PINEMISSILE:
			case TITANSTARSHRAPNEL:
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;

			case TITANSTARMISSILE:
				TempVector.x = 0.0F;
				TempVector.y = 1.0F;
				TempVector.z = 0.0F;
				CreateExplosionDonut( &SecBulls[ i ].Pos, (VECTOR *) &TempVector, SecBulls[ i ].GroupImIn );
				if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( SecBulls[i].Owner == WhoIAm ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
				{
					NewDir.x = ( MInt_Point.x - SecBulls[i].Pos.x );
					NewDir.y = ( MInt_Point.y - SecBulls[i].Pos.y );
					NewDir.z = ( MInt_Point.z - SecBulls[i].Pos.z );
					TempGroup = MoveGroup( &Mloadheader, &SecBulls[i].Pos, SecBulls[i].GroupImIn, &NewDir );
					CreateTitanStarShrapnel( i, (VECTOR *) &MInt_Point, &SecBulls[ i ].DirVector, TempGroup );
				}
				DestroySecondary( i, (VECTOR *) &MInt_Point );
				break;
		}
		return( FALSE );
	}
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Hit Ship
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	if( HitTarget != (uint16) -1 )												/* Hit Target? */
	{
		if( !( IsServerGame && ColPerspective != COLPERS_Forsaken )  )
	 		PlayPannedSfx( SFX_ShipHit, Ships[ HitTarget ].Object.Group , &Int_Point, 0.0F );

		switch( ColPerspective )
		{
			case COLPERS_Server:
				if( IsServer )
				{
					if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) ) || ( SecBulls[i].OwnerType != OWNER_SHIP ) )
   					{
						Server_WhoHitYou = (BYTE)SecBulls[i].Owner;
   						Damage = SecBulls[i].Damage;

   						Recoil.x = ( SecBulls[i].DirVector.x * ( Damage / 10.0F ) );
   						Recoil.y = ( SecBulls[i].DirVector.y * ( Damage / 10.0F ) );
   						Recoil.z = ( SecBulls[i].DirVector.z * ( Damage / 10.0F ) );

						if( SecBulls[i].OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

						if( HitTarget == WhoIAm )
						{
							HitMe( SecBulls[i].OwnerType, SecBulls[i].Owner, Damage, WEPTYPE_Secondary, SecBulls[i].Weapon );
							ForceExternalOneOff( WhoIAm, &Recoil );
						}
						else
						{
	   						IHitYou( (BYTE) HitTarget, Damage, &Recoil, &Int_Point, &SecBulls[i].DirVector, D2R( ( Damage / 10.0F ) ), WEPTYPE_Secondary, SecBulls[i].Weapon, ONEOFF_RECOIL );
						}
   					}
				}
				break;

			case COLPERS_Forsaken:
				if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( WhoIAm == SecBulls[i].Owner ) ) || ( SecBulls[i].OwnerType != OWNER_SHIP ) )
   				{
   					Damage = SecBulls[i].Damage;

   					Recoil.x = ( SecBulls[i].DirVector.x * ( Damage / 10.0F ) );
   					Recoil.y = ( SecBulls[i].DirVector.y * ( Damage / 10.0F ) );
   					Recoil.z = ( SecBulls[i].DirVector.z * ( Damage / 10.0F ) );

					if( SecBulls[i].OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

					if( HitTarget == WhoIAm )
					{
						HitMe( SecBulls[i].OwnerType, SecBulls[i].Owner, Damage, WEPTYPE_Secondary, SecBulls[i].Weapon );
						ForceExternalOneOff( WhoIAm, &Recoil );
					}
					else
					{
	   					IHitYou( (BYTE) HitTarget, Damage, &Recoil, &Int_Point, &SecBulls[i].DirVector, D2R( ( Damage / 10.0F ) ), WEPTYPE_Secondary, SecBulls[i].Weapon, ONEOFF_RECOIL );
					}
   				}
				break;

			case COLPERS_Descent:
				if( HitTarget == WhoIAm )
   				{
   					Damage = SecBulls[i].Damage;

   					Recoil.x = ( SecBulls[i].DirVector.x * ( Damage / 10.0F ) );
   					Recoil.y = ( SecBulls[i].DirVector.y * ( Damage / 10.0F ) );
   					Recoil.z = ( SecBulls[i].DirVector.z * ( Damage / 10.0F ) );

					if( SecBulls[i].OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

					HitMe( SecBulls[i].OwnerType, SecBulls[i].Owner, Damage, WEPTYPE_Secondary, SecBulls[i].Weapon );
					ForceExternalOneOff( WhoIAm, &Recoil );
   				}
				break;
		}

//		if( ( WhoIAm != HitTarget ) || ( Current_Camera_View != WhoIAm ) )
		if( ( ( WhoIAm != HitTarget ) || ( Current_Camera_View != WhoIAm ) ) && !( IsServerGame && ColPerspective != COLPERS_Forsaken ) )
		{
			if( Ships[ HitTarget ].Object.Shield )
			{
				switch( SecBulls[ i ].Weapon )
				{
					case THIEFMISSILE:
						CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 2, 255, 192, 0 );
						break;

					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case ENEMYFIREBALL:
					case ENEMYDEPTHCHARGE:
					case MUGMISSILE:
					case SOLARISMISSILE:
					case ENEMYTENTACLE:
					case SCATTERMISSILE:
					case GRAVGONMISSILE:
					case TITANSTARMISSILE:
					case PINEMISSILE:
					case TITANSTARSHRAPNEL:
						CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 5, 255, 255, 255 );
						break;
					case MULTIPLEMISSILE:
						CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 1, 255, 255, 255 );
						break;
				}
			}
			else
			{
				switch( SecBulls[ i ].Weapon )
				{
					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case ENEMYFIREBALL:
					case ENEMYDEPTHCHARGE:
					case MUGMISSILE:
					case SOLARISMISSILE:
					case THIEFMISSILE:
					case ENEMYTENTACLE:
					case SCATTERMISSILE:
					case GRAVGONMISSILE:
					case TITANSTARMISSILE:
					case PINEMISSILE:
					case MULTIPLEMISSILE:
					case TITANSTARSHRAPNEL:
						TempVector.x = ( Int_Point.x - Ships[ HitTarget ].Object.Pos.x );
						TempVector.y = ( Int_Point.y - Ships[ HitTarget ].Object.Pos.y );
						TempVector.z = ( Int_Point.z - Ships[ HitTarget ].Object.Pos.z );
						NormaliseVector( &TempVector );
						CreateColSparks( &Int_Point, &TempVector, Ships[ HitTarget ].Object.Group );
						break;
				}
			}
		}

		switch( SecBulls[ i ].Weapon )
		{
			case ENEMYSPIRALMISSILE:
			case ENEMYHOMINGMISSILE:
			case ENEMYBLUEHOMINGMISSILE:
			case ENEMYFIREBALL:
			case ENEMYDEPTHCHARGE:
			case MUGMISSILE:
			case SOLARISMISSILE:
			case GRAVGONMISSILE:
			case MULTIPLEMISSILE:
			case PINEMISSILE:
			case TITANSTARSHRAPNEL:
			default:
				DestroySecondary( i, &Int_Point );
				return( FALSE );
				break;

			case TITANSTARMISSILE:
				if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( SecBulls[i].Owner == WhoIAm ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
				{
					NewDir.x = ( Int_Point.x - SecBulls[ i ].Pos.x );
					NewDir.y = ( Int_Point.y - SecBulls[ i ].Pos.y );
					NewDir.z = ( Int_Point.z - SecBulls[ i ].Pos.z );
					TempGroup = MoveGroup( &Mloadheader, &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, &NewDir );
					TempVector.x = ( Int_Point.x - Ships[ HitTarget ].Object.Pos.x );
					TempVector.y = ( Int_Point.y - Ships[ HitTarget ].Object.Pos.y );
					TempVector.z = ( Int_Point.z - Ships[ HitTarget ].Object.Pos.z );
					NormaliseVector( &TempVector );
					CreateTitanStarShrapnel( i, &Int_Point, &TempVector, TempGroup );
				}
				DestroySecondary( i, &Int_Point );
				return( FALSE );
				break;

			case THIEFMISSILE:
				DestroySecondary( i, &Int_Point );
				return( FALSE );
				break;
	
			case ENEMYTENTACLE:
				SecBulls[ i ].State = MIS_RETRACTTENTACLE;
				SecBulls[ i ].RetractPos = 0.0F;
				break;

			case SCATTERMISSILE:
				switch( ColPerspective )
				{
					case COLPERS_Server:
						break;

					case COLPERS_Forsaken:
						break;

					case COLPERS_Descent:
						if( HitTarget == WhoIAm )
						{
							if( !GodMode )
							{
								if( IsServerGame )
								{
									ExplodeShip( WhoIAm );
									ShortScatterWeapons();
								}
								else
								{
									TempVector.x = ( Ships[ SecBulls[ i ].Owner ].Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x );
									TempVector.y = ( Ships[ SecBulls[ i ].Owner ].Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y );
									TempVector.z = ( Ships[ SecBulls[ i ].Owner ].Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z );
									NormaliseVector( &TempVector );
									ScatterWeapons( &TempVector, MAXSCATTERED );

								}
								PlayPannedSfx( SFX_Scattered, Ships[ HitTarget ].Object.Group , &Ships[ HitTarget ].Object.Pos, 0.0F );
								if( !bSoundEnabled ) AddMessageToQue( YOUVE_BEEN_SCATTERED );
							}
						}
						break;
				}
				DestroySecondary( i, &Int_Point );
				return( FALSE );
				break;
		}
	}
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Hit Wall
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	if( HitWall != (uint16) -1 )
	{
		switch( SecBulls[ i ].Weapon )
		{
			case ENEMYSPIRALMISSILE:
			case ENEMYHOMINGMISSILE:
			case ENEMYBLUEHOMINGMISSILE:
			case GRAVGONMISSILE:
			case MUGMISSILE:
			case MULTIPLEMISSILE:
			case SOLARISMISSILE:
			case SCATTERMISSILE:
			default:
				MissileShockWave( (VECTOR *) &SecBulls[i].ColPoint, MISSILE_SHOCKWAVE, SecBulls[i].Owner, ( SecBulls[i].Damage / 2.5F ), SecBulls[i].GroupImIn, SecBulls[i].Weapon );
				DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				return( FALSE );
				break;

			case ENEMYTENTACLE:
				SecBulls[ i ].State = MIS_RETRACTTENTACLE;
				SecBulls[ i ].RetractPos = 0.0F;

				CreateShockwave( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, SecBulls[i].GroupImIn, TENTACLE_SHOCKWAVE, SecBulls[i].Weapon );
				break;

			case ENEMYFIREBALL:
				if( SecBulls[i].NumBounces )
				{
					SecBulls[i].NumBounces--;
					DirVector.x = ( SecBulls[i].ColPoint.x - SecBulls[i].Pos.x );
					DirVector.y = ( SecBulls[i].ColPoint.y - SecBulls[i].Pos.y );
					DirVector.z = ( SecBulls[i].ColPoint.z - SecBulls[i].Pos.z );
					NewPos.x = SecBulls[i].ColPoint.x;
					NewPos.y = SecBulls[i].ColPoint.y;
					NewPos.z = SecBulls[i].ColPoint.z;
       				TempVector = SecBulls[i].DirVector;
       				ReflectVector( &TempVector, &SecBulls[i].ColPointNormal, &SecBulls[i].DirVector );
					SecBulls[i].ColFlag = 0;
				}
				else
				{
					DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				}
				return( FALSE );
				break;

			case ENEMYDEPTHCHARGE:
				DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				return( FALSE );
				break;

			case THIEFMISSILE:
				DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				return( FALSE );
				break;

			case PINEMISSILE:
				DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				return( FALSE );
				break;

			case TITANSTARSHRAPNEL:
				if( SecBulls[i].LifeCount < 15.0F )
				{
					DirVector.x = ( SecBulls[i].ColPoint.x - SecBulls[i].Pos.x );
					DirVector.y = ( SecBulls[i].ColPoint.y - SecBulls[i].Pos.y );
					DirVector.z = ( SecBulls[i].ColPoint.z - SecBulls[i].Pos.z );
					NewPos.x = SecBulls[i].ColPoint.x;
					NewPos.y = SecBulls[i].ColPoint.y;
					NewPos.z = SecBulls[i].ColPoint.z;
        			TempVector = SecBulls[i].DirVector;
        			ReflectVector( &TempVector, &SecBulls[i].ColPointNormal, &SecBulls[i].DirVector );
					SecBulls[i].ColFlag = 0;
				}
				else
				{
					DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
					return( FALSE );
				}
				break;

			case TITANSTARMISSILE:
       			TempVector = SecBulls[i].DirVector;
       			ReflectVector( &TempVector, &SecBulls[i].ColPointNormal, &TempVector );

				CreateExplosionDonut( &SecBulls[ i ].Pos, (VECTOR *) &SecBulls[ i ].ColPointNormal, SecBulls[ i ].GroupImIn );
				if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( SecBulls[i].Owner == WhoIAm ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
				{
					NewDir.x = ( SecBulls[ i ].ColPoint.x - SecBulls[ i ].Pos.x );
					NewDir.y = ( SecBulls[ i ].ColPoint.y - SecBulls[ i ].Pos.y );
					NewDir.z = ( SecBulls[ i ].ColPoint.z - SecBulls[ i ].Pos.z );
					TempGroup = MoveGroup( &Mloadheader, &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, &NewDir );
					CreateTitanStarShrapnel( i, (VECTOR *) &SecBulls[ i ].ColPoint, (VECTOR *) &TempVector, TempGroup );
				}
				DestroySecondary( i, (VECTOR *) &SecBulls[i].ColPoint );
				return( FALSE );
				break;
		}
	}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Hit Enemy
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	if( HitEnemy != NULL )
	{
		TempVector.x = ( EInt_Point.x - HitEnemy->Object.Pos.x );
		TempVector.y = ( EInt_Point.y - HitEnemy->Object.Pos.y );
		TempVector.z = ( EInt_Point.z - HitEnemy->Object.Pos.z );
		NormaliseVector( &TempVector );
	
  		ReflectVector( &SecBulls[i].DirVector, (NORMAL *) &TempVector, &NewDir );

		if( !( HitEnemy->Object.Flags & SHIP_Invul ) )
		{
			if( DamageEnemy( HitEnemy , -SecBulls[i].Damage , &EInt_Point , &SecBulls[i].DirVector , SecBulls[ i ].Speed , SecBulls[ i ].Owner , SecBulls[ i ].OwnerType ) )
			{
				CreateNewExplosion( &EInt_Point, &NewDir, SecBulls[i].GroupImIn );
				KillUsedEnemy( HitEnemy );
			}
			else
			{
				CreateColSparks( &EInt_Point, &TempVector, HitEnemy->Object.Group );
			}
		}
		else
		{
			if( !Random_Range( 4 ) )
			{
				PlayPannedSfx( SFX_Ric, SecBulls[i].GroupImIn, (VECTOR *) &EInt_Point, 0.0F );
			}
		}
	
		switch( SecBulls[ i ].Weapon )
		{
			case ENEMYSPIRALMISSILE:
			case ENEMYHOMINGMISSILE:
			case ENEMYBLUEHOMINGMISSILE:
			case GRAVGONMISSILE:
			case MUGMISSILE:
			case MULTIPLEMISSILE:
			case SOLARISMISSILE:
			default:
				MissileShockWave( &EInt_Point, MISSILE_SHOCKWAVE, SecBulls[i].Owner, ( SecBulls[i].Damage / 2.5F ), SecBulls[i].GroupImIn, SecBulls[i].Weapon );
				DestroySecondary( i, &EInt_Point );
				return( FALSE );
				break;

			case ENEMYFIREBALL:
				DestroySecondary( i, &EInt_Point );
				return( FALSE );
				break;

			case ENEMYDEPTHCHARGE:
				DestroySecondary( i, &EInt_Point );
				return( FALSE );
				break;

			case ENEMYTENTACLE:
				SecBulls[ i ].State = MIS_RETRACTTENTACLE;
				SecBulls[ i ].RetractPos = 0.0F;
				break;

			case SCATTERMISSILE:
				if( HitEnemy->PickupHeld != (uint16) -1 )
				{
					if( !( HitEnemy->Object.Flags & SHIP_Scattered ) )
					{
	   					TempVector.x = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   					TempVector.y = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   					TempVector.z = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
						NormaliseVector( &TempVector );
						if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
						else LifeCount = -1.0F;
						Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
						InitOnePickup( &HitEnemy->Object.Pos, HitEnemy->Object.Group, &TempVector, Speed, HitEnemy->PickupHeld, (uint16) -1, ++Ships[WhoIAm].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
						if( IsHost ) DropPickupSend( &HitEnemy->Object.Pos, HitEnemy->Object.Group, &TempVector, 0.0F, HitEnemy->PickupHeld, ++Ships[WhoIAm].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
						if( HitEnemy->PickupHeld <= LASER ) HitEnemy->Object.Flags |= SHIP_Scattered;
						HitEnemy->PickupHeld = (uint16) -1;
					}
				}
				else
				{
	   				TempVector.x = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   				TempVector.y = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   				TempVector.z = ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
					NormaliseVector( &TempVector );
					if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
					else LifeCount = -1.0F;
					Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
					InitOnePickup( &HitEnemy->Object.Pos, HitEnemy->Object.Group, &TempVector, Speed, PICKUP_Shield, (uint16) -1, ++Ships[WhoIAm].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
					if( IsHost ) DropPickupSend( &HitEnemy->Object.Pos, HitEnemy->Object.Group, &TempVector, 0.0F, PICKUP_Shield, ++Ships[WhoIAm].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				}
				MissileShockWave( &EInt_Point, MISSILE_SHOCKWAVE, SecBulls[i].Owner, ( SecBulls[i].Damage / 2.5F ), SecBulls[i].GroupImIn, SecBulls[i].Weapon );
				DestroySecondary( i, &EInt_Point );
				return( FALSE );
				break;

			case THIEFMISSILE:
				DestroySecondary( i, &EInt_Point );
				return( FALSE );
				break;

			case PINEMISSILE:
				DestroySecondary( i, &EInt_Point );
				return( FALSE );
				break;

			case TITANSTARSHRAPNEL:
				DestroySecondary( i, &EInt_Point );
				return( FALSE );
				break;

			case TITANSTARMISSILE:
				CreateExplosionDonut( &SecBulls[ i ].Pos, &TempVector, SecBulls[ i ].GroupImIn );
				if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( SecBulls[i].Owner == WhoIAm ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
					( ( SecBulls[i].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
				{
					NewDir.x = ( EInt_Point.x - SecBulls[ i ].Pos.x );
					NewDir.y = ( EInt_Point.y - SecBulls[ i ].Pos.y );
					NewDir.z = ( EInt_Point.z - SecBulls[ i ].Pos.z );
					TempGroup = MoveGroup( &Mloadheader, &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, &NewDir );
					CreateTitanStarShrapnel( i, &EInt_Point, &NewDir, TempGroup );
				}
				DestroySecondary( i, &EInt_Point );
				return( FALSE );
				break;
		}
	}

	SecBulls[i].GroupImIn = MoveGroup( &Mloadheader, &SecBulls[i].Pos, SecBulls[i].GroupImIn, &DirVector );
	SecBulls[i].Pos = NewPos;

	if( OldGroup != SecBulls[ i ].GroupImIn )
	{
		MoveSecBullToGroup( i, OldGroup, SecBulls[ i ].GroupImIn );
	}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Do Trails
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	if( IsGroupVisible[ SecBulls[i].GroupImIn ] )
	{
		if( ( SecBulls[i].State != MIS_DROP ) &&
			( ( SecBulls[i].Speed != 0.0F ) || ( SecBulls[i].State == MIS_RETRACTTENTACLE ) ) )	/* No Smoke Trail when Dropping or Stationary */
		{
			if( !InWater( SecBulls[i].GroupImIn, &SecBulls[i].Pos, &Damage ) )
			{
				switch( SecBulls[i].Weapon )
				{
					case ENEMYTENTACLE:
						CreateTentacle( i );
						break;

					case SCATTERMISSILE:
						CreateScatterTrail( i );
						break;
			
					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case ENEMYFIREBALL:
					case ENEMYDEPTHCHARGE:
					case GRAVGONMISSILE:
					case SOLARISMISSILE:
					case THIEFMISSILE:
					case MUGMISSILE:
					case MULTIPLEMISSILE:
					case PINEMISSILE:
					case TITANSTARMISSILE:
					case TITANSTARSHRAPNEL:
			
						SecBulls[i].Time += NewFramelag; //framelag;
						if( SecBulls[i].Time >= SecBulls[i].Interval )
						{
						 	SecBulls[i].Time = FMOD( SecBulls[i].Time, SecBulls[i].Interval );
				
							switch( SecBulls[i].Weapon )
							{
								case GRAVGONMISSILE:
									CreateGravgonTrail( (VECTOR *) &SecBulls[i].Pos, (VECTOR *) &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;
					
								case SOLARISMISSILE:
									CreateSolarisTrail( (VECTOR *) &SecBulls[i].Pos, (VECTOR *) &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;
					
								case ENEMYTENTACLE:
								case SCATTERMISSILE:
								case ENEMYHOMINGMISSILE:
								case ENEMYBLUEHOMINGMISSILE:
									break;
					
								case THIEFMISSILE:
									CreateNewTrail( (VECTOR *) &SecBulls[i].Pos, (VECTOR *) &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;
					
								case ENEMYSPIRALMISSILE:
								case MUGMISSILE:
								case MULTIPLEMISSILE:
								case PINEMISSILE:
								case TITANSTARMISSILE:
									CreateTrail( (VECTOR *) &SecBulls[i].Pos, (VECTOR *) &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;
					
								case TITANSTARSHRAPNEL:
									CreateNewTrail( (VECTOR *) &SecBulls[i].Pos, (VECTOR *) &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;

								case ENEMYFIREBALL:
									CreateFireBall( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
									break;

								case ENEMYDEPTHCHARGE:
									break;
							}
						}

						break;
				}
			}
			else
			{
				switch( SecBulls[i].Weapon )
				{
					case ENEMYTENTACLE:
						CreateTentacle( i );
						break;

					case SCATTERMISSILE:
						CreateScatterTrail( i );
						break;
			
					case ENEMYSPIRALMISSILE:
					case ENEMYHOMINGMISSILE:
					case ENEMYBLUEHOMINGMISSILE:
					case GRAVGONMISSILE:
					case SOLARISMISSILE:
					case THIEFMISSILE:
					case MUGMISSILE:
					case MULTIPLEMISSILE:
					case PINEMISSILE:
					case TITANSTARMISSILE:
					case TITANSTARSHRAPNEL:
					case ENEMYDEPTHCHARGE:
			
						SecBulls[i].Time += NewFramelag; //framelag;
						if( SecBulls[i].Time >= SecBulls[i].Interval )
						{
						 	SecBulls[i].Time = FMOD( SecBulls[i].Time, SecBulls[i].Interval );
				
							TempVector.x = ( SecBulls[i].Pos.x + ( -SecBulls[i].DirVector.x * ( 200.0F * GLOBAL_SCALE ) ) );
							TempVector.y = ( SecBulls[i].Pos.y + ( -SecBulls[i].DirVector.y * ( 200.0F * GLOBAL_SCALE ) ) );
							TempVector.z = ( SecBulls[i].Pos.z + ( -SecBulls[i].DirVector.z * ( 200.0F * GLOBAL_SCALE ) ) );

							switch( SecBulls[i].Weapon )
							{
								case GRAVGONMISSILE:
								case SOLARISMISSILE:
								case THIEFMISSILE:
								case ENEMYSPIRALMISSILE:
								case ENEMYHOMINGMISSILE:
								case ENEMYBLUEHOMINGMISSILE:
								case ENEMYDEPTHCHARGE:
								case MUGMISSILE:
								case MULTIPLEMISSILE:
								case PINEMISSILE:
								case TITANSTARMISSILE:
								case TITANSTARSHRAPNEL:
									CreateBubbleTrail( &TempVector, &SecBulls[i].DirVector, SecBulls[i].GroupImIn, 64, 64, 64 );
									break;
							}
						}
						break;

					case ENEMYFIREBALL:
						CreateSpotFXBurning( &SecBulls[i].Pos, &SecBulls[i].DirVector, SecBulls[i].GroupImIn );
						break;

					default:
						break;
				}
			}
		}
	}

	return( TRUE );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateExplosion( VECTOR * Pos, uint16 Group )
{
	uint16	fmpoly;
	int16	Count;
	float	Scale;

	if( IsGroupVisible[ Group ] )
	{
		for( Count = 0; Count < ( Random_Range( 3 ) + 1 ); Count++ )
		{
			fmpoly = FindFreeFmPoly();
	
			if( fmpoly != (uint16 ) -1 )
			{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
				FmPolys[ fmpoly ].Pos = *Pos;
				FmPolys[ fmpoly ].SeqNum = FM_ANIM;
				FmPolys[ fmpoly ].Frame = 0.0F;
				FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
				FmPolys[ fmpoly ].R = 192;
				FmPolys[ fmpoly ].G = 192;
				FmPolys[ fmpoly ].B = 192;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
				FmPolys[ fmpoly ].Dir.x = 0.0F;
				FmPolys[ fmpoly ].Dir.y = 0.0F;
				FmPolys[ fmpoly ].Dir.z = 0.0F;
				FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Group = Group;
	
				if( Count == 0 )
				{
					FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
					FmPolys[ fmpoly ].xsize = NORM_EXPLO_SIZE;
					FmPolys[ fmpoly ].ysize = NORM_EXPLO_SIZE;
				}
				else
				{
					FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
					FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
					FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
					FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
					Scale = (float) ( Random_Range( 10 ) + 6 ) * GLOBAL_SCALE;
					FmPolys[ fmpoly ].xsize = Scale;
					FmPolys[ fmpoly ].ysize = Scale;
				}
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
			else
			{
				return;
			}
		}
	}

	PlayPannedSfx( SFX_BangBang, Group , Pos, 0.0F );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Small Explosion
	Input		:	VECTOR	*	Position
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateSmallExplosion( VECTOR * Pos, uint16 Group )
{
	uint16	fmpoly;
	float	Scale;

	if( IsGroupVisible[ Group ] )
	{
		fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (uint16 ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].SeqNum = FM_ANIM;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
			FmPolys[ fmpoly ].R = 192;
			FmPolys[ fmpoly ].G = 192;
			FmPolys[ fmpoly ].B = 192;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
			FmPolys[ fmpoly ].Dir.x = 0.0F;
			FmPolys[ fmpoly ].Dir.y = 0.0F;
			FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Group = Group;
			FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			Scale = (float) ( Random_Range( 10 ) + 6 ) * GLOBAL_SCALE;
			FmPolys[ fmpoly ].xsize = Scale;
			FmPolys[ fmpoly ].ysize = Scale;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}

	PlayPannedSfx( SFX_BangBang, Group , Pos, 0.0F );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Big Explosion
	Input		:	VECTOR	*	Position
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateBigExplosion( VECTOR * Pos, uint16 Group )
{
	int16	Count;
	int16	NumExplosions;
	uint16	fmpoly;
	float	Scale;

	if( IsGroupVisible[ Group ] )
	{
		NumExplosions = ( Random_Range( 3 ) + 1 );

		for( Count = 0; Count < NumExplosions; Count++ )
		{
			fmpoly = FindFreeFmPoly();
		
			if( fmpoly != (uint16 ) -1 )
			{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
				FmPolys[ fmpoly ].Pos = *Pos;
				FmPolys[ fmpoly ].SeqNum = FM_ANIM;
				FmPolys[ fmpoly ].Frame = 0.0F;
				FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
				FmPolys[ fmpoly ].R = 192;
				FmPolys[ fmpoly ].G = 192;
				FmPolys[ fmpoly ].B = 192;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
				FmPolys[ fmpoly ].Dir.x = 0.0F;
				FmPolys[ fmpoly ].Dir.y = 0.0F;
				FmPolys[ fmpoly ].Dir.z = 0.0F;
				FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Group = Group;
				FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
				Scale = (float) ( Random_Range( 20 ) + 12 ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].xsize = Scale;
				FmPolys[ fmpoly ].ysize = Scale;
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Single Random sized Explosion
	Input		:	VECTOR	*	Position
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateSingleExplosion( VECTOR * Pos, uint16 Group )
{
	uint16	fmpoly;
	float	Scale;

	if( IsGroupVisible[ Group ] )
	{
		fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (uint16 ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
			FmPolys[ fmpoly ].SeqNum = FM_ANIM;
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].R = 192;
			FmPolys[ fmpoly ].G = 192;
			FmPolys[ fmpoly ].B = 192;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
			FmPolys[ fmpoly ].Dir.x = 0.0F;
			FmPolys[ fmpoly ].Dir.y = 0.0F;
			FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Group = Group;
			FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			Scale = ( ( (float) ( Random_Range( 12 ) + 4 ) ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].xsize = Scale;
			FmPolys[ fmpoly ].ysize = Scale;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateShipExplosion( VECTOR * Pos, uint16 Group )
{
	uint16	fmpoly;

	if( IsGroupVisible[ Group ] )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = *Pos;
	   		FmPolys[ fmpoly ].SeqNum = FM_ANIM2;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &Exp2_Header;
	   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
			FmPolys[ fmpoly ].Trans = 255;
	   		FmPolys[ fmpoly ].Dir.x = 0.0F;
	   		FmPolys[ fmpoly ].Dir.y = 0.0F;
	   		FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].R = 255;
			FmPolys[ fmpoly ].G = 255;
			FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
	   		FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = SHIP_EXPLO_SIZE;
			FmPolys[ fmpoly ].ysize = SHIP_EXPLO_SIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}

	PlayPannedSfx( SFX_BangBang, Group , Pos, 0.0F );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (uint16 ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos.x = ( Pos->x + ( -Dir->x * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.y = ( Pos->y + ( -Dir->y * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.z = ( Pos->z + ( -Dir->z * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].SeqNum = FM_SMOKE_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = SMOKE_TRAIL_SIZE;
		FmPolys[ fmpoly ].ysize = SMOKE_TRAIL_SIZE;
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( MISSILE_SPEED * 0.8F );
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateBloodTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;

	fmpoly = FindFreeFmPoly();
	
   	if( fmpoly != (uint16 ) -1 )
   	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
   		FmPolys[ fmpoly ].Pos = *Pos;
   		FmPolys[ fmpoly ].Dir = *Dir;
   		FmPolys[ fmpoly ].SeqNum = FM_BLOOD;
   		FmPolys[ fmpoly ].Frame = 0.0F;
   		FmPolys[ fmpoly ].Frm_Info = &Blood_Header; //&Bits_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
  		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 10240.0F ) - 0.25F );
   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 10240.0F ) - 0.25F );
   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 10240.0F ) - 0.25F );
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].R = 192;
		FmPolys[ fmpoly ].G = 192;
		FmPolys[ fmpoly ].B = 192;
   		FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 2.0F ) * GLOBAL_SCALE;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = BLOODSIZE;
		FmPolys[ fmpoly ].ysize = BLOODSIZE;
		FmPolys[ fmpoly ].Group = Group;
		NormaliseVector( &FmPolys[ fmpoly ].Dir );
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Blood splat
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateBloodSplat( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	VECTOR	TempUp;
	VECTOR	TempVector;
	QUAT	TempQuat;
	MATRIX	TempMat;
	uint16	fmpoly;
	float	Angle;

	if( IsGroupVisible[ Group ] )
	{
		Angle = DotProduct( Dir, &SlideUp );
	
	   	fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (uint16 ) -1 )
		{
			QuatFrom2Vectors( &TempQuat, &Forward, Dir );
		   	QuatToMatrix( &TempQuat, &TempMat );
		   	ApplyMatrix( &TempMat, &Mloadheader.Group[ Group ].up, &TempUp );
		
		   	FmPolys[ fmpoly ].LifeCount = 48.0F;
		  	FmPolys[ fmpoly ].Pos.x = Pos->x + ( Dir->x * ( 8.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.y = Pos->y + ( Dir->y * ( 8.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.z = Pos->z + ( Dir->z * ( 8.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Frame = 0.0F;
	
			if( ( Angle <= 0.9F ) && ( Angle >= -0.9F ) )
			{
			  	FmPolys[ fmpoly ].Frm_Info = &BloodDribble_Header;
			  	FmPolys[ fmpoly ].SeqNum = FM_BLOODDRIBBLE;
				CrossProduct( Dir, &Mloadheader.Group[ Group ].up, &TempVector );	/* Calc Temp Left Vector */
				CrossProduct( &TempVector, Dir, &TempUp );						/* Calc Up Vector */
			}
			else
			{
			  	FmPolys[ fmpoly ].Frm_Info = &BloodSplat_Header;
			  	FmPolys[ fmpoly ].SeqNum = FM_BLOODSPLAT;
			}
	
		  	FmPolys[ fmpoly ].Flags = ( FM_FLAG_DIRCONST | FM_FLAG_TWOSIDED );
		   	FmPolys[ fmpoly ].Trans = 255;
		  	FmPolys[ fmpoly ].Dir.x = 0.0F;
		  	FmPolys[ fmpoly ].Dir.y = 0.0F;
		  	FmPolys[ fmpoly ].Dir.z = 0.0F;
		  	FmPolys[ fmpoly ].DirVector = *Dir;
		  	FmPolys[ fmpoly ].UpVector = TempUp;
		   	FmPolys[ fmpoly ].R = 192; //255;
		   	FmPolys[ fmpoly ].G = 50; //255;
		   	FmPolys[ fmpoly ].B = 50; //255;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
		  	FmPolys[ fmpoly ].Speed = 0.0F;
		   	FmPolys[ fmpoly ].Rot = 0.0F;
		   	FmPolys[ fmpoly ].RotSpeed = 0.0F;
		   	FmPolys[ fmpoly ].xsize = BLOODSIZE * 2.0F;
		   	FmPolys[ fmpoly ].ysize = BLOODSIZE * 2.0F;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Smoke Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateGravgonTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (uint16 ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos.x = ( Pos->x + ( -Dir->x * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.y = ( Pos->y + ( -Dir->y * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.z = ( Pos->z + ( -Dir->z * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].SeqNum = FM_GRAVGON_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &GravgonTrail_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = GRAVGON_TRAIL_SIZE;
		FmPolys[ fmpoly ].ysize = GRAVGON_TRAIL_SIZE;
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( MISSILE_SPEED * 0.8F );
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Smoke Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateSolarisTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (uint16 ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos.x = ( Pos->x + ( -Dir->x * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.y = ( Pos->y + ( -Dir->y * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.z = ( Pos->z + ( -Dir->z * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].SeqNum = FM_SOLARIS_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &SolarisTrail_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = SOLARIS_TRAIL_SIZE;
		FmPolys[ fmpoly ].ysize = SOLARIS_TRAIL_SIZE;
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( MISSILE_SPEED * 0.8F );
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create New Smoke Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateNewTrail( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (uint16 ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos.x = ( Pos->x + ( -Dir->x * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.y = ( Pos->y + ( -Dir->y * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].Pos.z = ( Pos->z + ( -Dir->z * ( 200.0F * GLOBAL_SCALE ) ) );
		FmPolys[ fmpoly ].SeqNum = FM_NEW_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &NewTrail_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = NEW_TRAIL_SIZE;
		FmPolys[ fmpoly ].ysize = NEW_TRAIL_SIZE;
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( MISSILE_SPEED * 0.8F );
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Init Link List
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitLinkList( void )
{
	int16		Count;

	FirstFree = &EntryList[ 0 ];													/* First Free = Entry[ 0 ] */
	FirstUsed = NULL;																/* None used */

	for( Count = 0; Count < MAX_ENTRYS; Count++ )
	{
		EntryList[ Count ].Prev = &EntryList[ ( Count - 1 ) ];						/* Prev Free = Prev Entry */
		EntryList[ Count ].Next = &EntryList[ ( Count + 1 ) ];						/* Next Free = Next Entry */
	}																				
																					
	EntryList[ 0 ].Prev = NULL;														/* Start of List */
	EntryList[ ( MAX_ENTRYS - 1 ) ].Next = NULL;									/* End of List */
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Find Free Entry
	Input		:	nothing
	Output		:	ENTRY	*	Entry Address
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
ENTRY * FindFreeLink( void )
{
	ENTRY	*	NextFree;

	if( FirstFree == NULL ) return( NULL );											/* None free! Return NULL */
	NextFree = FirstFree->Next;														/* Get Next Free */

	if( FirstUsed != NULL ) FirstUsed->Prev = FirstFree;							/* Prev Used = First Free */
	FirstFree->Next = FirstUsed;													/* Next Used = First Used */
	FirstFree->Prev = NULL;															/* Prev Used = NULL */
	FirstUsed = FirstFree;															/* First Used = First Free */

	FirstFree = NextFree;															/* First Free = Next Free */
	if( FirstFree != NULL ) FirstFree->Prev = NULL;									/* Prev Free = NULL */
	
	return( FirstUsed );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Free Entry
	Input		:	ENTRY	*	Entry Address
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void FreeLink( ENTRY * Entry )
{
	ENTRY	*	PrevUsed;
	ENTRY	*	NextUsed;

	if( Entry == FirstUsed ) FirstUsed = Entry->Next;								/* If Firstused then next becomes first */

	PrevUsed = Entry->Prev;
	NextUsed = Entry->Next;
	if( PrevUsed != NULL ) PrevUsed->Next = NextUsed;								/* Cut out link */
	if( NextUsed != NULL ) NextUsed->Prev = PrevUsed;

	Entry->Next = FirstFree;														/* Next Free = First Free */
	Entry->Prev = NULL;																/* Prev Free = NULL */
	if( FirstFree != NULL ) FirstFree->Prev = Entry;								/* Next Prev Free = Entry */
	FirstFree = Entry;																/* First Free = Entry */
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display Sec Ammo Level
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DispSecAmmoLevels( void )
{
	int16	x, y;
	int16	Ammo;

	x = (d3dappi.szClient.cx / 2 ) + ( 16 * 4 );
	y = d3dappi.szClient.cy - ( d3dappi.szClient.cy / 5 ) + 6;

	Ammo = SecondaryAmmo[ Ships[ WhoIAm ].Secondary ];

	if( Ammo ) Printuint16( Ammo, x - ( 4 * 4 ), y, 2 );
	else RightJustifyPrint4x5Text( "NO AMMO", x, y, 2 );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Gravgon Missile Effect
	Input		:	uint16	SecBull Index
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateGravgonField( uint16 i )
{
	uint16	Model;

	Model =	FindFreeModel();
	if( Model != (uint16 ) -1 )
	{
		Models[ Model ].OwnerType = SecBulls[i].OwnerType;
		Models[ Model ].Owner = SecBulls[i].Owner;
		Models[ Model ].ModelNum = MODEL_Sphere;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].Visible = TRUE;
		Models[ Model ].Pos.x = SecBulls[i].ColPoint.x;
		Models[ Model ].Pos.y = SecBulls[i].ColPoint.y;
		Models[ Model ].Pos.z = SecBulls[i].ColPoint.z;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_Scale;
		Models[ Model ].MaxScale = 10.0F;
		Models[ Model ].Scale = Models[ Model ].MaxScale;
		Models[ Model ].Group = SecBulls[i].GroupImIn;
		Models[ Model ].LifeCount = 600.0F;
	}
	
	Model =	FindFreeModel();
	if( Model != (uint16 ) -1 )
	{
		Models[ Model ].OwnerType = SecBulls[i].OwnerType;
		Models[ Model ].Owner = SecBulls[i].Owner;
		Models[ Model ].ModelNum = MODEL_Sphere;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].Visible = TRUE;
		Models[ Model ].Pos.x = SecBulls[i].ColPoint.x;
		Models[ Model ].Pos.y = SecBulls[i].ColPoint.y;
		Models[ Model ].Pos.z = SecBulls[i].ColPoint.z;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_Scale2;
		Models[ Model ].MaxScale = 10.0F;
		Models[ Model ].Scale = ( Models[ Model ].MaxScale / 2.0F );
		Models[ Model ].Group = SecBulls[i].GroupImIn;
		Models[ Model ].LifeCount = 600.0F;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Explosion Dome
	Input		:	uint16	OwnerType
				:	uint16	Owner
				:	VECTOR	Pos
				:	uint16	Group
				:	float	Shockwave Size
				:	BYTE	Weapon
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateShockwave( uint16 OwnerType, uint16 Owner, VECTOR * Pos, uint16 Group, float ShockwaveSize, BYTE Weapon )
{
	uint16	Model;

	Model =	FindFreeModel();
	if( Model != (uint16 ) -1 )
	{
		Models[ Model ].OwnerType = OwnerType;
		Models[ Model ].Owner = Owner;
		Models[ Model ].ModelNum = MODEL_Sphere;
		Models[ Model ].Type = MODTYPE_Field;
		Models[ Model ].Flags = MODFLAG_Nothing;
		Models[ Model ].SecWeapon = Weapon;
		Models[ Model ].Visible = TRUE;
		Models[ Model ].Pos.x = Pos->x;
		Models[ Model ].Pos.y = Pos->y;
		Models[ Model ].Pos.z = Pos->z;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_Explode;
		Models[ Model ].Scale = 0.0F;
		Models[ Model ].MaxScale = ShockwaveSize;
		Models[ Model ].Group = Group;
		Models[ Model ].LifeCount = -1.0F;
	}

	CreateBigFlash( Pos, Group, ShockwaveSize );
	CreateExplosion( Pos, Group );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Explode Secondary Weapon
	Input		:	VECTOR	*	Pos
				:	uint16		Group
				:	uint16		Owner
				:	uint16		ID
				:	float		ShockwaveSize
	Output		:	int16		Number of generated pickups
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ExplodeSecondary( VECTOR * Pos, uint16 Group, uint16 OwnerType, uint16 Owner, uint16 ID, float ShockwaveSize )
{
	uint16	i;
	uint16	Next;

	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( ( SecBulls[ i ].OwnerType == OwnerType ) && ( SecBulls[ i ].Owner == Owner ) && ( SecBulls[ i ].ID == ID ) )
		{
			if( ShockwaveSize )
			{
				SecBulls[i].ColPoint.x = SecBulls[i].Pos.x;
				SecBulls[i].ColPoint.y = SecBulls[i].Pos.y;
				SecBulls[i].ColPoint.z = SecBulls[i].Pos.z;
				CreateShockwave( SecBulls[i].OwnerType, SecBulls[i].Owner, &SecBulls[i].Pos, SecBulls[i].GroupImIn, ShockwaveSize, SecBulls[i].Weapon );
			}

			if( IsServerGame && IsServer )
			{
				if( ( SecBulls[ i ].SecType == SEC_MINE ) &&
					( SecBulls[ i ].OwnerType == OWNER_SHIP ) )
				{
					Host_SecAmmoUsed[ SecBulls[i].Weapon ]++;
				}
			}
			else
			{
				if( ( SecBulls[ i ].SecType == SEC_MINE ) &&
					( SecBulls[ i ].OwnerType == OWNER_SHIP ) &&
					( SecBulls[ i ].Owner == WhoIAm ) )
				{
					SecAmmoUsed[ SecBulls[i].Weapon ]++;
				}
			}

 			CleanUpSecBull( i );
			return;
		}

		i = Next;
	}																				
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Kill all mines/missiles owned by specific person
	Input		:	uint16		Owner
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void KillOwnersSecBulls( uint16 Owner )
{
	uint16	i;
	uint16	Next;

	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( ( SecBulls[ i ].OwnerType == OWNER_SHIP ) && ( SecBulls[ i ].Owner == Owner ) )
		{
			if( ( SecBulls[i].SecType == SEC_MINE ) && ( IsHost ) )	SecAmmoUsed[ SecBulls[i].Weapon ]++;
			DestroySecondary( i, &SecBulls[ i ].Pos );
		}

		i = Next;
	}																				
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Scatter all weapons and powerups
	Input		:	VECTOR	*	Genereal Direction to scatter
				:				weapons ( Normalised )
				:	int16		Max Weapons allowed to throw out.
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ScatterWeapons( VECTOR * Dir, int16 MaxPickupsAllowed )
{
	int16	Count;
	int8	PickupNum;
	VECTOR	DirVector;
	int16	NumSecWeapons;
	int16	TotalPickups = 0;
	float	LifeCount;
	float	Speed;
	uint16	i;
	int		team;

	RegenDelay = ( 5.0F * ANIM_SECOND );

	if( IsServerGame ) Msg( "Should not be calling ScatterWeapons!!!" );

	if( MyGameStatus == STATUS_SinglePlayer ) MaxPickupsAllowed = MAXPICKUPS;

	if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
	{

/* new code */
/* this code will cause golden to be taken away and regenerate somewhere else */

		/* take away our golden abilities
		   and add golden to the global regen list */
		LoseSuperNashram();

		/* up the count of total pickups dropped */
		TotalPickups++;

/* end new code */

/*
    This code will drop the golden
    But as stands objects don't remember their own values
	This should be an option once objects remember their own values
	So leave this code here for later
*/

/* 

        // take away golden and add to regen list
		LoseSuperNashram();

		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );

		NormaliseVector( &DirVector );

		PickupNum = PICKUP_GoldenPowerPod;

		if( MyGameStatus != STATUS_SinglePlayer )
		  LifeCount = (float) ( Random_Range( 600 ) + 600 );
		else
		  LifeCount = -1.0F;

		Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) +
		        ( PICKUP_SPEED / 2.0F );

		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group,
		                   &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount,
						   -1, FALSE, LifeCount, (uint16) -1 );

		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector,
			                Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount,
							(uint16) -1 );
			TotalPickups++;
		}

		// take golden off regen list
		// cause we just spit it out
		if( !GodMode )
		{
			NumSuperNashrams--;
		}

*/
	}

	if ( BountyHunt && Ships[ WhoIAm ].Object.Flags & SHIP_CarryingBounty )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Bounty;
		LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			TotalPickups++;
		}
		PickupsGot[ PICKUP_Bounty ] = 0;
		Ships[ WhoIAm ].Object.Flags &= ~SHIP_CarryingBounty;
	}

	if ( CaptureTheFlag && Ships[ WhoIAm ].Object.Flags & SHIP_CarryingFlag )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Flag;
		LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			TotalPickups++;
		}
		PickupsGot[ PICKUP_Flag ] = 0;
		Ships[ WhoIAm ].Object.Flags &= ~SHIP_CarryingFlag;
	}

	if ( CTF && Ships[ WhoIAm ].Object.Flags & SHIP_CarryingFlags )
	{
		for ( team = 0; team < MAX_TEAMS; team++ )
		{
			if ( Ships[ WhoIAm ].Object.Flags & TeamFlagMask[ team ] )
			{
				DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = TeamFlagPickup[ team ];
				LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
				{
					DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
					TotalPickups++;
					PickupsGot[ TeamFlagPickup[ team ] ] = 0;
					Ships[ WhoIAm ].Object.Flags &= ~TeamFlagMask[ team ];
				}
				else
				{
					DebugPrintf( "Unable to scatter %s flag\n", TeamName[ team ] );
				}
			}
		}
	}

	for( Count = 1; Count < MAXPRIMARYWEAPONS; Count++ )
	{
		if( PrimaryWeaponsGot[ Count ] && ( TotalPickups < MaxPickupsAllowed ) )
		{
	   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
			NormaliseVector( &DirVector );
			PickupNum = ( PICKUP_Trojax + ( Count - 1 ) );
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
			{
				DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				TotalPickups++;
			}
			PrimaryWeaponsGot[ Count ] = 0;
		}
	}

	while( Ships[ WhoIAm ].Object.PowerLevel )
	{
		if( TotalPickups < MaxPickupsAllowed )
		{
			if( Ships[ WhoIAm ].Object.PowerLevel >= 3 )
			{
				Ships[ WhoIAm ].Object.PowerLevel -= 3;
		   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = PICKUP_GoldenPowerPod;
				if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
				else LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
				{
					DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
					TotalPickups++;
				}
			}
			else
			{
				Ships[ WhoIAm ].Object.PowerLevel--;
		   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = PICKUP_PowerPod;
				if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
				else LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
				{
					DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
					TotalPickups++;
				}
			}
		}
	}

	Ships[ WhoIAm ].Object.PowerLevel = 0;

	if( TotalPickups < MaxPickupsAllowed )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Shield;
		if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
		else LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			TotalPickups++;
		}
	}

	NumSecWeapons = GenerateSecondaryPickups( (int16) ( MaxPickupsAllowed - TotalPickups ) );

	for( Count = 0; Count < NumSecWeapons; Count++ )
	{
   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = SecWeaponsGot[ Count ];
		if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
		else LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			TotalPickups++;
		}
	}

	if( NitroFuel )
	{
		if( MyGameStatus == STATUS_SinglePlayer )
		{
			while( NitroFuel )
			{
				if( TotalPickups < MaxPickupsAllowed )
				{
					DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					NormaliseVector( &DirVector );
					PickupNum = PICKUP_Nitro;
					if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
					else LifeCount = -1.0F;
					Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
					i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
					if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
					{
						DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
						TotalPickups++;
					}
					NitroFuel -= 50.0F;
					if( NitroFuel < 0.0F ) NitroFuel = 0.0F;
				}
				else
				{
					NitroFuelUsed += NitroFuel;
					NitroFuel = 0.0F;
				}
			}
		}
		else
		{
			NitroFuelUsed += NitroFuel;
			NitroFuel = 0.0F;
		}
	}

	if(	( Ships[ WhoIAm ].Object.Flags & SHIP_Stealth ) )
	{
		Ships[ WhoIAm ].Object.Flags &= ~SHIP_Stealth;
		Ships[ WhoIAm ].StealthTime = 0.0F;

		if( TotalPickups < MaxPickupsAllowed )
		{
	   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
			NormaliseVector( &DirVector );
			PickupNum = PICKUP_Mantle;
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
			{
				DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				TotalPickups++;
			}
		}
		else
		{
			NumStealths++;
		}
	}

	if(	Ships[ WhoIAm ].Invul && PickupInvulnerability )
	{
		Ships[ WhoIAm ].Invul = FALSE;
		Ships[ WhoIAm ].InvulTimer = 0.0F;
		NumInvuls++;
		PickupInvulnerability = FALSE;
	}

	if(	( Ships[ WhoIAm ].NumMultiples ) )
	{
		for( Count = 0; Count < Ships[ WhoIAm ].NumMultiples; Count++ )
		{
			if( TotalPickups < MaxPickupsAllowed )
			{
		   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = PICKUP_Orb;
				if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
				else LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
				{
					DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
					TotalPickups++;
				}
			}
			else
			{
				NumOrbs++;
			}

			if( Ships[ WhoIAm ].OrbModels[ Count ] != (uint16) -1 )
			{
				KillUsedModel( Ships[ WhoIAm ].OrbModels[ Count ] );
				Ships[ WhoIAm ].OrbModels[ Count ] = (uint16) -1;
			}
		}

		Ships[ WhoIAm ].NumMultiples = 0;
	}

	Ships[ WhoIAm ].Primary = PULSAR;
	Ships[ WhoIAm ].Secondary = MUGMISSILE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Scatter all weapons and powerups
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ShortScatterWeapons( void )
{
	int16	Count;

	if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) ) LoseSuperNashram();

	Ships[ WhoIAm ].Object.PowerLevel = 0;
	NitroFuel = 0.0F;
	Ships[ WhoIAm ].Object.Flags &= ~( SHIP_Stealth | SHIP_CarryingBounty | SHIP_CarryingFlag | SHIP_CarryingFlags );
	Ships[ WhoIAm ].StealthTime = 0.0F;
	Ships[ WhoIAm ].Invul = FALSE;
	Ships[ WhoIAm ].InvulTimer = 0.0F;

	if(	( Ships[ WhoIAm ].NumMultiples ) )
	{
		for( Count = 0; Count < Ships[ WhoIAm ].NumMultiples; Count++ )
		{
			if( Ships[ WhoIAm ].OrbModels[ Count ] != (uint16) -1 )
			{
				KillUsedModel( Ships[ WhoIAm ].OrbModels[ Count ] );
				Ships[ WhoIAm ].OrbModels[ Count ] = (uint16) -1;
			}
		}

		Ships[ WhoIAm ].NumMultiples = 0;
	}

	RestoreAmmo();
	RestoreWeapons();

	Ships[ WhoIAm ].Primary = PULSAR;
	Ships[ WhoIAm ].Secondary = MUGMISSILE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Scatter all weapons and powerups for specific
				:	Ship
	Input		:	VECTOR	*	Genereal Direction to scatter
				:				weapons ( Normalised )
				:	int16		Max Weapons allowed to throw out.
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ScatterWeaponsForShip( uint16 Ship, VECTOR * Dir, int16 MaxPickupsAllowed )
{
	int16	Count;
	int8	PickupNum;
	VECTOR	DirVector;
	int16	NumSecWeapons;
	int16	TotalPickups = 0;
	float	LifeCount;
	float	Speed;
	uint16	i;
	int		team;
	VECTOR	Pos;
	uint16	Group;

	Pos = Ships[ Ship ].RealPos;
	Group = Ships[ Ship ].RealGroup;

	if( MyGameStatus == STATUS_SinglePlayer ) MaxPickupsAllowed = MAXPICKUPS;

//	if( ( Ships[ Ship ].Object.Flags & SHIP_SuperNashram ) ) HostLoseSuperNashram( Ship );
	if( Ships[ Ship ].SuperNashramTimer != 0.0F ) HostLoseSuperNashram( Ship );

	if ( BountyHunt && ( Host_Flags[ Ship ] & SHIP_CarryingBounty ) )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Bounty;
		LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			Pickups[ i ].ActualOwner = Ship;
			DropPickupSend( &Pos, Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			TotalPickups++;
		}
		Host_PickupsGot[ Ship ][ PICKUP_Bounty ] = 0;
		Host_Flags[ Ship ] &= ~SHIP_CarryingBounty;
	}

	if ( CaptureTheFlag && ( Host_Flags[ Ship ] & SHIP_CarryingFlag ) )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Flag;
		LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			Pickups[ i ].ActualOwner = Ship;
			DropPickupSend( &Pos, Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			TotalPickups++;
		}
		Host_PickupsGot[ Ship ][ PICKUP_Flag ] = 0;
		Host_Flags[ Ship ] &= ~SHIP_CarryingFlag;
	}

	if ( CTF && ( Host_Flags[ Ship ] & SHIP_CarryingFlags ) )
	{
		for ( team = 0; team < MAX_TEAMS; team++ )
		{
			if ( Host_Flags[ Ship ] & TeamFlagMask[ team ] )
			{
				DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = TeamFlagPickup[ team ];
				LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
				{
					Pickups[ i ].ActualOwner = Ship;
					DropPickupSend( &Pos, Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
					TotalPickups++;
					Host_PickupsGot[ Ship ][ TeamFlagPickup[ team ] ] = 0;
					Host_Flags[ Ship ] &= ~TeamFlagMask[ team ];
				}
				else
				{
					DebugPrintf( "Unable to scatter %s flag from %s\n", TeamName[ team ], Names[ Ship ] );
				}
			}
		}
	}

	for( Count = 1; Count < MAXPRIMARYWEAPONS; Count++ )
	{
		if( Host_PrimaryWeaponsGot[ Ship ][ Count ] && ( TotalPickups < MaxPickupsAllowed ) )
		{
	   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
			NormaliseVector( &DirVector );
			PickupNum = ( PICKUP_Trojax + ( Count - 1 ) );
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
			{
				Pickups[ i ].ActualOwner = Ship;
				DropPickupSend( &Pos, Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				Host_PrimaryWeaponsGot[ Ship ][ Count ] = 0;
				TotalPickups++;
			}
			else
			{
				Msg( "Failed to init primary weapon pickup1" );
			}
		}
	}

	while( Host_PowerLevel[ Ship ] )
	{
		if( TotalPickups < MaxPickupsAllowed )
		{
			Host_PowerLevel[ Ship ]--;
		   	DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   	DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   	DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
			NormaliseVector( &DirVector );
			PickupNum = PICKUP_PowerPod;
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
			{
				Pickups[ i ].ActualOwner = Ship;
				DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				TotalPickups++;
			}
			else
			{
				Msg( "Failed to init powerpod pickup" );
			}
		}
	}

	Host_PowerLevel[ Ship ] = 0;

	if( TotalPickups < MaxPickupsAllowed )
	{
		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = PICKUP_Shield;
		if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
		else LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			Pickups[ i ].ActualOwner = Ship;
			DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			TotalPickups++;
		}
		else
		{
			Msg( "Failed to init shield pickup" );
		}
	}

	NumSecWeapons = HostGenerateSecondaryPickups( Ship, (int16) ( MaxPickupsAllowed - TotalPickups ) );

	for( Count = 0; Count < NumSecWeapons; Count++ )
	{
   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		NormaliseVector( &DirVector );
		PickupNum = Host_SecWeaponsGot[ Ship ][ Count ];
		if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
		else LifeCount = -1.0F;
		Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
		i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			Pickups[ i ].ActualOwner = Ship;
			DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			TotalPickups++;
		}
		else
		{
			Msg( "Failed to init secondary weapon pickup" );	
		}
	}

	if( Host_NitroFuel[ Ship ] )
	{
		if( MyGameStatus == STATUS_SinglePlayer )
		{
			while( Host_NitroFuel[ Ship ] )
			{
				if( TotalPickups < MaxPickupsAllowed )
				{
					DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
					NormaliseVector( &DirVector );
					PickupNum = PICKUP_Nitro;
					if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
					else LifeCount = -1.0F;
					Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
					i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
					if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
					{
						Pickups[ i ].ActualOwner = Ship;
						DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
						TotalPickups++;
					}
					Host_NitroFuel[ Ship ] -= 50.0F;
					if( Host_NitroFuel[ Ship ] < 0.0F ) Host_NitroFuel[ Ship ] = 0.0F;
				}
				else
				{
					Host_NitroFuelUsed += Host_NitroFuel[ Ship ];
					Host_NitroFuel[ Ship ] = 0.0F;
				}
			}
		}
		else
		{
			Host_NitroFuelUsed += Host_NitroFuel[ Ship ];
			Host_NitroFuel[ Ship ] = 0.0F;
		}
	}

//	if(	( Ships[ Ship ].Object.Flags & SHIP_Stealth ) )
	if(	Ships[ Ship ].StealthTime != 0.0F )
	{
//		Ships[ Ship ].Object.Flags &= ~SHIP_Stealth;
		Ships[ Ship ].StealthTime = 0.0F;

		if( TotalPickups < MaxPickupsAllowed )
		{
	   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
	   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
			NormaliseVector( &DirVector );
			PickupNum = PICKUP_Mantle;
			if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
			else LifeCount = -1.0F;
			Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
			i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
			{
				Pickups[ i ].ActualOwner = Ship;
				DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				TotalPickups++;
			}
		}
		else
		{
			NumStealths++;
		}
	}

//	if(	Ships[ Ship ].Invul && PickupInvulnerability )
	if(	Ships[ Ship ].InvulTimer != 0.0F )
	{
		Ships[ Ship ].Invul = FALSE;
		Ships[ Ship ].InvulTimer = 0.0F;
		NumInvuls++;
		PickupInvulnerability = FALSE;
	}

	if(	( Host_NumOfOrbitals[ Ship ] ) )
	{
		for( Count = 0; Count < Host_NumOfOrbitals[ Ship ]; Count++ )
		{
			if( TotalPickups < MaxPickupsAllowed )
			{
		   		DirVector.x = ( Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.y = ( Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
		   		DirVector.z = ( Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F ) );
				NormaliseVector( &DirVector );
				PickupNum = PICKUP_Orb;
				if( MyGameStatus != STATUS_SinglePlayer ) LifeCount = (float) ( Random_Range( 600 ) + 600 );
				else LifeCount = -1.0F;
				Speed = ( (float) Random_Range( (uint16) ( PICKUP_SPEED / 2.0F ) ) ) + ( PICKUP_SPEED / 2.0F );
				i = InitOnePickup( &Pos, Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
				if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
				{
					Pickups[ i ].ActualOwner = Ship;
					DropPickupSend( &Pos, Group, &DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
					TotalPickups++;
				}
			}
			else
			{
				NumOrbs++;
			}
		}

		Host_NumOfOrbitals[ Ship ] = 0;
	}

	Ships[ Ship ].Primary = PULSAR;
	Ships[ Ship ].Secondary = MUGMISSILE;

	HostRestoreWeapons( Ship );
	HostRestoreAmmo( Ship );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create List of Pickups from secondary weapons
	Input		:	Nothing
	Output		:	int16		Number of generated pickups
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int16 GenerateSecondaryPickups( int16 NumAllowed )
{
	int16	NumSecWeapons = 0;
	int16	Count = 0;

	while( SecondaryAmmo[ QUANTUMMINE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_QuantumPickup;
			SecondaryAmmo[ QUANTUMMINE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ QUANTUMMINE ] += SecondaryAmmo[ QUANTUMMINE ];
			SecondaryAmmo[ QUANTUMMINE ] = 0;
		}
	}

	while( SecondaryAmmo[ SPIDERMINE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_SpiderPod;
			SecondaryAmmo[ SPIDERMINE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ SPIDERMINE ] += SecondaryAmmo[ SPIDERMINE ];
			SecondaryAmmo[ SPIDERMINE ] = 0;
		}
	}

	while( SecondaryAmmo[ GRAVGONMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_Gravgon;
			SecondaryAmmo[ GRAVGONMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ GRAVGONMISSILE ] += SecondaryAmmo[ GRAVGONMISSILE ];
			SecondaryAmmo[ GRAVGONMISSILE ]	= 0;
		}
	}

	while( SecondaryAmmo[ TITANSTARMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_TitanStar;
			SecondaryAmmo[ TITANSTARMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ TITANSTARMISSILE ] += SecondaryAmmo[ TITANSTARMISSILE ];
			SecondaryAmmo[ TITANSTARMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ SCATTERMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_Scatter;
			SecondaryAmmo[ SCATTERMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ SCATTERMISSILE ] += SecondaryAmmo[ SCATTERMISSILE ];
			SecondaryAmmo[ SCATTERMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ THIEFMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_Thief;
			SecondaryAmmo[ THIEFMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ THIEFMISSILE ] += SecondaryAmmo[ THIEFMISSILE ];
			SecondaryAmmo[ THIEFMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ MULTIPLEMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( SecondaryAmmo[ MULTIPLEMISSILE ] >= 50 ) )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_Launcher;
			SecondaryAmmo[ MULTIPLEMISSILE ] -= 50;
			if( SecondaryAmmo[ MULTIPLEMISSILE ] < 0 ) SecondaryAmmo[ MULTIPLEMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ MULTIPLEMISSILE ] += SecondaryAmmo[ MULTIPLEMISSILE ];
			SecondaryAmmo[ MULTIPLEMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ SOLARISMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( SecondaryAmmo[ SOLARISMISSILE ] >= 3 ) )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_HeatseakerPickup;
			SecondaryAmmo[ SOLARISMISSILE ] -= 3;
			if( SecondaryAmmo[ SOLARISMISSILE ] < 0 ) SecondaryAmmo[ SOLARISMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ SOLARISMISSILE ] += SecondaryAmmo[ SOLARISMISSILE ];
			SecondaryAmmo[ SOLARISMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ PINEMINE ] )
	{
		if( ( NumAllowed > 0 ) && ( SecondaryAmmo[ PINEMINE ] >= 3 ) )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_PinePickup;
			SecondaryAmmo[ PINEMINE ] -= 3;
			if( SecondaryAmmo[ PINEMINE ] < 0 ) SecondaryAmmo[ PINEMINE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ PINEMINE ] += SecondaryAmmo[ PINEMINE ];
			SecondaryAmmo[ PINEMINE ] = 0;
		}
	}

	while( SecondaryAmmo[ MUGMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( SecondaryAmmo[ MUGMISSILE ] >= 3 ) )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_Mugs;
			SecondaryAmmo[ MUGMISSILE ] -= 3;
			if( SecondaryAmmo[ MUGMISSILE ] < 0 ) SecondaryAmmo[ MUGMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ MUGMISSILE ] += SecondaryAmmo[ MUGMISSILE ];
			SecondaryAmmo[ MUGMISSILE ] = 0;
		}
	}

	while( SecondaryAmmo[ PURGEMINE ] )
	{
		if( ( NumAllowed > 0 ) && ( SecondaryAmmo[ PURGEMINE ] >= 3 ) )
		{
			SecWeaponsGot[ NumSecWeapons ] = PICKUP_PurgePickup;
			SecondaryAmmo[ PURGEMINE ] -= 3;
			if( SecondaryAmmo[ PURGEMINE ] < 0 ) SecondaryAmmo[ PURGEMINE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			SecAmmoUsed[ PURGEMINE ] += SecondaryAmmo[ PURGEMINE ];
			SecondaryAmmo[ PURGEMINE ] = 0;
		}
	}

	for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
	{
		SecondaryWeaponsGot[ Count ] = 0;
	}
	return NumSecWeapons;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create List of Pickups from secondary weapons
				:	for specific ship
	Input		:	Nothing
	Output		:	int16		Number of generated pickups
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int16 HostGenerateSecondaryPickups( uint16 Ship, int16 NumAllowed )
{
	int16	NumSecWeapons = 0;
	int16	Count = 0;

	while( Host_SecondaryAmmo[ Ship ][ QUANTUMMINE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_QuantumPickup;
			Host_SecondaryAmmo[ Ship ][ QUANTUMMINE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ QUANTUMMINE ] += Host_SecondaryAmmo[ Ship ][ QUANTUMMINE ];
			Host_SecondaryAmmo[ Ship ][ QUANTUMMINE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ SPIDERMINE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_SpiderPod;
			Host_SecondaryAmmo[ Ship ][ SPIDERMINE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ SPIDERMINE ] += Host_SecondaryAmmo[ Ship ][ SPIDERMINE ];
			Host_SecondaryAmmo[ Ship ][ SPIDERMINE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ GRAVGONMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_Gravgon;
			Host_SecondaryAmmo[ Ship ][ GRAVGONMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ GRAVGONMISSILE ] += Host_SecondaryAmmo[ Ship ][ GRAVGONMISSILE ];
			Host_SecondaryAmmo[ Ship ][ GRAVGONMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ TITANSTARMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_TitanStar;
			Host_SecondaryAmmo[ Ship ][ TITANSTARMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ TITANSTARMISSILE ] += Host_SecondaryAmmo[ Ship ][ TITANSTARMISSILE ];
			Host_SecondaryAmmo[ Ship ][ TITANSTARMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ SCATTERMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_Scatter;
			Host_SecondaryAmmo[ Ship ][ SCATTERMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ SCATTERMISSILE ] += Host_SecondaryAmmo[ Ship ][ SCATTERMISSILE ];
			Host_SecondaryAmmo[ Ship ][ SCATTERMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ THIEFMISSILE ] )
	{
		if( NumAllowed > 0 )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_Thief;
			Host_SecondaryAmmo[ Ship ][ THIEFMISSILE ]--;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ THIEFMISSILE ] += Host_SecondaryAmmo[ Ship ][ THIEFMISSILE ];
			Host_SecondaryAmmo[ Ship ][ THIEFMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] >= 50 ) )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_Launcher;
			Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] -= 50;
			if( Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] <= 0 ) Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ MULTIPLEMISSILE ] += Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ];
			Host_SecondaryAmmo[ Ship ][ MULTIPLEMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] >= 3 ) )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_HeatseakerPickup;
			Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] -= 3;
			if( Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] <= 0 ) Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ SOLARISMISSILE ] += Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ];
			Host_SecondaryAmmo[ Ship ][ SOLARISMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ PINEMINE ] )
	{
		if( ( NumAllowed > 0 ) && ( Host_SecondaryAmmo[ Ship ][ PINEMINE ] >= 3 ) )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_PinePickup;
			Host_SecondaryAmmo[ Ship ][ PINEMINE ] -= 3;
			if( Host_SecondaryAmmo[ Ship ][ PINEMINE ] <= 0 ) Host_SecondaryAmmo[ Ship ][ PINEMINE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ PINEMINE ] += Host_SecondaryAmmo[ Ship ][ PINEMINE ];
			Host_SecondaryAmmo[ Ship ][ PINEMINE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] )
	{
		if( ( NumAllowed > 0 ) && ( Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] >= 3 ) )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_Mugs;
			Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] -= 3;
			if( Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] <= 0 ) Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ MUGMISSILE ] += Host_SecondaryAmmo[ Ship ][ MUGMISSILE ];
			Host_SecondaryAmmo[ Ship ][ MUGMISSILE ] = 0;
		}
	}

	while( Host_SecondaryAmmo[ Ship ][ PURGEMINE ] )
	{
		if( ( NumAllowed > 0 ) && ( Host_SecondaryAmmo[ Ship ][ PURGEMINE ] >= 3 ) )
		{
			Host_SecWeaponsGot[ Ship ][ NumSecWeapons ] = PICKUP_PurgePickup;
			Host_SecondaryAmmo[ Ship ][ PURGEMINE ] -= 3;
			if( Host_SecondaryAmmo[ Ship ][ PURGEMINE ] <= 0 ) Host_SecondaryAmmo[ Ship ][ PURGEMINE ] = 0;
			NumSecWeapons++;
			NumAllowed--;
		}
		else
		{
			Host_SecAmmoUsed[ PURGEMINE ] += Host_SecondaryAmmo[ Ship ][ PURGEMINE ];
			Host_SecondaryAmmo[ Ship ][ PURGEMINE ] = 0;
		}
	}

	for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
	{
		Host_SecondaryWeaponsGot[ Ship ][ Count ] = 0;
		Host_SecondaryAmmo[ Ship ][ Count ] = 0;
	}
	return NumSecWeapons;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get Current Secondary Ammo Level
	Input		:	Nothing
	Output		:	int16	Ammo Number
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int16 GetCurSecAmmo( void )
{
	return( SecondaryAmmo[ Ships[ WhoIAm ].Secondary ] );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Ammo level control
	Input		:	Check ammo levels and do appropriate changed
	Output		:	void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SecondarySelectControl( void )
{
	int16	Count;
	uint16	CurrentSecType, NewSecType;

	CurrentSecType = SecondaryWeaponAttribs[ Ships[ WhoIAm ].Secondary ].SecType;

	if( player_config->secondary_priority[ Ships[ WhoIAm ].Secondary ] != -1 )
	{
		if( !SecondaryAmmo[ Ships[ WhoIAm ].Secondary ] )
		{
			for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
			{
				if( player_config->secondary_order[ Count ] != -1 )
				{
					if( SecondaryWeaponsGot[ player_config->secondary_order[ Count ] ] )
					{
						if( SecondaryAmmo[ player_config->secondary_order[ Count ] ] )
						{
							NewSecType = SecondaryWeaponAttribs[ player_config->secondary_order[ Count ] ].SecType;
							if( CurrentSecType == NewSecType )
							{
								Ships[ WhoIAm ].Secondary = (BYTE) player_config->secondary_order[ Count ];
								return;
							}
						}
					}
				}
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Drop best mine you have
	Input		:	Nothing
	Output		:	uint16	Mine ( -1 None )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 GetBestMine( void )
{
	uint16	Mine = (uint16) -1;
	int16	Count = 0;
	uint16	Weapon;
	uint16	Type;

	Type = SecondaryWeaponAttribs[ Ships[ WhoIAm ].Secondary ].SecType;
	
	if( ( Type == SEC_MINE ) && ( SecondaryAmmo[ Ships[ WhoIAm ].Secondary ] ) )
	{
		Mine = Ships[ WhoIAm ].Secondary;
	}
	else
	{
		while( ( Count < MAXSECONDARYWEAPONS ) && ( Mine == (uint16) -1 ) )
		{
			Weapon = player_config->secondary_order[ Count ];

			if( Weapon != (uint16) -1 )
			{
				Type = SecondaryWeaponAttribs[ Weapon ].SecType;
			
				if( Type == SEC_MINE )
				{
					if( SecondaryAmmo[ Weapon ] )
					{
						Mine = Weapon;
					}
				}
			}
			Count++;
		}
	}

	return( Mine );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Drop best mine you have
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DropMine( void )
{
	uint16	Mine;
	VECTOR	UpVector;
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	DropVector;

	Mine = GetBestMine();

	if( Mine == (uint16) -1 ) return;

	ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &Dir );
	ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideUp, &UpVector );		/* Calc Up Vector */

	switch( Mine )
	{

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Drop Behind
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case PURGEMINE:
			DropVector.x = 0.0F;
			DropVector.y = 0.0F;
			DropVector.z = -1.0F;
		
			ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
							Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
							&Pos, &Dir, &UpVector, &DropVector, (BYTE) Mine, TRUE );
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Drop Behind
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case PINEMINE:
			DropVector.x = 0.0F;
			DropVector.y = 0.0F;
			DropVector.z = -1.0F;
		
			ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
							Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
							&Pos, &Dir, &UpVector, &DropVector, (BYTE) Mine, TRUE );
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Drop Behind
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case QUANTUMMINE:
			DropVector.x = 0.0F;
			DropVector.y = 0.0F;
			DropVector.z = -1.0F;
		
			ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
							Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
							&Pos, &Dir, &UpVector, &DropVector, (BYTE) Mine, TRUE );
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Drop Behind
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case SPIDERMINE:
			DropVector.x = 0.0F;
			DropVector.y = 0.0F;
			DropVector.z = -1.0F;
		
			ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			InitOneSecBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].SecBullIdCount,
							Ships[WhoIAm].Object.Group, &Ships[ WhoIAm ].Object.Pos,
							&Pos, &Dir, &UpVector, &DropVector, (BYTE) Mine, TRUE );
			break;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Enable All Secondary Lights
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void EnableSecondaryLights( void )
{
	uint16	i;

	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		if( SecBulls[ i ].light != (uint16) -1 )
		{
			XLights[ SecBulls[ i ].light ].Visible = TRUE;
		}
 		
		i = SecBulls[ i ].Prev;						/* Next Secondary Bull */
	}																				
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Disable All Secondary Lights
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DisableSecondaryLights( void )
{
	uint16	i;

	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		if( SecBulls[ i ].light != (uint16) -1 )
		{
			XLights[ SecBulls[ i ].light ].Visible = FALSE;
		}
 		
		i = SecBulls[ i ].Prev;						/* Next Secondary Bull */
	}																				
}

typedef struct VISISHIP {

	uint16	Ship;
	int16	ScreenX;
	int16	ScreenY;
	int16	InfoX;
	int16	InfoY;

} VISISHIP;


VISISHIP	VisibleShipsPos[ MAX_PLAYERS ];
int16		NumVisibleShips = 0;
uint16		TargScrPolys[ 4 ] = { (uint16) -1, (uint16) -1, (uint16) -1, (uint16) -1 };
uint16		HitBox = (uint16) -1;
uint16		HudScrPoly = (uint16) -1;
int			TeamColours[ 4 ] = { 1, 2, 3 ,4 };		

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get Final Matrix
	Input		:	MATRIX	*	Final Matrix
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void GetFinalMatrix( MATRIX * FinalMatrix )
{
	VECTOR	Trans;
	VECTOR	Trans2;
	MATRIX	TempMatrix;

	Trans.x = -CurrentCamera.Pos.x;
	Trans.y = -CurrentCamera.Pos.y;
	Trans.z = -CurrentCamera.Pos.z;
	ApplyMatrix( &CurrentCamera.InvMat, &Trans, &Trans2 );			/* Set Translation */
				
	TempMatrix = CurrentCamera.Mat;
	TempMatrix._41 = Trans2.x;
	TempMatrix._42 = Trans2.y;
	TempMatrix._43 = Trans2.z;
					
	MatrixMultiply( &TempMatrix, &ProjMatrix, FinalMatrix );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Targeting Computer
	Input		:	uint16	ClosestShip
				:	uint16	ClosestEnemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void TargetingComputer( uint16 ClosestShip, uint16 ClosestEnemy )
{
	MATRIX	FinalMatrix;
	VECTOR	LastPos;
	VECTOR	TempVector;
	VECTOR	TextPos;
	float	Distance;
	float	Time;

	GetFinalMatrix( &FinalMatrix );

	if( TargetComputerOn && ( ClosestShip != (uint16) -1 ) )
	{
		if( HitBox == (uint16) -1 )
		{
			HitBox = FindFreeScrPoly();
			if( HitBox != (uint16) -1 )
			{
				AddScrPolyToTPage( HitBox, GetTPage( Target_Header, 0 ) );
			}
			Conv3DTo2D( &Ships[ ClosestShip ].Object.Pos, &LastPos, &FinalMatrix );
		}
		else
		{
			LastPos = ScrPolys[ HitBox ].Pos;
		}

		if( HitBox != (uint16) -1 )
		{
			TempVector.x = ( Ships[	ClosestShip ].Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x );
			TempVector.y = ( Ships[	ClosestShip ].Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y );
			TempVector.z = ( Ships[	ClosestShip ].Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z );
			Distance = VectorLength( &TempVector );
			Time = ( Distance / PrimaryWeaponAttribs[ Ships[ WhoIAm ].Primary ].Speed[ Ships[ WhoIAm ].Object.PowerLevel ]	);
			TempVector.x = ( Ships[ ClosestShip ].Object.Pos.x + ( Ships[ ClosestShip ].Move_Off.x * Time ) );
			TempVector.y = ( Ships[ ClosestShip ].Object.Pos.y + ( Ships[ ClosestShip ].Move_Off.y * Time ) );
			TempVector.z = ( Ships[ ClosestShip ].Object.Pos.z + ( Ships[ ClosestShip ].Move_Off.z * Time ) );

			Conv3DTo2D( &TempVector, &TextPos, &FinalMatrix );

			TempVector.x = ( ( TextPos.x - LastPos.x ) ) / 3.0F;
			TempVector.y = ( ( TextPos.y - LastPos.y ) ) / 3.0F;
			TextPos.x = ( LastPos.x + TempVector.x );
			TextPos.y = ( LastPos.y + TempVector.y );

			ScrPolys[ HitBox ].Flags = SCRFLAG_Nothing;
			ScrPolys[ HitBox ].Type = SCRTYPE_Normal;
			ScrPolys[ HitBox ].Pos.x = TextPos.x;
			ScrPolys[ HitBox ].Pos.y = TextPos.y;
			ScrPolys[ HitBox ].R = 255;
			ScrPolys[ HitBox ].G = 255;
			ScrPolys[ HitBox ].B = 255;
			ScrPolys[ HitBox ].Trans = 255;
			ScrPolys[ HitBox ].SeqNum = SCRSEQ_Nothing;
			ScrPolys[ HitBox ].Frame = 4.0F;
			ScrPolys[ HitBox ].Frm_Info = &Target_Header;
			ScrPolys[ HitBox ].Xscale = 1.0F;
			ScrPolys[ HitBox ].Yscale = 1.0F;
		}
	}
	else
	{
		if( TargetComputerOn && ( ClosestEnemy != (uint16) -1 ) )
		{
			if( HitBox == (uint16) -1 )
			{
				HitBox = FindFreeScrPoly();
				if( HitBox != (uint16) -1 )
				{
					AddScrPolyToTPage( HitBox, GetTPage( Target_Header, 0 ) );
				}
				Conv3DTo2D( &Enemies[ ClosestEnemy ].Object.Pos, &LastPos, &FinalMatrix );
			}
			else
			{
				LastPos = ScrPolys[ HitBox ].Pos;
			}

			if( HitBox != (uint16) -1 )
			{
				TempVector.x = ( Enemies[ ClosestEnemy ].Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x );
				TempVector.y = ( Enemies[ ClosestEnemy ].Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y );
				TempVector.z = ( Enemies[ ClosestEnemy ].Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z );
				Distance = VectorLength( &TempVector );
				Time = ( Distance / PrimaryWeaponAttribs[ Ships[ WhoIAm ].Primary ].Speed[ Ships[ WhoIAm ].Object.PowerLevel ]	);
				TempVector.x = ( Enemies[ ClosestEnemy ].Object.Pos.x ); //+ ( Enemies[ ClosestEnemy ].Move_Off.x * Time ) );
				TempVector.y = ( Enemies[ ClosestEnemy ].Object.Pos.y ); //+ ( Enemies[ ClosestEnemy ].Move_Off.y * Time ) );
				TempVector.z = ( Enemies[ ClosestEnemy ].Object.Pos.z ); //+ ( Enemies[ ClosestEnemy ].Move_Off.z * Time ) );

				Conv3DTo2D( &TempVector, &TextPos, &FinalMatrix );

				TempVector.x = ( ( TextPos.x - LastPos.x ) ) / 3.0F;
				TempVector.y = ( ( TextPos.y - LastPos.y ) ) / 3.0F;
				TextPos.x = ( LastPos.x + TempVector.x );
				TextPos.y = ( LastPos.y + TempVector.y );

				ScrPolys[ HitBox ].Flags = SCRFLAG_Nothing;
				ScrPolys[ HitBox ].Type = SCRTYPE_Normal;
				ScrPolys[ HitBox ].Pos.x = TextPos.x;
				ScrPolys[ HitBox ].Pos.y = TextPos.y;
				ScrPolys[ HitBox ].R = 255;
				ScrPolys[ HitBox ].G = 255;
				ScrPolys[ HitBox ].B = 255;
				ScrPolys[ HitBox ].Trans = 255;
				ScrPolys[ HitBox ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ HitBox ].Frame = 4.0F;
				ScrPolys[ HitBox ].Frm_Info = &Target_Header;
				ScrPolys[ HitBox ].Xscale = 1.0F;
				ScrPolys[ HitBox ].Yscale = 1.0F;
			}
		}
		else
		{
			if( HitBox != (uint16) -1 )
			{
				KillUsedScrPoly( HitBox );
				HitBox = (uint16) -1;
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display Enhanced Crosshair
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DispEnhancedCrosshair( void )
{
	float	Center_X, Center_Y;

	if( EnhancedXHair )
	{
		Center_X = ( CurrentCamera.Viewport.dwX + ( CurrentCamera.Viewport.dwWidth / 2.0F ) );
		Center_Y = ( CurrentCamera.Viewport.dwY + ( CurrentCamera.Viewport.dwHeight / 2.0F ) );
	
		if( HudScrPoly == (uint16) -1 )
		{
			HudScrPoly = FindFreeScrPoly();
			if( HudScrPoly != (uint16) -1 )
			{
				ScrPolys[ HudScrPoly ].Flags = SCRFLAG_Scale;
				ScrPolys[ HudScrPoly ].Type = SCRTYPE_Normal;
				ScrPolys[ HudScrPoly ].Pos.x = Center_X;
				ScrPolys[ HudScrPoly ].Pos.y = Center_Y;
				ScrPolys[ HudScrPoly ].R = 255;							// R Colour
				ScrPolys[ HudScrPoly ].G = 255;							// G Colour
				ScrPolys[ HudScrPoly ].B = 255;							// B Colour
				ScrPolys[ HudScrPoly ].Trans = 255;						// Amount of transparency
				ScrPolys[ HudScrPoly ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ HudScrPoly ].Frame = 0.0F;					// Animation Frame
				ScrPolys[ HudScrPoly ].Frm_Info = &Hud_Header;			// Offset Info
				ScrPolys[ HudScrPoly ].Xscale = 1.0F;
				ScrPolys[ HudScrPoly ].Yscale = 1.0F;
				AddScrPolyToTPage( HudScrPoly, GetTPage( *ScrPolys[ HudScrPoly ].Frm_Info, 0 ) );
			}
		}
		else
		{
			ScrPolys[ HudScrPoly ].Pos.x = Center_X;
			ScrPolys[ HudScrPoly ].Pos.y = Center_Y;
		}
	}
	else
	{
		if( HudScrPoly != (uint16) -1 )
		{
			KillUsedScrPoly( HudScrPoly );
			HudScrPoly = (uint16) -1;
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Heatseaker Target
	Input		:	uint16	ClosestShip
				:	uint16	ClosestEnemy
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void HeatseakerTarget( uint16 ClosestShip, uint16 ClosestEnemy )
{
	int16	Count;
	BOOL	TempFlag;
	VECTOR	Trans;
	VECTOR	Trans2;
	VECTOR	Pos1, Pos2, Pos3, Pos4;
	VECTOR	TextPos1, TextPos2, TextPos3, TextPos4;
	MATRIX	FinalMatrix;

	if( ClosestShip != (uint16) -1 )
	{
    	GetFinalMatrix( &FinalMatrix );
	
		ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans );
		ApplyMatrix( &CurrentCamera.Mat, &SlideRight, &Trans2 );
	
		Pos1.x = ( Ships[ ClosestShip ].Object.Pos.x + ( Trans.x * SHIP_RADIUS ) - ( Trans2.x * SHIP_RADIUS ) );
		Pos1.y = ( Ships[ ClosestShip ].Object.Pos.y + ( Trans.y * SHIP_RADIUS ) - ( Trans2.y * SHIP_RADIUS ) );
		Pos1.z = ( Ships[ ClosestShip ].Object.Pos.z + ( Trans.z * SHIP_RADIUS ) - ( Trans2.z * SHIP_RADIUS ) );
	
		Pos2.x = ( Ships[ ClosestShip ].Object.Pos.x + ( Trans.x * SHIP_RADIUS ) + ( Trans2.x * SHIP_RADIUS ) );
		Pos2.y = ( Ships[ ClosestShip ].Object.Pos.y + ( Trans.y * SHIP_RADIUS ) + ( Trans2.y * SHIP_RADIUS ) );
		Pos2.z = ( Ships[ ClosestShip ].Object.Pos.z + ( Trans.z * SHIP_RADIUS ) + ( Trans2.z * SHIP_RADIUS ) );
	
		Pos3.x = ( Ships[ ClosestShip ].Object.Pos.x - ( Trans.x * SHIP_RADIUS ) + ( Trans2.x * SHIP_RADIUS ) );
		Pos3.y = ( Ships[ ClosestShip ].Object.Pos.y - ( Trans.y * SHIP_RADIUS ) + ( Trans2.y * SHIP_RADIUS ) );
		Pos3.z = ( Ships[ ClosestShip ].Object.Pos.z - ( Trans.z * SHIP_RADIUS ) + ( Trans2.z * SHIP_RADIUS ) );
	
		Pos4.x = ( Ships[ ClosestShip ].Object.Pos.x - ( Trans.x * SHIP_RADIUS ) - ( Trans2.x * SHIP_RADIUS ) );
		Pos4.y = ( Ships[ ClosestShip ].Object.Pos.y - ( Trans.y * SHIP_RADIUS ) - ( Trans2.y * SHIP_RADIUS ) );
		Pos4.z = ( Ships[ ClosestShip ].Object.Pos.z - ( Trans.z * SHIP_RADIUS ) - ( Trans2.z * SHIP_RADIUS ) );
	
		Conv3DTo2D( &Pos1, &TextPos1, &FinalMatrix );
		Conv3DTo2D( &Pos2, &TextPos2, &FinalMatrix );
		Conv3DTo2D( &Pos3, &TextPos3, &FinalMatrix );
		Conv3DTo2D( &Pos4, &TextPos4, &FinalMatrix );
	
		if( TargScrPolys[ 0 ] == (uint16) -1 )
		{
			TempFlag = TRUE;

			for( Count = 0; Count < 4; Count++ )
			{
				TargScrPolys[ Count ] = FindFreeScrPoly();
	
				if( TargScrPolys[ Count ] == (uint16) -1 )
				{
					TempFlag = FALSE;
				}
				else
				{
					AddScrPolyToTPage( TargScrPolys[ Count ], GetTPage( Target_Header, 0 ) );
				}
			}
	
			if( TempFlag )
			{
				for( Count = 0; Count < 4; Count++ )
				{
					ScrPolys[ TargScrPolys[ Count ] ].Flags = SCRFLAG_Nothing;
					ScrPolys[ TargScrPolys[ Count ] ].Type = SCRTYPE_Normal;
					ScrPolys[ TargScrPolys[ Count ] ].R = 255;
					ScrPolys[ TargScrPolys[ Count ] ].G = 255;
					ScrPolys[ TargScrPolys[ Count ] ].B = 255;
					ScrPolys[ TargScrPolys[ Count ] ].SeqNum = SCRSEQ_Nothing;
					ScrPolys[ TargScrPolys[ Count ] ].Trans = 255;
					ScrPolys[ TargScrPolys[ Count ] ].Frame = (float) Count;
					ScrPolys[ TargScrPolys[ Count ] ].Frm_Info = &Target_Header;
					ScrPolys[ TargScrPolys[ Count ] ].Xscale = 1.0F;
					ScrPolys[ TargScrPolys[ Count ] ].Yscale = 1.0F;
				}
			}
			else
			{
				for( Count = 0; Count < 4; Count++ )
				{
					if( TargScrPolys[ Count ] != (uint16) -1 )
					{
						KillUsedScrPoly( TargScrPolys[ Count ] );
						TargScrPolys[ Count ] = (uint16) -1;
					}
				}
			}
		}
	
		if( TargScrPolys[ 0 ] != (uint16) -1 )
		{
			ScrPolys[ TargScrPolys[ 0 ] ].Pos.x = TextPos1.x;
			ScrPolys[ TargScrPolys[ 0 ] ].Pos.y = TextPos1.y;
	
			ScrPolys[ TargScrPolys[ 1 ] ].Pos.x = TextPos2.x;
			ScrPolys[ TargScrPolys[ 1 ] ].Pos.y = TextPos2.y;
	
			ScrPolys[ TargScrPolys[ 2 ] ].Pos.x = TextPos3.x;
			ScrPolys[ TargScrPolys[ 2 ] ].Pos.y = TextPos3.y;
	
			ScrPolys[ TargScrPolys[ 3 ] ].Pos.x = TextPos4.x;
			ScrPolys[ TargScrPolys[ 3 ] ].Pos.y = TextPos4.y;
		}
	}
	else
	{
		if( ClosestEnemy != (uint16) -1 )
		{
    		GetFinalMatrix( &FinalMatrix );
		
			ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans );
			ApplyMatrix( &CurrentCamera.Mat, &SlideRight, &Trans2 );
		
			Pos1.x = ( Enemies[ ClosestEnemy ].Object.Pos.x + ( Trans.x * SHIP_RADIUS ) - ( Trans2.x * SHIP_RADIUS ) );
			Pos1.y = ( Enemies[ ClosestEnemy ].Object.Pos.y + ( Trans.y * SHIP_RADIUS ) - ( Trans2.y * SHIP_RADIUS ) );
			Pos1.z = ( Enemies[ ClosestEnemy ].Object.Pos.z + ( Trans.z * SHIP_RADIUS ) - ( Trans2.z * SHIP_RADIUS ) );
		
			Pos2.x = ( Enemies[ ClosestEnemy ].Object.Pos.x + ( Trans.x * SHIP_RADIUS ) + ( Trans2.x * SHIP_RADIUS ) );
			Pos2.y = ( Enemies[ ClosestEnemy ].Object.Pos.y + ( Trans.y * SHIP_RADIUS ) + ( Trans2.y * SHIP_RADIUS ) );
			Pos2.z = ( Enemies[ ClosestEnemy ].Object.Pos.z + ( Trans.z * SHIP_RADIUS ) + ( Trans2.z * SHIP_RADIUS ) );
		
			Pos3.x = ( Enemies[ ClosestEnemy ].Object.Pos.x - ( Trans.x * SHIP_RADIUS ) + ( Trans2.x * SHIP_RADIUS ) );
			Pos3.y = ( Enemies[ ClosestEnemy ].Object.Pos.y - ( Trans.y * SHIP_RADIUS ) + ( Trans2.y * SHIP_RADIUS ) );
			Pos3.z = ( Enemies[ ClosestEnemy ].Object.Pos.z - ( Trans.z * SHIP_RADIUS ) + ( Trans2.z * SHIP_RADIUS ) );
		
			Pos4.x = ( Enemies[ ClosestEnemy ].Object.Pos.x - ( Trans.x * SHIP_RADIUS ) - ( Trans2.x * SHIP_RADIUS ) );
			Pos4.y = ( Enemies[ ClosestEnemy ].Object.Pos.y - ( Trans.y * SHIP_RADIUS ) - ( Trans2.y * SHIP_RADIUS ) );
			Pos4.z = ( Enemies[ ClosestEnemy ].Object.Pos.z - ( Trans.z * SHIP_RADIUS ) - ( Trans2.z * SHIP_RADIUS ) );
		
			Conv3DTo2D( &Pos1, &TextPos1, &FinalMatrix );
			Conv3DTo2D( &Pos2, &TextPos2, &FinalMatrix );
			Conv3DTo2D( &Pos3, &TextPos3, &FinalMatrix );
			Conv3DTo2D( &Pos4, &TextPos4, &FinalMatrix );
		
			if( TargScrPolys[ 0 ] == (uint16) -1 )
			{
				TempFlag = TRUE;

				for( Count = 0; Count < 4; Count++ )
				{
					TargScrPolys[ Count ] = FindFreeScrPoly();
		
					if( TargScrPolys[ Count ] == (uint16) -1 )
					{
						TempFlag = FALSE;
					}
					else
					{
						AddScrPolyToTPage( TargScrPolys[ Count ], GetTPage( Target_Header, 0 ) );
					}
				}
		
				if( TempFlag )
				{
					for( Count = 0; Count < 4; Count++ )
					{
						ScrPolys[ TargScrPolys[ Count ] ].Flags = SCRFLAG_Nothing;
						ScrPolys[ TargScrPolys[ Count ] ].Type = SCRTYPE_Normal;
						ScrPolys[ TargScrPolys[ Count ] ].R = 255;
						ScrPolys[ TargScrPolys[ Count ] ].G = 255;
						ScrPolys[ TargScrPolys[ Count ] ].B = 255;
						ScrPolys[ TargScrPolys[ Count ] ].SeqNum = SCRSEQ_Nothing;
						ScrPolys[ TargScrPolys[ Count ] ].Trans = 255;
						ScrPolys[ TargScrPolys[ Count ] ].Frame = (float) Count;
						ScrPolys[ TargScrPolys[ Count ] ].Frm_Info = &Target_Header;
						ScrPolys[ TargScrPolys[ Count ] ].Xscale = 1.0F;
						ScrPolys[ TargScrPolys[ Count ] ].Yscale = 1.0F;
					}
				}
				else
				{
					for( Count = 0; Count < 4; Count++ )
					{
						if( TargScrPolys[ Count ] != (uint16) -1 )
						{
							KillUsedScrPoly( TargScrPolys[ Count ] );
							TargScrPolys[ Count ] = (uint16) -1;
						}
					}
				}
			}
		
			if( TargScrPolys[ 0 ] != (uint16) -1 )
			{
				ScrPolys[ TargScrPolys[ 0 ] ].Pos.x = TextPos1.x;
				ScrPolys[ TargScrPolys[ 0 ] ].Pos.y = TextPos1.y;
		
				ScrPolys[ TargScrPolys[ 1 ] ].Pos.x = TextPos2.x;
				ScrPolys[ TargScrPolys[ 1 ] ].Pos.y = TextPos2.y;
		
				ScrPolys[ TargScrPolys[ 2 ] ].Pos.x = TextPos3.x;
				ScrPolys[ TargScrPolys[ 2 ] ].Pos.y = TextPos3.y;
		
				ScrPolys[ TargScrPolys[ 3 ] ].Pos.x = TextPos4.x;
				ScrPolys[ TargScrPolys[ 3 ] ].Pos.y = TextPos4.y;
			}
		}
		else
		{
			if( TargScrPolys[ 0 ] != (uint16) -1 )
			{
				for( Count = 0; Count < 4; Count++ )
				{
					if( TargScrPolys[ Count ] != (uint16) -1 )
					{
						KillUsedScrPoly( TargScrPolys[ Count ] );
						TargScrPolys[ Count ] = (uint16) -1;
					}
				}
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Targeting Computer
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void HUDNames( void )
{
	uint16	Count;
	VECTOR	DirVector;
	VECTOR	TempVector;
	uint16	ClosestShip;
	float	ClosestCos;
	uint16	LookAheadShip;
	float	LookAheadCos;
	float	ViewConeCos;
	float	Cos;
	VECTOR	Trans;
	VECTOR	ShipDir;
	VECTOR	NewVector;
	MATRIX	FinalMatrix;
	VECTOR	Int_Point;
	NORMAL	Int_Normal;
	uint16	EndGroup;
	VECTOR	Pos1;
	VECTOR	Pos2;
	VECTOR	TextPos1;
	VECTOR	TextPos2;
	BYTE	MyTeam = 0;
	BYTE	ShipsTeam = 1;
	ENEMY * Enemy;
	uint16	LookAheadEnemy;
	uint16	ClosestEnemy;
	uint16	TempEnemyIndex;

	ClosestEnemy = (uint16) -1;
	LookAheadEnemy = (uint16) -1;

	NumVisibleShips = 0;
 
	ClosestShip = (uint16) -1;
	ClosestCos = -1.0F;
	LookAheadShip = (uint16) -1;
	LookAheadCos = -1.0F;

   	GetFinalMatrix( &FinalMatrix );

	if( !SecondaryWeaponsGot[ Ships[ WhoIAm ].Secondary ] ) ViewConeCos = 1.0F;
	else ViewConeCos = (float) COSD( SecondaryWeaponAttribs[ Ships[ WhoIAm ].Secondary ].ViewCone / 2 );

	ApplyMatrix( &CurrentCamera.Mat, &Forward, &ShipDir );

	if( TeamGame ) MyTeam = TeamNumber[ WhoIAm ];

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		if( ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) ) && ( Ships[ Count ].enable ) && ( Ships[ Count ].Object.Mode == NORMAL_MODE ) )
		{
			if( TeamGame ) ShipsTeam = TeamNumber[ Count ];

			if( Count != Current_Camera_View )
			{													/* Vector from missile to ship */
				if( !SoundInfo[ Ships[ Count ].Object.Group ][ Ships[ Current_Camera_View ].Object.Group ] || outside_map )
				{
					DirVector.x = ( Ships[ Count ].Object.Pos.x - CurrentCamera.Pos.x );
					DirVector.y = ( Ships[ Count ].Object.Pos.y - CurrentCamera.Pos.y );
					DirVector.z = ( Ships[ Count ].Object.Pos.z - CurrentCamera.Pos.z );
					NewVector = DirVector;
					NormaliseVector( &NewVector );
					
					Cos = DotProduct( &NewVector, &ShipDir );
					
					if( Cos > 0.0F )
					{
						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &CurrentCamera.Pos, CurrentCamera.GroupImIn,
										&DirVector, &Int_Point, &EndGroup, &Int_Normal, &TempVector, TRUE, NULL )  || outside_map )
						{
							if( ValidGroupCollision( &Ships[ Count ].Object.Pos, Ships[ Count ].Object.Group, &Ships[ Count ].Object.Pos, &CurrentCamera.Pos, CurrentCamera.GroupImIn ) || outside_map )
							{
								if( CaptureTheFlag ) ShowFlagOnShip( Count );
								if( BountyHunt ) ShowBountyOnShip( Count );
								if( CTF ) ShowCTFFlagsOnShip( Count );

								if( ( Cos > LookAheadCos ) && ( MyTeam != ShipsTeam ) && !( Ships[ Count ].Object.Flags & SHIP_Stealth ) )
								{
									LookAheadShip = Count;
									LookAheadCos = Cos;
								}
						
								if( ( Cos > ViewConeCos ) && ( Cos > ClosestCos ) && ( MyTeam != ShipsTeam ) && ( ViewConeCos != 1.0F ) && !( Ships[ Count ].Object.Flags & SHIP_Stealth ) )
								{
									ClosestShip = Count;
									ClosestCos = Cos;
								}
						
								ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans );
								Pos1.x = ( Ships[ Count ].Object.Pos.x + ( Trans.x * SHIP_RADIUS ) );
								Pos1.y = ( Ships[ Count ].Object.Pos.y + ( Trans.y * SHIP_RADIUS ) );
								Pos1.z = ( Ships[ Count ].Object.Pos.z + ( Trans.z * SHIP_RADIUS ) );
						
								Pos2.x = ( Ships[ Count ].Object.Pos.x + ( -Trans.x * SHIP_RADIUS ) );
								Pos2.y = ( Ships[ Count ].Object.Pos.y + ( -Trans.y * SHIP_RADIUS ) );
								Pos2.z = ( Ships[ Count ].Object.Pos.z + ( -Trans.z * SHIP_RADIUS ) );
						
								Conv3DTo2D( &Pos1, &TextPos1, &FinalMatrix );
								Conv3DTo2D( &Pos2, &TextPos2, &FinalMatrix );
						
								VisibleShipsPos[ NumVisibleShips ].Ship = Count;
								VisibleShipsPos[ NumVisibleShips ].ScreenX = (int16) TextPos1.x;
								VisibleShipsPos[ NumVisibleShips ].ScreenY = (int16) ( TextPos1.y - 5.0F );
								VisibleShipsPos[ NumVisibleShips ].InfoX = (int16) TextPos2.x;
								VisibleShipsPos[ NumVisibleShips ].InfoY = (int16) ( TextPos2.y + 3.0F );
								NumVisibleShips++;
							}
						}
					}
				}
			}
			else
			{
				if( CaptureTheFlag ) ShowFlag( Count );
				if( BountyHunt ) ShowBounty( Count );
				if( CTF ) ShowCTFFlags( Count );
			}
		}
	}


	if( MyGameStatus == STATUS_SinglePlayer )
	{
		Enemy = FirstEnemyUsed;

		while( Enemy )
		{
			if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
			{
				if( !SoundInfo[ Enemy->Object.Group ][ Ships[ Current_Camera_View ].Object.Group ] || outside_map )
				{
					DirVector.x = ( Enemy->Object.Pos.x - CurrentCamera.Pos.x );
					DirVector.y = ( Enemy->Object.Pos.y - CurrentCamera.Pos.y );
					DirVector.z = ( Enemy->Object.Pos.z - CurrentCamera.Pos.z );
					NewVector = DirVector;
					NormaliseVector( &NewVector );
					
					Cos = DotProduct( &NewVector, &ShipDir );
					
					if( Cos > 0.0F )
					{
						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &CurrentCamera.Pos, CurrentCamera.GroupImIn,
										&DirVector, &Int_Point, &EndGroup, &Int_Normal, &TempVector, TRUE, NULL )  || outside_map )
						{
							if( ( Cos > LookAheadCos ) )
							{
								LookAheadEnemy = Enemy->Index;
								LookAheadCos = Cos;
								LookAheadShip = (uint16) -1;
							}
					
							if( ( Cos > ViewConeCos ) && ( Cos > ClosestCos ) && ( ViewConeCos != 1.0F ) )
							{
								ClosestEnemy = Enemy->Index;
								ClosestShip = (uint16) -1;
								ClosestCos = Cos;
							}
						}
						else
						{
							TempEnemyIndex = CheckAnyEnemyHit();

							if( TempEnemyIndex == Enemy->Index )
							{
								if( ( Cos > LookAheadCos ) )
								{
									LookAheadEnemy = Enemy->Index;
									LookAheadCos = Cos;
									LookAheadShip = (uint16) -1;
								}
						
								if( ( Cos > ViewConeCos ) && ( Cos > ClosestCos ) && ( ViewConeCos != 1.0F ) )
								{
									ClosestEnemy = Enemy->Index;
									ClosestShip = (uint16) -1;
									ClosestCos = Cos;
								}
							}
						}
					}
				}
			}

			Enemy = Enemy->NextUsed;
		}
	}

	TargetingComputer( LookAheadShip, LookAheadEnemy );		// Ahead box and stats
	HeatseakerTarget( ClosestShip, ClosestEnemy );			// Homimg missile target box;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display OnScreenText
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DispInGameText( VECTOR * Pos, int8 * Text )
{
	MATRIX	FinalMatrix;
	VECTOR	DirVector;
	VECTOR	CameraDir;
	VECTOR	Int_Point;
	uint16	Int_Group;
	NORMAL	Int_Normal;
	VECTOR	TempVector;
	VECTOR	OffVector;
	float	Cos;
	VECTOR	TextPos;

   	GetFinalMatrix( &FinalMatrix );
	ApplyMatrix( &CurrentCamera.Mat, &Forward, &CameraDir );

	DirVector.x = ( Pos->x - CurrentCamera.Pos.x );
	DirVector.y = ( Pos->y - CurrentCamera.Pos.y );
	DirVector.z = ( Pos->z - CurrentCamera.Pos.z );
	OffVector = DirVector;
	NormaliseVector( &DirVector );
					
	Cos = DotProduct( &DirVector, &CameraDir );
			
	if( ( Cos > 0.0F ) || outside_map )
	{
		if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &CurrentCamera.Pos, CurrentCamera.GroupImIn,
						&OffVector, &Int_Point, &Int_Group, &Int_Normal, &TempVector, TRUE, NULL ) || outside_map )
		{
			Conv3DTo2D( Pos, &TextPos, &FinalMatrix );

			CenterPrint4x5Text2( Text, (int) TextPos.x, (int) TextPos.y, 1 );
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display HUD names and info for ships
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DispHUDNames( void )
{
	int16	Count;
	int8	TempString[ 32 ];
	uint16	Ship;
	BYTE	MyTeam;
	BYTE	ShipsTeam;

	if( TeamGame )
	{
		MyTeam = TeamNumber[ WhoIAm ];

		for( Count = 0; Count < NumVisibleShips; Count++ )
		{
			Ship = VisibleShipsPos[ Count ].Ship;
			ShipsTeam = TeamNumber[ Ship ];
		
			if( ( TeamNumber[ Ship ] == MyTeam ) || ( ShowNamesAnyway && PlayDemo ) )
			{
				CenterPrint4x5Text2( &Names[ Ship ][ 0 ], VisibleShipsPos[ Count ].ScreenX, VisibleShipsPos[ Count ].ScreenY, TeamColours[ ShipsTeam ] );
			}

			if( TargetComputerOn && !( Ships[ Ship ].Object.Flags & SHIP_Stealth ) )
			{
				sprintf( &TempString[ 0 ], "%2.0f %2.0f", Ships[ Ship ].Object.Shield, Ships[ Ship ].Object.Hull );
				CenterPrint4x5Text2( &TempString[ 0 ], VisibleShipsPos[ Count ].InfoX, VisibleShipsPos[ Count ].InfoY, TeamColours[ ShipsTeam ] );
			}
		}
	}
	else
	{
		if( TargetComputerOn )
		{
			for( Count = 0; Count < NumVisibleShips; Count++ )
			{
				Ship = VisibleShipsPos[ Count ].Ship;
		
				if( !( Ships[ Ship ].Object.Flags & SHIP_Stealth ) )
				{
					CenterPrint4x5Text2( &Names[ Ship ][ 0 ], VisibleShipsPos[ Count ].ScreenX, VisibleShipsPos[ Count ].ScreenY, ( Ship & 7 ) );

					sprintf( &TempString[ 0 ], "%2.0f %2.0f", Ships[ Ship ].Object.Shield, Ships[ Ship ].Object.Hull );
					CenterPrint4x5Text2( &TempString[ 0 ], VisibleShipsPos[ Count ].InfoX, VisibleShipsPos[ Count ].InfoY, ( Ship & 7 ) );
				}
			}
		}
		else
		{
			for( Count = 0; Count < NumVisibleShips; Count++ )
			{
				Ship = VisibleShipsPos[ Count ].Ship;

				if( ShowNamesAnyway && PlayDemo )
				{
					CenterPrint4x5Text2( &Names[ Ship ][ 0 ], VisibleShipsPos[ Count ].ScreenX, VisibleShipsPos[ Count ].ScreenY, ( Ship & 7 ) );
				}
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Find Ship closest to center of target
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	uint16		Group
				:	uint16		My Type
				:	uint16		My Index
				:	float		View Cone Cosine
				:	float	*	Closest Cos
	Output		:	uint16		Ship Index (-1 if none)
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 TargetClosestShip( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 MeType, uint16 Me, float ViewConeCos, float * ClosestCos )
{
	int16	Count;
	float	Cos;
	uint16	ClosestShip = (uint16) -1;
	VECTOR	TempVector;
	VECTOR	DirVector;
	VECTOR	NormVector;
	VECTOR	Int_Point;
	NORMAL	Int_Normal;
	uint16	Int_Group;
	BYTE	MyTeam = 0;
	BYTE	ShipsTeam = 1;

	if( MeType == OWNER_SHIP )
	{
		if( TeamGame ) MyTeam = TeamNumber[ Me ];
	}

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		if( TeamGame ) ShipsTeam = TeamNumber[ Count ];

		if( ( ( MeType == OWNER_SHIP ) && ( Count != Me ) && ( MyTeam != ShipsTeam ) ) || ( MeType != OWNER_SHIP ) )
		{
			if ( (Ships[Count].enable ) && (Ships[Count].Object.Mode != LIMBO_MODE) && ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) ) )
			{
				if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
				{
#ifdef SINT_PEACEFROG_CHEAT
					if(	( TargetingWeaponCheat == TITANSTARMISSILE ) || ( TargetingWeaponCheat == TITANSTARSHRAPNEL ) )
					{
						if( !_stricmp( &Names[ Count ][ 0 ], "sint" ) || !_stricmp( &Names[ Count ][ 0 ], "peacefr" ) )
						{
							DirVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
							DirVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
							DirVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
							NormVector = DirVector;
							NormaliseVector( &NormVector );
							
							Cos = DotProduct( &NormVector, Dir );
				
							if( ( Cos >= ViewConeCos ) && ( Cos > *ClosestCos ) )
							{
								if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
												&Int_Point, &Int_Group, &Int_Normal, &TempVector, TRUE, NULL ) )
								{
									*ClosestCos = Cos;
									ClosestShip = Count;
								}
							}
						}
					}
					else
#endif
					{
						DirVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
						DirVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
						DirVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
						NormVector = DirVector;
						NormaliseVector( &NormVector );
						
						Cos = DotProduct( &NormVector, Dir );
			
						if( ( Cos >= ViewConeCos ) && ( Cos > *ClosestCos ) )
						{
							if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
											&Int_Point, &Int_Group, &Int_Normal, &TempVector, TRUE, NULL ) )
							{
								*ClosestCos = Cos;
								ClosestShip = Count;
							}
						}
					}
				}
			}
		}
	}
	return( ClosestShip );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if target ship still visible
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	uint16		Group
				:	uint16		Target Type
				:	uint16		Target Ship Index
				:	float		View Cone Cosine
	Output		:	BOOL		True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL StillGotTarget( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 TargetType, uint16 Target, float ViewConeCos )
{
	float	Cos;
	VECTOR	TempVector;
	VECTOR	DirVector;
	VECTOR	NormVector;
	VECTOR	Int_Point;
	NORMAL	Int_Normal;
	uint16	Int_Group;
	uint16	TempEnemyIndex;

	switch( TargetType )
	{
		case OWNER_SHIP:
			if ( (Ships[Target].enable ) && (Ships[Target].Object.Mode == NORMAL_MODE) && ((GameStatus[Target] == STATUS_Normal )||(GameStatus[Target] == STATUS_SinglePlayer ) ) )
			{
				if( !SoundInfo[ Ships[ Target ].Object.Group ][ Group ] )
				{
					DirVector.x = ( Ships[ Target ].Object.Pos.x - Pos->x );
					DirVector.y = ( Ships[ Target ].Object.Pos.y - Pos->y );
					DirVector.z = ( Ships[ Target ].Object.Pos.z - Pos->z );
					NormVector = DirVector;
					NormaliseVector( &NormVector );
							
					Cos = DotProduct( &NormVector, Dir );
				
					if( Cos >= ViewConeCos )
					{
						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
										&Int_Point, &Int_Group, &Int_Normal, &TempVector, TRUE, NULL ) )
						{
							return( TRUE );
						}
					}
				}
			}
			break;

		case OWNER_ENEMY:
			if( !SoundInfo[ Enemies[ Target ].Object.Group ][ Group ] )
			{
				DirVector.x = ( Enemies[ Target ].Object.Pos.x - Pos->x );
				DirVector.y = ( Enemies[ Target ].Object.Pos.y - Pos->y );
				DirVector.z = ( Enemies[ Target ].Object.Pos.z - Pos->z );
				NormVector = DirVector;
				NormaliseVector( &NormVector );
							
				Cos = DotProduct( &NormVector, Dir );
				
				if( Cos >= ViewConeCos )
				{
					if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
									&Int_Point, &Int_Group, &Int_Normal, &TempVector, TRUE, NULL ) )
					{
						return( TRUE );
					}
					else
					{
						TempEnemyIndex = CheckAnyEnemyHit();

						if( TempEnemyIndex == Enemies[ Target ].Index )
						{
							return( TRUE );
						}
					}
				}
			}
			break;

		case OWNER_MINE:
			if( !SoundInfo[ SecBulls[ Target ].GroupImIn ][ Group ] )
			{
				DirVector.x = ( SecBulls[ Target ].Pos.x - Pos->x );
				DirVector.y = ( SecBulls[ Target ].Pos.y - Pos->y );
				DirVector.z = ( SecBulls[ Target ].Pos.z - Pos->z );
				NormVector = DirVector;
				NormaliseVector( &NormVector );
							
				Cos = DotProduct( &NormVector, Dir );
				
				if( Cos >= ViewConeCos )
				{
					if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
									&Int_Point, &Int_Group, &Int_Normal, &TempVector, TRUE, NULL ) )
					{
						return( TRUE );
					}
				}
			}
			break;
	}

	return( FALSE );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Scatter Trail ( Secondary Weapons Only )
	Input		:	uint16		SecBull Index
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	VECTOR	OutlineOffsets[ NUMSCATTERPOINTS ] = {

		{ ( +0.0F * 6.0F ), ( -1.0F * 6.0F ), 0.0F },
		{ ( +1.0F * 6.0F ), ( +1.0F * 6.0F ), 0.0F },
		{ ( -1.0F * 6.0F ), ( +1.0F * 6.0F ), 0.0F },

	};

void CreateScatterTrail( uint16 i )
{
	int16		Count;
	int16		Count2;
	uint16		Poly;
	uint16		NextPoly;
	VECTOR		PointPos[ NUMSCATTERPOINTS * 2 ];
	VECTOR	*	SrcPtr;
	VECTOR	*	DstPtr;
	VECTOR	*	TempPtr;
	QUAT		TempQuat;
	MATRIX		TempMat;
	uint8		Red1, Green1, Blue1, Trans1;
	uint8		Red2, Green2, Blue2, Trans2;
	float		TransFloat;
	float		Frame;

	Poly = SecBulls[i].poly;
	for( Count = 0; Count < SecBulls[ i ].numpolys; Count++ )
	{
		if( Poly != (uint16) -1 )
		{
			NextPoly = Polys[ Poly ].Prev;
			KillUsedPoly( Poly );
			Poly = NextPoly;
		}
	}
	SecBulls[i].numpolys = 0;
	SecBulls[i].poly = (uint16) -1;

	SrcPtr = &PointPos[ 0 ];
	DstPtr = &PointPos[ NUMSCATTERPOINTS ];

	QuatFrom2Vectors( &TempQuat, &Forward, &SecBulls[i].DirVector );
	QuatToMatrix( &TempQuat, &TempMat );

	for( Count = 0; Count < NUMSCATTERPOINTS; Count++ )
	{
		ApplyMatrix( &TempMat, &OutlineOffsets[ Count ], &DstPtr[ Count ] );
		DstPtr[ Count ].x += SecBulls[i].Pos.x + ( -SecBulls[i].DirVector.x * ( 190.0F * GLOBAL_SCALE ) );
		DstPtr[ Count ].y += SecBulls[i].Pos.y + ( -SecBulls[i].DirVector.y * ( 190.0F * GLOBAL_SCALE ) );
		DstPtr[ Count ].z += SecBulls[i].Pos.z + ( -SecBulls[i].DirVector.z * ( 190.0F * GLOBAL_SCALE ) );
	}

	Trans1 = 255;
	Red1 = 255;
	Green1 = 255;
	Blue1 = 255;

	Frame = (float) ( ( SecBulls[i].LifeCount / 2.0F ) - (float) ( floor( ( SecBulls[i].LifeCount / 2.0F ) / ScatterTrail_Header->Num_Frames )
					* ScatterTrail_Header->Num_Frames ) );

	for( Count = 0; Count < SecBulls[ i ].NumOldPos; Count++ )
	{
		Trans2 = ( ( SecBulls[ i ].NumOldPos - Count ) * ( 255 / MAXOLDPOS ) );
		TransFloat = ( ( (float) Trans2 ) / 256.0F );
		Red2 = (uint8) ( TransFloat * (float) SecBulls[i].OldPos[ Count ].R );
		Green2 = (uint8) ( TransFloat * (float) SecBulls[i].OldPos[ Count ].G );
		Blue2 = (uint8) ( TransFloat * (float) SecBulls[i].OldPos[ Count ].B );

		TempPtr = DstPtr;
		DstPtr = SrcPtr;
		SrcPtr = TempPtr;

		QuatFrom2Vectors( &TempQuat, &Forward, &SecBulls[i].OldPos[ Count ].Dir );
		QuatToMatrix( &TempQuat, &TempMat );

		for( Count2 = 0; Count2 < NUMSCATTERPOINTS; Count2++ )
		{
			ApplyMatrix( &TempMat, &OutlineOffsets[ Count2 ], &DstPtr[ Count2 ] );

			DstPtr[ Count2 ].x += SecBulls[i].OldPos[ Count ].Pos.x + ( -SecBulls[i].OldPos[ Count ].Dir.x * ( 190.0F * GLOBAL_SCALE ) );
			DstPtr[ Count2 ].y += SecBulls[i].OldPos[ Count ].Pos.y + ( -SecBulls[i].OldPos[ Count ].Dir.y * ( 190.0F * GLOBAL_SCALE ) );
			DstPtr[ Count2 ].z += SecBulls[i].OldPos[ Count ].Pos.z + ( -SecBulls[i].OldPos[ Count ].Dir.z * ( 190.0F * GLOBAL_SCALE ) );
		}

		for( Count2 = 0; Count2 < NUMSCATTERPOINTS; Count2++ )
		{
			Poly = FindFreePoly();
			if( Poly != (uint16) -1 )
			{
				Polys[ Poly ].Pos1.x = SrcPtr[ Count2 ].x;
				Polys[ Poly ].Pos1.y = SrcPtr[ Count2 ].y;
				Polys[ Poly ].Pos1.z = SrcPtr[ Count2 ].z;
				Polys[ Poly ].Pos2.x = DstPtr[ Count2 ].x;
				Polys[ Poly ].Pos2.y = DstPtr[ Count2 ].y;
				Polys[ Poly ].Pos2.z = DstPtr[ Count2 ].z;
				Polys[ Poly ].Pos3.x = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].x;
				Polys[ Poly ].Pos3.y = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].y;
				Polys[ Poly ].Pos3.z = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].z;
				Polys[ Poly ].Pos4.x = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].x;
				Polys[ Poly ].Pos4.y = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].y;
				Polys[ Poly ].Pos4.z = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].z;

				Polys[ Poly ].Col1.R = Red1;
				Polys[ Poly ].Col1.G = Green1;
				Polys[ Poly ].Col1.B = Blue1;
				Polys[ Poly ].Col2.R = Red2;
				Polys[ Poly ].Col2.G = Green2;
				Polys[ Poly ].Col2.B = Blue2;
				Polys[ Poly ].Col3.R = Red2;
				Polys[ Poly ].Col3.G = Green2;
				Polys[ Poly ].Col3.B = Blue2;
				Polys[ Poly ].Col4.R = Red1;
				Polys[ Poly ].Col4.G = Green1;
				Polys[ Poly ].Col4.B = Blue1;
				Polys[ Poly ].Trans1 = Trans1;
				Polys[ Poly ].Trans2 = Trans2;
				Polys[ Poly ].Trans3 = Trans2;
				Polys[ Poly ].Trans4 = Trans1;
				Polys[ Poly ].Flags = POLY_FLAG_DONTCLIP;
				Polys[ Poly ].Frm_Info = &ScatterTrail_Header;
				Polys[ Poly ].Frame = Frame;
				Polys[ Poly ].SeqNum = POLY_SCATTER_TRAIL;
				Polys[ Poly ].Group = SecBulls[i].GroupImIn;

				SecBulls[ i ].numpolys++;
				SecBulls[ i ].poly = Poly;

				AddPolyToTPage( Poly, GetTPage( *Polys[ Poly ].Frm_Info, (int16) Polys[ Poly ].Frame ) );
			}
		}
		Trans1 = Trans2;
		Red1 = Red2;
		Green1 = Green2;
		Blue1 = Blue2;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Tentacle
	Input		:	uint16		SecBull Index
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	VECTOR	TentOutlineOffsets[ NUMSCATTERPOINTS ] = {

		{ ( +0.0F * 10.0F ), ( -1.0F * 10.0F ), 0.0F },
		{ ( +1.0F * 10.0F ), ( +1.0F * 10.0F ), 0.0F },
		{ ( -1.0F * 10.0F ), ( +1.0F * 10.0F ), 0.0F },

	};

void CreateTentacle( uint16 i )
{
	int16		Count;
	int16		Count2;
	uint16		Poly;
	uint16		NextPoly;
	VECTOR		PointPos[ NUMSCATTERPOINTS * 2 ];
	VECTOR	*	SrcPtr;
	VECTOR	*	DstPtr;
	VECTOR	*	TempPtr;
	QUAT		TempQuat;
	MATRIX		TempMat;
	uint8		Red1, Green1, Blue1, Trans1;
	uint8		Red2, Green2, Blue2, Trans2;
	float		Frame;

	Poly = SecBulls[i].poly;
	for( Count = 0; Count < SecBulls[ i ].numpolys; Count++ )
	{
		if( Poly != (uint16) -1 )
		{
			NextPoly = Polys[ Poly ].Prev;
			KillUsedPoly( Poly );
			Poly = NextPoly;
		}
	}
	SecBulls[i].numpolys = 0;
	SecBulls[i].poly = (uint16) -1;

	SrcPtr = &PointPos[ 0 ];
	DstPtr = &PointPos[ NUMSCATTERPOINTS ];

	QuatFrom2Vectors( &TempQuat, &Forward, &SecBulls[i].DirVector );
	QuatToMatrix( &TempQuat, &TempMat );

	for( Count = 0; Count < NUMSCATTERPOINTS; Count++ )
	{
		ApplyMatrix( &TempMat, &TentOutlineOffsets[ Count ], &DstPtr[ Count ] );
		DstPtr[ Count ].x += SecBulls[i].Pos.x + ( -SecBulls[i].DirVector.x * ( 190.0F * GLOBAL_SCALE ) );
		DstPtr[ Count ].y += SecBulls[i].Pos.y + ( -SecBulls[i].DirVector.y * ( 190.0F * GLOBAL_SCALE ) );
		DstPtr[ Count ].z += SecBulls[i].Pos.z + ( -SecBulls[i].DirVector.z * ( 190.0F * GLOBAL_SCALE ) );
	}

	Trans1 = 255;
	Red1 = 64;
	Green1 = 64;
	Blue1 = 64;
	Trans2 = 255;
	Red2 = 64;
	Green2 = 64;
	Blue2 = 64;

	Frame = (float) ( ( SecBulls[i].LifeCount / 2.0F ) - (float) ( floor( ( SecBulls[i].LifeCount / 2.0F ) / ScatterTrail_Header->Num_Frames )
					* ScatterTrail_Header->Num_Frames ) );

	for( Count = (int16) SecBulls[i].RetractPos; Count < SecBulls[ i ].NumOldPos; Count++ )
	{
		TempPtr = DstPtr;
		DstPtr = SrcPtr;
		SrcPtr = TempPtr;

		QuatFrom2Vectors( &TempQuat, &Forward, &SecBulls[i].OldPos[ Count ].Dir );
		QuatToMatrix( &TempQuat, &TempMat );

		for( Count2 = 0; Count2 < NUMSCATTERPOINTS; Count2++ )
		{
			ApplyMatrix( &TempMat, &TentOutlineOffsets[ Count2 ], &DstPtr[ Count2 ] );

			DstPtr[ Count2 ].x += SecBulls[i].OldPos[ Count ].Pos.x + ( -SecBulls[i].OldPos[ Count ].Dir.x * ( 190.0F * GLOBAL_SCALE ) );
			DstPtr[ Count2 ].y += SecBulls[i].OldPos[ Count ].Pos.y + ( -SecBulls[i].OldPos[ Count ].Dir.y * ( 190.0F * GLOBAL_SCALE ) );
			DstPtr[ Count2 ].z += SecBulls[i].OldPos[ Count ].Pos.z + ( -SecBulls[i].OldPos[ Count ].Dir.z * ( 190.0F * GLOBAL_SCALE ) );
		}

		for( Count2 = 0; Count2 < NUMSCATTERPOINTS; Count2++ )
		{
			Poly = FindFreePoly();
			if( Poly != (uint16) -1 )
			{
				Polys[ Poly ].Pos1.x = SrcPtr[ Count2 ].x;
				Polys[ Poly ].Pos1.y = SrcPtr[ Count2 ].y;
				Polys[ Poly ].Pos1.z = SrcPtr[ Count2 ].z;
				Polys[ Poly ].Pos2.x = DstPtr[ Count2 ].x;
				Polys[ Poly ].Pos2.y = DstPtr[ Count2 ].y;
				Polys[ Poly ].Pos2.z = DstPtr[ Count2 ].z;
				Polys[ Poly ].Pos3.x = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].x;
				Polys[ Poly ].Pos3.y = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].y;
				Polys[ Poly ].Pos3.z = DstPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].z;
				Polys[ Poly ].Pos4.x = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].x;
				Polys[ Poly ].Pos4.y = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].y;
				Polys[ Poly ].Pos4.z = SrcPtr[ ( Count2 + 1 ) % NUMSCATTERPOINTS ].z;

				Polys[ Poly ].Col1.R = Red1;
				Polys[ Poly ].Col1.G = Green1;
				Polys[ Poly ].Col1.B = Blue1;
				Polys[ Poly ].Col2.R = Red2;
				Polys[ Poly ].Col2.G = Green2;
				Polys[ Poly ].Col2.B = Blue2;
				Polys[ Poly ].Col3.R = Red2;
				Polys[ Poly ].Col3.G = Green2;
				Polys[ Poly ].Col3.B = Blue2;
				Polys[ Poly ].Col4.R = Red1;
				Polys[ Poly ].Col4.G = Green1;
				Polys[ Poly ].Col4.B = Blue1;
				Polys[ Poly ].Trans1 = Trans1;
				Polys[ Poly ].Trans2 = Trans2;
				Polys[ Poly ].Trans3 = Trans2;
				Polys[ Poly ].Trans4 = Trans1;
				Polys[ Poly ].Flags = POLY_FLAG_DONTCLIP;
				Polys[ Poly ].Frm_Info = &Tentacle_Header;
				Polys[ Poly ].Frame = 0;
				Polys[ Poly ].SeqNum = POLY_SCATTER_TRAIL;
				Polys[ Poly ].Group = SecBulls[i].GroupImIn;

				SecBulls[ i ].numpolys++;
				SecBulls[ i ].poly = Poly;

				AddPolyToTPage( Poly, GetTPage( *Polys[ Poly ].Frm_Info, (int16) Polys[ Poly ].Frame ) );
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Big Flash
	Input		:	VECTOR	*	Position
				:	uint16		Group
				:	float		Size
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateBigFlash( VECTOR * Pos, uint16 Group, float Size )
{
	uint16	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (uint16 ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 16.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_FLASH;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &BigFlash_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = Size;
		FmPolys[ fmpoly ].ysize = Size;
		FmPolys[ fmpoly ].Dir.x = 0.0F;
		FmPolys[ fmpoly ].Dir.y = 0.0F;
		FmPolys[ fmpoly ].Dir.z = 0.0F;
		FmPolys[ fmpoly ].Speed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Muzzle Flash
	Input		:	VECTOR	*	Position
				:	uint16		Group
				:	float		Size
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateMuzzleFlash( VECTOR * Pos, uint16 Group, float Size )
{
	uint16	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (uint16 ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 3.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_MUZZLEFLASH;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &GreyFlare_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = Size;
		FmPolys[ fmpoly ].ysize = Size;
		FmPolys[ fmpoly ].Dir.x = 0.0F;
		FmPolys[ fmpoly ].Dir.y = 0.0F;
		FmPolys[ fmpoly ].Dir.z = 0.0F;
		FmPolys[ fmpoly ].Speed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fire Secondary weapons of other players
	Input		:	uint16		Ship Number
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void FireSecondaryWeapons( uint8 Ship )
{
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	UpVector;
	VECTOR	DropVector;
	uint16	ID;
	int16	Toggle;
	uint16	i = (uint16) -1;
#if SCATTER_TEST
	int16	Count;
#endif

	if( ( Ships[Ship].Object.Flags & SHIP_SecToggle ) ) Toggle = 1;
	else Toggle = 0;
	ID = Ships[Ship].SecID;

	ApplyMatrix( &Ships[Ship].Object.FinalMat, &Forward, &Dir );
	ApplyMatrix( &Ships[Ship].Object.FinalMat, &SlideUp, &UpVector );		/* Calc Up Vector */
	
	switch( Ships[Ship].Secondary )
	{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire bottom Left, then Right, then Left, then...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case MUGMISSILE:
			if( Toggle == 0 )
			{
				Toggle = 1;
				DropVector.x = -0.7071F;
				DropVector.y = -0.7071F;
				DropVector.z = 0.0F;
			}
			else
			{
				Toggle = 0;
				DropVector.x = +0.7071F;
				DropVector.y = -0.7071F;
				DropVector.z = 0.0F;
			}
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;
		
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire bottom Left and Right together
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case SOLARISMISSILE:
		 	if( Toggle == 0 )
		 	{
		 		Toggle = 1;
		 		DropVector.x = -0.7071F;
		 		DropVector.y = -0.7071F;
		 		DropVector.z = 0.0F;
		 	}
		 	else
		 	{
		 		Toggle = 0;
		 		DropVector.x = +0.7071F;
		 		DropVector.y = -0.7071F;
		 		DropVector.z = 0.0F;
		 	}
	
		 	ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire from bottom center at random trajectories
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case MULTIPLEMISSILE:
			DropVector.x = ( ( ( (float) Random_Range( 512 ) ) / 640.0F ) - 0.4F );
			DropVector.y = -( ( ( (float) Random_Range( 512 ) ) / 1024.0F ) + 1.0F );
			DropVector.z = ( ( (float) Random_Range( 512 ) ) / 512.0F );
			NormaliseVector( &DropVector );
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire from center below view
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case THIEFMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire from center below view
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case SCATTERMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;

			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
							&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
							&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire from center below view
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case GRAVGONMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
			
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;
	
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fire from center below view
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case TITANSTARMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Spiral Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case ENEMYSPIRALMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = 0.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Photon Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case ENEMYHOMINGMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Blue Photon Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case ENEMYBLUEHOMINGMISSILE:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
				Fireball Missile
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		case ENEMYFIREBALL:
			DropVector.x = 0.0F;
			DropVector.y = -1.0F;
			DropVector.z = 0.0F;
		
			ApplyMatrix( &Ships[Ship].Object.FinalMat, &DropVector, &DropVector ); /* Calc Drop Vector */
			Pos.x = ( DropVector.x * ( 75.0F * GLOBAL_SCALE ) );
			Pos.y = ( DropVector.y * ( 75.0F * GLOBAL_SCALE ) );
			Pos.z = ( DropVector.z * ( 75.0F * GLOBAL_SCALE ) );
			i = InitOneSecBull( OWNER_SHIP, Ship, ID, Ships[ Ship ].Object.Group,
								&Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
								&DropVector, Ships[ Ship ].Secondary, FALSE );
			break;
	}

	if( i != (uint16) -1 )
	{
		SecBulls[ i ].FramelagAddition = GlobalFramelagAddition;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Destroy secondary missile/mine and clean up.
	Input		:	uint16		Secondary weapon index
				:	VECTOR	*	Detonation point
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DestroySecondary( uint16 i, VECTOR * Int_Point )
{
	if( MyGameStatus != STATUS_SinglePlayer )
	{
		if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SecFire ) &&
			( SecBulls[ i ].OwnerType == OWNER_SHIP ) &&
			( SecBulls[ i ].Owner == WhoIAm ) &&
			( SecBulls[ i ].ID == Ships[ WhoIAm ].SecBullIdCount ) )
		{
			return;
		}
	}

	switch( SecBulls[ i ].Weapon )
	{
		case ENEMYSPIRALMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
			break;

		case ENEMYHOMINGMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
			break;

		case ENEMYBLUEHOMINGMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
			break;

		case ENEMYFIREBALL:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
			break;

		case ENEMYDEPTHCHARGE:
			PlayPannedSfx( SFX_BangBang, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondarySend( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, DEPTHCHARGE_SHOCKWAVE );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, DEPTHCHARGE_SHOCKWAVE );
			break;

   		case MUGMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;
   
   		case SOLARISMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;
   
   		case THIEFMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
 			CleanUpSecBull( i );
   			break;

		case ENEMYTENTACLE:
   			CleanUpSecBull( i );
   			break;
			
   		case SCATTERMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;
   
   		case GRAVGONMISSILE:
   			SecBulls[i].ColPoint.x = Int_Point->x;
   			SecBulls[i].ColPoint.y = Int_Point->y;
   			SecBulls[i].ColPoint.z = Int_Point->z;
   			CreateGravgonField( i );
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;
   
   		case MULTIPLEMISSILE:
 			CreateSmallExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;
   
   		case TITANSTARMISSILE:
 			CreateExplosion( Int_Point, SecBulls[i].GroupImIn );
			CleanUpSecBull( i );
   			break;

   		case PINEMISSILE:
 			CreateSmallExplosion( Int_Point, SecBulls[i].GroupImIn );
   			CleanUpSecBull( i );
   			break;

		case TITANSTARSHRAPNEL:
			PlayPannedSfx( SFX_Exp3, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			if( ( SecBulls[ i ].OwnerType == OWNER_SHIP ) && ( SecBulls[ i ].Owner == WhoIAm ) )
				ExplodeSecondarySend( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, TITANSTAR_SHOCKWAVE );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, TITANSTAR_SHOCKWAVE );
   			break;

		case PINEMINE:
			PlayPannedSfx( SFX_Exp5, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondarySend( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, ( PINE_SHOCKWAVE + ( SecBulls[ i ].Ammo / 2 ) ) );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, ( PINE_SHOCKWAVE + ( SecBulls[ i ].Ammo / 2 ) ) );
			break;
	  	
		case PURGEMINE:
			PlayPannedSfx( SFX_Exp4, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondarySend( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, PURGE_SHOCKWAVE );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, PURGE_SHOCKWAVE );
			break;
	  
		case QUANTUMMINE:
			PlayPannedSfx( SFX_QuantumMineExplode, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondarySend( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, QUANTUM_SHOCKWAVE );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, QUANTUM_SHOCKWAVE );
			break;
	
		case SPIDERMINE:
			PlayPannedSfx( SFX_Exp6, SecBulls[i].GroupImIn , &SecBulls[ i ].Pos, 0.0F );
			ExplodeSecondarySend( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, SPIDER_SHOCKWAVE );
			ExplodeSecondary( &SecBulls[ i ].Pos, SecBulls[ i ].GroupImIn, SecBulls[ i ].OwnerType, SecBulls[ i ].Owner, SecBulls[ i ].ID, SPIDER_SHOCKWAVE );
			break;

		default:
			CleanUpSecBull( i );
			break;
   	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Generate Mine list for new player
	Input		:	uint16			Ship
				:	SHORTMINE	*	Mine Array to be filled in
				:	BYTE		*	Mine Number to be filled in
				:	BYTE			Section of Mines
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void GenMineList( uint16 Ship, SHORTMINE * MineSlots, BYTE * NumMines, BYTE Section )
{
	int16	Count;
	int16	i;
	int16	NumUsed = 0;

	LONGLONG	Time_Freq;
	LONGLONG	Time_Diff;
	LONGLONG	Time_Value;
	float		Diff_Float;

	QueryPerformanceCounter((LARGE_INTEGER *) &Time_Value);
	QueryPerformanceFrequency((LARGE_INTEGER *) &Time_Freq);
	Time_Diff = ( Time_Value - Time_LastValue );
	Diff_Float = (float) ( Time_Diff * 1000.0F / Time_Freq ) / ticksperframe;

	Section = ( ( ( MAXSECONDARYWEAPONBULLETS + ( MAXGENMINECOUNT - 1 ) ) / MAXGENMINECOUNT ) - Section );

	for( Count = 0; Count < MAXGENMINECOUNT; Count++ )
	{
		i = ( Count + ( Section * MAXGENMINECOUNT ) );

		if( i < MAXSECONDARYWEAPONBULLETS )
		{
			if( MinesCopy[ Ship ][ i ].Owner != (uint16) -1 )
			{
				MineSlots[ NumUsed ].Owner		= MinesCopy[ Ship ][ i ].Owner;
				MineSlots[ NumUsed ].Group		= MinesCopy[ Ship ][ i ].Group;
				MineSlots[ NumUsed ].Pos		= MinesCopy[ Ship ][ i ].Pos;
				MineSlots[ NumUsed ].Offset		= MinesCopy[ Ship ][ i ].Offset;
				MineSlots[ NumUsed ].LifeCount	= ( MinesCopy[ Ship ][ i ].LifeCount + Diff_Float );
				MineSlots[ NumUsed ].Dir		= MinesCopy[ Ship ][ i ].Dir;
				MineSlots[ NumUsed ].UpVector	= MinesCopy[ Ship ][ i ].UpVector;
				MineSlots[ NumUsed ].DropDir	= MinesCopy[ Ship ][ i ].DropDir;
				MineSlots[ NumUsed ].BulletID	= MinesCopy[ Ship ][ i ].BulletID;
				MineSlots[ NumUsed ].Weapon		= MinesCopy[ Ship ][ i ].Weapon;
				NumUsed++;
			}
		}
	}

	*NumMines = (BYTE) NumUsed;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Generate Mines from list for new player
	Input		:	SHORTMINE	*	Mine Array
				:	BYTE			Number of Mines
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void RegenMineList( SHORTMINE * Slots, BYTE Num )
{
	int16		Count;
	uint16		i;

	for( Count = 0; Count < Num; Count++ )
	{
		i = InitOneSecBull( OWNER_SHIP, (BYTE) Slots[ Count ].Owner, Slots[ Count ].BulletID,
							Slots[ Count ].Group, &Slots[ Count ].Pos,
							&Slots[ Count ].Offset, &Slots[ Count ].Dir, &Slots[ Count ].UpVector,
							&Slots[ Count ].DropDir, (int8) Slots[ Count ].Weapon, FALSE );
		if( i != (uint16) -1 )
		{
			SecBulls[i].LifeCount = Slots[ Count ].LifeCount;
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Copy Mines into copy of mines
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SyncMines( void )
{
	uint16		i;
	uint16		Next;
	LONGLONG	Time_Freq;
	LONGLONG	Time_Diff;
	LONGLONG	Time_Value;
	float		Diff_Float;

	QueryPerformanceCounter((LARGE_INTEGER *) &Time_Value);
	QueryPerformanceFrequency((LARGE_INTEGER *) &Time_Freq);
	Time_Diff = ( Time_Value - Time_LastValue );
	Diff_Float = (float) ( Time_Diff * 1000.0F / Time_Freq ) / ticksperframe;

	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( SecBulls[ i ].SecType == SEC_MINE )
		{
			SecBulls[i].LifeCount += Diff_Float;
		}

		i = Next;
	}																				
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Regenerate qued Mines
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void RegenerateQuedMines( void )
{
	int16				Count;
	SHORTMINE		*	I_Ptr;
	SHORTKILLMINE	*	K_Ptr;

	if( !Ships[ WhoIAm ].Mines && NumMissedMines )
	{
		for( Count = 0; Count < NumMissedMines; Count++ )
		{
			switch( MissedMines[ Count ].Type )
			{
				case MM_TYPE_INIT:
					I_Ptr = (SHORTMINE *) MissedMines[ Count ].Struct;

					DebugPrintf( "Regenerated MissedInitMine() %d\n", Count );

					InitOneSecBull( I_Ptr->OwnerType, I_Ptr->Owner, I_Ptr->BulletID,
									I_Ptr->Group, &I_Ptr->Pos, &I_Ptr->Offset, &I_Ptr->Dir,
									&I_Ptr->UpVector, &I_Ptr->DropDir, (char) I_Ptr->Weapon, FALSE );
					break;

				case MM_TYPE_KILL:
					K_Ptr = (SHORTKILLMINE *) MissedMines[ Count ].Struct;
					KillOwnerIDSecBulls( K_Ptr->OwnerType, K_Ptr->Owner, K_Ptr->ID );
					break;
			}
		}

		NumMissedMines = 0;
		NumMissedInitMines = 0;
		NumMissedKillMines = 0;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Copy Mines into copy of mines
	Input		:	uint16		Player
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CopyMines( uint16 Player )
{
	uint16	i;
	uint16	Next;
	int16	Num = 0;

	QueryPerformanceCounter((LARGE_INTEGER *) &Time_LastValue);

	for( i = 0; i < MAXSECONDARYWEAPONBULLETS; i++ )
	{
		MinesCopy[ Player ][ i ].Owner = (uint16) -1;
	}

	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( SecBulls[ i ].SecType == SEC_MINE )
		{
			MinesCopy[ Player ][ Num ].Owner	= SecBulls[ i ].Owner;
			MinesCopy[ Player ][ Num ].Group	= SecBulls[ i ].GroupImIn;
			MinesCopy[ Player ][ Num ].Pos		= SecBulls[ i ].Pos;
			MinesCopy[ Player ][ Num ].Offset	= SecBulls[ i ].Offset;
			MinesCopy[ Player ][ Num ].LifeCount= SecBulls[ i ].LifeCount;
			MinesCopy[ Player ][ Num ].Dir		= SecBulls[ i ].DirVector;
			MinesCopy[ Player ][ Num ].UpVector	= SecBulls[ i ].UpVector;
			MinesCopy[ Player ][ Num ].DropDir	= SecBulls[ i ].DropVector;
			MinesCopy[ Player ][ Num ].BulletID	= SecBulls[ i ].ID;
			MinesCopy[ Player ][ Num ].Weapon	= SecBulls[ i ].Weapon;
			Num++;
		}

		i = Next;
	}																				
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Kill mine/missiles by owner/id
	Input		:	uint16		OwnerType
				:	uint16		Owner
				:	uint16		ID
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void KillOwnerIDSecBulls( uint16 OwnerType, uint16 Owner, uint16 ID )
{
	uint16	i;
	uint16	Next;

	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( ( SecBulls[ i ].OwnerType == OwnerType ) && ( SecBulls[ i ].Owner == Owner ) && ( SecBulls[ i ].ID == ID ) )
		{
			if( IsServerGame && IsServer )
			{
				if( SecBulls[i].SecType == SEC_MINE ) Host_SecAmmoUsed[ SecBulls[i].Weapon ]++;
			}
			else
			{
				if( ( SecBulls[i].SecType == SEC_MINE ) && ( Owner == WhoIAm ) ) SecAmmoUsed[ SecBulls[i].Weapon ]++;
			}
 			CleanUpSecBull( i );
		}

		i = Next;
	}																				
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if within range of mine
	Input		:	VECTOR	*	Pos
	Output		:	BOOL		True/False ( True if collision )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL CheckForMines( VECTOR * Pos )
{
	uint16		i;
	uint16		Next;
	VECTOR		DistVector;
	float		DistToMine;

	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		Next = SecBulls[ i ].Prev;							/* Next Secondary Bullet */

		if( SecBulls[ i ].SecType == SEC_MINE )
		{
			DistVector.x = ( Pos->x - SecBulls[ i ].Pos.x );
			DistVector.y = ( Pos->y - SecBulls[ i ].Pos.y );
			DistVector.z = ( Pos->z - SecBulls[ i ].Pos.z );
			DistToMine = VectorLength( &DistVector );

			if( DistToMine < ( BLOCKSIZE * 6.0F ) )			// 6 Blocks
			{
				return TRUE;
			}
		}

		i = Next;
	}																				

	return FALSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Explosion
	Input		:	uint16		Ship
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float	ShipSmokeTimes[ MAX_PLAYERS ];
float	ShipSmokeInterval = 1.0F;

void DoDamagedEffects( uint16 i )
{
	uint16	fmpoly;
	VECTOR	TempUp;
	VECTOR	MoveDir;
	VECTOR	LookDir;
	float	Cos;

	ShipSmokeInterval = (float) ( 11 - TrailDetailSlider.value );

	if( AutoDetail )
	{
		if( avgframelag > 2.0F ) ShipSmokeInterval +=  ( avgframelag - 1.0F );
	}

	if( ( Ships[ i ].enable ) &&
		( Ships[ i ].Object.Mode != LIMBO_MODE ) &&
		( Ships[ i ].Object.Mode != GAMEOVER_MODE ) &&
		( ( GameStatus[i] == STATUS_Normal ) || ( GameStatus[i] == STATUS_SinglePlayer ) ) )
	{
		if( Ships[i].Object.Mode == DEATH_MODE )
		{
			MoveDir = Ships[i].LastMove;
			NormaliseVector( &MoveDir );
			LookDir.x = ( Ships[i].Object.Pos.x + ( MoveDir.x * ( SHIP_RADIUS * 0.65F ) ) );
			LookDir.y = ( Ships[i].Object.Pos.y + ( MoveDir.y * ( SHIP_RADIUS * 0.65F ) ) );
			LookDir.z = ( Ships[i].Object.Pos.z + ( MoveDir.z * ( SHIP_RADIUS * 0.65F ) ) );
			MoveDir.x = -MoveDir.x;
			MoveDir.y = -MoveDir.y;
			MoveDir.z = -MoveDir.z;
			CreateSpotFXBurning( &LookDir, &MoveDir, Ships[ i ].Object.Group );
		}

		if( Ships[ i ].Object.Hull < 64 )
		{
			if( IsGroupVisible[ Ships[ i ].Object.Group ] )
			{
				ShipSmokeTimes[ i ] += framelag;
	
				if( ShipSmokeTimes[ i ] >= ShipSmokeInterval )
				{
					ShipSmokeTimes[ i ] = 0.0F;
	
					MoveDir = Ships[i].LastMove;
					ApplyMatrix( &Ships[ i ].Object.FinalMat, &Forward, &LookDir );
					NormaliseVector( &MoveDir );
					NormaliseVector( &LookDir );
					
					Cos = DotProduct( &MoveDir, &LookDir );
					
					if( Cos > -0.1F )														// Only when moving forward
					{
						fmpoly = FindFreeFmPoly();
					
						if( fmpoly != (uint16 ) -1 )
						{
							FmPolys[ fmpoly ].LifeCount = 1000.0F;
							ApplyMatrix( &Ships[ i ].Object.FinalMat, &Backward, &FmPolys[ fmpoly ].Dir );
							ApplyMatrix( &Ships[ i ].Object.FinalMat, &SlideUp, &TempUp );
							FmPolys[ fmpoly ].UpVector = SlideUp;
							FmPolys[ fmpoly ].Pos.x = ( Ships[ i ].Object.Pos.x + ( FmPolys[ fmpoly ].Dir.x * ( 250.0F * GLOBAL_SCALE ) ) + ( TempUp.x * ( -26.0F * GLOBAL_SCALE ) ) );
							FmPolys[ fmpoly ].Pos.y = ( Ships[ i ].Object.Pos.y + ( FmPolys[ fmpoly ].Dir.y * ( 250.0F * GLOBAL_SCALE ) ) + ( TempUp.y * ( -26.0F * GLOBAL_SCALE ) ) );
							FmPolys[ fmpoly ].Pos.z = ( Ships[ i ].Object.Pos.z + ( FmPolys[ fmpoly ].Dir.z * ( 250.0F * GLOBAL_SCALE ) ) + ( TempUp.z * ( -26.0F * GLOBAL_SCALE ) ) );
							FmPolys[ fmpoly ].SeqNum = FM_BLACK_SMOKE;															   
							FmPolys[ fmpoly ].Frame = 0.0F;
							FmPolys[ fmpoly ].R = ( 64 - (BYTE)Ships[ i ].Object.Hull ) * 3;
							FmPolys[ fmpoly ].G = ( 64 - (BYTE)Ships[ i ].Object.Hull ) * 3;
							FmPolys[ fmpoly ].B = ( 64 - (BYTE)Ships[ i ].Object.Hull ) * 3;
#if ACTUAL_TRANS
							FmPolys[ fmpoly ].Trans = 128;
#else
							FmPolys[ fmpoly ].Trans = 255;
#endif
							FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
							FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
							FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
							FmPolys[ fmpoly ].xsize = SMOKE_TRAIL_SIZE;
							FmPolys[ fmpoly ].ysize = SMOKE_TRAIL_SIZE;
							FmPolys[ fmpoly ].Speed = ( ( ( ( (float) Random_Range( 4000 ) ) / 1000.0F ) + 2.0F ) * GLOBAL_SCALE ); //( 16.0F * GLOBAL_SCALE );
							FmPolys[ fmpoly ].UpSpeed = 0.0F;
							FmPolys[ fmpoly ].Group = Ships[ i ].Object.Group;
							AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
						}
					}
				}
			}
		}
		else
		{
			ShipSmokeTimes[ i ] = 0.0F;
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Drop Currently selected secondary weapon
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DropSecondary( void )
{
	float	Speed;
	float	LifeCount;
	int16	Count = 0;
	int16	PickupNum;
	int16	Secondary;
	VECTOR	DirVector;

	Secondary = Ships[ WhoIAm ].Secondary;

	switch( Secondary )
	{
		case QUANTUMMINE:
			if( SecondaryWeaponsGot[ Secondary ] && SecondaryAmmo[ Secondary ] )
			{
				PickupNum = PICKUP_QuantumPickup;
				Count = 1;
			}
			break;

		case GRAVGONMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && SecondaryAmmo[ Secondary ] )
			{
				PickupNum = PICKUP_Gravgon;
				Count = 1;
			}
			break;

		case TITANSTARMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && SecondaryAmmo[ Secondary ] )
			{
				PickupNum = PICKUP_TitanStar;
				Count = 1;
			}
			break;

		case SCATTERMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && SecondaryAmmo[ Secondary ] )
			{
				PickupNum = PICKUP_Scatter;
				Count = 1;
			}
			break;

		case THIEFMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && SecondaryAmmo[ Secondary ] )
			{
				PickupNum = PICKUP_Thief;
				Count = 1;
			}
			break;

		case SPIDERMINE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 3 ) )
			{
				PickupNum = PICKUP_SpiderPod;
				Count = 3;
			}
			break;

		case MULTIPLEMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 50 ) )
			{
				PickupNum = PICKUP_Launcher;
				Count = 50;
			}
			break;

		case SOLARISMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 3 ) )
			{
				PickupNum = PICKUP_HeatseakerPickup;
				Count = 3;
			}
			break;

		case PINEMINE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 3 ) )
			{
				PickupNum = PICKUP_PinePickup;
				Count = 3;
			}
			break;

		case MUGMISSILE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 3 ) )
			{
				PickupNum = PICKUP_Mugs;
				Count = 3;
			}
			break;

		case PURGEMINE:
			if( SecondaryWeaponsGot[ Secondary ] && ( SecondaryAmmo[ Secondary ] >= 3 ) )
			{
				PickupNum = PICKUP_PurgePickup;
				Count = 3;
			}
			break;
	}

	if( Count )
	{
		SecondaryAmmo[ Secondary ] -= Count;
		if( !SecondaryAmmo[ Secondary ] ) SecondaryWeaponsGot[ Secondary ] = 0;
		ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &DirVector );
		LifeCount = -1.0F;
		Speed = PICKUP_SPEED;
		InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create TitanStar Shrapnel
	Input		:	uint16		SecBull Index
				:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateTitanStarShrapnel( uint16 i, VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	int16	Count;
	int16	NumLeft;
	int16	ShipTargCount;
	VECTOR	UpVector = { 0.0F, 1.0F, 0.0F };
	VECTOR	DropVector = { 0.0F, 0.0F, 0.0F };
	VECTOR	DirVector;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;
	BYTE	MyTeam = 0;
	BYTE	ShipsTeam = 1;

	uint16	FirstID;
	VECTOR	Directions[ NUMTITANBITS ];

	if( SecBulls[i].OwnerType == OWNER_SHIP )
	{
		if( TeamGame ) MyTeam = TeamNumber[ SecBulls[i].Owner ];
	}

	NumLeft = NUMTITANBITS;
	ShipTargCount = 0;

	FirstID = Ships[ SecBulls[i].Owner ].SecBullIdCount + 1;

	while( NumLeft )
	{
		for( Count = 0; Count < MAX_PLAYERS; Count++ )
		{
			if( TeamGame ) ShipsTeam = TeamNumber[ Count ];

			if( ( ( SecBulls[i].OwnerType == OWNER_SHIP ) && ( Count != SecBulls[i].Owner ) && ( MyTeam != ShipsTeam ) ) || ( SecBulls[i].OwnerType != OWNER_SHIP ) )
			{
				if( NumLeft )
				{
					if( (Ships[Count].enable ) && (Ships[Count].Object.Mode != LIMBO_MODE) && ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) ) && !Ships[Count].Invul )
					{
						if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
						{
							DirVector.x = ( Ships[ Count ].Object.Pos.x - SecBulls[i].Pos.x );
							DirVector.y = ( Ships[ Count ].Object.Pos.y - SecBulls[i].Pos.y );
							DirVector.z = ( Ships[ Count ].Object.Pos.z - SecBulls[i].Pos.z );
							NormaliseVector( &DirVector );

							if( ShipTargCount )
							{
								DirVector.x += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
					   			DirVector.y += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
					   			DirVector.z += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
								NormaliseVector( &DirVector );
							}

//							InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
//											Group, Pos, &DropVector, &DirVector, &UpVector,
//											&DropVector, TITANSTARSHRAPNEL, TRUE );
							InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
											Group, Pos, &DropVector, &DirVector, &UpVector,
											&DropVector, TITANSTARSHRAPNEL, FALSE );
							Directions[ NumLeft-1 ] = DirVector;
							NumLeft--;
						}
					}
				}
			}
		}


		Enemy = FirstEnemyUsed;
	
		while( ( Enemy != NULL ) && ( NumLeft ) )
		{
			NextEnemy = Enemy->NextUsed;
	
			if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
			{
				if( !( ( SecBulls[i].OwnerType == OWNER_ENEMY ) && ( SecBulls[i].Owner == Enemy->Index ) ) )
				{
					if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
					{
						DirVector.x = ( Enemy->Object.Pos.x - SecBulls[i].Pos.x );
						DirVector.y = ( Enemy->Object.Pos.y - SecBulls[i].Pos.y );
						DirVector.z = ( Enemy->Object.Pos.z - SecBulls[i].Pos.z );
						NormaliseVector( &DirVector );

						if( ShipTargCount )
						{
							DirVector.x += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
							DirVector.y += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
							DirVector.z += ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
							NormaliseVector( &DirVector );
						}

//						InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
//										Group, Pos, &DropVector, &DirVector, &UpVector,
//										&DropVector, TITANSTARSHRAPNEL, TRUE );
						InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
										Group, Pos, &DropVector, &DirVector, &UpVector,
										&DropVector, TITANSTARSHRAPNEL, FALSE );
						Directions[ NumLeft-1 ] = DirVector;

						Enemy->AIFlags |= AI_TARGETED_ONEOFF;
						Enemy->TargetingPos = SecBulls[ i ].Pos;
						Enemy->TargetingDir = SecBulls[ i ].DirVector;

						NumLeft--;
					}
				}
			}
			Enemy = NextEnemy;
		}

		ShipTargCount++;

		if( NumLeft == NUMTITANBITS )
		{
			for( Count = 0; Count < NUMTITANBITS; Count++ )
			{
				DirVector.x = Dir->x + ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
		   		DirVector.y = Dir->y + ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
		   		DirVector.z = Dir->z + ( ( ( (float) Random_Range( 10000 ) ) / 12500.0F ) - 0.4F );
				NormaliseVector( &DirVector );
		
//				InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
//								Group, Pos, &DropVector, &DirVector, &UpVector,
//								&DropVector, TITANSTARSHRAPNEL, TRUE );
				InitOneSecBull( SecBulls[i].OwnerType, (BYTE) SecBulls[i].Owner, ++Ships[ SecBulls[i].Owner ].SecBullIdCount,
								Group, Pos, &DropVector, &DirVector, &UpVector,
								&DropVector, TITANSTARSHRAPNEL, FALSE );
				Directions[ Count ] = DirVector;

			}
			break;
		}
	}

	if( ( SecBulls[ i ].OwnerType == OWNER_SHIP ) && ( SecBulls[ i ].Owner == WhoIAm ) )
	{
		TitanBitsSend( SecBulls[i].OwnerType, SecBulls[i].Owner, FirstID, Group, Pos,
							&DropVector, &UpVector, &DropVector, TITANSTARSHRAPNEL, &Directions[ 0 ] );
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create New Smoke Ring
	Input		:	VECTOR	*	Position
				:	VECTOR	*	DirVector
				:	VECTOR	*	UpVector
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateSmokeRing( VECTOR * Pos, VECTOR * DirVector, VECTOR * UpVector, uint16 Group )
{
	int16	Count;
	VECTOR	TempDir;
	QUAT	TempQuat;
	MATRIX	TempMat;
	uint16	fmpoly;

	QuatFromDirAndUp( DirVector, UpVector, &TempQuat );
	QuatToMatrix( &TempQuat, &TempMat );

	for( Count = 0; Count < 20; Count++ )
	{
		TempDir.x = (float) sin( D2R( Count * 18 ) );
		TempDir.y = 0.0F;
		TempDir.z = (float) cos( D2R( Count * 18 ) );

		ApplyMatrix( &TempMat, &TempDir, &TempDir );

		fmpoly = FindFreeFmPoly();

		if( fmpoly != (uint16 ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].SeqNum = FM_SMOKE_RING;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].AnimSpeed = 8.0F;
			FmPolys[ fmpoly ].R = 255;
			FmPolys[ fmpoly ].G = 255;
			FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
			FmPolys[ fmpoly ].Frm_Info = &NewTrail_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			FmPolys[ fmpoly ].xsize = NEW_TRAIL_SIZE * 4.0F;
			FmPolys[ fmpoly ].ysize = NEW_TRAIL_SIZE * 4.0F;
			FmPolys[ fmpoly ].Dir = TempDir;
			FmPolys[ fmpoly ].Speed = ( MISSILE_SPEED * 0.4F );
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create New Smoke Ring
	Input		:	VECTOR	*	Position
				:	VECTOR	*	DirVector
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateSplash( VECTOR * Pos, VECTOR * DirVector, uint16 Group )
{
	int16	Count;
	VECTOR	TempDir;
	uint16	fmpoly;
	VECTOR	RevDir;
	VECTOR	NewPos;
	float	Scalar;
	uint8	Red;
	uint8	Green;
	uint8	Blue;

	GetWaterColour( Group, &Red, &Green, &Blue );

	NewPos.x = Pos->x;
	NewPos.y = Pos->y + 1.0F;
	NewPos.z = Pos->z;

	RevDir.x = -DirVector->x;
	RevDir.y = -DirVector->y;
	RevDir.z = -DirVector->z;
	NormaliseVector( &RevDir );

	for( Count = 0; Count < 20; Count++ )
	{
		TempDir.x = (float) cos( D2R( Count * 18 ) );
		TempDir.y = 0.0F;
		TempDir.z = (float) sin( D2R( Count * 18 ) );

		Scalar = 1.5F + ( DotProduct( &TempDir, &RevDir ) * -( ( ( (float) Random_Range( 10000 ) ) / 2000.0F ) ) + 1.5F );
		TempDir.y = 1.0F;

		NormaliseVector( &TempDir );
		TempDir.x *= Scalar;
		TempDir.y *= Scalar;
		TempDir.z *= Scalar;

		fmpoly = FindFreeFmPoly();
		
		if( fmpoly != (uint16 ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = ( (float) Random_Range( 180 ) ) + 60.0F;
			FmPolys[ fmpoly ].Pos = NewPos;
			FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_WATERSPLASH;
			FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].AnimSpeed = ( ( (float) Random_Range( 10000 ) ) / 1000.0F ) + 8.0F;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Trans = 255;
			FmPolys[ fmpoly ].Dir = TempDir;
			FmPolys[ fmpoly ].R = ( 32 + ( Red / 2 ) );
			FmPolys[ fmpoly ].G = ( 32 + ( Green / 2 ) );
			FmPolys[ fmpoly ].B = ( 32 + ( Blue / 2 ) );
			FmPolys[ fmpoly ].Start_R = FmPolys[ fmpoly ].R;
			FmPolys[ fmpoly ].Start_G = FmPolys[ fmpoly ].G;
			FmPolys[ fmpoly ].Start_B = FmPolys[ fmpoly ].B;
			FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = ( ( ( (float) Random_Range( 16 ) ) + 8.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].ysize = FmPolys[ fmpoly ].xsize;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}

	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Find Enemy closest to center of target
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	uint16		Group
				:	uint16		My Type
				:	uint16		My Index
				:	float		View Cone Cosine
				:	float	*	Closest Cos
	Output		:	uint16		Enemy Index (-1 if none)
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 TargetClosestEnemy( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 MeType, uint16 Me, float ViewConeCos, float * ClosestCos )
{
	float		Cos;
	uint16		ClosestEnemy = (uint16) -1;
	VECTOR		TempVector;
	VECTOR		DirVector;
	VECTOR		NormVector;
	VECTOR		Int_Point;
	NORMAL		Int_Normal;
	uint16		Int_Group;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;
	uint16		TempEnemyIndex;

	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
		{
			if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
			{
				DirVector.x = ( Enemy->Object.Pos.x - Pos->x );
				DirVector.y = ( Enemy->Object.Pos.y - Pos->y );
				DirVector.z = ( Enemy->Object.Pos.z - Pos->z );
				NormVector = DirVector;
				NormaliseVector( &NormVector );
				
				Cos = DotProduct( &NormVector, Dir );
			
				if( ( Cos >= ViewConeCos ) && ( Cos > *ClosestCos ) )
				{
					if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
											&Int_Point, &Int_Group, &Int_Normal, &TempVector, TRUE, NULL ) )
					{
						*ClosestCos = Cos;
						ClosestEnemy = Enemy->Index;
					}
					else
					{
						TempEnemyIndex = CheckAnyEnemyHit();

						if( TempEnemyIndex == Enemy->Index )
						{
							*ClosestCos = Cos;
							ClosestEnemy = TempEnemyIndex;
						}
					}
				}
			}
		}

		Enemy = NextEnemy;
	}
	return( ClosestEnemy );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Find Secondary closest to center of target
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	uint16		Group
				:	uint16		My Type
				:	uint16		My Index
				:	float		View Cone Cosine
				:	float	*	Closest Cos
	Output		:	uint16		SecBulls (Secondary) Index (-1 if none)
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 TargetClosestSecondary( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 MeType, uint16 Me, float ViewConeCos, float * ClosestCos )
{
	float		Cos;
	uint16		ClosestSecondary = (uint16) -1;
	VECTOR		TempVector;
	VECTOR		DirVector;
	VECTOR		NormVector;
	VECTOR		Int_Point;
	NORMAL		Int_Normal;
	uint16		Int_Group;

	int16		Count;
	GROUPLIST * GroupsVisible;
	uint16	  * GroupList;
	uint16		CurrentGroup;
	SECONDARYWEAPONBULLET * SecBull;
	BYTE		MyTeam = 0;
	BYTE		ShipsTeam = 1;

	if( MeType == OWNER_SHIP )
	{
		if( TeamGame ) MyTeam = TeamNumber[ Me ];
	}

	GroupsVisible = VisibleGroups( Group );

	GroupList = GroupsVisible->group;

	for( Count = 0; Count < GroupsVisible->groups; Count++ )
	{
		CurrentGroup = GroupList[ Count ];

		SecBull = SecBullGroups[ CurrentGroup ];

		while( SecBull )
		{
			if( SecBull->Flags & SECFLAGS_CanTarget )
			{
				if( TeamGame ) ShipsTeam = TeamNumber[ SecBull->Owner ];

				if( ( TeamGame && ( MyTeam != ShipsTeam ) ) || !TeamGame )
				{
					if( ( SecBull->OwnerType != MeType ) || ( SecBull->Owner != Me ) )
					{
						DirVector.x = ( SecBull->Pos.x - Pos->x );
						DirVector.y = ( SecBull->Pos.y - Pos->y );
						DirVector.z = ( SecBull->Pos.z - Pos->z );
						NormVector = DirVector;
						NormaliseVector( &NormVector );
							
						Cos = DotProduct( &NormVector, Dir );
					
						if( ( Cos >= ViewConeCos ) && ( Cos > *ClosestCos ) )
						{
							if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &DirVector,
													&Int_Point, &Int_Group, &Int_Normal, &TempVector, TRUE, NULL ) )
							{
								*ClosestCos = Cos;
								ClosestSecondary = SecBull->Index;
							}
						}
					}
				}
			}

			SecBull = SecBull->NextInGroup;
		}
	}

	return( ClosestSecondary );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Test proximity to ships
	Input		:	VECTOR	*	Pos
				:	uint16		Group
				:	float		Radius
				:	uint16		My Type
				:	uint16		My Index ( -1 Even Me )
	Output		:	BOOL		True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL CheckProximityToShips( VECTOR * Pos, uint16 Group, float Radius, uint16 OwnerType, uint16 Owner )
{ 
	uint16	Count;
	VECTOR	TempVector;
	VECTOR	IntPoint;
	NORMAL	IntPointNormal;
	uint16	EndGroup;
	float	DistToShip;
	VECTOR	DirVector;

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		if( !( ( OwnerType == OWNER_SHIP ) && ( Owner == Count ) ) )
   		{
			if( (Ships[Count].enable ) && (Ships[Count].Object.Mode != LIMBO_MODE) && ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) ) )
   			{
				if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
				{
   					DirVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
   					DirVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
   					DirVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
   					
   					DistToShip = VectorLength( &DirVector );
   					
   					if( DistToShip < Radius )
   					{
   						if( BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group,
   										&DirVector, (VECTOR *) &IntPoint, &EndGroup, &IntPointNormal,
										&TempVector, TRUE, NULL ) == FALSE )
   						{
							return( TRUE );
   						}
   					}
				}
   			}
   		}
	}
	return( FALSE );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Test proximity to Enemies
	Input		:	VECTOR	*	Pos
				:	uint16		Group
				:	float		Radius
				:	uint16		My Type
				:	uint16		My Index ( -1 Even Me )
	Output		:	BOOL		True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL CheckProximityToEnemies( VECTOR * Pos, uint16 Group, float Radius, uint16 OwnerType, uint16 Owner )
{ 
	VECTOR		TempVector;
	VECTOR		IntPoint;
	NORMAL		IntPointNormal;
	uint16		EndGroup;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;
	float		DistToEnemy;
	VECTOR		DirVector;
	uint16		TempEnemyIndex;

	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( !( ( OwnerType == OWNER_ENEMY ) && ( Owner == Enemy->Index ) ) )
		{
			if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
   			{
				if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
				{
					DirVector.x = ( Enemy->Object.Pos.x - Pos->x );
  					DirVector.y = ( Enemy->Object.Pos.y - Pos->y );
   					DirVector.z = ( Enemy->Object.Pos.z - Pos->z );
   					
   					DistToEnemy = VectorLength( &DirVector );
   					
   					if( DistToEnemy < Radius )
   					{
   						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group,
   										&DirVector, (VECTOR *) &IntPoint, &EndGroup, &IntPointNormal,
										&TempVector, TRUE, NULL ) )
   						{
							return( TRUE );
   						}
						else
						{
							TempEnemyIndex = CheckAnyEnemyHit();

							if( TempEnemyIndex == Enemy->Index )
							{
								return( TRUE );
							}
						}
   					}
   				}
   			}
		}

		Enemy = NextEnemy;
	}
	return( FALSE );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Test proximity to Enemies
	Input		:	VECTOR	*	Pos
				:	uint16		Group
				:	float	*	Radius
				:	uint16		My Type
				:	uint16		My Index ( -1 Even Me )
	Output		:	uint16		Enemy
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 GetClosestEnemy( VECTOR * Pos, uint16 Group, float * Radius, uint16 OwnerType, uint16 Owner )
{ 
	VECTOR		TempVector;
	VECTOR		IntPoint;
	NORMAL		IntPointNormal;
	uint16		EndGroup;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;
	float		DistToEnemy;
	VECTOR		DirVector;
	uint16		Closest = (uint16) -1;
	uint16		TempEnemyIndex;

	Enemy = FirstEnemyUsed;

	while( Enemy != NULL )
	{
		NextEnemy = Enemy->NextUsed;

		if( !( ( OwnerType == OWNER_ENEMY ) && ( Owner == Enemy->Index ) ) )
		{
			if( ( Enemy->Status & ENEMY_STATUS_Enable ) )
   			{
				if( !SoundInfo[ Enemy->Object.Group ][ Group ] )
				{
					DirVector.x = ( Enemy->Object.Pos.x - Pos->x );
  					DirVector.y = ( Enemy->Object.Pos.y - Pos->y );
   					DirVector.z = ( Enemy->Object.Pos.z - Pos->z );
   					
   					DistToEnemy = VectorLength( &DirVector );
   					
   					if( DistToEnemy < *Radius )
   					{
   						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group,
   										&DirVector, (VECTOR *) &IntPoint, &EndGroup, &IntPointNormal,
										&TempVector, TRUE, NULL ) )
   						{
							Closest = Enemy->Index;
							*Radius = DistToEnemy;
   						}
						else
						{
							TempEnemyIndex = CheckAnyEnemyHit();

							if( TempEnemyIndex == Enemy->Index )
							{
								Closest = TempEnemyIndex;
								*Radius = DistToEnemy;
							}
						}
   					}
   				}
   			}
		}

		Enemy = NextEnemy;
	}
	return( Closest );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create FireBall
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateFireBall( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	int16	Count;
	uint16	fmpoly;
	VECTOR	Offset;
	float	Dist;

	for( Count = 0; Count < 10; Count++ )
	{
		Offset.x = ( ( (float) Random_Range( 10000 ) / 5000.0F ) - 1.0F );	// -1 to +1
		Offset.y = ( ( (float) Random_Range( 10000 ) / 5000.0F ) - 1.0F );
		Offset.z = ( ( (float) Random_Range( 10000 ) / 5000.0F ) - 1.0F );
		NormaliseVector( &Offset );
		Dist = (float) Random_Range( 40 );
		Offset.x *= Dist;
		Offset.y *= Dist;
		Offset.z *= Dist;
		Offset.x += ( Dir->x * 20.0F );
		Offset.y += ( Dir->y * 20.0F );
		Offset.z += ( Dir->z * 20.0F );

		fmpoly = FindFreeFmPoly();

		if( fmpoly != (uint16 ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = Dist;
			FmPolys[ fmpoly ].Pos.x = ( Pos->x + Offset.x );
			FmPolys[ fmpoly ].Pos.y = ( Pos->y + Offset.y );
			FmPolys[ fmpoly ].Pos.z = ( Pos->z + Offset.z );
			FmPolys[ fmpoly ].SeqNum = FM_FIREBALL;
	   		FmPolys[ fmpoly ].AnimSpeed = ( 2.0F - ( Dist / 22.0F ) );
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].R = 128;
			FmPolys[ fmpoly ].G = 64;
			FmPolys[ fmpoly ].B = 64;
			FmPolys[ fmpoly ].Start_R = 128;
			FmPolys[ fmpoly ].Start_G = 64;
			FmPolys[ fmpoly ].Start_B = 64;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
			FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Dir.x = -Dir->x;
			FmPolys[ fmpoly ].Dir.y = -Dir->y;
			FmPolys[ fmpoly ].Dir.z = -Dir->z;
			NormaliseVector( &FmPolys[ fmpoly ].Dir );
			FmPolys[ fmpoly ].Speed = ( -10.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].UpSpeed = 0.0F;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}

	fmpoly = FindFreeFmPoly();

   	if( fmpoly != (uint16 ) -1 )
   	{
   		FmPolys[ fmpoly ].LifeCount = 1000.0F;
   		FmPolys[ fmpoly ].Pos = *Pos;
   		FmPolys[ fmpoly ].SeqNum = FM_ANIMSPEED;
   		FmPolys[ fmpoly ].AnimSpeed = 1.0F;
   		FmPolys[ fmpoly ].Frame = 0.0F;
   		FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
   		FmPolys[ fmpoly ].R = 64;
   		FmPolys[ fmpoly ].G = 64;
   		FmPolys[ fmpoly ].B = 64;
		FmPolys[ fmpoly ].Start_R = 64;
		FmPolys[ fmpoly ].Start_G = 64;
		FmPolys[ fmpoly ].Start_B = 64;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
   		FmPolys[ fmpoly ].Dir = *Dir;
   		FmPolys[ fmpoly ].Speed = 0.0F;
   		FmPolys[ fmpoly ].Group = Group;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Setup Group link lists for secondary weapoons
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SetupSecBullGroups( void )
{
	int16	Count;

	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
		SecBullGroups[ Count ] = NULL;
		NumSecBullsPerGroup[ Count ] = 0;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Add Secondary bullet to group link list
	Input		:	uint16		SecBull Index
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void AddSecBullToGroup( uint16 i, uint16 Group )
{
	if( Group == (uint16) -1 ) return;
	SecBulls[ i ].PrevInGroup = NULL;
	SecBulls[ i ].NextInGroup = SecBullGroups[ Group ];
	if( SecBulls[ i ].NextInGroup ) SecBulls[ i ].NextInGroup->PrevInGroup = &SecBulls[ i ];
	SecBullGroups[ Group ] = &SecBulls[ i ];
	NumSecBullsPerGroup[ Group ]++;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Remove Secondary bullet from group link list
	Input		:	uint16		SecBull Index
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void RemoveSecBullFromGroup( uint16 i, uint16 Group )
{
	if( Group == (uint16) -1 ) return;
	if( SecBulls[ i ].PrevInGroup ) SecBulls[ i ].PrevInGroup->NextInGroup = SecBulls[ i ].NextInGroup;
	else SecBullGroups[ Group ] = SecBulls[ i ].NextInGroup;
	if( SecBulls[ i ].NextInGroup ) SecBulls[ i ].NextInGroup->PrevInGroup = SecBulls[ i ].PrevInGroup;
	SecBulls[ i ].PrevInGroup = NULL;
	SecBulls[ i ].NextInGroup = NULL;
	NumSecBullsPerGroup[ Group ]--;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Move Secondary bullet from 1 group to another
	Input		:	uint16		SecBull Index
				:	uint16		OldGroup
				:	uint16		NewGroup
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void MoveSecBullToGroup( uint16 i, uint16 OldGroup, uint16 NewGroup )
{
	if( OldGroup == (uint16) -1 ) return;
	RemoveSecBullFromGroup( i, OldGroup );
	if( NewGroup == (uint16) -1 ) return;
	AddSecBullToGroup( i, NewGroup );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get Secondary Bullet ID
	Input		:	uint16		OwnerType
				:	uint16		Owner
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 GetSecondaryBulletID( uint16 OwnerType, uint16 Owner )
{
	uint16	BulletID = 0;

	switch( OwnerType )
	{
		case OWNER_NOBODY:
		case OWNER_BGOBJECT:
		case OWNER_MINE:
		case OWNER_MODELSPOTFX:
			BulletID = GlobalSecBullsID++;
			break;

		case OWNER_SHIP:
			BulletID = ++Ships[ Owner ].SecBullIdCount;
			break;

		case OWNER_ENEMY:
			BulletID = ++Enemies[ Owner ].BulletID;
			break;
	}

	return( BulletID );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Save SecBulls Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * SaveSecBulls( FILE * fp )
{
	uint16	i;
	uint16	TempIndex = (uint16) -1;

	if( fp )
	{
		for( i = 0; i < MAXSECONDARYWEAPONS; i++ )
		{
			fwrite( &SecondaryWeaponsGot[ i ], sizeof( int16 ), 1, fp );
			fwrite( &SecAmmoUsed[ i ], sizeof( int16 ), 1, fp );
			fwrite( &SecondaryAmmo[ i ], sizeof( int16 ), 1, fp );
		}

		for( i = 0; i < MAXPICKUPS; i++ )
		{
			fwrite( &SecWeaponsGot[ i ], sizeof( int8 ), 1, fp );
		}

		for( i = 0; i < MAXGROUPS; i++ )
		{
			fwrite( &NumSecBullsPerGroup[ i ], sizeof( int16 ), 1, fp );
			if( SecBullGroups[ i ] ) fwrite( &SecBullGroups[ i ]->Index, sizeof( uint16 ), 1, fp );
			else fwrite( &TempIndex, sizeof( uint16 ), 1, fp );
		}


		fwrite( &FirstSecBullUsed, sizeof( FirstSecBullUsed ), 1, fp );
		fwrite( &FirstSecBullFree, sizeof( FirstSecBullFree ), 1, fp );
		fwrite( &SecondaryFireDelay, sizeof( SecondaryFireDelay ), 1, fp );
		fwrite( &TargetComputerOn, sizeof( TargetComputerOn ), 1, fp );
		fwrite( &ImTargeted, sizeof( ImTargeted ), 1, fp );
		fwrite( &TargetedDelay, sizeof( TargetedDelay ), 1, fp );
		fwrite( &GlobalSecBullsID, sizeof( GlobalSecBullsID ), 1, fp );

		i = FirstSecBullUsed;

		while( i != (uint16) -1 )
		{
			fwrite( &SecBulls[ i ].Used, sizeof( BOOL ), 1, fp );
			fwrite( &SecBulls[ i ].Next, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].Prev, sizeof( uint16 ), 1, fp );
			if( SecBulls[i].NextInGroup ) fwrite( &SecBulls[ i ].NextInGroup->Index, sizeof( uint16 ), 1, fp );
			else fwrite( &TempIndex, sizeof( uint16 ), 1, fp );
			if( SecBulls[i].PrevInGroup ) fwrite( &SecBulls[ i ].PrevInGroup->Index, sizeof( uint16 ), 1, fp );
			else fwrite( &TempIndex, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].Flags, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].Index, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].SecType, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].Type, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].MoveType, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].State, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].OwnerType, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].Owner, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].ID, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].Weapon, sizeof( int8 ), 1, fp );
			fwrite( &SecBulls[ i ].Lensflare, sizeof( BOOL ), 1, fp );
			fwrite( &SecBulls[ i ].LifeCount, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].LifeSpan, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].SpeedInc, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].SpeedWanted, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Speed, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].DropCount, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].DropVector, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].Size, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].Offset, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].StartPos, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].StartDir, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].StartMat, sizeof( MATRIX ), 1, fp );
			fwrite( &SecBulls[ i ].ColStart, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].ColDist, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].ColFlag, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].ColGroup, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].ColPoint, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].ColPointNormal, sizeof( NORMAL ), 1, fp );
			fwrite( &SecBulls[ i ].GroupImIn, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].ModelNum, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].ModelIndex, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fwrite( &SecBulls[ i ].fmpoly, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].numfmpolys, sizeof( int16 ), 1, fp );
			fwrite( &SecBulls[ i ].poly, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].numpolys, sizeof( int16 ), 1, fp );
			fwrite( &SecBulls[ i ].xsize, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].ysize, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].light, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].lightsize, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].r, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].g, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].b, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].TurnSpeed, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].ViewCone, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].TargetType, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].Target, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].DirQuat, sizeof( QUAT ), 1, fp );
			fwrite( &SecBulls[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fwrite( &SecBulls[ i ].Shield, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Damage, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Ammo, sizeof( uint16 ), 1, fp );
			fwrite( &SecBulls[ i ].NumBounces, sizeof( int16 ), 1, fp );
			fwrite( &SecBulls[ i ].NumOldPos, sizeof( int16 ), 1, fp );
			fwrite( &SecBulls[ i ].RetractPos, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].ColRadius, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Interval, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].Time, sizeof( float ), 1, fp );
			fwrite( &SecBulls[ i ].OldPos[ 0 ], sizeof( SecBulls[ i ].OldPos ), 1, fp );
			i = SecBulls[ i ].Prev;
		}

		i = FirstSecBullFree;

		while( i != (uint16) -1 )
		{
			fwrite( &SecBulls[ i ].Next, sizeof( SecBulls[ i ].Next ), 1, fp );
			i = SecBulls[ i ].Next;
		}
	}

	return( fp );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Load SecBulls Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * LoadSecBulls( FILE * fp )
{
	uint16	i;
	uint16	TempIndex = (uint16) -1;

	if( fp )
	{
		for( i = 0; i < MAXSECONDARYWEAPONS; i++ )
		{
			fread( &SecondaryWeaponsGot[ i ], sizeof( int16 ), 1, fp );
			fread( &SecAmmoUsed[ i ], sizeof( int16 ), 1, fp );
			fread( &SecondaryAmmo[ i ], sizeof( int16 ), 1, fp );
		}

		for( i = 0; i < MAXPICKUPS; i++ )
		{
			fread( &SecWeaponsGot[ i ], sizeof( int8 ), 1, fp );
		}

		for( i = 0; i < MAXGROUPS; i++ )
		{
			fread( &NumSecBullsPerGroup[ i ], sizeof( int16 ), 1, fp );
			fread( &TempIndex, sizeof( uint16 ), 1, fp );
			if( TempIndex != (uint16) -1 ) SecBullGroups[ i ] = &SecBulls[ TempIndex ];
			else SecBullGroups[ i ] = NULL;
		}


		fread( &FirstSecBullUsed, sizeof( FirstSecBullUsed ), 1, fp );
		fread( &FirstSecBullFree, sizeof( FirstSecBullFree ), 1, fp );
		fread( &SecondaryFireDelay, sizeof( SecondaryFireDelay ), 1, fp );
		fread( &TargetComputerOn, sizeof( TargetComputerOn ), 1, fp );
		fread( &ImTargeted, sizeof( ImTargeted ), 1, fp );
		fread( &TargetedDelay, sizeof( TargetedDelay ), 1, fp );
		fread( &GlobalSecBullsID, sizeof( GlobalSecBullsID ), 1, fp );

		i = FirstSecBullUsed;

		while( i != (uint16) -1 )
		{
			fread( &SecBulls[ i ].Used, sizeof( BOOL ), 1, fp );
			fread( &SecBulls[ i ].Next, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].Prev, sizeof( uint16 ), 1, fp );

			fread( &TempIndex, sizeof( uint16 ), 1, fp );
			if( TempIndex != (uint16) -1 ) SecBulls[ i ].NextInGroup = &SecBulls[ TempIndex ];
			else SecBulls[ i ].NextInGroup = NULL;
			fread( &TempIndex, sizeof( uint16 ), 1, fp );
			if( TempIndex != (uint16) -1 ) SecBulls[ i ].PrevInGroup = &SecBulls[ TempIndex ];
			else SecBulls[ i ].PrevInGroup = NULL;

			fread( &SecBulls[ i ].Flags, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].Index, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].SecType, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].Type, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].MoveType, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].State, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].OwnerType, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].Owner, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].ID, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].Weapon, sizeof( int8 ), 1, fp );
			fread( &SecBulls[ i ].Lensflare, sizeof( BOOL ), 1, fp );
			fread( &SecBulls[ i ].LifeCount, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].LifeSpan, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].SpeedInc, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].SpeedWanted, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Speed, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].DropCount, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].DropVector, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].Size, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].Offset, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].StartPos, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].StartDir, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].StartMat, sizeof( MATRIX ), 1, fp );
			fread( &SecBulls[ i ].ColStart, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].ColDist, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].ColFlag, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].ColGroup, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].ColPoint, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].ColPointNormal, sizeof( NORMAL ), 1, fp );
			fread( &SecBulls[ i ].GroupImIn, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].ModelNum, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].ModelIndex, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fread( &SecBulls[ i ].fmpoly, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].numfmpolys, sizeof( int16 ), 1, fp );
			fread( &SecBulls[ i ].poly, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].numpolys, sizeof( int16 ), 1, fp );
			fread( &SecBulls[ i ].xsize, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].ysize, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].light, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].lightsize, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].r, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].g, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].b, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].TurnSpeed, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].ViewCone, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].TargetType, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].Target, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].DirQuat, sizeof( QUAT ), 1, fp );
			fread( &SecBulls[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fread( &SecBulls[ i ].Shield, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Damage, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Ammo, sizeof( uint16 ), 1, fp );
			fread( &SecBulls[ i ].NumBounces, sizeof( int16 ), 1, fp );
			fread( &SecBulls[ i ].NumOldPos, sizeof( int16 ), 1, fp );
			fread( &SecBulls[ i ].RetractPos, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].ColRadius, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Interval, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].Time, sizeof( float ), 1, fp );
			fread( &SecBulls[ i ].OldPos[ 0 ], sizeof( SecBulls[ i ].OldPos ), 1, fp );
			i = SecBulls[ i ].Prev;
		}

		i = FirstSecBullFree;

		while( i != (uint16) -1 )
		{
			SecBulls[ i ].Used = FALSE;
			SecBulls[ i ].Prev = (uint16) -1;
			SecBulls[ i ].NextInGroup = NULL;
			SecBulls[ i ].PrevInGroup = NULL;
			SecBulls[ i ].State = MIS_STRAIGHT;
			SecBulls[ i ].Flags = SECFLAGS_Nothing;
			SecBulls[ i ].Index = i;
			SecBulls[ i ].Type = (uint16) -1;
			SecBulls[ i ].SecType = SEC_MISSILE;
			SecBulls[ i ].DropCount = 0.0F;
			SecBulls[ i ].MoveType = MISMOVE_STRAIGHT;
			SecBulls[ i ].Owner = (uint16) -1;
			SecBulls[ i ].LifeCount = 0.0F;
			SecBulls[ i ].ColFlag = 0;
			SecBulls[ i ].GroupImIn = (uint16) -1;
			SecBulls[ i ].ModelNum = (uint16) -1;
			SecBulls[ i ].ModelIndex = (uint16) -1;
			SecBulls[ i ].fmpoly = (uint16) -1;
			SecBulls[ i ].numfmpolys = 0;
			SecBulls[ i ].poly = (uint16) -1;
			SecBulls[ i ].numpolys = 0;
			SecBulls[ i ].light = (uint16) -1;
			SecBulls[ i ].Target = (uint16) -1;
			SecBulls[ i ].TargetType = (uint16) -1;
			SecBulls[ i ].SpeedWanted = 32.0F;
			SecBulls[ i ].SpeedInc = 32.0F;
			SecBulls[ i ].Speed = 32.0F;
			SecBulls[ i ].TurnSpeed = 0.0F;
			SecBulls[ i ].ViewCone = 0.0F;
			SecBulls[ i ].DirVector = Forward;
			SecBulls[ i ].UpVector = SlideUp;
			SecBulls[ i ].DropVector = SlideDown;
			SecBulls[ i ].NumOldPos = 0;
			QuatFrom2Vectors( &SecBulls[ i ].DirQuat, &Forward, &SecBulls[ i ].DirVector );
			QuatToMatrix( &SecBulls[ i ].DirQuat, &SecBulls[ i ].Mat );

			fread( &SecBulls[ i ].Next, sizeof( SecBulls[ i ].Next ), 1, fp );
			i = SecBulls[ i ].Next;
		}
	}

	return( fp );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get Next Valid Secondary Weapon
	Input		:	Nothing
	Output		:	uint8	Primary Weapon
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 GetNextValidSeconadaryWeapon( void )
{
	int8	TempWeapon;

	TempWeapon = Ships[ WhoIAm ].Secondary;

	TempWeapon++;
	if( TempWeapon >= MAX_SECONDARY_WEAPONS ) TempWeapon = 0;

	while( !SecondaryWeaponsGot[ TempWeapon ] && ( TempWeapon != Ships[ WhoIAm ].Secondary ) )
	{
		TempWeapon++;
		if( TempWeapon >= MAX_SECONDARY_WEAPONS ) TempWeapon = 0;
	}

	return( TempWeapon );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get Prev Valid Secondary Weapon
	Input		:	Nothing
	Output		:	uint8	Primary Weapon
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 GetPrevValidSecondaryWeapon( void )
{
	int8	TempWeapon;

	TempWeapon = Ships[ WhoIAm ].Secondary;

	if( TempWeapon == 0 ) TempWeapon = ( MAX_SECONDARY_WEAPONS - 1 );
	else TempWeapon--;

	while( !SecondaryWeaponsGot[ TempWeapon ] && ( TempWeapon != Ships[ WhoIAm ].Secondary ) )
	{
		if( TempWeapon == 0 ) TempWeapon = ( MAX_SECONDARY_WEAPONS - 1 );
		else TempWeapon--;
	}

	return( TempWeapon );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Conv Secondary Weapon number from cheat to norm
	Input		:	int8	Secondary Weapon
	Output		:	uint8	Secondary Weapon ( Valid One )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int8 ConvSecToNormWeapon( int8 Secondary )
{
	int8	NewSecondary;

	NewSecondary = Secondary;

	switch( Secondary )
	{
		case ENEMYSPIRALMISSILE:
			NewSecondary = MUGMISSILE;
			break;
		case ENEMYFIREBALL:
			NewSecondary = MUGMISSILE;
			break;
		case ENEMYHOMINGMISSILE:
			NewSecondary = SOLARISMISSILE;
			break;
		case ENEMYBLUEHOMINGMISSILE:
			NewSecondary = SOLARISMISSILE;
			break;

		default:
			break;
	}

	return( NewSecondary );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Toggle Secondary Weapon Type
	Input		:	int8	Secondary Weapon
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ToggleSecondaryWeapons( int8 Secondary )
{
	switch( SecondaryToFireLookup[ Secondary ] )
	{
		case MUGMISSILE:
			SecondaryToFireLookup[ Secondary ] = ENEMYSPIRALMISSILE;
			break;
		case ENEMYSPIRALMISSILE:
			SecondaryToFireLookup[ Secondary ] = ENEMYFIREBALL;
			break;
		case ENEMYFIREBALL:
			SecondaryToFireLookup[ Secondary ] = MUGMISSILE;
			break;
		case SOLARISMISSILE:
			SecondaryToFireLookup[ Secondary ] = ENEMYHOMINGMISSILE;
			break;
		case ENEMYHOMINGMISSILE:
			SecondaryToFireLookup[ Secondary ] = ENEMYBLUEHOMINGMISSILE;
			break;
		case ENEMYBLUEHOMINGMISSILE:
			SecondaryToFireLookup[ Secondary ] = SOLARISMISSILE;
			break;
		default:
			break;
	}
}


#ifdef OPT_ON
#pragma optimize( "", off )
#endif
