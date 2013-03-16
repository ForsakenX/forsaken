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
#include "main.h"

#include "new3d.h"
#include "ships.h"
#include "mload.h"
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
//#define	TIMERSTARTSCALE		( 0.59565F )
#define	TIMERSTARTSCALE		( 0.3F )
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

	u_int8_t	R;
	u_int8_t	G;
	u_int8_t	B;
	u_int8_t	Trans;

} SCRPOLY_RGB;

typedef struct SCRPOLY {

	u_int16_t			Next;
	u_int16_t			Prev;

	u_int16_t			NextInTPage;
	u_int16_t			PrevInTPage;

	u_int16_t			Type;				// Type
	u_int16_t			Flags;				// Flags
	VECTOR			Pos;				// Position
	u_int8_t			R;					// Colour
	u_int8_t			G;					// Colour
	u_int8_t			B;					// Colour
	u_int8_t			Trans;				// Amount of transparency
	float			Xscale;				// XScale
	float			Yscale;				// XScale
	float			Frame;				// Animation Frame
	float			FrameRate;			// FrameRate
	FRAME_INFO	**	Frm_Info;			// Offset Info
	int16_t			SeqNum;				// FrameRate
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
void KillUsedScrPoly( u_int16_t i );
u_int16_t FindFreeScrPoly( void );
void ScreenPolyProcess( void );
void DoLensflareEffect( void );
void Conv3DTo2D( VECTOR * SrcVert, VECTOR * DstVert, MATRIX * FinalMat );
bool ClipConv3DTo2D( VECTOR * SrcVert, VECTOR * DstVert, MATRIX * FinalMat );
void KillScrOneFramers( void );
//bjd - CHECK bool ClipBox( LPTLVERTEX topleft, LPTLVERTEX bottomright );
void DoAllSecBullLensflare( void );
void SecBullLensflare( u_int16_t i );
void CreateCountdownDigits( void );
void DeleteCountdownDigits( void );
void CreateDigit( u_int16_t * DigitArray, float XPos, float YPos );
void UpdateCountdownDigits( void );
void UpdateDigit( u_int16_t * DigitArray, float XPos, float YPos, int16_t Number, float Scale, float Col ,FRAME_INFO	** Graphics );
void CreateSeperatorDigit( u_int16_t * DigitArray, float XPos, float YPos );
void DeleteDigit( u_int16_t * DigitArray );
void DeleteSeperatorDigit( u_int16_t * DigitArray );
void StartCountDown( int16_t Minutes, int16_t Seconds );
void ClearCountdownBuffers( void );
void AddScreenPolyText( u_int16_t Frame, float XPos, float YPos, u_int8_t Red, u_int8_t Green, u_int8_t Blue, u_int8_t Trans );
void AddScreenPolyTextColor( u_int16_t Frame, float XPos, float YPos, int Color, u_int8_t Trans );
void AddScreenPolyTextScale( u_int16_t Frame, float XPos, float YPos, float XScale, float YScale, u_int8_t Red, u_int8_t Green, u_int8_t Blue, u_int8_t Trans );
void ShowScreenMultiples( void );
void MakeScreenFlash( u_int8_t Red, u_int8_t Green, u_int8_t Blue, u_int8_t Trans, u_int16_t *ScreenPolyStore, int16_t SeqNum );
void InitThermo( void );
void KillThermo( void );
void ResetCountDownBombTag( float Amount );
void InitScrPolyTPages( void );
void AddScrPolyToTPage( u_int16_t i, int16_t TPage );
void RemoveScrPolyFromTPage( u_int16_t i, int16_t TPage );
bool DisplaySolidScrPolys( RENDEROBJECT *renderObject );
bool DisplayNonSolidScrPolys( RENDEROBJECT *renderObject );
bool ScrPolyDispSolid( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextScrPoly );
bool ScrPolyDispNonSolid( /*LPDIRECT3DEXECUTEBUFFER ExecBuff*/RENDEROBJECT *renderObject, int16_t * TPage, u_int16_t * NextScrPoly );
void FreeAllLastAFrameScrPolys( void );
void LoadTimeForLevel( void );
FILE * SaveScreenPolys( FILE * fp );
FILE * LoadScreenPolys( FILE * fp );
void ShowFlagOnShip( u_int16_t Ship );
void ShowFlag( u_int16_t Ship );
void ShowBountyOnShip( u_int16_t Ship );
void ShowBounty( u_int16_t Ship );
void ShowCTFFlagsOnShip( u_int16_t Ship );
void ShowCTFFlags( u_int16_t Ship );

#endif

