/*===================================================================
*	p o l y s . c
*	All routines to do with 3d user defined polygons...
===================================================================*/
#include <stdio.h>
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"

#include "2dpolys.h"
#include "polys.h"
#include "controls.h"
#include "camera.h"
#include "secondary.h"
#include "main.h"
#include "util.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	Externs
===================================================================*/
extern	CAMERA			CurrentCamera;
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	TLOADHEADER		Tloadheader;
extern	float			framelag;
extern	GLOBALSHIP		Ships[MAX_PLAYERS];
extern	FRAME_INFO	*	Laser_Header;
extern	FRAME_INFO	*	AfterBurner_Header;
extern	FRAME_INFO	*	LensFlare_Header;
extern	SHIPCONTROL		control;
extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
extern	BYTE			Current_Camera_View;
extern	MATRIX			ProjMatrix;
extern	D3DVIEWPORT		OldViewPort;
extern	D3DVIEWPORT		viewport;
extern	float			pixel_aspect_ratio;
extern	DWORD			CurrentSrcBlend;
extern	DWORD			CurrentDestBlend;
extern	DWORD			CurrentTextureBlend;
extern	int16			MakeColourMode;
extern	BYTE			GameStatus[MAX_PLAYERS];


/*===================================================================
	Globals
===================================================================*/
POLY		Polys[ MAXPOLYS ];
uint16		FirstPolyUsed;
uint16		FirstPolyFree;
uint32		TotalPolysInUse = 0;
TPAGEINFO	PolyTPages[ MAXTPAGESPERTLOAD + 1 ];

/*===================================================================
	Procedure	:	Init poly structures
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitPolys( void )
{
	int i;

	FirstPolyUsed = (uint16) -1;
	FirstPolyFree = 0;
	
	for( i = 0; i < MAXPOLYS; i++ )
	{
		memset( &Polys[ i ], 0, sizeof( POLY ) );
		Polys[i].Next = i + 1;
		Polys[i].Prev = (uint16) -1;

		Polys[i].NextInTPage = (uint16) -1;
		Polys[i].PrevInTPage = (uint16) -1;

		Polys[i].Frm_Info = NULL;
	}
	Polys[ MAXPOLYS - 1 ].Next = (uint16) -1;

	InitPolyTPages();
}

/*===================================================================
	Procedure	:	Find a free Poly and move it from the free list
				:	to the used list
	Input		:	Nothing
	Output		:	uint16	Number of the free Poly
===================================================================*/
uint16 FindFreePoly( void )
{
	uint16 i;

	i = FirstPolyFree;
	if( i == (uint16) -1 ) return i;
 
	Polys[ i ].Prev = FirstPolyUsed;

	if( FirstPolyUsed != (uint16) -1) Polys[ FirstPolyUsed ].Next = i;

	FirstPolyUsed = i;
	FirstPolyFree = Polys[i].Next;

	TotalPolysInUse++;

	return i ;
}
/*===================================================================
	Procedure	:	Kill a used Poly and move it from the used list
				:	to the free list
	Input		:	uint16		Number of Poly to free....
	Output		:	Nothing
===================================================================*/
void KillUsedPoly( uint16 i )
{
	uint16	its_prev;
	uint16	its_next;
	
	its_prev = Polys[i].Prev;
	its_next = Polys[i].Next;

	if( i == FirstPolyUsed ) FirstPolyUsed = Polys[ i ].Prev;
	if( its_prev != (uint16) -1 ) Polys[ its_prev ].Next = its_next;
	if( its_next != (uint16) -1 ) Polys[ its_next ].Prev = its_prev;

	TotalPolysInUse--;

	RemovePolyFromTPage( i, GetTPage( *Polys[i].Frm_Info, 0 ) );

	Polys[ i ].Prev = (uint16) -1;
	Polys[ i ].Next = FirstPolyFree;
	Polys[ i ].Frm_Info = NULL;
	FirstPolyFree = i;
}

/*===================================================================
	Procedure	:	Process Poly's
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
VECTOR	ShieldVerts[ 4 ] = {
			{ ( -128.0F * GLOBAL_SCALE ), ( -128.0F * GLOBAL_SCALE ), ( 300.0F * GLOBAL_SCALE ) },
			{ ( +128.0F * GLOBAL_SCALE ), ( -128.0F * GLOBAL_SCALE ), ( 300.0F * GLOBAL_SCALE ) },
			{ ( +128.0F * GLOBAL_SCALE ), ( +128.0F * GLOBAL_SCALE ), ( 300.0F * GLOBAL_SCALE ) },
			{ ( -128.0F * GLOBAL_SCALE ), ( +128.0F * GLOBAL_SCALE ), ( 300.0F * GLOBAL_SCALE ) } };

VECTOR	NewShieldVerts[ 4 ];

void ProcessPolys( void )
{
	uint16	i;
	uint16	nextpoly;
	MATRIX	TempMat;
	MATRIX	TempInvMat;
	float	R, G, B;

	i = FirstPolyUsed;

	while( i != (uint16) -1 )
	{
		nextpoly = Polys[i].Prev;

		switch( Polys[i].SeqNum )
		{
		   	case POLY_NOTHING:								// Nothing?
				break;

			case POLY_PULSAR_TRAIL:							// Pulsar Trail?
				break;

		   	case POLY_AFTERBURNER:							// Afterburner Trail?
				break;

		   	case POLY_LASER:								// Laser?
		   		Polys[i].Frame += ( framelag / 4.0F );
		   		if( Polys[i].Frame >= (*Polys[i].Frm_Info)->Num_Frames )
		   		{
		   			Polys[i].Frame -= ( ( Polys[i].Frame / (*Polys[i].Frm_Info)->Num_Frames )
										* (*Polys[i].Frm_Info)->Num_Frames );
		   		}
				break;

		   	case POLY_POWERLASER:							// Laser?
		   		Polys[i].Frame += ( framelag / Polys[i].AnimSpeed );
		   		if( Polys[i].Frame >= (float) ( (*Polys[i].Frm_Info)->Num_Frames - 2 )  )
		   		{
		   			Polys[i].Frame = ( (float) ( (*Polys[i].Frm_Info)->Num_Frames - 2 ) ) - 0.01F;
		   		}
				break;

		   	case POLY_INVBITS:								// Bits?
		   		Polys[i].Frame += ( framelag / 6.0F );
		   		if( Polys[i].Frame >= (*Polys[i].Frm_Info)->Num_Frames )
		   		{
		   			Polys[i].Frame -= ( ( Polys[i].Frame / (*Polys[i].Frm_Info)->Num_Frames )
										* (*Polys[i].Frm_Info)->Num_Frames );
				}

    			if( Polys[ i ].Qlerp.time != 1.0F )
				{				
					Polys[ i ].Qlerp.time += ( Polys[ i ].TimeStep * framelag );
					if( Polys[ i ].Qlerp.time > 1.0F ) Polys[ i ].Qlerp.time = 1.0F;

					R = ( (float) Polys[ i ].Col1.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col1.R = 0;
					else Polys[ i ].Col1.R = (uint8) R;
					R = ( (float) Polys[ i ].Col2.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col2.R = 0;
					else Polys[ i ].Col2.R = (uint8) R;
					R = ( (float) Polys[ i ].Col3.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col3.R = 0;
					else Polys[ i ].Col3.R = (uint8) R;
					R = ( (float) Polys[ i ].Col4.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col4.R = 0;
					else Polys[ i ].Col4.R = (uint8) R;
				
					G = ( (float) Polys[ i ].Col1.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col1.G = 0;
					else Polys[ i ].Col1.G = (uint8) G;
					G = ( (float) Polys[ i ].Col2.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col2.G = 0;
					else Polys[ i ].Col2.G = (uint8) G;
					G = ( (float) Polys[ i ].Col3.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col3.G = 0;
					else Polys[ i ].Col3.G = (uint8) G;
					G = ( (float) Polys[ i ].Col4.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col4.G = 0;
					else Polys[ i ].Col4.G = (uint8) G;
				
					B = ( (float) Polys[ i ].Col1.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col1.B = 0;
					else Polys[ i ].Col1.B = (uint8) B;
					B = ( (float) Polys[ i ].Col2.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col2.B = 0;
					else Polys[ i ].Col2.B = (uint8) B;
					B = ( (float) Polys[ i ].Col3.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col3.B = 0;
					else Polys[ i ].Col3.B = (uint8) B;
					B = ( (float) Polys[ i ].Col4.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col4.B = 0;
					else Polys[ i ].Col4.B = (uint8) B;

					QuatInterpolate( &Polys[ i ].Qlerp );

					QuatToMatrix( &Polys[ i ].Quat, &TempMat );
					MatrixTranspose( &TempMat, &TempInvMat );
					TempInvMat._41 = Ships[ Polys[ i ].Ship ].Object.Pos.x;
					TempInvMat._42 = Ships[ Polys[ i ].Ship ].Object.Pos.y;
					TempInvMat._43 = Ships[ Polys[ i ].Ship ].Object.Pos.z;

					NewShieldVerts[ 0 ].x = ShieldVerts[ 0 ].x * Polys[ i ].Scale;
					NewShieldVerts[ 0 ].y = ShieldVerts[ 0 ].y * Polys[ i ].Scale;
					NewShieldVerts[ 0 ].z = ShieldVerts[ 0 ].z;
					NewShieldVerts[ 1 ].x = ShieldVerts[ 1 ].x * Polys[ i ].Scale;
					NewShieldVerts[ 1 ].y = ShieldVerts[ 1 ].y * Polys[ i ].Scale;
					NewShieldVerts[ 1 ].z = ShieldVerts[ 1 ].z;
					NewShieldVerts[ 2 ].x = ShieldVerts[ 2 ].x * Polys[ i ].Scale;
					NewShieldVerts[ 2 ].y = ShieldVerts[ 2 ].y * Polys[ i ].Scale;
					NewShieldVerts[ 2 ].z = ShieldVerts[ 2 ].z;
					NewShieldVerts[ 3 ].x = ShieldVerts[ 3 ].x * Polys[ i ].Scale;
					NewShieldVerts[ 3 ].y = ShieldVerts[ 3 ].y * Polys[ i ].Scale;
					NewShieldVerts[ 3 ].z = ShieldVerts[ 3 ].z;

					VisPolyApplyMatrix( &TempInvMat, &NewShieldVerts[ 0 ], &Polys[ i ].Pos1 );
					VisPolyApplyMatrix( &TempInvMat, &NewShieldVerts[ 1 ], &Polys[ i ].Pos2 );
					VisPolyApplyMatrix( &TempInvMat, &NewShieldVerts[ 2 ], &Polys[ i ].Pos3 );
					VisPolyApplyMatrix( &TempInvMat, &NewShieldVerts[ 3 ], &Polys[ i ].Pos4 );
				}
				else
				{
					KillUsedPoly( i );
				}
				break;

		   	case POLY_SHIELD:								// Ship Shield Effect>
    			if( Polys[ i ].Qlerp.time != 1.0F )
				{				
					Polys[ i ].Qlerp.time += ( Polys[ i ].TimeStep * framelag );
					if( Polys[ i ].Qlerp.time > 1.0F ) Polys[ i ].Qlerp.time = 1.0F;

					R = ( (float) Polys[ i ].Col1.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col1.R = 0;
					else Polys[ i ].Col1.R = (uint8) R;
					R = ( (float) Polys[ i ].Col2.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col2.R = 0;
					else Polys[ i ].Col2.R = (uint8) R;
					R = ( (float) Polys[ i ].Col3.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col3.R = 0;
					else Polys[ i ].Col3.R = (uint8) R;
					R = ( (float) Polys[ i ].Col4.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col4.R = 0;
					else Polys[ i ].Col4.R = (uint8) R;
				
					G = ( (float) Polys[ i ].Col1.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col1.G = 0;
					else Polys[ i ].Col1.G = (uint8) G;
					G = ( (float) Polys[ i ].Col2.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col2.G = 0;
					else Polys[ i ].Col2.G = (uint8) G;
					G = ( (float) Polys[ i ].Col3.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col3.G = 0;
					else Polys[ i ].Col3.G = (uint8) G;
					G = ( (float) Polys[ i ].Col4.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col4.G = 0;
					else Polys[ i ].Col4.G = (uint8) G;
				
					B = ( (float) Polys[ i ].Col1.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col1.B = 0;
					else Polys[ i ].Col1.B = (uint8) B;
					B = ( (float) Polys[ i ].Col2.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col2.B = 0;
					else Polys[ i ].Col2.B = (uint8) B;
					B = ( (float) Polys[ i ].Col3.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col3.B = 0;
					else Polys[ i ].Col3.B = (uint8) B;
					B = ( (float) Polys[ i ].Col4.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col4.B = 0;
					else Polys[ i ].Col4.B = (uint8) B;

					QuatInterpolate( &Polys[ i ].Qlerp );

					QuatToMatrix( &Polys[ i ].Quat, &TempMat );
					MatrixTranspose( &TempMat, &TempInvMat );
					TempInvMat._41 = Ships[ Polys[ i ].Ship ].Object.Pos.x;
					TempInvMat._42 = Ships[ Polys[ i ].Ship ].Object.Pos.y;
					TempInvMat._43 = Ships[ Polys[ i ].Ship ].Object.Pos.z;

					NewShieldVerts[ 0 ].x = ShieldVerts[ 0 ].x * Polys[ i ].Scale;
					NewShieldVerts[ 0 ].y = ShieldVerts[ 0 ].y * Polys[ i ].Scale;
					NewShieldVerts[ 0 ].z = ShieldVerts[ 0 ].z;
					NewShieldVerts[ 1 ].x = ShieldVerts[ 1 ].x * Polys[ i ].Scale;
					NewShieldVerts[ 1 ].y = ShieldVerts[ 1 ].y * Polys[ i ].Scale;
					NewShieldVerts[ 1 ].z = ShieldVerts[ 1 ].z;
					NewShieldVerts[ 2 ].x = ShieldVerts[ 2 ].x * Polys[ i ].Scale;
					NewShieldVerts[ 2 ].y = ShieldVerts[ 2 ].y * Polys[ i ].Scale;
					NewShieldVerts[ 2 ].z = ShieldVerts[ 2 ].z;
					NewShieldVerts[ 3 ].x = ShieldVerts[ 3 ].x * Polys[ i ].Scale;
					NewShieldVerts[ 3 ].y = ShieldVerts[ 3 ].y * Polys[ i ].Scale;
					NewShieldVerts[ 3 ].z = ShieldVerts[ 3 ].z;

					VisPolyApplyMatrix( &TempInvMat, &NewShieldVerts[ 0 ], &Polys[ i ].Pos1 );
					VisPolyApplyMatrix( &TempInvMat, &NewShieldVerts[ 1 ], &Polys[ i ].Pos2 );
					VisPolyApplyMatrix( &TempInvMat, &NewShieldVerts[ 2 ], &Polys[ i ].Pos3 );
					VisPolyApplyMatrix( &TempInvMat, &NewShieldVerts[ 3 ], &Polys[ i ].Pos4 );
				}
				else
				{
					KillUsedPoly( i );
				}
				break;

			case POLY_SCATTER_TRAIL:
				break;

			default:
				break;
		}

		i = nextpoly;
	}
}

/*===================================================================
	Procedure	:	Create Afterburner Effect
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
#define	MAX_AFTERBURNER_LENGTH	( 512.0F * GLOBAL_SCALE )
#define	AFTERBURNER_INC			( 30.0F * GLOBAL_SCALE )

static	uint16	TurboIndex[ MAX_PLAYERS ];
static	float	TurboLength[ MAX_PLAYERS ];
static	float	FlameDiameter = ( 128.0F * GLOBAL_SCALE );

void InitAfterBurners( void )
{
	int16	Count;

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		TurboIndex[ Count ] = (uint16) -1;
		TurboLength[ Count ] = 0.0F;
	}
}

void DoAfterBurnerEffects( void )
{
	VECTOR	DirVector;
	VECTOR	LeftVector;
	VECTOR	UpVector;
	VECTOR	Pos;
	VECTOR	LeftDir;
	float	Angle;
	uint16	Ship;
	uint16	TI;
	float	TL;

	for( Ship = 0; Ship < MAX_PLAYERS; Ship++ )
	{
		if( ( Ships[ Ship ].enable ) &&
			( Ships[ Ship ].Object.Mode != LIMBO_MODE ) &&
			( Ships[ Ship ].Object.Mode != GAMEOVER_MODE ) &&
			( ( GameStatus[ Ship ] == STATUS_Normal ) || ( GameStatus[ Ship ] == STATUS_SinglePlayer ) ) )
		{
			if( ( Ships[ Ship ].Object.Flags & SHIP_Turbo ) || ( TurboLength[ Ship ] ) )
			{
				if( ( Ships[ Ship ].Object.Flags & SHIP_Turbo ) )
				{
					if( TurboIndex[ Ship ] == (uint16) -1 )
					{
						TurboIndex[ Ship ] = FindFreePoly();
						if( TurboIndex[ Ship ] != (uint16) -1 )
						{
							AddPolyToTPage( TurboIndex[ Ship ], GetTPage( AfterBurner_Header, 0 ) );
						}
					}
		
					if( TurboLength[ Ship ] != MAX_AFTERBURNER_LENGTH )
					{
						TurboLength[ Ship ] += AFTERBURNER_INC;
						if( TurboLength[ Ship ] > MAX_AFTERBURNER_LENGTH ) TurboLength[ Ship ] = MAX_AFTERBURNER_LENGTH;
					}
				}
				else
				{
					if( TurboLength[ Ship ] > 0.0F )
					{
						TurboLength[ Ship ] -= AFTERBURNER_INC;
						if( TurboLength[ Ship ] <= 0.0F )
						{
							TurboLength[ Ship ] = 0.0F;
		
							if( TurboIndex[ Ship ] != (uint16) -1 )
							{
								KillUsedPoly( TurboIndex[ Ship ] );
								TurboIndex[ Ship ] = (uint16) -1;
							}
						}
					}
				}
		
				if( TurboIndex[ Ship ] != (uint16) -1 )
				{
					TI = TurboIndex[ Ship ];
					TL = TurboLength[ Ship ];
		
					if( framelag ) Angle = (float) Random_Range( 360 );
					else Angle = 0.0F;
					LeftDir.x = (float) COSD( Angle );
					LeftDir.y = (float) SIND( Angle );
					LeftDir.z = (float) 0.0F;
		
					ApplyMatrix( &Ships[ Ship ].Object.FinalMat, &Forward, &DirVector );
					ApplyMatrix( &Ships[ Ship ].Object.FinalMat, &LeftDir, &LeftVector );
					ApplyMatrix( &Ships[ Ship ].Object.FinalMat, &SlideUp, &UpVector );
			
					Pos.x = ( Ships[ Ship ].Object.Pos.x + ( ( 270.0F * GLOBAL_SCALE ) * -DirVector.x ) + ( ( 26.0F * GLOBAL_SCALE ) * UpVector.x ) );
					Pos.y = ( Ships[ Ship ].Object.Pos.y + ( ( 270.0F * GLOBAL_SCALE ) * -DirVector.y ) + ( ( 26.0F * GLOBAL_SCALE ) * UpVector.y ) );
					Pos.z = ( Ships[ Ship ].Object.Pos.z + ( ( 270.0F * GLOBAL_SCALE ) * -DirVector.z ) + ( ( 26.0F * GLOBAL_SCALE ) * UpVector.z ) );
		
					Polys[ TI ].Pos1.x = Pos.x - ( LeftVector.x * FlameDiameter );
					Polys[ TI ].Pos1.y = Pos.y - ( LeftVector.y * FlameDiameter );
					Polys[ TI ].Pos1.z = Pos.z - ( LeftVector.z * FlameDiameter );
					Polys[ TI ].Pos2.x = Pos.x + ( LeftVector.x * FlameDiameter );
					Polys[ TI ].Pos2.y = Pos.y + ( LeftVector.y * FlameDiameter );
					Polys[ TI ].Pos2.z = Pos.z + ( LeftVector.z * FlameDiameter );
					Polys[ TI ].Pos3.x = Polys[ TI ].Pos2.x + ( -DirVector.x * TL );
					Polys[ TI ].Pos3.y = Polys[ TI ].Pos2.y + ( -DirVector.y * TL );
					Polys[ TI ].Pos3.z = Polys[ TI ].Pos2.z + ( -DirVector.z * TL );
					Polys[ TI ].Pos4.x = Polys[ TI ].Pos1.x + ( -DirVector.x * TL );
					Polys[ TI ].Pos4.y = Polys[ TI ].Pos1.y + ( -DirVector.y * TL );
					Polys[ TI ].Pos4.z = Polys[ TI ].Pos1.z + ( -DirVector.z * TL );
		
					Polys[ TI ].Col1.R = 255;
					Polys[ TI ].Col1.G = 255;
					Polys[ TI ].Col1.B = 255;
					Polys[ TI ].Col2.R = 255;
					Polys[ TI ].Col2.G = 255;
					Polys[ TI ].Col2.B = 255;
					Polys[ TI ].Col3.R = 255;
					Polys[ TI ].Col3.G = 255;
					Polys[ TI ].Col3.B = 255;
					Polys[ TI ].Col4.R = 255;
					Polys[ TI ].Col4.G = 255;
					Polys[ TI ].Col4.B = 255;
					Polys[ TI ].Trans1 = 255;
					Polys[ TI ].Trans2 = 255;
					Polys[ TI ].Trans3 = 255;
					Polys[ TI ].Trans4 = 255;
					Polys[ TI ].Flags = POLY_FLAG_TWOSIDED;
					Polys[ TI ].Frm_Info = &AfterBurner_Header;
					Polys[ TI ].Frame = 0.0F;
					Polys[ TI ].SeqNum = POLY_AFTERBURNER;
					Polys[ TI ].Group = Ships[ Ship ].Object.Group;

					if( framelag ) CreateNewTrail( &Pos, &DirVector, Ships[ Ship ].Object.Group );
				}
			}
		}
		else
		{
			if( TurboIndex[ Ship ] != (uint16) -1 )
			{
				KillUsedPoly( TurboIndex[ Ship ] );
				TurboIndex[ Ship ] = (uint16) -1;
				TurboLength[ Ship ] = 0.0F;
				Ships[ Ship ].Object.Flags &= ~SHIP_Turbo;
			}
		}
	}
}

/*===================================================================
	Procedure	:	Init Poly TPage Groups
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitPolyTPages( void )
{
	uint16	i;

	for( i = 0; i < MAXTPAGESPERTLOAD; i++ )
	{
		PolyTPages[ i ].FirstPoly = (uint16) -1;
	}
}

/*===================================================================
	Procedure	:	Add Poly To TPage Link List
	Input		:	uint16		Poly Index
				:	uint16		TPage to add to
	Output		:	Nothing
===================================================================*/
void AddPolyToTPage( uint16 i, int16 TPage )
{
	if( TPage == -1 ) TPage = MAXTPAGESPERTLOAD;

	Polys[ i ].PrevInTPage = (uint16) -1;
	Polys[ i ].NextInTPage = PolyTPages[ TPage ].FirstPoly;

	if( Polys[ i ].NextInTPage != (uint16) -1 )
	{
		Polys[ Polys[ i ].NextInTPage ].PrevInTPage = i;
	}

	PolyTPages[ TPage ].FirstPoly = i;
}

/*===================================================================
	Procedure	:	Remove Poly From TPage Link List
	Input		:	uint16		Poly Index
				:	uint16		TPage to add to
	Output		:	Nothing
===================================================================*/
void RemovePolyFromTPage( uint16 i, int16 TPage )
{
	if( TPage == -1 ) TPage = MAXTPAGESPERTLOAD;

	if( Polys[ i ].PrevInTPage != (uint16) -1 )
	{
		Polys[ Polys[ i ].PrevInTPage ].NextInTPage = Polys[ i ].NextInTPage;
	}
	else
	{
		PolyTPages[ TPage ].FirstPoly = Polys[ i ].NextInTPage;
	}

	if( Polys[ i ].NextInTPage != (uint16) -1 )
	{
		Polys[ Polys[ i ].NextInTPage ].PrevInTPage = Polys[ i ].PrevInTPage;
	}

	Polys[ i ].PrevInTPage = (uint16) -1;
	Polys[ i ].NextInTPage = (uint16) -1;
}


/*===================================================================
	Procedure	:	Display all group clipped Polys
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	uint16						Group
				:	LPDIRECT3DDEVICE			D3D Device
				:	LPDIRECT3DVIEWPORT			D3D ViewPort
	Output		:	True/False
===================================================================*/
BOOL DisplayGroupClippedPolys( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject, uint16 Group
								/*LPDIRECT3DDEVICE D3D_Device,*/ /*LPDIRECT3DVIEWPORT D3D_ViewPort*/ ) // bjd
{
	int16	TPage;
	uint16	i;

	TPage = 0;
	i = PolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !PolyDispGroupClipped( Group, /*ExecBuff*/renderObject, &TPage, &i ) )
			return( TRUE );

//		if( D3D_Device->lpVtbl->Execute( D3D_Device, ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//		if (FSExecuteBuffer(ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//			return FALSE;

		cull_none();

		if (FAILED(draw_object(renderObject)))
			return FALSE;

		reset_cull();
	}
	return( FALSE );
}

/*===================================================================
	Procedure	:	Display all non group clipped Polys
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	LPDIRECT3DDEVICE			D3D Device
				:	LPDIRECT3DVIEWPORT			D3D ViewPort
	Output		:	True/False
===================================================================*/
BOOL DisplayGroupUnclippedPolys( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject
								   /*LPDIRECT3DDEVICE D3D_Device,*/ /*LPDIRECT3DVIEWPORT D3D_ViewPort*/ ) // bjd
{
	int16	TPage;
	uint16	i;

	TPage = 0;
	i = PolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !PolyDispGroupUnclipped( /*ExecBuff*/renderObject, &TPage, &i ) )
			return( TRUE );

			//if( D3D_Device->lpVtbl->Execute( D3D_Device, ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//			if (FSExecuteBuffer(ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//				return FALSE;

		cull_none();

		if (FAILED(draw_object(renderObject)))
			return FALSE;

		reset_cull();
	}

	return( FALSE );
}

/*===================================================================
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	uint16						Group
				:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16	*					Current TPage List
				:	uint16	*					Current Poly
	Output		:	True/False
===================================================================*/

BOOL PolyDispGroupClipped( uint16 Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextPoly )
{
	uint16			i;
	int16			Count;
	int16			TotalVerts;
	int16			StartVert;
	int16			NumVerts;
	int16			NumTris;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	D3DCOLOR		Colour;
	D3DCOLOR		Specular;
//	D3DEXECUTEBUFFERDESC ExecBuffer_debdesc;
//	D3DEXECUTEDATA	ExecBuffer_d3dexdata;
	LPD3DLVERTEX	PolyVertPnt;
	LPD3DTRIANGLE	PolyFacePnt;
    LPD3DLVERTEX	lpBufStart;//, lpInsStart, lpPointer;
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

		if( Count == *TPage ) i = *NextPoly;
		else i = PolyTPages[ Count ].FirstPoly;

		while( ( i != (uint16) -1 ) && ( ( StartVert + NumVerts ) < MAXPOLYVERTS ) )
		{
			if( !( Polys[i].Flags & POLY_FLAG_SOLID ) )
			{
				if( !( Polys[i].Flags & POLY_FLAG_DONTCLIP ) && ( Polys[i].Group == Group ) )
				{
					if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
					{
						Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16) Polys[ i ].Frame );
						NumTris += ( 2 * Bit_Ptr->numbits );
						NumVerts += ( 4 * Bit_Ptr->numbits );
					}
				}
			}
				
			i = Polys[ i ].NextInTPage;
		}

		PolyTPages[ Count ].StartVert = StartVert;
		PolyTPages[ Count ].NumVerts = NumVerts;
		PolyTPages[ Count ].NumTris = NumTris;

		TotalVerts = ( StartVert + NumVerts );
		if( TotalVerts >= MAXPOLYVERTS ) break;
	}

	if( !TotalVerts ) return( FALSE );

	Specular = RGB_MAKE( 255, 255, 255 );
	/* bjd curr driver = 0 use to be software mode
	if(d3dappi.CurrDriver != 0)	Specular = RGB_MAKE( 255, 255, 255 );
	else Specular = RGB_MAKE( 128, 128, 128 );
	*/

	renderObject->material = Tloadheader.lpMat[ *TPage ];
	renderObject->numTextureGroups = 0;

/*===================================================================
		Lock Exec Buffer and get ready to fill in...
===================================================================*/

/*
	memset( &ExecBuffer_debdesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	ExecBuffer_debdesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/		
//	if( ExecBuffer->lpVtbl->Lock( ExecBuffer, &ExecBuffer_debdesc) != D3D_OK ) return( FALSE );
//	if (FSLockExecuteBuffer(ExecBuffer, &ExecBuffer_debdesc) != D3D_OK )
//		return FALSE;
	if (FAILED(FSLockVertexBuffer(renderObject, &lpBufStart)))
	{
		return FALSE;
	}
		
	
	if (FAILED(FSLockIndexBuffer(renderObject, &lpIndices)))
	{
		return FALSE;
	}

	PolyFacePnt = (LPD3DTRIANGLE) lpIndices;

//	lpBufStart = ExecBuffer_debdesc.lpData;
	PolyVertPnt = (LPD3DLVERTEX) lpBufStart;
	//lpPointer = (LPVOID) ( PolyVertPnt + TotalVerts );
	//lpInsStart = lpPointer;

/*===================================================================
		Fill in Exec Buffer ( Verts and Faces Simultaneously )
===================================================================*/
	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = PolyTPages[ Count ].StartVert;
		NumVerts = PolyTPages[ Count ].NumVerts;
		NumTris = PolyTPages[ Count ].NumTris;

		if( NumVerts )
		{
/* bjd - TODO
		   	OP_STATE_LIGHT( 1, lpPointer );
		   	    STATE_DATA( D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[ Count ], lpPointer );
		   	OP_PROCESS_VERTICES( 1, lpPointer );
		   	    PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, StartVert, NumVerts, lpPointer );
		   	OP_STATE_RENDER( 1, lpPointer );
		   	    STATE_DATA( D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Count ], lpPointer );
		   	OP_TRIANGLE_LIST( NumTris, lpPointer );
*/	 
	   		//PolyFacePnt = (LPD3DTRIANGLE) lpPointer;
			
			if( Count == *TPage ) i = *NextPoly;
			else i = PolyTPages[ Count ].FirstPoly;
	
			while( ( i != (uint16) -1 ) && ( StartVert < MAXPOLYVERTS ) )
			{
				if( !( Polys[i].Flags & POLY_FLAG_SOLID ) )
				{
					if( !( Polys[i].Flags & POLY_FLAG_DONTCLIP ) && ( Polys[i].Group == Group ) )
					{
						if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
						{
							int ntris = 0;

		   					Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16) Polys[ i ].Frame );
		   					Off_Ptr = ( (*Polys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
		   					Box_Ptr = ( (*Polys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );

							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 / 2 );
									break;

								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos1.x;
		   					PolyVertPnt->y = Polys[ i ].Pos1.y;
		   					PolyVertPnt->z = Polys[ i ].Pos1.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   							
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos2.x;
		   					PolyVertPnt->y = Polys[ i ].Pos2.y;
		   					PolyVertPnt->z = Polys[ i ].Pos2.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   							
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos3.x;
		   					PolyVertPnt->y = Polys[ i ].Pos3.y;
		   					PolyVertPnt->z = Polys[ i ].Pos3.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   					
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
	   		
		   					PolyVertPnt->x = Polys[ i ].Pos4.x;
		   					PolyVertPnt->y = Polys[ i ].Pos4.y;
		   					PolyVertPnt->z = Polys[ i ].Pos4.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   		
				   			PolyFacePnt->v1 = 0;
				   			PolyFacePnt->v2 = 1;
				   			PolyFacePnt->v3 = 2;
//				   			PolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
				   			PolyFacePnt++;
							ntris++;

				   			PolyFacePnt->v1 = 0;
				   			PolyFacePnt->v2 = 2;
				   			PolyFacePnt->v3 = 3;
//				   			PolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 );
				   			PolyFacePnt++;
							ntris++;

							renderObject->textureGroups[renderObject->numTextureGroups].numTriangles = ntris;
							renderObject->textureGroups[renderObject->numTextureGroups].numVerts = 4;
							renderObject->textureGroups[renderObject->numTextureGroups].startIndex = start_index;
							renderObject->textureGroups[renderObject->numTextureGroups].startVert = StartVert;
							renderObject->textureGroups[renderObject->numTextureGroups].texture = Tloadheader.lpTexture[Count];
							renderObject->textureGroups[renderObject->numTextureGroups].colourkey = Tloadheader.ColourKey[Count];
							INCREASE_TEXTURE_GROUPS(renderObject);

							start_index += ntris*3;
							StartVert += 4;
							Off_Ptr++;
						}
					}
				}
		
				i = Polys[ i ].NextInTPage;
			}

//	   		lpPointer = ( LPVOID ) PolyFacePnt;
		}

		if( StartVert >= MAXPOLYVERTS ) break;
	}

//	OP_EXIT( lpPointer );

/*===================================================================
		UnLock Exec Buffer and set data description
===================================================================*/
//	if( ExecBuffer->lpVtbl->Unlock( ExecBuffer ) != D3D_OK ) return( FALSE );
	if (FAILED(FSUnlockVertexBuffer(renderObject)))
	{
		return FALSE;
	}

	if (FAILED(FSUnlockIndexBuffer(renderObject)))
	{
		Msg( "FSUnlockIndexBuffer failed");
		return FALSE ;
	}

//	renderObject->numVerts = TotalVerts;
//	renderObject->texture = 0;
/*
	memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
	ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
	ExecBuffer_d3dexdata.dwVertexCount = TotalVerts;
	ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
	ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
	if( ( ExecBuffer->lpVtbl->SetExecuteData( ExecBuffer, &ExecBuffer_d3dexdata ) ) != D3D_OK) return( FALSE );
*/
	*TPage = Count;
	*NextPoly = i;

	return( TRUE );
}

/*===================================================================
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16	*					Current TPage List
				:	uint16	*					Current Poly
	Output		:	True/False
===================================================================*/
BOOL PolyDispGroupUnclipped( /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextPoly )
{
	uint16			i;
	int16			Count;
	int16			TotalVerts;
	int16			StartVert;
	int16			NumVerts;
	int16			NumTris;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	D3DCOLOR		Colour;
	D3DCOLOR		Specular;
//	D3DEXECUTEBUFFERDESC ExecBuffer_debdesc;
//	D3DEXECUTEDATA	ExecBuffer_d3dexdata;
	LPD3DLVERTEX	PolyVertPnt;
	LPD3DTRIANGLE	PolyFacePnt;
    LPD3DLVERTEX	lpBufStart; //, lpInsStart, lpPointer;
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

		if( Count == *TPage ) i = *NextPoly;
		else i = PolyTPages[ Count ].FirstPoly;

		while( ( i != (uint16) -1 ) && ( ( StartVert + NumVerts ) < MAXPOLYVERTS ) )
		{
			if( !( Polys[i].Flags & POLY_FLAG_SOLID ) )
			{
				if( Polys[i].Flags & POLY_FLAG_DONTCLIP )
				{
					if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
					{
						Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16) Polys[ i ].Frame );
						NumTris += ( 2 * Bit_Ptr->numbits );
						NumVerts += ( 4 * Bit_Ptr->numbits );
					}
				}
			}
			
			i = Polys[ i ].NextInTPage;
		}

		PolyTPages[ Count ].StartVert = StartVert;
		PolyTPages[ Count ].NumVerts = NumVerts;
		PolyTPages[ Count ].NumTris = NumTris;

		TotalVerts = ( StartVert + NumVerts );
		if( TotalVerts >= MAXPOLYVERTS ) break;
	}

	if( !TotalVerts ) return( FALSE );

	Specular = RGB_MAKE( 255, 255, 255 );
	/* bjd curr driver = 0 use to be software mode
	if(d3dappi.CurrDriver != 0)	Specular = RGB_MAKE( 255, 255, 255 );
	else Specular = RGB_MAKE( 128, 128, 128 );
	*/

	renderObject->material = Tloadheader.lpMat[ *TPage ];
	renderObject->numTextureGroups = 0;

/*===================================================================
		Lock Exec Buffer and get ready to fill in...
===================================================================*/
/*
	memset( &ExecBuffer_debdesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	ExecBuffer_debdesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/		
//	if( ExecBuffer->lpVtbl->Lock( ExecBuffer, &ExecBuffer_debdesc) != D3D_OK ) return( FALSE ); // bjd
//	if (FSLockExecuteBuffer(ExecBuffer, &ExecBuffer_debdesc) != D3D_OK )
//		return FALSE;
	if (FAILED(FSLockVertexBuffer(renderObject, &lpBufStart)))
	{
		return FALSE;
	}
	
	if (FAILED(FSLockIndexBuffer(renderObject, &lpIndices)))
	{
		return FALSE;
	}

	PolyFacePnt = (LPD3DTRIANGLE) lpIndices;
		
//	lpBufStart = ExecBuffer_debdesc.lpData;
	PolyVertPnt = (LPD3DLVERTEX) lpBufStart;
	//lpPointer = (LPVOID) ( PolyVertPnt + TotalVerts );
	//lpInsStart = lpPointer;

/*===================================================================
		Fill in Exec Buffer ( Verts and Faces Simultaneously )
===================================================================*/
	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = PolyTPages[ Count ].StartVert;
		NumVerts = PolyTPages[ Count ].NumVerts;
		NumTris = PolyTPages[ Count ].NumTris;

		if( NumVerts )
		{
/* bjd - TODO
		   	OP_STATE_LIGHT( 1, lpPointer );
		   	    STATE_DATA( D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[ Count ], lpPointer );
		   	OP_PROCESS_VERTICES( 1, lpPointer );
		   	    PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, StartVert, NumVerts, lpPointer );
		   	OP_STATE_RENDER( 1, lpPointer );
		   	    STATE_DATA( D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Count ], lpPointer );
		   	OP_TRIANGLE_LIST( NumTris, lpPointer );
*/
//	   		PolyFacePnt = (LPD3DTRIANGLE) lpPointer;
			
			if( Count == *TPage ) i = *NextPoly;
			else i = PolyTPages[ Count ].FirstPoly;
	
			while( ( i != (uint16) -1 ) && ( StartVert < MAXPOLYVERTS ) )
			{
				if( !( Polys[i].Flags & POLY_FLAG_SOLID ) )
				{
					if( Polys[i].Flags & POLY_FLAG_DONTCLIP )
					{
						if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
						{
							int ntris = 0;

		   					Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16) Polys[ i ].Frame );
		   					Off_Ptr = ( (*Polys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
		   					Box_Ptr = ( (*Polys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );

							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos1.x;
		   					PolyVertPnt->y = Polys[ i ].Pos1.y;
		   					PolyVertPnt->z = Polys[ i ].Pos1.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;

							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos2.x;
		   					PolyVertPnt->y = Polys[ i ].Pos2.y;
		   					PolyVertPnt->z = Polys[ i ].Pos2.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   							
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos3.x;
		   					PolyVertPnt->y = Polys[ i ].Pos3.y;
		   					PolyVertPnt->z = Polys[ i ].Pos3.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   					
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
	   		
		   					PolyVertPnt->x = Polys[ i ].Pos4.x;
		   					PolyVertPnt->y = Polys[ i ].Pos4.y;
		   					PolyVertPnt->z = Polys[ i ].Pos4.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   		
				   			PolyFacePnt->v1 = 0;
				   			PolyFacePnt->v2 = 1;
				   			PolyFacePnt->v3 = 2;
//				   			PolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
				   			PolyFacePnt++;
							ntris++;

				   			PolyFacePnt->v1 = 0;
				   			PolyFacePnt->v2 = 2;
				   			PolyFacePnt->v3 = 3;
//				   			PolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 );
				   			PolyFacePnt++;
							ntris++;
				
							renderObject->textureGroups[renderObject->numTextureGroups].numTriangles = ntris;
							renderObject->textureGroups[renderObject->numTextureGroups].numVerts = 4;
							renderObject->textureGroups[renderObject->numTextureGroups].startIndex = start_index;
							renderObject->textureGroups[renderObject->numTextureGroups].startVert = StartVert;
							renderObject->textureGroups[renderObject->numTextureGroups].texture = Tloadheader.lpTexture[Count];
							renderObject->textureGroups[renderObject->numTextureGroups].colourkey = Tloadheader.ColourKey[Count];
							INCREASE_TEXTURE_GROUPS(renderObject);

							start_index += ntris*3;
							StartVert += 4;
							Off_Ptr++;
						}
					}
				}
		
				i = Polys[ i ].NextInTPage;
			}

//	   		lpPointer = ( LPVOID ) PolyFacePnt;
		}

		if( StartVert >= MAXPOLYVERTS ) break;
	}

//	OP_EXIT( lpPointer );

/*===================================================================
		UnLock Exec Buffer and set data description
===================================================================*/
//	if( ExecBuffer->lpVtbl->Unlock( ExecBuffer ) != D3D_OK ) return( FALSE );
	if (FAILED(FSUnlockVertexBuffer(renderObject)))
	{
		return FALSE;
	}	

	if (FAILED(FSUnlockIndexBuffer(renderObject)))
	{
		Msg( "FSUnlockIndexBuffer failed");
		return FALSE ;
	}

/*
	memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
	ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
	ExecBuffer_d3dexdata.dwVertexCount = TotalVerts;
	ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
	ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
	if( ( ExecBuffer->lpVtbl->SetExecuteData( ExecBuffer, &ExecBuffer_d3dexdata ) ) != D3D_OK) return( FALSE );
*/

	*TPage = Count;
	*NextPoly = i;

	return( TRUE );
}

/*===================================================================
	Procedure	:	Display all solid group clipped Polys
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	uint16						Group
				:	LPDIRECT3DDEVICE			D3D Device
				:	LPDIRECT3DVIEWPORT			D3D ViewPort
	Output		:	True/False
===================================================================*/
BOOL DisplaySolidGroupClippedPolys( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/ RENDEROBJECT *renderObject, uint16 Group
								/*LPDIRECT3DDEVICE D3D_Device,*/ /*LPDIRECT3DVIEWPORT D3D_ViewPort*/ ) // bjd
{
	int16	TPage;
	uint16	i;

	TPage = 0;
	i = PolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !SolidPolyDispGroupClipped( Group, /*ExecBuff*/renderObject, &TPage, &i ) )
			return( TRUE );

//			if( D3D_Device->lpVtbl->Execute( D3D_Device, ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//			if (FSExecuteBuffer(ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//				return FALSE;

		cull_none();

		if (FAILED(draw_object(renderObject)))
			return FALSE;

		reset_cull();
	}

	return( FALSE );
}

/*===================================================================
	Procedure	:	Display all solid non group clipped Polys
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	LPDIRECT3DDEVICE			D3D Device
				:	LPDIRECT3DVIEWPORT			D3D ViewPort
	Output		:	True/False
===================================================================*/
BOOL DisplaySolidGroupUnclippedPolys( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject
								   /*LPDIRECT3DDEVICE D3D_Device,*/ /*LPDIRECT3DVIEWPORT D3D_ViewPort*/ )
{
	int16	TPage;
	uint16	i;

	TPage = 0;
	i = PolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !SolidPolyDispGroupUnclipped( /*ExecBuff*/renderObject, &TPage, &i ) )
			return( TRUE );

		cull_none();

		if (FAILED(draw_object(renderObject)))
			return FALSE;

		reset_cull();

	}

	return( FALSE );
}

/*===================================================================
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	uint16						Group
				:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16	*					Current TPage List
				:	uint16	*					Current Poly
	Output		:	True/False
===================================================================*/
BOOL SolidPolyDispGroupClipped( uint16 Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextPoly )
{
	uint16			i;
	int16			Count;
	int16			TotalVerts;
	int16			StartVert;
	int16			NumVerts;
	int16			NumTris;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	D3DCOLOR		Colour;
	D3DCOLOR		Specular;
//	D3DEXECUTEBUFFERDESC ExecBuffer_debdesc;
//	D3DEXECUTEDATA	ExecBuffer_d3dexdata;
	LPD3DLVERTEX	PolyVertPnt;
	LPD3DTRIANGLE	PolyFacePnt;
    LPD3DLVERTEX	lpBufStart;//, lpInsStart, lpPointer;
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

		if( Count == *TPage ) i = *NextPoly;
		else i = PolyTPages[ Count ].FirstPoly;

		while( ( i != (uint16) -1 ) && ( ( StartVert + NumVerts ) < MAXPOLYVERTS ) )
		{
			if( ( Polys[i].Flags & POLY_FLAG_SOLID ) )
			{
				if( !( Polys[i].Flags & POLY_FLAG_DONTCLIP ) && ( Polys[i].Group == Group ) )
				{
					if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
					{
						Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16) Polys[ i ].Frame );
						NumTris += ( 2 * Bit_Ptr->numbits );
						NumVerts += ( 4 * Bit_Ptr->numbits );
					}
				}
			}
			
			i = Polys[ i ].NextInTPage;
		}

		PolyTPages[ Count ].StartVert = StartVert;
		PolyTPages[ Count ].NumVerts = NumVerts;
		PolyTPages[ Count ].NumTris = NumTris;

		TotalVerts = ( StartVert + NumVerts );
		if( TotalVerts >= MAXPOLYVERTS ) break;
	}

	if( !TotalVerts ) return( FALSE );

	renderObject->material = Tloadheader.lpMat[ *TPage ];
	renderObject->numTextureGroups = 0;

	Specular = RGB_MAKE( 255, 255, 255 );
	/* bjd curr driver = 0 use to be software mode
	if(d3dappi.CurrDriver != 0)	Specular = RGB_MAKE( 255, 255, 255 );
	else Specular = RGB_MAKE( 128, 128, 128 );
	*/

/*===================================================================
		Lock Exec Buffer and get ready to fill in...
===================================================================*/
/*
	memset( &ExecBuffer_debdesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	ExecBuffer_debdesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/		
//	if( ExecBuffer->lpVtbl->Lock( ExecBuffer, &ExecBuffer_debdesc) != D3D_OK ) return( FALSE );
//	if (FSLockExecuteBuffer(ExecBuffer, &ExecBuffer_debdesc) != D3D_OK )
//		return FALSE;
	if (FAILED(FSLockVertexBuffer(renderObject, &lpBufStart)))
	{
		return FALSE;
	}
			
	if (FAILED(FSLockIndexBuffer(renderObject, &lpIndices)))
	{
		return FALSE;
	}

	PolyFacePnt = (LPD3DTRIANGLE) lpIndices;

//	lpBufStart = ExecBuffer_debdesc.lpData;
	PolyVertPnt = (LPD3DLVERTEX) lpBufStart;
	//lpPointer = (LPVOID) ( PolyVertPnt + TotalVerts );
	//lpInsStart = lpPointer;

/*===================================================================
		Fill in Exec Buffer ( Verts and Faces Simultaneously )
===================================================================*/
	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = PolyTPages[ Count ].StartVert;
		NumVerts = PolyTPages[ Count ].NumVerts;
		NumTris = PolyTPages[ Count ].NumTris;

		if( NumVerts )
		{
/* bjd - TODO?
		   	OP_STATE_LIGHT( 1, lpPointer );
		   	    STATE_DATA( D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[ Count ], lpPointer );
		   	OP_PROCESS_VERTICES( 1, lpPointer );
		   	    PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, StartVert, NumVerts, lpPointer );
		   	OP_STATE_RENDER( 1, lpPointer );
		   	    STATE_DATA( D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Count ], lpPointer );
		   	OP_TRIANGLE_LIST( NumTris, lpPointer );
*/
//	   		PolyFacePnt = (LPD3DTRIANGLE) lpPointer;
			
			if( Count == *TPage ) i = *NextPoly;
			else i = PolyTPages[ Count ].FirstPoly;
	
			while( ( i != (uint16) -1 ) && ( StartVert < MAXPOLYVERTS ) )
			{
				if( ( Polys[i].Flags & POLY_FLAG_SOLID ) )
				{
					if( !( Polys[i].Flags & POLY_FLAG_DONTCLIP ) && ( Polys[i].Group == Group ) )
					{
						if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
						{
							int ntris = 0;

		   					Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16) Polys[ i ].Frame );
		   					Off_Ptr = ( (*Polys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
		   					Box_Ptr = ( (*Polys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );

							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos1.x;
		   					PolyVertPnt->y = Polys[ i ].Pos1.y;
		   					PolyVertPnt->z = Polys[ i ].Pos1.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   							
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos2.x;
		   					PolyVertPnt->y = Polys[ i ].Pos2.y;
		   					PolyVertPnt->z = Polys[ i ].Pos2.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   							
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos3.x;
		   					PolyVertPnt->y = Polys[ i ].Pos3.y;
		   					PolyVertPnt->z = Polys[ i ].Pos3.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   					
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
	   		
		   					PolyVertPnt->x = Polys[ i ].Pos4.x;
		   					PolyVertPnt->y = Polys[ i ].Pos4.y;
		   					PolyVertPnt->z = Polys[ i ].Pos4.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   		
				   			PolyFacePnt->v1 = 0;
				   			PolyFacePnt->v2 = 1;
				   			PolyFacePnt->v3 = 2;
//				   			PolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
				   			PolyFacePnt++;
							ntris++;

				   			PolyFacePnt->v1 = 0;
				   			PolyFacePnt->v2 = 2;
				   			PolyFacePnt->v3 = 3;
//				   			PolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 );
				   			PolyFacePnt++;
							ntris++;
				
							renderObject->textureGroups[renderObject->numTextureGroups].numTriangles = 0;
							renderObject->textureGroups[renderObject->numTextureGroups].numVerts = NumVerts;
							renderObject->textureGroups[renderObject->numTextureGroups].startIndex = 0;
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
		
				i = Polys[ i ].NextInTPage;
			}

	   		//lpPointer = ( LPVOID ) PolyFacePnt;
		}

		if( StartVert >= MAXPOLYVERTS ) break;
	}

//	OP_EXIT( lpPointer );

/*===================================================================
		UnLock Exec Buffer and set data description
===================================================================*/
//	if( ExecBuffer->lpVtbl->Unlock( ExecBuffer ) != D3D_OK ) return( FALSE );
	if (FAILED(FSUnlockVertexBuffer(renderObject)))
	{
		return FALSE;
	}

	if (FAILED(FSUnlockIndexBuffer(renderObject)))
	{
		Msg( "FSUnlockIndexBuffer failed");
		return FALSE ;
	}

/*
	memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
	ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
	ExecBuffer_d3dexdata.dwVertexCount = TotalVerts;
	ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
	ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
	if( ( ExecBuffer->lpVtbl->SetExecuteData( ExecBuffer, &ExecBuffer_d3dexdata ) ) != D3D_OK) return( FALSE );
*/

//FIXME	renderObject->numVerts = TotalVerts;
//	renderObject->texture = 0;

	*TPage = Count;
	*NextPoly = i;

	return( TRUE );
}

/*===================================================================
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16	*					Current TPage List
				:	uint16	*					Current Poly
	Output		:	True/False
===================================================================*/
BOOL SolidPolyDispGroupUnclipped( /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextPoly )
{
	uint16			i;
	int16			Count;
	int16			TotalVerts;
	int16			StartVert;
	int16			NumVerts;
	int16			NumTris;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	D3DCOLOR		Colour;
	D3DCOLOR		Specular;
//	D3DEXECUTEBUFFERDESC ExecBuffer_debdesc;
//	D3DEXECUTEDATA	ExecBuffer_d3dexdata;
	LPD3DLVERTEX	PolyVertPnt;
	LPD3DTRIANGLE	PolyFacePnt;
    LPD3DLVERTEX	lpBufStart;//, lpInsStart, lpPointer;
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

		if( Count == *TPage ) i = *NextPoly;
		else i = PolyTPages[ Count ].FirstPoly;

		while( ( i != (uint16) -1 ) && ( ( StartVert + NumVerts ) < MAXPOLYVERTS ) )
		{
			if( ( Polys[i].Flags & POLY_FLAG_SOLID ) )
			{
				if( Polys[i].Flags & POLY_FLAG_DONTCLIP )
				{
					if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
					{
						Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16) Polys[ i ].Frame );
						NumTris += ( 2 * Bit_Ptr->numbits );
						NumVerts += ( 4 * Bit_Ptr->numbits );
					}
				}
			}
			
			i = Polys[ i ].NextInTPage;
		}

		PolyTPages[ Count ].StartVert = StartVert;
		PolyTPages[ Count ].NumVerts = NumVerts;
		PolyTPages[ Count ].NumTris = NumTris;

		TotalVerts = ( StartVert + NumVerts );
		if( TotalVerts >= MAXPOLYVERTS ) break;
	}

	if( !TotalVerts ) return( FALSE );

	renderObject->material = Tloadheader.lpMat[ *TPage ];
	renderObject->numTextureGroups = 0;

	/* bjd curr driver = 0 use to be software mode
	if(d3dappi.CurrDriver != 0)	Specular = RGB_MAKE( 255, 255, 255 );
	else Specular = RGB_MAKE( 128, 128, 128 );
	*/
	Specular = RGB_MAKE( 255, 255, 255 );

/*===================================================================
		Lock Exec Buffer and get ready to fill in...
===================================================================*/
/*
	memset( &ExecBuffer_debdesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
	ExecBuffer_debdesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
*/		
//	if( ExecBuffer->lpVtbl->Lock( ExecBuffer, &ExecBuffer_debdesc) != D3D_OK ) return( FALSE );
//	if (FSLockExecuteBuffer(ExecBuffer, &ExecBuffer_debdesc) != D3D_OK )
//		return FALSE;
	if (FAILED(FSLockVertexBuffer(renderObject, &lpBufStart)))
	{
		return FALSE;
	}
		
	if (FAILED(FSLockIndexBuffer(renderObject, &lpIndices)))
	{
		return FALSE;
	}
	
	PolyFacePnt = (LPD3DTRIANGLE) lpIndices;

//	lpBufStart = ExecBuffer_debdesc.lpData;
	PolyVertPnt = (LPD3DLVERTEX) lpBufStart;
	//lpPointer = (LPVOID) ( PolyVertPnt + TotalVerts );
	//lpInsStart = lpPointer;

/*===================================================================
		Fill in Exec Buffer ( Verts and Faces Simultaneously )
===================================================================*/
	for( Count = *TPage; Count < MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = PolyTPages[ Count ].StartVert;
		NumVerts = PolyTPages[ Count ].NumVerts;
		NumTris = PolyTPages[ Count ].NumTris;

		if( NumVerts )
		{
/* bjd - TODO?
		   	OP_STATE_LIGHT( 1, lpPointer );
		   	    STATE_DATA( D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[ Count ], lpPointer );
		   	OP_PROCESS_VERTICES( 1, lpPointer );
		   	    PROCESSVERTICES_DATA( D3DPROCESSVERTICES_TRANSFORM, StartVert, NumVerts, lpPointer );
		   	OP_STATE_RENDER( 1, lpPointer );
		   	    STATE_DATA( D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Count ], lpPointer );
		   	OP_TRIANGLE_LIST( NumTris, lpPointer );
*/
//	   		PolyFacePnt = (LPD3DTRIANGLE) lpPointer;
			
			if( Count == *TPage ) i = *NextPoly;
			else i = PolyTPages[ Count ].FirstPoly;
	
			while( ( i != (uint16) -1 ) && ( StartVert < MAXPOLYVERTS ) )
			{
				if( ( Polys[i].Flags & POLY_FLAG_SOLID ) )
				{
					if( Polys[i].Flags & POLY_FLAG_DONTCLIP )
					{
						if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
						{
							int ntris = 0;

		   					Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16) Polys[ i ].Frame );
		   					Off_Ptr = ( (*Polys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
		   					Box_Ptr = ( (*Polys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );

							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos1.x;
		   					PolyVertPnt->y = Polys[ i ].Pos1.y;
		   					PolyVertPnt->z = Polys[ i ].Pos1.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   							
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos2.x;
		   					PolyVertPnt->y = Polys[ i ].Pos2.y;
		   					PolyVertPnt->z = Polys[ i ].Pos2.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   							
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
   		
		   					PolyVertPnt->x = Polys[ i ].Pos3.x;
		   					PolyVertPnt->y = Polys[ i ].Pos3.y;
		   					PolyVertPnt->z = Polys[ i ].Pos3.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   					
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, 255 );
#endif
									break;
			
								case MCM_Stipple:
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 / 2 );
									break;
				
								case MCM_Software:
									Colour = RGBA_MAKE( 128, 128, 128, 255 );
									break;
							}
	   		
		   					PolyVertPnt->x = Polys[ i ].Pos4.x;
		   					PolyVertPnt->y = Polys[ i ].Pos4.y;
		   					PolyVertPnt->z = Polys[ i ].Pos4.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
		   					PolyVertPnt->specular = Specular;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   		
				   			PolyFacePnt->v1 = 0;
				   			PolyFacePnt->v2 = 1;
				   			PolyFacePnt->v3 = 2;
//				   			PolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE1 | D3DTRIFLAG_EDGEENABLE2 );
				   			PolyFacePnt++;
							ntris++;

				   			PolyFacePnt->v1 = 0;
				   			PolyFacePnt->v2 = 2;
				   			PolyFacePnt->v3 = 3;
//				   			PolyFacePnt->wFlags = ( D3DTRIFLAG_EDGEENABLE2 | D3DTRIFLAG_EDGEENABLE3 );
				   			PolyFacePnt++;
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
				i = Polys[ i ].NextInTPage;
			}

	   		//lpPointer = ( LPVOID ) PolyFacePnt;
		}

		if( StartVert >= MAXPOLYVERTS ) break;
	}

//	OP_EXIT( lpPointer );

/*===================================================================
		UnLock Exec Buffer and set data description
===================================================================*/
//	if( ExecBuffer->lpVtbl->Unlock( ExecBuffer ) != D3D_OK ) return( FALSE );
	if (FAILED(FSUnlockVertexBuffer(renderObject)))
	{
		return FALSE;
	}

	if (FAILED(FSUnlockIndexBuffer(renderObject)))
	{
		Msg( "FSUnlockIndexBuffer failed");
		return FALSE ;
	}
/*
	memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
	ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
	ExecBuffer_d3dexdata.dwVertexCount = TotalVerts;
	ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
	ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
	if( ( ExecBuffer->lpVtbl->SetExecuteData( ExecBuffer, &ExecBuffer_d3dexdata ) ) != D3D_OK) return( FALSE );
*/
//FIXME	renderObject->numVerts = TotalVerts;
//	renderObject->texture = 0;

	*TPage = Count;
	*NextPoly = i;

	return( TRUE );
}

/*===================================================================
	Procedure	:	Save Polys Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SavePolys( FILE * fp )
{
	uint16	i;
	int16	Frm_Info_Index;

	if( fp )
	{
		fwrite( &TotalPolysInUse, sizeof( uint32 ),1 ,fp );
		fwrite( &FirstPolyUsed, sizeof( FirstPolyUsed ), 1, fp );
		fwrite( &FirstPolyFree, sizeof( FirstPolyFree ), 1, fp );
		
		for( i = 0; i < ( MAXTPAGESPERTLOAD + 1 ); i++ )
		{
			fwrite( &PolyTPages[ i ].FirstPoly, sizeof( uint16 ), 1, fp );
		}

		i = FirstPolyUsed;

		while( i != (uint16) -1 )
		{
			fwrite( &Polys[ i ].Next, sizeof( uint16 ), 1, fp );
			fwrite( &Polys[ i ].Prev, sizeof( uint16 ), 1, fp );
			fwrite( &Polys[ i ].NextInTPage, sizeof( uint16 ), 1, fp );
			fwrite( &Polys[ i ].PrevInTPage, sizeof( uint16 ), 1, fp );
			fwrite( &Polys[ i ].Pos1, sizeof( VECTOR ), 1, fp );
			fwrite( &Polys[ i ].Pos2, sizeof( VECTOR ), 1, fp );
			fwrite( &Polys[ i ].Pos3, sizeof( VECTOR ), 1, fp );
			fwrite( &Polys[ i ].Pos4, sizeof( VECTOR ), 1, fp );
			fwrite( &Polys[ i ].Col1, sizeof( POLY_RGB ), 1, fp );
			fwrite( &Polys[ i ].Col2, sizeof( POLY_RGB ), 1, fp );
			fwrite( &Polys[ i ].Col3, sizeof( POLY_RGB ), 1, fp );
			fwrite( &Polys[ i ].Col4, sizeof( POLY_RGB ), 1, fp );
			fwrite( &Polys[ i ].Trans1, sizeof( uint8 ), 1, fp );
			fwrite( &Polys[ i ].Trans2, sizeof( uint8 ), 1, fp );
			fwrite( &Polys[ i ].Trans3, sizeof( uint8 ), 1, fp );
			fwrite( &Polys[ i ].Trans4, sizeof( uint8 ), 1, fp );
			fwrite( &Polys[ i ].Flags, sizeof( int16 ), 1, fp );
			fwrite( &Polys[ i ].Scale, sizeof( float ), 1, fp );
			fwrite( &Polys[ i ].Frame, sizeof( float ), 1, fp );
			fwrite( &Polys[ i ].AnimSpeed, sizeof( float ), 1, fp );
			Frm_Info_Index = Get_Frm_Info_Index( Polys[ i ].Frm_Info );
			fwrite( &Frm_Info_Index, sizeof( int16 ), 1, fp );
			fwrite( &Polys[ i ].SeqNum, sizeof( int16 ), 1, fp );
			fwrite( &Polys[ i ].Group, sizeof( uint16 ), 1, fp );
			fwrite( &Polys[ i ].TimeStep, sizeof( float ), 1, fp );
			fwrite( &Polys[ i ].Qlerp, sizeof( QUATLERP ), 1, fp );
			fwrite( &Polys[ i ].Quat, sizeof( QUAT ), 1, fp );
			fwrite( &Polys[ i ].Ship, sizeof( uint16 ), 1, fp );
			i = Polys[ i ].Prev;
		}

		i = FirstPolyFree;

		while( i != (uint16) -1 )
		{
			fwrite( &Polys[ i ].Next, sizeof( uint16 ), 1, fp );
			i = Polys[ i ].Next;
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load Polys Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadPolys( FILE * fp )
{
	uint16	i;
	int16	Frm_Info_Index;

	if( fp )
	{
		fread( &TotalPolysInUse, sizeof( uint32 ),1 ,fp );
		fread( &FirstPolyUsed, sizeof( FirstPolyUsed ), 1, fp );
		fread( &FirstPolyFree, sizeof( FirstPolyFree ), 1, fp );
		
		for( i = 0; i < ( MAXTPAGESPERTLOAD + 1 ); i++ )
		{
			fread( &PolyTPages[ i ].FirstPoly, sizeof( uint16 ), 1, fp );
		}

		i = FirstPolyUsed;

		while( i != (uint16) -1 )
		{
			fread( &Polys[ i ].Next, sizeof( uint16 ), 1, fp );
			fread( &Polys[ i ].Prev, sizeof( uint16 ), 1, fp );
			fread( &Polys[ i ].NextInTPage, sizeof( uint16 ), 1, fp );
			fread( &Polys[ i ].PrevInTPage, sizeof( uint16 ), 1, fp );
			fread( &Polys[ i ].Pos1, sizeof( VECTOR ), 1, fp );
			fread( &Polys[ i ].Pos2, sizeof( VECTOR ), 1, fp );
			fread( &Polys[ i ].Pos3, sizeof( VECTOR ), 1, fp );
			fread( &Polys[ i ].Pos4, sizeof( VECTOR ), 1, fp );
			fread( &Polys[ i ].Col1, sizeof( POLY_RGB ), 1, fp );
			fread( &Polys[ i ].Col2, sizeof( POLY_RGB ), 1, fp );
			fread( &Polys[ i ].Col3, sizeof( POLY_RGB ), 1, fp );
			fread( &Polys[ i ].Col4, sizeof( POLY_RGB ), 1, fp );
			fread( &Polys[ i ].Trans1, sizeof( uint8 ), 1, fp );
			fread( &Polys[ i ].Trans2, sizeof( uint8 ), 1, fp );
			fread( &Polys[ i ].Trans3, sizeof( uint8 ), 1, fp );
			fread( &Polys[ i ].Trans4, sizeof( uint8 ), 1, fp );
			fread( &Polys[ i ].Flags, sizeof( int16 ), 1, fp );
			fread( &Polys[ i ].Scale, sizeof( float ), 1, fp );
			fread( &Polys[ i ].Frame, sizeof( float ), 1, fp );
			fread( &Polys[ i ].AnimSpeed, sizeof( float ), 1, fp );
			fread( &Frm_Info_Index, sizeof( int16 ), 1, fp );
			Polys[ i ].Frm_Info = Get_Frm_Info_Address( Frm_Info_Index );
			fread( &Polys[ i ].SeqNum, sizeof( int16 ), 1, fp );
			fread( &Polys[ i ].Group, sizeof( uint16 ), 1, fp );
			fread( &Polys[ i ].TimeStep, sizeof( float ), 1, fp );
			fread( &Polys[ i ].Qlerp, sizeof( QUATLERP ), 1, fp );
			Polys[ i ].Qlerp.crnt = &Polys[ i ].Quat;
			fread( &Polys[ i ].Quat, sizeof( QUAT ), 1, fp );
			fread( &Polys[ i ].Ship, sizeof( uint16 ), 1, fp );
			i = Polys[ i ].Prev;
		}

		i = FirstPolyFree;

		while( i != (uint16) -1 )
		{
			memset( &Polys[ i ], 0, sizeof( POLY ) );
			Polys[i].Prev = (uint16) -1;
			Polys[i].NextInTPage = (uint16) -1;
			Polys[i].PrevInTPage = (uint16) -1;
			Polys[i].Frm_Info = NULL;

			fread( &Polys[ i ].Next, sizeof( uint16 ), 1, fp );
			i = Polys[ i ].Next;
		}
	}

	return( fp );
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif
