
#include <stdio.h>
#include <math.h>

#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "mload.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "node.h"
#include "networking.h"
#include "ships.h"
#include "2dpolys.h"
#include "lights.h"
#include "models.h"
#include "text.h"
#include "primary.h"
#include "secondary.h"
#include "triggers.h"
#include "sphere.h"
#include "pickups.h"
#include "enemies.h"
#include "controls.h"
#include "ai.h"
#include "lines.h"

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

//--------------------------------------------------------------------------
// Externals
//--------------------------------------------------------------------------

extern	float framelag;
extern	VECTOR	Forward;
extern	VECTOR	SlideRight;
extern	VECTOR	SlideUp;
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	LINE			Lines[ MAXLINES ];
extern	NODENETWORKHEADER	NodeNetworkHeader;
extern	SECONDARYWEAPONBULLET	SecBulls[MAXSECONDARYWEAPONBULLETS];
extern	u_int16_t		FirstSecBullUsed;
bool WouldObjectCollide( OBJECT *Obj, VECTOR *Move_Off, float radius, BGOBJECT **BGObject );

//--------------------------------------------------------------------------
// Global Data
//--------------------------------------------------------------------------

TARGETDATA			TargetInfo;

TARGETDATA *		Tinfo = & TargetInfo;   


AIMDATA			AimData;
/*===================================================================
	Procedure	:	Find Closest Mine
	Input		:	void
	Output		:	void
===================================================================*/
void AI_GetNearestMINETarget( void )
{
	u_int16_t	i;
	float	dist;

	i = FirstSecBullUsed;
	while( i != (u_int16_t) -1 )
	{
		if( SecBulls[ i ].SecType == SEC_MINE )
		{
			// can there groups see each other???
			if( !SoundInfo[ SecBulls[i].GroupImIn ][ Tinfo->Group ] )
			{
				// make sure its a Players mine
				if( SecBulls[i].OwnerType == OWNER_SHIP  )
				{
					dist = DistanceVector2Vector( &SecBulls[i].Pos , &Tinfo->Pos );
					if( dist < Tinfo->Nearest )
					{
						Tinfo->TObject = (void*) &SecBulls[i];
						Tinfo->TFlags = _TARGET_MINES;
						Tinfo->Nearest = dist;         
					}
				}
			}
		}
		i = SecBulls[ i ].Prev;											/* Next SecBull */
	}																				
}
/*===================================================================
	Procedure	:	Find Nearest Ship in any group...
	Input		:	void
	Output		:	void
===================================================================*/
void AI_GetNearestSHIPTargetInAnyGROUP( void )
{
#ifdef COOPERATIVE
	float	dist;
	int	i;
	for( i = 0 ; i < MAX_PLAYERS ; i++ )
	{
		if( Ships[i].enable && (Ships[i].Object.Mode == NORMAL_MODE) && ( &Ships[i].Object != Tinfo->SObject ) && ( !(Ships[i].Object.Flags & SHIP_Stealth) || (Ships[i].Object.Flags & SHIP_Litup) ) )
		{
			if( !SoundInfo[Tinfo->Group][Ships[i].Object.Group] )
			{
				dist = DistanceVector2Vector( &Ships[i].Object.Pos , &Tinfo->Pos );
				if( Ships[i].Object.Noise != 0.0F )
				{
					dist *= ( 1.0F * ( 0.2F * Ships[i].Object.Noise ) );
				}
				
				if( dist < Tinfo->Nearest )
				{
					Tinfo->TObject = (void*) &Ships[i];
					Tinfo->Nearest = dist;         
				}
			}
		}
	}
#else
	if( Ships[WhoIAm].enable && (Ships[WhoIAm].Object.Mode == NORMAL_MODE) && ( !(Ships[WhoIAm].Object.Flags & SHIP_Stealth) || (Ships[WhoIAm].Object.Flags & SHIP_Litup) ) )
	{
		Tinfo->TObject = (void*) &Ships[WhoIAm];
		Tinfo->Nearest = DistanceVector2Vector( &Ships[WhoIAm].Object.Pos , &Tinfo->Pos);         
	}
#endif
}
/*===================================================================
	Procedure	:	AI Look Ahead and try and predict where someone will be...
	Output		:	float amount , VECTOR * Pos, OBJECT	* TargetObject , VECTOR * NewPos , float BulletSpeed
	Output		:	Nothing
===================================================================*/
void AI_LookAhead( float Accuracy, VECTOR * SPos ,OBJECT * TObject , VECTOR * NewPos , float SpeedOfBullet )
{
	VECTOR	Move_Off;
	VECTOR	Move_Dir;
	float	Speed;
	float	Time;
	float	Distance;
	
	if( Accuracy == 0.0F )
	{
		NewPos->x = TObject->Pos.x;
		NewPos->y = TObject->Pos.y;
		NewPos->z = TObject->Pos.z;
		return;
	}
#if 1
	Speed = TObject->Speed.x;
	ApplyMatrix( &TObject->Mat, &SlideRight, &Move_Dir );			/* Calc Direction Vector */
	Move_Off.x = ( Move_Dir.x * Speed );
	Move_Off.y = ( Move_Dir.y * Speed );
	Move_Off.z = ( Move_Dir.z * Speed );
	Speed = TObject->Speed.y;
	ApplyMatrix( &TObject->Mat, &SlideUp, &Move_Dir );			/* Calc Direction Vector */
	Move_Off.x += ( Move_Dir.x * Speed );
	Move_Off.y += ( Move_Dir.y * Speed );
	Move_Off.z += ( Move_Dir.z * Speed );
	Speed = TObject->Speed.z;
	ApplyMatrix( &TObject->Mat, &Forward, &Move_Dir );			/* Calc Direction Vector */
	Move_Off.x += ( Move_Dir.x * Speed );
	Move_Off.y += ( Move_Dir.y * Speed );
	Move_Off.z += ( Move_Dir.z * Speed );
#else
	Move_Off.x = ( TObject->Move_Off.x );
	Move_Off.y = ( TObject->Move_Off.y );
	Move_Off.z = ( TObject->Move_Off.z );
#endif
	
	Distance = DistanceVector2Vector( &TObject->Pos , SPos  );
	Time = ( Distance / SpeedOfBullet );
	
	Time *= Accuracy;
	NewPos->x = TObject->Pos.x + ( Move_Off.x * Time );
	NewPos->y = TObject->Pos.y + ( Move_Off.y * Time );
	NewPos->z = TObject->Pos.z + ( Move_Off.z * Time );
}



/*===================================================================
	Procedure	:	AI Find the best least hassle free Next Node...
	Input		:	OBJECT	*	Object 
	Output		:	Nothing
===================================================================*/
void AI_GetNearestNODETarget( OBJECT * Object )
{
	float Angle = 0.0f;
	int i;
	float TempAngle;
	NODE * Node;
	NODE * NodeLink;
	bool	FoundOne = false;

	if( !NodeNetworkHeader.State )
		return;
	if( !Object->NearestNode )
		return;
	Node = (NODE *) Object->NearestNode;

	for( i = 0 ; i < Node->NumOfLinks ; i ++ )
	{
		NodeLink = Node->NodeLink[i];
		if( NodeLink->NetMask & Object->NodeNetwork )
		{
			AI_AimAtTarget( &Object->InvMat , &Object->Pos, &NodeLink->Pos );
			
			TempAngle = (AimData.Angle.x*AimData.Angle.x) + (AimData.Angle.y*AimData.Angle.y);
			
			if( !FoundOne || ( (TempAngle < Angle) && ( Random_Range(2) || !(NodeLink->Flags&NODE_DECISION) ) ) )
			{
				FoundOne = true;
				Angle = TempAngle;
				Tinfo->TObject = (void*) NodeLink;
				
			}
		}

	}
}
/*===================================================================
	Procedure	:	AI Aim At a target...
	Input		:	MATRIX * InvMat	Inv mat of the thing thats aiming..
				:	VECTOR * Spos   Position of the thing thats aiming...
				:	VECTOR * Tpos   Position of the thing thats being aimed at...
	Output		:	void ..But Fills in the AimData Struct....
===================================================================*/
void AI_AimAtTarget( MATRIX * InvMat , VECTOR * SPos, VECTOR * TPos )
{
	VECTOR WantedDir;
	VECTOR TempDir;
	float Angle;

	AimData.Flags = 0;
	AimData.Angle.x = 0.0F;
	AimData.Angle.y = 0.0F;
	AimData.Angle.z = 0.0F;

	WantedDir.x = ( TPos->x - SPos->x );
	WantedDir.y = ( TPos->y - SPos->y );
	WantedDir.z = ( TPos->z - SPos->z );
	ApplyMatrix( InvMat, &WantedDir, &TempDir );			/* Calc Direction Vector */

	NormaliseVector( &TempDir );
	Angle = (float) acos( TempDir.x );
	Angle = 90.0F - R2D( Angle );
	if( TempDir.z < 0.0F )
		Angle = 180.0F - Angle;
	if( Angle > 180.0F )
	{
		Angle -= 360.0F;
	}
	AimData.Angle.y =  Angle;

//	if( Angle > 3.0F || ( TempDir.z < 0.0F ) )
	if( Angle > 3.0F )
	{
		AimData.Flags |= AI_CONTROL_TURNRIGHT;
	}
	if( Angle < -3.0F )
	{
		AimData.Flags |= AI_CONTROL_TURNLEFT;
	}
	
	Angle = (float) acos( TempDir.y );
	Angle = 90.0F - R2D( Angle );
	Angle *= -1.0F;
	if( Angle > 180.0F )
	{
		Angle -= 360.0F;
	}
	AimData.Angle.x = Angle;

	if( Angle < -3.0F && ( TempDir.z > 0.0F ))
	{
		AimData.Flags |= AI_CONTROL_TURNUP;
	}
	if( Angle > 3.0F && ( TempDir.z > 0.0F ))
	{
		AimData.Flags |= AI_CONTROL_TURNDOWN;
	}

}

/*===================================================================
	Procedure	:	Is a Vector Pos in a ViewCone
	Input		:	VECTOR *	Pos
				:	MATRIX *	Mat
				:	VECTOR *	TargetPos
				:	float		View Cone Cosine
	Output		:	bool true/false
===================================================================*/
bool AI_InViewCone( VECTOR * Pos, MATRIX * Mat , VECTOR * TPos, float ViewConeCos )
{
	float	Cos;
	VECTOR	NormVector;
	VECTOR	Dir;

	if( ViewConeCos == 1.0F )
		return true;
	
	Dir.x = ( TPos->x - Pos->x );
	Dir.y = ( TPos->y - Pos->y );
	Dir.z = ( TPos->z - Pos->z );
	NormVector = Dir;
	NormaliseVector( &NormVector );
			
	ApplyMatrix( Mat, &Forward, &Dir );			/* Calc Direction Vector */

	Cos = DotProduct( &NormVector, &Dir );
	
	if( Cos > ViewConeCos )
	{
		return true;
	}
	return false;
}
/*===================================================================
	Procedure	:	Is a VECTOR * Pos in Clear LOS
	Input		:	VECTOR * Pos
				:	u_int16_t Group
				:	VECTOR * TargetPos
	Output		:	bool true/false
===================================================================*/
bool AI_ClearLOS( VECTOR * SPos, u_int16_t Group , VECTOR * Pos )
{
	VECTOR	Dir;
	VECTOR	Int_Point;
	u_int16_t	Int_Group;
	NORMAL	Int_Normal;
	VECTOR	TempVector;

	Dir.x = ( Pos->x - SPos->x );
	Dir.y = ( Pos->y - SPos->y );
	Dir.z = ( Pos->z - SPos->z );
	
	if( BackgroundCollide( &MCloadheadert0, &Mloadheader, SPos, Group, &Dir,
						&Int_Point, &Int_Group, &Int_Normal, &TempVector, true, NULL ) )
	{
		return false;
	}
	
	return true;
}
/*===================================================================
	Procedure	:	Is a VECTOR * Pos in Clear LOS And will the SPos
				:	Be able to get to the TargetPos
	Input		:	OBJECT * SObject
				:	VECTOR * TargetPos
				:	float radius
	Output		:	bool true/false
===================================================================*/
bool AI_ClearLOSNonZero( OBJECT * SObject, VECTOR * Pos , float radius )
{
	VECTOR	Dir;
	BGOBJECT * BGObject;

	Dir.x = ( Pos->x - SObject->Pos.x );
	Dir.y = ( Pos->y - SObject->Pos.y );
	Dir.z = ( Pos->z - SObject->Pos.z );

	if( WouldObjectCollide( SObject, &Dir, radius, &BGObject ) )
	{
		return false;
	}
	return true;
}
/*===================================================================
	Procedure	:	Is a VECTOR * Pos in Clear LOS And will the SPos
				:	Be able to get to the TargetPos
	Input		:	OBJECT * SObject
				:	VECTOR * TargetPos
				:	float radius
	Output		:	bool true/false
===================================================================*/
bool AI_ClearLOSNonZeroNonObject( VECTOR * SPos, u_int16_t Group , VECTOR * Pos , float radius )
{
	VECTOR	Dir;
	VECTOR Impact_Point;
	u_int16_t Impact_Group;
	NORMAL Impact_Normal;

	Dir.x = ( Pos->x - SPos->x );
	Dir.y = ( Pos->y - SPos->y );
	Dir.z = ( Pos->z - SPos->z );
	return !QCollide( SPos , Group , &Dir, radius , &Impact_Point, &Impact_Group, &Impact_Normal );
}
/*===================================================================
	Procedure	:	Find a Target...
	Input		:	ENEMY * Enemy
	Output		:	Nothing..But Tinfo struct is filled in...
===================================================================*/
void AI_GetDistToNearestTarget( register ENEMY * Enemy )
{
	register u_int16_t	group = Enemy->Object.Group;
	OBJECT * TObject;
	   
	// Initialise
	Tinfo->Pos.x 	= Enemy->Object.Pos.x;
	Tinfo->Pos.y 	= Enemy->Object.Pos.y;
	Tinfo->Pos.z 	= Enemy->Object.Pos.z;
	Tinfo->Group 	= Enemy->Object.Group;
	Tinfo->Nearest = BIGDISTANCE;
	Tinfo->TFlags = 0;

	Tinfo->SObject	= &Enemy->Object;

	Tinfo->TObject	= NULL;

	// What type of target are we looking for ...
   
	if ( TARGET_PLAYERS )
	{
	    AI_GetNearestSHIPTargetInAnyGROUP();
	   
		if ( TObject = (OBJECT*) Tinfo->TObject )
		{
			Enemy->LastDistance	= Tinfo->Nearest + SoundInfo[Enemy->Object.Group][TObject->Group];
			//See if target is within think range....
			if( Enemy->LastDistance <= EnemyTypes[Enemy->Type].Behave.ThinkRange )
			{
				Tinfo->TFlags = _TARGET_PLAYERS;
				return;
			}
		}
	}      
	if( TARGET_NODES )
	{
		AI_GetNearestNODETarget( &Enemy->Object );
		if ( Tinfo->TObject )
		{
			Tinfo->TFlags = _TARGET_NODES;
			return;
		}
	}	  	
	if( TARGET_MINES )
	{
		AI_GetNearestMINETarget();

		if ( Tinfo->TObject )
		{
			Tinfo->TFlags = _TARGET_MINES;
			return;
		}
	}
}
/*===================================================================
	Procedure	:	Set Enemy AI Mode to...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SetRETREAT( ENEMY * Enemy )
{
	Enemy->Object.AI_Mode = AIMODE_RETREAT;
	Enemy->TNode = NULL;
	Enemy->Timer = EnemyTypes[Enemy->Type].Behave.RetreatTime;	// How long I Run for
}
/*===================================================================
	Procedure	:	Set Enemy AI Mode to...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SetMOVETOTARGET( ENEMY * Enemy )
{
	Enemy->AIFlags &= ~AI_IHAVESEENPLAYER;
	Enemy->Object.AI_Mode = AIMODE_MOVETOTARGET;
	Enemy->TNode = NULL;
}
/*===================================================================
	Procedure	:	Set Enemy AI Mode to...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SetMOVEFOLLOWPATH( ENEMY * Enemy )
{
	Enemy->Object.AI_Mode = AIMODE_FOLLOWPATH;
	Enemy->Timer  = RESET_VALIDATE_TIME + (float) Random_Range( (u_int16_t) RESET_VALIDATE_TIME );
}
/*===================================================================
	Procedure	:	Set Enemy AI Mode to...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SetDOGFIGHT( ENEMY * Enemy )
{
	Enemy->TNode = NULL;
	Enemy->Object.AI_Mode = AIMODE_DOGFIGHT;
	Enemy->Timer  = RESET_VALIDATE_TIME + (float) Random_Range( (u_int16_t) RESET_VALIDATE_TIME );
}
/*===================================================================
	Procedure	:	Set Enemy AI Mode to...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SetFORMATION( ENEMY * Enemy )
{
	Enemy->Object.AI_Mode = AIMODE_FORMATION;
	Enemy->Timer  = RESET_VALIDATE_TIME + (float) Random_Range( (u_int16_t) RESET_VALIDATE_TIME );
}
/*===================================================================
	Procedure	:	Set Enemy AI Mode to...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SetKILLMINE( ENEMY * Enemy )
{
	Enemy->Object.AI_Mode = AIMODE_KILLMINE;
	Enemy->Timer  = RESET_VALIDATE_TIME + (float) Random_Range( (u_int16_t) RESET_VALIDATE_TIME );
}
/*===================================================================
	Procedure	:	Set Enemy AI Mode to...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SetIDLE( register ENEMY * Enemy )
{
	Enemy->TShip = NULL;
	Enemy->Object.AI_Mode = AIMODE_IDLE;
	Enemy->Timer  = EnemyTypes[Enemy->Type].Behave.IdleTime + (float) Random_Range( (u_int16_t) EnemyTypes[Enemy->Type].Behave.IdleTime);
}
/*===================================================================
	Procedure	:	Set Enemy AI Mode to...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SetSCAN( register ENEMY * Enemy )
{
	Enemy->TShip = NULL;
	Enemy->Object.AI_Mode = AIMODE_SCAN;
	Enemy->Timer  = RESET_SCAN_TIME + (float) Random_Range( (u_int16_t) RESET_SCAN_TIME );
}
/*===================================================================
	Procedure	:	Set Enemy AI Mode to...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SetFOLLOWPATH( register ENEMY * Enemy )
{
	Enemy->TShip = NULL;
	Enemy->Object.AI_Mode = AIMODE_FOLLOWPATH;
	Enemy->Timer  = RESET_SCAN_TIME + (float) Random_Range( (u_int16_t) RESET_SCAN_TIME );
}
/*===================================================================
	Procedure	:	Set Enemy AI Mode to...
	Input		:	ENEMY * Enemy
	Output		:	Nothing
===================================================================*/
void AI_SetFIREATTARGET( register ENEMY * Enemy )
{
	Enemy->Object.AI_Mode = AIMODE_FIRE;
	Enemy->Timer  = RESET_FIRE_TIME + (float) Random_Range( (u_int16_t) RESET_FIRE_TIME );
}
#ifdef OPT_ON
#pragma optimize( "", off )
#endif

