

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Include Files...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <malloc.h>
#include <ddraw.h>
#include "util.h"
size_t	MemUsed =0;
#ifdef DEBUG_ON

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Defines...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	MAXBLOCKS 16384

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Globals...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/


BOOL	BlockUsed[MAXBLOCKS];
void *	BlockPnts[MAXBLOCKS];
size_t	BlockSize[MAXBLOCKS];
char *	BlockInFile[MAXBLOCKS];
int		BlockInLine[MAXBLOCKS];
int BlocksUsed = 0;

CRITICAL_SECTION XMemKey;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	X Mem Init...
	Input		:	void
	Output		:	void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void XMem_Init( void )
{
	int i;

	MemUsed =0;
	for( i = 0 ; i < MAXBLOCKS ; i++ )
	{
		BlockUsed[i] = FALSE;
		BlockPnts[i] = NULL;
		BlockSize[i] = 0;
	}

	InitializeCriticalSection( &XMemKey );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	X Mem Find Free...
	Input		:	void
	Output		:	int Free memblock -1 if none
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int XMem_FindFree( void )
{
	int i;

	for( i = 0 ; i < MAXBLOCKS ; i++ )
	{
		if( !BlockUsed[i] )
		{
			BlocksUsed++;
			return i;
		}
	}

	return -1;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	X Mem Find Same Block...
	Input		:	void * Pnt
	Output		:	int Same memblock -1 if none
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int XMem_FindSame( void * Pnt )
{
	int i;
	for( i = 0 ; i < MAXBLOCKS ; i++ )
	{
		if( BlockUsed[i] && ( BlockPnts[i] == Pnt ) )
			return i;
	}
	return -1;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	X Mem malloc some memory....
	Input		:	size_t size
	Output		:	void * pnt to block of memory or NULL if none
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void * X_malloc( size_t size, char *in_file, int in_line )
{
	void * Pnt;
	int i;

	EnterCriticalSection( &XMemKey );
	
	i = XMem_FindFree();
	if( i == -1 )
	{
		Msg( "Ran out of free memory Blocks");
		LeaveCriticalSection( &XMemKey );
		return NULL;
	}

	Pnt = malloc( size );
	
	if( !Pnt )
	{
		LeaveCriticalSection( &XMemKey );
		return Pnt;
	}

	BlockUsed[i] = TRUE;
	BlockPnts[i] = Pnt;
	BlockSize[i] = size;
	BlockInFile[i] = in_file;
	BlockInLine[i] = in_line;
	MemUsed += size;

	LeaveCriticalSection( &XMemKey );

	return Pnt;

}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	X Mem calloc some memory....
	Input		:	size_t size
	Output		:	void * pnt to block of memory or NULL if none
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void * X_calloc( size_t num,size_t size, char *in_file, int in_line )
{
	void * Pnt;
	int i;

	EnterCriticalSection( &XMemKey );

	i = XMem_FindFree();
	if( i == -1 )
	{
		LeaveCriticalSection( &XMemKey );
		Msg( "Ran out of free memory Blocks");
		return NULL;
	}

	Pnt = calloc( num , size );

	if( !Pnt )
	{
		LeaveCriticalSection( &XMemKey );
		return Pnt;
	}

	BlockUsed[i] = TRUE;
	BlockPnts[i] = Pnt;
	BlockSize[i] = num * size;
	BlockInFile[i] = in_file;
	BlockInLine[i] = in_line;
	MemUsed += num * size;

	LeaveCriticalSection( &XMemKey );
	
	return Pnt;

}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	X Mem free some memory..
	Input		:	void * Pnt
	Output		:	void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void X_free( void * Pnt, char *in_file, int in_line )
{
	int i;
	static char *last_file = NULL;
	static int last_line = -1;

	EnterCriticalSection( &XMemKey );
	
	if ( !Pnt )
	{
		if ( in_file != last_file || in_line != last_line )
			Msg( "Tried to free NULL block in %s line %d", in_file, in_line );
		last_file = in_file;
		last_line = in_line;
		LeaveCriticalSection( &XMemKey );
		return;
	}
	i = XMem_FindSame( Pnt );
	if( i == -1 )
	{
		if ( in_file != last_file || in_line != last_line )
	 		Msg( "Tried to free un-malloced block in %s line %d", in_file, in_line );
		last_file = in_file;
		last_line = in_line;
		LeaveCriticalSection( &XMemKey );
		return;
	}
	free(Pnt);
	BlockUsed[i] = FALSE;
	BlockPnts[i] = NULL;
	BlockInFile[ i ] = NULL;
	BlockInLine[ i ] = 0;

	MemUsed -= BlockSize[i];

	BlockSize[i] = 0;
	BlocksUsed--;

	LeaveCriticalSection( &XMemKey );
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	X Mem malloc some memory....
	Input		:	size_t size
	Output		:	void * pnt to block of memory or NULL if none
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void * X_realloc( void * Pnt , size_t size, char *in_file, int in_line )
{
	int i;

	EnterCriticalSection( &XMemKey );
	
	i = XMem_FindSame( Pnt );
	if( i == -1 )
	{
		LeaveCriticalSection( &XMemKey );
		Msg( "tried to realloc un-alloced block");
		return NULL;
	}

	Pnt = realloc( Pnt , size );

	if( !Pnt )
	{
		LeaveCriticalSection( &XMemKey );
		return Pnt;
	}

	BlockUsed[i] = TRUE;
	BlockPnts[i] = Pnt;
	BlockInFile[i] = in_file;
	BlockInLine[i] = in_line;
	MemUsed -= BlockSize[i];
	BlockSize[i] = size;
	MemUsed += size;

	LeaveCriticalSection( &XMemKey );
	
	return Pnt;

}





int UnMallocedBlocks( void )
{
	int i;

	if ( BlocksUsed )
	{
		for ( i = 0; i < MAXBLOCKS; i++ )
		{
			if ( BlockUsed[ i ] )
			{
				DebugPrintf( "Block size %d allocated in %s line %d but not freed\n",
					BlockSize[ i ], BlockInFile[ i ], BlockInLine[ i ] );
			}
		}
	}
	DebugPrintf( "MemUsed = %d   BlocksUsed = %d\n",
		MemUsed, BlocksUsed );
	return BlocksUsed;
}


#if 0

typedef struct _USEDMEMLIST
{
	char *start;
	DWORD size;
} USEDMEMLIST;

#define MAX_ANY_MEMBLOCKS 1024

USEDMEMLIST UsedMem[ MAX_ANY_MEMBLOCKS ];
DWORD NumUsedMemBlocks;

void InitUsedMemList( void )
{
	NumUsedMemBlocks = 0;
	memset( UsedMem, 0, sizeof( USEDMEMLIST ) * MAX_ANY_MEMBLOCKS );
}

void AddToUsedMemList( char *start, DWORD size )
{
	UsedMem[ NumUsedMemBlocks ].start = start;
	UsedMem[ NumUsedMemBlocks ].size = size;
	NumUsedMemBlocks++;
}

void FreeUsedMemList( void )
{
	NumUsedMemBlocks = 0;
}

#define NUM_LIMBO_PAGES 16

static int
_cdecl CompareMemBlocks( const void *arg1, const void *arg2 )
{
	USEDMEMLIST *item1, *item2;

	item1 = (USEDMEMLIST *)arg1;
	item2 = (USEDMEMLIST *)arg2;

	if ( item1->start < item2->start )
		return -1;

	if ( item1->start > item2->start )
		return 1;

	return 0;
}


void MakeMemNonVolitile( void )
{
	char *PreviousPtr[ NUM_LIMBO_PAGES ];
	char *InitialTouchPtr, *LastTouched;
	char temp;
	SYSTEM_INFO SystemInfo;
	DWORD PageSize;
	DWORD i, j;
	
	if ( !NumUsedMemBlocks )
		return;

	// sort memory blocks...
	qsort( (void *)UsedMem, (size_t) NumUsedMemBlocks, sizeof( USEDMEMLIST ), CompareMemBlocks );

	GetSystemInfo( &SystemInfo ); 
	PageSize = SystemInfo.dwPageSize;

	j = 0;
	LastTouched = NULL;
	for ( i = 0; i < NumUsedMemBlocks; i++ )
	{
		InitialTouchPtr = UsedMem[ i ].start;

	   	while ( InitialTouchPtr <= ( UsedMem[ i ].start + UsedMem[ i ].size ) )
		{
			// if new block starts on same page as last page of last block, ignore
			if ( ( InitialTouchPtr - PageSize ) < LastTouched )
			{
				InitialTouchPtr += PageSize;
				continue;
			}
			
			PreviousPtr[ j++ ] = InitialTouchPtr;
			LastTouched = InitialTouchPtr;
			if ( j == NUM_LIMBO_PAGES )
			{
				temp = *PreviousPtr[ 0 ];
				memmove( &PreviousPtr[ 0 ], &PreviousPtr[ 1 ], sizeof( char *) * ( NUM_LIMBO_PAGES - 1 ) );
				j--;
			}
			temp = *InitialTouchPtr;

			InitialTouchPtr += PageSize;
		}
	}

	for ( i = 0; i < j; i++ )
	{
		// touch very first page again ( a page must be accessed in between each refresh touch )
		temp = *UsedMem[ 0 ].start;

		// refresh remaining pages
		temp = *PreviousPtr[ i ];
	}
}
/*
  Assume that pb and cb are both rounded to page boundaries
  and that PageSize has been determined from GetSystemInformation().
*/
void MakeRegionPresent(volatile BYTE *pb, UINT unalignedcb)
{
	UINT ib, cbLag, cb;
	SYSTEM_INFO SystemInfo;


	GetSystemInfo( &SystemInfo ); 
	cbLag = 16 * SystemInfo.dwPageSize;

	cb = unalignedcb + ( SystemInfo.dwPageSize - 1 );
	cb /= SystemInfo.dwPageSize;
	cb *= SystemInfo.dwPageSize;

  for (ib = 0; ib < cb; ib += SystemInfo.dwPageSize )
  {
	if ( ib >= cbLag )
	{
		pb[ib - cbLag];
	}
    pb[ib];
  }
}
#endif

#endif // DEBUG_ON