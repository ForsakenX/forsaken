#ifdef BREAKPAD
// breakpad running through wine, built for windows doens't work well..
#ifndef __WINE__

#include "main.h"
#include "util.h"

//
// Google Breakpad for cross platform brack trace generation on crashes
//

#include "client/windows/handler/exception_handler.h"

static void wchar_to_char( char * dst_ptr, const wchar_t * src_ptr )
{
	while( *src_ptr != 0 ) // while not end of source
		*dst_ptr++ = (char) *src_ptr++; // conver to char
	*dst_ptr = 0; // finish string
}

static bool breakpad_callback(	const wchar_t* dump_path, const wchar_t* minidump_id, void* context,
								EXCEPTION_POINTERS* exinfo, MDRawAssertionInfo* assertion, bool succeeded )
{
	// if writing to dump file succeeded
	if(succeeded)
	{    
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		char path[256];
		char dir[256];
		char id[256];

		wchar_to_char( &dir[0], &dump_path[0] );
		wchar_to_char( &id[0],  &minidump_id[0] );

		sprintf(path, "%s\\%s.dmp", dir, id);
		
		ZeroMemory( &si, sizeof(si) );
		ZeroMemory( &pi, sizeof(pi) );
		si.cb = sizeof(si);

		if(!CreateProcess( "Utils/crash_reporter.exe", path,
							NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ))
		{
			Msg("%s\n%s\n%s\n%s",
				"Error launching crash reporting tool!",
				"Crash report saved to: ", path,
				"Please email it to fskn.methods@gmail.com");
		}
	}

	// if we succeeded creating a dump
	// then stop any other handlers from running
	return succeeded;
}
extern "C" BOOL breakpad_init( void )
{
	using namespace google_breakpad;
	static ExceptionHandler * e;
	wstring path(L"Dumps");
	e = new ExceptionHandler(
		path, 
		NULL, 
		(google_breakpad::ExceptionHandler::MinidumpCallback) &breakpad_callback, 
		NULL, 
		ExceptionHandler::HANDLER_ALL 
	);//, MiniDumpNormal, pipe_name, custom_info );
	DebugPrintf("initialized breakpad\n");
	return TRUE;
}

#endif // ! __WINE__
#endif // BREAKPAD
