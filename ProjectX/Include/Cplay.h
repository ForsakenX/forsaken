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
#ifndef CPLAY_INC
#define CPLAY_INC
#define APP_NAME "Player"

typedef struct
{
    char szAppName[ 32 ];
    HANDLE hInstance;
    HWND hwndMainFrame;
    BOOL bFatalError;
} AppVars;

void PlayerMessageBox( UINT nResource );

int PASCAL PlayMovie( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    int nCmdShow , HWND Hwnd);


void PlayVideoInWindow(HWND hTargetWindow);
void	NewDoMainLoop( void );

#endif	// CPLAY_INC