
#include <stdio.h>
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "Object.h"
#include "mydplay.h"
#include "2dtextures.h"
#include "triggers.h"
#include "pickups.h"
#include "magic.h"

void rp ( char * name , void * size , FILE * fp ) {

}

int parse_pic ( char * filename ) {

  FILE * fp;
  uint32 MagicNumber, VersionNumber;
  int16	NumRegenPoints, Count, tmp_int = 0;
  float tmp_float = 0.0;

  fp = fopen( filename, "rb" );
  if( fp == NULL ) {
    printf("Could not open file!\n");
    return 1;
  }

  fread( &MagicNumber, sizeof( uint32 ), 1, fp );
  printf("magic number\t = %i\n",&MagicNumber);
  if( MagicNumber != MAGIC_NUMBER ) {
    printf("Notice: Magic number not correctly set...\n");
  }

  fread( &VersionNumber, sizeof( uint32 ), 1, fp );
  printf("pic version\t = %i\n",&VersionNumber);
  if( VersionNumber != PIC_VERSION_NUMBER  ) {
    printf("Notice: Pic Version Number not correctly set...\n");
  }

  fread( &NumRegenPoints, sizeof( int16 ), 1, fp );
  printf("Number of regen points\t = %i\n",NumRegenPoints);

  printf("\n");
  for( Count = 0; Count < NumRegenPoints; Count++ ) {

    printf("Pickup:\t %i\n",Count);

    fread( &tmp_int, sizeof( int16 ), 1, fp );
    printf("\tGenType:\t %i\n",tmp_int);

    fread( &tmp_int, sizeof( int16 ), 1, fp );
    printf("\tReGenType:\t %i\n",tmp_float);

    fread( &tmp_float, sizeof( float ), 1, fp );
    printf("\tGenDelay:\t %i\n",tmp_float);

    fread( &tmp_float, sizeof( float ), 1, fp );
    printf("\tLifeSpan:\t %i\n",tmp_float);

    fread( &tmp_float, sizeof( float ), 1, fp );
    printf("\tx:\t %i\n",tmp_float);

    fread( &tmp_float, sizeof( float ), 1, fp );
    printf("\ty:\t %i\n",tmp_float);

    fread( &tmp_float, sizeof( float ), 1, fp );
    printf("\tz:\t %i\n",tmp_float);

    fread( &tmp_int, sizeof( int16 ), 1, fp );
    printf("\tGroup:\t %i\n",tmp_int);

    fread( &tmp_int, sizeof( int16 ), 1, fp );
    printf("\tType:\t %i\n",tmp_int);

    fread( &tmp_int, sizeof( int16 ), 1, fp );
    printf("\tTriggerMod:\t %i\n",tmp_int);

//				if( ( RegenPoints[ Count ].Type >= MAXPICKUPTYPES ) ||
//					( ( RegenPoints[ Count ].TriggerMod != (uint16) -1 ) && ( RegenPoints[ Count ].TriggerMod >= NumOfTrigMods ) ) )
//				{
//					Msg( "Error loading pickups\nIllegal pickup or trigger\n" );
//					fclose( fp );
//					return( FALSE );
//				}

    printf("\n");
	}
  fclose( fp );
  return 0;
}

int main( int argc , char *argv[] ){
  if ( argc < 2 ) {
    printf("Usage: ./%s <filename>\n",argv[0]);
    return 1;
  }
  parse_pic( argv[1] );
  return 0;
}


