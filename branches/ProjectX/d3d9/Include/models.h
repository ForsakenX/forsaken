
#ifndef MODELS_INCLUDED
#define MODELS_INCLUDED

#include <math.h>
#include "d3dmain.h"
#include "typedefs.h"
#include "New3D.h"
#include "Mload.h"
#include "collision.h"
#include "Ships.h"
#include "Mxload.h"
#include "Mxaload.h"
#include "lights.h"
#include "camera.h"

/*
 * defines
 */
#define MAXNUMOFMODELS		1024
#define NUMOFTITLEMODELS	32
#define NUM_INTERLEVEL_MODELS 17
#define	MAXMODELHEADERS		1024
#define	ORBIT_RADIUS		( 256 * GLOBAL_SCALE )
#define	MAXMODELSPOTFX		10

#define SCAN_WINDOW_WIDTH 20.0F

#define	MODFLAG_Nothing			0
#define	MODFLAG_Clip			1
#define	MODFLAG_Light			2
#define	MODFLAG_AmbientLight	4
#define	MODFLAG_UseClipGroup	8
#define	MODFLAG_RealLight		16
#define	MODFLAG_Stealth			32
#define	MODFLAG_Original		64
#define MODFLAG_SpotFXDisabled	128
#define MODFLAG_ShockwaveHitMe	256
#define MODFLAG_StealthOffset	512

enum {
	BIKE_Lokasenna, 
	BIKE_Beard, 
	BIKE_LAJay, 
	BIKE_ExCop, 
	BIKE_RexHardy,
	BIKE_Foetoid,
	BIKE_NimSoo,
	BIKE_Nutta,
	BIKE_Sceptre,
	BIKE_Jo,
	BIKE_Shark,
	BIKE_HK5,
	BIKE_Nubia,
	BIKE_Mofisto,
	BIKE_Cerbero,
	BIKE_Slick,
	BIKE_FlyGirl,
};

enum {

	MODEL_Heatseaker,			// 0
	MODEL_HeatseakerSmall,		// 1
	MODEL_Gravgon,				// 2
	MODEL_Gravgon_LOD1,			// 3
	MODEL_Trojax,				// 4
	MODEL_Trojax_LOD1,			// 5
	MODEL_Pyrolite,				// 6
	MODEL_Pyrolite_LOD1,		// 7
	MODEL_Transpulse,			// 8
	MODEL_Transpulse_LOD1,		// 9
	MODEL_SussGun,				// 10 
	MODEL_SussGun_LOD1,			// 11 
	MODEL_Laser,				// 12
	MODEL_Laser_LOD1,			// 13
	MODEL_PowerPod,				// 14
	MODEL_PowerPod_LOD1,		// 15
	MODEL_PyroliteAmmo,			// 16
//	MODEL_PyroliteAmmo_LOD1,	// 17
	MODEL_Shield,				// 18
	MODEL_Shield_LOD1,			// 19
	MODEL_SussGunAmmo,			// 20
	MODEL_SussGunAmmo_LOD1,		// 21
	MODEL_GeneralAmmo,			// 22
	MODEL_GeneralAmmo_LOD1,		// 23
	MODEL_Mug,					// 24
	MODEL_MugSmall,				// 25
	MODEL_Mugs,					// 26
	MODEL_Mugs_LOD1,			// 27
	MODEL_Scatter,				// 28
	MODEL_ScatterSmall,			// 29
//	MODEL_Thief,				// 30
//	MODEL_ThiefSmall,			// 31
	MODEL_TitanStar,			// 32
	MODEL_TitanStarSmall,		// 33
	MODEL_Launcher,				// 34
	MODEL_Launcher_LOD1,		// 35
	MODEL_Computer,				// 36
	MODEL_Computer_LOD1,		// 37
	MODEL_Pine,					// 38
	MODEL_Pine_LOD1,			// 39
	MODEL_PinePickup,			// 40
	MODEL_PinePickup_LOD1,  	// 41
	MODEL_QuantumPickup,		// 42
	MODEL_QuantumPickup_LOD1,	// 43
	MODEL_Quantum,				// 44
	MODEL_Quantum_LOD1, 		// 45
	MODEL_Purge,				// 46
	MODEL_Purge_LOD1,   		// 47
//	MODEL_Spider,				// 48
//	MODEL_Spider_LOD1,			// 49
	MODEL_HeatseakerPickup,		// 50
	MODEL_HeatseakerPickup_LOD1,// 51
//	MODEL_Smoke,				// 52
//	MODEL_Smoke_LOD1,			// 53
	MODEL_PurgePickup,			// 54
	MODEL_PurgePickup_LOD1, 	// 55
//	MODEL_Parasite,				// 56
//	MODEL_Parasite_LOD1,		// 57
	MODEL_Nitro,				// 58
	MODEL_Nitro_LOD1,			// 59
//	MODEL_Goggles,				// 60
//	MODEL_Goggles_LOD1,			// 61
	MODEL_Inv,					// 62
	MODEL_Inv_LOD1,				// 63
	MODEL_Gold,					// 64
	MODEL_Mantle,				// 65
	MODEL_Mantle_LOD1,			// 66
//	MODEL_Flare,				// 67
//	MODEL_Flare_LOD1,			// 68
//	MODEL_SpiderPod,			// 69
//	MODEL_SpiderPod_LOD1,		// 70
	MODEL_Crystal,				// 71
	MODEL_Orb,					// 72
	MODEL_Orb_LOD1,  			// 73
	MODEL_ExtraLife,			// 74
	MODEL_ExtraLife_LOD1,		// 75
	MODEL_PineMissile,			// 76
	MODEL_Sphere,				// 77
	MODEL_Sphere_LOD1,			// 78
	MODEL_Sphere_LOD2,			// 79
//	MODEL_Chaf,					// 80
//	MODEL_Chaf_LOD1,			// 81
//	MODEL_Panel,				// 82
	MODEL_GoldenPowerPod,		// 83
	MODEL_GoldenPowerPod_LOD1,	// 84
	MODEL_OrbitPulsar,			// 85
	MODEL_OrbitPulsar_Light,	// 86
	MODEL_TitanStarShrapnel,	// 87
	MODEL_Tracker,				// 88
	MODEL_Ping,					// 89
	MODEL_Brain,				// 90
	MODEL_Ear,					// 91
	MODEL_Eyeball,				// 92
	MODEL_Heart,				// 93
	MODEL_Kidney,				// 94
	MODEL_Leg,					// 95
	MODEL_ExplosionDonut,		// 96
	MODEL_EnemySpiralMissile,	// 97
	MODEL_DNA,					// 98
	MODEL_SkeletonKey,			// 99

	MODEL_Shrapnel,				// 99
	MODEL_Shrapnel_LOD1,		// 99
	MODEL_Shrapnel2,			// 99
	MODEL_Shrapnel2_LOD1,		// 99
	MODEL_Glass,				// 99
	MODEL_Rock,					// 99
	MODEL_Rock2,				// 99

	MODEL_Bomb,					// 99
	MODEL_GoldFigure,			// 99
	MODEL_Flag,					// 99
	MODEL_RedFlag,				// 99
	MODEL_GreenFlag,			// 99
	MODEL_BlueFlag,				// 99
	MODEL_YellowFlag,			// 99

	MODEL_Tom0,					// 100
	MODEL_Tom1,
	MODEL_Tom2,
	MODEL_Tom3,
	MODEL_Tom4,
	MODEL_Tom5,
	MODEL_Tom6,
	MODEL_Tom7,
	MODEL_Tom8,
	MODEL_Tom9,
	MODEL_Tom10,
	MODEL_Tom11,
	MODEL_Tom12,
	MODEL_Tom13,
	MODEL_Tom14,
	MODEL_Tom15,
	MODEL_Tom16,
	MODEL_Tom17,
	MODEL_Tom18,
	MODEL_Tom19,
	MODEL_Tom20,
	MODEL_Tom21,
	MODEL_Tom22,
	MODEL_Tom23,
	MODEL_Tom24,
	MODEL_Tom25,
	MODEL_Tom26,

/*--------------- Bikes -----------------*/

	MODEL_Borg,					// 127
	MODEL_Borg_LOD1,
	MODEL_Borg_LOD2,
	MODEL_Borg_LOD3,
	MODEL_Borg_LOD4,
	MODEL_Borg_Stealth,
	MODEL_Borg_Stealth_LOD1,
	MODEL_Borg_Stealth_LOD2,
	MODEL_Borg_Stealth_LOD3,
	MODEL_Borg_Stealth_LOD4,

	MODEL_Beard,				// 137
	MODEL_Beard_LOD1,
	MODEL_Beard_LOD2,
	MODEL_Beard_LOD3,
	MODEL_Beard_LOD4,
	MODEL_Beard_Stealth,
	MODEL_Beard_Stealth_LOD1,
	MODEL_Beard_Stealth_LOD2,
	MODEL_Beard_Stealth_LOD3,
	MODEL_Beard_Stealth_LOD4,
	
	MODEL_LaJay,				// 147
	MODEL_LaJay_LOD1,
	MODEL_LaJay_LOD2,
	MODEL_LaJay_LOD3,
	MODEL_LaJay_LOD4,
	MODEL_LaJay_Stealth,
	MODEL_LaJay_Stealth_LOD1,
	MODEL_LaJay_Stealth_LOD2,
	MODEL_LaJay_Stealth_LOD3,
	MODEL_LaJay_Stealth_LOD4,
	
	MODEL_Excop,				// 157
	MODEL_Excop_LOD1,
	MODEL_Excop_LOD2,
	MODEL_Excop_LOD3,
	MODEL_Excop_LOD4,
	MODEL_Excop_Stealth,
	MODEL_Excop_Stealth_LOD1,
	MODEL_Excop_Stealth_LOD2,
	MODEL_Excop_Stealth_LOD3,
	MODEL_Excop_Stealth_LOD4,
	
	MODEL_Trucker,				// 167
	MODEL_Trucker_LOD1,				 
	MODEL_Trucker_LOD2,
	MODEL_Trucker_LOD3,
	MODEL_Trucker_LOD4,
	MODEL_Trucker_Stealth,
	MODEL_Trucker_Stealth_LOD1,
	MODEL_Trucker_Stealth_LOD2,
	MODEL_Trucker_Stealth_LOD3,
	MODEL_Trucker_Stealth_LOD4,
	
	MODEL_Foetoid,				// 177
	MODEL_Foetoid_LOD1,
	MODEL_Foetoid_LOD2,
	MODEL_Foetoid_LOD3,
	MODEL_Foetoid_LOD4,
	MODEL_Foetoid_Stealth,
	MODEL_Foetoid_Stealth_LOD1,
	MODEL_Foetoid_Stealth_LOD2,
	MODEL_Foetoid_Stealth_LOD3,
	MODEL_Foetoid_Stealth_LOD4,
	
	MODEL_JapBird,				// 187
	MODEL_JapBird_LOD1,
	MODEL_JapBird_LOD2,
	MODEL_JapBird_LOD3,
	MODEL_JapBird_LOD4,
	MODEL_JapBird_Stealth,
	MODEL_JapBird_Stealth_LOD1,
	MODEL_JapBird_Stealth_LOD2,
	MODEL_JapBird_Stealth_LOD3,
	MODEL_JapBird_Stealth_LOD4,
	
	MODEL_Nutter,				// 197
	MODEL_Nutter_LOD1,
	MODEL_Nutter_LOD2,
	MODEL_Nutter_LOD3,
	MODEL_Nutter_LOD4,
	MODEL_Nutter_Stealth,
	MODEL_Nutter_Stealth_LOD1,
	MODEL_Nutter_Stealth_LOD2,
	MODEL_Nutter_Stealth_LOD3,
	MODEL_Nutter_Stealth_LOD4,
	
	MODEL_Rhesus,				// 207
	MODEL_Rhesus_LOD1,
	MODEL_Rhesus_LOD2,
	MODEL_Rhesus_LOD3,
	MODEL_Rhesus_LOD4,
	MODEL_Rhesus_Stealth,
	MODEL_Rhesus_Stealth_LOD1,
	MODEL_Rhesus_Stealth_LOD2,
	MODEL_Rhesus_Stealth_LOD3,
	MODEL_Rhesus_Stealth_LOD4,
	
	MODEL_Joe,					// 217
	MODEL_Joe_LOD1,
	MODEL_Joe_LOD2,
	MODEL_Joe_LOD3,
	MODEL_Joe_LOD4,
	MODEL_Joe_Stealth,
	MODEL_Joe_Stealth_LOD1,
	MODEL_Joe_Stealth_LOD2,
	MODEL_Joe_Stealth_LOD3,
	MODEL_Joe_Stealth_LOD4,
	
	MODEL_Shark,				// 227
	MODEL_Shark_LOD1,
	MODEL_Shark_LOD2,
	MODEL_Shark_LOD3,
	MODEL_Shark_LOD4,
	MODEL_Shark_Stealth,
	MODEL_Shark_Stealth_LOD1,
	MODEL_Shark_Stealth_LOD2,
	MODEL_Shark_Stealth_LOD3,
	MODEL_Shark_Stealth_LOD4,
	
	MODEL_HK5,					// 237
	MODEL_HK5_LOD1,
	MODEL_HK5_LOD2,
	MODEL_HK5_LOD3,
	MODEL_HK5_LOD4,
	MODEL_HK5_Stealth,
	MODEL_HK5_Stealth_LOD1,
	MODEL_HK5_Stealth_LOD2,
	MODEL_HK5_Stealth_LOD3,
	MODEL_HK5_Stealth_LOD4,
	
	MODEL_Nubia,				// 247
	MODEL_Nubia_LOD1,
	MODEL_Nubia_LOD2,
	MODEL_Nubia_LOD3,
	MODEL_Nubia_LOD4,
	MODEL_Nubia_Stealth,
	MODEL_Nubia_Stealth_LOD1,
	MODEL_Nubia_Stealth_LOD2,
	MODEL_Nubia_Stealth_LOD3,
	MODEL_Nubia_Stealth_LOD4,

	MODEL_Mofisto,				// 257
	MODEL_Mofisto_LOD1,
	MODEL_Mofisto_LOD2,
	MODEL_Mofisto_LOD3,
	MODEL_Mofisto_LOD4,
	MODEL_Mofisto_Stealth,
	MODEL_Mofisto_Stealth_LOD1,
	MODEL_Mofisto_Stealth_LOD2,
	MODEL_Mofisto_Stealth_LOD3,
	MODEL_Mofisto_Stealth_LOD4,

	MODEL_Cerbero,				// 267
	MODEL_Cerbero_LOD1,
	MODEL_Cerbero_LOD2,
	MODEL_Cerbero_LOD3,
	MODEL_Cerbero_LOD4,
	MODEL_Cerbero_Stealth,
	MODEL_Cerbero_Stealth_LOD1,
	MODEL_Cerbero_Stealth_LOD2,
	MODEL_Cerbero_Stealth_LOD3,
	MODEL_Cerbero_Stealth_LOD4,

	MODEL_Slick,				// 277
	MODEL_Slick_LOD1,
	MODEL_Slick_LOD2,
	MODEL_Slick_LOD3,
	MODEL_Slick_LOD4,
	MODEL_Slick_Stealth,
	MODEL_Slick_Stealth_LOD1,
	MODEL_Slick_Stealth_LOD2,
	MODEL_Slick_Stealth_LOD3,
	MODEL_Slick_Stealth_LOD4,

/*---------------------------------------*/

	MODEL_ExtraModels,			// 287
};

enum {

	TITLE_MODEL_Disc1,				//0 - Disc 1 of spinning mechanical device
	TITLE_MODEL_Disc2,				//1 - Disc 2
	TITLE_MODEL_Disc3,				//2 - Disc 3
	TITLE_MODEL_Disc4,				//3 - Disc 4
	TITLE_MODEL_Disc5,				//4 - Disc 5
	TITLE_MODEL_Disc6,				//5 - Disc 6			  
	TITLE_MODEL_Mbot,				//6 - Bottom of mech. device
	TITLE_MODEL_Room,				//7 - Main Room
	TITLE_MODEL_VDU,				//8 - VDU
	TITLE_MODEL_Holopad,			//9 - Holopad
	TITLE_MODEL_Mtop,				//10 - Top of spinning mechanical device
	TITLE_MODEL_MenuTVDummy,				//11 - Top of spinning mechanical device
	TITLE_MODEL_MenuTV,				//11 - Top of spinning mechanical device
	TITLE_MODEL_HoloLight,			//12 - Top of spinning mechanical device
	TITLE_MODEL_Bracket,			//13 - Top of spinning mechanical device

	TITLE_MODEL_Lokasenna, 
	TITLE_MODEL_Beard, 
	TITLE_MODEL_LAJay, 
	TITLE_MODEL_ExCop, 
	TITLE_MODEL_RexHardy,
	TITLE_MODEL_Foetoid,
	TITLE_MODEL_NimSoo,
	TITLE_MODEL_Nutta,
	TITLE_MODEL_Sceptre,
	TITLE_MODEL_Jo,
	TITLE_MODEL_Shark,
	TITLE_MODEL_HK5,
	TITLE_MODEL_Nubia,
	TITLE_MODEL_Mofisto,
	TITLE_MODEL_Cerbero,
	TITLE_MODEL_Slick,

	TITLE_MODEL_ExtraModels,		// ??

};

enum{
	TITLE_MODEL_Crystal = TITLE_MODEL_Bracket + 1,
	TITLE_MODEL_GoldBar,
};
		
enum {								
									
	MODFUNC_Nothing,				
	MODFUNC_Explode,				
	MODFUNC_Scale,					
	MODFUNC_Scale2,
	MODFUNC_BodyPart,
	MODFUNC_OrbitPulsar,
	MODFUNC_ReScale,
	MODFUNC_RealScale,
	MODFUNC_ScaleDonut,
	MODFUNC_Regen,
	MODFUNC_TomCruise,
	MODFUNC_RestoreColours,
	MODFUNC_ScaleXYZandRotate,
	MODFUNC_ProcessSpotFX,
	MODFUNC_SphereZone,
	MODFUNC_LargeShrapnel,
	MODFUNC_SmallShrapnel,
	MODFUNC_Fade,

};

enum {

	MODTYPE_Pickup,
	MODTYPE_Missile,
	MODTYPE_Field,

};

enum {

	MODELTYPE_Static,
	MODELTYPE_Morphing,

};

/*
 * structures
 */
typedef struct MODEL{
	uint16		Next;
	uint16		Prev;
	int16		Type;
	uint16		Flags;
	BYTE		SecWeapon;
	BOOL		Visible;
	uint16		ModelNum;
	uint16		Group;
	uint16		OwnerType;
	uint16		Owner;
	int16		Func;
	float		Scale;
	float		Xscale;
	float		Yscale;
	float		Zscale;
	float		MaxScale;
	float		LifeCount;
	float		AxisRot;
	float		Speed;
	float		TimeInterval;
	float		TimeCount;

	int16		Frame;			// Current Frame
	int16		InterpFrame;	// Wanted Frame
	float		InterpTime;		// Interpolation Time

	QUATLERP	Qlerp;
	QUAT		Quat;
	uint16		Ship;
	VECTOR		Pos;
	VECTOR		OldPos;
	VECTOR		Dir;
	VECTOR		Rot;
	MATRIX		Mat;
	MATRIX		InvMat;

	float		Radius;			// Bounding Radius ( Used for Real Lighting

	int			Red;
	int			Green;
	int			Blue;

	uint16		SpotFXFlags[ MAXMODELSPOTFX ];
	uint16		SpotFXState[ MAXMODELSPOTFX ];
	float		SpotFXDelay[ MAXMODELSPOTFX ];
	float		SpotFXTimeInterval[ MAXMODELSPOTFX ];
	float		SpotFXTimeCount[ MAXMODELSPOTFX ];
	uint32		SpotFXSFX_ID[ MAXMODELSPOTFX ];

	int16		StealthOffset;
	uint16		ClipGroup;
	uint16		TempLines[ 12 ];

	PLANE		IntersectionPlane;

}MODEL;

#define NUM_FADE_FRAMES 120.0F

typedef struct MODELNAME{
	char		Name[128];			// Name of the Model...
	int			LOD;				// How many of the following Model are Levels Of Detail..
	BOOL		Panel;
	BOOL		DoIMorph;			// Morphing (MXA?) TRUE/FALSE
	uint16		ModelIndex;			// MXA or MX Header index
	BOOL		StoreTriangles;		// retain triangle info?
	int			AllocateTpage;		// do we want to allocate space for a T-Page?
	BOOL		LevelSpecific;		// Level Specific?
	BOOL		LoadEnable;			// LoadEnable?

}MODELNAME;

/*
 * fn prototypes
 */

void OnceOnlyInitModel( void );
BOOL PreInitModel( /*LPDIRECT3DDEVICE lpDev,*/ MODELNAME * ModelNames ); // bjd
BOOL InitModel( /*LPDIRECT3DDEVICE lpDev,*/ MODELNAME * ModelNames); // bjd
BOOL ModelDisp( uint16 group, /*LPDIRECT3DDEVICE lpDev,*/ MODELNAME * NamePnt  ); // bjd
void ProcessModels( void );


void	KillUsedModel( uint16 i );
uint16	FindFreeModel();

BOOL ReleaseModels( );
BOOL ProcessModel( MXLOADHEADER * DstMloadheader, float Scale, float MaxScale, int8 R, int8 G, int8 B );
BOOL ProcessModelExec( /*LPDIRECT3DEXECUTEBUFFER lpExBuf*/RENDEROBJECT *renderObject, int16 NumVerts, float Scale, float MaxScale, int8 R, int8 G, int8 B );
BOOL LightModel( uint16 Model, VECTOR * Pos );
BOOL LightMxModel( uint16 Model, VECTOR * Pos, float RF, float GF, float BF, float TF );
BOOL LightMxaModel( uint16 Model, VECTOR * Pos, float RF, float GF, float BF, float TF );
void ShockWave( VECTOR * Pos, float Radius, uint16 OwnerType, uint16 Owner, float Center_Damage, uint16 Group, BYTE Weapon, uint16 model );
void GravityWave( VECTOR * Pos, float Radius, uint16 Owner, float Center_Grav, uint16 Group );
void ExplodeBody( VECTOR * Pos, VECTOR * Dir, uint16 Group );
void ThrowOutRider( uint16 Ship );
void CreateBodyPart( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint16 ModelNum );
void CreateTomCruise( VECTOR * Pos, uint16 Group );
void MissileShockWave( VECTOR * Pos, float Radius, uint16 Owner, float Center_Damage, uint16 Group, BYTE Weapon );
void CreateOrbitPulsar( uint16 Ship );
BOOL LightModel2( uint16 Model, VECTOR * Pos );
BOOL CheckForGravgons( VECTOR * Pos );
BOOL TintModel( uint16 Model, float RF, float GF, float BF, float TF );
BOOL ShadeModel( uint16 Model, MATRIX * Matrix, float ZTrans, float Range );
void CreateExplosionDonut( VECTOR * Pos, VECTOR * Up, uint16 Group );
BOOL ReleaseTitleModels( );
BOOL TintMxaModel( MXALOADHEADER * DstMloadheader, float RF, float GF, float BF, float TF );
BOOL AmbientLightMxaModel( MXALOADHEADER * DstMloadheader, int R, int G, int B, int A , float r , float g , float b);
BOOL AmbientLightMxModel( MXLOADHEADER * DstMloadheader, int R, int G, int B, int A , float rp , float gp , float bp);
void HitMe( uint16 OwnerType, uint16 OwnerID, float Damage, uint8 WeaponType, uint8 Weapon );
BOOL ProcessModel2( MXLOADHEADER * DstMloadheader, float Scale, float MaxScale, int8 R, int8 G, int8 B );

uint16 CreateLine( float x1, float y1, float z1, float x2, float y2, float z2, uint16 Group );
BOOL GetMXBoundingBox( MXLOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft,
					   VECTOR * BottomRight );
BOOL GetMXABoundingBox( MXALOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft,
					    VECTOR * BottomRight );
BOOL CreateMXBoundingBox( MXLOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, uint16 * LineArray, uint16 Group );
BOOL CreateMXABoundingBox( MXALOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, uint16 * LineArray, uint16 Group );
BOOL ProcessSphereZoneModelExec( /*LPDIRECT3DEXECUTEBUFFER lpExBuf*/RENDEROBJECT *renderObject, int16 NumVerts, uint8 R, uint8 G, uint8 B );
void SetupModelSpotFX( uint16 i );
void CreateModelSpotFXFirePrimary( VECTOR * Pos, VECTOR * Dir, VECTOR * Up,
							    uint16 Group, uint8 Weapon, uint16 ModelIndex, uint16 SpotFXIndex );
void UpdateShipModel( uint16 Ship );
void UpdateObjectAmbientColour( OBJECT * Object );
BOOL UpdateMxaModel( MXALOADHEADER * MXAloadheader );
uint32 EngineCode( uint16 OwnerType, uint16 OwnerID, uint32 SoundFX_ID, float Speed );
void KillAttachedSoundFX( uint16 i );
FILE * SaveModels( FILE * fp );
FILE * LoadModels( FILE * fp );
BOOL ReinitSpotFXSFX( uint16 i );
void SetShipsVisibleFlag( void );
void EnableRelavantModels( MODELNAME * ModelNamesPtr );
void KillAttachedSpotFX( uint16 i );
void KillAllBikeEngines( void );
BOOL EngineEnabled( uint16 OwnerType, uint16 Owner );

#endif // MODELS_INCLUDED


