

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
#include "trigarea.h"
#include "teleport.h"

#include "sfx.h"
#include "ships.h"
#include "util.h"

/*===================================================================
		Defines
===================================================================*/
#define	TELEPORTS_VERSION_NUMBER	2

																   
/*===================================================================
		Externals ...
===================================================================*/
extern	VECTOR Forward;
extern	VECTOR SlideUp;
extern	float framelag;
extern	bool	Entry;
extern	bool	Exit;
extern	bool	In;
extern	bool	ShowTeleports;
bool RayToHull( TRIGGER_ZONE * StartSide , VECTOR * StartPos , VECTOR * EndPos , u_int16_t StartNumSides );
void DisplayDirAndUp( VECTOR * Pos, VECTOR * Dir, VECTOR *Up, u_int16_t Group );

/*===================================================================
		Globals ...
===================================================================*/
int16_t	NumOfTeleports = 0;

TELEPORT * TeleportsGroupLink[MAXGROUPS];

TELEPORT * Teleports = NULL;

/*===================================================================
	Procedure	:	Teleports load...
	Input		:	char * filename....
	Output		:	bool
===================================================================*/
bool TeleportsLoad( char * Filename )
{
	long			File_Size;
	long			Read_Size;
	char		*	Buffer;
	char		*	OrgBuffer;
	u_int16_t		*	Uint16Pnt;
	TELEPORT * TPpnt;
	float * floatpnt;
	int i,j,e;
	TRIGGER_ZONE * ZonePnt;
	u_int32_t			MagicNumber;
	u_int32_t			VersionNumber;
	u_int32_t		*	u_int32Pnt;


	NumOfTeleports = 0;
	for( i = 0 ; i < MAXGROUPS ; i++ )
	{
		TeleportsGroupLink[i] = NULL;
	}

	
	File_Size = Get_File_Size( Filename );	
	if( !File_Size )
	{
		// dont need Teleports..
		return true;
	}
	Buffer = malloc( File_Size );
	OrgBuffer = Buffer;

	if( !Buffer )
	{
		Msg( "Teleports Load : Unable to allocate file buffer %s\n", Filename );
		return( false );
	}
	Read_Size = Read_File( Filename, Buffer, File_Size );
	if( Read_Size != File_Size )
	{
		Msg( "Teleports Load Error reading %s\n", Filename );
		return( false );
	}


	u_int32Pnt = (u_int32_t *) Buffer;
	MagicNumber = *u_int32Pnt++;
	VersionNumber = *u_int32Pnt++;
	Buffer = (char *) u_int32Pnt;

	if( ( MagicNumber != MAGIC_NUMBER ) || ( VersionNumber != TELEPORTS_VERSION_NUMBER  ) )
	{
		Msg( "Teleportload() Incompatible Teleports( .tel ) file %s", Filename );
		return( false );
	}

	
	
	Uint16Pnt = (u_int16_t *) Buffer;
	NumOfTeleports = *Uint16Pnt++;
	Buffer = (char *) Uint16Pnt;

	Teleports	= (TELEPORT *) malloc( NumOfTeleports * sizeof(TELEPORT) );
	TPpnt = Teleports;
	if( !TPpnt )
	{
		Msg( "Teleports : cant allocate buffer %s\n", Filename );
		return( false );
	}

	for( i = 0 ; i < NumOfTeleports ; i++ )
	{
		Uint16Pnt = (u_int16_t *) Buffer;
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
			return( false );
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
  
		Uint16Pnt = (u_int16_t *) Buffer;
		TPpnt->zone_type = *Uint16Pnt++;
		Buffer = (char *) Uint16Pnt;
		
		if( TPpnt->zone_type != ZONE_Sphere )
		{
			// convex hull...
			Uint16Pnt = (u_int16_t *) Buffer;
			TPpnt->num_sides = *Uint16Pnt++;
			Buffer = (char *) Uint16Pnt;
			
			TPpnt->Zone = (TRIGGER_ZONE*) malloc( TPpnt->num_sides * sizeof( TRIGGER_ZONE ) );
			ZonePnt = TPpnt->Zone;
			
			if( !ZonePnt )
			{
				Msg( "Teleport maloc Error with %s\n", Filename );
				return( false );
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

	return true;
}
/*===================================================================
	Procedure	:	Release Forces load...
	Input		:	void
	Output		:	void
===================================================================*/
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



/*===================================================================
	Procedure	:	Check if im in an Active Teleport....
	Input		:	void
	Output		:	true/false
===================================================================*/
bool TeleportsAreaCheck( VECTOR * NewPos , VECTOR * OldPos ,u_int16_t Group, OBJECT *obj )
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
				return true;
			}
		}
		TPpnt = TPpnt->NextInGroup;
	}
	return false;
}

/*===================================================================
	Procedure	:	Display Teleports in group
	Input		:	u_int16_t	Group
	Output		:	Nothing
===================================================================*/
void DisplayTeleportsInGroup( u_int16_t Group )
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


/*===================================================================
	Procedure	:	Check if im in a Teleport Zone....
	Input		:	void
	Output		:	void
===================================================================*/
void TeleportsZoneCheck( VECTOR * OldPos , VECTOR * NewPos , TELEPORT * TPpnt )
{

	bool	OldIn;
	bool	NewIn;
	Entry = false;
	Exit = false;
	In = false;
	if( TPpnt->zone_type == ZONE_Sphere )
	{
   		OldIn = DistanceVector2Vector( &TPpnt->Pos , OldPos ) <= TPpnt->half_size.x;
   		NewIn = DistanceVector2Vector( &TPpnt->Pos , NewPos ) <= TPpnt->half_size.x;
   		if( !OldIn && NewIn )
   			Entry = true;
   		if( OldIn && !NewIn )
   			Exit = true;
   		if( OldIn && NewIn )
   			In = true;
   	}else{
   		RayToHull( TPpnt->Zone , OldPos , NewPos , TPpnt->num_sides );
   	}
}


/*===================================================================
	Procedure	:	Start a Teleport..
	Input		:	void
	Output		:	void
===================================================================*/
void StartTeleport( u_int16_t * Data )
{
	TELEPORT * TPpnt;
	TPpnt = Teleports;
	if( !TPpnt )
		return;
	TPpnt += *Data;
	TPpnt->Status = TELEPORTACTIVE;
}
/*===================================================================
	Procedure	:	Stop a Teleport..
	Input		:	void
	Output		:	void
===================================================================*/
void StopTeleport( u_int16_t * Data )
{
	TELEPORT * TPpnt;
	TPpnt = Teleports;
	if( !TPpnt )
		return;
	TPpnt += *Data;
	TPpnt->Status = TELEPORTINACTIVE;
}


/*===================================================================
	Procedure	:	Save Teleports arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * SaveTeleports( FILE * fp )
{
	int				i;
	TELEPORT	*	TeleportPtr;

	if( fp )
	{
		fwrite( &NumOfTeleports, sizeof( int16_t ), 1, fp );

		TeleportPtr = Teleports;

		for( i = 0; i < NumOfTeleports; i++ )
		{
			fwrite( &TeleportPtr->Status, sizeof( u_int16_t ), 1, fp );
			TeleportPtr++;
		}
	}

	return( fp );
}

/*===================================================================
	Procedure	:	Load Teleports Arrays & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
===================================================================*/
FILE * LoadTeleports( FILE * fp )
{
	int				i;
	TELEPORT	*	TeleportPtr;
	int16_t			NumTeleports;

	if( fp )
	{
		fread( &NumTeleports, sizeof( int16_t ), 1, fp );

		if( NumTeleports != NumOfTeleports )
		{
			fclose( fp );
			return( NULL );
		}

		TeleportPtr = Teleports;

		for( i = 0; i < NumOfTeleports; i++ )
		{
			fread( &TeleportPtr->Status, sizeof( u_int16_t ), 1, fp );
			TeleportPtr++;
		}
	}

	return( fp );
}
