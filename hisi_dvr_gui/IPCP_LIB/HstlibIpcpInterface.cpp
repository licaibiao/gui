//#include "HstlibIpcpInterface.h"
#include "IPCP_LIB/HstlibIpcpInterface.h"
static unsigned short g_u16FlowNum =0;

_MsgUnpackPreTreatment UnpackPreTreatmentFun[]=         //对消息的解包预处理  用到增加
{
    {QUEUE_RESET_CMD,NULL},
    {QUEUE_COMMACK_CMD,NULL},
    {QUEUE_REGPLATFORM_CMD,Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCE},
    {QUEUE_DESTORYPLATFORM_CMD,Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCD},
    {QUEUE_GETSTATUS_CMD,Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCC},
    {0,NULL},
    {0,NULL}
};
_MsgPackPreTreatment PackPreTreatmentFun[]=         //对消息的打包预处理   用到增加
{
    {QUEUE_REGPLATFORM_CMD,Hst_Arch_Msg_AnalyzeCmdIdPack_0xCE},
    {QUEUE_DESTORYPLATFORM_CMD,Hst_Arch_Msg_AnalyzeCmdIdPack_0xCD},
    {QUEUE_GETSTATUS_CMD,Hst_Arch_Msg_AnalyzeCmdIdPack_0xCC},
    {QUEUE_RESET_CMD,NULL},
    {QUEUE_COMMACK_CMD,NULL},
    {0,NULL},
    {0,NULL}
};

int Hst_Arch_Msg_MSgUnpackPreTreatmen(int CmdId,ARCH_MSG_S *pstMsg,void *st_P)
{
    for(int i =0;i<(int)ARRAY_SIZE(UnpackPreTreatmentFun);i++)
    {
        if(0==UnpackPreTreatmentFun[i].CmdID)
        {
            break;
        }
        if(CmdId==UnpackPreTreatmentFun[i].CmdID)
        {
            if(NULL!=UnpackPreTreatmentFun[i].Fun)
            {
               UnpackPreTreatmentFun[i].Fun(pstMsg,st_P);
            }
            break;
        }
    }
    return WISDOM_TRUE;
}
int Hst_Arch_Msg_MSgPackPreTreatmen(int CmdId,unsigned char *Msg_D,void *st_P)
{
    for(int i =0;i<(int)ARRAY_SIZE(PackPreTreatmentFun);i++)
    {
        if(0==PackPreTreatmentFun[i].CmdID)
        {
            break;
        }
        if(CmdId==PackPreTreatmentFun[i].CmdID)
        {
            if(NULL!=PackPreTreatmentFun[i].Fun)
            {
              return  PackPreTreatmentFun[i].Fun(Msg_D,st_P);
            }
        }
    }
    return WISDOM_TRUE;
}

static int Hst_Arch_Msg_GetFreeSpace(int msgid)
{
    struct msqid_ds msg_info;

    if(msgctl(msgid,IPC_STAT,&msg_info)==-1)
    {
        perror("msgctl:");
        return WISDOM_FALSE;
    }
    return msg_info.msg_qbytes - msg_info.msg_cbytes;
}
int Hst_Arch_Msg_Check(ARCH_MSG_S *pstMsg,int MsgLen)
{
    if((unsigned int) MsgLen < MSG_ARCH_MSG_LEN)
    {
        return 0;
    }
    return MsgLen;
}
int Hst_Arch_Msg_Send(key_t key,ARCH_MSG_S *pmi,int milen)
{
    int nret=WISDOM_FALSE;
    int iMsgId = msgget(key,666|IPC_CREAT);
    struct msgbuf *pmsg = (msgbuf *)pmi;
    pmi->MsgType=1;
    if(iMsgId >= 0)
    {
        INT8U count = 10;
        while(--count)
        {
            if(Hst_Arch_Msg_GetFreeSpace(iMsgId) > milen) break;
            usleep(300000);
        }
        if(count > 0)
            nret = msgsnd(iMsgId,pmsg,milen,IPC_NOWAIT);
        else
        {
            if( msgctl(iMsgId,IPC_RMID,0)==-1)
            {
                perror("msgctl IPC_RMID !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! err:\n");
                return WISDOM_FALSE;
            }
        }
        if(nret < 0)
            perror("msgsnd send err:\n");
        else
        {
            MsgDEBUG("msg:%d sent,nret=%d\n",key,milen);
        }
    }
    else perror("perror msgget\n");
    return nret;
}

int  Hst_Arch_Msg_Recv(key_t key,ARCH_MSG_S *pmi)
{
    int nret=-1;
    int iMsgId = -1;
    struct msgbuf *pmsg = (msgbuf *)pmi;
    iMsgId = msgget(key,666|IPC_CREAT);
    if(iMsgId >= 0)
    {
        nret = msgrcv(iMsgId,pmsg,MAXSIZE,0,IPC_NOWAIT);
    }
    else
    {
        perror("key msgget err");
    }
    return nret;
}

void Hst_Arch_Msg_Clear(key_t key,ARCH_MSG_S *pmi)
{
    int iMsgId = -1;
    long int msg_to_receive = 1;

    iMsgId = msgget(key,0666|IPC_CREAT);
    if(iMsgId >= 0)
    {
        MsgDEBUG("Message: Start message(queueid=%d,type=%ld) clear,waiting...\n",iMsgId,msg_to_receive);
        while(1)
        {
            if (msgrcv(iMsgId, pmi,sizeof(ARCH_MSG_S), msg_to_receive, IPC_NOWAIT| MSG_NOERROR)==-1)
            {
                break;
            }
        }
        MsgDEBUG("Message: Message(queueid=%d,type=%ld) finished clear.\n",iMsgId,msg_to_receive);
    }
}
int Hst_Arch_Msg_PackSend(key_t Key,QueueMsgPack *Msg)
{
    unsigned short l_s16FlowNum =g_u16FlowNum++;

    if(0==Msg->SrcAddr||0==Msg->TargAddr)
        return WISDOM_FALSE;
    if(0==Msg->Len)
        return WISDOM_FALSE;
    ARCH_MSG_S Msg_St={0};
    unsigned  char *pData=Msg_St.SomeText;
    unsigned short DataLen=0;
    *(pData+0)=(unsigned  char)(QUEUE_MSG_HEAD>>8);              //消息头
    *(pData+1)= (unsigned  char)(QUEUE_MSG_HEAD>>0);
    *(pData+2)= Msg->SrcAddr;
    *(pData+3)= Msg->TargAddr;
    *(pData+4)= (l_s16FlowNum%65535)>>8;
    *(pData+5)= (l_s16FlowNum%65535)>>0;                     //消息流水号
    DataLen+=6;
    *(pData+DataLen+0)= Msg->CmdId>>8;
    *(pData+DataLen+1)= Msg->CmdId>>0;

    *(pData+DataLen+2)= Msg->Len>>8;            //消息长度
    *(pData+DataLen+3)= Msg->Len>>0;
    DataLen+=4;
    if(Msg->Len+DataLen>=MAXSIZE)
        return WISDOM_FALSE;
    memcpy(pData+DataLen,Msg->Data,Msg->Len);    //消息内容
    DataLen+=Msg->Len;

    *(pData+DataLen+0)=0x55;                    //消息校验
    DataLen+=1;

    *(pData+DataLen+0)=(unsigned  char)(QUEUE_MSG_END>>8);        //消息结束
    *(pData+DataLen+1)=(unsigned  char)(QUEUE_MSG_END);
    DataLen+=2;

    Msg_St.MsgType=1;
    return Hst_Arch_Msg_Send(Key,&Msg_St,DataLen);
}

void Hst_Arch_Msg_SrcAddr2DirtAddr(unsigned short Cmd,U8* Data,U16 Datalength,U8 SrcAddr,U8 DirtAddr,U16 Key)
{
    QueueMsgPack Msg={0};
    Msg.Data=Data;
    Msg.SrcAddr=SrcAddr;
    Msg.TargAddr=DirtAddr;
    Msg.CmdId=Cmd;
    Msg.Len=Datalength;
    Hst_Arch_Msg_PackSend(Key,&Msg);
}

int Hst_Arch_Msg_CommACK(key_t Key,QueueMsgPack *Msg,unsigned short RecCmdId,unsigned short RecFlow,unsigned char Result)
{
    if(0==Msg->SrcAddr||0==Msg->TargAddr)
        return WISDOM_FALSE;
    ARCH_MSG_S Msg_St={0};
    unsigned  char *pData=Msg_St.SomeText;
    unsigned short DataLen=0;
    unsigned short FlowNum =g_u16FlowNum++;

    Msg->Len=5;     //通用应答内容只有长度只有5
    Msg->CmdId=QUEUE_COMMACK_CMD;

    *(pData+0)= (unsigned  char)(QUEUE_MSG_HEAD>>8);
    *(pData+1)= (unsigned  char)(QUEUE_MSG_HEAD>>0);
    *(pData+2)= Msg->SrcAddr;
    *(pData+3)= Msg->TargAddr;
    *(pData+4)= FlowNum>>8;
    *(pData+5)= FlowNum>>0;                     //消息流水号
    DataLen+=6;
    *(pData+DataLen+0)= Msg->CmdId>>8;
    *(pData+DataLen+0)= Msg->CmdId>>0;
    *(pData+DataLen+1)= Msg->Len>>8;
    *(pData+DataLen+2)= Msg->Len>>0;
    DataLen+=4;

    *(pData+DataLen+0)= RecCmdId>>8;
    *(pData+DataLen+1)= RecCmdId>>0;            //应答CMDID
    DataLen+=2;

    *(pData+DataLen+0)= RecFlow>>8;
    *(pData+DataLen+1)= RecFlow>>0;             //应答流水号
    DataLen+=2;

    *(pData+DataLen+0)= Result;                 //应答结果
    DataLen+=1;

    *(pData+DataLen+0)=0x55;                    //消息校验
    DataLen+=1;

    *(pData+DataLen+0)=(unsigned  char)(QUEUE_MSG_END>>8);        //消息结束
    *(pData+DataLen+1)=(unsigned  char)(QUEUE_MSG_END);
    DataLen+=2;

    Msg_St.MsgType=1;
    return Hst_Arch_Msg_Send(Key,&Msg_St,DataLen);
}

int Hst_Arch_Msg_AnalyzeGetFlowNum()
{
    return g_u16FlowNum;             //已经是加1的值
}
//a5 a5 a4 a3 93 a9 fe 00 03 93 a8 00
int Hst_Arch_Msg_AnalyzeGetACKResult(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    if(QUEUE_COMMACK_CMD!=Hst_Arch_Msg_AnalyzeGetCmdID(pstMsg))
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    if(0==Dpt[MSG_ACK_RES_ADDR])
        return (Dpt[MSG_ACK_SERIAL_NUM_ADDR]<<8|Dpt[MSG_ACK_SERIAL_NUM_ADDR+1]);
    return WISDOM_FALSE;
}
int Hst_Arch_Msg_AnalyzeGetACKCMDId(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    if(QUEUE_COMMACK_CMD!=Hst_Arch_Msg_AnalyzeGetCmdID(pstMsg))
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    return (Dpt[MSG_ACK_CMID_ADDR]<<8|Dpt[MSG_ACK_CMID_ADDR+1]);
}
int Hst_Arch_Msg_AnalyzeGetRecFlow(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    return (Dpt[MSG_SERIAL_NUM_ADDR]<<8|Dpt[MSG_SERIAL_NUM_ADDR+1]);
}

int Hst_Arch_Msg_ChangeSrcAddr(ARCH_MSG_S *pstMsg,unsigned  char SrcAddr)
{
    if(NULL==pstMsg||0==SrcAddr)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    Dpt[MSG_SRC_ADDR]=SrcAddr;
    return WISDOM_TRUE;
}
int Hst_Arch_Msg_ChangeTargAddr(ARCH_MSG_S *pstMsg,unsigned  char TargAddr)
{
    if(NULL==pstMsg||0==TargAddr)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    Dpt[MSG_TAR_ADDR]=TargAddr;
    return WISDOM_TRUE;
}
int Hst_Arch_Msg_AnalyzeGetSrcAddr(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    return Dpt[MSG_SRC_ADDR];
}
int Hst_Arch_Msg_AnalyzeGetTargAddr(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    return Dpt[MSG_TAR_ADDR];
}

int Hst_Arch_Msg_AnalyzeGetCmdID(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    return Dpt[MSG_CMID_ADDR]<<8|Dpt[MSG_CMID_ADDR+1]<<0;
}

int Hst_Arch_Msg_AnalyzeGetLen(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    return (Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
}
int  Hst_Arch_Msg_Send2SLPThread(key_t Key,ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=Key;
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}

int  Hst_Arch_Msg_Send2PLATFORM(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_PLATFORM_KEY;  //不同平台KEy还不一样，多平台时要注意这一项
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}

int  Hst_Arch_Msg_Send2LBSPLATFORM(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_LBS_PLATFORM_KEY;  //不同平台KEy还不一样，多平台时要注意这一项
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}

int  Hst_Arch_Msg_Send2PLATFORM2(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_PLATFORM2_KEY;  //不同平台KEy还不一样，多平台时要注意这一项
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}
int  Hst_Arch_Msg_Send2PLATFORM3(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_PLATFORM_UDP_KEY;  //不同平台KEy还不一样，多平台时要注意这一项
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}
int  Hst_Arch_Msg_Send2LBSSERVICE(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_LBS_GPSSERVER_KEY;  //不同平台KEy还不一样，多平台时要注意这一项
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}

int Hst_Arch_Msg_Send2Modem(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_MODEM_KEY;
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}
int Hst_Arch_Msg_Send2Daemon(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_DAEMON_KEY;
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}
int Hst_Arch_Msg_Send2Router(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_ROUTER_KEY;
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}
int Hst_Arch_Msg_Send2Peripher(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_PERIPHER_KEY;
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}
int Hst_Arch_Msg_Send2Gpssevice(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_GPSSERVICE_KEY;
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}
int Hst_Arch_Msg_Send2Capture(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_CAPTURE_KEY;

    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}
int Hst_Arch_Msg_Send2Other(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_TOTHER_KEY;
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}

int Hst_Arch_Msg_Send2Maintain(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_MAINTAIN_KEY;
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}

int Hst_Arch_Msg_Send2Transport(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_TRANSPORT_KEY;
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}



int Hst_Arch_Msg_Send2BroadCast(ARCH_MSG_S *pstMsg)
{
    Hst_Arch_Msg_Send2Daemon(pstMsg);
    Hst_Arch_Msg_Send2Gpssevice(pstMsg);
    Hst_Arch_Msg_Send2Peripher(pstMsg);
    Hst_Arch_Msg_Send2Modem(pstMsg);
    Hst_Arch_Msg_Send2Capture(pstMsg);
    Hst_Arch_Msg_Send2Other(pstMsg);
    return 0;
}

int Hst_Arch_Msg_Send2GaodeMap(ARCH_MSG_S *pstMsg)
{
    if(NULL==pstMsg)
        return WISDOM_FALSE;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned short DLen=(Dpt[MSG_MSG_LEN_ADDR]<<8|Dpt[MSG_MSG_LEN_ADDR+1]);
    int MsgLen=DLen+MSG_HEAD_LEN+MSG_END_LEN;
    key_t Sendkey=QUEUE_GAODEMAP_KEY;
    return Hst_Arch_Msg_Send(Sendkey,pstMsg,MsgLen);
}
int Hst_Arch_Msg_AnalyzeCmdIdPack_0xCE(unsigned char *Msg_D,void* InPara)
{
    if(NULL==Msg_D||NULL==InPara)
        return WISDOM_FALSE;
    int offset =0;
    int IPLen=0;
    MSG_0X00CE_S *stMSG_0X00CE=(MSG_0X00CE_S *)InPara;
    //平台地址
    Msg_D[offset]=stMSG_0X00CE->PalatfomAddr;
    offset+=1;
    //平台Kye
    Msg_D[offset+0]=stMSG_0X00CE->PalaMSGQueKey>>24;
    Msg_D[offset+1]=stMSG_0X00CE->PalaMSGQueKey>>16;
    Msg_D[offset+2]=stMSG_0X00CE->PalaMSGQueKey>>8;
    Msg_D[offset+3]=stMSG_0X00CE->PalaMSGQueKey>>0;
    offset+=4;
    //IP
    IPLen=strlen(stMSG_0X00CE->PalatfomIPAddr);
    memcpy(&Msg_D[offset+0],stMSG_0X00CE->PalatfomIPAddr,IPLen);
    offset+=IPLen;
    //PROT
    Msg_D[offset+0]=stMSG_0X00CE->PalatfomPort>>24;
    Msg_D[offset+1]=stMSG_0X00CE->PalatfomPort>>16;
    Msg_D[offset+2]=stMSG_0X00CE->PalatfomPort>>8;
    Msg_D[offset+3]=stMSG_0X00CE->PalatfomPort>>0;
    offset+=4;
    //TCP /UDP
    Msg_D[offset+0]=stMSG_0X00CE->DataPro;
    offset+=1;
    //协议
    Msg_D[offset+0]=stMSG_0X00CE->PalP;
    offset+=1;
    return offset;
}

int Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCE(ARCH_MSG_S *pstMsg,void* OutPara)
{
    if(NULL==pstMsg||NULL==OutPara)
        return WISDOM_FALSE;
    MSG_0X00CE_S *p=(MSG_0X00CE_S *)OutPara;
    unsigned char *Dpt=pstMsg->SomeText;
    unsigned int Iplen=0;
    unsigned int offset=MSG_CONTENT_OFFSET;
    p->PalatfomAddr =Dpt[offset];
    offset+=1;
    printf("p->PalatfomAddr: 0x%x \n",p->PalatfomAddr);
    p->PalaMSGQueKey=Dpt[offset+0]<<24|Dpt[offset+1]<<16|Dpt[offset+2]<<8|Dpt[offset+3]<<0;
    offset+=4;
    printf("p->PalatfomAddr: 0x%x \n",p->PalaMSGQueKey);
    Iplen =strlen((char*)&Dpt[offset]);
    memcpy(p->PalatfomIPAddr,(char*)&Dpt[offset],Iplen);
    offset+=Iplen;
    printf("p->PalatfomIPAddr Iplen %u : %s \n",Iplen,p->PalatfomIPAddr);
    p->PalatfomPort=Dpt[offset+0]<<24|Dpt[offset+1]<<16|Dpt[offset+2]<<8|Dpt[offset+3]<<0;
    offset+=4;
    p->DataPro=(DATA_PROTOCOL_E)Dpt[offset+0];
    offset+=1;
    printf("p->PalatfomIPAddr: %s Port %u \n",p->PalatfomIPAddr,p->PalatfomPort);
    p->PalP =(PALATFOM_PRO_E)Dpt[offset];
    offset+=1;
    int l_s32TmpLen=(offset-MSG_CONTENT_OFFSET);
    if( l_s32TmpLen!= Hst_Arch_Msg_AnalyzeGetLen(pstMsg))
    {
        printf("Hst_Arch_Msg_Analyze ERR %x !!!\n",offset);
        return WISDOM_FALSE;
    }
    return WISDOM_TRUE;
}
int Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCD(ARCH_MSG_S *pstMsg,void* OutPara)
{
    return WISDOM_TRUE;
}
int Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCC(ARCH_MSG_S *pstMsg,void* OutPara)
{
    return WISDOM_TRUE;
}
int Hst_Arch_Msg_AnalyzeCmdIdUnpack_0xCA(ARCH_MSG_S *pstMsg,void* OutPara)
{
    return WISDOM_TRUE;
}
int Hst_Arch_Msg_AnalyzeCmdIdPack_0xCD(unsigned char *Msg_D,void* InPara)
{
    return WISDOM_TRUE;
}
int Hst_Arch_Msg_AnalyzeCmdIdPack_0xCC(unsigned char *Msg_D,void* InPara)
{
    return WISDOM_TRUE;
}
int Hst_Arch_Msg_AnalyzeCmdIdPack_0xCA(unsigned char *Msg_D,void* InPara)
{
    return WISDOM_TRUE;
}

void Hst_Arch_Msg_PlatformStartP(U8** p,U8* Data)
{
    *p= &Data[MSG_CONTENT_OFFSET];
}
int Hst_Arch_Msg_PowerControl(char cSrcAddr,unsigned char Device,unsigned char Handle)
{
    MSG_0X00F8_S stMsg_0XF8;
    stMsg_0XF8.Handle=Handle;
    stMsg_0XF8.Efficacious=Device;
    Hst_Arch_Msg_SrcAddr2DirtAddr(QUEUE_PWCONTROL_CMD,(unsigned char*)&stMsg_0XF8,\
        sizeof(stMsg_0XF8),cSrcAddr,WSD_PERIPHER_ADDR,QUEUE_ROUTER_KEY);
    printf("电源控制[0x%x -> %d ]!!!\n",Device,Handle);
    return 0;
}
int HstArchDevSystemDaemonSet(char cSrcAddr,const char* pstrCmd)
{
    MSG_0X00F7_S stMsg_0XF7={0};
    int iCmdLen=0;
    if(NULL==pstrCmd)
        return -1;
    iCmdLen=strlen(pstrCmd);
    if(iCmdLen)
    {
        stMsg_0XF7.ucSleepTime=0;
        memcpy(stMsg_0XF7.strSysCmd,pstrCmd,iCmdLen);
        stMsg_0XF7.strSysCmd[iCmdLen]='\0';
        Hst_Arch_Msg_SrcAddr2DirtAddr(QUEUE_COM_SYSTEM_CMD,(unsigned char*)&stMsg_0XF7,
            sizeof(stMsg_0XF7),cSrcAddr,WSD_DAEMON_ADDR,QUEUE_ROUTER_KEY);
    }
    hst_msleep(500);
    return 0;
}
bool Hst_Arch_Set_SYSClock(SYSTEM_TIME_SET_S *pstSystemTime)
{
    char cDateTime[32]={0};
    char cCheckTime[32]={0};
    const char *pstrMaxDate ="202312310000"; 	/*modified by fjw 20180417*/
    #if defined(FOR_CRITERION_BD)
    const char *pstrMinDate ="201804120000";  //modified by fjw 20180411
    #else
    const char *pstrMinDate ="201301010000";  //modified by fjw 20180411
    #endif

    if(NULL==pstSystemTime)
    {
        return false;
    }
    snprintf(cCheckTime, sizeof(cCheckTime),"20%02x%02x%02x%02x20%02x",
        pstSystemTime->year,pstSystemTime->month,
        pstSystemTime->date,pstSystemTime->hour,
        pstSystemTime->minute);
    snprintf(cDateTime, sizeof(cDateTime), "date %02x%02x%02x%02x20%02x.%02x",
        pstSystemTime->month,pstSystemTime->date,pstSystemTime->hour,
        pstSystemTime->minute,pstSystemTime->year,pstSystemTime->second);
    if(strcmp(cCheckTime,pstrMinDate)>=0&&strcmp(cCheckTime,pstrMaxDate)<=0)
    {
        HstArchDevSystemDaemonSet(WSD_GPSSERVICE_ADDR,cDateTime);
        memset(cDateTime,InitChar,sizeof(cDateTime));
        snprintf(cDateTime,sizeof(cDateTime),"%s","hwclock -uw");  //modified by fjw 20180411
        printf(" %s\n",cDateTime);
        hst_msleep(10); //modified by fjw 20180411
        HstArchDevSystemDaemonSet(WSD_GPSSERVICE_ADDR,cDateTime);
        HstArchDevSystemDaemonSet(WSD_GPSSERVICE_ADDR,cDateTime);
        HstArchDevSystemDaemonSet(WSD_GPSSERVICE_ADDR,cDateTime);
        HstArchDevSystemDaemonSet(WSD_GPSSERVICE_ADDR,cDateTime);
        printf("******校时*******\n");
        return true;
    }
    else
    {
        printf("[%s %d]******校时出错[%s]*******\n",__func__,__LINE__,cCheckTime);
    }
    return false;
}

/***************************************************************************
*函数名称：HST_PrintHexBytes
*函数介绍：字符转换
*输入参数：无
*输出参数：无
*返回值  ：空间大小
***************************************************************************/
int HST_PrintHexBytes(unsigned char *pBin,int nLen)
{
    int nRet = 1;
    int i;
    if (nLen<=0)  return 0;
    if (nLen>3000)  nLen=3000;
    for(i=0;i<nLen;i++)
    {
        printf("%02x ",pBin[i]);
        if((i + 1)%30==0 && i!=0)
        {
            printf("\r\n");
        }
    }
    printf("\r\n");
    return nRet;
}


int HST_BcdTime2DecTime_C(unsigned char *pBcdTime, unsigned char *pDecTime)
{
  unsigned char High4Bit = 0;
  unsigned char Low4Bit = 0;
  for(int i = 0; i < 6; i++)
  {
    High4Bit = pBcdTime[i] & 0xF0;
    Low4Bit = pBcdTime[i] & 0x0F;
    pDecTime[i] = ((High4Bit>>4) * 10) + Low4Bit;
  }

  return  0 ;
}

int HST_DecTime2SysTime_C(unsigned char *pDecTime, unsigned int *pFileID)
{
  struct tm l_stTm = {0};
  struct tm *l_pstTm = &l_stTm;
  l_stTm.tm_year = pDecTime[0] + 2000 - 1900;
  l_stTm.tm_mon =  pDecTime[1] - 1;
  l_stTm.tm_mday = pDecTime[2];
  l_stTm.tm_hour = pDecTime[3];
  l_stTm.tm_min =  pDecTime[4];
  l_stTm.tm_sec =  pDecTime[5];
  *pFileID = mktime(l_pstTm);
  return 0;
}

int HST_DecTime2BcdTime_C(unsigned char *pDecTime, unsigned char *pBcdTime)
{
    int temp = 0;
  for(int i = 0; i < 6; i++)
  {
    temp = pDecTime[i]%100;
    pBcdTime[i] = ((temp / 10) << 4) + ((temp % 10) & 0x0F);
  }
  return  0 ;
}
int HST_SystemTime2BcdTime_C(time_t SysTime, unsigned char *pBcdTime)
{
    struct tm * pstTime = localtime(&SysTime);
  unsigned char szDecTime[6] = {0};
  szDecTime[0] = pstTime->tm_year-100;
  szDecTime[1] = 1+pstTime->tm_mon;
  szDecTime[2] = pstTime->tm_mday;
  szDecTime[3] = pstTime->tm_hour;
  szDecTime[4] = pstTime->tm_min;
  szDecTime[5] = pstTime->tm_sec;
  HST_DecTime2BcdTime_C(szDecTime, pBcdTime);
  return 0;
}

int HST_BcdTime2SysTime_C(unsigned char *pBcdTime, unsigned int *pFileID)
{
  unsigned char szDecTime[6] = {0};
  HST_BcdTime2DecTime_C(pBcdTime, szDecTime);
  HST_DecTime2SysTime_C(szDecTime, pFileID);
  return 0;
}

