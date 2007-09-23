//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1992 - 1996  Microsoft Corporation.  All Rights Reserved.
//
//--------------------------------------------------------------------------;

// Allows Visual C++ to precompile the standard header files

#ifndef	STDWIN_INC
#define STDWIN_INC

#ifdef NDEBUG
  #define ASSERT( exp ) ((void)0)
#else
  void DbgAssert(const char *pCondition, const char *pFileName, int iLine);
  #define ASSERT(exp) if(!(exp)) DbgAssert( TEXT(#exp), TEXT(__FILE__), TEXT(__LINE__) );
#endif

#include <windows.h>
#include <commdlg.h>
#include <string.h>
#include <objbase.h>

#ifdef USE_A3D
#include <a3dguid.h>
#endif


// DEBUG GUID
#ifdef DEBUG_ON
  
  // {7365f4b3-aeba-496f-ae26-558675489b31}
  DEFINE_GUID(PROJX_GUID, 0x7365f4b3, 0xaeba, 0x496f, 0xae, 0x26, 0x55, 0x86, 0x75, 0x48, 0x9b, 0x31 );

// RELEASE GUID
#else

  // {b1a00cdf-4660-44b6-b703-6fe593d9725a}
  DEFINE_GUID(PROJX_GUID, 0xb1a00cdf, 0x4660, 0x44b6, 0xb7, 0x03, 0x6f, 0xe5, 0x93, 0xd9, 0x72, 0x5a );

#endif


#endif // STDWIN_INC



