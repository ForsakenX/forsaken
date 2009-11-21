/*==========================================================================
 *
 *
 *  File:       ScreenPolys.h
 *  Content:    ScreenPolys.h include file
 *
 *
 ***************************************************************************/

#ifndef SCREENPOLYS_INCLUDED
#define SCREENPOLYS_INCLUDED

/*===================================================================
	Includes
===================================================================*/
#include <math.h>

#include "New3D.h"
#include <time.h>
#include "Ships.h"
#include "Mload.h"
#include "collision.h"
#include "lights.h"
#include "2dtextures.h"
#include "tload.h"

/*===================================================================
	Defines
===================================================================*/
#define MAXNUMOFSCRPOLYS	1000
#define	MAXSCREENPOLYVERTS	800
#define	TIMERSTARTSCREENX	( 320.0F - 64.0F )
#define	TIMERSTARTSCREENY	( 240.0F - 36.0F )
#define	TIMERSTARTSCALE		( 0.59565F )
#define	TIMERENDSCREENX		( 160.0F )
#define	TIMERENDSCREENY		( 120.0F )
#define	TIMERENDSCALE		( 1.0F )

/*===================================================================
	Types
===================================================================*/
#define	SCRTYPE_Normal		0
#define	SCRTYPE_LastAFrame	1

/*===================================================================
	Flags
===================================================================*/
#define	SCRFLAG_Nothing		0
#define	SCRFLAG_Scale		1
#define	SCRFLAG_UseCoords	2
#define	SCRFLAG_Onscreen	4
#define	SCRFLAG_Solid		8
#define	SCRFLAG_Rot90		16

/*===================================================================
	Process Sequences
===================================================================*/
#define	SCRSEQ_Nothing		0
#define	SCRSEQ_Anim			1
#define	SCRSEQ_Fade			2
#define	SCRSEQ_Thermo		3
#define	SCRSEQ_Logo			4
#define	SCRSEQ_FadeUp 		5

/*===================================================================
	Structures
===================================================================*/
typedef	struct SCRPOLY_RGB {

	uint8	R;
	uint8	G;
	uint8	B;
	uint8	Trans;

} SCRPOLY_RGB;

typedef struct SCRPOLY {

	uint16			Next;
	uint16			Prev;

	uint16			NextInTPage;
	uint16			PrevInTPage;

	uint16			Type;				// Type
	uint16			Flags;				// Flags
	VECTOR			Pos;				// Position
	uint8			R;					// Colour
	uint8			G;					// Colour
	uint8			B;					// Colour
	uint8			Trans;				// Amount of transparency
	float			Xscale;				// XScale
	float			Yscale;				// XScale
	float			Frame;				// Animation Frame
	float			FrameRate;			// FrameRate
	FRAME_INFO	**	Frm_Info;			// Offset Info
	int16			SeqNum;				// FrameRate
	float			LifeCount;			// LifeCount
	float			FadeRed;			// Red
	float			FadeGreen;			// Green
	float			FadeBlue;			// Blue
	float			FadeTrans;			// Trans
	float			x1;					// x1
	float			y1;					// y1
	float			x2;					// x2
	float			y2;					// y2
	float			x3;					// x3
	float			y3;					// y3
	float			x4;					// x4
	float			y4;					// y4
	SCRPOLY_RGB		Col1;				// Colour 1
	SCRPOLY_RGB		Col2;				// Colour 2
	SCRPOLY_RGB		Col3;				// Colour 3
	SCRPOLY_RGB		Col4;				// Colour 4

} SCRPOLY;

/*===================================================================
	Protptypes
===================================================================*/
void InitScrPolys( void );
void KillUsedScrPoly( uint16 i );
uint16 FindFreeScrPoly( void );
void ScreenPolyProcess( void );
void DoLensflareEffect( void );
void Conv3DTo2D( VECTOR * SrcVert, VECTOR * DstVert, MATRIX * FinalMat );
BOOL ClipConv3DTo2D( VECTOR * SrcVert, VECTOR * DstVert, MATRIX * FinalMat );
void KillScrOneFramers( void );
//bjd - CHECK BOOL ClipBox( LPTLVERTEX topleft, LPTLVERTEX bottomright );
void DoAllSecBullLensflare( void );
void SecBullLensflare( uint16 i );
void CreateCountdownDigits( void );
void DeleteCountdownDigits( void );
void CreateDigit( uint16 * DigitArray, float XPos, float YPos );
void UpdateCountdownDigits( void );
void UpdateDigit( uint16 * DigitArray, float XPos, float YPos, int16 Number, float Scale, float Col ,FRAME_INFO	** Graphics );
void CreateSeperatorDigit( uint16 * DigitArray, float XPos, float YPos );
void DeleteDigit( uint16 * DigitArray );
void DeleteSeperatorDigit( uint16 * DigitArray );
void StartCountDown( int16 Minutes, int16 Seconds );
void ClearCountdownBuffers( void );
void AddScreenPolyText( uint16 Frame, float XPos, float YPos, uint8 Red, uint8 Green, uint8 Blue, uint8 Trans );
void AddScreenPolyTextScale( uint16 Frame, float XPos, float YPos, float XScale, float YScale, uint8 Red, uint8 Green, uint8 Blue, uint8 Trans );
void ShowScreenMultiples( void );
void MakeScreenFlash( uint8 Red, uint8 Green, uint8 Blue, uint8 Trans, uint16 *ScreenPolyStore, int16 SeqNum );
void InitThermo( void );
void KillThermo( void );
void ResetCountDownBombTag( float Amount );
void InitScrPolyTPages( void );
void AddScrPolyToTPage( uint16 i, int16 TPage );
void RemoveScrPolyFromTPage( uint16 i, int16 TPage );
BOOL DisplaySolidScrPolys( RENDEROBJECT *renderObject );
BOOL DisplayNonSolidScrPolys( RENDEROBJECT *renderObject );
BOOL ScrPolyDispSolid( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextScrPoly );
BOOL ScrPolyDispNonSolid( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject, int16 * TPage, uint16 * NextScrPoly );
void FreeAllLastAFrameScrPolys( void );
void LoadTimeForLevel( void );
FILE * SaveScreenPolys( FILE * fp );
FILE * LoadScreenPolys( FILE * fp );
void ShowFlagOnShip( uint16 Ship );
void ShowFlag( uint16 Ship );
void ShowBountyOnShip( uint16 Ship );
void ShowBounty( uint16 Ship );
void ShowCTFFlagsOnShip( uint16 Ship );
void ShowCTFFlags( uint16 Ship );

#endif

