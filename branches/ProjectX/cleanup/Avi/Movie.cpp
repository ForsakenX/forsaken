// movie.cpp
//

// This code and information is provided "as is" without warranty of
// any kind, either expressed or implied, including but not limited to
// the implied warranties of merchantability and/or fitness for a
// particular purpose.

// Copyright (C) 1995 Intel Corporation.  All rights reserved.
//

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#include "aviMovie.h"
#include "movie.h"
#include "AVIresource.h"
#include "dinput.h"
#include "d3dmain.h"
#include "main.h"


#ifdef _DEBUG
#include <assert.h>
#define ASSERT assert
#else
#define ASSERT / ## /
#endif

// externs...
extern int AVI_bpp;
extern int AVI_ZoomMode;
extern "C" HANDLE AVIEvent;
extern "C" ExitAVIThread;

extern "C" void __cdecl
Msg( LPSTR fmt, ... );
extern "C" int Print4x5Text( char * Text , int x , int y , int col );
extern "C"void ReadInput( void );
extern "C" int IsKeyPressed( int di_keycode );
extern "C" int AnyKeyPressed( void );
extern "C" void DebugPrintf( const char * format, ... );

// Globals

//extern HWND ghWndStatusBar;
//extern HINSTANCE ghInst;

// Current movie is kept globally
CAviMovie 		* gpMovie;

// Play settings

extern "C" CRITICAL_SECTION	gPlayKey; // Guards one variable, gbPlay

BOOL	gbPlay;  // Set iff playing
BOOL	gbPause; // Implies gbPlay=0

BOOL	gbLoop;  // Set if looping

DWORD	gthreadID; // Play thread information
HANDLE	ghThread;

DWORD	gIdle;   // Amount of idle ms b/ each frame:
                 //  This is to emulate slower processors

///////////////////////////////////////////////////////////////////////////////
// PLAY THREAD

// Only one instance of this thread can run!
//
// All we do here is play from current frame then step frame count.
// Since I can't think of a better way to do it right now, we'll
//  sleep when we can't play then try again.
// TODO: more elegant solution here (use events?).
//
// Note that if MovieIsPlaying() is true than all initialization is assumed
//  to have taken place.
//
// Note that this thread will NOT change the value of gbPlay (only check it)
//  hence once movie is stopped (in a critical section), we can do what we
//  want to the Movie state before restarting.  Also, it is valid to check
//  the state of gbPlay outside of a critical section in the main thread and
//  it will be valid.
//
BOOL MovieThreadProc (void * pParm)
{
//	CAviMovie * pMovie = (CAviMovie *) pParm;
	CAviMovie * pMovie = gpMovie;
	BOOL ReachedEnd = FALSE;

	if (!pMovie)
	{
		return FALSE;
	}

	while (1)
	{
		// Must be in critical section since we key
		//  off gbPlay
		EnterCriticalSection (&gPlayKey);

		if ( ExitAVIThread )
		{
			LeaveCriticalSection (&gPlayKey);
			break;
		}

		ReadInput();

		// Don't use SendMessage ():
		//  could deadlock if we are in critical section

		// Not playing:
		//  wait then try again
		if (!gbPlay)
		{
			LeaveCriticalSection (&gPlayKey);
			Sleep (33); // 1/30 sec., seems a good time to wait
			//return FALSE;
			continue;
		}

		// Reached end of movie:
		//  loop if necessary
		if (pMovie->MovieIsEnd() || AnyKeyPressed() )
		{
			ReachedEnd = TRUE;
			gbPlay = FALSE; // don't play
			LeaveCriticalSection (&gPlayKey);

			MovieStop ( pMovie->m_hwnd );
			//PostMessage (pMovie->m_hwnd, WM_COMMAND, MAKELONG (ID_MOVIE_STOP,0), 0);

			SetEvent( AVIEvent );

			return FALSE;

		}
		
	   if (pMovie->vidsVideoDraw () < 0)
          ;
	
		LeaveCriticalSection (&gPlayKey);

		// Idle if necessary:
		//  this is for emulating slower processors.
		if (gIdle)
		{
			DWORD start = timeGetTime ();
			DWORD tick = timeGetTime ();
			while (tick - start < gIdle)
				tick = timeGetTime ();
		}

		Sleep (0); // need to let main thread run for audio and user reactions
		           // note our implicit assumption that this thread is
			   // <= priorit of main thread
		           
		       
		
	}

		return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// General MOVIE functions

// Initialize movie, must be first call
//
void MovieInitialize (HWND hwnd)
{
	gbPlay = gbPause = FALSE;
	ghThread = 0;
	
	InitializeCriticalSection (&gPlayKey);

	gpMovie = new CAviMovie ();	
}

// Clean up movie
//
void MovieTerminate (HWND hwnd)
{
	/*
	// thread will now be allowed to exit itself ( breaking out of infinite while loop )
	if (ghThread)
	{
		TerminateThread (ghThread, 0);
		ghThread = 0;
	}
	*/

	if (gpMovie)
	{
		delete gpMovie;
		gpMovie = 0;
	}
}

// Size of current file in KB:
//  needed to figure out data rate of file
long gSizeFile;

// Open a file
//
BOOL MovieFileOpen (HWND hwnd, LPSTR lpszPathName)
{
	CAviMovie * pMovie = gpMovie;

	// Get file size
	HFILE    hfile;
	hfile = _lopen (lpszPathName, OF_READ);
	if (hfile == HFILE_ERROR)
		return FALSE;
	gSizeFile = GetFileSize ((HANDLE)hfile, NULL);
	gSizeFile /= 1024;
	_lclose (hfile);

	// Check if file is already open
	if (pMovie->MovieIsFileOpen ())
	{
		SendMessage (hwnd, WM_COMMAND, ID_FILE_CLOSE, 0);
	}

	// Initialize/Open File
	long rval = pMovie->FileOpen (lpszPathName);
	if (rval != 0L)
	{
		Msg("Cannot Open AVI file");
		return FALSE;
	}

	// Initialize/Open Movie
	rval = pMovie->MovieOpen (hwnd);
	if (rval != 0L)
	{
//		MessageBox (hwnd,
//		            "Error: Cannot read Video or Audio stream",
//			    "K-Scale Error",
//		            MB_ICONEXCLAMATION);
		Msg("Error: Cannot read Video or Audio stream");
		return FALSE;
	}

	// Resize main window
	PostMessage (hwnd, AVIMOV_NOTIFYSIZE, 0, 0);

	return TRUE;
}

// Close a file
//
void MovieFileClose (HWND hwnd)
{
	CAviMovie * pMovie = gpMovie;

	if (pMovie)
	{
		if (!pMovie->MovieIsFileOpen ())
			return;

		// Stop if still playing
		DebugPrintf("about to do MovieStop\n");
		MovieStop (hwnd);
		/*
		// thread will now be allowed to exit itself ( breaking out of infinite while loop )
		if (ghThread)
		{
			TerminateThread (ghThread, 0);
			ghThread = 0;
		}
		*/
	
		DebugPrintf("about to do MovieClose\n");
		pMovie->MovieClose ();
		DebugPrintf("about to do FileClose\n");
		pMovie->FileClose ();
	}

	// Resize main window
	PostMessage (hwnd, AVIMOV_NOTIFYSIZE, 0, 0);
}

// Handle WM_PAINT
//
void MoviePaint (HWND hwnd, HDC hdc, PAINTSTRUCT * ps)
{
	CAviMovie * pMovie = gpMovie;
	
	if (!pMovie->MovieIsFileOpen ())
	{
	// Movie has not been opened - draw empty frame
		return;
	}

	if (pMovie)
	{
		if (gbPlay)
		{
			// Thread is doing the painting when playing
			return;
		}
		else
		{
			pMovie->vidsVideoStart (hdc);
			pMovie->vidsVideoDraw ();
			pMovie->vidsVideoStop ();
		}
	}

}

// Handle WM_PALETTECHANGED and WM_QUERYNEWPALETTE
//  hwndPalChg is HWND of focused window
void MoviePaletteChanged (HWND hwnd, HWND hwndPalChg)
{
	CAviMovie * pMovie = gpMovie;
	if (!pMovie || !pMovie->m_pVCM)
		return;
	
	if (hwnd == hwndPalChg)
		return;

	if (pMovie->m_pVCM->m_hpalSection)
	{
		HPALETTE hpalOld = SelectPalette (pMovie->m_hdc, pMovie->m_pVCM->m_hpalSection, FALSE);
		RealizePalette (pMovie->m_hdc);
		if (hpalOld)
			SelectPalette (pMovie->m_hdc, hpalOld, FALSE);
	}
}

// Play movie
//
void MoviePlay (HWND hwnd)
{
	CAviMovie * pMovie = gpMovie;

	if (!pMovie || !pMovie->MovieIsFileOpen ())
		return;

	if (gbPlay)
		return; // already playing in thread

	// We are in STOPPED mode here (gbPlay = FALSE)
	
	if (!gbPause && !ghThread)
	{
		ghThread =
		CreateThread (NULL, 0,
		              (LPTHREAD_START_ROUTINE) MovieThreadProc,
		              (LPVOID) gpMovie, 0, &gthreadID);
	}	

	HDC hdc = GetDC (hwnd);
	pMovie->MovieStart (hdc);

	EnterCriticalSection (&gPlayKey);
	gbPause = FALSE;
	gbPlay  = TRUE;
	LeaveCriticalSection (&gPlayKey);
}

// Stop movie
//
void MovieStop (HWND hwnd)
{
	CAviMovie * pMovie = gpMovie;

	if (!pMovie || !pMovie->MovieIsFileOpen ())
		return;

	// already in critical section!!!
	//EnterCriticalSection (&gPlayKey);
	gbPause = FALSE;
	gbPlay  = FALSE;
	//LeaveCriticalSection (&gPlayKey);
			
	// MovieStop () clears m_hdc, so save it here -
	//  bad to rely on internal members but laziness prevails
	HDC hdc = pMovie->m_hdc;
	//pMovie->MovieStop ();
	
	pMovie->vidsResetSkipCount ();
	
	if (hdc)
		ReleaseDC (hwnd, hdc);
}

// Pause a movie
//
void MoviePause (HWND hwnd)
{
	CAviMovie * pMovie = gpMovie;

	if (!pMovie || !pMovie->MovieIsFileOpen ())
		return;

	if (!gbPlay)
		return;

	EnterCriticalSection (&gPlayKey);
	gbPlay  = FALSE;
	gbPause = TRUE;
	LeaveCriticalSection (&gPlayKey);

	// MoviePause () clears m_hdc
	HDC hdc = pMovie->m_hdc;
	pMovie->MoviePause ();

	if (hdc)
		ReleaseDC (hwnd, hdc);
}

// Note the seek functions will stop the video (if playing or
//  paused).  Also checks boundry conditions.

// Seek to absolute position lpos
//
void MovieSeek (HWND hwnd, long lpos)
{
	CAviMovie * pMovie = gpMovie;

	if (!pMovie || !pMovie->MovieIsFileOpen ())
		return;

	if (gbPlay || gbPause)
	{
		MovieStop (hwnd);
		pMovie->MovieSeek (lpos);
		MoviePlay (hwnd);
	}
	else
	{
		pMovie->MovieSeek (lpos);
	}
}

// Seeks from current position +- lframes
//
void MovieSeekCurrent (HWND hwnd, long lframe)
{
	CAviMovie * pMovie = gpMovie;

	if (!pMovie || !pMovie->MovieIsFileOpen ())
		return;

	if (gbPlay || gbPause)
		MovieStop (hwnd);

	pMovie->MovieSeekCurrent (lframe);
}

// Seeks from current position +- percent of file
//
void MovieSeekPercent (HWND hwnd, int percent)
{
	CAviMovie * pMovie = gpMovie;

	if (!pMovie || !pMovie->MovieIsFileOpen ())
		return;

	if (gbPlay || gbPause)
		MovieStop (hwnd);
				      
	float per = float(percent)/float(100);
	int cur   = pMovie->vidsGetFrame ();
	int numf  = pMovie->vidsGetNumFrames ();
	int frame = cur + (int)(per * (float)numf);
	
	pMovie->MovieSeek (frame);
}

// Seeks from current position +- sec seconds
//
void MovieSeekSec (HWND hwnd, int sec)
{
	CAviMovie * pMovie = gpMovie;

	if (!pMovie || !pMovie->MovieIsFileOpen ())
		return;

	if (gbPlay || gbPause)
		MovieStop (hwnd);
				      
	int frate = (int) (pMovie->MovieGetFrameRate () + float (0.5));
	frate *= sec;
	
	pMovie->MovieSeekCurrent (frate);
}

// Re-decompress current frame
//
void MovieRedraw (HWND hwnd)
{
	CAviMovie * pMovie = gpMovie;

	if (!pMovie || !pMovie->MovieIsFileOpen ())
		return;

	if (gbPlay)
		return;

	pMovie->vidsResetDraw ();
}

// Handle MM_WOM messages
//
void MovieAudioMsg (HWND hwnd, UINT msg)
{
	CAviMovie * pMovie = gpMovie;

	switch (msg)
	{
	case MM_WOM_OPEN:
	case MM_WOM_CLOSE:
		break;

	case MM_WOM_DONE:
		pMovie->audsDoneBuf ();
		break;
	}
}

// Resize window to fit current size of video
//
void MovieNotifySize (HWND hwnd)
{
	CAviMovie * pMovie = gpMovie;

#if 0
	RECT rcStatus;
	GetClientRect (ghWndStatusBar, &rcStatus);
#endif
	// Get size client window
	RECT rcMovie;
	if (pMovie->m_pVCM)
		pMovie->m_pVCM->vcmGetWndRect(&rcMovie);
	else
	{
		// No file - just have title bar/menu/status
		rcMovie.left = rcMovie.top = 0;
		rcMovie.right  = 320; // default width
		rcMovie.bottom = 0;   // doesn't matter - self adjusting
	}
	
	RECT rc;
	// Make 0,0 based output rect
	rc.left = rc.top = 0;
	rc.right  = (rcMovie.right-rcMovie.left);
	rc.bottom = (rcMovie.bottom-rcMovie.top);

	AdjustWindowRect (&rc, GetWindowLong (hwnd, GWL_STYLE), TRUE);

	SetWindowPos (hwnd, NULL,
	              0, 0,
		      rc.right-rc.left,
		      rc.bottom-rc.top,
		      SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE);
}

// Toggle looping
//
void MovieToggleLoop (HWND hwnd)
{
	gbLoop = !gbLoop;
}

// TRUE iff current movie is iv41
//
BOOL MovieIsIndeoR4 ()
{
	if (gpMovie && gpMovie->m_pVCM)
		return (gpMovie->m_pVCM->m_fccHandler == FOURCC_iv41);
	else
		return FALSE;
}

void MovieGetInfo (HWND hwnd, char * file, KFILEINFO * info)
{
	CAviMovie * pMovie = gpMovie;
	
	if (!pMovie || !pMovie->m_pVCM)
		return;
	
	char szBuf [32];
	
	info->fcc = pMovie->m_pVCM->m_fccHandler;

	// Fill general info
	
	// File name (contained in the title)
	GetWindowText (hwnd, info->szFileName, 128);

	AVIFILEINFO avif;
	AVIFileInfo (pMovie->m_pfile, &avif, sizeof (avif));

	// Media Length
	double sec = (double)avif.dwLength / ((double)avif.dwRate/(double)avif.dwScale);
	int min = ((int)(sec+0.5)) / 60;
	while (sec-60.0 >= 0.0)
		sec -= 60.0;
	if (min)
		sprintf (info->szMediaLen, "%d min., %.3f sec.", min, sec+0.0005);
	else
		sprintf (info->szMediaLen, "%.3f sec.", sec+0.0005);

	// Audio Format
	if (pMovie->m_paviAudio)
	{
		long cb;
		WAVEFORMATEX wfx;
		AVIStreamFormatSize (pMovie->m_paviAudio, 0, &cb);
		AVIStreamReadFormat (pMovie->m_paviAudio, 0, &wfx, &cb);

		if (wfx.nChannels == 2)
			strcpy (szBuf, "Stereo");
		else
			strcpy (szBuf, "Mono");

		// Since we only read PCM audio, it must be the format
		sprintf (info->szAudioFmt, 
		   "PCM, %d Hz, %d Bit, %s", wfx.nSamplesPerSec, wfx.wBitsPerSample, szBuf);
	}
	else
	{
		strcpy (info->szAudioFmt, "No Audio");
	}

	// Video Format
	sprintf (info->szVideoRes, "%d x %d Pixels", avif.dwWidth, avif.dwHeight);
	sprintf (info->szVideoNFm, "%d Frames", pMovie->vidsGetNumFrames ());
	sprintf (info->szVideoFPS, "%.3f Frames/Sec", pMovie->MovieGetFrameRate ());
	sprintf (info->szVideoFmt, "%c%c%c%c", LOBYTE(LOWORD(info->fcc)), HIBYTE(LOWORD(info->fcc)),
	                                       LOBYTE(HIWORD(info->fcc)), HIBYTE(HIWORD(info->fcc)));

	// Data Rate
	int datarate = (int)(double(gSizeFile)/double(avif.dwLength) *
	                     (double(avif.dwRate)/double(avif.dwScale)) + 0.5);
	sprintf (info->szDataRate, "%d KB/sec.", datarate);


	
}
///////////////////////////////////////////////////////////////////////////////
// Status Bar Routines

// Update Status Bar from playing video
//
void MovieUpdateStatusBarContents (HWND hwnd)
{
	CAviMovie * pMovie = gpMovie;
	int min, sec, hundreth;
	long msTime, frame;
	int fskipped;
	static int fskipped_ = 0;

	if (!pMovie)
		return;

	if (!pMovie->MovieIsFileOpen ())
	{
		min = sec = hundreth = 0;
		msTime = 0;
		frame = 0;
		fskipped_ = 0;
	}
	else
	{	
		// Get time and calculate in readable format (min/sec/100th)
		msTime = pMovie->vidsGetTime ();
		min       = msTime/60000;
		msTime   %= 60000;
		sec       = msTime/1000;

		// Get frame number
		frame = pMovie->vidsGetFrame ();
	}
	
	char szBuf [128];
	if (gbPlay)
	{
		// fps info
		fskipped = pMovie->vidsFramesSkipped ();
		if (fskipped < 0) fskipped = 0;
		fskipped_ = fskipped;
		wsprintf (szBuf, " %d skipped", fskipped);
		//SendMessage (ghWndStatusBar, SB_SETTEXT, 1, (LPARAM)(LPSTR)szBuf);
		Print4x5Text( szBuf , 10 , 10 , 2 );

		// Playing - don't print 100th sec or frame #
		//  since wouldn't be very accurate
		if (hundreth > 50)
			sec++;
		wsprintf (szBuf, " Pos: %02d:%02d [%d]",min,sec,frame);
		//SendMessage (ghWndStatusBar, SB_SETTEXT, 2, (LPARAM)(LPSTR)szBuf);
		Print4x5Text( szBuf , 10 , 30 , 2 );
	}
	else
	{
		// get Key info
		//BOOL bKey = pMovie->vidsIsKey ();
		//strcpy (szBuf, bKey ? " K" : "");
		//SendMessage (ghWndStatusBar, SB_SETTEXT, 0, (LPARAM)(LPSTR)szBuf);

		// fps info
		wsprintf (szBuf, " [%d] skipped", fskipped_);
		//SendMessage (ghWndStatusBar, SB_SETTEXT, 1, (LPARAM)(LPSTR)szBuf);
		Print4x5Text( szBuf , 10 , 10 , 2 );

		// get 1/100th sec.
		hundreth  = ((msTime % 1000)+5) / 10;
		if (hundreth >= 100)
		{
			hundreth = 0;
			sec++;
		}

		wsprintf (szBuf, " Pos: %02d:%02d.%02d [%d]",min,sec,hundreth,frame);
		//SendMessage (ghWndStatusBar, SB_SETTEXT, 2, (LPARAM)(LPSTR)szBuf);
		Print4x5Text( szBuf , 10 , 30 , 2 );
	}
}
#if 0

// Resize status bar to fit current window
//
void MovieUpdateStatusBarExtent (HWND hwnd)
{
	HDC	hdc;
	RECT	rc;
	SIZE	size;
	int	ptArray [3];

	hdc = GetDC (hwnd);
	GetClientRect (hwnd, &rc);

	ptArray [2] = rc.right;

	if (GetTextExtentPoint32 (hdc, "Pos: 00:00.00 [00000]", 21, &size))
		ptArray [1] = ptArray [2] - size.cx - 32; // pad more spaces
	else
		ptArray [1] = 0;

	// Skipped frame is leftover space

	if (GetTextExtentPoint32 (hdc, " K  ", 4, &size))
		ptArray [0] = size.cx;
	else
		ptArray [0] = 0;

	ReleaseDC (hwnd, hdc);

	SendMessage (ghWndStatusBar, SB_SETPARTS,
	             sizeof (ptArray)/sizeof (ptArray [0]),
		     (LPARAM) (LPINT) ptArray);
}
#endif
///////////////////////////////////////////////////////////////////////////////
// Options Routines : Bitdepth and Resolution

// Set id to 0 if want native bitdepth
// Set id to negative if want to set the last bitdepth selected
//
// added - ZoomMode ( 0, 1 or 2 )
void MovieSetOptionsBitdepth (HWND hwnd, int bpp)
{
	
	CAviMovie * pMovie = gpMovie;

	if (!pMovie->MovieIsFileOpen ())
		return;

	AVI_bpp = bpp;

	float zoom;
	switch (AVI_ZoomMode)
	{
	case 0:
		zoom = float (1);
		break;
	case 1:
		zoom = float (2);
		break;
	case 2:
		zoom = float (2);
		break;
	default:
		zoom = float (1);
	}
	
	// If playing, pause then restart
	BOOL bRestart = FALSE;
	if (gbPlay)
	{
		bRestart = TRUE;
		MovieStop(hwnd);
	}

	if (pMovie->MovieSetDestOptions (zoom, AVI_bpp, AVI_ZoomMode))
	{
		// Set failed - default to CLUT8
		//ASSERT (id != ID_OPTIONS_BITDEPTH_CLUT8);
		pMovie->MovieSetDestOptions (float (zoom), 8, 1);
      AVI_bpp = 8;
	}

	if (bRestart)
		MoviePlay (hwnd);
}


// Check proper resolution menu item
//
void MovieSetOptionsResolution (HWND hwnd, int id, int xres, int yres)
{
	BOOL bRestart = FALSE;
	
	CAviMovie * pMovie = gpMovie;

	if (!pMovie->MovieIsFileOpen ())
	{
		return;
	}

   int altMode = 0;
	float zoom = float (0);

   switch(id)
   {
      case ID_OPTIONS_RESOLUTION_NATIVE:
         zoom = float (1);
         break;
         
      case ID_OPTIONS_RESOLUTION_ZOOMBY2:   
         altMode = 1;
         zoom = float (2);
         break;

      case ID_OPTIONS_RESOLUTION_ZOOMBY2ALT:   
         altMode = 2;
         zoom = float (2);
         break;
   }
   
	if (gbPlay)
	{
		bRestart = TRUE;
		MoviePause (hwnd);
	}

	pMovie->MovieSetDestOptions (zoom, 0, altMode, xres, yres);

   if(id)
	   PostMessage (hwnd, AVIMOV_NOTIFYSIZE, 0, 0);

	if (bRestart)
		MoviePlay (hwnd);

	// Note: This assumes that src is native (valid for now) - must change
	//       if this assumtion changes.

}


BOOL MoviePointInImage(int xpos, int ypos )
{
	RECT rc;
   POINT pt;
	CAviMovie * pMovie = gpMovie;

   if (!pMovie->MovieIsFileOpen ())
      return FALSE;
        
	if( pMovie->m_pVCM->vcmGetBoundRect(&rc) == VCMERR_OK)
   {
      pt.x = xpos;
      pt.y = ypos;
      if(PtInRect( &rc, pt ) )
         return TRUE;
   }

   return FALSE;
}

void MovieSetImagePos(int xpos, int ypos)
{
   RECT rc;
	CAviMovie * pMovie = gpMovie;

   if (!pMovie->MovieIsFileOpen ())
      return;

   pMovie->m_pVCM->vcmGetDestRect( &rc );
   
   if( pMovie->m_pVCM->vcmQueryDest( NULL, rc.left + xpos, rc.top + ypos, 
                                        0, 0 ) == ICERR_OK)
   {      
      rc.top    += ypos;
      rc.bottom += ypos;
      rc.right  += xpos;
      rc.left   += xpos;

      pMovie->m_pVCM->vcmSetDestRect( &rc );   
   }

}

// Set the proper palette
//
void MovieSetOptionsPalette 
   (
   HWND hwnd, 
   int  id
   )
{
	BOOL bRestart = FALSE;
   BOOL bError   = FALSE;
   LOGPALETTE* pLogPal;
   HPALETTE hPal;


	CAviMovie * pMovie = gpMovie;
	
	if (!pMovie->MovieIsFileOpen ())
	{
		return;
	}


   if( id == ID_OPTIONS_PALETTE_NATIVE )
   {
      pLogPal = NULL;
   }
   else
   {
      pLogPal = (LOGPALETTE *) new char [sizeof (LOGPALETTE) +
                                      256 * sizeof (PALETTEENTRY)];
      if (!pLogPal)
      {
         //MessageBox(hwnd, "Memory Error", "KPLAY Error", MB_ICONEXCLAMATION | MB_OK);
		  Msg("Movie.cpp: Memory Error");
         return;
      }

      switch(id)
      {
         case ID_OPTIONS_PALETTE_CLIPBOARD:   
            if(!OpenClipboard(hwnd))
            {
               bError = TRUE;
               break;                                 // Can't open clipboard
            }
            else
            {
               hPal = GetClipboardData(CF_PALETTE);   // Not supported on NT
               CloseClipboard();               
               if(!hPal)
               {
                  bError = TRUE;
      		      break;                              // No clipboard palette
               }

      	      pLogPal->palVersion = 0x300;
      	      pLogPal->palNumEntries = 236;

               GetPaletteEntries(hPal, 10, 236, pLogPal->palPalEntry);
            }
            break;

         case ID_OPTIONS_PALETTE_LOAD:

            if(MovieLoadPalette(hwnd, pLogPal) != 0)
            {
               bError = TRUE;
               break;
            }
            break;
      }
   }

   // Note: IVI Alt Line Zoom doesn't support active palette. 
   //       Can't do active palette from any zoom by 2 mode. Must
   //       first go native, then active, then zoom, but only pixel
   //       doubling mode
   

   if(!bError)
   {
   
	if (gbPlay)
	{
		bRestart = TRUE;
		MovieStop(hwnd);
	}
   	if(pMovie->MovieSetDestPalette (pLogPal) == MOVERR_OK)
      {

         MoviePaletteChanged(hwnd, NULL);
      }

   	if (bRestart)
   	{
   		MoviePlay (hwnd);
      }
   }

	PostMessage (hwnd, AVIMOV_NOTIFYSIZE, 0, 0);
   
   if(pLogPal)
      delete [] pLogPal;
}

///////////////////////////////////////////////////////////////////////////////
// Indeo(r) Video Interactive Options

// On all Get/Set Options, fDefault=TRUE means we use the codec default values

void MovieSetBackground(HWND hwnd, char * file)
{
	BOOL bRestart = FALSE;
   OPENFILENAME ofn = {0};

	CAviMovie * pMovie = gpMovie;

	if (!pMovie->MovieIsFileOpen ())
	{
		return;
	}

	if (!file)
		return;

	/*
   **  Pause the movie
   */

	if (gbPlay)
	{
		bRestart = TRUE;
		MovieStop(hwnd);
	}

   /*
   **  Set the background.
   */
   
   MovieSetOptionsBitdepth(hwnd, 0);
   MovieSetOptionsResolution(hwnd, ID_OPTIONS_RESOLUTION_NATIVE);

	if (bRestart)
		MoviePlay (hwnd);

	PostMessage (hwnd, AVIMOV_NOTIFYSIZE, 0, 0);

}









// Get/Set Brightness, Saturation, Contrast
//
void MovieGetOptionsBSC (HWND hwnd, long *bval, long *sval, long *cval,
                         BOOL fDefault)
{
	CAviMovie * pMovie = gpMovie;

	if (pMovie->m_pVCM)
		pMovie->m_pVCM->vcmGetBSC (bval, sval, cval, fDefault);
}
void MovieSetOptionsBSC (HWND hwnd, long bval, long sval, long cval,
                         BOOL fDefault)
{
	CAviMovie * pMovie = gpMovie;

	if (pMovie->m_pVCM)
	{
		pMovie->m_pVCM->vcmSetBSC (bval, sval, cval, fDefault);
		if (!gbPlay)
			pMovie->vidsResetDraw ();
	}
}


// Get/Set Scalability:
//  note that Indeo(r) Interactive works on ms allowed per decoded frame
//
void MovieGetOptionsScalability (HWND hwnd, int * ms, BOOL fDefault)
{
	CAviMovie * pMovie = gpMovie;

	float frate = pMovie->MovieGetFrameRate ();

	if (pMovie->m_pVCM)
		pMovie->m_pVCM->vcmGetScalability (frate, ms, fDefault);
}
void MovieSetOptionsScalability (HWND hwnd, int ms, BOOL fDefault)
{
	CAviMovie * pMovie = gpMovie;

	float frate = pMovie->MovieGetFrameRate ();

	if (pMovie->m_pVCM)
	{
		pMovie->m_pVCM->vcmSetScalability (frate, ms, fDefault);
		if (!gbPlay)
			pMovie->vidsResetDraw ();
	}
}

// Set AccessKey. There is no GetAccessKey option because it's a secret.
//
void MovieSetOptionsAccessKey (HWND hwnd, DWORD dwKey)
{
	CAviMovie * pMovie = gpMovie;
	if (pMovie->m_pVCM)
	{
		pMovie->m_pVCM->vcmSetAccessKey (dwKey);
	}
}


///////////////////////////////////////////////////////////////////////////////
// SCALABITY

// ms -> processor (index into processor_list)
// VERY rough correlation
long MovieGetMSToProcessor (long ms, PROCESSOR processor_list [], int nlen)
{
	CAviMovie * pMovie = gpMovie;
	if (!pMovie->m_pVCM)
		return (0);

	if (!setProcessor (processor_list, nlen))
		return 0;

	if (ms <= 0)
		return (giActualProcessor);
		
	double actual_time = (double) pMovie->MovieGetTimeslice ();
	double desired_time = ms;
	double actual_mhz = processor_list [giActualProcessor].mhz;
	double desired_mhz = desired_time/actual_time * actual_mhz;

	int mhz = (long)(desired_mhz+0.5);
	int processor = 1;
	for (int i = 2; i < nlen; i++)
	{
		if (ABS_(mhz - processor_list[i].mhz) < ABS_(mhz - processor_list[processor].mhz))
			processor = i;
	}
	
	return (processor);
}

long MovieGetProcessorToMS (int index, PROCESSOR processor_list [], int nlen)
{
	CAviMovie * pMovie = gpMovie;
	if (!pMovie->m_pVCM)
		return (-1);

	if (!setProcessor (processor_list, nlen))
		return 0;

	if (index <= 0)
		return 0;

	double actual_mhz = processor_list [giActualProcessor] .mhz;
	double desired_mhz = processor_list [index] .mhz;
	double actual_time = (double) pMovie ->MovieGetTimeslice ();
	double desired_time = desired_mhz/actual_mhz * actual_time + 0.5;

	return (long)desired_time;
}

void MovieSetgIdle (int index, PROCESSOR processor_list [])
{
	CAviMovie * pMovie = gpMovie;
	if (!pMovie->m_pVCM)
		return;

	if (index < 0)
		gIdle = 0;
	else
	{
		double actual_mhz = processor_list [giActualProcessor] .mhz;
		double desired_mhz = processor_list [index] .mhz;
		double actual_time = (double) pMovie ->MovieGetTimeslice ();
		double desired_time = desired_mhz/actual_mhz * actual_time + 0.5;

		if (actual_time > desired_time)
			gIdle = (int)(actual_time - desired_time);
		else
			gIdle = 0;
	}
}


long MovieLoadPalette(HWND hwnd, LOGPALETTE* pLogPal)
{
   HMMIO       hmmio;              // file handle for open file 
   MMCKINFO    mmckinfoParent;     // parent chunk information 
   MMCKINFO    mmckinfoSubchunk;   // subchunk information structure 
   DWORD       dwDataSize;         // size of "DATA" chunk 

   OPENFILENAME	ofn						 = {0};
   char			   szFileName[_MAX_PATH] = "d:\\avi\\quant.pal";
   char			   szFilter[128]			 = "Palette Files\0*.pal\0\0";
	

	// Set up Common Dialog structure
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner 	 = NULL;
	ofn.lpstrTitle  = "Select Palette File";
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile 	 = szFileName;
	ofn.nMaxFile 	 = sizeof(szFileName);
	ofn.Flags 		 = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	// Prompt the user
	if( !GetOpenFileName( &ofn ) )
	{
      return 1;
	}

   // Open the file for reading with buffered I/O 
   // by using the default internal buffer 
   if(!(hmmio = mmioOpen(szFileName, NULL, MMIO_READ))) 
   { 
      //Error("Failed to open file."); 
      return 1; 
   } 

   // Locate a "RIFF" chunk with a "WAVE" form type to make 
   // sure the file is a waveform-audio file. 
   mmckinfoParent.fccType = mmioFOURCC('P', 'A', 'L', ' '); 
   if (mmioDescend(hmmio, (LPMMCKINFO) &mmckinfoParent, NULL, 
        MMIO_FINDRIFF)) 
   { 
      //Error("This is not a palette file."); 
      mmioClose(hmmio, 0); 
      return 1; 
   } 


   // Find the data subchunk. The current file position should be at 
   // the beginning of the data chunk; however, you should not make 
   // this assumption. Use mmioDescend to locate the data chunk. 
   mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
   if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent, 
        MMIO_FINDCHUNK)) 
   { 
      //Error("Waveform-audio file has no data chunk."); 
      mmioClose(hmmio, 0); 
      return 1; 
   } 

   // Get the size of the data subchunk. 
   dwDataSize = mmckinfoSubchunk.cksize; 
   if (dwDataSize == 0L)
   { 
      //Error("The data chunk contains no data."); 
      mmioClose(hmmio, 0); 
      return 1; 
   } 


   // Read the palette data subchunk. 
   if((DWORD)mmioRead(hmmio, (HPSTR) pLogPal, dwDataSize) != dwDataSize)
   { 
      //Error("Failed to read data chunk."); 
      mmioClose(hmmio, 0); 
      return 1; 
   } 

   // Close the file. 
   mmioClose(hmmio, 0); 

   return 0;

   } 

