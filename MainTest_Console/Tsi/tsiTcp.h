#ifndef _tsi_tcp_h_
#define _tsi_tcp_h_

#include "tsPubDef.h"
#include "tsiTcpError.h"

#define CS_MAX_SKEY_LEN 256

ns_begin_tscNet;

//////////////////////////////////////////////////////////////////////////
class ItnTcpEvent;

//////////////////////////////////////////////////////////////////////////
// PREDEFINED DOMAIN
#define DNF_CONN_SVR_DOMAIN     ("conn.gamesafe.qq.com")
#define CF_CONN_SVR_DOMAIN      ("conncf.gamesafe.qq.com")

typedef enum tagGameID 
{
    GAME_ID_OTHER           = 0,
    GAME_ID_QQFFO           = 1,
    GAME_ID_QQFO            = 2,
    GAME_ID_CROSSFIRE       = 3,
    GAME_ID_GAMEAPP         = 4,
    GAME_ID_DNF             = 5,
    GAME_ID_UI_LAUNCH_DX    = 6,
    GAME_ID_QY              = 7,
    GAME_ID_QQSG            = 8,
    GAME_ID_DP              = 9,
    GAME_ID_AVA             = 10,
    GAME_ID_QQPETS3D        = 11,
    GAME_ID_CLIENT          = 12,
    GAME_ID_GAME            = 13,
    GAME_ID_NCLIENT         = 14,
    GAME_ID_TTY3D           = 15,
    GAME_ID_QQHXSJ          = 16,
    GAME_ID_LAUNCHER        = 17,  
    GAME_ID_XYCLIENT        = 18,
    GAME_ID_QQGAMEHALL      = 19,   
    GAME_ID_QQMINIGAME      = 20,
    GAME_ID_C9              = 21,
    GAME_ID_H2              = 22,
    GAME_ID_LEAGUEOFLEGENDS = 23,
    GAME_ID_NBA2KONLINE     = 24,
    GAME_ID_GAME_NX         = 25,   
    GAME_ID_QQHX            = 26,
    GAME_ID_XXZSHELL        = 27,
    GAME_ID_TGAME           = 28,
    GAME_ID_ASURA           = 29,  
    GAME_ID_WAROFZOMBIE     = 30,
    GAME_ID_XCLIENT         = 31,
    GAME_ID_DJ2             = 32,
	GAME_ID_ARCHEAGE		= 44,
    GAME_ID_X52             = 45,
	GAME_ID_CODOL			= 52,
	GAME_ID_TPHELPER		= 86,
	GAME_ID_LOLCLIENT		= 89,
    //...
    GAME_ID_QQLOGIN         = 200,
    GAME_ID_QQGameProtect   = 201,
} EGameID;

typedef enum tagSkeyVerifyResult
{
	SKEY_VERIFY_NOT_NEED	= 0,
	SKEY_VERIFY_INIT		= 1,
	SKEY_VERIFY_SUCC		= 2,
	SKEY_VERIFY_FAIL		= 3
}SkeyVerifyResult;

typedef struct tagTS_TCPINIT_PARAM
{
    USHORT uGameId;
    UINT   uUin;
    CHAR  *pszDomain;   //此处必须指定域名
    DWORD  dwWorldId;
} TS_TCPINIT_PARAM;

#pragma pack(1)
typedef struct tagTSConnSkey
{
	DWORD PtloginAppId;
	USHORT uServiceId;
	BYTE SkeyBuffLen;                             
	BYTE SkeyBuff[CS_MAX_SKEY_LEN];          
	BYTE SkeyBuffID;                               	/*    Bind Macrosgroup:TCSConnIdentifyReqID,*/
	BYTE Reserved[8];                              	/*   保留字段 */
}TSConnSkey;
#pragma pack()

typedef struct tagTS_TCPREG_PARAM
{
    UINT         uServiceId;
    ItnTcpEvent *pTcpEnvent;
} TS_TCPREG_PARAM;

typedef struct tagTS_TCPREG_PARAM_EX
{
	TS_TCPREG_PARAM tcpParam;
	TSConnSkey		Skey;
	UINT			uStates;
}TS_TCPREG_PARAM_EX;

//c# sdk，支持open_id初始化cs通道
#pragma pack(1)
typedef struct tagAccountId
{
	unsigned char account_[65];
	unsigned short account_type_;
}AccountId;

typedef struct tagAccountInfo
{
	AccountId account_id_;
	unsigned short plat_id_;
	unsigned int game_id_;
}AccountInfo;

typedef struct tagTS_TCPINIT_PARAM_EX
{
	TS_TCPINIT_PARAM tcpInitParam;
	AccountInfo accountInfo;
}TS_TCPINIT_PARAM_EX;
#pragma pack()

//大数据收发接口
class ItnBigDataRecvHandler
{
public:
	virtual void OnRcvData(BYTE *pbyData, DWORD dwDataSize) = 0;
	virtual void OnError(DWORD dwErrorId) = 0;
	//文件上报完成触发回调，这边是对有需求的用户做异步处理用的
	virtual void OnDataSendCompeled(DWORD dwCrc) = 0;
};

typedef struct tagTS_BIG_PACKET_PARAM
{
	UINT	uServiceId;
	UINT	uCmd;
	ItnBigDataRecvHandler* pBigPacketEvent;
}TS_BIG_PACKET_PARAM;

typedef enum enmCsChannelSvrId
{
    CS_CHANNEL_SVRID_DEFAULT       = 0,    //default server
    CS_CHANNEL_SVRID_TCJ           = 1,    //TCJ模块
    CS_CHANNEL_SVRID_TEST          = 2,    //测试服务器
    CS_CHANNEL_SVRID_TSI           = 3,    //TSI服务器
    CS_CHANNEL_SVRID_TSU           = 4,    //TSU服务器
    CS_CHANNEL_SVRID_ANTIBOT       = 5,    //antibot使用
    CS_CNANNEL_SVRID_SPIDER        = 6,
    CS_CHANNEL_SVRID_SOFTKEYBOARD  = 7,    //仓库锁
    CS_CHANNEL_SVRID_RPCODE        = 8,    //RPCODE
    CS_CHANNEL_SVRID_CSCHANNEL     = 9,    //第二通道
    CSCHANNEL_SERVICE_CTRL         = 10,   // CS通道控制包
    CS_CHANNEL_SVRID_TCJ2_UPLOAD   = 11,   //TCJ2.0 Upload模块
    CS_CHANNEL_SVRID_TCJ2_UPLOAD_ENCRYPT = 12, //TCJ2.0 Upload模块, 加密
    CSCHANNEL_SERVICE_UPGRADE_SVR  = 500, // 组件升级服务
	CS_CHANNEL_SERVICE_WUKONG  = 71,//wukong
	CS_CHANNEL_SERVICE_BIG_PACKET  = 72, // 大包转发服务
    CS_CHANNEL_SVRID_END           = 1024  //end
} ENM_CS_CHANNEL_SVR_ID;

typedef enum enmSendFlags
{
    SEND_FLAGS_NONE           = 0x00,
    SEND_FLAGS_ENCRYPT        = 0x01,
    SEND_FLAGS_COMPRESS       = 0x02,
    SEND_FLAGS_ENCRYPT_QUICK  = 0x04, //高性能解密算法
    SEND_FLAGS_SYNC           = 0xF4,
} ENM_SEND_FLAGS;

//这里的命名有歧义, 和实际业务逻辑对应不恰当
enum EEncryptFunction
{
    EENCRYPT_TenTean  = 0,
    EENCRYPT_TenRijnDael2  = 1,
    EENCRYPT_TenRijnDael3  = 2,
    EENCRYPT_TenRijnDael4  = 3,
    EENCRYPT_TenRijnDael5  = 4,
};

typedef enum enmCsChannelErrId
{
    CS_CHANNEL_CONNECT_ERROR                    = 404,
    CS_CHANNEL_TICKET_VERIFY_ERROR              = 504, 
    CS_CHANNEL_TICKET_VERIFY_SUCCESS            = 604,
    CS_CHANNEL_RECONNECT_FAIL                   = 704,
	CS_CHANNEL_CHANGE_ANTIBOT					= 804,
	CS_CHANNEL_CHANGE_CS						= 904,
	CS_CHANNEL_SKEY_VERIFY_FAIL					= 1004,
	CS_CHANNEL_SKEY_VERIFY_SUCCESS				= 1104,

}ENM_CS_CHANNEL_ERR_ID;

//////////////////////////////////////////////////////////////////////////
static const unsigned int TCP_SEEION_ID_LEN = 16;
class ItnTcpEvent
{
public:
    virtual void OnRcvData(BYTE *pbyData, DWORD dwDataSize) = 0;
    virtual void OnError(DWORD dwErrorId) = 0;
    virtual UINT OnTcpSessionChange(unsigned char byNewSessionID[TCP_SEEION_ID_LEN])  = 0;
};

/* 使用者注意

1、发送函数为异步，调用后立即返回。单次发送数据上限4MB。

2、CS通道回调机制：

   1) 数据回调(OnRcvData)会放到一个队列，通道将单开一个线程，依次回调队列中的函数。
      因此，如果某个回调阻塞住，将会影响到后续数据回调。建议只要不阻塞此函数即可。

   2) 错误回调(OnError)放在窗口线程中进行，所以，请不要在OnError中不要处理大量事务。
      否则会影响整个通道的的工作效率。

3、压缩选项尚未实现。
*/

typedef struct SCsChannelStatus
{
    UINT   uSendSuccPkgCount;                        	   /*   发送pkg成功的个数，非分片 */
    UINT   uSendFailPkgCount;                              /*   发送pkg失败的个数，非分片*/
    UINT   uRecvSuccPkgCount;                        	   /*   接收pkg成功的个数, 非分片 */
    UINT   uRecvFailPkgCount;                              /*   接收pkg失败的个数, 非分片 */
    UINT   uLastRecvPkgSeq;                          	   /*   最后一次接收包的pkg seq*/
    UINT   uLastSendPkgSeq;                          	   /*   最后一次发送包的pkg seq*/
    UINT   uConnectFailCount;                        	   /*   连接失败次数 */
    UINT   uGetSessionFailCount;                     	   /*   请求getsession失败的次数 */
    UINT   uSenderQueueSize;                               /*   发送队列的长度*/
    UINT   uSenderMapSize;                                 /*   发送map的长度*/
    UINT   uRecverMapSize;                                 /*   接收map的长度*/
    UINT   uCallbackQueueSize;                             /*   Callback队列长度*/
    UINT   uClientIPAddress;                               /*   客户端IP地址*/
    UINT   uServiceIPAddress;                              /*   服务器端IP地址*/  
    UINT   uMainCtrlCount;                                 /*   MainCtrl运行次数*/
    UINT   uCallbackCount;                                 /*   CallBack运行次数*/
    UINT   uMainCtrlThreadId;                              /*   线程MainCtrlId*/
    UINT   uCallBackThreadId;                              /*   线程CallBackId*/
    BOOL   bIsConnectOK;                                   /*   连接是否已经建立*/
}CS_CHANNEL_STATUS,*PCS_CHANNEL_STATUS;

//////////////////////////////////////////////////////////////////////////

class ItnTcp
{
public:
    ~ItnTcp(){}
    virtual UINT Init(TS_TCPINIT_PARAM *pInitParam) = 0;
    virtual UINT Register(TS_TCPREG_PARAM *pRegParam) = 0;
    virtual UINT SendData(DWORD dwServiceId, BYTE *pbyData, DWORD dwDataSize,int nSendFlags = SEND_FLAGS_NONE) = 0;
    virtual UINT SetHeartBeat(BOOL bSet) = 0;
    virtual UINT Release() = 0;
    virtual void SetQosWork(bool bUseQos) = 0;
    virtual void SetHttpWork(bool bUseHttp) = 0;
    virtual void SetQosReportWork(bool bUseQosReport) = 0;//加入tqos上报开关
    virtual void GetCsChannelstatus(CS_CHANNEL_STATUS& stCsChannelStatus) = 0;
    virtual UINT SendTicketBuff(BYTE *pbyData, DWORD dwDataSize) = 0;
	virtual UINT GetCsChannelErrCode()=0;
	virtual UINT RegisterSafeChannelEx(TS_TCPREG_PARAM_EX *pInitParam)=0;	//鉴权模式的CS通道初始化
	virtual void SendSkeyBuff(TSConnSkey* skey)=0;	//断线重连以后，需要传递Skey作鉴权。OnTcpSessionChange函数。
	virtual UINT ResetCsChannel(TS_TCPINIT_PARAM *pInitParam)=0;
	virtual void StopCsChannel()=0;
	virtual void UnRegisterSessionChangeCallBack(UINT dwServiceId)=0;
	//支持open_id初始化cs通道
	virtual void SetUserInfo(AccountInfo* pstAccountInfo, BYTE* pbyToken, DWORD dwTokenLen, DWORD dwGameVer)=0;
	virtual UINT InitEx(TS_TCPINIT_PARAM_EX *pInitParam)=0;
	virtual UINT RegisterBigDataService(TS_BIG_PACKET_PARAM *pRegParam) = 0;
	virtual UINT SendBigData(DWORD dwServiceId, BYTE* pbyData, DWORD dwDataSize) = 0;
};


ItnTcp *CreateTnTcp();

int ReportTransStatistic();

ns_end;

//tersafe DllEntry6 导出函数接口
void * WINAPI CreateCsChannelObj();

//如果编译成dll，使用tersafe.dll
#define CS_CHANNEL_DLL_NAME			    TEXT("tersafe.dll")

#define CS_CHANNEL_EXPORT_FUNCTION		"DllEntry"//GetProcAddress 没有unicode 版本

//Dll 导出函数使用方式
typedef tscNet::ItnTcp *(WINAPI *PFNCreateCsChannel)();

inline tscNet::ItnTcp *CreateCsChannel(const TCHAR *pszSafeDllPath = NULL)
{
    HMODULE hModule = ::GetModuleHandle(CS_CHANNEL_DLL_NAME);
	if (hModule == NULL)
	{
		TCHAR szPath[MAX_PATH] = {0};

		if (pszSafeDllPath == NULL)
		{
			::GetModuleFileName(NULL, szPath, MAX_PATH);

			TCHAR *pszLastBackSlash = _tcsrchr(szPath, '\\');

			if (pszLastBackSlash != NULL)
			{
				*(pszLastBackSlash + 1) = 0;
			}
		}
		else
		{
			_tcsncpy(szPath, pszSafeDllPath, sizeof(szPath) / sizeof(szPath[0]) - 1);
		}

		size_t cRemaining = sizeof(szPath) / sizeof(szPath[0]) - _tcslen (szPath) - 1;

		_tcsncat(szPath, CS_CHANNEL_DLL_NAME, cRemaining);
		hModule = ::LoadLibrary(szPath);
		if (hModule == NULL)
		{
			return NULL;
		}
	}

	PFNCreateCsChannel pCreateCsChannel = (PFNCreateCsChannel)::GetProcAddress(hModule, CS_CHANNEL_EXPORT_FUNCTION);

	if (pCreateCsChannel == NULL)
	{
		return NULL;
	}    

	return (tscNet::ItnTcp *)pCreateCsChannel();
}

#endif //_tsi_tcp_h_
