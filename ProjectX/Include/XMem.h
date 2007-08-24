/*
 * The X Men, November 1997
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 4 $
 *
 * $Header: /PcProjectX/Include/XMem.h 4     18/03/98 16:14 Philipy $
 *
 * $Log: /PcProjectX/Include/XMem.h $
 * 
 * 4     18/03/98 16:14 Philipy
 * messed about with stopping memory from being paged out
 * Stopped due to lack of time & information
 * 
 * 3     28/01/98 12:57 Oliverc
 * Changed allocation checking to working on DEBUG_ON flag instead of the
 * generic _DEBUG
 * 
 * 2     11/29/97 4:35p Phillipd
 * 
 * 1     11/29/97 4:35p Phillipd
 * 
 */
#ifndef XMEM_INCLUDED
#define XMEM_INCLUDED

#include <ddraw.h>

/*
 * Defines
 */
/*
 * Structures
 */
/*
 * fn prototypes
 */

#if 0

void InitUsedMemList( void );
void AddToUsedMemList( char *start, DWORD size );
void FreeUsedMemList( void );
void MakeMemNonVolitile( void );
void MakeRegionPresent(volatile BYTE *pb, UINT cb);

#endif

#ifdef DEBUG_ON


#undef malloc
#undef calloc
#undef realloc
#undef free

#define malloc( S )		X_malloc( (S), __FILE__, __LINE__ )
#define calloc( N, S )	X_calloc( (N), (S), __FILE__, __LINE__ )
#define realloc( P, S )	X_realloc( (P), (S), __FILE__, __LINE__ )
#define free( P )		X_free( (P), __FILE__, __LINE__ )

void XMem_Init( void );
void * X_calloc( size_t num,size_t size, char *in_file, int in_line );
void * X_malloc( size_t size, char *in_file, int in_line );
void X_free( void * Pnt, char *in_file, int in_line );
void * X_realloc( void * Pnt , size_t size, char *in_file, int in_line );
int UnMallocedBlocks( void );


#endif


#endif	// XMEM_INCLUDED

