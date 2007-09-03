#ifndef SHOWSTREAM_H
#define SHOWSTREAM_H

#include "mmstream.h"

#ifdef __cplusplus
extern "C" {
#endif

HRESULT OpenMMStream(const char * pszFileName, IDirectDraw *pDD, IMultiMediaStream **ppMMStream);
HRESULT RenderStreamToSurface(IDirectDraw *pDD, IDirectDrawSurface *pPrimary, IMultiMediaStream *pMMStream);

#ifdef __cplusplus
};
#endif

#endif
