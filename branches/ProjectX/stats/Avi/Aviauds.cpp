// aviAuds.cpp : Implementation of audio for the CAviMovie class
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

void EndProcess (int);

#include "aviMovie.h"

extern "C" void DebugPrintf( const char * format, ... );

/* Applications should not call any system-defined functions from inside 
 * a callback function, except for PostMessage, timeGetSystemTime, 
 * timeGetTime, timeSetEvent, timeKillEvent, midiOutShortMsg, midiOutLongMsg,
 * and OutputDebugStr.
 */

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, 
    DWORD dwParam1, DWORD dwParam2)
{
   static CAviMovie* pMovie;

   switch( uMsg ) {

      case WOM_OPEN:
         /* Sent when the device is opened using the waveOutOpen function. 
          */

         pMovie = (CAviMovie*) dwInstance;
         break;

      case WOM_CLOSE:
         /* Sent when the device is closed using the waveOutClose function. 
          */

         pMovie = NULL;
         break;

      case WOM_DONE:
         /* Sent when the device driver is finished with a data block sent
          * using the waveOutWrite function.                               
          */

         if( pMovie != NULL )
            pMovie->audsDoneBuf();
         break;

   }
}






///////////////////////////////////////////////////////////////////////////////
// Audio Operations

// Basic strategy assumes 1:1 interleave with some audio skew
//
// Since we want to read the file sequentially we use relatively small audio
//  blocks (1/fps) constantly filling a circular buffer.
//
//
// For now, we pre-read audio skew frames (typically 1/fps * samples/sec samples)
//  then fill up buffer as we get to them.  This can be improved for 1:x
//  interleave by filling up buffer x frames at a time.
//

// Open audio:
//  hwnd - window that will receive WOM messages
//
long CAviMovie::audsWaveOpen (HWND hwnd)
{
	m_timeAudioStart = -1L;

	if (!m_paviAudio)
	//if ( 1 );
		return (AUDSERR_NOAUDIO);

	// Check if output device already opened
	if (m_hWaveOut)
		audsWaveClose ();
	
	// Get stream info
	AVISTREAMINFO avis;
	AVIStreamInfo (m_paviAudio, &avis, sizeof (avis));

	// Assume that dwSuggestedBufferSize is 1/fps worth of samples -
	//  if not case still okay since we will sync up correctly later
	m_sampsPerFrame = avis.dwSuggestedBufferSize * avis.dwSampleSize;
	m_sampSize      = avis.dwSampleSize;

	m_audsFirst = avis.dwStart;
	m_audsLast  = avis.dwStart + avis.dwLength / m_sampsPerFrame;
	m_audsSkew  = avis.dwInitialFrames;

	// Must have at least 1 free frame buffer
	// For 1:x interleave should be x free frame buffers
	if (m_audsSkew >= MAX_AUDIOBUFS)
	{
		audsWaveClose ();
		return (AUDSERR_MAXBUFSEXCEEDED);
	}

	// Read first frame to get source wave format
	long cb, rval;
	WAVEFORMATEX wfFormat;
	
	// Backward compatability for WAVEFORMAT
	wfFormat.cbSize = 0; // ignored for PCM, let's set anyway
	wfFormat.wBitsPerSample = m_sampSize == 1 ? 8 : 16; // must be 8 or 16 for PCM

	AVIStreamFormatSize (m_paviAudio, 0, &cb);
	rval = AVIStreamReadFormat (m_paviAudio, 0, &wfFormat, &cb);

	// Make sure it's a PCM file
	if (wfFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		audsWaveClose ();
		return (AUDSERR_UNSUPPORTEDFORMAT);
	}

	m_sampsPerSec = wfFormat.nSamplesPerSec;
	m_bytesPerSec = wfFormat.nAvgBytesPerSec;
	
	// Open a waveform output device
	MMRESULT mmrval =
	waveOutOpen (&m_hWaveOut,
	             WAVE_MAPPER,
	             &wfFormat,
	             (DWORD)waveOutProc,
	             (DWORD) this,
	             CALLBACK_FUNCTION
      );

	if (mmrval != 0)
	{
		audsWaveClose ();
		return (AUDSERR_OPEN);
	}

	// Initialize all audio buffers
	for (int i = 0; i < MAX_AUDIOBUFS; i++)
	{
		m_audsBuf [i] = (WAVEHDR *)
		                new char [sizeof (WAVEHDR) + m_sampsPerFrame*m_sampSize];
		if (!m_audsBuf [i])
			EndProcess (ERROR_NOT_ENOUGH_MEMORY);
		
		m_audsBuf [i]->dwBufferLength = m_sampsPerFrame*m_sampSize;
		m_audsBuf [i]->dwFlags = 0L;
		m_audsBuf [i]->lpData = (char *) m_audsBuf[i] + sizeof (WAVEHDR);
		
		long mmrval =
		waveOutPrepareHeader (m_hWaveOut, m_audsBuf [i], sizeof (WAVEHDR));

		switch ( mmrval )
		{
		case MMSYSERR_NOERROR:
			DebugPrintf("Aviauds.cpp: audsWaveOpen MMSYSERR_NOERROR\n");
			break;
		case MMSYSERR_INVALHANDLE:
			DebugPrintf("Aviauds.cpp: audsWaveOpen MMSYSERR_INVALHANDLE\n");
			break;
		case MMSYSERR_NODRIVER:
			DebugPrintf("Aviauds.cpp: audsWaveOpen MMSYSERR_NODRIVER\n");
			break;
		case MMSYSERR_NOMEM:
			DebugPrintf("Aviauds.cpp: audsWaveOpen MMSYSERR_NOMEM\n");
			break;
		}

		if (mmrval != 0)
		{
			audsWaveClose ();
			return (AUDSERR_WAVEOUT);
		}
	}
	
	return (AUDSERR_OK);
}

// Close audio
//
long CAviMovie::audsWaveClose ()
{
	m_timeAudioStart = -1L;

	for (int i = 0; i < MAX_AUDIOBUFS; i++)
	{
		if (m_audsBuf [i])
		{
			waveOutUnprepareHeader (m_hWaveOut, m_audsBuf [i], sizeof (WAVEHDR));
		   delete [] m_audsBuf [i];
			m_audsBuf [i] = 0L;
		}
	}

	if (m_hWaveOut)
	{
		waveOutClose (m_hWaveOut);
		m_hWaveOut = 0L;
	}

	return (AUDSERR_OK);
}

// Play audio
//
// Params:
//  fStart - frame/sample to start play (0 first in stream, neg to ignore)
//  fEnd   - frame/sample to end play   (0 until end of stream)
//  bFrameSamp - fStart/fEnd is frame if TRUE else is sample
//
long CAviMovie::audsWavePlay (long fStart, long fEnd, BOOL bFrameSamp)
{
	long rval;
	
	// If paused, just restart the audio
	if (m_bPaused)
	{
		audsWaveRestart ();
		return (AUDSERR_OK);
	}

	// Open audio device
	if (rval = audsWaveOpen (m_hwnd))
		return (rval);

	if (bFrameSamp)
		m_audsCurrent = fStart;
	else
		m_audsCurrent = fStart/m_sampsPerFrame;
	
	m_timeAudioStart =
        AVIStreamSampleToTime (m_paviAudio, m_audsCurrent*m_sampsPerFrame);

	// Pause until buffers filled to get better synchronizaztion
	waveOutPause (m_hWaveOut);

	m_iaudsFreeBuf = 0;
	m_iaudsPlayBuf = 0;
	m_naudsBufsOut = 0;
	
	// Fill audio buffer with skewed audio; we need at least 7 frames of skew
	//  to get proper playback - this should later be taken care of in
	//  an i/o buffering mechanism
	if (m_audsSkew > 7)
		audsFillBuf (m_audsSkew);
	else
		audsFillBuf (7);

	// Start audio stream
	audsOutBuf ();

	waveOutRestart (m_hWaveOut);

	return (AUDSERR_OK);
}

// Stop immediately and clear buffers
//
long CAviMovie::audsWaveStop () 
{
	if (m_hWaveOut)
		waveOutReset (m_hWaveOut);
	else
		return (AUDSERR_NOAUDIO);

	// Stop on last played audio frame rather than last filled frame
	//  so when we start again there is no perceived audio skip
	m_audsCurrent -= m_naudsBufsOut;
	if (m_audsCurrent < 0)
		m_audsCurrent = 0;

	m_iaudsFreeBuf = 0;
	m_iaudsPlayBuf = 0;
	m_naudsBufsOut = 0;

	audsWaveClose ();

	return (AUDSERR_OK);
}

// Pause audio
//
long CAviMovie::audsWavePause ()
{
	if (!m_hWaveOut)
		return (AUDSERR_NOAUDIO);
	
	waveOutPause (m_hWaveOut);

	return (AUDSERR_OK);
}

// Restart paused audio
//
long CAviMovie::audsWaveRestart ()
{
	if (!m_hWaveOut)
		return (AUDSERR_NOAUDIO);
	
	waveOutRestart (m_hWaveOut);

	return (AUDSERR_OK);
}

// Get current time of playing stream since audsWavePlay
//
long CAviMovie::audsGetTime ()
{
	MMTIME	mmtime;
	long    lPos;

	if (!m_hWaveOut)
		return (AUDSERR_NOAUDIO);

	mmtime.wType = TIME_SAMPLES;

	MMRESULT mmrval =
	waveOutGetPosition (m_hWaveOut, &mmtime, sizeof (mmtime));
	if (mmrval != MMSYSERR_NOERROR)
		return (AUDSERR_WAVEOUT);

	if (mmtime.wType == TIME_SAMPLES)
	{
		lPos = long ((double (mmtime.u.sample * 1000)/double (m_sampsPerSec) + 0.5));
		lPos += m_timeAudioStart;
	}
	else if (mmtime.wType == TIME_BYTES)
	{
		lPos = long ((double (mmtime.u.cb * 1000)/double (m_bytesPerSec) + 0.5));
		lPos += m_timeAudioStart;
	}
	else
		lPos = -1;

	// Guard against errant audio cards - which either give bad values
	// or else will give same value always

	static int lPosLast;
	static int count;
   static DWORD startTime;

	if (lPos < 0)
      return -1;

   
   if(lPos == lPosLast)
   {
      if(count == 0)
      {
         startTime = timeGetTime();
         count = 1;
      }
      else
      {
		   count++;
         lPos += (timeGetTime() - startTime);
      }
   }
	else
	{
		count = 0;
		lPosLast = lPos;
	}
           

   /*   
   if(count > 15)
   {
      audsWaveStop();
      audsWavePlay(lPos);
   }
   */
   
   /*
	if (lPos == lPosLast)
		count++;
	else
	{
		count = 0;
		lPosLast = lPos;
	}

	if (count > 15)
      lPos = -1;	
   */
  
	return (lPos);
}

// Convert ms to sample
//
long CAviMovie::audsTimeToSample (long lTime)
{
	long lSamp =
	AVIStreamTimeToSample (m_paviVideo, lTime);

	return (lSamp);
}

///////////////////////////////////////////////////////////////////////////////
// Internal audio functions

// Fill nLen free buffers begining at audsStart
//
// Prereq:
//  m_audsCurrent contains starting frame to play, this will be updated sequentially
// Return:
//  number of audio frame buffers filled
long CAviMovie::audsFillBuf (int nLen)
{
	for (int i = 0; i < nLen; i++)
	{
		// Check if we are at end of audio stream
		if (m_audsCurrent > m_audsLast)
			break;

		long cb, rval =
		AVIStreamRead (m_paviAudio,
		               (m_audsCurrent+i) * m_sampsPerFrame,
			       m_sampsPerFrame,
			       m_audsBuf [m_iaudsFreeBuf]->lpData,
			       m_sampsPerFrame * m_sampSize,
			       &cb,
			       NULL);
		if (rval || cb != m_sampsPerFrame * m_sampSize)
			return (i);

		++m_iaudsFreeBuf %= MAX_AUDIOBUFS;
	}

	m_audsCurrent += i;
	return (i);
}

// Output all filled buffers of audio
//
// Return number of buffers filled.
//
long CAviMovie::audsOutBuf ()
{
	
	for (int i = 0; i < MAX_AUDIOBUFS; i++)
	{
		// Make sure we have free buffers
		if (m_naudsBufsOut >= MAX_AUDIOBUFS )
			break;

		// Did we get to end of filled buffers?
		if (m_iaudsPlayBuf == m_iaudsFreeBuf)
			break;
		
		MMRESULT mmrval =
		waveOutWrite (m_hWaveOut, m_audsBuf [m_iaudsPlayBuf], sizeof (WAVEHDR));
	
		++m_naudsBufsOut;
		++m_iaudsPlayBuf %= MAX_AUDIOBUFS;
	}

	return (i);
}

// One buffer has been finished with, fill another buffer then output it
//
long CAviMovie::audsDoneBuf ()
{
	if (m_timeAudioStart < 0L)
		return (-1L);

   if( m_bPlaying == FALSE )
		return (-1L);

	// Fill in one more buffer
	audsFillBuf (1);

	// Free one buffer count
	--m_naudsBufsOut;
	
	// Output filled buffer
	audsOutBuf ();

	return (0L);
}

