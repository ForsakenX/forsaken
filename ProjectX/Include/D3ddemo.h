
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
