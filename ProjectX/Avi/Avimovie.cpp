// aviMovie.cpp : Implementation of the CAviMovie class
//

// This code and information is provided "as is" without warranty of
// any kind, either expressed or implied, including but not limited to
// the implied warranties of merchantability and/or fitness for a
// particular purpose.

// Copyright (C) 1995 Intel Corporation.  All rights reserved.
//

#include <windows.h>

#ifdef _DEBUG
#include <stdio.h>
#include <assert.h>
#define ASSERT assert
#else
#define ASSERT / ## /
#endif

#include "aviMovie.h"

extern "C" AVISTREAMINFO VidInfo;
extern "C" void DebugPrintf( const char * format, ... );

///////////////////////////////////////////////////////////////////////////////
// Constructor/Deconstructor

CAviMovie::CAviMovie ()
{
	m_pVCM = NULL;
   m_pDIB = NULL;
	m_pfile = NULL;
	m_paviVideo = NULL;
	m_paviAudio = NULL;
	m_hwnd = NULL;
   m_bPlaying = NULL;
	m_bPaused = NULL; 
	m_fps = (float)0.0;
	m_frate = (float)0.0;
   m_resolution = 0;

	m_hdd = NULL;
	m_hdc = NULL;

	m_timePlayStart = 0;
   m_timePlayStartPos = 0;

	m_play_fprev = 0;
	m_play_fskipped = 0;

   m_vidsCurrent = 0;
	m_vidsPrevious = 0;

	m_vidsPrevKey = 0;
   m_vidsNextKey = 0;

	m_vidsFirst = 0;
   m_vidsLast = 0;

	m_pvBuf = NULL;
	m_cbvBuf = 0;

	m_hWaveOut = NULL;
	
	m_timeAudioStart = 0;

	m_audsFirst = 0;
   m_audsLast = 0;
	m_audsSkew = 0;

	m_sampsPerSec = 0;
	m_bytesPerSec = 0;
	m_sampSize = 0;
	m_sampsPerFrame = 0;

	m_audsCurrent = 0;

   for(int i=0; i<MAX_AUDIOBUFS; i++)
      m_audsBuf[i] = NULL;

   m_iaudsFreeBuf = 0;
	m_iaudsPlayBuf = 0;
	m_naudsBufsOut = 0;


   AVIFileInit ();
}

CAviMovie::~CAviMovie ()
{
	// Just in case
	MovieClose ();
	FileClose ();
	
	AVIFileExit ();
}

///////////////////////////////////////////////////////////////////////////////
// AVI File Operations

// Open a file
//
long CAviMovie::FileOpen (LPCTSTR lpszPathName)
{
	long	rval;

	// Make sure m_pfile is closed
	if (m_pfile)
		FileClose ();

	m_paviVideo = m_paviAudio = 0L;

	rval = AVIFileOpen (&m_pfile, lpszPathName, OF_SHARE_DENY_WRITE, 0L);
	if (rval)
	{
		FileClose ();
		return (rval);
	}
	
	return 0L;
}

// Close a file
//
long CAviMovie::FileClose ()
{
	if (m_pfile)
	{
		AVIFileRelease (m_pfile);
		m_pfile = 0L;
	}

	return (0);
}

///////////////////////////////////////////////////////////////////////////////
// Movie Operations

// A movie must have a video stream and optionally an audio stream
// Return codes should be properly done but for now, 0 is no error
// while !0 is error unless otherwise noted

// Open a movie (video/audio stream(s)) from an open file
//
long CAviMovie::MovieOpen (HWND hwnd)
{
	long rval;

	// Make sure movie closed
	if (m_paviVideo || m_paviAudio)
		MovieClose ();

	m_bPlaying = m_bPaused = 0;
	m_hwnd     = hwnd;

	// Initialize Video and Audio in stop state
	m_pVCM = 0L;
	m_hWaveOut = 0L;

	// Open first video and audio stream found
	// Note that we don't handle multiple video/audio streams
	rval = AVIFileGetStream (m_pfile, &m_paviVideo, streamtypeVIDEO, 0);
	if (rval == AVIERR_NODATA)
	{
		m_paviVideo = 0L;
		return (MOVERR_NOVIDEOSTREAM);
	}
	rval = AVIFileGetStream (m_pfile, &m_paviAudio, streamtypeAUDIO, 0);
	if (rval == AVIERR_NODATA)
		m_paviAudio = 0L;
/*
	// Get video stream info
	AVISTREAMINFO avis;
	AVIStreamInfo (m_paviVideo, &avis, sizeof (avis));
	m_fps = m_frate = float (avis.dwRate) / float (avis.dwScale);
*/
	// Get video stream info
	AVIStreamInfo (m_paviVideo, &VidInfo, sizeof (VidInfo));
	m_fps = m_frate = float (VidInfo.dwRate) / float (VidInfo.dwScale);

	// Open vids
	rval = vidsVideoOpen ();
	if (rval)
		return (rval);

	return (MOVERR_OK);
}

// Close a movie, this means vids, auds, and streams
//
long CAviMovie::MovieClose ()
{
	// Make sure movie is stopped
	DebugPrintf("MovieClose() - about to do MovieStop\n");
	MovieStop ();
	
	// Close vids/auds
	DebugPrintf("MovieClose() - about to do vidsVideoClose\n");
	vidsVideoClose ();
	DebugPrintf("MovieClose() - about to do audsWaveClose\n");
	audsWaveClose ();

	// Release streams
	DebugPrintf("MovieClose() - about to do AVIStreamRelease (m_paviVideo)\n");
	if (m_paviVideo)
		AVIStreamRelease (m_paviVideo);
	DebugPrintf("MovieClose() - about to do AVIStreamRelease (m_paviAudio)\n");
	if (m_paviAudio)
		AVIStreamRelease (m_paviAudio);

	m_paviVideo = m_paviAudio = 0;
	
	DebugPrintf("MovieClose() - about to return MOVERR_OK\n");
	return (MOVERR_OK);
}

#define TIME_RES 1 // desired timer resolution in ms

// Start a movie, i.e. begin play from stopped state
//  or restart from paused state
//
long CAviMovie::MovieStart (HDC hdc)
{
	if (!m_paviVideo)
		return (MOVERR_NOVIDEOSTREAM);

	// Check if starting without stopping
	if (m_bPlaying)
		return (MOVERR_PLAYING);
	
	m_bPlaying = TRUE;

	// Set timing info and set min resolution we can use
	TIMECAPS tc;
	timeGetDevCaps (&tc, sizeof (TIMECAPS));
	m_resolution = max (TIME_RES, tc.wPeriodMin);
	timeBeginPeriod (m_resolution);

	// (Re)start audio:
	//   note that pausing only affect audio because we always stop video
	//   whether paused or stopped

	if (m_bPaused)
	{
		audsWaveRestart ();
	}
	else
	{
		// Audio must be started before video since video is cued off
		//  of it.
		// Note that this will not give you good perceived start in
		//  cases where user stop/start (relatively) immediately
		//  because vds takes time (x frames) to initialize and those
		//  x frames will be skipped.  To fix this, init (pause) audio/
		//  video then start audio/video instead of init&start audio/video.
		
		long lTime = vidsSampleToTime (m_vidsCurrent);
		long lSamp = audsTimeToSample (lTime);

		audsWavePlay (lSamp, 0, TRUE);
	}

	m_bPaused = 0;
	
	// Start video, note only one state var, play or stop
	vidsVideoStart (hdc);

	return (MOVERR_OK);
}

// Stop playing a movie
//
long CAviMovie::MovieStop ()
{
	m_bPlaying = 0;
	m_bPaused  = 0;

	
	audsWaveStop ();
	vidsVideoStop ();
	
	// Clear timing info
	timeEndPeriod (m_resolution);
	
	return (MOVERR_OK);
}

// Pause a movie
//
long CAviMovie::MoviePause ()
{
	m_bPlaying = 0;
	m_bPaused  = 1;

	audsWavePause ();
  	vidsVideoStop (); // pause doesn't exist for video
	
	// Clear timing info
	timeEndPeriod (m_resolution);
	
	return (MOVERR_OK);
}

// Absolute position seeking, also checks boundry conditions
// Must be stopped to seek
//
// Return frame seeked to
//
long CAviMovie::MovieSeek (long lpos)
{
	if (m_bPlaying)
		return (-1L); // must be stopped to seek

	m_vidsCurrent = lpos;
	if (m_vidsCurrent < m_vidsFirst)
		m_vidsCurrent = m_vidsFirst;
	else if (m_vidsCurrent > m_vidsLast)
		m_vidsCurrent = m_vidsLast;

	if (m_vidsCurrent == m_vidsFirst)
	{
		m_vidsPrevious = -1;
		m_vidsPrevKey = -1; 
		m_vidsNextKey = -1; 	
	} 


	if (m_paviAudio && m_sampsPerFrame)
	{
	// With audio: get corresponding audio frame
		long lsample, ltime;

		ltime = AVIStreamSampleToTime (m_paviVideo, lpos);
		lsample = AVIStreamTimeToSample (m_paviAudio, ltime);
		m_audsCurrent = lsample/m_sampsPerFrame;
	}
	// Without audio: don't need to do anyting

	return (m_vidsCurrent);
}

// Relative position seeking, also checks boundry conditions
// Must be stopped to seek
//
// Return frame seeked to
//
long CAviMovie::MovieSeekCurrent (long ldiff)
{
	if (m_bPlaying)
		return (-1L); // must be stopped to seek

	m_vidsCurrent += ldiff;
	if (m_vidsCurrent < m_vidsFirst)
		m_vidsCurrent = m_vidsFirst;
	else if (m_vidsCurrent > m_vidsLast)
		m_vidsCurrent = m_vidsLast;

	if (m_paviAudio && m_sampsPerFrame)
	{
	// With audio: get corresponding audio frame
		long lsample, ltime;

		ltime = AVIStreamSampleToTime (m_paviVideo, m_vidsCurrent);
		lsample = AVIStreamTimeToSample (m_paviAudio, ltime);
		m_audsCurrent = lsample/m_sampsPerFrame;
	}
	// Without audio: don't need to do anything
	
	return (m_vidsCurrent);
}


// Set zoom and/or bitdepth options.
//
// Note that zoom must always be set whereas bitdepth is optional.  This is
//  because zoom cannot be ignored since it must be reset when bpp is set
//  since they are a combination.
//
// We can set zoom two ways:
//  zoom > 0 means we zoom horiz and vert by zoom (ignore xres,yres)
//  zoom = 0 means xres,yres are absolute horiz and vert dimensions
// If bpp = 0, we ignore it.
//
long CAviMovie::MovieSetDestOptions (float zoom, int bpp, int altMode,
                                     long xres, long yres)
{
	if (!m_paviVideo || !m_pVCM)
		return (MOVERR_NOVIDEOSTREAM);

	// We can set options only when stopped
	if (m_bPlaying)
		return (MOVERR_PLAYING);

	// Get src/dest bitmaps
	BITMAPINFOHEADER biSrc, biDst;
	m_pVCM->vcmGetSrcBi (&biSrc);
	m_pVCM->vcmGetDestBi (&biDst);

	// Get src/dest rects
	RECT rcSrc, rcDst;
	m_pVCM->vcmGetSrcRect (&rcSrc);
	m_pVCM->vcmGetDestRect (&rcDst);

	// Default current bitdepth
	if (bpp == 0)
		bpp = biDst.biBitCount;
	
	if (bpp != 8 && bpp != 16 && bpp != 24 && bpp != 32)
		return (MOVERR_BADOPTION);

	// zoom overides xres, yres
	if (zoom > 0)
	{
		xres = (long) (float (biSrc.biWidth) * zoom);
		yres = (long) (float (biSrc.biHeight) * zoom);
	}

	// We default to reset destination rect here also - but you can also
	//  leave it as is and change it seperately.

	biDst.biBitCount = bpp;

	// Set bitmap
	biDst.biWidth  = xres;
	biDst.biHeight = yres;
	biDst.biSizeImage = xres*ALIGNULONG(yres)*bpp/8;

	// Set destination rect
	rcDst.left   = rcSrc.left;
	rcDst.right  = rcSrc.left + xres;
	rcDst.top    = rcSrc.top;
	rcDst.bottom = rcSrc.top + yres;


	// Query destination format
	if (m_pVCM->vcmQueryDest (&biDst, rcDst.left, rcDst.top, rcDst.left+rcDst.right, rcDst.top+rcDst.bottom) == 0)
	{
		// Success - 
		m_pVCM->vcmSetDestBi (&biDst);
		m_pVCM->vcmSetDestRect (&rcDst);
		m_pVCM->vcmSetOutRect (&rcDst);

      if(altMode)
      {
         if( !m_pVCM->vcmSetZoomMode(altMode, 0))
            return (MOVERR_BADOPTION);
      }
	}
	else
	{
		// Query failed - probably a combination of zoom and bitdepth.
		// Try native zoom and do stretching ourselves.

		// Set bitmap to native res
		biDst.biWidth  = biSrc.biWidth;
		biDst.biHeight = biSrc.biHeight;
		biDst.biSizeImage = biDst.biWidth*ALIGNULONG(biDst.biHeight)*bpp/8;

		// Note using source rect as dest rect
		if (m_pVCM->vcmQueryDest (&biDst, rcSrc.left, rcSrc.top, rcSrc.left+rcSrc.right, rcSrc.top+rcSrc.bottom))
		{
			//failed...
			return (MOVERR_BADOPTION);
		}
		else
		{
			m_pVCM->vcmSetDestBi (&biDst);
			m_pVCM->vcmSetDestRect (&rcSrc);
			m_pVCM->vcmSetOutRect (&rcDst);  // Set output rect for stretching
		}
	}

	// Re-decode current frame with new options
	vidsResetDraw ();

	return (MOVERR_OK);
}


long CAviMovie::MovieSetDestPalette (LOGPALETTE* pLogPal)
{

	if (!m_paviVideo || !m_pVCM)
		return (MOVERR_NOVIDEOSTREAM);

   if(m_pVCM->vcmSetPalette(pLogPal))
      return(MOVERR_OK);
   else
      return(MOVERR_BADOPTION);
}



long CAviMovie::MovieSetBackground
   (
   char* lpszBmpName
   )
{
   BITMAPINFOHEADER bmih;

	if (!m_paviVideo || !m_pVCM)
		return (MOVERR_NOVIDEOSTREAM);

	// We can set options only when stopped
	if (m_bPlaying)
		return (MOVERR_PLAYING);

	if(lpszBmpName)
   {

      m_pVCM->vcmGetDestBi(&bmih);

      m_pDIB = new DIB;
      if(!m_pDIB->Load(GetActiveWindow(), lpszBmpName, bmih.biBitCount))
         return(MOVERR_BADOPTION);
   }      
	
   if(m_pVCM->vcmSetBackground(m_pDIB))
      return(MOVERR_OK);
   else
      return(MOVERR_BADOPTION);

}
