
// registry info
#define REGISTRY_ROOT_KEY		HKEY_LOCAL_MACHINE

#ifdef FINAL_RELEASE

#if defined( WIN98SHAREWARE )
#define REGISTRY_GAME_KEY		(TEXT("Software\\Acclaim Entertainment\\Forsaken Demo\\1.10"))
#elif defined( GATEWAY )
#define REGISTRY_GAME_KEY		(TEXT("Software\\Acclaim Entertainment\\Forsaken\\1.01"))
#else
#define REGISTRY_GAME_KEY		(TEXT("Software\\Acclaim Entertainment\\Forsaken\\1.00"))
#endif

#elif defined( SHAREWARE )
#define REGISTRY_GAME_KEY		(TEXT("Software\\Acclaim\\Forsaken Demo\\1.0"))
#else
#define REGISTRY_GAME_KEY		(TEXT("Software\\Acclaim\\Forsaken Demo\\0.1"))
#endif

#ifdef	WIN98SHAREWARE
#define REGISTRY_LOBBY_KEY		(TEXT("Software\\Microsoft\\DirectPlay\\Applications\\Forsaken Demo"))
#else
#define REGISTRY_LOBBY_KEY		(TEXT("Software\\Microsoft\\DirectPlay\\Applications\\Forsaken"))
#endif
