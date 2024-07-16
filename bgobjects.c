/*===================================================================
	Includes
===================================================================*/
#include <stdio.h>
#include <math.h>

#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "mload.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "ships.h"
#include "2dpolys.h"
#include "lights.h"
#include "models.h"
#include "text.h"
#include "primary.h"
#include "secondary.h"
#include "triggers.h"
#include "pickups.h"
#include "trigarea.h"
#include "bsp.h"
#include "lines.h"

#include "visi.h"
#include "enemies.h"
#include "spotfx.h"
#include "node.h"
#include "controls.h"
#include "util.h"
#include "oct2.h"

/*===================================================================
	External Variables
===================================================================*/
extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
extern	MODEL			Models[ MAXNUMOFMODELS ];
extern	float			framelag;
extern	MATRIX			MATRIX_Identity;
extern	int16_t			LevelNum;
extern	char			LevelNames[MAXLEVELS][128];
extern	BYTE			GameStatus[MAX_PLAYERS];
extern	BYTE			WhoIAm;
extern	MODELNAME		ModelNames[MAXMODELHEADERS];
extern	MODELNAME		TitleModelNames[MAXMODELHEADERS];
extern	MODELNAME		SplashModelNames[MAXMODELHEADERS];
extern	u_int16_t			IsGroupVisible[MAXGROUPS];
extern	MODELNAME		ModelNames[MAXMODELHEADERS];
extern	TRIGGERMOD	*	TrigMods;
extern	int				NumOfTrigMods;
extern	int16_t			NextNewModel;
extern	LINE			Lines[ MAXLINES ];
extern	MLOADHEADER		Mloadheader;
extern	ENEMY			Enemies[ MAXENEMIES ];
extern	ENEMY		*	EnemyGroups[ MAXGROUPS ];
extern	ENEMY_TYPES		EnemyTypes[ MAX_ENEMY_TYPES ];
extern	PICKUP	*		PickupGroups[ MAXGROUPS ];
extern	MXLOADHEADER	ModelHeaders[MAXMODELHEADERS];
extern	ENEMY	*		FirstEnemyUsed;
extern	int16_t			PickupsGot[ MAXPICKUPTYPES ];
extern	int16_t			BikeModels[ MAXBIKETYPES ];
extern	MODELNAME	*	ModNames;
extern	TLOADHEADER		Tloadheader;
extern	SECONDARYWEAPONBULLET *	SecBullGroups[ MAXGROUPS ];

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	Global Variables
===================================================================*/
BGO_FILE	*	BGOFilesPtr = NULL;
BGOBJECT	*	FirstBGObjectUsed = NULL;
BGOBJECT	*	FirstBGObjectFree = NULL;
BGOBJECT		BGObjects[ MAXBGOBJECTS ];
bool			ShowColZones = false;

DOORTYPESFX DoorTypeSFX[] = {

{ -1, "gndoorop",	-1, NULL,		-1, "gndoorst",	-1, NULL,	-1, "gndoorop",	-1, NULL,		-1, "gndoorcl",	-1, NULL },
{ -1, "stndrstr",	-1, "stndrmov",	-1, "stndrstp", -1, NULL,	-1, "stndrstr",	-1, "stndrmov",	-1, "stndrstp", -1, NULL },
{ -1, "ffdoropn",	-1, NULL,		-1, NULL,		-1, NULL,	-1, "ffdorcls",	-1, NULL,		-1, "ffdorcns",	-1, NULL },
{ -1, "lift1str",	-1, "lift1lp",	-1, "lift1stp",	-1, NULL,	-1, "lift1str",	-1, "lift1lp",	-1, "lift1stp", -1, NULL },
{ -1, "smkdorop",	-1, NULL,		-1, NULL,		-1, NULL,	-1, "smkdorcl",	-1, NULL,		-1, NULL,		-1, NULL },
{ -1, "eled1str",	-1, "eled1mov",	-1, "eled1stp",	-1, NULL,	-1, "eled1str",	-1, "eled1mov",	-1, "eled1stp",	-1, NULL },
{ -1, "eled2str",	-1, "eled2mov",	-1, "eled2stp",	-1, NULL,	-1, "eled2str",	-1, "eled2mov",	-1, "eled2stp",	-1, NULL },
{ -1, "eled3str",	-1, "eled3mov",	-1, "eled3stp",	-1, NULL,	-1, "eled3str",	-1, "eled3mov",	-1, "eled3stp",	-1, NULL },
{ -1, "eled4str",	-1, "eled4mov",	-1, "eled4stp",	-1, NULL,	-1, "eled4str",	-1, "eled4mov",	-1, "eled4stp",	-1, NULL },

};

/*===================================================================
	Defines
===================================================================*/
#define	BGO_VERSION_NUMBER	3
#define	BGOPush				1
#define	DUPLICATE_BGOBJECTS	1

/*===================================================================
	Procedure	:	Initialise all Background Objects
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void InitBGObjects( void )
{
	u_int16_t	i;

	FirstBGObjectUsed = NULL;
	FirstBGObjectFree = &BGObjects[ 0 ];

	for( i = 0; i < MAXBGOBJECTS; i++ )
	{
		memset( &BGObjects[ i ], 0, sizeof( BGOBJECT ) );

		BGObjects[ i ].ModelIndex = (u_int16_t) -1;

		BGObjects[ i ].Index = i;
		BGObjects[ i ].NextUsed = NULL;
		BGObjects[ i ].PrevUsed = NULL;
		BGObjects[ i ].NextFree = &BGObjects[ i + 1 ];
		BGObjects[ i ].PrevFree = &BGObjects[ i - 1 ];
	}

	BGObjects[ 0 ].PrevFree = NULL;
	BGObjects[ MAXBGOBJECTS - 1 ].NextFree = NULL;
}

/*===================================================================
	Procedure	:	Find free Background Object
	Input		:	nothing
	Output		:	BGObject	*	Object ( NULL if not available )
===================================================================*/
BGOBJECT * FindFreeBGObject( void )
{
	BGOBJECT * Object;

	Object = FirstBGObjectFree;

	if( Object != NULL )
	{
		FirstBGObjectFree = Object->NextFree;
		if( FirstBGObjectFree != NULL )
		{
			FirstBGObjectFree->PrevFree = NULL;
		}

		if( FirstBGObjectUsed != NULL )
		{
			FirstBGObjectUsed->PrevUsed = Object;
			Object->NextUsed = FirstBGObjectUsed;
		}
		else
		{
			Object->NextUsed = NULL;
		}

		Object->PrevUsed = NULL;
		FirstBGObjectUsed = Object;
	}

	return( Object );
}

/*===================================================================
	Procedure	:	Kill a Background Object
	Input		:	BGObject	*	Object
	Output		:	nothing
===================================================================*/
void KillUsedBGObject( BGOBJECT * Object )
{
	BGOBJECT	*	PrevObject;
	BGOBJECT	*	NextObject;

	if( Object != NULL )
	{
		if( Object == FirstBGObjectUsed )
		{
			NextObject = FirstBGObjectUsed->NextUsed;

			FirstBGObjectUsed = NextObject;

			if( FirstBGObjectUsed != NULL )
			{
				FirstBGObjectUsed->PrevUsed = NULL;
			}
		}
		else
		{
			PrevObject = Object->PrevUsed;
			NextObject = Object->NextUsed;

			if( PrevObject != NULL )
			{
				PrevObject->NextUsed = NextObject;
			}

			if( NextObject != NULL )
			{
				NextObject->PrevUsed = PrevObject;
			}
		}

		if( FirstBGObjectFree != NULL )
		{
			FirstBGObjectFree->PrevFree = Object;
		}

		if( Object->ModelIndex != (u_int16_t) -1 )
		{
			KillUsedModel( Object->ModelIndex );
			Object->ModelIndex = (u_int16_t) -1;
		}

		FreeCompObjChildren( Object->Children, Object->NumChildren );
		if( Object->Children )
		{
			free( Object->Children );
			Object->Children = NULL;
			Object->NumChildren = 0;
		}

		Object->NextFree = FirstBGObjectFree;
		Object->PrevFree = NULL;
		FirstBGObjectFree = Object;
	}
}

/*===================================================================
	Procedure	:	Process all Background Objects
	Input		:	bool	Collide with objects?
	Output		:	nothing
===================================================================*/
void ProcessBGObjects( bool Collide )
{
	float			Damage = 0.0F;
	BGOBJECT	*	Object;
	BGOBJECT	*	NextObject;
	VECTOR			PushVector;
	bool			UndoAnim = false;	

	Object = FirstBGObjectUsed;

	while( Object != NULL )
	{
		NextObject = Object->NextUsed;

		switch( Object->Type )
		{
/*===================================================================
			STATIC
===================================================================*/
			case BGOTYPE_Static:
				UpdateCompObjChildren( Object->Children, Object->NumChildren, &Object->Matrix, &Object->Pos, Object->Time, Object->Group, &Object->Center );
				break;

/*===================================================================
			DOOR
===================================================================*/
			case BGOTYPE_Door:
				if( Object->StateChangeDelay )
				{
					Object->StateChangeDelay -= framelag;
					if( Object->StateChangeDelay < 0.0F ) Object->StateChangeDelay = 0.0F;
				}

				Object->OldOldTime = Object->OldTime;
				Object->OldTime = Object->Time;
		
				switch( Object->State )
				{	
					case DOOR_OPENING:
						Object->Time += ( framelag * Object->AnimSpeed );
						if( Object->Time > Object->MidTime )
						{
							Object->State = DOOR_OPEN;
							Object->Timer = ( ANIM_SECOND * 3.0F ) - ( Object->Time - Object->MidTime );
							Object->Time = Object->MidTime;
							StartOrChangeDoorSoundFX( Object );

							if( Object->OpenTrigModPtr != NULL )
							{
								ApplyTriggerMod( Object->OpenTrigModPtr );
							}
						}
						else
						{
							break;
						}
						break;
			
					case DOOR_REVERSEOPENING:
						Object->Time -= ( framelag * Object->AnimSpeed );
						if( Object->Time < 0.0F )
						{
							Object->State = DOOR_CLOSED;
							Object->Time = 0.0F;
							StartOrChangeDoorSoundFX( Object );

							if( Object->CloseTrigModPtr != NULL )
							{
								ApplyTriggerMod( Object->CloseTrigModPtr );
							}
						}
						else
						{
							break;
						}
						break;
		
					case DOOR_OPEN:
						Object->Timer -= ( framelag * Object->AnimSpeed );

						if( Object->Timer < 0.0F )
						{
							Object->Time = Object->MidTime + ( 0.0F - Object->Timer );
							Object->Timer = 0.0F;
							Object->State = DOOR_CLOSING;
							StartOrChangeDoorSoundFX( Object );
						}
						else
						{
							Object->Time = Object->MidTime;
							break;
						}
						break;
			
					case DOOR_CLOSING:
						Object->Time += ( framelag * Object->AnimSpeed );
						if( Object->Time > Object->OverallTime )
						{
							Object->State = DOOR_CLOSED;
							Object->Time = 0.0F;
							StartOrChangeDoorSoundFX( Object );

							if( Object->CloseTrigModPtr != NULL )
							{
								ApplyTriggerMod( Object->CloseTrigModPtr );
							}
						}
						break;
			
					case DOOR_REVERSECLOSING:
						Object->Time -= ( framelag * Object->AnimSpeed );
						if( Object->Time < Object->MidTime )
						{
							Object->State = DOOR_OPEN;
							Object->Timer = ( ANIM_SECOND * 3.0F ) - ( Object->MidTime - Object->Time );
							Object->Time = Object->MidTime;
							StartOrChangeDoorSoundFX( Object );

							if( Object->OpenTrigModPtr != NULL )
							{
								ApplyTriggerMod( Object->OpenTrigModPtr );
							}
						}
						break;
		
					case DOOR_CLOSED:
						Object->Time = 0.0F;
						break;
		
					default:
						break;
				}
		
				if( Object->UpdateCount || ( Object->Time != Object->OldTime ) )
				{
					UpdateCompObjChildren( Object->Children, Object->NumChildren, &Object->Matrix, &Object->Pos, Object->Time, Object->Group, &Object->Center );
			
					if( Object->Time != Object->OldTime )
					{
						Object->UpdateCount = 3;

						UndoAnim = false;

						if( Collide )
						{
							if( Ships[ WhoIAm ].Object.Mode != LIMBO_MODE )
							{
								if( CheckBGObjectCollision( &Ships[ WhoIAm ].Object.Pos, Object, &PushVector, &Damage, SHIP_RADIUS ) )
								{
									ChangeBGState( Object, OWNER_SHIP, WhoIAm, BUMP, 0.0F );

//									if( WouldObjectCollide( &Ships[ WhoIAm ].Object, &PushVector, SHIP_RADIUS, NULL ) )
									{
										HitMe( OWNER_NOBODY, 0, ( Damage * framelag ), WEPTYPE_Various, 0 );
									}
//									else
//									{
										ForceExternalOneOff( WhoIAm, &PushVector );
//									}
									UndoAnim = true;
								}
							}

							if( CheckBGObjectToEnemies( Object ) )
							{
								ChangeBGState( Object, OWNER_ENEMY, 0, BUMP, 0.0F );
								UndoAnim = true;
							}

							if( CheckBGObjectToPickups( Object ) )
							{
								UndoAnim = true;
							}

							CheckBGObjectToMines( Object );
						}

						if( UndoAnim )
						{
							UndoBGObjectAnim( Object );
							UpdateBGObjectSend( Object->Index, Object->State, Object->Time );
						}
					}
					else
					{
						if( Object->UpdateCount ) Object->UpdateCount--;
					}
				}
				break;

/*===================================================================
			LOOPING
===================================================================*/
			case BGOTYPE_Looping_Anim:
				Object->OldOldTime = Object->OldTime;
				Object->OldTime = Object->Time;

				switch( Object->State )
				{
					case LOOPANIM_WaitForDelay:
						Object->Delay -= framelag;
						if( Object->Delay < 0.0F )
						{
							Object->Delay = 0.0F;
							Object->State = LOOPANIM_Animating;
						}
						break;

					case LOOPANIM_WaitForTrigger:
						break;

					case LOOPANIM_Animating:
						if( Object->OverallTime > 0.001F )
						{
							Object->Time += framelag;

							if( Object->Time >= Object->OverallTime )
							{
								Object->Time -= ( (float) floor( Object->Time / Object->OverallTime ) * Object->OverallTime );
							}
						}
						break;
				}

				if( Object->UpdateCount || ( Object->Time != Object->OldTime ) )
				{
					UpdateCompObjChildren( Object->Children, Object->NumChildren, &Object->Matrix, &Object->Pos, Object->Time, Object->Group, &Object->Center );

					if( Object->Time != Object->OldTime )
					{
						Object->UpdateCount = 3;

						UndoAnim = false;

						if( Collide )
						{
							if( Ships[ WhoIAm ].Object.Mode != LIMBO_MODE )
							{
								if( CheckBGObjectCollision( &Ships[ WhoIAm ].Object.Pos, Object, &PushVector, &Damage, SHIP_RADIUS ) )
								{
//									if( WouldObjectCollide( &Ships[ WhoIAm ].Object, &PushVector, SHIP_RADIUS, NULL ) )
									{
										HitMe( OWNER_NOBODY, 0, ( Damage * framelag ), WEPTYPE_Various, 0 );
									}

									ForceExternalOneOff( WhoIAm, &PushVector );
									UndoAnim = true;
								}
							}

							if( CheckBGObjectToEnemies( Object ) )
							{
								UndoAnim = true;
							}

							if( CheckBGObjectToPickups( Object ) )
							{
								UndoAnim = true;
							}

							CheckBGObjectToMines( Object );
						}

						if( UndoAnim )
						{
							UndoBGObjectAnim( Object );
							UpdateBGObjectSend( Object->Index, Object->State, Object->Time );
						}
					}
					else
					{
						if( Object->UpdateCount ) Object->UpdateCount--;
					}
				}
				break;

/*===================================================================
			ANIMATE AND STOP AT END
===================================================================*/
			case BGOTYPE_OneOff_Anim:
				Object->OldOldTime = Object->OldTime;
				Object->OldTime = Object->Time;

				switch( Object->State )
				{
					case ONEOFFANIM_WaitForDelay:
						Object->Delay -= framelag;
						if( Object->Delay < 0.0F )
						{
							Object->Delay = 0.0F;
							Object->State = ONEOFFANIM_Animating;
						}
						break;

					case ONEOFFANIM_WaitForTrigger:
						break;

					case ONEOFFANIM_Animating:
						if( Object->Time < Object->OverallTime )
						{
							Object->Time += framelag;
						}
						else
						{
							if( Object->EndTrigModPtr != NULL )
							{
								ApplyTriggerMod( Object->EndTrigModPtr );
							}

							Object->Time = Object->OverallTime;
							if( Object->DestroyAtEnd )
							{
								ExplodeChildren( Object->Children, 1 );
								KillUsedBGObject( Object );
								Object = NULL;
							}
							else
							{
								Object->State = ONEOFFANIM_WaitForTrigger;
							}
						}
						break;
				}

				if( Object )
				{
					if( Object->UpdateCount || ( Object->Time != Object->OldTime ) )
					{
						UpdateCompObjChildren( Object->Children, Object->NumChildren, &Object->Matrix, &Object->Pos, Object->Time, Object->Group, &Object->Center );

						if( Object->Time != Object->OldTime )
						{
							Object->UpdateCount = 3;

							UndoAnim = false;

							if( Collide )
							{
								if( Ships[ WhoIAm ].Object.Mode != LIMBO_MODE )
								{
									if( CheckBGObjectCollision( &Ships[ WhoIAm ].Object.Pos, Object, &PushVector, &Damage, SHIP_RADIUS ) )
									{
//										if( WouldObjectCollide( &Ships[ WhoIAm ].Object, &PushVector, SHIP_RADIUS, NULL ) )
										{
											HitMe( OWNER_NOBODY, 0, ( Damage * framelag ), WEPTYPE_Various, 0 );
										}

										ForceExternalOneOff( WhoIAm, &PushVector );
										UndoAnim = true;
									}
								}

								if( CheckBGObjectToEnemies( Object ) )
								{
									UndoAnim = true;
								}

								if( CheckBGObjectToPickups( Object ) )
								{
									UndoAnim = true;
								}

								CheckBGObjectToMines( Object );
							}

							if( UndoAnim )
							{
								UndoBGObjectAnim( Object );
								UpdateBGObjectSend( Object->Index, Object->State, Object->Time );
							}
						}
						else
						{
							if( Object->UpdateCount ) Object->UpdateCount--;
						}
					}
				}
				break;

/*===================================================================
			STEPPED ANIM CAUSED BY EXTERNAL CONDITIONS
===================================================================*/
			case BGOTYPE_MultipleStep_Anim:
				Object->OldOldTime = Object->OldTime;
				Object->OldTime = Object->Time;

				if( Object->Time < Object->WantedTime )
				{
					Object->Time += framelag;

					if( Object->Time > Object->WantedTime )
					{
						Object->Time = Object->WantedTime;
					}

					if( Object->Time >= Object->OverallTime )
					{
						Object->Time = Object->OverallTime;

						if( Object->EndTrigModPtr != NULL )
						{
							ApplyTriggerMod( Object->EndTrigModPtr );
						}

						if( Object->DestroyAtEnd )
						{
							ExplodeChildren( Object->Children, 1 );
							KillUsedBGObject( Object );
							Object = NULL;
						}
					}
				}

				if( Object != NULL )
				{
					if( Object->UpdateCount || ( Object->Time != Object->OldTime ) )
					{
						UpdateCompObjChildren( Object->Children, Object->NumChildren, &Object->Matrix, &Object->Pos, Object->Time, Object->Group, &Object->Center );

						if( Object->Time != Object->OldTime )
						{
							Object->UpdateCount = 3;

							UndoAnim = false;

							if( Collide )
							{
								if( Ships[ WhoIAm ].Object.Mode != LIMBO_MODE )
								{
									if( CheckBGObjectCollision( &Ships[ WhoIAm ].Object.Pos, Object, &PushVector, &Damage, SHIP_RADIUS ) )
									{
										if( WouldObjectCollide( &Ships[ WhoIAm ].Object, &PushVector, SHIP_RADIUS, NULL ) )
										{
											HitMe( OWNER_NOBODY, 0, ( Damage * framelag ), WEPTYPE_Various, 0 );
										}

										ForceExternalOneOff( WhoIAm, &PushVector );
										UndoAnim = true;
									}
								}

								if( CheckBGObjectToEnemies( Object ) )
								{
									UndoAnim = true;
								}

								if( CheckBGObjectToPickups( Object ) )
								{
									UndoAnim = true;
								}

								CheckBGObjectToMines( Object );
							}

							if( UndoAnim )
							{
								UndoBGObjectAnim( Object );
								UpdateBGObjectSend( Object->Index, Object->State, Object->Time );
							}
						}
						else
						{
							if( Object->UpdateCount ) Object->UpdateCount--;
						}
					}
				}
				break;

/*===================================================================
			SEQUENCE ANIM
===================================================================*/
			case BGOTYPE_Sequences:
				break;
		}

		Object = NextObject;
	}
}

/*===================================================================
	Procedure	:	Kill All Background Objects
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void KillAllBGObjects( void )
{
	BGOBJECT	*	Object;
	BGOBJECT	*	NextObject;

	Object = FirstBGObjectUsed;

	while( Object != NULL )
	{
		NextObject = Object->NextUsed;
		KillUsedBGObject( Object );
		Object = NextObject;
	}
}

/*===================================================================
	Procedure	:	Change BGObject State
	Input		:	BGOBJECT	*	Object
				:	u_int16_t			Owner Type
				:	u_int16_t			Owner
				:	int16_t			How
				:	bool			Override
	Output		:	Nothing
===================================================================*/
void ChangeBGState( BGOBJECT * Object, u_int16_t OwnerType, u_int16_t Owner, int16_t How, float Damage )
{
	bool	Activate;

	switch( Object->Type )
	{
/*===================================================================
			STATIC
===================================================================*/
		case BGOTYPE_Static:
			break;

/*===================================================================
			DOOR
===================================================================*/
		case BGOTYPE_Door:
			Activate = false;

			if( How == BUMP )
			{
				if( Object->BumpTrigModPtr != NULL )
				{
					ApplyTriggerMod( Object->BumpTrigModPtr );
				}

				if( !Object->Locked )
				{
					if( ( OwnerType == OWNER_SHIP ) && ( Object->OpenedBy & WHEN_PlayerBump ) ) Activate = true;
					else if( ( OwnerType == OWNER_ENEMY ) && ( Object->OpenedBy & WHEN_EnemyBump ) ) Activate = true;
				}
				else
				{
					if( ( OwnerType == OWNER_SHIP ) && ( Owner == WhoIAm ) )
					{
						if( Object->PickupNeeded != -1 )
						{
							if( PickupsGot[ Object->PickupNeeded ] )
							{
								Object->Locked = false;
								Activate = true;
								if( Object->PickupNeeded == PICKUP_SkeletonKey ) PickupsGot[ Object->PickupNeeded ]--;
								if( Object->PickupNeeded == PICKUP_GoldFigure ) PickupsGot[ Object->PickupNeeded ]--;
							}
						}
					}
				}
			}
			else
			{
				if( Object->ShotTrigModPtr != NULL )
				{
					ApplyTriggerMod( Object->ShotTrigModPtr );
				}

				if( !Object->Locked )
				{
					if( ( OwnerType == OWNER_SHIP ) && ( Object->OpenedBy & WHEN_PlayerShot ) ) Activate = true;
					else if( ( OwnerType == OWNER_ENEMY ) && ( Object->OpenedBy & WHEN_EnemyShot ) ) Activate = true;
				}
				else
				{
					if( ( OwnerType == OWNER_SHIP ) && ( Owner == WhoIAm ) )
					{
						if( Object->PickupNeeded != -1 )
						{
							if( PickupsGot[ Object->PickupNeeded ] )
							{
								Object->Locked = false;
								Activate = true;
								if( Object->PickupNeeded == PICKUP_SkeletonKey ) PickupsGot[ Object->PickupNeeded ]--;
								if( Object->PickupNeeded == PICKUP_GoldFigure ) PickupsGot[ Object->PickupNeeded ]--;
							}
						}
					}
				}
			}

			if( Activate && !Object->StateChangeDelay )
			{
				switch( Object->State )
				{
					case DOOR_CLOSED:
						Object->State = DOOR_OPENING;
						if( ( OwnerType == OWNER_SHIP ) && ( Owner == WhoIAm ) ) UpdateBGObjectSend( Object->Index, Object->State, Object->Time );
						Object->StateChangeDelay = ANIM_SECOND;
						StartOrChangeDoorSoundFX( Object );
						break;
						
					case DOOR_OPEN:
						Object->State = DOOR_CLOSING;
						if( ( OwnerType == OWNER_SHIP ) && ( Owner == WhoIAm ) ) UpdateBGObjectSend( Object->Index, Object->State, Object->Time );
						Object->StateChangeDelay = ANIM_SECOND;
						StartOrChangeDoorSoundFX( Object );
						break;

					case DOOR_OPENING:
						break;
					
					case DOOR_CLOSING:
						Object->State = DOOR_REVERSECLOSING;
						if( ( OwnerType == OWNER_SHIP ) && ( Owner == WhoIAm ) ) UpdateBGObjectSend( Object->Index, Object->State, Object->Time );
						Object->StateChangeDelay = ANIM_SECOND;
						StartOrChangeDoorSoundFX( Object );
						break;

					case DOOR_REVERSEOPENING:
						Object->State = DOOR_OPENING;
						if( ( OwnerType == OWNER_SHIP ) && ( Owner == WhoIAm ) ) UpdateBGObjectSend( Object->Index, Object->State, Object->Time );
						Object->StateChangeDelay = ANIM_SECOND;
						StartOrChangeDoorSoundFX( Object );
						break;

					case DOOR_REVERSECLOSING:
						break;

					default:
						break;
				}
			}
			break;

/*===================================================================
			LOOPING ANIM
===================================================================*/
		case BGOTYPE_Looping_Anim:
			if( How == BUMP )
			{
				if( Object->BumpTrigModPtr != NULL )
				{
					if( OwnerType == OWNER_SHIP )
					{
						ApplyTriggerMod( Object->BumpTrigModPtr );
					}
				}
			}
			else
			{
				if( Object->ShotTrigModPtr != NULL )
				{
					if( OwnerType == OWNER_SHIP )
					{
						ApplyTriggerMod( Object->ShotTrigModPtr );
					}
				}
			}
			break;

/*===================================================================
			ONEOFF ANIM
===================================================================*/
		case BGOTYPE_OneOff_Anim:
			if( How == BUMP )
			{
				if( Object->State != ONEOFFANIM_Animating )
				{
					if( Object->BumpTrigModPtr != NULL )
					{
						if( OwnerType == OWNER_SHIP )
						{
							ApplyTriggerMod( Object->BumpTrigModPtr );
						}
					}
				}
			}
			else
			{
				if( Object->State != ONEOFFANIM_Animating )
				{
					if( Object->ShotTrigModPtr != NULL )
					{
						if( OwnerType == OWNER_SHIP )
						{
							ApplyTriggerMod( Object->ShotTrigModPtr );
						}
					}
				}
			}
			break;

/*===================================================================
			MULTI-STEP ANIM
===================================================================*/
		case BGOTYPE_MultipleStep_Anim:
			Activate = false;

			if( How == BUMP )
			{
				if( ( OwnerType == OWNER_SHIP ) && ( Object->OpenedBy & WHEN_PlayerBump ) ) Activate = true;
				else if( ( OwnerType == OWNER_ENEMY ) && ( Object->OpenedBy & WHEN_EnemyBump ) ) Activate = true;

				if( Object->BumpTrigModPtr != NULL )
				{
					if( Activate )
					{
						ApplyTriggerMod( Object->BumpTrigModPtr );
					}
				}
			}
			else
			{
				if( ( OwnerType == OWNER_SHIP ) && ( Object->OpenedBy & WHEN_PlayerShot ) ) Activate = true;
				else if( ( OwnerType == OWNER_ENEMY ) && ( Object->OpenedBy & WHEN_EnemyShot ) ) Activate = true;

				if( Object->ShotTrigModPtr != NULL )
				{
					if( Activate )
					{
						ApplyTriggerMod( Object->ShotTrigModPtr );
					}
				}
			}


			if( Activate )
			{
				Object->DamageTime += ( Damage * Object->TimePerDamageUnit );

				if( Object->DamageTime > Object->OverallTime )
				{
					Object->DamageTime = Object->OverallTime;
				}

				if( Object->StepInterval > 0.0001F )
				{
					Object->WantedTime = FDIV( Object->DamageTime, Object->StepInterval );
				}
				else
				{
					Object->WantedTime = 0.0F;
				}
			}
			break;

/*===================================================================
			SEQUENCE ANIM
===================================================================*/
		case BGOTYPE_Sequences:
			break;

/*===================================================================
			UNKNOWN ANIM
===================================================================*/
		default:
			break;

	}
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif

/*===================================================================
	Procedure	:	PreLoad All Background Animating Objects
	Input		:	int8_t	*	Filename
	Output		:	bool		true/false
===================================================================*/
extern	int16_t		LevelNum;
bool PreLoadBGOFiles( void )
{
	BGO_FILE	*	FilePtr = NULL;
	char			TempFilename[ 256 ];
	u_int16_t			BaseModel;
	u_int16_t			TempBaseModel = (u_int16_t) -1;
	BGO_FILE	*	TempPtr;
	FILE		*	fp;
	char		*	NewExt = ".BGO";
	int16_t			NumBGObjects;
	int16_t			Count;
	int16_t			i;
	u_int32_t			MagicNumber;
	u_int32_t			VersionNumber;

	ModNames = &ModelNames[ 0 ];
	BaseModel = NextNewModel;

	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &TempFilename[ 0 ], NewExt );

	fp = file_open( &TempFilename[ 0 ], "rb" );

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != BGO_VERSION_NUMBER  ) )
		{
			fclose( fp );
			Msg( "PreLoadBGOFiles() Incompatible BGObject (.BGO) file %s", &TempFilename[ 0 ] );
			return( false );
		}

		fread( &NumBGObjects, sizeof( int16_t ), 1, fp );

		if( NumBGObjects > MAXBGOBJECTS )
		{
			fclose( fp );
			Msg( "PreLoadBGOFiles() Too Many BGObjects\n" );
			return( false );
		}

		BGOFilesPtr = (BGO_FILE *) malloc( ( NumBGObjects + 1 ) * sizeof( BGO_FILE ) );
		
		if( BGOFilesPtr != NULL )
		{
			FilePtr = BGOFilesPtr;

			for( Count = 0; Count < NumBGObjects; Count++ )
			{
				i = 0;
				
				do
				{
					fread( &TempFilename[ i ], sizeof( int8_t ), 1, fp );
					i++;
				}
				while( TempFilename[ ( i - 1 ) ] != 0 );

				sprintf( &FilePtr->Filename[ 0 ], "data\\levels\\%s\\bgobjects\\%s", &ShortLevelNames[ LevelNum ][ 0 ], &TempFilename[ 0 ] );

				fread( &FilePtr->Type, sizeof( int16_t ), 1, fp );

				fread( &FilePtr->Group, sizeof( int16_t ), 1, fp );

				fread( &FilePtr->Pos.x, sizeof( float ), 1, fp );
				fread( &FilePtr->Pos.y, sizeof( float ), 1, fp );
				fread( &FilePtr->Pos.z, sizeof( float ), 1, fp );

				fread( &FilePtr->Dir.x, sizeof( float ), 1, fp );
				fread( &FilePtr->Dir.y, sizeof( float ), 1, fp );
				fread( &FilePtr->Dir.z, sizeof( float ), 1, fp );

				fread( &FilePtr->Up.x, sizeof( float ), 1, fp );
				fread( &FilePtr->Up.y, sizeof( float ), 1, fp );
				fread( &FilePtr->Up.z, sizeof( float ), 1, fp );

				switch( FilePtr->Type )
				{
/*컴컴컴컴�	STATIC ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

					case BGOTYPE_Static:
						FilePtr->open_event = -1;
						FilePtr->CloseEvent = -1;
						break;

/*컴컴컴컴�	DOOR 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

					case BGOTYPE_Door:
						fread( &FilePtr->open_event, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->CloseEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->ShotEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->BumpEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->PickupNeeded, sizeof( int32_t ), 1, fp );
						fread( &FilePtr->OpenedBy, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->Locked, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->DoorSfxType, sizeof( int16_t ), 1, fp );
						RequestDoorTypeSFX( FilePtr->DoorSfxType );
						break;

/*컴컴컴컴�	LOOPING ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

					case BGOTYPE_Looping_Anim:
						fread( &FilePtr->ShotEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->BumpEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->EndEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->GenType, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->Delay, sizeof( float ), 1, fp );
						break;

/*컴컴컴컴�	ONEOFF ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

					case BGOTYPE_OneOff_Anim:
						fread( &FilePtr->ShotEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->BumpEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->EndEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->GenType, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->Delay, sizeof( float ), 1, fp );
						fread( &FilePtr->DestroyAtEnd, sizeof( int16_t ), 1, fp );
						break;

/*컴컴컴컴�	MULTISTEP ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

					case BGOTYPE_MultipleStep_Anim:
						fread( &FilePtr->ShotEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->BumpEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->EndEvent, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->DestroyAtEnd, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->OpenedBy, sizeof( int16_t ), 1, fp );
						fread( &FilePtr->NumIntervals, sizeof( u_int16_t ), 1, fp );
						fread( &FilePtr->DamagePerInterval, sizeof( float ), 1, fp );
						break;

/*컴컴컴컴�	SEQUENCES ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

					case BGOTYPE_Sequences:
						break;

/*컴컴컴컴�	UNKNOWN ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

					default:
						break;

				}
				FilePtr++;

			}

			FilePtr->Filename[ 0 ] = 0;

			FilePtr = BGOFilesPtr;

			while( FilePtr->Filename[ 0 ] != 0 )
			{
				TempPtr = BGOFilesPtr;
				TempBaseModel = (u_int16_t) -1;

#if	DUPLICATE_BGOBJECTS == 0
				while( TempPtr != FilePtr )
				{
					if( !strcasecmp( &TempPtr->Filename[ 0 ], &FilePtr->Filename[ 0 ] ) )
					{
						TempBaseModel = TempPtr->BaseModel;
						break;
					}
					TempPtr++;
				}
#endif		
				if( TempBaseModel != (u_int16_t) -1 )
				{
					FilePtr->BaseModel = TempBaseModel;
				}
				else
				{
					FilePtr->BaseModel = BaseModel;

					if( !PreLoadCompObj( &FilePtr->Filename[ 0 ], &BaseModel, LEVEL_SPECIFIC ) )
					{
				        Msg( "PreloadBGOFiles() PreloadCompObj() failed on %s\n", &FilePtr->Filename[ 0 ] );
						return( false );
				}	}
		
				FilePtr++;
			}

		}
		else
		{
	        Msg( "PreloadBGOFiles() alloc failed on %s\n", &TempFilename[ 0 ] );
			return( false );
		}

		NextNewModel = BaseModel;

		fclose( fp );
	}
	else
	{
		BGOFilesPtr = NULL;
	}

	RequestDoorTypeSFX( 0 );

	return( true );
}

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	Procedure	:	Load All Background Animating Objects
	Input		:	Nothing
	Output		:	bool		true/false
===================================================================*/
bool LoadBGOFiles( void )
{
	BGO_FILE	*	FilePtr;
	BGOBJECT	*	Object;
	u_int16_t			BaseModel = MODEL_ExtraModels;

	if( BGOFilesPtr )
	{
		FilePtr = BGOFilesPtr;
	
		while( FilePtr->Filename[ 0 ] != 0 )
		{
			BaseModel = FilePtr->BaseModel;

			Object = LoadBGObjectData( &FilePtr->Filename[ 0 ], FilePtr->Type, &FilePtr->Pos,
									   &FilePtr->Dir, &FilePtr->Up, FilePtr->Group,
									   FilePtr->open_event, FilePtr->CloseEvent,
									   FilePtr->ShotEvent, FilePtr->BumpEvent, FilePtr->EndEvent,
									   FilePtr->OpenedBy, FilePtr->DestroyAtEnd, FilePtr->GenType,
									   FilePtr->Delay, FilePtr->Locked, FilePtr->PickupNeeded, &BaseModel,
									   FilePtr->NumIntervals, FilePtr->DamagePerInterval, FilePtr->DoorSfxType );
			if( Object == NULL )
			{
				return( false );
			}

			GetCompObjBoundingBox( &Object->Matrix, &Object->Pos, Object->Children, Object->NumChildren,
								   Object->OverallTime, &Object->TopLeft, &Object->BottomRight );

			Object->Center.x = Object->TopLeft.x + ( ( Object->BottomRight.x - Object->TopLeft.x ) / 2.0F );
			Object->Center.y = Object->TopLeft.y + ( ( Object->BottomRight.y - Object->TopLeft.y ) / 2.0F );
			Object->Center.z = Object->TopLeft.z + ( ( Object->BottomRight.z - Object->TopLeft.z ) / 2.0F );

			Object->Radius = CalcObjectRadius( &Object->Center, &Object->TopLeft, &Object->BottomRight );

			GetCompObjColBoundingBox( &Object->Matrix, &Object->Pos, Object->Children, Object->NumChildren,
								   Object->OverallTime, &Object->ColTopLeft, &Object->ColBottomRight );

			Object->ColCenter.x = Object->ColTopLeft.x + ( ( Object->ColBottomRight.x - Object->ColTopLeft.x ) / 2.0F );
			Object->ColCenter.y = Object->ColTopLeft.y + ( ( Object->ColBottomRight.y - Object->ColTopLeft.y ) / 2.0F );
			Object->ColCenter.z = Object->ColTopLeft.z + ( ( Object->ColBottomRight.z - Object->ColTopLeft.z ) / 2.0F );

			Object->ColRadius = CalcObjectRadius( &Object->ColCenter, &Object->ColTopLeft, &Object->ColBottomRight );

			SetCompObjRealLighting( Object->Children, Object->NumChildren, Object->Radius );

//			CreateBoundingBox( &Object->TopLeft, &Object->BottomRight, Object->Group );
//			CreateBoundingBox( &Object->ColTopLeft, &Object->ColBottomRight, Object->Group );

			FilePtr++;
		}

		free( BGOFilesPtr );
		BGOFilesPtr = NULL;
	}

	ProcessBGObjects( false );

	return( true );
}

/*===================================================================
	Procedure	:	Load Background Object Data
	Input		:	int8_t		*	Filename of .BGO data
				:	int16_t			Type
				:	VECTOR		*	Pos
				:	VECTOR		*	DirVector
				:	VECTOR		*	UpVector
				:	u_int16_t			Group
				:	int16_t			OpenTrigMod
				:	int16_t			CloseTrigMod
				:	int16_t			ShotTrigMod
				:	int16_t			BumpTrigMod
				:	int16_t			EndTrigMod
				:	int16_t			Open/Activate By
				:	int16_t			Destroy At End
				:	int16_t			GenType
				:	int16_t			Gen Delay
				:	int16_t			Locked
				:	int32_t			Pickup Needed
				:	u_int16_t		*	BaseModel
				:	u_int16_t			NumIntervals
				:	float			DamagePerInterval
				:	int16_t			DoorSfxType
	Output		:	BGOBJECT	*	Object ( NULL = Error )
===================================================================*/
BGOBJECT * LoadBGObjectData( int8_t * Filename, int16_t Type, VECTOR * Pos, VECTOR * DirVector,
							 VECTOR * UpVector, u_int16_t Group, int16_t OpenTrigMod, int16_t CloseTrigMod,
							 int16_t ShotTrigMod, int16_t BumpTrigMod, int16_t EndTrigMod, int16_t OpenedBy,
							 int16_t DestroyAtEnd, int16_t GenType, float Delay, int16_t Locked, int32_t PickupNeeded,
							 u_int16_t * BaseModel, u_int16_t NumIntervals, float DamagePerInterval, int16_t DoorSfxType )
{
	BGOBJECT	*	Object;
	QUAT			TempQuat;
	MATRIX			TempMatrix;

	Object = FindFreeBGObject();
	
	if( Object != NULL )
	{
		Object->State = DOOR_CLOSED;

		switch( Type )
		{
/*컴컴컴컴�	STATIC ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

			case BGOTYPE_Static:
				Object->OpenTrigMod = -1;
				Object->CloseTrigMod = -1;
				Object->ShotTrigMod = -1;
				Object->BumpTrigMod = -1;
				Object->EndTrigMod = -1;
				Object->OpenTrigModPtr = NULL;
				Object->CloseTrigModPtr = NULL;
				Object->ShotTrigModPtr = NULL;
				Object->BumpTrigModPtr = NULL;
				Object->EndTrigModPtr = NULL;
				break;

/*컴컴컴컴�	DOOR 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

			case BGOTYPE_Door:
				Object->OpenTrigMod = OpenTrigMod;
				Object->CloseTrigMod = CloseTrigMod;
				if( OpenTrigMod == -1 ) Object->OpenTrigModPtr = NULL;
				else Object->OpenTrigModPtr = &TrigMods[ OpenTrigMod ];
				if( CloseTrigMod == -1 ) Object->CloseTrigModPtr = NULL;
				else Object->CloseTrigModPtr = &TrigMods[ CloseTrigMod ];
				Object->ShotTrigMod = ShotTrigMod;
				Object->BumpTrigMod = BumpTrigMod;
				Object->Locked = Locked;
				Object->PickupNeeded = PickupNeeded;
				if( ShotTrigMod == -1 ) Object->ShotTrigModPtr = NULL;
				else Object->ShotTrigModPtr = &TrigMods[ ShotTrigMod ];
				if( BumpTrigMod == -1 ) Object->BumpTrigModPtr = NULL;
				else Object->BumpTrigModPtr = &TrigMods[ BumpTrigMod ];
				Object->EndTrigMod = -1;
				Object->EndTrigModPtr = NULL;
				Object->OpenedBy = OpenedBy;
				Object->DoorSfxType = DoorSfxType;
				break;
	
/*컴컴컴컴�	LOOPING ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

			case BGOTYPE_Looping_Anim:
				Object->OpenTrigMod = -1;
				Object->CloseTrigMod = -1;
				Object->OpenTrigModPtr = NULL;
				Object->CloseTrigModPtr = NULL;
				Object->ShotTrigMod = ShotTrigMod;
				Object->BumpTrigMod = BumpTrigMod;
				Object->EndTrigMod = EndTrigMod;
				if( ShotTrigMod == -1 ) Object->ShotTrigModPtr = NULL;
				else Object->ShotTrigModPtr = &TrigMods[ ShotTrigMod ];
				if( BumpTrigMod == -1 ) Object->BumpTrigModPtr = NULL;
				else Object->BumpTrigModPtr = &TrigMods[ BumpTrigMod ];
				if( EndTrigMod == -1 ) Object->EndTrigModPtr = NULL;
				else Object->EndTrigModPtr = &TrigMods[ EndTrigMod ];
				Object->GenType = GenType;
				Object->Delay = ( Delay * ANIM_SECOND );

				switch( GenType )
				{
					case BGOGENTYPE_Immediate:
						Object->State = LOOPANIM_Animating;
						break;
					case BGOGENTYPE_AfterTime:
						Object->State = LOOPANIM_WaitForDelay;
						break;
					case BGOGENTYPE_Triggered:
						Object->State = LOOPANIM_WaitForTrigger;
						break;
				}
				break;

/*컴컴컴컴�	ONEOFF ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

			case BGOTYPE_OneOff_Anim:
				Object->OpenTrigMod = -1;
				Object->CloseTrigMod = -1;
				Object->OpenTrigModPtr = NULL;
				Object->CloseTrigModPtr = NULL;
				Object->ShotTrigMod = ShotTrigMod;
				Object->BumpTrigMod = BumpTrigMod;
				Object->EndTrigMod = EndTrigMod;
				if( ShotTrigMod == -1 ) Object->ShotTrigModPtr = NULL;
				else Object->ShotTrigModPtr = &TrigMods[ ShotTrigMod ];
				if( BumpTrigMod == -1 ) Object->BumpTrigModPtr = NULL;
				else Object->BumpTrigModPtr = &TrigMods[ BumpTrigMod ];
				if( EndTrigMod == -1 ) Object->EndTrigModPtr = NULL;
				else Object->EndTrigModPtr = &TrigMods[ EndTrigMod ];
				Object->GenType = GenType;
				Object->Delay = ( Delay * ANIM_SECOND );
				Object->DestroyAtEnd = DestroyAtEnd;

				switch( GenType )
				{
					case BGOGENTYPE_Immediate:
						Object->State = ONEOFFANIM_Animating;
						break;
					case BGOGENTYPE_AfterTime:
						Object->State = ONEOFFANIM_WaitForDelay;
						break;
					case BGOGENTYPE_Triggered:
						Object->State = ONEOFFANIM_WaitForTrigger;
						break;
				}
				break;

/*컴컴컴컴�	MULTISTEP ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

			case BGOTYPE_MultipleStep_Anim:
				Object->OpenTrigMod = -1;
				Object->CloseTrigMod = -1;
				Object->OpenTrigModPtr = NULL;
				Object->CloseTrigModPtr = NULL;
				Object->ShotTrigMod = ShotTrigMod;
				Object->BumpTrigMod = BumpTrigMod;
				Object->EndTrigMod = EndTrigMod;
				if( ShotTrigMod == -1 ) Object->ShotTrigModPtr = NULL;
				else Object->ShotTrigModPtr = &TrigMods[ ShotTrigMod ];
				if( BumpTrigMod == -1 ) Object->BumpTrigModPtr = NULL;
				else Object->BumpTrigModPtr = &TrigMods[ BumpTrigMod ];
				if( EndTrigMod == -1 ) Object->EndTrigModPtr = NULL;
				else Object->EndTrigModPtr = &TrigMods[ EndTrigMod ];
				Object->DestroyAtEnd = DestroyAtEnd;
				Object->OpenedBy = OpenedBy;
				break;

/*컴컴컴컴�	SEQUENCES ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

			case BGOTYPE_Sequences:
				break;

/*컴컴컴컴�	UNKNOWN ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

			default:
				break;
		}

		Object->Type = Type;
		Object->Group = Group;
		Object->StartPos = *Pos;
		Object->Pos = *Pos;
		Object->DirVector = *DirVector;
		Object->UpVector = *UpVector;
		CrossProduct( UpVector, DirVector, &Object->RightVector );
		Object->MoveOff.x = 0.0F;
		Object->MoveOff.y = 0.0F;
		Object->MoveOff.z = 0.0F;
		Object->CrushCount = 0;
		Object->Frame = 0.0F;
    	Object->Offset = 0.0F;
		Object->OffsetInc = 0.0F;
		Object->MaxOffset = 0.0F;
		Object->Timer = 0.0F;
		Object->Width = 1.0F;
		Object->Height = 1.0F;
		Object->Shield = 0.0F;
		Object->OldOldTime = 0.0F;
		Object->OldTime = 0.0F;
		Object->Time = 0.0F;
		Object->OverallTime = 0.0F;
		Object->MidTime = 0.0F;
		Object->AnimSpeed = 1.0F;
		Object->UpdateCount = 3;
		Object->SoundFX_ID = 0;
	
	   	QuatFromDirAndUp( DirVector, UpVector, &TempQuat );
	   	QuatToMatrix( &TempQuat, &Object->ColMatrix );
		MatrixTranspose( &Object->ColMatrix, &Object->InvColMatrix );
	   	TempMatrix = MATRIX_Identity;
	   	TempMatrix._11 = 1.0F;
	   	TempMatrix._22 = 1.0F;
	   	TempMatrix._33 = 1.0F;
	   	MatrixMultiply( &Object->ColMatrix, &TempMatrix, &Object->Matrix );
		MatrixTranspose( &Object->Matrix, &Object->InvMatrix );
	
		TempMatrix = MATRIX_Identity;
		TempMatrix._11 = 1.0F;
		TempMatrix._22 = 1.0F;
		TempMatrix._33 = 1.0F;
		Object->InvScaleMatrix = TempMatrix;
	
	   	TempMatrix = MATRIX_Identity;
	   	TempMatrix._11 = 1.0F;
	   	TempMatrix._22 = 1.0F;
	   	TempMatrix._33 = 1.0F;
		Object->ScaleMatrix = TempMatrix;
	
		Object->NumChildren = 1;
		Object->Children = LoadCompObj( Filename, Pos, DirVector, Group,
				    &Object->OverallTime, &Object->MidTime, BaseModel, OWNER_BGOBJECT, Object->Index );

		if( !Object->Children )
		{
	        Msg( "LoadBGObjectData() LoadCompObj() failded in %s\n", Filename );
			return( NULL );
		}

		switch( Type )
		{
			case BGOTYPE_MultipleStep_Anim:
				Object->StepInterval = ( Object->OverallTime / NumIntervals );
				Object->TimePerDamageUnit = ( Object->StepInterval / DamagePerInterval );
				break;

			case BGOTYPE_Static:
			case BGOTYPE_Door:
			case BGOTYPE_Looping_Anim:
			case BGOTYPE_OneOff_Anim:
			case BGOTYPE_Sequences:
			default:
				break;
		}

	}
	else
	{
        Msg( "LoadBGObjectData() unable to allocate bgobject in %s\n", Filename );
		return( NULL );
	}

	return( Object );
}

/*===================================================================
 	Procedure	:		Update BGObject State
	Input		:		u_int16_t		BGObjbect
				:		int16_t		State
				:		float		Time
	Output		:		Nothing
===================================================================*/
void UpdateBGObject( u_int16_t BGObject, int16_t State, float Time )
{
	int16_t	OldState;

	OldState = BGObjects[ BGObject ].State;

	BGObjects[ BGObject ].State = State;
	BGObjects[ BGObject ].Time = Time;

	switch( State )
	{
		case DOOR_OPENING:
			if( OldState != State )
			{
				StartOrChangeDoorSoundFX( &BGObjects[ BGObject ] );
			}
			break;

		default:
			break;
	}
}

/*===================================================================
	Procedure	:	Resort Back to last anim pos
	Input		:	BGOBJECT	*	BGObject to process
	Output		:	Nothing
===================================================================*/
void UndoBGObjectAnim( BGOBJECT * Object )
{
	if( Object->NumChildren && Object->Children )
	{
		Object->Time = Object->OldTime;
		Object->OldTime = Object->OldOldTime;

		UndoCompObjAnim( Object->Children, Object->NumChildren );
	}
}

/*===================================================================
	Procedure	:	Lock Door
	Input		:	u_int16_t			Index
	Output		:	Nothing
===================================================================*/
void LockDoor( u_int16_t Index )
{
	BGOBJECT * Object;

	Object = &BGObjects[ Index ];
	Object->Locked = true;
}

/*===================================================================
	Procedure	:	Unlock Door
	Input		:	u_int16_t			Index
	Output		:	Nothing
===================================================================*/
void UnlockDoor( u_int16_t Index )
{
	BGOBJECT * Object;

	Object = &BGObjects[ Index ];
	Object->Locked = false;
}

/*===================================================================
	Procedure	:	Start Anim
	Input		:	u_int16_t			Index
	Output		:	Nothing
===================================================================*/
void StartBGOAnim( u_int16_t Index )
{
	BGOBJECT * Object;

	Object = &BGObjects[ Index ];

	switch( Object->Type )
	{
/*컴컴컴컴�	STATIC ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

		case BGOTYPE_Static:
			break;

/*컴컴컴컴�	DOOR 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

		case BGOTYPE_Door:
			break;

/*컴컴컴컴�	LOOPING ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

		case BGOTYPE_Looping_Anim:
			switch( Object->State )
			{
				case LOOPANIM_WaitForDelay:
					Object->State = LOOPANIM_Animating;
					Object->Delay = 0.0F;
					break;

				case LOOPANIM_WaitForTrigger:
					if( Object->Delay )
					{
						Object->State = LOOPANIM_WaitForDelay;
					}
					else
					{
						Object->State = LOOPANIM_Animating;
						Object->Time = 0.0F;
					}
					break;

				case LOOPANIM_Animating:
				default:
					break;
			}
			break;

/*컴컴컴컴�	ONEOFF ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

		case BGOTYPE_OneOff_Anim:
			switch( Object->State )
			{
				case ONEOFFANIM_WaitForDelay:
					Object->State = ONEOFFANIM_Animating;
					Object->Delay = 0.0F;
					break;

				case ONEOFFANIM_WaitForTrigger:
					if( Object->Delay )
					{
						Object->State = ONEOFFANIM_WaitForDelay;
					}
					else
					{
						Object->State = ONEOFFANIM_Animating;
						Object->Time = 0.0F;
					}
					break;

				case ONEOFFANIM_Animating:
					break;

				default:
					break;
			}
			break;

/*컴컴컴컴�	MULTISTEP ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

		case BGOTYPE_MultipleStep_Anim:
			break;

/*컴컴컴컴�	SEQUENCES ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

		case BGOTYPE_Sequences:
			break;

/*컴컴컴컴�	UNKNOWN ANIM 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴*/

		default:
			break;
	}
}

/*===================================================================
	Procedure	:	Stop Anim
	Input		:	u_int16_t			Index
	Output		:	Nothing
===================================================================*/
void StopBGOAnim( u_int16_t Index )
{
	BGOBJECT * Object;

	Object = &BGObjects[ Index ];

	switch( Object->Type )
	{
		case BGOTYPE_Looping_Anim:
			switch( Object->State )
			{
				case LOOPANIM_Animating:
					Object->State = LOOPANIM_WaitForTrigger;
					break;

				case LOOPANIM_WaitForDelay:
				case LOOPANIM_WaitForTrigger:
				default:
					break;
			}
			break;

		case BGOTYPE_OneOff_Anim:
			switch( Object->State )
			{
				case ONEOFFANIM_Animating:
					Object->State = ONEOFFANIM_WaitForTrigger;
					break;

				case ONEOFFANIM_WaitForDelay:
				case ONEOFFANIM_WaitForTrigger:
				default:
					break;
			}
			break;

		case BGOTYPE_Sequences:
			break;

		case BGOTYPE_Static:
		case BGOTYPE_Door:
		case BGOTYPE_MultipleStep_Anim:
		default:
			break;
	}
}

/*===================================================================
	Procedure	:	Open Door
	Input		:	u_int16_t			Index
	Output		:	Nothing
===================================================================*/
void OpenDoor( u_int16_t Index )
{
	BGOBJECT * Object;

	Object = &BGObjects[ Index ];

	if( Object->Locked ) return;

	switch( Object->State )
	{
		case DOOR_CLOSED:
			Object->State = DOOR_OPENING;
			break;
						
		case DOOR_OPEN:
			break;

		case DOOR_OPENING:
			break;
		
		case DOOR_CLOSING:
			Object->State = DOOR_REVERSECLOSING;
			break;

		default:
			break;
	}
}

/*===================================================================
	Procedure	:	Open Door
	Input		:	u_int16_t			Index
	Output		:	Nothing
===================================================================*/
void CloseDoor( u_int16_t Index )
{
	BGOBJECT * Object;

	Object = &BGObjects[ Index ];

	switch( Object->State )
	{
		case DOOR_CLOSED:
			break;
						
		case DOOR_OPEN:
			Object->State = DOOR_CLOSING;
			break;

		case DOOR_OPENING:
			Object->State = DOOR_REVERSEOPENING;
			break;
		
		case DOOR_CLOSING:
			break;

		default:
			break;
	}
}

/*===================================================================
	Procedure	:	Display All Collision Zones
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void ShowAllColZones( u_int16_t Group )
{
	BGOBJECT	*	Object;
	BGOBJECT	*	NextObject;
	ENEMY		*	NMEObject;
	ENEMY		*	NextNMEObject;

	if( ShowColZones )
	{
		Object = FirstBGObjectUsed;

		while( Object != NULL )
		{
			NextObject = Object->NextUsed;

			if( Object->Group == Group ) //Ships[ WhoIAm ].Object.Group )
			{
				if( Object->NumChildren && Object->Children )
				{
					ShowCompObjColZones( Object->Children, Object->NumChildren, Object->Group );
				}
			}

			Object = NextObject;
		}

		NMEObject = FirstEnemyUsed;

		while( NMEObject != NULL )
		{
			NextNMEObject = NMEObject->NextUsed;

			if( NMEObject->Object.Group == Group ) //Ships[ WhoIAm ].NMEObject.Group )
			{
				if( NMEObject->Object.Components )
				{
					ShowCompObjColZones( NMEObject->Object.Components, 1, NMEObject->Object.Group );
				}
			}

			NMEObject = NextNMEObject;
		}
	}
}

/*===================================================================
	Procedure	:	Create Bounding Box ( Lines )
	Input		:	VECTOR	*	x1
				:	VECTOR	*	x2
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateBoundingBox( VECTOR * x1, VECTOR * x2, u_int16_t Group )
{
	CreateLine( x1->x, x1->y, x1->z, x2->x, x1->y, x1->z, Group );
	CreateLine( x2->x, x1->y, x1->z, x2->x, x2->y, x1->z, Group );
	CreateLine( x2->x, x2->y, x1->z, x1->x, x2->y, x1->z, Group );
	CreateLine( x1->x, x2->y, x1->z, x1->x, x1->y, x1->z, Group );

	CreateLine( x1->x, x1->y, x2->z, x2->x, x1->y, x2->z, Group );
	CreateLine( x2->x, x1->y, x2->z, x2->x, x2->y, x2->z, Group );
	CreateLine( x2->x, x2->y, x2->z, x1->x, x2->y, x2->z, Group );
	CreateLine( x1->x, x2->y, x2->z, x1->x, x1->y, x2->z, Group );

	CreateLine( x1->x, x1->y, x1->z, x1->x, x1->y, x2->z, Group );
	CreateLine( x2->x, x1->y, x1->z, x2->x, x1->y, x2->z, Group );
	CreateLine( x2->x, x2->y, x1->z, x2->x, x2->y, x2->z, Group );
	CreateLine( x1->x, x2->y, x1->z, x1->x, x2->y, x2->z, Group );
}

/*===================================================================
	Procedure	:	Update BGObjects Clip Group
	Input		:	CAMERA	*	Camera
	Output		:	nothing
===================================================================*/
void UpdateBGObjectsClipGroup( CAMERA * Camera )
{
	BGOBJECT	*	Object;
	BGOBJECT	*	NextObject;
	u_int16_t			ClipGroup;

	Object = FirstBGObjectUsed;

	while( Object != NULL )
	{
		NextObject = Object->NextUsed;

		if( IsGroupVisible[ Object->Group ] || (CAMERA_VIEW_IS_VALID && (
				IsGroupVisible[ Ships[ Current_Camera_View ].Object.Group ] ||
				VisibleOverlap( Ships[ Current_Camera_View ].Object.Group, Object->Group, NULL )
		)))
		{
			ClipGroup = FindClipGroup( Camera, &Mloadheader, &Object->TopLeft, &Object->BottomRight );
			UpdateCompObjClipGroup( Object->Children, Object->NumChildren, ClipGroup );
		}

		Object = NextObject;
	}
}


/*===================================================================
	Procedure	:	Calc Largest Radius of Bounding Box
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	TopLeft of bounding Box
				:	VECTOR	*	BottomRight of bounding Box
	Output		:	Nothing
===================================================================*/
float CalcObjectRadius( VECTOR * Pos, VECTOR * TopLeft, VECTOR * BottomRight )
{
	VECTOR	Vert;
	float	Distance = 0.0F;
	float	Length;

	Vert.x = TopLeft->x - Pos->x;
	Vert.y = TopLeft->y - Pos->y;
	Vert.z = TopLeft->z - Pos->z;
	Length = VectorLength( &Vert );
	if( Length > Distance ) Distance = Length;
	Vert.x = BottomRight->x - Pos->x;
	Vert.y = TopLeft->y - Pos->y;
	Vert.z = TopLeft->z - Pos->z;
	Length = VectorLength( &Vert );
	if( Length > Distance ) Distance = Length;
	Vert.x = TopLeft->x - Pos->x;
	Vert.y = BottomRight->y - Pos->y;
	Vert.z = TopLeft->z - Pos->z;
	Length = VectorLength( &Vert );
	if( Length > Distance ) Distance = Length;
	Vert.x = TopLeft->x - Pos->x;
	Vert.y = TopLeft->y - Pos->y;
	Vert.z = BottomRight->z - Pos->z;
	Length = VectorLength( &Vert );
	if( Length > Distance ) Distance = Length;
	Vert.x = BottomRight->x - Pos->x;
	Vert.y = BottomRight->y - Pos->y;
	Vert.z = TopLeft->z - Pos->z;
	Length = VectorLength( &Vert );
	if( Length > Distance ) Distance = Length;
	Vert.x = TopLeft->x - Pos->x;
	Vert.y = BottomRight->y - Pos->y;
	Vert.z = BottomRight->z - Pos->z;
	Length = VectorLength( &Vert );
	if( Length > Distance ) Distance = Length;
	Vert.x = BottomRight->x - Pos->x;
	Vert.y = TopLeft->y - Pos->y;
	Vert.z = BottomRight->z - Pos->z;
	Length = VectorLength( &Vert );
	if( Length > Distance ) Distance = Length;
	Vert.x = BottomRight->x - Pos->x;
	Vert.y = BottomRight->y - Pos->y;
	Vert.z = BottomRight->z - Pos->z;
	Length = VectorLength( &Vert );
	if( Length > Distance ) Distance = Length;
	return( Distance );
}

/*===================================================================
	Procedure	:	Check if Background object hit enemy
	Input		:	BGOBJECT	*	Object
	Output		:	bool			True/False
===================================================================*/
bool CheckBGObjectToEnemies( BGOBJECT * Object )
{
	int16_t		Count;
	GROUPLIST * GroupsVisible;
	u_int16_t	  * GroupList;
	u_int16_t		CurrentGroup;
	u_int16_t		DebugCount = 0;
	ENEMY	*	Enemy;
	ENEMY	*	NextEnemy;
	bool		HitFlag = false;
	VECTOR		PushVector;
	float		Damage;

	GroupsVisible = VisibleGroups( Ships[ Current_Camera_View ].Object.Group );
	GroupList = GroupsVisible->group;

	for( Count = 0; Count < GroupsVisible->groups; Count++ )
	{
		CurrentGroup = GroupList[ Count ];

		Enemy = EnemyGroups[ CurrentGroup ];

		while( Enemy )
		{
			NextEnemy = Enemy->NextInGroup;

			if( ( Enemy->Status & ENEMY_STATUS_Enable ) && Enemy->Alive )
			{
				if( DebugCount > MAXENEMIES )
				{
					Msg( "CheckBGObjectToEnemies() Link list corrupt!!" );
					return( (u_int16_t) -1 );
				}

				DebugCount++;

				if( CheckBGObjectCollision( &Enemy->Object.Pos, Object, &PushVector, &Damage, EnemyTypes[ Enemy->Type ].Radius ) )
				{
					HitFlag = true;

//					if( WouldObjectCollide( &Ships[ WhoIAm ].Object, &PushVector, EnemyTypes[ Enemy->Type ].Radius, NULL ) )
					{
						if( Damage != 0.0F )
						{
							Enemy->Object.Shield -= Damage;
							if( Enemy->Object.Shield < 0 ) Enemy->Object.Shield = 0;
							if( Enemy->Object.Shield == 0 )
							{
								CreateNewExplosion( &Enemy->Object.Pos, &PushVector, CurrentGroup );
								KillUsedEnemy( Enemy );
							}
							else
							{
								CreateColSparks( &Enemy->Object.Pos, &PushVector, CurrentGroup );
							}
						}
					}

					Enemy->Object.ExternalForce.x += PushVector.x;
					Enemy->Object.ExternalForce.y += PushVector.y;
					Enemy->Object.ExternalForce.z += PushVector.z;
				}
			}
			
			Enemy = NextEnemy;
		}
	}

	return( HitFlag );
}

/*===================================================================
	Procedure	:	Check if Background object hit pickup
	Input		:	BGOBJECT	*	Object
	Output		:	bool			True/False
===================================================================*/
extern  BYTE          MyGameStatus;
bool CheckBGObjectToPickups( BGOBJECT * Object )
{
	int16_t		Count;
	GROUPLIST * GroupsVisible;
	u_int16_t	  * GroupList;
	u_int16_t		CurrentGroup;
	u_int16_t		DebugCount = 0;
	PICKUP	*	Pickup;
	PICKUP	*	NextPickup;
	bool		HitFlag = false;
	VECTOR		PushVector;
	float		Damage;

	VECTOR		ColPoint;
	u_int16_t		ColGroup;
	NORMAL		ColNormal;

	GroupsVisible = VisibleGroups( Ships[ Current_Camera_View ].Object.Group );
	GroupList = GroupsVisible->group;

	for( Count = 0; Count < GroupsVisible->groups; Count++ )
	{
		CurrentGroup = GroupList[ Count ];

		Pickup = PickupGroups[ CurrentGroup ];

		while( Pickup )
		{
			NextPickup = Pickup->NextInGroup;

			if( DebugCount > MAXPICKUPS )
			{
				Msg( "CheckBGObjectToPickups() Link list corrupt!!" );
				return( (u_int16_t) -1 );
			}

			DebugCount++;

			if( CheckBGObjectCollision( &Pickup->Pos, Object, &PushVector, &Damage, PICKUP_MODEL_RADIUS ) )
			{
				HitFlag = true;

				Pickup->ExternalSpeed = ( 2.0F * VectorLength( &PushVector ) );
				Pickup->Dir = PushVector;
				Pickup->ColFlag = 0;
				NormaliseVector( &Pickup->Dir );

				if( QCollide( &Pickup->Pos, Pickup->Group, &PushVector, PICKUP_MODEL_RADIUS, 
							  &ColPoint, &ColGroup, &ColNormal ) )
				{
					if( MyGameStatus == STATUS_SinglePlayer )			// Pickup being crushed!!!
					{
						if( ReturnClosestNode( &Pickup->Pos, &ColPoint, &ColGroup, &Object->TopLeft, &Object->BottomRight ) >= 0.0F )
						{
							InitOnePickup( &ColPoint, ColGroup, &Forward, 0.0F, Pickup->Type,
										   Pickup->Owner, ++Ships[WhoIAm].PickupIdCount, -1,
										   true, -1.0F, (u_int16_t) -1 );

							KillPickup( Pickup->Owner, Pickup->ID, PICKUPKILL_Immediate );
						}
					}
					else
					{
//						KillPickup( Pickup->Owner, Pickup->ID, PICKUPKILL_Disappear );
					}
				}
			}
			
			Pickup = NextPickup;
		}
	}

	return( HitFlag );
}

/*===================================================================
	Procedure	:	Check if Background object hit Mine
	Input		:	BGOBJECT	*	Object
	Output		:	bool			True/False
===================================================================*/
bool CheckBGObjectToMines( BGOBJECT * Object )
{
	int16_t		Count;
	GROUPLIST * GroupsVisible;
	u_int16_t	  * GroupList;
	u_int16_t		CurrentGroup;
	SECONDARYWEAPONBULLET * SecBull;
	SECONDARYWEAPONBULLET * NextSecBull;
	bool		HitFlag = false;
	VECTOR		PushVector;
	float		Damage;

	GroupsVisible = VisibleGroups( Ships[ Current_Camera_View ].Object.Group );
	GroupList = GroupsVisible->group;

	for( Count = 0; Count < GroupsVisible->groups; Count++ )
	{
		CurrentGroup = GroupList[ Count ];

		SecBull = SecBullGroups[ CurrentGroup ];

		while( SecBull )
		{
			NextSecBull = SecBull->NextInGroup;

			switch( SecBull->SecType )
			{
				case SEC_MINE:
					if( CheckBGObjectCollision( &SecBull->Pos, Object, &PushVector, &Damage, MINE_RADIUS ) )
					{
						DestroySecondary( SecBull->Index, &SecBull->Pos );
					}
					break;

				case SEC_MISSILE:
				case SEC_XMISSILE:
				default:
					break;
			}
			
			SecBull = NextSecBull;
		}
	}

	return( HitFlag );
}

/*===================================================================
	Procedure	:	Explode Childern
	Input		:	COMP_OBJ	*	Children
				:	int16_t			Number of Children
	Output		:	Nothing
===================================================================*/
#define	STEPSIZE	( BLOCKSIZE * 0.5F )

void ExplodeChildren( COMP_OBJ * Children, int16_t NumChildren )
{
	int16_t	Count;
	VECTOR	Pos;
	VECTOR	TempPos;
	int16_t	XCount, YCount, ZCount;
	int16_t	XNum, YNum, ZNum;
	float	XOffset, YOffset, ZOffset;
	int16_t	Chance;
	float	Radius;
	u_int16_t	Group;

	int16_t	VisNum;
	u_int16_t	VisGroups[ MAXGROUPS ];
	int16_t	Count2;

	if( Children && NumChildren )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				ApplyMatrix( &Models[ Children->ModelIndex ].Mat,
							 &ModelHeaders[ Models[ Children->ModelIndex ].ModelNum ].Center, &Pos );
				Pos.x += Models[ Children->ModelIndex ].Pos.x;
				Pos.y += Models[ Children->ModelIndex ].Pos.y;
				Pos.z += Models[ Children->ModelIndex ].Pos.z;

				VisNum = VisibleOverlap( Ships[ Current_Camera_View ].Object.Group, Models[ Children->ModelIndex ].Group, &VisGroups[ 0 ] );
				Group = Models[ Children->ModelIndex ].Group;

				for( Count2 = 0; Count2 < VisNum; Count2++ )
				{
					if( PointInsideSkin( &Pos, VisGroups[ Count2 ] ) )
					{
						Group = VisGroups[ Count2 ];
						break;
					}
				}

				XNum = (int16_t) ( ModelHeaders[ Models[ Children->ModelIndex ].ModelNum ].Sizes.x / STEPSIZE );
				YNum = (int16_t) ( ModelHeaders[ Models[ Children->ModelIndex ].ModelNum ].Sizes.y / STEPSIZE );
				ZNum = (int16_t) ( ModelHeaders[ Models[ Children->ModelIndex ].ModelNum ].Sizes.z / STEPSIZE );
				if( !XNum ) XNum = 1;
				if( !YNum ) YNum = 1;
				if( !ZNum ) ZNum = 1;

				Chance = (int16_t) ( 5.0F + ( ( XNum * YNum * ZNum ) * 0.1F ) );

				Radius = VectorLength( &ModelHeaders[ Models[ Children->ModelIndex ].ModelNum ].Sizes ) + ( BLOCKSIZE * 0.5F );
				MissileShockWave( &Pos, Radius, WhoIAm, 20.0F, Models[ Children->ModelIndex ].Group, 0 );

				PlayPannedSfx( SFX_BangBang, Group, &Pos, 0.0F );

				ZOffset = Pos.z - ( ( ZNum / 2 ) * STEPSIZE );

				for( ZCount = 0; ZCount < ZNum; ZCount++ )
				{
					YOffset = Pos.y - ( ( YNum / 2 ) * STEPSIZE );

					for( YCount = 0; YCount < YNum; YCount++ )
					{
						XOffset = Pos.x - ( ( XNum / 2 ) * STEPSIZE );

						for( XCount = 0; XCount < XNum; XCount++ )
						{
							TempPos.x = XOffset;
							TempPos.y = YOffset;
							TempPos.z = ZOffset;

							if( PointInsideSkin( &TempPos, Group ) )
							{
								if( !Random_Range( Chance ) )
								{
									CreateSpotFXShrapnel( &TempPos, &Models[ Children->ModelIndex ].Dir, Group );
								}
								else
								{
									CreateBigExplosion( &TempPos, Group );
								}
							}
							else
							{
								DebugPrintf( "Attempt to generate shrapnel outside map\n" );
							}

							XOffset += STEPSIZE;
						}

						YOffset += STEPSIZE;
					}

					ZOffset += STEPSIZE;
				}
			}

			ExplodeChildren( Children->Children, Children->NumChildren );
			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Save BGObjects Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveBGObjects( FILE * fp )
{
	int		i;
	u_int16_t	TempIndex = (u_int16_t) -1;

	if( fp )
	{
		if( FirstBGObjectUsed != NULL ) fwrite( &FirstBGObjectUsed->Index, sizeof( u_int16_t ), 1, fp );
		else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
		if( FirstBGObjectFree != NULL ) fwrite( &FirstBGObjectFree->Index, sizeof( u_int16_t ), 1, fp );
		else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
		
		for( i = 0; i < MAXBGOBJECTS; i++ )
		{
			fwrite( &BGObjects[ i ].State, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].Type, sizeof( u_int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].Group, sizeof( u_int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].ModelIndex, sizeof( u_int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].StartPos, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].RightVector, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].MoveOff, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].CrushCount, sizeof( u_int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].Frame, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].Matrix, sizeof( MATRIX ), 1, fp );
			fwrite( &BGObjects[ i ].InvMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &BGObjects[ i ].ColMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &BGObjects[ i ].InvColMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &BGObjects[ i ].ScaleMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &BGObjects[ i ].InvScaleMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &BGObjects[ i ].Offset, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].OffsetInc, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].MaxOffset, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].Timer, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].Width, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].Height, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].Shield, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].NumChildren, sizeof( int16_t ), 1, fp );

			if( BGObjects[ i ].Children && BGObjects[ i ].NumChildren )
			{
				fp = SaveAllCompObj( fp, BGObjects[ i ].Children, BGObjects[ i ].NumChildren );
			}

			fwrite( &BGObjects[ i ].OldOldTime, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].OldTime, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].Time, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].OverallTime, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].MidTime, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].AnimSpeed, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].StateChangeDelay, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].OpenTrigMod, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].CloseTrigMod, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].ShotTrigMod, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].BumpTrigMod, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].EndTrigMod, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].Locked, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].OpenedBy, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].DestroyAtEnd, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].PickupNeeded, sizeof( int32_t ), 1, fp );
			fwrite( &BGObjects[ i ].GenType, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].Delay, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].WantedTime, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].DamageTime, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].StepInterval, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].TimePerDamageUnit, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].DamagePerInterval, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].Center, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].TopLeft, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].BottomRight, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].Radius, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].ColCenter, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].ColTopLeft, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].ColBottomRight, sizeof( VECTOR ), 1, fp );
			fwrite( &BGObjects[ i ].ColRadius, sizeof( float ), 1, fp );
			fwrite( &BGObjects[ i ].Index, sizeof( u_int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].UpdateCount, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].DoorSfxType, sizeof( int16_t ), 1, fp );
			fwrite( &BGObjects[ i ].SoundFX_ID, sizeof( u_int32_t ), 1, fp );

			if( BGObjects[ i ].PrevUsed != NULL ) fwrite( &BGObjects[ i ].PrevUsed->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( BGObjects[ i ].NextUsed != NULL ) fwrite( &BGObjects[ i ].NextUsed->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( BGObjects[ i ].PrevFree != NULL ) fwrite( &BGObjects[ i ].PrevFree->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( BGObjects[ i ].NextFree != NULL ) fwrite( &BGObjects[ i ].NextFree->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load BGObjects Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadBGObjects( FILE * fp )
{
	int		i;
	u_int16_t	TempIndex;
	int16_t	NumChildren;

	if( fp )
	{
		fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
		if( TempIndex != (u_int16_t) -1 ) FirstBGObjectUsed = &BGObjects[ TempIndex ];
		else FirstBGObjectUsed = NULL;
		fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
		if( TempIndex != (u_int16_t) -1 ) FirstBGObjectFree = &BGObjects[ TempIndex ];
		else FirstBGObjectFree = NULL;

		for( i = 0; i < MAXBGOBJECTS; i++ )
		{
			fread( &BGObjects[ i ].State, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].Type, sizeof( u_int16_t ), 1, fp );
			fread( &BGObjects[ i ].Group, sizeof( u_int16_t ), 1, fp );
			fread( &BGObjects[ i ].ModelIndex, sizeof( u_int16_t ), 1, fp );
			fread( &BGObjects[ i ].StartPos, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].RightVector, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].MoveOff, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].CrushCount, sizeof( u_int16_t ), 1, fp );
			fread( &BGObjects[ i ].Frame, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].Matrix, sizeof( MATRIX ), 1, fp );
			fread( &BGObjects[ i ].InvMatrix, sizeof( MATRIX ), 1, fp );
			fread( &BGObjects[ i ].ColMatrix, sizeof( MATRIX ), 1, fp );
			fread( &BGObjects[ i ].InvColMatrix, sizeof( MATRIX ), 1, fp );
			fread( &BGObjects[ i ].ScaleMatrix, sizeof( MATRIX ), 1, fp );
			fread( &BGObjects[ i ].InvScaleMatrix, sizeof( MATRIX ), 1, fp );
			fread( &BGObjects[ i ].Offset, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].OffsetInc, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].MaxOffset, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].Timer, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].Width, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].Height, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].Shield, sizeof( float ), 1, fp );
			fread( &NumChildren, sizeof( int16_t ), 1, fp );

			if( NumChildren )
			{
				fp = LoadAllCompObj( fp, BGObjects[ i ].Children, BGObjects[ i ].NumChildren );
			}
			else
			{
				if( BGObjects[ i ].Children && BGObjects[ i ].NumChildren )
				{
					FreeCompObjChildren( BGObjects[ i ].Children, BGObjects[ i ].NumChildren );
					free( BGObjects[ i ].Children );
					BGObjects[ i ].Children = NULL;
					BGObjects[ i ].NumChildren = 0;
				}
			}

			fread( &BGObjects[ i ].OldOldTime, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].OldTime, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].Time, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].OverallTime, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].MidTime, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].AnimSpeed, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].StateChangeDelay, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].OpenTrigMod, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].CloseTrigMod, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].ShotTrigMod, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].BumpTrigMod, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].EndTrigMod, sizeof( int16_t ), 1, fp );
			if( BGObjects[ i ].OpenTrigMod == -1 ) BGObjects[ i ].OpenTrigModPtr = NULL;
			else BGObjects[ i ].OpenTrigModPtr = &TrigMods[ BGObjects[ i ].OpenTrigMod ];
			if( BGObjects[ i ].CloseTrigMod == -1 ) BGObjects[ i ].CloseTrigModPtr = NULL;
			else BGObjects[ i ].CloseTrigModPtr = &TrigMods[ BGObjects[ i ].CloseTrigMod ];
			if( BGObjects[ i ].ShotTrigMod == -1 ) BGObjects[ i ].ShotTrigModPtr = NULL;
			else BGObjects[ i ].ShotTrigModPtr = &TrigMods[ BGObjects[ i ].ShotTrigMod ];
			if( BGObjects[ i ].BumpTrigMod == -1 ) BGObjects[ i ].BumpTrigModPtr = NULL;
			else BGObjects[ i ].BumpTrigModPtr = &TrigMods[ BGObjects[ i ].BumpTrigMod ];
			if( BGObjects[ i ].EndTrigMod == -1 ) BGObjects[ i ].EndTrigModPtr = NULL;
			else BGObjects[ i ].EndTrigModPtr = &TrigMods[ BGObjects[ i ].EndTrigMod ];
			fread( &BGObjects[ i ].Locked, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].OpenedBy, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].DestroyAtEnd, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].PickupNeeded, sizeof( int32_t ), 1, fp );
			fread( &BGObjects[ i ].GenType, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].Delay, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].WantedTime, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].DamageTime, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].StepInterval, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].TimePerDamageUnit, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].DamagePerInterval, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].Center, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].TopLeft, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].BottomRight, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].Radius, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].ColCenter, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].ColTopLeft, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].ColBottomRight, sizeof( VECTOR ), 1, fp );
			fread( &BGObjects[ i ].ColRadius, sizeof( float ), 1, fp );
			fread( &BGObjects[ i ].Index, sizeof( u_int16_t ), 1, fp );
			fread( &BGObjects[ i ].UpdateCount, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].DoorSfxType, sizeof( int16_t ), 1, fp );
			fread( &BGObjects[ i ].SoundFX_ID, sizeof( u_int32_t ), 1, fp );

			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) BGObjects[ i ].PrevUsed = &BGObjects[ TempIndex ];
			else  BGObjects[ i ].PrevUsed = NULL;
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) BGObjects[ i ].NextUsed = &BGObjects[ TempIndex ];
			else  BGObjects[ i ].NextUsed = NULL;
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) BGObjects[ i ].PrevFree = &BGObjects[ TempIndex ];
			else  BGObjects[ i ].PrevFree = NULL;
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) BGObjects[ i ].NextFree = &BGObjects[ TempIndex ];
			else  BGObjects[ i ].NextFree = NULL;
		}
	}

	return( fp );
}


/*===================================================================
	BIKE INFO
===================================================================*/
BIKEINFO BikeCompFiles[ MAXBIKETYPES ] = {

	{ "", 0, 0, 0.0F },				//"Lokasenna",
	{ "", 0, 0, 0.0F },				//"Beard",
	{ "", 0, 0, 0.0F },				//"L.A. Jay",
	{ "", 0, 0, 0.0F },				//"Ex-Cop",
	{ "", 0, 0, 0.0F },				//"Rex Hardy",
	{ "", 0, 0, 0.0F },				//"Foetoid",
	{ "", 0, 0, 0.0F },				//"Nim Soo Sun",
	{ "", 0, 0, 0.0F },				//"Nutta",
	{ "", 0, 0, 0.0F },				//"Sceptre",
	{ "", 0, 0, 0.0F },				//"Jo",
	{ "", 0, 0, 0.0F },				//"Cuvel Clark",
	{ "", 0, 0, 0.0F },				//"HK 5",
	{ "", 0, 0, 0.0F },				//"Nubia",
	{ "", 0, 0, 0.0F },				//"Mofisto",
	{ "", 0, 0, 0.0F },				//"Cerbero",
	{ "", 0, 0, 0.0F },				//"Slick",
	{ "FlyGirl.cob", 0, 0, 0.0F },	//"FlyGirl"
};

/*===================================================================
	Procedure	:	PreLoad All Components for Ships
	Input		:	Nothing
	Output		:	bool		true/false
===================================================================*/
bool PreLoadShips( void )
{
	int16_t	Count;
	u_int16_t	BaseModel;
	char	TempFilename[ 256 ];

	ModNames = &ModelNames[ 0 ];

	BaseModel = NextNewModel;

	for( Count = 0; Count < MAXBIKETYPES; Count++ )
	{
		if( BikeCompFiles[ Count ].Filename[ 0 ] )
		{
			sprintf( &TempFilename[ 0 ], "data\\bgobjects\\%s", BikeCompFiles[ Count ].Filename );

			BikeCompFiles[ Count ].BaseModel = BaseModel;
			BikeModels[ Count ] = BaseModel;

			if( !PreLoadCompObj( &TempFilename[ 0 ], &BaseModel, NOT_LEVEL_SPECIFIC ) )
			{
				Msg( "PreLoadShips() failed on %s\n", &TempFilename[ 0 ] );
				return( false );
			}

			sprintf( &TempFilename[ 0 ], "data\\bgobjects\\s%s", BikeCompFiles[ Count ].Filename );

			BikeCompFiles[ Count ].StealthBaseModel = BaseModel;

			if( !PreLoadCompObj( &TempFilename[ 0 ], &BaseModel, NOT_LEVEL_SPECIFIC ) )
			{
				Msg( "PreLoadShips() failed on %s\n", &TempFilename[ 0 ] );
				return( false );
			}
		}
	}

	NextNewModel = BaseModel;

	return( true );
}

/*===================================================================
	Procedure	:	Allocate all Components for Ship
	Input		:	u_int16_t	Ship
	Output		:	bool	true/false
===================================================================*/
bool AllocateCompShip( u_int16_t Ship )
{
	int16_t			Type;
	u_int16_t			StartModel, BaseModel;
	char			TempFilename[ 256 ];
	COMP_OBJ	*	CompObj;
	float			MidTime;
	VECTOR			DirVector;

	Type = ( Ships[ Ship ].BikeNum % MAXBIKETYPES );

	if( BikeCompFiles[ Type ].Filename[ 0 ] )
	{
		sprintf( &TempFilename[ 0 ], "data\\bgobjects\\%s", BikeCompFiles[ Type ].Filename );

		BaseModel = BikeCompFiles[ Type ].BaseModel;
		StartModel = BaseModel;

		ApplyMatrix( &Ships[ Ship ].Object.FinalMat, &Forward, &DirVector );

		CompObj = LoadCompObj( &TempFilename[ 0 ], &Ships[ Ship ].Object.Pos, &DirVector,
							   Ships[ Ship ].Object.Group, &BikeCompFiles[ Type ].OverallTime,
							   &MidTime, &BaseModel, OWNER_SHIP, Ship );
		if( CompObj )
		{
			SetStealthOffset( CompObj, 1, (int16_t) ( BaseModel - StartModel ) );

			Ships[ Ship ].Object.Components = CompObj;
			Ships[ Ship ].Object.AnimSpeed = 1.0F;
			Ships[ Ship ].Object.Time = 0.0F;
			Ships[ Ship ].Object.OverallTime = BikeCompFiles[ Type ].OverallTime;
		}
		else
		{
			Msg( "AllocateCompShip() failded in %s\n", &TempFilename[ 0 ] );
			return( false );
		}
	}
	else
	{
		return( false );
	}

	return( true );
}

/*===================================================================
	Procedure	:	Request Door SFX
	Input		:	int16_t	Type
	Output		:	Nothing
===================================================================*/
void RequestDoorTypeSFX( int16_t Type )
{
	DoorTypeSFX[ Type ].StartOpeningSoundFX	= ReturnSFXIndex( DoorTypeSFX[ Type ].StartOpeningName );	// Start Opening
	DoorTypeSFX[ Type ].OpeningSoundFX		= ReturnSFXIndex( DoorTypeSFX[ Type ].OpeningName );		// Opening
	DoorTypeSFX[ Type ].StartOpenSoundFX	= ReturnSFXIndex( DoorTypeSFX[ Type ].StartOpenName );		// Start Open
	DoorTypeSFX[ Type ].OpenSoundFX			= ReturnSFXIndex( DoorTypeSFX[ Type ].OpenName );			// Open
	DoorTypeSFX[ Type ].StartClosingSoundFX	= ReturnSFXIndex( DoorTypeSFX[ Type ].StartClosingName );	// Start Closing
	DoorTypeSFX[ Type ].ClosingSoundFX		= ReturnSFXIndex( DoorTypeSFX[ Type ].ClosingName );		// Closing
	DoorTypeSFX[ Type ].StartClosedSoundFX	= ReturnSFXIndex( DoorTypeSFX[ Type ].StartClosedName );	// Start Closed
	DoorTypeSFX[ Type ].ClosedSoundFX		= ReturnSFXIndex( DoorTypeSFX[ Type ].ClosedName );			// Closed
}

/*===================================================================
	Procedure	:	Start Or Change Door SFX
	Input		:	BGOBJECt	*	Door
	Output		:	Nothing
===================================================================*/
void StartOrChangeDoorSoundFX( BGOBJECT * Object )
{
	int16_t	Sfx1, Sfx2;

	if( Object->SoundFX_ID )
	{
		StopSfx( Object->SoundFX_ID );
		Object->SoundFX_ID = 0;
	}

	switch( Object->State )
	{
		case DOOR_CLOSED:
			Sfx1 = DoorTypeSFX[ Object->DoorSfxType ].StartClosedSoundFX;
			Sfx2 = DoorTypeSFX[ Object->DoorSfxType ].ClosedSoundFX;
			break;
			
		case DOOR_OPEN:
			Sfx1 = DoorTypeSFX[ Object->DoorSfxType ].StartOpenSoundFX;
			Sfx2 = DoorTypeSFX[ Object->DoorSfxType ].OpenSoundFX;
			break;

		case DOOR_OPENING:
			Sfx1 = DoorTypeSFX[ Object->DoorSfxType ].StartOpeningSoundFX;
			Sfx2 = DoorTypeSFX[ Object->DoorSfxType ].OpeningSoundFX;
			break;
		
		case DOOR_CLOSING:
			Sfx1 = DoorTypeSFX[ Object->DoorSfxType ].StartClosingSoundFX;
			Sfx2 = DoorTypeSFX[ Object->DoorSfxType ].ClosingSoundFX;
			break;

		case DOOR_REVERSEOPENING:
			Sfx1 = -1;
			Sfx2 = DoorTypeSFX[ Object->DoorSfxType ].ClosingSoundFX;
			break;

		case DOOR_REVERSECLOSING:
			Sfx1 = -1;
			Sfx2 = DoorTypeSFX[ Object->DoorSfxType ].OpeningSoundFX;
			break;

		default:
			Sfx1 = -1;
			Sfx2 = -1;
			break;
	}

	if( Sfx1 != -1 )
	{
		Object->SoundFX_ID = PlayFixedSpotSfx( Sfx1, Object->Group, &Object->Center, 1.0F, 1.0F, SPOT_SFX_TYPE_Normal );
	}

	if( Sfx2 != -1 )
	{
		Object->SoundFX_ID = PlayFixedSpotSfx( Sfx2, Object->Group, &Object->Center, 1.0F, 1.0F, SPOT_SFX_TYPE_Normal );
	}
}

BIKEINFO FlyGirlCompFile = { "FlyGirl.cob", 0, 0, 0.0F };
COMP_OBJ * FlyGirlComps = NULL;
float FlyGirlAnim;

/*===================================================================
	Procedure	:	PreLoad All Components for FlyGirl
	Input		:	Nothing
	Output		:	bool		true/false
===================================================================*/
bool PreLoadFlyGirl( void )
{
	u_int16_t	BaseModel;
	char	TempFilename[ 256 ];

	ModNames = &TitleModelNames[ 0 ];
	FlyGirlComps = NULL;

	BaseModel = TITLE_MODEL_ExtraModels;

	if( FlyGirlCompFile.Filename[ 0 ] )
	{
		sprintf( &TempFilename[ 0 ], "data\\bgobjects\\%s", FlyGirlCompFile.Filename );

		FlyGirlCompFile.BaseModel = BaseModel;

		if( !PreLoadCompObj( &TempFilename[ 0 ], &BaseModel, NOT_LEVEL_SPECIFIC ) )
		{
			Msg( "PreLoadFlyGirl() failed on %s\n", &TempFilename[ 0 ] );
			return( false );
		}
	}
	return( true );
}

/*===================================================================
	Procedure	:	Allocate all Components for Ship
	Input		:	Nothing
	Output		:	bool	true/false
===================================================================*/
bool AllocateCompFlyGirl( void )
{
	u_int16_t			BaseModel;
	char			TempFilename[ 256 ];
	COMP_OBJ	*	CompObj;
	float			MidTime;
	VECTOR			TempVector = { 0.0F, 0.0F, 0.0F };
	VECTOR			DirVector;

	DebugPrintf("Allocating components for flygirl...\n");

	if( FlyGirlCompFile.Filename[ 0 ] )
	{
		sprintf( &TempFilename[ 0 ], "data\\bgobjects\\%s", FlyGirlCompFile.Filename );

		BaseModel = FlyGirlCompFile.BaseModel;

		DirVector = Forward;

		CompObj = LoadCompObj( &TempFilename[ 0 ], &TempVector, &DirVector,
							   0, &FlyGirlCompFile.OverallTime,
							   &MidTime, &BaseModel, OWNER_SHIP, 0 );
		if( CompObj )
		{
			FlyGirlComps = CompObj;
		}
		else
		{
			Msg( "AllocateCompFlyGirl() failded in %s\n", &TempFilename[ 0 ] );
			return( false );
		}
	}
	else
	{
		return( false );
	}

	return( true );
}

/*===================================================================
	Procedure	:	Release Flygirl components and models
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void ReleaseFlyGirl( void )
{
	if( FlyGirlComps )
	{
		FreeCompObjChildren( FlyGirlComps, 1 );

		if( FlyGirlComps )
		{
			free( FlyGirlComps );
			FlyGirlComps = NULL;
		}
	}
}

/*===================================================================
	Procedure	:	Animate And Update Components
	Input		:	VECTOR	*	Pos
				:	MATRIX	*	Matrix
				:	bool		Visible
	Output		:	Nothing
===================================================================*/
void UpdateFlyGirl( VECTOR * Pos, MATRIX * Matrix, bool Visible )
{
	FlyGirlAnim += framelag;

	if( FlyGirlAnim >= FlyGirlCompFile.OverallTime )
	{
		FlyGirlAnim = FMOD( FlyGirlAnim, FlyGirlCompFile.OverallTime );
	}

	SetCompObjModelsState( FlyGirlComps, 1, Visible );
	UpdateCompObjChildren( FlyGirlComps, 1, Matrix, Pos, FlyGirlAnim, 0, Pos );
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif
