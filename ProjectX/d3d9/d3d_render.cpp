
#include <d3dx9.h>

extern "C" {
#include "typedefs.h"
#include "d3dappi.h"
#include "tload.h"
#include "new3d.h"
#include <assert.h>
#include "util.h"
#include <dxerr9.h>

// d3d caps
// D3DPTEXTURECAPS_SQUAREONLY
BOOL  bSquareOnly = FALSE;


/***************************************************************************/
/*                            Creation of D3D                              */
/***************************************************************************/
extern BOOL InitView(void);
extern BOOL VSync;
extern int default_width;
extern int default_height;
extern int default_bpp;

BOOL render_initialized = FALSE;

LPDIRECT3D9			lpD3D; /* D3D interface object */
LPDIRECT3DDEVICE9	lpD3DDevice;	/* D3D device */

BOOL init_renderer(HWND hwnd, BOOL fullscreen)
{
	HRESULT LastError;
	MYD3DVIEWPORT9 viewport;

	// Set up Direct3D interface object
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!lpD3D)
	{
		OutputDebugString("couldnt create d3d9\n");
		return FALSE;
	}

	// create d3d device
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory (&d3dpp, sizeof(d3dpp));

	//
	d3dappi.bFullscreen = fullscreen;
	d3dpp.Windowed = !fullscreen;

	//
	// presentation settings
	//

	d3dpp.hDeviceWindow					= hwnd;							// the window handle
	d3dpp.BackBufferCount				= 1;							// we only have one swap chain
    d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// does not protect the contents of the backbuffer after flipping (faster)
																		// shouldn't we specify D3DSWAPEFFECT_FLIP ?
																		// wouldn't D3DSWAPEFFECT_OVERLAY be fastest ?
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// display refresh
	d3dpp.EnableAutoDepthStencil		= TRUE;							// let d3d manage the z-buffer

	if(VSync)	
		d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_ONE;			// enable vsync
	else
		d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;	// disable vsync

	// 16 bit zbuffer
	//d3dpp.AutoDepthStencilFormat	= D3DFMT_D15S1; // 16 bit depth buffer with 1 bit stencil
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;	// 16 bit depth buffer

	// 32 bit back buffer
	// Also supports 32 bit zbuffer
	if( default_bpp >= 32 && 
		SUCCEEDED(lpD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, d3dpp.Windowed)))
	{
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
		CloseWindow(hwnd);
		Msg("Failed to find a suitable back buffer format");
		exit(1);
	}

	//
	// Enumerates display modes 
	// picking default_height & default_width if they exist
	// or picking the biggest mode possible :]
	//

	{
		int mode = 0;
		int desired_mode = -1;
		unsigned int best_mode = 0;
		unsigned int i;
		unsigned int num_modes = lpD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat );
		D3DDISPLAYMODE * modes = (D3DDISPLAYMODE *) malloc( num_modes * sizeof(D3DDISPLAYMODE) );
		for ( i = 0; i < num_modes; i++ )
		{
			lpD3D->EnumAdapterModes( D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat, i, &modes[i] );
			if(modes[i].Width == default_width && modes[i].Height == default_height )
				desired_mode = i;
			if(modes[i].Width > modes[best_mode].Width)
				best_mode = i;
		}
		if( desired_mode >= 0 )
		{
			mode = desired_mode;
		}
		else
		{
			mode = best_mode;
		}
		{
			D3DDISPLAYMODE m = modes[mode];
			d3dpp.BackBufferWidth  = m.Width;
			d3dpp.BackBufferHeight = m.Height;
		}
	}

	DebugPrintf("Using display size of %dx%d\n",d3dpp.BackBufferWidth,d3dpp.BackBufferHeight);

	// window mode
	if ( ! fullscreen )
	{
		SetWindowPos( 
			hwnd,			// the window handle
			HWND_TOP,		// bring window to the front
			0, 0,			// top left of screen
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
		hwnd,										// the window handle

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
		LastError = lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
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
		LastError = lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
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
		LastError = lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
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
		CloseWindow(hwnd);
		Msg("Failed to create a suitable d3d device");
		exit(1);
	}

	render_initialized = TRUE;
	d3dappi.bRenderingIsOK = TRUE;

	d3dappi.szClient.cx = d3dpp.BackBufferWidth; 
	d3dappi.szClient.cy = d3dpp.BackBufferHeight;

	d3dappi.WindowsDisplay.w = d3dpp.BackBufferWidth;
	d3dappi.WindowsDisplay.h = d3dpp.BackBufferHeight;

	d3dappi.ThisMode.w = d3dpp.BackBufferWidth;
	d3dappi.ThisMode.h = d3dpp.BackBufferHeight;

	/* do "after device created" stuff */
	ZeroMemory( &viewport, sizeof(viewport) );
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = 800;
	viewport.Height = 600;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;

	LastError = FSSetViewPort(&viewport);
	if (FAILED(LastError))
	{
		OutputDebugString("couldn't set viewport\n");
	}

	// load the view
	if (!InitView() )
	{
	    Msg("InitView failed.\n");
//		CleanUpAndPostQuit();
        return FALSE;
	}

    if(!init_render_states())
		return FALSE;

	return TRUE;
}

void render_cleanup( void )
{
    d3dappi.bRenderingIsOK = FALSE;
    RELEASE(lpD3DDevice);
	RELEASE(lpD3D);
}

BOOL FlipBuffers()
{
	if (!d3dappi.bRenderingIsOK) 
	{
		OutputDebugString("Cannot call FlipBuffers() while bRenderingIsOK is FALSE.\n");
		return FALSE;
	}

	lpD3DDevice->Present(NULL, NULL, NULL, NULL);

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

	//FlipBuffers();

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

BOOL init_render_states()
{
	//STATE( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	STATE(	D3DRS_SHADEMODE,		D3DSHADE_GOURAUD );
	STATE(	D3DRS_LIGHTING,			FALSE);

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

HRESULT FSGetViewPort(MYD3DVIEWPORT9 *returnViewPort)
{
	return lpD3DDevice->GetViewport( (D3DVIEWPORT9*) returnViewPort );
}

HRESULT FSSetViewPort(MYD3DVIEWPORT9 *newViewPort)
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
	D3DXSaveTextureToFile(path, D3DXIFF_PNG, texture, 0);
}

char saveFile[MAX_PATH];

int imageCount = 0;

HRESULT create_texture(LPDIRECT3DTEXTURE9 *texture, const char *fileName, int width, int height, int numMips, BOOL * colourkey, D3DPOOL pool)
{
	D3DXIMAGE_INFO imageInfo;

	HRESULT LastError = D3DXCreateTextureFromFileEx(lpD3DDevice, 
				fileName, 
				width, 
				height, 
				numMips, 
				0,
				// most likely will end up as the format the file is in
				D3DFMT_A8R8G8B8,
				pool,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				(*colourkey) ? FSColourKeyBlack : 0, // colour key
				&imageInfo,
				NULL,
				texture);

	if (FAILED(LastError))
	{
		OutputDebugString("couldn't create texture\n");
	}

	// image has no alpha layer
	if( imageInfo.Format != D3DFMT_A8R8G8B8 )
		(*colourkey) = FALSE;

	DebugPrintf("FSCreateTexture: %s\n",fileName);

	{
		static int count = 0;
		sprintf(buf, ".\\Dumps\\%s.png", fileName);
		D3DXSaveTextureToFile(buf, D3DXIFF_PNG, (*texture), 0);
		count++;
	}

	return LastError;
}

HRESULT update_texture_from_file(LPDIRECT3DTEXTURE9 dstTexture, const char *fileName, int width, int height, int numMips, BOOL * colourkey)
{
	HRESULT hr;
    LPDIRECT3DTEXTURE9 new_texture = NULL;
	if(!dstTexture) // incase texture doesn't exist yet
		return FSCreateTexture(&dstTexture, fileName, width, height, numMips, colourkey);
	create_texture(&new_texture, fileName, width, height, numMips, colourkey, D3DPOOL_SYSTEMMEM);
	dstTexture->AddDirtyRect(NULL);
	hr = lpD3DDevice->UpdateTexture( (IDirect3DBaseTexture9*) new_texture, (IDirect3DBaseTexture9*) dstTexture );
	new_texture = NULL;
	return hr;
}

HRESULT FSCreateTexture(LPDIRECT3DTEXTURE9 *texture, const char *fileName, int width, int height, int numMips, BOOL * colourkey)
{
	// had to use the default pool so we could update the texture above
	return create_texture(texture, fileName, width, height, numMips, colourkey, D3DPOOL_DEFAULT);//D3DPOOL_MANAGED);
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
		OutputDebugString("can't create vertex buffer\n");
	}

	OutputDebugString("created vertex buffer\n");

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
		OutputDebugString("can't create vertex buffer\n");
	}

	OutputDebugString("created vertex buffer\n");

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
	if (FAILED(LastError))
	{
		OutputDebugString("can't create vertex buffer\n");
	}

	OutputDebugString("created vertex buffer\n");

	return LastError;
}

int lockTest = 0;

HRESULT FSLockVertexBuffer(RENDEROBJECT *renderObject, D3DLVERTEX **verts)
{
	assert(renderObject->vbLocked == 0);

	LastError = renderObject->lpD3DVertexBuffer->Lock(0, 0, (void**)verts, D3DLOCK_DISCARD);
	if (FAILED(LastError))
	{
		OutputDebugString("can't lock vertex buffer!\n");
	}

	renderObject->vbLocked = TRUE;
	lockTest++;

//	OutputDebugString("locked vertex buffer\n");

	return LastError;
}

HRESULT FSLockPretransformedVertexBuffer(RENDEROBJECT *renderObject, D3DTLVERTEX **verts)
{
	assert(renderObject->vbLocked == 0);

	LastError = renderObject->lpD3DVertexBuffer->Lock(0, 0, (void**)verts, D3DLOCK_DISCARD);
	if (FAILED(LastError))
	{
		OutputDebugString("can't lock vertex buffer!\n");
	}

	renderObject->vbLocked = TRUE;
	lockTest++;

//	OutputDebugString("locked vertex buffer\n");

	return LastError;
}

HRESULT FSUnlockVertexBuffer(RENDEROBJECT *renderObject)
{
	assert(renderObject->vbLocked == 1);

//	OutputDebugString("unlocking vertex buffer\n");
	LastError = renderObject->lpD3DVertexBuffer->Unlock();
	if (FAILED(LastError))
	{
		OutputDebugString("can't unlock vertex buffer!\n");
	}

	renderObject->vbLocked = FALSE;
	lockTest--;

//	OutputDebugString("unlocked vertex buffer\n");

	return LastError;
}

// can just use the above if we want...
HRESULT FSUnlockPretransformedVertexBuffer(RENDEROBJECT *renderObject)
{
	assert(renderObject->vbLocked == 1);

//	OutputDebugString("unlocking vertex buffer\n");
	LastError = renderObject->lpD3DVertexBuffer->Unlock();
	if (FAILED(LastError))
	{
		OutputDebugString("can't unlock vertex buffer!\n");
	}

	renderObject->vbLocked = FALSE;
	lockTest--;

//	OutputDebugString("unlocked vertex buffer\n");

	return LastError;
}

HRESULT FSCreateIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	LastError = lpD3DDevice->CreateIndexBuffer(numIndices * 3 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &renderObject->lpD3DIndexBuffer, NULL);
	if (FAILED(LastError))
	{
		OutputDebugString("can't create vertex buffer\n");
	}

	OutputDebugString("created vertex buffer\n");

	return LastError;
}

HRESULT FSCreateDynamicIndexBuffer(RENDEROBJECT *renderObject, int numIndices)
{
	LastError = lpD3DDevice->CreateIndexBuffer(numIndices * 3 * sizeof(WORD),  D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &renderObject->lpD3DIndexBuffer, NULL);
	if (FAILED(LastError))
	{
		OutputDebugString("can't create vertex buffer\n");
	}

	OutputDebugString("created vertex buffer\n");

	return LastError;
}

HRESULT FSLockIndexBuffer(RENDEROBJECT *renderObject, WORD **indices)
{
	LastError = renderObject->lpD3DIndexBuffer->Lock(0, 0, (void**)indices, D3DLOCK_DISCARD);
	if (FAILED(LastError))
	{
		OutputDebugString("can't lock index buffer!\n");
	}

	return LastError;
}

HRESULT FSUnlockIndexBuffer(RENDEROBJECT *renderObject)
{
	LastError = renderObject->lpD3DIndexBuffer->Unlock();
	if (FAILED(LastError))
	{
		OutputDebugString("can't lock index buffer!\n");
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
			// the texture code probably already deals with this.
			// but we need to figure out why this is crashing for sure.
			//renderObject->textureGroups[i].texture->Release();
			renderObject->textureGroups[i].texture = NULL;
		}
	}
}

}; // end of c linkage (extern "C")

const char * render_error_description( HRESULT hr )
{
	return DXGetErrorDescription9(hr);
}
