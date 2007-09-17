
/************************************************************************
*  vfw_spec.h                                                           *
*                                                                       *
* This code and information is provided "as is" without warranty of     *
* any kind, either expressed or implied, including but not limited to   *
* the implied warranties of merchantability and/or fitness for a        *
* particular purpose.                                                   *
*                                                                       *
*               Copyright (c) 1994-1995 Intel Corporation.              *
*                         All Rights Reserved.                          *
*                                                                       *
************************************************************************/

/*
 *
 *  DESCRIPTION:
 *  The Indeo(R) Video Interactive codec provides access to new features
 *  using the ICM_SETCODECSTATE and ICM_GETCODECSTATE messages. This header
 *  file defines the data structures used in these messages.
 */


#ifndef __VFW_SPEC_H__
#define __VFW_SPEC_H__


/* The specific interface version is defined as follows:
 */
#define INTERFACE_MAJOR_NUMBER(v)                       ((v) >> 16)
#define INTERFACE_MINOR_NUMBER(v)                       ((v) & 0xffff)
#define CREATE_INTERFACE_VERSION(maj,min)       ((((DWORD)maj)<<16) | (((DWORD)min) & 0xffff))
#define SPECIFIC_INTERFACE_VERSION                      CREATE_INTERFACE_VERSION(1,1)

/* The codec specific information is handled by the general state
 * handler.
 */
#define ICM_GETCODECSTATE                                       ICM_GETSTATE
#define ICM_SETCODECSTATE                                       ICM_SETSTATE

/* OS Environments - Within the Microsoft Windows domain (Win 3.1, Win 95,
 * and Win NT), both applications and codecs can be compiled in either 16 or
 * 32-bit formats.  Any combination is allowed with the knowledge that data
 * pointers will not be thunked.
 */
typedef enum {
	OE_UNDEFINED,
	OE_16,
	OE_32,
} R4_OS_ENVIRONMENT;


/* The type field identifies the kind of operation that should be done as part
 * of the ICM_GETCODECSTATE and ICM_SETCODECSTATE messages.
 *
 * The MT_UNDEFINED value is not a supported type - it should only be used
 * to initialize variables to an "undefined" state.
 */
typedef enum {
	MT_UNDEFINED,
	MT_DECODE_FRAME_DEFAULT,
	MT_DECODE_FRAME_VALUE,
	MT_DECODE_SEQ_DEFAULT,
	MT_DECODE_SEQ_VALUE,
	MT_ENCODE_SEQ_DEFAULT,
	MT_ENCODE_SEQ_VALUE
} R4_MESSAGE_TYPE;

/*
 * MT_QUEUE is for 16-bit applications that call 32-bit codecs, only.  It
 * is necessary to pre-set ICM_GETCODECSTATE messages by first issuing an
 * ICM_SETCODECSTATE with the MT_QUEUE flag or'ed with the desired
 * message type to get.  The ICM_GETCODECSTATE message which follows
 * must set its dwFourCC field to 0.
 */
#define MT_QUEUE  (1UL<<31)


/* A flag is false if it is zero, otherwise it is true.
 */
typedef DWORD R4_FLAG;

/* Pointer to a Flag - A pointer to a flag is used by the codec to write 
 * status information at a later time.
 */
typedef R4_FLAG FAR * PTR_R4_FLAG;
 

/* Rectangle - The following structure is used to describe a rectangle.
 */
typedef struct {
	DWORD dwX;
	DWORD dwY;
	DWORD dwWidth;
	DWORD dwHeight;
} R4_RECT, FAR * PTR_R4_RECT;


/* Transparency Bitmask - A transparency bitmask contains one bit for each 
 * pixel in the source image. If a bit is zero the data is transparent.  
 * The first bit in each byte is identified by the mask 0x80.  The last 
 * bit in each byte is identified by the mask 0x01. 
 * 
 * To allow faster access each row must begin on a DWORD boundary and the 
 * total number of bytes must be rounded-up to a multipe of four.
 */
typedef BYTE FAR * PTR_R4_BITMASK;


/* Playback Platform - The codec supports several playback options.  This is
 * not hardware specific.  Instead it rates the relative capabilities of the  
 * hardware.
 */
typedef enum {
	PP_LOW,
	PP_MEDIUM,
	PP_HIGH,
} R4_PLAYBACK_PLATFORM;


/* Transparency Method - The codec supports a number of tranparency encoding
 * methods. The enumeration specifies the transparency method.
 */
typedef enum {
	TM_UNDEFINED,
	TM_NONE,
	TM_RANGE,
	TM_BITMASK,
	TM_FRAME,
	TM_ALPHA_CHANNEL                /* XRGB color formats only */
} R4_TRANS_METHOD;


/* Transparency Range - A transparency range is specified using minimum 
 * and maximum values for R, G, and B. Any pixel with values within this range 
 * (inclusive) is considered transparent.
 */
typedef struct {
	WORD  u16Reserved;
	UCHAR u8BlueLow;
	UCHAR u8BlueHigh;
	UCHAR u8GreenLow;
	UCHAR u8GreenHigh;
	UCHAR u8RedLow;
	UCHAR u8RedHigh;
} R4_RANGE, FAR * PTR_R4_RANGE;


/* The structure header is a set fields that are common,
 * to each of the data structures that follows.
 */
typedef struct {
	DWORD   dwSize;
	DWORD   dwFourCC;
	DWORD   dwVersion;
	LONG    mtType; 
	LONG    oeEnvironment;
	DWORD   dwFlags;
} R4_HEADER, FAR * PTR_R4_HEADER;
/* The structure header size is enough room for dwSize, dwFourCC,
 * dwVersion, dwType, dwEnvironment, and dwFlags
 */
#define R4_STRUCTURE_HEADER_SIZE (sizeof(R4_HEADER))


/* Some decode parameters can be changed prior to either an ICM_DECOMPRESS or 
 * ICM_DECOMPRESSEX mesage. These are found in the  R4_DEC_FRAME_DATA 
 * structure.  See CUSTOM_INTERFACE_ENCODER_DATA for structure size constraints.
 */
#define DECFRAME_TIME_LIMIT             (1UL<<0)
#define DECFRAME_DECODE_RECT    (1UL<<1)
#define DECFRAME_VIEW_RECT              (1UL<<2)
#define DECFRAME_BOUNDING_RECT  (1UL<<3)
#define DECFRAME_TRANS_BITMASK  (1UL<<4)
#define DECFRAME_BRIGHTNESS             (1UL<<5)
#define DECFRAME_SATURATION             (1UL<<6)
#define DECFRAME_CONTRAST               (1UL<<7)
#define DECFRAME_VALID                  (1UL<<31)

typedef struct {
	DWORD dwSize;
	DWORD dwFourCC;
	DWORD dwVersion;
	LONG mtType;    
	LONG oeEnvironment;
	DWORD dwFlags;

	DWORD dwTimeLimit;                                              /* version 1.0 */
	R4_RECT rDecodeRect;                                    /* version 1.0 */
	R4_RECT rViewRect;                                              /* version 1.0 */
	PTR_R4_RECT prBoundingRect;                             /* version 1.0 */
	PTR_R4_BITMASK pbmTransparencyBitmask;  /* version 1.0 */
	PTR_R4_FLAG pfUpdatedTransparencyBitmask; /* version 1.0 */
	LONG lBrightness;                                               /* version 1.0 */
	LONG lSaturation;                                               /* version 1.0 */
	LONG lContrast;                                                 /* version 1.0 */
} R4_DEC_FRAME_DATA, FAR * PTR_R4_DEC_FRAME_DATA;


/* Other decode parameters can only be changed prior to either an 
 * ICM_DECOMPRESS_BEGIN or an ICM_DECOMPRESSEX_BEGIN message.  These 
 * are found in the R4_DEC_SEQ_DATA structure.
 * See CUSTOM_INTERFACE_ENCODER_DATA for structure size constraints.
 */ 
#define DECSEQ_KEY                              (1UL<<0)
#define DECSEQ_SCALABILITY              (1UL<<1)
#define DECSEQ_FILL_TRANSPARENT (1UL<<2)
#define DECSEQ_ALT_LINE                 (1UL<<3)
#define DECSEQ_VALID                    (1UL<<31)

typedef struct {
	DWORD dwSize;
	DWORD dwFourCC;
	DWORD dwVersion;
	LONG mtType;
	LONG oeEnvironment;
	DWORD dwFlags;

	DWORD dwKey;                                                    /* version 1.0 */
	R4_FLAG fEnabledKey;                                    /* version 1.0 */
	R4_FLAG fScalability;                                   /* version 1.0 */
	R4_FLAG fFillTransparentPixels;                 /* version 1.0 */
	R4_FLAG fAltLine;                                               /* version 1.1 */
} R4_DEC_SEQ_DATA, FAR * PTR_R4_DEC_SEQ_DATA;


/* Encode parameters can only be changed prior to an ICM_COMPRESS_BEGIN 
 * messsage.  These are found in the R4_ENC_SEQ_DATA structure (these 
 * include the parameters that can be set in the super dialog box).
 * See CUSTOM_INTERFACE_ENCODER_DATA for structure size constraints.
 */  
#define ENCSEQ_KEY                                      (1UL<<0)
#define ENCSEQ_MINIMUM_VIEWPORT         (1UL<<1)
#define ENCSEQ_TRANSPARENCY                     (1UL<<2)
#define ENCSEQ_SCALABILITY                      (1UL<<3)
#define ENCSEQ_BI_DIR_PREDICTION        (1UL<<4)
#define ENCSEQ_PLAYBACK_PLATFORM        (1UL<<5)
#define ENCSEQ_RANDOM_KEY_AND_BIDIR (1UL<<6)
#define ENCSEQ_FRAME_INFO                       (1UL<<7)
#define ENCSEQ_QUICK_COMPRESS           (1UL<<8)
#define ENCSEQ_VALID                            (1UL<<31)

typedef struct {
	DWORD dwSize;
	DWORD dwFourCC;
	DWORD dwVersion;
	LONG mtType;    
	LONG oeEnvironment;
	DWORD dwFlags;

	DWORD dwKey;                                                    /* version 1.0 */
	DWORD dwMinViewportWidth;                               /* version 1.0 */
	DWORD dwMinViewportHeight;                              /* version 1.0 */
	R4_RANGE rngTransparencyRange;                  /* version 1.0 */
	PTR_R4_BITMASK pbmTransparencyBitmask;  /* version 1.0 */
	LONG tmTransparencyMethod;                              /* version 1.0 */
	R4_FLAG fEnabledKey;                                    /* version 1.0 */
	R4_FLAG fScalability;                                   /* version 1.0 */
	R4_FLAG fBiDirPrediction;                               /* version 1.0 */
	R4_FLAG fRandomKeyAndBiDir;                             /* version 1.0 */
	DWORD dwPlaybackPlatform;                               /* version 1.0 */
	WORD wFrameLatency;                                             /* version 1.0 */
	WORD wDeadFrames;                                               /* version 1.0 */
	R4_FLAG fQuickCompress;                                 /* version 1.1 */
} R4_ENC_SEQ_DATA, FAR * PTR_R4_ENC_SEQ_DATA;


#endif /* __VFW_SPEC_H__ */
