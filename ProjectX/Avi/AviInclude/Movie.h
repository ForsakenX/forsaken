// movie.h
//

// This code and information is provided "as is" without warranty of
// any kind, either expressed or implied, including but not limited to
// the implied warranties of merchantability and/or fitness for a
// particular purpose.

// Copyright (C) 1995 Intel Corporation.  All rights reserved.
//

#include "scale.h"

// Globals

// Movie instance
extern CAviMovie * gpMovie;

// Critical Section gPlayKey and var gbPlay for synchronization
extern BOOL gbPlay;
	
// Stores information for file properties
typedef struct tagKFILEINFO
{
	FOURCC fcc;
	// General
	char szFileName [128];
	char szMediaLen [128];
	char szAudioFmt [128];
	char szVideoRes [128];
	char szVideoNFm [128];
	char szVideoFPS [128];
	char szVideoFmt [128];
	char szDataRate [128];
} KFILEINFO;


// Commands

// Thread Proc

extern "C" BOOL MovieThreadProc (void * pParm);

// Init/Term

extern "C" void MovieInitialize (HWND hwnd);
extern "C" void MovieTerminate (HWND hwnd);

// File	ops

extern "C" BOOL MovieFileOpen (HWND hwnd, LPSTR lpszPathName);
extern "C" void MovieFileClose (HWND hwnd);

// Movie ops

extern "C" void MoviePlay (HWND hwnd);
extern "C" void MovieStop (HWND);
extern "C" void MoviePause (HWND);

void MovieSeek (HWND, long);
void MovieSeekCurrent (HWND, long);
void MovieSeekPercent (HWND, int);
void MovieSeekSec (HWND, int);

void MovieToggleLoop (HWND);

extern "C" void MovieRedraw (HWND hwnd);      // ID_MOVE_REDRAW

void MovieGetInfo (HWND, char *, KFILEINFO *);

// Options

extern "C" void MovieSetOptionsBitdepth (HWND, int);
extern "C" void MovieSetOptionsResolution (HWND hwnd, int id, int xres = 0, int yres = 0);
extern "C" void MovieSetOptionsPalette (HWND, int);

// Indeo(R) Video Interactive Options

// Transparency background
void MovieSetBackground(HWND hwnd, char* file = NULL);
long MovieLoadPalette(HWND hwnd, LOGPALETTE* pLogPal);
BOOL MoviePointInImage(int xpos, int ypos );
void MovieSetImagePos(int xpos, int ypos);

// Brightness, Saturation, Contrast
void MovieGetOptionsBSC (HWND, long *, long *, long *, BOOL);
void MovieSetOptionsBSC (HWND, long, long, long, BOOL);

// Scalability
void MovieGetOptionsScalability (HWND, int *, BOOL);
void MovieSetOptionsScalability (HWND, int, BOOL);
void MovieSetOptionsAccessKey (HWND hwnd, DWORD dwKey);

extern DWORD gIdle;
void MovieSetgIdle (int, PROCESSOR []);
long MovieGetMSToProcessor (long, PROCESSOR [], int);
long MovieGetProcessorToMS (int, PROCESSOR [], int);

// Other messages

void MoviePaint (HWND, HDC, PAINTSTRUCT *); // WM_PAINT

void MoviePaletteChanged (HWND, HWND);

void MovieInitializeMenuBitdepth (HWND);
void MovieUpdateMenuMovie (HWND);
extern "C" void MovieUpdateStatusBarContents (HWND);
void MovieUpdateStatusBarExtent (HWND);

extern "C" void MovieAudioMsg (HWND, UINT); // MM_WOM_ msgs

void MovieNotifySize (HWND);

