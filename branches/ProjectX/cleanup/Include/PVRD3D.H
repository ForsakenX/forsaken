/**************************************************************************
 PVRD3D.H
**************************************************************************/

#ifndef _PVRD3D_

#define _PVRD3D_
#define BUILDFOURCC(a,b,c,d)	((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

#define FOURCC_PVR_CTL			BUILDFOURCC ('P', 'V', 'R', 'C')

/**************************************************************************
 PVR feature control and shadow/light volume structure.

 To use create texture surface with FOURCC = FOURCC_PVR_CTL,
 if the create fails then you're not running on PVR.
 Lock surface - returns ptr to structure below
 Set dwType field as required (i.e PVR_FCTL, PVR_SHADOWVOL or PVR_LIGHTVOL
 Unlock surface.
 GetHandle for "Texture" to associate with D3D context.
 If type specifies shadow or light volumes, handle is used in execute 
 buffer to indicate following polygons define a convex volume of type as 
 per dwType.
 To set shadow brightness, light volume colour or enable/disable features
 lock surface, setup appropriate union member and unlock surface.
**************************************************************************/
typedef struct _PVR_CTL_
{
	DWORD	dwType;
	PVOID	pvReserved;
	union
	{
		DWORD		dwFeatureCTL;			/* if dwType = PVR_FCTL, Specifies feature CTL flags 	  */
		float		fShadowBrightness;		/* if dwType = PVR_SHADOWVOL, Specifies shadow brightness */
		D3DCOLOR	dcLightVolumeColour;	/* if dwType = PVR_LIGHTVOL, Specifies light vol colour   */
	};
	DWORD	dwBGColour;						/* See feature control flags below  */
	DWORD	dwReserved[8];
} PVR_CTL, *PPVR_CTL;

/**************************************************************************
 PVR control types.
**************************************************************************/
#define PVR_FCTL		0
#define PVR_SHADOWVOL	1
#define PVR_LIGHTVOL	2 

/**************************************************************************
 PVR feature control flags.
**************************************************************************/
/* 
	Enable quads, use D3D strips/fans to specify 
*/
#define PVR_FCTL_ENABLEQUADS		0x00000001
	
/* 
	Enable fast translucency, indicates translucent data will already be 
	grouped for optimal performance/correctness.
*/
#define PVR_FCTL_ENABLEFASTTRANS	0x00000002	

/*
	Enable texture alpha channel modulation, 4444 textures loaded when set
	will have their alpha channel set to a value that is proportional to
	each texels intensity. ** NOT IMPLEMENTED YET **
*/
#define PVR_FCTL_ENABLEIBASEDALPHA	0x00000004

/*
	Enable rendering of empty regions i.e render buffer background will 
	be cleared to the value in dwBGColour (32 bit XRGB specifier). Saves 
	the use of an explicit blit or polygons to clear background.
*/ 
#define PVR_FCTL_ENABLEBGCLEAR		0x00000008

/*
	Disable driver deferal of 2D operations.
*/ 
#define PVR_FCTL_DISABLEDEFERRED2D	0x00000010

#endif
/**************************************************************************
 End of file (PVRD3D.H)
**************************************************************************/
