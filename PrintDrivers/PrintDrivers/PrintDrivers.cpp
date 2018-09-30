// PrintDrivers.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include <windows.h>
#include <Shlwapi.h>
#include <vector>
#include <new>

typedef LONG NTSTATUS;

#define NT_SUCCESS(status)    ((NTSTATUS)(status)>=0)

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemModuleInformation = 11,
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG  Reserved[2];
	PVOID  Base;
	ULONG  Size;
	ULONG  Flags;
	USHORT Index;
	USHORT Unknown;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	CHAR  ImageName[256];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef NTSTATUS(__stdcall *ZWQUERYSYSTEMINFORMATION) (IN int SystemInformationClass, IN OUT PVOID SystemInformation, IN ULONG SystemInformationLength, OUT PULONG ReturnLength OPTIONAL);


// 获取函数地址
ZWQUERYSYSTEMINFORMATION GetZwQuerySystemInformationProcAddress()
{
	ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation = NULL;
	HMODULE hMoudule = GetModuleHandleA("ntdll.dll");

	if (hMoudule == NULL)
	{
		goto Exit;
	}

	ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(hMoudule, "ZwQuerySystemInformation");

	if (ZwQuerySystemInformation == NULL)
	{
		goto Exit;
	}

Exit:
	return ZwQuerySystemInformation;
}

// 比较是否命中敏感驱动
bool IsDisableByDriver()
{
	NTSTATUS status;
	DWORD length = 0;
	bool result = false;
	PSYSTEM_MODULE_INFORMATION module = NULL;
	void* buf = NULL;

	ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation = GetZwQuerySystemInformationProcAddress();
	if (ZwQuerySystemInformation == NULL)
	{
		goto Exit;
	}
	ZwQuerySystemInformation(11, NULL, 0, &length);

	buf = new(std::nothrow) byte[length];

	if (buf == NULL)
	{
		return false;
	}

	status = ZwQuerySystemInformation(11, buf, length, NULL);
	if (!NT_SUCCESS(status))
	{
		return false;
	}

	module = (PSYSTEM_MODULE_INFORMATION)((PDWORD)buf + 1);
	DWORD count = *((PDWORD)buf); //模块数量

	char szPath[MAX_PATH];

	for (DWORD i = 0; i < count; i++)
	{
		ZeroMemory(szPath, MAX_PATH);
		strcpy_s(szPath, strlen(module->ImageName) + 1, module->ImageName);
		//PathStripPathA(szPath);
		printf("%s\n", szPath);
		module = module++;
	}

Exit:
	if (buf != NULL)
	{
		free(buf);
		buf = NULL;
	}

	return result;
}


int main()
{
	IsDisableByDriver();
	system("pause");
    return 0;
}

