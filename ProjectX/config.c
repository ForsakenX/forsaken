#include <stdio.h>
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
#include "controls.h"

#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "dinput.h"
#include "models.h"
#include "mxaload.h"
#include "config.h"
#include "XMem.h"

extern int	Num_Joysticks;
extern LIST BikeList;
extern LIST BikeComputer;
extern JOYSTICKINFO	JoystickInfo[MAX_JOYSTICKS];
extern TEXT	MacroText1;
extern TEXT	MacroText2;
extern TEXT	MacroText3;
extern LIST BikeComputerList;

void DebugPrintf( const char * format, ... );
#if 1
extern int total_malloced;
extern void *mem;
#if 0
#define X_malloc( THINGAMAJIG )\
 ( DebugPrintf( "malloc( %s ) total_malloced = %d address %X\n", #THINGAMAJIG, ++total_malloced , (int) (mem = malloc( THINGAMAJIG ) ) ), mem)
#define X_free( THINGAMAJIG ) \
( DebugPrintf( "address %X free( %s )  total_malloced = %d\n", (int)THINGAMAJIG, #THINGAMAJIG, --total_malloced ), free( THINGAMAJIG ) )
#endif
#endif

extern void ConfigureSpaceorbAxis( int joystick );


typedef int (*ReadConfig)( FILE *, USERCONFIG *, char * );

typedef struct {
	char *keyword;
	ReadConfig handle;
} READCONFIGTABLE;

extern char *JoystickPOVDirections[];

USERCONFIG default_config = {
	"Player",						// player name
	1,								// bike (1=Beard)
	1,								// bike computer ( 1 = Brenda )
	0.6F,							// mouse x sensitivity
	0.6F,							// mouse y sensitivity
	0,								// pilot control (pitch forward = down, backward = up)
	0,								// roll control...
	0.05F,							// autolevel rate
//	{ num_keys, { key list [ num_keys ] } },
	{ 2,	{ DIK_UP,		DIK_NUMPAD8	} },	// pitch forward
	{ 2,	{ DIK_DOWN,		DIK_NUMPAD2	} },	// pitch backward
	{ 2,	{ DIK_LEFT,		DIK_NUMPAD4	} },	// yaw left
	{ 2,	{ DIK_RIGHT,	DIK_NUMPAD6	} },	// yaw right
	{ 2,	{ DIK_Q,		DIK_NUMPAD7	} },	// roll left
	{ 2,	{ DIK_E,		DIK_NUMPAD9	} },	// roll right
	{ 1,	{ DIK_NUMPAD1 } },					// slide left
	{ 1,	{ DIK_NUMPAD3 } },					// slide right
	{ 1,	{ DIK_SUBTRACT } },					// slide up
	{ 1,	{ DIK_ADD } },						// slide down
	{ 1,	{ DIK_A	} },						// forward
	{ 1,	{ DIK_Z	} },						// backward
	{ 1,	{ DIK_LMENU	} },					// slide
	{ 0 },										// roll
	{ 1,	{ DIK_S	} },						// turbo
	{ 0 },										// cruise faster
	{ 0 },										// cruise slower
	{ 2,	{ DIK_LBUTTON, DIK_LCONTROL } },	// fire primary
	{ 2,	{ DIK_RBUTTON, DIK_SPACE } },		// fire secondary
	{ 1,	{ DIK_B } },						// fire mine
	{ 1,	{ DIK_NEXT } },						// select next primary
	{ 1,	{ DIK_PRIOR } },					// select prev primary
	{ 1,	{ DIK_HOME } },						// select next secondary
	{ 1,	{ DIK_END } },						// select prev secondary
	{ 1,	{ DIK_R } },						// select rear view
	{ 1,	{ DIK_H } },						// toggle headlights
	{
		{ 1, { DIK_GRAVE } },					// select primary #1
		{ 1, { DIK_1	} },					// select primary #2
		{ 1, { DIK_4	} },					// select primary #3
		{ 1, { DIK_3	} },					// select primary #4
		{ 1, { DIK_2	} },					// select primary #5
		{ 1, { DIK_5	} },					// select primary #6
	},
	{
		{ 1, { DIK_6 } },						// select secondary #1
		{ 1, { DIK_7 } },						// select secondary #2
		{ 0	},									// select secondary #3 (thief missile -- not used)
		{ 1, { DIK_8 } },						// select secondary #4
		{ 1, { DIK_9 } },						// select secondary #5
		{ 1, { DIK_0 } },						// select secondary #6
		{ 1, { DIK_MINUS } },					// select secondary #7
		{ 1, { DIK_Y } },						// select secondary #8
		{ 1, { DIK_U } },						// select secondary #9
		{ 1, { DIK_I } },						// select secondary #10
		{ 0 },									// select secondary #11 (spider mine -- not used)
	},
	{ 0 },										// drop primary
	{ 0 },										// drop secondary
	{ 0 },										// drop shield
	{ 0 },										// drop ammo
	{ 1, { DIK_RETURN } },					// send text message
#ifdef PLAYER_SPEECH_TAUNTS
	{ 0 },					// send speech taunt
#endif
	// weapon selection priorities and orders
	{
			5,							// primary #1 priority
			2,							// primary #2 priority
			3,							// primary #3 priority
			4,							// primary #4 priority
			1,							// primary #5 priority
			0,							// primary #6 priority
	},
	{
			5,							// primary order #1 
			4,							// primary order #2 
			1,							// primary order #3 
			2,							// primary order #4 
			3,							// primary order #5 
			0,							// primary order #6 
	},
	{
			6,							// secondary #1 priority
			5,							// secondary #2 priority
			-1,							// secondary #3 priority
			1,							// secondary #4 priority
			3,							// secondary #5 priority
			2,							// secondary #6 priority
			0,							// secondary #7 priority
			8,							// secondary #8 priority
			7,							// secondary #9 priority
			6,							// secondary #10 priority
			-1,							// secondary #11 priority
	},
	{
			6,							// secondary order #1 
			3,							// secondary order #2 
			5,							// secondary order #3 
			4,							// secondary order #4 
			1,							// secondary order #5 
			0,							// secondary order #6 
			9,							// secondary order #7 
			8,							// secondary order #8 
			7,							// secondary order #9 
			-1,							// secondary order #10
			-1,							// secondary order #11
	},

	"you ugly son of a bitch...",		// taunt 1 ( F9 )
	"Time to die...",					// taunt 2 ( F10 )
	"I will tear your soul apart...",	// taunt 3 ( F11 )
};


USERCONFIG my_controls = {
	"Player",						// player name
	1,								// bike (1=Beard)
	1,								// bike computer ( 1 = Brenda )
	0.6F,							// mouse x sensitivity
	0.6F,							// mouse y sensitivity
	0,								// pilot control (pitch forward = down, backward = up)
	0,								// roll control...
	0.05F,							// autolevel rate
//	{ num_keys, { key list [ num_keys ] } },
	{ 2,	{ DIK_UP,		DIK_NUMPAD8	} },	// pitch forward
	{ 2,	{ DIK_DOWN,		DIK_NUMPAD2	} },	// pitch backward
	{ 2,	{ DIK_LEFT,		DIK_NUMPAD4	} },	// yaw left
	{ 2,	{ DIK_RIGHT,	DIK_NUMPAD6	} },	// yaw right
	{ 2,	{ DIK_Q,		DIK_NUMPAD7	} },	// roll left
	{ 2,	{ DIK_E,		DIK_NUMPAD9	} },	// roll right
	{ 1,	{ DIK_NUMPAD1 } },					// slide left
	{ 1,	{ DIK_NUMPAD3 } },					// slide right
	{ 1,	{ DIK_SUBTRACT } },					// slide up
	{ 1,	{ DIK_ADD } },						// slide down
	{ 1,	{ DIK_A	} },						// forward
	{ 1,	{ DIK_Z	} },						// backward
	{ 1,	{ DIK_LMENU	} },					// slide
	{ 0 },										// roll
	{ 1,	{ DIK_S	} },						// turbo
	{ 0 },										// cruise faster
	{ 0 },										// cruise slower
	{ 2,	{ DIK_LBUTTON, DIK_LCONTROL } },	// fire primary
	{ 2,	{ DIK_RBUTTON, DIK_SPACE } },		// fire secondary
	{ 1,	{ DIK_B } },						// fire mine
	{ 1,	{ DIK_NEXT } },						// select next primary
	{ 1,	{ DIK_PRIOR } },					// select prev primary
	{ 1,	{ DIK_HOME } },						// select next secondary
	{ 1,	{ DIK_END } },						// select prev secondary
	{ 1,	{ DIK_R } },						// select rear view
	{ 1,	{ DIK_H } },						// toggle headlights
	{
		{ 1, { DIK_GRAVE } },					// select primary #1
		{ 1, { DIK_1	} },					// select primary #2
		{ 1, { DIK_4	} },					// select primary #3
		{ 1, { DIK_3	} },					// select primary #4
		{ 1, { DIK_2	} },					// select primary #5
		{ 1, { DIK_5	} },					// select primary #6
	},
	{
		{ 1, { DIK_6 } },						// select secondary #1
		{ 1, { DIK_7 } },						// select secondary #2
		{ 0	},									// select secondary #3 (thief missile -- not used)
		{ 1, { DIK_8 } },						// select secondary #4
		{ 1, { DIK_9 } },						// select secondary #5
		{ 1, { DIK_0 } },						// select secondary #6
		{ 1, { DIK_MINUS } },					// select secondary #7
		{ 1, { DIK_Y } },						// select secondary #8
		{ 1, { DIK_U } },						// select secondary #9
		{ 1, { DIK_I } },						// select secondary #10
		{ 0 },									// select secondary #11 (spider mine -- not used)
	},
	{ 0 },										// drop primary
	{ 0 },										// drop secondary
	{ 0 },										// drop shield
	{ 0 },										// drop ammo

	{ 1, { DIK_RETURN } },					// send text message
#ifdef PLAYER_SPEECH_TAUNTS
	{ 0 },					// send speech taunt
#endif

	// weapon selection priorities and orders
	{
			5,							// primary #1 priority
			2,							// primary #2 priority
			3,							// primary #3 priority
			4,							// primary #4 priority
			1,							// primary #5 priority
			0,							// primary #6 priority
	},
	{
			5,							// primary order #1 
			4,							// primary order #2 
			1,							// primary order #3 
			2,							// primary order #4 
			3,							// primary order #5 
			0,							// primary order #6 
	},
	{
			6,							// secondary #1 priority
			5,							// secondary #2 priority
			-1,							// secondary #3 priority
			1,							// secondary #4 priority
			3,							// secondary #5 priority
			2,							// secondary #6 priority
			0,							// secondary #7 priority
			8,							// secondary #8 priority
			7,							// secondary #9 priority
			6,							// secondary #10 priority
			-1,							// secondary #11 priority
	},
	{
			6,							// secondary order #1 
			3,							// secondary order #2 
			5,							// secondary order #3 
			4,							// secondary order #4 
			1,							// secondary order #5 
			0,							// secondary order #6 
			9,							// secondary order #7 
			8,							// secondary order #8 
			7,							// secondary order #9 
			-1,							// secondary order #10
			-1,							// secondary order #11
	},

	"you ugly son of a bitch...",		// taunt 1 ( F9 )
	"Time to die...",					// taunt 2 ( F10 )
	"I will tear your soul apart...",	// taunt 3 ( F11 )
};


USERCONFIG *player_config = &my_controls;


VIRTUALKEYMAP vkey_map[] = {
	{ "DIK_ESCAPE",         DIK_ESCAPE },      
	{ "DIK_ESC",			DIK_ESCAPE },      
	{ "DIK_1",              DIK_1 },           
	{ "DIK_2",              DIK_2 },           
	{ "DIK_3",              DIK_3 },           
	{ "DIK_4",              DIK_4 },           
	{ "DIK_5",              DIK_5 },           
	{ "DIK_6",              DIK_6 },           
	{ "DIK_7",              DIK_7 },           
	{ "DIK_8",              DIK_8 },           
	{ "DIK_9",              DIK_9 },           
	{ "DIK_0",              DIK_0 },           
	{ "DIK_-",				DIK_MINUS },           /* - on main keyboard */
	{ "DIK_MINUS",          DIK_MINUS },           /* - on main keyboard */
	{ "DIK_=",				DIK_EQUALS },      
	{ "DIK_EQUALS",         DIK_EQUALS },      
	{ "DIK_BACKSPACE",      DIK_BACK },            /* backspace */
	{ "DIK_BACK",           DIK_BACK },            /* backspace */
	{ "DIK_TAB",            DIK_TAB },         
	{ "DIK_Q",              DIK_Q },           
	{ "DIK_W",              DIK_W },           
	{ "DIK_E",              DIK_E },           
	{ "DIK_R",              DIK_R },           
	{ "DIK_T",              DIK_T },           
	{ "DIK_Y",              DIK_Y },           
	{ "DIK_U",              DIK_U },           
	{ "DIK_I",              DIK_I },           
	{ "DIK_O",              DIK_O },           
	{ "DIK_P",              DIK_P },           
	{ "DIK_[",				DIK_LBRACKET },    
	{ "DIK_LBRACKET",       DIK_LBRACKET },    
	{ "DIK_]",				DIK_RBRACKET },    
	{ "DIK_RBRACKET",       DIK_RBRACKET },    
	{ "DIK_ENTER",			DIK_RETURN },          /* Enter on main keyboard */
	{ "DIK_RETURN",         DIK_RETURN },          /* Enter on main keyboard */
	{ "DIK_LEFT_CONTROL",	DIK_LCONTROL },    
	{ "DIK_LCTRL",			DIK_LCONTROL },    
	{ "DIK_LCONTROL",       DIK_LCONTROL },    
	{ "DIK_A",              DIK_A },           
	{ "DIK_S",              DIK_S },           
	{ "DIK_D",              DIK_D },           
	{ "DIK_F",              DIK_F },           
	{ "DIK_G",              DIK_G },           
	{ "DIK_H",              DIK_H },           
	{ "DIK_J",              DIK_J },           
	{ "DIK_K",              DIK_K },           
	{ "DIK_L",              DIK_L },           
	{ "DIK_;",				DIK_SEMICOLON },   
	{ "DIK_SEMICOLON",      DIK_SEMICOLON },   
	{ "DIK_'",				DIK_APOSTROPHE },  
	{ "DIK_APOSTROPHE",     DIK_APOSTROPHE },  
	{ "DIK_GRAVE",          DIK_GRAVE },           /* accent grave */
	{ "DIK_LEFT_SHIFT",		DIK_LSHIFT },      
	{ "DIK_LSHFT",			DIK_LSHIFT },      
	{ "DIK_LSHIFT",         DIK_LSHIFT },      
	{ "DIK_BACKSLASH",      DIK_BACKSLASH },   
	{ "DIK_BACK/",			DIK_BACKSLASH },   
	{ "DIK_Z",              DIK_Z },           
	{ "DIK_X",              DIK_X },           
	{ "DIK_C",              DIK_C },           
	{ "DIK_V",              DIK_V },           
	{ "DIK_B",              DIK_B },           
	{ "DIK_N",              DIK_N },           
	{ "DIK_M",              DIK_M },           
	{ "DIK_,",				DIK_COMMA },       
	{ "DIK_COMMA",          DIK_COMMA },       
	{ "DIK_.",				DIK_PERIOD },          /* . on main keyboard */
	{ "DIK_PERIOD",			DIK_PERIOD },          /* . on main keyboard */
	{ "DIK_/",				DIK_SLASH },           /* / on main keyboard */
	{ "DIK_SLASH",          DIK_SLASH },           /* / on main keyboard */
	{ "DIK_RIGHT_SHIFT",	DIK_RSHIFT },      
	{ "DIK_RSHFT",			DIK_RSHIFT },      
	{ "DIK_RSHIFT",         DIK_RSHIFT },      
	{ "DIK_NUMPAD_*",		DIK_MULTIPLY },        /* * on numeric keypad */
	{ "DIK_NUM*",			DIK_MULTIPLY },        /* * on numeric keypad */
	{ "DIK_MULTIPLY",       DIK_MULTIPLY },        /* * on numeric keypad */
	{ "DIK_LEFT_ALT",		DIK_LMENU },           /* left Alt */
	{ "DIK_LALT",			DIK_LMENU },           /* left Alt */
	{ "DIK_LMENU",          DIK_LMENU },           /* left Alt */
	{ "DIK_SPACE",          DIK_SPACE },       
	{ "DIK_ ",				DIK_SPACE },       
	{ "DIK_CAPS_LOCK",		DIK_CAPITAL },     
	{ "DIK_CAPS",			DIK_CAPITAL },     
	{ "DIK_CAPITAL",        DIK_CAPITAL },     
	{ "DIK_F1",             DIK_F1 },          
	{ "DIK_F2",             DIK_F2 },          
	{ "DIK_F3",             DIK_F3 },          
	{ "DIK_F4",             DIK_F4 },          
	{ "DIK_F5",             DIK_F5 },          
	{ "DIK_F6",             DIK_F6 },          
	{ "DIK_F7",             DIK_F7 },          
	{ "DIK_F8",             DIK_F8 },          
	{ "DIK_F9",             DIK_F9 },          
	{ "DIK_F10",            DIK_F10 },         
	{ "DIK_NUM_LOCK",		DIK_NUMLOCK },     
	{ "DIK_NUMLK",			DIK_NUMLOCK },     
	{ "DIK_NUMLOCK",        DIK_NUMLOCK },     
	{ "DIK_SCROLL_LOCK",	DIK_SCROLL },          /* Scroll Lock */
	{ "DIK_SCRLL",			DIK_SCROLL },          /* Scroll Lock */
	{ "DIK_SCROLL",         DIK_SCROLL },          /* Scroll Lock */
	{ "DIK_NUMPAD_7",       DIK_NUMPAD7 },     
	{ "DIK_NUM7",			DIK_NUMPAD7 },     
	{ "DIK_NUMPAD_8",       DIK_NUMPAD8 },     
	{ "DIK_NUM8",			DIK_NUMPAD8 },     
	{ "DIK_NUMPAD_9",       DIK_NUMPAD9 },     
	{ "DIK_NUM9",			DIK_NUMPAD9 },     
	{ "DIK_NUMPAD_-",		DIK_SUBTRACT },        /* - on numeric keypad */
	{ "DIK_NUM-",			DIK_SUBTRACT },        /* - on numeric keypad */
	{ "DIK_SUBTRACT",       DIK_SUBTRACT },        /* - on numeric keypad */
	{ "DIK_NUMPAD_4",       DIK_NUMPAD4 },     
	{ "DIK_NUM4",			DIK_NUMPAD4 },     
	{ "DIK_NUMPAD_5",       DIK_NUMPAD5 },     
	{ "DIK_NUM5",			DIK_NUMPAD5 },     
	{ "DIK_NUMPAD_6",       DIK_NUMPAD6 },     
	{ "DIK_NUM6",			DIK_NUMPAD6 },     
	{ "DIK_NUMPAD_+",       DIK_ADD },             /* + on numeric keypad */
	{ "DIK_NUM+",           DIK_ADD },             /* + on numeric keypad */
	{ "DIK_ADD",            DIK_ADD },             /* + on numeric keypad */
	{ "DIK_NUMPAD_1",       DIK_NUMPAD1 },     
	{ "DIK_NUM1",			DIK_NUMPAD1 },     
	{ "DIK_NUMPAD_2",       DIK_NUMPAD2 },     
	{ "DIK_NUM2",			DIK_NUMPAD2 },     
	{ "DIK_NUMPAD_3",       DIK_NUMPAD3 },     
	{ "DIK_NUM3",			DIK_NUMPAD3 },     
	{ "DIK_NUMPAD_0",       DIK_NUMPAD0 },     
	{ "DIK_NUM0",			DIK_NUMPAD0 },     
	{ "DIK_NUMPAD_.",		DIK_DECIMAL },         /* . on numeric keypad */
	{ "DIK_NUM.",			DIK_DECIMAL },         /* . on numeric keypad */
	{ "DIK_DECIMAL",        DIK_DECIMAL },         /* . on numeric keypad */
	{ "DIK_F11",            DIK_F11 },         
	{ "DIK_F12",            DIK_F12 },         
					               
	{ "DIK_F13",            DIK_F13 },             /*                     (NEC PC98) */
	{ "DIK_F14",            DIK_F14 },             /*                     (NEC PC98) */
	{ "DIK_F15",            DIK_F15 },             /*                     (NEC PC98) */
					               
	{ "DIK_KANA",           DIK_KANA },            /* (Japanese keyboard)            */
	{ "DIK_CONVERT",        DIK_CONVERT },         /* (Japanese keyboard)            */
	{ "DIK_NOCONVERT",      DIK_NOCONVERT },       /* (Japanese keyboard)            */
	{ "DIK_YEN",            DIK_YEN },             /* (Japanese keyboard)            */
	{ "DIK_NUMPAD=",		DIK_NUMPADEQUALS },    /* = on numeric keypad (NEC PC98) */
	{ "DIK_NUMPADEQUALS",   DIK_NUMPADEQUALS },    /* = on numeric keypad (NEC PC98) */
	{ "DIK_CIRCUMFLEX",     DIK_CIRCUMFLEX },      /* (Japanese keyboard)            */
	{ "DIK_AT",             DIK_AT },              /*                     (NEC PC98) */
	{ "DIK_COLON",          DIK_COLON },           /*                     (NEC PC98) */
	{ "DIK_UNDERLINE",      DIK_UNDERLINE },       /*                     (NEC PC98) */
	{ "DIK_KANJI",          DIK_KANJI },           /* (Japanese keyboard)            */
	{ "DIK_STOP",           DIK_STOP },            /*                     (NEC PC98) */
	{ "DIK_AX",             DIK_AX },              /*                     (Japan AX) */
	{ "DIK_UNLABELED",      DIK_UNLABELED },       /*                        (J3100) */
	{ "DIK_NUMPAD_ENTER",   DIK_NUMPADENTER },     /* Enter on numeric keypad */
	{ "DIK_NUMENT",			DIK_NUMPADENTER },     /* Enter on numeric keypad */
	{ "DIK_RIGHT_CONTROL",	DIK_RCONTROL },    
	{ "DIK_RCTRL",			DIK_RCONTROL },    
	{ "DIK_RCONTROL",       DIK_RCONTROL },    
	{ "DIK_NUMPAD_,",		DIK_NUMPADCOMMA },     /* , on numeric keypad (NEC PC98) */
	{ "DIK_NUM,",			DIK_NUMPADCOMMA },     /* , on numeric keypad (NEC PC98) */
	{ "DIK_NUMPADCOMMA",    DIK_NUMPADCOMMA },     /* , on numeric keypad (NEC PC98) */
	{ "DIK_NUMPAD_/",		DIK_DIVIDE },          /* / on numeric keypad */
	{ "DIK_NUM/",			DIK_DIVIDE },          /* / on numeric keypad */
	{ "DIK_DIVIDE",         DIK_DIVIDE },          /* / on numeric keypad */
	{ "DIK_SYSRQ",          DIK_SYSRQ },       
	{ "DIK_RIGHT_ALT",		DIK_RMENU },           /* right Alt */
	{ "DIK_RALT",			DIK_RMENU },           /* right Alt */
	{ "DIK_RMENU",          DIK_RMENU },           /* right Alt */
	{ "DIK_HOME",           DIK_HOME },            /* Home on arrow keypad */
	{ "DIK_CURSOR_UP",      DIK_UP },              /* UpArrow on arrow keypad */
	{ "DIK_UP",             DIK_UP },              /* UpArrow on arrow keypad */
	{ "DIK_PAGE_UP",		DIK_PRIOR },           /* PgUp on arrow keypad */
	{ "DIK_PGUP",			DIK_PRIOR },           /* PgUp on arrow keypad */
	{ "DIK_PRIOR",          DIK_PRIOR },           /* PgUp on arrow keypad */
	{ "DIK_CURSOR_LEFT",    DIK_LEFT },            /* LeftArrow on arrow keypad */
	{ "DIK_LEFT",           DIK_LEFT },            /* LeftArrow on arrow keypad */
	{ "DIK_CURSOR_RIGHT",   DIK_RIGHT },           /* RightArrow on arrow keypad */
	{ "DIK_RIGHT",          DIK_RIGHT },           /* RightArrow on arrow keypad */
	{ "DIK_END",            DIK_END },             /* End on arrow keypad */
	{ "DIK_CURSOR_DOWN",    DIK_DOWN },            /* DownArrow on arrow keypad */
	{ "DIK_DOWN",           DIK_DOWN },            /* DownArrow on arrow keypad */
	{ "DIK_PAGE_DOWN",      DIK_NEXT },            /* PgDn on arrow keypad */
	{ "DIK_PGDN",           DIK_NEXT },            /* PgDn on arrow keypad */
	{ "DIK_NEXT",           DIK_NEXT },            /* PgDn on arrow keypad */
	{ "DIK_INSERT",         DIK_INSERT },          /* Insert on arrow keypad */
	{ "DIK_INS",			DIK_INSERT },          /* Insert on arrow keypad */
	{ "DIK_DELETE",         DIK_DELETE },          /* Delete on arrow keypad */
	{ "DIK_DEL",			DIK_DELETE },          /* Delete on arrow keypad */
	{ "DIK_LWIN",           DIK_LWIN },            /* Left Windows key */
	{ "DIK_RWIN",           DIK_RWIN },            /* Right Windows key */
	{ "DIK_APPS",           DIK_APPS },            /* AppMenu key */

	{ "DIK_BACKSPACE",      DIK_BACK },            /* backspace */
	{ "DIK_NUMPADSTAR",     DIK_MULTIPLY },        /* * on numeric keypad */
	{ "DIK_CAPSLOCK",       DIK_CAPITAL },         /* CapsLock */
	{ "DIK_NUMPADMINUS",    DIK_SUBTRACT },        /* - on numeric keypad */
	{ "DIK_NUMPADPLUS",     DIK_ADD },             /* + on numeric keypad */
	{ "DIK_NUMPADPERIOD",   DIK_DECIMAL },         /* . on numeric keypad */
	{ "DIK_NUMPADSLASH",    DIK_DIVIDE },          /* / on numeric keypad */
	{ "DIK_UPARROW",        DIK_UP },              /* UpArrow on arrow keypad */
	{ "DIK_LEFTARROW",      DIK_LEFT },            /* LeftArrow on arrow keypad */
	{ "DIK_RIGHTARROW",     DIK_RIGHT },           /* RightArrow on arrow keypad */
	{ "DIK_DOWNARROW",      DIK_DOWN },            /* DownArrow on arrow keypad */

	// my own "pseudo-DIK" codes for mouse controls
	{ "DIK_MOUSE_1",		DIK_LBUTTON },		// Left mouse button 
	{ "DIK_MB1",			DIK_LBUTTON },		// Left mouse button 
	{ "DIK_LBUTTON",		DIK_LBUTTON },		// Left mouse button 
	{ "DIK_MOUSE_2",		DIK_RBUTTON },		// Right mouse button 
	{ "DIK_MB2",			DIK_RBUTTON },		// Right mouse button 
	{ "DIK_RBUTTON",		DIK_RBUTTON },		// Right mouse button 
	{ "DIK_MOUSE_3",		DIK_MBUTTON },		// Middle mouse button 
	{ "DIK_MB3",			DIK_MBUTTON },		// Middle mouse button 
	{ "DIK_MBUTTON",		DIK_MBUTTON },		// Middle mouse button 
	{ "DIK_MOUSE_4",		DIK_MBUTTON },		// Middle mouse button 
	{ "DIK_MB4",			DIK_MBUTTON },		// Middle mouse button 
	{ "DIK_TBUTTON",		DIK_MBUTTON },		// Middle mouse button 
	{ "DIK_WHEEL_UP",		DIK_WHEELUP },		// Mouse wheel up 
	{ "DIK_MWUP",			DIK_WHEELUP },		// Mouse wheel up 
	{ "DIK_WHEELUP",		DIK_WHEELUP },		// Mouse wheel up 
	{ "DIK_WHEEL_DOWN",		DIK_WHEELDOWN },		// Mouse wheel down 
	{ "DIK_MWDN",			DIK_WHEELDOWN },		// Mouse wheel down 
	{ "DIK_WHEELDOWN",		DIK_WHEELDOWN },		// Mouse wheel down 

	// virtual keycodes retained from original for compatibility

	{ "VK_LBUTTON",			DIK_LBUTTON },		// Left mouse button 
	{ "VK_RBUTTON",			DIK_RBUTTON },		// Right mouse button 
//	{ "VK_CANCEL",			DIK_CANCEL },		// Control-break processing 
	{ "VK_MBUTTON",			DIK_MBUTTON },		// Middle mouse button (three-button mouse) 
	{ "VK_BACK",			DIK_BACK },			// BACKSPACE key 
	{ "VK_TAB",				DIK_TAB },			// TAB key 
//	{ "VK_CLEAR",			DIK_CLEAR },		// CLEAR key 
	{ "VK_RETURN",			DIK_RETURN },		// ENTER key 
	{ "VK_SHIFT",			DIK_LSHIFT },		// SHIFT key *** defaults to LEFT SHIFT
	{ "VK_CONTROL",			DIK_LCONTROL },		// CTRL key *** defaults to LEFT CONTROL
	{ "VK_MENU",			DIK_LMENU },		// ALT key **** defaults to LEFT ALT
//	{ "VK_PAUSE",			DIK_PAUSE },		// PAUSE key 
	{ "VK_CAPITAL",			DIK_CAPITAL },		// CAPS LOCK key 
	{ "VK_ESCAPE",			DIK_ESCAPE },		// ESC key 
	{ "VK_SPACE",			DIK_SPACE },		// SPACEBAR 
	{ "VK_PRIOR",			DIK_PRIOR },		// PAGE UP key 
	{ "VK_NEXT",			DIK_NEXT },			// PAGE DOWN key 
	{ "VK_END",				DIK_END },			// END key 
	{ "VK_HOME",			DIK_HOME },			// HOME key 
	{ "VK_LEFT",			DIK_LEFT },			// LEFT ARROW key 
	{ "VK_UP",				DIK_UP },			// UP ARROW key 
	{ "VK_RIGHT",			DIK_RIGHT },		// RIGHT ARROW key 
	{ "VK_DOWN",			DIK_DOWN },			// DOWN ARROW key 
//	{ "VK_SELECT",			DIK_SELECT },		// SELECT key 
//	{ "VK_EXECUTE",			DIK_EXECUTE },		// EXECUTE key 
//	{ "VK_SNAPSHOT",		DIK_SNAPSHOT },		// PRINT SCREEN key for Windows 3.0 and later 
	{ "VK_INSERT",			DIK_INSERT },		// INS key 
	{ "VK_DELETE",			DIK_DELETE },		// DEL key 
//	{ "VK_HELP",			DIK_HELP },			// HELP key 
	{ "VK_0",				DIK_0 },			// 0 key 
	{ "VK_1",				DIK_1 },			// 1 key 
	{ "VK_2",				DIK_2 },			// 2 key 
	{ "VK_3",				DIK_3 },			// 3 key 
	{ "VK_4",				DIK_4 },			// 4 key 
	{ "VK_5",				DIK_5 },			// 5 key 
	{ "VK_6",				DIK_6 },			// 6 key 
	{ "VK_7",				DIK_7 },			// 7 key 
	{ "VK_8",				DIK_8 },			// 8 key 
	{ "VK_9",				DIK_9 },			// 9 key 
	{ "VK_A",				DIK_A },			// A key 
	{ "VK_B",				DIK_B },			// B key 
	{ "VK_C",				DIK_C },			// C key 
	{ "VK_D",				DIK_D },			// D key 
	{ "VK_E",				DIK_E },			// E key 
	{ "VK_F",				DIK_F },			// F key 
	{ "VK_G",				DIK_G },			// G key 
	{ "VK_H",				DIK_H },			// H key 
	{ "VK_I",				DIK_I },			// I key 
	{ "VK_J",				DIK_J },			// J key 
	{ "VK_K",				DIK_K },			// K key 
	{ "VK_L",				DIK_L },			// L key 
	{ "VK_M",				DIK_M },			// M key 
	{ "VK_N",				DIK_N },			// N key 
	{ "VK_O",				DIK_O },			// O key 
	{ "VK_P",				DIK_P },			// P key 
	{ "VK_Q",				DIK_Q },			// Q key 
	{ "VK_R",				DIK_R },			// R key 
	{ "VK_S",				DIK_S },			// S key 
	{ "VK_T",				DIK_T },			// T key 
	{ "VK_U",				DIK_U },			// U key 
	{ "VK_V",				DIK_V },			// V key 
	{ "VK_W",				DIK_W },			// W key 
	{ "VK_X",				DIK_X },			// X key 
	{ "VK_Y",				DIK_Y },			// Y key 
	{ "VK_Z",				DIK_Z },			// Z key 
	{ "VK_NUMPAD0",			DIK_NUMPAD0 },		// Numeric keypad 0 key 
	{ "VK_NUMPAD1",			DIK_NUMPAD1 },		// Numeric keypad 1 key 
	{ "VK_NUMPAD2",			DIK_NUMPAD2 },		// Numeric keypad 2 key 
	{ "VK_NUMPAD3",			DIK_NUMPAD3 },		// Numeric keypad 3 key 
	{ "VK_NUMPAD4",			DIK_NUMPAD4 },		// Numeric keypad 4 key 
	{ "VK_NUMPAD5",			DIK_NUMPAD5 },		// Numeric keypad 5 key 
	{ "VK_NUMPAD6",			DIK_NUMPAD6 },		// Numeric keypad 6 key 
	{ "VK_NUMPAD7",			DIK_NUMPAD7 },		// Numeric keypad 7 key 
	{ "VK_NUMPAD8",			DIK_NUMPAD8 },		// Numeric keypad 8 key 
	{ "VK_NUMPAD9",			DIK_NUMPAD9 },		// Numeric keypad 9 key 
	{ "VK_MULTIPLY",		DIK_MULTIPLY },		// Multiply key 
	{ "VK_ADD",				DIK_ADD },			// Add key 
//	{ "VK_SEPARATOR",		DIK_SEPARATOR },	// Separator key 
	{ "VK_SUBTRACT",		DIK_SUBTRACT },		// Subtract key 
	{ "VK_DECIMAL",			DIK_DECIMAL },		// Decimal key 
	{ "VK_DIVIDE",			DIK_DIVIDE },		// Divide key 
	{ "VK_F1",				DIK_F1 },			// F1 key 
	{ "VK_F2",				DIK_F2 },			// F2 key 
	{ "VK_F3",				DIK_F3 },			// F3 key 
	{ "VK_F4",				DIK_F4 },			// F4 key 
	{ "VK_F5",				DIK_F5 },			// F5 key 
	{ "VK_F6",				DIK_F6 },			// F6 key 
	{ "VK_F7",				DIK_F7 },			// F7 key 
	{ "VK_F8",				DIK_F8 },			// F8 key 
	{ "VK_F9",				DIK_F9 },			// F9 key 
	{ "VK_F10",				DIK_F10 },			// F10 key 
	{ "VK_F11",				DIK_F11 },			// F11 key 
	{ "VK_F12",				DIK_F12 },			// F12 key 
	{ "VK_F13",				DIK_F13 },			// F13 key 
	{ "VK_F14",				DIK_F14 },			// F14 key 
	{ "VK_F15",				DIK_F15 },			// F15 key 
//	{ "VK_F16",				DIK_F16 },			// F16 key 
//	{ "VK_F17",				DIK_F17 },   		// F17 key 
//	{ "VK_F18",				DIK_F18 },   		// F18 key 
//	{ "VK_F19",				DIK_F19 },   		// F19 key 
//	{ "VK_F20",				DIK_F20 },   		// F20 key 
//	{ "VK_F21",				DIK_F21 },   		// F21 key 
//	{ "VK_F22",				DIK_F22 },   		// F22 key 
//	{ "VK_F23",				DIK_F23 },   		// F23 key 
//	{ "VK_F24",				DIK_F24 },   		// F24 key 
	{ "VK_NUMLOCK",			DIK_NUMLOCK },		// NUM LOCK key 
	{ "VK_SCROLL",			DIK_SCROLL },		// SCROLL LOCK key 

	{ NULL,					0x00 }		// (end of table marker)
};

/* read user defined controls from config file */
static int
read_keydef( FILE *f, USERKEY *k, char *last_token )
{
	USERKEY save_key;
	VIRTUALKEYMAP *vk;
	int j;
	int keys_defined;

	keys_defined = 0;
	for ( j = 0; j < MAX_KEYS_PER_CONTROL; j++ )
	{
		save_key.key[ j ] = k->key[ j ]; // save old definition in case of failure
		k->key[ j ] = 0; // clear any existing key definitions
	}
	while ( fscanf( f, " %80s", last_token ) == 1 )
	{
		for ( vk = vkey_map; vk->keyword; vk++ )
		{
			if ( !_stricmp( last_token, vk->keyword ) )
				break;
		}
		if ( vk->keyword )
		{
			for ( j = 0; j < MAX_KEYS_PER_CONTROL; j++ )
			{
				if ( !k->key[ j ] )
				{
					k->key[ j ] = vk->keycode;
					keys_defined++;
					break;
				}
			}
		}
		else
			break;
	}

	if ( keys_defined )
	{
		k->keys = keys_defined;
	}
	else
	{
		for ( j = 0; j < MAX_KEYS_PER_CONTROL; j++ )
			k->key[ j ] = save_key.key[ j ];
	}

	return keys_defined;
}


static int
read_keydef_indexed( FILE *f, USERKEY *k, char *last_token, int max_index )
{
	int index;

	if ( fscanf( f, " %d", &index ) == 1 && index > 0 && index <= max_index )
		return read_keydef( f, k + index - 1, last_token );
	else
		return 0;
}

static int
read_up( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->up, last_token );
}

static int
read_down( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->down, last_token );
}

static int
read_left( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->left, last_token );
}

static int
read_right( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->right, last_token );
}

static int
read_forward( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->move_forward, last_token );
}

static int
read_backward( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->move_backward, last_token );
}

static int
read_cruise_faster( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->cruise_faster, last_token );
}

static int
read_cruise_slower( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->cruise_slower, last_token );
}

static int
read_roll_left( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->roll_left, last_token );
}

static int
read_roll_right( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->roll_right, last_token );
}

static int
read_move_left( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->move_left, last_token );
}

static int
read_move_right( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->move_right, last_token );
}

static int
read_move_up( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->move_up, last_token );
}

static int
read_move_down( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->move_down, last_token );
}

static int
read_move( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->move, last_token );
}

static int
read_roll( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->roll, last_token );
}

static int
read_turbo( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->turbo, last_token );
}

static int
read_fire_primary( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->fire_primary, last_token );
}

static int
read_fire_secondary( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->fire_secondary, last_token );
}

static int
read_fire_mine( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->fire_mine, last_token );
}

static int
read_select_next_primary( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->select_next_primary, last_token );
}

static int
read_select_prev_primary( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->select_prev_primary, last_token );
}

static int
read_select_primary( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef_indexed( f, &u->select_primary[ 0 ], last_token, MAX_PRIMARY_WEAPONS );
}

static int
read_select_next_secondary( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->select_next_secondary, last_token );
}

static int
read_select_prev_secondary( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->select_prev_secondary, last_token );
}

static int
read_select_secondary( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef_indexed( f, &u->select_secondary[ 0 ], last_token, MAX_SECONDARY_WEAPONS );
}

static int
read_drop_primary( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->drop_primary, last_token );
}

static int
read_drop_secondary( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->drop_secondary, last_token );
}

static int
read_drop_shield( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->drop_shield, last_token );
}

static int
read_drop_ammo( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->drop_ammo, last_token );
}

static int
read_send_text( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->send_msg, last_token );
}

#ifdef PLAYER_SPEECH_TAUNTS
static int
read_send_speech( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->send_speech, last_token );
}
#endif

static int
read_full_rearview( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->full_rear_view, last_token );
}

static int
read_headlights( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->headlights, last_token );
}

static int
read_general_macro( FILE *f, char *config_macro, char *last_token, TEXT *macro )
{
	char tempstr[ MAXTEXTMSG ];
	static char fmt[80];

	sprintf( fmt, " %%%d[^\n]", MAXTEXTMSG );
	
	fscanf( f, fmt, tempstr );
   	strncpy( config_macro, tempstr, MAXTEXTMSG );
	strncpy( macro->text, tempstr, MAXTEXTMSG );

	fscanf( f, " %80s", last_token );

	return 1;
}

static int
read_macro1( FILE *f, USERCONFIG *u, char *last_token )
{
	read_general_macro( f, u->macro1, last_token, &MacroText1 );
	
	return 1;
}

static int
read_macro2( FILE *f, USERCONFIG *u, char *last_token )
{
	read_general_macro( f, u->macro2, last_token, &MacroText2 );
	
	return 1;
}

static int
read_macro3( FILE *f, USERCONFIG *u, char *last_token )
{
	read_general_macro( f, u->macro3, last_token, &MacroText3 );
	
	return 1;
}


static int
read_mouse_sensitivity( FILE *f, USERCONFIG *u, char *last_token )
{
	float s1, s2;
	int num_read;

	switch ( fscanf( f, " %f %f", &s1, &s2 ) )
	{
	case 1:
		u->mouse_x_sensitivity = s1;
		u->mouse_y_sensitivity = s1;
		num_read = 1;
		break;
	case 2:
		u->mouse_x_sensitivity = s1;
		u->mouse_y_sensitivity = s2;
		num_read = 2;
		break;
	default:
		num_read = 0;
		break;
	}
	fscanf( f, " %80s", last_token );

	return num_read;
}

static int
read_name( FILE *f, USERCONFIG *u, char *last_token )
{
	char namebuf[80];
	int ok;

	ok = fscanf( f, " %80s", namebuf );
	fscanf( f, " %80s", last_token );
	if ( ok == 1 )
	{
		strncpy( u->name, namebuf, MAX_PLAYER_NAME_LENGTH );
		u->name[ MAX_PLAYER_NAME_LENGTH - 1 ] = 0;
		return 1;
	}
	else
		return 0;
}

uint32 EncodePlayerName( char *name )
{
	uint32 code, temp;
	uint8 i;

	code = 0;
	for ( i = 0; i < strlen( name ); i++ )
	{
		if ( i < 4 )
		{
			code |= name[ i ];
			code <<= 8;
		}
		else
		{
			temp = 0;
			temp |= name[ i ];
			temp <<= i % 4; 
			code ^= temp;
		}
	}

	return ( code ^ 0x1a7d3f08 );
}

static int
read_invert_pitch( FILE *f, USERCONFIG *u, char *last_token )
{
	if ( fscanf( f, " %d", &u->invert_pitch ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

static int
read_invert_turn( FILE *f, USERCONFIG *u, char *last_token )
{
	if ( fscanf( f, " %d", &u->invert_turn ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

static int
read_bike( FILE *f, USERCONFIG *u, char *last_token )
{
	if ( fscanf( f, " %hd", &u->bike ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

static int
read_bikecomp( FILE *f, USERCONFIG *u, char *last_token )
{
	if ( fscanf( f, " %hd", &u->bikecomp ) == 1 )
	{
		if( u->bikecomp > ( BikeComputerList.items - 1 ) )
			u->bikecomp = 0;
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

static int
read_autolevel( FILE *f, USERCONFIG *u, char *last_token )
{
	if ( fscanf( f, " %f", &u->autolevel_rate ) == 1 )
	{
		fscanf( f, " %80s", last_token );
		return 1;
	}
	else
		return 0;
}

static int
read_preferred_primary( FILE *f, USERCONFIG *u, char *last_token )
{
	uint16 j;
	uint16 pnum;

	for ( j = 0; j < MAX_PRIMARY_WEAPONS; j++ )
	{
		u->primary_priority[ j ] = -1;
		u->primary_order[ j ] = -1;
	}
	j = 0;
	while ( fscanf( f, " %hd", &pnum ) == 1 )
	{
		if ( j < MAX_PRIMARY_WEAPONS )
		{
			if ( pnum >= 0 && pnum < MAX_PRIMARY_WEAPONS )
			{
				u->primary_priority[ pnum ] = j;
				u->primary_order[ j ] = pnum;
				j++;
			}
		}
	}
	fscanf( f, " %80s", last_token );
	return 1;
}


static int
read_preferred_secondary( FILE *f, USERCONFIG *u, char *last_token )
{
	uint16 j;
	uint16 pnum;

	for ( j = 0; j < MAX_SECONDARY_WEAPONS; j++ )
	{
		u->secondary_priority[ j ] = -1;
		u->secondary_order[ j ] = -1;
	}
	j = 0;
	while ( fscanf( f, " %hd", &pnum ) == 1 )
	{
		if ( j < MAX_SECONDARY_WEAPONS )
		{
			if ( pnum >= 0 && pnum < MAX_SECONDARY_WEAPONS )
			{
				u->secondary_priority[ pnum ] = j;
				u->secondary_order[ j ] = pnum;
				j++;
			}
		}
	}
	fscanf( f, " %80s", last_token );
	return 1;
}

static USERKEY *action_key( int action, USERCONFIG *u )
{
	switch ( action )
	{
	case SHIPACTION_RotateUp:
		return &u->up;
		break;
	case SHIPACTION_RotateDown:
		return &u->down;
		break;
	case SHIPACTION_RotateLeft:
		return &u->left;
		break;
	case SHIPACTION_RotateRight:
		return &u->right;
		break;
	case SHIPACTION_RollLeft:
		return &u->roll_left;
		break;
	case SHIPACTION_RollRight:
		return &u->roll_right;
		break;
	case SHIPACTION_SlideUp:
		return &u->move_up;
		break;
	case SHIPACTION_SlideDown:
		return &u->move_down;
		break;
	case SHIPACTION_SlideLeft:
		return &u->move_left;
		break;
	case SHIPACTION_SlideRight:
		return &u->move_right;
		break;
	case SHIPACTION_MoveForward:
		return &u->move_forward;
		break;
	case SHIPACTION_MoveBack:
		return &u->move_backward;
		break;
	case SHIPACTION_SlideMode:
		return &u->move;
		break;
	case SHIPACTION_RollMode:
		return &u->roll;
		break;
	case SHIPACTION_Turbo:
		return &u->turbo;
		break;
	case SHIPACTION_CruiseIncrease:
		return &u->cruise_faster;
		break;
	case SHIPACTION_CruiseDecrease:
		return &u->cruise_slower;
		break;
	case SHIPACTION_FirePrimary:
		return &u->fire_primary;
		break;
	case SHIPACTION_FireSecondary:
		return &u->fire_secondary;
		break;
	case SHIPACTION_DropMine:
		return &u->fire_mine;
		break;
	case SHIPACTION_SelectNextPrimary:
		return &u->select_next_primary;
		break;
	case SHIPACTION_SelectPrevPrimary:
		return &u->select_prev_primary;
		break;
	case SHIPACTION_SelectNextSecondary:
		return &u->select_next_secondary;
		break;
	case SHIPACTION_SelectPrevSecondary:
		return &u->select_prev_secondary;
		break;
	case SHIPACTION_DropPrimary:
		return &u->drop_primary;
		break;
	case SHIPACTION_DropSecondary:
		return &u->drop_secondary;
		break;
	case SHIPACTION_DropShield:
		return &u->drop_shield;
		break;
	case SHIPACTION_DropAmmo:
		return &u->drop_ammo;
		break;
	default:
		return NULL;
	}
}


static int
add_joystick_button( int joystick, int button, int action, USERCONFIG *u )
{
	USERKEY *k;

	k = action_key( action, u );
	if ( !k )
		return -1;
	if ( k->keys < MAX_KEYS_PER_CONTROL )
	{
		k->key[ k->keys++ ] = JOYSTICK_BUTTON_KEYCODE( joystick, button );
		return 1;
	}
	else
		return 0;
}


static int
add_joystick_povdir( int joystick, int pov, int dir, int action, USERCONFIG *u )
{
	USERKEY *k;

	k = action_key( action, u );
	if ( !k )
		return -1;
	if ( k->keys < MAX_KEYS_PER_CONTROL )
	{
		k->key[ k->keys++ ] = JOYSTICK_POVDIR_KEYCODE( joystick, pov, dir );
		return 1;
	}
	else
		return 0;
}


static int
read_joystick_info( FILE *f, USERCONFIG *u, char *last_token )
{
	JOYSTICKINFO *joy;
	char c;
	int num, i, axis, a;
	char axistoken[MAX_JOYSTICK_AXIS][80] = { "axisx", "axisy", "axisz", "axisrx", "axisry", "axisrz", "axiss1", "axiss2"};
	BOOL joystickfound = FALSE;
	char tempstr[MAX_PATH];
	int joynum;

	// move to next line...
	do
	{
		c = (char) fgetc( f );
		if ( feof( f ) )
			return 0;
	}while (c != '\n');

	// get joystick name...
	fgets( tempstr, MAX_PATH, f );
	if ( feof( f ) || (strlen(tempstr) == 0))
		return 0;
	tempstr[strlen(tempstr) - 1] = '\0';

	if (fscanf( f, " %80s", last_token ) != 1)
		return 0;

	// look for matching, joystick. If found, assign actions & set assigned to TRUE
	joy = NULL;
	for (i = 0; i < Num_Joysticks; i++)
	{
		if (_stricmp( JoystickInfo[i].Name, tempstr ) )
			continue;

		if ( JoystickInfo[i].assigned )
			continue;

		JoystickInfo[i].assigned = TRUE;
		joynum = i;
		joy = &JoystickInfo[ joynum ];
		break;
	}

	if ( !joy )
	{
		if (Num_Joysticks < MAX_JOYSTICKS)
		{
			// create joystick info, but mark as unattached 
			// (will ensure that joystick is written back to config file)
			joynum = Num_Joysticks++;
			joy = &JoystickInfo[ joynum ];
			joy->assigned = TRUE;
			joy->connected = FALSE;
			joy->Name = (char *)malloc(strlen(tempstr) + 1 );
			strcpy( joy->Name, tempstr );
		}
		else
			return 1;
	}

	// read in joystick button bindings
	while ( !_stricmp( last_token, "button" ) )
	{
		if ( fscanf( f, " %d %d", &i, &a ) == 2 )
		{
			if ( i >= 0 && i < MAX_JOYSTICK_BUTTONS )
			{
				add_joystick_button( joynum, i, a, u );
				if ( !joy->connected && joy->NumButtons < i + 1 )
					joy->NumButtons = i + 1;
			}
		}
		if (fscanf( f, " %80s", last_token ) != 1)
			return 0;
	}

	// read in joystick pov direction bindings
	while ( !_stricmp( last_token, "povdir" ) )
	{
		if ( fscanf( f, " %d %d %d", &i, &num, &a ) == 3 )
		{
			if ( i >= 0 && i < MAX_JOYSTICK_POVS
				&& num >= 0 && num < MAX_POV_DIRECTIONS )
			{
				add_joystick_povdir( joynum, i, num, a, u );
				if ( !joy->connected && joy->NumPOVs < i + 1 )
					joy->NumPOVs = i + 1;
			}
		}
		if (fscanf( f, " %80s", last_token ) != 1)
			return 0;
	}

	// read in joystickaxis
	for (axis = AXIS_Start; axis <= AXIS_End; axis++)
	{
		if ( feof( f ) )
		{
		 	joy->Axis[axis].exists = FALSE;
			continue;
		}
		if (!_stricmp( last_token, axistoken[axis] ) )
		{
			if ( !joy->connected && joy->NumAxis < axis + 1 )
				joy->NumAxis = axis + 1;
			joy->Axis[axis].exists = TRUE;
			if ( fscanf( f, " %d", &num ) != 1 )
			{
				fscanf( f, " %80s", last_token );
				continue;
			}
			joy->Axis[axis].inverted = num;
			if ( fscanf( f, " %d", &num ) != 1 )
			{
				fscanf( f, " %80s", last_token );
				continue;
			}
			joy->Axis[axis].deadzone = num;
			if ( fscanf( f, " %d", &num ) != 1 )
			{
				fscanf( f, " %80s", last_token );
				continue;
			}
			joy->Axis[ axis ].sensitivity = ( num < 100 ) ?
				1.0F / ( 100 - num ) : 1.0F;
			if ( fscanf( f, " %d", &num ) != 1 )
			{
				fscanf( f, " %80s", last_token );
				continue;
			}
			joy->Axis[ axis ].fine = num;
			if ( fscanf( f, " %d", &num ) != 1 )
			{
				fscanf( f, " %80s", last_token );
				continue;
			}
			fscanf( f, " %80s", last_token );
			joy->Axis[axis].action = num;
		}else
			joy->Axis[axis].exists = FALSE;
	}

	if ( joy->connected )
	{
		SetUpJoystickAxis( joynum );
	}

	return 1;

}


void ConfigureSpaceorbAxis( int joystick )
{
	int k;

	DebugPrintf("Spaceorb detected - auto configuring axis...\n");
	JoystickInfo[ joystick ].assigned = TRUE;
	
	for ( k = AXIS_Start; k <= AXIS_End; k++ )
	{
		JoystickInfo[ joystick ].Axis[ k ].deadzone = 1;
		JoystickInfo[ joystick ].Axis[ k ].sensitivity = 0.01F;
		JoystickInfo[ joystick ].Axis[ k ].inverted = FALSE;
		JoystickInfo[ joystick ].Axis[ k ].fine = FALSE;
	}

	JoystickInfo[ joystick ].Axis[ AXIS_XAxis ].action = SHIPACTION_SlideLeft;
	JoystickInfo[ joystick ].Axis[ AXIS_YAxis ].action = SHIPACTION_SlideUp;
	JoystickInfo[ joystick ].Axis[ AXIS_ZAxis ].action = SHIPACTION_MoveForward;
	JoystickInfo[ joystick ].Axis[ AXIS_RzAxis ].action = SHIPACTION_RotateUp;
	JoystickInfo[ joystick ].Axis[ AXIS_SliderAxis0 ].action = SHIPACTION_RotateLeft;
	JoystickInfo[ joystick ].Axis[ AXIS_SliderAxis1 ].action = SHIPACTION_RollLeft;
#if NOT_DONE_THIS_WAY
	JoystickInfo[ joystick ].Button[ 0 ].action = SHIPACTION_FirePrimary;
	JoystickInfo[ joystick ].Button[ 1 ].action = SHIPACTION_FireSecondary;
	JoystickInfo[ joystick ].Button[ 2 ].action = SHIPACTION_Turbo;
	JoystickInfo[ joystick ].Button[ 3 ].action = SHIPACTION_MoveForward;
	JoystickInfo[ joystick ].Button[ 4 ].action = SHIPACTION_SelectNextPrimary;
	JoystickInfo[ joystick ].Button[ 5 ].action = SHIPACTION_SelectNextSecondary;
#endif
	DebugPrintf( "ConfigureSpaceorbAxis: axes=%d\n", JoystickInfo[ joystick ].NumAxis );
}


int AddButton( int joystick, int button, USERKEY *k )
{
	if ( button < JoystickInfo[ joystick ].NumButtons && k->keys < MAX_KEYS_PER_CONTROL )
	{
		VirtualKeycode c;
		int j;

		c = JOYSTICK_BUTTON_KEYCODE( joystick, button );

		for ( j = 0; j < k->keys; j++ )
		{
			if ( k->key[ j ] == c )
				break;
		}
		if ( j >= k->keys )
			k->key[ k->keys++ ] = c;
		return 1;
	}
	else
		return 0;
}


void DefaultJoystickSettings( USERCONFIG *u )
{
	int j, k;

	DebugPrintf( "DefaultJoystickSettings: player '%s'\n", u->name );
	for ( j = 0; j < Num_Joysticks; j++ )
	{
		DebugPrintf( "DefaultJoystickSettings: player '%s' joystick #%d='%s' connected=%d assigned=%d\n",
			u->name,
			j,
			JoystickInfo[ j ].Name,
			JoystickInfo[ j ].connected,
			JoystickInfo[ j ].assigned );
		if ( JoystickInfo[ j ].connected && !JoystickInfo[ j ].assigned )
		{
			// if spaceorb...
			if ( !_stricmp( JoystickInfo[ j ].Name, "Spacetec SpaceOrb 360" ) )
			{
				DebugPrintf( "DefaultJoystickSettings: player '%s' joystick #%d='%s' configuring as SpaceOrb\n",
					u->name,
					j,
					JoystickInfo[ j ].Name );
				ConfigureSpaceorbAxis( j );
			}
			else
			{
				DebugPrintf( "DefaultJoystickSettings: player '%s' joystick #%d='%s' configuring normally\n",
					u->name,
					j,
					JoystickInfo[ j ].Name );
				for ( k = AXIS_Start; k <= AXIS_End; k++ )
				{
					JoystickInfo[ j ].Axis[ k ].action = SHIPACTION_Nothing;
					JoystickInfo[ j ].Axis[ k ].sensitivity = 0.02F;
					JoystickInfo[ j ].Axis[ k ].deadzone = 20;
					JoystickInfo[ j ].Axis[ k ].inverted = FALSE;
					JoystickInfo[ j ].Axis[ k ].fine = TRUE;
				}
				JoystickInfo[ j ].Axis[ AXIS_XAxis ].action = SHIPACTION_RotateLeft;
				JoystickInfo[ j ].Axis[ AXIS_YAxis ].action = SHIPACTION_RotateUp;
			}
			AddButton( j, 0, &u->fire_primary );
			AddButton( j, 1, &u->fire_secondary );
			AddButton( j, 2, &u->turbo );
			AddButton( j, 3, &u->fire_mine );
			AddButton( j, 4, &u->select_next_primary );
			AddButton( j, 5, &u->select_next_secondary );
			JoystickInfo[ j ].assigned = TRUE;
			SetUpJoystickAxis( j );
		}
	}
}


void ReInitJoysticks( void )
{
	JOYSTICKINFO tempJoystickInfo[MAX_JOYSTICKS];	
	int i, j, k;

	DebugPrintf("ReInitJoysticks: started with %d joysticks\n", Num_Joysticks);
	for (j = 0; j < Num_Joysticks; j++)
	{
		if ( !JoystickInfo[ j ].connected )
		{
			if ( JoystickInfo[ j ].Name )
			{
				free (JoystickInfo[j].Name);
				JoystickInfo[ j ].Name = NULL;
			}
		}
	}

	memset( tempJoystickInfo, 0, sizeof( JOYSTICKINFO ) * MAX_JOYSTICKS );

	j = 0;
	for ( i = 0; i < Num_Joysticks; i++ )
	{
		if ( JoystickInfo[ i ].connected )
		{
			tempJoystickInfo[ j ] = JoystickInfo[ i ];
			tempJoystickInfo[ j ].assigned = FALSE;
			for ( k = AXIS_Start; k <= AXIS_End; k++ )
			{
				tempJoystickInfo[ j ].Axis[ k ].action = SHIPACTION_Nothing;
				tempJoystickInfo[ j ].Axis[ k ].sensitivity = 0.02F;
				tempJoystickInfo[ j ].Axis[ k ].deadzone = 20;
				tempJoystickInfo[ j ].Axis[ k ].inverted = FALSE;
				tempJoystickInfo[ j ].Axis[ k ].fine = TRUE;
			}
			j++;
		}
	}

	Num_Joysticks = j;
	
	memmove( JoystickInfo, tempJoystickInfo, sizeof( JOYSTICKINFO ) * MAX_JOYSTICKS );
	DebugPrintf("ReInitJoysticks: finished with %d joysticks\n", Num_Joysticks);
}

int
read_config( USERCONFIG *u, char *cfg_name )
{

	FILE *f;
	char token[80];
	int j;

	// all the config settings
	static READCONFIGTABLE jumptab[] = {
		{ "down",			read_down					},
		{ "up",				read_up						},
		{ "left",			read_left					},
		{ "right",			read_right					},
		{ "forward", 		read_forward				},
		{ "backward",		read_backward				},
		{ "faster",			read_cruise_faster			},
		{ "slower",			read_cruise_slower			},
		{ "roll_left",		read_roll_left				},
		{ "roll_right",		read_roll_right				},
		{ "move_left",		read_move_left				},
		{ "move_right",		read_move_right				},
		{ "move_up",		read_move_up				},
		{ "move_down",		read_move_down				},
		{ "move",			read_move					},
		{ "roll",			read_roll					},
		{ "turbo",			read_turbo					},
		{ "nitro",			read_turbo					},
		{ "fire1",			read_fire_primary			},
		{ "next1",			read_select_next_primary	},
		{ "prev1",			read_select_prev_primary	},
		{ "select1",		read_select_primary			},
		{ "fire2",			read_fire_secondary			},
		{ "next2",			read_select_next_secondary	},
		{ "prev2",			read_select_prev_secondary	},
		{ "select2",		read_select_secondary		},
		{ "mine",			read_fire_mine				},
		{ "sensitivity",	read_mouse_sensitivity		},
		{ "name",			read_name					},
		{ "invert",			read_invert_pitch			},
		{ "invert_turn",	read_invert_turn			},
		{ "bike",			read_bike					},
		{ "bikecomp",		read_bikecomp				},
		{ "autolevel",		read_autolevel				},
		{ "preferred1",		read_preferred_primary		},
		{ "preferred2",		read_preferred_secondary	},
		{ "drop1",			read_drop_primary			},
		{ "drop2",			read_drop_secondary			},
		{ "dropshield",		read_drop_shield			},
		{ "dropammo",  		read_drop_ammo				},
		{ "joystick",  		read_joystick_info			},
		{ "sendtext",  		read_send_text				},
#ifdef PLAYER_SPEECH_TAUNTS
		{ "sendspeech",  	read_send_speech		   	},
#endif
		{ "full_rearview",  	read_full_rearview	   	},
		{ "headlights",  		read_headlights 	   	},

		{ "macro1",  		read_macro1					},
		{ "macro2",  		read_macro2					},
		{ "macro3",  		read_macro3					},
		{ NULL,				NULL						}
	};

	// try to open the config file.
	f = fopen( cfg_name, "r" );

	// leave if it failed
	if ( !f )
	{

		// tell debuggers
		DebugPrintf( "read_config: couldn't open '%s'\n", cfg_name );

		// failed ...
		return 0;

	}

	// clear the memory in the passed in user config
	memset( u, 0, sizeof( *u ) );

	// since all config settings are now zero, any new additions to config structure
	// must be initialised here, in order to be backwardly compatible with old config
	// files...
	u->invert_turn = 0;	
	u->invert_pitch = 0;
	u->mouse_x_sensitivity = 0.6F;
	u->mouse_y_sensitivity = 0.6F;
	u->send_msg.key[ 0 ] = DIK_RETURN;

	// reset all joystick settings
	ReInitJoysticks();


	// get a 80 character wide string from file
	if ( fscanf( f, " %80s", token ) == 1 )
	{
		do
		{
			// find which keyword is equal to the current cfg value
			for ( j = 0; jumptab[ j ].keyword; j++ )
				if ( !_stricmp( token, jumptab[ j ].keyword ) )
					break;

			// if the handle is valid
			if ( jumptab[ j ].handle )

				// execute the handle
				(jumptab[ j ].handle)( f, u, token );

			// the handle was not valid
			else

				// get the next token
				fscanf( f, " %80s", token );

		// while were not at the end of the file
		} while ( !feof( f ) );
	}
	else
	{
		// tell debuggers
		DebugPrintf( "read_config: failed at first fscanf() '%s'\n", cfg_name );
	}

	// close the config file
	fclose( f );

	// tell debuggers
	DebugPrintf( "read_config: finished reading '%s'\n", cfg_name );

	// setup the default joystick settings
	DefaultJoystickSettings( u );

	// success !
	return 1;

}


const char *key_fullname( int keycode )
{
	VIRTUALKEYMAP *vk;

	for ( vk = vkey_map; vk->keyword; vk++ )
	{
		if ( keycode == vk->keycode )
			return vk->keyword;
	}

	return NULL;
}


#ifdef USE_DINPUT_KEYNAMES

extern char *DI_KeyName( DWORD key );
extern uint8 CharTrans[256];

#endif // USE_DINPUT_KEYNAMES

const char *key_name( int keycode )
{
	if ( KEY_ON_KEYBOARD( keycode ) || KEY_ON_MOUSE( keycode ) )
	{
		VIRTUALKEYMAP *vk;
		const char *name;

#ifdef USE_DINPUT_KEYNAMES
		// check for Direct Input key name first
		name = DI_KeyName( keycode );
		if ( name && ( strlen( name ) > 1 || CharTrans[ *name ] ) )
			return name;

		// use internal key table if no Direct Input key name found
#endif // USE_DINPUT_KEYNAMES

		for ( vk = vkey_map; vk->keyword; vk++ )
		{
			if ( keycode == vk->keycode )
			{
				name = strchr( vk->keyword, '_' );
				if ( name && *++name )
					return name;
				else
					return vk->keyword; // should never need this...!
			}
		}
	}
	else if ( KEY_ON_JOYSTICK( keycode ) )
	{
		int joystick;

		joystick = KEY_JOYSTICK( keycode );
		if ( joystick >= 0 && joystick < Num_Joysticks )
		{
			JOYSTICKINFO *joy;

			joy = &JoystickInfo[ joystick ];
			if ( KEY_ON_JOYSTICK_BUTTON( keycode ) )
			{
				int button;

				button = KEY_JOYSTICK_BUTTON( keycode );
				if ( button >= 0 && button < joy->NumButtons )
					return ( joy->connected ) ? joy->Button[ button ].name : "unconnected";
			}
			else if ( KEY_ON_JOYSTICK_POV( keycode ) )
			{
#ifdef BUGGED_STATIC_POV_NAME
				static char name[ 256 ];
#endif
				int pov, dir;

				pov = KEY_JOYSTICK_POV( keycode );
				dir = KEY_JOYSTICK_POVDIR( keycode );
				if ( pov >= 0 && pov < joy->NumPOVs
					&& dir >= 0 && dir < MAX_POV_DIRECTIONS )
				{
#ifdef BUGGED_STATIC_POV_NAME
					sprintf( name, "%s %s", joy->POV[ pov ].name, JoystickPOVDirections[ dir ] );
					return name;
#else
					return ( joy->connected ) ? joy->POV[ pov ].dirname[ dir ] : "unconnected";
#endif
				}
			}
		}
	}

	return NULL;
}


static void
write_keydef( FILE *f, char *ctl_name, USERKEY *keydef )
{
	int k;

	for ( k = 0; k < keydef->keys; k++ )
	{
		if ( KEY_ON_KEYBOARD( keydef->key[ k ] ) || KEY_ON_MOUSE( keydef->key[ k ] ) )
			break;
	}

	if ( k < keydef->keys )
	{
		fprintf( f, "%s\t", ctl_name );
		for ( k = 0; k < keydef->keys; k++ )
		{
			if ( KEY_ON_KEYBOARD( keydef->key[ k ] ) || KEY_ON_MOUSE( keydef->key[ k ] ) )
				fprintf( f, "\t%s", key_fullname( keydef->key[ k ] ) );
		}
		fprintf( f, "\n" );
	}
}


static void
write_keydef_indexed( FILE *f, char *ctl_name, USERKEY *keydef, int index )
{
	int k;

	keydef += index;
	for ( k = 0; k < keydef->keys; k++ )
	{
		if ( KEY_ON_KEYBOARD( keydef->key[ k ] ) || KEY_ON_MOUSE( keydef->key[ k ] ) )
			break;
	}

	if ( k < keydef->keys )
	{
		fprintf( f, "%s\t\t%d", ctl_name, index + 1 );
		for ( k = 0; k < keydef->keys; k++ )
		{
			if ( KEY_ON_KEYBOARD( keydef->key[ k ] ) || KEY_ON_MOUSE( keydef->key[ k ] ) )
				fprintf( f, "\t%s", key_fullname( keydef->key[ k ] ) );
		}
		fprintf( f, "\n" );
	}
}


static void
write_preferred( FILE *f, char *ctl_name, int16 *order, int max_entries )
{
	int j;

	fprintf( f, "%s\t\t", ctl_name );
	for ( j = 0; j < max_entries; j++ )
	{
		if ( order[ j ] < 0 )
			break;
		fprintf( f, " %hd", order[ j ] );
	}
	fprintf( f, "\n" );
}

int
write_config( USERCONFIG *u, char *cfg_name )
{
	FILE *f;
	int j, joystick, axis, button, pov, povdir, action;
	char axistok[MAX_JOYSTICK_AXIS][80] = { "AXISX",  "AXISY",  "AXISZ",  "AXISRX",
		                                    "AXISRY", "AXISRZ", "AXISS1", "AXISS2"};

	f = fopen( cfg_name, "w" );
	if ( !f )
		return 0;

	fprintf( f, "NAME\t\t%s\n", u->name );
	fprintf( f, "BIKE\t\t%hd\n", u->bike );
	fprintf( f, "BIKECOMP\t\t%hd\n", u->bikecomp );
	fprintf( f, "AUTOLEVEL\t\t%f\n", u->autolevel_rate );
	fprintf( f, "INVERT\t\t%d\n", u->invert_pitch );
	fprintf( f, "INVERT_TURN\t%d\n", u->invert_turn );
	fprintf( f, "SENSITIVITY\t\t%f %f\n", u->mouse_x_sensitivity, u->mouse_y_sensitivity );

	write_keydef( f, "UP", &u->up );
	write_keydef( f, "DOWN", &u->down );
	write_keydef( f, "LEFT", &u->left );
	write_keydef( f, "RIGHT", &u->right );

	write_keydef( f, "FORWARD", &u->move_forward );
	write_keydef( f, "BACKWARD", &u->move_backward );

	write_keydef( f, "FASTER", &u->cruise_faster );
	write_keydef( f, "SLOWER", &u->cruise_slower );

	write_keydef( f, "MOVE_UP", &u->move_up );
	write_keydef( f, "MOVE_DOWN", &u->move_down );
	write_keydef( f, "MOVE_LEFT", &u->move_left );
	write_keydef( f, "MOVE_RIGHT", &u->move_right );

	write_keydef( f, "ROLL_LEFT", &u->roll_left );
	write_keydef( f, "ROLL_RIGHT", &u->roll_right );

	write_keydef( f, "MOVE", &u->move );
	write_keydef( f, "ROLL", &u->roll );
	write_keydef( f, "NITRO", &u->turbo );

	write_keydef( f, "FIRE1", &u->fire_primary );
	write_keydef( f, "FIRE2", &u->fire_secondary );
	write_keydef( f, "MINE", &u->fire_mine );

	write_keydef( f, "NEXT1", &u->select_next_primary );
	write_keydef( f, "NEXT2", &u->select_next_secondary );

	write_keydef( f, "PREV1", &u->select_prev_primary );
	write_keydef( f, "PREV2", &u->select_prev_secondary );

	write_keydef( f, "SENDTEXT", &u->send_msg );
#ifdef PLAYER_SPEECH_TAUNTS
	write_keydef( f, "SENDSPEECH", &u->send_speech );
#endif
	write_keydef( f, "FULL_REARVIEW", &u->full_rear_view );
	write_keydef( f, "HEADLIGHTS", &u->headlights );

	for ( j = 0; j < MAX_PRIMARY_WEAPONS; j++ )
	{
		write_keydef_indexed( f, "SELECT1", u->select_primary, j );
	}
	for ( j = 0; j < MAX_SECONDARY_WEAPONS; j++ )
	{
		write_keydef_indexed( f, "SELECT2", u->select_secondary, j );
	}

	write_keydef( f, "DROP1", &u->drop_primary );
	write_keydef( f, "DROP2", &u->drop_secondary );
	write_keydef( f, "DROPSHIELD", &u->drop_shield );
	write_keydef( f, "DROPAMMO", &u->drop_ammo );

	write_preferred( f, "PREFERRED1", u->primary_order, MAX_PRIMARY_WEAPONS );
	write_preferred( f, "PREFERRED2", u->secondary_order, MAX_SECONDARY_WEAPONS );

	fprintf( f, "MACRO1\t%s\n", MacroText1.text );
	fprintf( f, "MACRO2\t%s\n", MacroText2.text );
	fprintf( f, "MACRO3\t%s\n", MacroText3.text );


	for (joystick = 0; joystick < Num_Joysticks; joystick++)
	{
		if (JoystickInfo[joystick].assigned)
		{
			fprintf( f, "JOYSTICK\n" );
			fprintf( f, "%s\n", JoystickInfo[joystick].Name );
#ifdef OLD_JOYSTICK_FORMAT
			if ( JoystickInfo[joystick].NumButtons > 0 )
				fprintf( f, "BTN " );
			for (button = 0; button < JoystickInfo[joystick].NumButtons; button++)
			{	fprintf( f, "%d ", JoystickInfo[joystick].Button[button].action );
				if ( button == JoystickInfo[joystick].NumButtons - 1 )
					fprintf( f, "\n" );
			}
			if ( JoystickInfo[joystick].NumPOVs > 0 )
				fprintf( f, "POV " );
			for (pov = 0; pov < JoystickInfo[joystick].NumPOVs; pov++)
			{	
				for (povdir = 0; povdir < MAX_POV_DIRECTIONS; povdir++)
				{
					fprintf( f, "%d ", JoystickInfo[joystick].POV[pov].action[povdir] );
				}

				if ( pov == JoystickInfo[joystick].NumPOVs - 1 )
					fprintf( f, "\n" );
			}
#else
			for ( action = SHIPACTION_RotateUp; action < SHIPACTION_MAX; action++ )
			{
				USERKEY *k;

				k = action_key( action, u );
				if ( k )
				{
					for ( j = 0; j < k->keys; j++ )
					{
						if ( KEY_ON_JOYSTICK( k->key[ j ] ) )
						{
							if ( KEY_JOYSTICK( k->key[ j ] ) == joystick )
							{
								if ( KEY_ON_JOYSTICK_BUTTON( k->key[ j ] ) )
								{
									button = KEY_JOYSTICK_BUTTON( k->key[ j ] );
									fprintf( f, "BUTTON %d %d\n",
										button, action );
								}
							}
						}
					}
				}
			}
			for ( action = SHIPACTION_RotateUp; action < SHIPACTION_MAX; action++ )
			{
				USERKEY *k;

				k = action_key( action, u );
				if ( k )
				{
					for ( j = 0; j < k->keys; j++ )
					{
						if ( KEY_ON_JOYSTICK( k->key[ j ] ) )
						{
							if ( KEY_JOYSTICK( k->key[ j ] ) == joystick )
							{
								if ( KEY_ON_JOYSTICK_POV( k->key[ j ] ) )
								{
									pov = KEY_JOYSTICK_POV( k->key[ j ] );
									povdir = KEY_JOYSTICK_POVDIR( k->key[ j ] );
									fprintf( f, "POVDIR %d %d %d\n",
										pov, povdir, action );
								}
							}
						}
					}
				}
			}
#endif
			for (axis = 0; axis < MAX_JOYSTICK_AXIS; axis++)
			{
				if ( JoystickInfo[joystick].Axis[axis].exists )
				{
					float sensitivity;

					sensitivity = JoystickInfo[ joystick ].Axis[ axis ].sensitivity;
					fprintf( f, "%s %d %d %d %d %d\n", axistok[axis],
						JoystickInfo[joystick].Axis[axis].inverted, 
						JoystickInfo[joystick].Axis[axis].deadzone, 
						( sensitivity < 1.0F ) ?
						(int) floor( 0.49F + ( 100 * sensitivity - 1 ) / sensitivity ) : 100,
						JoystickInfo[ joystick ].Axis[ axis ].fine,
						JoystickInfo[joystick].Axis[axis].action );
				}
			}
		}
	}

	fclose( f );

	return 1;
}
