									    
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
void	AddToPolySort( D3DLVERTEX * v1 , D3DLVERTEX * v2 , D3DLVERTEX * v3 , uint16 tpage );


#endif	//	POLYSORT_INCLUDED
