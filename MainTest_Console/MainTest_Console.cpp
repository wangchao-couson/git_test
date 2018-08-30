// MainTest_Console.cpp : Defines the entry point for the console application.
//用于接口使用Demo

#include "stdafx.h"
#include "windows.h"
#include "tsiTcp.h"
#include "Shlwapi.h"
#include "Pathcch.h"
#pragma comment (lib,"Shlwapi.lib")
#define DF_FAKE_UIN 999999999
#define TSI_DNF_CONN_SVR_DOMAIN "tgscp.mainconn.gamesafe.qq.com"
using namespace tscNet;

HMODULE g_hModule;
/* 支持微信体系帐号结构 */
typedef struct
{
	unsigned char account[64];
	unsigned short account_type;
}Accountid;

typedef struct
{
	Accountid account_id;
	unsigned char plat_id;
}UserAccountInfo, *pUserAccountInfo;

// dwCmdID为命令ID，可根据自己定义在net_desc.h中查询得到。如SafeLogin用于上报的dwCmdID为CS_NET_CLIENT_SYSTEM_CHECK_RESULT
typedef struct tagCSManagerInit
{
	ULONG   uGameID;
	ULONG   uUin;
	UserAccountInfo stUserAccountInfo; //新增成员用于支持新账户信息初始化 ChangeByGaeawang
} CSMANAGERINIT, *PCSMANAGERINIT;

void GetTersafePath(DWORD gameId, TCHAR (&terSafeDllFilePath)[MAX_PATH])
{
	ZeroMemory(terSafeDllFilePath, sizeof(terSafeDllFilePath));

	// 获取当前TASChannel.dll的路径, 假设默认的TerSafe.dll与TASChannel.dll路径相同.
	// 以此假设拼接出默认的TerSafe.dll的路径.
	TCHAR defaultTerSafeDllFilePath[MAX_PATH] = { 0 };
	GetModuleFileNameW(g_hModule, defaultTerSafeDllFilePath, MAX_PATH);
	PathRemoveFileSpecW(defaultTerSafeDllFilePath);
	PathAppendW(defaultTerSafeDllFilePath, CS_CHANNEL_DLL_NAME);
	// 通常认为游戏目录下使用的TerSafe.dll组件更新频率更快, 总是比自身目录下的要新.
	// 在此处先将更加新的TerSafe.dll的路径与自身目录设置相同, 后续比较时, 可根据
	// 这两个路径的异同, 判定是否根据不同游戏设置了不同的游戏目录TerSafe.dll路径.
	// 若游戏的TerSafe.dll路径与默认的路径相同, 则表明没有匹配到含有TerSafe.dll组件的游戏,
	// 那么则返回默认的TerSafe.dll路径, 若不同, 则证明某些游戏自带了TerSafe.dll组件,
	// 我们需要将游戏内的新组件拷贝到默认路径下, 并返回默认路径.
	TCHAR newerTerSafeDllFilePath[MAX_PATH] = { 0 };
	_tcsncpy_s(newerTerSafeDllFilePath, defaultTerSafeDllFilePath, _TRUNCATE);

	{
		TCHAR innerPath[] = { 'G', 'a', 'm', 'e', 0 };

		for (int i = 0; i < 3; i++)
		{
			PathRemoveFileSpec(newerTerSafeDllFilePath);
		}

		PathAppendW(newerTerSafeDllFilePath, innerPath);
		PathAppendW(newerTerSafeDllFilePath, CS_CHANNEL_DLL_NAME);
	}


	// 此处判断游戏TerSafe.dll路径是否和默认的TerSafe.dll路径相同, 若不同, 且游戏包含的TerSafe.dll组件存在,
	// 则将其拷贝到默认路径下.
	if (_tcsicmp(newerTerSafeDllFilePath, defaultTerSafeDllFilePath) && PathFileExists(newerTerSafeDllFilePath))
	{
		CopyFile(newerTerSafeDllFilePath, defaultTerSafeDllFilePath, FALSE);
	}

	// 若默认路径下存在TerSafe.dll组件, 则将该组件路径复制返回.
	if (PathFileExists(defaultTerSafeDllFilePath))
	{
		_tcsncpy_s(terSafeDllFilePath, defaultTerSafeDllFilePath, _TRUNCATE);
		// TerSafe开发者非常笨, 接收的创建路径必须不包含文件名, 但必须包含最后的反斜杠.
		// 所以需要下面的处理.
		PathRemoveFileSpec(terSafeDllFilePath);
		PathAddBackslashW(terSafeDllFilePath);
	}
}

BOOL Init(PCSMANAGERINIT pstCSManagerInit)
{
	BOOL bRet = FALSE;
	DWORD  dwTempCode = 0;
	TS_TCPINIT_PARAM  stInitParam = { 0 };
	TS_TCPINIT_PARAM_EX  stInitParamEx = { 0 };
	TS_TCPREG_PARAM  stTsiRegister = { 0 };

		//加载Tersafe.dll，创建新的通道
		TCHAR szModulePath[MAX_PATH] = { 0 };
		GetTersafePath(pstCSManagerInit->uGameID, szModulePath);
		ItnTcp *pTnTcp = CreateCsChannel(_tcslen(szModulePath) ? szModulePath : NULL);

		if (pTnTcp == NULL)
		{
			MessageBoxA(0, "CreateTnTcp Error", 0, 0);
			return bRet;
		}

		//初始化通道
		//不用TP6接口，因为可能出现在游戏中获取不成功的情况
		/*sec_proto_tersafe::AccountInfo stAccountInfo = { 0 };
		CSManager::GetAccountInfo(&stAccountInfo); //若是在游戏外，此函数不能获取到AccountInfo信息*/

		pTnTcp->SetQosWork(1);
		pTnTcp->SetHttpWork(1);
		pTnTcp->SetQosReportWork(1);

/*
		//if (pstCSManagerInit->stUserAccountInfo.account_id.account[0]=='\0')//游戏内，用老的方式初始化CS通道 ChangeByGaeawang
		{
			stInitParam.uGameId = (USHORT)pstCSManagerInit->uGameID;
			stInitParam.uUin = pstCSManagerInit->uUin;
			stInitParam.pszDomain = TSI_DNF_CONN_SVR_DOMAIN;
			VDBGOUT_A(("####CSManager::Init stInitParam.uUin= %d", stInitParam.uUin));
			VDBGOUT_A(("####CSManager::Init stInitParam.uGameId= %d", stInitParam.uGameId));
			VDBGOUT_A(("####CSManager::Init stInitParam.pszDomain= %s", stInitParam.pszDomain));
			//MessageBoxA(0,0,0,0);
			dwTempCode = pTnTcp->Init(&stInitParam);
		}
*/

		//else //游戏外，由上层函数传参数进来
		{
		stInitParamEx.tcpInitParam.uGameId = (USHORT)pstCSManagerInit->uGameID;
		if (pstCSManagerInit->uUin==0)
		{
		pstCSManagerInit->uUin = DF_FAKE_UIN ;//如果uin为0通道会初始化失败
		}
		stInitParamEx.tcpInitParam.uUin = pstCSManagerInit->uUin;
		stInitParamEx.tcpInitParam.pszDomain = TSI_DNF_CONN_SVR_DOMAIN;
		if (stInitParamEx.accountInfo.account_id_.account_[0]=='\0')
		{
		strcpy_s((char*)stInitParamEx.accountInfo.account_id_.account_,sizeof(stInitParamEx.accountInfo.account_id_.account_),"999999999");
		}
		else
		{
		strcpy_s((char*)stInitParamEx.accountInfo.account_id_.account_,sizeof(stInitParamEx.accountInfo.account_id_.account_), (const char*)pstCSManagerInit->stUserAccountInfo.account_id.account);
		}
		stInitParamEx.accountInfo.game_id_ = pstCSManagerInit->uGameID;
		stInitParamEx.accountInfo.plat_id_ = 2;// atoi((char*)(&pstCSManagerInit->stUserAccountInfo.plat_id));
		stInitParamEx.accountInfo.account_id_.account_type_ = pstCSManagerInit->stUserAccountInfo.account_id.account_type;
		//MessageBoxA(0,0,0,0);
		dwTempCode = pTnTcp->InitEx(&stInitParamEx); //使用TerSafe提供的新接口初始化CS通道，用于支持微信登录
		}



		if (SUCCESS != dwTempCode)
		{
			MessageBoxA(0,"CSManager Init Error %u",0,0);
			return bRet;
		}

		//注册通道回调，先回调到CSManager，CSManager再回调给各个ICSEvent
		/*if (bRegister)
		{
			stTsiRegister.uServiceId = dwServiceID;
			stTsiRegister.pTcpEnvent = (ItnTcpEvent *)this;
			dwTempCode = pTnTcp->Register(&stTsiRegister);

			if (SUCCESS != dwTempCode)
			{
				goto Exit0;
			}
		}*/
	
	Exit0:
		return bRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*CSMANAGERINIT stCSManagerInit = {0};
	stCSManagerInit.uGameID = 23;
	stCSManagerInit.uUin = DF_FAKE_UIN;
	BOOL bret = Init(&stCSManagerInit);*/
	DWORD dwUin= 4289999999;
	char qq[64] = {0};
	_ultoa_s(dwUin, qq, 64, 10);
	system("pause");
	return 0;
}

