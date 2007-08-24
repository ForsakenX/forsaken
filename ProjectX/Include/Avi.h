#ifndef	AVI_INCLUDED
#define	AVI_INCLUDED

#define AVI_MODE_Letterbox 0
#define AVI_MODE_FullScreen 1

BOOL AVIfileOpen (HWND hwnd, char * file);
void AVIShutDown ( HWND hwnd );
void InitAVI( char *FileNamePtr );
void ReleaseAVI( void );
void AviFinished( void );

#endif
