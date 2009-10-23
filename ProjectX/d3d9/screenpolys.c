/*===================================================================
*	2 d p o l y s . c
*	All routines to do with 2d always face you polygons...
===================================================================*/
#include <stdio.h>
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
#include "models.h"
#include "2dpolys.h"
#include "screenpolys.h"
#include "controls.h"
#include "secondary.h"
#include "camera.h"
#include "title.h"
#include "main.h"
#include "text.h"
#include "triggers.h"
#include "local.h"
#include "util.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

#define THERMAL_STEP		(1.6F)
#define THERMAL_INC			(0.0092593F)
#define THERMAL_DEC			(0.25F)

/*===================================================================
	Externs
===================================================================*/
extern	BOOL			MipMap;
extern	BOOL			Is3Dfx2;
extern	CAMERA			CurrentCamera;
extern	CAMERA			MainCamera;
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	TLOADHEADER		Tloadheader;
extern	float			framelag;
extern	GLOBALSHIP		Ships[MAX_PLAYERS];
extern	BOOL			UsedStippledAlpha;
extern	FRAME_INFO	*	Laser_Header;
extern	FRAME_INFO	*	AfterBurner_Header;
extern	FRAME_INFO	*	LensFlare_Header;
extern	FRAME_INFO	*	Numbers_Header;
extern	FRAME_INFO	*	Numbers2_Header;
extern	FRAME_INFO	*	Text512_Header;
extern	FRAME_INFO	*	Orb_Header;
extern	FRAME_INFO	*	Solid_Header;
extern	FRAME_INFO	*	Thermo_Header;
extern	FRAME_INFO	*	Logos_Header;
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
extern	BOOL			CanCullFlag;
extern	BOOL			LensFlare;
extern	SECONDARYWEAPONBULLET	SecBulls[MAXSECONDARYWEAPONBULLETS];
extern	uint16		FirstSecBullUsed;
extern	float			ticksperframe;
extern	int16			NewLevelNum;
extern	int16			NumLevels;
extern	MODEL		Models[ MAXNUMOFMODELS ];
extern	int				FontWidth;
extern	int				FontHeight;
extern	int16			MakeColourMode;
extern	float			SoundInfo[MAXGROUPS][MAXGROUPS];
extern	BOOL			CountDownOn;
extern	int16			LevelNum;
extern	char			LevelNames[MAXLEVELS][128];
extern	int16			Lives;
extern	BOOL			DebugInfo;
extern	TRIGGERVAR	*	DecreaseTemperature;
extern	FRAME_INFO	*	Flag_Header;
extern	FRAME_INFO	*	Flags_Header;
extern	int				outside_map;
extern	uint16			HitBox;
extern	uint16			TargScrPolys[ 4 ];
extern	uint32			TeamFlagMask[ MAX_TEAMS ];
extern	TRIGGERVAR	*	TimeLimitTrigger;

// message colours (Title.c)
extern int KillMessageColour; 
extern int SystemMessageColour;
extern int FlagMessageColour;
extern int PlayerMessageColour;

#define RGBA_MAKE2(r, g, b, a)   ((D3DCOLOR) (( (DWORD) ((a) & 0xff) << 24) | ( (DWORD) ((r) & 0xff) << 16) | ( (DWORD) ((g) & 0xff) << 8) | (DWORD) ((b) & 0xff)))
#define	CUTOFF				( 5.0F * 1024.0F * GLOBAL_SCALE )

/*===================================================================
	Globals
===================================================================*/
		BOOL		CountDownOn = FALSE;
static 	BOOL		Toggle = FALSE;
static 	float		ScaleInc = 0.0F;
static	float		Interp_Time = 0.0F;
static	float		CountDown_X = TIMERSTARTSCREENX;
static	float		CountDown_Y = TIMERSTARTSCREENY;
static	float		CountDown_Xoff = 0.0F;
static	float		CountDown_Yoff = 0.0F;
static	float		CountDown_Wanted_X = TIMERSTARTSCREENX;
static	float		CountDown_Wanted_Y = TIMERSTARTSCREENY;
static	float		CountDown_Scale = TIMERSTARTSCALE;
static	float		CountDown_Wanted_Scale = TIMERSTARTSCALE;
static	float		CountDown_Col = 1.0F;

uint16	ScreenMultiples[ MAXMULTIPLES ] = { 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff };
int		SinglePlayerTimeLimit = 0;
uint16	ThermoScrPoly = (uint16) -1;
uint16	FlashScreenPoly	= (uint16) -1;
uint32	TotalScrPolysInUse = 0;
SCRPOLY	ScrPolys[ MAXNUMOFSCRPOLYS ];
uint16	FirstScrPolyUsed;
uint16	FirstScrPolyFree;
float	Old_Time_Float;
float	Countdown_Float = 3000.0F;	// 30 Seconds
float	ZValue;
float	RHWValue;
BOOL	BilinearSolidScrPolys = FALSE;
float	ThermalTemp = 0.0F;
float	WantedThermalTemp = 0.0F;
float	ThermalMinimum = 0.0F;

FRAME_INFO	**	CountDownFontGraphics[ 2 ] = { &Numbers2_Header, &Numbers_Header };
float	CountDownFontWidth[ 2 ] = { 16.0F, 23.0F };
int16	CurrentCountDownFont = 1;
float		Time_Diff= 0.0F;
BOOL		KilledPlayer = FALSE;
BOOL		IllegalTime = FALSE;

int16	NumberSegments[ 11 ][ 7 ] = {
//		  0   1   2   3   4   5   6 
		{ -1, -1, -1, -1, -1, -1,  0 },	// 0
		{  0, -1, -1,  0,  0,  0,  0 },	// 1
		{ -1, -1,  0, -1, -1,  0, -1 },	// 2
		{ -1, -1, -1, -1,  0,  0, -1 },	// 3
		{  0, -1, -1,  0,  0, -1, -1 },	// 4
		{ -1,  0, -1, -1,  0, -1, -1 },	// 5
		{ -1,  0, -1, -1, -1, -1, -1 },	// 6
		{ -1, -1, -1,  0,  0,  0,  0 },	// 7
		{ -1, -1, -1, -1, -1, -1, -1 },	// 8
		{ -1, -1, -1, -1,  0, -1, -1 },	// 9
		{  0,  0,  0,  0,  0,  0,  0 },	// Blank
};

uint16	Min1Digit[ 7 ] = { 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff };
uint16	Min0Digit[ 7 ] = { 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff };
uint16	Bit0Digit[ 1 ] = { 0xffff };
uint16	Sec1Digit[ 7 ] = { 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff };
uint16	Sec0Digit[ 7 ] = { 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff };
uint16	Bit1Digit[ 1 ] = { 0xffff };
uint16	Hun1Digit[ 7 ] = { 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff };
uint16	Hun0Digit[ 7 ] = { 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff };

// defaults a screen poly
void InitScrPoly( uint16 i )
{
		ScrPolys[i].Flags = SCRFLAG_Nothing;
		ScrPolys[i].Type = SCRTYPE_Normal;
		ScrPolys[i].R = 255;
		ScrPolys[i].G = 255;
		ScrPolys[i].B = 255;
		ScrPolys[i].Trans = 255;
		ScrPolys[i].Frame = 0.0F;
		ScrPolys[i].FrameRate = 1.0F;
		ScrPolys[i].Frm_Info = NULL;
		ScrPolys[i].SeqNum = SCRSEQ_Nothing;
		ScrPolys[i].Xscale = 1.0F;
		ScrPolys[i].Yscale = 1.0F;

		ScrPolys[i].NextInTPage = (uint16) -1;
		ScrPolys[i].PrevInTPage = (uint16) -1;

		ScrPolys[i].x1 = 0.0F;
		ScrPolys[i].y1 = 0.0F;
		ScrPolys[i].x2 = 0.0F;
		ScrPolys[i].y2 = 0.0F;
		ScrPolys[i].x3 = 0.0F;
		ScrPolys[i].y3 = 0.0F;
		ScrPolys[i].x4 = 0.0F;
		ScrPolys[i].y4 = 0.0F;
}

void InitScrPolys( void )
{
	uint16	i;

	FlashScreenPoly = (uint16) -1;
	HitBox = (uint16) -1;
	for( i = 0; i < 4; i++ ) TargScrPolys[ i ] = (uint16) -1;
	for( i = 0; i < MAXMULTIPLES; i++ ) ScreenMultiples[ i ] = (uint16) -1;
	ClearCountdownBuffers();

	FirstScrPolyUsed = (uint16) -1;
	FirstScrPolyFree = 0;
	
	for( i = 0; i < MAXNUMOFSCRPOLYS; i++ )
	{
		ScrPolys[i].Next = i + 1;
		ScrPolys[i].Prev = (uint16) -1;
		InitScrPoly(i);
	}

	ScrPolys[ MAXNUMOFSCRPOLYS-1 ].Next = (uint16) -1;

	InitScrPolyTPages();

	if( CountDownOn ) CreateCountdownDigits();
}

/*===================================================================
	Procedure	:	Find a free ScrPoly and move it from the free
				:	list to	the used list
	Input		:	Nothing
	Output		:	uint16	Number of the free ScrPoly
===================================================================*/
uint16 FindFreeScrPoly( void )
{
	uint16 i;

	i = FirstScrPolyFree;
	if( i == (uint16) -1 ) return i;
 
	ScrPolys[i].Prev = FirstScrPolyUsed;
							 
	if ( FirstScrPolyUsed != (uint16) -1)
	{
		ScrPolys[ FirstScrPolyUsed ].Next = i;
	}

	FirstScrPolyUsed = i;
	FirstScrPolyFree = ScrPolys[i].Next;

	TotalScrPolysInUse++;

	return i ;
}

/*===================================================================
	Procedure	:	Kill a used ScrPoly and move it from the used
				:	list to the free list
	Input		:	uint16		Number of ScrPoly to free....
	Output		:	Nothing
===================================================================*/
void KillUsedScrPoly( uint16 i )
{
	uint16	its_prev;
	uint16	its_next;

	if( i == (uint16) -1 ) return;
	
	its_prev = ScrPolys[i].Prev;
	its_next = ScrPolys[i].Next;

	if ( i == FirstScrPolyUsed ) FirstScrPolyUsed = ScrPolys[ i ].Prev;
	if( its_prev != (uint16) -1) ScrPolys[ its_prev ].Next = its_next;
	if( its_next != (uint16) -1) ScrPolys[ its_next ].Prev = its_prev;

	TotalScrPolysInUse--;

	if( ScrPolys[i].Frm_Info )
	{
		RemoveScrPolyFromTPage( i, GetTPage( *ScrPolys[i].Frm_Info, 0 ) );
	}
	else
	{
		RemoveScrPolyFromTPage( i, GetTPage( NULL, 0 ) );
	}

	ScrPolys[i].Prev = (uint16) -1;
	ScrPolys[i].Next = FirstScrPolyFree;
	FirstScrPolyFree = i;

	// cleanup the poly
	// this is important other wise bugs will appear
	// not all routines that get a poly 
	// clear out all previous possible settings

	InitScrPoly( i );
}

/*===================================================================
	Procedure	:	Kill all lastaframe Screen Polys
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void FreeAllLastAFrameScrPolys( void )
{
	uint16	i;
	uint16	NextScrPoly;

	i = FirstScrPolyUsed;

	while( i != (uint16) -1 )
	{
		NextScrPoly = ScrPolys[i].Prev;

		if( ScrPolys[i].Type == SCRTYPE_LastAFrame )
		{
			KillUsedScrPoly( i );
		}
		i = NextScrPoly;
	}
}


/*===================================================================
	Procedure	:	Process Screen Polys
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void ScreenPolyProcess( void )
{
	uint16	i;
	uint16	NextScrPoly;
	float	Speed;

	i = FirstScrPolyUsed;

	while( i != (uint16) -1 )
	{
		NextScrPoly = ScrPolys[i].Prev;

		if( ScrPolys[i].Type == SCRTYPE_LastAFrame )
		{
			KillUsedScrPoly( i );
		}
		else
		{
			if( ( ScrPolys[i].Frm_Info != NULL ) && ( *ScrPolys[i].Frm_Info != NULL ) )
			{
				switch( ScrPolys[i].SeqNum )
				{
				   	case SCRSEQ_Anim:								// Explosion?
				   		ScrPolys[i].Frame += ( framelag / ScrPolys[i].FrameRate );
		
				   		if( ScrPolys[i].Frame >= (*ScrPolys[i].Frm_Info)->Num_Frames )
				   		{
				   			KillUsedScrPoly( i );
				   		}
				   		break;
	
				   	case SCRSEQ_Fade:								// Fade
						Speed = ( framelag * 20.0F );
						ScrPolys[i].LifeCount -= Speed;
						if( ScrPolys[i].LifeCount <= 0.0F )
						{
				   			KillUsedScrPoly( i );
							FlashScreenPoly = (uint16) -1;
						}
						else
						{
							ScrPolys[i].FadeRed -= Speed;
							if( ScrPolys[i].FadeRed < 0.0F ) ScrPolys[i].FadeRed = 0.0F;
							ScrPolys[i].FadeGreen -= Speed;
							if( ScrPolys[i].FadeGreen < 0.0F ) ScrPolys[i].FadeGreen = 0.0F;
							ScrPolys[i].FadeBlue -= Speed;
							if( ScrPolys[i].FadeBlue < 0.0F ) ScrPolys[i].FadeBlue = 0.0F;
							ScrPolys[i].FadeTrans -= Speed;
							if( ScrPolys[i].FadeTrans < 0.0F ) ScrPolys[i].FadeTrans = 0.0F;
							ScrPolys[i].R = (uint8) ScrPolys[i].FadeRed;
							ScrPolys[i].G = (uint8) ScrPolys[i].FadeGreen;
							ScrPolys[i].B = (uint8) ScrPolys[i].FadeBlue;
							ScrPolys[i].Trans = (uint8) ScrPolys[i].FadeTrans;
						}
						break;
				   	case SCRSEQ_FadeUp:								// Fade
						ScrPolys[i].LifeCount += framelag / ( 60.0F * 10.0F );
						if( ScrPolys[i].LifeCount >= 1.0F )
						{
							ScrPolys[i].LifeCount = 1.0F;
						}

						ScrPolys[i].R = (uint8) ((float)ScrPolys[i].FadeRed * ScrPolys[i].LifeCount);
						ScrPolys[i].G = (uint8) ((float)ScrPolys[i].FadeGreen * ScrPolys[i].LifeCount);
						ScrPolys[i].B = (uint8) ((float)ScrPolys[i].FadeBlue * ScrPolys[i].LifeCount);
						ScrPolys[i].Trans = (uint8) ((float)ScrPolys[i].FadeTrans * ScrPolys[i].LifeCount);
						break;
				   	case SCRSEQ_Thermo:								// Explosion?
						if( CheckDecreaseTemperature() )
						{
							if( WantedThermalTemp > ThermalMinimum )
							{
								WantedThermalTemp -= ( THERMAL_DEC * framelag );
								if( WantedThermalTemp < ThermalMinimum ) WantedThermalTemp = ThermalMinimum;
							}
						}
						else
						{
							WantedThermalTemp += ( THERMAL_INC * framelag );
							if( WantedThermalTemp > 100.0F ) WantedThermalTemp = 100.0F;
						}

						if( ThermalTemp > WantedThermalTemp )
						{
							ThermalTemp -= ( THERMAL_STEP * framelag );
							if( ThermalTemp < WantedThermalTemp )
							{
								ThermalTemp = WantedThermalTemp;
							}
						}
						else
						{
							if( ThermalTemp < WantedThermalTemp )
							{
								ThermalTemp += ( THERMAL_STEP * framelag );
								if( ThermalTemp > WantedThermalTemp )
								{
									ThermalTemp = WantedThermalTemp;

									if( WantedThermalTemp > 99.9F )
									{
										if( !KilledPlayer )
										{
											Ships[WhoIAm].Damage = 512.0F;
											Lives = 0;
											DoDamage( OVERRIDE_INVUL );

											Ships[WhoIAm].ShipThatLastKilledMe = WhoIAm;

											if( (Ships[WhoIAm].Object.Mode == DEATH_MODE) || (Ships[WhoIAm].Object.Mode == LIMBO_MODE) || (Ships[WhoIAm].Object.Mode == DEATH_MODE) )
											{
												Ships[WhoIAm].Object.Mode = GAMEOVER_MODE;
											}else{
												Ships[WhoIAm].Object.Mode = DEATH_MODE;
											}
											AddColourMessageToQue(SystemMessageColour, IF_YOU_CANT_TAKE_THE_HEAT );
											AddColourMessageToQue(SystemMessageColour, GET_OUT_OF_THE_KITCHEN );
											Ships[WhoIAm].Timer = 0.0F;

											KilledPlayer = TRUE;
										}
									}
								}
							}
						}

				   		ScrPolys[i].Frame = ( ThermalTemp / ( 100.0F / ( (float) ( Thermo_Header->Num_Frames - 1 ) ) ) );
				   		break;

				   	case SCRSEQ_Logo:								// Logos
						ScrPolys[i].LifeCount += framelag;

						if( ScrPolys[i].LifeCount >= 600.0F )
						{
							ScrPolys[i].Frame += 2.0F;
							ScrPolys[i].LifeCount = 0.0F;
						}

						Speed = ( 2.125F * framelag );

						if( ScrPolys[i].LifeCount < 60.0F )
						{
							ScrPolys[i].FadeRed += Speed;
							if( ScrPolys[i].FadeRed > 128.0F ) ScrPolys[i].FadeRed = 128.0F;
							ScrPolys[i].FadeGreen += Speed;
							if( ScrPolys[i].FadeGreen > 128.0F ) ScrPolys[i].FadeGreen = 128.0F;
							ScrPolys[i].FadeBlue += Speed;
							if( ScrPolys[i].FadeBlue > 128.0F ) ScrPolys[i].FadeBlue = 128.0F;
							ScrPolys[i].FadeTrans += Speed;
							if( ScrPolys[i].FadeTrans > 128.0F ) ScrPolys[i].FadeTrans = 128.0F;
						}
						else
						{
							if( ScrPolys[i].LifeCount >= ( 600.0F - 60.0F ) )
							{
								ScrPolys[i].FadeRed -= Speed;
								if( ScrPolys[i].FadeRed < 0.0F ) ScrPolys[i].FadeRed = 0.0F;
								ScrPolys[i].FadeGreen -= Speed;
								if( ScrPolys[i].FadeGreen < 0.0F ) ScrPolys[i].FadeGreen = 0.0F;
								ScrPolys[i].FadeBlue -= Speed;
								if( ScrPolys[i].FadeBlue < 0.0F ) ScrPolys[i].FadeBlue = 0.0F;
								ScrPolys[i].FadeTrans -= Speed;
								if( ScrPolys[i].FadeTrans < 0.0F ) ScrPolys[i].FadeTrans = 0.0F;
							}
							else
							{
								ScrPolys[i].FadeRed = 128.0F;
								ScrPolys[i].FadeGreen = 128.0F;
								ScrPolys[i].FadeBlue = 128.0F;
								ScrPolys[i].FadeTrans = 128.0F;
							}
						}

						ScrPolys[i].R = (uint8) ScrPolys[i].FadeRed;
						ScrPolys[i].G = (uint8) ScrPolys[i].FadeGreen;
						ScrPolys[i].B = (uint8) ScrPolys[i].FadeBlue;
						ScrPolys[i].Trans = (uint8) ScrPolys[i].FadeTrans;
						break;
						
					case SCRSEQ_Nothing:
					default:
				   		break;
				}
			}
		}

		i = NextScrPoly;
	}
}

/*===================================================================
	Procedure	:	Lensflare Effect
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/

void DoLensflareEffect( void )
{
	VECTOR		DirVector;
	VECTOR		LeftVector;
	VECTOR		UpVector;
	VECTOR		Trans;
	VECTOR		Trans2;
	VECTOR		Vert;
	VECTOR		NewVert;
	MATRIX		TempMatrix;
	MATRIX		FinalMatrix;
	int16		Count;
	float		Center_X;
	float		Center_Y;
	D3DCOLOR	specular;
	VECTOR		Int_Point;
	uint16		Int_Group;
	NORMAL		Int_Normal;
	VECTOR		ShipVector;
	VECTOR		TempVector;
	VECTOR		TempVector2;
	float		Cos, Cos2, Angle, Angle2;
	float		DistToShip;
	float		DistFactor;
	uint8		Int;
	uint16		i;

	/* bjd curr driver = 0 use to be software mode
	if( d3dappi.CurrDriver == 0 )
	{	}
	else
	*/
	{
		if( UsedStippledAlpha == TRUE ) return;
	}

	if( !LensFlare ) return;

	specular = RGB_MAKE( 255, 255, 255 );

	Trans.x = -CurrentCamera.Pos.x;
	Trans.y = -CurrentCamera.Pos.y;
	Trans.z = -CurrentCamera.Pos.z;
	ApplyMatrix( &CurrentCamera.InvMat, &Trans, &Trans2 );			/* Set Translation */

	TempMatrix = CurrentCamera.Mat;
	TempMatrix._41 = Trans2.x;
	TempMatrix._42 = Trans2.y;
	TempMatrix._43 = Trans2.z;

	MatrixMultiply( &TempMatrix, &ProjMatrix, &FinalMatrix );

	Center_X = ( CurrentCamera.Viewport.X + ( CurrentCamera.Viewport.Width / 2.0F ) );
	Center_Y = ( CurrentCamera.Viewport.Y + ( CurrentCamera.Viewport.Height / 2.0F ) );

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		if( Ships[ Count ].enable && ( Current_Camera_View != Count ) )
		{
			if( ( Ships[ Count ].Object.Flags & SHIP_Turbo ) )
			{
				if( !SoundInfo[ Ships[ Count ].Object.Group ][ CurrentCamera.GroupImIn ] )
				{
					ApplyMatrix( &Ships[ Count ].Object.FinalMat, &Forward, &DirVector );
					ApplyMatrix( &Ships[ Count ].Object.FinalMat, &SlideLeft, &LeftVector );
					ApplyMatrix( &Ships[ Count ].Object.FinalMat, &SlideUp, &UpVector );
					
					Vert.x = ( Ships[ Count ].Object.Pos.x + ( ( 270.0F * GLOBAL_SCALE ) * -DirVector.x ) + ( ( 26.0F * GLOBAL_SCALE ) * UpVector.x ) );
					Vert.y = ( Ships[ Count ].Object.Pos.y + ( ( 270.0F * GLOBAL_SCALE ) * -DirVector.y ) + ( ( 26.0F * GLOBAL_SCALE ) * UpVector.y ) );
					Vert.z = ( Ships[ Count ].Object.Pos.z + ( ( 270.0F * GLOBAL_SCALE ) * -DirVector.z ) + ( ( 26.0F * GLOBAL_SCALE ) * UpVector.z ) );
					
					ShipVector.x = ( Vert.x - CurrentCamera.Pos.x );
					ShipVector.y = ( Vert.y - CurrentCamera.Pos.y );
					ShipVector.z = ( Vert.z - CurrentCamera.Pos.z );
					
					DistToShip = VectorLength( &ShipVector );
					
					TempVector = ShipVector;
					NormaliseVector( &TempVector );
					ApplyMatrix( &CurrentCamera.Mat, &Forward, &TempVector2 );
					NormaliseVector( &TempVector2 );
					Cos = DotProduct( &TempVector, &TempVector2 );
					if( Cos < -1.0F ) Cos = -1.0F;
					else if ( Cos > 1.0F ) Cos = 1.0F;
					Angle = (float) R2D( acos( Cos ) );
					
					ApplyMatrix( &Ships[ Count ].Object.FinalMat, &Forward, &TempVector );
					NormaliseVector( &TempVector );
					Cos2 = DotProduct( &TempVector, &TempVector2 );
					if( Cos2 < -1.0F ) Cos2 = -1.0F;
					else if ( Cos2 > 1.0F ) Cos2 = 1.0F;
					Angle2 = (float) R2D( acos( Cos2 ) );
					
					if( DistToShip < CUTOFF ) DistFactor = 1.0F;
					else DistFactor = ( 1.0F - ( ( DistToShip - CUTOFF ) / CUTOFF ) );
					
					Int = (uint8) ( ( Cos * Cos2 * DistFactor ) * 255.0F );
					
					if( ( Angle < 45.0F ) && ( Angle2 < 90.0F ) && ( DistToShip < ( 2.0F * CUTOFF ) ) )
					{
						if( !BackgroundCollide( &MCloadheadert0, &Mloadheader,
									&CurrentCamera.Pos,
									CurrentCamera.GroupImIn,
									&ShipVector, &Int_Point, &Int_Group, &Int_Normal,
									&TempVector, TRUE, NULL ) )
						{
							Conv3DTo2D( &Vert, &NewVert, &FinalMatrix );
							DirVector.x = ( NewVert.x - Center_X );
							DirVector.y = ( NewVert.y - Center_Y );
					
							i = FindFreeScrPoly();
							if( i != (uint16) -1 )
							{
								ScrPolys[ i ].Flags = SCRFLAG_Scale;
								ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
								ScrPolys[ i ].Pos.x = ( Center_X + ( DirVector.x * 0.7F ) );
								ScrPolys[ i ].Pos.y = ( Center_Y + ( DirVector.y * 0.7F ) );
								ScrPolys[ i ].R = ( Int / 2 );				// R Colour
								ScrPolys[ i ].G = ( Int / 4 );				// G Colour
								ScrPolys[ i ].B = ( Int );					// B Colour
								ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
								ScrPolys[ i ].Frame = 0.0F;					// Animation Frame
								ScrPolys[ i ].FrameRate = 1.0F;
								ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
								ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
								ScrPolys[ i ].Xscale = 0.3125F;
								ScrPolys[ i ].Yscale = 0.3125F;
								AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
							}
					
							i = FindFreeScrPoly();
							if( i != (uint16) -1 )
							{
								ScrPolys[ i ].Flags = SCRFLAG_Scale;
								ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
								ScrPolys[ i ].Pos.x = ( Center_X + ( DirVector.x * 0.4F ) );
								ScrPolys[ i ].Pos.y = ( Center_Y + ( DirVector.y * 0.4F ) );
								ScrPolys[ i ].R = ( Int / 4 );				// R Colour
								ScrPolys[ i ].G = ( Int / 4 );				// G Colour
								ScrPolys[ i ].B = ( Int );					// B Colour
								ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
								ScrPolys[ i ].Frame = 0.0F;					// Animation Frame
								ScrPolys[ i ].FrameRate = 1.0F;
								ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
								ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
								ScrPolys[ i ].Xscale = 0.234F;
								ScrPolys[ i ].Yscale = 0.234F;
								AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
							}
					
							i = FindFreeScrPoly();
							if( i != (uint16) -1 )
							{
								ScrPolys[ i ].Flags = SCRFLAG_Scale;
								ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
								ScrPolys[ i ].Pos.x = ( Center_X + ( -DirVector.x * 0.2F ) );
								ScrPolys[ i ].Pos.y = ( Center_Y + ( -DirVector.y * 0.2F ) );
								ScrPolys[ i ].R = ( Int );					// R Colour
								ScrPolys[ i ].G = ( Int / 2 );				// G Colour
								ScrPolys[ i ].B = ( 0 );					// B Colour
								ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
								ScrPolys[ i ].Frame = 1.0F;					// Animation Frame
								ScrPolys[ i ].FrameRate = 1.0F;
								ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
								ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
								ScrPolys[ i ].Xscale = 0.50F;
								ScrPolys[ i ].Yscale = 0.5F;
								AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
							}
					
							i = FindFreeScrPoly();
							if( i != (uint16) -1 )
							{
								ScrPolys[ i ].Flags = SCRFLAG_Scale;
								ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
								ScrPolys[ i ].Pos.x = ( Center_X + ( -DirVector.x * 0.5F ) );
								ScrPolys[ i ].Pos.y = ( Center_Y + ( -DirVector.y * 0.5F ) );
								ScrPolys[ i ].R = ( Int / 4 );				// R Colour
								ScrPolys[ i ].G = ( Int / 4 );				// G Colour
								ScrPolys[ i ].B = ( Int );					// B Colour
								ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
								ScrPolys[ i ].Frame = 0.0F;					// Animation Frame
								ScrPolys[ i ].FrameRate = 1.0F;
								ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
								ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
								ScrPolys[ i ].Xscale = 1.167F;
								ScrPolys[ i ].Yscale = 1.167F;
								AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
							}
					
							i = FindFreeScrPoly();
							if( i != (uint16) -1 )
							{
								ScrPolys[ i ].Flags = SCRFLAG_Scale;
								ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
								ScrPolys[ i ].Pos.x = ( Center_X + ( -DirVector.x * 0.8F ) );
								ScrPolys[ i ].Pos.y = ( Center_Y + ( -DirVector.y * 0.8F ) );
								ScrPolys[ i ].R = ( Int / 4 );				// R Colour
								ScrPolys[ i ].G = ( Int / 4 );				// G Colour
								ScrPolys[ i ].B = ( Int / 2 );					// B Colour
								ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
								ScrPolys[ i ].Frame = 2.0F;					// Animation Frame
								ScrPolys[ i ].FrameRate = 1.0F;
								ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
								ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
								ScrPolys[ i ].Xscale = 0.75F;
								ScrPolys[ i ].Yscale = 0.75F;
								AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
							}
					
							i = FindFreeScrPoly();
							if( i != (uint16) -1 )
							{
								ScrPolys[ i ].Flags = SCRFLAG_Scale;
								ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
								ScrPolys[ i ].Pos.x = ( Center_X + ( -DirVector.x * 1.2F ) );
								ScrPolys[ i ].Pos.y = ( Center_Y + ( -DirVector.y * 1.2F ) );
								ScrPolys[ i ].R = ( Int );					// R Colour
								ScrPolys[ i ].G = ( Int );					// G Colour
								ScrPolys[ i ].B = ( Int );					// B Colour
								ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
								ScrPolys[ i ].Frame = 0.0F;					// Animation Frame
								ScrPolys[ i ].FrameRate = 1.0F;
								ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
								ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
								ScrPolys[ i ].Xscale = 0.75F;
								ScrPolys[ i ].Yscale = 0.75F;
								AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
							}
						}
					}
				}
			}
		}
	}
}

/*===================================================================
	Procedure	:	3D World to 2D Screen
	Input		:	VECTOR	*	Vertex
				:	VECTOR	*	2D Vertex
				:	MATRIX	*	Final View Matrix
	Output		:	Nothing
===================================================================*/
void Conv3DTo2D( VECTOR * SrcVert, VECTOR * DstVert, MATRIX * FinalMat )
{
	VECTOR	TempVert;

	VisPolyApplyMatrix( FinalMat, SrcVert, &TempVert );
	DstVert->x = CurrentCamera.Viewport.X + ( ( CurrentCamera.Viewport.Width / 2 ) + ( CurrentCamera.Viewport.ScaleX * TempVert.x ) );
	DstVert->y = CurrentCamera.Viewport.Y + ( ( CurrentCamera.Viewport.Height / 2 ) - ( CurrentCamera.Viewport.ScaleY * TempVert.y ) );
	DstVert->z = TempVert.z;
}

/*===================================================================
	Procedure	:	3D World to 2D Screen and clip
	Input		:	VECTOR	*	Vertex
				:	VECTOR	*	2D Vertex
				:	MATRIX	*	Final View Matrix
	Output		:	BOOL		True/False
===================================================================*/
BOOL ClipConv3DTo2D( VECTOR * SrcVert, VECTOR * DstVert, MATRIX * FinalMat )
{
	DWORD	Flags;
	VECTOR	TempVert;

	VisPolyApplyMatrix( FinalMat, SrcVert, &TempVert );

	Flags = 0;

	if( TempVert.z > 1.0F )
	{
		TempVert.x = -TempVert.x; 
		TempVert.y = -TempVert.y;
		Flags |= D3DCLIP_FRONT;
	}

	if( TempVert.x < -1.0F )
	{
		Flags |= D3DCLIP_LEFT;
		TempVert.x = -1.0F;
	}
	else
	{
		if( TempVert.x > 1.0F )
		{
			Flags |= D3DCLIP_RIGHT;
			TempVert.x = 1.0F;
		}
	}

   	if( TempVert.y < -1.0F )
	{
		Flags |= D3DCLIP_TOP;
		TempVert.y = -1.0F;
	}
	else
	{
		if( TempVert.y > 1.0F )
		{
			Flags |= D3DCLIP_BOTTOM;
			TempVert.y = 1.0F;
		}
	}
	DstVert->x = CurrentCamera.Viewport.X + ( ( CurrentCamera.Viewport.Width / 2 ) + ( CurrentCamera.Viewport.ScaleX * TempVert.x ) );
	DstVert->y = CurrentCamera.Viewport.Y + ( ( CurrentCamera.Viewport.Height / 2 ) - ( CurrentCamera.Viewport.ScaleY * TempVert.y ) );
	DstVert->z = TempVert.z;
	if( Flags ) return( TRUE );
	return( FALSE );
}

/*===================================================================
	Procedure	:	Clip box to viewport
	Input		:	LPD3DTLVERTEX	Vert1
				:	LPD3DTLVERTEX	Vert3
	Output		:	FALSE if box is inside viewport, TRUE if outside
===================================================================*/

BOOL ClipBox( LPD3DTLVERTEX topleft, LPD3DTLVERTEX bottomright )
{
	D3DVALUE xmin, ymin, xmax, ymax;
	int clip_topleft, clip_bottomright;
	D3DVALUE dx, dy;
	D3DVALUE du, dv;

	xmin = (D3DVALUE) CurrentCamera.Viewport.X;
	ymin = (D3DVALUE) CurrentCamera.Viewport.Y;
	xmax = (D3DVALUE) ( CurrentCamera.Viewport.X + CurrentCamera.Viewport.Width );
	ymax = (D3DVALUE) ( CurrentCamera.Viewport.Y + CurrentCamera.Viewport.Height );

	if ( topleft->sx < xmin )
		clip_topleft = D3DCLIP_LEFT;
	else if ( topleft->sx >= xmax )
		return TRUE;
	else
		clip_topleft = 0;
	if ( topleft->sy < ymin )
		clip_topleft |= D3DCLIP_TOP;
	else if ( topleft->sy >= ymax )
		return TRUE;

	if ( bottomright->sx < xmin )
		return TRUE;
	else if ( bottomright->sx > xmax )
		clip_bottomright = D3DCLIP_RIGHT;
	else
		clip_bottomright = 0;
	if ( bottomright->sy < ymin )
		return TRUE;
	else if ( bottomright->sy > ymax )
		clip_bottomright |= D3DCLIP_BOTTOM;

	dx = bottomright->sx - topleft->sx;
	dy = bottomright->sy - topleft->sy;
	du = bottomright->tu - topleft->tu;
	dv = bottomright->tv - topleft->tv;

	if ( clip_topleft & D3DCLIP_LEFT )
	{
		topleft->tu += du * ( xmin - topleft->sx ) / dx;
		topleft->sx = xmin;
	}
	if ( clip_topleft & D3DCLIP_TOP )
	{
		topleft->tv += dv * ( ymin - topleft->sy ) / dy;
		topleft->sy = ymin;
	}

	if ( clip_bottomright & D3DCLIP_RIGHT )
	{
		bottomright->tu -= du * ( bottomright->sx - xmax ) / dx;
		bottomright->sx = xmax;
	}
	if ( clip_bottomright & D3DCLIP_BOTTOM )
	{
		bottomright->tv -= dv * ( bottomright->sy - ymax ) / dy;
		bottomright->sy = ymax;
	}

	return FALSE;
}

/*===================================================================
	Procedure	:	Create Lensflare effect on missiles
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DoAllSecBullLensflare( void )
{
	uint16	i;

	i = FirstSecBullUsed;

	while( i != (uint16) -1 )
	{
		if( !SoundInfo[ SecBulls[i].GroupImIn ][ CurrentCamera.GroupImIn ] )
		{
			if( SecBulls[ i ].Lensflare ) SecBullLensflare( i );
		}
		i = SecBulls[ i ].Prev;											/* Next Missile */
	}
}

/*===================================================================
	Procedure	:	Create Lensflare effect on missiles
	Input		:	uint16	Secondary bullet index
	Output		:	Nothing
===================================================================*/
void SecBullLensflare( uint16 i )
{
	VECTOR		DirVector;
	VECTOR		LeftVector;
	VECTOR		UpVector;
	VECTOR		Trans;
	VECTOR		Trans2;
	VECTOR		Vert;
	VECTOR		NewVert;
	MATRIX		TempMatrix;
	MATRIX		FinalMatrix;
	float		Center_X;
	float		Center_Y;
	D3DCOLOR	specular;
	VECTOR		Int_Point;
	uint16		Int_Group;
	NORMAL		Int_Normal;
	VECTOR		BullVector;
	VECTOR		TempVector;
	VECTOR		TempVector2;
	float		Cos, Cos2, Angle, Angle2;
	float		DistToBull;
	float		DistFactor;
	uint8		Int;

	/* bjd curre driver = 0 use to be software mode
	if( d3dappi.CurrDriver == 0 )
	{	}
	else
	*/
	{
		if( UsedStippledAlpha == TRUE ) return;
	}

	if( !LensFlare ) return;

	specular = RGB_MAKE( 255, 255, 255 );

	Trans.x = -CurrentCamera.Pos.x;
	Trans.y = -CurrentCamera.Pos.y;
	Trans.z = -CurrentCamera.Pos.z;
	ApplyMatrix( &CurrentCamera.InvMat, &Trans, &Trans2 );			/* Set Translation */

	TempMatrix = CurrentCamera.Mat;
	TempMatrix._41 = Trans2.x;
	TempMatrix._42 = Trans2.y;
	TempMatrix._43 = Trans2.z;

	MatrixMultiply( &TempMatrix, &ProjMatrix, &FinalMatrix );

	Center_X = ( CurrentCamera.Viewport.X + ( CurrentCamera.Viewport.Width / 2.0F ) );
	Center_Y = ( CurrentCamera.Viewport.Y + ( CurrentCamera.Viewport.Height / 2.0F ) );

	ApplyMatrix( &SecBulls[ i ].Mat, &Forward, &DirVector );
	ApplyMatrix( &SecBulls[ i ].Mat, &SlideLeft, &LeftVector );
	ApplyMatrix( &SecBulls[ i ].Mat, &SlideUp, &UpVector );
				
	Vert.x = ( SecBulls[ i ].Pos.x + ( ( 270.0F * GLOBAL_SCALE ) * -DirVector.x ) + ( ( 26.0F * GLOBAL_SCALE ) * UpVector.x ) );
	Vert.y = ( SecBulls[ i ].Pos.y + ( ( 270.0F * GLOBAL_SCALE ) * -DirVector.y ) + ( ( 26.0F * GLOBAL_SCALE ) * UpVector.y ) );
	Vert.z = ( SecBulls[ i ].Pos.z + ( ( 270.0F * GLOBAL_SCALE ) * -DirVector.z ) + ( ( 26.0F * GLOBAL_SCALE ) * UpVector.z ) );

	BullVector.x = ( Vert.x - CurrentCamera.Pos.x );
	BullVector.y = ( Vert.y - CurrentCamera.Pos.y );
	BullVector.z = ( Vert.z - CurrentCamera.Pos.z );

	DistToBull = VectorLength( &BullVector );

	TempVector = BullVector;
	NormaliseVector( &TempVector );
	ApplyMatrix( &CurrentCamera.Mat, &Forward, &TempVector2 );
	NormaliseVector( &TempVector2 );
	Cos = DotProduct( &TempVector, &TempVector2 );
	if( Cos < -1.0F ) Cos = -1.0F;
	else if ( Cos > 1.0F ) Cos = 1.0F;
	Angle = (float) R2D( acos( Cos ) );

	ApplyMatrix( &SecBulls[ i ].Mat, &Forward, &TempVector );
	NormaliseVector( &TempVector );
	Cos2 = DotProduct( &TempVector, &TempVector2 );
	if( Cos2 < -1.0F ) Cos2 = -1.0F;
	else if ( Cos2 > 1.0F ) Cos2 = 1.0F;
	Angle2 = (float) R2D( acos( Cos2 ) );

	if( DistToBull < CUTOFF ) DistFactor = 1.0F;
	else DistFactor = ( 1.0F - ( ( DistToBull - CUTOFF ) / CUTOFF ) );

	Int = (uint8) ( ( Cos * Cos2 * DistFactor ) * 255.0F );

	if( ( Angle < 45.0F ) && ( Angle2 < 90.0F ) && ( DistToBull < ( 2.0F * CUTOFF ) ) )
	{
		if( !BackgroundCollide( &MCloadheadert0, &Mloadheader,
					&CurrentCamera.Pos,
					CurrentCamera.GroupImIn,
					&BullVector, &Int_Point, &Int_Group, &Int_Normal,
					&TempVector, TRUE, NULL ) )
		{
			Conv3DTo2D( &Vert, &NewVert, &FinalMatrix );
			DirVector.x = ( NewVert.x - Center_X );
			DirVector.y = ( NewVert.y - Center_Y );
	
			i = FindFreeScrPoly();
			if( i != (uint16) -1 )
			{
				ScrPolys[ i ].Flags = SCRFLAG_Scale;
				ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
				ScrPolys[ i ].Pos.x = ( Center_X + ( DirVector.x * 0.7F ) );
				ScrPolys[ i ].Pos.y = ( Center_Y + ( DirVector.y * 0.7F ) );
				ScrPolys[ i ].R = ( Int / 2 );				// R Colour
				ScrPolys[ i ].G = ( Int / 4 );				// G Colour
				ScrPolys[ i ].B = ( Int );					// B Colour
				ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
				ScrPolys[ i ].Frame = 0.0F;					// Animation Frame
				ScrPolys[ i ].FrameRate = 1.0F;
				ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
				ScrPolys[ i ].Xscale = 0.3125F;
				ScrPolys[ i ].Yscale = 0.3125F;
				AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
			}

			i = FindFreeScrPoly();
			if( i != (uint16) -1 )
			{
				ScrPolys[ i ].Flags = SCRFLAG_Scale;
				ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
				ScrPolys[ i ].Pos.x = ( Center_X + ( DirVector.x * 0.4F ) );
				ScrPolys[ i ].Pos.y = ( Center_Y + ( DirVector.y * 0.4F ) );
				ScrPolys[ i ].R = ( Int / 4 );				// R Colour
				ScrPolys[ i ].G = ( Int / 4 );				// G Colour
				ScrPolys[ i ].B = ( Int );					// B Colour
				ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
				ScrPolys[ i ].Frame = 0.0F;					// Animation Frame
				ScrPolys[ i ].FrameRate = 1.0F;
				ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
				ScrPolys[ i ].Xscale = 0.234F;
				ScrPolys[ i ].Yscale = 0.234F;
				AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
			}
	
			i = FindFreeScrPoly();
			if( i != (uint16) -1 )
			{
				ScrPolys[ i ].Flags = SCRFLAG_Scale;
				ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
				ScrPolys[ i ].Pos.x = ( Center_X + ( -DirVector.x * 0.2F ) );
				ScrPolys[ i ].Pos.y = ( Center_Y + ( -DirVector.y * 0.2F ) );
				ScrPolys[ i ].R = ( Int );					// R Colour
				ScrPolys[ i ].G = ( Int / 2 );				// G Colour
				ScrPolys[ i ].B = ( 0 );					// B Colour
				ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
				ScrPolys[ i ].Frame = 1.0F;					// Animation Frame
				ScrPolys[ i ].FrameRate = 1.0F;
				ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
				ScrPolys[ i ].Xscale = 0.50F;
				ScrPolys[ i ].Yscale = 0.5F;
				AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
			}
	
			i = FindFreeScrPoly();
			if( i != (uint16) -1 )
			{
				ScrPolys[ i ].Flags = SCRFLAG_Scale;
				ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
				ScrPolys[ i ].Pos.x = ( Center_X + ( -DirVector.x * 0.5F ) );
				ScrPolys[ i ].Pos.y = ( Center_Y + ( -DirVector.y * 0.5F ) );
				ScrPolys[ i ].R = ( Int / 4 );				// R Colour
				ScrPolys[ i ].G = ( Int / 4 );				// G Colour
				ScrPolys[ i ].B = ( Int );					// B Colour
				ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
				ScrPolys[ i ].Frame = 0.0F;					// Animation Frame
				ScrPolys[ i ].FrameRate = 1.0F;
				ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
				ScrPolys[ i ].Xscale = 1.167F;
				ScrPolys[ i ].Yscale = 1.167F;
				AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
			}
	
			i = FindFreeScrPoly();
			if( i != (uint16) -1 )
			{
				ScrPolys[ i ].Flags = SCRFLAG_Scale;
				ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
				ScrPolys[ i ].Pos.x = ( Center_X + ( -DirVector.x * 0.8F ) );
				ScrPolys[ i ].Pos.y = ( Center_Y + ( -DirVector.y * 0.8F ) );
				ScrPolys[ i ].R = ( Int / 4 );				// R Colour
				ScrPolys[ i ].G = ( Int / 4 );				// G Colour
				ScrPolys[ i ].B = ( Int / 2 );					// B Colour
				ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
				ScrPolys[ i ].Frame = 2.0F;					// Animation Frame
				ScrPolys[ i ].FrameRate = 1.0F;
				ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
				ScrPolys[ i ].Xscale = 0.75F;
				ScrPolys[ i ].Yscale = 0.75F;
				AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
			}

			i = FindFreeScrPoly();
			if( i != (uint16) -1 )
			{
				ScrPolys[ i ].Flags = SCRFLAG_Scale;
				ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
				ScrPolys[ i ].Pos.x = ( Center_X + ( -DirVector.x * 1.2F ) );
				ScrPolys[ i ].Pos.y = ( Center_Y + ( -DirVector.y * 1.2F ) );
				ScrPolys[ i ].R = ( Int );					// R Colour
				ScrPolys[ i ].G = ( Int );					// G Colour
				ScrPolys[ i ].B = ( Int );					// B Colour
				ScrPolys[ i ].Trans = ( Int );				// Amount of transparency
				ScrPolys[ i ].Frame = 0.0F;					// Animation Frame
				ScrPolys[ i ].FrameRate = 1.0F;
				ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ i ].Frm_Info = &LensFlare_Header;	// Offset Info
				ScrPolys[ i ].Xscale = 0.75F;
				ScrPolys[ i ].Yscale = 0.75F;
				AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
			}
		}
	}
}

/*===================================================================
	Procedure	:	Update Position and digits of counter
	Input		:	float		XPos
				:	float		YPos
				:	LONGLONG	Number
	Output		:	Nothing
===================================================================*/
void UpdateCountdownDigits( void )
{
	float		XPos, YPos;
	LONGLONG	DigitNumber;
	LONGLONG	Time_Freq;
	LONGLONG	Time_Value;
	LONGLONG	Number;
	float		Time_Float;
	float		Scale = 0.125F;
	float		Center_X = 160.0F;
	float		Center_Y = 120.0F;
	FRAME_INFO	**	Graphics;
	float		Width;

	Graphics = CountDownFontGraphics[ CurrentCountDownFont ];
	Width = CountDownFontWidth[ CurrentCountDownFont ];

	QueryPerformanceCounter((LARGE_INTEGER *) &Time_Value);
	QueryPerformanceFrequency((LARGE_INTEGER *) &Time_Freq);
	Time_Float = ( ( Time_Value * 100.0F ) / Time_Freq );
	Time_Diff = ( Time_Float - Old_Time_Float );

	if( MyGameStatus == STATUS_SinglePlayer )
	{
		if( ( framelag == 0.0F ) || ( DebugInfo ) )
		{
			Time_Diff = 0.0F;
		}
	}
	else
	{
		if( IllegalTime )
		{
			RequestTime();
		}
	}

	if( ( Countdown_Float >= 1000.0F ) && ( ( Countdown_Float - Time_Diff ) < 1000.0F ) )
	{
		CurrentCountDownFont = 1;
		CountDown_Wanted_X = TIMERENDSCREENX;
		CountDown_Wanted_Y = TIMERENDSCREENY;
		CountDown_Wanted_Scale = TIMERENDSCALE;
		Toggle = TRUE;
	}

	Old_Time_Float = Time_Float;
	Countdown_Float -= Time_Diff;
	Number = (LONGLONG) Countdown_Float;

	if( Countdown_Float < 0.0F )
	{
		if( MyGameStatus != STATUS_SinglePlayer )
		{
			LevelNum = -1;
			NewLevelNum++;
			if( NewLevelNum >= NumLevels ) NewLevelNum = 0;
		}
		else
		{
			if( !KilledPlayer )
			{
				Ships[WhoIAm].Damage = 512.0F;
				Lives = 0;
				DoDamage( OVERRIDE_INVUL );

				Ships[WhoIAm].ShipThatLastKilledMe = WhoIAm;

				if( (Ships[WhoIAm].Object.Mode == DEATH_MODE) || (Ships[WhoIAm].Object.Mode == LIMBO_MODE) || (Ships[WhoIAm].Object.Mode == DEATH_MODE) )
				{
					Ships[WhoIAm].Object.Mode = GAMEOVER_MODE;
				}else{
					Ships[WhoIAm].Object.Mode = DEATH_MODE;
				}
				AddColourMessageToQue(SystemMessageColour, OUT_OF_TIME );
				Ships[WhoIAm].Timer = 0.0F;

				KilledPlayer = TRUE;
			}
		}

	 	Countdown_Float = 0.0F;
	}

	
	if( Toggle )
	{
		ScaleInc = ( ( CountDown_Wanted_Scale - CountDown_Scale ) / 420.0F );
		CountDown_Xoff = ( ( CountDown_Wanted_X - CountDown_X ) / 420.0F );
		CountDown_Yoff = ( ( CountDown_Wanted_Y - CountDown_Y ) / 420.0F );
		Interp_Time = 420.0F;
		Toggle = FALSE;
	}

	if( Interp_Time )
	{
		Interp_Time -= framelag;
		if( Interp_Time < 0.0F )
		{
			Interp_Time = 0.0F;
			CountDown_X = CountDown_Wanted_X;
			CountDown_Y = CountDown_Wanted_Y;
			CountDown_Scale = CountDown_Wanted_Scale;
		}
		else
		{
			CountDown_X += ( CountDown_Xoff * framelag );
			CountDown_Y += ( CountDown_Yoff * framelag );
			CountDown_Scale += ( ScaleInc * framelag );
		}
	}

	if( Countdown_Float < 400.0F )
	{
		CountDown_Col -= ( ( 1.0F / 300.0F ) * framelag );
		if( CountDown_Col < 0.0F ) CountDown_Col = 0.0F;
	}
	else
	{
		CountDown_Col += ( ( 1.0F / 300.0F ) * framelag );
		if( CountDown_Col > 1.0F ) CountDown_Col = 1.0F;
	}
	
	Center_X = CountDown_X;
	Center_Y = CountDown_Y;
	Scale = CountDown_Scale * ( 23.0F / CountDownFontWidth[ CurrentCountDownFont ] );

	DigitNumber = ( Number / 60000 );
	Number -= ( DigitNumber * 60000 );
	XPos = Center_X - ( ( Width * 4.173913F ) * Scale );
	YPos = Center_Y;
	UpdateDigit( &Min1Digit[ 0 ], XPos, YPos, (int16) ( DigitNumber % 10 ), Scale, CountDown_Col, Graphics );
	
	DigitNumber = ( Number / 6000 );
	Number -= ( DigitNumber * 6000 );
	XPos = Center_X - ( ( Width * 2.7826F ) * Scale );
	UpdateDigit( &Min0Digit[ 0 ], XPos, YPos, (int16) ( DigitNumber % 10 ), Scale, CountDown_Col, Graphics );

	XPos = Center_X - ( ( Width * 1.73913F ) * Scale );
	UpdateDigit( &Bit0Digit[ 0 ], XPos, YPos, 10, Scale, CountDown_Col, Graphics );

	DigitNumber = ( Number / 1000 );
	Number -= ( DigitNumber * 1000 );
	XPos = Center_X - ( ( Width * 0.695652F ) * Scale );
	UpdateDigit( &Sec1Digit[ 0 ], XPos, YPos, (int16) ( DigitNumber % 10 ), Scale, CountDown_Col, Graphics );

	DigitNumber = ( Number / 100 );
	Number -= ( DigitNumber * 100 );
	XPos = Center_X + ( ( Width * 0.695652F ) * Scale );
	UpdateDigit( &Sec0Digit[ 0 ], XPos, YPos, (int16) ( DigitNumber % 10 ), Scale, CountDown_Col, Graphics );

	XPos = Center_X + ( ( Width * 1.73813F ) * Scale );
	UpdateDigit( &Bit1Digit[ 0 ], XPos, YPos, 10, Scale, CountDown_Col, Graphics );

	DigitNumber = ( Number / 10 );
	Number -= ( DigitNumber * 10 );
	XPos = Center_X + ( ( Width * 2.7826F ) * Scale );
	UpdateDigit( &Hun1Digit[ 0 ], XPos, YPos, (int16) ( DigitNumber % 10 ), Scale, CountDown_Col, Graphics );

	DigitNumber = Number;
	XPos = Center_X + ( ( Width * 4.173913F ) * Scale );
	UpdateDigit( &Hun0Digit[ 0 ], XPos, YPos, (int16) ( DigitNumber % 10 ), Scale, CountDown_Col, Graphics );
}

/*===================================================================
	Procedure	:	Update Digit Position and Number
	Input		:	uint16	*	Digit Segmnent Array
				:	float		X Pos
				:	float		Y Pos
				:	int16		Number
				:	float		Scale
				:	float		Colour Brightness;
	Output		:	Nothing
===================================================================*/
void UpdateDigit( uint16 * DigitArray, float XPos, float YPos, int16 Number, float Scale, float Col ,FRAME_INFO	** Graphics )
{
	uint16	i;
	int16	Count;
	uint8	Red, Green, Blue, Trans;
	int16	NumSegments;

	XPos = ( XPos * ( MainCamera.Viewport.Width / 320.0F ) );
	YPos = ( YPos * ( MainCamera.Viewport.Height / 240.0F ) );

	Green = 0;
	Blue = 0;
	Trans = (uint8) ( 255.0F * Col );

	if( Number == 10 ) NumSegments = 1;
	else NumSegments = 7;

	for( Count = 0; Count < NumSegments; Count++ )
	{
		i = DigitArray[ Count ];

		if( i != (uint16) -1 )
		{
			if( Number == 10 )
			{
				Red = (uint8) ( 255.0F * Col );
			}
			else
			{
				if( NumberSegments[ Number ][ Count ] ) Red = (uint8) ( 255.0F * Col );
				else Red = (uint8) ( 80.0F * Col );
			}

			ScrPolys[i].Pos.x = XPos;
			ScrPolys[i].Pos.y = YPos;
			ScrPolys[i].R = Red;
			ScrPolys[i].G = Green;
			ScrPolys[i].B = Blue;
			ScrPolys[i].Trans = Trans;
			ScrPolys[i].Xscale = Scale;
			ScrPolys[i].Yscale = Scale;

			if( ScrPolys[i].Frm_Info != Graphics )
			{
				RemoveScrPolyFromTPage( i, GetTPage( *ScrPolys[i].Frm_Info, 0 ) );
				ScrPolys[i].Frm_Info = Graphics;
				AddScrPolyToTPage( i, GetTPage( *ScrPolys[i].Frm_Info, 0 ) );
			}
		}
	}
}

/*===================================================================
	Procedure	:	Lensflare Effect
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void CreateCountdownDigits( void )
{
	DeleteCountdownDigits();

	CreateDigit( &Min1Digit[ 0 ], 160.0F, 120.0F );
	CreateDigit( &Min0Digit[ 0 ], 160.0F, 120.0F );
	CreateSeperatorDigit( &Bit0Digit[ 0 ], 160.0F, 120.0F );
	CreateDigit( &Sec1Digit[ 0 ], 160.0F, 120.0F );
	CreateDigit( &Sec0Digit[ 0 ], 160.0F, 120.0F );
	CreateSeperatorDigit( &Bit1Digit[ 0 ], 160.0F, 120.0F );
	CreateDigit( &Hun1Digit[ 0 ], 160.0F, 120.0F );
	CreateDigit( &Hun0Digit[ 0 ], 160.0F, 120.0F );
}

/*===================================================================
	Procedure	:	Lensflare Effect
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void DeleteCountdownDigits( void )
{
	DeleteDigit( &Min1Digit[ 0 ] );
	DeleteDigit( &Min0Digit[ 0 ] );
	DeleteSeperatorDigit( &Bit0Digit[ 0 ] );
	DeleteDigit( &Sec1Digit[ 0 ] );
	DeleteDigit( &Sec0Digit[ 0 ] );
	DeleteSeperatorDigit( &Bit1Digit[ 0 ] );
	DeleteDigit( &Hun1Digit[ 0 ] );
	DeleteDigit( &Hun0Digit[ 0 ] );
}

/*===================================================================
	Procedure	:	Clear Countdown ScrPolys index tables
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void ClearCountdownBuffers( void )
{
	int16	Count;

	for( Count = 0; Count < 7; Count++ )
	{
		Min1Digit[ Count ] = (uint16) -1;
		Min0Digit[ Count ] = (uint16) -1;
		Sec1Digit[ Count ] = (uint16) -1;
		Sec0Digit[ Count ] = (uint16) -1;
		Hun1Digit[ Count ] = (uint16) -1;
		Hun0Digit[ Count ] = (uint16) -1;
	}

	Bit0Digit[ 0 ] = (uint16) -1;
	Bit1Digit[ 0 ] = (uint16) -1;
}

/*===================================================================
	Procedure	:	Create Digit
	Input		:	uint16	*	Digit Array ( 7 )
				:	float		X Position
				:	float		Y Position
	Output		:	Nothing
===================================================================*/
void CreateDigit( uint16 * DigitArray, float XPos, float YPos )
{
	uint16	i;
	int16	Count;

	for( Count = 0; Count < 7; Count++ )
	{
		i = FindFreeScrPoly();

		DigitArray[ Count ] = i;

		if( i != (uint16) -1 )
		{
			ScrPolys[ i ].Flags = SCRFLAG_Scale;
			ScrPolys[ i ].Type = SCRTYPE_Normal;
			ScrPolys[ i ].Pos.x = XPos;
			ScrPolys[ i ].Pos.y = YPos;
			ScrPolys[ i ].Pos.z = 0.0F;
			ScrPolys[ i ].R = 64;						// R Colour
			ScrPolys[ i ].G = 0;						// G Colour
			ScrPolys[ i ].B = 0;						// B Colour
			ScrPolys[ i ].Trans = 64;					// Amount of transparency
			ScrPolys[ i ].Frame = (float) Count;		// Animation Frame
			ScrPolys[ i ].FrameRate = 0.0F;
			ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
			ScrPolys[ i ].Frm_Info = &Numbers_Header;	// Offset Info
			ScrPolys[ i ].Xscale = 0.125F;
			ScrPolys[ i ].Yscale = 0.125F;
			AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
		}
	}
}

/*===================================================================
	Procedure	:	Create ":" Seperator Digit
	Input		:	uint16	*	Digit Array ( 7 )
				:	float		X Position
				:	float		Y Position
	Output		:	Nothing
===================================================================*/
void CreateSeperatorDigit( uint16 * DigitArray, float XPos, float YPos )
{
	uint16	i;

	i = FindFreeScrPoly();

	DigitArray[ 0 ] = i;

	if( i != (uint16) -1 )
	{
		ScrPolys[ i ].Flags = SCRFLAG_Scale;
		ScrPolys[ i ].Type = SCRTYPE_Normal;
		ScrPolys[ i ].Pos.x = XPos;
		ScrPolys[ i ].Pos.y = YPos;
		ScrPolys[ i ].Pos.z = 0.0F;
		ScrPolys[ i ].R = 64;						// R Colour
		ScrPolys[ i ].G = 0;						// G Colour
		ScrPolys[ i ].B = 0;						// B Colour
		ScrPolys[ i ].Trans = 64;					// Amount of transparency
		ScrPolys[ i ].Frame = 7.0F;					// Animation Frame
		ScrPolys[ i ].FrameRate = 0.0F;
		ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
		ScrPolys[ i ].Frm_Info = &Numbers_Header;	// Offset Info
		ScrPolys[ i ].Xscale = 0.125F;
		ScrPolys[ i ].Yscale = 0.125F;
		AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Delete Digit
	Input		:	uint16	*	Digit Array[ 7 ]
	Output		:	Nothing
===================================================================*/
void DeleteDigit( uint16 * DigitArray )
{
	int16	Count;

	for( Count = 0; Count < 7; Count++ )
	{
		if( DigitArray[ Count ] != (uint16) -1 )
		{
			KillUsedScrPoly( DigitArray[ Count ] );
			DigitArray[ Count ] = (uint16) -1;
		}
	}
}

/*===================================================================
	Procedure	:	Delete ":" Seperator Digit
	Input		:	uint16	*	Digit Array[ 1 ]
	Output		:	Nothing
===================================================================*/
void DeleteSeperatorDigit( uint16 * DigitArray )
{
	if( DigitArray[ 0 ] != (uint16) -1 )
	{
		KillUsedScrPoly( DigitArray[ 0 ] );
		DigitArray[ 0 ] = (uint16) -1;
	}
}

/*===================================================================
	Procedure	:	Start CountDown Timer
	Input		:	int16	Minutes
				:	int16	Seconds
	Output		:	Nothing
===================================================================*/
void StartCountDown( int16 Minutes, int16 Seconds )
{
	LONGLONG	Time_Freq;
	LONGLONG	Time_Value;

	CurrentCountDownFont = 0;
	Countdown_Float = ( ( Minutes * 6000.0F ) + ( Seconds * 100.0F ) );

	QueryPerformanceCounter((LARGE_INTEGER *) &Time_Value);
	QueryPerformanceFrequency((LARGE_INTEGER *) &Time_Freq);
	Old_Time_Float = ( ( Time_Value * 100.0F ) / Time_Freq );

	Toggle = FALSE;
	ScaleInc = 0.0F;
	Interp_Time = 0.0F;
	CountDown_X = TIMERSTARTSCREENX;
	CountDown_Y = TIMERSTARTSCREENY;
	CountDown_Xoff = 0.0F;
	CountDown_Yoff = 0.0F;
	CountDown_Wanted_X = TIMERSTARTSCREENX;
	CountDown_Wanted_Y = TIMERSTARTSCREENY;
	CountDown_Scale = TIMERSTARTSCALE;
	CountDown_Wanted_Scale = TIMERSTARTSCALE;
	CountDown_Col = 1.0F;
}

/*===================================================================
	Procedure	:	Add Solid ScreenPoly Text
	Input		:	uint16	BoxNumber
				:	float	XPos
				:	float	YPos
				:	uint8	Red
				:	uint8	Green
				:	uint8	Blue
				:	uint8	Trans
	Output		:	void
===================================================================*/
void AddScreenPolyText( uint16 Frame, float XPos, float YPos, uint8 Red, uint8 Green, uint8 Blue, uint8 Trans )
{
	uint16	i;

	i = FindFreeScrPoly();

	if( i != (uint16) -1 )
	{
		if( Trans == 255 ) ScrPolys[ i ].Flags = SCRFLAG_Solid;
		else ScrPolys[ i ].Flags = SCRFLAG_Nothing;
		ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
		ScrPolys[ i ].Pos.x = XPos;
		ScrPolys[ i ].Pos.y = YPos;
		ScrPolys[ i ].R = Red;						// R Colour
		ScrPolys[ i ].G = Green;					// G Colour
		ScrPolys[ i ].B = Blue;						// B Colour
		ScrPolys[ i ].Trans = Trans;				// Amount of transparency
		ScrPolys[ i ].Frame = (float) Frame;		// Animation Frame
		ScrPolys[ i ].FrameRate = 0.0F;
		ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
		ScrPolys[ i ].Frm_Info = &Text512_Header;	// Offset Info
		ScrPolys[ i ].Xscale = 1.0F;
		ScrPolys[ i ].Yscale = 1.0F;
		AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Start CountDown Timer
	Input		:	int16	Minutes
				:	int16	Seconds
	Output		:	Nothing
===================================================================*/
void ResetCountDownBombTag( float Amount )
{
	LONGLONG	Time_Freq;
	LONGLONG	Time_Value;

	CurrentCountDownFont = 0;
	Countdown_Float = Amount;

	QueryPerformanceCounter((LARGE_INTEGER *) &Time_Value);
	QueryPerformanceFrequency((LARGE_INTEGER *) &Time_Freq);
	Old_Time_Float = ( ( Time_Value * 100.0F ) / Time_Freq );

	Toggle = FALSE;
	ScaleInc = 0.0F;
	Interp_Time = 0.0F;
	CountDown_X = TIMERSTARTSCREENX;
	CountDown_Y = TIMERSTARTSCREENY;
	CountDown_Xoff = 0.0F;
	CountDown_Yoff = 0.0F;
	CountDown_Wanted_X = TIMERSTARTSCREENX;
	CountDown_Wanted_Y = TIMERSTARTSCREENY;
	CountDown_Scale = TIMERSTARTSCALE;
	CountDown_Wanted_Scale = TIMERSTARTSCALE;
	CountDown_Col = 1.0F;

	if( Countdown_Float < 1000.0F )
	{
		CurrentCountDownFont = 1;
		CountDown_Wanted_X = TIMERENDSCREENX;
		CountDown_Wanted_Y = TIMERENDSCREENY;
		CountDown_Wanted_Scale = TIMERENDSCALE;
		ScaleInc = ( ( CountDown_Wanted_Scale - CountDown_Scale ) / 420.0F );
		CountDown_Xoff = ( ( CountDown_Wanted_X - CountDown_X ) / 420.0F );
		CountDown_Yoff = ( ( CountDown_Wanted_Y - CountDown_Y ) / 420.0F );
		Interp_Time = 420.0F;

		if( Countdown_Float >= 300.0F )
		{
			Interp_Time = ( ( Countdown_Float - 300.0F ) * ( 600.0F / 1000.0F ) );
			CountDown_X = ( TIMERSTARTSCREENX + ( CountDown_Xoff * ( 420.0F - Interp_Time ) ) );
			CountDown_Y = ( TIMERSTARTSCREENY + ( CountDown_Yoff * ( 420.0F - Interp_Time ) ) );
			CountDown_Scale = ( TIMERSTARTSCALE + ( ScaleInc * ( 420.0F - Interp_Time ) ) );
		}
		else
		{
			Interp_Time = 0.0F;
			CountDown_X = CountDown_Wanted_X;
			CountDown_Y = CountDown_Wanted_Y;
			CountDown_Scale = CountDown_Wanted_Scale;
			CountDown_Col = ( Countdown_Float / 300.0F );
		}
	}
}

/*===================================================================
	Procedure	:	Add Solid ScreenPoly Text
	Input		:	uint16	BoxNumber
				:	float	XPos
				:	float	YPos
				:	float	XScale
				:	float	YScale
				:	uint8	Red
				:	uint8	Green
				:	uint8	Blue
				:	uint8	Trans
	Output		:	void
===================================================================*/
void AddScreenPolyTextScale( uint16 Frame, float XPos, float YPos, float XScale, float YScale, uint8 Red, uint8 Green, uint8 Blue, uint8 Trans )
{
	uint16	i;

	i = FindFreeScrPoly();

	if( i != (uint16) -1 )
	{
		if( Trans == 255 ) ScrPolys[ i ].Flags = SCRFLAG_Solid;
		else ScrPolys[ i ].Flags = SCRFLAG_Nothing;
		ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
		ScrPolys[ i ].Pos.x = XPos;
		ScrPolys[ i ].Pos.y = YPos;
		ScrPolys[ i ].R = Red;						// R Colour
		ScrPolys[ i ].G = Green;					// G Colour
		ScrPolys[ i ].B = Blue;						// B Colour
		ScrPolys[ i ].Trans = Trans;				// Amount of transparency
		ScrPolys[ i ].Frame = (float) Frame;		// Animation Frame
		ScrPolys[ i ].FrameRate = 0.0F;
		ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
		ScrPolys[ i ].Frm_Info = &Text512_Header;	// Offset Info
		ScrPolys[ i ].Xscale = XScale;
		ScrPolys[ i ].Yscale = YScale;
		AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Allocate Screen Multiples
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void ShowScreenMultiples( void )
{
	uint16	i;
	uint16	Model;
	int16	Count;
	VECTOR	Rotation;
	float	Center_X;
	float	Center_Y;

	if( Current_Camera_View != WhoIAm ) return;				// Dont show multiples in external view.

	Center_X = ( MainCamera.Viewport.X + ( MainCamera.Viewport.Width / 2.0F ) );
	Center_Y = ( MainCamera.Viewport.Y + ( MainCamera.Viewport.Height / 2.0F ) );

	for( Count = 0; Count < Ships[ WhoIAm ].NumMultiples; Count++ )
	{
		Model = Ships[ WhoIAm ].OrbModels[ Count ];

		if( Model != (uint16) -1 )
		{
			Rotation.x = (float) sin( D2R( Models[ Model ].AxisRot + 180.0F ) );
			Rotation.y = (float) cos( D2R( Models[ Model ].AxisRot + 180.0F ) );

			i = FindFreeScrPoly();
	
			ScreenMultiples[ Count ] = i;
	
			if( i != (uint16) -1 )
			{
				ScrPolys[ i ].Flags = SCRFLAG_Nothing;
				ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
				ScrPolys[ i ].Pos.x = Center_X - ( Rotation.x * 20.0F );
				ScrPolys[ i ].Pos.y = Center_Y + ( Rotation.y * 20.0F );
				ScrPolys[ i ].R = 255;						// R Colour
				ScrPolys[ i ].G = 255;						// G Colour
				ScrPolys[ i ].B = 255;						// B Colour
				ScrPolys[ i ].Trans = 255;					// Amount of transparency
				ScrPolys[ i ].Frame = 0.0F;
				ScrPolys[ i ].FrameRate = 0.0F;
				ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ i ].Frm_Info = &Orb_Header;		// Offset Info
				ScrPolys[ i ].Xscale = 1.0F;
				ScrPolys[ i ].Yscale = 1.0F;
				AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
			}
		}
	}
}

/*===================================================================
	Procedure	:	Make Screen Flash
				:	uint8	Red
				:	uint8	Green
				:	uint8	Blue
				:	uint8	Trans
	Output		:	Nothing
===================================================================*/
void MakeScreenFlash( uint8 Red, uint8 Green, uint8 Blue, uint8 Trans, uint16 *ScreenPolyStore, int16 SeqNum )
{
	uint16	i;

	if( *ScreenPolyStore != (uint16) -1 )
	{
		i = *ScreenPolyStore;

		if( Trans == 255 )
			ScrPolys[ i ].Flags = SCRFLAG_Solid | SCRFLAG_UseCoords;
		else
			ScrPolys[ i ].Flags = SCRFLAG_UseCoords;

		if( SeqNum != SCRSEQ_FadeUp ) 
		{
			ScrPolys[ i ].LifeCount = 255.0F;
			ScrPolys[ i ].R = Red;						// R Colour
			ScrPolys[ i ].G = Green;					// G Colour
			ScrPolys[ i ].B = Blue;						// B Colour
			ScrPolys[ i ].Trans = Trans;				// Amount of transparency
		}
		else
		{
			ScrPolys[ i ].LifeCount = 0.0F;
			ScrPolys[ i ].R = 0;						// R Colour
			ScrPolys[ i ].G = 0;					// G Colour
			ScrPolys[ i ].B = 0;						// B Colour
			ScrPolys[ i ].Trans = 0;				// Amount of transparency
		}
		ScrPolys[ i ].FadeRed = (float) Red;
		ScrPolys[ i ].FadeGreen = (float) Green;
		ScrPolys[ i ].FadeBlue = (float) Blue;
		ScrPolys[ i ].FadeTrans = (float) Trans;
	}
	else
	{
		i = FindFreeScrPoly();
	
		if( i != (uint16) -1 )
		{
			*ScreenPolyStore = i;
	
			if( Trans == 255 )
				ScrPolys[ i ].Flags = SCRFLAG_Solid | SCRFLAG_UseCoords;
			else
				ScrPolys[ i ].Flags = SCRFLAG_UseCoords;

			if( SeqNum != SCRSEQ_FadeUp ) 
			{
				ScrPolys[ i ].LifeCount = 255.0F;
				ScrPolys[ i ].R = Red;						// R Colour
				ScrPolys[ i ].G = Green;					// G Colour
				ScrPolys[ i ].B = Blue;						// B Colour
				ScrPolys[ i ].Trans = Trans;				// Amount of transparency
			}
			else
			{
				ScrPolys[ i ].LifeCount = 0.0F;
		  	   	ScrPolys[ i ].R = 0;						// R Colour
				ScrPolys[ i ].G = 0;					// G Colour
				ScrPolys[ i ].B = 0;						// B Colour
				ScrPolys[ i ].Trans = 0;				// Amount of transparency
			}

			ScrPolys[ i ].Type = SCRTYPE_Normal; //SCRTYPE_LastAFrame;
			ScrPolys[ i ].Pos.x = 0.0F;
			ScrPolys[ i ].Pos.y = 0.0F;
			ScrPolys[ i ].x1 = 0.0F;
			ScrPolys[ i ].y1 = 0.0F;
			ScrPolys[ i ].x2 = (float) MainCamera.Viewport.Width;
			ScrPolys[ i ].y2 = 0.0F;
			ScrPolys[ i ].x3 = (float) MainCamera.Viewport.Width;
			ScrPolys[ i ].y3 = (float) MainCamera.Viewport.Height;
			ScrPolys[ i ].x4 = 0.0F;
			ScrPolys[ i ].y4 = (float) MainCamera.Viewport.Height;
			ScrPolys[ i ].FadeRed = (float) Red;
			ScrPolys[ i ].FadeGreen = (float) Green;
			ScrPolys[ i ].FadeBlue = (float) Blue;
			ScrPolys[ i ].FadeTrans = (float) Trans;
			ScrPolys[ i ].Frame = 0.0F;
			ScrPolys[ i ].FrameRate = 0.0F;
			ScrPolys[ i ].SeqNum = SeqNum;
			ScrPolys[ i ].Frm_Info = &Solid_Header;		// Offset Info
			ScrPolys[ i ].Xscale = 1.0F;
			ScrPolys[ i ].Yscale = 1.0F;
			AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
		}
	}
}

/*===================================================================
	Procedure	:	Setup Thermo Screenpolys
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitThermo( void )
{
	uint16	i;
	float	Center_X;
	float	Center_Y;

	if( !DecreaseTemperature ) return;

	ThermalTemp = 0.0F;
	WantedThermalTemp = 0.0F;
	ThermalMinimum = 0.0F;

	if( ThermoScrPoly == (uint16) -1 )
	{
		i = FindFreeScrPoly();
	
		if( i != (uint16) -1 )
		{
			ThermoScrPoly = i;

			Center_X = ( CurrentCamera.Viewport.X + CurrentCamera.Viewport.Width - ( CurrentCamera.Viewport.Width / 6.0F ) );
			Center_Y = ( CurrentCamera.Viewport.Y + CurrentCamera.Viewport.Height - ( CurrentCamera.Viewport.Height / 5.0F ) );
			
			ScrPolys[ i ].Flags = SCRFLAG_Scale;
			ScrPolys[ i ].Type = SCRTYPE_Normal;
			ScrPolys[ i ].LifeCount = 255.0F;
			ScrPolys[ i ].Pos.x = Center_X;
			ScrPolys[ i ].Pos.y = Center_Y;
			ScrPolys[ i ].x1 = 0.0F;
			ScrPolys[ i ].y1 = 0.0F;
			ScrPolys[ i ].x2 = 0.0F;
			ScrPolys[ i ].y2 = 0.0F;
			ScrPolys[ i ].x3 = 0.0F;
			ScrPolys[ i ].y3 = 0.0F;
			ScrPolys[ i ].x4 = 0.0F;
			ScrPolys[ i ].y4 = 0.0F;
			ScrPolys[ i ].R = 255;					// R Colour
			ScrPolys[ i ].G = 255;					// G Colour
			ScrPolys[ i ].B = 255;					// B Colour
			ScrPolys[ i ].Trans = 255;				// Amount of transparency
			ScrPolys[ i ].FadeRed = (float) 255.0F;
			ScrPolys[ i ].FadeGreen = (float) 255.0F;
			ScrPolys[ i ].FadeBlue = (float) 255.0F;
			ScrPolys[ i ].FadeTrans = (float) 255.0F;
			ScrPolys[ i ].Frame = 0.0F;
			ScrPolys[ i ].FrameRate = 4.0F;
			ScrPolys[ i ].SeqNum = SCRSEQ_Thermo;
			ScrPolys[ i ].Frm_Info = &Thermo_Header; // Offset Info
			ScrPolys[ i ].Xscale = 1.0F;
			ScrPolys[ i ].Yscale = 1.0F;
			AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
		}
	}
}

/*===================================================================
	Procedure	:	Remove Thermo Screenpolys
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void KillThermo( void )
{
	if( ThermoScrPoly != (uint16) -1 )
	{
		KillUsedScrPoly( ThermoScrPoly );
		ThermoScrPoly = (uint16) -1;
	}
}
	
#if 0
/*===================================================================
	Procedure	:	Check if MMX present
	Input		:	Nothing
	Output		:	BOOL	TRUE/FALSE
===================================================================*/
BOOL IsMMX( void )
{
	int MMX = 0;
    SYSTEM_INFO si;

    GetSystemInfo(&si);

    if (si.dwProcessorType==PROCESSOR_INTEL_PENTIUM) 
	{
		_asm 
		{
			 mov EAX, 1         ; request for feature flags
			_emit 0x0f			; CPUID (00001111 10100010) - This is a Pentium
								; specific instruction which gets information on the
			_emit 0xa2			; processor. A Pentium family processor should set
								; bits 11-8 of eax to 5.
 
			mov MMX, EDX		; is MMX technology Bit(bit 23)in feature      
		}
	}
 	return( !!(MMX & 0x00800000) );
}
#endif

TPAGEINFO	ScrPolyTPages[ MAXTPAGESPERTLOAD + 1 ];

/*===================================================================
	Procedure	:	Init ScreenPoly TPage Groups
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void InitScrPolyTPages( void )
{
	uint16	i;

	for( i = 0; i <= MAXTPAGESPERTLOAD; i++ )
	{
		ScrPolyTPages[ i ].FirstPoly = (uint16) -1;
	}
}

/*===================================================================
	Procedure	:	Add ScreenPoly To TPage Link List
	Input		:	uint16		ScreenPoly Index
				:	uint16		TPage to add to
	Output		:	Nothing
===================================================================*/
void AddScrPolyToTPage( uint16 i, int16 TPage )
{
	if( TPage == -1 ) TPage = MAXTPAGESPERTLOAD;

	ScrPolys[ i ].PrevInTPage = (uint16) -1;
	ScrPolys[ i ].NextInTPage = ScrPolyTPages[ TPage ].FirstPoly;

	if( ScrPolys[ i ].NextInTPage != (uint16) -1 )
	{
		ScrPolys[ ScrPolys[ i ].NextInTPage ].PrevInTPage = i;
	}

	ScrPolyTPages[ TPage ].FirstPoly = i;
}

/*===================================================================
	Procedure	:	Remove ScreenPoly From TPage Link List
	Input		:	uint16		ScreenPoly Index
				:	uint16		TPage to add to
	Output		:	Nothing
===================================================================*/
void RemoveScrPolyFromTPage( uint16 i, int16 TPage )
{
	if( TPage == -1 ) TPage = MAXTPAGESPERTLOAD;

	if( ScrPolys[ i ].PrevInTPage != (uint16) -1 )
	{
		ScrPolys[ ScrPolys[ i ].PrevInTPage ].NextInTPage = ScrPolys[ i ].NextInTPage;
	}
	else
	{
		ScrPolyTPages[ TPage ].FirstPoly = ScrPolys[ i ].NextInTPage;
	}

	if( ScrPolys[ i ].NextInTPage != (uint16) -1 )
	{
		ScrPolys[ ScrPolys[ i ].NextInTPage ].PrevInTPage = ScrPolys[ i ].PrevInTPage;
	}

	ScrPolys[ i ].PrevInTPage = (uint16) -1;
	ScrPolys[ i ].NextInTPage = (uint16) -1;
}

/*===================================================================
	Procedure	:	Display all solid screen Polys
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	LPDIRECT3DDEVICE			D3D Device
				:	LPDIRECT3DVIEWPORT			D3D ViewPort
	Output		:	True/False
===================================================================*/
BOOL DisplaySolidScrPolys( RENDEROBJECT *renderObject )
{
	int16	TPage;
	uint16	i;

	TPage = 0;
	i = ScrPolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !ScrPolyDispSolid( renderObject, &TPage, &i ) )
			return( TRUE );

		disable_zbuff();

		if (FAILED(draw_2d_object(renderObject)))
			return FALSE;

		reset_zbuff();
	}

	return( FALSE );
}

/*===================================================================
	Procedure	:	Display all solid screen Polys
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	LPDIRECT3DDEVICE			D3D Device
				:	LPDIRECT3DVIEWPORT			D3D ViewPort
	Output		:	True/False
===================================================================*/
BOOL DisplayNonSolidScrPolys( RENDEROBJECT *renderObject )
{
	int16	TPage;
	uint16	i;

	TPage = 0;
	i = ScrPolyTPages[ 0 ].FirstPoly;

	while( 1 )
	{
 		if( !ScrPolyDispNonSolid( renderObject, &TPage, &i ) )
			return( TRUE );

		disable_zbuff();

		if (FAILED(draw_2d_object(renderObject)))
			return FALSE;

		reset_zbuff();
	}

	return( FALSE );
}

/*===================================================================
	Procedure	:	Display All Solid Screen Polygons
	Input		:	LPDIRECT3DEXECUTEBUFFER		Execute Buffer
				:	int16	*					Current TPage List
				:	uint16	*					Current ScrPoly
	Output		:	True/False
===================================================================*/
BOOL ScrPolyDispSolid( RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextScrPoly )
{
	uint16			i;
	int16			Count;
	int16			BitCount;
	int16			TotalVerts;
	int16			StartVert;
	int16			NumVerts;
	int16			NumTris;
	float			Xoff;
	float			Yoff;
	float			Xsize;
	float			Ysize;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	D3DCOLOR		Colour;
	D3DCOLOR		Specular;
	LPD3DTLVERTEX	ScrPolyVertPnt; // pre-transformed verts!
	LPD3DTRIANGLE	ScrPolyFacePnt;
    LPD3DTLVERTEX	lpBufStart;//, lpInsStart, lpPointer;
	float			u1,v1,u2,v2;
	float			x1,y1,x2,y2,x3,y3,x4,y4;
	BOOL			Textured;
	WORD			*lpIndices = NULL;
	int				start_index = 0;

/*===================================================================
		Find out how may verts involved in Exec Buffer
===================================================================*/
	TotalVerts = 0;

	for( Count = *TPage; Count <= MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = TotalVerts;
		NumVerts = 0;
		NumTris = 0;

		if( Count == MAXTPAGESPERTLOAD ) Textured = FALSE;
		else Textured = TRUE;

		if( Count == *TPage ) i = *NextScrPoly;
		else i = ScrPolyTPages[ Count ].FirstPoly;

		while( ( i != (uint16) -1 ) && ( ( StartVert + NumVerts ) < MAXSCREENPOLYVERTS ) )
		{
			if( ScrPolys[ i ].Flags & SCRFLAG_Solid )
			{
				if( Textured )
				{
					if( ScrPolys[i].Frm_Info && (*ScrPolys[i].Frm_Info ) )
					{
						Bit_Ptr = ( (*ScrPolys[ i ].Frm_Info)->Bit_Info + (int16) ScrPolys[ i ].Frame );
						NumTris += ( 2 * Bit_Ptr->numbits );
						NumVerts += ( 4 * Bit_Ptr->numbits );
					}
				}
				else
				{
					NumTris += 2;
					NumVerts += 4;
				}
			}
			
			i = ScrPolys[ i ].NextInTPage;
		}

		ScrPolyTPages[ Count ].StartVert = StartVert;
		ScrPolyTPages[ Count ].NumVerts = NumVerts;
		ScrPolyTPages[ Count ].NumTris = NumTris;

		TotalVerts = ( StartVert + NumVerts );
		if( TotalVerts >= MAXSCREENPOLYVERTS ) break;
	}

	if( !TotalVerts ) return( FALSE );

	renderObject->material = Tloadheader.lpMat[ *TPage ];
	renderObject->numTextureGroups = 0;

//	if(d3dappi.CurrDriver != 0)	Specular = RGB_MAKE( 255, 255, 255 );
//	else Specular = RGB_MAKE( 128, 128, 128 );

	Specular = RGB_MAKE( 0, 0, 0 );

	ZValue = 1.0F;
	RHWValue = ( 1.0F / ZValue );

/*===================================================================
		Lock Buffer and get ready to fill in...
===================================================================*/

	if (FAILED(FSLockPretransformedVertexBuffer(renderObject, &lpBufStart)))
	{
		return FALSE;
	}	

	if (FAILED(FSLockIndexBuffer(renderObject, &lpIndices)))
	{
		return FALSE;
	}

	ScrPolyFacePnt = (LPD3DTRIANGLE) lpIndices;

	ScrPolyVertPnt = (LPD3DTLVERTEX) lpBufStart;

/*===================================================================
		Fill in Buffer ( Verts and Faces Simultaneously )
===================================================================*/

	for( Count = *TPage; Count <= MAXTPAGESPERTLOAD; Count++ )
	{
		if( Count == MAXTPAGESPERTLOAD ) Textured = FALSE;
		else Textured = TRUE;

		StartVert = ScrPolyTPages[ Count ].StartVert;
		NumVerts = ScrPolyTPages[ Count ].NumVerts;
		NumTris = ScrPolyTPages[ Count ].NumTris;

		if( NumVerts )
		{
			if( !Textured )
			{
				renderObject->textureGroups[renderObject->numTextureGroups].texture = NULL;
				renderObject->textureGroups[renderObject->numTextureGroups].colourkey = FALSE;
/* bjd - TODO
				OP_STATE_LIGHT( 1, lpPointer );
			   	    STATE_DATA(D3DLIGHTSTATE_MATERIAL, 0, lpPointer);
			   	OP_PROCESS_VERTICES( 1, lpPointer );
			        PROCESSVERTICES_DATA( D3DPROCESSVERTICES_COPY, StartVert, NumVerts, lpPointer );
			   	OP_STATE_RENDER( 1, lpPointer );
			   	    STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, 0, lpPointer);
			   	OP_TRIANGLE_LIST( NumTris, lpPointer );
*/
			}
			else
			{	
				renderObject->textureGroups[renderObject->numTextureGroups].texture = Tloadheader.lpTexture[Count];
				renderObject->textureGroups[renderObject->numTextureGroups].colourkey = Tloadheader.ColourKey[Count];
/* bjd - TODO
				OP_STATE_LIGHT( 1, lpPointer );
			   	    STATE_DATA(D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[ Count ], lpPointer);
			   	OP_PROCESS_VERTICES( 1, lpPointer );
			        PROCESSVERTICES_DATA( D3DPROCESSVERTICES_COPY, StartVert, NumVerts, lpPointer );
			   	OP_STATE_RENDER( 1, lpPointer );
			   	    STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Count ], lpPointer);
			   	OP_TRIANGLE_LIST( NumTris, lpPointer );
*/
			}

	   		//ScrPolyFacePnt = (LPD3DTRIANGLE) lpPointer;
			
			if( Count == *TPage ) i = *NextScrPoly;
			else i = ScrPolyTPages[ Count ].FirstPoly;
	
			while( ( i != (uint16) -1 ) && ( StartVert < MAXSCREENPOLYVERTS ) )
			{
				if( ScrPolys[ i ].Flags & SCRFLAG_Solid )
				{
					if( Textured && ScrPolys[i].Frm_Info && (*ScrPolys[i].Frm_Info ) )
					{
	   					Bit_Ptr = ( (*ScrPolys[ i ].Frm_Info)->Bit_Info + (int16) ScrPolys[ i ].Frame );
	   					Off_Ptr = ( (*ScrPolys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
      		
		  				for( BitCount = 0; BitCount < Bit_Ptr->numbits; BitCount++ )
	      				{
							int ntris = 0;

   	   						Box_Ptr = ( (*ScrPolys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );
   
	      					if( ( ScrPolys[ i ].Flags & SCRFLAG_UseCoords ) )
	      					{
   	   							u1 = Box_Ptr->u1;
   	   							v1 = Box_Ptr->v1;
   	   							u2 = Box_Ptr->u2;
   	   							v2 = Box_Ptr->v2;
      							x1 = ScrPolys[ i ].x1;
      							y1 = ScrPolys[ i ].y1;
      							x2 = ScrPolys[ i ].x2;
      							y2 = ScrPolys[ i ].y2;
      							x3 = ScrPolys[ i ].x3;
      							y3 = ScrPolys[ i ].y3;
      							x4 = ScrPolys[ i ].x4;
      							y4 = ScrPolys[ i ].y4;
      						}
	      					else
		  					{
	      		   				Xoff = ( Off_Ptr->xoff * ScrPolys[ i ].Xscale );
		  		   				Yoff = ( Off_Ptr->yoff * ScrPolys[ i ].Yscale );
      			   				Xsize = ( Box_Ptr->xsize * ScrPolys[ i ].Xscale );
      			   				Ysize = ( Box_Ptr->ysize * ScrPolys[ i ].Yscale );
      						
      							if( ( ScrPolys[ i ].Flags & SCRFLAG_Scale ) )
      							{
      			   					Ysize = ( Ysize / Xsize );
      			   					Xsize = ( ( CurrentCamera.Viewport.Width / 320.0F ) * Xsize );
      			   					Ysize *= ( ( Xsize ) * pixel_aspect_ratio );
      							
      								if( Xoff != 0.0F ) Yoff = ( Yoff / Xoff );
      								else Yoff = 0.0F;
      			   					Xoff = ( ( CurrentCamera.Viewport.Width / 320.0F ) * Xoff );
      			   					Yoff *= ( ( Xoff ) * pixel_aspect_ratio );
      							}
      							
      							switch( Off_Ptr->box & BOX_BOTHFLIP )
      							{
      								case BOX_XFLIP:							// X Flip
      					   				x3 = ( ScrPolys[ i ].Pos.x + Xoff );
      					   				y1 = ( ScrPolys[ i ].Pos.y + Yoff );
      					   				x1 = ( x3 - Xsize );
      					   				y3 = ( y1 + Ysize );
      									u1 = Box_Ptr->u2;
      									v1 = Box_Ptr->v1;
      									u2 = Box_Ptr->u1;
      									v2 = Box_Ptr->v2;
      									break;
      							
      								case BOX_YFLIP:							// Y Flip
      					   				x1 = ( ScrPolys[ i ].Pos.x + Xoff );
      					   				y3 = ( ScrPolys[ i ].Pos.y + Yoff );
      					   				x3 = ( x1 + Xsize );
      					   				y1 = ( y3 - Ysize );
      									u1 = Box_Ptr->u1;
      									v1 = Box_Ptr->v2;
      									u2 = Box_Ptr->u2;
      									v2 = Box_Ptr->v1;
      									break;
      							
      								case BOX_BOTHFLIP:						// X & Y Flip
      					   				x3 = ( ScrPolys[ i ].Pos.x + Xoff );
      					   				y3 = ( ScrPolys[ i ].Pos.y + Yoff );
      					   				x1 = ( x3 - Xsize );
      					   				y1 = ( y3 - Ysize );
      									u1 = Box_Ptr->u2;
      									v1 = Box_Ptr->v2;
      									u2 = Box_Ptr->u1;
      									v2 = Box_Ptr->v1;
      									break;
      							
      								default:								// No Flip
      					   				x1 = ( ScrPolys[ i ].Pos.x + Xoff );
      					   				y1 = ( ScrPolys[ i ].Pos.y + Yoff );
      					   				x3 = ( x1 + Xsize );
      					   				y3 = ( y1 + Ysize );
      									u1 = Box_Ptr->u1;
      									v1 = Box_Ptr->v1;
      									u2 = Box_Ptr->u2;
      									v2 = Box_Ptr->v2;
      									break;
      							
      							}
							}

/*===================================================================
		Create Colour Value for polys
===================================================================*/
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE2( ScrPolys[ i ].R, ScrPolys[ i ].G, ScrPolys[ i ].B, ScrPolys[ i ].Trans );
#else
									Colour = RGBA_MAKE2( ScrPolys[ i ].R, ScrPolys[ i ].G, ScrPolys[ i ].B, 255 );
#endif
									break;
		
								case MCM_Stipple:
									Colour = RGBA_MAKE2( ScrPolys[ i ].R, ScrPolys[ i ].G, ScrPolys[ i ].B, ScrPolys[ i ].Trans / 2 );
									break;
			
								case MCM_Software:
									Colour = RGBA_MAKE2( 128, 128, 128, 255 );
									break;
							}

		      				ScrPolyVertPnt->sx = x1;
		      				ScrPolyVertPnt->sy = y1;
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt->tu = u1;
		      				ScrPolyVertPnt->tv = v1;
		      				ScrPolyVertPnt++;
		      					
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt++;
		      					
		      				ScrPolyVertPnt->sx = x3;
		      				ScrPolyVertPnt->sy = y3;
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt->tu = u2;
		      				ScrPolyVertPnt->tv = v2;
		      				ScrPolyVertPnt++;
		      						
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt++;
    					
		      				if( ClipBox( ScrPolyVertPnt-4, ScrPolyVertPnt-2 ) )
		      				{
		      					( ScrPolyVertPnt - 4 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 4 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 3 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 3 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 2 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 2 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 1 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 1 )->sy = 0.0F;
		      				}

		      				( ScrPolyVertPnt - 3 )->sx = ( ScrPolyVertPnt - 2 )->sx;
		      				( ScrPolyVertPnt - 3 )->sy = ( ScrPolyVertPnt - 4 )->sy;
		      				( ScrPolyVertPnt - 3 )->tu = ( ScrPolyVertPnt - 2 )->tu;
		      				( ScrPolyVertPnt - 3 )->tv = ( ScrPolyVertPnt - 4 )->tv;
		      					
		      				( ScrPolyVertPnt - 1 )->sx = ( ScrPolyVertPnt - 4 )->sx;
		      				( ScrPolyVertPnt - 1 )->sy = ( ScrPolyVertPnt - 2 )->sy;
		      				( ScrPolyVertPnt - 1 )->tu = ( ScrPolyVertPnt - 4 )->tu;
		      				( ScrPolyVertPnt - 1 )->tv = ( ScrPolyVertPnt - 2 )->tv;
		   
	      					if( ( ScrPolys[ i ].Flags & SCRFLAG_Rot90 ) )
							{
								u1 = ( ScrPolyVertPnt - 4 )->tu;
								v1 = ( ScrPolyVertPnt - 4 )->tv;
								( ScrPolyVertPnt - 4 )->tu = ( ScrPolyVertPnt - 3 )->tu;
								( ScrPolyVertPnt - 4 )->tv = ( ScrPolyVertPnt - 3 )->tv;
								( ScrPolyVertPnt - 3 )->tu = ( ScrPolyVertPnt - 2 )->tu;
								( ScrPolyVertPnt - 3 )->tv = ( ScrPolyVertPnt - 2 )->tv;
								( ScrPolyVertPnt - 2 )->tu = ( ScrPolyVertPnt - 1 )->tu;
								( ScrPolyVertPnt - 2 )->tv = ( ScrPolyVertPnt - 1 )->tv;
								( ScrPolyVertPnt - 1 )->tu = u1;
								( ScrPolyVertPnt - 1 )->tv = v1;
							}

		      		   		ScrPolyFacePnt->v1 = 0;
		      		   		ScrPolyFacePnt->v2 = 1;
		      		   		ScrPolyFacePnt->v3 = 2;
//		      		   		ScrPolyFacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
		      		   		ScrPolyFacePnt++;
							ntris++;

		      		   		ScrPolyFacePnt->v1 = 0;
		      		   		ScrPolyFacePnt->v2 = 2;
		      		   		ScrPolyFacePnt->v3 = 3;
//		      		   		ScrPolyFacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
		      		   		ScrPolyFacePnt++;
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
					else
					{
						if( !Textured )
						{
							int ntris = 0;

   							u1 = 0.0F;
							v1 = 0.0F;
							u2 = 1.0F;
							v2 = 1.0F;
							x1 = ScrPolys[ i ].x1;
							y1 = ScrPolys[ i ].y1;
							x2 = ScrPolys[ i ].x2;
							y2 = ScrPolys[ i ].y2;
							x3 = ScrPolys[ i ].x3;
							y3 = ScrPolys[ i ].y3;
							x4 = ScrPolys[ i ].x4;
							y4 = ScrPolys[ i ].y4;
							
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS			
									Colour = RGBA_MAKE2( ScrPolys[ i ].R, ScrPolys[ i ].G, ScrPolys[ i ].B, ScrPolys[ i ].Trans );
#else						
									Colour = RGBA_MAKE2( ScrPolys[ i ].R, ScrPolys[ i ].G, ScrPolys[ i ].B, 255 );
#endif						
									break;
							
								case MCM_Stipple:
									Colour = RGBA_MAKE2( ScrPolys[ i ].R, ScrPolys[ i ].G, ScrPolys[ i ].B, ScrPolys[ i ].Trans / 2 );
									break;
							
								case MCM_Software:
									Colour = RGBA_MAKE2( 128, 128, 128, 255 );
									break;
							}
									
	      					ScrPolyVertPnt->sx = x1;
	      					ScrPolyVertPnt->sy = y1;
	      					ScrPolyVertPnt->sz = ZValue;
	      					ScrPolyVertPnt->color = Colour;
	      					ScrPolyVertPnt->specular = Specular;
	      					ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt->tu = u1;
		      				ScrPolyVertPnt->tv = v1;
		      				ScrPolyVertPnt++;
		      					
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt++;
		      					
		      				ScrPolyVertPnt->sx = x3;
		      				ScrPolyVertPnt->sy = y3;
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt->tu = u2;
		      				ScrPolyVertPnt->tv = v2;
		      				ScrPolyVertPnt++;
		      						
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt++;

		      				if( ClipBox( ScrPolyVertPnt-4, ScrPolyVertPnt-2 ) )
		      				{
		      					( ScrPolyVertPnt - 4 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 4 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 3 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 3 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 2 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 2 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 1 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 1 )->sy = 0.0F;
		      				}
   					
		      				( ScrPolyVertPnt - 3 )->sx = ( ScrPolyVertPnt - 2 )->sx;
		      				( ScrPolyVertPnt - 3 )->sy = ( ScrPolyVertPnt - 4 )->sy;
		      				( ScrPolyVertPnt - 3 )->tu = ( ScrPolyVertPnt - 2 )->tu;
		      				( ScrPolyVertPnt - 3 )->tv = ( ScrPolyVertPnt - 4 )->tv;
		      					
		      				( ScrPolyVertPnt - 1 )->sx = ( ScrPolyVertPnt - 4 )->sx;
		      				( ScrPolyVertPnt - 1 )->sy = ( ScrPolyVertPnt - 2 )->sy;
		      				( ScrPolyVertPnt - 1 )->tu = ( ScrPolyVertPnt - 4 )->tu;
		      				( ScrPolyVertPnt - 1 )->tv = ( ScrPolyVertPnt - 2 )->tv;
							
	      					if( ( ScrPolys[ i ].Flags & SCRFLAG_Rot90 ) )
							{
								u1 = ( ScrPolyVertPnt - 4 )->tu;
								v1 = ( ScrPolyVertPnt - 4 )->tv;
								( ScrPolyVertPnt - 4 )->tu = ( ScrPolyVertPnt - 3 )->tu;
								( ScrPolyVertPnt - 4 )->tv = ( ScrPolyVertPnt - 3 )->tv;
								( ScrPolyVertPnt - 3 )->tu = ( ScrPolyVertPnt - 2 )->tu;
								( ScrPolyVertPnt - 3 )->tv = ( ScrPolyVertPnt - 2 )->tv;
								( ScrPolyVertPnt - 2 )->tu = ( ScrPolyVertPnt - 1 )->tu;
								( ScrPolyVertPnt - 2 )->tv = ( ScrPolyVertPnt - 1 )->tv;
								( ScrPolyVertPnt - 1 )->tu = u1;
								( ScrPolyVertPnt - 1 )->tv = v1;
							}

							ScrPolyFacePnt->v1 = 0;
		      		   		ScrPolyFacePnt->v2 = 1;
		      		   		ScrPolyFacePnt->v3 = 2;
//		      		   		ScrPolyFacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
		      		   		ScrPolyFacePnt++;
							ntris++;

		      		   		ScrPolyFacePnt->v1 = 0;
		      		   		ScrPolyFacePnt->v2 = 2;
		      		   		ScrPolyFacePnt->v3 = 3;
//		      		   		ScrPolyFacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
		      		   		ScrPolyFacePnt++;
							ntris++;

							renderObject->textureGroups[renderObject->numTextureGroups].numTriangles = ntris;
							renderObject->textureGroups[renderObject->numTextureGroups].numVerts = 4;
							renderObject->textureGroups[renderObject->numTextureGroups].startIndex = start_index;
							renderObject->textureGroups[renderObject->numTextureGroups].startVert = StartVert;
							renderObject->textureGroups[renderObject->numTextureGroups].texture = NULL;
							renderObject->textureGroups[renderObject->numTextureGroups].colourkey = FALSE;
							INCREASE_TEXTURE_GROUPS(renderObject);

							start_index += ntris*3; // each triangle has three indexes...
							StartVert += 4;
						}
					}
				}
		
				i = ScrPolys[ i ].NextInTPage;
			}

	   		//lpPointer = ( LPVOID ) ScrPolyFacePnt;
		}

		if( StartVert >= MAXSCREENPOLYVERTS ) break;
	}

//	OP_EXIT( lpPointer );

/*===================================================================
		UnLock Exec Buffer and set data description
===================================================================*/
/*
	if( ExecBuffer->lpVtbl->Unlock( ExecBuffer ) != D3D_OK ) return( FALSE );

	memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
	ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
	ExecBuffer_d3dexdata.dwVertexCount = TotalVerts;
	ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
	ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
	if( ( ExecBuffer->lpVtbl->SetExecuteData( ExecBuffer, &ExecBuffer_d3dexdata ) ) != D3D_OK) return( FALSE );
*/
	if (FAILED(FSUnlockPretransformedVertexBuffer(renderObject)))
	{
		return FALSE;
	}

	if (FAILED(FSUnlockIndexBuffer(renderObject)))
	{
		Msg( "FSUnlockIndexBuffer failed");
		return FALSE ;
	}

	*TPage = Count;
	*NextScrPoly = i;

	return( TRUE );
}

/*===================================================================
	Procedure	:	Display All NonSolid Screen Polygons
	Input		:	RENDEROBJECT  *				RenderObject struct pointer
				:	int16	*					Current TPage List
				:	uint16	*					Current ScrPoly
	Output		:	True/False
===================================================================*/
BOOL ScrPolyDispNonSolid( RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextScrPoly )
{
	uint16			i;
	int16			Count;
	int16			BitCount;
	int16			TotalVerts;
	int16			StartVert;
	int16			NumVerts;
	int16			NumTris;
	float			Xoff;
	float			Yoff;
	float			Xsize;
	float			Ysize;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	D3DCOLOR		Colour;
	D3DCOLOR		Specular;
	LPD3DTLVERTEX	ScrPolyVertPnt; // pre-transformed vertex type!
	LPD3DTRIANGLE	ScrPolyFacePnt;
    LPD3DTLVERTEX	lpBufStart;//, lpInsStart, lpPointer;
	float			u1,v1,u2,v2;
	float			x1,y1,x2,y2,x3,y3,x4,y4;
	BOOL			Textured;
	WORD			*lpIndices = NULL;
	int				start_index = 0;

/*===================================================================
		Find out how may verts involved in Exec Buffer
===================================================================*/
	TotalVerts = 0;

	for( Count = *TPage; Count <= MAXTPAGESPERTLOAD; Count++ )
	{
		StartVert = TotalVerts;
		NumVerts = 0;
		NumTris = 0;

		if( Count == MAXTPAGESPERTLOAD ) Textured = FALSE;
		else Textured = TRUE;

		if( Count == *TPage ) i = *NextScrPoly;
		else i = ScrPolyTPages[ Count ].FirstPoly;

		while( ( i != (uint16) -1 ) && ( ( StartVert + NumVerts ) < MAXSCREENPOLYVERTS ) )
		{
			if( !( ScrPolys[ i ].Flags & SCRFLAG_Solid ) )
			{
				if( Textured )
				{
					if( ScrPolys[i].Frm_Info && (*ScrPolys[i].Frm_Info ) )
					{
						Bit_Ptr = ( (*ScrPolys[ i ].Frm_Info)->Bit_Info + (int16) ScrPolys[ i ].Frame );
						NumTris += ( 2 * Bit_Ptr->numbits );
						NumVerts += ( 4 * Bit_Ptr->numbits );
					}
				}
				else
				{
					NumTris += 2;
					NumVerts += 4;
				}
			}
			
			i = ScrPolys[ i ].NextInTPage;
		}

		ScrPolyTPages[ Count ].StartVert = StartVert;
		ScrPolyTPages[ Count ].NumVerts = NumVerts;
		ScrPolyTPages[ Count ].NumTris = NumTris;

		TotalVerts = ( StartVert + NumVerts );
		if( TotalVerts >= MAXSCREENPOLYVERTS ) break;
	}

	if( !TotalVerts ) return( FALSE );

	renderObject->material = Tloadheader.lpMat[ *TPage ];
	renderObject->numTextureGroups = 0;

	/* bjd curre driver = 0 use to be software mode
	if(d3dappi.CurrDriver != 0)	Specular = RGB_MAKE( 255, 255, 255 );
	else Specular = RGB_MAKE( 128, 128, 128 );
	*/
	Specular = RGB_MAKE( 255, 255, 255 );

	ZValue = 1.0F;
	RHWValue = ( 1.0F / ZValue );

/*===================================================================
		Lock Exec Buffer and get ready to fill in...
===================================================================*/
//	memset( &ExecBuffer_debdesc, 0, sizeof(D3DEXECUTEBUFFERDESC) );
//	ExecBuffer_debdesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
		
//	if( ExecBuffer->lpVtbl->Lock( ExecBuffer, &ExecBuffer_debdesc) != D3D_OK ) return( FALSE );
//	if (FSLockExecuteBuffer(ExecBuffer, &ExecBuffer_debdesc) != D3D_OK )
//		return FALSE;
	if (FAILED(FSLockPretransformedVertexBuffer(renderObject, &lpBufStart)))
	{
		return FALSE;
	}

	if (FAILED(FSLockIndexBuffer(renderObject, &lpIndices)))
	{
		return FALSE;
	}

	ScrPolyFacePnt = (LPD3DTRIANGLE) lpIndices;
		
//	lpBufStart = ExecBuffer_debdesc.lpData;
	ScrPolyVertPnt = (LPD3DTLVERTEX) lpBufStart;
	//lpPointer = (LPVOID) ( ScrPolyVertPnt + TotalVerts );
//	lpInsStart = lpPointer;

/*===================================================================
		Fill in Exec Buffer ( Verts and Faces Simultaneously )
===================================================================*/
	for( Count = *TPage; Count <= MAXTPAGESPERTLOAD; Count++ )
	{
		if( Count == MAXTPAGESPERTLOAD ) Textured = FALSE;
		else Textured = TRUE;

		StartVert = ScrPolyTPages[ Count ].StartVert;
		NumVerts = ScrPolyTPages[ Count ].NumVerts;
		NumTris = ScrPolyTPages[ Count ].NumTris;

		if( NumVerts )
		{
			if( !Textured )
			{
				renderObject->textureGroups[renderObject->numTextureGroups].texture = NULL;
				renderObject->textureGroups[renderObject->numTextureGroups].colourkey = FALSE;
/* bjd - CHECK
				OP_STATE_LIGHT( 1, lpPointer );
			   	    STATE_DATA(D3DLIGHTSTATE_MATERIAL, 0, lpPointer);
			   	OP_PROCESS_VERTICES( 1, lpPointer );
			        PROCESSVERTICES_DATA( D3DPROCESSVERTICES_COPY, StartVert, NumVerts, lpPointer );
			   	OP_STATE_RENDER( 1, lpPointer );
			   	    STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, 0, lpPointer);
			   	OP_TRIANGLE_LIST( NumTris, lpPointer );
*/
			}
			else
			{
				renderObject->textureGroups[renderObject->numTextureGroups].texture = Tloadheader.lpTexture[Count];
				renderObject->textureGroups[renderObject->numTextureGroups].colourkey = Tloadheader.ColourKey[Count];
/* bjd - CHECK
				OP_STATE_LIGHT( 1, lpPointer );
			   	    STATE_DATA(D3DLIGHTSTATE_MATERIAL, Tloadheader.hMat[ Count ], lpPointer);
			   	OP_PROCESS_VERTICES( 1, lpPointer );
			        PROCESSVERTICES_DATA( D3DPROCESSVERTICES_COPY, StartVert, NumVerts, lpPointer );
			   	OP_STATE_RENDER( 1, lpPointer );
			   	    STATE_DATA(D3DRENDERSTATE_TEXTUREHANDLE, Tloadheader.hTex[ Count ], lpPointer);
			   	OP_TRIANGLE_LIST( NumTris, lpPointer );
*/
			}

	   		//ScrPolyFacePnt = (LPD3DTRIANGLE) lpPointer;
			
			if( Count == *TPage ) i = *NextScrPoly;
			else i = ScrPolyTPages[ Count ].FirstPoly;
	
			while( ( i != (uint16) -1 ) && ( StartVert < MAXSCREENPOLYVERTS ) )
			{
				if( !( ScrPolys[ i ].Flags & SCRFLAG_Solid ) )
				{
					if( Textured && ScrPolys[i].Frm_Info && (*ScrPolys[i].Frm_Info ) )
					{
	   					Bit_Ptr = ( (*ScrPolys[ i ].Frm_Info)->Bit_Info + (int16) ScrPolys[ i ].Frame );
	   					Off_Ptr = ( (*ScrPolys[ i ].Frm_Info)->Off_Info + Bit_Ptr->startbit );
      		
		  				for( BitCount = 0; BitCount < Bit_Ptr->numbits; BitCount++ )
	      				{
							int ntris = 0;

   	   						Box_Ptr = ( (*ScrPolys[ i ].Frm_Info)->Box_Info + ( Off_Ptr->box & 0x0fff ) );
   
	      					if( ( ScrPolys[ i ].Flags & SCRFLAG_UseCoords ) )
	      					{
   	   							u1 = Box_Ptr->u1;
   	   							v1 = Box_Ptr->v1;
   	   							u2 = Box_Ptr->u2;
   	   							v2 = Box_Ptr->v2;
      							x1 = ScrPolys[ i ].x1;
      							y1 = ScrPolys[ i ].y1;
      							x2 = ScrPolys[ i ].x2;
      							y2 = ScrPolys[ i ].y2;
      							x3 = ScrPolys[ i ].x3;
      							y3 = ScrPolys[ i ].y3;
      							x4 = ScrPolys[ i ].x4;
      							y4 = ScrPolys[ i ].y4;
      						}
	      					else
		  					{
	      		   				Xoff = ( Off_Ptr->xoff * ScrPolys[ i ].Xscale );
		  		   				Yoff = ( Off_Ptr->yoff * ScrPolys[ i ].Yscale );
      			   				Xsize = ( Box_Ptr->xsize * ScrPolys[ i ].Xscale );
      			   				Ysize = ( Box_Ptr->ysize * ScrPolys[ i ].Yscale );
      						
      							if( ( ScrPolys[ i ].Flags & SCRFLAG_Scale ) )
      							{
      			   					Ysize = ( Ysize / Xsize );
      			   					Xsize = ( ( CurrentCamera.Viewport.Width / 320.0F ) * Xsize );
      			   					Ysize *= ( ( Xsize ) * pixel_aspect_ratio );
      							
      								if( Xoff != 0.0F ) Yoff = ( Yoff / Xoff );
      								else Yoff = 0.0F;
      			   					Xoff = ( ( CurrentCamera.Viewport.Width / 320.0F ) * Xoff );
      			   					Yoff *= ( ( Xoff ) * pixel_aspect_ratio );
      							}
      							
      							switch( Off_Ptr->box & BOX_BOTHFLIP )
      							{
      								case BOX_XFLIP:							// X Flip
      					   				x3 = ( ScrPolys[ i ].Pos.x + Xoff );
      					   				y1 = ( ScrPolys[ i ].Pos.y + Yoff );
      					   				x1 = ( x3 - Xsize );
      					   				y3 = ( y1 + Ysize );
      									u1 = Box_Ptr->u2;
      									v1 = Box_Ptr->v1;
      									u2 = Box_Ptr->u1;
      									v2 = Box_Ptr->v2;
      									break;
      							
      								case BOX_YFLIP:							// Y Flip
      					   				x1 = ( ScrPolys[ i ].Pos.x + Xoff );
      					   				y3 = ( ScrPolys[ i ].Pos.y + Yoff );
      					   				x3 = ( x1 + Xsize );
      					   				y1 = ( y3 - Ysize );
      									u1 = Box_Ptr->u1;
      									v1 = Box_Ptr->v2;
      									u2 = Box_Ptr->u2;
      									v2 = Box_Ptr->v1;
      									break;
      							
      								case BOX_BOTHFLIP:						// X & Y Flip
      					   				x3 = ( ScrPolys[ i ].Pos.x + Xoff );
      					   				y3 = ( ScrPolys[ i ].Pos.y + Yoff );
      					   				x1 = ( x3 - Xsize );
      					   				y1 = ( y3 - Ysize );
      									u1 = Box_Ptr->u2;
      									v1 = Box_Ptr->v2;
      									u2 = Box_Ptr->u1;
      									v2 = Box_Ptr->v1;
      									break;
      							
      								default:								// No Flip
      					   				x1 = ( ScrPolys[ i ].Pos.x + Xoff );
      					   				y1 = ( ScrPolys[ i ].Pos.y + Yoff );
      					   				x3 = ( x1 + Xsize );
      					   				y3 = ( y1 + Ysize );
      									u1 = Box_Ptr->u1;
      									v1 = Box_Ptr->v1;
      									u2 = Box_Ptr->u2;
      									v2 = Box_Ptr->v2;
      									break;
      							}
							}

/*===================================================================
		Create Colour Value for polys
===================================================================*/
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS
									Colour = RGBA_MAKE2( ScrPolys[ i ].R, ScrPolys[ i ].G, ScrPolys[ i ].B, ScrPolys[ i ].Trans );
#else
									Colour = RGBA_MAKE2( ScrPolys[ i ].R, ScrPolys[ i ].G, ScrPolys[ i ].B, 255 );
#endif
									break;
		
								case MCM_Stipple:
									Colour = RGBA_MAKE2( ScrPolys[ i ].R, ScrPolys[ i ].G, ScrPolys[ i ].B, ScrPolys[ i ].Trans / 2 );
									break;
			
								case MCM_Software:
									Colour = RGBA_MAKE2( 128, 128, 128, 255 );
									break;
							}
								
		      				ScrPolyVertPnt->sx = x1;
		      				ScrPolyVertPnt->sy = y1;
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt->tu = u1;
		      				ScrPolyVertPnt->tv = v1;
		      				ScrPolyVertPnt++;
		      					
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt++;
		      					
		      				ScrPolyVertPnt->sx = x3;
		      				ScrPolyVertPnt->sy = y3;
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt->tu = u2;
		      				ScrPolyVertPnt->tv = v2;
		      				ScrPolyVertPnt++;
		      						
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt++;
      					
		      				if( ClipBox( ScrPolyVertPnt-4, ScrPolyVertPnt-2 ) )
		      				{
		      					( ScrPolyVertPnt - 4 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 4 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 3 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 3 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 2 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 2 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 1 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 1 )->sy = 0.0F;
		      				}

		      				( ScrPolyVertPnt - 3 )->sx = ( ScrPolyVertPnt - 2 )->sx;
		      				( ScrPolyVertPnt - 3 )->sy = ( ScrPolyVertPnt - 4 )->sy;
		      				( ScrPolyVertPnt - 3 )->tu = ( ScrPolyVertPnt - 2 )->tu;
		      				( ScrPolyVertPnt - 3 )->tv = ( ScrPolyVertPnt - 4 )->tv;
		      					
		      				( ScrPolyVertPnt - 1 )->sx = ( ScrPolyVertPnt - 4 )->sx;
		      				( ScrPolyVertPnt - 1 )->sy = ( ScrPolyVertPnt - 2 )->sy;
		      				( ScrPolyVertPnt - 1 )->tu = ( ScrPolyVertPnt - 4 )->tu;
		      				( ScrPolyVertPnt - 1 )->tv = ( ScrPolyVertPnt - 2 )->tv;
		   
	      					if( ( ScrPolys[ i ].Flags & SCRFLAG_Rot90 ) )
							{
								u1 = ( ScrPolyVertPnt - 4 )->tu;
								v1 = ( ScrPolyVertPnt - 4 )->tv;
								( ScrPolyVertPnt - 4 )->tu = ( ScrPolyVertPnt - 3 )->tu;
								( ScrPolyVertPnt - 4 )->tv = ( ScrPolyVertPnt - 3 )->tv;
								( ScrPolyVertPnt - 3 )->tu = ( ScrPolyVertPnt - 2 )->tu;
								( ScrPolyVertPnt - 3 )->tv = ( ScrPolyVertPnt - 2 )->tv;
								( ScrPolyVertPnt - 2 )->tu = ( ScrPolyVertPnt - 1 )->tu;
								( ScrPolyVertPnt - 2 )->tv = ( ScrPolyVertPnt - 1 )->tv;
								( ScrPolyVertPnt - 1 )->tu = u1;
								( ScrPolyVertPnt - 1 )->tv = v1;
							}

							ScrPolyFacePnt->v1 = 0;
		      		   		ScrPolyFacePnt->v2 = 1;
		      		   		ScrPolyFacePnt->v3 = 2;
//		      		   		ScrPolyFacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
		      		   		ScrPolyFacePnt++;
							ntris++;

		      		   		ScrPolyFacePnt->v1 = 0;
		      		   		ScrPolyFacePnt->v2 = 2;
		      		   		ScrPolyFacePnt->v3 = 3;
//		      		   		ScrPolyFacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
		      		   		ScrPolyFacePnt++;
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
					else
					{
						if( !Textured )
						{
							int ntris = 0;

   							u1 = 0.0F;
							v1 = 0.0F;
							u2 = 1.0F;
							v2 = 1.0F;
							x1 = ScrPolys[ i ].x1;
							y1 = ScrPolys[ i ].y1;
							x2 = ScrPolys[ i ].x2;
							y2 = ScrPolys[ i ].y2;
							x3 = ScrPolys[ i ].x3;
							y3 = ScrPolys[ i ].y3;
							x4 = ScrPolys[ i ].x4;
							y4 = ScrPolys[ i ].y4;
							
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS			
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col1.R, ScrPolys[ i ].Col1.G, ScrPolys[ i ].Col1.B, ScrPolys[ i ].Col1.Trans );
#else																						      				        
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col1.R, ScrPolys[ i ].Col1.G, ScrPolys[ i ].Col1.B, 255 );
#endif																						      				        
									break;													      				        
																							      				        
								case MCM_Stipple:											      				        
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col1.R, ScrPolys[ i ].Col1.G, ScrPolys[ i ].Col1.B, ScrPolys[ i ].Col1.Trans / 2 );
									break;
							
								case MCM_Software:
									Colour = RGBA_MAKE2( 128, 128, 128, 255 );
									break;
							}
									
	      					ScrPolyVertPnt->sx = x1;
	      					ScrPolyVertPnt->sy = y1;
	      					ScrPolyVertPnt->sz = ZValue;
	      					ScrPolyVertPnt->color = Colour;
	      					ScrPolyVertPnt->specular = Specular;
	      					ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt->tu = u1;
		      				ScrPolyVertPnt->tv = v1;
		      				ScrPolyVertPnt++;
		      					
							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS			
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col2.R, ScrPolys[ i ].Col2.G, ScrPolys[ i ].Col2.B, ScrPolys[ i ].Col2.Trans );
#else																						      				        
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col2.R, ScrPolys[ i ].Col2.G, ScrPolys[ i ].Col2.B, 255 );
#endif																						      				        
									break;													      				        
																							      				        
								case MCM_Stipple:											      				        
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col2.R, ScrPolys[ i ].Col2.G, ScrPolys[ i ].Col2.B, ScrPolys[ i ].Col2.Trans / 2 );
									break;
							
								case MCM_Software:
									Colour = RGBA_MAKE2( 128, 128, 128, 255 );
									break;
							}

		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt++;

							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS			
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col3.R, ScrPolys[ i ].Col3.G, ScrPolys[ i ].Col3.B, ScrPolys[ i ].Col3.Trans );
#else																						      				        
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col3.R, ScrPolys[ i ].Col3.G, ScrPolys[ i ].Col3.B, 255 );
#endif																						      				        
									break;													      				        
																							      				        
								case MCM_Stipple:											      				        
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col3.R, ScrPolys[ i ].Col3.G, ScrPolys[ i ].Col3.B, ScrPolys[ i ].Col3.Trans / 2 );
									break;
							
								case MCM_Software:
									Colour = RGBA_MAKE2( 128, 128, 128, 255 );
									break;
							}
							
		      				ScrPolyVertPnt->sx = x3;
		      				ScrPolyVertPnt->sy = y3;
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt->tu = u2;
		      				ScrPolyVertPnt->tv = v2;
		      				ScrPolyVertPnt++;

							switch( MakeColourMode )
							{
								case MCM_Normal:
#if ACTUAL_TRANS			
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col4.R, ScrPolys[ i ].Col4.G, ScrPolys[ i ].Col4.B, ScrPolys[ i ].Col4.Trans );
#else																						      				        
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col4.R, ScrPolys[ i ].Col4.G, ScrPolys[ i ].Col4.B, 255 );
#endif																						      				        
									break;													      				        
																							      				        
								case MCM_Stipple:											      				        
									Colour = RGBA_MAKE2( ScrPolys[ i ].Col4.R, ScrPolys[ i ].Col4.G, ScrPolys[ i ].Col4.B, ScrPolys[ i ].Col4.Trans / 2 );
									break;
							
								case MCM_Software:
									Colour = RGBA_MAKE2( 128, 128, 128, 255 );
									break;
							}
							
		      				ScrPolyVertPnt->sz = ZValue;
		      				ScrPolyVertPnt->color = Colour;
		      				ScrPolyVertPnt->specular = Specular;
		      				ScrPolyVertPnt->rhw = RHWValue;
		      				ScrPolyVertPnt++;
  					
		      				if( ClipBox( ScrPolyVertPnt-4, ScrPolyVertPnt-2 ) )
		      				{
		      					( ScrPolyVertPnt - 4 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 4 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 3 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 3 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 2 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 2 )->sy = 0.0F;
		      					( ScrPolyVertPnt - 1 )->sx = 0.0F;
		      					( ScrPolyVertPnt - 1 )->sy = 0.0F;
		      				}
 					
		      				( ScrPolyVertPnt - 3 )->sx = ( ScrPolyVertPnt - 2 )->sx;
		      				( ScrPolyVertPnt - 3 )->sy = ( ScrPolyVertPnt - 4 )->sy;
		      				( ScrPolyVertPnt - 3 )->tu = ( ScrPolyVertPnt - 2 )->tu;
		      				( ScrPolyVertPnt - 3 )->tv = ( ScrPolyVertPnt - 4 )->tv;
		      					
		      				( ScrPolyVertPnt - 1 )->sx = ( ScrPolyVertPnt - 4 )->sx;
		      				( ScrPolyVertPnt - 1 )->sy = ( ScrPolyVertPnt - 2 )->sy;
		      				( ScrPolyVertPnt - 1 )->tu = ( ScrPolyVertPnt - 4 )->tu;
		      				( ScrPolyVertPnt - 1 )->tv = ( ScrPolyVertPnt - 2 )->tv;
							
	      					if( ( ScrPolys[ i ].Flags & SCRFLAG_Rot90 ) )
							{
								u1 = ( ScrPolyVertPnt - 4 )->tu;
								v1 = ( ScrPolyVertPnt - 4 )->tv;
								( ScrPolyVertPnt - 4 )->tu = ( ScrPolyVertPnt - 3 )->tu;
								( ScrPolyVertPnt - 4 )->tv = ( ScrPolyVertPnt - 3 )->tv;
								( ScrPolyVertPnt - 3 )->tu = ( ScrPolyVertPnt - 2 )->tu;
								( ScrPolyVertPnt - 3 )->tv = ( ScrPolyVertPnt - 2 )->tv;
								( ScrPolyVertPnt - 2 )->tu = ( ScrPolyVertPnt - 1 )->tu;
								( ScrPolyVertPnt - 2 )->tv = ( ScrPolyVertPnt - 1 )->tv;
								( ScrPolyVertPnt - 1 )->tu = u1;
								( ScrPolyVertPnt - 1 )->tv = v1;
							}

							ScrPolyFacePnt->v1 = 0;
		      		   		ScrPolyFacePnt->v2 = 1;
		      		   		ScrPolyFacePnt->v3 = 2;
//		      		   		ScrPolyFacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
		      		   		ScrPolyFacePnt++;
							ntris++;

		      		   		ScrPolyFacePnt->v1 = 0;
		      		   		ScrPolyFacePnt->v2 = 2;
		      		   		ScrPolyFacePnt->v3 = 3;
//		      		   		ScrPolyFacePnt->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
		      		   		ScrPolyFacePnt++;
							ntris++;
      						
							renderObject->textureGroups[renderObject->numTextureGroups].numTriangles = ntris;
							renderObject->textureGroups[renderObject->numTextureGroups].numVerts = 4;
							renderObject->textureGroups[renderObject->numTextureGroups].startIndex = start_index;
							renderObject->textureGroups[renderObject->numTextureGroups].startVert = StartVert;
							renderObject->textureGroups[renderObject->numTextureGroups].texture = NULL;
							renderObject->textureGroups[renderObject->numTextureGroups].colourkey = FALSE;
							INCREASE_TEXTURE_GROUPS(renderObject);

							start_index += ntris*3; // each triangle has three indexes...
							StartVert += 4;
						}
					}
				}
		
				i = ScrPolys[ i ].NextInTPage;
			}

	   		//lpPointer = ( LPVOID ) ScrPolyFacePnt;
		}

		if( StartVert >= MAXSCREENPOLYVERTS ) break;
	}

//	OP_EXIT( lpPointer );

/*===================================================================
		UnLock Exec Buffer and set data description
===================================================================*/
/*
	if( ExecBuffer->lpVtbl->Unlock( ExecBuffer ) != D3D_OK ) return( FALSE );

	memset( &ExecBuffer_d3dexdata, 0, sizeof(D3DEXECUTEDATA) );
	ExecBuffer_d3dexdata.dwSize = sizeof(D3DEXECUTEDATA);
	ExecBuffer_d3dexdata.dwVertexCount = TotalVerts;
	ExecBuffer_d3dexdata.dwInstructionOffset = (ULONG) ( (char *) lpInsStart - (char *) lpBufStart );
	ExecBuffer_d3dexdata.dwInstructionLength = (ULONG) ( (char *) lpPointer - (char *) lpInsStart );
	if( ( ExecBuffer->lpVtbl->SetExecuteData( ExecBuffer, &ExecBuffer_d3dexdata ) ) != D3D_OK) return( FALSE );
*/
	if (FAILED(FSUnlockPretransformedVertexBuffer(renderObject)))
	{
		return FALSE;
	}

	if (FAILED(FSUnlockIndexBuffer(renderObject)))
	{
		Msg( "FSUnlockIndexBuffer failed");
		return FALSE ;
	}

	*TPage = Count;
	*NextScrPoly = i;

	return( TRUE );
}

/*===================================================================
	Procedure	:	Load Time for level
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void LoadTimeForLevel( void )
{
#if 0

	char		NewFilename[ 256 ];
	char	*	NewExt = ".TIM";
	int			Minutes;
	FILE	*	f;

	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NewFilename[ 0 ], NewExt );

	f = fopen( &NewFilename[ 0 ], "r" );

	if( f )
	{
		if ( fscanf( f, " %d", &Minutes ) == 1 )
		{
			CountDownOn = TRUE;
			SinglePlayerTimeLimit = Minutes;
			KilledPlayer = FALSE;
		}
		else
		{
			CountDownOn = FALSE;
			SinglePlayerTimeLimit = 0;
		}

		fclose( f );
	}
	else
	{
		CountDownOn = FALSE;
		SinglePlayerTimeLimit = 0;
	}

#else

	CountDownOn = FALSE;
	SinglePlayerTimeLimit = 0;

#endif
}

/*===================================================================
	Procedure	:	Save ScreenPolys arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveScreenPolys( FILE * fp )
{
	if( fp )
	{
		fwrite( &CountDownOn, sizeof( CountDownOn ), 1, fp );
		fwrite( &Countdown_Float, sizeof( Countdown_Float ), 1, fp );
		fwrite( &ThermalTemp, sizeof( ThermalTemp ), 1, fp );
		fwrite( &WantedThermalTemp, sizeof( WantedThermalTemp ), 1, fp );
		fwrite( &ThermalMinimum, sizeof( ThermalMinimum ), 1, fp );
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load ScreenPolys Arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadScreenPolys( FILE * fp )
{
	float			Time;

	if( fp )
	{
		fread( &CountDownOn, sizeof( CountDownOn ), 1, fp );
		fread( &Time, sizeof( Countdown_Float ), 1, fp );
		fread( &ThermalTemp, sizeof( ThermalTemp ), 1, fp );
		fread( &WantedThermalTemp, sizeof( WantedThermalTemp ), 1, fp );
		fread( &ThermalMinimum, sizeof( ThermalMinimum ), 1, fp );

		if( CountDownOn )
		{
			CountDownOn = TRUE;
			KilledPlayer = FALSE;
			CreateCountdownDigits();
			StartCountDown( 1, 0 );
			TimeLimitTrigger = NULL;
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Create Flag Above Players Head
	Input		:	uint16		Ship
	Output		:	Nothing
===================================================================*/
void ShowFlagOnShip( uint16 Ship )
{
	uint16			i;
	MATRIX			FinalMatrix;
	VECTOR			TempPos;
	VECTOR			Pos;
	VECTOR			Trans1, Trans2;
	VECTOR			Pos1, Pos2;
	VECTOR			ScrPos1, ScrPos2;
	VECTOR			DirVector;
	float			Dist;
	float			Xsize, Ysize;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;

	if( !( Ships[ Ship ].Object.Flags & SHIP_CarryingFlag ) )
		return;

   	GetFinalMatrix( &FinalMatrix );
	ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans1 );
	ApplyMatrix( &CurrentCamera.Mat, &SlideRight, &Trans2 );
	Trans1.x *= ( SHIP_RADIUS * 0.5F );
	Trans1.y *= ( SHIP_RADIUS * 0.5F );
	Trans1.z *= ( SHIP_RADIUS * 0.5F );
	Trans2.x *= ( SHIP_RADIUS * 0.5F );
	Trans2.y *= ( SHIP_RADIUS * 0.5F );
	Trans2.z *= ( SHIP_RADIUS * 0.5F );
	
	Pos1.x = ( Ships[ Ship ].Object.Pos.x + Trans1.x - Trans2.x );
	Pos1.y = ( Ships[ Ship ].Object.Pos.y + Trans1.y - Trans2.y );
	Pos1.z = ( Ships[ Ship ].Object.Pos.z + Trans1.z - Trans2.z );

	Pos2.x = ( Ships[ Ship ].Object.Pos.x - Trans1.x + Trans2.x );
	Pos2.y = ( Ships[ Ship ].Object.Pos.y - Trans1.y + Trans2.y );
	Pos2.z = ( Ships[ Ship ].Object.Pos.z - Trans1.z + Trans2.z );
	
	Conv3DTo2D( &Pos1, &ScrPos1, &FinalMatrix );
	Conv3DTo2D( &Pos2, &ScrPos2, &FinalMatrix );

	Bit_Ptr = ( Flag_Header->Bit_Info + 0 );
	Off_Ptr = ( Flag_Header->Off_Info + Bit_Ptr->startbit );
	Box_Ptr = ( Flag_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

	Xsize = ( ( ScrPos2.x - ScrPos1.x ) / Box_Ptr->xsize ) * 1.8F;
	Ysize = ( ( ScrPos2.y - ScrPos1.y ) / Box_Ptr->ysize ) * 1.0F;

	DirVector.x = ( Ships[ Ship ].Object.Pos.x - CurrentCamera.Pos.x );
	DirVector.y = ( Ships[ Ship ].Object.Pos.y - CurrentCamera.Pos.y );
	DirVector.z = ( Ships[ Ship ].Object.Pos.z - CurrentCamera.Pos.z );
	Dist = VectorLength( &DirVector );

	ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans1 );

	TempPos.x = ( Ships[ Ship ].Object.Pos.x + ( Trans1.x * 100.0F ) );
	TempPos.y = ( Ships[ Ship ].Object.Pos.y + ( Trans1.y * 100.0F ) );
	TempPos.z = ( Ships[ Ship ].Object.Pos.z + ( Trans1.z * 100.0F ) );

	Conv3DTo2D( &TempPos, &Pos, &FinalMatrix );

	i = FindFreeScrPoly();
	if( i != (uint16) -1 )
	{
		ScrPolys[ i ].Flags = SCRFLAG_Nothing;
		ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
		ScrPolys[ i ].Pos.x = Pos.x;
		ScrPolys[ i ].Pos.y = Pos.y;
		ScrPolys[ i ].R = 255;
		ScrPolys[ i ].G = 255;
		ScrPolys[ i ].B = 255;
		ScrPolys[ i ].Trans = 255;
		ScrPolys[ i ].Frame = 0.0F;					// Animation Frame
		ScrPolys[ i ].FrameRate = 1.0F;
		ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
		ScrPolys[ i ].Frm_Info = &Flag_Header;	// Offset Info
		ScrPolys[ i ].Xscale = Xsize;
		ScrPolys[ i ].Yscale = Ysize;
		AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create Bounty Above Players Head
	Input		:	uint16		Ship
	Output		:	Nothing
===================================================================*/
void ShowBountyOnShip( uint16 Ship )
{
	uint16			i;
	MATRIX			FinalMatrix;
	VECTOR			TempPos;
	VECTOR			Pos;
	VECTOR			Trans1, Trans2;
	VECTOR			Pos1, Pos2;
	VECTOR			ScrPos1, ScrPos2;
	VECTOR			DirVector;
	float			Dist;
	float			Xsize, Ysize;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;

	if( !( Ships[ Ship ].Object.Flags & SHIP_CarryingBounty ) )
		return;

   	GetFinalMatrix( &FinalMatrix );
	ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans1 );
	ApplyMatrix( &CurrentCamera.Mat, &SlideRight, &Trans2 );
	Trans1.x *= SHIP_RADIUS;
	Trans1.y *= SHIP_RADIUS;
	Trans1.z *= SHIP_RADIUS;
	Trans2.x *= SHIP_RADIUS;
	Trans2.y *= SHIP_RADIUS;
	Trans2.z *= SHIP_RADIUS;
	
	Pos1.x = ( Ships[ Ship ].Object.Pos.x + Trans1.x - Trans2.x );
	Pos1.y = ( Ships[ Ship ].Object.Pos.y + Trans1.y - Trans2.y );
	Pos1.z = ( Ships[ Ship ].Object.Pos.z + Trans1.z - Trans2.z );

	Pos2.x = ( Ships[ Ship ].Object.Pos.x - Trans1.x + Trans2.x );
	Pos2.y = ( Ships[ Ship ].Object.Pos.y - Trans1.y + Trans2.y );
	Pos2.z = ( Ships[ Ship ].Object.Pos.z - Trans1.z + Trans2.z );
	
	Conv3DTo2D( &Pos1, &ScrPos1, &FinalMatrix );
	Conv3DTo2D( &Pos2, &ScrPos2, &FinalMatrix );

	Bit_Ptr = ( Flag_Header->Bit_Info + 1 );
	Off_Ptr = ( Flag_Header->Off_Info + Bit_Ptr->startbit );
	Box_Ptr = ( Flag_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

	Xsize = ( ( ScrPos2.x - ScrPos1.x ) / Box_Ptr->xsize ) * 0.75F;
	Ysize = ( ( ScrPos2.y - ScrPos1.y ) / Box_Ptr->ysize ) * 0.75F;

	DirVector.x = ( Ships[ Ship ].Object.Pos.x - CurrentCamera.Pos.x );
	DirVector.y = ( Ships[ Ship ].Object.Pos.y - CurrentCamera.Pos.y );
	DirVector.z = ( Ships[ Ship ].Object.Pos.z - CurrentCamera.Pos.z );
	Dist = VectorLength( &DirVector );

	ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans1 );

	TempPos.x = ( Ships[ Ship ].Object.Pos.x + ( Trans1.x * 100.0F ) );
	TempPos.y = ( Ships[ Ship ].Object.Pos.y + ( Trans1.y * 100.0F ) );
	TempPos.z = ( Ships[ Ship ].Object.Pos.z + ( Trans1.z * 100.0F ) );

	Conv3DTo2D( &TempPos, &Pos, &FinalMatrix );

	i = FindFreeScrPoly();
	if( i != (uint16) -1 )
	{
		ScrPolys[ i ].Flags = SCRFLAG_Nothing;
		ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
		ScrPolys[ i ].Pos.x = Pos.x;
		ScrPolys[ i ].Pos.y = Pos.y;
		ScrPolys[ i ].R = 255;
		ScrPolys[ i ].G = 255;
		ScrPolys[ i ].B = 255;
		ScrPolys[ i ].Trans = 255;
		ScrPolys[ i ].Frame = 1.0F;					// Animation Frame
		ScrPolys[ i ].FrameRate = 1.0F;
		ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
		ScrPolys[ i ].Frm_Info = &Flag_Header;	// Offset Info
		ScrPolys[ i ].Xscale = Xsize;
		ScrPolys[ i ].Yscale = Ysize;
		AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create Flag(s) Above Players Head
	Input		:	uint16		Ship
	Output		:	Nothing
===================================================================*/
VECTOR	TeamOffsets[ MAX_TEAMS ][ MAX_TEAMS ] = {

	{ { 0.0F, 0.0F, 0.0F },			// 1 flag
	  { 0.0F, 0.0F, 0.0F },
	  { 0.0F, 0.0F, 0.0F },
	  { 0.0F, 0.0F, 0.0F } },

	{ { -5.0F, -5.0F, 0.0F },			// 2 flags
	  { +5.0F, +5.0F, 0.0F },
	  { 0.0F, 0.0F, 0.0F },
	  { 0.0F, 0.0F, 0.0F } },

	{ { -10.0F, -10.0F, 0.0F },			// 3 flags
	  { 0.0F, 0.0F, 0.0F },
	  { +10.0F, +10.0F, 0.0F },
	  { 0.0F, 0.0F, 0.0F } },

	{ { -15.0F, -15.0F, 0.0F },			// 4 flags
	  { -5.0F, -5.0F, 0.0F },
	  { +5.0F, +5.0F, 0.0F },
	  { +15.0F, +15.0F, 0.0F } },
};

void ShowCTFFlagsOnShip( uint16 Ship )
{
	uint16			i;
	int16			NumFlags;
	MATRIX			FinalMatrix;
	VECTOR			TempPos;
	VECTOR			Pos;
	VECTOR			Trans1, Trans2;
	VECTOR			Pos1, Pos2;
	VECTOR			ScrPos1, ScrPos2;
	VECTOR			DirVector;
	float			Dist;
	float			Xsize, Ysize;
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	int16			Count;
	int16			Team;

	NumFlags = 0;

	for( Count = 0; Count < MAX_TEAMS; Count++ )
	{
		if( ( Ships[ Ship ].Object.Flags & TeamFlagMask[ Count ] ) )
		{
			NumFlags++;
		}
	}

	if( NumFlags )
	{
   		GetFinalMatrix( &FinalMatrix );
		ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans1 );
		ApplyMatrix( &CurrentCamera.Mat, &SlideRight, &Trans2 );
		Trans1.x *= ( SHIP_RADIUS * 0.5F );
		Trans1.y *= ( SHIP_RADIUS * 0.5F );
		Trans1.z *= ( SHIP_RADIUS * 0.5F );
		Trans2.x *= ( SHIP_RADIUS * 0.5F );
		Trans2.y *= ( SHIP_RADIUS * 0.5F );
		Trans2.z *= ( SHIP_RADIUS * 0.5F );
		
		Pos1.x = ( Ships[ Ship ].Object.Pos.x + Trans1.x - Trans2.x );
		Pos1.y = ( Ships[ Ship ].Object.Pos.y + Trans1.y - Trans2.y );
		Pos1.z = ( Ships[ Ship ].Object.Pos.z + Trans1.z - Trans2.z );

		Pos2.x = ( Ships[ Ship ].Object.Pos.x - Trans1.x + Trans2.x );
		Pos2.y = ( Ships[ Ship ].Object.Pos.y - Trans1.y + Trans2.y );
		Pos2.z = ( Ships[ Ship ].Object.Pos.z - Trans1.z + Trans2.z );
		
		Conv3DTo2D( &Pos1, &ScrPos1, &FinalMatrix );
		Conv3DTo2D( &Pos2, &ScrPos2, &FinalMatrix );

		Bit_Ptr = ( Flags_Header->Bit_Info + 0 );
		Off_Ptr = ( Flags_Header->Off_Info + Bit_Ptr->startbit );
		Box_Ptr = ( Flags_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

		Xsize = ( ( ScrPos2.x - ScrPos1.x ) / Box_Ptr->xsize ) * 1.8F;
		Ysize = ( ( ScrPos2.y - ScrPos1.y ) / Box_Ptr->ysize ) * 1.0F;

		DirVector.x = ( Ships[ Ship ].Object.Pos.x - CurrentCamera.Pos.x );
		DirVector.y = ( Ships[ Ship ].Object.Pos.y - CurrentCamera.Pos.y );
		DirVector.z = ( Ships[ Ship ].Object.Pos.z - CurrentCamera.Pos.z );
		Dist = VectorLength( &DirVector );

		ApplyMatrix( &CurrentCamera.Mat, &SlideUp, &Trans1 );
		ApplyMatrix( &CurrentCamera.Mat, &SlideRight, &Trans2 );

		Team = 0;

		for( Count = 0; Count < MAX_TEAMS; Count++ )
		{
			if( ( Ships[ Ship ].Object.Flags & TeamFlagMask[ Count ] ) )
			{
				Pos.x = TeamOffsets[ NumFlags -1 ][ Team ].x;
				Pos.y = TeamOffsets[ NumFlags -1 ][ Team ].y;
				Pos.z = TeamOffsets[ NumFlags -1 ][ Team ].z;
				ApplyMatrix( &CurrentCamera.Mat, &Pos, &TempPos );

				TempPos.x += ( Ships[ Ship ].Object.Pos.x + ( Trans1.x * 120.0F ) );
				TempPos.y += ( Ships[ Ship ].Object.Pos.y + ( Trans1.y * 120.0F ) );
				TempPos.z += ( Ships[ Ship ].Object.Pos.z + ( Trans1.z * 120.0F ) );

				Conv3DTo2D( &TempPos, &Pos, &FinalMatrix );

				i = FindFreeScrPoly();

				if( i != (uint16) -1 )
				{
					ScrPolys[ i ].Flags = SCRFLAG_Solid;
					ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
					ScrPolys[ i ].Pos.x = Pos.x;
					ScrPolys[ i ].Pos.y = Pos.y;
					ScrPolys[ i ].R = 255;
					ScrPolys[ i ].G = 255;
					ScrPolys[ i ].B = 255;
					ScrPolys[ i ].Trans = 255;
					ScrPolys[ i ].Frame = (float) Count;					// Animation Frame
					ScrPolys[ i ].FrameRate = 1.0F;
					ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
					ScrPolys[ i ].Frm_Info = &Flags_Header;	// Offset Info
					ScrPolys[ i ].Xscale = Xsize;
					ScrPolys[ i ].Yscale = Ysize;
					AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
				}

				Team++;
			}
		}
	}
}

/*===================================================================
	Procedure	:	Show Flag on screen for player who has flag
	Input		:	uint16		Ship
	Output		:	Nothing
===================================================================*/
void ShowFlag( uint16 Ship )
{
	uint16	i;

	if( !( Ships[ Ship ].Object.Flags & SHIP_CarryingFlag ) )
		return;

	i = FindFreeScrPoly();
	if( i != (uint16) -1 )
	{
		ScrPolys[ i ].Flags = SCRFLAG_Nothing;
		ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
		ScrPolys[ i ].Pos.x = ( CurrentCamera.Viewport.Width / 2.0F );
		ScrPolys[ i ].Pos.y = ( CurrentCamera.Viewport.Height - 30.0F );
		ScrPolys[ i ].R = 255;
		ScrPolys[ i ].G = 255;
		ScrPolys[ i ].B = 255;
		ScrPolys[ i ].Trans = 255;
		ScrPolys[ i ].Frame = 0.0F;					// Animation Frame
		ScrPolys[ i ].FrameRate = 1.0F;
		ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
		ScrPolys[ i ].Frm_Info = &Flag_Header;	// Offset Info
		ScrPolys[ i ].Xscale = 0.8F;
		ScrPolys[ i ].Yscale = 0.8F;
		AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Show Bounty on screen for player who has flag
	Input		:	uint16		Ship
	Output		:	Nothing
===================================================================*/
void ShowBounty( uint16 Ship )
{
	uint16	i;

	if( !( Ships[ Ship ].Object.Flags & SHIP_CarryingBounty ) )
		return;

	i = FindFreeScrPoly();
	if( i != (uint16) -1 )
	{
		ScrPolys[ i ].Flags = SCRFLAG_Nothing;
		ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
		ScrPolys[ i ].Pos.x = ( CurrentCamera.Viewport.Width / 2.0F );
		ScrPolys[ i ].Pos.y = ( CurrentCamera.Viewport.Height - 30.0F );
		ScrPolys[ i ].R = 255;
		ScrPolys[ i ].G = 255;
		ScrPolys[ i ].B = 255;
		ScrPolys[ i ].Trans = 255;
		ScrPolys[ i ].Frame = 1.0F;					// Animation Frame
		ScrPolys[ i ].FrameRate = 1.0F;
		ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
		ScrPolys[ i ].Frm_Info = &Flag_Header;	// Offset Info
		ScrPolys[ i ].Xscale = 0.8F;
		ScrPolys[ i ].Yscale = 0.8F;
		AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Show Flag(s) on screen for player who has flag
	Input		:	uint16		Ship
	Output		:	Nothing
===================================================================*/
void ShowCTFFlags( uint16 Ship )
{
	uint16	i;
	int16	Count;
	VECTOR	Pos;
	VECTOR	CenterPos;
	int16	NumFlags;
	int16	Team;

	NumFlags = 0;

	for( Count = 0; Count < MAX_TEAMS; Count++ )
	{
		if( ( Ships[ Ship ].Object.Flags & TeamFlagMask[ Count ] ) )
		{
			NumFlags++;
		}
	}

	if( NumFlags )
	{
		CenterPos.x = ( CurrentCamera.Viewport.Width / 2.0F );
		CenterPos.y = ( CurrentCamera.Viewport.Height - 30.0F );

		Team = 0;

		for( Count = 0; Count < MAX_TEAMS; Count++ )
		{
			if( ( Ships[ Ship ].Object.Flags & TeamFlagMask[ Count ] ) )
			{
				Pos.x = CenterPos.x + TeamOffsets[ NumFlags -1 ][ Team ].x;
				Pos.y = CenterPos.y + TeamOffsets[ NumFlags -1 ][ Team ].y;

				i = FindFreeScrPoly();

				if( i != (uint16) -1 )
				{
					ScrPolys[ i ].Flags = SCRFLAG_Solid;
					ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
					ScrPolys[ i ].Pos.x = Pos.x;
					ScrPolys[ i ].Pos.y = Pos.y;
					ScrPolys[ i ].R = 255;
					ScrPolys[ i ].G = 255;
					ScrPolys[ i ].B = 255;
					ScrPolys[ i ].Trans = 255;
					ScrPolys[ i ].Frame = (float) Count;					// Animation Frame
					ScrPolys[ i ].FrameRate = 1.0F;
					ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
					ScrPolys[ i ].Frm_Info = &Flags_Header;	// Offset Info
					ScrPolys[ i ].Xscale = 0.5F;
					ScrPolys[ i ].Yscale = 0.5F;
					AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, 0 ) );
				}

				Team++;
			}
		}
	}
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif
