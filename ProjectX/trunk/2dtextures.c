/*===================================================================
	Includes
===================================================================*/
#include <stdio.h>
#include "typedefs.h"
#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "networking.h"
#include "util.h"

#include	<stdlib.h>
#include	<malloc.h>
#include	<string.h>
#include	"main.h"
#include	"mload.h"
#include	"tload.h"
#include	"2dtextures.h"
#include	"triggers.h"
#include	"pickups.h"
#include	"restart.h"
#include	"xmem.h"

/*===================================================================
	External variables
===================================================================*/
	extern	TLOADHEADER	Tloadheader;
	extern	TRIGGERVAR	*	DecreaseTemperature;
	extern	BOOL	CaptureTheFlag;
	extern	BOOL	CTF;
	extern	BOOL	BountyHunt;
	extern	BYTE	ChangeLevel_MyGameStatus;
	extern	float	UV_Fix;

extern render_info_t render_info;
/*===================================================================
	Globals
===================================================================*/
#define	DO_SCALE		TRUE
#define	DONT_SCALE		FALSE

int8	*	Font512File = "font512.off";
int8	*	VduFont512	= "f512x384.off";
int8		FontFile[ 16 ];
int8		TitleFontFile[ 16 ];

float VduScaleX, VduScaleY;


	FRAME_INFO	*	Arc_Header = NULL;
	FRAME_INFO	*	Bits_Header = NULL;
	FRAME_INFO	*	Exp2_Header = NULL;
	FRAME_INFO	*	Flare_Header = NULL;
	FRAME_INFO	*	Laser_Header = NULL;
	FRAME_INFO	*	Pulsar_Header = NULL;
	FRAME_INFO	*	Pulsar_Exp_Header = NULL;
	FRAME_INFO	*	Pulsar_Trail_Header = NULL;
	FRAME_INFO	*	Shrapnel_Header = NULL;
	FRAME_INFO	*	Exp_Header = NULL;
	FRAME_INFO	*	Trojax1_Header = NULL;
	FRAME_INFO	*	Trojax2_Header = NULL;
	FRAME_INFO	*	Trojax_Exp_Header = NULL;
	FRAME_INFO	*	Circle_Header = NULL;
	FRAME_INFO	*	Blood_Header = NULL;
	FRAME_INFO	*	LensFlare_Header = NULL;
	FRAME_INFO	*	AfterBurner_Header = NULL;
	FRAME_INFO	*	GravgonTrail_Header = NULL;
	FRAME_INFO	*	SolarisTrail_Header = NULL;
	FRAME_INFO	*	NewTrail_Header = NULL;
	FRAME_INFO	*	PickupRegen_Header = NULL;
	FRAME_INFO	*	ScatterTrail_Header = NULL;
	FRAME_INFO	*	GreyFlare_Header = NULL;
	FRAME_INFO	*	BigFlash_Header = NULL;
	FRAME_INFO	*	BloodSplat_Header = NULL;
	FRAME_INFO	*	BloodDribble_Header = NULL;
	FRAME_INFO	*	Target_Header = NULL;
	FRAME_INFO	*	Numbers_Header = NULL;
	FRAME_INFO	*	Numbers2_Header = NULL;
	FRAME_INFO	*	Text512_Header = NULL;
	FRAME_INFO	*	Orb_Header = NULL;
	FRAME_INFO	*	Multiple_Header = NULL;
	FRAME_INFO	*	Solid_Header = NULL;
	FRAME_INFO	*	Thermo_Header = NULL;
	FRAME_INFO	*	Logos_Header = NULL;
	FRAME_INFO	*	NMEBullet_Header = NULL;
	FRAME_INFO	*	BigExp1_Header = NULL;
	FRAME_INFO	*	Mushroom_Header = NULL;
	FRAME_INFO	*	Bubble_Header = NULL;
	FRAME_INFO	*	EnemyGen_Header = NULL;
	FRAME_INFO	*	Drip_Header = NULL;
	FRAME_INFO	*	Restart_Header = NULL;
	FRAME_INFO	*	BlueSpark_Header = NULL;
	FRAME_INFO	*	Tentacle_Header = NULL;
	FRAME_INFO	*	Flag_Header = NULL;
	FRAME_INFO	*	Flags_Header = NULL;
	FRAME_INFO	*	CreditsFont_Header = NULL;

OFF_FILES OffsetFiles[] = {

	{ "Credits.off",	&CreditsFont_Header,	DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Credits Font
	{ "Num2.off",		&Numbers_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Countdown Numbers
	{ "Num3.off",		&Numbers2_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Countdown Numbers
	{ "arc.off",		&Arc_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Arc
	{ "bits.off",		&Bits_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Bits
	{ "expl.off",		&Exp2_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Gregs Explosion
	{ "flare.off",		&Flare_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Flare
	{ "laser.off",		&Laser_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Laser
	{ "pulsar.off",		&Pulsar_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Pulsar
	{ "puls_exp.off",	&Pulsar_Exp_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Pulsar Explosion
	{ "pultrail.off",	&Pulsar_Trail_Header,	DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Pulsar Trail
	{ "shrapnel.off",	&Shrapnel_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Shrapnel Explosion
	{ "smlexpl.off",	&Exp_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// My Explosion
	{ "Trojax1.off",	&Trojax1_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Trojax 1
	{ "Trojax2.off",	&Trojax2_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Trojax 2
	{ "Troj_exp.off",	&Trojax_Exp_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Trojax Explosion
	{ "Circle.off",		&Circle_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Pickup Circle
	{ "Blood.off",		&Blood_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Blood
	{ "Lensflar.off",	&LensFlare_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Lensflare
	{ "Afterbrn.off",	&AfterBurner_Header,	DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Afterburner
	{ "Grvtrail.off",	&GravgonTrail_Header,	DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Gravgon Trail
	{ "Soltrail.off",	&SolarisTrail_Header,	DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Solaris Trail
	{ "Newtrail.off",	&NewTrail_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// New Trail
	{ "Pikgen.off",		&PickupRegen_Header,	DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Pickup Regeneration
    { "Scttrail.off",	&ScatterTrail_Header,	DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Scatter Trail
	{ "Greyflar.off",	&GreyFlare_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Grey Flare Trail
	{ "BigFlash.off",	&BigFlash_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Grey Flare Trail
	{ "Splat.off",		&BloodSplat_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Blood Splat
	{ "SpltDrib.off",	&BloodDribble_Header,	DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Blood Splat & Dribble
	{ "Target.off",		&Target_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Target
	{ "Orb.off",		&Orb_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Orb
	{ "Solid.off",		&Solid_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Solid 8x8
	{ "Multiple.off",	&Multiple_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Multiple
	{ "Thermo.off",		&Thermo_Header,			DONT_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Thermo Meter
	{ "Nmebuits.off",	&NMEBullet_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// NME Bullets
	{ "Bigexp1.off",	&BigExp1_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Big Explosion 1
	{ "Mushroom.off",	&Mushroom_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Mushroom Explosion
	{ "Bubble.off",		&Bubble_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Bubble Header
	{ "Nmegen.off",		&EnemyGen_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Enemy Generation
	{ "Bubble.off",		&Drip_Header,			DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Drip
	{ "Restart.off",	&Restart_Header,		DONT_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Restart
	{ "BlueSprk.off",	&BlueSpark_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Blue Spark
	{ "Tentacle.off",	&Tentacle_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Tentacle
	{ "Flag.off",		&Flag_Header,			DONT_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Flag
	{ "Flags.off",		&Flags_Header,			DONT_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Flags
	{ &FontFile[ 0 ],	&Text512_Header,		DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// 512x284 Font/Text
	{ NULL,				NULL,					DO_LOAD,	DO_SCALE,	LOAD_TPAGES_FINISH, 0, 0 },

};

FRAME_INFO	*	Title_Chars1_Header;
FRAME_INFO	*	Title_Chars2_Header;
FRAME_INFO	*	Title_Fonts_Header;
FRAME_INFO	*	Title_LevelPics_Header;
FRAME_INFO	*	Title_TVFrame_Header;

OFF_FILES Title_OffsetFiles[] = {

	{ "chars1.off",			&Title_Chars1_Header, DO_LOAD, DONT_SCALE, LOAD_TPAGES, 0, 0 },		
	{ "chars2.off",			&Title_Chars2_Header, DO_LOAD, DO_SCALE, LOAD_TPAGES, 0, 0 },		
	{ "",					&Title_LevelPics_Header, DO_LOAD, DONT_SCALE, LOAD_TPAGES_PLACEHOLDER | LOAD_TPAGES, 256, 128 },		
	{ "",					&Title_TVFrame_Header, DO_LOAD, DONT_SCALE, LOAD_TPAGES_PLACEHOLDER | LOAD_TPAGES, 128, 128 },		
	{ &TitleFontFile[ 0 ],	&Title_Fonts_Header, DO_LOAD, DONT_SCALE, LOAD_TPAGES, 0, 0 },	// vdu font page
	{ &FontFile[ 0 ],		&Text512_Header, DO_LOAD, DONT_SCALE, LOAD_TPAGES, 0, 0 },	// 512x284 Font/Text
	{ NULL,					NULL, DO_LOAD, DO_SCALE, LOAD_TPAGES_FINISH, 0, 0 },			  
};

OFF_FILES Splash_OffsetFiles[] = {
	{ "Credits.off", &CreditsFont_Header,	DO_LOAD,	DONT_SCALE, LOAD_TPAGES, 0, 0 },	// Credits Font
	{ "Bubble.off", &Drip_Header, DO_LOAD, DONT_SCALE, LOAD_TPAGES, 0, 0 },				// Drip
	{ "bits.off", &Bits_Header, DO_LOAD, DONT_SCALE, LOAD_TPAGES, 0, 0 },				// Bits
	{ "flare.off", &Flare_Header, DO_LOAD, DONT_SCALE, LOAD_TPAGES, 0, 0 },				// Flare
	{ NULL, NULL, DO_SCALE, DO_SCALE, LOAD_TPAGES_FINISH, 0, 0 },			  
};

typedef struct
{
	BOX_INFO box_info;
	OFF_INFO off_info;
	BIT_INFO bit_info;
} OFF_BUFFER;

/*===================================================================
	Procedure	:	Load Texture Pages, Offsets and Sizes
				:	for 2D Faceme polygons
	Input		:	int8		*	Filename of offset file
				:	BOOL			Scale
				:	BOOL			LoadTPages
	Output		:	FRAME_INFO	*	Structure for offset file
===================================================================*/
FRAME_INFO * Load_Off_File( int8 * Filename, BOOL Scale, int LoadTPages, int16 *last_tpage, int16 xsize, int16 ysize, BOOL placeholder )
{
	int16			Name_Index[ MAX_NAMES ];
	int8		*	Off_Buffer;
	uint32			File_Size;
	uint32			Read_Size;
	int8		*	Char_Ptr;
	int16		*	Short_Ptr;
	BOX_INFO	*	Box_Info;
	int16			Num_Boxes;
	BIT_INFO	*	Bit_Info;
	int16			Num_Frames;
	OFF_INFO	*	Off_Info;
	int16			Num_Offs;
	FRAME_INFO	*	Frm_Info = NULL;
	int16			Num_TPages;
	int16			Count;
	char			tpagename[32];
	int8			TempFilename[ 256 ];
	OFF_BUFFER		*Placeholder_Off_Buffer;
	
	Frm_Info = malloc( sizeof( FRAME_INFO ) );							// Allocate memory for return structure
	if( Frm_Info == NULL ) return( NULL );

	if( placeholder )
	{
		Placeholder_Off_Buffer = (OFF_BUFFER *)malloc( sizeof( OFF_BUFFER ) );									// Allocate memory 
		if( Placeholder_Off_Buffer == NULL )
		{
			free( Frm_Info );
			return( NULL );
		}

		Frm_Info->vid_tpage_index = AddTexture( &Tloadheader, "", TRUE , Scale , FALSE, xsize, ysize );	// add dummy texture
		if( Frm_Info->vid_tpage_index == -1 )
		{
			free( Frm_Info );
			free( Placeholder_Off_Buffer );
			return ( NULL );
		}

		Frm_Info->tpage_name[ 0 ] = 0;
		Frm_Info->sys_tpage_index = -1;

		Box_Info = &Placeholder_Off_Buffer->box_info;
		Off_Info = &Placeholder_Off_Buffer->off_info;
		Bit_Info = &Placeholder_Off_Buffer->bit_info;

		Box_Info->tpage = Frm_Info->vid_tpage_index; 
		Box_Info->u1 = 0.0F;
		Box_Info->v1 = 0.0F;
		Box_Info->u2 = 1.0F;
		Box_Info->v2 = 1.0F;
		Box_Info->xsize = xsize;
		Box_Info->ysize = ysize;

		Off_Info->xoff = 0;
		Off_Info->yoff = 0;
		Off_Info->box = 0;

		Bit_Info->startbit = 0;
		Bit_Info->numbits = 1;

		Frm_Info->Num_Frames = 1;
		Frm_Info->File_Addr = ( int8 *)Placeholder_Off_Buffer;
		Frm_Info->Box_Info = Box_Info;
		Frm_Info->Off_Info = Off_Info;
		Frm_Info->Bit_Info = Bit_Info;
		Frm_Info->NumBoxes = 1;

	}
	else
	{

		File_Size = Get_File_Size( Filename );								// Get size of file to load
		if( File_Size == 0 )
		{
			free( Frm_Info );
			return( NULL );
		}
																			
		Off_Buffer = malloc( File_Size );									// Allocate memory to load file into
		if( Off_Buffer == NULL )
		{
			free( Frm_Info );
			return( NULL );
		}
																			
		Read_Size = Read_File( Filename, Off_Buffer, File_Size );			// Read file
		if( Read_Size != File_Size )
		{
			free( Frm_Info );
			free( Off_Buffer );
			return( NULL );
		}
																			
		Short_Ptr = (int16 *) Off_Buffer;									
																			
		Num_TPages = *Short_Ptr++;											// Number of possible new texture pages

		Char_Ptr = (int8 *) Short_Ptr;

		for( Count = 0; Count < Num_TPages; Count++ )
		{
			sprintf( &TempFilename[ 0 ], "data\\textures\\%s", Char_Ptr );

			if( LoadTPages & LOAD_TPAGES)
			{
				Name_Index[ Count ] = AddTexture( &Tloadheader, &TempFilename[ 0 ], TRUE , Scale , FALSE, 0, 0 );
				strcpy ( tpagename, Char_Ptr );
				*last_tpage = Name_Index[ Count ];
				if( Name_Index[ Count ] == -1 )
				{
		   			free( Frm_Info );
					free( Off_Buffer );
					return( NULL );
				}
			}

			Char_Ptr += ( strlen( Char_Ptr ) + 1 );
		}

		// copy t-page info into Frm_Info - this is only valid if only one t-page is used!
		strcpy ( Frm_Info->tpage_name, tpagename );
		Frm_Info->vid_tpage_index = Name_Index[ 0 ];
		Frm_Info->sys_tpage_index = -1;

		Short_Ptr = (int16 *) Char_Ptr;
		Num_Boxes = *Short_Ptr++;											// Number of Boxes
		Box_Info = (BOX_INFO *) Short_Ptr;

		Short_Ptr = (int16 *) ( Box_Info + Num_Boxes );
		Num_Frames = *Short_Ptr++;											// Number of Frames
		Bit_Info = (BIT_INFO *) Short_Ptr;

		Short_Ptr = (int16 *) ( Bit_Info + Num_Frames );
		Num_Offs = *Short_Ptr++;											// Number of Offsets
		Off_Info = (OFF_INFO *) Short_Ptr;

		for( Count = 0; Count < Num_Boxes; Count++ )
		{
			if (LoadTPages & LOAD_TPAGES)
				Box_Info[ Count ].tpage = Name_Index[ Box_Info[ Count ].tpage ];	// Update TPages
		}

		Frm_Info->Num_Frames = Num_Frames;
		Frm_Info->File_Addr = Off_Buffer;
		Frm_Info->Box_Info = Box_Info;
		Frm_Info->Off_Info = Off_Info;
		Frm_Info->Bit_Info = Bit_Info;
		Frm_Info->NumBoxes = Num_Boxes;
	}

	return( Frm_Info );
}

/*===================================================================
	Procedure	:	Load All Offset Files
	Input		:	Nothing
	Output		:	BOOL	True/False
===================================================================*/
BOOL Load_All_Off_Files( OFF_FILES * FileInfo )
{
	int8			TempFilename[ 256 ];
	int8		*	OffsetPath = "data\\offsets\\";
	int16			last_tpage;

	if( !FileInfo ) return( FALSE );

	EnableRelevantOffFiles( FileInfo );

	// polytext is default now
	strcpy( &FontFile[ 0 ], Font512File );

	// this still uses the old blitted font file
	strcpy ( &TitleFontFile[ 0 ], VduFont512 );

	//
	VduScaleX = (float)floor((double)((float)render_info.szClient.cx / 512.0F));
	VduScaleY = (float)floor((double)((float)render_info.szClient.cy / 384.0F));

	last_tpage = 0;

	while (!( FileInfo->LoadTPages & LOAD_TPAGES_FINISH ) )
	{
		if( FileInfo->LoadEnable )
		{
			if ( strcmp(&FileInfo->Filename[ 0 ], &TitleFontFile[ 0 ] ) == 0 )
			{
				FileInfo->LoadTPages = LOAD_TPAGES;
			}

			if ( ( !FileInfo->Filename[0] ) && (!( FileInfo->LoadTPages & LOAD_TPAGES_PLACEHOLDER ) ) )
			{
				FileInfo++;
				continue;
			}

			Add_Path( OffsetPath, FileInfo->Filename, &TempFilename[ 0 ] );

			*FileInfo->InfoPtrAddr = Load_Off_File( &TempFilename[ 0 ], FileInfo->Scale, FileInfo->LoadTPages, &last_tpage, FileInfo->xsize, FileInfo->ysize, ( FileInfo->LoadTPages & LOAD_TPAGES_PLACEHOLDER ) );
			if( *FileInfo->InfoPtrAddr == NULL )
			{
				Msg( "offset file missing or too many tpages\n%s\n", &TempFilename[ 0 ] );
				return FALSE;
			}
		}
		FileInfo++;
	}

	return TRUE;
}

/*===================================================================
	Procedure	:	Free All Offset Files
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void Free_All_Off_Files( OFF_FILES * FileInfo )
{
	if( !FileInfo ) return;

	while( FileInfo->InfoPtrAddr != NULL )
	{
		if( *FileInfo->InfoPtrAddr != NULL )
		{
			free( (*FileInfo->InfoPtrAddr)->File_Addr );
			free( *FileInfo->InfoPtrAddr );
			*FileInfo->InfoPtrAddr = NULL;
		}
		FileInfo++;
	}
}

/*===================================================================
	Procedure	:	Get TPage
	Input		:	FRAME_INFO	*	FrameInfo
				:	int16			Frame
	Output		:	int16			TPage (-1 is non specified)
===================================================================*/
int16 GetTPage( FRAME_INFO * FrameInfo, int16 Frame )
{
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;

	if( FrameInfo )
	{
	   	Bit_Ptr = ( FrameInfo->Bit_Info + Frame );
	   	Off_Ptr = ( FrameInfo->Off_Info + Bit_Ptr->startbit );
	   	Box_Ptr = ( FrameInfo->Box_Info + ( Off_Ptr->box & 0x0fff ) );
		return ( Box_Ptr->tpage );
	}
	return( (uint16) -1 );
}

/*===================================================================
	Procedure	:	Calc FrameInfo Ptr Index
	Input		:	FRM_INFO	**	FrameInfoPtr Address
	Output		:	int16			Index
===================================================================*/
int16 Get_Frm_Info_Index( FRAME_INFO ** Frame_Info )
{
	OFF_FILES * FileInfo;
	int16		Count;

	FileInfo = &OffsetFiles[ 0 ];
	Count = 0;

	while( FileInfo->InfoPtrAddr != NULL )
	{
		if( FileInfo->InfoPtrAddr == Frame_Info )
		{
			return( Count );
		}

		Count++;
		FileInfo++;
	}

	return( -1 );
}

/*===================================================================
	Procedure	:	Calc FrameInfo Ptr Index
	Input		:	int16			Index
	Output		:	FRM_INFO	**	FraneInfoPtr Address
===================================================================*/
FRAME_INFO ** Get_Frm_Info_Address( int16 Index )
{
	if( Index == -1 ) return( NULL );

	return( OffsetFiles[ Index ].InfoPtrAddr );
}

/*===================================================================
	Procedure	:	Enable all relevant off files for
				:	level / game type
	Input		:	Nothing
	Output		:	Nothing
===================================================================*/
void EnableRelevantOffFiles( OFF_FILES * FileInfo )
{
	if( !FileInfo ) return;
	if( FileInfo != &OffsetFiles[ 0 ] ) return;

	if( ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
		( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
		( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) )
	{
		if( !DecreaseTemperature ) DisableOffFile( &OffsetFiles[ 0 ], "Thermo.off" );
		else EnableOffFile( &OffsetFiles[ 0 ], "Thermo.off" );

		DisableOffFile( &OffsetFiles[ 0 ], "Restart.off" );
		DisableOffFile( &OffsetFiles[ 0 ], "Flag.off" );
		DisableOffFile( &OffsetFiles[ 0 ], "Flags.off" );

	}
	else
	{
		if( !DecreaseTemperature ) DisableOffFile( &OffsetFiles[ 0 ], "Thermo.off" );
		else EnableOffFile( &OffsetFiles[ 0 ], "Thermo.off" );

		if( !ENABLE_RESTART ) DisableOffFile( &OffsetFiles[ 0 ], "Restart.off" );
		else EnableOffFile( &OffsetFiles[ 0 ], "Restart.off" );

		if( !CaptureTheFlag && !BountyHunt ) DisableOffFile( &OffsetFiles[ 0 ], "Flag.off" );
		else EnableOffFile( &OffsetFiles[ 0 ], "Flag.off" );

		if( !CTF ) DisableOffFile( &OffsetFiles[ 0 ], "Flags.off" );
		else EnableOffFile( &OffsetFiles[ 0 ], "Flags.off" );
	}
}

/*===================================================================
	Procedure	:	Enable Specific off files for loading
	Input		:	OFF_FILES	*	Ptr to first Offset filename
				:	int8		*	File to enable
	Output		:	Nothing
===================================================================*/
void EnableOffFile( OFF_FILES * FileInfo, int8 * Filename )
{
	while( FileInfo->InfoPtrAddr != NULL )
	{
		if( !_stricmp( &FileInfo->Filename[ 0 ], Filename ) )
		{
			FileInfo->LoadEnable = TRUE;
			return;
		}

		FileInfo++;
	}
}

/*===================================================================
	Procedure	:	Disable Specific off files for loading
	Input		:	OFF_FILES	*	Ptr to first Offset filename
				:	int8		*	File to disable
	Output		:	Nothing
===================================================================*/
void DisableOffFile( OFF_FILES * FileInfo, int8 * Filename )
{		
	while( FileInfo->InfoPtrAddr != NULL )
	{
		if( !_stricmp( &FileInfo->Filename[ 0 ], Filename ) )
		{
			FileInfo->LoadEnable = FALSE;
			return;
		}

		FileInfo++;
	}
}
