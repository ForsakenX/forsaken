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
#include "tload.h"
#include "lines.h"
#include "mxload.h"
#include "mxaload.h"

#include "spotfx.h"
#include "util.h"

/*===================================================================
	Defines
===================================================================*/
//#define	DEBUG_COMP			1
#define	COB_VERSION_NUMBER	2

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
extern	u_int16_t			IsGroupVisible[MAXGROUPS];
extern	MODELNAME		ModelNames[MAXMODELHEADERS];
extern	TRIGGERMOD	*	TrigMods;
extern	int				NumOfTrigMods;
extern	MXLOADHEADER	ModelHeaders[MAXMODELHEADERS];
extern	MXALOADHEADER	MxaModelHeaders[MAXMODELHEADERS];
extern	LINE			Lines[ MAXLINES ];
extern	MLOADHEADER		Mloadheader;

bool GetMXBoundingBox( MXLOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft,
					   VECTOR * BottomRight );
bool GetMXABoundingBox( MXALOADHEADER * DstMloadheader, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft,
					    VECTOR * BottomRight );

//#ifdef OPT_ON
//#pragma optimize( "gty", on )
//#endif

/*===================================================================
	Global Variables
===================================================================*/
int8_t * CompObjPath = "data\\bgobjects\\";
MODELNAME	*	ModNames = &ModelNames[ 0 ];

/*===================================================================
	Procedure	:	PreLoad Componented Object Data
	Input		:	int8_t		*	Filename of .COB data
				:	u_int16_t		*	BaseModel
				:	bool			Level Specific?
	Output		:	bool			true/false
===================================================================*/
bool PreLoadCompObj( int8_t * path, u_int16_t * BaseModel, bool LevelSpecific )
{
	int16_t		i;
	FILE	*	fp;
	int16_t		Count;
	int16_t		NumModels;
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;

	fp = file_open( path, "rb" );

	if( fp != NULL )
	{
		if( fread( &MagicNumber, sizeof( u_int32_t ), 1, fp ) != 1 )
		{
			Msg( "PreLoadCompObj() Corrupt Componented Object (.COB) file %s", path );
			return( false );
		}

		if( fread( &VersionNumber, sizeof( u_int32_t ), 1, fp ) != 1 )
		{
			Msg( "PreLoadCompObj() Corrupt Componented Object (.COB) file %s", path );
			return( false );
		}

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != COB_VERSION_NUMBER  ) )
		{
			fclose( fp );
			Msg( "PreLoadCompObj() Incompatible Componented Object (.COB) file %s", path );
			return( false );
		}

		if( fread( &NumModels, sizeof( int16_t ), 1, fp ) != 1 )
		{
			Msg( "PreLoadCompObj() Corrupt Componented Object (.COB) file %s", path );
			return( false );
		}
   	
		if( ( (*BaseModel) + NumModels + 1 ) < MAXMODELHEADERS )
		{
			for( Count = 0; Count < NumModels; Count++ )
			{
				i = 0;
		
				do
				{
					if( fread( &ModNames[ (*BaseModel) + Count ].Name[ i ], sizeof( int8_t ), 1, fp ) != 1 )
					{
						Msg( "PreLoadCompObj() Corrupt Componented Object (.COB) file %s", path );
						return( false );
					}
					i++;
				}
				while( ModNames[ (*BaseModel) + Count ].Name[ ( i - 1 ) ] );

				ModNames[ (*BaseModel) + Count ].LOD = 0;
				ModNames[ (*BaseModel) + Count ].Panel = false;
				ModNames[ (*BaseModel) + Count ].DoIMorph = false;
				ModNames[ (*BaseModel) + Count ].ModelIndex = (*BaseModel) + Count;
				ModNames[ (*BaseModel) + Count ].StoreTriangles = false;
				ModNames[ (*BaseModel) + Count ].AllocateTpage = LOAD_TPAGES;
				ModNames[ (*BaseModel) + Count ].LevelSpecific = LevelSpecific;
				ModNames[ (*BaseModel) + Count ].LoadEnable = DO_LOAD;
			}
 		
			ModNames[ (*BaseModel) + NumModels ].Name[0] = 0;
			ModNames[ (*BaseModel) + NumModels ].LOD = 0;
			ModNames[ (*BaseModel) + NumModels ].Panel = false;
			ModNames[ (*BaseModel) + NumModels ].DoIMorph = false;
			ModNames[ (*BaseModel) + NumModels ].ModelIndex = (*BaseModel) + NumModels;
			ModNames[ (*BaseModel) + NumModels ].StoreTriangles = false;
			ModNames[ (*BaseModel) + NumModels ].AllocateTpage = LOAD_TPAGES;
			ModNames[ (*BaseModel) + NumModels ].LevelSpecific = LevelSpecific;
			ModNames[ (*BaseModel) + NumModels ].LoadEnable = DO_LOAD;

			(*BaseModel) += NumModels;
		}
		else
		{
			fclose( fp );
	        Msg( "PreLoadCompObj() Too many models in %s\n", path );
			return( false );
		}
 	}
	else
	{
        Msg( "PreLoadCompObj() Unable to open %s\n", path );
		return( false );
	}

 	fclose( fp );

	return( true );
}

/*===================================================================
	Procedure	:	Load Componented Object Data
	Input		:	int8_t		*	Filename of .COB data ( Including Path )
				:	VECTOR		*	Pos
				:	VECTOR		*	Dir
				:	u_int16_t			Group
				:	float		*	Overall Anim Time
				:	float		*	Mid Time
				:	u_int16_t		*	BaseModel
				:	bool			Level Specific?
				:	u_int16_t			OwnerType
				:	u_int16_t			OwnerID
	Output		:	COMP_OBJ	*	Components
===================================================================*/
COMP_OBJ * LoadCompObj( int8_t * path, VECTOR * Pos, VECTOR * Dir, u_int16_t Group,
					    float * OverallTime, float * MidTime, u_int16_t * BaseModel,
						u_int16_t OwnerType, u_int16_t OwnerID )
{
	FILE		*	fp;
	int16_t			Count;
	int16_t			NumModels;
	COMP_OBJ	*	CompsPtr;
	char			TempChar;
	u_int32_t			MagicNumber;
	u_int32_t			VersionNumber;

	*OverallTime = 0.0F;
	*MidTime = 0.0F;

	fp = file_open( path, "rb" );

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != COB_VERSION_NUMBER  ) )
		{
			fclose( fp );
			Msg( "LoadCompObj() Incompatible Componented Object (.COB) file %s", path );
			return( NULL );
		}

  		CompsPtr = malloc( sizeof( COMP_OBJ ) );

		if( CompsPtr )
		{
			fread( &NumModels, sizeof( int16_t ), 1, fp );
		
			for( Count = 0; Count < NumModels; Count++ )
			{
				do
				{
					fread( &TempChar, sizeof( int8_t ), 1, fp );		// Skip model filenames
				}
				while( TempChar );
			}

			fp = LoadCompObjChildren( fp, CompsPtr, 1, Pos, Dir, Group, OverallTime, MidTime, BaseModel,
									  OwnerType, OwnerID );

			if( !fp )
			{
				free( CompsPtr );
				return( NULL );
			}

			(*BaseModel) += NumModels;
		}
		else
		{
	        Msg( "LoadCompObj() Unable to alloc CompsPtr in %s\n", path );
			return( NULL );
		}
	}
	else
	{
        Msg( "LoadCompObj() Unable to load Componented object %s\n", path );
		return( NULL );
	}

 	fclose( fp );

	*OverallTime *= ANIM_SECOND;
	if( *MidTime ) *MidTime *= ANIM_SECOND;
	else *MidTime = ( *OverallTime * 0.5F );

	return( CompsPtr );
}

/*===================================================================
	Procedure	:	Load and allocate background object children
	Input		:	FILE		*	File Ptr
				:	COMP_OBJ	*	Components Address
				:	int16_t			Number of Components
				:	VECTOR		*	Pos
				:	VECTOR		*	Direction
				:	u_int16_t			Group
				:	float		*	Overall Time ( TBFI )
				:	float		*	Mid Time ( TBFI )
				:	u_int16_t		*	BaseModel ( TBU )
				:	u_int16_t			OwnerType
				:	u_int16_t			OwnerID
	Output		:	FILE		*	Updated File Pointer
===================================================================*/
FILE * LoadCompObjChildren( FILE * fp, COMP_OBJ * Comp, int16_t NumComp,
						    VECTOR * Pos, VECTOR * Dir, u_int16_t Group,
							float * Time, float * MidTime, u_int16_t * BaseModel,
							u_int16_t OwnerType, u_int16_t OwnerID )
{
	int16_t				ID;
	int16_t				Count;
	int16_t				CompCount;
	int16_t				NumTrans;
	float				TransStart;
	float				TransDuration;
	int16_t				NumMorphs;
	QUAT				TempQuat;
	u_int16_t				ModelIndex;
	COMP_OBJ		*	CompsPtr;
	ANI_TRANS		*	TransPtr;
	ANI_TRANSLATE	*	AniTransPtr;
	ANI_ROTATE		*	AniRotPtr;
	ANI_MORPH		*	AniMorphPtr;
	ANI_PROPERTY	*	AniPropPtr;
	int16_t				NumColZones;
	int16_t				ColZoneType;
	int16_t				ColZoneSensitive;
	float				ColZoneDamage;
	int16_t				Zone;
	int16_t				Side;
	int16_t				NumSides;
	ANI_ZONES		*	ZonesPtr;
	ANI_ZONE_SPHERE *	AniZoneSpherePtr;
	ANI_ZONE_BOX	*	AniZoneBoxPtr;
	ANI_ZONE_POLYGONAL * AniZonePolyPtr;
	VECTOR				BoxCenter;
	VECTOR				BoxHalfSize;
	ZONESIDE		*	SidesPtr;
	int16_t				i;
	int8_t				SFXFilename[ 256 ];

	for( CompCount = 0; CompCount < NumComp; CompCount++ )
	{
		fread( &ID, sizeof( int16_t ), 1, fp );

		Comp->Flags = COMPFLAG_Enable;
		Comp->ID = ID;
		Comp->OffPos.x = 0.0F;
		Comp->OffPos.y = 0.0F;
		Comp->OffPos.z = 0.0F;
		Comp->Dir.x = 0.0F;
		Comp->Dir.y = 0.0F;
		Comp->Dir.z = 1.0F;
		Comp->Up.x = 0.0F;
		Comp->Up.y = 1.0F;
		Comp->Up.z = 0.0F;
		Comp->RotPoint.x = 0.F;
		Comp->RotPoint.y = 0.F;
		Comp->RotPoint.z = 0.F;
		Comp->RotAxis.x = 0.0F;
		Comp->RotAxis.y = 1.0F;
		Comp->RotAxis.z = 0.0F;
		Comp->RotAngle = 0.0F;
		Comp->LocalMatrix = MATRIX_Identity;

		Comp->UserControl = false;
		Comp->UserAxis.x = 0.0F;
		Comp->UserAxis.y = 1.0F;
		Comp->UserAxis.z = 0.0F;
		Comp->UserAxisPoint.x = 0.0F;
		Comp->UserAxisPoint.y = 0.0F;
		Comp->UserAxisPoint.z = 0.0F;
		Comp->UserAngle = 0.0F;

		Comp->DisplayPos.x = 0.0F;
		Comp->DisplayPos.y = 0.0F;
		Comp->DisplayPos.z = 0.0F;
		Comp->DisplayMatrix = MATRIX_Identity;
		Comp->InvDisplayMatrix = MATRIX_Identity;

		Comp->OldDisplayPos.x = 0.0F;
		Comp->OldDisplayPos.y = 0.0F;
		Comp->OldDisplayPos.z = 0.0F;
		Comp->OldDisplayMatrix = MATRIX_Identity;
		Comp->OldInvDisplayMatrix = MATRIX_Identity;
		
		Comp->OldOldDisplayPos.x = 0.0F;
		Comp->OldOldDisplayPos.y = 0.0F;
		Comp->OldOldDisplayPos.z = 0.0F;
		Comp->OldOldDisplayMatrix = MATRIX_Identity;
		Comp->OldOldInvDisplayMatrix = MATRIX_Identity;

		QuatFromDirAndUp( &Comp->Dir, &Comp->Up, &TempQuat );
		QuatToMatrix( &TempQuat, &Comp->InitMatrix );
	
		if( ID != -1 )
		{
			ModelIndex = FindFreeModel();

			Comp->ModelIndex = ModelIndex;

			if( ModelIndex != (u_int16_t) -1 )
			{
				Models[ ModelIndex ].ModelNum = ( (*BaseModel) + ID );
				Models[ ModelIndex ].Func = MODFUNC_Nothing;

				if( ModNames[ Models[ ModelIndex ].ModelNum ].DoIMorph )
				{
					if( MxaModelHeaders[ Models[ ModelIndex ].ModelNum ].NumSpotFX &&
						MxaModelHeaders[ Models[ ModelIndex ].ModelNum ].SpotFX )
					{
						Models[ ModelIndex ].Func = MODFUNC_ProcessSpotFX;
						SetupModelSpotFX( ModelIndex );
					}
				}
				else
				{
					if( ModelHeaders[ Models[ ModelIndex ].ModelNum ].NumSpotFX &&
						ModelHeaders[ Models[ ModelIndex ].ModelNum ].SpotFX )
					{
						Models[ ModelIndex ].Func = MODFUNC_ProcessSpotFX;
						SetupModelSpotFX( ModelIndex );
					}
				}

				Models[ ModelIndex ].Type = MODTYPE_Field;
				Models[ ModelIndex ].Flags = MODFLAG_Nothing;
				Models[ ModelIndex ].Visible = true;
				Models[ ModelIndex ].Pos.x = ( Pos->x + Comp->OffPos.x );
				Models[ ModelIndex ].Pos.y = ( Pos->y + Comp->OffPos.y );
				Models[ ModelIndex ].Pos.z = ( Pos->z + Comp->OffPos.z );
				Models[ ModelIndex ].Dir = *Dir;
				Models[ ModelIndex ].Rot.x = 0.0F;
				Models[ ModelIndex ].Rot.y = 0.0F;
				Models[ ModelIndex ].Rot.z = 0.0F;
				Models[ ModelIndex ].Quat.w = 1.0F;
				Models[ ModelIndex ].Quat.x = 0.0F;
				Models[ ModelIndex ].Quat.y = 0.0F;
				Models[ ModelIndex ].Quat.z = 0.0F;
				Models[ ModelIndex ].AxisRot = 0.0F;
				Models[ ModelIndex ].Scale = 1.0F;
				Models[ ModelIndex ].MaxScale = 1.0F;
				Models[ ModelIndex ].Group = Group;
				Models[ ModelIndex ].LifeCount = -1.0F;
				Models[ ModelIndex ].Mat = MATRIX_Identity;
				Models[ ModelIndex ].OwnerType = OwnerType;
				Models[ ModelIndex ].Owner = OwnerID;
				MatrixTranspose( &Models[ ModelIndex ].Mat, &Models[ ModelIndex ].InvMat );
	
			}
			else
			{
		        Msg( "LoadCompObjChildren() Unable to alloc model\n" );
				return( NULL );
			}
		}
		else
		{
			Comp->ModelIndex = (u_int16_t) -1;
		}
	
		fread( &NumTrans, sizeof( int16_t ), 1, fp );
		
		NumMorphs = 0;

		if( NumTrans )
		{
			TransPtr = malloc( sizeof( ANI_TRANS ) * NumTrans );
	
			if( TransPtr != NULL )
			{
#ifdef DEBUG_COMP
				DebugPrintf("TRANSFORM	:	Number %d\n", NumTrans );
#endif

				for( Count = 0; Count < NumTrans; Count++ )
				{
					fread( &TransPtr[ Count ].Type, sizeof( int16_t ), 1, fp );
					fread( &TransStart, sizeof( float ), 1, fp );
					fread( &TransDuration, sizeof( float ), 1, fp );
		
					if( ( TransStart + TransDuration ) > *Time ) *Time = ( TransStart + TransDuration );

					TransPtr[ Count ].TimeStart = ( TransStart * ANIM_SECOND );
					TransPtr[ Count ].TimeDuration = ( TransDuration * ANIM_SECOND );

					switch( TransPtr[ Count ].Type )
					{
						case TRANS_TRANSLATE:
    						TransPtr[ Count ].Data = malloc( sizeof( ANI_TRANSLATE ) );
							if( TransPtr[ Count ].Data != NULL )
							{
								AniTransPtr = (ANI_TRANSLATE *) TransPtr[ Count ].Data;
								fread( &AniTransPtr->Trans.x, sizeof( float ), 1, fp );
								fread( &AniTransPtr->Trans.y, sizeof( float ), 1, fp );
								fread( &AniTransPtr->Trans.z, sizeof( float ), 1, fp );
								fread( &AniTransPtr->Local, sizeof( int16_t ), 1, fp );
#ifdef DEBUG_COMP
								DebugPrintf("	TRANSLATION	:%f:%f: %f, %f, %f\n", TransStart, ( TransStart + TransDuration ),
												AniTransPtr->Trans.x, AniTransPtr->Trans.y, AniTransPtr->Trans.z ); 
#endif

							}
							else
							{
						        Msg( "LoadCompObjChildren() Unable to alloc TranslationData\n" );
								return( NULL );
							}
							break;
				
						case TRANS_ROTATE:
							TransPtr[ Count ].Data = malloc( sizeof( ANI_ROTATE ) );
							if( TransPtr[ Count ].Data != NULL )
							{
								AniRotPtr = (ANI_ROTATE *) TransPtr[ Count ].Data;
								fread( &AniRotPtr->Axis.x, sizeof( float ), 1, fp );
								fread( &AniRotPtr->Axis.y, sizeof( float ), 1, fp );
								fread( &AniRotPtr->Axis.z, sizeof( float ), 1, fp );
								fread( &AniRotPtr->Origin.x, sizeof( float ), 1, fp );
								fread( &AniRotPtr->Origin.y, sizeof( float ), 1, fp );
								fread( &AniRotPtr->Origin.z, sizeof( float ), 1, fp );
								fread( &AniRotPtr->Angle, sizeof( float ), 1, fp );
								fread( &AniRotPtr->Local, sizeof( int16_t ), 1, fp );
								AniRotPtr->Angle = D2R( AniRotPtr->Angle );

								AniRotPtr->Origin.x = -AniRotPtr->Origin.x;
								AniRotPtr->Origin.y = -AniRotPtr->Origin.y;
								AniRotPtr->Origin.z = -AniRotPtr->Origin.z;
#ifdef DEBUG_COMP
								DebugPrintf("	ROTATION	:%f:%f: Axis %f, %f, %f, Origin %f, %f, %f, Angle %f, Local %d\n",
												TransStart, ( TransStart + TransDuration ),
												AniRotPtr->Axis.x, AniRotPtr->Axis.y, AniRotPtr->Axis.z,
												AniRotPtr->Origin.x, AniRotPtr->Origin.y, AniRotPtr->Origin.z,
												AniRotPtr->Angle, AniRotPtr->Local ); 
#endif
							}
							else
							{
						        Msg( "LoadCompObjChildren() Unable to alloc RotationData\n" );
								return( NULL );
							}
							break;
				
						case TRANS_MORPH:
							NumMorphs++;
							TransPtr[ Count ].Data = malloc( sizeof( ANI_MORPH ) );
							if( TransPtr[ Count ].Data != NULL )
							{
								AniMorphPtr = (ANI_MORPH *) TransPtr[ Count ].Data;
								AniMorphPtr->Frame = NumMorphs;
#ifdef DEBUG_COMP
								DebugPrintf("	MORPH		:%f:%f: %d\n", TransStart,
											( TransStart + TransDuration ), AniMorphPtr->Frame );
#endif
							}
							else
							{
						        Msg( "LoadCompObjChildren() Unable to alloc MorphData\n" );
								return( NULL );
							}
							break;
				
						case TRANS_PROPERTY:
							TransPtr[ Count ].Data = malloc( sizeof( ANI_PROPERTY ) );
							if( TransPtr[ Count ].Data != NULL )
							{
								AniPropPtr = (ANI_PROPERTY *) TransPtr[ Count ].Data;
								fread( &AniPropPtr->Type, sizeof( int16_t ), 1, fp );

								switch( AniPropPtr->Type )
								{
									case PROPTYPE_State:
										fread( &AniPropPtr->Value, sizeof( int16_t ), 1, fp );
										break;
									case PROPTYPE_SFX:
										i = 0;
										do
										{
											fread( &SFXFilename[ i ], sizeof( int8_t ), 1, fp );
											i++;
										}
										while( SFXFilename[ ( i - 1 ) ] != 0 );

										AniPropPtr->Value = ReturnSFXIndex( &SFXFilename[ 0 ] );
										break;

									case PROPTYPE_Unknown:
										fread( &AniPropPtr->Value, sizeof( int16_t ), 1, fp );
										break;

									default:
										Msg( "Unknown AnimProperty Type %hd\n", AniPropPtr->Type );
										fread( &AniPropPtr->Value, sizeof( int16_t ), 1, fp );
										break;
								}

								switch( AniPropPtr->Type )
								{
									case PROPTYPE_State:
										switch( AniPropPtr->Value )
										{
											case PROPSTATE_Open:
												*MidTime = TransStart;
												break;

											case PROPSTATE_Opening:
											case PROPSTATE_Closing:
											case PROPSTATE_Closed:
											case PROPSTATE_UserControl:
											case PROPSTATE_Invulnerable:
												break;

											case PROPSTATE_DisableSpotFX:
												break;
										}
#ifdef DEBUG_COMP
										DebugPrintf("	PROPERTY	:%f:%f: %d,%d\n", TransStart, ( TransStart + TransDuration ),
													AniPropPtr->Type, AniPropPtr->Value );
#endif
										break;

									case PROPTYPE_SFX:
										break;

									case PROPTYPE_Unknown:
									default:
#ifdef DEBUG_COMP
										DebugPrintf("	PROPERTY	:%f:%f:	Unknown Property !!!!\n", TransStart, ( TransStart + TransDuration ) );
#endif
										break;
								}
							}
							else
							{
						        Msg( "LoadCompObjChildren() Unable to alloc PropertyData\n" );
								return( NULL );
							}
							break;
					}
				}
	
				Comp->NumTrans = NumTrans;
				Comp->Trans = TransPtr;
			}
			else
			{
		        Msg( "LoadCompObjChildren() Unable to alloc Trans\n" );
				return( NULL );
			}
		}
		else
		{
			Comp->NumTrans = NumTrans;
			Comp->Trans = NULL;
		}
		
		fread( &NumColZones, sizeof( int16_t ), 1, fp );

		if( NumColZones )
		{
			ZonesPtr = malloc( sizeof( ANI_ZONES ) * NumColZones );
	
			if( ZonesPtr )
			{
				for( Zone = 0; Zone < NumColZones; Zone++ )
				{
					fread( &ColZoneType, sizeof( int16_t ), 1, fp );			// 0-sphere 1-Box 2-Convex Shape
					fread( &ColZoneSensitive, sizeof( int16_t ), 1, fp );		// 0-Do anything 1-Activate door
					fread( &ColZoneDamage, sizeof( float ), 1, fp );		// Damage ??
					ColZoneDamage /= ANIM_SECOND;

					ZonesPtr[ Zone ].Type = ColZoneType;
					ZonesPtr[ Zone ].Sensitive = ColZoneSensitive;
					ZonesPtr[ Zone ].Damage = ColZoneDamage;
	
					fread( &BoxCenter.x, sizeof( float ), 1, fp );
					fread( &BoxCenter.y, sizeof( float ), 1, fp );
					fread( &BoxCenter.z, sizeof( float ), 1, fp );
					
					fread( &BoxHalfSize.x, sizeof( float ), 1, fp );
					fread( &BoxHalfSize.y, sizeof( float ), 1, fp );
					fread( &BoxHalfSize.z, sizeof( float ), 1, fp );
	
					switch( ColZoneType )
					{
						case ZONE_Sphere:
							ZonesPtr[ Zone ].Data = malloc( sizeof( ANI_ZONE_SPHERE ) );
	
							if( ZonesPtr[ Zone ].Data )
							{
								AniZoneSpherePtr = (ANI_ZONE_SPHERE *) ZonesPtr[ Zone ].Data;
								AniZoneSpherePtr->Center = BoxCenter;
								AniZoneSpherePtr->HalfSize = BoxHalfSize;
							}
							else
							{
						        Msg( "LoadCompObjChildren() Unable to alloc ColZoneSphere\n" );
								return( NULL );
							}
							break;
	
						case ZONE_Box:
							ZonesPtr[ Zone ].Data = malloc( sizeof( ANI_ZONE_BOX ) );
	
							if( ZonesPtr[ Zone ].Data )
							{
								AniZoneBoxPtr = (ANI_ZONE_BOX *) ZonesPtr[ Zone ].Data;
								AniZoneBoxPtr->Center = BoxCenter;
								AniZoneBoxPtr->HalfSize = BoxHalfSize;
	
								fread( &NumSides, sizeof( int16_t ), 1, fp );
	
								AniZoneBoxPtr->NumSides = NumSides;
								AniZoneBoxPtr->Sides = malloc( sizeof( ZONESIDE ) * NumSides );
	
								if( AniZoneBoxPtr->Sides )
								{
									SidesPtr = AniZoneBoxPtr->Sides;
	
									for( Side = 0; Side < NumSides; Side++ )
									{
										fread( &SidesPtr->Normal.x, sizeof( float ), 1, fp );
										fread( &SidesPtr->Normal.y, sizeof( float ), 1, fp );
										fread( &SidesPtr->Normal.z, sizeof( float ), 1, fp );
										fread( &SidesPtr->PlaneOffset, sizeof( float ), 1, fp );
										fread( &SidesPtr->Sensitive, sizeof( int16_t ), 1, fp );
										fread( &SidesPtr->Damage, sizeof( float ), 1, fp );
										SidesPtr->Damage /= ANIM_SECOND;
										SidesPtr++;
									}
								}
								else
								{
							        Msg( "LoadCompObjChildren() Unable to alloc ColZoneBoxSides\n" );
									return( NULL );
								}
							}
							else
							{
						        Msg( "LoadCompObjChildren() Unable to alloc ColZoneSphere\n" );
								return( NULL );
							}
							break;
	
						case ZONE_Polygonal:
							ZonesPtr[ Zone ].Data = malloc( sizeof( ANI_ZONE_POLYGONAL ) );
	
							if( ZonesPtr[ Zone ].Data )
							{
								AniZonePolyPtr = (ANI_ZONE_POLYGONAL *) ZonesPtr[ Zone ].Data;
								AniZonePolyPtr->Center = BoxCenter;
								AniZonePolyPtr->HalfSize = BoxHalfSize;
	
								fread( &NumSides, sizeof( int16_t ), 1, fp );
	
								AniZonePolyPtr->NumSides = NumSides;
								AniZonePolyPtr->Sides = malloc( sizeof( ZONESIDE ) * NumSides );
	
								if( AniZonePolyPtr->Sides )
								{
									SidesPtr = AniZonePolyPtr->Sides;
	
									for( Side = 0; Side < NumSides; Side++ )
									{
										fread( &SidesPtr->Normal.x, sizeof( float ), 1, fp );
										fread( &SidesPtr->Normal.y, sizeof( float ), 1, fp );
										fread( &SidesPtr->Normal.z, sizeof( float ), 1, fp );
										fread( &SidesPtr->PlaneOffset, sizeof( float ), 1, fp );
										fread( &SidesPtr->Sensitive, sizeof( int16_t ), 1, fp );
										fread( &SidesPtr->Damage, sizeof( float ), 1, fp );
										SidesPtr->Damage /= ANIM_SECOND;
										SidesPtr++;
									}
								}
								else
								{
							        Msg( "LoadCompObjChildren() Unable to alloc ColZonePoly\n" );
									return( NULL );
								}
							}
							else
							{
						        Msg( "LoadCompObjChildren() Unable to alloc ColZonePolySides\n" );
								return( NULL );
							}
							break;
					}
				}
	
				Comp->NumColZones = NumColZones;
				Comp->ColZones = ZonesPtr;
			}
			else
			{
		        Msg( "LoadCompObjChildren() Unable to alloc ColZonesPtr\n" );
				return( NULL );
			}
		}
		else
		{
			Comp->NumColZones = 0;
			Comp->ColZones = NULL;
		}

		fread( &Comp->NumChildren, sizeof( int16_t ), 1, fp );
	
		if( Comp->NumChildren )
		{
			CompsPtr = malloc( ( sizeof( COMP_OBJ ) * Comp->NumChildren ) );

			if( CompsPtr )
			{
				Comp->Children = CompsPtr;
				fp = LoadCompObjChildren( fp, CompsPtr, Comp->NumChildren, Pos, Dir, Group, Time, MidTime,
										  BaseModel, OwnerType, OwnerID );
			}
			else
			{
		        Msg( "LoadCompObjChildren() Unable to alloc children\n" );
				return( NULL );
			}
		}
		else
		{
			Comp->Children = NULL;
		}

		Comp++;
	}

	return( fp );
}

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

/*===================================================================
	Procedure	:	Free components of object
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
	Output		:	Nothing
===================================================================*/
void FreeCompObjChildren( COMP_OBJ * Children, int16_t NumChildren )
{
	int16_t			Count;
	int16_t			Trans;
	int16_t			ColZone;
	ANI_ZONES		*	ZonesPtr;
	ANI_ZONE_BOX	*	AniZoneBoxPtr;
	ANI_ZONE_POLYGONAL * AniZonePolyPtr;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->NumTrans )
			{
				for( Trans = 0; Trans < Children->NumTrans; Trans++ )
				{
					free( Children->Trans[ Trans ].Data );
					Children->Trans[ Trans ].Data = NULL;
				}

				free( Children->Trans );
				Children->NumTrans = 0;
				Children->Trans = NULL;
			}

			if( Children->NumColZones && Children->ColZones )
			{
				ZonesPtr = Children->ColZones;

				for( ColZone = 0; ColZone < Children->NumColZones; ColZone++ )
				{
					switch( ZonesPtr->Type )
					{
						case ZONE_Sphere:
							break;
	
						case ZONE_Box:
							AniZoneBoxPtr = (ANI_ZONE_BOX *) ZonesPtr->Data;

							if( AniZoneBoxPtr->NumSides && AniZoneBoxPtr->Sides )
							{
								free( AniZoneBoxPtr->Sides );
								AniZoneBoxPtr->NumSides = 0;
							}
							break;
	
						case ZONE_Polygonal:
							AniZonePolyPtr = (ANI_ZONE_POLYGONAL *) ZonesPtr->Data;

							if( AniZonePolyPtr->NumSides && AniZonePolyPtr->Sides )
							{
								free( AniZonePolyPtr->Sides );
								AniZonePolyPtr->NumSides = 0;
							}
							break;
					}

					free( ZonesPtr->Data );
					ZonesPtr->Data = NULL;

					ZonesPtr++;
				}

				free( Children->ColZones );
				Children->ColZones = NULL;
				Children->NumColZones = 0;
			}

			if( Children->NumChildren )
			{
				FreeCompObjChildren( Children->Children, Children->NumChildren );
				free( Children->Children );
				Children->Children = NULL;
				Children->NumChildren = 0;
			}

			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				KillUsedModel( Children->ModelIndex );
				Children->ModelIndex = (u_int16_t) -1;
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Process components of an object
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	MATRIX		*	Parent Matrix
				:	VECTOR		*	Parent Position
				:	float			Time
				:	u_int16_t			Group
				:	VECTOR		*	Center
	Output		:	Nothing
===================================================================*/
void UpdateCompObjChildren( COMP_OBJ * Children, int16_t NumChildren, MATRIX * ParentMatrix, VECTOR * ParentPos, float Time, u_int16_t Group, VECTOR * Center )
{
	int16_t				Count;
	int16_t				Trans;
	VECTOR				NewPos;
	MATRIX				RotMatrix;
	ANI_TRANSLATE	*	TransData;
	ANI_ROTATE		*	RotData;
	ANI_MORPH		*	MorphData;
	ANI_PROPERTY	*	PropData;
	float				Start;
	float				Duration;
	float				End;
	float				Scale;
	VECTOR				NewPos2;
	VECTOR				NewAxis;
	VECTOR				NewOrigin;
	VECTOR				TempPos;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			Children->OldOldTime			 = Children->OldTime;
			Children->OldOldDisplayMatrix	 = Children->OldDisplayMatrix;
			Children->OldOldInvDisplayMatrix = Children->OldInvDisplayMatrix;
			Children->OldOldDisplayPos		 = Children->OldDisplayPos;

			Children->OldTime				 = Time;
			Children->OldDisplayMatrix		 = Children->DisplayMatrix;
			Children->OldInvDisplayMatrix	 = Children->InvDisplayMatrix;
			Children->OldDisplayPos			 = Children->DisplayPos;

			Children->Frame = 0;
			Children->InterpFrame = 0;
			Children->InterpTime = 0.0F;

			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				Models[ Children->ModelIndex ].Flags &= ~MODFLAG_SpotFXDisabled;
			}

			if( Children->UserControl )
			{
				Children->LocalMatrix = MATRIX_Identity;
				MatrixFromAxisAndAngle( Children->UserAngle, &Children->UserAxis, &RotMatrix );
				MatrixMultiply( &RotMatrix, &Children->LocalMatrix, &Children->LocalMatrix );
				ApplyMatrix( &Children->LocalMatrix, &Children->UserAxisPoint, &NewPos );
				NewPos.x -= Children->UserAxisPoint.x;
				NewPos.y -= Children->UserAxisPoint.y;
				NewPos.z -= Children->UserAxisPoint.z;

				ApplyMatrix( ParentMatrix, &NewPos, &NewPos );
				NewPos.x += ParentPos->x;
				NewPos.y += ParentPos->y;
				NewPos.z += ParentPos->z;

				MatrixMultiply( ParentMatrix, &Children->LocalMatrix, &Children->DisplayMatrix );
			}
			else
			{
				Children->LocalMatrix = MATRIX_Identity;
				NewPos.x = 0.0F;
				NewPos.y = 0.0F;
				NewPos.z = 0.0F;
	
				if( Children->NumTrans )
				{
					for( Trans = 0; Trans < Children->NumTrans; Trans++ )
					{
						Start = Children->Trans[ Trans ].TimeStart;
						Duration = Children->Trans[ Trans ].TimeDuration;
						End = ( Start + Duration );
		
						if( Time >= Start )
						{
							if( Time < End ) Scale = ( ( Time - Start ) / Duration );
							else Scale = 1.0F;
		
							switch( Children->Trans[ Trans ].Type )
							{
								case TRANS_TRANSLATE:
									TransData = (ANI_TRANSLATE *) Children->Trans[ Trans ].Data;

									NewPos.x += ( TransData->Trans.x * Scale );
									NewPos.y += ( TransData->Trans.y * Scale );
									NewPos.z += ( TransData->Trans.z * Scale );
									break;
							
								case TRANS_ROTATE:
									RotData = (ANI_ROTATE *) Children->Trans[ Trans ].Data;
	
									if( RotData->Local )
									{
										ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewOrigin );
										ApplyMatrix( &Children->LocalMatrix, &RotData->Axis, &NewAxis );
										MatrixFromAxisAndAngle( ( RotData->Angle * Scale ), &NewAxis, &RotMatrix );
										MatrixMultiply( &RotMatrix, &Children->LocalMatrix, &Children->LocalMatrix );
										ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewPos2 );
										NewPos.x += ( NewPos2.x - NewOrigin.x );
										NewPos.y += ( NewPos2.y - NewOrigin.y );
										NewPos.z += ( NewPos2.z - NewOrigin.z );
									}
									else
									{
										ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewOrigin );
										MatrixFromAxisAndAngle( ( RotData->Angle * Scale ), &RotData->Axis, &RotMatrix );
										MatrixMultiply( &RotMatrix, &Children->LocalMatrix, &Children->LocalMatrix );
										ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewPos2 );
										NewPos.x += ( NewPos2.x - NewOrigin.x );
										NewPos.y += ( NewPos2.y - NewOrigin.y );
										NewPos.z += ( NewPos2.z - NewOrigin.z );
									}
									break;
							
								case TRANS_MORPH:
									MorphData = (ANI_MORPH *) Children->Trans[ Trans ].Data;
	
									if( Scale != 1.0F )
									{
										Children->InterpFrame = MorphData->Frame;
										Children->InterpTime = Scale;
									}
									else
									{
										Children->Frame = MorphData->Frame;
									}
									break;
							
								case TRANS_PROPERTY:
									PropData = (ANI_PROPERTY *) Children->Trans[ Trans ].Data;


									switch( PropData->Type )
									{
										case PROPTYPE_State:
											switch( PropData->Value )
											{
												case PROPSTATE_Destroy:
													if( Children->ModelIndex != (u_int16_t) -1 )
													{
														ApplyMatrix( &Models[ Children->ModelIndex ].Mat,
															         &ModelHeaders[ Models[ Children->ModelIndex ].ModelNum ].Center, &TempPos );
														TempPos.x += Models[ Children->ModelIndex ].Pos.x;
														TempPos.y += Models[ Children->ModelIndex ].Pos.y;
														TempPos.z += Models[ Children->ModelIndex ].Pos.z;

														CreateSpotFXShrapnel( &TempPos,
																			  &Models[ Children->ModelIndex ].Dir,
																			  Models[ Children->ModelIndex ].Group );

														KillUsedModel( Children->ModelIndex );
														Children->Flags &= ~COMPFLAG_Enable;
														Children->ModelIndex = (u_int16_t) -1;
													}
													break;

												case PROPSTATE_DisableSpotFX:
													if( Scale < 1.0F )
													{
														if( Children->ModelIndex != (u_int16_t) -1 )
														{
															Models[ Children->ModelIndex ].Flags |= MODFLAG_SpotFXDisabled;
														}
													}
													break;

												case PROPSTATE_Open:
												case PROPSTATE_Opening:
												case PROPSTATE_Closing:
												case PROPSTATE_Closed:
												case PROPSTATE_UserControl:
												case PROPSTATE_Invulnerable:
												default:
													break;
											}
											break;

										case PROPTYPE_SFX:
											if( PropData->Value != -1 )
											{
												if( ( Children->OldOldTime < Start ) && ( Children->OldTime >= Start ) )
												{
													PlayPannedSfx( PropData->Value, Group, Center, 0.0F );
												}
											}
											break;

										case PROPTYPE_Unknown:
										default:
											break;
									}
									break;
							}
						}
					}
				}

				ApplyMatrix( ParentMatrix, &NewPos, &NewPos );
				NewPos.x += ParentPos->x;
				NewPos.y += ParentPos->y;
				NewPos.z += ParentPos->z;

				MatrixMultiply( ParentMatrix, &Children->LocalMatrix, &Children->DisplayMatrix );
			}

			MatrixTranspose( &Children->DisplayMatrix, &Children->InvDisplayMatrix );
			Children->DisplayPos = NewPos;

			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				Models[ Children->ModelIndex ].Frame = Children->Frame;
				Models[ Children->ModelIndex ].InterpFrame = Children->InterpFrame;
				Models[ Children->ModelIndex ].InterpTime = Children->InterpTime;
				Models[ Children->ModelIndex ].Group = Group;

				Models[ Children->ModelIndex ].Pos = NewPos;
				MatrixMultiply( &Children->DisplayMatrix, &Children->InitMatrix, &Models[ Children->ModelIndex ].Mat );
				MatrixTranspose( &Models[ Children->ModelIndex ].Mat, &Models[ Children->ModelIndex ].InvMat );
			}

			if( Children->NumChildren )
			{
				UpdateCompObjChildren( Children->Children, Children->NumChildren, &Children->DisplayMatrix, &NewPos, Time, Group, Center );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Undo Anim of CompObject and children
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
	Output		:	Nothing
===================================================================*/
void UndoCompObjAnim( COMP_OBJ * Children, int16_t NumChildren )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			Children->DisplayMatrix			 = Children->OldDisplayMatrix;
			Children->InvDisplayMatrix		 = Children->OldInvDisplayMatrix;
			Children->DisplayPos			 = Children->OldDisplayPos;

			Children->OldTime				 = Children->OldOldTime;
			Children->OldDisplayMatrix		 = Children->OldOldDisplayMatrix;
			Children->OldInvDisplayMatrix	 = Children->OldOldInvDisplayMatrix;
			Children->OldDisplayPos			 = Children->OldOldDisplayPos;

			if( Children->NumChildren )
			{
				UndoCompObjAnim( Children->Children, Children->NumChildren );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Get Component Address
	Input		:	int16_t			ID
				:	int16_t			Number of Children
				:	COMP_OBJ	*	Start of Componented Object
	Output		:	COMP_OBJ	*	Component
===================================================================*/
COMP_OBJ * GetCompObjAddress( int16_t ID, int16_t NumChildren, COMP_OBJ * Children )
{
	int16_t			Count;
	COMP_OBJ	*	CompObj;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( ID == Children->ID ) return( Children );

			if( Children->NumChildren )
			{
				CompObj = GetCompObjAddress( ID, Children->NumChildren, Children->Children );
				if( CompObj ) return( CompObj );
			}

			Children++;
		}
	}
	return( NULL );
}

/*===================================================================
	Procedure	:	Process components of an object
	Input		:	COMP_OBJ	*	Component
	Output		:	bool			True/False
===================================================================*/
bool GetCompObjAxis( COMP_OBJ * Comp )
{
	int16_t				Trans;
	ANI_ROTATE		*	RotData;

	if( Comp->NumTrans )
	{
		for( Trans = 0; Trans < Comp->NumTrans; Trans++ )
		{
			switch( Comp->Trans[ Trans ].Type )
			{
				case TRANS_TRANSLATE:
					break;
			
				case TRANS_ROTATE:
					RotData = (ANI_ROTATE *) Comp->Trans[ Trans ].Data;

					Comp->UserAxisPoint = RotData->Origin;
					Comp->UserAxis = RotData->Axis;
					return( true );
					break;
			
				case TRANS_MORPH:
					break;
			
				case TRANS_PROPERTY:
					break;
			}
		}
	}

	return( false );
}

/*===================================================================
	Procedure	:	Enable Component Object models
	Input		:	COMP_OBJ	*	Start of Componented Object
				:	int16_t			Number of Children
				:	bool			True/False ( Visible / Invisible )
	Output		:	Nothing
===================================================================*/
void SetCompObjModelsState( COMP_OBJ * Children, int16_t NumChildren, bool Visible )
{
	int16_t			Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				Models[ Children->ModelIndex ].Visible = Visible;
			}

			if( Children->NumChildren )
			{
				SetCompObjModelsState( Children->Children, Children->NumChildren, Visible );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Display components collision zones
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	u_int16_t			Group
	Output		:	Nothing
===================================================================*/
void ShowCompObjColZones( COMP_OBJ * Children, int16_t NumChildren, u_int16_t Group )
{
	int16_t					Count;
	int16_t					Zone;
	int16_t					NumSides;
	int16_t					Side;
	ZONESIDE			*	Sides;
	ANI_ZONES			*	ZonesPtr;
	ANI_ZONE_SPHERE		*	AniZoneSpherePtr;
	ANI_ZONE_BOX		*	AniZoneBoxPtr;
	ANI_ZONE_POLYGONAL	*	AniZonePolyPtr;
	ZONESIDE				ZoneSides[ 64 ];
	MATRIX					ModelMatrix;
	VECTOR					Vert;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->NumColZones )
			{
				MatrixMultiply( &Children->DisplayMatrix, &Children->InitMatrix, &ModelMatrix );

				ZonesPtr = Children->ColZones;

				for( Zone = 0; Zone < Children->NumColZones; Zone++ )
				{
					switch( ZonesPtr->Type )
					{
						case ZONE_Sphere:
							AniZoneSpherePtr = (ANI_ZONE_SPHERE *) ZonesPtr->Data;

							NumSides = 0;
							break;
	
						case ZONE_Box:
							AniZoneBoxPtr = (ANI_ZONE_BOX *) ZonesPtr->Data;

							Sides = AniZoneBoxPtr->Sides;
							NumSides = AniZoneBoxPtr->NumSides;

							for( Side = 0; Side < NumSides; Side++ )
							{
								Vert.x = ( Sides->Normal.x * -Sides->PlaneOffset );
								Vert.y = ( Sides->Normal.y * -Sides->PlaneOffset );
								Vert.z = ( Sides->Normal.z * -Sides->PlaneOffset );
								ApplyMatrix( &ModelMatrix, &Vert, &Vert );
								Vert.x += Children->DisplayPos.x;
								Vert.y += Children->DisplayPos.y;
								Vert.z += Children->DisplayPos.z;
								ApplyMatrix( &ModelMatrix, &Sides->Normal, &ZoneSides[ Side ].Normal );
								ZoneSides[ Side ].PlaneOffset = -DotProduct( &Vert, &ZoneSides[ Side ].Normal );
								ZoneSides[ Side ].Sensitive = Sides->Sensitive;
								Sides++;
							}

							DisplayColZone( &ZoneSides[ 0 ], NumSides, Group, ZonesPtr->Sensitive );
							break;
	
						case ZONE_Polygonal:
							AniZonePolyPtr = (ANI_ZONE_POLYGONAL *) ZonesPtr->Data;

							Sides = AniZonePolyPtr->Sides;
							NumSides = AniZonePolyPtr->NumSides;

							for( Side = 0; Side < NumSides; Side++ )
							{
								Vert.x = ( Sides->Normal.x * -Sides->PlaneOffset );
								Vert.y = ( Sides->Normal.y * -Sides->PlaneOffset );
								Vert.z = ( Sides->Normal.z * -Sides->PlaneOffset );
								ApplyMatrix( &ModelMatrix, &Vert, &Vert );
								Vert.x += Children->DisplayPos.x;
								Vert.y += Children->DisplayPos.y;
								Vert.z += Children->DisplayPos.z;
								ApplyMatrix( &ModelMatrix, &Sides->Normal, &ZoneSides[ Side ].Normal );
								ZoneSides[ Side ].PlaneOffset = -DotProduct( &Vert, &ZoneSides[ Side ].Normal );
								ZoneSides[ Side ].Sensitive = Sides->Sensitive;
								Sides++;
							}

							DisplayColZone( &ZoneSides[ 0 ], NumSides, Group, ZonesPtr->Sensitive );
							break;
					}

					ZonesPtr++;
				}
			}

			if( Children->NumChildren && Children->Children )
			{
				ShowCompObjColZones( Children->Children, Children->NumChildren, Group );
			}

			Children++;
		}
	}
}

/*===================================================================
 	Procedure	:		Check Ray to Col Zone
	Input		:		VECTOR	*	StartPos
				:		VECTOR	*	EndPos
				:		ZONESIDE *	Sides
				:		int16_t		NumSides
				:		VECTOR	*	Intersection Point ( TBFI )
				:		ZONESIDE **	Intersection Side ( TBFI )
				:		float		Collision Radius
	Output		:		void
===================================================================*/
bool RayToColZone( VECTOR * StartPos, VECTOR * EndPos, ZONESIDE * StartSide, int16_t StartNumSides, VECTOR * IntPoint, ZONESIDE ** IntSide, float Radius )
{
	float		d1, d2;
	float		DistToPlane;
	ZONESIDE *	Sides;
	int16_t		NumSides;
	VECTOR		TempInt;

	Sides = StartSide;
	NumSides = StartNumSides;

	while( NumSides-- )
	{
		d1 = DotProduct( StartPos, &Sides->Normal ) + ( Sides->PlaneOffset - Radius );
		d2 = DotProduct( EndPos, &Sides->Normal ) + ( Sides->PlaneOffset - Radius );

		if( ( d1 >= 0.0F ) && ( d2 >= 0.0F ) ) return( false );
		if( ( d1 > 0.0F ) && ( d2 < 0.0F ) )
		{
			DistToPlane = ( d1 / ( d1 - d2 ) );
			TempInt.x = StartPos->x + ( DistToPlane * ( EndPos->x - StartPos->x ) );
			TempInt.y = StartPos->y + ( DistToPlane * ( EndPos->y - StartPos->y ) );
			TempInt.z = StartPos->z + ( DistToPlane * ( EndPos->z - StartPos->z ) );

			if( PointInside( &TempInt, StartSide, StartNumSides, Radius, NumSides ) )
			{
				*IntSide = Sides;
				*IntPoint = TempInt;
				return( true );
			}
		}

		Sides++;
	}

	return( false );
}

/*===================================================================
 	Procedure	:		Is point inside convex shape?
	Input		:		VECTOR	*	Pos
				:		ZONESIDE *	Sides
				:		int16_t		NumSides
	Output		:		bool		true/false
===================================================================*/
bool PointInside( VECTOR * Pos, ZONESIDE * Sides, int16_t NumSides, float Radius, int16_t Side )
{
	float		d1;

	while( NumSides-- )
	{
		if( NumSides != Side )
		{
			d1 = DotProduct( Pos, &Sides->Normal ) + ( Sides->PlaneOffset - Radius );
			if( d1 > 0.0F ) return false;
		}
		Sides++;
	}

	return true;
}

/*===================================================================
	Procedure	:	Get Componented Object Bounding Box
	Input		:	MATRIX		*	Parent Matrix
				:	VECTOR		*	Parent Pos
				:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	float			OverallTime
				:	VECTOR		*	Top Left (TBFI)
				:	VECTOR		*	Botom Right (TBFI)
	Output		:	Nothing
===================================================================*/
void GetCompObjBoundingBox( MATRIX * ParentMatrix, VECTOR * ParentPos,
						   COMP_OBJ * Children, int16_t NumChildren, float OverallTime, VECTOR * TopLeft,
						   VECTOR * BottomRight )
{
	float	Time;
	float	Step;

	TopLeft->x = 0.0F;
	TopLeft->y = 0.0F;
	TopLeft->z = 0.0F;
	BottomRight->x = 0.0F;
	BottomRight->y = 0.0F;
	BottomRight->z = 0.0F;

	Step = ( OverallTime / 100.0F );

	Time = 0.0F;

	for( Time = 0.0F; Time < OverallTime; Time += Step )
	{
		GetCompObjBoundingBoxChildren( Children, NumChildren, ParentMatrix, ParentPos, Time, TopLeft,
									   BottomRight );
	}

	Time = 0.0F;

	GetCompObjBoundingBoxChildren( Children, NumChildren, ParentMatrix, ParentPos, Time, TopLeft,
								   BottomRight );
}

/*===================================================================
	Procedure	:	Process components of an object
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	MATRIX		*	Parent Matrix
				:	VECTOR		*	Parent Position
				:	float			Time
				:	VECTOR		*	TopLeft
				:	VECTOR		*	BottomRight
	Output		:	Nothing
===================================================================*/
void GetCompObjBoundingBoxChildren( COMP_OBJ * Children, int16_t NumChildren, MATRIX * ParentMatrix,
								    VECTOR * ParentPos, float Time, VECTOR * TopLeft, VECTOR * BottomRight )
{
	int16_t				Count;
	int16_t				Trans;
	VECTOR				NewPos;
	MATRIX				RotMatrix;
	ANI_TRANSLATE	*	TransData;
	ANI_ROTATE		*	RotData;
	ANI_MORPH		*	MorphData;
	ANI_PROPERTY	*	PropData;
	float				Start;
	float				Duration;
	float				End;
	float				Scale;

	VECTOR				NewPos2;
	VECTOR				NewAxis;
	VECTOR				NewOrigin;
	MATRIX				DisplayMatrix;
	MATRIX				InvDisplayMatrix;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			Children->LocalMatrix = MATRIX_Identity;
			NewPos.x = 0.0F;
			NewPos.y = 0.0F;
			NewPos.z = 0.0F;

			if( Children->NumTrans )
			{
				for( Trans = 0; Trans < Children->NumTrans; Trans++ )
				{
					Start = Children->Trans[ Trans ].TimeStart;
					Duration = Children->Trans[ Trans ].TimeDuration;
					End = ( Start + Duration );
		
					if( Time > Start )
					{
						if( Time < End ) Scale = ( ( Time - Start ) / Duration );
						else Scale = 1.0F;
		
						switch( Children->Trans[ Trans ].Type )
						{
							case TRANS_TRANSLATE:
								TransData = (ANI_TRANSLATE *) Children->Trans[ Trans ].Data;

								NewPos.x += ( TransData->Trans.x * Scale );
								NewPos.y += ( TransData->Trans.y * Scale );
								NewPos.z += ( TransData->Trans.z * Scale );
								break;
						
							case TRANS_ROTATE:
								RotData = (ANI_ROTATE *) Children->Trans[ Trans ].Data;
	
								if( RotData->Local )
								{
									ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewOrigin );
									ApplyMatrix( &Children->LocalMatrix, &RotData->Axis, &NewAxis );
									MatrixFromAxisAndAngle( ( RotData->Angle * Scale ), &NewAxis, &RotMatrix );
									MatrixMultiply( &RotMatrix, &Children->LocalMatrix, &Children->LocalMatrix );
									ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewPos2 );
									NewPos.x += ( NewPos2.x - NewOrigin.x );
									NewPos.y += ( NewPos2.y - NewOrigin.y );
									NewPos.z += ( NewPos2.z - NewOrigin.z );
								}
								else
								{
									ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewOrigin );
									MatrixFromAxisAndAngle( ( RotData->Angle * Scale ), &RotData->Axis, &RotMatrix );
									MatrixMultiply( &RotMatrix, &Children->LocalMatrix, &Children->LocalMatrix );
									ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewPos2 );
									NewPos.x += ( NewPos2.x - NewOrigin.x );
									NewPos.y += ( NewPos2.y - NewOrigin.y );
									NewPos.z += ( NewPos2.z - NewOrigin.z );
								}
								break;
						
							case TRANS_MORPH:
								MorphData = (ANI_MORPH *) Children->Trans[ Trans ].Data;
	
								if( Scale != 1.0F )
								{
									Children->InterpFrame = MorphData->Frame;
									Children->InterpTime = Scale;
								}
								else
								{
									Children->Frame = MorphData->Frame;
								}
								break;
						
							case TRANS_PROPERTY:
								PropData = (ANI_PROPERTY *) Children->Trans[ Trans ].Data;
								break;
						}
					}
				}
			}

			ApplyMatrix( ParentMatrix, &NewPos, &NewPos );
			NewPos.x += ParentPos->x;
			NewPos.y += ParentPos->y;
			NewPos.z += ParentPos->z;

			MatrixMultiply( ParentMatrix, &Children->LocalMatrix, &Children->DisplayMatrix );

			MatrixTranspose( &Children->DisplayMatrix, &Children->InvDisplayMatrix );
			Children->DisplayPos = NewPos;

			MatrixMultiply( &Children->DisplayMatrix, &Children->InitMatrix, &DisplayMatrix );
			MatrixTranspose( &DisplayMatrix, &InvDisplayMatrix );

			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				if( ModNames[ Models[ Children->ModelIndex ].ModelNum ].DoIMorph )
				{
					GetMXABoundingBox( &MxaModelHeaders[ Models[ Children->ModelIndex ].ModelNum ],
										&DisplayMatrix, &NewPos, TopLeft, BottomRight );
				}
				else
				{
					GetMXBoundingBox( &ModelHeaders[ Models[ Children->ModelIndex ].ModelNum ],
										&DisplayMatrix, &NewPos, TopLeft, BottomRight );
				}
			}

			if( Children->NumChildren )
			{
				GetCompObjBoundingBoxChildren( Children->Children, Children->NumChildren,
											   &Children->DisplayMatrix, &NewPos, Time, TopLeft, BottomRight );
			}

			Children++;
		}
	}
}


/*===================================================================
	Procedure	:	Update components model colours
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	int				Red
				:	int				Green
				:	int				Blue
	Output		:	Nothing
===================================================================*/
void UpdateCompObjColours( COMP_OBJ * Children, int16_t NumChildren, int Red, int Green, int Blue )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				Models[ Children->ModelIndex ].Flags |= MODFLAG_AmbientLight;
				Models[ Children->ModelIndex ].Red = Red;
				Models[ Children->ModelIndex ].Green = Green;
				Models[ Children->ModelIndex ].Blue = Blue;
			}

			if( Children->NumChildren && Children->Children )
			{
				UpdateCompObjColours( Children->Children, Children->NumChildren, Red, Green, Blue );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Update components model Flags
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	u_int16_t			Flags
	Output		:	Nothing
===================================================================*/
void UpdateCompObjFlags( COMP_OBJ * Children, int16_t NumChildren, u_int16_t Flags )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				Models[ Children->ModelIndex ].Flags &= MODFLAG_SpotFXDisabled;
				Models[ Children->ModelIndex ].Flags |= Flags;
			}

			if( Children->NumChildren && Children->Children )
			{
				UpdateCompObjFlags( Children->Children, Children->NumChildren, Flags );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Update components model clip groups
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	u_int16_t			Clip Group
	Output		:	Nothing
===================================================================*/
void UpdateCompObjClipGroup( COMP_OBJ * Children, int16_t NumChildren, u_int16_t ClipGroup )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				if( ClipGroup != (u_int16_t) -1 )
				{
					Models[ Children->ModelIndex ].Flags |= MODFLAG_UseClipGroup;
					Models[ Children->ModelIndex ].ClipGroup = ClipGroup;
				}
				else
				{
					Models[ Children->ModelIndex ].Flags &= ~MODFLAG_UseClipGroup;
					Models[ Children->ModelIndex ].ClipGroup = Models[ Children->ModelIndex ].Group;
				}
			}

			if( Children->NumChildren && Children->Children )
			{
				UpdateCompObjClipGroup( Children->Children, Children->NumChildren, ClipGroup );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Set components model real lighting flag
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	float			Radius
	Output		:	Nothing
===================================================================*/
void SetCompObjRealLighting( COMP_OBJ * Children, int16_t NumChildren, float Radius )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				Models[ Children->ModelIndex ].Flags |= MODFLAG_RealLight;
				Models[ Children->ModelIndex ].Radius = Radius;
			}

			if( Children->NumChildren && Children->Children )
			{
				SetCompObjRealLighting( Children->Children, Children->NumChildren, Radius );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Clear components model real lighting flag
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
	Output		:	Nothing
===================================================================*/
void ClearCompObjRealLighting( COMP_OBJ * Children, int16_t NumChildren )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				Models[ Children->ModelIndex ].Flags &= ~MODFLAG_RealLight;
			}

			if( Children->NumChildren && Children->Children )
			{
				ClearCompObjRealLighting( Children->Children, Children->NumChildren );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Maximize Bounding box
	Input		:	VECTOR	*	TopLeft
				:	VECTOR	*	BottomRight
	Output		:	Nothing
===================================================================*/
void MaximizeBoundingBox( VECTOR * TopLeft, VECTOR * BottomRight )
{
	float	GreatestDistance;
	float	Distance;

	GreatestDistance = VectorLength( TopLeft );
	Distance = VectorLength( BottomRight );
	if( GreatestDistance < Distance ) GreatestDistance = Distance;

	TopLeft->x = -GreatestDistance;
	TopLeft->y = -GreatestDistance;
	TopLeft->z = -GreatestDistance;
	BottomRight->x = GreatestDistance;
	BottomRight->y = GreatestDistance;
	BottomRight->z = GreatestDistance;
}

/*===================================================================
	Procedure	:	Process components of an object
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	MATRIX		*	Parent Matrix
				:	VECTOR		*	Parent Position
				:	float			Time
				:	u_int16_t			Group
	Output		:	Nothing
===================================================================*/
bool AmIInvulnerable( COMP_OBJ * Children, int16_t NumChildren, float Time )
{
	int16_t				Count;
	int16_t				Trans;
	ANI_PROPERTY	*	PropData;
	float				Start;
	float				Duration;
	float				End;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->NumTrans )
			{
				for( Trans = 0; Trans < Children->NumTrans; Trans++ )
				{
					Start = Children->Trans[ Trans ].TimeStart;
					Duration = Children->Trans[ Trans ].TimeDuration;
					End = ( Start + Duration );
		
					if( ( Time >= Start ) && ( Time < End ) )
					{
						switch( Children->Trans[ Trans ].Type )
						{
							case TRANS_TRANSLATE:
							case TRANS_ROTATE:
							case TRANS_MORPH:
								break;
							
							case TRANS_PROPERTY:
								PropData = (ANI_PROPERTY *) Children->Trans[ Trans ].Data;

								switch( PropData->Type )
								{
									case PROPTYPE_State:
										switch( PropData->Value )
										{
											case PROPSTATE_Open:
											case PROPSTATE_Opening:
											case PROPSTATE_Closing:
											case PROPSTATE_Closed:
											case PROPSTATE_UserControl:
												break;

											case PROPSTATE_Invulnerable:
												return( true );
												break;
										}
										break;

									case PROPTYPE_SFX:
									case PROPTYPE_Unknown:
									default:
										break;
								}

								break;
						}
					}
				}
			}

			if( Children->NumChildren )
			{
				if( AmIInvulnerable( Children->Children, Children->NumChildren, Time ) )
				{
					return( true );
				}
			}

			Children++;
		}
	}
	return( false );
}

/*===================================================================
	Procedure	:	Save CompObj + Children
	Input		:	FILE		*	FilePtr
				:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
	Output		:	FILE		*	Updated FilePtr
===================================================================*/
FILE * SaveAllCompObj( FILE * fp, COMP_OBJ * Children, int16_t NumChildren )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			fwrite( &Children->Flags, sizeof( int16_t ), 1, fp );
			fwrite( &Children->ID, sizeof( int16_t ), 1, fp );
			fwrite( &Children->DisplayPos, sizeof( VECTOR ), 1, fp );
			fwrite( &Children->OffPos, sizeof( VECTOR ), 1, fp );
			fwrite( &Children->Dir, sizeof( VECTOR ), 1, fp );
			fwrite( &Children->Up, sizeof( VECTOR ), 1, fp );
			fwrite( &Children->RotPoint, sizeof( VECTOR ), 1, fp );
			fwrite( &Children->RotAxis, sizeof( VECTOR ), 1, fp );
			fwrite( &Children->RotAngle, sizeof( float ), 1, fp );
			fwrite( &Children->InitMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &Children->LocalMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &Children->DisplayMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &Children->InvDisplayMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &Children->Frame, sizeof( int16_t ), 1, fp );
			fwrite( &Children->InterpFrame, sizeof( int16_t ), 1, fp );
			fwrite( &Children->InterpTime, sizeof( float ), 1, fp );
			fwrite( &Children->OldOldTime, sizeof( float ), 1, fp );
			fwrite( &Children->OldOldDisplayPos, sizeof( VECTOR ), 1, fp );
			fwrite( &Children->OldOldDisplayMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &Children->OldOldInvDisplayMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &Children->OldTime, sizeof( float ), 1, fp );
			fwrite( &Children->OldDisplayPos, sizeof( VECTOR ), 1, fp );
			fwrite( &Children->OldDisplayMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &Children->OldInvDisplayMatrix, sizeof( MATRIX ), 1, fp );
			fwrite( &Children->UserControl, sizeof( bool ), 1, fp );
			fwrite( &Children->UserAxis, sizeof( VECTOR ), 1, fp );
			fwrite( &Children->UserAxisPoint, sizeof( VECTOR ), 1, fp );
			fwrite( &Children->UserAngle, sizeof( float ), 1, fp );
			fwrite( &Children->ModelIndex, sizeof( u_int16_t ), 1, fp );

			if( Children->NumChildren )
			{
				fp = SaveAllCompObj( fp, Children->Children, Children->NumChildren );
			}

			Children++;
		}
	}
	return( fp );
}

/*===================================================================
	Procedure	:	Load CompObj + Children
	Input		:	FILE		*	FilePtr
				:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
	Output		:	FILE		*	Updated FilePtr
===================================================================*/
FILE * LoadAllCompObj( FILE * fp, COMP_OBJ * Children, int16_t NumChildren )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			fread( &Children->Flags, sizeof( int16_t ), 1, fp );
			fread( &Children->ID, sizeof( int16_t ), 1, fp );
			fread( &Children->DisplayPos, sizeof( VECTOR ), 1, fp );
			fread( &Children->OffPos, sizeof( VECTOR ), 1, fp );
			fread( &Children->Dir, sizeof( VECTOR ), 1, fp );
			fread( &Children->Up, sizeof( VECTOR ), 1, fp );
			fread( &Children->RotPoint, sizeof( VECTOR ), 1, fp );
			fread( &Children->RotAxis, sizeof( VECTOR ), 1, fp );
			fread( &Children->RotAngle, sizeof( float ), 1, fp );
			fread( &Children->InitMatrix, sizeof( MATRIX ), 1, fp );
			fread( &Children->LocalMatrix, sizeof( MATRIX ), 1, fp );
			fread( &Children->DisplayMatrix, sizeof( MATRIX ), 1, fp );
			fread( &Children->InvDisplayMatrix, sizeof( MATRIX ), 1, fp );
			fread( &Children->Frame, sizeof( int16_t ), 1, fp );
			fread( &Children->InterpFrame, sizeof( int16_t ), 1, fp );
			fread( &Children->InterpTime, sizeof( float ), 1, fp );
			fread( &Children->OldOldTime, sizeof( float ), 1, fp );
			fread( &Children->OldOldDisplayPos, sizeof( VECTOR ), 1, fp );
			fread( &Children->OldOldDisplayMatrix, sizeof( MATRIX ), 1, fp );
			fread( &Children->OldOldInvDisplayMatrix, sizeof( MATRIX ), 1, fp );
			fread( &Children->OldTime, sizeof( float ), 1, fp );
			fread( &Children->OldDisplayPos, sizeof( VECTOR ), 1, fp );
			fread( &Children->OldDisplayMatrix, sizeof( MATRIX ), 1, fp );
			fread( &Children->OldInvDisplayMatrix, sizeof( MATRIX ), 1, fp );
			fread( &Children->UserControl, sizeof( bool ), 1, fp );
			fread( &Children->UserAxis, sizeof( VECTOR ), 1, fp );
			fread( &Children->UserAxisPoint, sizeof( VECTOR ), 1, fp );
			fread( &Children->UserAngle, sizeof( float ), 1, fp );
			fread( &Children->ModelIndex, sizeof( u_int16_t ), 1, fp );

			if( Children->NumChildren )
			{
				fp = LoadAllCompObj( fp, Children->Children, Children->NumChildren );
			}

			Children++;
		}
	}
	return( fp );
}

/*===================================================================
	Procedure	:	Get Componented Object Collision Bounding Box
	Input		:	MATRIX		*	Parent Matrix
				:	VECTOR		*	Parent Pos
				:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	float			OverallTime
				:	VECTOR		*	Top Left (TBFI)
				:	VECTOR		*	Botom Right (TBFI)
	Output		:	Nothing
===================================================================*/
void GetCompObjColBoundingBox( MATRIX * ParentMatrix, VECTOR * ParentPos,
						   COMP_OBJ * Children, int16_t NumChildren, float OverallTime, VECTOR * TopLeft,
						   VECTOR * BottomRight )
{
	float	Time;
	float	Step;

	TopLeft->x = 0.0F;
	TopLeft->y = 0.0F;
	TopLeft->z = 0.0F;
	BottomRight->x = 0.0F;
	BottomRight->y = 0.0F;
	BottomRight->z = 0.0F;

	Step = ( OverallTime / 100.0F );

	Time = 0.0F;

	for( Time = 0.0F; Time < OverallTime; Time += Step )
	{
		GetCompObjColBoundingBoxChildren( Children, NumChildren, ParentMatrix, ParentPos, Time, TopLeft,
									   BottomRight );
	}

	Time = 0.0F;

	GetCompObjColBoundingBoxChildren( Children, NumChildren, ParentMatrix, ParentPos, Time, TopLeft,
								   BottomRight );
}

/*===================================================================
	Procedure	:	Process components of an object
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	MATRIX		*	Parent Matrix
				:	VECTOR		*	Parent Position
				:	float			Time
				:	VECTOR		*	TopLeft
				:	VECTOR		*	BottomRight
	Output		:	Nothing
===================================================================*/
void GetCompObjColBoundingBoxChildren( COMP_OBJ * Children, int16_t NumChildren, MATRIX * ParentMatrix,
								    VECTOR * ParentPos, float Time, VECTOR * TopLeft, VECTOR * BottomRight )
{
	int16_t				Count;
	int16_t				Trans;
	VECTOR				NewPos;
	MATRIX				RotMatrix;
	ANI_TRANSLATE	*	TransData;
	ANI_ROTATE		*	RotData;
	ANI_MORPH		*	MorphData;
	ANI_PROPERTY	*	PropData;
	float				Start;
	float				Duration;
	float				End;
	float				Scale;

	VECTOR				NewPos2;
	VECTOR				NewAxis;
	VECTOR				NewOrigin;
	MATRIX				DisplayMatrix;
	MATRIX				InvDisplayMatrix;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			Children->LocalMatrix = MATRIX_Identity;
			NewPos.x = 0.0F;
			NewPos.y = 0.0F;
			NewPos.z = 0.0F;

			if( Children->NumTrans )
			{
				for( Trans = 0; Trans < Children->NumTrans; Trans++ )
				{
					Start = Children->Trans[ Trans ].TimeStart;
					Duration = Children->Trans[ Trans ].TimeDuration;
					End = ( Start + Duration );
		
					if( Time > Start )
					{
						if( Time < End ) Scale = ( ( Time - Start ) / Duration );
						else Scale = 1.0F;
		
						switch( Children->Trans[ Trans ].Type )
						{
							case TRANS_TRANSLATE:
								TransData = (ANI_TRANSLATE *) Children->Trans[ Trans ].Data;

								NewPos.x += ( TransData->Trans.x * Scale );
								NewPos.y += ( TransData->Trans.y * Scale );
								NewPos.z += ( TransData->Trans.z * Scale );
								break;
						
							case TRANS_ROTATE:
								RotData = (ANI_ROTATE *) Children->Trans[ Trans ].Data;
	
								if( RotData->Local )
								{
									ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewOrigin );
									ApplyMatrix( &Children->LocalMatrix, &RotData->Axis, &NewAxis );
									MatrixFromAxisAndAngle( ( RotData->Angle * Scale ), &NewAxis, &RotMatrix );
									MatrixMultiply( &RotMatrix, &Children->LocalMatrix, &Children->LocalMatrix );
									ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewPos2 );
									NewPos.x += ( NewPos2.x - NewOrigin.x );
									NewPos.y += ( NewPos2.y - NewOrigin.y );
									NewPos.z += ( NewPos2.z - NewOrigin.z );
								}
								else
								{
									ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewOrigin );
									MatrixFromAxisAndAngle( ( RotData->Angle * Scale ), &RotData->Axis, &RotMatrix );
									MatrixMultiply( &RotMatrix, &Children->LocalMatrix, &Children->LocalMatrix );
									ApplyMatrix( &Children->LocalMatrix, &RotData->Origin, &NewPos2 );
									NewPos.x += ( NewPos2.x - NewOrigin.x );
									NewPos.y += ( NewPos2.y - NewOrigin.y );
									NewPos.z += ( NewPos2.z - NewOrigin.z );
								}
								break;
						
							case TRANS_MORPH:
								MorphData = (ANI_MORPH *) Children->Trans[ Trans ].Data;
	
								if( Scale != 1.0F )
								{
									Children->InterpFrame = MorphData->Frame;
									Children->InterpTime = Scale;
								}
								else
								{
									Children->Frame = MorphData->Frame;
								}
								break;
						
							case TRANS_PROPERTY:
								PropData = (ANI_PROPERTY *) Children->Trans[ Trans ].Data;
								break;
						}
					}
				}
			}

			ApplyMatrix( ParentMatrix, &NewPos, &NewPos );
			NewPos.x += ParentPos->x;
			NewPos.y += ParentPos->y;
			NewPos.z += ParentPos->z;

			MatrixMultiply( ParentMatrix, &Children->LocalMatrix, &Children->DisplayMatrix );

			MatrixTranspose( &Children->DisplayMatrix, &Children->InvDisplayMatrix );
			Children->DisplayPos = NewPos;

			MatrixMultiply( &Children->DisplayMatrix, &Children->InitMatrix, &DisplayMatrix );
			MatrixTranspose( &DisplayMatrix, &InvDisplayMatrix );

			GetCompObjColZoneBoundingBox( Children, &DisplayMatrix, &NewPos, TopLeft, BottomRight );

			if( Children->NumChildren )
			{
				GetCompObjColBoundingBoxChildren( Children->Children, Children->NumChildren,
											   &Children->DisplayMatrix, &NewPos, Time, TopLeft, BottomRight );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Process components of an object
	Input		:	COMP_OBJ	*	Children
				:	MATRIX		*	Display Matrix
				:	VECTOR		*	Display Position
				:	VECTOR		*	TopLeft
				:	VECTOR		*	BottomRight
	Output		:	Nothing
===================================================================*/
void GetCompObjColZoneBoundingBox( COMP_OBJ * Children, MATRIX * DisplayMatrix, VECTOR * DisplayPos,
								   VECTOR * TopLeft, VECTOR * BottomRight )
{
	int16_t					ColZone;
	ANI_ZONES			*	ZonesPtr;
	ANI_ZONE_BOX		*	AniZoneBoxPtr;
	ANI_ZONE_POLYGONAL	*	AniZonePolyPtr;

	if( Children->NumColZones && Children->ColZones )
	{
		ZonesPtr = Children->ColZones;

		for( ColZone = 0; ColZone < Children->NumColZones; ColZone++ )
		{
			switch( ZonesPtr->Type )
			{
				case ZONE_Sphere:
					break;
	
				case ZONE_Box:
					AniZoneBoxPtr = (ANI_ZONE_BOX *) ZonesPtr->Data;

					GetZoneBoundingBox( &AniZoneBoxPtr->Center, &AniZoneBoxPtr->HalfSize,
										DisplayMatrix, DisplayPos, TopLeft, BottomRight );
					break;
	
				case ZONE_Polygonal:
					AniZonePolyPtr = (ANI_ZONE_POLYGONAL *) ZonesPtr->Data;

					GetZoneBoundingBox( &AniZonePolyPtr->Center, &AniZonePolyPtr->HalfSize,
										DisplayMatrix, DisplayPos, TopLeft, BottomRight );
					break;
			}

			ZonesPtr++;
		}
	}
}

/*===================================================================
	Procedure	:	Process components of an object
	Input		:	VECTOR		*	Center Offset
				:	VECTOR		*	HalfSize
				:	MATRIX		*	Display Matrix
				:	VECTOR		*	Display Position
				:	VECTOR		*	TopLeft
				:	VECTOR		*	BottomRight
	Output		:	Nothing
===================================================================*/
void GetZoneBoundingBox( VECTOR * Center, VECTOR * HalfSize, MATRIX * Matrix, VECTOR * Pos,
						 VECTOR * TopLeft, VECTOR * BottomRight )
{
	VECTOR	TempVert;

	TempVert.x = ( Center->x - HalfSize->x );
	TempVert.y = ( Center->y - HalfSize->y );
	TempVert.z = ( Center->z - HalfSize->z );
	AddVertToBoundingBox( &TempVert, Matrix, Pos, TopLeft, BottomRight );

	TempVert.x = ( Center->x - HalfSize->x );
	TempVert.y = ( Center->y - HalfSize->y );
	TempVert.z = ( Center->z + HalfSize->z );
	AddVertToBoundingBox( &TempVert, Matrix, Pos, TopLeft, BottomRight );

	TempVert.x = ( Center->x - HalfSize->x );
	TempVert.y = ( Center->y + HalfSize->y );
	TempVert.z = ( Center->z - HalfSize->z );
	AddVertToBoundingBox( &TempVert, Matrix, Pos, TopLeft, BottomRight );

	TempVert.x = ( Center->x + HalfSize->x );
	TempVert.y = ( Center->y - HalfSize->y );
	TempVert.z = ( Center->z - HalfSize->z );
	AddVertToBoundingBox( &TempVert, Matrix, Pos, TopLeft, BottomRight );

	TempVert.x = ( Center->x - HalfSize->x );
	TempVert.y = ( Center->y + HalfSize->y );
	TempVert.z = ( Center->z + HalfSize->z );
	AddVertToBoundingBox( &TempVert, Matrix, Pos, TopLeft, BottomRight );

	TempVert.x = ( Center->x + HalfSize->x );
	TempVert.y = ( Center->y + HalfSize->y );
	TempVert.z = ( Center->z - HalfSize->z );
	AddVertToBoundingBox( &TempVert, Matrix, Pos, TopLeft, BottomRight );

	TempVert.x = ( Center->x + HalfSize->x );
	TempVert.y = ( Center->y - HalfSize->y );
	TempVert.z = ( Center->z + HalfSize->z );
	AddVertToBoundingBox( &TempVert, Matrix, Pos, TopLeft, BottomRight );

	TempVert.x = ( Center->x + HalfSize->x );
	TempVert.y = ( Center->y + HalfSize->y );
	TempVert.z = ( Center->z + HalfSize->z );
	AddVertToBoundingBox( &TempVert, Matrix, Pos, TopLeft, BottomRight );
}

/*===================================================================
	Procedure	:	Add Vertex to bounding Box
	Input		:	VECTOR		*	Vertex
				:	MATRIX		*	Display Matrix
				:	VECTOR		*	Display Position
				:	VECTOR		*	TopLeft
				:	VECTOR		*	BottomRight
	Output		:	Nothing
===================================================================*/
void AddVertToBoundingBox( VECTOR * Vert, MATRIX * Matrix, VECTOR * Pos, VECTOR * TopLeft, VECTOR * BottomRight )
{
	VECTOR		TempVert;

	TempVert = *Vert;

	ApplyMatrix( Matrix, &TempVert, &TempVert );
	TempVert.x += Pos->x;
	TempVert.y += Pos->y;
	TempVert.z += Pos->z;

	if( ( TopLeft->x == BottomRight->x ) && ( TopLeft->y == BottomRight->y ) && ( TopLeft->z == BottomRight->z ) &&
		( TopLeft->x == 0.0F ) && ( TopLeft->y == 0.0F ) && ( TopLeft->z == 0.0F ) )
	{
		TopLeft->x = TempVert.x;
		TopLeft->y = TempVert.y;
		TopLeft->z = TempVert.z;
		BottomRight->x = TempVert.x;
		BottomRight->y = TempVert.y;
		BottomRight->z = TempVert.z;
	}
	else
	{
		if( TempVert.x < TopLeft->x ) TopLeft->x = TempVert.x;
		if( TempVert.x > BottomRight->x ) BottomRight->x = TempVert.x;
		if( TempVert.y < TopLeft->y ) TopLeft->y = TempVert.y;
		if( TempVert.y > BottomRight->y ) BottomRight->y = TempVert.y;
		if( TempVert.z < TopLeft->z ) TopLeft->z = TempVert.z;
		if( TempVert.z > BottomRight->z ) BottomRight->z = TempVert.z;
	}
}

/*===================================================================
	Procedure	:	Clear components model real lighting flag
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
				:	int16_t			Model Offset for stealth
	Output		:	Nothing
===================================================================*/
void SetStealthOffset( COMP_OBJ * Children, int16_t NumChildren, u_int16_t Offset )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				Models[ Children->ModelIndex ].Flags |= MODFLAG_StealthOffset;
				Models[ Children->ModelIndex ].StealthOffset = Offset;
			}

			if( Children->NumChildren && Children->Children )
			{
				SetStealthOffset( Children->Children, Children->NumChildren, Offset );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Set Stealthmode
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
	Output		:	Nothing
===================================================================*/
void SetCompObjStealth( COMP_OBJ * Children, int16_t NumChildren )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				Models[ Children->ModelIndex ].Flags &= ~MODFLAG_AmbientLight;
				Models[ Children->ModelIndex ].Flags |= MODFLAG_Stealth;
			}

			if( Children->NumChildren && Children->Children )
			{
				SetCompObjStealth( Children->Children, Children->NumChildren );
			}

			Children++;
		}
	}
}

/*===================================================================
	Procedure	:	Set Stealthmode
	Input		:	COMP_OBJ	*	Children
				:	int16_t			NumChildren
	Output		:	Nothing
===================================================================*/
void SetCompObjNonStealth( COMP_OBJ * Children, int16_t NumChildren )
{
	int16_t	Count;

	if( Children )
	{
		for( Count = 0; Count < NumChildren; Count++ )
		{
			if( Children->ModelIndex != (u_int16_t) -1 )
			{
				Models[ Children->ModelIndex ].Flags |= MODFLAG_AmbientLight;
				Models[ Children->ModelIndex ].Flags &= ~MODFLAG_Stealth;
			}

			if( Children->NumChildren && Children->Children )
			{
				SetCompObjNonStealth( Children->Children, Children->NumChildren );
			}

			Children++;
		}
	}
}


#ifdef OPT_ON
#pragma optimize( "", off )
#endif
