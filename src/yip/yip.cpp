// yip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "App.h"
#include <exception_report.h>

int main()
{
	// An exception writes deploy\exceptions\exception_<pid>.txt / .dmp (kazilib);
	// `catchexceptions` (a test run) ends the process after it instead of the JIT dialog
	lib::exc::Install(lib::exc::CatchArg());

	return app::Create<App>(global::g_app)
		->Tick()
		->Destroy();
}
