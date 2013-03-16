

#ifndef CAMERA_INCLUDED
#define CAMERA_INCLUDED

#include "main.h"
/*
 * Defines
 */

#define	CAMRENDERING_None		0
#define CAMRENDERING_Main		2
#define CAMRENDERING_Rear		3
#define CAMRENDERING_Missile	4
#define CAMRENDERING_Pip		5

/*
 * structures
 */
typedef struct
{
	VECTOR min;
	VECTOR max;
} EXTENT;


typedef struct _VISGROUP VISGROUP;


struct _VISGROUP
{
	int			group;
	int			visible;
	EXTENT		extent;
	render_viewport_t viewport;
	RENDERMATRIX	projection;
	VISGROUP	*next_visible;
};


typedef struct
{
	int			groups;
	VISGROUP	group[ MAXGROUPS ];
	VISGROUP	*first_visible;
	VISGROUP	*last_visible;
	render_viewport_t	*viewport;
	RENDERMATRIX	viewproj;
} VISLIST;


typedef struct CAMERA{
	bool	enable;
	bool	UseLowestLOD;
	int		Type;
	u_int16_t	TypeNum;
	int16_t	GroupImIn;
	VECTOR	Pos;
	MATRIX	Mat;
	MATRIX	InvMat;
	RENDERMATRIX	Proj;
	render_viewport_t Viewport;
	void *	CurrentOwner;
	VISLIST visible;
	RENDERMATRIX	View;
}CAMERA;

typedef struct _REMOTECAMERA{
	bool	enable;
	int16_t	Group;
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	Up;
	MATRIX	Mat;
	MATRIX	InvMat;
}REMOTECAMERA;






/*
 * fn prototypes
 */
void CameraRelease( void);
bool Cameraload( char * Filename );
void EnableRemoteCamera( u_int16_t * Data );
void DisableRemoteCamera( u_int16_t * Data );
FILE * SaveRemoteCameras( FILE * fp );
FILE * LoadRemoteCameras( FILE * fp );

#endif


