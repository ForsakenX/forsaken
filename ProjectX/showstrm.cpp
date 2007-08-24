#include <windows.h>
#include "typedefs.h"
#include <stdio.h>
#include "ddraw.h"
#include "amstream.h"
#include "ddstream.h"
#include "showstrm.h"
#include "D3dmain.h"
#include <vfwmsgs.h>

extern "C" {
int AnyKeyPressed( void );
void ReadInput( void );
};


//#define RELEASE(x) if (x) (x)->Release();
#define CHECK_ERROR(x)     \
   if (FAILED(hr = (x))) { \
       Msg( "  failed with HRESULT(0x%8.8X)\n", hr); \
       goto Exit;          \
   }


HRESULT OpenMMStream(const char * pszFileName, IDirectDraw *pDD, IMultiMediaStream **ppMMStream)
{
    *ppMMStream = NULL;
    IAMMultiMediaStream *pAMStream;
    HRESULT hr;

    CHECK_ERROR(CoCreateInstance(CLSID_AMMultiMediaStream, NULL, CLSCTX_INPROC_SERVER,
				 IID_IAMMultiMediaStream, (void **)&pAMStream));
    CHECK_ERROR(pAMStream->Initialize(STREAMTYPE_READ, 0, NULL));
    CHECK_ERROR(pAMStream->AddMediaStream(pDD, &MSPID_PrimaryVideo, 0, NULL));
    
	//CHECK_ERROR(pAMStream->AddMediaStream(NULL, &MSPID_PrimaryAudio, AMMSF_ADDDEFAULTRENDERER, NULL));
	
	hr = pAMStream->AddMediaStream(NULL, &MSPID_PrimaryAudio, AMMSF_ADDDEFAULTRENDERER, NULL);
	if ( hr != VFW_E_NO_AUDIO_HARDWARE )
		CHECK_ERROR( hr );

    WCHAR       wPath[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, pszFileName, -1, wPath, sizeof(wPath)/sizeof(wPath[0]));

    CHECK_ERROR(pAMStream->OpenFile(wPath, 0));

    *ppMMStream = pAMStream;
    pAMStream->AddRef();

Exit:
    if (pAMStream == NULL) {
	Msg("Could not create a CLSID_MultiMediaStream object\n" );
    }
    RELEASE(pAMStream);
    return hr;
}




HRESULT RenderStreamToSurface(IDirectDraw *pDD, IDirectDrawSurface *pPrimary,
			      IMultiMediaStream *pMMStream)
{

    HRESULT hr;
	DDSURFACEDESC surface;
    IMediaStream *pPrimaryVidStream = NULL;
    IDirectDrawMediaStream *pDDStream = NULL;
    IDirectDrawSurface *pSurface = NULL;
    IDirectDrawStreamSample *pSample = NULL;

    RECT rect, dest;

	int Letterbox; 

    CHECK_ERROR(pMMStream->GetMediaStream(MSPID_PrimaryVideo, &pPrimaryVidStream));
    CHECK_ERROR(pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream, (void **)&pDDStream));

    CHECK_ERROR(pDDStream->CreateSample(NULL, NULL, 0, &pSample));
    CHECK_ERROR(pSample->GetSurface(&pSurface, &rect));
    
    CHECK_ERROR(pMMStream->SetState(STREAMSTATE_RUN));


	surface.dwSize = sizeof( DDSURFACEDESC );
	pPrimary->GetSurfaceDesc( &surface );

	//AVI_Letterbox = (d3dapp->ThisMode.h - (VidInfo.rcFrame.bottom - VidInfo.rcFrame.top) * d3dapp->ThisMode.h / AVI_FullScreenHeight) / 2;
	Letterbox = ( surface.dwHeight - ( 172 ) * surface.dwHeight / 240 ) / 2;
	
	dest.top = Letterbox;
	dest.left = 0;
	dest.right = surface.dwWidth;
	dest.bottom = surface.dwHeight - Letterbox;

    while ( !AnyKeyPressed() ) 
	{
		ReadInput();

		if (pSample->Update(0, NULL, NULL, 0) != S_OK) 
		{
		   break;
		}

		pPrimary->Blt(/*&rect*/&dest, pSurface, /*&rect*/NULL, DDBLT_WAIT, NULL);
    }

Exit:
    RELEASE(pPrimaryVidStream);
    RELEASE(pDDStream);
    RELEASE(pSample);
    RELEASE(pSurface);

    return hr;
}

/*
//int _CRTAPI1
int __cdecl
main(
    int argc,
    char *argv[]
    )
{
    if (argc < 2) {
	printf("Usage : showstrm movie.ext\n");
	exit(0);
    }
    CoInitialize(NULL);
    IDirectDraw *pDD;

    HRESULT hr = DirectDrawCreate(NULL, &pDD, NULL);
    if (SUCCEEDED(hr)) {
	DDSURFACEDESC ddsd;
	IDirectDrawSurface *pPrimarySurface;

	pDD->SetCooperativeLevel(GetDesktopWindow(), DDSCL_NORMAL);

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	hr = pDD->CreateSurface(&ddsd, &pPrimarySurface, NULL);
	if (SUCCEEDED(hr)) {
	    IMultiMediaStream *pMMStream;
	    hr = OpenMMStream(argv[1], pDD, &pMMStream);
	    if (SUCCEEDED(hr)) {
		RenderStreamToSurface(pDD, pPrimarySurface, pMMStream);
		pMMStream->Release();
	    }
	    pPrimarySurface->Release();
	}
	pDD->Release();
    } else {
	printf("Could not open DirectDraw - check it is installed\n");
    }
    CoUninitialize();
    return 0;
}
*/