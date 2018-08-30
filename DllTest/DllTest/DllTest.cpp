// DllTest.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DllTest.h"

void DllTest::print()
{
	MessageBoxA(NULL,NULL,"Iam a Test!!",MB_OK);
}

