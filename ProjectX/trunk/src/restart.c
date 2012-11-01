/*===================================================================
	Includes
===================================================================*/
#include "main.h"
#include <stdio.h>

#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "util.h"
#include <math.h>
#include "mload.h"
#include "models.h"

#include "2dpolys.h"
#include "primary.h"
#include "restart.h"
#include "spotfx.h"

/*===================================================================
	Defines
===================================================================*/
#define	RESTART_VERSION_NUMBER	1

/*===================================================================
	External Variables
===================================================================*/
extern	int16_t			NextNewModel;
extern	int16_t			LevelNum;
extern	char			LevelNames[ MAXLEVELS ][ 128 ];
extern	float			framelag;
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
extern	BYTE			ChangeLevel_MyGameStatus;
extern	BYTE			MyGameStatus;
extern	MATRIX			MATRIX_Identity;
extern	u_int16_t			num_start_positions;
extern	GAMESTARTPOS	StartPositions[ MAXSTARTPOSITIONS ];
extern	FMPOLY			FmPolys[MAXNUMOF2DPOLYS];
extern	FRAME_INFO	*	Restart_Header;
extern	u_int16_t			last_start_position;
extern	MODELNAME	*	ModNames;
extern	MODELNAME		ModelNames[MAXMODELHEADERS];
extern	u_int16_t	FirstStartPositionInGroup[MAXGROUPS];

/*===================================================================
	Global Variables
===================================================================*/
int16_t		RestartType = RESTARTTYPE_FACEME;
u_int16_t		RestartModel = (u_int16_t) -1;
RESTART	*	FirstRestartUsed = NULL;
RESTART	*	FirstRestartFree = NULL;
RESTART		RestartPoints[ MAXRESTARTPOINTS ];
RESTART	*	RestartPointGroups[ MAXGROUPS ];
int16_t		NumRestartPointsPerGroup[ MAXGROUPS ];
bool		ShowStartPoints = false;

ANIM_SEQ	RestartSeqs[] = {

	{ 0.01F * ANIM_SECOND, 0.01F * ANIM_SECOND },	// Inactive
	{ 0.0F * ANIM_SECOND, 2.1F * ANIM_SECOND },	// Activating
	{ 2.1F * ANIM_SECOND, 3.0F * ANIM_SECOND },	// Active
	{ 2.1F * ANIM_SECOND, 0.0F * ANIM_SECOND },	// Deactivating
};

/*===================================================================
	Procedure	:	Initialise RestartPoints
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void InitRestartPoints( void )
{
	u_int16_t	i;

	if( !ENABLE_RESTART ) return;

	SetupRestartPointGroups();

	FirstRestartUsed = NULL;
	FirstRestartFree = &RestartPoints[ 0 ];

	for( i = 0; i < MAXRESTARTPOINTS; i++ )
	{
		memset( &RestartPoints[ i ], 0, sizeof( RESTART ) );

		RestartPoints[ i ].NextUsed = NULL;
		RestartPoints[ i ].PrevUsed = NULL;
		RestartPoints[ i ].NextFree = &RestartPoints[ i + 1 ];
		RestartPoints[ i ].PrevFree = &RestartPoints[ i - 1 ];
		RestartPoints[ i ].NextInGroup = NULL;
		RestartPoints[ i ].PrevInGroup = NULL;
		RestartPoints[ i ].FmPoly = (u_int16_t) -1;
		RestartPoints[ i ].Index = i;
	}

	RestartPoints[ 0 ].PrevFree = NULL;
	RestartPoints[ MAXRESTARTPOINTS - 1 ].NextFree = NULL;
}

/*===================================================================
	Procedure	:	Find Free RestartPoint
	Input		:	Nothing
	Output		:	RESTART	*	RestartPoint ( NULL if none )
===================================================================*/
RESTART * FindFreeRestartPoint( void )
{
	RESTART * Object;

	Object = FirstRestartFree;

	if( Object != NULL )
	{
		FirstRestartFree = Object->NextFree;
		if( FirstRestartFree != NULL )
		{
			FirstRestartFree->PrevFree = NULL;
		}

		if( FirstRestartUsed != NULL )
		{
			FirstRestartUsed->PrevUsed = Object;
			Object->NextUsed = FirstRestartUsed;
		}
		else
		{
			Object->NextUsed = NULL;
		}

		Object->PrevUsed = NULL;
		FirstRestartUsed = Object;
	}

	return( Object );
}

/*===================================================================
	Procedure	:	Kill a RestartPoint
	Input		:	RESTART	*	Object
	Output		:	nothing
===================================================================*/
void KillUsedRestartPoint( RESTART * Object )
{
	RESTART	*	PrevObject;
	RESTART	*	NextObject;

	if( Object != NULL )
	{
		RemoveRestartPointFromGroup( Object, Object->Group );

		if( Object == FirstRestartUsed )
		{
			NextObject = FirstRestartUsed->NextUsed;

			FirstRestartUsed = NextObject;

			if( FirstRestartUsed != NULL )
			{
				FirstRestartUsed->PrevUsed = NULL;
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

		if( FirstRestartFree != NULL )
		{
			FirstRestartFree->PrevFree = Object;
		}

		switch( RestartType )
		{
			case RESTARTTYPE_MODEL:
				if( Object->Components )
				{
					FreeCompObjChildren( Object->Components, 1 );
					free( Object->Components );
					Object->Components = NULL;
				}
				break;

			case RESTARTTYPE_FACEME:
				if( Object->FmPoly != (u_int16_t) -1 )
				{
					KillUsedFmPoly( Object->FmPoly );
					Object->FmPoly = (u_int16_t) -1;
				}
				break;
		}

		Object->NextFree = FirstRestartFree;
		Object->PrevFree = NULL;
		FirstRestartFree = Object;
	}
}

/*===================================================================
	Procedure	:	Release all RestartPoints
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void ReleaseAllRestartPoints( void )
{
	RESTART	*	Object;
	RESTART	*	NextUsedObject;

	if( !ENABLE_RESTART ) return;

	Object = FirstRestartUsed;

	while( Object != NULL )
	{
		NextUsedObject = Object->NextUsed;
		KillUsedRestartPoint( Object );
		Object = NextUsedObject;
	}
}

/*===================================================================
	Procedure	:	Load StartPoints
	Input		:	Nothing
	Output		:	bool	True/False
===================================================================*/
bool LoadStartPoints( void )
{
	FILE	*	fp;
	int16_t		Count;
	char	*	NewExt = ".STP";
	char		NewFilename[ 256 ];
	u_int32_t		MagicNumber;
	u_int32_t		VersionNumber;
	int16_t		OllyPad;

	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NewFilename[ 0 ], NewExt );


	fp = file_open( &NewFilename[ 0 ], "rb" );

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( u_int32_t ), 1, fp );
		fread( &VersionNumber, sizeof( u_int32_t ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != RESTART_VERSION_NUMBER  ) )
		{
			fclose( fp );
			Msg( "LoadStartPoints() Incompatible start point file %s", &NewFilename[ 0 ] );
			return( false );
		}

		for( Count = 0 ; Count < MAXGROUPS ; Count++ )
		{
			FirstStartPositionInGroup[Count] = (u_int16_t) -1;
		}

		
		fread( &num_start_positions, sizeof( int16_t ), 1, fp );

		for( Count = 0; Count < num_start_positions; Count++ )
		{
			fread( &StartPositions[ Count ].Pos.x, sizeof( float ), 1, fp );
			fread( &StartPositions[ Count ].Pos.y, sizeof( float ), 1, fp );
			fread( &StartPositions[ Count ].Pos.z, sizeof( float ), 1, fp );
			fread( &StartPositions[ Count ].Dir.x, sizeof( float ), 1, fp );
			fread( &StartPositions[ Count ].Dir.y, sizeof( float ), 1, fp );
			fread( &StartPositions[ Count ].Dir.z, sizeof( float ), 1, fp );
			fread( &StartPositions[ Count ].Up.x, sizeof( float ), 1, fp );
			fread( &StartPositions[ Count ].Up.y, sizeof( float ), 1, fp );
			fread( &StartPositions[ Count ].Up.z, sizeof( float ), 1, fp );
			fread( &StartPositions[ Count ].Group, sizeof( u_int16_t ), 1, fp );
			fread( &OllyPad, sizeof( int16_t ), 1, fp );
			Mloadheader.Group[ StartPositions[ Count ].Group ].StartPosInThisGroup = Count;

			if( FirstStartPositionInGroup[StartPositions[Count].Group] == (u_int16_t) -1 )
			{
				StartPositions[Count].NextInGroup = (u_int16_t) -1;
				FirstStartPositionInGroup[StartPositions[Count].Group] = Count;
			}else{
				StartPositions[Count].NextInGroup = FirstStartPositionInGroup[StartPositions[Count].Group];
				FirstStartPositionInGroup[StartPositions[Count].Group] = Count;
			}

		}

		fclose( fp );
	}

	return( true );
}

/*===================================================================
	Procedure	:	PreLoad Restart Points
	Input		:	Nothing
	Output		:	bool	True/False
===================================================================*/
bool PreLoadRestartPoints( void )
{
	char	*	RestartFilename = "data\\bgobjects\\Restart.cob";
	int16_t		TempModel;

	if( !ENABLE_RESTART ) return( true );

	if( !( ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
		  ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
		  ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) ) ) return true;

	ModNames = &ModelNames[ 0 ];

	switch( RestartType )
	{
		case RESTARTTYPE_MODEL:
			RestartModel = NextNewModel;
			TempModel = NextNewModel;

			if( !PreLoadCompObj( RestartFilename, &TempModel, false ) )
			{
				Msg( "PreLoadRestart() Error preloading restart point\n" );
				return( false );
			}

			NextNewModel = TempModel;
			break;

		case RESTARTTYPE_FACEME:
			break;
	}

	return( true );
}

/*===================================================================
	Procedure	:	Load RestartPoints
	Input		:	Nothing
	Output		:	bool	True/False
===================================================================*/
bool LoadRestartPoints( void )
{
	int16_t		Count;
	char	*	RestartFilename = "data\\bgobjects\\Restart.cob";
	u_int16_t		Group;
	VECTOR		Pos;
	VECTOR		DirVector;
	VECTOR		UpVector;
	int16_t		TempModel;
	COMP_OBJ *	Comp = NULL;
	float		OverallTime, MidTime;
	u_int16_t		Int_Group;
	VECTOR		Int_Point;
	NORMAL		Int_Normal;
	u_int16_t		Int_Group2;
	VECTOR		Int_Point2;
	NORMAL		Int_Normal2;
	VECTOR		TempVector;
	VECTOR		RayVector;
	RESTART	*	Object;
	VECTOR		TopLeft, BottomRight;
	VECTOR		TempPos = { 0.0F, 0.0F, 0.0F };

	if( !ENABLE_RESTART ) return( true );

	if( !( ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
		  ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
		  ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) ) ) return true;


	switch( RestartType )
	{
		case RESTARTTYPE_MODEL:
			for( Count = 0; Count < num_start_positions; Count++ )
			{
				Group = StartPositions[ Count ].Group;
				Pos.x = StartPositions[ Count ].Pos.x;
				Pos.y = StartPositions[ Count ].Pos.y;
				Pos.z = StartPositions[ Count ].Pos.z;
				DirVector.x = StartPositions[ Count ].Dir.x;
				DirVector.y = StartPositions[ Count ].Dir.y;
				DirVector.z = StartPositions[ Count ].Dir.z;
				UpVector.x = StartPositions[ Count ].Up.x;
				UpVector.y = StartPositions[ Count ].Up.y;
				UpVector.z = StartPositions[ Count ].Up.z;

				RayVector.x = ( SlideUp.x * MaxColDistance );
				RayVector.y = ( SlideUp.y * MaxColDistance );
				RayVector.z = ( SlideUp.z * MaxColDistance );

				if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &Pos, Group, &RayVector, &Int_Point, &Int_Group,
										&Int_Normal, &TempVector, false, NULL ) )
				{
					Msg( "LoadRestartPoijnts() Restart point didn't collide with ceiling()\n" );
					return( false );
				}

				RayVector.x = ( SlideDown.x * MaxColDistance );
				RayVector.y = ( SlideDown.y * MaxColDistance );
				RayVector.z = ( SlideDown.z * MaxColDistance );

				if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &Pos, Group, &RayVector, &Int_Point2, &Int_Group2,
										&Int_Normal2, &TempVector, false, NULL ) )
				{
					Msg( "LoadRestartPoijnts() Restart point didn't collide with floor()\n" );
					return( false );
				}

				RayVector.x = ( Int_Point.x - Int_Point2.x );
				RayVector.y = ( Int_Point.y - Int_Point2.y );
				RayVector.z = ( Int_Point.z - Int_Point2.z );
				if( VectorLength( &RayVector ) < ( BLOCKSIZE * 1.75F ) )
				{
					Msg( "Restart point group %s less than 2 blocks high\n", Mloadheader.Group[ Group ].name );
				}

				Pos = Int_Point;
				Pos.y -= ( 41.0F ); // Temp until can do in database
				StartPositions[ Count ].Pos = Int_Point;
				StartPositions[ Count ].Pos.y -= ( 1.0F * BLOCKSIZE );
				StartPositions[ Count ].Group = Int_Group;

				TempModel = RestartModel;

				Comp = LoadCompObj( RestartFilename, &Pos, &DirVector, Int_Group, &OverallTime, &MidTime,
									&TempModel, OWNER_BGOBJECT, Object->Index );

				if( !Comp )
				{
					Msg( "LoadRestartPoints() Error loading restart point\n" );
					return( false );
				}
				else
				{
					TopLeft = TempPos;
					BottomRight = TempPos;
					GetCompObjBoundingBoxChildren( Comp, 1, &MATRIX_Identity, &Pos, 0.0F, &TopLeft, &BottomRight );

					Object = InitOneRestartPoint( Group, &Pos, &DirVector, &UpVector, Comp, OverallTime );

					if( !Object )
					{
						Msg( "LoadRestartPoints() Couldn't find free restart point\n" );
						return( false );
					}
					else
					{
						if( !Count )
						{
							Object->Sequence = RESTARTSEQ_Active;
							Object->Time = RestartSeqs[ Object->Sequence ].StartTime;
						}
						else
						{
							Object->Sequence = RESTARTSEQ_Inactive;
							Object->Time = RestartSeqs[ Object->Sequence ].StartTime;
						}
					}
				}
			}
			break;

		case RESTARTTYPE_FACEME:
			for( Count = 0; Count < num_start_positions; Count++ )
			{
				Group = StartPositions[ Count ].Group;
				Pos.x = StartPositions[ Count ].Pos.x;
				Pos.y = StartPositions[ Count ].Pos.y;
				Pos.z = StartPositions[ Count ].Pos.z;
				DirVector.x = StartPositions[ Count ].Dir.x;
				DirVector.y = StartPositions[ Count ].Dir.y;
				DirVector.z = StartPositions[ Count ].Dir.z;
				UpVector.x = StartPositions[ Count ].Up.x;
				UpVector.y = StartPositions[ Count ].Up.y;
				UpVector.z = StartPositions[ Count ].Up.z;

				Object = InitOneFaceMeRestartPoint( Group, &Pos, &DirVector, &UpVector );

				if( !Object )
				{
					Msg( "LoadRestartPoints() Couldn't find free restart point\n" );
					return( false );
				}
				else
				{
					if( !Count )
					{
						Object->Sequence = RESTARTSEQ_Active;

						if( Object->FmPoly != (u_int16_t) -1 )
						{
							FmPolys[ Object->FmPoly ].SeqNum = FM_RESTART_ACTIVE;
							FmPolys[ Object->FmPoly ].R = 64;
							FmPolys[ Object->FmPoly ].G = 64;
							FmPolys[ Object->FmPoly ].B = 255;
							FmPolys[ Object->FmPoly ].Start_R = 64;
							FmPolys[ Object->FmPoly ].Start_G = 64;
							FmPolys[ Object->FmPoly ].Start_B = 255;
						}
					}
				}
			}
			break;
	}

	return( true );
}

/*===================================================================
	Procedure	:	Setup FaceMe RestartPoint
	Input		:	u_int16_t		Group
				:	VECTOR	*	Pos
				:	VECTOR	*	DirVector
				:	VECTOR	*	UpVector
	Output		:	RESTART	*	Restart Point ( Null if None )
===================================================================*/
RESTART * InitOneFaceMeRestartPoint( u_int16_t Group, VECTOR * Pos, VECTOR * DirVector, VECTOR * UpVector )
{
	RESTART	*	Object;
	u_int16_t		FmPoly;

	Object = FindFreeRestartPoint();

	if( Object )
	{
		Object->Group = Group;
		Object->Pos = *Pos;
		Object->DirVector = *DirVector;
		Object->UpVector = *UpVector;
		Object->Sequence = RESTARTSEQ_Inactive;

		FmPoly = FindFreeFmPoly();					// Faceme polygon attached

		if( FmPoly != (u_int16_t ) -1 )
		{
			FmPolys[ FmPoly ].LifeCount = 1000.0F;
			FmPolys[ FmPoly ].Pos = *Pos;
			FmPolys[ FmPoly ].SeqNum = FM_RESTART_INACTIVE;
			FmPolys[ FmPoly ].RotSpeed = 5.0F;
			FmPolys[ FmPoly ].Frame = 0.0F;
#if ACTUAL_TRANS
			FmPolys[ FmPoly ].Trans = 128;
#else
			FmPolys[ FmPoly ].Trans = 255;
#endif
			FmPolys[ FmPoly ].R = 128;
			FmPolys[ FmPoly ].G = 128;
			FmPolys[ FmPoly ].B = 128;
			FmPolys[ FmPoly ].Start_R = 128;
			FmPolys[ FmPoly ].Start_G = 128;
			FmPolys[ FmPoly ].Start_B = 128;
			FmPolys[ FmPoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ FmPoly ].xsize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ FmPoly ].ysize = ( 6.0F * GLOBAL_SCALE );
			FmPolys[ FmPoly ].Frm_Info = &Restart_Header;
			FmPolys[ FmPoly ].Group = Group;
			Object->FmPoly = FmPoly;
			AddFmPolyToTPage( FmPoly, GetTPage( *FmPolys[ FmPoly ].Frm_Info, 0 ) );
		}
		else
		{
			Object->FmPoly = (u_int16_t) -1;
		}
	}
	return( Object );
}

/*===================================================================
	Procedure	:	Setup RestartPoint
	Input		:	u_int16_t		Group
				:	VECTOR	*	Pos
				:	VECTOR	*	DirVector
				:	VECTOR	*	UpVector
				:	COMP_OBJ *	CompObject
				:	float		OverallTime
	Output		:	RESTART	*	Restart Point ( Null if None )
===================================================================*/
RESTART * InitOneRestartPoint( u_int16_t Group, VECTOR * Pos, VECTOR * DirVector, VECTOR * UpVector, COMP_OBJ * CompObj, float OverallTime )
{
	RESTART	*	Object;
	QUAT		TempQuat;

	Object = FindFreeRestartPoint();

	if( Object )
	{
		Object->Group = Group;
		Object->Pos = *Pos;
		Object->DirVector = *DirVector;
		Object->UpVector = *UpVector;
		Object->Components = CompObj;
		Object->Sequence = RESTARTSEQ_Inactive;
		Object->Time = 0.0F;
		Object->OverallTime = OverallTime;
	   	QuatFromDirAndUp( DirVector, UpVector, &TempQuat );
	   	QuatToMatrix( &TempQuat, &Object->Matrix );
	}
	return( Object );
}

/*===================================================================
	Procedure	:	Process RestartPoints
	Input		:	nothing
	Output		:	nothing
===================================================================*/
void ProcessRestartPoints( void )
{
	RESTART		*	Object;
	RESTART		*	NextObject;
	float			StartTime, EndTime;

	if( !ENABLE_RESTART ) return;

	Object = FirstRestartUsed;

	while( Object != NULL )
	{
		NextObject = Object->NextUsed;

		switch( RestartType )
		{
			case RESTARTTYPE_MODEL:
				StartTime = RestartSeqs[ Object->Sequence ].StartTime;
				EndTime = RestartSeqs[ Object->Sequence ].EndTime;

				switch( Object->Sequence )
				{
					case RESTARTSEQ_Inactive:
						break;

					case RESTARTSEQ_Activating:
						Object->Time += framelag;

						if( Object->Time >= EndTime )
						{
							Object->Sequence = RESTARTSEQ_Active;
							Object->Time = RestartSeqs[ Object->Sequence ].StartTime;
						}
						break;

					case RESTARTSEQ_Active:
						Object->Time += framelag;

						if( Object->Time >= EndTime )
						{
							Object->Time = RestartSeqs[ Object->Sequence ].StartTime;
						}
						break;

					case RESTARTSEQ_Deactivating:
						Object->Time -= framelag;

						if( Object->Time <= EndTime )
						{
							Object->Sequence = RESTARTSEQ_Inactive;
							Object->Time = RestartSeqs[ Object->Sequence ].StartTime;
						}
						break;
				}

				UpdateCompObjChildren( Object->Components, 1, &Object->Matrix, &Object->Pos, Object->Time, Object->Group, &Object->Pos );
				break;

			case RESTARTTYPE_FACEME:
				switch( Object->Sequence )
				{
					case RESTARTSEQ_Inactive:
						break;

					case RESTARTSEQ_Activating:
						if( Object->FmPoly != (u_int16_t) -1 )
						{
							FmPolys[ Object->FmPoly ].SeqNum = FM_RESTART_ACTIVE;
							FmPolys[ Object->FmPoly ].Start_R = 64;
							FmPolys[ Object->FmPoly ].Start_G = 64;
							FmPolys[ Object->FmPoly ].Start_B = 255;
						}
						Object->Sequence = RESTARTSEQ_Active;
						break;

					case RESTARTSEQ_Active:
						break;

					case RESTARTSEQ_Deactivating:
						if( Object->FmPoly != (u_int16_t) -1 )
						{
							FmPolys[ Object->FmPoly ].SeqNum = FM_RESTART_INACTIVE;
							FmPolys[ Object->FmPoly ].Start_R = 128;
							FmPolys[ Object->FmPoly ].Start_G = 128;
							FmPolys[ Object->FmPoly ].Start_B = 128;
						}
						Object->Sequence = RESTARTSEQ_Inactive;
						break;
				}
				break;
		}
		
		Object = NextObject;
	}
}

/*===================================================================
	Procedure	:	Setup RestartPoint group link list
	Input		:	u_int16_t		New RestartPoint
				:	u_int16_t		Old RestartPoint
	Output		:	Nothing
===================================================================*/
void ActivateRestartPoint( u_int16_t NewStartPos, u_int16_t OldStartPos )
{
	if( !ENABLE_RESTART ) return;

	if( MyGameStatus == STATUS_SinglePlayer )
	{
		switch( RestartType )
		{
			case RESTARTTYPE_MODEL:
				RestartPoints[ OldStartPos ].Sequence = RESTARTSEQ_Deactivating;
				RestartPoints[ OldStartPos ].Time = RestartSeqs[ RestartPoints[ OldStartPos ].Sequence ].StartTime;

				RestartPoints[ NewStartPos ].Sequence = RESTARTSEQ_Activating;
				RestartPoints[ NewStartPos ].Time = RestartSeqs[ RestartPoints[ NewStartPos ].Sequence ].StartTime;
				break;

			case RESTARTTYPE_FACEME:
				RestartPoints[ OldStartPos ].Sequence = RESTARTSEQ_Deactivating;
				RestartPoints[ NewStartPos ].Sequence = RESTARTSEQ_Activating;
				break;
		}
	}
}

/*===================================================================
	Procedure	:	Setup RestartPoint group link list
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void SetupRestartPointGroups( void )
{
	int16_t	Count;

	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
		RestartPointGroups[ Count ] = NULL;
		NumRestartPointsPerGroup[ Count ] = 0;
	}
}

/*===================================================================
	Procedure	:	Add RestartPoint to group link list
	Input		:	RESTART	*	RestartPoint
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void AddRestartPointToGroup( RESTART * Object, u_int16_t Group )
{
	Object->PrevInGroup = NULL;
	Object->NextInGroup = RestartPointGroups[ Group ];
	if( Object->NextInGroup ) Object->NextInGroup->PrevInGroup = Object;
	RestartPointGroups[ Group ] = Object;
	NumRestartPointsPerGroup[ Group ]++;
}

/*===================================================================
	Procedure	:	Remove RestartPoint from group link list
	Input		:	RESTART	*	RestartPoint
				:	u_int16_t		Group
	Output		:	Nothing
===================================================================*/
void RemoveRestartPointFromGroup( RESTART * Object, u_int16_t Group )
{
	if( Object->PrevInGroup ) Object->PrevInGroup->NextInGroup = Object->NextInGroup;
	else RestartPointGroups[ Group ] = Object->NextInGroup;
	if( Object->NextInGroup ) Object->NextInGroup->PrevInGroup = Object->PrevInGroup;
	Object->PrevInGroup = NULL;
	Object->NextInGroup = NULL;
	NumRestartPointsPerGroup[ Group ]--;
}

/*===================================================================
	Procedure	:	Move RestartPoint from 1 group to another
	Input		:	RESTART	*	Retart Point
				:	u_int16_t		OldGroup
				:	u_int16_t		NewGroup
	Output		:	Nothing
===================================================================*/
void MoveRestartPointToGroup( RESTART * Object, u_int16_t OldGroup, u_int16_t NewGroup )
{
	RemoveRestartPointFromGroup( Object, OldGroup );
    AddRestartPointToGroup( Object, NewGroup );
}

/*===================================================================
	Procedure	:	Save RestartPoint arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveStartRestartPoints( FILE * fp )
{
	if( fp )
	{
		fwrite( &last_start_position, sizeof( u_int16_t ), 1, fp );
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load RestartPoint Arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadStartRestartPoints( FILE * fp )
{
	if( fp )
	{
		fread( &last_start_position, sizeof( u_int16_t ), 1, fp );
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Display StartPoints
	Input		:	u_int16_t Group
	Output		:	Nothing
===================================================================*/
void DisplayStartPoints( void )
{
	int16_t	Count;
	VECTOR	TopLeft;
	VECTOR	BottomRight;

	for( Count = 0; Count < num_start_positions; Count++ )
	{
		TopLeft.x = ( StartPositions[ Count ].Pos.x - ( SHIP_RADIUS ) );
		TopLeft.y = ( StartPositions[ Count ].Pos.y - ( SHIP_RADIUS ) );
		TopLeft.z = ( StartPositions[ Count ].Pos.z - ( SHIP_RADIUS ) );
		BottomRight.x = ( StartPositions[ Count ].Pos.x + ( SHIP_RADIUS ) );
		BottomRight.y = ( StartPositions[ Count ].Pos.y + ( SHIP_RADIUS ) );
		BottomRight.z = ( StartPositions[ Count ].Pos.z + ( SHIP_RADIUS ) );
		CreateBoundingBox( &TopLeft, &BottomRight, StartPositions[ Count ].Group );
	}
}
