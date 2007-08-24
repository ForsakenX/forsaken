/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 5 $
 *
 * $Header: /PcProjectX/Include/Exechand.h 5     28/01/98 12:57 Oliverc $
 *
 * $Log: /PcProjectX/Include/Exechand.h $
 * 
 * 5     28/01/98 12:57 Oliverc
 * Changed allocation checking to working on DEBUG_ON flag instead of the
 * generic _DEBUG
 * 
 * 4     12/01/97 3:27p Phillipd
 * 
 * 3     1/10/97 4:25p Phillipd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*==========================================================================
 *
 *
 *  File:       exechand.h
 *  Content:    Execute buffer handle include file
 *
 *
 ***************************************************************************/

#ifndef EXECHAND_INCLUDED
#define EXECHAND_INCLUDED

#ifdef DEBUG_ON

#define MakeExecuteBuffer( A, B, C, D )	XMakeExecuteBuffer( (A), (B), (C), (D), __FILE__, __LINE__ )
#define XRELEASE( A )	EXECRELEASE( (A), __FILE__, __LINE__ )
/*
 * fn prototypes
 */
void XExec_Init( void );
BOOL XMakeExecuteBuffer(  LPD3DEXECUTEBUFFERDESC	debDesc , LPDIRECT3DDEVICE lpDev , LPDIRECT3DEXECUTEBUFFER  * lpBuf , size_t size , char *in_file, int in_line );
void EXECRELEASE( LPDIRECT3DEXECUTEBUFFER Pnt, char *in_file, int in_line );
int UnMallocedExecBlocks( void );

#else // DEBUG_ON

BOOL MakeExecuteBuffer(  LPD3DEXECUTEBUFFERDESC	debDesc , LPDIRECT3DDEVICE lpDev , LPDIRECT3DEXECUTEBUFFER  * lpBuf , size_t size );
#define XRELEASE(x) x->lpVtbl->Release(x)

#endif // DEBUG_ON

#endif

