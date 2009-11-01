
#include <windows.h>

int extern main( HINSTANCE hInstance, char * cli );

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, char * cli, int nCmdShow )
{
	return main( hInstance, cli );
}
