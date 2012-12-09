

/*==========================================================================
 *
 *
 *  File:       ships.h
 *  Content:    shipstuff include file
 *
 *
 ***************************************************************************/

#ifndef SHIPS_INCLUDED
#define SHIPS_INCLUDED

#include "main.h"
#include <math.h>
#include "new3d.h"
#include "mload.h"
#include "quat.h"
#include "collision.h"
#include "visi.h"
#include "sfx.h"
#include "models.h"
#include "mxload.h"

extern bool SwitchedToWatchMode;

#define DEG2RAD(D)				((D) * PI / 180.0F)

#define RESPAWN_TIMER -3.5F // = -250.0F / 71.0f - framelag hack reversal

//#define	SHIP_RADIUS	( 256.0F * GLOBAL_SCALE )
#define	SHIP_RADIUS	( 300.0F * GLOBAL_SCALE )
#define MOVE_TOLERANCE ( 2.0F * GLOBAL_SCALE )
#define	MAX_NITRO_FUEL	( 100.0F )
#define	NITROLOSS	( 0.08333F )

#define SHIELD_CRITICAL_LEVEL ( 10.0F )
#define HULL_CRITICAL_LEVEL ( 64.0F )

#define	OVERRIDE_INVUL		( true )
#define	DONT_OVERRIDE_INVUL	( false )
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

bool SetUpShips();
bool ProcessShips();
void	InitShipsChangeLevel( MLOADHEADER * Mloadheader );
bool	ENV( MXLOADHEADER * Mloadheader , MATRIX * Mat ,VECTOR * Pos);
int16_t DoDamage( bool OverrideInvul );

void SetBikeMods( u_int16_t Bike );

void AddKill( void );
void InitMultiSfxHandle( void );

// force ship to move
void ForceExternal( u_int16_t ship, VECTOR *force );
void ForceInternal( u_int16_t ship, VECTOR *force );
void ForceExternalOneOff( u_int16_t ship, VECTOR *force );
void ForceInternalOneOff( u_int16_t ship, VECTOR *force );

void StartShipScreenShake( float Force );
void StopShipScreenShake( void );

// force ship to rotate
void RotateExternal( u_int16_t ship, VECTOR *point, VECTOR *dir, float force );
void RotateInternal( u_int16_t ship, float xrot, float yrot, float zrot );


void InitShipRandomPos( int16_t i );
void InitShipStartPos( int16_t ship, int16_t pos );
bool GotoRoom( int16_t i, char *roomname );

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


