
#include "input.h"
#include "util.h"
#include "render.h"

#include <stdio.h>
#include <windows.h>

extern JOYSTICKINFO JoystickInfo[MAX_JOYSTICKS]; 

LPDIRECTINPUT                   lpdi = NULL;
LPDIRECTINPUTDEVICE             lpdiMouse = NULL;
LPDIRECTINPUTDEVICE       lpdiKeyboard = NULL;
LPDIRECTINPUTDEVICE       lpdiBufferedKeyboard = NULL;
LPDIRECTINPUTDEVICE2      lpdiJoystick[MAX_JOYSTICKS];
DIDEVCAPS           diJoystickCaps[MAX_JOYSTICKS];
int               Num_Joysticks;
RECT cursorclip;

BOOL is_space_pressed( void )
{
	return IsKeyPressed( DIK_SPACE );
}

void flush_keyboard( void )
{
	if (lpdiBufferedKeyboard)
	{
		DWORD dwItems = INFINITE;
		IDirectInputDevice_GetDeviceData( lpdiBufferedKeyboard, sizeof(DIDEVICEOBJECTDATA), NULL, &dwItems, 0); 
	}
}

BOOL FAR PASCAL InitJoystickInput(LPCDIDEVICEINSTANCE pdinst, 
                                  LPVOID pvRef) 
{ 
   LPDIRECTINPUT pdi = pvRef; 
   LPDIRECTINPUTDEVICE pdev;
   HRESULT hr;
   char tempstr[MAX_PATH];

   // create the DirectInput joystick device 
   if (pdi->lpVtbl->CreateDevice(pdi, &pdinst->guidInstance, 
                                 &pdev, NULL) != DI_OK) 
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
BOOL CALLBACK DIEnumDeviceObjectsProc( 
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
      _snprintf( JoystickInfo[joysticknum].Axis[axis].name, MAX_JOYNAME, "Axis %d", axis );

    /* this current axis exists */
    JoystickInfo[joysticknum].Axis[axis].exists = TRUE;

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
    JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name = 
        (char *) malloc (MAX_JOYNAME+1);
  
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
      _snprintf(
         JoystickInfo[joysticknum].Button[JoystickInfo[joysticknum].NumButtons].name,
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
      _snprintf(
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
      _snprintf(
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


#ifdef USE_DINPUT_KEYNAMES

#define MAX_KEYS  (256)
DWORD KeyMax;
DIDEVICEOBJECTINSTANCE KeyInfo[ MAX_KEYS ];

BOOL CALLBACK DIEnumKeyboardObjectsProc( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef )
{
  DWORD key = lpddoi->dwOfs;
  if ( key < MAX_KEYS )
    KeyInfo[ key ] = *lpddoi;
  if ( KeyMax < key )
    KeyMax = key;
  return DIENUM_CONTINUE;
}


char *DI_KeyName( DWORD key )
{
  return ( key < MAX_KEYS && KeyInfo[ key ].dwSize ) ? KeyInfo[ key ].tszName : NULL;
}

#endif // USE_DINPUT_KEYNAMES

// show or hide the cursor
void ReallyShowCursor( BOOL show )
{

  // increment display count by one
  if ( show )
    while ( ShowCursor( TRUE ) < 0 )
      ; // try again

  // deincrement display count by one
  else
    while ( ShowCursor( FALSE ) >= 0 )
      ; // try again

}

extern HINSTANCE hInstApp;
extern BOOL MouseExclusive;
extern render_info_t render_info;
extern BOOL ActLikeWindow;
extern void SetCursorClip( BOOL clip );
extern void SetInputAcquired( BOOL acquire );

BOOL InitDInput(void)
{
  HRESULT  err;
  GUID     guid_mouse		= GUID_SysMouse;
  GUID     guid_keyboard	= GUID_SysKeyboard;
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
	BOOL failjoystick;

    err = DirectInputCreate(hInstApp, DIRECTINPUT_VERSION, &lpdi, NULL);
	if (FAILED(err))//DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, &IID_IDirectInput8, (void**)&lpdi, NULL)))
    {
		return FALSE;
    }

    // Create a mouse.
	err = IDirectInput_CreateDevice(lpdi, &guid_mouse, &lpdiMouse, NULL);
	if ( err != DI_OK )
	{
		goto fail;
	}

    // Tell DirectInput that we want to receive data in mouse format
    err = IDirectInputDevice_SetDataFormat(lpdiMouse, &c_dfDIMouse);

    if(err != DI_OK)
    {
		goto fail;
    }

	DebugPrintf("Setting mouse mode: %s\n",MouseExclusive?"Exclusive":"Non Exclusive");

	if ( MouseExclusive )
	{
		err = IDirectInputDevice_SetCooperativeLevel(
			lpdiMouse,			// mouse handle
			render_info.window,	// window handle
			DISCL_EXCLUSIVE |	// application requires exclusive access to device
								// this cuases the mouse to disapear
								// and be fully controlled by direct input
			DISCL_FOREGROUND);	// Application only wants mouse access when it's in the foreground
								// automatically unacquires on window de-activate

		// doesn't work as it should...

		// if acting like a window or not fullscreen
		if ( ActLikeWindow || !render_info.bFullscreen )
		{
			SetInputAcquired( FALSE );
			SetCursorClip( FALSE );
		}

	}
	else
	{
		err = IDirectInputDevice_SetCooperativeLevel(
			lpdiMouse,			// mouse handle
			render_info.window,	// window handle
			DISCL_NONEXCLUSIVE |// this mode does not lock the mouse down
								// the mouse still works but is free to roam to other windows...
			DISCL_BACKGROUND);	// allows mouse to be acquired even when it's not active window
	}

    if(err != DI_OK)
    {
		switch(err)
		{
		case DIERR_INVALIDPARAM:
			DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params.\n");
			break;
		case DIERR_NOTINITIALIZED:
			DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Not Initialized.\n");
			break;
		case E_HANDLE:
			DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params\n");
			break;
		case E_NOTIMPL:
			DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Method Not Supported\n");
			break;
		}
        goto fail;
    }

	// this is the moment the mouse disapears when usnig exclusive access.
	// we would most likely just want a global state flag defining the type of mouse state we want.
	// and then make sure the mouse is properly set in the right state.
	err = IDirectInputDevice_Acquire(lpdiMouse);

    // try to create keyboard device
    if(IDirectInput_CreateDevice(lpdi, &guid_keyboard, &lpdiKeyboard, NULL) !=DI_OK)
    {
            goto fail;
    }

    // Tell DirectInput that we want to receive data in keyboard format
    if (IDirectInputDevice_SetDataFormat(lpdiKeyboard, &c_dfDIKeyboard) != DI_OK)
    {
            goto fail;
    }

    // set cooperative level
    if(IDirectInputDevice_SetCooperativeLevel(lpdiKeyboard, render_info.window,
                     DISCL_EXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
    {
            goto fail;
    }

#ifdef USE_DINPUT_KEYNAMES
  // get key info
  KeyMax = 0;
  memset( KeyInfo, 0, sizeof( KeyInfo ) );
  if (IDirectInputDevice_EnumObjects(lpdiKeyboard, DIEnumKeyboardObjectsProc, NULL, DIDFT_ALL ) != DI_OK)
  {
    // don't care -- fall back on internal key name info
  }
#endif // USE_DINPUT_KEYNAMES

    // try to acquire the keyboard
    err = IDirectInputDevice_Acquire(lpdiKeyboard);

    if(err != DI_OK)
    {
//            goto fail;
    }

    // try to create another keyboard device (which will be buffered, for menu screens)
    if(IDirectInput_CreateDevice(lpdi, &guid_keyboard, &lpdiBufferedKeyboard, NULL) !=DI_OK)
    {
            goto fail;
    }

    // Tell DirectInput that we want to receive data in keyboard format
    if (IDirectInputDevice_SetDataFormat(lpdiBufferedKeyboard, &c_dfDIKeyboard) != DI_OK)
    {
            goto fail;
    }

    // set cooperative level
    if(IDirectInputDevice_SetCooperativeLevel(lpdiBufferedKeyboard, render_info.window,
                     DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
    {
            goto fail;
    }

    // set the buffer size...
  err = lpdiBufferedKeyboard->lpVtbl->SetProperty(lpdiBufferedKeyboard, DIPROP_BUFFERSIZE, &dipdw.diph);

    if(err != DI_OK)
    {
            goto fail;
    }

    // try to acquire the keyboard
    err = IDirectInputDevice_Acquire(lpdiBufferedKeyboard);

    if(err != DI_OK)
    {
 //           goto fail;
    }

  // try to create Joystick devices
  for ( i = 0; i < MAX_JOYSTICKS; i++ )
    lpdiJoystick[i] = NULL;
  Num_Joysticks = 0;

  lpdi->lpVtbl->EnumDevices(lpdi,
						DIDEVTYPE_JOYSTICK,
						//DI8DEVCLASS_GAMECTRL, 
                         InitJoystickInput, lpdi, DIEDFL_ATTACHEDONLY); 

  failjoystick = FALSE;
  for (i = 0; i < Num_Joysticks; i++)
  {
    JoystickInfo[i].assigned = FALSE;
    JoystickInfo[i].connected = TRUE;
    JoystickInfo[i].NumAxis = 0;
    JoystickInfo[i].NumButtons = 0;
    JoystickInfo[i].NumPOVs = 0;

    joysticknumptr = (LPVOID)&i;
    for (j = AXIS_Start; j <= AXIS_End; j++)
    {
      JoystickInfo[i].Axis[j].exists = FALSE;
    }
    lpdiJoystick[i]->lpVtbl->EnumObjects(lpdiJoystick[i], DIEnumDeviceObjectsProc, 
                     joysticknumptr, DIDFT_ALL); 

    for (j = AXIS_Start; j <= AXIS_End; j++)
    {
      JoystickInfo[i].Axis[j].action = SHIPACTION_Nothing;
      JoystickInfo[i].Axis[j].inverted = FALSE;
      JoystickInfo[i].Axis[j].deadzone = 20;
      JoystickInfo[i].Axis[j].fine = TRUE;
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
      if(IDirectInputDevice2_SetCooperativeLevel(lpdiJoystick[i], render_info.window,
                 DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) == DI_OK)
      {
        // try to acquire the Joystick
        err = IDirectInputDevice2_Acquire(lpdiJoystick[i]);
        if (err != DI_OK)
        {
          failjoystick = TRUE;
        }
      }else
      {
        failjoystick = TRUE;
      }
    }else
    {
      failjoystick = TRUE;
    }

    
    if (failjoystick)
    {
      failjoystick = FALSE;
      IDirectInputDevice2_Release(lpdiJoystick[i]);
      lpdiJoystick[i] = NULL;
    }
#if NOW_DONE_INSIDE_DEFAULTJOYSTICKSETTINGS
    SetUpJoystickAxis(i);

     // if spaceorb...
    if ( !_stricmp( JoystickInfo[i].Name, "Spacetec SpaceOrb 360" ) )
    {
      DebugPrintf("Spaceorb detected - auto configuring axis...\n");
      ConfigureSpaceorbAxis( i );
    }
#endif
  }

  DebugPrintf( "InitDInput: %d joysticks connected\n", Num_Joysticks );

  // if we get here, all DirectInput objects were created ok
  return TRUE;

fail:
    if (lpdiMouse)  IDirectInputDevice_Release(lpdiMouse), lpdiMouse  = NULL;
    if (lpdiKeyboard)  IDirectInputDevice_Release(lpdiKeyboard), lpdiKeyboard  = NULL;
    if (lpdiBufferedKeyboard)  IDirectInputDevice_Release(lpdiBufferedKeyboard), lpdiBufferedKeyboard  = NULL;
  for (i = 0; i < Num_Joysticks; i++)
  {
      if (lpdiJoystick[i])
    {
      IDirectInputDevice2_Release(lpdiJoystick[i]);
      lpdiJoystick[i] = NULL;
    }
  }
    if (lpdi)   IDirectInputDevice_Release(lpdi), lpdi     = NULL;
    return FALSE;    
}

BOOL TermDInput( void )
{
    int i;

  if (lpdiMouse)
  {
    IDirectInputDevice_Unacquire(lpdiMouse);
    IDirectInputDevice_Release(lpdiMouse);
    lpdiMouse  = NULL;
  }
    if (lpdiKeyboard)
  {
    IDirectInputDevice_Unacquire(lpdiKeyboard);
    IDirectInputDevice_Release(lpdiKeyboard);
    lpdiKeyboard  = NULL;
  }
    if (lpdiBufferedKeyboard)
  {
    IDirectInputDevice_Unacquire(lpdiBufferedKeyboard);
    IDirectInputDevice_Release(lpdiBufferedKeyboard);
    lpdiBufferedKeyboard  = NULL;
  }

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

  return TRUE;
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

void SetInputAcquired( BOOL acquire )
{
    HRESULT         err;
	if ( acquire )
	{
		/*
												// set desired access mode -- RESET BACK TO DISCL_EXCLUSIVE -- D0 N0T M355!!!111
												err = IDirectInputDevice_SetCooperativeLevel(
													lpdiMouse,			// mouse handle
													render_info.window,	// window handle

													DISCL_EXCLUSIVE |	// application requires exclusive access to device
																		// this cuases the mouse to disapear
																		// and be fully controlled by direct input

													DISCL_FOREGROUND);	// Application only wants mouse access when it's in the foreground
																		// automatically unacquires on window de-activate

												if(err != DI_OK)
												{
													switch(err)
													{
													case DIERR_INVALIDPARAM:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params.\n");
														break;
													case DIERR_NOTINITIALIZED:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Not Initialized.\n");
														break;
													case E_HANDLE:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params\n");
														break;
													case E_NOTIMPL:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Method Not Supported\n");
														break;
													}
												}
												*/

		if ( lpdiMouse )			err = IDirectInputDevice_Acquire(lpdiMouse);
		if ( lpdiKeyboard )			err = IDirectInputDevice_Acquire(lpdiKeyboard);
		if ( lpdiBufferedKeyboard ) err = IDirectInputDevice_Acquire(lpdiBufferedKeyboard);
		DebugPrintf( "Input has been Acquired.\n" );
	}
	else
	{
/*
												// set desired access mode -- RESET BACK TO DISCL_EXCLUSIVE -- D0 N0T M355!!!111
												err = IDirectInputDevice_SetCooperativeLevel(
													lpdiMouse,			// mouse handle
													render_info.window,	// window handle
													DISCL_NONEXCLUSIVE | // this mode does not lock the mouse down
																		// the mouse still works but is free to roam to other windows...
													DISCL_BACKGROUND);	// allows mouse to be acquired even when it's not active window

												if(err != DI_OK)
												{
													switch(err)
													{
													case DIERR_INVALIDPARAM:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params.\n");
														break;
													case DIERR_NOTINITIALIZED:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Not Initialized.\n");
														break;
													case E_HANDLE:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Invalid Params\n");
														break;
													case E_NOTIMPL:
														DebugPrintf("IDirectInputDevice_SetCooperativeLevel failed: Method Not Supported\n");
														break;
													}
												}

*/

		if ( lpdiMouse )			err = IDirectInputDevice_Unacquire(lpdiMouse);
		if ( lpdiKeyboard )			err = IDirectInputDevice_Unacquire(lpdiKeyboard);
		if ( lpdiBufferedKeyboard )	err = IDirectInputDevice_Unacquire(lpdiBufferedKeyboard);
		DebugPrintf( "Input has been UN-Acquired.\n" );
	}
}

BOOL cursor_clipped;
BOOL NoCursorClip = FALSE;
void SetCursorClip( BOOL clip )
{

// yea but exclusive mouse mode hides it on us without asking...
// maybe just set cusor_clipped to true when acquiring with exclusive...

	// we already are in this state...
	if ( cursor_clipped && clip ) return;
	if ( !cursor_clipped && !clip ) return;

	// the clipping area
	cursorclip.left = render_info.pClientOnPrimary.x + render_info.szClient.cx / 2;
	cursorclip.top = render_info.pClientOnPrimary.y + render_info.szClient.cy / 2;
	cursorclip.right = cursorclip.left + 1;
	cursorclip.bottom = cursorclip.top + 1;

	// save last state
	cursor_clipped = clip;
	//DebugPrintf( "SetCursorClip: cursor is %s\n", ( clip ? "clipped" : "not clipped" ));

	// this shit is just shoved into this function
	// cause it's pretty tied to the procedure...
	if ( clip )
	{
		SetInputAcquired( TRUE );	// must acquire before clipping
		ReallyShowCursor( FALSE );
	}
	else
	{
		//SetInputAcquired( FALSE );
		ReallyShowCursor( TRUE );
	}

	// don't do any clipping if cli option says not to
	// wine already clips the mouse and it causes input to go haywire
	if ( ! NoCursorClip )
	{
		if ( clip ) // clip
		{
			ClipCursor( &cursorclip );
			SetCursorPos( cursorclip.left, cursorclip.top );
		}
		else // unclip
		{
			ClipCursor( NULL );
		}
	}
	else
	{
		// logs
		if ( NoCursorClip )
			DebugPrintf("NoCursorClip.\n");
	}
}
