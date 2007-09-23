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
#define	INITGUID
#include "stdwin.h"
#include "cplay.h"
#include "media.h"
#include <evcode.h>


extern	Media media;

//
//
// What this sample illustrates
//
// A simple ActiveMovie application written in C.
//
//
// Summary
//
// This is an ActiveMovie application that is written in C. We create a window
// that has a simple menu allowing files to be opened and closed. The window
// also has three buttons for stopping, pausing and running the media. We are
// written to call the standard filtergraph interfaces. The only complication
// we have added is two UI alternatives. The filter can either be compiled to
// rewind to the start of the media when it is stopped, or alternatively it
// can leave the current position (and therefore frame) wherever it is stopped
//


#define UNTITLED_STRING " - Untitled"
#define nMaxResourceStrSize 128

AppVars appVars;

extern	BOOL QuitIt;
BOOL	MoviePlaying;


//
// InitApplication
//
BOOL InitApplication()
{
    strcpy( appVars.szAppName, APP_NAME );

    // Filter interface initialize?
    if( SUCCEEDED( CoInitialize( NULL )))
		return TRUE;

    return FALSE;
}


//
// UnInitApplication
//
void UnInitApplication()
{
    CoUninitialize( );
}


//
// DoMainLoop
//
// Main message loop
//
UINT DoMainLoop()
{
    MSG msg;
    HANDLE  ahObjects[1];       // Handles that need to be waited on
    const int cObjects = 1;     // Number of objects that we have

    // Message loop lasts until we get a WM_QUIT message
    // Upon which we shall return from the function

    while (TRUE) {
        if( (ahObjects[ 0 ] = GetGraphEventHandle()) == NULL ) {
            WaitMessage();
        } else {

            // Wait for any message or a graph notification

            DWORD Result = MsgWaitForMultipleObjects( cObjects,
                                                      ahObjects,
                                                      FALSE,
                                                      INFINITE,
                                                      QS_ALLINPUT);

            // Have we received an event notification

            if( Result != (WAIT_OBJECT_0 + cObjects) )
            {
                if( Result == WAIT_OBJECT_0 ) {
                    OnGraphNotify();
                }
				if( QuitIt == TRUE )
				{
//					OnMediaPlay();
//					QuitIt = FALSE;
					return TRUE;
				}
                continue;
            }
        }

        // Read all of the messages in this next loop
        // removing each message as we read it

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return msg.wParam;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

} // DoMainLoop


//
// WinMain
//
// Application entry point
//
int PASCAL PlayMovie( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    int nCmdShow, HWND Hwnd )
{
    UINT nReturn;

	QuitIt = FALSE;
    // Initialise COM and the application
    if ( InitApplication() == FALSE ) return 0;

    if( InitMedia() )
    {
    OpenMediaFile( Hwnd, "data\\movies\\Bike1.avi" );

	OnMediaPlay();

	
	nReturn = DoMainLoop();

	// Stop the graph if we can
	if( CanStop() )
            OnMediaStop();

        // Release the filter graph
	DeleteContents();
    }

    UnInitApplication();
    return nReturn;

} // WinMain


IGraphBuilder *pigb;
IVideoWindow *pivw;
IMediaEvent *pime;
IMediaControl *pimc;
HANDLE hGraphNotifyEvent;

void PlayVideoInWindow(HWND hTargetWindow)
{   // PlayVideoInWindow //
    HRESULT hr;
    RECT rc;
//    long l;
    WCHAR wPath[MAX_PATH];


	MultiByteToWideChar( CP_ACP, 0, "data\\movies\\cpc8mono.avi", -1, wPath, MAX_PATH );
	
	CoInitialize(NULL);

    // Create an empty filter graph object
	hr = CoCreateInstance(&CLSID_FilterGraph,
        NULL,
        CLSCTX_INPROC_SERVER,
        &IID_IGraphBuilder,
        (void **)&pigb);

    if (FAILED(hr))
        return;

    hr = pigb->lpVtbl->RenderFile( pigb, wPath, NULL);


    if (FAILED(hr)) {
        pigb->lpVtbl->Release(pigb);
        return;
        }   // Bail out, file probably wasn't found! //

    hr = pigb->lpVtbl->QueryInterface( pigb , &IID_IVideoWindow, (void **)&pivw);
    pivw->lpVtbl->put_Owner( pivw , (OAHWND)hTargetWindow);

	// pass all messages on to the parent window...
    pivw->lpVtbl->put_MessageDrain( pivw , (OAHWND)hTargetWindow);

    // Here's the key: we must set the required flags, 
    //   AND set the position
    pivw->lpVtbl->put_WindowStyle( pivw ,  WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS );

    GetClientRect(hTargetWindow, &rc);
//	rc.left = 0;
//	rc.top = 0;
//	rc.right = 320;
//	rc.bottom = 200;

    pivw->lpVtbl->SetWindowPosition( pivw , rc.left, rc.top, rc.right, rc.bottom);

    hr = pigb->lpVtbl->QueryInterface( pigb , &IID_IMediaControl, (void **)&pimc);

    hr = pigb->lpVtbl->QueryInterface( pigb, &IID_IMediaEvent, (void **)&pime);
    pimc->lpVtbl->Run( pimc);

    hr = pime->lpVtbl->GetEventHandle(pime, (OAEVENT*) &hGraphNotifyEvent);


	MoviePlaying = TRUE;
#if 0	
    pime->lpVtbl->WaitForCompletion( pime, INFINITE, &l);

#else
	NewDoMainLoop();
#endif	
	MoviePlaying = FALSE;
	
	// The following MUST be called otherwise nasty things can happen!
//    pivw->lpVtbl->put_Owner(pivw, (OAHWND) NULL);
//    pime->lpVtbl->Release(pime);

    pimc->lpVtbl->Release(pimc);
						      
    pivw->lpVtbl->Release(pivw);
    pigb->lpVtbl->Release(pigb);

    CoUninitialize();

}   // PlayVideoInWindow 




void	NewDoMainLoop( void )
{
    long lEventCode, lParam1, lParam2;
    MSG msg;
    HANDLE  ahObjects[1];       // Handles that need to be waited on
    const int cObjects = 1;     // Number of objects that we have

    // Message loop lasts until we get a WM_QUIT message
    // Upon which we shall return from the function

    while (TRUE) {
        if( (ahObjects[ 0 ] = hGraphNotifyEvent) == NULL ) {
            WaitMessage();
        } else {

            // Wait for any message or a graph notification

            DWORD Result = MsgWaitForMultipleObjects( cObjects,
                                                      ahObjects,
                                                      FALSE,
                                                      INFINITE,
                                                      QS_ALLINPUT);

            // Have we received an event notification

            if( Result != (WAIT_OBJECT_0 + cObjects) )
            {

				lEventCode = !EC_COMPLETE;
                
				if( Result == WAIT_OBJECT_0 ) {

					SUCCEEDED(pime->lpVtbl->GetEvent(pime, &lEventCode, &lParam1, &lParam2, 0));
				}
				if( lEventCode == EC_COMPLETE )
				{
					return;
				}
               continue;
            }
        }

        // Read all of the messages in this next loop
        // removing each message as we read it

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

