
#include "tasker.h"
#include "XMem.h"

/*-----------------------------------------------------------------------
 * Variables
 *---------------------------------------------------------------------*/

static _JUMP_BUFFER	Environment;	// Storage for scheduler environment


static TASK *		TaskMEM = (TASK*) 0;			// Pointer to TASK memory
static TASK *		TaskFree;			// Poiner to first FREE TASK
static TASK *		TaskUsed;			// Pointer to first USED TASK
static TASK *		TaskCrnt;			// Pointer to current TASK


/*-----------------------------------------------------------------------
 * Add TASK to start of FREE list
 *
 * Input:
 *		TASK * task - Pointer to TASK to add
 *
 * Output:
 *		None
 *
 *---------------------------------------------------------------------*/

static void AddTASKToFree( TASK * t )
{
	t->Next  = TaskFree;
	TaskFree = t;
}

/*-----------------------------------------------------------------------
 * Initialise TASKING system
 *
 * Input:
 *		None
 *
 * Output:
 *		None
 *
 *---------------------------------------------------------------------*/

void TaskerInit( void )
{
	TASK * t; 
	int16	count;


	if( !TaskMEM )
	{
		TaskMEM  = (TASK *) malloc( MAXTASKS * sizeof(TASK) );
	}

	TaskFree = (TASK *) 0;
	TaskUsed = (TASK *) 0;
	TaskCrnt = (TASK *) 0;

	for ( t = TaskMEM, count = 0; count < MAXTASKS; count ++, t ++ )
		AddTASKToFree( t );
}

/*-----------------------------------------------------------------------
 * Get FREE TASK from top of free list
 *
 * Input:
 *		None
 *
 * Output:
 *		TASK * task - Pointer to free TASK
 *
 *---------------------------------------------------------------------*/

static TASK * GetTASKFromFree( void )
{
	TASK * first;


	if ( ( first = TaskFree ) )
		TaskFree = first->Next;

	return first;
}

/*-----------------------------------------------------------------------
 * Add TASK to in use list in order of priority
 *
 * Input:
 *		TASK * task - Pointer to TASK to add
 *
 * Output:
 *		None
 *
 *---------------------------------------------------------------------*/

static void AddTASKToInUse( TASK * t )
{
	TASK * this;
	TASK * next;


	t->Next = (TASK *) 0;
	t->Prev = (TASK *) 0;

	if ( ! (this = TaskUsed) )
	{
		TaskUsed = t;
		return;
	}

	if ( t->Priority < this->Priority )
	{
		TaskUsed   = t;
		t->Next	   = this;
		this->Prev = t;
		return;
	}

	while ( next = this->Next )
	{
		if ( t->Priority < next->Priority )
		{
			this->Next = t;
			next->Prev = t;
			t->Next	   = next;
			t->Prev	   = this;
			return;
		}
		this = next;
	}

	this->Next = t;
	t->Prev	   = this;
}

/*-----------------------------------------------------------------------
 * Remove TASK from in use list
 *
 * Input:
 *		TASK * task - Pointer to TASK to remove
 *
 * Output:
 *		None
 *
 *---------------------------------------------------------------------*/

static void RemoveTASKFromInUse( TASK * t )
{
	TASK * next;
	TASK * prev;


	next = t->Next;
	prev = t->Prev;

	if ( next )
		next->Prev = prev;

	if ( prev )
		prev->Next = next;
	
	else
		TaskUsed = next;

	t->Next = (TASK *) 0;
	t->Prev = (TASK *) 0;
}

/*-----------------------------------------------------------------------
 * Create and initialise a TASK
 *
 * Input:
 *		void 	(* func)()	-	Pointer to TASK function
 *		uint16	Priority	-	Execution priority of this TASK
 *
 * Output:
 *		TASK * task - Pointer to created TASK
 *
 *---------------------------------------------------------------------*/

TASK * TaskCreate( void (*Function)( void ), uint16 Priority )
{
	TASK * 	t;
	uint32 * 	stack;

	if ( ( t = GetTASKFromFree() ) )
	{
		t->SleepCount = 1;
		t->Priority	  = Priority;

		stack = (uint32 *) & t->Stack[ MAXSTACKSIZE - 1 ];

		setjmp( (int*) &t->Environment );				// save current environment
	
		t->Environment.Eip = (int) Function;	   	// new PC
		t->Environment.Esp = (int) stack;			// new Stack Pointer

		AddTASKToInUse( t );	
	}
	return t;
}

/*-----------------------------------------------------------------------
 * Service all TASKS with sleepcount == 0
 *
 * Input:
 *		None
 *
 * Output:
 *		None
 *
 *---------------------------------------------------------------------*/

void TaskDispatch( void )
{
	TaskCrnt = TaskUsed;

	while ( TaskCrnt )
	{
		if ( ! -- TaskCrnt->SleepCount )
		{
			if ( ! setjmp( (int*) &Environment ) )			 	// save environment and
				longjmp( (int*) &TaskCrnt->Environment, 1 );	// execute current task
		}

		if ( TaskCrnt )
			TaskCrnt = TaskCrnt->Next;
		else
			TaskCrnt = TaskUsed;
	}
}

/*-----------------------------------------------------------------------
 * Kill current TASK and return to scheduler
 *
 * Input:
 *		None
 *
 * Output:
 *		None
 *
 *---------------------------------------------------------------------*/

void TaskDie( void )
{
	TASK * prev = TaskCrnt->Prev;


	RemoveTASKFromInUse( TaskCrnt );
		
	AddTASKToFree( TaskCrnt );

	TaskCrnt = prev;

	longjmp( (int*) &Environment, 1 );					// return to scheduler
}

/*-----------------------------------------------------------------------
 * Send current TASK to sleep and return to scheduler
 *
 * Input:
 *		int16 Sleepcount	-	Number of 'game ticks' to sleep for
 *
 * Output:
 *		None
 *
 *---------------------------------------------------------------------*/

void TaskSleep( int16 SleepCount )
{
	TaskCrnt->SleepCount = SleepCount;

	if ( ! setjmp( (int*) &TaskCrnt->Environment ) )	// save current environment and
		longjmp( (int*) &Environment, 1 );				// return to scheduler
}

/*-----------------------------------------------------------------------
 * Jump from current TASK to new routine
 *
 * Input:
 *		void 	(* func)()	-	Pointer to new TASK function
 *
 * Output:
 *		None
 *
 *---------------------------------------------------------------------*/

void TaskJump( void (*function)(void) )
{
	setjmp( (int*) &TaskCrnt->Environment );				 						// save current environment

	TaskCrnt->Environment.Eip = (int) function; 						// new PC
	TaskCrnt->Environment.Esp = (int) TaskCrnt->Stack + MAXSTACKSIZE;	// reset stack

	longjmp( (int*) &TaskCrnt->Environment, 1 );													// jump
}

