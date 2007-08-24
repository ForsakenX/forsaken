/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 10 $
 *
 * $Header: /PcProjectX/Include/D3ddemo.h 10    1/10/97 11:30a Phillipd $
 *
 * $Log: /PcProjectX/Include/D3ddemo.h $
 * 
 * 10    1/10/97 11:30a Phillipd
 * movies are now doable
 * 
 * 9     11/22/96 5:10p Phillipd
 * 
 * 8     11/13/96 9:07a Phillipd
 * All the Menus in the world....And then Some
 * 
 * 7     11/06/96 6:10p Phillipd
 * Got rid of some useless files...
 * 
 * 6     10/28/96 8:50a Phillipd
 * 
 * 5     10/23/96 4:24p Phillipd
 * Lots of crap taken out of D3dapp and its associated functions and
 * files....
 * 
 * 4     10/05/96 7:00p Phillipd
 * Got rid of the extents management from RenderLoop()
 * Removed clearing back buffers from d3dmain.cpp..
 * now done every time a camera view needs to do it..
 * Removed load of / 2.0f from quat.c and made some of the functions
 * __inline
 * 
 * 3     28/06/96 14:10 Oliverc
 * 
 * 2     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

/*==========================================================================
 *
 *  Copyright (C) 1995 Microsoft Corporation. All Rights Reserved.
 *
 *  File: d3ddemo.h
 *
 ***************************************************************************/

#ifndef __D3DDEMO_H__
#define __D3DDEMO_H__

#include <ddraw.h>
#include <d3d.h>
#include "d3dapp.h"
#include "d3dmacs.h"


#ifdef __cplusplus
extern "C" {
#endif
/*
 * These functions are found in the source for each example.
 */
    /*
     * RenderScene
     * Clears the viewport and viewport Z-buffer and render the scene.
     */
    BOOL RenderScene(LPDIRECT3DDEVICE lpDev, LPDIRECT3DVIEWPORT lpView );
    /*
     * InitScene
     * Builds the model which will be rendered.
     */
    BOOL InitScene(void);

    /*
     * InitView
     * Builds execute buffer and all components needed to be added to viewport.
     */
    BOOL InitView(void);
    /*
     * Release all objects and free all memory allocated in InitScene
     */
    void ReleaseScene(void);

    /*
     * Release all objects and free all memory allocated in InitView
     */
    void ReleaseView(void);

    typedef struct Defaultstag {
        D3DAppRenderState rs;
        BOOL bTexturesDisabled;
        BOOL bResizingDisabled;
        BOOL bClearsOn;
        char Name[30];
    } Defaults;

    /*
     * Allows each example to begin with different defaults
     */
    void OverrideDefaults(Defaults* defaults);

    /*
     * Some of our own Functions and some stuff thats usefull externed
     */

	void My_Key_Down ( WPARAM  , LPARAM  );
	int My_Key_Up ( WPARAM  , LPARAM  );
    extern D3DAppInfo* d3dapp;         /* Pointer to read only collection of DD and D3D	*/
	extern	short	ModeCase;

#ifdef __cplusplus
};
#endif

#endif // __D3DDEMO_H__
