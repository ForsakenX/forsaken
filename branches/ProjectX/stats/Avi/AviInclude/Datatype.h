
/************************************************************************
*  datatype.h                                                           *
*                                                                       *
* This code and information is provided "as is" without warranty of     *
* any kind, either expressed or implied, including but not limited to   *
* the implied warranties of merchantability and/or fitness for a        *
* particular purpose.                                                   *
*                                                                       *
*               Copyright (c) 1994-1995 Intel Corporation.              *
*                         All Rights Reserved.                          *
*                                                                       *
************************************************************************/

#ifndef __DATATYPE_H__
#define __DATATYPE_H__

#include <stddef.h>
/* These are the non platform specific data types
 * for which all data and return types are defined
 */

/* These types of data must allways be the same
 * number of bits regardless of the platform.
 * Therefore, these generic names are defined
 * to be:
 * 
 * U8      8 bits,  unsigned 0..255
 * I8      8 bits,  signed -128..127
 * U16     16 bits, unsigned 0..65535
 * I16     16 bits, signed -32768..32767
 * U32     32 bits, unsigned 0..4294967295
 * I32     32 bits, signed -2147483648..2147483647
 * Sngl    32 bit floating point (4 bytes)
 * Dbl     64 bit floating point (8 bytes)
 * 
 * These pointers are naturally platform specific,
 * but what they point to is allways the same size
 *
 *               These are huge pointers
 * PU8
 * PI8
 * PU16
 * PI16
 * PU32
 * PI32
 * PSngl
 * PDbl
 *               These are far pointers
 * FPU8
 * FPI8
 * FPU16
 * FPI16
 * FPU32
 * FPI32
 * FPSngl
 * FPDbl
 */

#if defined HUGE
#undef HUGE
#endif
#if defined FAR
#undef FAR
#endif
#if defined NEAR
#undef NEAR
#endif

#if defined WINDOWS
#if defined WIN32
/* WINDOWS NT data types, Only X86 support at this time */
#define NEAR
#define FAR
#define HUGE

// TCHAR should be appropriately defined in std header files - ??
// #ifndef _TCHAR_DEFINED
// typedef wchar_t TCHAR; /* unicode for strings */
// #endif

typedef unsigned char  U8;
typedef signed char    I8;
typedef unsigned short U16;
typedef signed short   I16;
typedef unsigned long  U32;
typedef signed long    I32;
typedef float          Sngl;
typedef double         Dbl;

#else /* WIN32 not defined */
/* WINDOWS 3.x under DOS data types */
#define NEAR _near
#define FAR  _far
#define HUGE _huge

#ifndef _INC_TCHAR
typedef char far *			LPTSTR;
typedef const char far *	LPCTSTR;
typedef char				TCHAR;
#define TEXT(quote)			quote
#endif
#ifndef BOOLEAN
typedef char BOOLEAN;
#endif
#ifndef INT
typedef int INT;
#endif

typedef unsigned char  U8;
typedef signed char    I8;
typedef unsigned short U16;
typedef signed short   I16;
typedef unsigned long  U32;
typedef signed long    I32;
typedef float          Sngl;
typedef double         Dbl;

#endif /* WIN32 */
#else /* WINDOWS not defined */
/* UNIX data types */
#define NEAR
#define FAR
#define HUGE
#define CDECL
#define BSENC 1
#define HGLOBAL PI16
#define LRESULT I32
//#define HINSTANCE PU8 // Currently defined elsewhere

typedef char TCHAR; /* for compatability with win/win32 datatypes for strings */

typedef unsigned char    U8;
typedef signed char      I8;
typedef unsigned short   U16;
typedef signed short int I16;
typedef unsigned long    U32;
typedef signed long      I32;
typedef float            Sngl;
typedef double           Dbl;

/* move WIN32 UNICODE data types to UNIX data types */
typedef char * LPTSTR;
typedef const char * LPCTSTR;

#ifndef TEXT
#define TEXT(quote) quote
#endif /* TEXT */
#endif /* WINDOWS */

typedef char Chr;        /* character string */
typedef char Qchar;  /* quoted character string */

/* pointers */
typedef Chr  NEAR * PChr;       /* near pointer to Character string */
typedef U8   NEAR * PU8;        /* near pointer to unsigned char */
typedef I8   NEAR * PI8;        /* near pointer to signed char */
typedef U16  NEAR * PU16;       /* near pointer to unsigned 16 bit value */
typedef I16  NEAR * PI16;       /* near pointer to signed 16 bit value */
typedef U32  NEAR * PU32;       /* near pointer to unsigned 32 bit value */
typedef I32  NEAR * PI32;       /* near pointer to signed 32 bit value */
typedef Sngl NEAR * PSngl;      /* near pointer to single precision float */
typedef Dbl  NEAR * PDbl;       /* near pointer to double precision float */

typedef Chr  FAR * FPChr;       /* far pointer to Character string */
typedef U8   FAR * FPU8;        /* far pointer to unsigned char */
typedef I8   FAR * FPI8;        /* far pointer to signed char */
typedef U16  FAR * FPU16;       /* far pointer to unsigned 16 bit value */
typedef I16  FAR * FPI16;       /* far pointer to signed 16 bit value */
typedef U32  FAR * FPU32;       /* far pointer to unsigned 32 bit value */
typedef I32  FAR * FPI32;       /* far pointer to signed 32 bit value */
typedef Sngl FAR * FPSngl;      /* far pointer to single precision float */
typedef Dbl  FAR * FPDbl;       /* far pointer to double precision float */

typedef Chr  HUGE * HPChr;      /* huge pointer to Character string */
typedef U8   HUGE * HPU8;       /* huge pointer to unsigned char */
typedef I8   HUGE * HPI8;       /* huge pointer to signed char */
typedef U16  HUGE * HPU16;      /* huge pointer to unsigned 16 bit value */
typedef I16  HUGE * HPI16;      /* huge pointer to signed 16 bit value */
typedef U32  HUGE * HPU32;      /* huge pointer to unsigned 32 bit value */
typedef I32  HUGE * HPI32;      /* huge pointer to signed 32 bit value */
typedef Sngl HUGE * HPSngl;     /* huge pointer to single precision float */
typedef Dbl  HUGE * HPDbl;      /* huge pointer to double precision float */

typedef PU8  NEAR * PPU8;        /* near handle to unsigned char */
typedef PI8  NEAR * PPI8;        /* near handle to unsigned char */
typedef PU16  NEAR * PPU16;        /* near handle to unsigned char */
typedef PI16  NEAR * PPI16;        /* near handle to unsigned char */
typedef PU32  NEAR * PPU32;        /* near handle to unsigned char */
typedef PI32  NEAR * PPI32;        /* near handle to unsigned char */

/* These types of data do not need to be the
 * same from platform to platform. The only
 * concern is that the values represented by
 * these types fit into the smallest bit size
 * of any platform we port to. As new platforms
 * are added, please update this table:
 * 
 *                  WIN3.x UNIX  WINNT (win32)
 * NaturalInt:      16     32    32
 * NaturalUnsigned: 16     32    32
 * 
 * These pointers are naturally platform specific,
 * and what they point to is allowed to be
 * platform specific also
 *
 *               These are huge pointers
 * PNaturalInt
 * PNaturalUnsigned
 *               These are far pointers
 * FPNaturalInt
 * FPNaturalUnsigned
 *
 * For those who complain about long identifiers,
 * these data types are spelled out specifically
 * to avoid confusion with the fixed length data
 * types and to let the reader who is unfamiliar
 * with our coding practices know that these data
 * types are different on each platform. The intent
 * is to be obvious and obnoxious.
 *
 * to facilitate testing, define TEST_16_BITS
 * which will force all natural data types to 16 bits
 */

#if defined TEST_16_BITS
typedef I16 NaturalInt;
typedef U16 NaturalUnsigned;
#else
#if defined TEST_32_BITS
typedef I32 NaturalInt;
typedef U32 NaturalUnsigned;
#else
typedef signed int NaturalInt;
typedef unsigned int NaturalUnsigned;
#endif /* TEST_32_BITS */
#endif /* TEST_16_BITS */

/* pointers */
typedef NaturalInt HUGE *      PNaturalInt;
typedef NaturalUnsigned HUGE * PNaturalUnsigned;

typedef NaturalInt FAR *       FPNaturalInt;
typedef NaturalUnsigned FAR *  FPNaturalUnsigned;

/* logical types */
typedef NaturalInt		Boo;    /* was U32 */
typedef Boo				Boolean;
typedef Boo*			pBoo;

/* BGR_ENTRY
 * The BGR_ENTRY structure is used to define one color.
 */
typedef struct {
	U8 u8B;
	U8 u8G;
	U8 u8R;
	U8 u8Reserved;
} BGR_ENTRY;
typedef BGR_ENTRY FAR * PTR_BGR_ENTRY;

/* ASM interface */
#if defined WIN32
#define ASM_CALLTYPE _stdcall
#else
#define ASM_CALLTYPE
#endif

typedef void NEAR * LOCAL_HANDLE;
typedef void NEAR * GLOBAL_HANDLE;
typedef void NEAR * VOID_LOCAL_PTR;
typedef void FAR * VOID_GLOBAL_PTR;  
typedef void HUGE * VOID_HUGE_PTR;  
typedef void NEAR * MUTEX_HANDLE;
typedef void NEAR * VOID_INSTANCE_PTR;

#define PChr	PU8
#define TRUE  1
#define FALSE 0

#endif /* __DATATYPE_H__ */


