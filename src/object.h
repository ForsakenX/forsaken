
#ifndef OBJECT_INCLUDED
#define OBJECT_INCLUDED

#include "main.h"
#include "quat.h"
#include "compobjects.h"

enum {
	OBJECT_TYPE_SHIP,
	OBJECT_TYPE_ENEMY,
};

typedef struct OBJECT
{
	u_int32_t				Flags;		// Various Flags....
	u_int16_t				Type;		// Ship....Enemy....
	void *				ID;			// Which ship...enemy...
	u_int16_t				AI_Mode;	// What My AI Thinks It is doing....
	BYTE				Mode;		// What Im Currently doing....flying around...dying...exploding a bit.......
	u_int16_t				ControlType; // Who is controling me

	VECTOR				Pos;		// x , y , z position
	QUAT				Quat;		// what my current viewing angles are...
	float				Bank;		// How much Am I banked....
	MATRIX				Mat;		// view matrix...............befor banking.. 
	MATRIX				InvMat;		// view matrix...............befor banking.. 
	
	MATRIX				FinalMat;	// Final view matrix...............after banking.. 
	MATRIX				FinalInvMat;// Final inverse view matrix.......after banking.. 
	QUAT				FinalQuat;	// what my current viewing angles are with the banking...

	VECTOR				DirVector;			// Direction Vector
//	VECTOR				UpVector; 			// Up Vector
//	VECTOR				LeftVector; 		// Left Vector
	u_int16_t				Group;		// Group the Object...
	VECTOR				Speed;		// Speed...x , y , z...Ship relative...
	VECTOR				Angle;		// what my current Step Turn Angles Are...

	int16_t				CruiseControl; // 0 = off, +1 = full ahead, -1 = full reverse, +2 = nitro ahead

	float				Shield;		// how much Shield ....
	float				Hull;		// how much Hull integrity ....

	int16_t				Energy;		// how much weapon energy....
	int16_t				PowerLevel;	// Power Level of weapon....
	VECTOR				ExternalForce; // external force impulse (global ie relative to world)
	VECTOR				InternalForce; // internal force impulse (local ie relative to ship)
	QUAT				RotationForce; // rotation force (local ie relative to ship)
	float				Autolevel;	// how much autolevel is in effect
	float				BobCount;	// where we are in the bob cycle
	VECTOR				LastBob;	// last bob movement (framelagged, in world coords)
	u_int16_t				light;
	float				Noise;		// says how much noise an object is making....

	u_int32_t				NodeNetwork;	// What node network im attached to....
	void *				NearestNode;	// What node Im nearest to....
	void *				LastNearestNode;	// A node that Was on my network that I was closest to...

	BYTE				HowManyGuns;	// How many guns there are...
	struct GUNOBJECT * FirstGun;		// the first gun...

	float				Time;
	float				OverallTime;
	float				AnimSpeed;
	VECTOR				TopLeft;		// Bounding Box TL
	VECTOR				BottomRight;	// Bounding Box BR
	ANIM_SEQ	*		AnimSeqs;
	int16_t				CurAnimSeq;		// Animation Sequence
	bool				Animating;
	COMP_OBJ	*		Components;
	COMP_OBJ	*		UserContComps[ MAX_USER_CONT_COMPS ];

	float		Red;
	float		Green;
	float		Blue;

//struct	COLTYPE	*		ColType;	//
} OBJECT,*LPOBJECT;


typedef struct GUNOBJECT
{
	u_int16_t				Type;			// Type of gun
	struct OBJECT	*	ParentObject;	// Which ship...enemy...Owns Me..
	u_int32_t				AIMoveFlags;
	VECTOR				AI_Angle;
	MATRIX				Mat;			// view matrix
	MATRIX				InvMat;			// inv view matrix
	VECTOR				Dir;			// Direction Vector
	VECTOR				FirePos;		// Fire Pos
	int16_t				FirePosCount;	// if I have more than 1 firing position then cycle between them..
	VECTOR				Angle;			// what my current Step Turn Angles Are...
	struct GUNOBJECT * Next;	// the next gun in this list...
	float				BurstTime;			// How long between each burst
	int					BurstCount;			// How Many left in current burst
	float				ReloadTime;			// how long before I am able to fire..
	float				BurstStartSign;		// did I start this on the left or right side....
	int16_t				GunNum;
} GUNOBJECT,*LPGUNOBJECT;

typedef struct GUNTYPE
{
	float				gtMaxTurnSpeed;
	float				gtTurnAccell;
	float				gtTurnDecell;
	float				ReloadTime;			// How long between each reload
	float				BurstMasterTime;	// How long between each burst
	int					BurstMasterCount;	// How many shots a burst should have
	float				BurstAngle;			// How much angle between each Shot in a burst
	float				XRotMin;			// How far in the X can I Rotate..
	float				XRotMax;			
	float				YRotMin;			// How far in the Y can I Rotate..
	float				YRotMax;			
	float				Range;			
	int16_t				PowerLevel;			// Power Level of weapon....
	bool				PrimarySecondary;	// False for Primary True for Secondary...
	BYTE				WeaponType;			// which weapon do I Fire...
	bool				PreciseRotation;	// How Precies should I rotate...
} GUNTYPE,*LPGUNTYPE;

#endif	// OBJECT_INCLUDED

