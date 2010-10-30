
/*===================================================================
		Include Files...	
===================================================================*/

#include <stdio.h>

#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "main.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "file.h"
#include "triggers.h"
#include "pickups.h"
#include "enemies.h"
#include "mload.h"
#include "water.h"
#include "spotfx.h"
#include "text.h"
#include "trigarea.h"
#include "extforce.h"
#include "rtlight.h"
#include "camera.h"
#include "teleport.h"
#include "screenpolys.h"
#include "xmem.h"
#include "util.h"

#undef DEBUG_TRIGGERS

/*===================================================================
		Defines
===================================================================*/
#define	TRG_VERSION_NUMBER	1

#define FLAG_MASK( N )	( 1 << ( (N) - 1 ) )

/*===================================================================
		Externals ...
===================================================================*/
extern	BYTE			WhoIAm;
extern	GLOBALSHIP		Ships[ MAX_PLAYERS+1 ];
extern	float framelag;
extern	BYTE	ChangeLevel_MyGameStatus;
extern	BOOL	CountDownOn;
extern	BOOL	KilledPlayer;
/*===================================================================
		Globals ...
===================================================================*/
TRIGGERVAR * AvatarActivated = NULL;
TRIGGERVAR * Level_End = NULL;
TRIGGERVAR * DecreaseTemperature = NULL;
TRIGGERVAR * MinimumTemperature = NULL;
TRIGGERVAR * TimeLimitTrigger = NULL;
int	NumOfTrigMods = 0;
TRIGGERMOD	*TrigMods = NULL;
int	NumOfTrigVars = 0;
TRIGGERVAR	*TrigVars = NULL;
int	NumOfTriggers = 0;
TRIGGER	*Triggers = NULL;
int	NumOfEvents = 0;
EVENT	*Events = NULL;
int	NumOfConditions = 0;
CONDITION *Conditions = NULL;

int init_shortcopy = 0;
SHORTTRIGGER *ShortTriggerCopy[ MAX_PLAYERS ];
SHORTTRIGVAR *ShortTrigVarCopy[ MAX_PLAYERS ];

int	NumOfActiveConditions = 0;
CONDITION ** ActiveConditions = NULL;

int NumOfTrigModQues = 0;
TRIGGERMODQUE *TrigModQue = NULL;


/*===================================================================
	Procedure	:		Add a trigger mod to the que..
	Input		:		TRIGGERMOD	* TrigMod
	Output		:		nothing
===================================================================*/
void AddTriggerModQue( TRIGGERMOD * TrigMod )
{
	int i;

	for( i = 0 ; i < NumOfTrigModQues ; i++ )
	{
		if( TrigModQue[i].Time == 0.0F )
			break;
	}
	if( i != NumOfTrigModQues )
	{
		TrigModQue[i].Time = TrigMod->Time;
		TrigModQue[i].TriggerMod = TrigMod;
	}
	else
		DebugPrintf( "Couldn't find slot in Trigger Que\n" );
}
/*===================================================================
	Procedure	:		Add a trigger mod to the que..
	Input		:		TRIGGERMOD	* TrigMod
	Output		:		nothing
===================================================================*/
void ProcessTriggerModQue( void )
{
	int i;
	TRIGGERMOD * TrigMod;
	for( i = 0 ; i < NumOfTrigModQues ; i++ )
	{
		if( TrigModQue[i].Time != 0.0F )
		{
			TrigModQue[i].Time -= framelag;
			if( TrigModQue[i].Time <= 0.0F )
			{
				TrigModQue[i].Time = 0.0F;
				TrigMod = TrigModQue[i].TriggerMod;
				ModifyTriggerVar( TrigMod->TrigVar, TrigMod->Op, TrigMod->Val );
			}
		}
	}
}


/*===================================================================
	Procedure	:		Update a trigger var..and then checks to see if 
						all the triggers associated with any condition that this
						trigger is used in is complete if it is Set the events off...
	Input		:		TRIGGERMOD	* TrigMod
	Output		:		nothing
===================================================================*/
void ApplyTriggerMod( TRIGGERMOD * TrigMod )
{
	// hacky way to stop pausing while inside a trigger zone messing up....
	if( framelag == 0.0F )
		return;
	while ( TrigMod )
	{
		if( TrigMod->Time != 0.0F )
		{
			AddTriggerModQue( TrigMod );
		}else{
			ModifyTriggerVar( TrigMod->TrigVar, TrigMod->Op, TrigMod->Val );
		}
		TrigMod = TrigMod->Next;
	}
}


/*===================================================================
	Procedure	:		Update a trigger var..and then checks to see if 
						all the triggers associated with any condition that this
						trigger is used in is complete if it is Set the events off...
	Input		:		TRIGGERVAR * TrigVar
						int	Op
						int	Val
	Output		:		nothing
===================================================================*/
void ModifyTriggerVar( TRIGGERVAR * TrigVar , int Op , int Val )
{
	BOOL	Doit;
	int		i;
	TRIGGER	* Trig;
	int		old_state;
	uint32	flags;
	int		count;

	old_state = TrigVar->State;
	switch( Op )
	{
	case TRIGGEROP_Set:
		TrigVar->State = Val;
		break;
	case TRIGGEROP_Reset:
		TrigVar->State = TrigVar->InitState;
		break;
	case TRIGGEROP_Inc:
		TrigVar->State += Val;
		break;
	case TRIGGEROP_Dec:
		TrigVar->State -= Val;
		break;
	case TRIGGEROP_Or:
		TrigVar->State |= Val;
		break;
	case TRIGGEROP_And:
		TrigVar->State &= Val;
		break;
	case TRIGGEROP_Xor:
		TrigVar->State ^= Val;
		break;
	case TRIGGEROP_Multiply:
		TrigVar->State *= Val;
		break;
	case TRIGGEROP_Divide:
		TrigVar->State /= Val;
		break;
	case TRIGGEROP_Random:
		TrigVar->State = (int) Random_Range( (uint16) Val );
		break;
	case TRIGGEROP_Setflag:
		TrigVar->State |= FLAG_MASK( Val );
		break;
	case TRIGGEROP_Clearflag:
		TrigVar->State &= ~FLAG_MASK( Val );
		break;
	}

	if( TrigVar->State == old_state ) return;

#ifdef DEBUG_TRIGGERS
	DebugPrintf( "modifytrigvar: var=%s(=%d) op=%d val=%d -> %d\n",
		TrigVar->Name, old_state, Op, Val, TrigVar->State );
#endif

	for( i = 0 ; i < TrigVar->NumOfTriggers ; i++ )
	{
		Trig = TrigVar->Triggers[i];
		Doit = FALSE;

		switch( Trig->Type )
		{
		case TRIGGERTYPE_Equal:
			if( TrigVar->State == Trig->ActiveState )
				Doit = TRUE;
			break;
		case TRIGGERTYPE_NotEqual:
			if( TrigVar->State != Trig->ActiveState )
				Doit = TRUE;
			break;
		case TRIGGERTYPE_Less:
			if( TrigVar->State < Trig->ActiveState )
				Doit = TRUE;
			break;
		case TRIGGERTYPE_Greater:
			if( TrigVar->State > Trig->ActiveState )
				Doit = TRUE;
			break;
		case TRIGGERTYPE_LessEqual:
			if( TrigVar->State <= Trig->ActiveState )
				Doit = TRUE;
			break;
		case TRIGGERTYPE_GreaterEqual:
			if( TrigVar->State >= Trig->ActiveState )
				Doit = TRUE;
			break;
		case TRIGGERTYPE_FlagCount:
			count = 0;
			for ( flags = TrigVar->State; flags; flags >>= 1 )
			{
				if ( flags & 1 )
					count++;
			}
			if ( count == Trig->ActiveState )
				Doit = TRUE;
			break;
		case TRIGGERTYPE_FlagTest:
			if ( TrigVar->State & FLAG_MASK( Trig->ActiveState ) )
				Doit = TRUE;
			break;
		}

		if ( Doit )
		{
#ifdef DEBUG_TRIGGERS
			DebugPrintf( "modifytrigvar: trigger %d (type=%d, activestate=%d) activated (was %d)\n",
				Trig - Triggers, (int) Trig->Type, Trig->ActiveState, Trig->Active );
#endif
			if( !(strncasecmp( "secret" , &TrigVar->Name[0], 6 ) ) )
			{
				PlaySfx( SFX_Secret, 1.0F );
			}
		}

		Trig->Active = Doit;

		if( Trig->Active )
		{
			TestAllConditions( Trig );
		}
	}
}

/*===================================================================
	Procedure	:		Test all Conditions..
	Input		:		TRIGGER * Trig
	Output		:		nothing
===================================================================*/
void TestAllConditions( TRIGGER * Trig )
{
	int i,e;
	TRIGGER * Trig2;
	CONDITION * Con;

	for( i = 0 ; i < Trig->NumOfConditions ; i ++ )
	{
		Con = Trig->Conditions[i];

		for( e = 0 ; e < Con->NumOfTriggers ; e ++ )
		{
			Trig2 = Con->Triggers[e];
			if( !Trig2->Active )
				break;

		}


		
		if( e == Con->NumOfTriggers )
		{
			AddCondition( Con );
		}
	}
}


/*===================================================================
	Procedure	:		Add Condition to the list of conditions that
						have been proved TRUE and will therefor have 
						there event list processed...
	Input		:		CONDITION * Con..
	Output		:		nothing
===================================================================*/
void AddCondition( CONDITION * Con )
{
	int i;

	for( i = 0 ; i < NumOfActiveConditions ; i++ )
	{
		if( ActiveConditions[i] == Con )
		{
			break;
		}
	}
	if( i != NumOfActiveConditions )
	{
		// it was allready in the list...
		return;
	}else{
		// it is a new condition...
#ifdef DEBUG_TRIGGERS
		DebugPrintf( "AddCondition: condition %d (triggers=%d) activated\n",
			Con - Conditions, Con->NumOfTriggers );
#endif
		ActiveConditions[NumOfActiveConditions] = Con;
		NumOfActiveConditions++;
	}
}

/*===================================================================
	Procedure	:		Process active conditions..
	Input		:		nothing
	Output		:		nothing
===================================================================*/
void ProcessActiveConditions( void )
{
	int i,f;
	CONDITION * Con;

	for( i = 0 ; i < NumOfActiveConditions ; i++ )
	{
		Con = ActiveConditions[i];

		
		// all triggers are Active so do the event List....
		for( f = 0 ; f < Con->NumOfEvents ; f++ )
		{
			ProcessEvent( Con->Events[f] );
		}
	}
	NumOfActiveConditions = 0;

	ProcessTriggerModQue();
}
	  
/*===================================================================
	Function	:	Event List Procs..
===================================================================*/
void (* EventList[ ])( uint8 * Data ) = {
		EVENT_PickupGenerate,			// TRIGGEREVENT_Pickup				0     
		EVENT_EnemyGenerate,			// TRIGGEREVENT_Enemy				1     
		EVENT_OpenDoor,					// TRIGGEREVENT_OpenDoor			2     
		EVENT_CloseDoor,				// TRIGGEREVENT_CloseDoor			3     
		EVENT_LockDoor,					// TRIGGEREVENT_LockDoor			4     
		EVENT_UnlockDoor,				// TRIGGEREVENT_UnlockDoor			5     
		EVENT_StartBGOAnim,				// TRIGGEREVENT_StartBGOAnim		6     
		EVENT_StopBGOAnim,				// TRIGGEREVENT_StopBGOAnim			7
		EVENT_StartTexAnim,				// TRIGGEREVENT_StartTexAnim		8     
		EVENT_StopTexAnim,				// TRIGGEREVENT_StopTexAnim			9
		EVENT_StartExternalForce,		// TRIGGEREVENT_StartExternalForce	10
		EVENT_StopExternalForce,		// TRIGGEREVENT_StopExternalForce	11
		EVENT_TeleportEnable,			// TRIGGEREVENT_EnableTeleport		12
		EVENT_TeleportDisable,			// TRIGGEREVENT_DisableTeleport		13
		EVENT_WaterFill,				// TRIGGEREVENT_WaterFill			14
		EVENT_WaterDrain,				// TRIGGEREVENT_WaterDrain			15
		EVENT_StartSpotFX,				// TRIGGEREVENT_StartSpotFX			16
		EVENT_StopSpotFX,				// TRIGGEREVENT_StopSpotFX			17
		EVENT_ShowMessages,				// TRIGGEREVENT_ShowMessage			18
		EVENT_EnableLight,				// TRIGGEREVENT_LightEnable			19
		EVENT_DisableLight,				// TRIGGEREVENT_LightDisable		20
		EVENT_TriggerAreaEnable,		// TRIGGEREVENT_TriggerAreaEnable	21
		EVENT_TriggerAreaDisable,		// TRIGGEREVENT_TriggerAreaDisable	22
		EVENT_CameraEnable,				// TRIGGEREVENT_CameraEnable		23
		EVENT_CameraDisable,			// TRIGGEREVENT_CameraDisable		24
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
};
 
/*===================================================================
	Procedure	:		Process Event...
	Input		:		EVENT * Event
	Output		:		nothing
===================================================================*/
void ProcessEvent( EVENT * Event )
{
#ifdef DEBUG_TRIGGERS
	DebugPrintf( "ProcessEvent: type=%d, data[0-3]=0x%08X\n",
		(int) Event->Type, *(int *) Event->Data );
#endif
	if( EventList[ Event->Type ] )
		( * EventList[ Event->Type ] )( (uint8*) &Event->Data);		//go off and do his thing...
}


/*===================================================================
	Procedure	:		Generate a Pickup Event..
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_PickupGenerate( uint8 * Data )
{
	InitSlotPickup( (uint16) *Data );
}

/*===================================================================
	Procedure	:		Generate an Enemy Event..
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_EnemyGenerate( uint8 * Data )
{
	EnableEnemy( (uint16) *Data );
}

/*===================================================================
	Procedure	:		Start SpotFX
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_StartSpotFX( uint8 * Data )
{
	uint16	*	Data2;

	Data2 = (uint16 *) Data;

	EnableSpotFX( *Data2 );
}

/*===================================================================
	Procedure	:		Stop SpotFX
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_StopSpotFX( uint8 * Data )
{
	uint16	*	Data2;

	Data2 = (uint16 *) Data;

	DisableSpotFX( *Data2 );
}

/*===================================================================
	Procedure	:		Open BGObject Door
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_OpenDoor( uint8 * Data )
{
	OpenDoor( (uint16) *Data );
}

/*===================================================================
	Procedure	:		Close BGObject Door
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_CloseDoor( uint8 * Data )
{
	CloseDoor( (uint16) *Data );
}

/*===================================================================
	Procedure	:		Lock BGObject Door
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_LockDoor( uint8 * Data )
{
	LockDoor( (uint16) *Data );
}

/*===================================================================
	Procedure	:		Unlock BGObject Door
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_UnlockDoor( uint8 * Data )
{
	UnlockDoor( (uint16) *Data );
}

/*===================================================================
	Procedure	:		Start BGObject Anim
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_StartBGOAnim( uint8 * Data )
{
	StartBGOAnim( (uint16) *Data );
}

/*===================================================================
	Procedure	:		Start BGObject Anim
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_StopBGOAnim( uint8 * Data )
{
	StopBGOAnim( (uint16) *Data );
}
/*===================================================================
	Procedure	:		Start Texture Anim
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_StartTexAnim( uint8 * Data )
{
	TriggerBackgroundAnimationGo( (uint16 *) Data );
}

/*===================================================================
	Procedure	:		Start Texture Anim
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_StopTexAnim( uint8 * Data )
{
	TriggerBackgroundAnimationStop( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Start Water Fill
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_WaterFill( uint8 * Data )
{
	TriggerWaterFill( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Start Water Drain
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_WaterDrain( uint8 * Data )
{
	TriggerWaterDrain( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Start Water Drain
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_ShowMessages( uint8 * Data )
{
	TriggerTextMessage( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Start External Force
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_StartExternalForce( uint8 * Data )
{
	StartExternalForce( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Stop External Force
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_StopExternalForce( uint8 * Data )
{
	StopExternalForce( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Enable real-time light
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_EnableLight( uint8 * Data )
{
	EnableRTLight( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Disable real-time light
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_DisableLight( uint8 * Data )
{
	DisableRTLight( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Enable trigger area
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_TriggerAreaEnable( uint8 * Data )
{
	EnableTriggerArea( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Disable trigger area
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_TriggerAreaDisable( uint8 * Data )
{
	DisableTriggerArea( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Enable Camera
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_CameraEnable( uint8 * Data )
{
	EnableRemoteCamera( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Disable Camera
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_CameraDisable( uint8 * Data )
{
	DisableRemoteCamera( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Enable Teleport
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_TeleportEnable( uint8 * Data )
{
	StartTeleport( (uint16 *) Data );
}
/*===================================================================
	Procedure	:		Disable Teleport
	Input		:		uint8 * Data
	Output		:		nothing
===================================================================*/
void EVENT_TeleportDisable( uint8 * Data )
{
	StopTeleport( (uint16 *) Data );
}


/*===================================================================
	Procedure	:		Load .Trg File
	Input		:		char	*	Filename
	Output		:		BOOL	TRUE/FALSE
===================================================================*/
BOOL Triggerload( char * Filename )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	char		*	OrgBuffer;
	int			*	intpnt;
	uint8		*	byteptr;
	float		*	floatptr;
	int			i, j, k;
	TRIGGERVAR	*	v;
	TRIGGER		*	t;
	TRIGGERMOD	*	m;
	EVENT		*	e;
	CONDITION	*	c;
	uint32			MagicNumber;
	uint32			VersionNumber;
	uint32		*	uint32Pnt;
	TRIGGERVAR	*	TVpnt;


  	NumOfTrigVars = 0;
	NumOfTriggers = 0;
	NumOfTrigMods = 0;
	NumOfTrigModQues = 0;
	NumOfEvents = 0;
	NumOfConditions = 0;
	Level_End = NULL;
	AvatarActivated = NULL;
	DecreaseTemperature = NULL;
	MinimumTemperature = NULL;
	TimeLimitTrigger = NULL;


	File_Size = Get_File_Size( Filename );
	if( !File_Size ) return TRUE;

	Buffer = malloc( File_Size );

	if( Buffer == NULL )
		return FALSE;
	OrgBuffer = Buffer;

	Read_Size = Read_File( Filename, Buffer, File_Size );

	if( Read_Size != File_Size )
		return FALSE;

	uint32Pnt = (uint32 *) Buffer;
	MagicNumber = *uint32Pnt++;
	VersionNumber = *uint32Pnt++;
	Buffer = (char *) uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != TRG_VERSION_NUMBER  ) )
	{
		Msg( "TriggerLoad() Incompatible triggers( .TRG ) file %s", Filename );
		return( FALSE );
	}

	intpnt = (int *) Buffer;

	if ( TrigVars )
	{
		for ( i = 0; i < NumOfTrigVars; i++ )
		{
			if ( TrigVars[ i ].Triggers )
				free( TrigVars[ i ].Triggers );
		}
		free( TrigVars );
	}
	TrigVars = NULL;

	if ( Triggers )
	{
		for ( i = 0; i < NumOfTriggers; i++ )
		{
			if ( Triggers[ i ].Conditions )
				free( Triggers[ i ].Conditions );
		}
		free( Triggers );
	}
	Triggers = NULL;

	if ( Events )
	{
		free( Events );
	}
	Events = NULL;

	if ( Conditions )
	{
		for ( i = 0; i < NumOfConditions; i++ )
		{
			if ( Conditions[ i ].Triggers )
				free( Conditions[ i ].Triggers );
			if ( Conditions[ i ].Events )
				free( Conditions[ i].Events );
		}
		free( Conditions );
	}
	Conditions = NULL;

	if ( TrigModQue )
	{
		free( TrigModQue );
	}
	TrigModQue = NULL;

	for ( i = 0; i < MAX_PLAYERS; i++ )
	{
		if ( init_shortcopy && ShortTriggerCopy[ i ] )
			free( ShortTriggerCopy[ i ] );
		ShortTriggerCopy[ i ] = NULL;
		if ( init_shortcopy && ShortTrigVarCopy[ i ] )
			free( ShortTrigVarCopy[ i ] );
		ShortTrigVarCopy[ i ] = NULL;
	}
	init_shortcopy = 1;

	if ( ActiveConditions )
	{
		free( ActiveConditions );
	}
	ActiveConditions = NULL;

	NumOfTrigVars = *intpnt++;
	NumOfTriggers = *intpnt++;
	NumOfTrigMods = *intpnt++;
	NumOfTrigModQues = NumOfTrigMods * 2; // bodge it and scarper...
	NumOfEvents = *intpnt++;
	NumOfConditions = *intpnt++;
	NumOfActiveConditions = 0;

	if ( NumOfTrigVars )
	{
		TrigVars = (TRIGGERVAR *) calloc( NumOfTrigVars, sizeof( TRIGGERVAR ) );
		if ( !TrigVars )
		{
			Msg( "TriggerLoad() malloc failed for %d triggervars", NumOfTrigVars );
			return FALSE;
		}
		for ( i = 0; i < MAX_PLAYERS; i++ )
		{
			ShortTrigVarCopy[ i ] = (SHORTTRIGVAR *) calloc( NumOfTrigVars, sizeof( SHORTTRIGVAR ) );
			if ( !ShortTrigVarCopy[ i ] )
			{
				Msg( "TriggerLoad() malloc failed for %d short triggervars for player %d of %d",
					NumOfTrigVars, i, MAX_PLAYERS );
				return FALSE;
			}
		}
	}

	if ( NumOfTriggers )
	{
		Triggers = (TRIGGER *) calloc( NumOfTriggers, sizeof( TRIGGER ) );
		if ( !Triggers )
		{
			Msg( "TriggerLoad() malloc failed for %d triggers", NumOfTriggers );
			return FALSE;
		}
		for ( i = 0; i < MAX_PLAYERS; i++ )
		{
			ShortTriggerCopy[ i ] = (SHORTTRIGGER *) calloc( NumOfTriggers, sizeof( SHORTTRIGGER ) );
			if ( !ShortTriggerCopy[ i ] )
			{
				Msg( "TriggerLoad() malloc failed for %d short triggers for player %d of %d",
					NumOfTriggers, i, MAX_PLAYERS );
				return FALSE;
			}
		}
	}

	if ( NumOfTrigMods )
	{
		TrigMods = (TRIGGERMOD *) calloc( NumOfTrigMods, sizeof( TRIGGERMOD ) );
		if ( !TrigMods )
		{
			Msg( "TriggerLoad() malloc failed for %d triggermods", NumOfTrigMods );
			return FALSE;
		}
		TrigModQue = (TRIGGERMODQUE *) calloc( NumOfTrigModQues, sizeof( TRIGGERMODQUE ) );
		if ( !TrigModQue )
		{
			Msg( "TriggerLoad() malloc failed for %d triggermodques", NumOfTrigModQues );
			return FALSE;
		}
		for( i = 0 ; i < NumOfTrigModQues ; i++ )
		{
			TrigModQue[i].Time = 0.0F;
		}
	}

	if ( NumOfEvents )
	{
		Events = (EVENT *) calloc( NumOfEvents, sizeof( EVENT ) );
		if ( !Events )
		{
			Msg( "TriggerLoad() malloc failed for %d events", NumOfEvents );
			return FALSE;
		}
	}

	if ( NumOfConditions )
	{
		Conditions = (CONDITION *) calloc( NumOfConditions, sizeof( CONDITION ) );
		if ( !Conditions )
		{
			Msg( "TriggerLoad() malloc failed for %d conditions", NumOfConditions );
			return FALSE;
		}
		ActiveConditions = (CONDITION **) calloc( NumOfConditions, sizeof( CONDITION ) );
		if ( !ActiveConditions )
		{
			Msg( "TriggerLoad() malloc failed for %d active conditions", NumOfConditions );
			return FALSE;
		}
	}


	for ( i = 0; i < NumOfTrigVars; i++ )
	{
		v = &TrigVars[ i ];
		byteptr = (uint8 *) intpnt;
		for ( j = 0; j < (int) sizeof( v->Name ); j++ )
		{
			v->Name[ j ] = *byteptr++;
		}
		intpnt = (int *) byteptr;
		v->InitState = *intpnt++;
		v->State = v->InitState;
		v->NumOfTriggers = *intpnt++;
		if ( v->NumOfTriggers )
		{
			v->Triggers = (TRIGGER **) calloc( v->NumOfTriggers, sizeof( TRIGGER * ) );
			if ( !v->Triggers )
			{
				Msg( "TriggerLoad() malloc failed for %d triggers in trigvar %d of %d",
					v->NumOfTriggers, i, NumOfTrigVars );
				return FALSE;
			}
		}
		else
			v->Triggers = NULL;
		for ( j = 0; j < v->NumOfTriggers; j++ )
		{
			k = *intpnt++;
			if ( k >= NumOfTriggers )
				return FALSE;
			v->Triggers[ j ] = &Triggers[ k ];
		}
	}

	for ( i = 0; i < NumOfTriggers; i++ )
	{
		t = &Triggers[ i ];
		byteptr = (uint8 *) intpnt;
		t->Type = *byteptr++;
		intpnt = (int *) byteptr;
		j = *intpnt++;
		if ( j >= NumOfTrigVars )
			return FALSE;
		t->TrigVar = &TrigVars[ j ];
		t->ActiveState = *intpnt++;
		t->Active = 0;
		t->NumOfConditions = *intpnt++;
		if ( t->NumOfConditions )
		{
			t->Conditions = (CONDITION **) calloc( t->NumOfConditions, sizeof( CONDITION * ) );
			if ( !t->Conditions )
			{
				Msg( "TriggerLoad() malloc failed for %d conditions in trigger %d of %d\n",
					t->NumOfConditions, i, NumOfTriggers );
				return FALSE;
			}
		}
		else
			t->Conditions = NULL;
		for ( j = 0; j < t->NumOfConditions; j++ )
		{
			k = *intpnt++;
			if ( k >= NumOfConditions )
				return FALSE;
			t->Conditions[ j ] = &Conditions[ k ];
		}
	}

	for ( i = 0; i < NumOfTrigMods; i++ )
	{
		m = &TrigMods[ i ];
		m->Op = *intpnt++;
		j = *intpnt++;
		if ( j >= NumOfTrigVars )
			return FALSE;
		m->TrigVar = &TrigVars[ j ];
		m->Val = *intpnt++;

		floatptr = (float*) intpnt;
		m->Time = *floatptr++ * 60.0F;
		intpnt = (int*) floatptr;
				
		j = *intpnt++;
		if ( j >= NumOfTrigMods )
			return FALSE;
		m->Next = ( j < 0 ) ? NULL : &TrigMods[ j ];

	}

	byteptr = (uint8 *) intpnt;
	for ( i = 0; i < NumOfEvents; i++ )
	{
		e = &Events[ i ];
		e->Type = *byteptr++;
		for ( j = 0; j < MAXDATAPEREVENT; j++ )
		{
			e->Data[ j ] = *byteptr++;
		}
	}

	intpnt = (int *) byteptr;
	for ( i = 0; i < NumOfConditions; i++ )
	{
		c = &Conditions[ i ];
		c->NumOfTriggers = *intpnt++;
		if ( c->NumOfTriggers )
		{
			c->Triggers =  (TRIGGER **) calloc( c->NumOfTriggers, sizeof( TRIGGER * ) );
			if ( !c->Triggers )
			{
				Msg( "TriggerLoad() malloc failed for %d triggers in condition %d of %d",
					c->NumOfTriggers, i, NumOfConditions );
				return FALSE;
			}
		}
		else
			c->Triggers = NULL;
		for ( j = 0; j < c->NumOfTriggers; j++ )
		{
			k = *intpnt++;
			if ( k >= NumOfTriggers )
				return FALSE;
			c->Triggers[ j ] = &Triggers[ k ];
		}
		c->NumOfEvents = *intpnt++;
		if ( c->NumOfEvents )
		{
			c->Events = (EVENT **) calloc( c->NumOfEvents, sizeof( EVENT * ) );
			if ( !c->Events )
			{
				Msg( "TriggerLoad() malloc failed for %d events in condition %d of %d",
					c->NumOfEvents, i, NumOfConditions );
				return FALSE;
			}
		}
		else
			c->Events = NULL;
		for ( j = 0; j < c->NumOfEvents; j++ )
		{
			k = *intpnt++;
			if ( k > NumOfEvents )
				return FALSE;
			c->Events[ j ] = &Events[ k ];
		}
	}

	TVpnt =	TrigVars;
	if( TVpnt )
	{
		for( i = 0 ; i < NumOfTrigVars ; i ++ )
		{
			if( !(strncasecmp( "LEVEL_END" , &TVpnt->Name[0], 9 ) ) )
			{
				Level_End = TVpnt;
				break;
			}
			TVpnt++;
		}
	}
	if( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer || ChangeLevel_MyGameStatus == STATUS_SinglePlayer || ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) )

	{
		if( !Level_End)
		{
			Msg( "TriggerLoad() Couldnt find a Level_end var");
		}
	}

	// Search for the Avatar boss Activation	
	TVpnt =	TrigVars;
	if( TVpnt )
	{
		for( i = 0 ; i < NumOfTrigVars ; i ++ )
		{
			if( !(strncasecmp( "AvatarActivated" , &TVpnt->Name[0], 15 ) ) )
			{
				AvatarActivated = TVpnt;
				break;
			}
			TVpnt++;
		}
	}

/*===================================================================
	Search for Temperature Variable
===================================================================*/
	TVpnt =	TrigVars;
	if( TVpnt )
	{
		for( i = 0 ; i < NumOfTrigVars ; i ++ )
		{
			if( !(strncasecmp( "Decreasetemperature" , &TVpnt->Name[0], 19 ) ) )
			{
				DecreaseTemperature = TVpnt;
				break;
			}
			TVpnt++;
		}

		TVpnt =	TrigVars;
		for( i = 0 ; i < NumOfTrigVars ; i ++ )
		{
			if( !(strncasecmp( "Minimumtemperature" , &TVpnt->Name[0], 18 ) ) )
			{
				MinimumTemperature = TVpnt;
				break;
			}
			TVpnt++;
		}
	}
/*===================================================================
	Search for Temperature Variable
===================================================================*/
	TVpnt =	TrigVars;
	if( TVpnt )
	{
		for( i = 0 ; i < NumOfTrigVars ; i ++ )
		{
			if( !(strncasecmp( "Timelimit" , &TVpnt->Name[0], 9 ) ) )
			{
				TimeLimitTrigger = TVpnt;
				break;
			}
			TVpnt++;
		}
	}
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

	for( i = 0 ; i < NumOfTrigModQues ; i++ )
	{
		TrigModQue[i].Time = 0.0F;
	}
	NumOfActiveConditions = 0;
	
	free( OrgBuffer );
	return TRUE;
}



/*===================================================================
	Procedure	:		release triggers
	Input		:		none
	Output		:		none
===================================================================*/
void ReleaseTriggers( void )
{
	int i;

	if ( TrigVars )
	{
		for ( i = 0; i < NumOfTrigVars; i++ )
		{
			if ( TrigVars[ i ].Triggers )
				free( TrigVars[ i ].Triggers );
		}
		free( TrigVars );
		for ( i = 0; i < MAX_PLAYERS; i++ )
		{
			if ( ShortTrigVarCopy[ i] )
				free( ShortTrigVarCopy[ i ] );
			ShortTrigVarCopy[ i ] = NULL;
		}
	}
	TrigVars = NULL;

	if ( Triggers )
	{
		for ( i = 0; i < NumOfTriggers; i++ )
		{
			if ( Triggers[ i ].Conditions )
				free( Triggers[ i ].Conditions );
		}
		free( Triggers );
		for ( i = 0; i < MAX_PLAYERS; i++ )
		{
			if ( ShortTriggerCopy[ i] )
				free( ShortTriggerCopy[ i ] );
			ShortTriggerCopy[ i ] = NULL;
		}
	}
	Triggers = NULL;

	if ( Events )
	{
		free( Events );
	}
	Events = NULL;

	if ( Conditions )
	{
		for ( i = 0; i < NumOfConditions; i++ )
		{
			if ( Conditions[ i ].Triggers )
				free( Conditions[ i ].Triggers );
			if ( Conditions[ i ].Events )
				free( Conditions[ i].Events );
		}
		free( Conditions );
	}
	Conditions = NULL;

	if ( ActiveConditions )
	{
		free( ActiveConditions );
	}
	ActiveConditions = NULL;

	if ( TrigMods )
	{
		free( TrigMods );
	}
	TrigMods = NULL;

	if ( TrigModQue )
	{
		free( TrigModQue );
	}
	TrigModQue = NULL;
}

/*===================================================================
	Procedure	:	Generate Trigger list for new player
	Input		:	uint16			Ship
				:	SHORTTRIGGER *	Trigger Array to be filled in
				:	BYTE		*	Number of Triggers to be filled in
				:	BYTE			Section group of Triggers
	Output		:	Nothing
===================================================================*/
void GenTriggerList( uint16 Ship, SHORTTRIGGER * TriggerSlots, BYTE * NumTriggerSlots, BYTE Section )
{
	int16	Count;
	int16	i;
	int16	NumUsed = 0;

	for( Count = 0; Count < MAXGENTRIGGERCOUNT; Count++ )
	{
		i = ( Count + ( ( Section - 1 ) * MAXGENTRIGGERCOUNT ) );

		if( i < NumOfTriggers )
		{
			TriggerSlots->Active = ShortTriggerCopy[ Ship ][ i ].Active;
			TriggerSlots++;
			NumUsed++;
		}
	}

#if DEBUG_TRIGGERS
	DebugPrintf( "Part %d, %d Triggers\n", ( Section - 1 ), NumUsed );
#endif

	*NumTriggerSlots = (BYTE) NumUsed;
}

/*===================================================================
	Procedure	:	Regeneralte Trigger Info from list for new player
	Input		:	SHORTTRIGGER *	Trigger Array
				:	BYTE			Number of Trigger Slots
	Output		:	Nothing
===================================================================*/
void RegenTriggerList( SHORTTRIGGER * Slots, BYTE Num )
{
	int16		Count;
	TRIGGER	*	Ptr;

	Ptr = &Triggers[ ( Ships[ WhoIAm ].Triggers * MAXGENTRIGGERCOUNT ) ];

	for( Count = 0; Count < Num; Count++ )
	{
#if DEBUG_TRIGGERS
		DebugPrintf( "Part %d, Trigger %d Active = %d\n", Ships[ WhoIAm ].Triggers, ( ( Ships[ WhoIAm ].Triggers * MAXGENTRIGGERCOUNT ) + Count ), Slots->Active );
#endif
		Ptr->Active = Slots->Active;
		Slots++;
		Ptr++;
	}
}

/*===================================================================
	Procedure	:	Copy Triggers into copy for player
	Input		:	uint16		Player
	Output		:	Nothing
===================================================================*/
void CopyTriggers( uint16 Player )
{
	int16	Count;

	for( Count = 0; Count < NumOfTriggers; Count++ )
	{
		ShortTriggerCopy[ Player ][ Count ].Active = Triggers[ Count ].Active;
	}
}

/*===================================================================
	Procedure	:	Generate TrigVar list for new player
	Input		:	uint16			Ship
				:	SHORTTRIGVAR *	TrigVar Array to be filled in
				:	BYTE		*	Number of TrigVars to be filled in
				:	BYTE			Section group of TrigVars
	Output		:	Nothing
===================================================================*/
void GenTrigVarList( uint16 Ship, SHORTTRIGVAR * TrigVarSlots, BYTE * NumTrigVarSlots, BYTE Section )
{
	int16	Count;
	int16	i;
	int16	NumUsed = 0;

	for( Count = 0; Count < MAXGENTRIGVARCOUNT; Count++ )
	{
		i = ( Count + ( ( Section - 1 ) * MAXGENTRIGVARCOUNT ) );

		if( i < NumOfTrigVars )
		{
			TrigVarSlots->State = ShortTrigVarCopy[ Ship ][ i ].State;
			TrigVarSlots++;
			NumUsed++;
		}
	}

#if DEBUG_TRIGGERS
	DebugPrintf( "Part %d, %d TrigVars\n", ( Section - 1 ), NumUsed );
#endif

	*NumTrigVarSlots = (BYTE) NumUsed;
}

/*===================================================================
	Procedure	:	Regeneralte TrigVar Info from list for new player
	Input		:	SHORTTRIGVAR *	TrigVar Array
				:	BYTE			Number of TrigVar Slots
	Output		:	Nothing
===================================================================*/
void RegenTrigVarList( SHORTTRIGVAR * Slots, BYTE Num )
{
	int16		Count;
	TRIGGERVAR	*	Ptr;

	Ptr = &TrigVars[ ( Ships[ WhoIAm ].TrigVars * MAXGENTRIGVARCOUNT ) ];

	for( Count = 0; Count < Num; Count++ )
	{
#if DEBUG_TRIGGERS
		DebugPrintf( "Part %d, TrigVar %d State = %d\n", Ships[ WhoIAm ].TrigVars, ( ( Ships[ WhoIAm ].TrigVars * MAXGENTRIGVARCOUNT ) + Count ), Slots->State );
#endif
		Ptr->State = Slots->State;
		Slots++;
		Ptr++;
	}
}

/*===================================================================
	Procedure	:	Copy TrigVars into copy for player
	Input		:	uint16		Player
	Output		:	Nothing
===================================================================*/
void CopyTrigVars( uint16 Player )
{
	int16	Count;

	for( Count = 0; Count < NumOfTrigVars; Count++ )
	{
		ShortTrigVarCopy[ Player ][ Count ].State = TrigVars[ Count ].State;
	}
}




/*===================================================================
	Procedure	:	Count how may secrest there are and how many have been found..
	Input		:	int * NumOfSecrets , int * SecretsFound
	Output		:	Nothing
===================================================================*/
void HowManySecrets( int * NumOfSecrets , int * SecretsFound )
{
	int i;
	TRIGGERVAR	* TVpnt;

	*NumOfSecrets = 0;
	*SecretsFound = 0;
	TVpnt =	TrigVars;
	if( !TVpnt )
		return;
	for( i = 0 ; i < NumOfTrigVars ; i ++ )
	{
		
		if( !(strncasecmp( "secret" , &TVpnt->Name[0], 6 ) ) )
		{
			*NumOfSecrets += 1;
			if( TVpnt->State )
			{
				*SecretsFound += 1;
			}
		}
		TVpnt++;
	}
}
/*===================================================================
	Procedure	:	Has the level ended
	Input		:	Nothing
	Output		:	BOOL yes/no
===================================================================*/
BOOL HasLevelEnded( void )
{
	TRIGGERVAR	* TVpnt;

	TVpnt = Level_End;

	if( TVpnt )
	{
		if( TVpnt->State )
		{
			// Level is Complete...
			return TRUE;
		}
	}

	return FALSE;
}

/*===================================================================
	Procedure	:	Am i in cool pool
	Input		:	Nothing
	Output		:	BOOL yes/no
===================================================================*/
BOOL CheckDecreaseTemperature( void )
{
	TRIGGERVAR	* TVpnt;

	TVpnt = DecreaseTemperature;

	if( TVpnt )
	{
		if( TVpnt->State )
		{
			TVpnt->State = 0;
			return TRUE;
		}
	}

	TVpnt = MinimumTemperature;

	if( TVpnt )
	{

	}

	return FALSE;
}

/*===================================================================
	Procedure	:	Check if timer enabled
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void CheckTimeLimit( void )
{
	TRIGGERVAR	* TVpnt;

	TVpnt = TimeLimitTrigger;

	if( TVpnt )
	{
		if( TVpnt->State )
		{
			CountDownOn = TRUE;
			KilledPlayer = FALSE;

			CreateCountdownDigits();
			StartCountDown( 1, 0 );
			ResetCountDownBombTag( ( TVpnt->State * 100.0F ) );
			TimeLimitTrigger = NULL;
		}
	}
}

/*===================================================================
	Procedure	:	Has the Avatar Boss been activated...
	Input		:	Nothing
	Output		:	BOOL yes/no
===================================================================*/
BOOL IsAvatarActive( void )
{
	TRIGGERVAR	* TVpnt;

	TVpnt = AvatarActivated;

	if( TVpnt )
	{
		if( TVpnt->State )
		{
			// Avatar is Active...
			return TRUE;
		}
	}

	return FALSE;
}

/*===================================================================
	Procedure	:	Save Trigger arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveAllTriggers( FILE * fp )
{
	int				i;
	TRIGGERVAR	*	TrigVar;
	TRIGGER		*	Trig;
	TRIGGERMODQUE *	TrigQue;
	int32			Index;

	if( fp )
	{
		fwrite( &NumOfTrigVars, sizeof( int ), 1, fp );

		TrigVar = TrigVars;

		for( i = 0; i < NumOfTrigVars; i++ )
		{
			fwrite( &TrigVar->State, sizeof( int ), 1, fp );
			TrigVar++;
		}

		fwrite( &NumOfTriggers, sizeof( int ), 1, fp );

		Trig = Triggers;

		for( i = 0; i < NumOfTriggers; i++ )
		{
			fwrite( &Trig->Active, sizeof( int ), 1, fp );
			Trig++;
		}

		fwrite( &NumOfTrigModQues, sizeof( int ), 1, fp );

		TrigQue = TrigModQue;

		for( i = 0; i < NumOfTrigModQues; i++ )
		{
			fwrite( &TrigQue->Time, sizeof( float ), 1, fp );
			Index = (int32) ( TrigQue->TriggerMod - TrigMods );
			fwrite( &Index, sizeof( int32 ), 1, fp );
			TrigQue++;
		}

	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load Trigger Arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadAllTriggers( FILE * fp )
{
	int				i;
	TRIGGERVAR	*	TrigVar;
	TRIGGER		*	Trig;
	TRIGGERMODQUE *	TrigQue;
	int32			Index;
	int				NumTrigVars;
	int				NumTrigs;
	int				NumTrigMods;

	if( fp )
	{
/*===================================================================
		TrigVars
===================================================================*/
		fread( &NumTrigVars, sizeof( int ), 1, fp );

		if( NumOfTrigVars != NumTrigVars )
		{
			fclose( fp );
			return( NULL );
		}
		TrigVar = TrigVars;

		for( i = 0; i < NumOfTrigVars; i++ )
		{
			fread( &TrigVar->State, sizeof( int ), 1, fp );
			TrigVar++;
		}

/*===================================================================
		Triggers
===================================================================*/
		fread( &NumTrigs, sizeof( int ), 1, fp );

		if( NumOfTriggers != NumTrigs )
		{
			fclose( fp );
			return( NULL );
		}

		Trig = Triggers;

		for( i = 0; i < NumOfTriggers; i++ )
		{
			fread( &Trig->Active, sizeof( int ), 1, fp );
			Trig++;
		}

/*===================================================================
		TrigMods ( Qued )
===================================================================*/
		fread( &NumTrigMods, sizeof( int ), 1, fp );

		if( NumOfTrigModQues != NumTrigMods )
		{
			fclose( fp );
			return( NULL );
		}

		TrigQue = TrigModQue;

		for( i = 0; i < NumOfTrigModQues; i++ )
		{
			fread( &TrigQue->Time, sizeof( float ), 1, fp );
			fread( &Index, sizeof( int32 ), 1, fp );
			TrigQue->TriggerMod = ( TrigMods + Index );
			TrigQue++;
		}

	}

	return( fp );
}
