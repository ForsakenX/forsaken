#ifndef VERSION_HEADER
#define VERSION_HEADER

// major version
#define PXV	 "1"

// multiplayer version (increase if you break multiplayer compatibility)
#define PXMPV	 "18"

// multiplayer compatibility flag
		// TODO: use this format in future for now hard coded to existing format
		//#define PXMPVINT PXV.PXMPV
#define PXMPVINT 118

// revision (should be provided at build time for official builds)
// make PXRV=$(svn info | grep Revision | awk '{print $NF}')
#ifndef PXRV
    #define PXRV	 "0"
#endif

// tag name so we know what type of build this is
#ifndef PXBV
    #ifdef WIN32
        #ifdef D3D
            #define PXBV  "D3D9"
        #else
            #define PXBV  "OPEN"
        #endif
    #elif defined(MACOSX)
        #define PXBV  "MACOSX"
    #else
        #define PXBV  "LINUX"
    #endif
#endif

// architecture
#ifdef __i386__
  #define ARCH "32"
#else
  #define ARCH "64"
#endif

// the full version name (must contain no spaces for net_tracker)
#define PXVersion "ProjectX-" PXV "." PXMPV "." PXRV "-" PXBV "-" ARCH

// this should be plenty if not please increase this
#define MAX_PXVersion 45

#endif
