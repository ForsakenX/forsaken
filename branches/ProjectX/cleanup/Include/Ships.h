/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 50 $
 *
 * $Header: /PcProjectX/Include/Ships.h 50    23/02/98 10:35 Collinsd $
 *
 * $Log: /PcProjectX/Include/Ships.h $
 * 
 * 50    23/02/98 10:35 Collinsd
 * Added Ship Load/Save Functions
 * 
 * 49    19/02/98 22:01 Collinsd
 * Added flygirl biker.
 * 
 * 48    1/27/98 2:58p Phillipd
 * 
 * 47    22/01/98 11:57 Collinsd
 * Added DoDamage override invul.
 * 
 * 46    18/01/98 23:46 Philipy
 * add shield critical & hull critical levels
 * 
 * 45    1/05/98 3:18p Phillipd
 * 
 * 44    10/30/97 12:40p Phillipd
 * 
 * 43    10/14/97 4:49p Phillipd
 * 
 * 42    10/07/97 11:33a Phillipd
 * 
 * 41    6/08/97 19:21 Collinsd
 * Changed external/internal forces. Commented out some more A3D Sfx stuff
 * 
 * 40    16-04-97 4:38p Collinsd
 * Added reflecting bullets for all weapons except laser.
 * Cheat mode now has invul effect.
 * 
 * 39    2/25/97 12:32p Phillipd
 * rotation interpilation...
 * 
 * 38    2/24/97 10:00a Phillipd
 * Demo mode with multispeed has been added..
 * 
 * 37    12/27/96 3:38p Phillipd
 * Primary.h Secondary.h pickups.h are now clean....
 * Still Lots to do though.....
 * 
 * 36    12/27/96 12:33p Phillipd
 * all files are not dependant on mydplay.h...just some..
 * including it several times in the same files didnt help..
 * 
 * 35    19/12/96 16:16 Oliverc
 * Added debug options menu with "go to room" function
 * 
 * 34    29/11/96 12:24 Oliverc
 * 
 * 33    28/11/96 11:42 Oliverc
 * 
 * 32    4/11/96 10:44 Oliverc
 * Changed display routines to clip to visible portal boundaries of each
 * group
 * 
 * 31    10/10/96 4:18p Phillipd
 * removed mesh.h..no longer used..
 * 
 * 30    3/10/96 14:04 Collinsd
 * Added nitro limit.
 * 
 * 29    2/10/96 18:04 Collinsd
 * Fixed pickup messages when you already have something.  Collision now
 * uses ray to sphere 2.
 * 
 * 28    9/26/96 11:21a Phillipd
 * MX load is now operational...
 * 
 * 27    19/09/96 9:20 Oliverc
 * Converted mouse input to use DirectInput instead of dmouse.vxd
 * 
 * 26    9/17/96 4:18p Phillipd
 * Limbo Mode added.....Dont do anything to the ship...
 * 
 * 25    9/11/96 10:30a Phillipd
 * 
 * 24    10/09/96 16:33 Oliverc
 * Added external/internal force vector to ship movement
 * 
 * 23    9/10/96 9:00a Phillipd
 * 
 * 22    9/09/96 11:23a Phillipd
 * 
 * 21    6/09/96 9:13 Collinsd
 * Timer missile works now.
 * 
 * 20    8/23/96 2:41p Phillipd
 * 
 * 19    23/08/96 9:18 Collinsd
 * All weapons have limits. New pickups in but not correct.
 * 
 * 18    13/08/96 14:57 Collinsd
 * Now prints strings to tell you what you've  picked up.
 * 
 * 17    8/08/96 9:13 Collinsd
 * Added Sfx and pickups
 * 
 * 16    8/08/96 8:48a Phillipd
 * 
 * 15    8/07/96 10:59a Phillipd
 * 
 * 14    6/08/96 12:39 Collinsd
 * You can now pickup pickups. added 2 new models
 * 
 * 13    8/02/96 5:41p Phillipd
 * 
 * 12    7/29/96 4:43p Phillipd
 * 
 * 11    7/25/96 5:38p Phillipd
 * 
 * 10    7/22/96 3:51p Phillipd
 * 
 * 9     7/16/96 11:11a Phillipd
 * moved all visipoly stuff to visi.c and visi.h..
 * 
 * 8     2/07/96 15:43 Oliverc
 * 
 * 7     27/06/96 12:59 Oliverc
 * 
 * 6     6/27/96 9:39a Phillipd
 * Major rearange of ships and oct2....
 * 
 * 5     6/26/96 3:55p Phillipd
 * 
 * 4     6/26/96 12:20p Phillipd
 * 
 * 3     6/25/96 3:15p Phillipd
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

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
#include "d3ddemo.h"
#include "typedefs.h"
#include "New3D.h"
#include <time.h>
#include "exechand.h"
#include "mload.h"
#include "quat.h"
#include "collision.h"
#include "visi.h"
#include "sfx.h"
#include "models.h"
#include "mxload.h"

#define DEG2RAD(D)				((D) * PI / 180.0F)

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
BOOL Mod_Ship_Exec_Buffer( uint16 group, LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );
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

//	Mode Functions...
void RemoteCameraMode0( GLOBALSHIP * ShipPnt , BYTE i );
void RemoteCameraMode1( GLOBALSHIP * ShipPnt , BYTE i );
void RemoteCameraMode3( GLOBALSHIP * ShipPnt , BYTE i );

void ReleaseComponentedShips( void );

FILE * SaveShips( FILE * fp );
FILE * LoadShips( FILE * fp );

#endif


