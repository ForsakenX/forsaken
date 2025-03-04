#define XMEM_INCLUDED // we don't want xmem.h to include here
#include <stdio.h>
#include "main.h"
#include "util.h"

size_t	MemUsed = 0;

#define	MAXBLOCKS 16384

bool	BlockUsed[MAXBLOCKS];
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
		BlockUsed[i] = false;
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

void * X_strdup( char *str, char *in_file, int in_line )
{
	void * Pnt;
	int i;
	
	i = XMem_FindFree();
	if( i == -1 )
	{
		DebugPrintf( "MEM: Ran out of free memory Blocks\n"); // break point
		return NULL;
	}

	Pnt = strdup( str );
	
	if( !Pnt )
		return Pnt;

	int size = strlen(str)+1;

	BlockUsed[i] = true;
	BlockPnts[i] = Pnt;
	BlockSize[i] = size;
	BlockInFile[i] = in_file;
	BlockInLine[i] = in_line;
	MemUsed += size;

	return Pnt;

}
void * X_malloc( size_t size, char *in_file, int in_line )
{
	void * Pnt;
	int i;
	
	i = XMem_FindFree();
	if( i == -1 )
	{
		DebugPrintf( "MEM: Ran out of free memory Blocks\n"); // break point
		return NULL;
	}

	Pnt = malloc( size );
	
	if( !Pnt )
		return Pnt;

	BlockUsed[i] = true;
	BlockPnts[i] = Pnt;
	BlockSize[i] = size;
	BlockInFile[i] = in_file;
	BlockInLine[i] = in_line;
	MemUsed += size;

	memset(Pnt,0,sizeof(Pnt)); // this protects whole program against dirty memory

	return Pnt;

}

void * X_calloc( size_t num,size_t size, char *in_file, int in_line )
{
	void * Pnt;
	int i;

	i = XMem_FindFree();
	if( i == -1 )
	{
		DebugPrintf( "MEM: Ran out of free memory Blocks\n"); // break point
		return NULL;
	}

	Pnt = calloc( num , size );

	if( !Pnt )
		return Pnt;

	BlockUsed[i] = true;
	BlockPnts[i] = Pnt;
	BlockSize[i] = num * size;
	BlockInFile[i] = in_file;
	BlockInLine[i] = in_line;
	MemUsed += num * size;
	
	memset(Pnt,0,sizeof(Pnt)); // this protects whole program against dirty memory

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
			DebugPrintf( "MEM: Tried to free NULL block in %s line %d\n", in_file, in_line ); // break point
		last_file = in_file;
		last_line = in_line;
		return;
	}
	i = XMem_FindSame( Pnt );
	if( i == -1 )
	{
		if ( in_file != last_file || in_line != last_line )
	 		DebugPrintf( "MEM: Tried to free un-malloced block in %s line %d\n", in_file, in_line ); // break point
		last_file = in_file;
		last_line = in_line;
		return;
	}
	free(Pnt);
	BlockUsed[i] = false;
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
		DebugPrintf( "MEM: tried to realloc un-alloced block\n"); // break point
		return NULL;
	}

	Pnt = realloc( Pnt , size );

	if( !Pnt )
		return Pnt;

	BlockUsed[i] = true;
	BlockPnts[i] = Pnt;
	BlockInFile[i] = in_file;
	BlockInLine[i] = in_line;

	if(size > BlockSize[i]) // this protects whole program against dirty memory
		memset(Pnt+BlockSize[i],0,size-BlockSize[i]);

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
				DebugPrintf( "MEM: Block size %d allocated in %s line %d but not freed\n",
					BlockSize[ i ], BlockInFile[ i ], BlockInLine[ i ] ); // break point
			}
		}
	}
	DebugPrintf( "MEM: MemUsed = %d   BlocksUsed = %d\n",
		MemUsed, BlocksUsed ); // break point
	return BlocksUsed;
}
