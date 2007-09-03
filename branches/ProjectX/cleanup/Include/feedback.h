#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "force.h"

extern int FB_Joystick;

extern int FB_Initialise( void );

extern int FB_Start( void );

extern int FB_Stop( void );

extern int FB_FirePrimary( int firing, float power );

extern int FB_FireSecondary( int firing );

extern int FB_JoltForce( VECTOR *jolt );

extern int FB_Jolt( void );

extern int FB_Engine( void );

extern int FB_Damper( void );

extern int FB_Inertia( void );


#endif // FEEDBACK_H
