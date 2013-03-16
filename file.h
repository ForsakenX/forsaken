#ifndef FILE_H
#define FILE_H

#include "main.h"
#include <stdio.h>

struct filetime
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

void touch_file( char* path );
bool is_folder( char* path );
int folder_exists( char *pathspec, ... );
long Get_File_Size( char * Filename );
long Read_File( char * Filename, char * File_Buffer, long Read_Size );
long Write_File( char * Filename, char * File_Buffer, long Write_Size );
long Get_File_Size( char * Filename );
bool file_time( const char * path, struct filetime *t );
long Read_File( char * Filename, char * File_Buffer, long Read_Size );
bool File_Exists( char * Filename );
bool delete_file( char * path );

char* find_file( char * path );
char* find_next_file( void );
void find_close( void );

FILE * file_open(char * filename, char * mode);

#endif // FILE_H
