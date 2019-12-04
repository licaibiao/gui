#ifndef HstComFunLibArchMsg_H
#define HstComFunLibArchMsg_H

#include "HstlibIPCPMsgStuct.h"
#include "HstlibIpcpCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef Msg_DEBUG__
//#define Msg_DEBUG__
#ifdef Msg_DEBUG__
    #define MsgDEBUG (printf("%s(%d)-<%s>: ",__FILE__, __LINE__, __FUNCTION__), printf)
#else
    #define MsgDEBUG(format,...)
#endif

typedef int (*MsgUnpacklFun) (ARCH_MSG_S *,void*);
typedef int (*MsgPacklFun) (unsigned char *,void*);
typedef struct 
{
    int CmdID;
    MsgUnpacklFun  Fun;
}_MsgUnpackPreTreatment;
typedef struct 
{
    int CmdID;
    MsgPacklFun  Fun;
}_MsgPackPreTreatment;

typedef struct 
{
    int TargAdd;
    int (*MsgSendFun) (ARCH_MSG_S*); 
}MSG_TRANSMIT_T;
typedef MSG_TRANSMIT_T  _stMsgSend; /*rename*/

#define SYSTEM_REBOOT(src_addr,funcstr,linenum)     \
do                                  \
{                                   \
    MSG_0X00F6_S stMsg_0XF6={0};        \
    stMsg_0XF6.ucRstTime=1;         \
	sprintf(stMsg_0XF6.strFunName,"%s",funcstr);\
	stMsg_0XF6.LineNum=linenum;\
    Hst_Arch_Msg_SrcAddr2DirtAddr(QUEUE_COM_SYSRST_CMD, \
    (unsigned char*)&stMsg_0XF6,sizeof(stMsg_0XF6), \
    src_addr,WSD_DAEMON_ADDR,QUEUE_ROUTER_KEY); \
    Hst_Arch_Msg_SrcAddr2DirtAddr(QUEUE_COM_SYSRST_CMD, \
    (unsigned char*)&stMsg_0XF6,sizeof(stMsg_0XF6), \
    src_addr,WSD_BROADCAST_ADDR,QUEUE_ROUTER_KEY); /*reboot for broadcast*/ \
    msleep(500);    \
}while(0)  

extern _MsgUnpackPreTreatment UnpackPreTreatmentFun[];
extern _MsgPackPreTreatment PackPreTreatmentFun[];
int  Hst_Arch_Msg_MSgPackPreTreatmen(int CmdId,unsigned char *Msg_D,void *stP);
int  Hst_Arch_Msg_MSgUnpackPreTreatmen(int CmdId,ARCH_MSG_S *pstMsg,void *stP);

int  Hst_Arch_Msg_Send(key_t key,ARCH_MSG_S *pmi,int milen);
int  Hst_Arch_Msg_Recv(key_t key,ARCH_MSG_S *pmi);
void Hst_Arch_Msg_Clear(key_t key,ARCH_MSG_S *pmi);
int  Hst_Arch_Msg_PackSend(key_t Key,QueueMsgPack *Msg);
int  Hst_Arch_Msg_CommACK(key_t Key,QueueMsgPack *Msg,unsigned short RecCmdId,unsigned short RecFlow,unsigned char Result);
int  Hst_Arch_Msg_Send2SLPThread(key_t Key,ARCH_MSG_S *pstMsg);

int  Hst_Arch_Msg_ChangeSrcAddr(ARCH_MSG_S *pstMsg,unsigned  char SrcAddr);
int  Hst_Arch_Msg_ChangeTargAddr(ARCH_MSG_S *pstMsg,unsigned  char TargAddr);
int  Hst_Arch_Msg_AnalyzeGetACKCMDId(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_AnalyzeGetACKResult(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_AnalyzeGetSrcAddr(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_AnalyzeGetTargAddr(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_AnalyzeGetCmdID(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_AnalyzeGetLen(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_AnalyzeGetgFlowNum();
int  Hst_Arch_Msg_AnalyzeGetRecFlow(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2PLATFORM(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2PLATFORM2(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2PLATFORM3(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2Modem(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2Daemon(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2Router(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2Peripher(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2Gpssevice(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2Capture(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2Other(ARCH_MSG_S *pstMsg);
int Hst_Arch_Msg_Send2BroadCast(ARCH_MSG_S *pstMsg);

int Hst_Arch_Msg_Send2Transport(ARCH_MSG_S *pstMsg);
int Hst_Arch_Msg_Send2Maintain(ARCH_MSG_S *pstMsg);

int  Hst_Arch_Msg_Send2GaodeMap(ARCH_MSG_S *pstMsg);
void Hst_Arch_Msg_SrcAddr2DirtAddr(unsigned short Cmd,unsigned char* Data,unsigned short Datalength,unsigned char SrcAddr,unsigned char DirtAddr,unsigned short Key);

int  Hst_Arch_Msg_Send2LBSSERVICE(ARCH_MSG_S *pstMsg);
int  Hst_Arch_Msg_Send2LBSPLATFORM(ARCH_MSG_S *pstMsg);

int Hst_Arch_Msg_Check(ARCH_MSG_S *pstMsg, int MsgLen);

int  Hst_Arch_Msg_AnalyzeCmdIdPack_0xCE(unsigned char *Msg_D,void* InPara);     //注册平台监控(0xCE)
int  Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCE(ARCH_MSG_S *pstMsg,void* OutPara);   //注册平台监控(0xCE)

int  Hst_Arch_Msg_AnalyzeCmdIdPack_0xCD(unsigned char *Msg_D,void* InPara);     //重连平台监控(0xCD)
int  Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCD(ARCH_MSG_S *pstMsg,void* OutPara);   //重连平台监控(0xCD)

int  Hst_Arch_Msg_AnalyzeCmdIdPack_0xCC(unsigned char *Msg_D,void* InPara);     //释放平台监控(0xCC)
int  Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCC(ARCH_MSG_S *pstMsg,void* OutPara);   //释放平台监控(0xCC)

int  Hst_Arch_Msg_AnalyzeCmdIdPack_0xCB(unsigned char *Msg_D,void* InPara);     //释放平台监控(0xCB)
int  Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCB(ARCH_MSG_S *pstMsg,void* OutPara);   //释放平台监控(0xCB)
void Hst_Arch_Msg_PlatformStartP(U8** p,U8* Data);                      //获取平台数据起始位置
int Hst_Arch_Msg_PowerControl(char cSrcAddr,unsigned char Device,unsigned char Handle);


int HST_PrintHexBytes(unsigned char *pBin,int nLen);
int HST_BcdTime2SysTime_C(unsigned char *pBcdTime, unsigned int *pFileID);
int HST_BcdTime2DecTime_C(unsigned char *pBcdTime, unsigned char *pDecTime);
int HST_DecTime2BcdTime_C(unsigned char *pDecTime, unsigned char *pBcdTime);
int HST_SystemTime2BcdTime_C(time_t stSysTime, unsigned char *pBcdTime);

int HstArchDevSystemDaemonSet(char cSrcAddr,const char* pstrCmd);
bool Hst_Arch_Set_SYSClock(SYSTEM_TIME_SET_S *pstSystemTime);

#ifdef __cplusplus
}
#endif


#endif
