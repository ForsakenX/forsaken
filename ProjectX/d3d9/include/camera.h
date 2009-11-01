

#ifndef CAMERA_INCLUDED
#define CAMERA_INCLUDED

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
	D3DMATRIX	projection;
	VISGROUP	*next_visible;
};


typedef struct
{
	int			groups;
	VISGROUP	group[ MAXGROUPS ];
	VISGROUP	*first_visible;
	VISGROUP	*last_visible;
	render_viewport_t	*viewport;
	D3DMATRIX	viewproj;
} VISLIST;


typedef struct CAMERA{
	BOOL	enable;
	BOOL	UseLowestLOD;
	int		Type;
	uint16	TypeNum;
	int16	GroupImIn;
	VECTOR	Pos;
	MATRIX	Mat;
	MATRIX	InvMat;
	D3DMATRIX	Proj;
	render_viewport_t Viewport;
	void *	CurrentOwner;
	VISLIST visible;
	D3DMATRIX	View;
}CAMERA;

typedef struct _REMOTECAMERA{
	BOOL	enable;
	int16	Group;
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
BOOL Cameraload( char * Filename );
void EnableRemoteCamera( uint16 * Data );
void DisableRemoteCamera( uint16 * Data );
FILE * SaveRemoteCameras( FILE * fp );
FILE * LoadRemoteCameras( FILE * fp );

#endif


