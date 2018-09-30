#include "stdafx.h"
#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib, "psapi")
#define NUMSIZE 1024

void EnumKernelModule()
{
	LPVOID drivers[NUMSIZE];
	DWORD cbNeeded;
	int cDrivers, i;
	int a = sizeof drivers;
	if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) &&
		cbNeeded < sizeof(drivers))
	{
		TCHAR szDriver[NUMSIZE];
		int b= sizeof(szDriver);
		cDrivers = cbNeeded / sizeof(drivers[0]);
		int ww = sizeof(szDriver[0]);
		_tprintf(TEXT("There are %d drivers:\n"), cDrivers);
		for (i = 0;i< cDrivers;i++)
		{
			if (GetDeviceDriverBaseName(drivers[i], szDriver,
				sizeof(szDriver) / sizeof(szDriver[0])))
			{
				_tprintf(TEXT("%s\n"), szDriver);
			}
		}
	}

	return;
}

void main()
{
	EnumKernelModule();

	getchar();
}
