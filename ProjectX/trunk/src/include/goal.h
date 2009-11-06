#ifndef GOAL_H
#define GOAL_H

#define GOALSTATE_Off	(0)
#define GOALSTATE_On	(1)

#define GOAL_NONE		(0)
#define GOAL_SCORED		(1)
#define GOAL_WRONG		(-1)

typedef struct GOAL
{
	uint16	state;
	int		team;
	uint16	group;
	uint16	type;
	VECTOR	pos;
	VECTOR	half_size;
	VECTOR	dir;
	VECTOR	up;
	float	width;
	float	height;
	float	depth;
	uint16	num_sides;
	TRIGGER_ZONE	*Zone;
} GOAL;


BOOL GoalLoad( void );
void ReleaseGoal( void );
void ProcessGoals( void );
GOAL *TeamGoal( int team );
BOOL GenerateFlagAtHome( int team );
int GoalCheckTeam( VECTOR * OldPos , VECTOR * NewPos, uint16 Group, int team );
int DisplayGoal( uint16 group );


#endif // GOAL_H