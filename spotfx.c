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
#include "title.h"
#include "spotfx.h"

#include "sfx.h"
#include "util.h"

/*===================================================================
	External Variables
===================================================================*/
extern	int16_t			LevelNum;
extern	char			LevelNames[MAXLEVELS][128];
extern	FMPOLY			FmPolys[MAXNUMOF2DPOLYS];
extern	FRAME_INFO	*	NewTrail_Header;
extern	FRAME_INFO	*	Bits_Header;
extern	FRAME_INFO	*	Exp_Header;
extern	FRAME_INFO	*	Flare_Header;
extern	FRAME_INFO	*	Circle_Header;
extern	FRAME_INFO	*	BigFlash_Header;
extern	FRAME_INFO	*	GravgonTrail_Header;
extern	FRAME_INFO	*	Bubble_Header;
extern	FRAME_INFO	*	EnemyGen_Header;
extern	FRAME_INFO	*	Drip_Header;
extern	XLIGHT			XLights[MAXXLIGHTS];
extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
extern	MODEL			Models[MAXNUMOFMODELS];
extern	MATRIX			MATRIX_Identity;
extern	u_int16_t			IsGroupVisible[MAXGROUPS];
extern	u_int16_t			GlobalPrimBullsID;
extern	u_int16_t			GlobalSecBullsID;
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	ENEMY			Enemies[ MAXENEMIES ];

/*===================================================================
	Defines
===================================================================*/
#define	FX_VERSION_NUMBER	4

/*===================================================================
	Global Variables
===================================================================*/
	int32_t		NumSpotFX;
	SPOTFX		SpotFX[ MAXSPOTFX ];
	SPOTFX	*	SpotFXGroups[ MAXGROUPS ];
	u_int16_t		NumSpotFXPerGroup[ MAXGROUPS ];
	SPOTFX	*	FirstSpotFXUsed = NULL;
	SPOTFX	*	FirstSpotFXFree = NULL;

/*===================================================================
	Procedure	:	Load all SpotFX
	Input		:	nothing
	Output		:	bool	True/False
===================================================================*/
bool LoadSpotFX( void )
{
	FILE	*	fp;
	int16_t		Count;
	VECTOR		Pos, Dir, Up;
	u_int16_t		Type, Group;
	int8_t		Filename[ 256 ];
	char	*	NewExt = ".FX";
	SPOTFX	*	SpotFX;
	u_int16_t		GenType;
	float		GenDelay;
	float		ActiveDelay;
	float		InactiveDelay;
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;
	int32_t		Colour = 0x00ffffff;
	int16_t		Primary = -1;
	int16_t		Secondary = -1;
	int16_t		SoundFX = -1;
	float		Volume;
	float		Speed;
	int16_t		SFXType;
	int16_t		i;
	int8_t		SFXFilename[ 128 ];

	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &Filename[ 0 ], NewExt );


	fp = file_open( &Filename[ 0 ], "rb" );

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != FX_VERSION_NUMBER  ) )
		{
			fclose( fp );
			Msg( "LoadSpotFX() Incompatible SpotFX (.FX) file %s", &Filename[ 0 ] );
			return( false );
		}

		fread( &NumSpotFX, sizeof( int32_t ), 1, fp );

		if( NumSpotFX > MAXSPOTFX )
		{
			DebugPrintf( "Too many SpotFX (%d)\n", NumSpotFX );
			fclose( fp );
			return( false );
		}

		for( Count = 0; Count < NumSpotFX; Count++ )
		{
			fread( &Type, sizeof( u_int16_t ), 1, fp );
			fread( &Group, sizeof( u_int16_t ), 1, fp );
			fread( &Colour, sizeof( u_int32_t ), 1, fp );
			fread( &Pos.x, sizeof( float ), 1, fp );
			fread( &Pos.y, sizeof( float ), 1, fp );
			fread( &Pos.z, sizeof( float ), 1, fp );
			fread( &Dir.x, sizeof( float ), 1, fp );
			fread( &Dir.y, sizeof( float ), 1, fp );
			fread( &Dir.z, sizeof( float ), 1, fp );
			fread( &Up.x, sizeof( float ), 1, fp );
			fread( &Up.y, sizeof( float ), 1, fp );
			fread( &Up.z, sizeof( float ), 1, fp );
			fread( &ActiveDelay, sizeof( float ), 1, fp );
			fread( &InactiveDelay, sizeof( float ), 1, fp );
			fread( &GenType, sizeof( u_int16_t ), 1, fp );
			fread( &GenDelay, sizeof( float ), 1, fp );
			fread( &Primary, sizeof( int16_t ), 1, fp );
			fread( &Secondary, sizeof( int16_t ), 1, fp );

			i = 0;
			do
			{
				fread( &SFXFilename[ i ], sizeof( int8_t ), 1, fp );
				i++;
			}
			while( SFXFilename[ ( i - 1 ) ] != 0 );

			fread( &Volume, sizeof( float ), 1, fp );
			fread( &Speed, sizeof( float ), 1, fp );
			fread( &SFXType, sizeof( int16_t ), 1, fp );

			if( Type == SPOTFX_SoundFX )
			{
				SoundFX = ReturnSFXIndex( &SFXFilename[ 0 ] );
			}
			else
			{
				SoundFX = -1;
			}

			SpotFX = InitOneSpotFX( GenType, ( GenDelay * ANIM_SECOND ), Type, Group, &Pos, &Dir, &Up,
									( ActiveDelay * ANIM_SECOND ), ( InactiveDelay * 60.0F ), Colour, Primary, Secondary, SoundFX, Volume, Speed, SFXType );

			if( !SpotFX )
			{
				DebugPrintf( "Couldn't Initialise SpotFX (%d)\n", Count );
				fclose( fp );
				return( false );
			}	
		}

		fclose( fp );
	}

	return( true );
}

/*===================================================================
	Procedure	:	Process all SpotFX
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void ProcessSpotFX( void )
{
	SPOTFX	*	SpotFX;

	if( framelag == 0.0F ) return;

	SpotFX = FirstSpotFXUsed;

	while( SpotFX )
	{
		SpotFX->TimeInterval = (float) 1;

		if( SpotFX->Flags & SPOTFX_FLAGS_Enabled )
		{
			switch( SpotFX->State )
			{
				case SPOTFX_STATE_WaitingForTrigger:
					break;

				case SPOTFX_STATE_WaitingForDelay:
					SpotFX->Delay -= framelag;

					if( SpotFX->Delay < 0.0F )
					{
						SpotFX->Delay += SpotFX->ActiveDelay;
						SpotFX->State = SPOTFX_STATE_Active;
						SpotFX->Time = 0.0F;
					}
					break;

				case SPOTFX_STATE_Active:
					switch( SpotFX->Type )
					{

						case SPOTFX_FirePrimary:
						case SPOTFX_Explosion:
						case SPOTFX_Shrapnel:
						case SPOTFX_ElectricBeams:
							if( IsGroupVisible[ SpotFX->Group ] || VisibleOverlap( Ships[ Current_Camera_View ].Object.Group, SpotFX->Group, NULL ) )
							{
								switch( SpotFX->Type )
								{
									case SPOTFX_FirePrimary:
										if( !( SpotFX->Flags & SPOTFX_FLAGS_DoneOnce ) )
										{
											CreateSpotFXFirePrimary( &SpotFX->Pos, &SpotFX->DirVector, &SpotFX->UpVector,
																	SpotFX->Group, SpotFX->Primary, OWNER_NOBODY, 0 );
											SpotFX->Flags |= SPOTFX_FLAGS_DoneOnce;
										}
										break;

									case SPOTFX_Explosion:
										if( !( SpotFX->Flags & SPOTFX_FLAGS_DoneOnce ) )
										{
											CreateSpotFXExplosion( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group );
											SpotFX->Flags |= SPOTFX_FLAGS_DoneOnce;
										}
										break;

									case SPOTFX_Shrapnel:
										if( !( SpotFX->Flags & SPOTFX_FLAGS_DoneOnce ) )
										{
											CreateSpotFXShrapnel( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group );
											SpotFX->Flags |= SPOTFX_FLAGS_DoneOnce;
										}
										break;

									case SPOTFX_ElectricBeams:
										CreateSpotFXElectricBeams( &SpotFX->Pos, &SpotFX->DirVector, &SpotFX->UpVector,
																	SpotFX->Group );
										break;

								}
							}
							break;

						case SPOTFX_Flame:
						case SPOTFX_Smoke:
						case SPOTFX_Steam:
						case SPOTFX_Sparks:
						case SPOTFX_GravgonTrail:
						case SPOTFX_NmeTrail:
						case SPOTFX_NmeGlow:
						case SPOTFX_NmeVapourTrail:
						case SPOTFX_FireWall:
						case SPOTFX_GravitySparks:
						case SPOTFX_Bubbles:
						case SPOTFX_Drip:
						default:
							if( IsGroupVisible[ SpotFX->Group ] || VisibleOverlap( Ships[ Current_Camera_View ].Object.Group, SpotFX->Group, NULL ) )
							{
								SpotFX->Time += framelag;

								if( SpotFX->Time >= SpotFX->TimeInterval )
								{
					 				SpotFX->Time = FMOD( SpotFX->Time, SpotFX->TimeInterval );

									switch( SpotFX->Type )
									{
										case SPOTFX_Flame:
											CreateSpotFXFlame( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group );
											break;

										case SPOTFX_Smoke:
											CreateSpotFXSmoke( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, SpotFX->Red, SpotFX->Green, SpotFX->Blue );
											break;

										case SPOTFX_Steam:
											CreateSpotFXSteam( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, SpotFX->Red, SpotFX->Green, SpotFX->Blue );

											if( !( SpotFX->Flags & SPOTFX_FLAGS_DoneOnce ) )
											{
												if( !SpotFX->SoundFX_ID )
												{
													SpotFX->SoundFX_ID = PlaySpotSfx( SFX_Steam, &SpotFX->Group, &SpotFX->Pos,
																					  0.25F, 1.0F, SPOT_SFX_TYPE_Normal );
													SpotFX->Flags |= SPOTFX_FLAGS_DoneOnce;
												}
												else
												{
#ifdef DEBUG_SPOTFX_SOUND
													Msg( "SpotFX:SpotFX_Steam Overwriting SFX ID %d\n", SpotFX->SoundFX_ID );
#endif
													StopSfx( SpotFX->SoundFX_ID );
													SpotFX->SoundFX_ID = PlaySpotSfx( SFX_Steam, &SpotFX->Group, &SpotFX->Pos,
																					  0.25F, 1.0F, SPOT_SFX_TYPE_Normal );
													SpotFX->Flags |= SPOTFX_FLAGS_DoneOnce;
												}
											}
											break;

										case SPOTFX_Sparks:
											CreateSpotFXSparks( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, SpotFX->Red, SpotFX->Green, SpotFX->Blue );
											break;

										case SPOTFX_GravgonTrail:
											CreateSpotFXGravgonTrail( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group );
											break;

										case SPOTFX_NmeTrail:
											CreateSpotFXNmeTrail( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, SpotFX->Red, SpotFX->Green, SpotFX->Blue );
											break;

										case SPOTFX_NmeGlow:
											CreateSpotFXNmeGlow( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, SpotFX->Red, SpotFX->Green, SpotFX->Blue );
											break;

										case SPOTFX_NmeVapourTrail:
											CreateSpotFXNmeVapourTrail( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, SpotFX->Red, SpotFX->Green, SpotFX->Blue );
											break;

										case SPOTFX_FireWall:
											CreateSpotFXFireWall( &SpotFX->Pos, &SpotFX->DirVector, &SpotFX->UpVector, SpotFX->Group );

											if( !( SpotFX->Flags & SPOTFX_FLAGS_DoneOnce ) )
											{
												if( !SpotFX->SoundFX_ID )
												{
													SpotFX->SoundFX_ID = PlaySpotSfx( SFX_MedFireBlast, &SpotFX->Group, &SpotFX->Pos,
																					  0.0F, 1.0F, SPOT_SFX_TYPE_Normal );
													SpotFX->Flags |= SPOTFX_FLAGS_DoneOnce;
												}
												else
												{
#ifdef DEBUG_SPOTFX_SOUND
													Msg( "SpotFX:Firewall Overwriting SFX ID %d\n", SpotFX->SoundFX_ID );
#endif
													StopSfx( SpotFX->SoundFX_ID );
													SpotFX->SoundFX_ID = PlaySpotSfx( SFX_MedFireBlast, &SpotFX->Group, &SpotFX->Pos,
																					  0.0F, 1.0F, SPOT_SFX_TYPE_Normal );
													SpotFX->Flags |= SPOTFX_FLAGS_DoneOnce;
												}
											}
											break;

										case SPOTFX_GravitySparks:
											CreateSpotFXRealSparks( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, SpotFX->Red, SpotFX->Green, SpotFX->Blue );
											break;

										case SPOTFX_Bubbles:
											CreateSpotFXBubbles( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, SpotFX->Red, SpotFX->Green, SpotFX->Blue, SpotFX->MaxHeight );
											break;

										case SPOTFX_Drip:
											if( !( SpotFX->Flags & SPOTFX_FLAGS_DoneOnce ) )
											{
												CreateSpotFXDrip( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, SpotFX->Red, SpotFX->Green, SpotFX->Blue );
												SpotFX->Flags |= SPOTFX_FLAGS_DoneOnce;
											}
											break;

										case SPOTFX_BeardAfterburner:
											CreateSpotFXBeardAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_BorgAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_ExcopAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_TruckerAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_NubiaAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_CerberoAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_FoetoidAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_FoetoidSmallAfterburner:
											CreateSpotFXSmallAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_Hk5Afterburner:
											CreateSpotFXHk5Afterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F, 32, 32, 128 );
											break;

										case SPOTFX_JapBirdAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_JoAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_JoSmallAfterburner:
											CreateSpotFXSmallAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_LaJayAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_MofistoAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_NutterAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_RhesusAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_SharkAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										case SPOTFX_SlickAfterburner:
											CreateSpotFXBorgAfterburner( &SpotFX->Pos, &SpotFX->DirVector, SpotFX->Group, 0.0F );
											break;

										default:
											break;
									}
								}
							}

							break;


						case SPOTFX_FireSecondary:
						case SPOTFX_SoundFX:
							SpotFX->Time += framelag;

							if( SpotFX->Time >= SpotFX->TimeInterval )
							{
				 				SpotFX->Time = FMOD( SpotFX->Time, SpotFX->TimeInterval );

								switch( SpotFX->Type )
								{
									case SPOTFX_FireSecondary:
										if( !( SpotFX->Flags & SPOTFX_FLAGS_DoneOnce ) )
										{
											CreateSpotFXFireSecondary( &SpotFX->Pos, &SpotFX->DirVector, &SpotFX->UpVector,
																		SpotFX->Group, SpotFX->Secondary, OWNER_NOBODY, 0 );
											SpotFX->Flags |= SPOTFX_FLAGS_DoneOnce;
										}
										break;

									case SPOTFX_SoundFX:
										if( !( SpotFX->Flags & SPOTFX_FLAGS_DoneOnce ) )
										{
											if( SpotFX->SoundFX != -1 )
											{
												if( !SpotFX->SoundFX_ID )
												{
													SpotFX->SoundFX_ID = PlaySpotSfx( SpotFX->SoundFX, &SpotFX->Group, &SpotFX->Pos,
																					  SpotFX->SoundFXSpeed, SpotFX->SoundFXVolume,
																					  SpotFX->SoundFXType );
												}
												else
												{
#ifdef DEBUG_SPOTFX_SOUND
													Msg( "SpotFX:SpotFX_SoundFX Overwriting SFX ID %d\n", SpotFX->SoundFX_ID );
#endif
													StopSfx( SpotFX->SoundFX_ID );
													SpotFX->SoundFX_ID = PlaySpotSfx( SpotFX->SoundFX, &SpotFX->Group, &SpotFX->Pos,
																					  SpotFX->SoundFXSpeed, SpotFX->SoundFXVolume,
																					  SpotFX->SoundFXType );
												}
											}
											SpotFX->Flags |= SPOTFX_FLAGS_DoneOnce;
										}
										break;

								}
							}
							break;
					}

					SpotFX->Delay -= framelag;

					if( SpotFX->Delay < 0.0F )
					{
						if( SpotFX->InactiveDelay < 0.0F )
						{
							SpotFX->Flags &= ~SPOTFX_FLAGS_Enabled;
							SpotFX->Delay += SpotFX->ActiveDelay;
							SpotFX->Time = 0.0F;
						}
						else
						{
							if( SpotFX->InactiveDelay != 0.0F )
							{
								if( SpotFX->SoundFX_ID )
								{
									StopSfx( SpotFX->SoundFX_ID );
									SpotFX->SoundFX_ID = 0;
								}

								SpotFX->Delay += SpotFX->InactiveDelay;
								SpotFX->State = SPOTFX_STATE_Inactive;
								SpotFX->Time = 0.0F;
							}
							else
							{
								SpotFX->Delay += SpotFX->ActiveDelay;
							}
						}
					}
					break;

				case SPOTFX_STATE_Inactive:
					SpotFX->Delay -= framelag;

					if( SpotFX->Delay < 0.0F )
					{
						SpotFX->Delay += SpotFX->ActiveDelay;
						SpotFX->State = SPOTFX_STATE_Active;
					SpotFX->Flags &= ~SPOTFX_FLAGS_DoneOnce;
						SpotFX->Time = 0.0F;
					}
					break;
			}
		}

		SpotFX = SpotFX->NextUsed;
	}
}

/*===================================================================
	Procedure	:	Setup all SpotFX
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void SetupSpotFX( void )
{
	u_int16_t	i;

	SetupSpotFXGroups();

	FirstSpotFXUsed = NULL;
	FirstSpotFXFree = &SpotFX[ 0 ];

	for( i = 0; i < MAXSPOTFX; i++ )
	{
		memset( &SpotFX[ i ], 0, sizeof( SPOTFX ) );

		SpotFX[ i ].NextUsed = NULL;
		SpotFX[ i ].PrevUsed = NULL;
		SpotFX[ i ].NextFree = &SpotFX[ i + 1 ];
		SpotFX[ i ].PrevFree = &SpotFX[ i - 1 ];
		SpotFX[ i ].NextInGroup = NULL;
		SpotFX[ i ].PrevInGroup = NULL;
		SpotFX[ i ].Index = i;
	}

	SpotFX[ 0 ].PrevFree = NULL;
	SpotFX[ MAXSPOTFX - 1 ].NextFree = NULL;
}

/*===================================================================
	Procedure	:	Init One SpotFX
	Input		:	u_int16_t		GenType
				:	float		GenDelay
				:	u_int16_t		Type
				:	u_int16_t		Group
				:	VECTOR	*	Pos
				:	VECTOR	*	Dir Vector
				:	VECTOR	*	Up Vector
				:	float		ActiveDelay
				:	float		InactiveDelay
				:	int32_t		Colour
				:	int16_t		Primary
				:	int16_t		Secondary
				:	int16_t		SoundFX
				:	float		Volume;
				:	float		Speed;
				:	float		SFXType;
	Output		:	SPOTFX	*	SpotFX ( NULL if not available )
===================================================================*/
SPOTFX * InitOneSpotFX( u_int16_t GenType, float GenDelay, u_int16_t Type, u_int16_t Group,
					    VECTOR * Pos, VECTOR * Dir, VECTOR * Up, float ActiveDelay, float InactiveDelay,
						int32_t Colour, int16_t Primary, int16_t Secondary, int16_t SoundFX, float Volume, float Speed, int16_t SFXType )
{
	SPOTFX	*	Object;
	u_int8_t		Red;
	u_int8_t		Green;
	u_int8_t		Blue;
	VECTOR		Int_Point;
	NORMAL		Int_Normal;
	u_int16_t		Int_Group;
	VECTOR		TempVector;
	VECTOR		RayVector;

	Red = (u_int8_t) ( ( Colour >> 16 ) & 255 );
	Green = (u_int8_t) ( ( Colour >> 8 ) & 255 );
	Blue = (u_int8_t) ( Colour & 255 );

	Object = FindFreeSpotFX();

	if( Object )
	{
		switch( GenType )
		{
			case SPOTFX_GENTYPE_Initialised:
				Object->Flags = SPOTFX_FLAGS_Enabled;
				Object->State = SPOTFX_STATE_Active;
				Object->Time = 0.0F;
				Object->Delay = ActiveDelay;
				break;

			case SPOTFX_GENTYPE_Time:
				Object->Flags = SPOTFX_FLAGS_Enabled;
				Object->State = SPOTFX_STATE_WaitingForDelay;
				Object->Delay = GenDelay;
				break;

			case SPOTFX_GENTYPE_Trigger:
				Object->Flags = SPOTFX_FLAGS_Enabled;
				Object->State = SPOTFX_STATE_WaitingForTrigger;
				Object->Delay = 0.0F;
				break;
		}

		Object->Type = Type;
		Object->Group = Group;
		Object->Pos = *Pos;
		Object->DirVector = *Dir;
		Object->UpVector = *Up;
		Object->ActiveDelay = ActiveDelay;
		Object->InactiveDelay = InactiveDelay;
		Object->Red = Red;
		Object->Green = Green;
		Object->Blue = Blue;
		Object->Primary = (int8_t) Primary;
		Object->Secondary = (int8_t) Secondary;
		Object->SoundFX = SoundFX;
		Object->SoundFXVolume = Volume;
		Object->SoundFXSpeed = Speed;
		Object->SoundFXType = SFXType;
		Object->GenDelay = GenDelay;
		Object->MaxHeight = ( 2.0F * BLOCKSIZE );
		Object->SoundFX_ID = 0;

		AddSpotFXToGroup( Object, Group );

		RayVector.x = ( SlideUp.x * MaxColDistance );
		RayVector.y = ( SlideUp.y * MaxColDistance );
		RayVector.z = ( SlideUp.z * MaxColDistance );

		if( BackgroundCollide( &MCloadheadert0, &Mloadheader, Pos, Group, &RayVector, &Int_Point, &Int_Group,
								&Int_Normal, &TempVector, false, NULL ) )
		{
			RayVector.x = ( Int_Point.x - Pos->x );
			RayVector.y = ( Int_Point.y - Pos->y );
			RayVector.z = ( Int_Point.z - Pos->z );
			Object->MaxHeight = VectorLength( &RayVector );
		}
	}

	return( Object );
}

/*===================================================================
	Procedure	:	Enable SpotFX
	Input		:	u_int16_t	SpotFXIndex
	Output		:	Nothing
===================================================================*/
void EnableSpotFX( u_int16_t SpotFXIndex )
{
	if( SpotFX[ SpotFXIndex ].GenDelay == 0.0F )
	{
		SpotFX[ SpotFXIndex ].Delay = SpotFX[ SpotFXIndex ].ActiveDelay;
		SpotFX[ SpotFXIndex ].State = SPOTFX_STATE_Active;
	}
	else
	{
		SpotFX[ SpotFXIndex ].Delay = SpotFX[ SpotFXIndex ].GenDelay;
		SpotFX[ SpotFXIndex ].State = SPOTFX_STATE_WaitingForDelay;
	}
	SpotFX[ SpotFXIndex ].Time = 0.0F;
	SpotFX[ SpotFXIndex ].Flags &= ~SPOTFX_FLAGS_DoneOnce;
}

/*===================================================================
	Procedure	:	Disable SpotFX
	Input		:	u_int16_t	SpotFXIndex
	Output		:	Nothing
===================================================================*/
void DisableSpotFX( u_int16_t SpotFXIndex )
{
	SpotFX[ SpotFXIndex ].Delay = 0.0F;
	SpotFX[ SpotFXIndex ].State = SPOTFX_STATE_WaitingForTrigger;

	if( SpotFX[ SpotFXIndex ].SoundFX_ID )
	{
		StopSfx( SpotFX[ SpotFXIndex ].SoundFX_ID );
		SpotFX[ SpotFXIndex ].SoundFX_ID = 0;
	}
}

/*===================================================================
	Procedure	:	Find free SpotFX
	Input		:	Nothing
	Output		:	SPOTFX	*	SpotFX ( NULL if not available )
===================================================================*/
SPOTFX * FindFreeSpotFX( void )
{
	SPOTFX * Object;

	Object = FirstSpotFXFree;

	if( Object != NULL )
	{
		FirstSpotFXFree = Object->NextFree;
		if( FirstSpotFXFree != NULL )
		{
			FirstSpotFXFree->PrevFree = NULL;
		}

		if( FirstSpotFXUsed != NULL )
		{
			FirstSpotFXUsed->PrevUsed = Object;
			Object->NextUsed = FirstSpotFXUsed;
		}
		else
		{
			Object->NextUsed = NULL;
		}

		Object->PrevUsed = NULL;
		FirstSpotFXUsed = Object;
	}

	return( Object );
}

/*===================================================================
	Procedure	:	Kill an SpotFX
	Input		:	SPOTFX	*	Object
	Output		:	nothing
===================================================================*/
void KillUsedSpotFX( SPOTFX * Object )
{
	SPOTFX	*	PrevObject;
	SPOTFX	*	NextObject;

	if( Object != NULL )
	{
		RemoveSpotFXFromGroup( Object, Object->Group );

		if( Object == FirstSpotFXUsed )
		{
			NextObject = FirstSpotFXUsed->NextUsed;

			FirstSpotFXUsed = NextObject;

			if( FirstSpotFXUsed != NULL )
			{
				FirstSpotFXUsed->PrevUsed = NULL;
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

		if( FirstSpotFXFree != NULL )
		{
			FirstSpotFXFree->PrevFree = Object;
		}

		Object->NextFree = FirstSpotFXFree;
		Object->PrevFree = NULL;
		FirstSpotFXFree = Object;
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Steam
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateSpotFXSteam( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_STEAM;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].AnimSpeed = 8.0F;
		FmPolys[ fmpoly ].R = Red;
		FmPolys[ fmpoly ].G = Green;
		FmPolys[ fmpoly ].B = Blue;
		FmPolys[ fmpoly ].Start_R = Red;
		FmPolys[ fmpoly ].Start_G = Green;
		FmPolys[ fmpoly ].Start_B = Blue;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &NewTrail_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = ( 15.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 15.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( 25.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].UpSpeed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Flame
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSpotFXFlame( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_FLAME;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
		FmPolys[ fmpoly ].Start_R = 255;
		FmPolys[ fmpoly ].Start_G = 255;
		FmPolys[ fmpoly ].Start_B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( 16.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].UpSpeed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Flame Smoke
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSpotFXFlameSmoke( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_SMOKE;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].AnimSpeed = 12.0F;
		FmPolys[ fmpoly ].R = 64;
		FmPolys[ fmpoly ].G = 64;
		FmPolys[ fmpoly ].B = 64;
		FmPolys[ fmpoly ].Start_R = 64;
		FmPolys[ fmpoly ].Start_G = 64;
		FmPolys[ fmpoly ].Start_B = 64;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 128;
#endif
		FmPolys[ fmpoly ].Frm_Info = &NewTrail_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = ( 8.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 8.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Dir = *Dir;
   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 10240.0F ) - 0.25F );
  		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 10240.0F ) - 0.25F );
   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 10240.0F ) - 0.25F );
		NormaliseVector( &FmPolys[ fmpoly ].Dir );
		FmPolys[ fmpoly ].Speed = ( 10.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].UpSpeed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Smoke
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateSpotFXSmoke( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_SMOKE;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].AnimSpeed = 25.0F;
		FmPolys[ fmpoly ].R = Red;
		FmPolys[ fmpoly ].G = Green;
		FmPolys[ fmpoly ].B = Blue;
		FmPolys[ fmpoly ].Start_R = Red;
		FmPolys[ fmpoly ].Start_G = Green;
		FmPolys[ fmpoly ].Start_B = Blue;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 128;
#endif
		FmPolys[ fmpoly ].Frm_Info = &NewTrail_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = ( 20.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 20.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Dir = *Dir;
   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
  		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		NormaliseVector( &FmPolys[ fmpoly ].Dir );
		FmPolys[ fmpoly ].Speed = ( 10.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].UpSpeed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Sparks
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateSpotFXSparks( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;
	u_int16_t	light;
	int16_t	NumSparks;
	int16_t	Count;

	if( !Random_Range( 8 ) )
	{
		if( !Random_Range( 3 ) )
		{
			PlayFixedSpotSfx( SFX_ElectricSpark, Group, Pos, 0.0F, 0.25F, SPOT_SFX_TYPE_Normal );
		}

		light = FindFreeXLight();

		if( light != (u_int16_t ) -1 )
		{
			XLights[ light ].Visible = true;
			XLights[ light ].Pos = *Pos;
			XLights[ light ].Size = ( 1536.0F * GLOBAL_SCALE );
			XLights[ light ].SizeCount = ( 153.6F * GLOBAL_SCALE );
			XLights[ light ].ColorCount = 25.0F;
			XLights[ light ].r = (float) Red;
			XLights[ light ].g = (float) Green;
			XLights[ light ].b = (float) Blue;
			XLights[ light ].Group = Group;
		}

		fmpoly = FindFreeFmPoly();					// Faceme polygon attached
	
		if( fmpoly != (u_int16_t ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = 4.0F;
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_SPARKSFACEME;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
			FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
#if ACTUAL_TRANS
		   	FmPolys[ fmpoly ].Trans = 128;
#else
		   	FmPolys[ fmpoly ].Trans = 255;
#endif
		   	FmPolys[ fmpoly ].R = Red;
		   	FmPolys[ fmpoly ].G = Green;
		   	FmPolys[ fmpoly ].B = Blue;
			FmPolys[ fmpoly ].Start_R = Red;
			FmPolys[ fmpoly ].Start_G = Green;
			FmPolys[ fmpoly ].Start_B = Blue;
			FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	
		NumSparks = ( Random_Range( 10 ) + 10 );
		
		for( Count = 0; Count < NumSparks; Count++ )
		{
		   	fmpoly = FindFreeFmPoly();
		
		   	if( fmpoly != (u_int16_t ) -1 )
		   	{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].Dir = *Dir;
		   		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_SPARKS;
				FmPolys[ fmpoly ].AnimSpeed = 6.0F;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
				FmPolys[ fmpoly ].Trans = 255;
		   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
				FmPolys[ fmpoly ].R = Red;
				FmPolys[ fmpoly ].G = Green;
				FmPolys[ fmpoly ].B = Blue;
				FmPolys[ fmpoly ].Start_R = Red;
				FmPolys[ fmpoly ].Start_G = Green;
				FmPolys[ fmpoly ].Start_B = Blue;
				FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].xsize = ( 8.0F * GLOBAL_SCALE );
				FmPolys[ fmpoly ].ysize = ( 8.0F * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Group = Group;
				NormaliseVector( &FmPolys[ fmpoly ].Dir );
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		   	}
			else
			{
				return;
			}
		}
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSpotFXExplosion( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	u_int16_t	fmpoly;
	int16_t	Count;
	u_int16_t	light;
	float	Scale;

	PlayPannedSfx( SFX_BangBang, Group , Pos, 0.0F );

	light = FindFreeXLight();

	if( light != (u_int16_t ) -1 )
	{
		XLights[ light ].Visible = true;
		XLights[ light ].Pos = *Pos;
		XLights[ light ].Size = ( 1536.0F * GLOBAL_SCALE );
		XLights[ light ].SizeCount = ( 153.6F * GLOBAL_SCALE );
		XLights[ light ].ColorCount = 25.0F;
		XLights[ light ].r = 255.0F;
		XLights[ light ].g = 64.0F;
		XLights[ light ].b = 64.0F;
		XLights[ light ].Group = Group;
	}

	for( Count = 0; Count < ( Random_Range( 3 ) + 1 ); Count++ )
	{
		fmpoly = FindFreeFmPoly();

   		if( fmpoly != (u_int16_t ) -1 )
   		{
   			FmPolys[ fmpoly ].LifeCount = 1000.0F;
   			FmPolys[ fmpoly ].Pos = *Pos;
   			FmPolys[ fmpoly ].SeqNum = FM_ANIM;
   			FmPolys[ fmpoly ].Frame = 0.0F;
   			FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
   			FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
   			FmPolys[ fmpoly ].R = 192;
   			FmPolys[ fmpoly ].G = 192;
   			FmPolys[ fmpoly ].B = 192;
			FmPolys[ fmpoly ].Start_R = 192;
			FmPolys[ fmpoly ].Start_G = 192;
			FmPolys[ fmpoly ].Start_B = 192;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
   			FmPolys[ fmpoly ].Dir = *Dir;
   			FmPolys[ fmpoly ].Speed = 0.0F;
   			FmPolys[ fmpoly ].Group = Group;
   
   			if( Count == 0 )
   			{
   				FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
   				FmPolys[ fmpoly ].xsize = NORM_EXPLO_SIZE;
   				FmPolys[ fmpoly ].ysize = NORM_EXPLO_SIZE;
   			}
   			else
   			{
   				FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
   				FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
   				FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
   				FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
   				Scale = (float) ( Random_Range( 10 ) + 6 ) * GLOBAL_SCALE;
   				FmPolys[ fmpoly ].xsize = Scale;
   				FmPolys[ fmpoly ].ysize = Scale;
   			}
   			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
   		}
   		else
   		{
   			return;
   		}
   	}
}

/*===================================================================
	Procedure	:	Create SpotFX Fire Secondary
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	VECTOR	*	Up
				:	u_int16_t		Group
				:	u_int16_t		Weapon
				:	u_int16_t		OwnerType
				:	u_int16_t		OwnerID
	Output		:	Nothing
===================================================================*/
void CreateSpotFXFireSecondary( VECTOR * Pos, VECTOR * Dir, VECTOR * Up,
							    u_int16_t Group, u_int8_t Weapon, u_int16_t OwnerType, u_int16_t OwnerID )
{
	VECTOR	TempVector = { 0.0F, 0.0F, 0.0F };
	u_int16_t	BulletID = 0;

	BulletID = GetSecondaryBulletID( OwnerType, OwnerID );

	if( Weapon != (u_int8_t) -1 )
	{
		InitOneSecBull( OwnerType, OwnerID, BulletID, Group, Pos, &TempVector, Dir, Up, &TempVector, Weapon, false );
	}

}

/*===================================================================
	Procedure	:	Create SpotFX Fire Primary
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	VECTOR	*	Up
				:	u_int16_t		Group
				:	u_int16_t		Weapon
				:	u_int16_t		OwnerType
				:	u_int16_t		OwnerID
	Output		:	Nothing
===================================================================*/
void CreateSpotFXFirePrimary( VECTOR * Pos, VECTOR * Dir, VECTOR * Up,
							    u_int16_t Group, u_int8_t Weapon, u_int16_t OwnerType, u_int16_t OwnerID )
{
	VECTOR	TempVector = { 0.0F, 0.0F, 0.0F };

	if( Weapon != (u_int8_t) -1 )
	{
		EnemyFirePrimary( OWNER_NOBODY, WhoIAm, GlobalSecBullsID++, Weapon, Group, Pos, &TempVector, Dir, Up, 2, 0.0F, false, NULL );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Electric Beams
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	VECTOR	*	Up
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSpotFXElectricBeams( VECTOR * Pos, VECTOR * Dir, VECTOR * Up, u_int16_t Group )
{
	VECTOR	TempVector = { 0.0F, 0.0F, 0.0F };
	VECTOR	TempDir;
	int16_t	Count;
	int16_t	NumBeams;

	NumBeams = Random_Range( 2 ) + 1;

	for( Count = 0; Count < NumBeams; Count++ )
	{
		TempDir.x = Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		TempDir.y = Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		TempDir.z = Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		NormaliseVector( &TempDir );

		InitOnePrimBull( OWNER_NOBODY, WhoIAm, GlobalPrimBullsID++, NME_LIGHTNING, Group, Pos, &TempVector, &TempDir, Up, 2, 0.0F, false );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Smoke Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSpotFXGravgonTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	u_int16_t	fmpoly;
	QUAT	TempQuat;
	MATRIX	TempMat;
	VECTOR	TempUp;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_GRAVGON_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 255;
		FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &GravgonTrail_Header;
   		FmPolys[ fmpoly ].Flags = FM_FLAG_DIRCONST;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].RotSpeed = 0.0F;
		FmPolys[ fmpoly ].xsize = ( 10.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 10.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( 15.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Group = Group;
		QuatFrom2Vectors( &TempQuat, &Forward, Dir );
	   	QuatToMatrix( &TempQuat, &TempMat );
	   	ApplyMatrix( &TempMat, &SlideUp, &TempUp );
	  	FmPolys[ fmpoly ].DirVector = *Dir;
	  	FmPolys[ fmpoly ].UpVector = TempUp;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create NME Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateSpotFXNmeTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 20.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_NME_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = Red;
		FmPolys[ fmpoly ].G = Green;
		FmPolys[ fmpoly ].B = Blue;
		FmPolys[ fmpoly ].Trans = 255;
		FmPolys[ fmpoly ].Frm_Info = &Circle_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = ( 7.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 7.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( 20.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create NME Glow
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateSpotFXNmeGlow( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	FmPoly;

	FmPoly = FindFreeFmPoly();

	if( FmPoly != (u_int16_t) -1 )
	{
		FmPolys[ FmPoly ].LifeCount = 1000.0F;
  		FmPolys[ FmPoly ].Pos = *Pos;
  		FmPolys[ FmPoly ].OnceOnly = 1;
		FmPolys[ FmPoly ].SeqNum = FM_SPOTFX_GLOW;
		FmPolys[ FmPoly ].Frame = 0.0F;
		FmPolys[ FmPoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ FmPoly ].xsize = ( 10.0F * GLOBAL_SCALE );
		FmPolys[ FmPoly ].ysize = ( 10.0F * GLOBAL_SCALE );
		FmPolys[ FmPoly ].Frm_Info = &Flare_Header;
		FmPolys[ FmPoly ].Group = Group;
		FmPolys[ FmPoly ].R = Red;
		FmPolys[ FmPoly ].G = Green;
		FmPolys[ FmPoly ].B = Blue;
	   	FmPolys[ FmPoly ].Trans = 255;
		AddFmPolyToTPage( FmPoly, GetTPage( *FmPolys[ FmPoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create NME Vapour Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateSpotFXNmeVapourTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_NME_VAPOUR_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = Red;
		FmPolys[ fmpoly ].G = Green;
		FmPolys[ fmpoly ].B = Blue;
		FmPolys[ fmpoly ].Start_R = Red;
		FmPolys[ fmpoly ].Start_G = Green;
		FmPolys[ fmpoly ].Start_B = Blue;
		FmPolys[ fmpoly ].Trans = 255;
		FmPolys[ fmpoly ].Frm_Info = &NewTrail_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = ( 5.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 5.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( 0.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Real Sparks ( Gravity )
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateSpotFXRealSparks( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;
	u_int16_t	light;
	int16_t	NumSparks;
	int16_t	Count;

	if( !Random_Range( 8 ) )
	{
		if( !Random_Range( 3 ) )
		{
			PlayFixedSpotSfx( SFX_ElectricSpark, Group, Pos, 0.0F, 0.25F, SPOT_SFX_TYPE_Normal );
		}

		light = FindFreeXLight();

		if( light != (u_int16_t ) -1 )
		{
			XLights[ light ].Visible = true;
			XLights[ light ].Pos = *Pos;
			XLights[ light ].Size = ( 1536.0F * GLOBAL_SCALE );
			XLights[ light ].SizeCount = ( 153.6F * GLOBAL_SCALE );
			XLights[ light ].ColorCount = 25.0F;
			XLights[ light ].r = (float) Red;
			XLights[ light ].g = (float) Green;
			XLights[ light ].b = (float) Blue;
			XLights[ light ].Group = Group;
		}

		fmpoly = FindFreeFmPoly();					// Faceme polygon attached
	
		if( fmpoly != (u_int16_t ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = 4.0F;
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_SPARKSFACEME;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
			FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
#if ACTUAL_TRANS
		   	FmPolys[ fmpoly ].Trans = 128;
#else
		   	FmPolys[ fmpoly ].Trans = 255;
#endif
		   	FmPolys[ fmpoly ].R = Red;
		   	FmPolys[ fmpoly ].G = Green;
		   	FmPolys[ fmpoly ].B = Blue;
			FmPolys[ fmpoly ].Start_R = Red;
			FmPolys[ fmpoly ].Start_G = Green;
			FmPolys[ fmpoly ].Start_B = Blue;
			FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	
		NumSparks = ( Random_Range( 5 ) + 5 );
		
		for( Count = 0; Count < NumSparks; Count++ )
		{
		   	fmpoly = FindFreeFmPoly();
		
		   	if( fmpoly != (u_int16_t ) -1 )
		   	{
				FmPolys[ fmpoly ].LifeCount = ( (float) Random_Range( 180 ) ) + 60.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].Dir = *Dir;
		   		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_REALSPARKS;
		   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
		   		FmPolys[ fmpoly ].Frame = (float) Random_Range( (*FmPolys[ fmpoly ].Frm_Info)->Num_Frames );
				FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
				FmPolys[ fmpoly ].Trans = 255;
		   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
				NormaliseVector( &FmPolys[ fmpoly ].Dir );
				FmPolys[ fmpoly ].Dir.x *= ( ( (float) Random_Range( 10000 ) ) / 2000.0F ) + 3.0F;
				FmPolys[ fmpoly ].Dir.y *= ( ( (float) Random_Range( 10000 ) ) / 2000.0F ) + 3.0F;
				FmPolys[ fmpoly ].Dir.z *= ( ( (float) Random_Range( 10000 ) ) / 2000.0F ) + 3.0F;
				FmPolys[ fmpoly ].R = Red;
				FmPolys[ fmpoly ].G = Green;
				FmPolys[ fmpoly ].B = Blue;
				FmPolys[ fmpoly ].Start_R = Red;
				FmPolys[ fmpoly ].Start_G = Green;
				FmPolys[ fmpoly ].Start_B = Blue;
				FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].xsize = ( 8.0F * GLOBAL_SCALE );
				FmPolys[ fmpoly ].ysize = ( 8.0F * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Group = Group;
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		   	}
			else
			{
				return;
			}
		}
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Rock Shrapnel Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	float		Speed
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSpotFXRockShrapnel( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	int16_t	NumBits;
	int16_t	Count;
	float	Speed;
	VECTOR	DirVector;

	CreateSpotFXExplosion( Pos, Dir, Group );

	NumBits = ( Random_Range( 4 ) + 3 );
		
	for( Count = 0; Count < NumBits; Count++ )
	{
		Speed = ( ( (float) Random_Range( 10000 ) ) / 1500.0F ) + 2.0F;
		DirVector.x = Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		DirVector.y = Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		DirVector.z = Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		NormaliseVector( &DirVector );

		CreateSmallShrapnelBit( Pos, &DirVector, Speed, Group, (u_int16_t) ( MODEL_Rock + Random_Range( 2 ) ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Shrapnel Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	float		Speed
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSpotFXShrapnel( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	int16_t	NumBits;
	int16_t	Count;
	float	Speed;
	VECTOR	DirVector;

	CreateSpotFXExplosion( Pos, Dir, Group );

	NumBits = ( Random_Range( 4 ) + 3 );
		
	for( Count = 0; Count < NumBits; Count++ )
	{
		Speed = ( ( (float) Random_Range( 10000 ) ) / 1500.0F ) + 2.0F;
		DirVector.x = Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		DirVector.y = Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		DirVector.z = Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		NormaliseVector( &DirVector );

		if( Random_Range( 3 ) )
		{
			CreateLargeShrapnelBit( Pos, &DirVector, Speed, Group, MODEL_Shrapnel );
		}
		else
		{
			CreateSmallShrapnelBit( Pos, &DirVector, Speed, Group, MODEL_Shrapnel2 );
		}
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Shrapnel Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	float		Speed
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSpotFXSmallShrapnel( VECTOR * Pos, VECTOR * Dir, float Speed, u_int16_t Group )
{
	int16_t	NumBits;
	int16_t	Count;
	VECTOR	DirVector;

	NumBits = ( Random_Range( 3 ) + 1 );
		
	for( Count = 0; Count < NumBits; Count++ )
	{
		DirVector.x = Dir->x + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		DirVector.y = Dir->y + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		DirVector.z = Dir->z + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		NormaliseVector( &DirVector );
		CreateSmallShrapnelBit( Pos, &DirVector, Speed, Group, MODEL_Shrapnel2 );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Bubbles
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
				:	float		MaxHeight
	Output		:	Nothing
===================================================================*/
void CreateSpotFXBubbles( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue, float MaxHeight )
{
	u_int16_t	fmpoly;
	int16_t	NumBubbles;

	NumBubbles = ( Random_Range( 2 ) + 1 );

	while( NumBubbles-- )
	{
		fmpoly = FindFreeFmPoly();

		if( fmpoly != (u_int16_t ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = (float) Random_Range( 4 * 60 );

			if( FmPolys[ fmpoly ].LifeCount > ( MaxHeight / 4.0F ) )
			{
				FmPolys[ fmpoly ].LifeCount = ( MaxHeight / 4.0F );
			}

			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_BUBBLE;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].AnimSpeed = 1.0F;
			FmPolys[ fmpoly ].R = Red;
			FmPolys[ fmpoly ].G = Green;
			FmPolys[ fmpoly ].B = Blue;
			FmPolys[ fmpoly ].Start_R = Red;
			FmPolys[ fmpoly ].Start_G = Green;
			FmPolys[ fmpoly ].Start_B = Blue;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
			FmPolys[ fmpoly ].Frm_Info = &Bubble_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = (float) ( Random_Range( 100 ) / 50.0F ) + 0.25F;
			FmPolys[ fmpoly ].ysize = FmPolys[ fmpoly ].xsize;
   			FmPolys[ fmpoly ].Dir.x = ( Dir->x * 2.0F ) + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
   			FmPolys[ fmpoly ].Dir.y = ( Dir->y * 2.0F ) + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
   			FmPolys[ fmpoly ].Dir.z = ( Dir->z * 2.0F ) + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
			NormaliseVector( &FmPolys[ fmpoly ].Dir );
			FmPolys[ fmpoly ].Speed = ( ( Random_Range( 10000 ) / 10000.0F ) * 4.0F ) + 1.0F;
			FmPolys[ fmpoly ].UpSpeed = 0.0F;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}
}

/*===================================================================
	Procedure	:	Create Bubble Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateBubbleTrail( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = (float) Random_Range( 3 * 60 );
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_BUBBLE;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].AnimSpeed = 1.0F;
		FmPolys[ fmpoly ].R = Red;
		FmPolys[ fmpoly ].G = Green;
		FmPolys[ fmpoly ].B = Blue;
		FmPolys[ fmpoly ].Start_R = Red;
		FmPolys[ fmpoly ].Start_G = Green;
		FmPolys[ fmpoly ].Start_B = Blue;
#if ACTUAL_TRANS
   		FmPolys[ fmpoly ].Trans = 128;
#else
   		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &Bubble_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = (float) ( Random_Range( 100 ) / 75.0F ) + 0.25F;
		FmPolys[ fmpoly ].ysize = FmPolys[ fmpoly ].xsize;
   		FmPolys[ fmpoly ].Dir.x = ( Dir->x * 3.0F ) + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
   		FmPolys[ fmpoly ].Dir.y = ( Dir->y * 3.0F ) + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
   		FmPolys[ fmpoly ].Dir.z = ( Dir->z * 3.0F ) + ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		NormaliseVector( &FmPolys[ fmpoly ].Dir );
		FmPolys[ fmpoly ].Speed = ( ( Random_Range( 10000 ) / 10000.0F ) * 4.0F ) + 1.0F;
		FmPolys[ fmpoly ].UpSpeed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Drip
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateSpotFXDrip( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
 	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_DRIP;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].AnimSpeed = 1.0F;
		FmPolys[ fmpoly ].R = Red;
		FmPolys[ fmpoly ].G = Green;
		FmPolys[ fmpoly ].B = Blue;
		FmPolys[ fmpoly ].Start_R = Red;
		FmPolys[ fmpoly ].Start_G = Green;
		FmPolys[ fmpoly ].Start_B = Blue;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &Drip_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = ( 2.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 4.0F * GLOBAL_SCALE );
   		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = 0.0F;
		FmPolys[ fmpoly ].UpSpeed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Drip Splash
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateSpotFXDripSplash( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;
	int16_t	NumSparks;
	int16_t	Count;

	PlayFixedSpotSfx( SFX_WaterDrip, Group, Pos, 0.0F, 0.75F, SPOT_SFX_TYPE_Normal );

	NumSparks = ( Random_Range( 5 ) + 5 );

	for( Count = 0; Count < NumSparks; Count++ )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (u_int16_t ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   	FmPolys[ fmpoly ].Pos = *Pos;
		   	FmPolys[ fmpoly ].Dir = *Dir;
		   	FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_SPARKS;
			FmPolys[ fmpoly ].AnimSpeed = 6.0F;
		   	FmPolys[ fmpoly ].Frame = 0.0F;
		   	FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Trans = 255;
		   	FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   	FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   	FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
			FmPolys[ fmpoly ].R = Red;
			FmPolys[ fmpoly ].G = Green;
			FmPolys[ fmpoly ].B = Blue;
			FmPolys[ fmpoly ].Start_R = Red;
			FmPolys[ fmpoly ].Start_G = Green;
			FmPolys[ fmpoly ].Start_B = Blue;
			FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 1.0F ) * GLOBAL_SCALE;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Group = Group;
			NormaliseVector( &FmPolys[ fmpoly ].Dir );
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
		else
		{
			return;
		}
	}
}


/*===================================================================
	Procedure	:	Create Large Shrapnel Bit
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	float		Speed
				:	u_int16_t		Group
				:	u_int16_t		Model
	Output		:	Nothing
===================================================================*/
void CreateLargeShrapnelBit( VECTOR * Pos, VECTOR * Dir, float Speed, int16_t Group, u_int16_t ModelNum )
{
	u_int16_t	Model;

	Model =	FindFreeModel();

	if( Model != (u_int16_t ) -1 )
	{
		Models[ Model ].OwnerType = OWNER_NOBODY;
		Models[ Model ].Owner = 0;
		Models[ Model ].ModelNum = ModelNum;
		Models[ Model ].Type = MODTYPE_Missile;
		Models[ Model ].Flags = MODFLAG_Clip;
		Models[ Model ].Visible = true;
		Models[ Model ].Pos = *Pos;
		Models[ Model ].Dir = *Dir;
		Models[ Model ].Dir.x *= Speed;
		Models[ Model ].Dir.y *= Speed;
		Models[ Model ].Dir.z *= Speed;
		Models[ Model ].Rot.x = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
		Models[ Model ].Rot.y = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
		Models[ Model ].Rot.z = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
		Models[ Model ].Quat.w = 1.0F;
		Models[ Model ].Quat.x = 0.0F;
		Models[ Model ].Quat.y = 0.0F;
		Models[ Model ].Quat.z = 0.0F;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_LargeShrapnel;
		Models[ Model ].Scale = 1.0F;
		Models[ Model ].MaxScale = 1.0F;
		Models[ Model ].Group = Group;
		Models[ Model ].LifeCount = ( (float) Random_Range( 300 ) ) + 300.0F;
	}
}

/*===================================================================
	Procedure	:	Create Small Shrapnel Bit
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	float		Speed
				:	u_int16_t		Group
				:	u_int16_t		Model
	Output		:	Nothing
===================================================================*/
void CreateSmallShrapnelBit( VECTOR * Pos, VECTOR * Dir, float Speed, int16_t Group, u_int16_t ModelNum )
{
	u_int16_t	Model;

	Model =	FindFreeModel();

	if( Model != (u_int16_t ) -1 )
	{
		Models[ Model ].OwnerType = OWNER_NOBODY;
		Models[ Model ].Owner = 0;
		Models[ Model ].ModelNum = ModelNum;
		Models[ Model ].Type = MODTYPE_Missile;
		Models[ Model ].Flags = MODFLAG_Clip;
		Models[ Model ].Visible = true;
		Models[ Model ].Pos = *Pos;
		Models[ Model ].Dir = *Dir;
		Models[ Model ].Dir.x *= Speed;
		Models[ Model ].Dir.y *= Speed;
		Models[ Model ].Dir.z *= Speed;
		Models[ Model ].Rot.x = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
		Models[ Model ].Rot.y = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
		Models[ Model ].Rot.z = ( ( (float) Random_Range( 10000 ) ) - 5000.0F ) / 1000.0F;
		Models[ Model ].Quat.w = 1.0F;
		Models[ Model ].Quat.x = 0.0F;
		Models[ Model ].Quat.y = 0.0F;
		Models[ Model ].Quat.z = 0.0F;
		Models[ Model ].Mat = MATRIX_Identity;
		MatrixTranspose( &Models[ Model ].Mat, &Models[ Model ].InvMat );
		Models[ Model ].Func = MODFUNC_SmallShrapnel;
		Models[ Model ].Scale = 1.0F;
		Models[ Model ].MaxScale = 1.0F;
		Models[ Model ].Group = Group;
		Models[ Model ].LifeCount = ( (float) Random_Range( 300 ) ) + 300.0F;
	}
}

/*===================================================================
	Procedure	:	Create Shrapnel Trail
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	float		Size
				:	u_int16_t		Group
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateShrapnelTrail( VECTOR * Pos, VECTOR * Dir, float Size, u_int16_t Group, u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_SHRAPNEL_TRAIL;
		FmPolys[ fmpoly ].AnimSpeed = 8.0F;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = Red;
		FmPolys[ fmpoly ].G = Green;
		FmPolys[ fmpoly ].B = Blue;
		FmPolys[ fmpoly ].Start_R = Red;
		FmPolys[ fmpoly ].Start_G = Green;
		FmPolys[ fmpoly ].Start_B = Blue;
		FmPolys[ fmpoly ].Trans = 255;
		FmPolys[ fmpoly ].Frm_Info = &NewTrail_Header;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = Size;
		FmPolys[ fmpoly ].ysize = Size;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( 5.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].UpSpeed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create Enemy Generation Effect
	Input		:	VECTOR	*	Position
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateNmeGenEffect( VECTOR * Pos, u_int16_t Group )
{
	u_int16_t	fmpoly;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = ( 2.0F * 60.0F );
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_NMEGEN_EFFECT;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].AnimSpeed = 1.0F;
		FmPolys[ fmpoly ].R = 0;
		FmPolys[ fmpoly ].G = 0;
		FmPolys[ fmpoly ].B = 0;
   		FmPolys[ fmpoly ].Trans = 255;
		FmPolys[ fmpoly ].Frm_Info = &EnemyGen_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].DirVector.x = +1.0F;
		FmPolys[ fmpoly ].DirVector.y = +1.0F;
		FmPolys[ fmpoly ].DirVector.z = -1.0F;
		FmPolys[ fmpoly ].UpVector.x = -1.0F;
		FmPolys[ fmpoly ].UpVector.y = +1.0F;
		FmPolys[ fmpoly ].UpVector.z = +1.0F;
		NormaliseVector( &FmPolys[ fmpoly ].DirVector );
		NormaliseVector( &FmPolys[ fmpoly ].UpVector );
   		FmPolys[ fmpoly ].Flags = FM_FLAG_RELATIVE;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = ( 16.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = FmPolys[ fmpoly ].xsize;
   		FmPolys[ fmpoly ].Dir.x = 0.0F;
   		FmPolys[ fmpoly ].Dir.y = 0.0F;
   		FmPolys[ fmpoly ].Dir.z = 0.0F;
		FmPolys[ fmpoly ].Speed = 0.0F;
		FmPolys[ fmpoly ].UpSpeed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );

		fmpoly = FindFreeFmPoly();

		if( fmpoly != (u_int16_t ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = ( 2.0F * 60.0F );
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].SeqNum = FM_NMEGEN_FLARE;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].AnimSpeed = 1.0F;
			FmPolys[ fmpoly ].R = 0;
			FmPolys[ fmpoly ].G = 0;
			FmPolys[ fmpoly ].B = 0;
   			FmPolys[ fmpoly ].Trans = 255;
			FmPolys[ fmpoly ].Frm_Info = &BigFlash_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = 0.1F;
			FmPolys[ fmpoly ].ysize = FmPolys[ fmpoly ].xsize;
   			FmPolys[ fmpoly ].Dir.x = 0.0F;
   			FmPolys[ fmpoly ].Dir.y = 0.0F;
   			FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].UpSpeed = 0.0F;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Burning
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSpotFXBurning( VECTOR * Pos, VECTOR * Dir, u_int16_t Group )
{
	int16_t	Count;
	u_int16_t	fmpoly;

	for( Count = 0; Count < 5; Count++ )
	{
		fmpoly = FindFreeFmPoly();

		if( fmpoly != (u_int16_t ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_BURNING;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].R = 64;
			FmPolys[ fmpoly ].G = 64;
			FmPolys[ fmpoly ].B = 64;
			FmPolys[ fmpoly ].Start_R = 64;
			FmPolys[ fmpoly ].Start_G = 64;
			FmPolys[ fmpoly ].Start_B = 64;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
			FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Dir = *Dir;
	   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 10000 ) ) / 7500.0F ) - 0.66F );
	   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 10000 ) ) / 7500.0F ) - 0.66F );
	   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 10000 ) ) / 7500.0F ) - 0.66F );
			NormaliseVector( &FmPolys[ fmpoly ].Dir );
			FmPolys[ fmpoly ].Speed = ( 24.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].UpSpeed = 0.0F;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}
	}

	fmpoly = FindFreeFmPoly();

   	if( fmpoly != (u_int16_t ) -1 )
   	{
   		FmPolys[ fmpoly ].LifeCount = 1000.0F;
   		FmPolys[ fmpoly ].Pos = *Pos;
   		FmPolys[ fmpoly ].SeqNum = FM_ANIMSPEED;
   		FmPolys[ fmpoly ].AnimSpeed = 1.0F;
   		FmPolys[ fmpoly ].Frame = 0.0F;
   		FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
   		FmPolys[ fmpoly ].R = 64;
   		FmPolys[ fmpoly ].G = 64;
   		FmPolys[ fmpoly ].B = 64;
		FmPolys[ fmpoly ].Start_R = 64;
		FmPolys[ fmpoly ].Start_G = 64;
		FmPolys[ fmpoly ].Start_B = 64;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
   		FmPolys[ fmpoly ].Dir = *Dir;
   		FmPolys[ fmpoly ].Speed = 0.0F;
   		FmPolys[ fmpoly ].Group = Group;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Beard Afterburner
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	float		Speed
	Output		:	Nothing
===================================================================*/
void CreateSpotFXBeardAfterburner( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, float Speed )
{
	u_int16_t	fmpoly;
	float	Contrast;

	if( Speed < 0.0F ) Speed = 0.0F;
	Contrast = ( Speed * 3.0F ) + 5.0F;
	if( Contrast > 255.0F ) Contrast = 255.0F;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_BEARD_AFTERBURNER;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].AnimSpeed = 1.0F;
		FmPolys[ fmpoly ].R = (u_int8_t) Contrast;
		FmPolys[ fmpoly ].G = (u_int8_t) ( Contrast * 0.5F );
		FmPolys[ fmpoly ].B = (u_int8_t) ( Contrast * 0.2F );
		FmPolys[ fmpoly ].Start_R = FmPolys[ fmpoly ].R;
		FmPolys[ fmpoly ].Start_G = FmPolys[ fmpoly ].G;
		FmPolys[ fmpoly ].Start_B = FmPolys[ fmpoly ].B;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
		FmPolys[ fmpoly ].xsize = ( 2.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 2.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( 16.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].UpSpeed = 0.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Borg Afterburner
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	float		Speed
	Output		:	Nothing
===================================================================*/
void CreateSpotFXBorgAfterburner( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, float Speed )
{
	u_int16_t	FmPoly;
	float	Size;

	if( Speed < 0.0F ) Speed = 0.0F;

	Size = ( Speed * 0.5F ) + 1.0F;
	if( Size > 5.0F ) Size = 5.0F;

	FmPoly = FindFreeFmPoly();

	if( FmPoly != (u_int16_t) -1 )
	{
		FmPolys[ FmPoly ].LifeCount = 1000.0F;
  		FmPolys[ FmPoly ].Pos = *Pos;
  		FmPolys[ FmPoly ].OnceOnly = 1;
		FmPolys[ FmPoly ].SeqNum = FM_SPOTFX_GLOW;
		FmPolys[ FmPoly ].Frame = 0.0F;
		FmPolys[ FmPoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ FmPoly ].xsize = Size; //( 10.0F * GLOBAL_SCALE );
		FmPolys[ FmPoly ].ysize = Size; //( 10.0F * GLOBAL_SCALE );
		FmPolys[ FmPoly ].Frm_Info = &Flare_Header;
		FmPolys[ FmPoly ].Group = Group;
		FmPolys[ FmPoly ].R = (u_int8_t) ( Size * 100.0F );
		FmPolys[ FmPoly ].G = (u_int8_t) ( Size * 100.0F );
		FmPolys[ FmPoly ].B = (u_int8_t) ( Size * 100.0F );
	   	FmPolys[ FmPoly ].Trans = 255;
		AddFmPolyToTPage( FmPoly, GetTPage( *FmPolys[ FmPoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX Small Afterburner
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	float		Speed
	Output		:	Nothing
===================================================================*/
void CreateSpotFXSmallAfterburner( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, float Speed )
{
	u_int16_t	FmPoly;
	float	Size;

	if( Speed < 0.0F ) Speed = 0.0F;

	Size = ( Speed * 0.25F ) + 0.5F;
	if( Size > 2.5F ) Size = 2.5F;

	FmPoly = FindFreeFmPoly();

	if( FmPoly != (u_int16_t) -1 )
	{
		FmPolys[ FmPoly ].LifeCount = 1000.0F;
  		FmPolys[ FmPoly ].Pos = *Pos;
  		FmPolys[ FmPoly ].OnceOnly = 1;
		FmPolys[ FmPoly ].SeqNum = FM_SPOTFX_GLOW;
		FmPolys[ FmPoly ].Frame = 0.0F;
		FmPolys[ FmPoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ FmPoly ].xsize = Size; //( 10.0F * GLOBAL_SCALE );
		FmPolys[ FmPoly ].ysize = Size; //( 10.0F * GLOBAL_SCALE );
		FmPolys[ FmPoly ].Frm_Info = &Flare_Header;
		FmPolys[ FmPoly ].Group = Group;
		FmPolys[ FmPoly ].R = (u_int8_t) ( Size * 100.0F );
		FmPolys[ FmPoly ].G = (u_int8_t) ( Size * 100.0F );
		FmPolys[ FmPoly ].B = (u_int8_t) ( Size * 100.0F );
	   	FmPolys[ FmPoly ].Trans = 255;
		AddFmPolyToTPage( FmPoly, GetTPage( *FmPolys[ FmPoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create HK5 Afterburner
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	u_int16_t		Group
				:	float		Speed
				:	u_int8_t		Red
				:	u_int8_t		Green
				:	u_int8_t		Blue
	Output		:	Nothing
===================================================================*/
void CreateSpotFXHk5Afterburner( VECTOR * Pos, VECTOR * Dir, u_int16_t Group, float Speed,
								 u_int8_t Red, u_int8_t Green, u_int8_t Blue )
{
	u_int16_t	fmpoly;
	float	Size;

	if( Speed < 0.0F ) Speed = 0.0F;

	Size = ( Speed * 0.05F ) + 0.5F;
	if( Size > 1.0F ) Size = 1.0F;

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (u_int16_t ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = ( Speed / 2.0F ) + 5.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_HK5_TRAIL;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].R = Red;
		FmPolys[ fmpoly ].G = Green;
		FmPolys[ fmpoly ].B = Blue;
		FmPolys[ fmpoly ].Trans = 255;
		FmPolys[ fmpoly ].Frm_Info = &Circle_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = Size;
		FmPolys[ fmpoly ].ysize = Size;
		FmPolys[ fmpoly ].Dir = *Dir;
		FmPolys[ fmpoly ].Speed = ( Speed / 8.0F ) + 2.0F;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*===================================================================
	Procedure	:	Create SpotFX FireWall
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	VECTOR	*	Up
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void CreateSpotFXFireWall( VECTOR * Pos, VECTOR * Dir, VECTOR * Up, u_int16_t Group )
{
	VECTOR	TempVector = { 0.0F, 0.0F, 0.0F };

	InitOnePrimBull( OWNER_NOBODY, 0, 0, FLAMES, Group, Pos, &TempVector, Dir, Up, 2, 0.0F, false );
}

/*===================================================================
	Procedure	:	Setup SpotFX group link list
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void SetupSpotFXGroups( void )
{
	int16_t	Count;

	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
		SpotFXGroups[ Count ] = NULL;
		NumSpotFXPerGroup[ Count ] = 0;
	}
}

/*===================================================================
	Procedure	:	Add SpotFX to group link list
	Input		:	SPOTFX	*	SpotFX
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void AddSpotFXToGroup( SPOTFX * SpotFX, u_int16_t Group )
{
	SpotFX->PrevInGroup = NULL;
	SpotFX->NextInGroup = SpotFXGroups[ Group ];
	if( SpotFX->NextInGroup ) SpotFX->NextInGroup->PrevInGroup = SpotFX;
	SpotFXGroups[ Group ] = SpotFX;
	NumSpotFXPerGroup[ Group ]++;
}

/*===================================================================
	Procedure	:	Remove SpotFX from group link list
	Input		:	SPOTFX	*	SpotFX
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void RemoveSpotFXFromGroup( SPOTFX * SpotFX, u_int16_t Group )
{
	if( SpotFX->PrevInGroup ) SpotFX->PrevInGroup->NextInGroup = SpotFX->NextInGroup;
	else SpotFXGroups[ Group ] = SpotFX->NextInGroup;
	if( SpotFX->NextInGroup ) SpotFX->NextInGroup->PrevInGroup = SpotFX->PrevInGroup;
	SpotFX->PrevInGroup = NULL;
	SpotFX->NextInGroup = NULL;
	NumSpotFXPerGroup[ Group ]--;
}

/*===================================================================
	Procedure	:	Move SpotFX from 1 group to another
	Input		:	SpotFX	*	SpotFX
				:	u_int16_t		OldGroup
				:	u_int16_t		NewGroup
	Output		:	Nothing
===================================================================*/
void MoveSpotFXToGroup( SPOTFX * SpotFX, u_int16_t OldGroup, u_int16_t NewGroup )
{
	RemoveSpotFXFromGroup( SpotFX, OldGroup );
	AddSpotFXToGroup( SpotFX, NewGroup );
}

/*===================================================================
	Procedure	:	Save SpotFX Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveAllSpotFX( FILE * fp )
{
	int		i;
	u_int16_t	TempIndex = (u_int16_t) -1; 

	if( fp )
	{
		fwrite( &NumSpotFX, sizeof( NumSpotFX ), 1, fp );

		for( i = 0; i < MAXGROUPS; i++ )
		{
			if( SpotFXGroups[ i ] != NULL ) fwrite( &SpotFXGroups[ i ]->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			fwrite( &NumSpotFXPerGroup[ i ], sizeof( u_int16_t ), 1, fp );
		}

		if( FirstSpotFXUsed != NULL ) fwrite( &FirstSpotFXUsed->Index, sizeof( u_int16_t ), 1, fp );
		else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
		if( FirstSpotFXFree != NULL ) fwrite( &FirstSpotFXFree->Index, sizeof( u_int16_t ), 1, fp );
		else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );

		for( i = 0; i < NumSpotFX; i++ )
		{
			fwrite( &SpotFX[ i ].Type, sizeof( u_int16_t ), 1, fp );
			fwrite( &SpotFX[ i ].Flags, sizeof( u_int16_t ), 1, fp );
			fwrite( &SpotFX[ i ].State, sizeof( u_int16_t ), 1, fp );
			fwrite( &SpotFX[ i ].Index, sizeof( u_int16_t ), 1, fp );
			fwrite( &SpotFX[ i ].Group, sizeof( u_int16_t ), 1, fp );
			fwrite( &SpotFX[ i ].Light, sizeof( u_int16_t ), 1, fp );
			fwrite( &SpotFX[ i ].SoundFX, sizeof( int16_t ), 1, fp );
			fwrite( &SpotFX[ i ].SoundFXVolume, sizeof( float ), 1, fp );
			fwrite( &SpotFX[ i ].SoundFXSpeed, sizeof( float ), 1, fp );
			fwrite( &SpotFX[ i ].SoundFXType, sizeof( int16_t ), 1, fp );
			fwrite( &SpotFX[ i ].SoundFX_ID, sizeof( u_int32_t ), 1, fp );
			fwrite( &SpotFX[ i ].Primary, sizeof( int8_t ), 1, fp );
			fwrite( &SpotFX[ i ].Secondary, sizeof( int8_t ), 1, fp );
			fwrite( &SpotFX[ i ].Red, sizeof( u_int8_t ), 1, fp );
			fwrite( &SpotFX[ i ].Green, sizeof( u_int8_t ), 1, fp );
			fwrite( &SpotFX[ i ].Blue, sizeof( u_int8_t ), 1, fp );
			fwrite( &SpotFX[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &SpotFX[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fwrite( &SpotFX[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fwrite( &SpotFX[ i ].Delay, sizeof( float ), 1, fp );
			fwrite( &SpotFX[ i ].ActiveDelay, sizeof( float ), 1, fp );
			fwrite( &SpotFX[ i ].InactiveDelay, sizeof( float ), 1, fp );
			fwrite( &SpotFX[ i ].Time, sizeof( float ), 1, fp );
			fwrite( &SpotFX[ i ].TimeInterval, sizeof( float ), 1, fp );
			fwrite( &SpotFX[ i ].GenDelay, sizeof( float ), 1, fp );
			fwrite( &SpotFX[ i ].MaxHeight, sizeof( float ), 1, fp );
			if( SpotFX[ i ].PrevUsed != NULL ) fwrite( &SpotFX[ i ].PrevUsed->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( SpotFX[ i ].NextUsed != NULL ) fwrite( &SpotFX[ i ].NextUsed->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( SpotFX[ i ].PrevFree != NULL ) fwrite( &SpotFX[ i ].PrevFree->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( SpotFX[ i ].NextFree != NULL ) fwrite( &SpotFX[ i ].NextFree->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( SpotFX[ i ].PrevInGroup != NULL ) fwrite( &SpotFX[ i ].PrevInGroup->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( SpotFX[ i ].NextInGroup != NULL ) fwrite( &SpotFX[ i ].NextInGroup->Index, sizeof( u_int16_t ), 1, fp );
			else fwrite( &TempIndex, sizeof( u_int16_t ), 1, fp );
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load SpotFX Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadAllSpotFX( FILE * fp )
{
	int		i;
	u_int16_t	TempIndex;
	int32_t	TempNumSpotFX;

	if( fp )
	{
		fread( &TempNumSpotFX, sizeof( TempNumSpotFX ), 1, fp );

		if( TempNumSpotFX != NumSpotFX )
		{
			fclose( fp );
			return( NULL );
		}

		for( i = 0; i < MAXGROUPS; i++ )
		{
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) SpotFXGroups[ i ] = &SpotFX[ TempIndex ];
			else SpotFXGroups[ i ] = NULL;
			fread( &NumSpotFXPerGroup[ i ], sizeof( u_int16_t ), 1, fp );
		}

		fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
		if( TempIndex != (u_int16_t) -1 ) FirstSpotFXUsed = &SpotFX[ TempIndex ];
		else FirstSpotFXUsed = NULL;
		fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
		if( TempIndex != (u_int16_t) -1 ) FirstSpotFXFree = &SpotFX[ TempIndex ];
		else FirstSpotFXFree = NULL;

		for( i = 0; i < NumSpotFX; i++ )
		{
			fread( &SpotFX[ i ].Type, sizeof( u_int16_t ), 1, fp );
			fread( &SpotFX[ i ].Flags, sizeof( u_int16_t ), 1, fp );
			fread( &SpotFX[ i ].State, sizeof( u_int16_t ), 1, fp );
			fread( &SpotFX[ i ].Index, sizeof( u_int16_t ), 1, fp );
			fread( &SpotFX[ i ].Group, sizeof( u_int16_t ), 1, fp );
			fread( &SpotFX[ i ].Light, sizeof( u_int16_t ), 1, fp );
			fread( &SpotFX[ i ].SoundFX, sizeof( int16_t ), 1, fp );
			fread( &SpotFX[ i ].SoundFXVolume, sizeof( float ), 1, fp );
			fread( &SpotFX[ i ].SoundFXSpeed, sizeof( float ), 1, fp );
			fread( &SpotFX[ i ].SoundFXType, sizeof( int16_t ), 1, fp );
			fread( &SpotFX[ i ].SoundFX_ID, sizeof( u_int32_t ), 1, fp );
			fread( &SpotFX[ i ].Primary, sizeof( int8_t ), 1, fp );
			fread( &SpotFX[ i ].Secondary, sizeof( int8_t ), 1, fp );
			fread( &SpotFX[ i ].Red, sizeof( u_int8_t ), 1, fp );
			fread( &SpotFX[ i ].Green, sizeof( u_int8_t ), 1, fp );
			fread( &SpotFX[ i ].Blue, sizeof( u_int8_t ), 1, fp );
			fread( &SpotFX[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &SpotFX[ i ].DirVector, sizeof( VECTOR ), 1, fp );
			fread( &SpotFX[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fread( &SpotFX[ i ].Delay, sizeof( float ), 1, fp );
			fread( &SpotFX[ i ].ActiveDelay, sizeof( float ), 1, fp );
			fread( &SpotFX[ i ].InactiveDelay, sizeof( float ), 1, fp );
			fread( &SpotFX[ i ].Time, sizeof( float ), 1, fp );
			fread( &SpotFX[ i ].TimeInterval, sizeof( float ), 1, fp );
			fread( &SpotFX[ i ].GenDelay, sizeof( float ), 1, fp );
			fread( &SpotFX[ i ].MaxHeight, sizeof( float ), 1, fp );
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) SpotFX[ i ].PrevUsed = &SpotFX[ TempIndex ];
			else SpotFX[ i ].PrevUsed = NULL;
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) SpotFX[ i ].NextUsed = &SpotFX[ TempIndex ];
			else SpotFX[ i ].NextUsed = NULL;
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) SpotFX[ i ].PrevFree = &SpotFX[ TempIndex ];
			else SpotFX[ i ].PrevFree = NULL;
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) SpotFX[ i ].NextFree = &SpotFX[ TempIndex ];
			else SpotFX[ i ].NextFree = NULL;
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) SpotFX[ i ].PrevInGroup = &SpotFX[ TempIndex ];
			else SpotFX[ i ].PrevInGroup = NULL;
			fread( &TempIndex, sizeof( u_int16_t ), 1, fp );
			if( TempIndex != (u_int16_t) -1 ) SpotFX[ i ].NextInGroup = &SpotFX[ TempIndex ];
			else SpotFX[ i ].NextInGroup = NULL;
		}
	}

	return( fp );
}
