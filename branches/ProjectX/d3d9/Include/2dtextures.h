#ifndef TEXT2D_INCLUDED
#define TEXT2D_INCLUDED

/*===================================================================
	Defines
===================================================================*/
#define	MAX_NAMES		8

#define	DO_LOAD			TRUE
#define	DONT_LOAD		FALSE

#define	BOX_NOFLIP		0x0000
#define	BOX_XFLIP		0x2000
#define	BOX_YFLIP		0x8000
#define	BOX_BOTHFLIP	0xA000

/*===================================================================
	Structures for 2D Texture info
===================================================================*/
typedef struct BOX_INFO {

	int16	tpage;
	float	u1;
	float	v1;
	float	u2;
	float	v2;
	int16	xsize;
	int16	ysize;

} BOX_INFO;

typedef struct OFF_INFO {

	int16	xoff;
	int16	yoff;
	uint16	box;

} OFF_INFO;

typedef struct BIT_INFO {

	int16	startbit;
	int16	numbits;

} BIT_INFO;

typedef struct FRAME_INFO {

	int16			Num_Frames;
	int8		*	File_Addr;
	BOX_INFO	*	Box_Info;
	OFF_INFO	*	Off_Info;
	BIT_INFO	*	Bit_Info;
	char			tpage_name[32];		// only valid if only one t-page is associated with box file
	int16			vid_tpage_index;		//					---"---
	int16			sys_tpage_index;		//					---"---
	int16			NumBoxes;

} FRAME_INFO;

typedef struct OFF_FILES {

	int8		*	Filename;
	FRAME_INFO	**	InfoPtrAddr;
	BOOL			LoadEnable;
	BOOL			Scale;
	int				LoadTPages;
	int16			xsize;
	int16			ysize;
} OFF_FILES;

/*===================================================================
	Prototypes
===================================================================*/
FRAME_INFO * Load_Off_File( int8 * Filename, BOOL Scale, int LoadTPages, int16 *last_tpage, int16 xsize, int16 ysize, BOOL placeholder );
BOOL Load_All_Off_Files( OFF_FILES * FileInfo );
void Free_All_Off_Files( OFF_FILES * FileInfo );
int16 GetTPage( FRAME_INFO * FrameInfo, int16 Frame );
BOOL FixTextureUVs( OFF_FILES * FileInfo );
int16 Get_Frm_Info_Index( FRAME_INFO ** Frame_Info );
FRAME_INFO ** Get_Frm_Info_Address( int16 Index );
void EnableOffFile( OFF_FILES * FileInfo, int8 * Filename );
void DisableOffFile( OFF_FILES * FileInfo, int8 * Filename );
void EnableRelevantOffFiles( OFF_FILES * FileInfo );

#endif