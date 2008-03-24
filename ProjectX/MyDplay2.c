

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


BOOL	DplayRecieveThread = FALSE;


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
		
/*
 * Change made by Methods
 * Tuesday March 18 07:42pm
 * Uncommented bellow lines
 * Attempt to find networking issue affecting entire game play
 */
		if (!(SetThreadPriority( ghReceiveThread, THREAD_PRIORITY_BELOW_NORMAL )) )
			DebugPrintf("Unable to set thread priority\n");


		
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

