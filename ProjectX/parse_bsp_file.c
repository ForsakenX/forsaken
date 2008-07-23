
#include <stdio.h>

#define BSP_VERSION_NUMBER      (1)
#define MAGIC_NUMBER    0x584A5250

// Windows types
typedef int int16;
typedef unsigned int uint16;
typedef long int int32;
typedef unsigned long int uint32;

// Represents a vector
typedef struct VECTOR
{
    float x;
    float y;
    float z;
}VECTOR;


// This represents a single bsp node in binary .BSP file.
// You can use this to cast the binary data in a .BSP file.
typedef struct BSP_RAWNODE
{
        VECTOR  Normal;
        float   Offset;
        int     Front;
        int     Back;
        int     Colour;
}BSP_RAWNODE;

// parse the bsp file

int parse_bsp ( char * filename ) {

  FILE * fp;
  uint32 MagicNumber, VersionNumber;
  int16	NumGroups, NumNodes = 0;
  int GroupCount, NodeCount = 0;

  // open the file

  fp = fopen( filename, "rb" );
  if( fp == NULL ) {
    printf("Could not open file!\n");
    return 1;
  }

  // get the magic number

  fread( &MagicNumber, sizeof( uint32 ), 1, fp );
  printf("magic number\t = %i\n",&MagicNumber);
  if( MagicNumber != MAGIC_NUMBER ) {
    printf("Notice: Magic number not correctly set...\n");
  }

  // get the bsp version number

  fread( &VersionNumber, sizeof( uint32 ), 1, fp );
  printf("pic version\t = %i\n",&VersionNumber);
  if( VersionNumber != BSP_VERSION_NUMBER ){
    printf("Notice: Pic Version Number not correctly set...\n");
  }
  
  // get number of groups

  fread( &NumGroups, sizeof( int16 ), 1, fp );
  printf("Number of Groups\t = %i\n",NumGroups);

  // for each group

  for( GroupCount = 0; GroupCount < NumGroups; GroupCount++ ) {
    printf("\n\n");

    printf("Group:\t %i\n",GroupCount);

    fread( &NumNodes, sizeof( int16 ), 1, fp );
    printf("Nodes:\t %i\n",NumNodes);

    // for each node
    
    for( NodeCount = 0; NodeCount < NumNodes; NodeCount++ ){
      BSP_RAWNODE Raw;

      printf("\n");
      printf("Node:\t %i\n",NodeCount);

      fread( &Raw, sizeof( BSP_RAWNODE ), 1, fp );

      printf("x (%i), y (%i), z (%i)\n",
                    Raw.Normal.x, Raw.Normal.y, Raw.Normal.z);

      printf("Front (%i), Back (%i), Colour (%i)\n",
                    Raw.Front,  Raw.Back,  Raw.Colour);
    }

	}
  fclose( fp );
  return 0;
}

int main( int argc , char *argv[] ){
  if ( argc < 2 ) {
    printf("Usage: ./%s <filename>\n",argv[0]);
    return 1;
  }
  parse_bsp( argv[1] );
  return 0;
}


