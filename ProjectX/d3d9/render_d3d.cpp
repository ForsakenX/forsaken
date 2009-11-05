
#define DIRECT3D_VERSION    0x0900

#include <d3dx9.h>

extern "C" {
#include "typedefs.h"
#include "render.h"
#include "tload.h"
#include "new3d.h"
#include <assert.h>
#include "util.h"
#include <dxerr9.h>
#include "file.h"
#include "texture.h"

// prototypes
static BOOL init_render_states( render_info_t * info );

// d3d caps
// D3DPTEXTURECAPS_SQUAREONLY
BOOL  bSquareOnly = FALSE;

// Build a Gamma Correction table
BYTE  gamma_lookup[256];
void render_gamma_correction( double gamma )
{
	double k;
	int i;

	// recover in release build
	if (gamma <= 0)
	    gamma = 1.0;
	
	k = 255.0/pow(255.0, 1.0/gamma);
	
	for (i = 0; i <= 255; i++)
	{
	    gamma_lookup[i] = (BYTE)(k*(pow((double)i, 1.0/gamma)));
		if( i )
		{
			if( !gamma_lookup[i] )
				gamma_lookup[i] = 1;
		}
	}
};

BOOL render_initialized = FALSE;

LPDIRECT3D9			lpD3D; /* D3D interface object */
LPDIRECT3DDEVICE9	lpD3DDevice;	/* D3D device */

BOOL init_renderer( render_info_t * info )
{
	HRESULT LastError;
	render_viewport_t viewport;
	int bpp = 16;

	// default gamma table
	render_gamma_correction(1.0);

	// Set up Direct3D interface object
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!lpD3D)
	{
		DebugPrintf("couldnt create d3d9\n");
		return FALSE;
	}

	// create d3d device
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory (&d3dpp, sizeof(d3dpp));

	//
	d3dpp.Windowed = !info->bFullscreen;

	//
	// presentation settings
	//

	d3dpp.hDeviceWindow					= info->window;					// the window handle
	d3dpp.BackBufferCount				= 1;							// we only have one swap chain
    d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// does not protect the contents of the backbuffer after flipping (faster)
																		// shouldn't we specify D3DSWAPEFFECT_FLIP ?
																		// wouldn't D3DSWAPEFFECT_OVERLAY be fastest ?
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// display refresh
	d3dpp.EnableAutoDepthStencil		= TRUE;							// let d3d manage the z-buffer

	if(info->vsync)	
		d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_ONE;			// enable vsync
	else
		d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;	// disable vsync

	// 16 bit zbuffer
	//d3dpp.AutoDepthStencilFormat	= D3DFMT_D15S1; // 16 bit depth buffer with 1 bit stencil
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;	// 16 bit depth buffer

	// 32 bit back buffer
	// Also supports 32 bit zbuffer
	if( info->default_mode.bpp >= 32 && 
		SUCCEEDED(lpD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, d3dpp.Windowed)))
	{
		bpp = 32;
		d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
		//d3dpp.AutoDepthStencilFormat	= D3DFMT_D32;	// 32 bit depth buffer
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;	// 24 bit depth buffer with 8 bit stencil buffer
		DebugPrintf("picked 32 bit textures\n");
	}
	// 16 bit
	else if(SUCCEEDED(lpD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X1R5G5B5, D3DFMT_X1R5G5B5, d3dpp.Windowed)))
	{
		d3dpp.BackBufferFormat	= D3DFMT_X1R5G5B5;
		DebugPrintf("picked 16 bit textures\n");
	}
	// 16 bit 
	else if(SUCCEEDED(lpD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5, D3DFMT_R5G6B5, d3dpp.Windowed)))
	{
		d3dpp.BackBufferFormat	= D3DFMT_R5G6B5;
		DebugPrintf("picked 16 bit textures\n");
	}
	// failed
	else
	{
		CloseWindow(info->window);
		Msg("Failed to find a suitable back buffer format");
		exit(1);
	}

	//
	// Enumerates display modes 
	// picking info->default_mode if it exists
	// or picking the biggest mode possible :]
	//

	{
		int mode = 0;
		int desired_mode = -1;
		int best_mode = 0; // default to the first mode
		int i;
		int x = 0;
		int count				=  (int) lpD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat );
		info->Mode			= (render_display_mode_t *) malloc( count * sizeof(render_display_mode_t) );
		D3DDISPLAYMODE * modes	= (D3DDISPLAYMODE *) malloc( count * sizeof(D3DDISPLAYMODE) );
		for ( i = 0; i < count; i++ )
		{
			// get the mode description
			lpD3D->EnumAdapterModes( D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat, i, &modes[i] );
			DebugPrintf("Enumerated mode: %dx%d @ %dhz , format=%d\n",
				modes[i].Width,modes[i].Height,modes[i].RefreshRate,modes[i].Format);

			// ignore modes under 640x480
			if(modes[i].Width < 640 || modes[i].Height < 480)
			{
				DebugPrintf("Skipping mode because it's to small for anyone to want...\n");
				continue;
			}

			// save the mode
			info->Mode[x].h    = modes[i].Height;
			info->Mode[x].w    = modes[i].Width;
			info->Mode[x].bpp  = bpp;
			info->Mode[x].rate = modes[i].RefreshRate;

			// if this is the mode the user wanted pick it
			if(	info->Mode[x].w == info->default_mode.w &&
				info->Mode[x].h == info->default_mode.h &&
				info->Mode[x].rate == info->default_mode.rate )
			{
				desired_mode = x;
			}
			
			// smallest mode as default
			if( info->Mode[x].w < info->Mode[best_mode].w && 
				info->Mode[x].h < info->Mode[best_mode].h && 
				info->Mode[x].rate < info->Mode[best_mode].rate )
			{
				best_mode = x;
			}

			// biggest mode by width as default
			//if(info->Mode[x].w > info->Mode[best_mode].w && info->Mode[x].rate == 60)
			//	best_mode = x;

			// 800x600 @ 60 as default
			//if(	info->Mode[x].w == 800 && info->Mode[x].h == 600 && info->Mode[x].rate == 60 )
			//	desired_mode = x;

			// go to next storage location
			x++;
		
		}
		info->NumModes = x;
		if( desired_mode >= 0 )
		{
			mode = desired_mode;
		}
		else
		{
			mode = best_mode;
		}
		info->CurrMode = mode;
		info->ThisMode = info->Mode[ info->CurrMode ];
		info->WindowsDisplay = info->Mode[ info->CurrMode ];
		d3dpp.BackBufferWidth   = info->ThisMode.w;
		d3dpp.BackBufferHeight  = info->ThisMode.h;
		info->szClient.cx		= info->ThisMode.w;
		info->szClient.cy		= info->ThisMode.h;
		info->WindowsDisplay.w  = info->ThisMode.w;
		info->WindowsDisplay.h  = info->ThisMode.h;
		free(modes);
	}

	DebugPrintf("Using display mode: %dx%dx%d @ %dhz\n",
		info->ThisMode.w,info->ThisMode.h,info->ThisMode.bpp,info->ThisMode.rate);

	// window mode
	if ( ! info->bFullscreen )
	{
		SetWindowPos( 
			info->window,		// the window handle
			HWND_TOP,			// bring window to the front
			0, 0,				// top left of screen
			d3dpp.BackBufferWidth, d3dpp.BackBufferHeight, // size of viewport
			SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_SHOWWINDOW 
		);
	}

	// try to create a device falling back to less capable versions

	//
	// TODO
	// the docs say that pure devices don't support Get* calls
	// which explains why people are crashing on FSGet* functions for no reason
	// to support pure device we'd have to track our own states
	//

	/*
	LastError = lpD3D->CreateDevice(
		D3DADAPTER_DEFAULT,							// the default video card 
		D3DDEVTYPE_HAL,								// device type - hal = Hardware rasterization
		info->window,							// the window handle

		// these define how the device is created

		D3DCREATE_HARDWARE_VERTEXPROCESSING |		// do vertex processing in hardware
		D3DCREATE_PUREDEVICE, 						// D3D will not support Get* calls for anything that can be stored in state blocks.
													// and not to provide any emulation services for vertex processing.
													// Thus if the device does not support vertex processing,
													// then the application can use only post-transformed vertices.

		&d3dpp,										// presentation parameters defined above
		&lpD3DDevice						// pointer that will contain the returned device
	);

	if (SUCCEEDED(LastError))
	{
		Msg("pure");
	}

	if (FAILED(LastError)) 
	*/
	{
		LastError = lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, info->window,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice);
		if (SUCCEEDED(LastError))
		{
			DebugPrintf("d3d device created: hardware\n");
		}
		else
		{
			const char * error = render_error_description(LastError);
			DebugPrintf("d3d device create failed with hardware: %s\n",error);
		}
	}

	if (FAILED(LastError)) 
	{
		LastError = lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, info->window,
			D3DCREATE_MIXED_VERTEXPROCESSING,		// do vertex processing in both hardware and software
			&d3dpp, &lpD3DDevice);
		if (SUCCEEDED(LastError))
		{
			DebugPrintf("d3d device created: mixed\n");
		}
		else
		{
			const char * error = render_error_description(LastError);
			DebugPrintf("d3d device create failed with mixed: %s\n",error);
		}
	}

	if (FAILED(LastError)) 
	{
		LastError = lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, info->window,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,	// do vertex processing in software only
			&d3dpp, &lpD3DDevice);
		if (SUCCEEDED(LastError))
		{
			DebugPrintf("d3d device created: software\n");
		}
		else
		{
			const char * error = render_error_description(LastError);
			DebugPrintf("d3d device create failed with software: %s\n",error);
		}
	}

	if (FAILED(LastError))
	{
		CloseWindow(info->window);
		Msg("Failed to create a suitable d3d device:\n%s",
			render_error_description(LastError));
		exit(1);
	}

	render_initialized = TRUE;
	info->bRenderingIsOK = TRUE;

	viewport.X = 0;
	viewport.Y = 0;
	/* do "after device created" stuff */
	ZeroMemory( &viewport, sizeof(viewport) );
	{
		WINDOWPLACEMENT placement;
		placement.length = sizeof(WINDOWPLACEMENT);
		if(GetWindowPlacement( info->window, &placement ))
		{
			viewport.X = placement.rcNormalPosition.left;
			viewport.Y = placement.rcNormalPosition.top;
		}
	}
	viewport.Width = d3dpp.BackBufferWidth;
	viewport.Height = d3dpp.BackBufferHeight;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;

	LastError = FSSetViewPort(&viewport);
	if (FAILED(LastError))
	{
		DebugPrintf("couldn't set viewport\n");
	}

    if(!init_render_states( info ))
		return FALSE;

	return TRUE;
}

void render_cleanup( render_info_t * info )
{
    info->bRenderingIsOK = FALSE;
	if(info->Mode)
		free(info->Mode);
    RELEASE(lpD3DDevice);
	RELEASE(lpD3D);
}

BOOL render_mode_select( render_info_t * info )
{
	render_cleanup( info );
	if(!init_renderer( info ))
		return FALSE;
	return TRUE;
}

// If the device can be restored,
// the application prepares the device by destroying all video-memory 
// resources and any swap chains.

// Then, the application calls the IDirect3DDevice9::Reset method.
// Reset is the only method that has an effect when a device is lost,
// and is the only method by which an application can change the device
// from a lost to an operational state.

// Reset will fail unless the application releases all resources that
// are allocated in D3DPOOL_DEFAULT, including those created by the 
// The application can determine what to do on encountering a lost device by querying the return
// value of the IDirect3DDevice9::TestCooperativeLevel method.

// All video memory must be released before a device can be reset from a lost state to an operational state.
// This means that the application should release any resources placed in the D3DPOOL_DEFAULT memory class.

// TODO - right now we just call render_reset which creates a new device
// * does the things we didn't release get released when the device gets destroyed?
// * fullscreen to window mode is completely broken

static BOOL needs_reset = FALSE;

BOOL render_reset( render_info_t * info )
{
	if(!needs_reset)
		return FALSE;
	if(!render_mode_select( info ))
		return FALSE;
	needs_reset = FALSE;
	return TRUE;
}

BOOL render_flip( render_info_t * info )
{
	HRESULT hr;
	if (!info->bRenderingIsOK) 
	{
		DebugPrintf("Cannot call render_flip() while bRenderingIsOK is FALSE.\n");
		return FALSE;
	}
	hr = lpD3DDevice->Present(NULL, NULL, NULL, NULL);
	if(FAILED(hr))
	{
		DebugPrintf("render_flip Error: %s\n",render_error_description(hr));
		switch(hr)
		{
		case D3DERR_DEVICELOST:
			needs_reset = TRUE;
			break;
		case D3DERR_DEVICENOTRESET:
			DebugPrintf("render_flip Stub: Device needs to be reset.\n");
			break;
		}
	}
	return TRUE;
}

#define STATE( K, V ) \
	lpD3DDevice->SetRenderState( K, V );

#define TSTATE( N, K, V ) \
	lpD3DDevice->SetTextureStageState( N, K, V );

#define SSTATE( N, K, V ) \
	lpD3DDevice->SetSamplerState( N, K, V );

// c helper
void render_state( D3DRENDERSTATETYPE type, int val )
{
	lpD3DDevice->SetRenderState( type, val );
}

void set_trans_state_3( void )
{
	STATE(D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA);
	STATE(D3DRS_DESTBLEND,	D3DBLEND_SRCALPHA);
}

void set_trans_state_2( void )
{
	STATE(D3DRS_SRCBLEND,	D3DBLEND_ONE);
	STATE(D3DRS_DESTBLEND,	D3DBLEND_ONE);
}

void set_trans_state_9( void )
{
	STATE(D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA);
	STATE(D3DRS_DESTBLEND,	D3DBLEND_ONE);
}

void set_trans_state_5( void )
{
	STATE(D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA);
	STATE(D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA);
}

// transparency tester
void render_state_trans( void )
{
	static int state = 0;
	static int count = 0;

	/*
    D3DBLEND_ZERO               = 1,
    D3DBLEND_ONE                = 2,
    D3DBLEND_SRCCOLOR           = 3,
    D3DBLEND_INVSRCCOLOR        = 4,
    D3DBLEND_SRCALPHA           = 5,
    D3DBLEND_INVSRCALPHA        = 6,
    D3DBLEND_DESTALPHA          = 7,
    D3DBLEND_INVDESTALPHA       = 8,
    D3DBLEND_DESTCOLOR          = 9,
    D3DBLEND_INVDESTCOLOR       = 10,
    D3DBLEND_SRCALPHASAT        = 11,
    D3DBLEND_BOTHSRCALPHA       = 12,
    D3DBLEND_BOTHINVSRCALPHA    = 13,
	*/

	// set the state
	switch( state )
	{
	case 0:
		STATE(D3DRS_SRCBLEND,	D3DBLEND_ZERO);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_ONE);
		break;
	case 1:
		STATE(D3DRS_SRCBLEND,	D3DBLEND_SRCCOLOR);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_INVDESTCOLOR);
		break;
	case 2:
		STATE(D3DRS_SRCBLEND,	D3DBLEND_ONE);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_ONE);
		break;
	case 3:
		STATE(D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_SRCALPHA);
		break;
	case 4:
		STATE(D3DRS_SRCBLEND,	D3DBLEND_INVSRCALPHA);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_SRCALPHA);
		break;
	case 5: // normal trans
		STATE(D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA);
		break;
	// new modes
	case 6:
		STATE(D3DRS_SRCBLEND,	D3DBLEND_ONE);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_ZERO);
		break;
	case 7: // color trans
		STATE(D3DRS_SRCBLEND,	D3DBLEND_ZERO);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_SRCCOLOR);
		break;
	case 8: // inv color trans
		STATE(D3DRS_SRCBLEND,	D3DBLEND_ZERO);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_INVSRCCOLOR);
		break;
	case 9: // glowing
		STATE(D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_ONE);
		break;
	case 10: // darkening color
		STATE(D3DRS_SRCBLEND,	D3DBLEND_INVDESTCOLOR);
		STATE(D3DRS_DESTBLEND,	D3DBLEND_ZERO);
		break;
	}
	DebugPrintf("trans state = %d ...... count %d\n",state,count);

	//render_flip();

	// try the next state
	count++;
	if( count > 100 )	
	{
		count = 0;
		state++;
		if( state > 10 )
			state = 0;
	}
}

// set transparency off
void reset_trans( void )
{
	STATE(	D3DRS_ALPHABLENDENABLE,		FALSE );
	STATE(	D3DRS_SRCBLEND,				D3DBLEND_ONE);
	STATE(	D3DRS_DESTBLEND,			D3DBLEND_ZERO);
}

void reset_zbuff( void )
{
	STATE(	D3DRS_ZENABLE,			D3DZB_TRUE);
	STATE(	D3DRS_ZWRITEENABLE,		TRUE);
	STATE(	D3DRS_ZFUNC,			D3DCMP_LESS);
}

void disable_zbuff_write( void )
{
	STATE(	D3DRS_ZWRITEENABLE,		FALSE);
}

void disable_zbuff( void )
{
	STATE(	D3DRS_ZENABLE,			D3DZB_FALSE);
}

void linear_filtering( void )
{
	SSTATE(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	SSTATE(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
}

void anisotropic_filtering( void )
{
	SSTATE(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );
	SSTATE(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );
}

void disable_filtering( void )
{
	SSTATE(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );
	SSTATE(0, D3DSAMP_MINFILTER, D3DTEXF_NONE );
}

// d3d default filtering settings
void default_filtering( void ){
	SSTATE(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	SSTATE(0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
}

// reset to our default filtering modes
void reset_filtering( void )
{
	anisotropic_filtering();
	SSTATE(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

void cull_none( void )
{
	STATE(	D3DRS_CULLMODE,	D3DCULL_NONE);
}

void cull_cw( void )
{
	STATE(	D3DRS_CULLMODE,	D3DCULL_CW);
}

void reset_cull( void )
{
	STATE(	D3DRS_CULLMODE,	D3DCULL_CCW);
}

void set_alpha_ignore( void )
{
	STATE( D3DRS_ALPHATESTENABLE,	TRUE); 
	STATE( D3DRS_ALPHAREF,			(DWORD)100 );
	STATE( D3DRS_ALPHAFUNC,			D3DCMP_GREATER);
}

void unset_alpha_ignore( void )
{
	STATE( D3DRS_ALPHATESTENABLE,	FALSE); 
}

// old lpD3DNormCmdBuf
void set_normal_states( void )
{
	reset_zbuff();
	reset_trans();
	// need to find equivalants
	//STATE( D3DRS_WRAPU, 				FALSE				);
	//STATE( D3DRS_WRAPV, 				FALSE				);
	//STATE( D3DRS_STIPPLEDALPHA,		FALSE				);
	//STATE( D3DRS_TEXTUREMAPBLEND, 	D3DTBLEND_MODULATE	);
}

// old lpD3DTransCmdBuf
void set_alpha_states( void )
{
	disable_zbuff_write();
	STATE( D3DRS_ALPHABLENDENABLE, TRUE );
	set_trans_state_9();
}

// old lpD3DSpcFxTransCmdBuf
void set_alpha_fx_states( void )
{
    STATE( D3DRS_ALPHABLENDENABLE, TRUE );
	// need some type of states here that would create white out affect
}

/* bjd - CHECK

// game attempted to use these modes in order
// first one that was fully supported was used

mode	D3DPTBLENDCAPS_MODULATE = D3DTBLEND_MODULATE;
src		D3DPBLENDCAPS_ONE		= D3DBLEND_ONE; 
dest	D3DPBLENDCAPS_ONE		= D3DBLEND_ONE;

mode	D3DPTBLENDCAPS_MODULATEALPHA = D3DTBLEND_MODULATEALPHA;
src		D3DPBLENDCAPS_SRCALPHA		 = D3DBLEND_SRCALPHA; 
dest	D3DPBLENDCAPS_SRCALPHA		 = D3DBLEND_SRCALPHA;

mode	D3DPTBLENDCAPS_ADD			 = D3DTBLEND_ADD;
src		D3DPBLENDCAPS_INVSRCALPHA	 = D3DBLEND_INVSRCALPHA; 
dest	D3DPBLENDCAPS_SRCALPHA		 = D3DBLEND_SRCALPHA;

mode	D3DPTBLENDCAPS_MODULATEALPHA	= D3DTBLEND_MODULATEALPHA;
src		D3DPBLENDCAPS_SRCALPHA			= D3DBLEND_SRCALPHA; 
dest	D3DPBLENDCAPS_INVSRCALPHA		= D3DBLEND_INVSRCALPHA;

*/

static BOOL init_render_states( render_info_t * info )
{
	//STATE( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	STATE(	D3DRS_SHADEMODE,		D3DSHADE_GOURAUD );
	STATE(	D3DRS_LIGHTING,			FALSE);

	if( info->ThisMode.bpp < 32 )
		STATE( D3DRS_DITHERENABLE, TRUE );

	reset_cull();
	reset_trans();
	reset_filtering();

	set_normal_states();

	return TRUE;
}

char buf[100];
HRESULT LastError;

BOOL FSClear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	if (FAILED(lpD3DDevice->Clear( Count, pRects, Flags, Color, Z, Stencil )))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL FSClearBlack(void)
{
	if (FAILED(lpD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, FSColourKeyBlack, 1.0f, 0 )))
	{
		return FALSE;
	}
	return TRUE;
}

HRESULT FSGetViewPort(render_viewport_t *returnViewPort)
{
	return lpD3DDevice->GetViewport( (D3DVIEWPORT9*) returnViewPort );
}

HRESULT FSSetViewPort(render_viewport_t *newViewPort)
{
	return lpD3DDevice->SetViewport( (D3DVIEWPORT9*) newViewPort );
}

HRESULT FSSetMatrix(D3DTRANSFORMSTATETYPE type, const D3DMATRIX *matrix)
{
	return lpD3DDevice->SetTransform(type, matrix);
}

HRESULT FSGetMatrix(D3DTRANSFORMSTATETYPE type, D3DMATRIX *matrix)
{
	return lpD3DDevice->GetTransform(type, matrix);
}

HRESULT FSSetMaterial(const D3DMATERIAL9 *material)
{
	return lpD3DDevice->SetMaterial(material);
}

HRESULT FSBeginScene()
{
	return lpD3DDevice->BeginScene();
}

HRESULT FSEndScene()
{
	return lpD3DDevice->EndScene();
}

void save_texture( char * path, LPDIRECT3DTEXTURE9 texture )
{
#ifndef __WINE__
	D3DXSaveTextureToFile(path, D3DXIFF_PNG, texture, 0);
#endif
}

extern double Gamma;
HRESULT create_texture(LPDIRECT3DTEXTURE9 *texture, const char *path, uint16 *width, uint16 *height, int numMips, BOOL * colorkey, D3DPOOL pool)
{
	HRESULT hr;
	texture_image_t image;

	Change_Ext( path, image.path, ".PNG" );
	if( ! File_Exists( (char*) image.path ) )
	{
		DebugPrintf("Could not find texture file: %s\n",path);
		return TRUE;
	}

	// at this point i have mips turned off for the commit
	// cause it appears that mip levels after 0 are not present
	// i think we need to copy each successive mipmap in at a time
	numMips = 1;

	// load the image
	if(load_image( &image, numMips )!=0)
	{
		DebugPrintf("couldn't load image\n");
		return S_FALSE;
	}

	// create d3d texture
	hr = lpD3DDevice->CreateTexture( image.w, image.h, numMips, 0, D3DFMT_A8R8G8B8, pool, texture, NULL );
	if (FAILED(hr))
	{
		DebugPrintf("couldn't create texture\n");
		return hr;
	}

	// copy data into the texture
	{
		int y, x;
		int size = 4;
		int pitch = size*image.w;
		D3DLOCKED_RECT lrect;
		BYTE* pBits;
		LPDIRECT3DTEXTURE9 _texture = *texture;
		_texture->LockRect(0,&lrect,NULL,D3DLOCK_DISCARD);
		pBits = (BYTE*)lrect.pBits;
		for (y = 0; y < image.h; y++)
		{
			for (x = 0; x < image.w; x++)
			{
				// move to the correct offset in the table
				// y is the row and pitch is the size of a row
				// (x*size) is the length of each pixel data (column)
				DWORD index = (y*pitch)+(x*size);
				// d3d has it's own pitch value cause it stores data at the end of the row
				DWORD index_p = (y*lrect.Pitch)+(x*size);
				// D3DFMT_A8R8G8B8 data will be accessible backwards: bgra
				// image.data is packed in rgba
				pBits[index_p]   = (BYTE)gamma_lookup[image.data[index+2]]; // blue
				pBits[index_p+1] = (BYTE)gamma_lookup[image.data[index+1]]; // green
				pBits[index_p+2] = (BYTE)gamma_lookup[image.data[index]];   // red
				// colour key
				if( image.colorkey && (pBits[index_p] + pBits[index_p+1] + pBits[index_p+2]) == 0 )
					pBits[index_p+3] = 0; // alpha - pixel will not be coppied do to alpha=0 ignore
				// do not colour key
				else
					pBits[index_p+3] = (BYTE)gamma_lookup[image.data[index+3]]; // alpha

			}
		}
		_texture->UnlockRect(0);
	}

	// return the values
	*width  = (uint16) image.w;
	*height = (uint16) image.h;
	(*colorkey) = (BOOL) image.colorkey;

	// info
	DebugPrintf( "Created texture: file=%s, width=%d, height=%d, colorkey=%s\n", 
		image.path, image.w, image.h, (image.colorkey ? "true" : "false") );

	//
	destroy_image( &image );

	//
	return S_OK;
}

// copies source texture into the surface of destination surface
// does a straight copy so you best make sure the images are the same size

static void copy_texture_bits(  LPDIRECT3DTEXTURE9 srcTexture, LPDIRECT3DTEXTURE9 dstTexture )
{
	HRESULT hr;
	D3DLOCKED_RECT srcRect;
	D3DLOCKED_RECT dstRect;
	unsigned int y, x;
	BYTE* srcBits;
	BYTE* dstBits;
	D3DSURFACE_DESC srcDesc;
	srcTexture->GetLevelDesc(0,&srcDesc);
	hr = srcTexture->LockRect(0,&srcRect,NULL,D3DLOCK_READONLY);
	if(FAILED(hr))
		return;
	hr = dstTexture->LockRect(0,&dstRect,NULL,D3DLOCK_DISCARD);
	if(FAILED(hr))
		return;
	srcBits = (BYTE*)srcRect.pBits;
	dstBits = (BYTE*)dstRect.pBits;
	for (y = 0; y < srcDesc.Height; y++)
	{
		for (x = 0; x < srcDesc.Width; x++)
		{
			// move to the correct off set in the table
			// pitch is the width of a row
			// (x*size) is the length of each pixel data
			DWORD srcIndex = (x*4)+(y*srcRect.Pitch);
			DWORD dstIndex = (x*4)+(y*dstRect.Pitch);
			// D3DFMT_A8R8G8B8 data will be accessible backwards: bgra
			dstBits[dstIndex]   = (BYTE)gamma_lookup[srcBits[srcIndex]];	// Blue
			dstBits[dstIndex+1] = (BYTE)gamma_lookup[srcBits[srcIndex+1]];	// Green
			dstBits[dstIndex+2] = (BYTE)gamma_lookup[srcBits[srcIndex+2]];	// Red
			dstBits[dstIndex+3] = (BYTE)gamma_lookup[srcBits[srcIndex+3]];	// Alpha
		}
	}
	dstTexture->UnlockRect(0);
	srcTexture->UnlockRect(0);
}

HRESULT update_texture_from_file(LPDIRECT3DTEXTURE9 dstTexture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{
	HRESULT hr;
    LPDIRECT3DTEXTURE9 new_texture = NULL;

	// incase texture doesn't exist yet then just create it for them and return it
	if(!dstTexture)
		return FSCreateTexture(&dstTexture, fileName, width, height, numMips, colourkey);

	// create the texture so we can copy the data over
	hr = create_texture(&new_texture, fileName, width, height, numMips, colourkey, D3DPOOL_SYSTEMMEM);
	if(FAILED(hr))
		return hr;

	// copy data from src to dst
	copy_texture_bits( new_texture, dstTexture );

	// 
	new_texture->Release();
	new_texture = NULL;
	return S_OK;
}

HRESULT FSCreateTexture(LPDIRECT3DTEXTURE9 *texture, const char *fileName, uint16 *width, uint16 *height, int numMips, BOOL * colourkey)
{
	return create_texture(texture, fileName, width, height, numMips, colourkey, D3DPOOL_MANAGED);
}

HRESULT FSCreateVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
//	assert (numVertices < 10000);

// this is not good cause LEVELRENDEROBJECT is of a diff size...
//	memset(renderObject, 0, sizeof(RENDEROBJECT));

	renderObject->vbLocked = 0;

	LastError = lpD3DDevice->CreateVertexBuffer(numVertices * sizeof(D3DLVERTEX), 0, D3DFVF_LVERTEX, D3DPOOL_MANAGED, &renderObject->lpD3DVertexBuffer, NULL);
	if (FAILED(LastError))
	{
		DebugPrintf("can't create vertex buffer\n");
	}

	DebugPrintf("created vertex buffer\n");

	return LastError;
}

HRESULT FSCreateDynamicVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
//	assert (numVertices < 10000);

// this is not good cause LEVELRENDEROBJECT is of a diff size...
//	memset(renderObject, 0, sizeof(RENDEROBJECT));

	renderObject->vbLocked = 0;

	LastError = lpD3DDevice->CreateVertexBuffer(numVertices * sizeof(D3DLVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_LVERTEX, D3DPOOL_DEFAULT, &renderObject->lpD3DVertexBuffer, NULL);
	if (FAILED(LastError))
	{
		DebugPrintf("can't create vertex buffer\n");
	}

	DebugPrintf("created vertex buffer\n");

	return LastError;
}


HRESULT FSCreateDynamic2dVertexBuffer(RENDEROBJECT *renderObject, int numVertices)
{
//	assert (numVertices < 10000);

	assert (renderObject->lpD3DVertexBuffer == NULL);

// this is not good cause LEVELRENDEROBJECT is of a diff size...
//	memset(renderObject, 0, sizeof(RENDEROBJECT));

	renderObject->vbLocked = 0;

	LastError = lpD3DDevice->CreateVertexBuffer(numVertices * sizeof(D3DTLVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_TLVERTEX, D3DPOOL_DEFAULT, &renderObject->lpD3DVertexBuffer, NULL);
	//LastError = lpD3DDevice->CreateVertexBuffer(numVertices * sizeof(D3DTLVERTEX), 0, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &renderObject->lpD3DVertexBuffer, NULL);
	if (FAILED(LastError))
	{
		DebugPrintf("can't create vertex buffer\n");
	}

	DebugPrintf("created vertex buffer\n");

	return LastError;
}

int lockTest = 0;

HRESULT FSLockVertexBuffer(RENDEROBJECT *renderObject, D3DLVERTEX **verts)
{
	assert(renderObject->vbLocked == 0);

	LastError = renderObject->lpD3DVertexBuffer->Lock(0, 0, (void**)verts, D3DLOCK_DISCARD);
	if (FAILED(LastError))
	{
		DebugPrintf("can't lock vertex buffer!\n");
	}

	renderObject->vbLocked = TRUE;
	lockTest++;

//	DebugPrintf("locked vertex buffer\n");

	return LastError;
}

HRESULT FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, D3DTLVERTEX **verts)
{
	assert(renderObject->vbLocked == 0);

	LastError = renderObject->lpD3DVertexBuffer->Lock(0, 0, (void**)verts, D3DLOCK_DISCARD);
	if (FAILED(LastError))
	{
		DebugPrintf("can't lock vertex buffer!\n");
	}

	renderObject->vbLocked = TRUE;
	lockTest++;

//	DebugPrintf("locked vertex buffer\n");

	return LastError;
}

HRESULT FSUnlockVertexBuffer(RENDEROBJECT *renderObject)
{
	assert(renderObject->vbLocked == 1);

//	DebugPrintf("unlocking vertex buffer\n");
	LastError = renderObject->lpD3DVertexBuffer->Unlock();
	if (FAILED(LastError))
	{
		DebugPrintf("can't unlock vertex buffer!\n");
	}

	renderObject->vbLocked = FALSE;
	lockTest--;

//	DebugPrintf("unlocked vertex buffer\n");

	return LastError;
}

// can just use the above if we want...
HRESULT FSUnlockPretransformedVertexBuffer(RENDEROBJECT *renderObject)
{
	assert(renderObject->vbLocked == 1);

//	DebugPrintf("unlocking vertex buffer\n");
	LastError = renderObject->lpD3DVertexBuffer->Unlock();
	if (FAILED(LastError))
	{
		DebugPrintf("can't unlock vertex buffer!\n");
	}

	renderObject->vbLocked = FALSE;
	lockTest--;

//	DebugPrintf("unlocked vertex buffer\n");

	return LastError;
}

HRESULT FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	LastError = lpD3DDevice->CreateIndexBuffer(numIndices * 3 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &renderObject->lpD3DIndexBuffer, NULL);
	if (FAILED(LastError))
	{
		DebugPrintf("can't create vertex buffer\n");
	}

	DebugPrintf("created vertex buffer\n");

	return LastError;
}

HRESULT FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	LastError = lpD3DDevice->CreateIndexBuffer(numIndices * 3 * sizeof(WORD),  D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &renderObject->lpD3DIndexBuffer, NULL);
	if (FAILED(LastError))
	{
		DebugPrintf("can't create vertex buffer\n");
	}

	DebugPrintf("created vertex buffer\n");

	return LastError;
}

HRESULT FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices)
{
	LastError = renderObject->lpD3DIndexBuffer->Lock(0, 0, (void**)indices, D3DLOCK_DISCARD);
	if (FAILED(LastError))
	{
		DebugPrintf("can't lock index buffer!\n");
	}

	return LastError;
}

HRESULT FSUnlockIndexBuffer(RENDEROBJECT *renderObject)
{
	LastError = renderObject->lpD3DIndexBuffer->Unlock();
	if (FAILED(LastError))
	{
		DebugPrintf("can't lock index buffer!\n");
	}

	return LastError;
}

HRESULT draw_render_object( RENDEROBJECT *renderObject, BOOL transformed /*aka 2d*/, D3DPRIMITIVETYPE primitive_type )
{
	HRESULT LastError;

	assert(renderObject->vbLocked == 0);

	if( transformed )
		LastError = lpD3DDevice->SetStreamSource(0, renderObject->lpD3DVertexBuffer, 0, sizeof(D3DTLVERTEX));
	else
		LastError = lpD3DDevice->SetStreamSource(0, renderObject->lpD3DVertexBuffer, 0, sizeof(D3DLVERTEX));

	if (FAILED(LastError))
		return LastError;

	if(renderObject->lpD3DIndexBuffer)
	{
		LastError = lpD3DDevice->SetIndices(renderObject->lpD3DIndexBuffer);
		if(FAILED(LastError)) 
			return LastError;
	}

	if( transformed )
		LastError = lpD3DDevice->SetFVF(D3DFVF_TLVERTEX);
	else
		LastError = lpD3DDevice->SetFVF(D3DFVF_LVERTEX);

	if (FAILED(LastError))
		return LastError;

	LastError = lpD3DDevice->SetMaterial(&renderObject->material);
	if (FAILED(LastError))
		return LastError;

	for (int i = 0; i < renderObject->numTextureGroups; i++)
	{
		if(renderObject->textureGroups[i].colourkey)
			set_alpha_ignore();

		LastError = lpD3DDevice->SetTexture(0, renderObject->textureGroups[i].texture);
		if (FAILED(LastError))
			return LastError;

		if(renderObject->lpD3DIndexBuffer)
		{
			LastError = lpD3DDevice->DrawIndexedPrimitive(
				primitive_type,
				renderObject->textureGroups[i].startVert,
				0, 
				renderObject->textureGroups[i].numVerts,
				renderObject->textureGroups[i].startIndex,
				renderObject->textureGroups[i].numTriangles
			);
		}
		else
		{
			LastError = lpD3DDevice->DrawPrimitive(
				primitive_type,
				renderObject->textureGroups[i].startVert,
				renderObject->textureGroups[i].numVerts
			);
		}

		if(renderObject->textureGroups[i].colourkey)
			unset_alpha_ignore();

		if (FAILED(LastError))
			return LastError;
	}

	return S_OK;
}

HRESULT draw_line_object(RENDEROBJECT *renderObject)
{
	return draw_render_object( renderObject, FALSE, D3DPT_LINELIST );
}

HRESULT draw_object(RENDEROBJECT *renderObject)
{
	return draw_render_object( renderObject, FALSE, D3DPT_TRIANGLELIST );
}

HRESULT draw_2d_object(RENDEROBJECT *renderObject)
{
	return draw_render_object( renderObject, TRUE, D3DPT_TRIANGLELIST );
}

void FSReleaseRenderObject(RENDEROBJECT *renderObject)
{
	if (renderObject->lpD3DVertexBuffer)
	{
		renderObject->lpD3DVertexBuffer->Release();
		renderObject->lpD3DVertexBuffer = NULL;
	}
	if (renderObject->lpD3DIndexBuffer)
	{
		renderObject->lpD3DIndexBuffer->Release();
		renderObject->lpD3DIndexBuffer = NULL;
	}
	for (int i = 0; i < renderObject->numTextureGroups; i++)
	{
		renderObject->textureGroups[i].numVerts = 0;
		renderObject->textureGroups[i].startVert = 0;

		if (renderObject->textureGroups[i].texture)
		{
			// this is just a pointer to Tloadheader
			// we do not need to worry about releasing it
			renderObject->textureGroups[i].texture = NULL;
		}
	}
}

}; // end of c linkage (extern "C")

const char * render_error_description( HRESULT hr )
{
#ifdef __WINE__
	return "";
#else
	return DXGetErrorDescription9(hr);
#endif
}
