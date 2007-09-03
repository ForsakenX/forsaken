/*
 * The X Men, November 1997
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 2 $
 *
 * $Header: /PcProjectX/Include/MipMap.h 2     11/10/97 4:08p Phillipd $
 *
 * $Log: /PcProjectX/Include/MipMap.h $
 * 
 * 2     11/10/97 4:08p Phillipd
 * 
 * 1     11/10/97 4:08p Phillipd
 * 
 * 
 */

#ifndef MIPMAP_INCLUDED
#define MIPMAP_INCLUDED

/*
 * Defines
 */
#define	MAXMIPMAP 8
#define	MIPMAPMIN 16

#define ColorOf(data, x, y, c , width) ((data)[((y)*(width)+(x))*3+c])

/*
 * structures
 */
typedef struct _MIPMAPDESC
{
	DWORD	Width;
	DWORD	Height;
	int		LOD;
	uint8 * Data[MAXMIPMAP];
} MIPMAPDESC;

/*
 * fn prototypes
 */
int BuildMipMaps( int Width , int Height , MIPMAPDESC * MipMapDesc , int Count );
BOOL InitMipMap(LPCSTR lpName , int16 Scale , MIPMAPDESC * MipMapDesc );
BOOL PackAndCopy(void *dest, uint8 * uint8pnt, LPDDSURFACEDESC lpFormat, int pitch , DWORD dwWidth , DWORD dwHeight );
/*Load and create a mipmapped texture surface*/
LPDIRECTDRAWSURFACE LoadMipMap(LPDIRECTDRAW lpDD, LPCSTR lpName, 
                   LPDDSURFACEDESC lpFormat, DWORD memoryflag , int16 Scale );
#endif // MIPMAP_INCLUDED


