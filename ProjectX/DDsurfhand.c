/*==========================================================================
 *
 *  File: DDSurfhand.c
***************************************************************************/

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Include File...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <malloc.h>
#include "typedefs.h"
#include "d3ddemo.h"
#include "util.h"

#ifdef DEBUG_ON

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Defines...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	MAXDDSURFBLOCKS 256

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Globals...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

BOOL	DDSurfBlockUsed[MAXDDSURFBLOCKS];
LPDIRECTDRAWSURFACE	DDSurfBlockPnts[MAXDDSURFBLOCKS];
char *	DDSurfBlockInFile[MAXDDSURFBLOCKS];
size_t	DDSurfBlockSize[MAXDDSURFBLOCKS];
int		DDSurfBlockInLine[MAXDDSURFBLOCKS];
char *DDSurfBlockPrevInFile[MAXDDSURFBLOCKS];
int	DDSurfBlockPrevInLine[MAXDDSURFBLOCKS];

size_t	DDSurfMemUsed = 0;
int DDSurfBlocksUsed = 0;
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	X Exec Init...
	Input		:	void
	Output		:	void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DDSurf_Init( void )
{
	int i;
	DDSurfMemUsed =0;
	for( i = 0 ; i < MAXDDSURFBLOCKS ; i++ )
	{
		DDSurfBlockUsed[i] = FALSE;
		DDSurfBlockPnts[i] = NULL;
		DDSurfBlockSize[i] = 0;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	X Exec Find Free...
	Input		:	void
	Output		:	int Free memblock -1 if none
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int DDSurf_FindFree( void )
{
	int i;
	for( i = 0 ; i < MAXDDSURFBLOCKS ; i++ )
	{
		if( !DDSurfBlockUsed[i] )
		{
			DDSurfBlocksUsed++;
			return i;
		}
	}
	return -1;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	X Exec Find Same Block...
	Input		:	void * Pnt
	Output		:	int Same memblock -1 if none
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int DDSurf_FindSame( LPDIRECTDRAWSURFACE Pnt )
{
	int i;
	for( i = 0 ; i < MAXDDSURFBLOCKS ; i++ )
	{
		if( DDSurfBlockUsed[i] && ( DDSurfBlockPnts[i] == Pnt ) )
			return i;
	}
	return -1;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Make DD Surface
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

BOOL XMakeDDSurf(  IDirectDraw *pdd , LPDDSURFACEDESC lpDDSurfaceDesc , LPDIRECTDRAWSURFACE FAR *lplpDDSurface , IUnknown FAR *pUnkOuter, char *prev_in_file, int prev_in_line, char *in_file, int in_line )
{
	int i, bitdepth;
	DDSURFACEDESC CreatedSurfaceDesc;

	i = DDSurf_FindFree();
	if( i == -1 )
	{
		Msg( "Ran out of free DDSurf Blocks");
		return FALSE;
	}

	// do surf create here...
    if (pdd->lpVtbl->CreateSurface(pdd, lpDDSurfaceDesc, lplpDDSurface, NULL) != DD_OK)
		return FALSE;

	memset( &CreatedSurfaceDesc, 0, sizeof( DDSURFACEDESC ) );
	CreatedSurfaceDesc.dwSize = sizeof( DDSURFACEDESC );
	if  ( (*lplpDDSurface)->lpVtbl->GetSurfaceDesc( *lplpDDSurface, &CreatedSurfaceDesc ) != DD_OK )
	{
	 	DebugPrintf("file %s, line %d - unable to get surface description\n", __FILE__, __LINE__ );
		return FALSE;
	}

	bitdepth = 0;
	if ( CreatedSurfaceDesc.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED1 )
		bitdepth = 1;
	if ( CreatedSurfaceDesc.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED2 )
		bitdepth = 2;
	if ( CreatedSurfaceDesc.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4 )
		bitdepth = 4;
	if (( CreatedSurfaceDesc.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8 ) ||
		( CreatedSurfaceDesc.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXEDTO8 ))
		bitdepth = 8;
	if ( CreatedSurfaceDesc.ddpfPixelFormat.dwFlags & DDPF_RGB )
		bitdepth = CreatedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount;

	DDSurfBlockUsed[i] = TRUE;
	DDSurfBlockPnts[i] = *lplpDDSurface;
	DDSurfBlockSize[i] = lpDDSurfaceDesc->dwHeight * lpDDSurfaceDesc->dwWidth * bitdepth;
	DDSurfBlockInFile[i] = in_file;
	DDSurfBlockInLine[i] = in_line;
	DDSurfBlockPrevInFile[i] = prev_in_file;
	DDSurfBlockPrevInLine[i] = prev_in_line;
	DDSurfMemUsed += DDSurfBlockSize[i];
	return TRUE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	XRELEASE free some memory from an execute buffer...
	Input		:	void * Pnt
	Output		:	void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void XReleaseDDSurf( LPDIRECTDRAWSURFACE Pnt, char *in_file, int in_line )
{
	int i;
	static char *last_file = NULL;
	static int last_line = -1;

	if ( !Pnt )
	{
		if ( in_file != last_file || in_line != last_line )
			Msg( "Tried to free NULL DDSurf block in %s line %d", in_file, in_line );
		last_file = in_file;
		last_line = in_line;
		return;
	}
	i = DDSurf_FindSame( Pnt );
	if( i == -1 )
	{
		if ( in_file != last_file || in_line != last_line )
	 		Msg( "Tried to free un-allocated DDSurf block in %s line %d", in_file, in_line );
		last_file = in_file;
		last_line = in_line;
		return;
	}

	Pnt->lpVtbl->Release(Pnt);
	DDSurfBlockUsed[i] = FALSE;
	DDSurfBlockPnts[i] = NULL;
	DDSurfBlockInFile[ i ] = NULL;
	DDSurfBlockInLine[ i ] = 0;
	DDSurfBlockPrevInFile[ i ] = NULL;
	DDSurfBlockPrevInLine[ i ] = 0;


	DDSurfMemUsed -= DDSurfBlockSize[i];

	DDSurfBlockSize[i] = 0;
	DDSurfBlocksUsed--;
}

int UnMallocedDDSurfBlocks( void )
{
	int i;

	if ( DDSurfBlocksUsed )
	{
		for ( i = 0; i < MAXDDSURFBLOCKS; i++ )
		{
			if ( DDSurfBlockUsed[ i ] )
			{
				DebugPrintf( "DDSurfBlock size %d allocated in %s line %d ( called from %s line %d ) but not freed\n",
					DDSurfBlockSize[ i ], DDSurfBlockInFile[ i ], DDSurfBlockInLine[ i ],
					DDSurfBlockPrevInFile[ i ], DDSurfBlockPrevInLine[ i ] );
			}
		}
	}
	DebugPrintf( "DDSurfMemUsed = %d   DDSurfBlocksUsed = %d\n",
		DDSurfMemUsed, DDSurfBlocksUsed );
	return DDSurfBlocksUsed;
}


#else // DEBUG_ON
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Make Execute Buffer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

BOOL MakeDDSurf(  IDirectDraw *pdd , LPDDSURFACEDESC lpDDSurfaceDesc , LPDIRECTDRAWSURFACE FAR *lplpDDSurface , IUnknown FAR *pUnkOuter )
{
    if (pdd->lpVtbl->CreateSurface(pdd, lpDDSurfaceDesc, lplpDDSurface, NULL) != DD_OK)
        return FALSE;

	return TRUE;
}

#endif // DEBUG_ON
