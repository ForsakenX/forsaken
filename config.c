
#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "controls.h"
#include "input.h"
#include "models.h"
#include "mxaload.h"
#include "config.h"
#include "util.h"

extern LIST BikeList;
extern LIST BikeComputer;
extern TEXT	MacroText1;
extern TEXT	MacroText2;
extern TEXT	MacroText3;
extern LIST BikeComputerList;
extern void ConfigureSpaceorbAxis( int joystick );

typedef int (*ReadConfig)( FILE *, USERCONFIG *, char * );

typedef struct {
	char *keyword;
	ReadConfig handle;
} READCONFIGTABLE;

extern char *JoystickPOVDirections[];

USERCONFIG default_config = {
	"Player",						// player name
	0,								// bike (0=Lokasenna)
	1,								// bike computer ( 1 = Brenda )
#ifdef PANDORA
	0.3F,							// mouse x sensitivity
	0.2F,							// mouse y sensitivity
#else
	0.6F,							// mouse x sensitivity
	0.6F,							// mouse y sensitivity
#endif
	0,								// pilot control (pitch forward = down, backward = up)
	0,								// roll control...
	0.05F,							// autolevel rate
//	{ num_keys, { key list [ num_keys ] } },
#ifdef PANDORA
	{ 2,	{ SDLK_q	} },					// pitch forward
	{ 2,	{ SDLK_e	} },					// pitch backward
	{ 2,	{ SDLK_1	} },					// yaw left
	{ 2,	{ SDLK_3	} },					// yaw right
	{ 2,	{ SDLK_LEFT		} },				// roll left
	{ 2,	{ SDLK_RIGHT	} },				// roll right
	{ 1,	{ SDLK_q } },						// slide left
	{ 1,	{ SDLK_e } },						// slide right
	{ 1,	{ SDLK_UP } },						// slide up
	{ 1,	{ SDLK_DOWN } },					// slide down
	{ 1,	{ SDLK_2	} },					// forward
	{ 1,	{ SDLK_4	} },					// backward
#else
	{ 2,	{ SDLK_DOWN	} },					// pitch forward
	{ 2,	{ SDLK_UP	} },					// pitch backward
	{ 2,	{ SDLK_LEFT	} },					// yaw left
	{ 2,	{ SDLK_RIGHT	} },				// yaw right
	{ 2,	{ SDLK_q		} },				// roll left
	{ 2,	{ SDLK_e		} },				// roll right
	{ 1,	{ SDLK_1 } },						// slide left
	{ 1,	{ SDLK_3 } },						// slide right
	{ 1,	{ SDLK_KP_MINUS } },				// slide up
	{ 1,	{ SDLK_KP_PLUS } },					// slide down
	{ 1,	{ SDLK_a	} },					// forward
	{ 1,	{ SDLK_z	} },					// backward
#endif
	{ 1,	{ SDLK_LALT	} },					// slide
	{ 0, {}  },										// roll
	{ 1,	{ SDLK_s	} },					// turbo
	{ 0, {}  },										// cruise faster
	{ 0, {}  },										// cruise slower
	{ 2,	{ LEFT_MOUSE } },					// fire primary
	{ 2,	{ RIGHT_MOUSE } },					// fire secondary
#ifdef PANDORA
	{ 1,	{ SDLK_BACKSPACE } },				// fire mine
#else
	{ 1,	{ SDLK_b } },						// fire mine
#endif
	{ 1,	{ SDLK_PAGEDOWN } },					// select next primary
	{ 1,	{ SDLK_PAGEUP } },					// select prev primary
	{ 1,	{ SDLK_HOME } },					// select next secondary
	{ 1,	{ SDLK_END } },						// select prev secondary
	{ 1,	{ SDLK_r } },						// select rear view
	{ 1,	{ SDLK_h } },						// toggle headlights
	{
#ifdef PANDORA
		{ 1, { SDLK_q } },						// select primary #1
#else
		{ 1, { SDLK_BACKQUOTE } },				// select primary #1
#endif
		{ 1, { SDLK_1	} },					// select primary #2
		{ 1, { SDLK_4	} },					// select primary #3
		{ 1, { SDLK_3	} },					// select primary #4
		{ 1, { SDLK_2	} },					// select primary #5
		{ 1, { SDLK_5	} },					// select primary #6
	},
	{
		{ 1, { SDLK_6 } },						// select secondary #1
		{ 1, { SDLK_7 } },						// select secondary #2
		{ 0, {} 	},									// select secondary #3 (thief missile -- not used)
		{ 1, { SDLK_8 } },						// select secondary #4
		{ 1, { SDLK_9 } },						// select secondary #5
		{ 1, { SDLK_0 } },						// select secondary #6
		{ 1, { SDLK_MINUS } },					// select secondary #7
		{ 1, { SDLK_y } },						// select secondary #8
		{ 1, { SDLK_u } },						// select secondary #9
		{ 1, { SDLK_i } },						// select secondary #10
		{ 0, {}  },									// select secondary #11 (spider mine -- not used)
	},
	{ 0, {}  },										// drop primary
	{ 0, {}  },										// drop secondary
	{ 0, {}  },										// drop shield
	{ 0, {}  },										// drop ammo
	{ 0, {}  },										// show messages
	{ 0, {}  },										// show statistics
	{ 0, {}  },										// show network info
	{ 1, { SDLK_RETURN } },				// send text message
#ifdef PLAYER_SPEECH_TAUNTS
	{ 0, {}  },					// send speech taunt
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
	0,								// bike (0=Lokasenna)
	1,								// bike computer ( 1 = Brenda )
#ifdef PANDORA
	0.3F,							// mouse x sensitivity
	0.2F,							// mouse y sensitivity
#else
	0.6F,							// mouse x sensitivity
	0.6F,							// mouse y sensitivity
#endif
	0,								// pilot control (pitch forward = down, backward = up)
	0,								// roll control...
	0.05F,							// autolevel rate
//	{ num_keys, { key list [ num_keys ] } },
#ifdef PANDORA
	{ 2,	{ SDLK_q	} },					// pitch forward
	{ 2,	{ SDLK_e	} },					// pitch backward
	{ 2,	{ SDLK_1	} },					// yaw left
	{ 2,	{ SDLK_3	} },					// yaw right
	{ 2,	{ SDLK_LEFT		} },				// roll left
	{ 2,	{ SDLK_RIGHT	} },				// roll right
	{ 1,	{ SDLK_q } },						// slide left
	{ 1,	{ SDLK_e } },						// slide right
	{ 1,	{ SDLK_UP } },						// slide up
	{ 1,	{ SDLK_DOWN } },					// slide down
	{ 1,	{ SDLK_2	} },					// forward
	{ 1,	{ SDLK_4	} },					// backward
#else
	{ 2,	{ SDLK_UP	} },	// pitch forward
	{ 2,	{ SDLK_DOWN	} },	// pitch backward
	{ 2,	{ SDLK_LEFT	} },	// yaw left
	{ 2,	{ SDLK_RIGHT	} },	// yaw right
	{ 2,	{ SDLK_q		} },	// roll left
	{ 2,	{ SDLK_e		} },	// roll right
	{ 1,	{ SDLK_1 } },					// slide left
	{ 1,	{ SDLK_3 } },					// slide right
	{ 1,	{ SDLK_KP_MINUS } },					// slide up
	{ 1,	{ SDLK_KP_PLUS } },						// slide down
	{ 1,	{ SDLK_a	} },						// forward
	{ 1,	{ SDLK_z	} },						// backward
#endif
	{ 1,	{ SDLK_LALT	} },					// slide
	{ 0, {}  },										// roll
	{ 1,	{ SDLK_s	} },						// turbo
	{ 0, {}  },										// cruise faster
	{ 0, {}  },										// cruise slower
	{ 2,	{ LEFT_MOUSE	} },	// fire primary
	{ 2,	{ RIGHT_MOUSE	} },		// fire secondary
#ifdef PANDORA
	{ 1,	{ SDLK_BACKSPACE } },				// fire mine
#else
	{ 1,	{ SDLK_b } },						// fire mine
#endif
	{ 1,	{ SDLK_PAGEDOWN } },						// select next primary
	{ 1,	{ SDLK_PAGEUP } },					// select prev primary
	{ 1,	{ SDLK_HOME } },						// select next secondary
	{ 1,	{ SDLK_END } },						// select prev secondary
	{ 1,	{ SDLK_r } },						// select rear view
	{ 1,	{ SDLK_h } },						// toggle headlights
	{
#ifdef PANDORA
		{ 1, { SDLK_q } },						// select primary #1
#else
		{ 1, { SDLK_BACKQUOTE } },				// select primary #1
#endif
		{ 1, { SDLK_1	} },					// select primary #2
		{ 1, { SDLK_4	} },					// select primary #3
		{ 1, { SDLK_3	} },					// select primary #4
		{ 1, { SDLK_2	} },					// select primary #5
		{ 1, { SDLK_5	} },					// select primary #6
	},
	{
		{ 1, { SDLK_6 } },						// select secondary #1
		{ 1, { SDLK_7 } },						// select secondary #2
		{ 0, {} 	},									// select secondary #3 (thief missile -- not used)
		{ 1, { SDLK_8 } },						// select secondary #4
		{ 1, { SDLK_9 } },						// select secondary #5
		{ 1, { SDLK_0 } },						// select secondary #6
		{ 1, { SDLK_MINUS } },					// select secondary #7
		{ 1, { SDLK_y } },						// select secondary #8
		{ 1, { SDLK_u } },						// select secondary #9
		{ 1, { SDLK_i } },						// select secondary #10
		{ 0, {}  },									// select secondary #11 (spider mine -- not used)
	},
	{ 0, {}  },										// drop primary
	{ 0, {}  },										// drop secondary
	{ 0, {}  },										// drop shield
	{ 0, {}  },										// drop ammo
	{ 0, {}  },										// show messages
	{ 0, {} },										// show statistics
	{ 0, {} },										// show network info
	{ 1, { SDLK_RETURN } },					// send text message
#ifdef PLAYER_SPEECH_TAUNTS
	{ 0, {}  },					// send speech taunt
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

char * get_key_name( int i )
{
	char * name = SDL_GetKeyName(i);
	if(0==strcasecmp(name,"unknown key"))
		return NULL;
	return convert_char( ' ', '_', 
		strdup(name)
	);
}

// text ,  keydef
#define KEY_MAP_LAST MOUSE_RANGE_END
VIRTUALKEYMAP vkey_map[ KEY_MAP_LAST + 1 ];

static void init_key_map( void )
{
	int i;
	static int initialized = 0;
	if(initialized)
		return;
	initialized = 1;
	for( i = 0; i <= KEY_MAP_LAST; i++ )
	{
		char * name;
		if(i > MOUSE_RANGE && i < DIK_JOYSTICK)
		{
			switch(i)
			{
			case LEFT_MOUSE:
				name = "left_mouse";
				break;
			case MIDDLE_MOUSE:
				name = "middle_mouse";
				break;
			case RIGHT_MOUSE:
				name = "right_mouse";
				break;
			case UP_MOUSE:
				name = "up_mouse";
				break;
			case DOWN_MOUSE:
				name = "down_mouse";
				break;
			default:
				{
					char n[255];
					sprintf(n,"mouse_%d",(i-LEFT_MOUSE));
					name = strdup(n);
				}
			}
		}
		else
		{
			name = get_key_name(i);
		}
		if(!name)
		{
			// key has no mapping...
			// do not attempt to map 1 to "1" etc...
			// this was the old approach and broke number keys
			vkey_map[i].keycode = 0x00;
			vkey_map[i].keyword = NULL;
			continue;
		}
		vkey_map[i].keycode = i;
		vkey_map[i].keyword = name;
		DebugPrintf("vkey_map: %s = %d\n",name,i);
	}
}


/* read user defined controls from config file */
static int
read_keydef( FILE *f, USERKEY *k, char *last_token )
{
	USERKEY save_key;
	VIRTUALKEYMAP *vk;
	int j;
	int keys_defined;

	char line[256];
	char * ptr = line;
	fgets(line,256,f);

	init_key_map();

	keys_defined = 0;
	for ( j = 0; j < MAX_KEYS_PER_CONTROL; j++ )
	{
		save_key.key[ j ] = k->key[ j ]; // save old definition in case of failure
		k->key[ j ] = 0; // clear any existing key definitions
	}
	while ( ptr && sscanf( ptr, " %80s", last_token ) == 1 )
	{
		int i;
		for ( i = 0; i < KEY_MAP_LAST; i++ )
		{
			vk = &vkey_map[i];
			if(!vk->keycode || !vk->keyword)
				continue;
			if(strcasecmp( last_token, vk->keyword ) == 0)
				break;
		}

		if ( ! vk->keyword )
			break;

		for ( j = 0; j < MAX_KEYS_PER_CONTROL; j++ )
		{
			if ( !k->key[ j ] )
			{
				k->key[ j ] = vk->keycode;
				keys_defined++;
				break;
			}
		}

		ptr = (strstr(ptr,last_token)+strlen(last_token));
	}
	last_token[0] = 0;

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

	// read the number after SELECT1 representing which primary is bound
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
read_show_messages( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->show_messages, last_token );
}

static int
read_show_stats( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->show_stats, last_token );
}

static int
read_show_networkinfo( FILE *f, USERCONFIG *u, char *last_token )
{
	return read_keydef( f, &u->show_networkinfo, last_token );
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
	u_int16_t j;
	u_int16_t pnum;

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
			if ( pnum < (u_int16_t) MAX_PRIMARY_WEAPONS )
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
	u_int16_t j;
	u_int16_t pnum;

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
			if ( pnum < (u_int16_t) MAX_SECONDARY_WEAPONS )
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
	char tempstr[255];
	int joynum;

	// move to next line...
	do
	{
		c = (char) fgetc( f );
		if ( feof( f ) )
			return 0;
	}while (c != '\n');

	// get joystick name...
	fgets( tempstr, 255, f );
	if ( feof( f ) || (strlen(tempstr) == 0))
		return 0;
	tempstr[strlen(tempstr) - 1] = '\0';

	if (fscanf( f, " %80s", last_token ) != 1)
		return 0;

	// look for matching, joystick. If found, assign actions & set assigned to true
	joy = NULL;
	for (i = 0; i < Num_Joysticks; i++)
	{
		if (strcasecmp( JoystickInfo[i].Name, tempstr ) )
			continue;

		if ( JoystickInfo[i].assigned )
			continue;

		JoystickInfo[i].assigned = true;
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
			joy->assigned = true;
			joy->connected = false;
			joy->Name = (char *)malloc(strlen(tempstr) + 1 );
			strcpy( joy->Name, tempstr );
		}
		else
			return 1;
	}

	// read in joystick button bindings
	while ( !strcasecmp( last_token, "button" ) )
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
	while ( !strcasecmp( last_token, "povdir" ) )
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
		// joystick does not have this axis
		if(!joy->Axis[axis].exists)
			continue;

		// end of file
		if ( feof( f ) )
			continue;

		// read next axis
		// axises should be defined in order!
		if ( 0 != strcasecmp(last_token, axistoken[axis]) )
			continue;

		//
		// setup joystick axis
		//

		if ( !joy->connected && joy->NumAxis < axis + 1 )
		{
			joy->NumAxis = axis + 1;
		}
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
	JoystickInfo[ joystick ].assigned = true;
	
	for ( k = AXIS_Start; k <= AXIS_End; k++ )
	{
		JoystickInfo[ joystick ].Axis[ k ].deadzone = 1;
		JoystickInfo[ joystick ].Axis[ k ].sensitivity = 0.01F;
		JoystickInfo[ joystick ].Axis[ k ].inverted = false;
		JoystickInfo[ joystick ].Axis[ k ].fine = false;
	}

#ifdef PANDORA
	JoystickInfo[ joystick ].Axis[ AXIS_XAxis ].action = SHIPACTION_SlideLeft;
	JoystickInfo[ joystick ].Axis[ AXIS_YAxis ].action = SHIPACTION_MoveForward;
	JoystickInfo[ joystick ].Axis[ AXIS_ZAxis ].action = SHIPACTION_SlideUp;
	JoystickInfo[ joystick ].Axis[ AXIS_RzAxis ].action = SHIPACTION_RotateUp;
#else
	JoystickInfo[ joystick ].Axis[ AXIS_XAxis ].action = SHIPACTION_SlideLeft;
	JoystickInfo[ joystick ].Axis[ AXIS_YAxis ].action = SHIPACTION_SlideUp;
	JoystickInfo[ joystick ].Axis[ AXIS_ZAxis ].action = SHIPACTION_MoveForward;
	JoystickInfo[ joystick ].Axis[ AXIS_RzAxis ].action = SHIPACTION_RotateUp;
#endif
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
			if ( !strcasecmp( JoystickInfo[ j ].Name, "Spacetec SpaceOrb 360" ) )
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
#ifdef PANDORA
					JoystickInfo[ j ].Axis[ k ].sensitivity = 0.01F;
#else
					JoystickInfo[ j ].Axis[ k ].sensitivity = 0.02F;
#endif
					JoystickInfo[ j ].Axis[ k ].deadzone = 20;
					JoystickInfo[ j ].Axis[ k ].inverted = false;
					JoystickInfo[ j ].Axis[ k ].fine = true;
				}
#ifdef PANDORA
				JoystickInfo[ j ].Axis[ AXIS_XAxis ].action = SHIPACTION_SlideLeft;
				JoystickInfo[ j ].Axis[ AXIS_YAxis ].action = SHIPACTION_MoveForward;
#else
				JoystickInfo[ j ].Axis[ AXIS_XAxis ].action = SHIPACTION_RotateLeft;
				JoystickInfo[ j ].Axis[ AXIS_YAxis ].action = SHIPACTION_RotateUp;
#endif
			}
			AddButton( j, 0, &u->fire_primary );
			AddButton( j, 1, &u->fire_secondary );
			AddButton( j, 2, &u->turbo );
			AddButton( j, 3, &u->fire_mine );
			AddButton( j, 4, &u->select_next_primary );
			AddButton( j, 5, &u->select_next_secondary );
			JoystickInfo[ j ].assigned = true;
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
			tempJoystickInfo[ j ].assigned = false;
			for ( k = AXIS_Start; k <= AXIS_End; k++ )
			{
				tempJoystickInfo[ j ].Axis[ k ].action = SHIPACTION_Nothing;
#ifdef PANDORA
				tempJoystickInfo[ j ].Axis[ k ].sensitivity = 0.01F;
#else
				tempJoystickInfo[ j ].Axis[ k ].sensitivity = 0.02F;
#endif
				tempJoystickInfo[ j ].Axis[ k ].deadzone = 20;
				tempJoystickInfo[ j ].Axis[ k ].inverted = false;
				tempJoystickInfo[ j ].Axis[ k ].fine = true;
			}
#ifdef PANDORA
			tempJoystickInfo[ j ].Axis[ AXIS_XAxis ].action = SHIPACTION_SlideLeft;
			tempJoystickInfo[ j ].Axis[ AXIS_YAxis ].action = SHIPACTION_MoveForward;
			if (j==0) tempJoystickInfo[j].assigned = true;	// always 1 nub activated, we want it enabled

#endif
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
		{ "show_messages",	read_show_messages	},
		{ "show_stats",	read_show_stats		},
		{ "show_networkinfo",	read_show_networkinfo	},
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
	f = file_open( cfg_name, "r" );

	// leave if it failed
	if ( !f )
	{

		// tell debuggers
		DebugPrintf( "read_config: couldn't open '%s'\n", cfg_name );

	// reset all joystick settings before exiting...
	ReInitJoysticks();

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
#ifdef PANDORA
	u->mouse_x_sensitivity = 0.3F;
	u->mouse_y_sensitivity = 0.2F;
#else
	u->mouse_x_sensitivity = 0.6F;
	u->mouse_y_sensitivity = 0.6F;
#endif
	u->send_msg.key[ 0 ] = SDLK_RETURN;

	// reset all joystick settings
	ReInitJoysticks();

	// get a 80 character wide string from file
	if ( fscanf( f, " %80s", token ) == 1 )
	{
		do
		{
			// find which keyword is equal to the current cfg value
			for ( j = 0; jumptab[ j ].keyword; j++ )
				if ( !strcasecmp( token, jumptab[ j ].keyword ) )
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
	int i;
	VIRTUALKEYMAP *vk;	
	init_key_map();

	for ( i = 0; i < KEY_MAP_LAST; i++ )
	{
		vk = &vkey_map[i];
		if(!vk->keycode || !vk->keyword)
			continue;
		if ( keycode == vk->keycode )
			return vk->keyword;
	}

	return NULL;
}

typedef struct {
	const char * name;
	const char * _char;
} ascii_map_t;

ascii_map_t key_char_lookup[] =
{
	{"[0]","0"},
	{"[1]","1"},
	{"[2]","2"},
	{"[3]","3"},
	{"[4]","4"},
	{"[5]","5"},
	{"[6]","6"},
	{"[7]","7"},
	{"[8]","8"},
	{"[9]","9"},
	{"[.]","."},
	{"[/]","/"},
	{"[*]","*"},
	{"[-]","-"},
	{"[+]","+"},
	{"equals","="},
	{"tab","\t"},
	{"space"," "},
	{NULL,NULL}
};

const char *key_char( int keycode )
{
	// get sdl name for keycode
	static char * name;
	name = (char*) key_name( keycode );

	if ( name )
	{
		// maps to a single char
		if(strlen( name ) == 1)
			return name;

		// try to find mapping
		{
			ascii_map_t * key = key_char_lookup;
			while(key++)
			{
				// last item in list
				if(!key->name)
					break;

				// if name matches
				if( 0 == strcasecmp(name,key->name) )
				{
					// return mapping
					return key->_char;
				}
			}
		}
	}

	// if less than 128 then we are ascii (per sdl/unicode docs)
	if(keycode < 128)
	{
		static char c[2];
		c[0] = (char) keycode;
		c[1] = 0;
		return c;
	}

	// didn't find anything
	// but return the long name anyway
	return name;
}

const char *key_name( int keycode )
{
	init_key_map();
	if ( KEY_ON_KEYBOARD( keycode ) || KEY_ON_MOUSE( keycode ) )
	{
		int i;
		VIRTUALKEYMAP *vk;

		for ( i = 0; i < KEY_MAP_LAST; i++ )
		{
			vk = &vkey_map[i];
			if(!vk->keycode || !vk->keyword)
				continue;
			if ( keycode == vk->keycode )
				return vk->keyword;
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
		// the first number after SELECT1 represents the primary weapon in enum table
		fprintf( f, "%s\t\t%d", ctl_name, index + 1 );

		for ( k = 0; k < keydef->keys; k++ )
		{
			if ( KEY_ON_KEYBOARD( keydef->key[ k ] ) || KEY_ON_MOUSE( keydef->key[ k ] ) )

				// each string after "SELECT1 n" represents the keys associated
				fprintf( f, "\t%s", key_fullname( keydef->key[ k ] ) );
		}
		fprintf( f, "\n" );
	}
}


static void
write_preferred( FILE *f, char *ctl_name, int16_t *order, int max_entries )
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

	f = file_open( cfg_name, "w" );
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
	write_keydef( f, "SHOW_MESSAGES", &u->show_messages );
	write_keydef( f, "SHOW_STATS", &u->show_stats );
	write_keydef( f, "SHOW_NETWORKINFO", &u->show_networkinfo );

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
