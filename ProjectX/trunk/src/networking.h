#ifndef NETWORKING_INCLUDED
#define NETWORKING_INCLUDED

//
// General Networking
//

#include "net.h"

// game tracker
char tracker_server[256];
int tracker_port;
BOOL tracker_enabled;

// my ship number
#define	UNASSIGNED_SHIP 0xff // WhoIAm before host gives me number
BYTE	WhoIAm;  // which ship number i am

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
#define FRAMELAGED_RECOIL					TRUE
#define ONEOFF_RECOIL						FALSE
#define MAX_TEAMS								4
#define NUMTITANBITS							10 

BOOL PlayerReady[MAX_PLAYERS];
/*
 * structures
 */
/*===================================================================
	Bodge Structures
===================================================================*/
typedef struct SHORTMINE{

	uint16	OwnerType;
	uint16	Owner;
	uint16	BulletID;
	uint16	Group;
	VECTOR	Pos;
	VECTOR	Offset;
	VECTOR	Dir;
	VECTOR	UpVector;
	VECTOR	DropDir;
	uint16	Weapon;
	float	LifeCount;

}SHORTMINE;

/*===================================================================
	Bodge Structures
===================================================================*/
typedef struct SHORTKILLMINE{
	uint16	OwnerType;			// Owner ID
	uint16	Owner;				// Owner ID
	uint16	ID;					// ID
	float	ShockwaveSize;		// Size
}SHORTKILLMINE;
/*===================================================================
	Bodge Structures
===================================================================*/

typedef struct MISSEDMINEMSG{
 	int16	Type;
	void *	Struct;
}MISSEDMINEMSG;

/*===================================================================
	Bodge Structures
===================================================================*/
typedef struct SHORTPICKUP{
 	int16	Type;
	int16	Group;
	uint16	Owner;				// Owner ID
	uint16	ID;					// ID
	VECTOR	Pos;
	VECTOR	Dir;
	int16	RegenSlot;
	float	Speed;
	BOOL	Sparkle;
	float	LifeCount;
	uint16	TriggerMod;
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

	uint16	GenType;			// Generation Type
	uint16	RegenType;			// Regeneration Type
	float	GenDelay;			// Generation Delay ( 60hz )
	float	Lifespan;			// Lifespan of pickup ( 60hz )
	float	Wait;				// Global count
	int16	Status;				// Status of slot
	int16	Group;				// Group to generate pickup
	VECTOR	Pos;				// Position to generate pickup
	uint16	Type;				// Type of pickup
	uint16	TriggerMod;			// Trigger Mod Index
	uint16	PickupIndex;		// Pickup Index
	uint16	PickupID;			// Pickup ID

}SHORTREGENSLOT;

/*===================================================================
	Bodge Structures
===================================================================*/
typedef struct SHORTKILLPICKUP{
	uint16	Owner;				// Owner ID
	uint16	ID;					// ID
	int16	Style;				// Style
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
 	int16	Type;
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
	uint16		IDCount;
	int16		Type;
	uint16		Group;
	VECTOR		Pos;
	VECTOR		Dir;
	float		Speed;
	int16		RegenSlot;
	BOOL		Sparkle;
	float		LifeCount;
	uint16		TriggerMod;
}PICKUPINFO; // 46

typedef struct _VERYSHORTPICKUPINFO
{
	uint16		IDCount;
	BYTE		Type;
	BYTE		Group;
	BYTE		RegenSlot;
	BYTE		Sparkle;
	SHORTVECTOR	Pos;
	SHORTVECTOR	Dir;
	float		Speed;
	float		LifeCount;
	uint16		TriggerMod;
}VERYSHORTPICKUPINFO; // 28



typedef struct _KILLPICKUPINFO
{
	uint16		Owner;
	uint16		IDCount;
	int16		Style;
}KILLPICKUPINFO;


typedef struct _EXPSECONDARYINFO
{
	uint16		OwnerType;
	uint16		Owner;
	uint16		IDCount;
	float		ShockwaveSize;
	VECTOR		Pos;
	uint16		Group;

}EXPSECONDARYINFO;

typedef struct _TEAMGOALSINFO
{
	uint16		TeamGoals[ MAX_TEAMS ];

}TEAMGOALSINFO;

typedef struct _SHOCKWAVEINFO
{
	uint16		Owner;
	uint16		Group;
	VECTOR		Pos;
	BYTE		Weapon;
	float		ShockwaveSize;
}SHOCKWAVEINFO;

typedef struct _BGOUPDATEINFO
{
	uint16		BGObject;
	int16		State;
	float		Time;
}BGOUPDATEINFO;

typedef struct _PRIMBULLPOSDIR
{
	uint16		OwnerType;
	uint16		OwnerID;
	uint16		BulletID;
	int8		Weapon;
	uint16		Group;
	VECTOR		Pos;
	VECTOR		Offset;
	VECTOR		Dir;
	VECTOR		Up;
	int16		PowerLevel;
	float		PLevel;

}PRIMBULLPOSDIR;

typedef struct _SECBULLPOSDIR
{
	uint16		OwnerType;
	uint16		Owner;
	uint16		BulletID;
	uint16		Group;
	VECTOR		Pos;
	VECTOR		Offset;
	VECTOR		Dir;
	VECTOR		UpVector;
	VECTOR		DropDir;
	int8		Weapon;

}SECBULLPOSDIR;

typedef struct _TITANBITS
{
	uint16		OwnerType;
	uint16		Owner;
	uint16		BulletID;
	uint16		Group;
	VECTOR		Pos;
	VECTOR		Offset;
	VECTOR		UpVector;
	VECTOR		DropDir;
	VECTOR		Directions[ NUMTITANBITS ];
	int8		Weapon;

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
	BOOL		OneOffExternalForce;

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
	uint16		OneOffExternalForce;
	uint16		Recoil_Scalar;
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
	BOOL				Invul;						// Am I Invulnerable...
	VECTOR				LastAngle;					// what my last Step Turn Angles Were...
	uint16				PrimBullIdCount;			// Id count for every bullet I fire....
	uint16				SecBullIdCount;				// Id count for every bullet I fire....
	uint16				PickupIdCount;				// Id count for every pickup I generate.....
	float				Damage;						// how much Damege I Have Taken...
	BYTE				Primary;					// which weapon is my primary....
	BYTE				Secondary;					// which wepaon is my secondary....
	uint16				ModelNum;					// which model to display for them...
	int16				BikeNum;					// which model to display for them...
	BYTE				Pickups;
	BYTE				RegenSlots;
	BYTE				Triggers;
	BYTE				TrigVars;
	BYTE				Mines;
	BOOL				JustRecievedPacket;			//
	VECTOR				LastMove;					// last movement vector (framelagged)
	VECTOR				Move_Off;					// Last MoveMent...x , y , z
	network_player_t *  network_player;
	uint16				OrbModels[ MAXMULTIPLES ];	// Orbit Pulsar Model
	float				OrbAmmo[ MAXMULTIPLES ];	// Orbit Pulsar Ammo

	float				PrimPowerLevel;				// Trojax Powerlevel
	uint16				PrimID;						// Bullet ID
	uint16				SecID;						// Missile ID
	BYTE				SecWeapon;					// Weapon
	BOOL	headlights; // are my headlights on?

	BOOL				DemoInterpolate;

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
	uint16		TempLines[ 12 ];

	float		ShakeTimer;
	float		ShakeDirTimer;
	float		ShakeForce;

	float		PacketDelay;

	uint8		ShieldHullCount;

	VECTOR		RealPos;
	uint16		RealGroup;

} GLOBALSHIP, *LPGLOBALSHIP;



/*		this is all that really needs to be sent	*/
typedef struct _SHORTGLOBALSHIP
{
	uint32				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Status;			// tells us constantly what this ship is doing...
	BYTE				GroupImIn;		// Group Im In

	VECTOR				Pos;			// x , y , z position
	VECTOR				Move_Off;		// Last MoveMent...x , y , z
	VECTOR				Angle;			// Last Rotation..
#ifdef	SHORTBANK
	int16				Bank;			// How much Am I banked....
#else
	float				Bank;			// How much Am I banked....
#endif
	QUAT				Quat;			// Final Quat...
} SHORTGLOBALSHIP, *LPSHORTGLOBALSHIP;

typedef struct _VERYSHORTGLOBALSHIP
{
	uint32				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Status;			// tells us constantly what this ship is doing...
	BYTE				GroupImIn;		// Group Im In
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTVECTOR			Move_Off;		// Last MoveMent...x , y , z
	SHORTVECTOR			Angle;			// Last Rotation..
	int16				Bank;			// How much Am I banked....
	uint16				Move_Off_Scalar;
	SHORTQUAT			Quat;			// Final Quat...
} VERYSHORTGLOBALSHIP, *LPVERYSHORTGLOBALSHIP;


typedef struct _FSHORTGLOBALSHIP
{
	uint32				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Primary;		// Primary weapon
	BYTE				Secondary;		// Secondary weapon
	BYTE				GroupImIn;		// Group Im In
	BYTE				PrimPowerLevel;	// Trojax Powerlevel
	VECTOR				Pos;			// x , y , z position
	VECTOR				Move_Off;		// Last MoveMent...x , y , z
	VECTOR				Angle;			// Last Rotation..
#ifdef	SHORTBANK
	int16				Bank;		// How much Am I banked....
#else
	float				Bank;		// How much Am I banked....
#endif
	QUAT				Quat;		// Final Quat...
} FSHORTGLOBALSHIP, *LPFSHORTGLOBALSHIP;

typedef struct _FVERYSHORTGLOBALSHIP
{
	uint32				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Primary;		// Primary weapon
	BYTE				Secondary;		// Secondary weapon
	BYTE				GroupImIn;		// Group Im In
	BYTE				PrimPowerLevel;	// Trojax Powerlevel
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTVECTOR			Move_Off;		// Last MoveMent...x , y , z
	SHORTVECTOR			Angle;			// Last Rotation..
	int16				Bank;		// How much Am I banked....
	uint16				Move_Off_Scalar;
	SHORTQUAT			Quat;		// Final Quat...
} FVERYSHORTGLOBALSHIP, *LPFVERYSHORTGLOBALSHIP;

typedef struct _GROUPONLY_FVERYSHORTGLOBALSHIP
{
	uint32				Flags;			// Flags Enable/Stealth/Turbo/PrimFire/SecFire/PrimToggle/SecToggle
	BYTE				Primary;		// Primary weapon
	BYTE				Secondary;		// Secondary weapon
	BYTE				GroupImIn;		// Group Im In
	BYTE				PrimPowerLevel;	// Trojax Powerlevel
	SHORTVECTOR			Pos;			// x , y , z position
	SHORTQUAT			Quat;		// Final Quat...
	int16				Bank;		// How much Am I banked....
} GROUPONLY_FVERYSHORTGLOBALSHIP, *LPGROUPONLY_FVERYSHORTGLOBALSHIP;

//----------------------------------------------------------
// Mask Defines for .Flags uint32
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
// converting all the BOOL's to BYTE's (even with all the new added fields) saved 72 bits...

typedef struct _INITMSG
{
    BYTE	MsgCode;

	// BOOLEANS

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

	uint16	RandomStartPosModify;
	uint16	Seed1;
	uint16	Seed2;

	int16	GoalScore;
	int16	BountyBonusInterval;

	uint32	PickupFlags[MAX_PICKUPFLAGS];

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

typedef	int16		SHORTSTATSTYPE[MAX_PLAYERS];

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
	BOOL		IsHost;			// from host ???
	BYTE		Status;
	BYTE		TeamNumber;
	BOOL		IAmReady;		// used for team game - game cannot start until everyone is ready
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
	BOOL		IsHost;
	float		PacketsPerSecond;
	BOOL		CollisionPerspective;
	BOOL		ShortPackets;	
} NETSETTINGSMSG, *LPNETSETTINGSMSG;

typedef struct _LONGSTATUSMSG
{
    BYTE        MsgCode;
    BYTE        WhoIAm;
	STATUSMSG	Status;
	char		LevelName[32];
	uint16		RandomStartPosModify;
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
	int16		NextBank;			// what my next bank angle will be..
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

#define MAXLEVELSPERBATCH 8

/*
 * fn prototypes
 */
void	DestroyGame( void );
void	SendGameMessage( BYTE msg, network_player_t * to, BYTE row, BYTE col, BYTE mask );
void	EvaluateMessage( network_player_t * from, DWORD len , BYTE * MsgPnt );
void	ReceiveGameMessages( void );
void	initShip( uint16 i );
void	NetworkGameUpdate();
void	SetupNetworkGame();
void	PrimBullPosDirSend( uint16 OwnerType, uint16 OwnerID, uint16 BulletID, int8 Weapon,
						uint16 Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16 PowerLevel, float PLevel );
void SecBullPosDirSend( uint16 OwnerType, uint16 Owner, uint16 BulletID, uint16 Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * UpVector,
						VECTOR * DropDir, int8 Weapon );
void TitanBitsSend( uint16 OwnerType, uint16 Owner, uint16 BulletID, uint16 Group,
					    VECTOR * Pos, VECTOR * Offset, VECTOR * UpVector,
						VECTOR * DropDir, int8 Weapon, VECTOR * Directions );
void	DropPickupSend( VECTOR * Pos, uint16 Group, VECTOR * Dir, float Speed, int16 Type, uint16 IDCount, int16 RegenSlot, BOOL Sparkle, float LifeCount, uint16 TriggerMod);
void	KillPickupSend( uint16 Owner, uint16 IDCount, int16 Style );
void	CreateShockwaveSend( uint16 OwnerType, uint16 Owner, VECTOR * Pos, uint16 Group, float ShockwaveSize, BYTE Weapon );
void	IHitYou( BYTE you, float Damage, VECTOR * Recoil, VECTOR * Point, VECTOR * Dir, float Force, BYTE WeaponType, BYTE Weapon, BOOL FramelagRecoil );
void	ShipDiedSend( BYTE WeaponType, BYTE Weapon );
void	UpdateBGObjectSend( uint16 BGObject, int16 State, float Time );
void	smallinitShip( uint16 i );
void DemoPlayingNetworkGameUpdate(void);
void DemoClean( void );
int FindSameLevel( char * Name );
void	RequestTime( void  );
void	SetTime( float Time );
void Demo_fwrite( const void *buffer, size_t size, size_t count , FILE *stream );
void StopDemoRecording( void );
BOOL UpdateAmmoAndValidateMessage( void * Message );
BOOL AutoJoinSession( void );
void AllocatePseudoHost( void );

void set_my_player_name( void );

#endif


