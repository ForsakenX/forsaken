
				...General Instructions...

Requirements:

	Visual Studios C++ 2008 Express

	Windows 2003 Platform SDK

	DirectX SDK


Environment Variables:

	You can set these environment variable or add the paths to include/lib directories in vs manually.

		MSSDK_DIR =  path to your Window 2003 Platform SDK
		DXSDK_DIR =  path to your DirectX SDK


Templates:

	copy and edit src/version.h.template to projects/msvc9/version.h


Debugging:

	Default Debugger:
	
		This may not be needed for you

			Key: HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug
			Value: "C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" -p %ld -e %ld

	Visual Studios:

		project -> propeteries -> debuggin -> command line arguments

			-window -sleep:1 -debug -chdir ..\\..\\skeleton

