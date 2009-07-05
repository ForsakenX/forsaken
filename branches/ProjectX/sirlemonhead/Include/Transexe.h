

#ifndef TRANSEXE_INCLUDED
#define TRANSEXE_INCLUDED

#include "typedefs.h"
#include "d3dmain.h"
#include "d3ddemo.h"
#include "d3dappi.h"
#include "New3D.h"


#define MAXTRANSEXE 128

typedef struct _TRANSEXE{
	 int	UseIdentity;
	 D3DMATRIX  Matrix;
	LPDIRECT3DEXECUTEBUFFER	lpExBuf;
	int16		NumVerts;
	uint16		Model;
	uint16		group;
}TRANSEXE;

extern int16	NumOfTransExe;

/*
 * fn prototypes
 */

void AddTransExe( LPD3DMATRIX Matrix , LPDIRECT3DEXECUTEBUFFER lpExBuf , int UseIdentity, uint16 Model, uint16 group, int16 NumVerts );
void ExecuteTransExe( uint16 group );
void ExecuteTransExeUnclipped( uint16 group );

#endif	// TRANSEXE_INCLUDED