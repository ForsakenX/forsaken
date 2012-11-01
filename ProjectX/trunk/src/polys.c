/*===================================================================
*	p o l y s . c
*	All routines to do with 3d user defined polygons...
===================================================================*/
#include <stdio.h>

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
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
extern	MATRIX			ProjMatrix;
extern	DWORD			CurrentSrcBlend;
extern	DWORD			CurrentDestBlend;
extern	DWORD			CurrentTextureBlend;
extern	BYTE			GameStatus[MAX_PLAYERS];


/*===================================================================
	Globals
===================================================================*/
POLY		Polys[ MAXPOLYS ];
u_int16_t		FirstPolyUsed;
u_int16_t		FirstPolyFree;
u_int32_t		TotalPolysInUse = 0;
TPAGEINFO	PolyTPages[ MAXTPAGESPERTLOAD + 1 ];

/*===================================================================
	Procedure	:	Init poly structures
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitPolys( void )
{
	int i;

	FirstPolyUsed = (u_int16_t) -1;
	FirstPolyFree = 0;
	
	for( i = 0; i < MAXPOLYS; i++ )
	{
		memset( &Polys[ i ], 0, sizeof( POLY ) );
		Polys[i].Next = i + 1;
		Polys[i].Prev = (u_int16_t) -1;

		Polys[i].NextInTPage = (u_int16_t) -1;
		Polys[i].PrevInTPage = (u_int16_t) -1;

		Polys[i].Frm_Info = NULL;
	}
	Polys[ MAXPOLYS - 1 ].Next = (u_int16_t) -1;

	InitPolyTPages();
}

/*===================================================================
	Procedure	:	Find a free Poly and move it from the free list
				:	to the used list
	Input		:	Nothing
	Output		:	u_int16_t	Number of the free Poly
===================================================================*/
u_int16_t FindFreePoly( void )
{
	u_int16_t i;

	i = FirstPolyFree;
	if( i == (u_int16_t) -1 ) return i;
 
	Polys[ i ].Prev = FirstPolyUsed;

	if( FirstPolyUsed != (u_int16_t) -1) Polys[ FirstPolyUsed ].Next = i;

	FirstPolyUsed = i;
	FirstPolyFree = Polys[i].Next;

	TotalPolysInUse++;

	return i ;
}
/*===================================================================
	Procedure	:	Kill a used Poly and move it from the used list
				:	to the free list
	Input		:	u_int16_t		Number of Poly to free....
	Output		:	Nothing
===================================================================*/
void KillUsedPoly( u_int16_t i )
{
	u_int16_t	its_prev;
	u_int16_t	its_next;
	
	its_prev = Polys[i].Prev;
	its_next = Polys[i].Next;

	if( i == FirstPolyUsed ) FirstPolyUsed = Polys[ i ].Prev;
	if( its_prev != (u_int16_t) -1 ) Polys[ its_prev ].Next = its_next;
	if( its_next != (u_int16_t) -1 ) Polys[ its_next ].Prev = its_prev;

	TotalPolysInUse--;

	RemovePolyFromTPage( i, GetTPage( *Polys[i].Frm_Info, 0 ) );

	Polys[ i ].Prev = (u_int16_t) -1;
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
	u_int16_t	i;
	u_int16_t	nextpoly;
	MATRIX	TempMat;
	MATRIX	TempInvMat;
	float	R, G, B;

	i = FirstPolyUsed;

	while( i != (u_int16_t) -1 )
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
					else Polys[ i ].Col1.R = (u_int8_t) R;
					R = ( (float) Polys[ i ].Col2.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col2.R = 0;
					else Polys[ i ].Col2.R = (u_int8_t) R;
					R = ( (float) Polys[ i ].Col3.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col3.R = 0;
					else Polys[ i ].Col3.R = (u_int8_t) R;
					R = ( (float) Polys[ i ].Col4.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col4.R = 0;
					else Polys[ i ].Col4.R = (u_int8_t) R;
				
					G = ( (float) Polys[ i ].Col1.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col1.G = 0;
					else Polys[ i ].Col1.G = (u_int8_t) G;
					G = ( (float) Polys[ i ].Col2.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col2.G = 0;
					else Polys[ i ].Col2.G = (u_int8_t) G;
					G = ( (float) Polys[ i ].Col3.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col3.G = 0;
					else Polys[ i ].Col3.G = (u_int8_t) G;
					G = ( (float) Polys[ i ].Col4.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col4.G = 0;
					else Polys[ i ].Col4.G = (u_int8_t) G;
				
					B = ( (float) Polys[ i ].Col1.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col1.B = 0;
					else Polys[ i ].Col1.B = (u_int8_t) B;
					B = ( (float) Polys[ i ].Col2.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col2.B = 0;
					else Polys[ i ].Col2.B = (u_int8_t) B;
					B = ( (float) Polys[ i ].Col3.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col3.B = 0;
					else Polys[ i ].Col3.B = (u_int8_t) B;
					B = ( (float) Polys[ i ].Col4.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col4.B = 0;
					else Polys[ i ].Col4.B = (u_int8_t) B;

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
					else Polys[ i ].Col1.R = (u_int8_t) R;
					R = ( (float) Polys[ i ].Col2.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col2.R = 0;
					else Polys[ i ].Col2.R = (u_int8_t) R;
					R = ( (float) Polys[ i ].Col3.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col3.R = 0;
					else Polys[ i ].Col3.R = (u_int8_t) R;
					R = ( (float) Polys[ i ].Col4.R ) - ( 5.0F * framelag );
					if( R < 0.0F ) Polys[ i ].Col4.R = 0;
					else Polys[ i ].Col4.R = (u_int8_t) R;
				
					G = ( (float) Polys[ i ].Col1.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col1.G = 0;
					else Polys[ i ].Col1.G = (u_int8_t) G;
					G = ( (float) Polys[ i ].Col2.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col2.G = 0;
					else Polys[ i ].Col2.G = (u_int8_t) G;
					G = ( (float) Polys[ i ].Col3.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col3.G = 0;
					else Polys[ i ].Col3.G = (u_int8_t) G;
					G = ( (float) Polys[ i ].Col4.G ) - ( 5.0F * framelag );
					if( G < 0.0F ) Polys[ i ].Col4.G = 0;
					else Polys[ i ].Col4.G = (u_int8_t) G;
				
					B = ( (float) Polys[ i ].Col1.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col1.B = 0;
					else Polys[ i ].Col1.B = (u_int8_t) B;
					B = ( (float) Polys[ i ].Col2.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col2.B = 0;
					else Polys[ i ].Col2.B = (u_int8_t) B;
					B = ( (float) Polys[ i ].Col3.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col3.B = 0;
					else Polys[ i ].Col3.B = (u_int8_t) B;
					B = ( (float) Polys[ i ].Col4.B ) - ( 5.0F * framelag );
					if( B < 0.0F ) Polys[ i ].Col4.B = 0;
					else Polys[ i ].Col4.B = (u_int8_t) B;

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

static	u_int16_t	TurboIndex[ MAX_PLAYERS ];
static	float	TurboLength[ MAX_PLAYERS ];
static	float	FlameDiameter = ( 128.0F * GLOBAL_SCALE );

void InitAfterBurners( void )
{
	int16_t	Count;

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		TurboIndex[ Count ] = (u_int16_t) -1;
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
	u_int16_t	Ship;
	u_int16_t	TI;
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
					if( TurboIndex[ Ship ] == (u_int16_t) -1 )
					{
						TurboIndex[ Ship ] = FindFreePoly();
						if( TurboIndex[ Ship ] != (u_int16_t) -1 )
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
		
							if( TurboIndex[ Ship ] != (u_int16_t) -1 )
							{
								KillUsedPoly( TurboIndex[ Ship ] );
								TurboIndex[ Ship ] = (u_int16_t) -1;
							}
						}
					}
				}
		
				if( TurboIndex[ Ship ] != (u_int16_t) -1 )
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
			if( TurboIndex[ Ship ] != (u_int16_t) -1 )
			{
				KillUsedPoly( TurboIndex[ Ship ] );
				TurboIndex[ Ship ] = (u_int16_t) -1;
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
	u_int16_t	i;

	for( i = 0; i < MAXTPAGESPERTLOAD; i++ )
	{
		PolyTPages[ i ].FirstPoly = (u_int16_t) -1;
	}
}

/*===================================================================
	Procedure	:	Add Poly To TPage Link List
	Input		:	u_int16_t		Poly Index
				:	u_int16_t		TPage to add to
	Output		:	Nothing
===================================================================*/
void AddPolyToTPage( u_int16_t i, int16_t TPage )
{
	if( TPage == -1 ) TPage = MAXTPAGESPERTLOAD;

	Polys[ i ].PrevInTPage = (u_int16_t) -1;
	Polys[ i ].NextInTPage = PolyTPages[ TPage ].FirstPoly;

	if( Polys[ i ].NextInTPage != (u_int16_t) -1 )
	{
		Polys[ Polys[ i ].NextInTPage ].PrevInTPage = i;
	}

	PolyTPages[ TPage ].FirstPoly = i;
}

/*===================================================================
	Procedure	:	Remove Poly From TPage Link List
	Input		:	u_int16_t		Poly Index
				:	u_int16_t		TPage to add to
	Output		:	Nothing
===================================================================*/
void RemovePolyFromTPage( u_int16_t i, int16_t TPage )
{
	if( TPage == -1 ) TPage = MAXTPAGESPERTLOAD;

	if( Polys[ i ].PrevInTPage != (u_int16_t) -1 )
	{
		Polys[ Polys[ i ].PrevInTPage ].NextInTPage = Polys[ i ].NextInTPage;
	}
	else
	{
		PolyTPages[ TPage ].FirstPoly = Polys[ i ].NextInTPage;
	}

	if( Polys[ i ].NextInTPage != (u_int16_t) -1 )
	{
		Polys[ Polys[ i ].NextInTPage ].PrevInTPage = Polys[ i ].PrevInTPage;
	}

	Polys[ i ].PrevInTPage = (u_int16_t) -1;
	Polys[ i ].NextInTPage = (u_int16_t) -1;
}

bool DisplayGroupClippedPolys(RENDEROBJECT *renderObject, u_int16_t Group )
{
	int16_t	TPage;
	u_int16_t	i;

	TPage = 0;
	i = PolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !PolyDispGroupClipped( Group, /*ExecBuff*/renderObject, &TPage, &i ) )
			return( true );

//		if( D3D_Device->lpVtbl->Execute( D3D_Device, ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//		if (FSExecuteBuffer(ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//			return false;

		cull_none();

		if (!draw_object(renderObject))
			return false;

		reset_cull();
	}
	return( false );
}

bool DisplayGroupUnclippedPolys( RENDEROBJECT *renderObject )
{
	int16_t	TPage;
	u_int16_t	i;

	TPage = 0;
	i = PolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !PolyDispGroupUnclipped( /*ExecBuff*/renderObject, &TPage, &i ) )
			return( true );

			//if( D3D_Device->lpVtbl->Execute( D3D_Device, ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//			if (FSExecuteBuffer(ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//				return false;

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
				:	u_int16_t	*					Current Poly
	Output		:	True/False
===================================================================*/

bool PolyDispGroupClipped( u_int16_t Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextPoly )
{
	u_int16_t			i;
	int16_t			Count;
	int16_t			TotalVerts;
	int16_t			StartVert;
	int16_t			NumVerts;
	int16_t			NumTris;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	COLOR		Colour;
	LPLVERTEX	PolyVertPnt;
	LPTRIANGLE	PolyFacePnt;
    LPLVERTEX	lpBufStart;
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

		while( ( i != (u_int16_t) -1 ) && ( ( StartVert + NumVerts ) < MAXPOLYVERTS ) )
		{
			if( !( Polys[i].Flags & POLY_FLAG_SOLID ) )
			{
				if( !( Polys[i].Flags & POLY_FLAG_DONTCLIP ) && ( Polys[i].Group == Group ) )
				{
					if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
					{
						Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16_t) Polys[ i ].Frame );
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

	PolyFacePnt = (LPTRIANGLE) lpIndices;
	PolyVertPnt = (LPLVERTEX) lpBufStart;

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
			if( Count == *TPage ) i = *NextPoly;
			else i = PolyTPages[ Count ].FirstPoly;
	
			while( ( i != (u_int16_t) -1 ) && ( StartVert < MAXPOLYVERTS ) )
			{
				if( !( Polys[i].Flags & POLY_FLAG_SOLID ) )
				{
					if( !( Polys[i].Flags & POLY_FLAG_DONTCLIP ) && ( Polys[i].Group == Group ) )
					{
						if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
						{
							int ntris = 0;

		   					Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16_t) Polys[ i ].Frame );
		   					Off_Ptr = ( (*Polys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
		   					Box_Ptr = ( (*Polys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );

#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos1.x;
		   					PolyVertPnt->y = Polys[ i ].Pos1.y;
		   					PolyVertPnt->z = Polys[ i ].Pos1.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;

#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos2.x;
		   					PolyVertPnt->y = Polys[ i ].Pos2.y;
		   					PolyVertPnt->z = Polys[ i ].Pos2.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
 
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos3.x;
		   					PolyVertPnt->y = Polys[ i ].Pos3.y;
		   					PolyVertPnt->z = Polys[ i ].Pos3.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   	
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, 255 );
#endif
	   		
		   					PolyVertPnt->x = Polys[ i ].Pos4.x;
		   					PolyVertPnt->y = Polys[ i ].Pos4.y;
		   					PolyVertPnt->z = Polys[ i ].Pos4.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
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
		}

		if( StartVert >= MAXPOLYVERTS ) break;
	}

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
	*NextPoly = i;

	return( true );
}

/*===================================================================
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16_t	*					Current TPage List
				:	u_int16_t	*					Current Poly
	Output		:	True/False
===================================================================*/
bool PolyDispGroupUnclipped( RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextPoly )
{
	u_int16_t			i;
	int16_t			Count;
	int16_t			TotalVerts;
	int16_t			StartVert;
	int16_t			NumVerts;
	int16_t			NumTris;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	COLOR		Colour;
	LPLVERTEX	PolyVertPnt;
	LPTRIANGLE	PolyFacePnt;
    LPLVERTEX	lpBufStart;
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

		while( ( i != (u_int16_t) -1 ) && ( ( StartVert + NumVerts ) < MAXPOLYVERTS ) )
		{
			if( !( Polys[i].Flags & POLY_FLAG_SOLID ) )
			{
				if( Polys[i].Flags & POLY_FLAG_DONTCLIP )
				{
					if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
					{
						Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16_t) Polys[ i ].Frame );
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

	if( !TotalVerts ) return( false );

	renderObject->numTextureGroups = 0;

	if (!(FSLockVertexBuffer(renderObject, &lpBufStart)))
	{
		return false;
	}
	
	if (!(FSLockIndexBuffer(renderObject, &lpIndices)))
	{
		return false;
	}

	PolyFacePnt = (LPTRIANGLE) lpIndices;
	PolyVertPnt = (LPLVERTEX) lpBufStart;

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
			if( Count == *TPage ) i = *NextPoly;
			else i = PolyTPages[ Count ].FirstPoly;
	
			while( ( i != (u_int16_t) -1 ) && ( StartVert < MAXPOLYVERTS ) )
			{
				if( !( Polys[i].Flags & POLY_FLAG_SOLID ) )
				{
					if( Polys[i].Flags & POLY_FLAG_DONTCLIP )
					{
						if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
						{
							int ntris = 0;

		   					Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16_t) Polys[ i ].Frame );
		   					Off_Ptr = ( (*Polys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
		   					Box_Ptr = ( (*Polys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );


#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos1.x;
		   					PolyVertPnt->y = Polys[ i ].Pos1.y;
		   					PolyVertPnt->z = Polys[ i ].Pos1.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;

#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos2.x;
		   					PolyVertPnt->y = Polys[ i ].Pos2.y;
		   					PolyVertPnt->z = Polys[ i ].Pos2.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   							
	
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos3.x;
		   					PolyVertPnt->y = Polys[ i ].Pos3.y;
		   					PolyVertPnt->z = Polys[ i ].Pos3.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   					
	
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, 255 );
#endif
	   		
		   					PolyVertPnt->x = Polys[ i ].Pos4.x;
		   					PolyVertPnt->y = Polys[ i ].Pos4.y;
		   					PolyVertPnt->z = Polys[ i ].Pos4.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
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
		}

		if( StartVert >= MAXPOLYVERTS ) break;
	}

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
	*NextPoly = i;

	return( true );
}

bool DisplaySolidGroupClippedPolys( RENDEROBJECT *renderObject, u_int16_t Group )
{
	int16_t	TPage;
	u_int16_t	i;

	TPage = 0;
	i = PolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !SolidPolyDispGroupClipped( Group, /*ExecBuff*/renderObject, &TPage, &i ) )
			return( true );

//			if( D3D_Device->lpVtbl->Execute( D3D_Device, ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//			if (FSExecuteBuffer(ExecBuff, D3D_ViewPort, D3DEXECUTE_CLIPPED ) != D3D_OK )
//				return false;

		cull_none();

		if (!draw_object(renderObject))
			return false;

		reset_cull();
	}

	return( false );
}

bool DisplaySolidGroupUnclippedPolys( RENDEROBJECT *renderObject )
{
	int16_t	TPage;
	u_int16_t	i;

	TPage = 0;
	i = PolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !SolidPolyDispGroupUnclipped( /*ExecBuff*/renderObject, &TPage, &i ) )
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
				:	u_int16_t	*					Current Poly
	Output		:	True/False
===================================================================*/
bool SolidPolyDispGroupClipped( u_int16_t Group, /*LPDIRECT3DEXECUTEBUFFER ExecBuffer*/RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextPoly )
{
	u_int16_t			i;
	int16_t			Count;
	int16_t			TotalVerts;
	int16_t			StartVert;
	int16_t			NumVerts;
	int16_t			NumTris;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	COLOR		Colour;
	LPLVERTEX	PolyVertPnt;
	LPTRIANGLE	PolyFacePnt;
    LPLVERTEX	lpBufStart;
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

		while( ( i != (u_int16_t) -1 ) && ( ( StartVert + NumVerts ) < MAXPOLYVERTS ) )
		{
			if( ( Polys[i].Flags & POLY_FLAG_SOLID ) )
			{
				if( !( Polys[i].Flags & POLY_FLAG_DONTCLIP ) && ( Polys[i].Group == Group ) )
				{
					if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
					{
						Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16_t) Polys[ i ].Frame );
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

	PolyFacePnt = (LPTRIANGLE) lpIndices;
	PolyVertPnt = (LPLVERTEX) lpBufStart;

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
			if( Count == *TPage ) i = *NextPoly;
			else i = PolyTPages[ Count ].FirstPoly;
	
			while( ( i != (u_int16_t) -1 ) && ( StartVert < MAXPOLYVERTS ) )
			{
				if( ( Polys[i].Flags & POLY_FLAG_SOLID ) )
				{
					if( !( Polys[i].Flags & POLY_FLAG_DONTCLIP ) && ( Polys[i].Group == Group ) )
					{
						if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
						{
							int ntris = 0;

		   					Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16_t) Polys[ i ].Frame );
		   					Off_Ptr = ( (*Polys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
		   					Box_Ptr = ( (*Polys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );


#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos1.x;
		   					PolyVertPnt->y = Polys[ i ].Pos1.y;
		   					PolyVertPnt->z = Polys[ i ].Pos1.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;

#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos2.x;
		   					PolyVertPnt->y = Polys[ i ].Pos2.y;
		   					PolyVertPnt->z = Polys[ i ].Pos2.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   							
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, 255 );
#endif

		   					PolyVertPnt->x = Polys[ i ].Pos3.x;
		   					PolyVertPnt->y = Polys[ i ].Pos3.y;
		   					PolyVertPnt->z = Polys[ i ].Pos3.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;

#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, 255 );
#endif
	   		
		   					PolyVertPnt->x = Polys[ i ].Pos4.x;
		   					PolyVertPnt->y = Polys[ i ].Pos4.y;
		   					PolyVertPnt->z = Polys[ i ].Pos4.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
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
		}

		if( StartVert >= MAXPOLYVERTS ) break;
	}

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
	*NextPoly = i;

	return( true );
}

/*===================================================================
	Procedure	:	Display All Faceme Polygons in specific group
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16_t	*					Current TPage List
				:	u_int16_t	*					Current Poly
	Output		:	True/False
===================================================================*/
bool SolidPolyDispGroupUnclipped( RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextPoly )
{
	u_int16_t			i;
	int16_t			Count;
	int16_t			TotalVerts;
	int16_t			StartVert;
	int16_t			NumVerts;
	int16_t			NumTris;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	COLOR		Colour;
	LPLVERTEX	PolyVertPnt;
	LPTRIANGLE	PolyFacePnt;
    LPLVERTEX	lpBufStart;
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

		while( ( i != (u_int16_t) -1 ) && ( ( StartVert + NumVerts ) < MAXPOLYVERTS ) )
		{
			if( ( Polys[i].Flags & POLY_FLAG_SOLID ) )
			{
				if( Polys[i].Flags & POLY_FLAG_DONTCLIP )
				{
					if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
					{
						Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16_t) Polys[ i ].Frame );
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
	
	PolyFacePnt = (LPTRIANGLE) lpIndices;
	PolyVertPnt = (LPLVERTEX) lpBufStart;

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
			if( Count == *TPage ) i = *NextPoly;
			else i = PolyTPages[ Count ].FirstPoly;
	
			while( ( i != (u_int16_t) -1 ) && ( StartVert < MAXPOLYVERTS ) )
			{
				if( ( Polys[i].Flags & POLY_FLAG_SOLID ) )
				{
					if( Polys[i].Flags & POLY_FLAG_DONTCLIP )
					{
						if( Polys[i].Frm_Info && (*Polys[i].Frm_Info ) )
						{
							int ntris = 0;

		   					Bit_Ptr = ( (*Polys[ i ].Frm_Info)->Bit_Info + (int16_t) Polys[ i ].Frame );
		   					Off_Ptr = ( (*Polys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
		   					Box_Ptr = ( (*Polys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );

#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, Polys[ i ].Trans1 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col1.R, Polys[ i ].Col1.G, Polys[ i ].Col1.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos1.x;
		   					PolyVertPnt->y = Polys[ i ].Pos1.y;
		   					PolyVertPnt->z = Polys[ i ].Pos1.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;

#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, Polys[ i ].Trans2 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col2.R, Polys[ i ].Col2.G, Polys[ i ].Col2.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos2.x;
		   					PolyVertPnt->y = Polys[ i ].Pos2.y;
		   					PolyVertPnt->z = Polys[ i ].Pos2.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v1;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   							
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, Polys[ i ].Trans3 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col3.R, Polys[ i ].Col3.G, Polys[ i ].Col3.B, 255 );
#endif
   		
		   					PolyVertPnt->x = Polys[ i ].Pos3.x;
		   					PolyVertPnt->y = Polys[ i ].Pos3.y;
		   					PolyVertPnt->z = Polys[ i ].Pos3.z;
		   					PolyVertPnt->tu = Box_Ptr->u2;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
//		   					PolyVertPnt->dwReserved = 0;
		   					PolyVertPnt++;
   					
#if ACTUAL_TRANS
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, Polys[ i ].Trans4 );
#else
									Colour = RGBA_MAKE( Polys[ i ].Col4.R, Polys[ i ].Col4.G, Polys[ i ].Col4.B, 255 );
#endif
	   		
		   					PolyVertPnt->x = Polys[ i ].Pos4.x;
		   					PolyVertPnt->y = Polys[ i ].Pos4.y;
		   					PolyVertPnt->z = Polys[ i ].Pos4.z;
		   					PolyVertPnt->tu = Box_Ptr->u1;
		   					PolyVertPnt->tv = Box_Ptr->v2;
		   					PolyVertPnt->color = Colour;
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
		}

		if( StartVert >= MAXPOLYVERTS ) break;
	}

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
	*NextPoly = i;

	return( true );
}

/*===================================================================
	Procedure	:	Save Polys Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SavePolys( FILE * fp )
{
	u_int16_t	i;
	int16_t	Frm_Info_Index;

	if( fp )
	{
		fwrite( &TotalPolysInUse, sizeof( u_int32_t ),1 ,fp );
		fwrite( &FirstPolyUsed, sizeof( FirstPolyUsed ), 1, fp );
		fwrite( &FirstPolyFree, sizeof( FirstPolyFree ), 1, fp );
		
		for( i = 0; i < ( MAXTPAGESPERTLOAD + 1 ); i++ )
		{
			fwrite( &PolyTPages[ i ].FirstPoly, sizeof( u_int16_t ), 1, fp );
		}

		i = FirstPolyUsed;

		while( i != (u_int16_t) -1 )
		{
			fwrite( &Polys[ i ].Next, sizeof( u_int16_t ), 1, fp );
			fwrite( &Polys[ i ].Prev, sizeof( u_int16_t ), 1, fp );
			fwrite( &Polys[ i ].NextInTPage, sizeof( u_int16_t ), 1, fp );
			fwrite( &Polys[ i ].PrevInTPage, sizeof( u_int16_t ), 1, fp );
			fwrite( &Polys[ i ].Pos1, sizeof( VECTOR ), 1, fp );
			fwrite( &Polys[ i ].Pos2, sizeof( VECTOR ), 1, fp );
			fwrite( &Polys[ i ].Pos3, sizeof( VECTOR ), 1, fp );
			fwrite( &Polys[ i ].Pos4, sizeof( VECTOR ), 1, fp );
			fwrite( &Polys[ i ].Col1, sizeof( POLY_RGB ), 1, fp );
			fwrite( &Polys[ i ].Col2, sizeof( POLY_RGB ), 1, fp );
			fwrite( &Polys[ i ].Col3, sizeof( POLY_RGB ), 1, fp );
			fwrite( &Polys[ i ].Col4, sizeof( POLY_RGB ), 1, fp );
			fwrite( &Polys[ i ].Trans1, sizeof( u_int8_t ), 1, fp );
			fwrite( &Polys[ i ].Trans2, sizeof( u_int8_t ), 1, fp );
			fwrite( &Polys[ i ].Trans3, sizeof( u_int8_t ), 1, fp );
			fwrite( &Polys[ i ].Trans4, sizeof( u_int8_t ), 1, fp );
			fwrite( &Polys[ i ].Flags, sizeof( int16_t ), 1, fp );
			fwrite( &Polys[ i ].Scale, sizeof( float ), 1, fp );
			fwrite( &Polys[ i ].Frame, sizeof( float ), 1, fp );
			fwrite( &Polys[ i ].AnimSpeed, sizeof( float ), 1, fp );
			Frm_Info_Index = Get_Frm_Info_Index( Polys[ i ].Frm_Info );
			fwrite( &Frm_Info_Index, sizeof( int16_t ), 1, fp );
			fwrite( &Polys[ i ].SeqNum, sizeof( int16_t ), 1, fp );
			fwrite( &Polys[ i ].Group, sizeof( u_int16_t ), 1, fp );
			fwrite( &Polys[ i ].TimeStep, sizeof( float ), 1, fp );
			fwrite( &Polys[ i ].Qlerp, sizeof( QUATLERP ), 1, fp );
			fwrite( &Polys[ i ].Quat, sizeof( QUAT ), 1, fp );
			fwrite( &Polys[ i ].Ship, sizeof( u_int16_t ), 1, fp );
			i = Polys[ i ].Prev;
		}

		i = FirstPolyFree;

		while( i != (u_int16_t) -1 )
		{
			fwrite( &Polys[ i ].Next, sizeof( u_int16_t ), 1, fp );
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
	u_int16_t	i;
	int16_t	Frm_Info_Index;

	if( fp )
	{
		fread( &TotalPolysInUse, sizeof( u_int32_t ),1 ,fp );
		fread( &FirstPolyUsed, sizeof( FirstPolyUsed ), 1, fp );
		fread( &FirstPolyFree, sizeof( FirstPolyFree ), 1, fp );
		
		for( i = 0; i < ( MAXTPAGESPERTLOAD + 1 ); i++ )
		{
			fread( &PolyTPages[ i ].FirstPoly, sizeof( u_int16_t ), 1, fp );
		}

		i = FirstPolyUsed;

		while( i != (u_int16_t) -1 )
		{
			fread( &Polys[ i ].Next, sizeof( u_int16_t ), 1, fp );
			fread( &Polys[ i ].Prev, sizeof( u_int16_t ), 1, fp );
			fread( &Polys[ i ].NextInTPage, sizeof( u_int16_t ), 1, fp );
			fread( &Polys[ i ].PrevInTPage, sizeof( u_int16_t ), 1, fp );
			fread( &Polys[ i ].Pos1, sizeof( VECTOR ), 1, fp );
			fread( &Polys[ i ].Pos2, sizeof( VECTOR ), 1, fp );
			fread( &Polys[ i ].Pos3, sizeof( VECTOR ), 1, fp );
			fread( &Polys[ i ].Pos4, sizeof( VECTOR ), 1, fp );
			fread( &Polys[ i ].Col1, sizeof( POLY_RGB ), 1, fp );
			fread( &Polys[ i ].Col2, sizeof( POLY_RGB ), 1, fp );
			fread( &Polys[ i ].Col3, sizeof( POLY_RGB ), 1, fp );
			fread( &Polys[ i ].Col4, sizeof( POLY_RGB ), 1, fp );
			fread( &Polys[ i ].Trans1, sizeof( u_int8_t ), 1, fp );
			fread( &Polys[ i ].Trans2, sizeof( u_int8_t ), 1, fp );
			fread( &Polys[ i ].Trans3, sizeof( u_int8_t ), 1, fp );
			fread( &Polys[ i ].Trans4, sizeof( u_int8_t ), 1, fp );
			fread( &Polys[ i ].Flags, sizeof( int16_t ), 1, fp );
			fread( &Polys[ i ].Scale, sizeof( float ), 1, fp );
			fread( &Polys[ i ].Frame, sizeof( float ), 1, fp );
			fread( &Polys[ i ].AnimSpeed, sizeof( float ), 1, fp );
			fread( &Frm_Info_Index, sizeof( int16_t ), 1, fp );
			Polys[ i ].Frm_Info = Get_Frm_Info_Address( Frm_Info_Index );
			fread( &Polys[ i ].SeqNum, sizeof( int16_t ), 1, fp );
			fread( &Polys[ i ].Group, sizeof( u_int16_t ), 1, fp );
			fread( &Polys[ i ].TimeStep, sizeof( float ), 1, fp );
			fread( &Polys[ i ].Qlerp, sizeof( QUATLERP ), 1, fp );
			Polys[ i ].Qlerp.crnt = &Polys[ i ].Quat;
			fread( &Polys[ i ].Quat, sizeof( QUAT ), 1, fp );
			fread( &Polys[ i ].Ship, sizeof( u_int16_t ), 1, fp );
			i = Polys[ i ].Prev;
		}

		i = FirstPolyFree;

		while( i != (u_int16_t) -1 )
		{
			memset( &Polys[ i ], 0, sizeof( POLY ) );
			Polys[i].Prev = (u_int16_t) -1;
			Polys[i].NextInTPage = (u_int16_t) -1;
			Polys[i].PrevInTPage = (u_int16_t) -1;
			Polys[i].Frm_Info = NULL;

			fread( &Polys[ i ].Next, sizeof( u_int16_t ), 1, fp );
			i = Polys[ i ].Next;
		}
	}

	return( fp );
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif
