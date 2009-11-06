/*===================================================================
	Includes
===================================================================*/
#include "main.h"
#include <stdio.h>
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "util.h"
#include <math.h>
#include <malloc.h>
#include "mload.h"
#include "models.h"

#include "2dpolys.h"
#include "primary.h"
#include "restart.h"
#include "spotfx.h"
#include "xmem.h"

/*===================================================================
	Defines
===================================================================*/
#define	RESTART_VERSION_NUMBER	1

/*===================================================================
	External Variables
===================================================================*/
extern	int16			NextNewModel;
extern	int16			LevelNum;
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
extern	uint16			num_start_positions;
extern	GAMESTARTPOS	StartPositions[ MAXSTARTPOSITIONS ];
extern	FMPOLY			FmPolys[MAXNUMOF2DPOLYS];
extern	FRAME_INFO	*	Restart_Header;
extern	uint16			last_start_position;
extern	MODELNAME	*	ModNames;
extern	MODELNAME		ModelNames[MAXMODELHEADERS];
extern	uint16	FirstStartPositionInGroup[MAXGROUPS];

/*===================================================================
	Global Variables
===================================================================*/
int16		RestartType = RESTARTTYPE_FACEME;
uint16		RestartModel = (uint16) -1;
RESTART	*	FirstRestartUsed = NULL;
RESTART	*	FirstRestartFree = NULL;
RESTART		RestartPoints[ MAXRESTARTPOINTS ];
RESTART	*	RestartPointGroups[ MAXGROUPS ];
int16		NumRestartPointsPerGroup[ MAXGROUPS ];
BOOL		ShowStartPoints = FALSE;

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
	uint16	i;

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
		RestartPoints[ i ].FmPoly = (uint16) -1;
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
				if( Object->FmPoly != (uint16) -1 )
				{
					KillUsedFmPoly( Object->FmPoly );
					Object->FmPoly = (uint16) -1;
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
	Output		:	BOOL	True/False
===================================================================*/
BOOL LoadStartPoints( void )
{
	FILE	*	fp;
	int16		Count;
	char	*	NewExt = ".STP";
	char		NewFilename[ 256 ];
	uint32		MagicNumber;
	uint32		VersionNumber;
	int16		OllyPad;

	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NewFilename[ 0 ], NewExt );


	fp = fopen( &NewFilename[ 0 ], "rb" );

	if( fp != NULL )
	{
		fread( &MagicNumber, sizeof( uint32 ), 1, fp );
		fread( &VersionNumber, sizeof( uint32 ), 1, fp );

		if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != RESTART_VERSION_NUMBER  ) )
		{
			fclose( fp );
			Msg( "LoadStartPoints() Incompatible start point file %s", &NewFilename[ 0 ] );
			return( FALSE );
		}

		for( Count = 0 ; Count < MAXGROUPS ; Count++ )
		{
			FirstStartPositionInGroup[Count] = (uint16) -1;
		}

		
		fread( &num_start_positions, sizeof( int16 ), 1, fp );

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
			fread( &StartPositions[ Count ].Group, sizeof( uint16 ), 1, fp );
			fread( &OllyPad, sizeof( int16 ), 1, fp );
			Mloadheader.Group[ StartPositions[ Count ].Group ].StartPosInThisGroup = Count;

			if( FirstStartPositionInGroup[StartPositions[Count].Group] == (uint16) -1 )
			{
				StartPositions[Count].NextInGroup = (uint16) -1;
				FirstStartPositionInGroup[StartPositions[Count].Group] = Count;
			}else{
				StartPositions[Count].NextInGroup = FirstStartPositionInGroup[StartPositions[Count].Group];
				FirstStartPositionInGroup[StartPositions[Count].Group] = Count;
			}

		}

		fclose( fp );
	}

	return( TRUE );
}

/*===================================================================
	Procedure	:	PreLoad Restart Points
	Input		:	Nothing
	Output		:	BOOL	True/False
===================================================================*/
BOOL PreLoadRestartPoints( void )
{
	char	*	NewExt = ".RES";
	char	*	RestartFilename = "data\\bgobjects\\Restart.cob";
	int16		TempModel;

	if( !ENABLE_RESTART ) return( TRUE );

	if( !( ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
		  ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
		  ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) ) ) return TRUE;

	ModNames = &ModelNames[ 0 ];

	switch( RestartType )
	{
		case RESTARTTYPE_MODEL:
			RestartModel = NextNewModel;
			TempModel = NextNewModel;

			if( !PreLoadCompObj( RestartFilename, &TempModel, FALSE ) )
			{
				Msg( "PreLoadRestart() Error preloading restart point\n" );
				return( FALSE );
			}

			NextNewModel = TempModel;
			break;

		case RESTARTTYPE_FACEME:
			break;
	}

	return( TRUE );
}

/*===================================================================
	Procedure	:	Load RestartPoints
	Input		:	Nothing
	Output		:	BOOL	True/False
===================================================================*/
BOOL LoadRestartPoints( void )
{
	int16		Count;
	char	*	NewExt = ".RES";
	char	*	RestartFilename = "data\\bgobjects\\Restart.cob";
	uint16		Group;
	VECTOR		Pos;
	VECTOR		DirVector;
	VECTOR		UpVector;
	int16		TempModel;
	COMP_OBJ *	Comp = NULL;
	COMP_OBJ *	TempComp = NULL;
	float		OverallTime, MidTime;
	uint16		Int_Group;
	VECTOR		Int_Point;
	NORMAL		Int_Normal;
	uint16		Int_Group2;
	VECTOR		Int_Point2;
	NORMAL		Int_Normal2;
	VECTOR		TempVector;
	VECTOR		RayVector;
	RESTART	*	Object;
	VECTOR		TopLeft, BottomRight;
	VECTOR		TempPos = { 0.0F, 0.0F, 0.0F };

	if( !ENABLE_RESTART ) return( TRUE );

	if( !( ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
		  ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
		  ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) ) ) return TRUE;


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
										&Int_Normal, &TempVector, FALSE, NULL ) )
				{
					Msg( "LoadRestartPoijnts() Restart point didn't collide with ceiling()\n" );
					return( FALSE );
				}

				RayVector.x = ( SlideDown.x * MaxColDistance );
				RayVector.y = ( SlideDown.y * MaxColDistance );
				RayVector.z = ( SlideDown.z * MaxColDistance );

				if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &Pos, Group, &RayVector, &Int_Point2, &Int_Group2,
										&Int_Normal2, &TempVector, FALSE, NULL ) )
				{
					Msg( "LoadRestartPoijnts() Restart point didn't collide with floor()\n" );
					return( FALSE );
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
					return( FALSE );
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
						return( FALSE );
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
					return( FALSE );
				}
				else
				{
					if( !Count )
					{
						Object->Sequence = RESTARTSEQ_Active;

						if( Object->FmPoly != (uint16) -1 )
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

	return( TRUE );
}

/*===================================================================
	Procedure	:	Setup FaceMe RestartPoint
	Input		:	uint16		Group
				:	VECTOR	*	Pos
				:	VECTOR	*	DirVector
				:	VECTOR	*	UpVector
	Output		:	RESTART	*	Restart Point ( Null if None )
===================================================================*/
RESTART * InitOneFaceMeRestartPoint( uint16 Group, VECTOR * Pos, VECTOR * DirVector, VECTOR * UpVector )
{
	RESTART	*	Object;
	uint16		FmPoly;

	Object = FindFreeRestartPoint();

	if( Object )
	{
		Object->Group = Group;
		Object->Pos = *Pos;
		Object->DirVector = *DirVector;
		Object->UpVector = *UpVector;
		Object->Sequence = RESTARTSEQ_Inactive;

		FmPoly = FindFreeFmPoly();					// Faceme polygon attached

		if( FmPoly != (uint16 ) -1 )
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
			Object->FmPoly = (uint16) -1;
		}
	}
	return( Object );
}

/*===================================================================
	Procedure	:	Setup RestartPoint
	Input		:	uint16		Group
				:	VECTOR	*	Pos
				:	VECTOR	*	DirVector
				:	VECTOR	*	UpVector
				:	COMP_OBJ *	CompObject
				:	float		OverallTime
	Output		:	RESTART	*	Restart Point ( Null if None )
===================================================================*/
RESTART * InitOneRestartPoint( uint16 Group, VECTOR * Pos, VECTOR * DirVector, VECTOR * UpVector, COMP_OBJ * CompObj, float OverallTime )
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
						if( Object->FmPoly != (uint16) -1 )
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
						if( Object->FmPoly != (uint16) -1 )
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
	Input		:	uint16		New RestartPoint
				:	uint16		Old RestartPoint
	Output		:	Nothing
===================================================================*/
void ActivateRestartPoint( uint16 NewStartPos, uint16 OldStartPos )
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
	int16	Count;

	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
		RestartPointGroups[ Count ] = NULL;
		NumRestartPointsPerGroup[ Count ] = 0;
	}
}

/*===================================================================
	Procedure	:	Add RestartPoint to group link list
	Input		:	RESTART	*	RestartPoint
				:	uint16		Group
	Output		:	Nothing
===================================================================*/
void AddRestartPointToGroup( RESTART * Object, uint16 Group )
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
				:	uint16		Group
	Output		:	Nothing
===================================================================*/
void RemoveRestartPointFromGroup( RESTART * Object, uint16 Group )
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
				:	uint16		OldGroup
				:	uint16		NewGroup
	Output		:	Nothing
===================================================================*/
void MoveRestartPointToGroup( RESTART * Object, uint16 OldGroup, uint16 NewGroup )
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
		fwrite( &last_start_position, sizeof( uint16 ), 1, fp );
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
		fread( &last_start_position, sizeof( uint16 ), 1, fp );
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Display StartPoints
	Input		:	uint16 Group
	Output		:	Nothing
===================================================================*/
void DisplayStartPoints( void )
{
	int16	Count;
	VECTOR	TopLeft;
	VECTOR	BottomRight;

	for( Count = 0; Count < num_start_positions; Count++ )
	{
		TopLeft.x = ( StartPositions[ Count ].Pos.x - ( SHIP_RADIUS * 4.0F ) );
		TopLeft.y = ( StartPositions[ Count ].Pos.y - ( SHIP_RADIUS * 4.0F ) );
		TopLeft.z = ( StartPositions[ Count ].Pos.z - ( SHIP_RADIUS * 4.0F ) );
		BottomRight.x = ( StartPositions[ Count ].Pos.x + ( SHIP_RADIUS * 4.0F ) );
		BottomRight.y = ( StartPositions[ Count ].Pos.y + ( SHIP_RADIUS * 4.0F ) );
		BottomRight.z = ( StartPositions[ Count ].Pos.z + ( SHIP_RADIUS * 4.0F ) );
		CreateBoundingBox( &TopLeft, &BottomRight, StartPositions[ Count ].Group );
	}
}
