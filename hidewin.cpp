// hidewin.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

# include <stdio.h>


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	PROCESS_INFORMATION pinfo ;
	STARTUPINFO sinfo ;
	DWORD status = 0 ;

	if( ! lpCmdLine || ! strlen( lpCmdLine ) )
	{
		MessageBox( NULL , "Must specify the command line to execute" , "Written by guy.leech@secureplatformsolutions.com (c) 2019" , MB_OK ) ;
		return 1 ;
	}

	GetStartupInfo( &sinfo ) ;

	sinfo.dwFlags = STARTF_USESHOWWINDOW ;
	sinfo.wShowWindow = SW_HIDE ;

	if( ! CreateProcess(
		NULL ,
		lpCmdLine ,
		NULL ,
		NULL ,
		TRUE ,
		CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS ,
		NULL ,
		NULL ,
		&sinfo ,
		&pinfo ) )
	{
		CHAR errbuf[  64 ] ;

		sprintf( errbuf , "CreateProcess failed - code %d" , status = GetLastError() ) ;
		MessageBox( NULL , errbuf , lpCmdLine , MB_OK ) ;
	}
	else
	{
		// we wait here since probably called by winlogon which is potentially waiting for us
		// to finish before continuing - if we exit directly without waiting for child to die,
		// winlogon syncs on us only so it will carry on despite the spawned process still continuing

		WaitForSingleObject( pinfo.hProcess , INFINITE ) ;
		CloseHandle( pinfo.hProcess ) ;
		CloseHandle( pinfo.hThread ) ;
		// if it fails, I do not know what to intelligently do so do nothing!
	}

	// we could possibly use OpenProcess to check that the process has died except that
	// the documentation says a process event only sets signalled state at exit - ie. once only

	return status ;
}
