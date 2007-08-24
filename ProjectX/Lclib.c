/*
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 4 $
 *
 * $Header: /PcProjectX/Lclib.c 4     10/24/96 3:01p Phillipd $
 *
 * $Log: /PcProjectX/Lclib.c $
 * 
 * 4     10/24/96 3:01p Phillipd
 * 
 * 3     6/25/96 11:37a Phillipd
 * First SS update
 * 
 */

#define WIN32_EXTRA_LEAN
#include        <windows.h>
#include        "lclib.h"


char* LSTRCHR( const char* lpString, int bChar )
{
    if( lpString != NULL )
    {
        while( *lpString != 0 )
        {
            if( *lpString == bChar )
            {
                return (char*)lpString;
            }

            lpString++;
        }
    }
    return NULL;

} /* LSTRCHR */

char* LSTRRCHR( const char* lpString, int bChar )
{
    if( lpString != NULL )
    {
        const char*     lpBegin;

        lpBegin = lpString;

        while( *lpString != 0 )
        {
            lpString++;
        }

        while( 1 )
        {
            if( *lpString == bChar )
            {
                return (char*)lpString;
            }
            
            if( lpString == lpBegin )
            {
                break;
            }

            lpString--;
        }
    }

    return NULL;
} /* LSTRRCHR */

