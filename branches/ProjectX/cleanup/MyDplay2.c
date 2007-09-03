/*
 * The X Men, Febuary 1997
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 36 $
 *
 * $Header: /PcProjectX/MyDplay2.c 36    11/11/98 16:00 Philipy $
 *
 * $Log: /PcProjectX/MyDplay2.c $
 * 
 * 36    11/11/98 16:00 Philipy
 * various fixes for warnings / errors when compiling under VC6
 * 
 * 35    14/08/98 9:13 Phillipd
 * DirectX6 is in effect.......
 * 
 * 34    4/27/98 4:01p Phillipd
 * 
 * 33    3/04/98 18:10 Collinsd
 * Moved position of xmem.h
 * 
 * 32    15/03/98 18:38 Philipy
 * fixed bug in SetupConnection
 * 
 * 31    3/06/98 4:27p Phillipd
 * 
 * 30    1/29/98 2:27p Phillipd
 * Demo can now be saved to ram....and can be saved if your not the
 * host...
 * 
 * 29    12/20/97 5:03p Phillipd
 * 
 * 28    12/17/97 5:19p Phillipd
 * 
 * 27    12/13/97 1:35p Phillipd
 * 
 * 26    12/12/97 10:22a Phillipd
 * Packets per second now dynamic
 * 
 * 25    12/02/97 3:47p Phillipd
 * No need for Sleep
 * 
 * 24    11/29/97 4:35p Phillipd
 * Xmem is now in effect...use it allways....
 * 
 * 23    26/11/97 9:39 Philipy
 * recieve thread now gives up remainder of timeslice when done ( Sleep(0)
 * )
 * 
 * 22    9/30/97 4:20p Phillipd
 * 
 * 21    9/29/97 11:55a Phillipd
 * Dawn of a new age with directplay 3
 * 
 * code and setting default values appropriate to demo attract mode
 * 
 * 19    17/07/97 15:38 Collinsd
 * BGObjects now use compobjs.
 * 
 * 18    8/07/97 16:30 Collinsd
 * Dicked about with include files FUCK!
 * 
 * 17    6/24/97 11:12a Phillipd
 * 
 * 16    6/14/97 1:51p Phillipd
 * 
 * 15    6/14/97 1:11p Phillipd
 * 
 * 14    6/11/97 3:48p Phillipd
 * 
 * 13    5/01/97 12:46p Phillipd
 * 
 * 12    4/10/97 4:29p Phillipd
 * DirectPlay3 is here...
 * 
 * 11    3/11/97 4:55p Phillipd
 * Demo Interpolate stuff is now working but not finished...
 * 
 * 10    3/05/97 4:36p Phillipd
 * 
 * 9     3/05/97 10:31a Phillipd
 * 
 * 8     3/01/97 12:57p Phillipd
 * handles multiple join requests from 1 person,,,,
 * 
 * 7     2/25/97 4:05p Phillipd
 * 
 * 6     2/25/97 2:51p Phillipd
 * 
 * 5     2/24/97 10:00a Phillipd
 * Demo mode with multispeed has been added..
 * 
 * 4     2/12/97 11:43a Phillipd
 * 
 * 3     2/12/97 9:31a Phillipd
 * 
 * 2     2/11/97 5:11p Phillipd
 * 
 * 1     2/10/97 10:01a Phillipd
 */

#define IDIRECTPLAY2_OR_GREATER
#include "typedefs.h"
#include "main.h"

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <dplay.h>
#include <stdio.h>

#include "MyDplay2.h"
#include "title.h"

#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"

#include <stdio.h>
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "Node.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
#include "file.h"
#include "triggers.h"
#include "models.h"
#include "enemies.h"
#include "mload.h"

#include "d3dmain.h"
#include "d3ddemo.h"
#include "d3dappi.h"

#include "AI.h"

#include "lines.h"
#include "magic.h"
#include "collision.h"
#include "title.h"

#include "XMem.h"


// externs

extern	LPDIRECTPLAY4A                       glpDP;     // directplay object pointer
extern	BOOL	IsHost;
void ReceiveGameMessages( void );
HRESULT ReceiveMessage(void);

extern	BOOL	RecordDemo;
extern	BOOL	PlayDemo;
extern	int16		NewLevelNum;
extern	TEXT	DemoGameName;
extern	char	biker_name[256];
extern	LONGLONG	GameStartedTime;
// structures

FILE	*	DemoFp = NULL;
FILE	*	DemoFpClean = NULL;

// globals

HANDLE			hPlayerEvent = NULL;					// player event to use
HANDLE			ghReceiveThread = NULL;			// handle of receive thread
DWORD			gidReceiveThread = 0;			// id of receive thread
HANDLE			ghKillReceiveEvent = NULL;		// event used to kill receive thread


BOOL	DplayRecieveThread = TRUE;


BOOL	Buffer1InUse = FALSE;
int		Buffer1Count = 0;
DWORD	Buffer1Offset = 0;
BYTE *	Buffer1Pnt = NULL;

BOOL	Buffer2InUse = FALSE;
int		Buffer2Count = 0;
DWORD	Buffer2Offset = 0;
BYTE *	Buffer2Pnt = NULL;


// prototypes
DWORD WINAPI	ReceiveThread(LPVOID lpThreadParameter);


HRESULT SetupConnection(HINSTANCE hInstance)
{
	HRESULT		hr;

	hPlayerEvent = NULL;					// player event to use
	ghReceiveThread = NULL;					// handle of receive thread
	gidReceiveThread = 0;					// id of receive thread
	ghKillReceiveEvent = NULL;				// event used to kill receive thread

	
	// Setup All the Buffer Stuff...
	Buffer1Count = 0;
	Buffer1Offset = 0;
	Buffer2Count = 0;
	Buffer2Offset = 0;


	if( DplayRecieveThread )
	{
		Buffer1Pnt = malloc( MAXBUFFERSIZE );
		Buffer2Pnt = malloc( MAXBUFFERSIZE );

		// create event used by DirectPlay to signal a message has arrived
		hPlayerEvent = CreateEvent(	NULL,		// no security
									FALSE,		// auto reset
									FALSE,		// initial event reset
									NULL);		// no name
		if ( hPlayerEvent == NULL )
		{
			hr = DPERR_NOMEMORY;
			goto FAILURE;
		}

		// create event used to signal that the receive thread should exit
		ghKillReceiveEvent = CreateEvent(NULL,		// no security
										 FALSE,		// auto reset
										 FALSE,		// initial event reset
										 NULL);		// no name
		if (ghKillReceiveEvent == NULL)
		{
			hr = DPERR_NOMEMORY;
			goto FAILURE;
		}

		// create thread to receive player messages
		ghReceiveThread = CreateThread(NULL,			// default security
									   0,				// default stack size
									   ReceiveThread,	// pointer to thread routine
									   NULL,			// argument for thread
									   0,				// start it right away
									   &gidReceiveThread);

		if (ghReceiveThread == NULL)
		{
			hr = DPERR_NOMEMORY;
			goto FAILURE;
		}

		
		/*
		removed by phil 11/11/98 ( VC6 warning )
		if FAILED(hr)
			goto FAILURE;
		*/
		
		//if (!(SetThreadPriority( ghReceiveThread, THREAD_PRIORITY_BELOW_NORMAL )) )
		//	DebugPrintf("Unable to set thread priority\n");


		
		return (DP_OK);
	}

	return DP_OK;
FAILURE:
	ShutdownConnection();

	return (hr);
}

HRESULT ShutdownConnection(void)
{
	StopDemoRecording();
	
	if (ghReceiveThread)
	{
		// wake up receive thread and wait for it to quit
		SetEvent(ghKillReceiveEvent);
		WaitForSingleObject(ghReceiveThread, INFINITE);

		CloseHandle(ghReceiveThread);
		ghReceiveThread = NULL;
	}

	if (ghKillReceiveEvent)
	{
		CloseHandle(ghKillReceiveEvent);
		ghKillReceiveEvent = NULL;
	}

	if (hPlayerEvent)
	{
		CloseHandle(hPlayerEvent);
		hPlayerEvent = NULL;
	}


	if( Buffer1Pnt )
	{
		free(Buffer1Pnt);
	}
	if( Buffer2Pnt )
	{
		free(Buffer2Pnt);
	}
	return (DP_OK);
}

DWORD WINAPI ReceiveThread(LPVOID lpThreadParameter)
{
	HANDLE		eventHandles[2];

	eventHandles[0] = hPlayerEvent;
	eventHandles[1] = ghKillReceiveEvent;

	// loop waiting for player events. If the kill event is signaled
	// the thread will exit
	while (WaitForMultipleObjects(2, eventHandles, FALSE, INFINITE) == WAIT_OBJECT_0)
	{
		// receive any messages in the queue
//		ReceiveGameMessages();
		ReceiveMessage();
	}

	ExitThread(0);

	return (0);
}




HRESULT ReceiveMessage(void)
{
	DPID				idFrom,idTo;
	DWORD				dwMsgBufferSize;
	HRESULT				hr;
	DWORD *				dwordpnt;
	LONGLONG *			longlongpnt;
	LONGLONG			TempTime;

	QueryPerformanceCounter((LARGE_INTEGER *) &TempTime);
	TempTime -= GameStartedTime;

	if( !Buffer1InUse )
	{
		Buffer1InUse = TRUE;
		// loop to read all messages in queue
		do
		{
			dwMsgBufferSize = MAXBUFFERSIZE-Buffer1Offset;
			if( dwMsgBufferSize > SMALLBUFFERSIZE )
				dwMsgBufferSize = SMALLBUFFERSIZE;
			// read messages from any player, including system player
			hr  = glpDP->lpVtbl->Receive( glpDP,
										  &idFrom,
										  &idTo,
										  DPRECEIVE_ALL,
										  (LPVOID) ( Buffer1Pnt+Buffer1Offset+(sizeof(DWORD)*2)+sizeof(LONGLONG) ),
										  &dwMsgBufferSize);

			
			if( hr == DP_OK )
			{
				longlongpnt = (LONGLONG*) (Buffer1Pnt + Buffer1Offset);
				*longlongpnt++ = TempTime;
				dwordpnt = (DWORD*) longlongpnt;
				*dwordpnt++ = dwMsgBufferSize;
				*dwordpnt++ = idFrom;

				Buffer1Offset += dwMsgBufferSize + (sizeof(DWORD)*2) +sizeof(LONGLONG);
				Buffer1Count++;
				if( Buffer1Offset >= MAXBUFFERSIZE )
				{
					OutputDebugString( "The Message Que has OverFlowed" );
				}
					
			}else{
				if( hr == DPERR_BUFFERTOOSMALL )
				{
					Buffer1Offset = 0;
					Buffer1Count = 0;
				}
			}
	
		} while( hr == DP_OK );

		Buffer1InUse = FALSE;
	}else{
		Buffer2InUse = TRUE;

		// loop to read all messages in queue
		do
		{
			dwMsgBufferSize = MAXBUFFERSIZE-Buffer2Offset;
			if( dwMsgBufferSize > SMALLBUFFERSIZE )
				dwMsgBufferSize = SMALLBUFFERSIZE;
			dwMsgBufferSize = SMALLBUFFERSIZE;
			// read messages from any player, including system player
			hr  = glpDP->lpVtbl->Receive( glpDP,
										  &idFrom,
										  &idTo,
										  DPRECEIVE_ALL,
										  (LPVOID) (Buffer2Pnt+Buffer2Offset+(sizeof(DWORD)*2)+sizeof(LONGLONG)),
										  &dwMsgBufferSize);

			if( hr == DP_OK )
			{
				longlongpnt = (LONGLONG*) (Buffer2Pnt + Buffer2Offset);
				*longlongpnt++ = TempTime;
				dwordpnt = (DWORD*) longlongpnt;
				*dwordpnt++ = dwMsgBufferSize;
				*dwordpnt++ = idFrom;

				Buffer2Offset += dwMsgBufferSize + (sizeof(DWORD)*2) + sizeof(LONGLONG);
				Buffer2Count++;
				if( Buffer2Offset >= MAXBUFFERSIZE )
				{
					OutputDebugString( "The Message Que has OverFlowed" );
				}
			}else{
				if( hr == DPERR_BUFFERTOOSMALL )
				{
					Buffer2Offset = 0;
					Buffer2Count = 0;
				}
			}
	
		} while( hr == DP_OK );
		Buffer2InUse = FALSE;

	}
	
	switch( hr )
	{
	case DPERR_NOMESSAGES:
		// this one is ok...
		break;
	case DPERR_BUFFERTOOSMALL:
		OutputDebugString( "The Message Que has OverFlowed" );
//		Msg( "The Message Que has OverFlowed" );
		break;
	default:
		OutputDebugString( "Dplay error" );
//		Msg( "Dplay error" );
		break;
	}
	return (DP_OK);
}




#if 0
HRESULT ReceiveMessage(void)
{
	DPID				idFrom, idTo;
	LPVOID				lpvMsgBuffer;
	DWORD				dwMsgBufferSize;
	HRESULT				hr;

	lpvMsgBuffer = NULL;
	dwMsgBufferSize = 0;

	// loop to read all messages in queue
	do
	{
		// loop until a single message is successfully read
		do
		{
			// read messages from any player, including system player
			idFrom = 0;
			idTo = 0;

			hr = lpDirectPlay4A->Receive(&idFrom, &idTo, DPRECEIVE_ALL,
										  lpvMsgBuffer, &dwMsgBufferSize);

			// not enough room, so resize buffer
			if (hr == DPERR_BUFFERTOOSMALL)
			{
				if (lpvMsgBuffer)
					GlobalFreePtr(lpvMsgBuffer);
				lpvMsgBuffer = GlobalAllocPtr(GHND, dwMsgBufferSize);
				if (lpvMsgBuffer == NULL)
					hr = DPERR_OUTOFMEMORY;
			}
		} while (hr == DPERR_BUFFERTOOSMALL);

		if ((SUCCEEDED(hr)) &&							// successfully read a message
			(dwMsgBufferSize >= sizeof(DPMSG_GENERIC)))	// and it is big enough
		{
			// check for system message
			if (idFrom == DPID_SYSMSG)
			{
				HandleSystemMessage( (LPDPMSG_GENERIC) lpvMsgBuffer,
									dwMsgBufferSize, idFrom, idTo);
			}
			else
			{
				HandleApplicationMessage( (LPDPMSG_GENERIC) lpvMsgBuffer,
										 dwMsgBufferSize, idFrom, idTo);
			}
		}
	} while (SUCCEEDED(hr));

	// free any memory we created
	if (lpvMsgBuffer)
		GlobalFreePtr(lpvMsgBuffer);

	return (DP_OK);
}

void HandleApplicationMessage( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize,
							   DPID idFrom, DPID idTo)
{


}

void HandleSystemMessage( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize,
						 DPID idFrom, DPID idTo)
{
    // The body of each case is there so you can set a breakpoint and examine
    // the contents of the message received.
	switch (lpMsg->dwType)
	{
	case DPSYS_CREATEPLAYERORGROUP:
        {
            LPDPMSG_CREATEPLAYERORGROUP		lp = (LPDPMSG_CREATEPLAYERORGROUP) lpMsg;
        }
		break;

	case DPSYS_DESTROYPLAYERORGROUP:
        {
            LPDPMSG_DESTROYPLAYERORGROUP	lp = (LPDPMSG_DESTROYPLAYERORGROUP)lpMsg;
        }
		break;

	case DPSYS_ADDPLAYERTOGROUP:
        {
            LPDPMSG_ADDPLAYERTOGROUP lp = (LPDPMSG_ADDPLAYERTOGROUP)lpMsg;
        }
		break;

	case DPSYS_DELETEPLAYERFROMGROUP:
        {
            LPDPMSG_DELETEPLAYERFROMGROUP lp = (LPDPMSG_DELETEPLAYERFROMGROUP)lpMsg;
        }
		break;

	case DPSYS_SESSIONLOST:
        {
            LPDPMSG_SESSIONLOST lp = (LPDPMSG_SESSIONLOST)lpMsg;
        }
		break;

	case DPSYS_HOST:
        {
            LPDPMSG_HOST	lp = (LPDPMSG_HOST)lpMsg;
        }
		break;

	case DPSYS_SETPLAYERORGROUPDATA:
        {
            LPDPMSG_SETPLAYERORGROUPDATA lp = (LPDPMSG_SETPLAYERORGROUPDATA)lpMsg;
        }
		break;

	case DPSYS_SETPLAYERORGROUPNAME:
        {
            LPDPMSG_SETPLAYERORGROUPNAME lp = (LPDPMSG_SETPLAYERORGROUPNAME)lpMsg;
        }
		break;
	}

}
#endif

