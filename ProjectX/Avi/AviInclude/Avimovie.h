
// aviMovie.h : Interface of the CAviMovie class
//

// This code and information is provided "as is" without warranty of
// any kind, either expressed or implied, including but not limited to
// the implied warranties of merchantability and/or fitness for a
// particular purpose.

// Copyright (C) 1995 Intel Corporation.  All rights reserved.
//

#ifndef _AVIMOVIE_H_
#define _AVIMOVIE_H_

#define OLE2ANSI
#include <vfw.h>
#undef OLE2ANSI

// Since we need VCM services
#include "videoVCM.h"
// and DIBs
#include "dib.h"

///////////////////////////////////////////////////////////////////////////////
//

// Messages
#define AVIMOV_NOTIFYSIZE	(WM_USER+1) // tell parent to resize

// Maximum # streams open at once
#define MAXSTREAMS 16

// Maximum frames of audio skew
#define MAX_AUDIOBUFS 8

// Error codes
#define MOVERR_OK				0L
#define MOVERR_NOVIDEOSTREAM 	-1L
#define MOVERR_VIDS				-2L
#define MOVERR_AUDS				-3L
#define MOVERR_PLAYING			-4L
#define MOVERR_BADOPTION		-5L

#define AUDSERR_OK				0L
#define AUDSERR_NOAUDIO			-10L
#define AUDSERR_MAXBUFSEXCEEDED	-11L
#define AUDSERR_UNSUPPORTEDFORMAT -12L
#define AUDSERR_OPEN			-13L
#define AUDSERR_WAVEOUT			-14L

#define VIDSERR_OK				0L
#define VIDSERR_NOVIDEO			-20L
#define VIDSERR_VCM				-21L
#define VIDSERR_DC				-22L
#define VIDSERR_NOTSTARTED		-23L
#define VIDSERR_READ			-24L

///////////////////////////////////////////////////////////////////////////////
// CAviV Defininition

class CAviMovie
{
public:
	CAviMovie ();
	~CAviMovie ();

// VCM
// WARNING: should only access VCM during stopped mode (for accesses that will
//  change the state).
	CVideoVCM * m_pVCM;

   DIB* m_pDIB;

// Avi File
	// Operations
public:
	long	FileOpen (LPCTSTR lpszPathName);
	long	FileClose ();

	// Information
	PAVIFILE	m_pfile;
	PAVISTREAM	m_paviVideo;	// video stream to play
	PAVISTREAM	m_paviAudio;	// audio stream to play

// Movie
	// Operations
public:
	HWND	m_hwnd;

	long	MovieOpen (HWND hwnd);
	long	MovieClose ();

	long	MovieStart (HDC hdc);
	long	MovieStop ();
	long	MoviePause ();

	long	MovieSeek (long lpos);         // absolute seek from beginning of file
	long	MovieSeekCurrent (long ldiff); // relative seek from current pointer

		// Set destination options, i.e. resolution and/or bpp
	long	MovieSetDestOptions (float zoom, int bpp = 0, int altMode = 0, long xres = 0, long yres = 0);
   long  MovieSetDestPalette (LOGPALETTE* pLogPal);

   long  MovieSetBackground (char * lpszBmpName);

		// Miscellaneous queries
	BOOL	MovieIsFileOpen () { return (m_pfile != 0); }
	BOOL	MovieIsPlaying () { return (m_bPlaying); }
	BOOL	MovieIsEnd () { return (m_vidsCurrent == m_vidsLast); }

	UINT	MovieGetTimeslice () { return (UINT) (1.0/m_frate*1000.0); }
	float	MovieGetFrameRate () { return m_frate; }

	// Information
protected:
	BOOL	m_bPlaying;	// F if stopped/paused
	BOOL	m_bPaused;      // T if paused
	
		// These are the same for now, can change the frate for faster/slower playback
	float	m_fps;          // frame rate of file
	float	m_frate;        // frame rate of playback

	// Syncronization
public:
	long	vidsTimeToSample (long lTime);
	long	vidsSampleToTime (long lSample);
private:
	UINT	m_resolution;   // minimum timing resolution for current machine

// Video
	// Operations
public:
	long	vidsVideoOpen ();
	long	vidsVideoClose ();

		// These functions can change state of m_bPlaying.
	long	vidsVideoStart (HDC hdc, long fStart = -1L);
	long	vidsVideoStop ();
	
	long	vidsVideoDraw ();
	
	long	vidsGetTime () { return ((long) (float(m_vidsCurrent-m_vidsFirst) / m_fps * 1000.0)); }
	long	vidsGetFrame () { return m_vidsCurrent; }

	int	vidsGetNumFrames () { return m_vidsLast-m_vidsFirst+1; }

		// frame=-1 means frame=m_vidsCurrent
	BOOL	vidsIsKey (long frame = -1);

		// Will always decode current frame
	void	vidsResetDraw () { m_vidsPrevious = -1; }

	// Information
	HDRAWDIB	m_hdd;
	HDC		m_hdc;

	void	vidsResetSkipCount () { m_play_fskipped = 0; }
	int	vidsFramesSkipped () { return m_play_fskipped; }

protected:
	long	m_timePlayStart, m_timePlayStartPos;

	int	m_play_fprev;   // to help us calculate ...
	int	m_play_fskipped;// frames skipped since last play

private:
	// Internal functions/vars
	BOOL	vidsSync ();
	void	vidsCatchup ();

	long	m_vidsCurrent;  // attempted frame to draw
	long	m_vidsPrevious; // last successfully decoded frame

	long	m_vidsPrevKey, m_vidsNextKey;

	long	m_vidsFirst, m_vidsLast;

	unsigned char * m_pvBuf; // for optimization - storage for returning frame bits
	int	m_cbvBuf;          // count bytes of above buffer
   RECT  m_Rect;           // bounding rectangle for transparency

// Audio
	// Operation
public:
	long	audsWaveOpen (HWND hwnd);
	long	audsWaveClose ();
	
	long	audsWavePlay (long fStart, long fEnd, BOOL bFrameSamp);
	long	audsWaveStop ();
	long	audsWavePause ();
	long	audsWaveRestart ();

	long	audsGetTime ();        // postion in ms since audsWavePlay ()
	long	audsFillBuf (int nLen);
	long	audsOutBuf ();
	long	audsDoneBuf ();

	// Information
	long	audsTimeToSample (long lTime);

protected:
	HWAVEOUT m_hWaveOut;
	
	long	m_timeAudioStart;

	long	m_audsFirst, m_audsLast; // first and last audio frame
	long	m_audsSkew; // # frames skew

	long	m_sampsPerSec;
	long	m_bytesPerSec;
	long	m_sampSize;      // size (bytes) of one audio sample
	long	m_sampsPerFrame; // # samples in one frame (1/fps sec)

private:
	long	m_audsCurrent; // current frame of audio during playback

	WAVEHDR	* m_audsBuf [MAX_AUDIOBUFS]; // data follows
	int	m_iaudsFreeBuf; // points to next free buffer
	int	m_iaudsPlayBuf; // points to next buffer to play
	int	m_naudsBufsOut; // number to buffers currently out
};

#endif // _AVIMOVIE_H_
