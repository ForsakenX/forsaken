/*
 * t r a n s e x e .h
 *
 * Translucent exexcute Routines
 *
 * Project X, July 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 6 $
 *
 * $Header: /PcProjectX/Include/Transexe.h 6     24-03-97 12:54p Collinsd $
 *
 * $Log: /PcProjectX/Include/Transexe.h $
 * 
 * 6     24-03-97 12:54p Collinsd
 * changed detail level of explosion.  Changed process model function
 * 
 * 5     4/11/96 10:45 Oliverc
 * Changed display routines to clip to visible portal boundaries of each
 * group
 * 
 * 4     17/10/96 14:43 Collinsd
 * Explosion and gravgon effects now work properly.
 * 
 * 3     8/29/96 5:41p Phillipd
 * 
 * 2     7/29/96 12:16p Phillipd
 * 
 * 1     7/29/96 11:25a Phillipd
 * 
 */

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