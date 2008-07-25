
// creates a cube map!

#include <stdio.h>

typedef	unsigned char		uint8;			/* Unsigned 8 Bit Integer  */
typedef	char				int8;			/* Signed 8 Bit Integer    */
typedef	unsigned short int	uint16;			/* Unsigned 16 Bit Integer */
typedef	short int			int16;			/* Signed 16 bit Integer   */
typedef	unsigned long		uint32;			/* Unsigned 32 Bit Integer */
typedef	long				int32;			/* Signed 32 Bit Integer   */

// Represents a vector
typedef struct VECTOR {
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

int create_bsp ( char * filename ) {

  int color = -16777153;

  // open the file

  FILE * fp;
  fp = fopen( filename, "w" );
  if( fp == NULL ) {
    printf("Could not open file!\n");
    return 1;
  }

  // magic number

  fputs( "PRJX", fp );

  // bsp version number

  {
    uint32 version = 1;
    fwrite( &version, sizeof( uint32 ), 1, fp );
  }
  
  // number of groups

  {
    int16 groups = 1;
    fwrite( &groups, sizeof( int16 ), 1, fp );
  }

  // number of nodes

  {
    int16 nodes  = 6; // 6 sides to a cube
    fwrite( &nodes, sizeof( int16 ), 1, fp );
  }

  // node 0 (root)

  {
    BSP_RAWNODE node;
    node.Normal.x = 0.0;
    node.Normal.y = 0.0;
    node.Normal.z = -1.0;
    node.Offset   = 1024.0;
    node.Front    = 1;
    node.Back     = 0;
    node.Colour   = color;
    fwrite( &node, sizeof( node ), 1, fp );
  }

  // node 1

  {
    BSP_RAWNODE node;
    node.Normal.x = 1.0;
    node.Normal.y = 0.0;
    node.Normal.z = 0.0;
    node.Offset   = 1024.0;
    node.Front    = 2;
    node.Back     = 0;
    node.Colour   = color;
    fwrite( &node, sizeof( node ), 1, fp );
  }

  // node 2

  {
    BSP_RAWNODE node;
    node.Normal.x = 0.0;
    node.Normal.y = 0.0;
    node.Normal.z = 1.0;
    node.Offset   = 1024.0;
    node.Front    = 3;
    node.Back     = 0;
    node.Colour   = color;
    fwrite( &node, sizeof( node ), 1, fp );
  }

  // node 3

  {
    BSP_RAWNODE node;
    node.Normal.x = 0.0;
    node.Normal.y = -1.0;
    node.Normal.z = 0.0;
    node.Offset   = 1024.0;
    node.Front    = 4;
    node.Back     = 0;
    node.Colour   = color;
    fwrite( &node, sizeof( node ), 1, fp );
  }

  // node 4

  {
    BSP_RAWNODE node;
    node.Normal.x = 0.0;
    node.Normal.y = 1.0;
    node.Normal.z = 0.0;
    node.Offset   = 1024.0;
    node.Front    = 5;
    node.Back     = 0;
    node.Colour   = color;
    fwrite( &node, sizeof( node ), 1, fp );
  }

  // node 5

  {
    BSP_RAWNODE node;
    node.Normal.x = -1.0;
    node.Normal.y = 0.0;
    node.Normal.z = 0.0;
    node.Offset   = 1024.0;
    node.Front    = 0;
    node.Back     = 0;
    node.Colour   = color;
    fwrite( &node, sizeof( node ), 1, fp );
  }

  // finish up

  fclose( fp );
  return 0;
}

int main( int argc , char *argv[] ){
  if ( argc < 2 ) {
    printf("Usage: ./%s <filename>\n",argv[0]);
    return 1;
  }
  create_bsp( argv[1] );
  return 0;
}


