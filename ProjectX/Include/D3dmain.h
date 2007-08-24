/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 20 $
 *
 * $Header: /PcProjectX/Include/D3dmain.h 20    7/08/98 10:25a Phillipd $
 *
 * $Log: /PcProjectX/Include/D3dmain.h $
 * 
 * 20    7/08/98 10:25a Phillipd
 * 
 * 19    23/03/98 10:19 Oliverc
 * Added basic CD checking at startup and whenever player respawns
 * 
 * 18    16/10/97 18:11 Philipy
 * Implemented pre-game AVI player
 * 
 * 17    31-01-97 10:20a Collinsd
 * Light Details moved into detail menu.
 * 
 * 16    7/11/96 10:47 Collinsd
 * Shortened short ships structures.
 * 
 * 15    11/06/96 6:10p Phillipd
 * Got rid of some useless files...
 * 
 * 14    10/29/96 2:52p Phillipd
 * 
 * 13    10/24/96 3:01p Phillipd
 * 
 * 12    19/10/96 20:50 Collinsd
 * Changed scatter/thief to give more chance for enemy to pickup up
 * scattered weapons.
 * Reduced thief homing.  Increased Heatseaker.
 * Lights now can be enabled and disabled properly.
 * started on flying body parts.  Plus lots more..... 
 * 
 * 11    10/18/96 4:59p Phillipd
 * 
 * 10    18/10/96 11:39 Collinsd
 * Added light detail options to menus
 * 
 * 9     10/05/96 7:00p Phillipd
 * Got rid of the extents management from RenderLoop()
 * Removed clearing back buffers from d3dmain.cpp..
 * now done every time a camera view needs to do it..
 * Removed load of / 2.0f from quat.c and made some of the functions
 * __inline
 * 
 * 8     9/25/96 5:53p Phillipd
 * added Simplified Models......And Bike Detail Level now
 * works..
 * 
 * 7     9/23/96 11:49a Phillipd
 * Added Level Select to the Menus..
 * 
 * 6     7/15/96 2:58p Phillipd
 * 
 * 5     4/07/96 11:56 Oliverc
 * 
 * 4     2/07/96 17:57 Oliverc
 * 
 * 3     26/06/96 17:14 Oliverc
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*
 *  Copyright (C) 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: d3dmain.h
 *
 */
#ifndef __D3DMAIN_H__
#define __D3DMAIN_H__

#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <search.h>
#include <ddraw.h>
#include <d3d.h>
#include "d3dapp.h"         /* prototypes for D3D helper functions */
#include "d3dres.h"         /* defines constants used in d3dmain.rc */
#include "d3ddemo.h"        /* prototypes for functions to communicate with
                               each sample */
#define START_WIN_SIZE 320  /* initial size of the window */
#define START_WIN_SIZE_X 320+8  /* initial size of the window */
#define START_WIN_SIZE_Y 200+46  /* initial size of the window */

#include "new3d.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct tagd3dmainglobals {
    HWND hWndMain;          /* application window handle */
    HINSTANCE hInstApp;     /* application instance for dialog boxes */

    D3DAppRenderState rstate; /* struct describing the current render state */

    BOOL bSingleStepMode;        /* render one frame at a time */
    BOOL bDrawAFrame;            /* render on this pass of the main loop */
    BOOL bClearsOn;              /* clear the back and z-buffer each frame */
    BOOL bShowFrameRate;         /* show the frame rate at the top */
    BOOL bShowInfo;              /* show window information at the bottom */
    BOOL bResizingDisabled;      /* do not allow resizing */

    BOOL bResized; /* the window has resized or some other drastic change, the
                      entire client area should be cleared */
    BOOL bQuit;    /* program is about to terminate */

    LPDIRECTDRAWSURFACE lpFrameRateBuffer; /* frame rate surface */
    LPDIRECTDRAWSURFACE lpInfoBuffer;      /* window info surface */
} d3dmainglobals;

#ifdef SUPER
	extern HANDLE AVIThreadControlEvent;
	extern	d3dmainglobals	myglobs;
	extern	VECTOR Camera_Pos;
	extern	uint16 NumGroupsVisible;
	extern	uint16 GroupImIn;
	extern	uint16 NumCollides;
	extern	float ticksperframe;
	extern	DWORD GroupPolyCol_timeMax;
	extern	long debug_long;
	extern	BOOL cursorclipped;
	extern	RECT cursorclip;
	extern	BYTE                    WhoIAm;

	extern	int16		NumLevels;
	extern	char	ShortLevelNames[MAXLEVELS][32];
	extern	int16		LevelNum;
	extern	int16		NewLevelNum;
	extern	BOOL		PrimaryLightDetail;
	extern	BOOL		SecondaryLightDetail;
	extern	BOOL		PickupLightDetail;

	extern	BYTE		MyGameStatus;
	
	void TestBlt();
	void SetCursorClip( void );
	void ReallyShowCursor( BOOL );
	void EnablePrimaryLights( void );
	void DisablePrimaryLights( void );
	void EnableSecondaryLights( void );
	void DisableSecondaryLights( void );
	void EnablePickupLights( void );
	void DisablePickupLights( void );

	extern	int		BikeDetail;

#endif
void __cdecl Msg( LPSTR fmt, ... );
int __cdecl RetryMsg( LPSTR fmt, ... );

#ifdef __cplusplus
};
#endif

#endif // __D3DMAIN_H__
