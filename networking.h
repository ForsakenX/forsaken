#ifndef NETWORKING_INCLUDED
#define NETWORKING_INCLUDED

// TODO in 1.19 we should change this to a byte and merge multiple bools into bit fields
typedef int net_bool_t; // backwards compatibility need bool's in pkt structures to be int's

//
// General Networking
//

#include "main.h"
#include "net.h"
#include "new3d.h"
#include "object.h"

// game tracker
char tracker_server[256];
int tracker_port;
bool tracker_enabled;

// my ship number
#define	UNASSIGNED_SHIP 0xff // WhoIAm before host gives me number
extern BYTE	WhoIAm;  // which ship number i am
extern network_player_t * host_network_player; // the host .. NULL if I am host

// don't change these directly...
// instead use:
//   local_port_str.text = "2300"
//   host_port_str.text = "2300"
//   TCPAddress.text = "192.168.0.30"
// the values are parsed before calling network_join and network_setup
int local_port;
int host_port;
char host_address[256];

 /*
 * defines
 */

#define MULTIPLAYER_VERSION				(0x010d)
#define DEMO_MULTIPLAYER_VERSION		(0x0109)
#define SHORTBANK
#define VERYSHORTPACKETS
#define NOISEFALLOFFFACTOR				( 1.0F / 120.0F )	// approx 2 seconds....
#define NORMAL_MODE							0
#define DEATH_MODE							1
#define LIMBO_MODE							2			// this is what the ship is in while viewing its own death....but has blown up...
#define DEMO_MODE								3			// this is the mode the camera goes into when Playing back a Demo
#define GAMEOVER_MODE						4			// this is the mode the player goes into when single player has finished...
#define WATCH_MODE							5
#define MAX_PLAYERS							16			//24 //16 //12
#define MAX_SHIELD								255.0F
#define START_SHIELD							128.0F
#define MAX_HULL								255.0F
#define START_HULL								128.0F
#define MAXTEXTMSG							512		// the number of chars in a message you can send.....
#define MAXGENPICKUPCOUNT				5
#define MAXGENREGENSLOTCOUNT			6
#define MAXGENMINECOUNT					3 //4
#define MAXGENTRIGGERCOUNT				60
#define MAXGENTRIGVARCOUNT				60
#define MAXMULTIPLES							8
#define MAX_PICKUPFLAGS					2
#define MAX_BUFFER_SIZE					1024
#define FRAMELAGED_RECOIL					true
#define ONEOFF_RECOIL						false
#define MAX_TEAMS								4
#define NUMTITANBITS							10 

bool PlayerReady[MAX_PLAYERS];
extern BYTE GameStatus[];

/*
 * structures
 */
/*===================================================================
	Bodge Structures
===================================================================*/
typedef struct SHORTMINE{

	u_int16_t	OwnerType;
	u_int16_t	Owner;
	u_int16_t	BulletID;
	u_int16_t	Group;
	VECTOR	Pos;
	VECTOR	Offset;
	VECTOR	Dir;
	VECTOR	UpVector;
	VECTOR	DropDir;
	u_int16_t	Weapon;
	float	LifeCount;

}SHORTMINE;

/*===================================================================
	Bodge Structures
===================================================================*/
typedef struct SHORTKILLMINE{
	u_int16_t	OwnerType;			// Owner ID
	u_int16_t	Owner;				// Owner ID
	u_int16_t	ID;					// ID
	float	ShockwaveSize;		// Size
}SHORTKILLMINE;
/*===================================================================
	Bodge Structures
===================================================================*/

typedef struct MISSEDMINEMSG{
 	int16_t	Type;
	void *	Struct;
}MISSEDMINEMSG;

/*===================================================================
	Bodge Structures
===================================================================*/
typedef struct SHORTPICKUP{
 	int16_t	Type;
	int16_t	Group;
	u_int16_t	Owner;				// Owner ID
	u_int16_t	ID;					// ID
	VECTOR	Pos;
	VECTOR	Dir;
	int16_t	RegenSlot;
	float	Speed;
	net_bool_t	Sparkle;
	float	LifeCount;
	u_int16_t	TriggerMod;
}SHORTPICKUP;

typedef struct SHORTTRIGVAR{
	int		State;
}SHORTTRIGVAR;

typedef struct SHORTTRIGGER{
	int		Active;
}SHORTTRIGGER;

/*===================================================================
	Bodge Structures
===================================================================*/
typedef struct SHORTREGENSLOT{

	u_int16_t	GenType;			// Generation Type
	u_int16_t	RegenType;			// Regeneration Type
	float	GenDelay;			// Generation Delay ( 60hz )
	float	Lifespan;			// Lifespan of pickup ( 60hz )
	float	Wait;				// Global count
	int16_t	Status;				// Status of slot
	int16_t	Group;				// Group to generate pickup
	VECTOR	Pos;				// Position to generate pickup
	u_int16_t	Type;				// Type of pickup
	u_int16_t	TriggerMod;			// Trigger Mod Index
	u_int16_t	PickupIndex;		// Pickup Index
	u_int16_t	PickupID;			// Pickup ID

}SHORTREGENSLOT;

/*===================================================================
	Bodge Structures
===================================================================*/
typedef struct SHORTKILLPICKUP{
	u_int16_t	Owner;				// Owner ID
	u_int16_t	ID;					// ID
	int16_t	Style;				// Style
}SHORTKILLPICKUP;

/*===================================================================
	Bodge Structures
===================================================================*/

typedef struct _PLAYERINFO
{
	char	IP[16];
	char	Name[8];
}PLAYERINFO;

/*===================================================================
	Bodge Structures
===================================================================*/

typedef struct MISSEDPICKUPMSG{
 	int16_t	Type;
	void *	Struct;
}MISSEDPICKUPMSG;

#define MAXSHORTNAME 8
typedef	char		SHORTNAMETYPE[MAX_PLAYERS][MAXSHORTNAME];

typedef struct _SHIPDIEDINFO
{
	BYTE		WeaponType;
	BYTE		Weapon;

} SHIPDIEDINFO;

typedef struct _PICKUPINFO
{
	u_int16_t		IDCount;
	int16_t		Type;
	u_int16_t		Group;
	VECTOR		Pos;
	VECTOR		Dir;
	float		Speed;
	int16_t		RegenSlot;
	net_bool_t		Sparkle;
	float		LifeCount;
	u_int16_t		TriggerMod;
}PICKUPINFO; // 46

typedef struct _VERYSHORTPICKUPINFO
{
	u_int16_t		IDCount;
	BYTE		Type;
	BYTE		Group;
	BYTE		RegenSlot;
	BYTE		Sparkle;
	SHORTVECTOR	Pos;
	SHORTVECTOR	Dir;
	float		Speed;
	float		LifeCount;
	u_int16_t		TriggerMod;
}VERYSHORTPICKUPINFO; // 28



typedef struct _KILLPICKUPINFO
{
	u_int16_t		Owner;
	u_int16_t		IDCount;
	int16_t		Style;
}KILLPICKUPINFO;


typedef struct _EXPSECONDARYINFO
{
	u_int16_t		OwnerType;
	u_int16_t		Owner;
	u_int16_t		IDCount;
	float		ShockwaveSize;
	VECTOR		Pos;
	u_int16_t		Group;

}EXPSECONDARYINFO;

typedef struct _TEAMGOALSINFO
{
	u_int16_t		TeamGoals[ MAX_TEAMS ];

}TEAMGOALSINFO;

typedef struct _SHOCKWAVEINFO
{
	u_int16_t		Owner;
	u_int16_t		Group;
	VECTOR		Pos;
	BYTE		Weapon;
	float		ShockwaveSize;
}SHOCKWAVEINFO;

typedef struct _BGOUPDATEINFO
{
	u_int16_t		BGObject;
	int16_t		State;
	float		Time;
}BGOUPDATEINFO;

typedef struct _PRIMBULLPOSDIR
{
	u_int16_t		OwnerType;
	u_int16_t		OwnerID;
	u_int16_t		BulletID;
	int8_t		Weapon;
	u_int16_t		Group;
	VECTOR		Pos;
	VECTOR		Offset;
	VECTOR		Dir;
	VECTOR		Up;
	int16_t		PowerLevel;
	float		PLevel;

}PRIMBULLPOSDIR;

typedef struct _SECBULLPOSDIR
{
	u_int16_t		OwnerType;
	u_int16_t		Owner;
	u_int16_t		BulletID;
	u_int16_t		Group;
	VECTOR		Pos;
	VECTOR		Offset;
	VECTOR		Dir;
	VECTOR		UpVector;
	VECTOR		DropDir;
	int8_t		Weapon;

}SECBULLPOSDIR;

typedef struct _TITANBITS
{
	u_int16_t		OwnerType;
	u_int16_t		Owner;
	u_int16_t		BulletID;
	u_int16_t		Group;
	VECTOR		Pos;
	VECTOR		Offset;
	VECTOR		UpVector;
	VECTOR		DropDir;
	VECTOR		Directions[ NUMTITANBITS ];
	int8_t		Weapon;

}TITANBITS;

typedef struct _SHIPHIT
{
	float		Damage;
	VECTOR		Recoil;
	VECTOR		Point;
	VECTOR		Dir;
	BYTE		WeaponType;
	BYTE		Weapon;
	float		Force;
	net_bool_t		OneOffExternalForce;

}SHIPHIT;


typedef struct _SHORTSHIPHIT
{
	float		Damage;
	float		Force;
	SHORTVECTOR	Recoil;
	SHORTVECTOR	Point;
	SHORTVECTOR	Dir;
	BYTE		WeaponType;
	BYTE		Weapon;
	u_int16_t		OneOffExternalForce;
	u_int16_t		Recoil_Scalar;
}SHORTSHIPHIT;



typedef struct _SETTIME
{
	float		Time;

}SETTIME;

typedef struct _GLOBALSHIP
{
	OBJECT				Object;						//
	
	BYTE				enable;						// is this ship active?
	BYTE				ShipThatLastKilledMe;		// the ship who killed me last...
	BYTE				ShipThatLastHitMe;			// the ship who killed me last...
	BYTE				NumMultiples;				// Number of multiples
	float				StealthTime;				// Stealth Mode Time Limit
	float				Timer;						// General Purpose Mode Timer....How Long to stay dead ...etc..
	float				InvulTimer;					// HowLong am I Invulnerable
	net_bool_t				Invul;						// Am I Invulnerable...
	VECTOR				LastAngle;					// what my last Step Turn Angles Were...
	u_int16_t				PrimBullIdCount;			// Id count for every bullet I fire....
	u_int16_t				SecBullIdCount;				// Id count for every bullet I fire....
	u_int16_t				PickupIdCount;				// Id count for every pickup I generate.....
	float				Damage;						// how much Damege I Have Taken...
	BYTE				Primary;					// which weapon is my primary....
	BYTE				Secondary;					// which wepaon is my secondary....
	u_int16_t				ModelNum;					// which model to display for them...
	int16_t				BikeNum;					// which model to display for them...
	BYTE				Pickups;
	BYTE				RegenSlots;
	BYTE				Triggers;
	BYTE				TrigVars;
	BYTE				Mines;
	net_bool_t				JustRecievedPacket;			//
	VECTOR				LastMove;					// last movement vector (framelagged)
	VECTOR				Move_Off;					// Last MoveMent...x , y , z
	network_player_t *  network_player;
	u_int16_t				OrbModels[ MAXMULTIPLES ];	// Orbit Pulsar Model
	float				OrbAmmo[ MAXMULTIPLES ];	// Orbit Pulsar Ammo

	float				PrimPowerLevel;				// Trojax Powerlevel
	u_int16_t				PrimID;						// Bullet ID
	u_int16_t				SecID;						// Missile ID
	BYTE				SecWeapon;					// Weapon
	net_bool_t	headlights; // are my headlights on?

	net_bool_t				DemoInterpolate;

	VECTOR		OldPos;								//
	VECTOR		NextPos;							// what my next position will be
	QUAT		OldQuat;							//
	QUAT		NextQuat;							// what my next view angle will be
	float		OldBank;							//
	float		NextBank;							// what my next bank angle will be..

#ifdef DEMO_SUPPORT
	LONGLONG	OldTime;							// how long before I am at those positions
	LONGLONG	NextTime;							// how long before I am at those positions
#endif

	float		SuperNashramTimer;					// HowLong have i left with the super nashram?
	u_int16_t		TempLines[ 12 ];

	float		ShakeTimer;
	float		ShakeDirTimer;
	float		ShakeForce;

	float		PacketDelay;

	u_int8_t		ShieldHullCount;

	VECTOR		RealPos;
	u_int16_t		RealGroup;

} GLOBALSHIP, *LPGLOBALSHIP;



/*		this is all that really needs to be sent	*/
typedef struct _SHORTGLOBALSHIP
{
	u_int32_t				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Status;			// tells us constantly what this ship is doing...
	BYTE				GroupImIn;		// Group Im In

	VECTOR				Pos;			// x , y , z position
	VECTOR				Move_Off;		// Last MoveMent...x , y , z
	VECTOR				Angle;			// Last Rotation..
#ifdef	SHORTBANK
	int16_t				Bank;			// How much Am I banked....
#else
	float				Bank;			// How much Am I banked....
#endif
	QUAT				Quat;			// Final Quat...
} SHORTGLOBALSHIP, *LPSHORTGLOBALSHIP;

typedef struct _VERYSHORTGLOBALSHIP
{
	u_int32_t				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Status;			// tells us constantly what this ship is doing...
	BYTE				GroupImIn;		// Group Im In
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTVECTOR			Move_Off;		// Last MoveMent...x , y , z
	SHORTVECTOR			Angle;			// Last Rotation..
	int16_t				Bank;			// How much Am I banked....
	u_int16_t				Move_Off_Scalar;
	SHORTQUAT			Quat;			// Final Quat...
} VERYSHORTGLOBALSHIP, *LPVERYSHORTGLOBALSHIP;


typedef struct _FSHORTGLOBALSHIP
{
	u_int32_t				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Primary;		// Primary weapon
	BYTE				Secondary;		// Secondary weapon
	BYTE				GroupImIn;		// Group Im In
	BYTE				PrimPowerLevel;	// Trojax Powerlevel
	VECTOR				Pos;			// x , y , z position
	VECTOR				Move_Off;		// Last MoveMent...x , y , z
	VECTOR				Angle;			// Last Rotation..
#ifdef	SHORTBANK
	int16_t				Bank;		// How much Am I banked....
#else
	float				Bank;		// How much Am I banked....
#endif
	QUAT				Quat;		// Final Quat...
} FSHORTGLOBALSHIP, *LPFSHORTGLOBALSHIP;

typedef struct _FVERYSHORTGLOBALSHIP
{
	u_int32_t				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Primary;		// Primary weapon
	BYTE				Secondary;		// Secondary weapon
	BYTE				GroupImIn;		// Group Im In
	BYTE				PrimPowerLevel;	// Trojax Powerlevel
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTVECTOR			Move_Off;		// Last MoveMent...x , y , z
	SHORTVECTOR			Angle;			// Last Rotation..
	int16_t				Bank;		// How much Am I banked....
	u_int16_t				Move_Off_Scalar;
	SHORTQUAT			Quat;		// Final Quat...
} FVERYSHORTGLOBALSHIP, *LPFVERYSHORTGLOBALSHIP;

typedef struct _GROUPONLY_FVERYSHORTGLOBALSHIP
{
	u_int32_t				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Primary;		// Primary weapon
	BYTE				Secondary;		// Secondary weapon
	BYTE				GroupImIn;		// Group Im In
	BYTE				PrimPowerLevel;	// Trojax Powerlevel
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTQUAT			Quat;		// Final Quat...
	int16_t				Bank;		// How much Am I banked....
} GROUPONLY_FVERYSHORTGLOBALSHIP, *LPGROUPONLY_FVERYSHORTGLOBALSHIP;

//----------------------------------------------------------
// Mask Defines for .Flags u_int32_t
//----------------------------------------------------------

#define	SHIP_Light_Bit			30
#define	SHIP_CarryingFlag4_Bit	29
#define	SHIP_CarryingFlag3_Bit	28
#define	SHIP_CarryingFlag2_Bit	27
#define	SHIP_CarryingFlag1_Bit	26
#define	SHIP_CarryingFlag_Bit	26
#define	SHIP_CarryingBounty_Bit	26
#define	SHIP_Shield_Bit			25
#define	SHIP_Scattered_Bit		24
#define	SHIP_Litup_Bit			23
#define	SHIP_InWater_Bit		22
#define	SHIP_SuperNashram_Bit	21
#define	SHIP_Invul_Bit			20
#define	SHIP_MulFire_Bit		19
#define	SHIP_Charging_Bit		18
#define	SHIP_Turbo_Bit			17
#define	SHIP_Stealth_Bit		16
#define	SHIP_PrimFire_Bit		15
#define	SHIP_PrimToggle_Bit		14
#define	SHIP_SecFire_Bit		13
#define	SHIP_SecToggle_Bit		12
#define	SHIP_IsHost_Bit			11
#define	SHIP_Enable_Bit			10
#define	SHIP_Hull_Bit2			9
#define	SHIP_Hull_Bit1			8
#define	SHIP_Mode_Bit2			7
#define	SHIP_Mode_Bit1			6
#define	SHIP_PowerLevel_Bit2	5
#define	SHIP_PowerLevel_Bit1	4
#define	SHIP_NumMultiples_Bit4	3
#define	SHIP_NumMultiples_Bit3	2
#define	SHIP_NumMultiples_Bit2	1
#define	SHIP_NumMultiples_Bit1	0

#define	SHIP_Light			( 1 << SHIP_Light_Bit )
#define	SHIP_CarryingFlag4	( 1 << SHIP_CarryingFlag4_Bit )
#define	SHIP_CarryingFlag3	( 1 << SHIP_CarryingFlag3_Bit )
#define	SHIP_CarryingFlag2	( 1 << SHIP_CarryingFlag2_Bit )
#define	SHIP_CarryingFlag1	( 1 << SHIP_CarryingFlag1_Bit )
#define	SHIP_CarryingFlag	( 1 << SHIP_CarryingFlag_Bit )
#define	SHIP_CarryingFlags	( SHIP_CarryingFlag1 \
							| SHIP_CarryingFlag2 \
							| SHIP_CarryingFlag3 \
							| SHIP_CarryingFlag4 )
#define	SHIP_NumMultiplesBit4	( 1 << SHIP_NumMultiples_Bit4 )
#define	SHIP_NumMultiplesBit3	( 1 << SHIP_NumMultiples_Bit3 )
#define	SHIP_NumMultiplesBit2	( 1 << SHIP_NumMultiples_Bit2 )
#define	SHIP_NumMultiplesBit1	( 1 << SHIP_NumMultiples_Bit1 )
#define	SHIP_Shield			( 1 << SHIP_Shield_Bit )
#define	SHIP_HullBit2		( 1 << SHIP_Hull_Bit2 )
#define	SHIP_HullBit1		( 1 << SHIP_Hull_Bit1 )
#define	SHIP_ModeBit2		( 1 << SHIP_Mode_Bit2 )
#define	SHIP_ModeBit1		( 1 << SHIP_Mode_Bit1 )
#define	SHIP_PowerLevelBit2	( 1 << SHIP_PowerLevel_Bit2 )
#define	SHIP_PowerLevelBit1	( 1 << SHIP_PowerLevel_Bit1 )
#define	SHIP_CarryingBounty	( 1 << SHIP_CarryingBounty_Bit )
#define	SHIP_Scattered		( 1 << SHIP_Scattered_Bit)
#define	SHIP_Litup			( 1 << SHIP_Litup_Bit )
#define	SHIP_InWater		( 1 << SHIP_InWater_Bit )
#define	SHIP_SuperNashram	( 1 << SHIP_SuperNashram_Bit )
#define	SHIP_Invul			( 1 << SHIP_Invul_Bit )
#define	SHIP_MulFire		( 1 << SHIP_MulFire_Bit )
#define	SHIP_Multiple		( ( 1 << SHIP_NumMultiples_Bit4 ) \
							| ( 1 << SHIP_NumMultiples_Bit3 ) \
							| ( 1 << SHIP_NumMultiples_Bit2 ) \
							| ( 1 << SHIP_NumMultiples_Bit1 ) )
#define	SHIP_Charging		( 1 << SHIP_Charging_Bit )
#define	SHIP_Turbo			( 1 << SHIP_Turbo_Bit )
#define	SHIP_Stealth		( 1 << SHIP_Stealth_Bit )
#define	SHIP_PrimFire		( 1 << SHIP_PrimFire_Bit )
#define	SHIP_PrimToggle		( 1 << SHIP_PrimToggle_Bit )
#define	SHIP_SecFire		( 1 << SHIP_SecFire_Bit )
#define	SHIP_SecToggle		( 1 << SHIP_SecToggle_Bit )
#define	SHIP_IsHost			( 1 << SHIP_IsHost_Bit )
#define	SHIP_Enable			( 1 << SHIP_Enable_Bit )

//----------------------------------------------------------
// communication packet structures
//----------------------------------------------------------
#define MSG_UPDATE							0x11
#define MSG_HEREIAM							0x22
#define MSG_INIT								0x33
#define MSG_SHIPHIT							0x44
#define MSG_PRIMBULLPOSDIR				0x55
#define MSG_SECBULLPOSDIR					0x66
#define MSG_SHIPDIED							0x77
#define MSG_DROPPICKUP						0x88
#define MSG_KILLPICKUP						0x99
#define MSG_NETSETTINGS					0xaa
#define MSG_STATUS							0xbb
#define MSG_SHORTPICKUP					0xee
#define MSG_SHOCKWAVE						0xff
#define MSG_FUPDATE							0x1f
#define MSG_SHORTMINE						0x2f
#define MSG_TEXTMSG							0x3f
#define MSG_SHORTREGENSLOT				0x4f
#define MSG_SHORTTRIGGER					0x5f
#define MSG_SHORTTRIGVAR					0x6f
#define MSG_INTERPOLATE					0x8f
#define MSG_BGOUPDATE						0xaf
#define MSG_LONGSTATUS					0xdf
#define MSG_SETTIME							0xef
#define MSG_REQTIME							0xf1
#define MSG_BIKENUM							0xf4
#define MSG_VERYSHORTUPDATE			0xf5
#define MSG_VERYSHORTFUPDATE			0xf6
#define MSG_VERYSHORTINTERPOLATE	0xf8
#define MSG_TEAMGOALS						0xf9
#define MSG_YOUQUIT							0xfa
#define MSG_SHORTSHIPHIT					0xfc
#define MSG_TITANBITS						0xfd
#define MSG_GROUPONLY_VERYSHORTFUPDATE		0xec
#define MSG_VERYSHORTDROPPICKUP		0xed
#define MSG_SHIPHEALTH              0xcc

typedef struct _SENDBIKENUMMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
}SENDBIKENUMMSG, *LPSENDBIKENUMMSG;


typedef struct _YOUQUITMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		You;
}YOUQUITMSG, *LPYOUQUITMSG;

typedef struct _BIKENUMMSG
{
    BYTE     MsgCode;
    BYTE     WhoIAm;
	BYTE		BikeNum;		// which model to display for them...
}BIKENUMMSG, *LPBIKENUMMSG;

typedef struct _SHORTPICKUPMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		HowManyPickups;
	BYTE		Pickups;
    struct SHORTPICKUP  ShortPickup[MAXGENPICKUPCOUNT];
} SHORTPICKUPMSG, *LPSHORTPICKUPMSG;

typedef struct _SHORTREGENSLOTMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		HowManyRegenSlots;
	BYTE		RegenSlots;
    struct SHORTREGENSLOT  ShortRegenSlot[MAXGENREGENSLOTCOUNT];
} SHORTREGENSLOTMSG, *LPSHORTREGENSLOTMSG;

typedef struct _SHORTTRIGGERMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		HowManyTriggers;
	BYTE		Triggers;
    struct SHORTTRIGGER  ShortTrigger[MAXGENTRIGGERCOUNT];
} SHORTTRIGGERMSG, *LPSHORTTRIGGERMSG;

typedef struct _SHORTTRIGVARMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		HowManyTrigVars;
	BYTE		TrigVars;
    struct SHORTTRIGVAR  ShortTrigVar[MAXGENTRIGVARCOUNT];
} SHORTTRIGVARMSG, *LPSHORTTRIGVARMSG;

typedef struct _SHORTMINEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BYTE		HowManyMines;
	BYTE		Mines;
    struct SHORTMINE  ShortMine[MAXGENMINECOUNT];
} SHORTMINEMSG, *LPSHORTMINEMSG;

typedef struct _UPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    SHORTGLOBALSHIP  ShortGlobalShip;
} UPDATEMSG, *LPUPDATEMSG;

typedef struct _VERYSHORTUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    VERYSHORTGLOBALSHIP  ShortGlobalShip;
} VERYSHORTUPDATEMSG, *LPVERYSHORTUPDATEMSG;

typedef struct _FUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    FSHORTGLOBALSHIP  ShortGlobalShip;
} FUPDATEMSG, *LPFUPDATEMSG;

typedef struct _VERYSHORTFUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    FVERYSHORTGLOBALSHIP  ShortGlobalShip;
} VERYSHORTFUPDATEMSG, *LPVERYSHORTFUPDATEMSG;

typedef struct _GROUPONLY_VERYSHORTFUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    GROUPONLY_FVERYSHORTGLOBALSHIP  ShortGlobalShip;
} GROUPONLY_VERYSHORTFUPDATEMSG, *LPGROUPONLY_VERYSHORTFUPDATEMSG;


typedef struct _HEREIAMMSG
{
    BYTE		MsgCode;
    BYTE		WhoIAm;
	BYTE		MPVersion;
} HEREIAMMSG, *LPHEREIAMMSG;

// later we should probably shove these all into a bit stream
// all the code is hidden away in the repo in the session description functions (search: dwUser)
// for now this is much simplar and this message is only sent once to new players
// converting all the bool's to BYTE's (even with all the new added fields) saved 72 bits...

typedef struct _INITMSG
{
    BYTE	MsgCode;

	// boolEANS

	BYTE	PlayerReady[MAX_PLAYERS];
	BYTE	RandomPickups;
	BYTE	TeamGame;
	BYTE	CaptureTheFlag;
	BYTE	CTF;
	BYTE	BountyHunt;
	BYTE	ResetKillsPerLevel;
	BYTE	UseShortPackets;

	// others

    BYTE	WhoIAm;
    BYTE	YouAre;
	BYTE	OverallGameStatus;
	BYTE	NumPrimaryPickups;
	BYTE	MaxKills;
	BYTE	TeamNumber[MAX_PLAYERS];
	BYTE	GameStatus[MAX_PLAYERS];
	BYTE	ColPerspective;
	BYTE	CTF_Type;
	BYTE	TimeLimit;
	BYTE	KillStats[MAX_PLAYERS][MAX_PLAYERS];
	BYTE	BonusStats[MAX_PLAYERS];
	BYTE	KillCounter[MAX_PLAYERS];

	u_int16_t	RandomStartPosModify;
	u_int16_t	Seed1;
	u_int16_t	Seed2;

	int16_t	GoalScore;
	int16_t	BountyBonusInterval;

	u_int32_t	PickupFlags[MAX_PICKUPFLAGS];

	float	NetUpdateInterval;

	char	LevelName[32];


} INITMSG, *LPINITMSG;

typedef struct _SHIPHITMSG
{
    BYTE     MsgCode;
	BYTE		WhoHitYou;
    BYTE     You;
	SHIPHIT	ShipHit;
} SHIPHITMSG, *LPSHIPHITMSG;

typedef struct _SHORTSHIPHITMSG
{
    BYTE		MsgCode;
	BYTE		WhoHitYou;
    BYTE		You;
	SHORTSHIPHIT	ShipHit;
} SHORTSHIPHITMSG, *LPSHORTSHIPHITMSG;


typedef struct _SHIPDIEDMSG
{
    BYTE     MsgCode;
    BYTE     WhoIAm;
    BYTE     WhoKilledMe;
	BYTE		Type;
	BYTE		WeaponType;
	BYTE		Weapon;
	VECTOR	Pos;
} SHIPDIEDMSG, *LPSHIPDIEDMSG;


typedef struct _KILLPICKUPMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	KILLPICKUPINFO	KillPickupInfo;

} KILLPICKUPMSG, *LPKILLPICKUPMSG;

typedef struct _TEAMGOALSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	TEAMGOALSINFO	TeamGoalsInfo;

} TEAMGOALSMSG, *LPTEAMGOALSMSG;

typedef struct _SHOCKWAVEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	SHOCKWAVEINFO	ShockwaveInfo;

} SHOCKWAVEMSG, *LPSHOCKWAVEMSG;

typedef struct _BGOUPDATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	BGOUPDATEINFO	BGOUpdateInfo;

} BGOUPDATEMSG, *LPBGOUPDATEMSG;

typedef struct _DROPPICKUPMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	PICKUPINFO	PickupInfo;

} DROPPICKUPMSG, *LPDROPPICKUPMSG;

typedef struct _VERYSHORTDROPPICKUPMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	VERYSHORTPICKUPINFO	PickupInfo;

} VERYSHORTDROPPICKUPMSG, *LPVERYSHORTDROPPICKUPMSG;

typedef struct _PRIMBULLPOSDIRMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	PRIMBULLPOSDIR	PrimBullPosDir;
} PRIMBULLPOSDIRMSG, *LPPRIMBULLPOSDIRMSG;

typedef struct _SECBULLPOSDIRMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	SECBULLPOSDIR	SecBullPosDir;
} SECBULLPOSDIRMSG, *LPSECBULLPOSDIRMSG;

typedef struct _TITANBITSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	TITANBITS	TitanBits;
} TITANBITSMSG, *LPTITANBITSMSG;

typedef	int16_t		SHORTSTATSTYPE[MAX_PLAYERS];

typedef struct _SHORTSTATSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
    BYTE        WhosStats;
	SHORTSTATSTYPE		ShortStats;
} SHORTSTATSMSG, *LPSHORTSTATSMSG;

typedef struct _STATUSMSG
{
    BYTE     MsgCode;
    BYTE     WhoIAm;
	net_bool_t		IsHost;			// from host ???
	BYTE		Status;
	BYTE		TeamNumber;
	net_bool_t		IAmReady;		// used for team game - game cannot start until everyone is ready
	BYTE		Pickups;			// tells how much of the pickup list I have recieved..
	BYTE		RegenSlots;		// tells how much of the pickup regen slots list I have recieved..
	BYTE		Mines;			// tells how much of the mine list I have recieved..
	BYTE		Triggers;			// tells how much of the mine list I have recieved..
	BYTE		TrigVars;		// tells how much of the mine list I have recieved..
} STATUSMSG, *LPSTATUSMSG;

typedef struct _NETSETTINGSMSG
{
	BYTE		MsgCode;
	BYTE		WhoIAm;
	net_bool_t		IsHost;
	float		PacketsPerSecond;
	net_bool_t		CollisionPerspective;
	net_bool_t		ShortPackets;	
} NETSETTINGSMSG, *LPNETSETTINGSMSG;

typedef struct _LONGSTATUSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	STATUSMSG	Status;
	char		LevelName[32];
	u_int16_t		RandomStartPosModify;
} LONGSTATUSMSG, *LPLONGSTATUSMSG;

enum{
	TEXTMSGTYPE_Taunt1,
	TEXTMSGTYPE_Taunt2,
	TEXTMSGTYPE_Taunt3,
	TEXTMSGTYPE_TitleMessage,
	TEXTMSGTYPE_JoiningTeamGame,
	TEXTMSGTYPE_EnteredWatchMode,
	TEXTMSGTYPE_ExitedWatchMode,
	TEXTMSGTYPE_QuickTaunt,
	TEXTMSGTYPE_CaptureFlagMessage,
	TEXTMSGTYPE_ScoredWithFlag,
	TEXTMSGTYPE_ReturningFlag,
	TEXTMSGTYPE_ReturnedFlag,
	TEXTMSGTYPE_FlagDriftedIn,
	TEXTMSGTYPE_FlagEscaped,
	TEXTMSGTYPE_BountyMessage,
	TEXTMSGTYPE_QuickTauntWhisper,
	TEXTMSGTYPE_SpeechTaunt,
};

typedef struct _TEXTMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	char		Text[MAXTEXTMSG];
	BYTE		TextMsgType;
} TEXTMSG, *LPTEXTMSG;

#ifdef DEMO_SUPPORT
typedef struct _INTERPOLATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	VECTOR		NextPos;			// what my next position will be
	QUAT		NextQuat;			// what my next view angle will be
	float		NextBank;			// what my next bank angle will be..
	LONGLONG	NextTime;			// how long before I am at thos positions
} INTERPOLATEMSG, *LPINTERPOLATEMSG;

typedef struct _VERYSHORTINTERPOLATEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	SHORTVECTOR	NextPos;			// what my next position will be
	SHORTQUAT	NextQuat;			// what my next view angle will be
	int16_t		NextBank;			// what my next bank angle will be..
	LONGLONG	NextTime;			// how long before I am at thos positions
} VERYSHORTINTERPOLATEMSG, *LPVERYSHORTINTERPOLATEMSG;
#endif

typedef struct _SETTIMEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	SETTIME		TimeInfo;

} SETTIMEMSG, *LPSETTIMEMSG;


typedef struct _REQTIMEMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;

} REQTIMEMSG, *LPREQTIMEMSG;

typedef struct _SHIPHEALTHMSG
{
    BYTE MsgCode;
    BYTE WhoIAm;
    u_int8_t Hull;
    u_int8_t Shield;
} SHIPHEALTHMSG, *LPSHIPHEALTHMSG;

#define MAXLEVELSPERBATCH 8

// expose Names to all that care
extern SHORTNAMETYPE Names;

extern GLOBALSHIP Ships[MAX_PLAYERS+1];
extern BYTE Current_Camera_View; // which object is currently using the camera view....
#define INVALID_CAMERA_VIEW MAX_PLAYERS+2
#define CAMERA_VIEW_IS_VALID (Current_Camera_View >= 0 && Current_Camera_View < MAX_PLAYERS+1)

/*
 * fn prototypes
 */
void	DestroyGame( void );
void	SendGameMessage( BYTE msg, network_player_t * to, BYTE row, BYTE col, BYTE mask );
void	EvaluateMessage( network_player_t * from, DWORD len , BYTE * MsgPnt );
void	ReceiveGameMessages( void );
void	initShip( u_int16_t i );
void	NetworkGameUpdate();
void	SetupNetworkGame();
void	PrimBullPosDirSend( u_int16_t OwnerType, u_int16_t OwnerID, u_int16_t BulletID, int8_t Weapon,
						u_int16_t Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16_t PowerLevel, float PLevel );
void SecBullPosDirSend( u_int16_t OwnerType, u_int16_t Owner, u_int16_t BulletID, u_int16_t Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * UpVector,
						VECTOR * DropDir, int8_t Weapon );
void TitanBitsSend( u_int16_t OwnerType, u_int16_t Owner, u_int16_t BulletID, u_int16_t Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * UpVector,
						VECTOR * DropDir, int8_t Weapon, VECTOR * Directions );
void	DropPickupSend( VECTOR * Pos, u_int16_t Group, VECTOR * Dir, float Speed, int16_t Type, u_int16_t IDCount, int16_t RegenSlot, bool Sparkle, float LifeCount, u_int16_t TriggerMod);
void	KillPickupSend( u_int16_t Owner, u_int16_t IDCount, int16_t Style );
void	CreateShockwaveSend( u_int16_t OwnerType, u_int16_t Owner, VECTOR * Pos, u_int16_t Group, float ShockwaveSize, BYTE Weapon );
void	IHitYou( BYTE you, float Damage, VECTOR * Recoil, VECTOR * Point, VECTOR * Dir, float Force, BYTE WeaponType, BYTE Weapon, bool FramelagRecoil );
void	ShipDiedSend( BYTE WeaponType, BYTE Weapon );
void	UpdateBGObjectSend( u_int16_t BGObject, int16_t State, float Time );
void	smallinitShip( u_int16_t i );
void DemoPlayingNetworkGameUpdate(void);
void DemoClean( void );
int FindSameLevel( char * Name );
void	RequestTime( void  );
void	SetTime( float Time );
void Demo_fwrite( const void *buffer, size_t size, size_t count , FILE *stream );
void StopDemoRecording( void );
bool UpdateAmmoAndValidateMessage( void * Message );
bool AutoJoinSession( void );
void AllocatePseudoHost( void );

void set_my_player_name( void );

#endif


