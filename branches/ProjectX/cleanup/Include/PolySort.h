/*
 * The X Men, May 1997
 * Copyright (c) 1997 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 3 $
 *
 * $Header: /PcProjectX/Include/PolySort.h 3     5/20/97 2:20p Phillipd $
 *
 * $Log: /PcProjectX/Include/PolySort.h $
 * 
 * 3     5/20/97 2:20p Phillipd
 * 
 * 2     5/17/97 5:17p Phillipd
 * 
 * 1     5/17/97 2:15p Phillipd
 */											    
#ifndef POLYSORT_INCLUDED
#define	POLYSORT_INCLUDED

// Defines

#define	MAXVERTSPERPOLYSORTPRIM 3
#define	MAXVERTSPERPOLYSORTEXEC 600
#define	BUCKETDEPTH 512
#define	MAXPOLYSORTPRIMS 2048

// Structures

typedef struct POLYSORTPRIM{
	struct	POLYSORTPRIM * Next;
	D3DLVERTEX	v[MAXVERTSPERPOLYSORTPRIM];
	uint16	tpage;

}POLYSORTPRIM;

typedef struct BUCKETENTRY{
	struct	POLYSORTPRIM * Next;
}BUCKETENTRY;



// fn Prototypes
BOOL	InitPolySort( void );
void	ReleasePolySort( void );
BOOL PolyListExecute( void );
void	AddToPolySort( D3DLVERTEX * v1 , D3DLVERTEX * v2 , D3DLVERTEX * v3 , uint16 tpage );


#endif	//	POLYSORT_INCLUDED
