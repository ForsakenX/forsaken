
/*컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
	Define True/False and Data sizes
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/

#ifndef	DATATYPES
#define	DATATYPES

#define DIRECT3D_VERSION    0x0700
#define DIRECTSOUND_VERSION 0x0700
#define DIRECTDRAW_VERSION  0x0700
#define DIRECTINPUT_VERSION 0x0700

#ifndef	TRUE
#define	TRUE					(1==1)
#endif

#ifndef	FALSE
#define	FALSE					(!TRUE)
#endif

#define	GLOBAL_SCALE			(0.25F)

typedef	unsigned char		uint8;			/* Unsigned 8 Bit Integer  */
typedef	char				int8;			/* Signed 8 Bit Integer    */
typedef	unsigned short int	uint16;			/* Unsigned 16 Bit Integer */
typedef	short int			int16;			/* Signed 16 bit Integer   */
typedef	unsigned long		uint32;			/* Unsigned 32 Bit Integer */
typedef	long				int32;			/* Signed 32 Bit Integer   */

#endif

