#include <stdio.h>
#include <malloc.h>
#include <strings.h>
#include "main.h"
#include "util.h"

size_t	MemUsed = 0;

#define	MAXBLOCKS 16384

BOOL	BlockUsed[MAXBLOCKS];
void *	BlockPnts[MAXBLOCKS];
size_t	BlockSize[MAXBLOCKS];
char *	BlockInFile[MAXBLOCKS];
int		BlockInLine[MAXBLOCKS];
int BlocksUsed = 0;

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
}

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

void * X_malloc( size_t size, char *in_file, int in_line )
{
	void * Pnt;
	int i;
	
	i = XMem_FindFree();
	if( i == -1 )
	{
		DebugPrintf( "Ran out of free memory Blocks"); // break point
		return NULL;
	}

	Pnt = malloc( size );
	
	if( !Pnt )
		return Pnt;

	BlockUsed[i] = TRUE;
	BlockPnts[i] = Pnt;
	BlockSize[i] = size;
	BlockInFile[i] = in_file;
	BlockInLine[i] = in_line;
	MemUsed += size;

	return Pnt;

}

void * X_calloc( size_t num,size_t size, char *in_file, int in_line )
{
	void * Pnt;
	int i;

	i = XMem_FindFree();
	if( i == -1 )
	{
		DebugPrintf( "Ran out of free memory Blocks"); // break point
		return NULL;
	}

	Pnt = calloc( num , size );

	if( !Pnt )
		return Pnt;

	BlockUsed[i] = TRUE;
	BlockPnts[i] = Pnt;
	BlockSize[i] = num * size;
	BlockInFile[i] = in_file;
	BlockInLine[i] = in_line;
	MemUsed += num * size;
	
	return Pnt;

}

void X_free( void * Pnt, char *in_file, int in_line )
{
	int i;
	static char *last_file = NULL;
	static int last_line = -1;
	
	if ( !Pnt )
	{
		if ( in_file != last_file || in_line != last_line )
			DebugPrintf( "Tried to free NULL block in %s line %d", in_file, in_line ); // break point
		last_file = in_file;
		last_line = in_line;
		return;
	}
	i = XMem_FindSame( Pnt );
	if( i == -1 )
	{
		if ( in_file != last_file || in_line != last_line )
	 		DebugPrintf( "Tried to free un-malloced block in %s line %d", in_file, in_line ); // break point
		last_file = in_file;
		last_line = in_line;
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
}

void * X_realloc( void * Pnt , size_t size, char *in_file, int in_line )
{
	int i;
	
	i = XMem_FindSame( Pnt );
	if( i == -1 )
	{
		DebugPrintf( "tried to realloc un-alloced block"); // break point
		return NULL;
	}

	Pnt = realloc( Pnt , size );

	if( !Pnt )
		return Pnt;

	BlockUsed[i] = TRUE;
	BlockPnts[i] = Pnt;
	BlockInFile[i] = in_file;
	BlockInLine[i] = in_line;
	MemUsed -= BlockSize[i];
	BlockSize[i] = size;
	MemUsed += size;
	
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
					BlockSize[ i ], BlockInFile[ i ], BlockInLine[ i ] ); // break point
			}
		}
	}
	DebugPrintf( "MemUsed = %d   BlocksUsed = %d\n",
		MemUsed, BlocksUsed ); // break point
	return BlocksUsed;
}
