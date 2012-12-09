#ifndef TEXT2D_INCLUDED
#define TEXT2D_INCLUDED
#include "main.h"

/*===================================================================
	Defines
===================================================================*/
#define	MAX_NAMES		8

#define	DO_LOAD			true
#define	DONT_LOAD		false

#define	BOX_NOFLIP		0x0000
#define	BOX_XFLIP		0x2000
#define	BOX_YFLIP		0x8000
#define	BOX_BOTHFLIP	0xA000

/*===================================================================
	Structures for 2D Texture info
===================================================================*/
typedef struct BOX_INFO {

	int16_t	tpage;
	float	u1;
	float	v1;
	float	u2;
	float	v2;
	int16_t	xsize;
	int16_t	ysize;

} BOX_INFO;

typedef struct OFF_INFO {

	int16_t	xoff;
	int16_t	yoff;
	u_int16_t	box;

} OFF_INFO;

typedef struct BIT_INFO {

	int16_t	startbit;
	int16_t	numbits;

} BIT_INFO;

typedef struct FRAME_INFO {

	int16_t			Num_Frames;
	int8_t		*	File_Addr;
	BOX_INFO	*	Box_Info;
	OFF_INFO	*	Off_Info;
	BIT_INFO	*	Bit_Info;
	char			tpage_name[32];		// only valid if only one t-page is associated with box file
	int16_t			vid_tpage_index;		//					---"---
	int16_t			sys_tpage_index;		//					---"---
	int16_t			NumBoxes;

} FRAME_INFO;

typedef struct OFF_FILES {

	int8_t		*	Filename;
	FRAME_INFO	**	InfoPtrAddr;
	bool			LoadEnable;
	bool			Scale;
	int				LoadTPages;
	int16_t			xsize;
	int16_t			ysize;
} OFF_FILES;

/*===================================================================
	Prototypes
===================================================================*/
FRAME_INFO * Load_Off_File( int8_t * Filename, bool Scale, int LoadTPages, int16_t *last_tpage, int16_t xsize, int16_t ysize, bool placeholder );
bool Load_All_Off_Files( OFF_FILES * FileInfo );
void Free_All_Off_Files( OFF_FILES * FileInfo );
int16_t GetTPage( FRAME_INFO * FrameInfo, int16_t Frame );
int16_t Get_Frm_Info_Index( FRAME_INFO ** Frame_Info );
FRAME_INFO ** Get_Frm_Info_Address( int16_t Index );
void EnableOffFile( OFF_FILES * FileInfo, int8_t * Filename );
void DisableOffFile( OFF_FILES * FileInfo, int8_t * Filename );
void EnableRelevantOffFiles( OFF_FILES * FileInfo );

#endif
