#ifndef VERSION_HEADER
#define VERSION_HEADER

//
// These are some helpers that make us able to concat values as desired
//

// you need 2 levels of indirection to make things work right
// https://stackoverflow.com/questions/1489932/how-can-i-concatenate-twice-with-the-c-preprocessor-and-expand-a-macro-as-in-ar

// combine two raw snippets into one
#define CONCAT2(a,b) a ## b
#define CONCAT(a,b) CONCAT2(a,b)

// wraps a value as a string
#define STR2(x) #x
#define STR(x) STR2(x)



//
// Version information
//
//
// The basic versioning premis is:
//
//   major.network.build-platform-arch
//
//   major     = major version
//   network   = network compatability (bump on breaking changes)
//   build     = incremental build version
//   platform  = windows/linux/mac/etc.
//   arch      = 32/64bit
//

#ifndef VERSION_MAJOR
	#define VERSION_MAJOR 1
#endif

#ifndef VERSION_NETWORK
	#define VERSION_NETWORK	18
#endif

#ifndef VERSION_BUILD
    #define VERSION_BUILD 0
#endif

#ifndef VERSION_PLATFORM
    #if defined(WIN32)
        #define VERSION_PLATFORM  "WINDOWS"
    #elif defined(MACOSX)
        #define VERSION_PLATFORM  "MACOSX"
    #else
        #define VERSION_PLATFORM  "LINUX"
    #endif
#endif

#ifdef __i386__
	#define ARCH "32"
#else
	#define ARCH "64"
#endif

// the full stringified version name
// must contain no spaces for net_tracker
#define PX_VERSION_STR \
		"ProjectX-" \
		STR(VERSION_MAJOR) "." \
		STR(VERSION_NETWORK) "." \
		STR(VERSION_BUILD) "-" \
		VERSION_PLATFORM \
		"-" \
		ARCH

// the multiplayer version as an integer used for network packets
#define PX_VERSION_NETWORK_COMPAT_INT CONCAT(VERSION_MAJOR, VERSION_NETWORK)

#endif
