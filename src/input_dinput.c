#ifdef DXMOUSE
#include "main.h"
#include "input.h"
#include "util.h"
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0700
#include <dinput.h>

JOYSTICKINFO JoystickInfo[MAX_JOYSTICKS];

LPDIRECTINPUT lpdi = NULL;
LPDIRECTINPUTDEVICE lpdiMouse;
DIMOUSESTATE mouse;

bool dx_init_mouse( void )
{
	HRESULT  err;
	GUID guid_mouse = GUID_SysMouse;

	err = DirectInputCreate(GetModuleHandle(NULL), DIRECTINPUT_VERSION, &lpdi, NULL);
	if (FAILED(err))
		return false;

	err = IDirectInput_CreateDevice(lpdi, &guid_mouse, &lpdiMouse, NULL);
	if ( err != DI_OK )
		return false;

	err = IDirectInputDevice_SetDataFormat(lpdiMouse, &c_dfDIMouse);
	if(err != DI_OK)
		return false;

	err = IDirectInputDevice_SetCooperativeLevel(
		lpdiMouse, GetActiveWindow(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND
	);
	if(err != DI_OK)
		return false;

	err = IDirectInputDevice_Acquire(lpdiMouse);

	return true;
}

extern float real_framelag;
mouse_state_t* read_mouse( void )
{
	static float framelagfix = 0.0f;
	static mouse_state_t state;
	HRESULT hr;
	framelagfix -= real_framelag;
	if(framelagfix <= 0.0f)
	{
		framelagfix = 0.028f;
	}
	else
	{
		return &state;
	}
	hr = IDirectInputDevice_GetDeviceState(
		lpdiMouse, sizeof(DIMOUSESTATE), &mouse);
	if( hr == DI_OK )
	{
		state.xrel = mouse.lX;
		state.yrel = mouse.lY;
	}
	else if( hr == DIERR_INPUTLOST )
	{
		DebugPrintf("dx input lost - reacuiring...\n");
		IDirectInputDevice_Acquire(lpdiMouse);
	}
	else
	{
		DebugPrintf("mouse error: %d\n",hr);
	}
	return &state;
}

#endif // DXMOUSE
#ifdef DINPUTJOY

#include "input.h"
#include "util.h"
#include "render.h"
#include <SDL.h>
#include <stdio.h>
#include <windows.h>
#include "dinput.h"

int Num_Joysticks = 0;

bool  IsEqualGuid(GUID *lpguid1, GUID *lpguid2)
{
   return (
      ((PLONG) lpguid1)[0] == ((PLONG) lpguid2)[0] &&
      ((PLONG) lpguid1)[1] == ((PLONG) lpguid2)[1] &&
      ((PLONG) lpguid1)[2] == ((PLONG) lpguid2)[2] &&
      ((PLONG) lpguid1)[3] == ((PLONG) lpguid2)[3]);
}

LPDIRECTINPUT             lpdi = NULL;
LPDIRECTINPUTDEVICE2      lpdiJoystick[MAX_JOYSTICKS];
DIDEVCAPS           diJoystickCaps[MAX_JOYSTICKS];

bool FAR PASCAL InitJoystickInput(LPCDIDEVICEINSTANCE pdinst, LPVOID pvRef) 
{ 
   LPDIRECTINPUT pdi = pvRef; 
   LPDIRECTINPUTDEVICE pdev;
   HRESULT hr;
   char tempstr[255];

   // create the DirectInput joystick device 
   if (pdi->lpVtbl->CreateDevice(pdi, &pdinst->guidInstance, &pdev, NULL) != DI_OK) 
   { 
      return DIENUM_CONTINUE; 
   } 

   strcpy (tempstr, &pdinst->tszProductName[0]);
   JoystickInfo[Num_Joysticks].Name = (char *) malloc ( strlen( tempstr ) + 1 );
   strcpy (JoystickInfo[Num_Joysticks].Name, tempstr);

   hr = pdev->lpVtbl->QueryInterface( pdev, &IID_IDirectInputDevice2,
                       (LPVOID *)&lpdiJoystick[Num_Joysticks]);
   pdev->lpVtbl->Release(pdev);

   if (hr != S_OK)
     return DIENUM_CONTINUE;

   diJoystickCaps[ Num_Joysticks ].dwSize = sizeof( DIDEVCAPS );
   hr = IDirectInputDevice2_GetCapabilities( lpdiJoystick[ Num_Joysticks ], &diJoystickCaps[ Num_Joysticks ] );

   if (hr != S_OK)
     return DIENUM_CONTINUE;

   Num_Joysticks++;

   if (Num_Joysticks > MAX_JOYSTICKS)
     Num_Joysticks = MAX_JOYSTICKS;

   return DIENUM_CONTINUE;
}

/* this is called for object in our joystick */
/* objects can be buttons, axis, sliders etc... */
bool CALLBACK DIEnumDeviceObjectsProc( 
       LPCDIDEVICEOBJECTINSTANCE lpddoi, /* the object instance */
       LPVOID pvRef) /* pointer to void we passed in from calling block */
{ 

  int joysticknum, axis, dir;

  /* you have to use these or you get a warning */
  GUID AxisGUID[MAX_JOYSTICK_AXIS - 1];
  GUID ButtonGUID  = GUID_Button;
  GUID POVGUID     = GUID_POV;
  GUID CompareGUID = lpddoi->guidType; /* pointer to the current object GUID type */
  
  /* pvRef is a pointer to a passed in object from our calling code */
  /* here we passed in the current joystick number */
  joysticknum = *((int *)(pvRef));

  /* types of axis's we have to support */
  AxisGUID[0] = GUID_XAxis;
  AxisGUID[1] = GUID_YAxis;
  AxisGUID[2] = GUID_ZAxis;
  AxisGUID[3] = GUID_RxAxis;
  AxisGUID[4] = GUID_RyAxis;
  AxisGUID[5] = GUID_RzAxis;
  AxisGUID[6] = GUID_Slider; // This holds an array of up to 2 sliders

  /* loop over the AxisGUID[] Array */
  /* find out if current object is one of type in AxisGUID[] */
  for (axis = 0; axis < MAX_JOYSTICK_AXIS - 1; axis++)
  {

    /* if the object is not equal to AxisGUID[axis] then continue */
    if ( ! IsEqualGuid( &AxisGUID[axis], &CompareGUID ))
      continue;

    /* allocate the memory */
    JoystickInfo[joysticknum].Axis[axis].name = (char*) malloc (MAX_JOYNAME+1);
  
    /* assign all /0's to the stack */
    memset (JoystickInfo[joysticknum].Axis[axis].name, 0, (MAX_JOYNAME+1));

    /* if object provides a name */
    if (lpddoi->tszName && *lpddoi->tszName)
      /* use the special name */
      strncpy( JoystickInfo[joysticknum].Axis[axis].name, lpddoi->tszName, MAX_JOYNAME );
    else
      /* use generic name */
      snprintf( JoystickInfo[joysticknum].Axis[axis].name, MAX_JOYNAME, "Axis %d", axis );

    /* this current axis exists */
    JoystickInfo[joysticknum].Axis[axis].exists = true;

    /* up the count */
    /* NumAxis==1 means JoystickInfo[0] is set */
    JoystickInfo[joysticknum].NumAxis++;

    /* were done we found out what object we were dealing with */
    return DIENUM_CONTINUE; 

  }

  /* if the object is a button */
  if (IsEqualGuid( &ButtonGUID, &CompareGUID ))
  {

    /* allocate the memory */
    JoystickInfo[joysticknum].Button[
		JoystickInfo[joysticknum].NumButtons
	].name = (char *) malloc (MAX_JOYNAME+1);
  
    /* assign all /0's to the stack */
    memset (
       JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name,
       0,
       (MAX_JOYNAME+1)
    );

    /* if object provides a name */
    if (lpddoi->tszName && *lpddoi->tszName)
      /* use the special name */
      strncpy(
         JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name,
         lpddoi->tszName,
         MAX_JOYNAME
      );
    else
      /* use generic name */
      snprintf(
         JoystickInfo[joysticknum].Button[
			 JoystickInfo[joysticknum].NumButtons
		 ].name,
         MAX_JOYNAME,
         "Button %d",
         JoystickInfo[joysticknum].NumButtons
      );

    /* up the count */
    JoystickInfo[joysticknum].NumButtons++;

    /* were done with this object go to next one */
    return DIENUM_CONTINUE;

  }

  /* if the object is a POV aka Hat Switch */
  if (IsEqualGuid( &POVGUID, &CompareGUID ))
  {
    
    /* allocate the memory */
    JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name = 
        (char *) malloc (MAX_JOYNAME+1);
  
    /* assign all /0's to the stack */
    memset (
       JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name,
       0,
       (MAX_JOYNAME+1)
    );

    /* if object provides a name */
    if (lpddoi->tszName && *lpddoi->tszName)
      /* use the special name */
      strncpy(
         JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name,
         lpddoi->tszName,
         MAX_JOYNAME
      );
    else
      /* use generic name */
      snprintf(
         JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name,
         MAX_JOYNAME,
         "Hat %d",
         JoystickInfo[joysticknum].NumPOVs
      );

    /* set the name of the direction */
    for ( dir = 0; dir < MAX_POV_DIRECTIONS; dir++ )
    {

      /* allocate the memory */
      JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].dirname[ dir ] = 
          (char *) malloc (MAX_JOYNAME+1);
  
      /* assign all /0's to the stack */
      memset (
         JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].dirname[ dir ],
         0,
         (MAX_JOYNAME+1)
      );

      /* use generic name */
      snprintf(
         JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].dirname[ dir ],
         MAX_JOYNAME,
         "%s %d",
         JoystickInfo[joysticknum].POV[JoystickInfo[joysticknum].NumPOVs].name,
         JoystickInfo[joysticknum].NumPOVs
      );
    }
  
    /* up the count */
    JoystickInfo[joysticknum].NumPOVs++;

    /* were done with this object go to next */
    return DIENUM_CONTINUE; 

  }

  /* default we couldn't find out what this object is so continue */
  return DIENUM_CONTINUE; 

}

extern render_info_t render_info;

bool joysticks_init(void)
{
  HRESULT  err;
  DIPROPDWORD dipdw =
        {
            {
                sizeof(DIPROPDWORD),        // diph.dwSize
                sizeof(DIPROPHEADER),       // diph.dwHeaderSize
                0,                          // diph.dwObj
                DIPH_DEVICE,                // diph.dwHow
            },
            DINPUT_BUFFERSIZE,              // dwData
        };
	LPDIRECTINPUTDEVICE     tempJoystick = NULL;
	LPVOID joysticknumptr;
	int i, j, k;
	bool failjoystick;

    err = DirectInputCreate(GetModuleHandle(NULL), DIRECTINPUT_VERSION, &lpdi, NULL);
	if (FAILED(err))//DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, &IID_IDirectInput8, (void**)&lpdi, NULL)))
    {
		return false;
    }

  // try to create Joystick devices
  for ( i = 0; i < MAX_JOYSTICKS; i++ )
    lpdiJoystick[i] = NULL;
  Num_Joysticks = 0;

  lpdi->lpVtbl->EnumDevices(lpdi,
						DIDEVTYPE_JOYSTICK,
						//DI8DEVCLASS_GAMECTRL, 
                         InitJoystickInput, lpdi, DIEDFL_ATTACHEDONLY); 

  failjoystick = false;
  for (i = 0; i < Num_Joysticks; i++)
  {
    JoystickInfo[i].assigned = false;
    JoystickInfo[i].connected = true;
    JoystickInfo[i].NumAxis = 0;
    JoystickInfo[i].NumButtons = 0;
    JoystickInfo[i].NumPOVs = 0;

    joysticknumptr = (LPVOID)&i;
    for (j = AXIS_Start; j <= AXIS_End; j++)
    {
      JoystickInfo[i].Axis[j].exists = false;
    }
    lpdiJoystick[i]->lpVtbl->EnumObjects(lpdiJoystick[i], DIEnumDeviceObjectsProc, 
                     joysticknumptr, DIDFT_ALL); 

    for (j = AXIS_Start; j <= AXIS_End; j++)
    {
      JoystickInfo[i].Axis[j].action = SHIPACTION_Nothing;
      JoystickInfo[i].Axis[j].inverted = false;
      JoystickInfo[i].Axis[j].deadzone = 20;
      JoystickInfo[i].Axis[j].fine = true;
    }

    for (j = 0; j < JoystickInfo[i].NumButtons; j++)
      JoystickInfo[i].Button[j].action = SHIPACTION_Nothing;

    for (j = 0; j < JoystickInfo[i].NumPOVs; j++)
    {
      for (k = 0; k < MAX_POV_DIRECTIONS; k++)
        JoystickInfo[i].POV[j].action[k] = SHIPACTION_Nothing;
    }

    // Tell DirectInput that we want to receive data in joystick format
    if (IDirectInputDevice2_SetDataFormat(lpdiJoystick[i], &c_dfDIJoystick2) == DI_OK)
    {                          
      // set cooperative level
      if(IDirectInputDevice2_SetCooperativeLevel(lpdiJoystick[i], GetActiveWindow(),
                 DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) == DI_OK)
      {
        // try to acquire the Joystick
        err = IDirectInputDevice2_Acquire(lpdiJoystick[i]);
        if (err != DI_OK)
        {
          failjoystick = true;
        }
      }else
      {
        failjoystick = true;
      }
    }
	else
    {
      failjoystick = true;
    }

    
    if (failjoystick)
    {
      failjoystick = false;
      IDirectInputDevice2_Release(lpdiJoystick[i]);
      lpdiJoystick[i] = NULL;
    }

  }

  DebugPrintf( "joysticks_init: %d joysticks connected\n", Num_Joysticks );

  // if we get here, all DirectInput objects were created ok
  return true;
}

void  ReleaseJoysticks( void )
{
  int joystick, i, j;

  for (joystick = 0; joystick < Num_Joysticks; joystick++)
  {
    free (JoystickInfo[joystick].Name);

    if ( JoystickInfo[ joystick ].connected )
    {
      for (i = 0; i < JoystickInfo[joystick].NumButtons; i++)
        free (JoystickInfo[joystick].Button[i].name);

      for (i = 0; i < JoystickInfo[joystick].NumPOVs; i++)
      {
        free (JoystickInfo[joystick].POV[i].name);
        for ( j = 0; j < MAX_POV_DIRECTIONS; j++ )
        {
          if ( JoystickInfo[ joystick ].POV[ i ].dirname[ j ] )
            free( JoystickInfo[ joystick ].POV[ i ].dirname[ j ] );
          JoystickInfo[ joystick ].POV[ i ].dirname[ j ] = NULL;
        }
      }

      for (i = 0; i < MAX_JOYSTICK_AXIS; i++)
      {
        if (JoystickInfo[joystick].Axis[i].exists)
          free (JoystickInfo[joystick].Axis[i].name);     
      }
    }
  }
}

bool joysticks_cleanup( void )
{
	int i;

	for (i = 0; i < Num_Joysticks; i++)
	{
		if (lpdiJoystick[i])
		{
			IDirectInputDevice2_Unacquire(lpdiJoystick[i]);
			IDirectInputDevice2_Release(lpdiJoystick[i]);
			lpdiJoystick[i]  = NULL;
		}
	}

	if (lpdi)
	{
		IDirectInputDevice_Release(lpdi);
		lpdi     = NULL;
	}

	ReleaseJoysticks();

	return true;
}

#endif // DINPUTJOY

