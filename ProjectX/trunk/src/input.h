#ifndef INPUT_INCLUDED
#define INPUT_INCLUDED

#define DIRECTINPUT_VERSION 0x0700

#include "dinput.h"
#include "controls.h"

struct {
	int wheel;
	int buttons[3];
} mouse_state;

#endif