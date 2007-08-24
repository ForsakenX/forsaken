#ifndef DEMO_ID_H
#define DEMO_ID_H


//#define FULL_RETAIL
//#define SHAREWARE_RETAIL
//#define SHAREWARE_MAGAZINE
//#define SHAREWARE_INTERNET
//#define SHAREWARE_TCP

//#define SECUROM_TEST
//#define MPLAYER_TEST
//#define WIREPLAY_TEST
//#define PCTEST
//#define PRESS_PREVIEW_DEMO
//#define ACCLAIM_EUROPE
//#define ACCLAIM_NY_UK_IGUANA_UK_US
//#define ACCLAIM_LANFEST
//#define ACCLAIM_MARKETING
//#define GAMESMASTER_DEMO
//#define ECTS_DEMO
//#define ACCLAIM_NY
//#define ACCLAIM_UK
//#define IGUANA_UK
//#define IGUANA_TEXAS_DEMO
//#define ACCLAIM_INTERNET_TEST

//#define BRIAN_BRUNING_DEMO
//#define DIAMOND_DEMO
//#define RENDITION_DEMO
//#define PERMIDIA_DEMO
//#define NEC_DEMO
//#define INTEL_DEMO
//#define MICROSOFT_DEBUG
//#define MICROSOFT_MELTDOWN
//#define VIDEOLOGIC_UK_DEMO

//#define VIDEOLOGIC_SELFPLAY_DEMO
//#define NEC_SELFPLAY_DEMO
//#define OAK_SELFPLAY_DEMO
//#define AUREAL_SELFPLAY_DEMO
//#define NVIDIA_SELFPLAY_DEMO



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

// Forsaken CD name
#define CD_VOLUME_NAME			"FORSAKEN"



// localisations
#if defined( VERSION_FRENCH )

#define MARKET_FRANCE
#define MARKET_NAME "French"

#elif defined( VERSION_GERMAN )

#define MARKET_GERMANY
#define MARKET_NAME "German"
#define NO_GORE

#elif defined( VERSION_ITALIAN )

#define MARKET_ITALY
#define MARKET_NAME "Italian"

#elif defined( VERSION_SPANISH )

#define MARKET_SPAIN
#define MARKET_NAME "Spanish"

#elif defined( VERSION_UK )

#define MARKET_UK
#define MARKET_NAME "UK"

#elif defined( VERSION_KOREAN )

#define MARKET_KOREAN
#define MARKET_NAME "Korean"
#define NO_GORE

#else

#define MARKET_USA
#define MARKET_NAME "US"

#endif



#ifdef SELF_PLAY


#define FINGERPRINT	"Self-Playing Demo 31 July 1997"
#undef	EXTRA_IP

#undef EXPIRE_DAYS
#define DISABLE_IP_CHECKING



#else



#define EXPIRE_DAYS		14
#undef DISABLE_IP_CHECKING


#if defined( BRIAN_BRUNING_DEMO )

#define FINGERPRINT "Brian Bruning 3Dfx Demo Test for E3 10 June 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0x62119ECDL },	/* Brian Bruning's IP address @3Dfx 205.158.17.98 */


#elif defined( VIDEOLOGIC_UK_DEMO )

#define FINGERPRINT "Videologic UK PowerVR Debug Test (John Catchpole) 5 February 1998"
#define EXTRA_IP	{ 0x00FFFFFFL, 0x8AC809C0L },	/* PowerVR IP address 192.9.200.138* */ \
					{ 0x00FFFFFFL, 0x0005A8C0L },	/* PowerVR IP address #2 192.168.5.* */


#elif defined( VIDEOLOGIC_SELFPLAY_DEMO )

#define FINGERPRINT "Videologic PowerVR Self-Playing Demo 26 July 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0xA9C809C0L },	/* PowerVR IP address 192.9.200.169 */ \
					{ 0xFFFFFFFFL, 0x0505A8C0L },	/* PowerVR IP address #2 192.168.5.5 */


#elif defined( NEC_DEMO )

#define FINGERPRINT "NEC PowerVR Demo Test for E3 10 June 1997"
#define EXTRA_IP	{ 0x00FFFFFFL, 0x00f481ceL },	/* NEC PowerVR Pcs at E3....206.129.244.??? */


#elif defined( NEC_SELFPLAY_DEMO )

#define FINGERPRINT "NEC PowerVR Self-Playing Demo 26 July 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0x1Af481ceL },	/* invented by request (but based on E3 PC IPs)....206.129.244.26 */


#elif defined( RENDITION_DEMO )

#define FINGERPRINT	"Rendition Demo Test for E3 12 June 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0x0100007FL },	/* J. Eisenlohr, Rendition, 1675 North Shoreline Blvd., Mountain View Ca, 94043, IP=127.0.0.1 */ \
					{ 0xFFFFFFFFL, 0x97E9F2CCL },	/* Mark Nillsa, Rendition, 204.242.233.151 */


#elif defined( DIAMOND_DEMO )

#define FINGERPRINT	"Diamond 3Dfx Demo Test for E3 10 June 1997"
#define EXTRA_IP	{ 0xFEFFFFFFL, 0xA6B4FAC7L },	/* Diamond 2 pc's (199.250.180.166, 199.250.180.167) Savannah Kimbel 408 325 7915 or John Kim 408 325 7284 */


#elif defined( OAK_SELFPLAY_DEMO )

#define FINGERPRINT	"Oak Technology Self-Playing Demo 26 July 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0x1A00007FL },	/* invented by request 127.0.0.26 */


#elif defined( INTEL_DEMO )

#define FINGERPRINT	"Intel Demo Test for E3 10 June 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0x0200007FL },	/* none specified, so this is an invented IP address...cos James told me to!??? */


#elif defined( PERMIDIA_DEMO )

#define FINGERPRINT "3D Labs Permidia Demo Test for E3 16 June 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0xCAD880C1L },	/* Richard Huddy 3D Labs Permidia 193.128.216.202 */ \
					{ 0xF8FFFFFFL, 0x28C0C0C0L },	/* E3 machines 192.192.192.40-47 incl */ \
					{ 0xFFFFFFFFL, 0x30C0C0C0L },	/* E3 machine 192.192.192.48 */


#elif defined( AUREAL_SELFPLAY_DEMO )

#define FINGERPRINT	"Aureal Semiconductor Self-Playing Demo 26 July 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0x5B807695L },	/* 149.118.128.91 */


#elif defined( NVIDIA_SELFPLAY_DEMO )

#define FINGERPRINT	"NVIDIA Self-Playing Demo 26 July 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0x1B00007FL },	/* invented by request 127.0.0.27 */


#elif defined( MICROSOFT_DEBUG )

#define FINGERPRINT "Microsoft Debug (Ajay Jindal) 28 July 1997"
#undef EXPIRE_DAYS
#define EXPIRE_DAYS		14
#define DISABLE_IP_CHECKING
	/* Microsoft debug version -- not IP protected, expires after 14 days */


#elif defined( IGUANA_TEXAS_DEMO )

#define FINGERPRINT	"Iguana Texas Demo 4 July 1997"
#define EXTRA_IP	{ 0x00FFFFFFL, 0x00006764L },	/* Iguana Texas	*/


#elif defined( MICROSOFT_MELTDOWN )

#define FINGERPRINT "Microsoft Meltdown Test 14 July 1997"
#undef EXPIRE_DAYS
#define EXPIRE_DAYS		3
#define DISABLE_IP_CHECKING
	/* Microsoft Meltdown version -- not IP protected, expires after 3 days */


#elif defined( IGUANA_UK )

#define FINGERPRINT	"Iguana UK (Robert McCracken) Demo 21 July 1997"
#define EXTRA_IP	{ 0x00FFFFFFL, 0x00006b64L },	/* Iguana 0... */ \
					{ 0x00FFFFFFL, 0x00016b64L },	/* Iguana 1... */ \
					{ 0x00FFFFFFL, 0x00026b64L },	/* Iguana 2... */


#elif defined( ACCLAIM_UK )


#define FINGERPRINT	"Acclaim UK Demo (Harvey Elliot) 1 October 1997"
#define EXTRA_IP	{ 0x00FFFFFFL, 0x00006964L },	/* Acclaim UK */


#elif defined( ACCLAIM_NY )

#define FINGERPRINT	"Acclaim Demo (Shawn Rosen) 21 July 1997"
#define EXTRA_IP	{ 0x00FFFFFFL, 0x00070064L },	/*  Acclaim's New York IP domain */ \
					{ 0x00FFFFFFL, 0x00050064L },	/* Acclaim's ??? IP domain */ \
					{ 0x00FFFFFFL, 0x00006864L },	/* Sculptured US */	\
					{ 0x00FFFFFFL, 0x00006A64L },	/* Probe's Acclaim IP */ \
					{ 0x00FFFFFFL, 0x00006C64L },	/* Acclaim Germany */ \
					{ 0x00FFFFFFL, 0x00006D64L },	/* Acclaim France */ \
					{ 0x00FFFFFFL, 0x0069F026L },	/* Acclaim's red network... */
#define DISABLE_IP_CHECKING
#define DISPLAY_BUILD_DATETIME


#elif defined( ECTS_DEMO )

#define FINGERPRINT "ECTS Demo 2 September 1997"
#undef EXTRA_IP
#undef EXPIRE_DAYS
#define EXPIRE_DAYS		8


#elif defined( GAMESMASTER_DEMO )

#define FINGERPRINT	"Rob Wood Gamesmaster Challenge 20 October 1997"
#define EXTRA_IP	{ 0xF8FFFFFFL, 0x8000007FL },	/* invented 127.0.0.128-135 */
#undef EXPIRE_DAYS
#define EXPIRE_DAYS		40	/* yes forty days...to last until the end of November (OK'd by Jimmy!) */


#elif defined( PCIDENT )

#define FINGERPRINT "Sneak Preview Demo 11 December 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0x940F3FC3L },	/* PC gamer: fs	195.63.15.148 */ \
					{ 0xFFFFFFFFL, 0x950F3FC3L },	/* PC gamer: jm	195.63.15.149 */ \
					{ 0xFFFFFFFFL, 0x910F3FC3L },	/* PC gamer: om	195.63.15.145 */ \
					{ 0xFFFFFFFFL, 0x900F3FC3L },	/* PC gamer: tb	195.63.15.144 */ \
					{ 0xFFFFFFFFL, 0x990F3FC3L },	/* PC gamer: cm	195.63.15.153 */ \
					{ 0xFFFFFFFFL, 0x980F3FC3L },	/* PC gamer: cb	195.63.15.152 */ \
					{ 0xFFFFFFFFL, 0xA70F3FC3L },	/* PC gamer: hf	195.63.15.167 */
#undef EXTRA_IP
#define DISABLE_IP_CHECKING
#undef EXPIRE_DAYS
#define EXPIRE_DAYS 30	/* thirty days for the magazines */


#elif defined( ACCLAIM_INTERNET_TEST )

#define FINGERPRINT	"Acclaim Internet Test (Shawn Rosen) 1 October 1997"
#undef EXTRA_IP
#define DISABLE_IP_CHECKING


#elif defined( ACCLAIM_MARKETING )

#define FINGERPRINT	"Acclaim Marketing Demo (Shawn Rosen) 21 October 1997"
#define EXTRA_IP	{ 0x00FFFFFFL, 0x00070064L },	/*  Acclaim's New York IP domain */ \
					{ 0x00FFFFFFL, 0x00050064L },	/* Acclaim's ??? IP domain */ \
					{ 0x00FFFFFFL, 0x00006864L },	/* Sculptured US */	\
					{ 0x00FFFFFFL, 0x00006A64L },	/* Probe's Acclaim IP */ \
					{ 0x00FFFFFFL, 0x00006C64L },	/* Acclaim Germany */ \
					{ 0x00FFFFFFL, 0x00006D64L },	/* Acclaim France */ \
					{ 0x00FFFFFFL, 0x0069F026L },	/* Acclaim's red network... */
#undef EXPIRE_DAYS
#define EXPIRE_DAYS		30	/* thirty days...at Tony's request */


#elif defined( ACCLAIM_LANFEST )

#define FINGERPRINT	"Acclaim LANfest Demo (Shawn Rosen) 4 March 1998"
#define EXTRA_IP	{ 0xF0FFFFFFL, 0x00070064L },	/* Acclaim's New York IP domain for LANfest 100.0.7.0 - 100.0.7.15 */


#elif defined( ACCLAIM_NY_UK_IGUANA_UK_US )
#define FINGERPRINT	"Acclaim/Iguana US/UK Demo (5 February 1998)"
#define EXTRA_IP	{ 0x00FFFFFFL, 0x00070064L },	/*  Acclaim's New York IP domain */ \
					{ 0x00FFFFFFL, 0x00050064L },	/* Acclaim's ??? IP domain */ \
					{ 0x00FFFFFFL, 0x0069F026L },	/* Acclaim's red network... */ \
					{ 0x00FFFFFFL, 0x00006964L },	/* Acclaim UK */ \
					{ 0x00FFFFFFL, 0x00006b64L },	/* Iguana UK 0... */ \
					{ 0x00FFFFFFL, 0x00016b64L },	/* Iguana UK 1... */ \
					{ 0x00FFFFFFL, 0x00026b64L },	/* Iguana UK 2... */ \
					{ 0x00FFFFFFL, 0x00006764L },	/* Iguana Texas	*/


#elif defined( ACCLAIM_EUROPE )
#define FINGERPRINT	"Acclaim Europe Demo (4 March 1998)"
#define EXTRA_IP	{ 0x00FFFFFFL, 0x00006964L },	/* Acclaim UK */ \
					{ 0x00FFFFFFL, 0x00006b64L },	/* Iguana UK 0... */ \
					{ 0x00FFFFFFL, 0x00016b64L },	/* Iguana UK 1... */ \
					{ 0x00FFFFFFL, 0x00026b64L },	/* Iguana UK 2... */ \
					{ 0x00FFFFFFL, 0x00006C64L },	/* Acclaim Germany */ \
					{ 0x00FFFFFFL, 0x00006D64L },	/* Acclaim France */


#elif defined( SHAREWARE_MAGAZINE )

#define FINGERPRINT "Shareware Coverdisk Demo (" MARKET_NAME ") 29 January 1997"
#undef EXTRA_IP
#undef EXPIRE_DAYS
#define DISABLE_IP_CHECKING


#elif defined( SHAREWARE_RETAIL )

#define FINGERPRINT "Shareware Retail Demo (" MARKET_NAME ") 29 January 1997"
#undef EXTRA_IP
#undef EXPIRE_DAYS
#define DISABLE_IP_CHECKING
#ifdef MARKET_USA
#define MINDSPRING
#endif

#elif defined( SHAREWARE_INTERNET )

#define FINGERPRINT "Shareware Internet Demo (" MARKET_NAME ") 29 January 1997"
#undef EXTRA_IP
#undef EXPIRE_DAYS
#define DISABLE_IP_CHECKING


#elif defined( SHAREWARE_TCP )

#define FINGERPRINT "Shareware TCP Demo (" MARKET_NAME ") 7 January 1997"
#undef EXTRA_IP
#undef EXPIRE_DAYS
#define DISABLE_IP_CHECKING


#elif defined( SHAREWARE )

#define FINGERPRINT "Shareware Test (Shawn Rosen) 6 January 1997"
#undef EXTRA_IP
#undef EXPIRE_DAYS
#define EXPIRE_DAYS		14
#define DISABLE_IP_CHECKING

#elif defined( WIN98SHAREWARE )

#define FINGERPRINT "Win98 Shareware Beta"
#undef EXTRA_IP
#undef EXPIRE_DAYS
#define EXPIRE_DAYS		14 // luckily this check is disabled by the FINAL_RELEASE flag -- but how we laughed when we thought it was still active on 2 million Win98 CDs...eh Phil!
#define DISABLE_IP_CHECKING
#define UNSCRAMBLE
#define NO_GORE

#elif defined( PRESS_PREVIEW_DEMO )
#define FINGERPRINT "Press Preview Demo 18 February 1997"
#undef EXTRA_IP
#undef EXPIRE_DAYS
#define EXPIRE_DAYS		30
#define DISABLE_IP_CHECKING


#elif defined( PCTEST )
#define FINGERPRINT "PCTEST Compatibility Test 5 March 1998"
#undef EXTRA_IP
#undef EXPIRE_DAYS
#define EXPIRE_DAYS		30
#define DISABLE_IP_CHECKING


#elif defined( MPLAYER_TEST )
#define FINGERPRINT "MPATH Test (Steve Fischer) 26 March 1998"
#undef EXTRA_IP
#define DISABLE_IP_CHECKING
//#define UNSCRAMBLE
#define LIMITED_LEVELS


#elif defined( WIREPLAY_TEST )
#define FINGERPRINT "BT Wireplay Test 26 March 1998"
#undef EXTRA_IP
#define DISABLE_IP_CHECKING
#define UNSCRAMBLE
#define LIMITED_LEVELS


#elif defined( SECUROM_TEST )

#define FINGERPRINT	"Securom Test 28 March 1998"
#undef EXTRA_IP
#define DISABLE_IP_CHECKING


#elif defined( FULL_RETAIL )

#define FINGERPRINT "Full Retail Version (" MARKET_NAME ") 11 April 1998"
#undef EXTRA_IP
#define DISABLE_IP_CHECKING


#elif defined( GATEWAY )

#define FINGERPRINT "Gateway Version 24 June 1998"
#undef EXTRA_IP
#define DISABLE_IP_CHECKING


#elif defined( FULL_RETAIL_PATCH )

#define FINGERPRINT "Full Retail Patch Version (" MARKET_NAME ") 8 July 1998"
#undef EXTRA_IP
#define DISABLE_IP_CHECKING


#else


#define FINGERPRINT "Probe Internal Version 21 July 1997"
#define EXTRA_IP	{ 0xFFFFFFFFL, 0x2112C095L },	/* Nick Magee's IP address 149.192.18.33 */
#undef MARKET_USA
#undef MARKET_NAME
#define MARKET_UK
#define MARKET_NAME "UK"


#endif


#endif // !SELF_PLAY

#define PATCH_VERSION	1.00

#endif //DEMO_ID_H
