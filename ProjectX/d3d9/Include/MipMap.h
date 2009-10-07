
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
// bjd - CHECK BOOL PackAndCopy(void *dest, uint8 * uint8pnt, LPDDSURFACEDESC lpFormat, int pitch , DWORD dwWidth , DWORD dwHeight );
/*Load and create a mipmapped texture surface*/
//bjd LPDIRECTDRAWSURFACE LoadMipMap(LPDIRECTDRAW lpDD, LPCSTR lpName, LPDDSURFACEDESC lpFormat, DWORD memoryflag , int16 Scale );
#endif // MIPMAP_INCLUDED


