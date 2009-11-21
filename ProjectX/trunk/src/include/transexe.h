#ifndef TRANSEXE_INCLUDED
#define TRANSEXE_INCLUDED

#include "render.h"
#include "new3d.h"

#define MAXTRANSEXE 128

typedef struct _TRANSEXE{
	int	UseIdentity;
	RENDERMATRIX  Matrix;
	RENDEROBJECT renderObject;
	int16		NumVerts;
	uint16		Model;
	uint16		group;
}TRANSEXE;

extern int16	NumOfTransExe;

/*
 * fn prototypes
 */

void AddTransExe( RENDERMATRIX *Matrix , RENDEROBJECT *renderObject, int UseIdentity, uint16 Model, uint16 group, int16 NumVerts );
void ExecuteTransExe( uint16 group );
void ExecuteTransExeUnclipped( uint16 group );

#endif	// TRANSEXE_INCLUDED
