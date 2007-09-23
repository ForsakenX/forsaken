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

typedef enum tagState {Uninitialized, Stopped, Paused, Playing } State;

typedef struct tagMedia
{
    State state;
    IGraphBuilder *pGraph;
    HANDLE hGraphNotifyEvent;
} Media;

BOOL InitMedia();
void OpenMediaFile( HWND hwnd, LPSTR szFile );
HANDLE GetGraphEventHandle( void );

BOOL CanPlay();
BOOL CanStop();
BOOL CanPause();
BOOL IsInitialized();
void DeleteContents();

// Event handlers

void OnMediaPlay();
void OnMediaPause();
void OnMediaStop();
void OnMediaAbortStop();
void OnGraphNotify();

