// aviVids.cpp : Implementation of video for the CAviMovie class
//

// This code and information is provided "as is" without warranty of
// any kind, either expressed or implied, including but not limited to
// the implied warranties of merchantability and/or fitness for a
// particular purpose.

// Copyright (C) 1995 Intel Corporation.  All rights reserved.
//

#include <windows.h>
#include <windowsx.h> 

#include "aviMovie.h"


extern "C" {
#include "XMem.h"
}


#ifdef _DEBUG
#include <stdio.h>
#include <assert.h>
#define ASSERT assert
#else
#define ASSERT / ## /
#endif
void EndProcess (int);

///////////////////////////////////////////////////////////////////////////////
// Video Operations

// Open and init VIDS
//
long CAviMovie::vidsVideoOpen ()
{
	if (!m_paviVideo)
		return (VIDSERR_NOVIDEO);

	// Check if opening w/o closing
	if (m_pVCM)
		vidsVideoClose ();

	m_timePlayStart = -1L;

	// Get stream info
	AVISTREAMINFO avis;
	AVIStreamInfo (m_paviVideo, &avis, sizeof (avis));

	m_vidsFirst = m_vidsCurrent = avis.dwStart;
	m_vidsLast  = avis.dwStart + avis.dwLength - 1;

	m_vidsPrevious = m_vidsCurrent-1;

	// Read first frame to get source info
	long cb, rval;
	BITMAPINFOHEADER biFormat;
	AVIStreamFormatSize (m_paviVideo, 0, &cb);
	if (cb != sizeof (BITMAPINFOHEADER))
		return (-1L);
	rval = AVIStreamReadFormat (m_paviVideo, 0, &biFormat, &cb);

	// Initiate VCM
	m_pVCM = (CVideoVCM *) new CVideoVCM ();
	
	// Open VCM for this instance
	if (m_pVCM->vcmOpen (avis.fccHandler, &biFormat))
	{
		vidsVideoClose ();
		return (VIDSERR_VCM);
	}

	// Create temporary buffer
	m_cbvBuf = biFormat.biHeight * ALIGNULONG (biFormat.biWidth)
	           * biFormat.biBitCount/8;
	m_pvBuf = (unsigned char *) malloc( m_cbvBuf);
	if (!m_pvBuf)
		EndProcess (ERROR_NOT_ENOUGH_MEMORY);
	
	// Reset internal skip count
	m_play_fskipped = 0;

	return (VIDSERR_OK);
}

// Close VIDS
//
long CAviMovie::vidsVideoClose ()
{
	if (m_pvBuf)
	{
		free(m_pvBuf);
		m_pvBuf = 0L;
	}
	if (m_pVCM)
	{
		delete m_pVCM;
		m_pVCM = 0L;
	}

	return (VIDSERR_OK);
}

// Start video, note that all decode/draw is done in vidsDraw()
//
// If fStart < 0, then start on current frame
//
long CAviMovie::vidsVideoStart (HDC hdc, long fStart /* = -1L */)
{
	if (!m_pVCM)
		return (VIDSERR_VCM);

	if (!hdc)
		return (VIDSERR_DC);
	m_hdc = hdc;

	if (fStart >= 0)
		m_vidsCurrent = fStart;
	
	// Begin VCM
	if (m_pVCM->vcmBegin (hdc, m_frate))
	{
		vidsVideoStop ();
		return (VIDSERR_VCM);
	}

	// Pass seperate start message to VCM only when playing
	if (m_bPlaying)
		m_pVCM->vcmDrawStart (m_frate);

	// Get start time
	m_timePlayStart    = timeGetTime ();
	m_timePlayStartPos = AVIStreamSampleToTime (m_paviVideo, m_vidsCurrent);
	
	// Init play stats
	m_play_fprev = m_vidsCurrent;
	
	return (VIDSERR_OK);
}

// Stop video
//
long CAviMovie::vidsVideoStop ()
{
	if (!m_hdc)
		return (VIDSERR_DC);

	m_pVCM->vcmDrawStop ();

	m_pVCM->vcmEnd ();

	m_timePlayStart = -1L;

	// Not responsible for releasing DC
	m_hdc = 0;
	
	return (VIDSERR_OK);
}
		   
// Draw current frame of video
//
long CAviMovie::vidsVideoDraw ()
{

	if (!m_pVCM)
		return (VIDSERR_VCM);

	// Check if started
	if (m_timePlayStart < 0)
		return (VIDSERR_NOTSTARTED);

	// Simple synchronization to the audio stream here:
	//  just query the audio driver the sample it is on then draw
	//  the video frame that corresponds to it.

	if (m_bPlaying)
	{
   	long lTime = audsGetTime ();

		// Sync to absolute time if no audio playing
      // this could also occur if the audio card is reporting incorrect timing info
		if (lTime < 0)
			lTime = m_timePlayStartPos + (timeGetTime () - m_timePlayStart);

		m_vidsCurrent = vidsTimeToSample (lTime);
		
		if (m_vidsCurrent > m_vidsLast)
			m_vidsCurrent = m_vidsLast;

		if (m_vidsCurrent == m_vidsPrevious)
      {
			// Going too fast!  Should actually return a ms
			//  count so calling app can Sleep() if desired.
			return (VIDSERR_OK);
		}

	}
	else
	{
		if (m_vidsCurrent > m_vidsLast)
			m_vidsCurrent = m_vidsLast;
		
		if (m_vidsCurrent == m_vidsPrevious)
		{							
			vidsResetDraw ();
		}
	}

	if (!vidsSync ())
		return (VIDSERR_OK); // don't draw this frame

	long rval =
	AVIStreamRead (m_paviVideo,
	               m_vidsCurrent,
		       1,
		       m_pvBuf,
		       m_cbvBuf,
		       NULL,
		       NULL);
	if (rval)
	{
		return (VIDSERR_READ);
	}

   /*
   if(rval = m_pBMP->bmpDraw(&m_Rect))
      return (VIDERR_VCM);
	*/

	if (rval = m_pVCM->vcmDraw (m_pvBuf))
		return (VIDSERR_VCM);

	m_vidsPrevious = m_vidsCurrent;
	
	if (m_bPlaying)
	{
		m_play_fskipped += (m_vidsCurrent-m_play_fprev-1);
		m_play_fprev = m_vidsCurrent;
	}

	return (VIDSERR_OK);
}

// Convert ms to sample (frame)
long CAviMovie::vidsTimeToSample (long lTime)
{
	long lSamp =
	AVIStreamTimeToSample (m_paviVideo, lTime);

	return (lSamp);
}

// Convert sample (frame) to ms
long CAviMovie::vidsSampleToTime (long lSamp)
{
	long lTime =
	AVIStreamSampleToTime (m_paviVideo, lSamp);

	return (lTime);
}

// TURE if frame is KEY, if frame < 0 then check current frame
BOOL CAviMovie::vidsIsKey (long frame /* = -1 */)
{
	if (!m_paviVideo)
		return FALSE;

	if (frame < 0)
		frame = m_vidsCurrent;
	
	return AVIStreamIsKeyFrame (m_paviVideo, frame);
}
	
///////////////////////////////////////////////////////////////////////////////
// Internal video routines

// Synchronization and Keyframe Management:
//  pretty simple plan, don't do anything too fancy.

BOOL CAviMovie::vidsSync ()
{
#define dist(x,y) ((x)-(y))
#define ABS_(x) (x<0 ? (-(x)) : (x))

	if (m_vidsCurrent < m_vidsPrevious) // seeked back - reset draw
      m_vidsPrevious = -1;

	if (dist (m_vidsCurrent, m_vidsPrevious) == 1)
	{
	// normal situation
		// fall thru and draw
	}
	else
	{
	// SKIPPED
		if (AVIStreamIsKeyFrame (m_paviVideo, m_vidsCurrent))
		{
		// we are on KF boundry just reset and start here
			m_vidsPrevKey = m_vidsCurrent;
			m_vidsNextKey = AVIStreamNextKeyFrame (m_paviVideo, m_vidsCurrent);
			// fall thru and draw
		}
		else if (dist (m_vidsCurrent, m_vidsPrevious) == 2)
		{
		// one frame off - just draw
			vidsCatchup ();
			// fall thru and draw
		}
		else
		{
		// We are greater than one frame off:
		//  if we went past a K frame, update K frame info then:
		//   if we are closer to previous frame than catchup and draw
		//   if we are closer to next KEY frame than don't draw
			if ( (m_vidsNextKey < m_vidsCurrent) || (m_vidsPrevKey > m_vidsCurrent) )   // seeked past previous key frame
			{
			// went past a K frame
				m_vidsPrevKey = AVIStreamPrevKeyFrame (m_paviVideo, m_vidsCurrent);
				m_vidsNextKey = AVIStreamNextKeyFrame (m_paviVideo, m_vidsCurrent);
			}

			if ( ABS_(dist (m_vidsCurrent, m_vidsPrevKey)) <= ABS_(dist (m_vidsCurrent, m_vidsNextKey)))
			{
				vidsCatchup ();
				// fall thru and draw
			}
			else
			{
				if (m_bPlaying)
					return (FALSE); // m_vidsPrev NOT updated
				else
					vidsCatchup (); // if not playing than we want to
					                // draw the current frame
			}
		}
	}

	return (TRUE);
}

// Readies to draw (but doesn't draw) m_vidsCurrent.
//  We just ICDECOMPRESS_HURRYUP frames from m_vidsPrevious or
//   m_vidsPrevKey, whichever is closer.
//  Updates m_vidsPrevious.
//
void CAviMovie::vidsCatchup ()
{
	if (m_vidsPrevious < m_vidsPrevKey)
		m_vidsPrevious = m_vidsPrevKey-1;

	long catchup = m_vidsPrevious+1;
	while (catchup < m_vidsCurrent)
	{
		long rval =
		AVIStreamRead (m_paviVideo,
		               catchup,
			       1,
			       m_pvBuf,
			       m_cbvBuf,
			       NULL,
			       NULL);
		if (rval)
		{
			break;
		}

		
		if (!m_bPlaying )
			m_pVCM->vcmDrawIn (m_pvBuf);
		else
			m_pVCM->vcmDrawIn (m_pvBuf, ICDECOMPRESS_HURRYUP);
		
		m_vidsPrevious = catchup++;
	}
}

