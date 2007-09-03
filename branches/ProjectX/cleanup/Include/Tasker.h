/*
 * t a s k e r . h
 *
 * Tasking System Header File
 *
 * The X Men, June 1996
 * Copyright (c) 1996 Probe Entertainment Limited
 * All Rights Reserved
 *
 * $Revision: 2 $
 *
 * $Header: /PcProjectX/Include/tasker.h 2     11/27/96 11:05a Phillipd $
 *
 * $Log: /PcProjectX/Include/tasker.h $
 * 
 * 2     11/27/96 11:05a Phillipd
 * Fixed Mouse speed on fast machines.....
 * 
 * 1     11/25/96 5:07p Phillipd
 * New working tasker system...
 * 
 * 
 */

#ifndef	TASKER_H
#define	TASKER_H

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include	<setjmp.h>
#include	"typedefs.h"

/*-----------------------------------------------------------------------
 * GLOBAL CONSTANTS
 *---------------------------------------------------------------------*/

#define	MAXSTACKSIZE	2048				// default stack size in bytes
#define	MAXDATASIZE		64					// default data size in bytes
#define	MAXTASKS		64					// max number of tasks

/*-----------------------------------------------------------------------
 * DEFINE TASK DATA STRUCTURE
 *---------------------------------------------------------------------*/

typedef void (*TaskFunc)(void);

typedef struct _TASK {

	struct _TASK *	Next;						// Pointer to next TASK
	struct _TASK * 	Prev;						// Pointer to previous TASK
	int16			SleepCount;					// Sleep timer
	int16			Priority;					// Execution priority
	uint8			Stack[ MAXSTACKSIZE ];		// Stack area
	uint8			Data[ MAXDATASIZE ];		// Data area
	_JUMP_BUFFER	Environment;				// Environment storage area

} TASK;

/*-----------------------------------------------------------------------
 * FUNCTION PROTOTYPES
 *---------------------------------------------------------------------*/

void	TaskerInit( void );
TASK *	TaskCreate( TaskFunc function, uint16 Priority );
void	TaskDispatch(void);
void	TaskDie( void );
void	TaskSleep( int16 count );
void	TaskJump( TaskFunc function );

#endif		/* TASKER_H		*/


