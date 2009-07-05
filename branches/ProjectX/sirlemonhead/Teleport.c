

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Include Files...	
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <stdio.h>
#include "typedefs.h"
#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "main.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "networking.h"
#include "file.h"
#include "Triggers.h"
#include "pickups.h"
#include "enemies.h"
#include "mload.h"
#include "water.h"
#include "spotfx.h"
#include "Trigarea.h"
#include "Teleport.h"
#include "magic.h"
#include "sfx.h"
#include "Ships.h"
#include "XMem.h"
#include "util.h"

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Defines
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define	TELEPORTS_VERSION_NUMBER	2

																   
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Externals ...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern	VECTOR Forward;
extern	VECTOR SlideUp;
extern	GLOBALSHIP		Ships[ MAX_PLAYERS ];
extern	float framelag;
extern	BOOL	Entry;
extern	BOOL	Exit;
extern	BOOL	In;
extern	BOOL	ShowTeleports;
BOOL RayToHull( TRIGGER_ZONE * StartSide , VECTOR * StartPos , VECTOR * EndPos , uint16 StartNumSides );
void DisplayDirAndUp( VECTOR * Pos, VECTOR * Dir, VECTOR *Up, uint16 Group );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		Globals ...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int16	NumOfTeleports = 0;

TELEPORT * TeleportsGroupLink[MAXGROUPS];

TELEPORT * Teleports = NULL;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Teleports load...
	Input		:	char * filename....
	Output		:	BOOL
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL TeleportsLoad( char * Filename )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	char		*	OrgBuffer;
	uint16		*	Uint16Pnt;
	TELEPORT * TPpnt;
	float * floatpnt;
	int i,j,e;
	TRIGGER_ZONE * ZonePnt;
	uint32			MagicNumber;
	uint32			VersionNumber;
	uint32		*	uint32Pnt;


	NumOfTeleports = 0;
	for( i = 0 ; i < MAXGROUPS ; i++ )
	{
		TeleportsGroupLink[i] = NULL;
	}

	
	File_Size = Get_File_Size( Filename );	
	if( !File_Size )
	{
		// dont need Teleports..
		return TRUE;
	}
	Buffer = malloc( File_Size );
	OrgBuffer = Buffer;

	if( !Buffer )
	{
		Msg( "Teleports Load : Unable to allocate file buffer %s\n", Filename );
		return( FALSE );
	}
	Read_Size = Read_File( Filename, Buffer, File_Size );
	if( Read_Size != File_Size )
	{
		Msg( "Teleports Load Error reading %s\n", Filename );
		return( FALSE );
	}


	uint32Pnt = (uint32 *) Buffer;
	MagicNumber = *uint32Pnt++;
	VersionNumber = *uint32Pnt++;
	Buffer = (char *) uint32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != TELEPORTS_VERSION_NUMBER  ) )
	{
		Msg( "Teleportload() Incompatible Teleports( .tel ) file %s", Filename );
		return( FALSE );
	}

	
	
	Uint16Pnt = (uint16 *) Buffer;
	NumOfTeleports = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;

	Teleports	= (TELEPORT *) malloc( NumOfTeleports * sizeof(TELEPORT) );
	TPpnt = Teleports;
	if( !TPpnt )
	{
		Msg( "Teleports : cant allocate buffer %s\n", Filename );
		return( FALSE );
	}

	for( i = 0 ; i < NumOfTeleports ; i++ )
	{
		Uint16Pnt = (uint16 *) Buffer;
		TPpnt->Group = *Uint16Pnt++;
		TPpnt->Status = *Uint16Pnt++;
//		TPpnt->Type = *Uint16Pnt++;
		if( TeleportsGroupLink[TPpnt->Group] )
		{
			TPpnt->NextInGroup = TeleportsGroupLink[TPpnt->Group];
			TeleportsGroupLink[TPpnt->Group] = TPpnt;
		}else{
			TeleportsGroupLink[TPpnt->Group] = TPpnt;
			TPpnt->NextInGroup = NULL;
		}

		TPpnt->num_links = *Uint16Pnt++;
		if( TPpnt->num_links > MAXTELEPORTLINKS )
		{
			Msg( "Teleports : To many Links in %s\n", Filename );
			return( FALSE );
		}

		for( e = 0 ; e < TPpnt->num_links ; e++ )
		{
			TPpnt->Links[e] = *Uint16Pnt++;
		}
		
		Buffer = (char *) Uint16Pnt;
		floatpnt = (float *) Buffer;

		TPpnt->Pos.x = *floatpnt++;
		TPpnt->Pos.y = *floatpnt++;
		TPpnt->Pos.z = *floatpnt++;

#if TELEPORTS_VERSION_NUMBER >= 2
		TPpnt->Dir.x = *floatpnt++;
		TPpnt->Dir.y = *floatpnt++;
		TPpnt->Dir.z = *floatpnt++;

		TPpnt->Up.x = *floatpnt++;
		TPpnt->Up.y = *floatpnt++;
		TPpnt->Up.z = *floatpnt++;
#endif

		TPpnt->half_size.x = *floatpnt++;
		TPpnt->half_size.y = *floatpnt++;
		TPpnt->half_size.z = *floatpnt++;
		Buffer = (char *) floatpnt;
  
		Uint16Pnt = (uint16 *) Buffer;
		TPpnt->zone_type = *Uint16Pnt++;
		Buffer = (char *) Uint16Pnt;
		
		if( TPpnt->zone_type != ZONE_Sphere )
		{
			// convex hull...
			Uint16Pnt = (uint16 *) Buffer;
			TPpnt->num_sides = *Uint16Pnt++;
			Buffer = (char *) Uint16Pnt;
			
			TPpnt->Zone = (TRIGGER_ZONE*) malloc( TPpnt->num_sides * sizeof( TRIGGER_ZONE ) );
			ZonePnt = TPpnt->Zone;
			
			if( !ZonePnt )
			{
				Msg( "Teleport maloc Error with %s\n", Filename );
				return( FALSE );
			}
			floatpnt = (float * ) Buffer;
			for( j = 0 ; j < TPpnt->num_sides ; j++ )
			{
				ZonePnt->normal.x = *floatpnt++;
				ZonePnt->normal.y = *floatpnt++;
				ZonePnt->normal.z = *floatpnt++;
				ZonePnt->offset   = *floatpnt++;
				ZonePnt++;
			}
			Buffer = (char*) floatpnt;
		}
		else
		{
			TPpnt->Zone = NULL;
		}
		
		TPpnt++;
	}
	free(OrgBuffer);

	return TRUE;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Release Forces load...
	Input		:	void
	Output		:	void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ReleaseTeleports( void )
{
	int i;
	TELEPORT * TPpnt;
	TPpnt = Teleports;

	if( TPpnt )
	{
		for( i = 0 ; i < NumOfTeleports ; i++ )
		{
			if( TPpnt->Zone )
			{
				free( TPpnt->Zone );
			}
			TPpnt++;
		}
		free(Teleports);
		Teleports = NULL;
	}
	NumOfTeleports = 0;
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if im in an Active Teleport....
	Input		:	void
	Output		:	TRUE/FALSE
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL TeleportsAreaCheck( VECTOR * NewPos , VECTOR * OldPos ,uint16 Group, OBJECT *obj )
{
	TELEPORT * TPpnt;
	TELEPORT * newTPpnt;

	TPpnt = TeleportsGroupLink[Group];
	
	while( TPpnt )
	{
		if( TPpnt->Status == TELEPORTACTIVE )
		{
			TeleportsZoneCheck( OldPos , NewPos , TPpnt );
			if( Entry )
			{

				PlaySfx( SFX_Teleport, 1.0F );

//				StartShipScreenShake( 10.0F );
				
				
				// Were in the Zone....
				newTPpnt = Teleports;
				newTPpnt += TPpnt->Links[Random_Range(TPpnt->num_links)];
				obj->Group = newTPpnt->Group;
				obj->Pos = newTPpnt->Pos;
#if TELEPORTS_VERSION_NUMBER >= 2
				QuatFromDirAndUp( &newTPpnt->Dir, &newTPpnt->Up, &obj->Quat);
				QuatToMatrix( &obj->Quat, &obj->Mat );
#endif
				return TRUE;
			}
		}
		TPpnt = TPpnt->NextInGroup;
	}
	return FALSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display Teleports in group
	Input		:	uint16	Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DisplayTeleportsInGroup( uint16 Group )
{
	TELEPORT * TPpnt;

	if( !ShowTeleports ) return;

	TPpnt = TeleportsGroupLink[Group];
	
	while( TPpnt )
	{
		if( TPpnt->Status == TELEPORTACTIVE )
		{
			if( TPpnt->zone_type == ZONE_Sphere )
			{
				DisplaySphereZone( &TPpnt->Pos, TPpnt->half_size.x, Group, 0, 64, 0, 128 );
			}
			else
			{
				DisplayTriggerZone( TPpnt->Zone, TPpnt->num_sides, Group, 0, 64, 0, 128 );
			}
		}
		else
		{
			if( TPpnt->zone_type == ZONE_Sphere )
			{
				DisplaySphereZone( &TPpnt->Pos, TPpnt->half_size.x, Group, 16, 16, 16, 128 );
			}
			else
			{
				DisplayTriggerZone( TPpnt->Zone, TPpnt->num_sides, Group, 16, 16, 16, 128 );
			}
		}
		DisplayDirAndUp( &TPpnt->Pos, &TPpnt->Dir, &TPpnt->Up, TPpnt->Group );

		TPpnt = TPpnt->NextInGroup;
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if im in a Teleport Zone....
	Input		:	void
	Output		:	void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void TeleportsZoneCheck( VECTOR * OldPos , VECTOR * NewPos , TELEPORT * TPpnt )
{

	BOOL	OldIn;
	BOOL	NewIn;
	Entry = FALSE;
	Exit = FALSE;
	In = FALSE;
	if( TPpnt->zone_type == ZONE_Sphere )
	{
   		OldIn = DistanceVector2Vector( &TPpnt->Pos , OldPos ) <= TPpnt->half_size.x;
   		NewIn = DistanceVector2Vector( &TPpnt->Pos , NewPos ) <= TPpnt->half_size.x;
   		if( !OldIn && NewIn )
   			Entry = TRUE;
   		if( OldIn && !NewIn )
   			Exit = TRUE;
   		if( OldIn && NewIn )
   			In = TRUE;
   	}else{
   		RayToHull( TPpnt->Zone , OldPos , NewPos , TPpnt->num_sides );
   	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Start a Teleport..
	Input		:	void
	Output		:	void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void StartTeleport( uint16 * Data )
{
	TELEPORT * TPpnt;
	TPpnt = Teleports;
	if( !TPpnt )
		return;
	TPpnt += *Data;
	TPpnt->Status = TELEPORTACTIVE;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Stop a Teleport..
	Input		:	void
	Output		:	void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void StopTeleport( uint16 * Data )
{
	TELEPORT * TPpnt;
	TPpnt = Teleports;
	if( !TPpnt )
		return;
	TPpnt += *Data;
	TPpnt->Status = TELEPORTINACTIVE;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Save Teleports arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * SaveTeleports( FILE * fp )
{
	int				i;
	TELEPORT	*	TeleportPtr;

	if( fp )
	{
		fwrite( &NumOfTeleports, sizeof( int16 ), 1, fp );

		TeleportPtr = Teleports;

		for( i = 0; i < NumOfTeleports; i++ )
		{
			fwrite( &TeleportPtr->Status, sizeof( uint16 ), 1, fp );
			TeleportPtr++;
		}
	}

	return( fp );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Load Teleports Arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * LoadTeleports( FILE * fp )
{
	int				i;
	TELEPORT	*	TeleportPtr;
	int16			NumTeleports;

	if( fp )
	{
		fread( &NumTeleports, sizeof( int16 ), 1, fp );

		if( NumTeleports != NumOfTeleports )
		{
			fclose( fp );
			return( NULL );
		}

		TeleportPtr = Teleports;

		for( i = 0; i < NumOfTeleports; i++ )
		{
			fread( &TeleportPtr->Status, sizeof( uint16 ), 1, fp );
			TeleportPtr++;
		}
	}

	return( fp );
}
