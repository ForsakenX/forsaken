

/*==========================================================================
 *
 *
 *  File:       Ships.h
 *  Content:    shipstuff include file
 *
 *
 ***************************************************************************/

#ifndef SHIPS_INCLUDED
#define SHIPS_INCLUDED

#include <math.h>
#include <d3d9.h>
#include "typedefs.h"
#include "New3D.h"
#include <time.h>
#include "mload.h"
#include "quat.h"
#include "collision.h"
#include "visi.h"
#include "sfx.h"
#include "models.h"
#include "mxload.h"

#define DEG2RAD(D)				((D) * PI / 180.0F)

#define RESPAWN_TIMER -250.0F

//#define	SHIP_RADIUS	( 256.0F * GLOBAL_SCALE )
#define	SHIP_RADIUS	( 300.0F * GLOBAL_SCALE )
#define MOVE_TOLERANCE ( 2.0F * GLOBAL_SCALE )
#define	MAX_NITRO_FUEL	( 100.0F )
#define	NITROLOSS	( 0.08333F )

#define SHIELD_CRITICAL_LEVEL ( 10.0F )
#define HULL_CRITICAL_LEVEL ( 64.0F )

#define	OVERRIDE_INVUL		( TRUE )
#define	DONT_OVERRIDE_INVUL	( FALSE )
/*
 * Structures...
 */
typedef struct _BIKEMOD
{
	float	MaxMoveSpeed;
	float	MoveAccell;		
	float	MoveDecell;		
	float	MaxTurboSpeed;   
	float	TurboAccell;		
	float	TurboDecell;		
	float	MaxTurnSpeed;	
	float	TurnAccell;		
	float	TurnDecell;		
	float	MaxRollSpeed;	
	float	RollAccell;		
	float	RollDecell;		
	float	MaxBankAngle;    
	float	BankAccell;		
	float	BankDecell;
	float	Start_Shield;
	float	Start_Hull;
	float	AutoLevel;
	
} BIKEMOD,*LPBIKEMOD;


/*
 * fn prototypes
 */

BOOL SetUpShips();
BOOL ProcessShips();
void	InitShipsChangeLevel( MLOADHEADER * Mloadheader );
BOOL	ENV( MXLOADHEADER * Mloadheader , MATRIX * Mat ,VECTOR * Pos);
int16 DoDamage( BOOL OverrideInvul );

void SetBikeMods( uint16 Bike );

void AddKill( void );
void InitMultiSfxHandle( void );

// force ship to move
void ForceExternal( uint16 ship, VECTOR *force );
void ForceInternal( uint16 ship, VECTOR *force );
void ForceExternalOneOff( uint16 ship, VECTOR *force );
void ForceInternalOneOff( uint16 ship, VECTOR *force );

void StartShipScreenShake( float Force );
void StopShipScreenShake( void );

// force ship to rotate
void RotateExternal( uint16 ship, VECTOR *point, VECTOR *dir, float force );
void RotateInternal( uint16 ship, float xrot, float yrot, float zrot );


void InitShipRandomPos( int16 i );
void InitShipStartPos( int16 ship, int16 pos );
BOOL GotoRoom( int16 i, char *roomname );

void Process_Remote_Camera( void );
//	Mode Functions...
void ShipMode0( GLOBALSHIP * ShipPnt , BYTE i );
void ShipMode1( GLOBALSHIP * ShipPnt , BYTE i );
void ShipMode2( GLOBALSHIP * ShipPnt , BYTE i );
void ShipMode4( GLOBALSHIP * ShipPnt , BYTE i );
void WatchMode5( GLOBALSHIP * ShipPnt , BYTE i );

//	Mode Functions...
void RemoteCameraMode0( GLOBALSHIP * ShipPnt , BYTE i );
void RemoteCameraMode1( GLOBALSHIP * ShipPnt , BYTE i );
void RemoteCameraMode3( GLOBALSHIP * ShipPnt , BYTE i );

void ReleaseComponentedShips( void );

FILE * SaveShips( FILE * fp );
FILE * LoadShips( FILE * fp );

#endif


