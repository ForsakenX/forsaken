/*===================================================================
*	2 d p o l y s . c
*	All routines to do with 2d always face you polygons...
===================================================================*/
#include "main.h"
#include <stdio.h>
#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "2dpolys.h"
#include "camera.h"
#include "primary.h"
#include "spotfx.h"
#include "water.h"
#include "util.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

#define	FMPOLYSCALE	32.0F

/*===================================================================
	Externs
===================================================================*/
extern	CAMERA	CurrentCamera;
extern	RENDERMATRIX			view;
extern	MATRIX				ProjMatrix;
extern	VECTOR				Camera_Dir;
extern	VECTOR				Forward;
extern	VECTOR				SlideUp;
extern	VECTOR				SlideLeft;
extern	BYTE				WhoIAm;
extern	TLOADHEADER			Tloadheader;
extern	FRAME_INFO	*		Troj_Exp_Header;
extern	FRAME_INFO	*		Shrapnel_Header;
extern	float				framelag;
extern	MATRIX				MATRIX_Identity;
extern	DWORD				CurrentSrcBlend;
extern	DWORD				CurrentDestBlend;
extern	DWORD				CurrentTextureBlend;
extern	MLOADHEADER			Mloadheader;
extern	MCLOADHEADER		MCloadheader;
extern	MCLOADHEADER		MCloadheadert0;

/*===================================================================
	Globals
===================================================================*/
FMPOLY		FmPolys[ MAXNUMOF2DPOLYS ];
u_int16_t		FirstFmPolyUsed;
u_int16_t		FirstFmPolyFree;
u_int32_t		TotalFmPolysInUse = 0;
TPAGEINFO	FmPolyTPages[ MAXTPAGESPERTLOAD + 1 ];

void FadeColour( u_int8_t * Colour, u_int8_t WantedColour, float Speed );

#define RGBA_MAKE2(r, g, b, a)   ((COLOR) (( (DWORD) ((a) & 0xff) << 24) | ( (DWORD) ((r) & 0xff) << 16) | ( (DWORD) ((g) & 0xff) << 8) | (DWORD) ((b) & 0xff)))

/*===================================================================
	Procedure	:	Init Faceme poly structures
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitFmPoly( void )
{
	int i;

	FirstFmPolyUsed = (u_int16_t) -1;
	FirstFmPolyFree = 0;

	for( i=0;i<MAXNUMOF2DPOLYS;i++)
	{
		memset( &FmPolys[i], 0, sizeof( FMPOLY ) );

		FmPolys[i].xsize = ( 16.0F * GLOBAL_SCALE );
		FmPolys[i].ysize = ( 16.0F * GLOBAL_SCALE );
		FmPolys[i].R = 255;
		FmPolys[i].G = 255;
		FmPolys[i].B = 255;
#if ACTUAL_TRANS
		FmPolys[i].Trans = 128;
#else
		FmPolys[i].Trans = 255;
#endif
		FmPolys[i].DirVector.x = 0.0F;
		FmPolys[i].DirVector.y = 0.0F;
		FmPolys[i].DirVector.z = 1.0F;
		FmPolys[i].UpVector.x = 0.0F;
		FmPolys[i].UpVector.y = 1.0F;
		FmPolys[i].UpVector.z = 0.0F;

		FmPolys[i].NextInTPage = (u_int16_t) -1;
		FmPolys[i].PrevInTPage = (u_int16_t) -1;

		FmPolys[i].Next = i + 1;
		FmPolys[i].Prev = (u_int16_t) -1;
	}
	FmPolys[MAXNUMOF2DPOLYS-1].Next = (u_int16_t) -1;

	InitFmPolyTPages();
}

/*===================================================================
	Procedure	:	Find a free FmPoly and move it from the free list to
					the used list
	Input		:	Nothing
	Output		:	u_int16_t	Number of the free FmPoly
===================================================================*/
u_int16_t FindFreeFmPoly( void )
{
	u_int16_t i;

	i = FirstFmPolyFree;
	if( i == (u_int16_t) -1 ) return i;

	FmPolys[i].Prev = FirstFmPolyUsed;

	if ( FirstFmPolyUsed != (u_int16_t) -1)
	{
		FmPolys[FirstFmPolyUsed].Next = i;
	}

	FirstFmPolyUsed = i;
	FirstFmPolyFree = FmPolys[i].Next;

	TotalFmPolysInUse++;

	return i ;
}
/*===================================================================
	Procedure	:	Kill a used FmPoly and move it from the used list to
					the free list
	Input		:	u_int16_t		Number of FmPoly to free....
	Output		:	Nothing
===================================================================*/
void KillUsedFmPoly( u_int16_t i )
{
	u_int16_t	its_prev;
	u_int16_t	its_next;

	its_prev = FmPolys[i].Prev;
	its_next = FmPolys[i].Next;

	if ( i == FirstFmPolyUsed )
		FirstFmPolyUsed = FmPolys[i].Prev;

	if( its_prev != (u_int16_t) -1)
		FmPolys[its_prev].Next = its_next;

	if( its_next != (u_int16_t) -1)
		FmPolys[its_next].Prev = its_prev;

	TotalFmPolysInUse--;

	RemoveFmPolyFromTPage( i, GetTPage( *FmPolys[i].Frm_Info, 0 ) );

	FmPolys[i].Prev = (u_int16_t) -1;
	FmPolys[i].Next = FirstFmPolyFree;
	FmPolys[i].LifeCount = 0.0F;
	FmPolys[i].xsize = ( 16.0F * GLOBAL_SCALE );
	FmPolys[i].ysize = ( 16.0F * GLOBAL_SCALE );
	FmPolys[i].Rot = 0.0F;
	FmPolys[i].RotSpeed = 0.0F;
	FmPolys[i].R = 255;
	FmPolys[i].G = 255;
	FmPolys[i].B = 255;
	FmPolys[i].Trans = 255;
	FmPolys[i].Flags = 0;
	FmPolys[i].Frame = 0.0F;
	FmPolys[i].SeqNum = 0;
	FmPolys[i].Dir.x = 0.0F;
	FmPolys[i].Dir.y = 0.0F;
	FmPolys[i].Dir.z = 0.0F;
   	FmPolys[i].DirVector.x = 0.0F;
   	FmPolys[i].DirVector.y = 0.0F;
   	FmPolys[i].DirVector.z = 1.0F;
   	FmPolys[i].UpVector.x = 0.0F;
   	FmPolys[i].UpVector.y = 1.0F;
   	FmPolys[i].UpVector.z = 0.0F;
	FmPolys[i].Speed = 0.0F;
	FmPolys[i].Frm_Info = NULL;
	FirstFmPolyFree	= i;
}

/*===================================================================
	Procedure	:	Process FmPoly's
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
#if 0

#define	NMEGEN_START		( 5.0F * ANIM_SECOND )
#define	NMEGEN_STAGE1		( 2.0F * ANIM_SECOND )
#define	NMEGEN_STAGE2		( 1.0F * ANIM_SECOND )
#define	NMEGEN_END			( 0.0F * ANIM_SECOND )
#define	NMEGEN_STAGE0_LEN	( NMEGEN_START - NMEGEN_STAGE1 )
#define	NMEGEN_STAGE1_LEN	( NMEGEN_STAGE1 - NMEGEN_STAGE2 )
#define	NMEGEN_STAGE2_LEN	( NMEGEN_STAGE2 - NMEGEN_END )

#else

#define	NMEGEN_START		( 2.0F * ANIM_SECOND )
#define	NMEGEN_STAGE1		( 0.8F * ANIM_SECOND )
#define	NMEGEN_STAGE2		( 0.4F * ANIM_SECOND )
#define	NMEGEN_END			( 0.0F * ANIM_SECOND )
#define	NMEGEN_STAGE0_LEN	( NMEGEN_START - NMEGEN_STAGE1 )
#define	NMEGEN_STAGE1_LEN	( NMEGEN_STAGE1 - NMEGEN_STAGE2 )
#define	NMEGEN_STAGE2_LEN	( NMEGEN_STAGE2 - NMEGEN_END )

#endif

static	int16_t	GlobCount;
static	u_int8_t	ShadeTable[ 16 ] = { 255, 200, 128, 104, 96, 92, 80, 77, 74, 72, 69, 66, 64, 64, 64, 64 };
static	u_int8_t	SmokeRingShadeTable[ 5 ] = { 255, 200, 150, 100, 50 };

void FmPolyProcess( void )
{
	u_int16_t	i;
	u_int16_t	nextprim;
	float	Speed;
	float	R, G, B, T, FadeDec;
	VECTOR	Rotation;
	float	OldSize;
	float	Damage;
	VECTOR	TempDir, TempVector;
	VECTOR	Int_Point;
	NORMAL	Int_Normal;
	MATRIX	UpMatrix;
	MATRIX	InvUpMatrix;
	QUAT	TempQuat;
	VECTOR	OldPos;

	GlobCount += (int16_t) framelag;								// Use Timer!!!

	i = FirstFmPolyUsed;

	while( i != (u_int16_t) -1 )
	{
		nextprim = FmPolys[i].Prev;

		FmPolys[i].LifeCount -= framelag;
		if( FmPolys[i].LifeCount < 0.0F ) FmPolys[i].LifeCount = 0.0F;

		switch( FmPolys[i].SeqNum )
		{
		   	case FM_ANIMSPEED:							// Anim using animspeed
		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );

		   		}
		   		break;

		   	case FM_ANIM:								// Explosion?
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( FmPolys[i].Frame * 8.0F ) ) );
		   		FmPolys[i].Frame += ( framelag / 3.0F );

		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		   		}
		   		break;

		   	case FM_ANIM2:								// Ship Explosion?
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( FmPolys[i].Frame * 8.0F ) ) );
		   		FmPolys[i].Frame += ( framelag / 4.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		   		}
		   		break;

		   	case FM_SMOKE_TRAIL:						// Smoke Trail?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( FmPolys[i].Frame * 8.0F ) ) );

		   		FmPolys[i].R = ShadeTable[ (int16_t) FmPolys[i].Frame ];
		   		FmPolys[i].G = ShadeTable[ (int16_t) FmPolys[i].Frame ];
		   		FmPolys[i].B = ShadeTable[ (int16_t) FmPolys[i].Frame ];

		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 4.0F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		FmPolys[i].Frame += ( framelag / 2.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );

		   		}
		   		break;

		   	case FM_TROJAX_EXP:								// Explosion?
				FadeDec = ( 8.0F * framelag );
				R = ( ( (float) FmPolys[i].R ) - FadeDec );
				G = ( ( (float) FmPolys[i].G ) - FadeDec );
				B = ( ( (float) FmPolys[i].B ) - FadeDec );
				T = ( ( (float) FmPolys[i].Trans ) - FadeDec );
				if( R < 0.0F ) R = 0.0F;
				if( G < 0.0F ) G = 0.0F;
				if( B < 0.0F ) B = 0.0F;
				if( T < 0.0F ) T = 0.0F;
				FmPolys[i].R = (u_int8_t) R;
				FmPolys[i].G = (u_int8_t) G;
				FmPolys[i].B = (u_int8_t) B;
				FmPolys[i].Trans = (u_int8_t) T;

	   			FmPolys[i].xsize += ( TROJAX_EXPINC * framelag );
	   			FmPolys[i].ysize += ( TROJAX_EXPINC * framelag );
	   			if( FmPolys[i].xsize >= TROJAX_EXPMAXSIZE )
	   			{
	   				KillUsedFmPoly( i );
	   			}
		   		break;

		   	case FM_TROJAX:								// Trojax bullet
		   		FmPolys[ i ].Rot += ( FmPolys[i].RotSpeed * framelag );
		   		if( FmPolys[ i ].Rot > 360.0F ) FmPolys[ i ].Rot -= 360.0F;
		   		else if( FmPolys[ i ].Rot < 0.0F ) FmPolys[ i ].Rot += 360.0F;
				Rotation.x = (float) sin( D2R( FmPolys[ i ].Rot ) );
				Rotation.y = (float) cos( D2R( FmPolys[ i ].Rot ) );
				Rotation.z = 0.0F;
				ApplyMatrix( &FmPolys[ i ].Mat, &Rotation, &FmPolys[ i ].UpVector );
		   		break;

		   	case FM_PULSAR_EXP:							// Pulsar Explosion?
		   		FmPolys[i].Frame += ( framelag / 4.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		   		}
		   		break;

		   	case FM_PULSAR:
		   		break;

			case FM_NMEBULLET1:
		   		FmPolys[i].Frame += ( framelag / 3.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			FmPolys[i].Frame -= ( ( FmPolys[i].Frame / (*FmPolys[i].Frm_Info)->Num_Frames )
											* (*FmPolys[i].Frm_Info)->Num_Frames );
		   		}
		   		break;

		   	case FM_SHRAPNEL_EXP:						// Shrapnel Explosion?
				if( FmPolys[i].LifeCount < 995.0F )
				{
					if( FmPolys[i].Frm_Info != &Shrapnel_Header )
					{
						RemoveFmPolyFromTPage( i, GetTPage( *FmPolys[i].Frm_Info, (int16_t) FmPolys[i].Frame ) );
				   		FmPolys[i].Frm_Info = &Shrapnel_Header;
						AddFmPolyToTPage( i, GetTPage( *FmPolys[i].Frm_Info, 0 ) );
						FmPolys[i].Frame = 0.0F;
						FmPolys[i].xsize = SUSSGUN_EXPSIZE;
						FmPolys[i].ysize = SUSSGUN_EXPSIZE;
						FmPolys[i].R = 128;
						FmPolys[i].G = 128;
						FmPolys[i].B = 128;
					}
					else
					{
				   		FmPolys[i].Frame += framelag;
				   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
				   		{
				   			KillUsedFmPoly( i );
				   		}
					}
				}
		   		break;

		   	case FM_FLAME:								// Flame Thrower?
				FmPolys[ i ].Rot += ( FmPolys[i].RotSpeed * framelag );
		   		if( FmPolys[ i ].Rot > 360.0F ) FmPolys[ i ].Rot -= 360.0F;
		   		else if( FmPolys[ i ].Rot < 0.0F ) FmPolys[ i ].Rot += 360.0F;

				FmPolys[i].Frame += framelag;
		   		if( FmPolys[i].Frame > 12.0F ) FmPolys[i].Frame = 12.0F;
		   		break;

		   	case FM_STATIC:								// Static Obj Don't move/rotate
		   		break;

		   	case FM_ARC:								// Arc?
		   		break;

		   	case FM_ARC2:								// Arc Glow?
		   		break;

		   	case FM_ARC_SPARK:							// Arc Sparks?
		   		FmPolys[i].Frame += ( framelag / 5.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
		   		else
		   		{
			   		if( FmPolys[i].Speed > 0.0F )
			   		{
			   			FmPolys[i].Speed -= 0.5F;
			   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
			   		}

					Speed = ( FmPolys[i].Speed * framelag );
		   			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
		   		}
		   		break;

		   	case FM_BITS:								// Bits?
		   		FmPolys[i].Frame += ( framelag / 6.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
				else
				{
					Speed = ( FmPolys[i].Speed * framelag );
		   			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
				}
		   		break;

		   	case FM_ARC_RING:							// Arc Explosion Ring?
				FadeDec = ( 8.0F * framelag );
				R = ( ( (float) FmPolys[i].R ) - FadeDec );
				G = ( ( (float) FmPolys[i].G ) - FadeDec );
				B = ( ( (float) FmPolys[i].B ) - FadeDec );
				T = ( ( (float) FmPolys[i].Trans ) - FadeDec );
				if( R < 0.0F ) R = 0.0F;
				if( G < 0.0F ) G = 0.0F;
				if( B < 0.0F ) B = 0.0F;
				if( T < 0.0F ) T = 0.0F;
				FmPolys[i].R = (u_int8_t) R;
				FmPolys[i].G = (u_int8_t) G;
				FmPolys[i].B = (u_int8_t) B;
				FmPolys[i].Trans = (u_int8_t) T;

	   			FmPolys[i].xsize += ( TRANSPULSE_EXP_INC * framelag );
	   			FmPolys[i].ysize += ( TRANSPULSE_EXP_INC * framelag );
	   			if( FmPolys[i].xsize >= TRANSPULSE_EXP_MAXSIZE )
	   			{
	   				KillUsedFmPoly( i );
	   			}
		   		break;

		   	case FM_FLARE:								// Arc Explosion Flare?
				if( FmPolys[i].LifeCount == 0.0F )
				{
	   				KillUsedFmPoly( i );
				}
				break;

		   	case FM_GRAVGON_TRAIL:						// Gravgon Smoke Trail?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( ( FmPolys[i].Frame * 5.0F ) * 8.0F ) ) );

		   		FmPolys[i].R = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 5.0F )];
		   		FmPolys[i].G = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 5.0F )];
		   		FmPolys[i].B = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 5.0F )];

		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				FmPolys[i].xsize += ( GRAVGON_TRAIL_INC * framelag );
				FmPolys[i].ysize += ( GRAVGON_TRAIL_INC * framelag );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= ( 2.0F * framelag );
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		FmPolys[i].Frame += ( 0.1F * framelag );
				if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames ) KillUsedFmPoly( i );
		   		break;

		   	case FM_SOLARIS_TRAIL:						// Solaris Smoke Trail?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( ( FmPolys[i].Frame * 3.0F ) * 8.0F ) ) );

		   		FmPolys[i].R = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 3.0F )];
		   		FmPolys[i].G = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 3.0F )];
		   		FmPolys[i].B = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 3.0F )];

		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= ( 2.0F * framelag );
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		FmPolys[i].Frame += ( 0.166F * framelag );
				if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames ) KillUsedFmPoly( i );
		   		break;

		   	case FM_NEW_TRAIL:								// New Smoke Trail?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( ( FmPolys[i].Frame * 3.0F ) * 8.0F ) ) );

		   		FmPolys[i].R = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 3.0F )];
		   		FmPolys[i].G = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 3.0F )];
		   		FmPolys[i].B = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 3.0F )];

		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= ( 2.0F * framelag );
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		FmPolys[i].Frame += ( 0.166F * framelag );
				if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames ) KillUsedFmPoly( i );
		   		break;

		   	case FM_PICKUP_REGEN:							// Regenerating Pickup?
		   		FmPolys[i].Frame += ( framelag / 6.0F );

		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		   		}
		   		break;

		   	case FM_BLOOD:									// Blood?
		   		FmPolys[i].Frame += ( framelag / 8.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
				else
				{
					Speed = ( FmPolys[i].Speed * framelag );
		   			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
				}
		   		break;

		   	case FM_BLOODSPLAT:								// Blood Splat?
		   		FmPolys[i].Frame += ( framelag / 8.0F ); //6.0F;
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
				else
				{
					FadeDec = ( 255.0F / ( (*FmPolys[i].Frm_Info)->Num_Frames * 8.0F ) ) * framelag;
					R = ( ( (float) FmPolys[i].R ) - FadeDec );
					G = ( ( (float) FmPolys[i].G ) - FadeDec );
					B = ( ( (float) FmPolys[i].B ) - FadeDec );
					T = ( ( (float) FmPolys[i].Trans ) - FadeDec );
					if( R < 0.0F ) R = 0.0F;
					if( G < 0.0F ) G = 0.0F;
					if( B < 0.0F ) B = 0.0F;
					if( T < 0.0F ) T = 0.0F;
 					FmPolys[i].R = (u_int8_t) R;
					FmPolys[i].G = (u_int8_t) G;
					FmPolys[i].B = (u_int8_t) B;
					FmPolys[i].Trans = (u_int8_t) T;
				}
		   		break;

		   	case FM_BLOODDRIBBLE:							// Blood Splat?
		   		FmPolys[i].Frame += ( framelag / 8.0F ); //6.0F;
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
		   		break;

			case FM_FLASH:								// Flash?
				if( FmPolys[i].LifeCount == 0.0F )
				{
					KillUsedFmPoly( i );
				}
				else
				{
					FmPolys[i].xsize -= ( 0.1F * framelag );
					if( FmPolys[i].xsize < 0.0F ) FmPolys[i].xsize = 0.0F;
					FmPolys[i].ysize -= ( 0.1F * framelag );
					if( FmPolys[i].ysize < 0.0F ) FmPolys[i].ysize = 0.0F;
				}
				break;

			case FM_MUZZLEFLASH:						// Muzzle Flash?
				if( FmPolys[i].LifeCount == 0.0F )
				{
					KillUsedFmPoly( i );
				}
				break;


		   	case FM_BLACK_SMOKE:						// Black Smoke?
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( FmPolys[i].Frame * 8.0F ) ) );

				Speed = ( FmPolys[i].UpSpeed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 0.2F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		if( FmPolys[i].UpSpeed < 10.0F )
		   		{
		   			FmPolys[i].UpSpeed += 0.2F;
		   			if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   		}

		   		FmPolys[i].Frame += ( framelag / 2.0F );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );

		   		}
		   		break;

		   	case FM_MUSHROOM:								// Mushroom Explosion?
		   		FmPolys[i].Frame += ( framelag / 6.0F );

		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
		   		}
		   		break;

		   	case FM_SMOKE_RING:								// Smoke Ring?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = (u_int8_t) 255;
		   		FmPolys[i].R = SmokeRingShadeTable[ (int16_t) FmPolys[i].Frame ];
		   		FmPolys[i].G = SmokeRingShadeTable[ (int16_t) FmPolys[i].Frame ];
		   		FmPolys[i].B = SmokeRingShadeTable[ (int16_t) FmPolys[i].Frame ];

		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );

		   		}
		   		break;

		   	case FM_SPOTFX_SMOKE:							// Smoke?
		   		FmPolys[i].Trans = ( 128 - ( (int16_t) ( FmPolys[i].Frame * 25.0F ) ) );

				Speed = (float) ( 64.0F - ( (float) ( FmPolys[i].Frame * 12.0F ) ) );
				FmPolys[i].R = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_R ) );
				FmPolys[i].G = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_G ) );
				FmPolys[i].B = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_B ) );

				Speed = ( FmPolys[i].UpSpeed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				FmPolys[i].xsize += ( ( 0.5F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize += ( ( 0.5F * GLOBAL_SCALE ) * framelag );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 0.01F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		if( FmPolys[i].UpSpeed < 4.0F )
		   		{
		   			FmPolys[i].UpSpeed += 0.075F;
		   			if( FmPolys[i].UpSpeed > 4.0F ) FmPolys[i].UpSpeed = 4.0F;
		   		}

		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );

		   		}
		   		break;

		   	case FM_SPOTFX_STEAM:							// STEAM?
		   		FmPolys[i].Trans = (u_int8_t) 255;

				Speed = SmokeRingShadeTable[ (int16_t) FmPolys[i].Frame ];
				FmPolys[i].R = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_R ) );
				FmPolys[i].G = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_G ) );
				FmPolys[i].B = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_B ) );

				Speed = ( FmPolys[i].UpSpeed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				FmPolys[i].xsize += ( ( 0.5F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize += ( ( 0.5F * GLOBAL_SCALE ) * framelag );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 0.2F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		if( FmPolys[i].UpSpeed < 10.0F )
		   		{
		   			FmPolys[i].UpSpeed += 0.2F;
		   			if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   		}

		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );

		   		}
		   		break;

		   	case FM_SPOTFX_FLAME:							// Flame?
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( FmPolys[i].Frame * 8.0F ) ) );

				Speed = ( FmPolys[i].UpSpeed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				OldSize = FmPolys[i].xsize;

				FmPolys[i].xsize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );

				if( ( OldSize >= ( 3.5F * GLOBAL_SCALE ) ) && ( FmPolys[i].xsize < ( 3.5F * GLOBAL_SCALE ) ) )
				{
					CreateSpotFXFlameSmoke( &FmPolys[i].Pos, &FmPolys[i].Dir, FmPolys[i].Group );
				}

				if( ( FmPolys[i].xsize > 0.0F ) && ( FmPolys[i].ysize > 0.0F ) )
				{
		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= 0.2F;
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}

		   			if( FmPolys[i].UpSpeed < 10.0F )
		   			{
		   				FmPolys[i].UpSpeed += 0.2F;
		   				if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   			}

		   			FmPolys[i].Frame += ( framelag / 2.0F );
		   			if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   			{
		   				KillUsedFmPoly( i );

		   			}
				}
				else
				{
	   				KillUsedFmPoly( i );
				}
		   		break;

		   	case FM_SPOTFX_SPARKS:							// Sparks?
		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
				else
				{
					Speed = ( FmPolys[i].Speed * framelag );
		   			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );
				}
		   		break;

			case FM_SPOTFX_SPARKSFACEME:
				if( FmPolys[i].LifeCount == 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
		   		break;

		   	case FM_SPOTFX_GRAVGON_TRAIL:						// Gravgon Smoke Trail?
				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( ( FmPolys[i].Frame * 5.0F ) * 8.0F ) ) );

		   		FmPolys[i].R = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 5.0F )];
		   		FmPolys[i].G = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 5.0F )];
		   		FmPolys[i].B = ShadeTable[ (int16_t) ( FmPolys[i].Frame * 5.0F )];

		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				FmPolys[i].xsize += ( GRAVGON_TRAIL_INC * framelag );
				FmPolys[i].ysize += ( GRAVGON_TRAIL_INC * framelag );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed += ( 0.1F * framelag );
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		FmPolys[i].Frame += ( 0.1F * framelag );
				if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames ) KillUsedFmPoly( i );
		   		break;

		   	case FM_SPOTFX_GLOW:							// SpotFX Glow?
				if( FmPolys[i].OnceOnly )
				{
					FmPolys[i].OnceOnly--;
				}
				else
				{
		   			KillUsedFmPoly( i );
				}
				break;

		   	case FM_NME_TRAIL:								// Nme Trail?
				if( FmPolys[i].LifeCount == 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					FmPolys[i].xsize -= ( ( 0.175F * GLOBAL_SCALE ) * framelag );
					FmPolys[i].ysize -= ( ( 0.175F * GLOBAL_SCALE ) * framelag );

					Speed = ( FmPolys[i].Speed * framelag );
					FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= ( 2.0F * framelag );
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}
				}
		   		break;

			case FM_NME_VAPOUR_TRAIL:						// Vapour Trail?
		   		FmPolys[i].Trans = ( 128 - ( (int16_t) ( FmPolys[i].Frame * 25.0F ) ) );

				Speed = (float) ( 64.0F - ( (float) ( FmPolys[i].Frame * 12.0F ) ) );
				FmPolys[i].R = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_R ) );
				FmPolys[i].G = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_G ) );
				FmPolys[i].B = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_B ) );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				FmPolys[i].xsize += ( ( 0.05F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize += ( ( 0.05F * GLOBAL_SCALE ) * framelag );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 0.01F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );

		   		}
		   		break;

		   	case FM_SPOTFX_SHRAPNEL_TRAIL:						// Shrapnel Trail?
		   		FmPolys[i].Trans = (u_int8_t) 255;

				Speed = SmokeRingShadeTable[ (int16_t) FmPolys[i].Frame ];
				FmPolys[i].R = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_R ) );
				FmPolys[i].G = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_G ) );
				FmPolys[i].B = (u_int8_t) ( ( Speed / 256.0F ) * ( (float) FmPolys[i].Start_B ) );

				FmPolys[i].xsize += ( ( 0.25F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize += ( ( 0.25F * GLOBAL_SCALE ) * framelag );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

		   		if( FmPolys[i].Speed > 0.0F )
		   		{
		   			FmPolys[i].Speed -= 0.2F;
		   			if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   		}

		   		if( FmPolys[i].UpSpeed < 10.0F )
		   		{
		   			FmPolys[i].UpSpeed += 0.2F;
		   			if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   		}

		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );

		   		}
		   		break;


		   	case FM_SPOTFX_REALSPARKS:						// Real Sparks?
		   		FmPolys[i].Frame += ( framelag * 0.1F );
				FmPolys[i].Frame = FMOD( FmPolys[i].Frame, (*FmPolys[i].Frm_Info)->Num_Frames );

				if( FmPolys[i].LifeCount == 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					QuatFrom2Vectors( &TempQuat, &Mloadheader.Group[ FmPolys[i].Group ].up, &SlideUp );
					QuatToMatrix( &TempQuat, &UpMatrix );
					MatrixTranspose( &UpMatrix, &InvUpMatrix );

	     			TempDir.x = ( FmPolys[i].Dir.x * framelag );
	     			TempDir.y = ( FmPolys[i].Dir.y * framelag );
  	     			TempDir.z = ( FmPolys[i].Dir.z * framelag );

					if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, &FmPolys[i].Pos,
	     								  FmPolys[i].Group, &TempDir, (VECTOR *) &Int_Point,
	     								  &FmPolys[i].Group, &Int_Normal, &TempVector, true, NULL ) == true )
	     			{
						FmPolys[i].Pos = Int_Point;

	     				TempVector = FmPolys[i].Dir;
	     				NormaliseVector( &TempVector );
	     				Speed = VectorLength( &FmPolys[i].Dir );
	     				ReflectVector( &TempVector, &Int_Normal, &FmPolys[i].Dir );
	     				Speed *= 0.5F;

						if( Speed < ( 5.0F * GLOBAL_SCALE ) )
	     				{
		  					KillUsedFmPoly( i );
	     					break;
	     				}

						FmPolys[i].Dir.x *= Speed;
	     				FmPolys[i].Dir.y *= Speed;
	     				FmPolys[i].Dir.z *= Speed;
	     			}
					else
					{
		     			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * framelag );
		     			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * framelag );
		     			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * framelag );
					}

					ApplyMatrix( &UpMatrix, &FmPolys[i].Dir, &TempDir );

	     			if( TempDir.x )
	     			{
	     				if( TempDir.x > 0.0F )
	     				{
	     					TempDir.x -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     					if( TempDir.x < 0.0F ) TempDir.x = 0.0F;
	     				}
	     				else
	     				{
	     					TempDir.x += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     					if( TempDir.x > 0.0F ) TempDir.x = 0.0F;
	     				}
	     			}
	     			if( TempDir.z )
	     			{
	     				if( TempDir.z > 0.0F )
	     				{
	     					TempDir.z -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     					if( TempDir.z < 0.0F ) TempDir.z = 0.0F;
	     				}
	     				else
	     				{
	     					TempDir.z += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     					if( TempDir.z > 0.0F ) TempDir.z = 0.0F;
	     				}
	     			}
	     			TempDir.y -= ( ( 0.5F * GLOBAL_SCALE ) * framelag );

					ApplyMatrix( &InvUpMatrix, &TempDir, &FmPolys[i].Dir );
				}
	 			break;

			case FM_SPOTFX_BUBBLE:							// Bubble?
				OldPos = FmPolys[i].Pos;

				Speed = ( FmPolys[i].UpSpeed * framelag );
	   			FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.10F ) ) * framelag );
	   			FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.05F ) ) * framelag );
	   			FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.15F ) ) * framelag );

				Speed = ( FmPolys[i].Speed * framelag );
	   			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.10F ) ) * framelag );
	   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.05F ) ) * framelag );
	   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed ) + ( (float) sin( FmPolys[i].LifeCount * ( 0.15F ) ) * framelag );

	   			if( FmPolys[i].Speed > 0.0F )
	   			{
	   				FmPolys[i].Speed -= ( 0.4F * framelag );
	   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
	   			}

	   			if( FmPolys[i].UpSpeed < 4.0F )
	   			{
	   				FmPolys[i].UpSpeed += ( 0.4F * framelag );
	   				if( FmPolys[i].UpSpeed > 4.0F ) FmPolys[i].UpSpeed = 4.0F;
	   			}

				TempDir.x = ( FmPolys[i].Pos.x - OldPos.x );
				TempDir.y = ( FmPolys[i].Pos.y - OldPos.y );
				TempDir.z = ( FmPolys[i].Pos.z - OldPos.z );
				FmPolys[i].Group = MoveGroup( &Mloadheader, &OldPos, FmPolys[i].Group, &TempDir );

				if( ( FmPolys[i].LifeCount == 0.0F ) ||	( !InWater( FmPolys[i].Group, &FmPolys[i].Pos, &Damage ) ) )
				{
					TempDir.x = ( FmPolys[i].Pos.x - OldPos.x );
					TempDir.y = ( FmPolys[i].Pos.y - OldPos.y );
					TempDir.z = ( FmPolys[i].Pos.z - OldPos.z );
					WaterObjectCollide( FmPolys[i].Group, &OldPos, &TempDir, &Int_Point, 8 );
		   			KillUsedFmPoly( i );
				}
		   		break;

			case FM_NMEGEN_EFFECT:
				if( FmPolys[ i ].LifeCount <= 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					if( FmPolys[ i ].LifeCount >= NMEGEN_STAGE1 )			// 0 - 3
					{
			   			FmPolys[ i ].Rot -= ( ( 360.0F / NMEGEN_STAGE0_LEN ) * framelag ); // 1 Rot in 3 seconds
			   			FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );
						FmPolys[ i ].R = (u_int8_t) ( 172.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE1 ) * ( 172.0F / NMEGEN_STAGE0_LEN ) ) );
						FmPolys[ i ].G = (u_int8_t) ( 49.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE1 ) * ( 49.0F / NMEGEN_STAGE0_LEN ) ) );
					}
					else
					{
			   			FmPolys[ i ].Rot -= ( ( 360.0F / ( NMEGEN_STAGE1_LEN + NMEGEN_STAGE2_LEN ) ) * framelag );	// 1 Rot in 2 Seconds
			   			FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );

						if( FmPolys[ i ].LifeCount >= NMEGEN_STAGE2 )		// 3 - 4
						{
							FmPolys[ i ].R = (u_int8_t) ( 255.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE2 ) * ( 83.0F / NMEGEN_STAGE1_LEN ) ) );
							FmPolys[ i ].G = (u_int8_t) ( 255.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE2 ) * ( 206.0F / NMEGEN_STAGE1_LEN ) ) );
							FmPolys[ i ].B = (u_int8_t) ( 255.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE2 ) * ( 255.0F / NMEGEN_STAGE1_LEN ) ) );
							FmPolys[ i ].xsize -= ( ( ( 6.0F * GLOBAL_SCALE ) / NMEGEN_STAGE1_LEN ) * framelag );
							FmPolys[ i ].ysize = FmPolys[ i ].xsize;
						}
						else												// 4 - 5
						{
							FmPolys[ i ].xsize -= ( ( ( 10.0F * GLOBAL_SCALE ) / NMEGEN_STAGE2_LEN ) * framelag );
							FmPolys[ i ].ysize = FmPolys[ i ].xsize;
						}
					}
				}
				break;

			case FM_NMEGEN_FLARE:
				if( FmPolys[ i ].LifeCount <= 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					if( FmPolys[ i ].LifeCount >= NMEGEN_STAGE1 )			// 0 - 3
					{
						FmPolys[ i ].R = (u_int8_t) ( 172.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE1 ) * ( 172.0F / NMEGEN_STAGE0_LEN ) ) );
						FmPolys[ i ].G = (u_int8_t) ( 172.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE1 ) * ( 172.0F / NMEGEN_STAGE0_LEN ) ) );
						FmPolys[ i ].B = (u_int8_t) ( 172.0F - ( ( FmPolys[ i ].LifeCount - NMEGEN_STAGE1 ) * ( 172.0F / NMEGEN_STAGE0_LEN ) ) );
						FmPolys[ i ].xsize += ( ( ( 14.0F * GLOBAL_SCALE ) / NMEGEN_STAGE0_LEN ) * framelag );
						FmPolys[ i ].ysize = FmPolys[ i ].xsize;
					}
					else
					{
						if( FmPolys[ i ].LifeCount >= NMEGEN_STAGE2 )		// 3 - 4
						{
							FmPolys[ i ].xsize -= ( ( ( 6.0F * GLOBAL_SCALE ) / NMEGEN_STAGE1_LEN ) * framelag );
							FmPolys[ i ].ysize = FmPolys[ i ].xsize;
						}
						else												// 4 - 5
						{
							FmPolys[ i ].xsize -= ( ( ( 8.0F * GLOBAL_SCALE ) / NMEGEN_STAGE2_LEN ) * framelag );
							FmPolys[ i ].ysize = FmPolys[ i ].xsize;
						}
					}
				}
				break;

			case FM_SPOTFX_DRIP:
				if( FmPolys[ i ].LifeCount <= 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					FmPolys[i].Speed += ( 0.1F * framelag );
					if( FmPolys[i].Speed > ( 40.0F * GLOBAL_SCALE ) ) FmPolys[i].Speed = ( 40.0F * GLOBAL_SCALE );

					Speed = ( FmPolys[i].Speed * framelag );

					TempDir.x = ( FmPolys[i].Dir.x * Speed );
					TempDir.y = ( FmPolys[i].Dir.y * Speed );
					TempDir.z = ( FmPolys[i].Dir.z * Speed );

					if( WaterObjectCollide( FmPolys[i].Group, &FmPolys[i].Pos, &TempDir, &Int_Point, 32 ) )
					{
						CreateSpotFXDripSplash( &Int_Point, &SlideUp, FmPolys[i].Group, FmPolys[i].R, FmPolys[i].G, FmPolys[i].B );
			   			KillUsedFmPoly( i );
					}
					else
					{
		     			if( BackgroundCollide( &MCloadheadert0 ,&Mloadheader, &FmPolys[i].Pos,
		     								  FmPolys[i].Group, &TempDir, (VECTOR *) &Int_Point,
			 								  &FmPolys[i].Group, &Int_Normal, &TempVector, true, NULL ) == true )
	     				{
							CreateSpotFXDripSplash( &Int_Point, &SlideUp, FmPolys[i].Group, FmPolys[i].R, FmPolys[i].G, FmPolys[i].B );
				   			KillUsedFmPoly( i );
						}
						else
						{
			   				FmPolys[i].Pos.x += TempDir.x;
			   				FmPolys[i].Pos.y += TempDir.y;
			   				FmPolys[i].Pos.z += TempDir.z;
						}
					}
				}
				break;

			case FM_PHOTON:
		   		FmPolys[ i ].Rot += ( FmPolys[i].RotSpeed * framelag );
		   		FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );

				FmPolys[i].ysize = (float) ( ( sin( FmPolys[ i ].Rot ) * 4.0F ) );
				break;

			case FM_BLUEPHOTON:
		   		FmPolys[ i ].Rot += ( FmPolys[i].RotSpeed * framelag );
		   		FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );

				FmPolys[i].ysize = (float) ( ( sin( FmPolys[ i ].Rot ) * 6.0F ) );
				break;

		   	case FM_SPOTFX_BURNING:
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( FmPolys[i].Frame * 8.0F ) ) );

				Speed = ( FmPolys[i].UpSpeed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				OldSize = FmPolys[i].xsize;

				FmPolys[i].xsize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );

				if( ( FmPolys[i].xsize > 0.0F ) && ( FmPolys[i].ysize > 0.0F ) )
				{
		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= ( 0.15F * framelag );
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}

		   			if( FmPolys[i].UpSpeed < 10.0F )
		   			{
		   				FmPolys[i].UpSpeed += ( 0.15F * framelag );
		   				if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   			}

		   			FmPolys[i].Frame += ( framelag / 2.0F );
		   			if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   			{
		   				KillUsedFmPoly( i );

		   			}
				}
				else
				{
	   				KillUsedFmPoly( i );
				}
		   		break;

		   	case FM_SPOTFX_BEARD_AFTERBURNER:
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( FmPolys[i].Frame * 8.0F ) ) );

				Speed = ( FmPolys[i].UpSpeed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				FmPolys[i].xsize -= ( ( 0.1F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize -= ( ( 0.1F * GLOBAL_SCALE ) * framelag );

				if( ( FmPolys[i].xsize > 0.0F ) && ( FmPolys[i].ysize > 0.0F ) )
				{
		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= ( 0.15F * framelag );
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}

		   			if( FmPolys[i].UpSpeed < 10.0F )
		   			{
		   				FmPolys[i].UpSpeed += ( 0.15F * framelag );
		   				if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   			}

			   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
			   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
			   		{
		   				KillUsedFmPoly( i );
					}
				}
				else
				{
	   				KillUsedFmPoly( i );
				}

		   		break;

		   	case FM_RESTART_INACTIVE:							// Restart Point Inactive
				FadeColour( &FmPolys[i].R, FmPolys[i].Start_R, 5.0F );
				FadeColour( &FmPolys[i].G, FmPolys[i].Start_G, 5.0F );
				FadeColour( &FmPolys[i].B, FmPolys[i].Start_B, 5.0F );

				FmPolys[ i ].Rot += ( FmPolys[ i ].RotSpeed * framelag );
				FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );
				break;

		   	case FM_RESTART_ACTIVE:								// Restart Point Active
				FadeColour( &FmPolys[i].R, FmPolys[i].Start_R, 5.0F );
				FadeColour( &FmPolys[i].G, FmPolys[i].Start_G, 5.0F );
				FadeColour( &FmPolys[i].B, FmPolys[i].Start_B, 5.0F );

				FmPolys[ i ].Rot += ( FmPolys[ i ].RotSpeed * framelag );
				FmPolys[ i ].Rot = FMOD( FmPolys[ i ].Rot, 360.0F );
		   		break;

		   	case FM_SPOTFX_WATERSPLASH:							// Water Splash?
		   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   		if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   		{
		   			KillUsedFmPoly( i );
				}
				else
				{
					QuatFrom2Vectors( &TempQuat, &Mloadheader.Group[ FmPolys[i].Group ].up, &SlideUp );
					QuatToMatrix( &TempQuat, &UpMatrix );
					MatrixTranspose( &UpMatrix, &InvUpMatrix );

	     			TempDir.x = ( FmPolys[i].Dir.x * framelag );
	     			TempDir.y = ( FmPolys[i].Dir.y * framelag );
  	     			TempDir.z = ( FmPolys[i].Dir.z * framelag );

					if( WaterObjectCollide( FmPolys[i].Group, &FmPolys[i].Pos, &TempDir, &Int_Point, 0 ) )
					{
			   			KillUsedFmPoly( i );
					}
					else
					{
		     			FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * framelag );
		     			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * framelag );
			 			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * framelag );

						ApplyMatrix( &UpMatrix, &FmPolys[i].Dir, &TempDir );

	     				if( TempDir.x )
	     				{
	     					if( TempDir.x > 0.0F )
	     					{
	     						TempDir.x -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x < 0.0F ) TempDir.x = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.x += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.x > 0.0F ) TempDir.x = 0.0F;
	     					}
	     				}
	     				if( TempDir.z )
	     				{
	     					if( TempDir.z > 0.0F )
	     					{
	     						TempDir.z -= ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z < 0.0F ) TempDir.z = 0.0F;
	     					}
	     					else
	     					{
	     						TempDir.z += ( ( 0.000001F * GLOBAL_SCALE ) * framelag );
	     						if( TempDir.z > 0.0F ) TempDir.z = 0.0F;
	     					}
	     				}
	     				TempDir.y -= ( ( 0.5F * GLOBAL_SCALE ) * framelag );

						ApplyMatrix( &InvUpMatrix, &TempDir, &FmPolys[i].Dir );
					}
				}
	 			break;

		   	case FM_HK5_TRAIL:								// Hk5 Trail?
				if( FmPolys[i].LifeCount == 0.0F )
				{
		   			KillUsedFmPoly( i );
				}
				else
				{
					FmPolys[i].xsize -= ( ( 0.25F * GLOBAL_SCALE ) * framelag );
					FmPolys[i].ysize -= ( ( 0.25F * GLOBAL_SCALE ) * framelag );

					Speed = ( FmPolys[i].Speed * framelag );
					FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   			FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   			FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= ( 0.125F * framelag );
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}
				}
		   		break;

		   	case FM_FIREBALL:
		   		FmPolys[i].Trans = ( 248 - ( (int16_t) ( FmPolys[i].Frame * 8.0F ) ) );

				Speed = ( FmPolys[i].UpSpeed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].UpVector.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].UpVector.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].UpVector.z * Speed );

				Speed = ( FmPolys[i].Speed * framelag );
		   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
		   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
		   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

				OldSize = FmPolys[i].xsize;

				FmPolys[i].xsize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );
				FmPolys[i].ysize -= ( ( 0.15F * GLOBAL_SCALE ) * framelag );

				if( ( FmPolys[i].xsize > 0.0F ) && ( FmPolys[i].ysize > 0.0F ) )
				{
		   			if( FmPolys[i].Speed > 0.0F )
		   			{
		   				FmPolys[i].Speed -= ( 0.15F * framelag );
		   				if( FmPolys[i].Speed < 0.0F ) FmPolys[i].Speed = 0.0F;
		   			}

		   			if( FmPolys[i].UpSpeed < 10.0F )
		   			{
		   				FmPolys[i].UpSpeed += ( 0.15F * framelag );
		   				if( FmPolys[i].UpSpeed > 10.0F ) FmPolys[i].UpSpeed = 10.0F;
		   			}

			   		FmPolys[i].Frame += ( framelag / FmPolys[i].AnimSpeed );
		   			if( FmPolys[i].Frame >= (*FmPolys[i].Frm_Info)->Num_Frames )
		   			{
		   				KillUsedFmPoly( i );

		   			}
				}
				else
				{
	   				KillUsedFmPoly( i );
				}
		   		break;

			case FM_SPARKLE:						// Sparkle?
				if( FmPolys[i].LifeCount == 0.0F )
				{
					KillUsedFmPoly( i );
				}
				else
				{
					Speed = ( FmPolys[i].Speed * framelag );
			   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
			   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
			   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

					Speed = FMOD( FmPolys[i].LifeCount, 20.0F );

					if( Speed > 10.0F )
					{
						FmPolys[i].xsize = ( ( 20.0F - Speed ) * 0.05F );
					}
					else
					{
						FmPolys[i].xsize = ( Speed * 0.05F );
					}
					FmPolys[i].ysize = FmPolys[i].xsize;
				}
				break;

			case FM_GOALSPARKLE:						// Sparkle?
				if( FmPolys[i].LifeCount == 0.0F )
				{
					KillUsedFmPoly( i );
				}
				else
				{
					Speed = ( FmPolys[i].Speed * framelag );
			   		FmPolys[i].Pos.x += ( FmPolys[i].Dir.x * Speed );
			   		FmPolys[i].Pos.y += ( FmPolys[i].Dir.y * Speed );
			   		FmPolys[i].Pos.z += ( FmPolys[i].Dir.z * Speed );

					Speed = FMOD( FmPolys[i].LifeCount, 20.0F );

					if( Speed > 10.0F )
					{
						FmPolys[i].xsize = ( ( 20.0F - Speed ) * 0.05F );
					}
					else
					{
						FmPolys[i].xsize = ( Speed * 0.05F );
					}
					FmPolys[i].ysize = FmPolys[i].xsize;
				}
				break;

		   	default:
		   		break;
		}
		i = nextprim;
	}
}

/*===================================================================
	Procedure	:	Init FmPoly TPage Groups
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitFmPolyTPages( void )
{
	u_int16_t	i;

	for( i = 0; i < MAXTPAGESPERTLOAD; i++ )
	{
		FmPolyTPages[ i ].FirstPoly = (u_int16_t) -1;
	}
}

/*===================================================================
	Procedure	:	Add FmPoly To TPage Link List
	Input		:	u_int16_t		FmPoly Index
				:	u_int16_t		TPage to add to
	Output		:	Nothing
===================================================================*/
void AddFmPolyToTPage( u_int16_t i, int16_t TPage )
{
	if( TPage == -1 ) TPage = MAXTPAGESPERTLOAD;

	FmPolys[ i ].PrevInTPage = (u_int16_t) -1;
	FmPolys[ i ].NextInTPage = FmPolyTPages[ TPage ].FirstPoly;

	if( FmPolys[ i ].NextInTPage != (u_int16_t) -1 )
	{
		FmPolys[ FmPolys[ i ].NextInTPage ].PrevInTPage = i;
	}

	FmPolyTPages[ TPage ].FirstPoly = i;
}

/*===================================================================
	Procedure	:	Remove FmPoly From TPage Link List
	Input		:	u_int16_t		FmPoly Index
				:	u_int16_t		TPage to add to
	Output		:	Nothing
===================================================================*/
void RemoveFmPolyFromTPage( u_int16_t i, int16_t TPage )
{
	if( TPage == -1 ) TPage = MAXTPAGESPERTLOAD;

	if( FmPolys[ i ].PrevInTPage != (u_int16_t) -1 )
	{
		FmPolys[ FmPolys[ i ].PrevInTPage ].NextInTPage = FmPolys[ i ].NextInTPage;
	}
	else
	{
		FmPolyTPages[ TPage ].FirstPoly = FmPolys[ i ].NextInTPage;
	}

	if( FmPolys[ i ].NextInTPage != (u_int16_t) -1 )
	{
		FmPolys[ FmPolys[ i ].NextInTPage ].PrevInTPage = FmPolys[ i ].PrevInTPage;
	}

	FmPolys[ i ].PrevInTPage = (u_int16_t) -1;
	FmPolys[ i ].NextInTPage = (u_int16_t) -1;
}

bool DisplayGroupClippedFmPolys( RENDEROBJECT *renderObject, u_int16_t Group )
{
	int16_t	TPage;
	u_int16_t	i;

	TPage = 0;
	i = FmPolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !FmPolyDispGroupClipped( Group, renderObject, &TPage, &i ) )
			return( true );

		cull_none();

		if (!draw_object(renderObject))
			return false;

		reset_cull();
	}

	return( false );
}

bool DisplayGroupUnclippedFmPolys(RENDEROBJECT *renderObject )
{
	int16_t	TPage;
	u_int16_t	i;

	TPage = 0;
	i = FmPolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !FmPolyDispGroupUnclipped( renderObject, &TPage, &i ) )
			return( true );

		cull_none();

		if (!draw_object(renderObject))
			return false;

		reset_cull();
	}

	return( false );
}

/*===================================================================
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	u_int16_t						Group
				:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16_t	*					Current TPage List
				:	u_int16_t	*					Current FmPoly
	Output		:	True/False
===================================================================*/
bool FmPolyDispGroupClipped( u_int16_t Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextFmPoly )
{
	u_int16_t			i;
	int16_t			Count;
	int16_t			BitCount;
	int16_t			TotalVerts;
	int16_t			StartVert;
	int16_t			NumVerts;
	int16_t			NumTris;
	VECTOR			TempVector;
	VECTOR			XVector;
	VECTOR			YVector;
	VECTOR			ZVector;
	VECTOR			Xoff;
	VECTOR			Yoff;
	VECTOR			Xsize; ZERO_STACK_MEM(Xsize);
	VECTOR			Ysize; ZERO_STACK_MEM(Ysize);
	VECTOR			NewPos;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
 	float			Distance;
	MATRIX			MatrixCopy;
	COLOR		Colour;
	LPLVERTEX	FmPolyVertPnt;
	LPTRIANGLE	FmPolyFacePnt;
    LPLVERTEX	lpBufStart;
	MATRIX			TempMatrix;
	QUAT			TempQuat;
	WORD			*lpIndices = NULL;
	int				start_index = 0;

/*===================================================================
		Find out how may verts involved in Exec Buffer
===================================================================*/
	TotalVerts = 0;

	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = TotalVerts;
		NumVerts = 0;
		NumTris = 0;

		if( Count == *TPage ) i = *NextFmPoly;
		else i = FmPolyTPages[ Count ].FirstPoly;

		while( ( i != (u_int16_t) -1 ) && ( ( StartVert + NumVerts ) < MAXFMPOLYVERTS ) )
		{
			if( !( FmPolys[i].Flags & FM_FLAG_DONTCLIP ) && ( FmPolys[i].Group == Group ) )
			{
				if( FmPolys[i].Frm_Info && (*FmPolys[i].Frm_Info ) )
				{
					Bit_Ptr = ( (*FmPolys[ i ].Frm_Info)->Bit_Info + (int16_t) FmPolys[ i ].Frame );
					NumTris += ( 2 * Bit_Ptr->numbits );
					NumVerts += ( 4 * Bit_Ptr->numbits );
				}
			}

			i = FmPolys[ i ].NextInTPage;
		}

		FmPolyTPages[ Count ].StartVert = StartVert;
		FmPolyTPages[ Count ].NumVerts = NumVerts;
		FmPolyTPages[ Count ].NumTris = NumTris;

		TotalVerts = ( StartVert + NumVerts );
		if( TotalVerts >= MAXFMPOLYVERTS ) break;
	}

	if( !TotalVerts ) return( false );

	renderObject->numTextureGroups = 0;

/*===================================================================
		Lock Exec Buffer and get ready to fill in...
===================================================================*/

	if (!(FSLockVertexBuffer(renderObject, &lpBufStart)))
	{
		return false;
	}

	if (!(FSLockIndexBuffer(renderObject, &lpIndices)))
	{
		return false;
	}

	FmPolyFacePnt = (LPTRIANGLE) lpIndices;

//	lpBufStart = ExecBuffer_debdesc.lpData;
	FmPolyVertPnt = (LPLVERTEX) lpBufStart;
	//lpPointer = (LPVOID) ( FmPolyVertPnt + TotalVerts );
	//lpInsStart = lpPointer;

/*===================================================================
		Fill in Exec Buffer ( Verts and Faces Simultaneously )
===================================================================*/
	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = FmPolyTPages[ Count ].StartVert;
		NumVerts = FmPolyTPages[ Count ].NumVerts;
		NumTris = FmPolyTPages[ Count ].NumTris;

		if( NumVerts )
		{
/* bjd - CHECK
		   	OP_PROCESS_VERTICES( 1, lpPointer );
		   	    PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, StartVert, NumVerts, lpPointer );
		   	OP_STATE_RENDER( 1, lpPointer );
		   	    STATE_DATA( D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Count ], lpPointer );
		   	OP_TRIANGLE_LIST( NumTris, lpPointer );
*/
	   		//FmPolyFacePnt = (LPTRIANGLE) lpPointer;

			if( Count == *TPage ) i = *NextFmPoly;
			else i = FmPolyTPages[ Count ].FirstPoly;

			while( ( i != (u_int16_t) -1 ) && ( StartVert < MAXFMPOLYVERTS ) )
			{
				if( !( FmPolys[i].Flags & FM_FLAG_DONTCLIP ) && ( FmPolys[i].Group == Group ) )
				{
					if( FmPolys[i].Frm_Info && (*FmPolys[i].Frm_Info ) )
					{
/*===================================================================
		Create Colour Value for polys
===================================================================*/

#if ACTUAL_TRANS
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, FmPolys[ i ].Trans );
#else
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, 255 );
#endif

/*===================================================================
		Set Dir and Up Vectors
===================================================================*/
						if( FmPolys[ i ].Flags & FM_FLAG_DIRCONST )
						{
							YVector = FmPolys[ i ].UpVector;					/* Calc Up Vector */
						   	ZVector = FmPolys[ i ].DirVector;					/* Calc Forward Vector */
						}
						else
						{
							YVector.x = (float) sin( D2R( FmPolys[ i ].Rot ) ); /* Calc Up Vector */
							YVector.y = (float) cos( D2R( FmPolys[ i ].Rot ) );
							YVector.z = 0.0F;
							ZVector = Forward;

							if( FmPolys[ i ].Flags & FM_FLAG_RELATIVE )
							{
								QuatFromDirAndUp( &FmPolys[i].DirVector, &FmPolys[i].UpVector, &TempQuat );
								QuatToMatrix( &TempQuat, &TempMatrix );
								ApplyMatrix( &TempMatrix, &YVector, &YVector );
								ApplyMatrix( &TempMatrix, &ZVector, &ZVector );
							}

							MatrixCopy = CurrentCamera.Mat;
							MatrixCopy._41 = 0.0F;
							MatrixCopy._42 = 0.0F;
							MatrixCopy._43 = 0.0F;
							ApplyMatrix( &MatrixCopy, &YVector, &YVector );		/* Calc Up Vector */
							ApplyMatrix( &MatrixCopy, &ZVector, &ZVector );		/* Calc Forward Vector */
						}

						CrossProduct( &YVector, &ZVector, &XVector );			/* Calc Left Vector */

/*===================================================================
		Setup FrameInfo description pointers
===================================================================*/
						Bit_Ptr = ( (*FmPolys[ i ].Frm_Info)->Bit_Info + (int16_t) FmPolys[ i ].Frame );
						Off_Ptr = ( (*FmPolys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );

						for( BitCount = 0; BitCount < Bit_Ptr->numbits; BitCount++ )
						{
							int ntris = 0;

							Box_Ptr = ( (*FmPolys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );

							Xoff.x = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.x );
							Xoff.y = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.y );
							Xoff.z = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.z );
							Yoff.x = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.x );
							Yoff.y = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.y );
							Yoff.z = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.z );

							switch( Off_Ptr->box & BOX_BOTHFLIP )
							{
								case BOX_NOFLIP:
			   						Xsize.x = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;

								case BOX_XFLIP:
			   						Xsize.x = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;

								case BOX_YFLIP:
			   						Xsize.x = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;

								case BOX_BOTHFLIP:
			   						Xsize.x = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;
							}

/*-------------- Errrrmmmmmmm ---------------*/
							if( ( FmPolys[ i ].Flags & FM_FLAG_MOVEOUT ) )
							{
								TempVector.x = ( FmPolys[ i ].Pos.x - CurrentCamera.Pos.x );
								TempVector.y = ( FmPolys[ i ].Pos.y - CurrentCamera.Pos.y );
								TempVector.z = ( FmPolys[ i ].Pos.z - CurrentCamera.Pos.z );
								Distance = VectorLength( &TempVector );
								NormaliseVector( &TempVector );

								if( Distance >= ( EXPLO_DISTANCE + 110.0F ) )
								{
									Distance = EXPLO_DISTANCE;
								}
								else
								{
									Distance -= 110.0F;
									if( Distance < 32.0F ) Distance = 32.0F;
								}

								NewPos.x = ( FmPolys[ i ].Pos.x + ( -Distance * TempVector.x ) );
								NewPos.y = ( FmPolys[ i ].Pos.y + ( -Distance * TempVector.y ) );
								NewPos.z = ( FmPolys[ i ].Pos.z + ( -Distance * TempVector.z ) );
							}
							else
							{
								NewPos = FmPolys[i].Pos;
							}
/*-------------------------------------------*/

							NewPos.x += ( Xoff.x + Yoff.x );
							NewPos.y += ( Xoff.y + Yoff.y );
							NewPos.z += ( Xoff.z + Yoff.z );

							FmPolyVertPnt->x = NewPos.x;						// v1
							FmPolyVertPnt->y = NewPos.y;
							FmPolyVertPnt->z = NewPos.z;
							FmPolyVertPnt->tu = Box_Ptr->u1;
							FmPolyVertPnt->tv = Box_Ptr->v1;
							FmPolyVertPnt->color = Colour;
//							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;

							FmPolyVertPnt->x = ( NewPos.x + Xsize.x );			// v2
							FmPolyVertPnt->y = ( NewPos.y + Xsize.y );
							FmPolyVertPnt->z = ( NewPos.z + Xsize.z );
							FmPolyVertPnt->tu = Box_Ptr->u2;
							FmPolyVertPnt->tv = Box_Ptr->v1;
							FmPolyVertPnt->color = Colour;
//							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;

							FmPolyVertPnt->x = ( NewPos.x + Xsize.x - Ysize.x ); // v3
							FmPolyVertPnt->y = ( NewPos.y + Xsize.y - Ysize.y );
							FmPolyVertPnt->z = ( NewPos.z + Xsize.z - Ysize.z );
							FmPolyVertPnt->tu = Box_Ptr->u2;
							FmPolyVertPnt->tv = Box_Ptr->v2;
							FmPolyVertPnt->color = Colour;
//							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;

							FmPolyVertPnt->x = ( NewPos.x - Ysize.x );			// v4
							FmPolyVertPnt->y = ( NewPos.y - Ysize.y );
							FmPolyVertPnt->z = ( NewPos.z - Ysize.z );
							FmPolyVertPnt->tu = Box_Ptr->u1;
							FmPolyVertPnt->tv = Box_Ptr->v2;
							FmPolyVertPnt->color = Colour;
//							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;

				   			FmPolyFacePnt->v1 = 0;
				   			FmPolyFacePnt->v2 = 1;
				   			FmPolyFacePnt->v3 = 2;
//				   			FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
				   			FmPolyFacePnt++;
							ntris++;

				   			FmPolyFacePnt->v1 = 0;
				   			FmPolyFacePnt->v2 = 2;
				   			FmPolyFacePnt->v3 = 3;
//				   			FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 );
				   			FmPolyFacePnt++;
							ntris++;

							renderObject->textureGroups[renderObject->numTextureGroups].numTriangles = ntris;
							renderObject->textureGroups[renderObject->numTextureGroups].numVerts = 4;
							renderObject->textureGroups[renderObject->numTextureGroups].startIndex = start_index;
							renderObject->textureGroups[renderObject->numTextureGroups].startVert = StartVert;
							renderObject->textureGroups[renderObject->numTextureGroups].texture = Tloadheader.lpTexture[Count];
							renderObject->textureGroups[renderObject->numTextureGroups].colourkey = Tloadheader.ColourKey[Count];
							INCREASE_TEXTURE_GROUPS(renderObject);

							start_index += ntris*3; // each triangle has three indexes...
							StartVert += 4;

							Off_Ptr++;
						}
					}
				}

				i = FmPolys[ i ].NextInTPage;
			}

	   		//lpPointer = ( LPVOID ) FmPolyFacePnt;
		}

		if( StartVert >= MAXFMPOLYVERTS ) break;
	}

//	OP_EXIT( lpPointer );

/*===================================================================
		UnLock Exec Buffer and set data description
===================================================================*/
	if (!(FSUnlockVertexBuffer(renderObject)))
	{
		return false;
	}

	if (!(FSUnlockIndexBuffer(renderObject)))
	{
		Msg( "FSUnlockIndexBuffer failed");
		return false ;
	}

	*TPage = Count;
	*NextFmPoly = i;

	return( true );
}

/*===================================================================
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16_t	*					Current TPage List
				:	u_int16_t	*					Current FmPoly
	Output		:	True/False
===================================================================*/
bool FmPolyDispGroupUnclipped( /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextFmPoly )
{
	u_int16_t			i;
	int16_t			Count;
	int16_t			BitCount;
	int16_t			TotalVerts;
	int16_t			StartVert;
	int16_t			NumVerts;
	int16_t			NumTris;
	VECTOR			TempVector;
	VECTOR			XVector;
	VECTOR			YVector;
	VECTOR			ZVector;
	VECTOR			Xoff;
	VECTOR			Yoff;
	VECTOR			Xsize; ZERO_STACK_MEM(Xsize);
	VECTOR			Ysize; ZERO_STACK_MEM(Ysize);
	VECTOR			NewPos;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
 	float			Distance;
	MATRIX			MatrixCopy;
	COLOR		Colour;
	LPLVERTEX	FmPolyVertPnt;
	LPTRIANGLE	FmPolyFacePnt;
    LPLVERTEX	lpBufStart;
	MATRIX			TempMatrix;
	QUAT			TempQuat;
	WORD			*lpIndices = NULL;
	int				start_index = 0;

/*===================================================================
		Find out how may verts involved in Exec Buffer
===================================================================*/
	TotalVerts = 0;

	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = TotalVerts;
		NumVerts = 0;
		NumTris = 0;

		if( Count == *TPage ) i = *NextFmPoly;
		else i = FmPolyTPages[ Count ].FirstPoly;

		while( ( i != (u_int16_t) -1 ) && ( ( StartVert + NumVerts ) < MAXFMPOLYVERTS ) )
		{
			if( ( FmPolys[i].Flags & FM_FLAG_DONTCLIP ) )
			{
				if( FmPolys[i].Frm_Info && (*FmPolys[i].Frm_Info ) )
				{
					Bit_Ptr = ( (*FmPolys[ i ].Frm_Info)->Bit_Info + (int16_t) FmPolys[ i ].Frame );
					NumTris += ( 2 * Bit_Ptr->numbits );
					NumVerts += ( 4 * Bit_Ptr->numbits );
				}
			}

			i = FmPolys[ i ].NextInTPage;
		}

		FmPolyTPages[ Count ].StartVert = StartVert;
		FmPolyTPages[ Count ].NumVerts = NumVerts;
		FmPolyTPages[ Count ].NumTris = NumTris;

		TotalVerts = ( StartVert + NumVerts );
		if( TotalVerts >= MAXFMPOLYVERTS ) break;
	}

	if( !TotalVerts ) return( false );

	renderObject->numTextureGroups = 0;

/*===================================================================
		Lock Exec Buffer and get ready to fill in...
===================================================================*/

	if (!(FSLockVertexBuffer(renderObject, &lpBufStart)))
	{
		return false;
	}

	if (!(FSLockIndexBuffer(renderObject, &lpIndices)))
	{
		return false;
	}

	FmPolyFacePnt = (LPTRIANGLE) lpIndices;

//	lpBufStart = ExecBuffer_debdesc.lpData;
	FmPolyVertPnt = (LPLVERTEX) lpBufStart;
	//lpPointer = (LPVOID) ( FmPolyVertPnt + TotalVerts );
	//lpInsStart = lpPointer;

/*===================================================================
		Fill in Exec Buffer ( Verts and Faces Simultaneously )
===================================================================*/
	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = FmPolyTPages[ Count ].StartVert;
		NumVerts = FmPolyTPages[ Count ].NumVerts;
		NumTris = FmPolyTPages[ Count ].NumTris;

		if( NumVerts )
		{
/* bjd - CHECK
		   	OP_PROCESS_VERTICES( 1, lpPointer );
		   	    PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, StartVert, NumVerts, lpPointer );
		   	OP_STATE_RENDER( 1, lpPointer );
		   	    STATE_DATA( D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Count ], lpPointer );
		   	OP_TRIANGLE_LIST( NumTris, lpPointer );
*/
	   		//FmPolyFacePnt = (LPTRIANGLE) lpPointer;

			if( Count == *TPage ) i = *NextFmPoly;
			else i = FmPolyTPages[ Count ].FirstPoly;

			while( ( i != (u_int16_t) -1 ) && ( StartVert < MAXFMPOLYVERTS ) )
			{
				if( ( FmPolys[i].Flags & FM_FLAG_DONTCLIP ) )
				{
					if( FmPolys[i].Frm_Info && (*FmPolys[i].Frm_Info ) )
					{
/*===================================================================
		Create Colour Value for polys
===================================================================*/

#if ACTUAL_TRANS
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, FmPolys[ i ].Trans );
#else
								Colour = RGBA_MAKE2( FmPolys[ i ].R, FmPolys[ i ].G, FmPolys[ i ].B, 255 );
#endif

/*===================================================================
		Set Dir and Up Vectors
===================================================================*/
						if( FmPolys[ i ].Flags & FM_FLAG_DIRCONST )
						{
							YVector = FmPolys[ i ].UpVector;					/* Calc Up Vector */
						   	ZVector = FmPolys[ i ].DirVector;					/* Calc Forward Vector */
						}
						else
						{
							YVector.x = (float) sin( D2R( FmPolys[ i ].Rot ) ); /* Calc Up Vector */
							YVector.y = (float) cos( D2R( FmPolys[ i ].Rot ) );
							YVector.z = 0.0F;
							ZVector = Forward;

							if( FmPolys[ i ].Flags & FM_FLAG_RELATIVE )
							{
								QuatFromDirAndUp( &FmPolys[i].DirVector, &FmPolys[i].UpVector, &TempQuat );
								QuatToMatrix( &TempQuat, &TempMatrix );
								ApplyMatrix( &TempMatrix, &YVector, &YVector );
								ApplyMatrix( &TempMatrix, &ZVector, &ZVector );
							}

							MatrixCopy = CurrentCamera.Mat;
							MatrixCopy._41 = 0.0F;
							MatrixCopy._42 = 0.0F;
							MatrixCopy._43 = 0.0F;
							ApplyMatrix( &MatrixCopy, &YVector, &YVector );		/* Calc Up Vector */
							ApplyMatrix( &MatrixCopy, &ZVector, &ZVector );		/* Calc Forward Vector */
						}

						CrossProduct( &YVector, &ZVector, &XVector );			/* Calc Left Vector */

/*===================================================================
		Setup FrameInfo description pointers
===================================================================*/
						Bit_Ptr = ( (*FmPolys[ i ].Frm_Info)->Bit_Info + (int16_t) FmPolys[ i ].Frame );
						Off_Ptr = ( (*FmPolys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );

						for( BitCount = 0; BitCount < Bit_Ptr->numbits; BitCount++ )
						{
							int ntris = 0;

							Box_Ptr = ( (*FmPolys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );

							Xoff.x = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.x );
							Xoff.y = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.y );
							Xoff.z = ( ( Off_Ptr->xoff * FmPolys[ i ].xsize ) * XVector.z );
							Yoff.x = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.x );
							Yoff.y = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.y );
							Yoff.z = ( ( -Off_Ptr->yoff * FmPolys[ i ].ysize ) * YVector.z );

							switch( Off_Ptr->box & BOX_BOTHFLIP )
							{
								case BOX_NOFLIP:
			   						Xsize.x = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;

								case BOX_XFLIP:
			   						Xsize.x = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = ( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;

								case BOX_YFLIP:
			   						Xsize.x = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = ( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;

								case BOX_BOTHFLIP:
			   						Xsize.x = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.x );
			   						Xsize.y = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.y );
			   						Xsize.z = -( ( Box_Ptr->xsize * FmPolys[ i ].xsize ) * XVector.z );
			   						Ysize.x = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.x );
			   						Ysize.y = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.y );
			   						Ysize.z = -( ( Box_Ptr->ysize * FmPolys[ i ].ysize ) * YVector.z );
									break;
							}

/*-------------- Errrrmmmmmmm ---------------*/
							if( ( FmPolys[ i ].Flags & FM_FLAG_MOVEOUT ) )
							{
								TempVector.x = ( FmPolys[ i ].Pos.x - CurrentCamera.Pos.x );
								TempVector.y = ( FmPolys[ i ].Pos.y - CurrentCamera.Pos.y );
								TempVector.z = ( FmPolys[ i ].Pos.z - CurrentCamera.Pos.z );
								Distance = VectorLength( &TempVector );
								NormaliseVector( &TempVector );

								if( Distance >= ( EXPLO_DISTANCE + 110.0F ) )
								{
									Distance = EXPLO_DISTANCE;
								}
								else
								{
									Distance -= 110.0F;
									if( Distance < 32.0F ) Distance = 32.0F;
								}

								NewPos.x = ( FmPolys[ i ].Pos.x + ( -Distance * TempVector.x ) );
								NewPos.y = ( FmPolys[ i ].Pos.y + ( -Distance * TempVector.y ) );
								NewPos.z = ( FmPolys[ i ].Pos.z + ( -Distance * TempVector.z ) );
							}
							else
							{
								NewPos = FmPolys[i].Pos;
							}
/*-------------------------------------------*/

							NewPos.x += ( Xoff.x + Yoff.x );
							NewPos.y += ( Xoff.y + Yoff.y );
							NewPos.z += ( Xoff.z + Yoff.z );

							FmPolyVertPnt->x = NewPos.x;						// v1
							FmPolyVertPnt->y = NewPos.y;
							FmPolyVertPnt->z = NewPos.z;
							FmPolyVertPnt->tu = Box_Ptr->u1;
							FmPolyVertPnt->tv = Box_Ptr->v1;
							FmPolyVertPnt->color = Colour;
//							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;

							FmPolyVertPnt->x = ( NewPos.x + Xsize.x );			// v2
							FmPolyVertPnt->y = ( NewPos.y + Xsize.y );
							FmPolyVertPnt->z = ( NewPos.z + Xsize.z );
							FmPolyVertPnt->tu = Box_Ptr->u2;
							FmPolyVertPnt->tv = Box_Ptr->v1;
							FmPolyVertPnt->color = Colour;
//							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;

							FmPolyVertPnt->x = ( NewPos.x + Xsize.x - Ysize.x ); // v3
							FmPolyVertPnt->y = ( NewPos.y + Xsize.y - Ysize.y );
							FmPolyVertPnt->z = ( NewPos.z + Xsize.z - Ysize.z );
							FmPolyVertPnt->tu = Box_Ptr->u2;
							FmPolyVertPnt->tv = Box_Ptr->v2;
							FmPolyVertPnt->color = Colour;
//							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;

							FmPolyVertPnt->x = ( NewPos.x - Ysize.x );			// v4
							FmPolyVertPnt->y = ( NewPos.y - Ysize.y );
							FmPolyVertPnt->z = ( NewPos.z - Ysize.z );
							FmPolyVertPnt->tu = Box_Ptr->u1;
							FmPolyVertPnt->tv = Box_Ptr->v2;
							FmPolyVertPnt->color = Colour;
//							FmPolyVertPnt->dwReserved = 0;
							FmPolyVertPnt++;

					   		FmPolyFacePnt->v1 = 0;
					   		FmPolyFacePnt->v2 = 1;
					   		FmPolyFacePnt->v3 = 2;
//					   		FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
					   		FmPolyFacePnt++;
							ntris++;

					   		FmPolyFacePnt->v1 = 0;
					   		FmPolyFacePnt->v2 = 2;
					   		FmPolyFacePnt->v3 = 3;
//					   		FmPolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 );
					   		FmPolyFacePnt++;
							ntris++;

							renderObject->textureGroups[renderObject->numTextureGroups].numTriangles = ntris;
							renderObject->textureGroups[renderObject->numTextureGroups].numVerts = 4;
							renderObject->textureGroups[renderObject->numTextureGroups].startIndex = start_index;
							renderObject->textureGroups[renderObject->numTextureGroups].startVert = StartVert;
							renderObject->textureGroups[renderObject->numTextureGroups].texture = Tloadheader.lpTexture[Count];
							renderObject->textureGroups[renderObject->numTextureGroups].colourkey = Tloadheader.ColourKey[Count];
							INCREASE_TEXTURE_GROUPS(renderObject);

							start_index += ntris*3; // 3 indexes in a triangle
							StartVert += 4;

							Off_Ptr++;
						}
					}
				}

				i = FmPolys[ i ].NextInTPage;
			}

	   		//lpPointer = ( LPVOID ) FmPolyFacePnt;
		}

		if( StartVert >= MAXFMPOLYVERTS ) break;
	}

//	OP_EXIT( lpPointer );

/*===================================================================
		UnLock Exec Buffer and set data description
===================================================================*/

	if (!(FSUnlockVertexBuffer(renderObject)))
	{
		return false;
	}

	if (!(FSUnlockIndexBuffer(renderObject)))
	{
		Msg( "FSUnlockIndexBuffer failed");
		return false ;
	}

	*TPage = Count;
	*NextFmPoly = i;

	return( true );
}

/*===================================================================
	Procedure	:	Fade Colour to wanted colour
	Input		:	u_int8_t	*	Colour
				:	u_int8_t		Wanted Colour
	Output		:	Nothing
===================================================================*/
void FadeColour( u_int8_t * Colour, u_int8_t WantedColour, float Speed )
{
	float	WanCol, CurCol;

	WanCol = (float) WantedColour;
	CurCol = (float) *Colour;

	if( CurCol < WanCol )
	{
		CurCol += Speed;
		if( CurCol > WanCol ) CurCol = WanCol;
	}

	if( CurCol > WanCol )
	{
		CurCol -= Speed;
		if( CurCol < WanCol ) CurCol = WanCol;
	}

	*Colour = (u_int8_t) CurCol;
}

/*===================================================================
	Procedure	:	Save FmPolys Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveFmPolys( FILE * fp )
{
	u_int16_t i;
	int16_t	Frm_Info_Index;

	if( fp )
	{
		fwrite( &TotalFmPolysInUse, sizeof( u_int32_t ),1 ,fp );
		fwrite( &FirstFmPolyUsed, sizeof( FirstFmPolyUsed ), 1, fp );
		fwrite( &FirstFmPolyFree, sizeof( FirstFmPolyFree ), 1, fp );

		for( i = 0; i < ( MAXTPAGESPERTLOAD + 1 ); i++ )
		{
			fwrite( &FmPolyTPages[ i ].FirstPoly, sizeof( u_int16_t ), 1, fp );
		}

		i = FirstFmPolyUsed;

		while( i != (u_int16_t) -1 )
		{
			fwrite( &FmPolys[ i ].Next, sizeof( u_int16_t ), 1, fp );
			fwrite( &FmPolys[ i ].Prev, sizeof( u_int16_t ), 1, fp );
			fwrite( &FmPolys[ i ].NextInTPage, sizeof( u_int16_t ), 1, fp );
			fwrite( &FmPolys[ i ].PrevInTPage, sizeof( u_int16_t ), 1, fp );
			fwrite( &FmPolys[ i ].LifeCount, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &FmPolys[ i ].Start_R, sizeof( u_int8_t ), 1, fp );
			fwrite( &FmPolys[ i ].Start_G, sizeof( u_int8_t ), 1, fp );
			fwrite( &FmPolys[ i ].Start_B, sizeof( u_int8_t ), 1, fp );
			fwrite( &FmPolys[ i ].R, sizeof( u_int8_t ), 1, fp );
			fwrite( &FmPolys[ i ].G, sizeof( u_int8_t ), 1, fp );
			fwrite( &FmPolys[ i ].B, sizeof( u_int8_t ), 1, fp );
			fwrite( &FmPolys[ i ].Trans, sizeof( u_int8_t ), 1, fp );
			fwrite( &FmPolys[ i ].OnceOnly, sizeof( u_int8_t ), 1, fp );
			fwrite( &FmPolys[ i ].Rot, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].RotSpeed, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].Frame, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].AnimSpeed, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].Flags, sizeof( int16_t ), 1, fp );
			Frm_Info_Index = Get_Frm_Info_Index( FmPolys[ i ].Frm_Info );
			fwrite( &Frm_Info_Index, sizeof( int16_t ), 1, fp );
			fwrite( &FmPolys[ i ].SeqNum, sizeof( int16_t ), 1, fp );
			fwrite( &FmPolys[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fwrite( &FmPolys[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fwrite( &FmPolys[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fwrite( &FmPolys[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fwrite( &FmPolys[ i ].Group, sizeof( u_int16_t ), 1, fp );
			fwrite( &FmPolys[ i ].Speed, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].UpSpeed, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].xsize, sizeof( float ), 1, fp );
			fwrite( &FmPolys[ i ].ysize, sizeof( float ), 1, fp );
			i = FmPolys[ i ].Prev;
		}


		i = FirstFmPolyFree;

		while( i != (u_int16_t) -1 )
		{
			fwrite( &FmPolys[ i ].Next, sizeof( u_int16_t ), 1, fp );
			i = FmPolys[ i ].Next;
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load FmPolys Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadFmPolys( FILE * fp )
{
	u_int16_t i;
	int16_t	Frm_Info_Index;

	if( fp )
	{
		fread( &TotalFmPolysInUse, sizeof( u_int32_t ),1 ,fp );
		fread( &FirstFmPolyUsed, sizeof( FirstFmPolyUsed ), 1, fp );
		fread( &FirstFmPolyFree, sizeof( FirstFmPolyFree ), 1, fp );

		for( i = 0; i < ( MAXTPAGESPERTLOAD + 1 ); i++ )
		{
			fread( &FmPolyTPages[ i ].FirstPoly, sizeof( u_int16_t ), 1, fp );
		}

		i = FirstFmPolyUsed;

		while( i != (u_int16_t) -1 )
		{
			fread( &FmPolys[ i ].Next, sizeof( u_int16_t ), 1, fp );
			fread( &FmPolys[ i ].Prev, sizeof( u_int16_t ), 1, fp );
			fread( &FmPolys[ i ].NextInTPage, sizeof( u_int16_t ), 1, fp );
			fread( &FmPolys[ i ].PrevInTPage, sizeof( u_int16_t ), 1, fp );
			fread( &FmPolys[ i ].LifeCount, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &FmPolys[ i ].Start_R, sizeof( u_int8_t ), 1, fp );
			fread( &FmPolys[ i ].Start_G, sizeof( u_int8_t ), 1, fp );
			fread( &FmPolys[ i ].Start_B, sizeof( u_int8_t ), 1, fp );
			fread( &FmPolys[ i ].R, sizeof( u_int8_t ), 1, fp );
			fread( &FmPolys[ i ].G, sizeof( u_int8_t ), 1, fp );
			fread( &FmPolys[ i ].B, sizeof( u_int8_t ), 1, fp );
			fread( &FmPolys[ i ].Trans, sizeof( u_int8_t ), 1, fp );
			fread( &FmPolys[ i ].OnceOnly, sizeof( u_int8_t ), 1, fp );
			fread( &FmPolys[ i ].Rot, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].RotSpeed, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].Frame, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].AnimSpeed, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].Flags, sizeof( int16_t ), 1, fp );
			fread( &Frm_Info_Index, sizeof( int16_t ), 1, fp );
			FmPolys[ i ].Frm_Info = Get_Frm_Info_Address( Frm_Info_Index );
			fread( &FmPolys[ i ].SeqNum, sizeof( int16_t ), 1, fp );
			fread( &FmPolys[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fread( &FmPolys[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fread( &FmPolys[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fread( &FmPolys[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fread( &FmPolys[ i ].Group, sizeof( u_int16_t ), 1, fp );
			fread( &FmPolys[ i ].Speed, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].UpSpeed, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].xsize, sizeof( float ), 1, fp );
			fread( &FmPolys[ i ].ysize, sizeof( float ), 1, fp );
			i = FmPolys[ i ].Prev;
		}

		i = FirstFmPolyFree;

		while( i != (u_int16_t) -1 )
		{
			memset( &FmPolys[i], 0, sizeof( FMPOLY ) );
			FmPolys[i].xsize = ( 16.0F * GLOBAL_SCALE );
			FmPolys[i].ysize = ( 16.0F * GLOBAL_SCALE );
			FmPolys[i].R = 255;
			FmPolys[i].G = 255;
			FmPolys[i].B = 255;
#if ACTUAL_TRANS
			FmPolys[i].Trans = 128;
#else
			FmPolys[i].Trans = 255;
#endif
			FmPolys[i].DirVector.x = 0.0F;
			FmPolys[i].DirVector.y = 0.0F;
			FmPolys[i].DirVector.z = 1.0F;
			FmPolys[i].UpVector.x = 0.0F;
			FmPolys[i].UpVector.y = 1.0F;
			FmPolys[i].UpVector.z = 0.0F;
			FmPolys[i].NextInTPage = (u_int16_t) -1;
			FmPolys[i].PrevInTPage = (u_int16_t) -1;
			FmPolys[i].Prev = (u_int16_t) -1;

			fread( &FmPolys[ i ].Next, sizeof( u_int16_t ), 1, fp );
			i = FmPolys[ i ].Next;
		}
	}

	return( fp );
}


#ifdef OPT_ON
#pragma optimize( "", off )
#endif
