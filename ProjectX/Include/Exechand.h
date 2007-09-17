
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

