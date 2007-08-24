//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1992 - 1996  Microsoft Corporation.  All Rights Reserved.
//
//--------------------------------------------------------------------------;

#include "stdwin.h"
#include <evcode.h>
#include "cplay.h"
#include "media.h"

// Current multimedia variables
Media media;

BOOL	QuitIt = FALSE;
//
// CanPlay
//
// Return true if we can go to a playing state from our current state
//
BOOL CanPlay()
{
    return (media.state == Stopped || media.state == Paused);
}


//
// CanStop
//
// Return true if we can go to a stopped state from our current state
//
BOOL CanStop()
{
    return (media.state == Playing || media.state == Paused);
}


//
// CanPause
//
// Return true if we can go to a paused state from our current state
//
BOOL CanPause()
{
    return (media.state == Playing || media.state == Stopped);
}


//
// IsInitialized
//
// Return true if we have loaded and initialized a multimedia file
//
BOOL IsInitialized()
{
    return (media.state != Uninitialized);
}


//
// ChangeStateTo
//
void ChangeStateTo( State newState )
{
    media.state = newState;

    // update the toolbar
}


//
// InitMedia
//
// Initialization
//
BOOL InitMedia()
{
    ChangeStateTo( Uninitialized );

    media.hGraphNotifyEvent = NULL;
    media.pGraph = NULL;

    return TRUE;
}


//
// CreateFilterGraph
//
BOOL CreateFilterGraph()
{
    IMediaEvent *pME;
    HRESULT hr;

//    ASSERT(media.pGraph == NULL);

    hr = CoCreateInstance(&CLSID_FilterGraph,           // CLSID of object
			  NULL,                         // Outer unknown
			  CLSCTX_INPROC_SERVER,         // Type of server
			  &IID_IGraphBuilder,           // Interface wanted
			  (void **) &media.pGraph);     // Returned object
    if (FAILED(hr)){
	media.pGraph = NULL;
	return FALSE;
    }


    
	
	// We use this to find out events sent by the filtergraph

    hr = media.pGraph->lpVtbl->QueryInterface(media.pGraph, &IID_IMediaEvent, (void **) &pME);
    if (FAILED(hr)) {
	DeleteContents();
	return FALSE;
    }

    hr = pME->lpVtbl->GetEventHandle(pME, (OAEVENT*) &media.hGraphNotifyEvent);
    pME->lpVtbl->Release( pME );

    if (FAILED(hr)) {
	DeleteContents();
	return FALSE;
    }

    return TRUE;

} // CreateFilterGraph


// Destruction
//
// DeleteContents
//
void DeleteContents()
{
    if (media.pGraph != NULL) {
	media.pGraph->lpVtbl->Release( media.pGraph );
	media.pGraph = NULL;
    }

    // this event is owned by the filter graph and is thus invalid
    media.hGraphNotifyEvent = NULL;

    ChangeStateTo( Uninitialized );
}


//
// RenderFile
//
BOOL RenderFile( LPSTR szFileName )
{
    HRESULT hr;
    WCHAR wPath[MAX_PATH];

    DeleteContents();

    if ( !CreateFilterGraph() ) {
//	PlayerMessageBox( IDS_CANT_INIT_QUARTZ );
	return FALSE;
    }

    MultiByteToWideChar( CP_ACP, 0, szFileName, -1, wPath, MAX_PATH );

    SetCursor( LoadCursor( NULL, IDC_WAIT ) );
    hr = media.pGraph->lpVtbl->RenderFile(media.pGraph, wPath, NULL);
    SetCursor( LoadCursor( NULL, IDC_ARROW ) );

    if (FAILED( hr )) {
//	PlayerMessageBox( IDS_CANT_RENDER_FILE );
	return FALSE;
    }
    return TRUE;

} // RenderFile

//
// OpenMediaFile
//
// File..Open has been selected
//
void OpenMediaFile( HWND hwnd, LPSTR szFile )
{
    static char szFileName[ _MAX_PATH ];
    static char szTitleName[ _MAX_FNAME + _MAX_EXT ];

    if( szFile != NULL && RenderFile( szFile ))
    {
	LPSTR szTitle;

	// Work out the full path name and the file name from the
	// specified file
	GetFullPathName( szFile, _MAX_PATH, szFileName, &szTitle );
	strncpy( szTitleName, szTitle, _MAX_FNAME + _MAX_EXT );
	szTitleName[ _MAX_FNAME + _MAX_EXT -1 ] = '\0';

	ChangeStateTo( Stopped );

    }

} // OpenMediaFile


//
// OnMediaPlay
//
// There are two possible UI strategies for an application: you either play
// from the start each time you stop, or you play from the current position,
// in which case you have to explicitly rewind at the end. If you want the
// play from current and rewind at end, enable this code, if you want the
// other option, then enable FROM_START in both OnMediaStop and OnAbortStop.

#undef REWIND
#define FROM_START

void OnMediaPlay()
{
    if( CanPlay() ){
	HRESULT hr;
	IMediaControl *pMC;

	// Obtain the interface to our filter graph
	hr = media.pGraph->lpVtbl->QueryInterface(media.pGraph, &IID_IMediaControl, (void **) &pMC);

	if( SUCCEEDED(hr) ){
#ifdef REWIND
	    IMediaPosition * pMP;
	    hr = media.pGraph->lpVtbl->QueryInterface(media.pGraph,
		                                      &IID_IMediaPosition,
		                                      (void**) &pMP);
	    if (SUCCEEDED(hr)) {
		// start from last position, but rewind if near the end
		REFTIME tCurrent, tLength;
		hr = pMP->lpVtbl->get_Duration(pMP, &tLength);
		if (SUCCEEDED(hr)) {
		    hr = pMP->lpVtbl->get_CurrentPosition(pMP, &tCurrent);
		    if (SUCCEEDED(hr)) {
			// within 1sec of end? (or past end?)
			if ((tLength - tCurrent) < 1) {
			    pMP->lpVtbl->put_CurrentPosition(pMP, 0);
			}
		    }
		}
		pMP->lpVtbl->Release(pMP);
	    }
#endif
	    // Ask the filter graph to play and release the interface
	    hr = pMC->lpVtbl->Run( pMC );
	    pMC->lpVtbl->Release( pMC );

	    if( SUCCEEDED(hr) ){
		ChangeStateTo( Playing );
		return;
	    }
	}

	// Inform the user that an error occurred
//	PlayerMessageBox( IDS_CANT_PLAY );

    }

} // OnMediaPlay


//
// OnMediaPause
//
void OnMediaPause()
{
    if( CanPause() ){
	HRESULT hr;
	IMediaControl *pMC;

	// Obtain the interface to our filter graph
	hr = media.pGraph->lpVtbl->QueryInterface(media.pGraph, &IID_IMediaControl, (void **) &pMC);

	if( SUCCEEDED(hr) ){
	    // Ask the filter graph to pause and release the interface
	    hr = pMC->lpVtbl->Pause( pMC );
	    pMC->lpVtbl->Release( pMC );

	    if( SUCCEEDED(hr) ){
		ChangeStateTo( Paused );
		return;
	    }
	}

	// Inform the user that an error occurred
//	PlayerMessageBox( IDS_CANT_PAUSE );
    }

} // OnMediaPause


//
// OnMediaAbortStop
//
// Called when we get an EC_USER_ABORT or EC_ERROR_ABORT event
//
void OnMediaAbortStop(void)
{
    if(CanStop())
    {
	HRESULT hr;
	IMediaControl *pMC;

	// Obtain the interface to our filter graph
	hr = media.pGraph->lpVtbl->QueryInterface(media.pGraph, &IID_IMediaControl, (void **) &pMC);

	if( SUCCEEDED(hr) ){
#ifdef FROM_START
	    IMediaPosition * pMP;
#endif
	    // Ask the filter graph to stop and release the interface
	    hr = pMC->lpVtbl->Stop( pMC );
	    pMC->lpVtbl->Release( pMC );

#ifdef FROM_START

	    // if we want always to play from the beginning
	    // then we should seek back to the start here
	    // (on app or user stop request, and also after EC_COMPLETE).
	    hr = media.pGraph->lpVtbl->QueryInterface(
			media.pGraph,
			&IID_IMediaPosition,
			(void**) &pMP);
	    if (SUCCEEDED(hr)) {
		pMP->lpVtbl->put_CurrentPosition(pMP, 0);
		pMP->lpVtbl->Release(pMP);
	    }

	    // no visible rewind or we will re-show the window!

#endif

	    if( SUCCEEDED(hr) ){
		ChangeStateTo( Stopped );
		return;
	    }
	}
	// Inform the user that an error occurred
//	PlayerMessageBox( IDS_CANT_STOP );
    }

} // OnMediaAbortStop


//
// OnMediaStop
//
// There are two different ways to stop a graph. We can stop and then when we
// are later paused or run continue from the same position. Alternatively the
// graph can be set back to the start of the media when it is stopped to have
// a more CDPLAYER style interface. These are both offered here conditionally
// compiled using the FROM_START definition. The main difference is that in
// the latter case we put the current position to zero while we change states
//
void OnMediaStop()
{
    if( CanStop() ){
	HRESULT hr;
	IMediaControl *pMC;

	// Obtain the interface to our filter graph
	hr = media.pGraph->lpVtbl->QueryInterface(media.pGraph, &IID_IMediaControl, (void **) &pMC);
	if( SUCCEEDED(hr) ){

#ifdef FROM_START
	    IMediaPosition * pMP;
	    OAFilterState state;

	    // Ask the filter graph to pause
	    hr = pMC->lpVtbl->Pause( pMC );

	    // if we want always to play from the beginning
	    // then we should seek back to the start here
	    // (on app or user stop request, and also after EC_COMPLETE).
	    hr = media.pGraph->lpVtbl->QueryInterface(media.pGraph,
						      &IID_IMediaPosition,
						      (void**) &pMP);
	    if (SUCCEEDED(hr)) {
		pMP->lpVtbl->put_CurrentPosition(pMP, 0);
		pMP->lpVtbl->Release(pMP);
	    }

	    // wait for pause to complete
	    pMC->lpVtbl->GetState(pMC, INFINITE, &state);
#endif

	    // now really do the stop
	    pMC->lpVtbl->Stop( pMC );
	    pMC->lpVtbl->Release( pMC );
	    ChangeStateTo( Stopped );

		QuitIt = TRUE;
		return;
	}

	// Inform the user that an error occurred
//	PlayerMessageBox( IDS_CANT_STOP );
    }

} // OnMediaStop


//
// GetGraphEventHandle
//
// We use this to check for graph events
//
HANDLE GetGraphEventHandle()
{
    return media.hGraphNotifyEvent;

} // GetGraphEventHandle


//
// OnGraphNotify
//
// If the event handle is valid, then ask the graph if
// anything has happened (eg the graph has stopped...)
//
void OnGraphNotify()
{
    IMediaEvent *pME;
    long lEventCode, lParam1, lParam2;

//    ASSERT( media.hGraphNotifyEvent != NULL );

    if( SUCCEEDED(media.pGraph->lpVtbl->QueryInterface(media.pGraph, &IID_IMediaEvent, (void **) &pME))){
	if( SUCCEEDED(pME->lpVtbl->GetEvent(pME, &lEventCode, &lParam1, &lParam2, 0))) {
	    if (lEventCode == EC_COMPLETE) {
//		OnMediaAbortStop();
		OnMediaStop();
	    } else if ((lEventCode == EC_USERABORT) ||
		       (lEventCode == EC_ERRORABORT)) {
		OnMediaAbortStop();
	    }
	}
	pME->lpVtbl->Release( pME );
    }
}


