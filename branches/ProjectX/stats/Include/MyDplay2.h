
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