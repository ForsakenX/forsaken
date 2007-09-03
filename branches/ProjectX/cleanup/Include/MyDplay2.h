/*
 * The X Men, Febuary 1997
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 4 $
 *
 * $Header: /PcProjectX/Include/MyDplay2.h 4     12/20/97 5:03p Phillipd $
 *
 * $Log: /PcProjectX/Include/MyDplay2.h $
 * 
 * 4     12/20/97 5:03p Phillipd
 * 
 * 3     12/13/97 3:05p Phillipd
 * Reduced size of message buffer.....now 128k each......
 * 
 * 2     6/14/97 1:23p Phillipd
 * 
 * 1     2/10/97 10:01a Phillipd
 */
#ifndef	MYDPLAY2_INCLUDED
#define MYDPLAY2_INCLUDED

#define WIN32_EXTRA_LEAN
//defines

#define	SMALLBUFFERSIZE 1024
#define	MAXBUFFERSIZE	(1024*256)

// prototypes
HRESULT SetupConnection(HINSTANCE hInstance);
HRESULT ShutdownConnection(void);



#endif //MYDPLAY2_INCLUDED