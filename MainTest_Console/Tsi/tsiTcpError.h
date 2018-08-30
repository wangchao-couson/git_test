#ifndef __TSCNET_TCP_ERROR_H__
#define __TSCNET_TCP_ERROR_H__

#pragma once

/* About Thie File
1¡¢Argument error will be returned by ret-value.
2¡¢Other error will be returned by ItnTcpEvent:OnError().
   It means that some fetal error occurs, the CsChannel will not work any more and CsChannel will Release.
*/

namespace tscNet
{

#define SUCCESS 0
#define FAIL    1

#define ERROR_START_SEQ     FAIL
#define ERROR_BASE(x)       (ERROR_START_SEQ + x)

// Argument Errors
#define ERROR_ARG1_NOT_VALID    ERROR_BASE(1)
#define ERROR_ARG1_IS_NULL      ERROR_BASE(2)
#define ERROR_ARG1_IS_ZERO      ERROR_BASE(3)

#define ERROR_ARG2_NOT_VALID    ERROR_BASE(4)
#define ERROR_ARG2_IS_NULL      ERROR_BASE(5)
#define ERROR_ARG2_IS_ZERO      ERROR_BASE(6)

#define ERROR_ARG3_NOT_VALID    ERROR_BASE(7)
#define ERROR_ARG3_IS_NULL      ERROR_BASE(8)
#define ERROR_ARG3_IS_ZERO      ERROR_BASE(9)

// CTnTcpImp Fetal Errors
#define ERROR_CTNTCPIMP_INIT_DOMAIN_IS_NULL                 ERROR_BASE(10)

// CCallbackMgr Fetal Errors
#define ERROR_CALLBACKMGR_REGISTER_ITCPEVENT_NULL           ERROR_BASE(11)

// CSvrIpMgr Fetal Errors
#define ERROR_SVRIPMGR_GETSVRIPLIST_GETHOSTBYNAME_FAIL      ERROR_BASE(12)

// CCsSocket Fetal Errors
#define ERROR_CCSSOCKET_ONCONNECTSUCCESS_LOSE_TENIO_PTR     ERROR_BASE(13)
#define ERROR_CCSSOCKET_ONCONNECTSUCCESS_SOCKET_INVALID     ERROR_BASE(14)

};

#endif
