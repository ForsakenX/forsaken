/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Includes
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <dplay.h>
#include "typedefs.h"
#include "main.h"
#include "readini.h"
#include "XMem.h"

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	External Variables
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern	int		PreferredWidth, PreferredHeight;
extern	int		ddchosen3d;
extern	int		default_width;
extern	int		default_height;
extern	int		default_bpp;
extern	BOOL	PowerVR_Overide;
extern	BOOL	Is3Dfx;
extern	BOOL	Is3Dfx2;
extern	BOOL	TriLinear;
extern	int		TextureMemory;
extern	BOOL	NoTextureScaling;
extern	BOOL	MipMap;
extern	BOOL	TripleBuffer;
extern	BOOL	PolygonText;
extern	float	UV_Fix;
extern	BOOL	RecordDemoToRam;
extern	BOOL	CanDoStrechBlt;
extern	DWORD	UserTotalCompoundSfxBufferSize;
extern	BOOL	CustomCompoundBufferSize;
extern BOOL NoCompoundSfxBuffer;
extern	BOOL	UseSendAsync;
extern	BOOL	ZClearsOn;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Global Variables
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL	OptFileOnCommandline = FALSE;
BOOL	DeviceOnCommandline = FALSE;

BOOL	DontColourKey = FALSE;
int		ScreenMemory = 0;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Reader Routines
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
// Read DontColourKey
static int read_DontColourKey( FILE *f, char *last_token )
{
	DontColourKey = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read 3Dfx
static int read_3Dfx( FILE *f, char *last_token )
{
	Is3Dfx = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read 3Dfx2
static int read_3Dfx2( FILE *f, char *last_token )
{
	Is3Dfx2 = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read TriLinear
static int read_Trilinear( FILE *f, char *last_token )
{
	TriLinear = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read PowerVR
static int read_PowerVR( FILE *f, char *last_token )
{
	PowerVR_Overide = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read RecordDemoToRam
static int read_RecordDemoToRam( FILE *f, char *last_token )
{
	RecordDemoToRam = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read NoBlitTextScaling
static int read_NoBlitTextScaling( FILE *f, char *last_token )
{
	CanDoStrechBlt = FALSE;
	fscanf( f, " %80s", last_token );
	return 1;
}
static int read_NoCompoundSfxBuffer( FILE *f, char *last_token )
{
	NoCompoundSfxBuffer = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
static int read_CompoundSfxBufferMem( FILE *f, char *last_token )
{
	DWORD temp;

	CustomCompoundBufferSize = TRUE;

	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		UserTotalCompoundSfxBufferSize = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

// Read PolyText
static int read_PolyText( FILE *f, char *last_token )
{
	PolygonText = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read MipMap
static int read_MipMap( FILE *f, char *last_token )
{
	MipMap = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read SendAsync
static int read_SendAsync( FILE *f, char *last_token )
{
	UseSendAsync = TRUE;
	MipMap = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read MipMap
static int read_TripleBuffer( FILE *f, char *last_token )
{
	TripleBuffer = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read NoTextureScaling
static int read_NoTextureScaling( FILE *f, char *last_token )
{
	NoTextureScaling = TRUE;
	fscanf( f, " %80s", last_token );
	return 1;
}
// Read ZClear
static int read_ZClear( FILE *f, char *last_token )
{
	ZClearsOn = FALSE;
	fscanf( f, " %80s", last_token );
	return 1;
}


// Read Device
static int read_Device( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		ddchosen3d = temp;
		DeviceOnCommandline = TRUE;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

// Read Width
static int read_Width( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		default_width = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read Height
static int read_Height( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		default_height = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read Bpp
static int read_Bpp( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		default_bpp = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read Preferred Width
static int read_PreferredWidth( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		PreferredWidth = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read Height
static int read_PreferredHeight( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		PreferredHeight = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read TextureMemory
static int read_TextureMemory( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		TextureMemory = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read ScreenMemory
static int read_ScreenMemory( FILE *f, char *last_token )
{
	int temp;
	if ( fscanf( f, " %d", &temp ) == 1 )
	{
		ScreenMemory = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
// Read UV fix
static int read_UVFix( FILE *f, char *last_token )
{
	float temp;
	if ( fscanf( f, " %f", &temp ) == 1 )
	{
		UV_Fix = temp;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Read in the .ini file..
	Input		:	char * Filename
	Output		:	BOOL TRUE/FALSE
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
typedef int (*READINIJMP)( FILE *, char * );
typedef struct {
	char *keyword;
	READINIJMP handle;
} READINIFILE;

BOOL ReadIniFile( char *Filename )
{
	static READINIFILE jumptab[] = {
		{ "3Dfx",					read_3Dfx					},
		{ "3Dfx2",					read_3Dfx2					},
		{ "TriLinear",				read_Trilinear				},
		{ "PowerVR",				read_PowerVR				},
		{ "PolyText",				read_PolyText				},
		{ "MipMap",					read_MipMap					},
		{ "TripleBuffer",			read_TripleBuffer			},
		{ "NoTextureScaling",		read_NoTextureScaling		},
		{ "NoZClear",				read_ZClear					},
		{ "dev",					read_Device					},
		{ "w",						read_Width					},
		{ "h",						read_Height					},
		{ "bpp",					read_Bpp					},
		{ "pw",						read_PreferredWidth			},
		{ "ph",						read_PreferredHeight		},
		{ "TextureMemory",			read_TextureMemory			},
		{ "DontColourKey",			read_DontColourKey			},
		{ "ScreenMemory",			read_ScreenMemory			},
		{ "UVFix",					read_UVFix					},
		{ "RecordDemoToRam",		read_RecordDemoToRam		},
		{ "NoBlitTextScaling",		read_NoBlitTextScaling		},
		{ "NoCompoundSfxBuffer",	read_NoCompoundSfxBuffer 	},
		{ "CompoundSfxBufferMem",	read_CompoundSfxBufferMem	},
		{ "SendAsync",				read_SendAsync				},
		{ NULL,						NULL						}
	};
	FILE *f;
	char token[256];
	int j;

	f = fopen( Filename, "r" );
	if ( !f )
		return FALSE;

	OptFileOnCommandline = TRUE;

	if ( fscanf( f, " %80s", token ) == 1 )
	{
		do
		{
			for ( j = 0; jumptab[ j ].keyword; j++ )
			{
				if ( !_stricmp( token, jumptab[ j ].keyword ) )
					break;
			}
			if ( jumptab[ j ].handle )
			{
				(jumptab[ j ].handle)( f, token );
			}
			else
				fscanf( f, " %80s", token );
		} while ( !feof( f ) );
	}
	fclose( f );

	return TRUE;
}

