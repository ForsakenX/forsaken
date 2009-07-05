
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

