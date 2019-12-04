#ifndef _HstlibIPCPMsgStuct_H
#define _HstlibIPCPMsgStuct_H

#include "SysInclude.h"
#include "HstlibIpcpCommon.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    PWCore=0,        //核心板电源
    PWCamera,      //摄像头电源
    PW3GModule,    ////3G模块电源
    PWGpsModule,  //GPS模块电源
    PWLCD,        //LCD电源
    PWExternal_1,  //外设置电源1
    PWExternal_2,  //外设置电源2
}Power_Set;
typedef enum
{
    Pro808=1,
    Pro905=2,
    ProBD =3 
}PALATFOM_PRO_E;
typedef enum
{
    TCP=0,
    UDP=1
}DATA_PROTOCOL_E;

const unsigned int POWER_CONTROL_CORE     =(0x03);
const unsigned int POWER_CONTROL_CAMERA   =(0x0b);
const unsigned int POWER_CONTROL_3GMODULE =(0x08);
const unsigned int POWER_CONTROL_LCD      =(0x01);

const unsigned int DISK_PART_MAX_NUM	  =(16);
const unsigned int DISK_NUM	              =(4);


//电源控制（0xF8）
typedef struct 
{
    unsigned char Efficacious;              //设备类型
    unsigned char Handle;               //0:关闭；1：开启
}__attribute__ ((__packed__))MSG_0X00F8_S;
typedef MSG_0X00F8_S MSG_0XF8;

//系统命令调用          (0xF7)
typedef struct 
{
    unsigned char ucSleepTime;               //延时时间
    char strSysCmd[64];            //命令
}__attribute__ ((__packed__))MSG_0X00F7_S;
typedef MSG_0X00F7_S MSG_0XF7;


//系统命令重启          (0xF6)
typedef struct 
{
    unsigned char ucRstTime;                 //重启次数
    char strFunName[64];
	unsigned int LineNum;
}__attribute__ ((__packed__))MSG_0X00F6_S;
typedef MSG_0X00F6_S MSG_0XF6;




// 注册平台监控(0xCE)
typedef struct 
{
    unsigned char PalatfomAddr;         //平台地址
    unsigned int PalaMSGQueKey;         //平台KEY
    char PalatfomIPAddr[32];            //平台地址IP
    unsigned int PalatfomPort;          //平台地址端口
    DATA_PROTOCOL_E DataPro;               //0 :TCP,1:UDP
    PALATFOM_PRO_E PalP;                   //平台协议
}__attribute__ ((__packed__))MSG_0X00CE_S;
typedef MSG_0X00CE_S MSG_0XCE;

// 重连平台监控(0xCD)
typedef struct 
{
    unsigned char PalatfomAddr;         //平台地址
    unsigned int PalaMSGQueKey;         //平台KEY
    char PalatfomIPAddr[32];            //平台地址IP
    unsigned int PalatfomPort;          //平台地址端口
    DATA_PROTOCOL_E DataPro;               //0 :TCP,1:UDP
    PALATFOM_PRO_E PalP;                   //平台协议
}__attribute__ ((__packed__))MSG_0X00CD_S;
typedef MSG_0X00CD_S MSG_0XCD;

//注册平台应答(0xCA)
typedef struct 
{
    unsigned short SerialNum;           //应答流水号
    unsigned char PalatfomAddr;         //平台地址
    unsigned int PalaMSGQueKey;         //平台KEY
    SOCKET Socket;                      //socket
    unsigned char Status;               //应答结果
}__attribute__ ((__packed__))MSG_0X00CA_S;
typedef MSG_0X00CA_S MSG_0XCA;

//获取平台信息(0xCB)
typedef struct 
{
    unsigned char PalatfomAddr;         //平台地址
    
}__attribute__ ((__packed__))MSG_0X00CB_S;
typedef MSG_0X00CB_S MSG_0XCB;


//释放平台监控(0xCC)
typedef struct 
{
    unsigned char PalatfomAddr;         //平台地址
    
}__attribute__ ((__packed__))MSG_0X00CC_S;
typedef MSG_0X00CC_S MSG_0XCC;


//释放平台应答(0xC9)
typedef struct 
{
    unsigned short SerialNum;           //应答流水号
    unsigned char PalatfomAddr;         //平台地址
    unsigned char Status;               //应答结果
}__attribute__ ((__packed__))MSG_0X00C9_S;

typedef MSG_0X00C9_S MSG_0XC9;


//拔号控制 （0xEE）
typedef struct 
{
    unsigned char DialType;                         //拔号类型 
    unsigned char Handle;                           //操作 0：无效；1：重新拔号；2：停止拔号
}__attribute__ ((__packed__))MSG_0X00EE_S;
typedef MSG_0X00EE_S MSG_0XEE;


//语音呼出（0xED）
typedef struct 
{
    unsigned char strPhonNum[16];                     //号码
}__attribute__ ((__packed__))MSG_0X00ED_S;
typedef MSG_0X00ED_S MSG_0XED;


//主叫状态返回（0xEC）
typedef struct 
{
    unsigned char strPhonNum[16];                     //号码
    unsigned char ucStatus;                           //返回状态 0：接通；1.忙音；2：挂断拒绝 3：成功响铃 4.失败
}__attribute__ ((__packed__))MSG_0X00EC_S;
typedef MSG_0X00EC_S MSG_0XEC;

//语音呼入（0xEB）
typedef struct 
{
    unsigned char strPhonNum[16];                        //号码
    unsigned char ucRingTimes;                           //响铃次数
}__attribute__ ((__packed__))MSG_0X00EB_S;
typedef MSG_0X00EB_S MSG_0XEB;

//被叫操作（0xEA）
typedef struct 
{
    unsigned char strPhonNum[16];                     //号码
    bool Handle;                             //0:挂断；1：接听;
}__attribute__ ((__packed__))MSG_0X00EA_S;
typedef MSG_0X00EA_S MSG_0XEA;

//调节音量操作（0xE9）
typedef struct 
{
    char Handle;                            //值1-8，1最小，8最大
}__attribute__ ((__packed__))MSG_0X00E9_S;
typedef MSG_0X00E9_S MSG_0XE9;

//DTMF操作（0xE8）
typedef struct 
{
    char Dtmf_Num;                            //字符型
}__attribute__ ((__packed__))MSG_0X00E8_S;
typedef MSG_0X00E8_S MSG_0XE8;

// 收到短信（0xE7）
typedef struct 
{
    /* 短信存储在通信模块中的位置 0:UIM 1:NV */
    unsigned char ucStorageType;    
    /* 短信存储在通信模块中的索引 */
    unsigned char ucMsmIndex;
    /* 发送该短信的号码长度 */
    unsigned char ucPhoneNumLen;
    /* 发送该短信的号码 */
    char cPhoneNum[20];
    /* 短信接收时间 */
    char cMsmRcvTime[14];
    /* 短信内容长度 */
    unsigned char ucMsmLen;
    /* 短信内容 */
    char cTxt[200];
}__attribute__ ((__packed__))MSG_0X00E7_S;

// 删除短信（0xE5）
typedef struct 
{
    /* 短信存储在通信模块中的位置 0:UIM 1:NV */
    unsigned char ucStorageType;    
    /* 短信存储在通信模块中的索引 */
    unsigned char ucMsmIndex;
}__attribute__ ((__packed__))MSG_0X00E5_S;

typedef MSG_0X00E5_S MSG_0XE5;

//通讯模块软件重启
typedef struct 
{
    unsigned char ucRstFlag;  // 0xaa:重启 0 :无
    unsigned char ucSleepTime;  // 延时重启时间 0:表示马上重启 秒为单位
}__attribute__ ((__packed__))MSG_0X00E3_S;
typedef MSG_0X00E3_S MSG_0XE3;

//通讯模块硬件重启
typedef struct 
{
    unsigned char ucRstFlag;  // 0xaa:重启 0 :无
    unsigned char ucSleepTime;  // 延时重启时间 0:表示马上重启 秒为单位
}__attribute__ ((__packed__))MSG_0X00E2_S;
typedef MSG_0X00E2_S MSG_0XE2;

//网络状况汇报（0xE1）
typedef struct 
{
    char ConnectStatus; // 网络连接状态 0:未连接 1:连接
    char SignalStrength;// 信号强度 (负数 单位dbm)                        
    char SIMStatus;// 0:未插卡1:插卡
    char SimICCID[21];//SIM卡的ICCID
}__attribute__ ((__packed__))MSG_0X00E1_S;
typedef MSG_0X00E1_S MSG_0XE1;

// 设置MODEM APN
typedef struct 
{
    char cApnName[32];                    
}__attribute__ ((__packed__))MSG_0X00E0_S;
typedef MSG_0X00E0_S MSG_0XE0;


//发送文本[GBK编码]到通讯模块TTS
typedef struct 
{
    char cMode;      // 0:等待当前TTS播完再播放本条文本 1:停止当前TTS播报，然后播报本条文本
    char cGbk[256];  // GBK字节最大256字节
}__attribute__ ((__packed__))MSG_0X00DF_S;
typedef MSG_0X00DF_S MSG_0xDF;

typedef struct 
{
    /* 0:运营商未知,1:中国移动,2:中国联通,3:中国电信 */
    char cOpr;                // 运营商              固定1字节
    char szIccid[20];         // ICCID               固定20字节
    char szModemVerDesc[257]; // Modem型号 版本描述  固定257字节
}__attribute__ ((__packed__))MSG_0X00DC_S;
typedef MSG_0X00DC_S MSG_0xDC;

// xzh 20170605 >
//多媒体检索 
typedef struct
{
	unsigned char ucMediaType;		// 0检索图片，1检索录音，2检索视频, 3log，0xff:不确定类型
	unsigned char ucChnId;			// 0表示检索所有摄像头，录音忽略此参数
	unsigned char ucEvent;			// 0xFF：表示所有
	unsigned char szStartTime[6];	// BCD格式 20YY-MM-DD-hh-mm-ss
	unsigned char szEndTime[6];		// BCD格式 20YY-MM-DD-hh-mm-ss

	unsigned char srcPlat;			// 下发命令的平台地址	
}__attribute__ ((__packed__))MSG_0X0050_S;  

typedef MSG_0X0050_S MSG_0X50;

//单条存储多媒体数据上传
typedef struct
{
	unsigned int  fileId;			// 文件id，不为0时，按文件id查找
	unsigned char deleteFlag;		// 删除标志:0：保留；1：删除
	unsigned char upStopFlag;		// 停止上传标志:0：保留；1：停止上传	
	unsigned char srcPlat;			// 下发命令的平台地址
}__attribute__ ((__packed__))MSG_0X0052_S;

/* 格式化存储器 */
typedef struct
{
	unsigned char u8PartCmd[DISK_PART_MAX_NUM];		       /* 0：不操作；1：格式化；数组中元素0对应分区1，以此类推 */
}__attribute__ ((__packed__))MSG_0X0053_S;

/* 通道项信息 */
typedef struct
{
	unsigned int    u32PhyChn;      /* 物理通道号,从0开始*/
	unsigned int    u32CamInput;    /* 摄像头输入属性,0:AHD;1:IPC;2:CVBS*/
	unsigned int    u32CamReslu;    /* 摄像头分辨率,0:D1;1:720P;2:1080P*/
	unsigned int    u32RecReslu;    /* 录制分辨率,0:QCIF;1:CIF;2:WCIF;3:D1;4:WD1;5:720P;6:1080P;*/
	unsigned int    u32RecFps;      /* 录制帧率,10--30FPS*/
	unsigned int    u32StartTime;  /* 连续录像的开始录像时间*/
	unsigned long long u64Size;/* 连续录像大小*/
	unsigned int    u32StopTime;/*连续录像的停止时间,若未停止则为0*/
}__attribute__ ((__packed__))PER_CHN_INFO_S;

/* 分区项信息 */
typedef struct
{
	unsigned int    u32PartNTotalVol;   /* 各分区总空间大小,以M为单位*/
	unsigned int    u32PartNFreeVol;	/* 各分区剩余空间大小,以M为单位*/
}__attribute__ ((__packed__))PER_PART_INFO_S;

/* 磁盘项信息 */
typedef struct
{
    unsigned int    u32DiskNo;/*磁盘号*/
    unsigned char   u8DiskType;/*磁盘类型:0:SD卡，1:硬盘*/	
    unsigned int    u32PartNum;/*磁盘分区数*/
    /*需要根据分区数拼接分区项信息,如果磁盘分区数为0,则此项不存在*/
}__attribute__ ((__packed__))PER_DISK_INFO_S;

/* 多媒体状态信息 */
typedef struct
{
	unsigned int    u32CamOnLine;       /* 按位分析：1表示在线，0表示不在线,Bit 0:摄像头1 */
	unsigned int    u32CamRecord;       /* 按位分析：1表示在录像，0表示没录像,Bit 0:摄像头1 */
	unsigned int    u32CamLive;         /* 按位分析：1表示正在发送实时视频，0表示无,Bit 0:摄像头1 */
	double          d32CpuUseRate;      /* CPU使用率，100%,注意:数据大小8字节*/
	double          d32MemUseRate;      /* 内存使用率，100%,注意:数据大小8字节*/
	unsigned int    u32FlashTotalVol;   /* 内置FLASH总空间大小,以M为单位*/
	unsigned int    u32FlashFreeVol;    /* 内置FLASH剩余空间大小,以M为单位*/
	unsigned int    u32ExStorageNum;    /* SD卡或硬盘总数个数*/
	unsigned int    u32ChnNum;          /* 设备通道数目*/
    /*********需要根据磁盘数及通道数,拼接磁盘项及通道项信息.*********/
}__attribute__ ((__packed__))MSG_0X0054_S;

//人脸识别结果
typedef struct
{
	unsigned char Type;		// 0人脸识别结果，1人头个数识别
	unsigned char result;   // type为0时，0成功，1失败
							// type为1时，0-255个
}__attribute__ ((__packed__))MSG_0X005D_S;  
typedef MSG_0X005D_S MSG_0X5D;

typedef struct
{
    BYTE u8ChnalID;        //通道ID,从1开始
    BYTE u8SrcPlat;        //消息来源平台地址
    PALATFOM_PRO_E ePalP;  //平台协议
    BYTE  u8Event;         //事件类型，注:不同协议，类型含义不同，应由gps进程赋值
    BYTE  u8StoreUpFlag;   //保存上传标志: 0：保存并且上传;1：只保存不上传
    DWORD u32PhotoLen;     //图片数据长度
    BYTE  arrPhotoName[64]; //图片所在的绝对路径
}__attribute__ ((__packed__))MSG_0x0078_S;
typedef MSG_0x0078_S MSG_0X78;

/*add by fjw 20180112*/
//位置信息获取
typedef struct
{
	unsigned char ucInterval;			// 时间间隔
	unsigned int uiDuration;    		// 持续时间
}__attribute__ ((__packed__))MSG_0X0090_S;

typedef MSG_0X0090_S MSG_0X90;

//位置信息获取应答
typedef struct
{
	unsigned char 	GpsBCD[6];			// 定位时间
	unsigned char 	SysBCD[6];			// 系统时间
	unsigned int  	uiLatitude;    		// 纬度 
	unsigned int 	uiLongtitude;    	// 经度 
	unsigned short 	SpeedX10;    		// 速度 
	unsigned char 	uiDirect;    		// 方向 
}__attribute__ ((__packed__))MSG_0X0009_S; 
typedef MSG_0X0009_S MSG_0X09;

//拍照控制 
typedef struct
{
 	BYTE ChnalID;				//通道ID,从1开始
    WORD PhotoGraphCmd;			//拍摄命令:0 表示停止拍摄；0xFFFF表示录像；其它表示拍照张数
    WORD PhotoGraphTime;		//拍照间隔/录像时间
    BYTE StoreFlag;				//保存标志: 1：保存;0：实时上传
    BYTE Resolution; 			//分辨率 
    BYTE PhotoQuality;			//图像/视频质量
    BYTE Bright;				//亮度
    BYTE Contrast;				//对比度 
    BYTE Saturability;			//饱和度 
    BYTE Chroma;				//色度

	BYTE SrcPlat;				//消息来源平台地址
	PALATFOM_PRO_E PalP;        //平台协议
	BYTE Event;					//事件类型，注:不同协议，类型含义不同，应由gps进程赋值
	BYTE enSize;				//图片格式，注:不同协议，格式含义不同，应由gps进程赋值
}__attribute__ ((__packed__))MSG_0X0070_S;

//回传拍照的文件id
typedef struct
{
    DWORD MultiId;				//多媒体ID,小端内存
   	WORD  TotalNum;				//下发拍照的总数
    WORD  CurNum;				//当前图片序号 	

	BYTE  PlatAddr;				//平台地址
}__attribute__ ((__packed__))MSG_0X0071_S;

//回传待上传文件信息
typedef struct
{
    DWORD MultiId;				//多媒体ID,小端内存
    BYTE  MultiType;			//多媒体类型: 0：图像；1：音频；2：视频； 
    BYTE  MediaFormat;			//多媒体格式编码:0：JPEG；1：TIF；2：MP3；3：WAV；4：WMV；
    BYTE  Event;				//事件项编码    
    BYTE  ChnalId;				//通道ID，从1开始 
    BYTE  GpsInfo[28];			//位置信息,北斗协议用
	
	BYTE  PlatAddr;				//平台地址
	DWORD FileSize;				/* 文件大小，单位字节 */
	BYTE  FileName[128];		//文件路径
}__attribute__ ((__packed__))MSG_0X0072_S;

//录音控制
typedef struct
{
    BYTE RecordCmd;				//录音命令 0：停止录音；0x01：开始录音 
    WORD RecordTime;			//录音时间 单位为秒(s)，0 表示一直录音
    BYTE StoreFlag;				//保存标志 0：实时上传；1：保存
    BYTE SampleRate;			//音频采样率 0：8K；1：11K；2：23K；3：32K；其他保留 

	BYTE SrcPlat;				//消息来源平台地址
	PALATFOM_PRO_E PalP;        //平台协议
}__attribute__ ((__packed__))MSG_0X0073_S;

/* 位置信息广播 */ 
typedef struct
{
	DWORD   u32Alarm;      		/* 报警 */
	DWORD 	u32Status;     		/* 状态 */
	DWORD 	u32Latitude;   		/* 纬度，百万分之一度 */  
	DWORD 	u32Longtitude; 		/* 经度，百万分之一度 */  
	WORD	u16Altitude;		/* 高程，米 */
	WORD	u16SpeedX10;   		/* gps速度，1/10 km/h */
	WORD  	u16Direct;     		/* 方向 */
	BYTE	u8Time[6];    		/* 时间，BCD,YYMMDDhhmmss */
 	BYTE    arrCarCardNo[16];	/* 机动车牌号码			 */
	WORD    u16SensorSpeed;     /* 脉冲速度，1/10 km/h */	
	BYTE    u8AdasAlarm[3];     /* ADAS报警标识，用于报警短视频标识,对应采集进程的U64的高24位 */
	BYTE    u8AdasChnalId;      /* ADAS报警录像通道号*/
	BYTE    u8AdasAlarmPreRecTm;/* ADAS报警短视频起始时间，单位:秒*/
	BYTE    u8AdasAlarmRecTm;   /* ADAS报警短视频持续时间，单位:秒*/
}__attribute__ ((__packed__))MSG_0X0074_S;

/* 休眠唤醒事件下发 */ 
typedef struct
{
	DWORD   u32ModeEvent;      		/* 休眠唤醒事件:1：休眠，2：唤醒 */	
}__attribute__ ((__packed__))MSG_0X0075_S;

/* 报警视频覆盖标记 */
typedef struct
{
	DWORD    u32ParaId;   		/* 参数id: 0x1000010C*/
	BYTE     u8ParaLen;			/* 参数长度: 4 */
	unsigned long long     u64AlarmFlag;		    /* bit*/
}__attribute__ ((__packed__))ALARM_FLAGS_S;


/* TCP 超时 */
typedef struct
{
	DWORD    u32ParaId;   		/* 参数id: 0x1000010C*/
	BYTE     u8ParaLen;			/* 参数长度: 4 */
	DWORD    u32TimeOut;		    /* 单位为秒*/
}__attribute__ ((__packed__))TCP_TIMEOUT_S;

/* 摄像头通道开关参数项信息 */
typedef struct
{
	DWORD    u32ParaId;   		/* 参数id: 0x1000010C*/
	BYTE     u8ParaLen;			/* 参数长度: 4 */
	DWORD    u32CamEn;		    /* 参数值: 0-31位对应1-32路通道(即bit0表示第一路)，为1表示通道开*/
}__attribute__ ((__packed__))PARA_CAMEN_S;

/* 摄像头输出设置参数项信息 */
typedef struct
{
	DWORD    u32ParaId;   		/* 参数id: 0x1000010E*/
	BYTE     u8ParaLen;			/* 参数长度: 4 */
	DWORD    u32CamOutputEn;	/* 参数值: 0-31位对应1-32路通道(即bit0表示第一路)，为1表示此通道开启输出，为0表示此通道关闭输出 */
}__attribute__ ((__packed__))PARA_CAMOUT_S;

/* 外部参数下发  */ 
typedef struct
{
	BYTE	u8ParaNum;          /* 参数总数 */
	/* 根据参数总数配置*/
}__attribute__ ((__packed__))MSG_0X0077_S;

typedef enum
{
	ENV_SYSTIME = 1,
	ENV_REBOOT  = 2, 
	ENV_DEF_PARA  = 3, 
	ENV_FORMAT_DISK = 4
}EXENV_TYPE_E;

/* 外部事件下发 */
typedef struct
{
	unsigned int    u32ExEnventType;       /* 外部事件类型: 详见EXENV_TYPE_E */
}__attribute__ ((__packed__))MSG_0X0076_S;

/*通知视频进程将格式化存储设备 */
typedef struct
{
	unsigned char u8FormatType;	/*格式化类型*/ 		 /* 0：不操作；1:格式化 低4位表示 SD1-SD4，高4位表示HD1-HD4，0xff 表示格式化所有 */
}__attribute__ ((__packed__))MSG_0X0067_S;

/* 多媒体报警信息 */
typedef struct
{
	unsigned int u32VideoAlarm;		/* 视频相关报警 */ 	
	unsigned int u32VideoSgnlLost;	/* 视频信号丢失报警状态 */
	unsigned int u32VideoSgnlCover;	/* 视频信号遮挡报警状态 */
	unsigned short u16StorErrInfo;  /* 存储器故障报警 */
	unsigned short u16AbnmlActInfo; /* 异常驾驶行为报警描述 */	
}__attribute__ ((__packed__))MSG_0X0068_S;

/* 音视频阈值报警信息 */
typedef struct
{
	unsigned short u16AlarmRecPercent;/* 阈值大小*/ 	
	unsigned int u32AVAlarmDataSize;  /* 报警音视频大小 */
	unsigned int u32AVAllDataSize;	  /* 所有音视频大小*/
}__attribute__ ((__packed__))MSG_0X0069_S;

/* 音视频阈值报警回复操作 */
typedef struct
{
	unsigned int u32CfmCtrl;/* 0：无效；1：删除；2：覆盖 */ 	
}__attribute__ ((__packed__))MSG_0X006A_S;

/* 终端状态信息 */
typedef struct 
{
    unsigned char u8StarNum;                /* 收星数 */
    unsigned char u8Service1Online;         /* 平台在线状态 bit0:平台1,bit1:平台2 */
    unsigned int arrCnnetTimes[4];          /* 平台连接次数 预留4个平台 */
    unsigned char arrSerialNumber[13];      /* 通讯号 */
    unsigned char arrDCEID[8];              /* 终端ID */
    unsigned short u16PowerVoltage;         /* 主电源电压 */
    unsigned int u32Adas_offline;           /* ADAS离线时间 */ 
    unsigned int arrMsg_num[4];             /* 单点上传个数 预留4个平台 */
    unsigned int arrMsg_num_bakup[4];       /* 盲点补传个数 预留4个平台 */
    unsigned int arrMsg_num_storage[4];     /* 现有盲点个数 预留4个平台 */
    unsigned int u32RunMileage;             /* 累计行驶里程 */
    unsigned int u32TotalFlowmeter;         /* 总累计流量 */
    unsigned char arrMcu_version[50];  
    unsigned char arrMpu_version[50];
    unsigned char arrLcd_version[50];
    unsigned char arrAdas_version[50];
    unsigned char arrModom_version[50]; 
    unsigned char arrGps_version[50];
    unsigned int u32RebootCnt;            /* 复位记录次数 */
    unsigned int u32FlowmeterCnt;         /* 断电记录次数 */
}__attribute__ ((__packed__))MSG_0X00A1_S;

/* MCU芯片序列号查询 */
typedef struct 
{ 
    unsigned char arrChipSerialNum[50];       /* MCU芯片序列号 考虑要不要定长 */
}__attribute__ ((__packed__))MSG_0X00A2_S;

/* 紧急事件上报 */
typedef struct 
{
    unsigned char u8Emergency;             
}__attribute__ ((__packed__))MSG_0X00A3_S;

/* 终端控制 */
typedef struct 
{ 
    int s32DevControl; /* 复位:0,断电:1,格式化存储设备:2,恢复出厂设置:3(清空用户数据、恢复默认参数),恢复默认参数:4 */
}__attribute__ ((__packed__))MSG_0X00A4_S;

/* 参数查询 */
typedef struct 
{ 
    int s32PlatAttribute;               /* 平台远程端口,原值返回 */
    unsigned int u32SerialNum;          /* 平台流水号,原值返回 */
    unsigned char arrParaID[50];  	    /* APP参数ID,原值返回 */
    unsigned int u32ParaID;        	    /* 参数ID */
    unsigned char ParaDataType;         /* 参数类型 */ 	
    unsigned int u32ParaLength;        	/* 参数值长度 */
    /* 根据长度动态获取 */
}__attribute__ ((__packed__))MSG_0X00A5_S;

/* 透传给MCU参数设置及查询 */
typedef struct 
{ 
    unsigned char u8State ;             /* 设置:0,查询:1 */
    int s32PlatAttribute;               /* 平台远程端口,原值返回 */
    unsigned int u32SerialNum;          /* 平台流水号,原值返回 */
    unsigned char arrParaID[50];  	    /* APP参数ID,原值返回 */
    unsigned char arrParaValue[128];    /* 参数值 注意:实际有时候透传长度大于128 */
}__attribute__ ((__packed__))MSG_0X00A6_S;

/* 录单参数设置及查询 */
typedef struct 
{ 
	unsigned char u8State ;             /* 状态：设置（0）/查询(1) */
    int s32PlatAttribute;               /* 平台远程端口,原值返回 */
    unsigned int u32SerialNum;          /* 平台流水号,原值返回 */
    /* 见JT808/0x8106表15 与JT808/0x8103表10 大端模式 */
}__attribute__ ((__packed__))MSG_0X00A7_S;

/* 参数设置 */
typedef struct 
{ 
    int s32PlatAttribute;               /* 平台远程端口,原值返回 */
    unsigned int u32SerialNum;          /* 平台流水号,原值返回 */
    unsigned char arrParaID[50];  	    /* APP参数ID,原值返回 */
    unsigned int u32ParaID;        	    /* 参数ID */
    unsigned char ParaDataType;         /* 参数类型 */ 
	unsigned int u32ParaLength;        	/* 参数值长度 */

    /* 根据长度动态获取 */
}__attribute__ ((__packed__))MSG_0X00A8_S;

/* 配置文件设置 */
typedef struct 
{ 
    unsigned char u8ProfileStatus;               /*0:参数配置文件,其他值后面扩展 */
}__attribute__ ((__packed__))MSG_0X00A9_S;

/* 与other进程升级标志交互 */
typedef struct 
{ 
    unsigned char u8UpdateFlag;               /*0:提示升级,1可升级(会重启),2不可升级... */
}__attribute__ ((__packed__))MSG_0X00AA_S;

/* 上传文件 */
typedef struct 
{ 
    unsigned char arrUrl[128]; /* eg ftp://192.168.20.251:21/test_921/ */
    unsigned char arrUserPass[50]; /* 用户名,密码 eg "wisdom:Hst2017" */
    unsigned char arrLocalFilePath[50]; /* 上传文件本地路径 */
}__attribute__ ((__packed__))MSG_0X00AB_S;

/* 上传文件应答 */
typedef struct 
{ 
    unsigned char u8UpfileAck;               /*0:成功 其他值:失败 */
}__attribute__ ((__packed__))MSG_0X00AC_S;



///* QT视频预览 */
//typedef struct
//{
//    unsigned char u8OnOff;               /*开启控制*/
//    unsigned char u8DisplayMode;         /*显示模式*/
//    unsigned char u8CHDisplay;           /*显示通道*/
//    unsigned char u8OSDSet;              /*OSD设置*/
//}__attribute__ ((__packed__))MSG_0x0105_S;

/**=====================================================**/
/**=====================================================**/
/**=====================================================**/
/* QT视频预览 */
typedef struct
{
    unsigned char u8OnOff;               /*开启控制*/
    unsigned char u8DisplayMode;         /*显示模式*/
    unsigned char u8CHDisplay;           /*显示通道*/
    unsigned char u8OSDSet;              /*OSD设置*/
}__attribute__ ((__packed__))MSG_0x0105_S;


/*摄像头输入状态*/
typedef struct
{
    unsigned char u8InputStatus;      //通道输入    UINT8   Bit位；1：有输入，0：无输入
    unsigned char u8InputAttr[8];   //输入属性    UINT8[16]   0x00：无法识别，0x01:D1,0x02:720P,0x03:960P,0x04:1080P
    unsigned char u8InputUse;        //输入设置    UINT8   Bit位；1：启用，0：不启用
}__attribute__ ((__packed__))MSG_CAMERA_STATUS_S;


/*硬盘/SD存储器状态*/
typedef struct
{
    unsigned int u32TotalSize;      //总大小   UINT32  单位M
    unsigned int u32PartitionNum;   //分区数   UINT32
    unsigned int u32VedioFree;       //录像内容大小  UINT32  单位M
}__attribute__ ((__packed__))MSG_SD_STATUS_S;


/*录像状态*/
typedef struct
{
    unsigned char u8RecIng;    //Bit位；1：正常，0：异常
    unsigned char u8RecAttr[8];    //录像属性    UINT8[16]   0x01:D1,0x02:720P,0x03:960P,0x04:1080P
}__attribute__ ((__packed__))MSG_RECORD_STATUS_S;

/* QT设备状态汇报 */
typedef struct
{
    MSG_CAMERA_STATUS_S stCamaraStatus;
    MSG_RECORD_STATUS_S stRecordStatus;
    MSG_SD_STATUS_S     stSDStatus;
}__attribute__ ((__packed__))MSG_0x0110_S;


/*QT用户管理参数*/
typedef struct
{
    unsigned char u8OnOff;           /*0：关闭；1：打开*/
    unsigned char arru8UserName[32]; /*用户名*/
    unsigned char arru8PassWord[32]; /*密码*/
    unsigned char arru8Temp[64];     /*预留备用*/
}__attribute__ ((__packed__))QT_USER_PARA_S;


/*QT终端设置参数*/
typedef struct
{
    unsigned char arru8CompanyName[64];     /*公司名称*/
    unsigned char arru8CarNum[32];          /*车牌号*/
    unsigned char arru8DriverName[32];      /*司机名称*/
    unsigned char arru8LineName[32];        /*线路名称*/
    unsigned char arru8DeviceID[32];        /*设备序号 */
    unsigned char u8LogOnOff;               /*0：关闭；1：打开*/
    unsigned char arru8Temp[64];            /*预留备用*/
}__attribute__ ((__packed__))QT_TERMINAL_PARA_S;

/*QT系统设置参数*/
typedef struct
{
    unsigned char u8Language;                /*语言设置，0：简体中文*/
    unsigned char arru8TimeSet[6];           /*时间设置*/
    unsigned char arru8CameraPreview;        /*0：关闭；1：打开*/
    unsigned char u8PreviewMode;             /*0：4分屏*/
    unsigned char u8SysReset;                /*软件复位 */
    unsigned char arru8SofltVersion[32];     /*软件版本*/
    unsigned char arru8Temp[64];             /*预留备用*/
}__attribute__ ((__packed__))QT_SYSTEM_PARA_S;

/*QT录像设置*/
typedef struct
{
    unsigned char arru8InputAttr[16];        /*输入属性*/
    unsigned char arru8DisplayMode[16];      /*显示模式*/
    unsigned char arru8VideoOnOff[16];       /*录像开关*/
    unsigned char arru8AudioOnOff[16];       /*录音开关*/
    unsigned char arru8ImageCenter[16];      /*图像中心*/
    unsigned char arru8ChannelName[16][32];  /*通道名称*/
    unsigned char arru8RecordAttr[16];       /*录像属性*/
    unsigned char arru8Temp[64];             /*预留备用*/
}__attribute__ ((__packed__))QT_RECORD_PARA_S;


/*QT触发设置*/
typedef struct
{
    unsigned char arru8OnOff[16];          /*使能*/
    unsigned char arru8DelaySet[16];       /*延时设置*/
    unsigned char arru8AuxiliaryLine[16];  /*辅助线*/
    unsigned char arru8PreviewCH[16];      /*预览通道*/
    unsigned char arru8Temp[64];           /*预留备用*/
}__attribute__ ((__packed__))QT_TRIGGER_PARA_S;


/* QT参数设置 */
typedef struct
{
    unsigned char arru8ParaID[2];    /*参数ID*/
    unsigned short arru8ParaLen;         /*参数长度*/
    unsigned char u8ParaData[128];        /*参数内容*/
    /*.....*/
}__attribute__ ((__packed__))MSG_0x0102_S;

/* QT参数查询 */
typedef struct
{
    unsigned char arru8ParaID[2];          /*参数ID 0:表示所有参数*/
    unsigned char u8ParaData[128];        /*参数内容*/
    /*.....*/
}__attribute__ ((__packed__))MSG_0x0103_S;


/* QT参数查询应答 */
typedef struct
{
    unsigned char arru8ParaID[2];    /*参数ID*/
    unsigned char arru8ParaLen[2];   /*参数长度*/
    unsigned char u8ParaData[1024];  /*参数内容*/
    /*.....*/
}__attribute__ ((__packed__))MSG_0x0130_S;


/* QT设备控制 */
typedef struct
{
    unsigned char u8SysReboot;        /*系统复位*/
    unsigned char u8FormatSD;         /*格式化存储器*/
    unsigned char u8SysTimeSet;       /*系统时间设置*/
    unsigned char u8SysBCDTime[6];    /*系统时间*/
}__attribute__ ((__packed__))MSG_0x0104_S;

typedef enum
{
    FORMAT_NONE = 0X00,
    FORMAT_ING = 0X01,
    FORMAT_OK = 0X02,
    FORMAT_ERROR = 0X03 ,
    FORMAT_NEED_AGAIN = 0X04
}HIS_FORMAT_STATUS_E;

/* QT设备控制 */
typedef struct
{
    unsigned char u8SysRebootACK;        /*系统复位*/
    unsigned char u8FormatSDACK;         /*格式化存储器*/
    unsigned char u8SysTimeSetACK;       /*系统时间设置*/
}__attribute__ ((__packed__))MSG_0x0140_S;


///* QT视频预览 */
//typedef struct查询
//{
//    unsigned char u8OnOff;               /*开启控制*/
//    unsigned char u8DisplayMode;         /*显示模式*/
//    unsigned char u8CHDisplay;           /*显示通道*/
//    unsigned char u8OSDSet;              /*OSD设置*/
//}__attribute__ ((__packed__))MSG_0x0105_S;

/* QT设备检索 */
typedef struct
{
    unsigned char u8Channel;               /*通道选择 0:全通道 */
    unsigned char arru8StartBCDTime[6];    /*开始时间*/
    unsigned char arru8EndBCDTime[6];      /*结束时间*/
}__attribute__ ((__packed__))MSG_0x0106_S;


/*QT设备检索应答*/
typedef struct
{
    unsigned short u16TotalResources;         /*资源总数*/
    unsigned char data[10];
    /*....*/
}__attribute__ ((__packed__))MSG_0x0160_S;


typedef struct
{
    unsigned short u16TotalResources;         /*资源总数*/
    unsigned short u16curResourcesSize;       /*本包资源数*/
    unsigned short u16starSerNum;             /*本包起始资源序号*/
}__attribute__ ((__packed__))MSG_Rec_ACK_S;



/*QT设备回放*/
typedef struct
{
    unsigned char u8Channel;               /*通道选择 0:全通道 */
    unsigned char u8DisplayMode;           /*显示模式 */
    unsigned char u8DisplayCHannel;        /*显示通道*/
    unsigned char arru8StartBCDTime[6];    /*开始时间*/
    unsigned char arru8EndBCDTime[6];      /*结束时间*/
}__attribute__ ((__packed__))MSG_0x0107_S;

/* QT文件导出 */
typedef struct
{
    unsigned char u8Channel;               /*通道选择 0:全通道 */
    unsigned char arru8StartBCDTime[6];    /*开始时间*/
    unsigned char arru8EndBCDTime[6];      /*结束时间*/
}__attribute__ ((__packed__))MSG_0x0108_S;


/*QT文件导出应答*/
typedef struct
{
    unsigned char u8FileExportACK;        /**导出结果**/
}__attribute__ ((__packed__))MSG_0x0180_S;



typedef struct
{   unsigned char u8CtrType;
    unsigned char u8Displaymode;
    unsigned char u8DisplayCH;
}__attribute__ ((__packed__))MSG_0x109_S;

typedef enum
{
    KEY_TYPE_NONE = 0X00,
    KEY_TYPE_SHORT_DOWN = 0X01,
    KEY_TYPE_SHORT_UP = 0X02,
    KEY_TYPE_LONG_DOWN = 0X03,
    KEY_TYPE_LONG_UP  = 0X04,
}HIS_KEY_TYPE_E;

typedef enum
{
    KEY_NONE = 0X00,
    KEY_DISPLAY = 0X01,
    KEY_EXIT = 0X02,
}HIS_KEY_VALE_E;

typedef struct
{   unsigned char u8KeyType;
    unsigned char u8KeyValue;
}__attribute__ ((__packed__))MSG_0x01a0_S;


/*QT触发设置*/
typedef struct
{
    unsigned char u8ParaRequery;          /*使能*/
}__attribute__ ((__packed__))MSG_0x010b_S;


typedef struct
{
    unsigned char arru8OnOff;          /*使能*/
    unsigned char arru8Delay[8];       /*延时设置*/
    unsigned char arru8AuxiliaryLine;  /*辅助线*/
    unsigned char arru8PreviewCH[8];   /*预览通道*/
    unsigned char arru8Temp[128];       /*预留备用*/
}__attribute__ ((__packed__))MSG_0x01b0_S;


typedef struct
{
    unsigned char arru8OnOff;          /*使能*/
    unsigned char arru8Delay[8];       /*延时设置*/
    unsigned char arru8AuxiliaryLine;  /*辅助线*/
    unsigned char arru8PreviewCH[8];   /*预览通道*/
    unsigned char arru8Temp[128];       /*预留备用*/
}__attribute__ ((__packed__))MSG_0x01b1_S;


/**录像参数查询**/
typedef struct
{
    unsigned char u8RecordParaRequery;   /*0：无效；非0：录像参数查询*/
}__attribute__ ((__packed__))MSG_0x010c_S;

/**录像参数查询**/
typedef struct
{
    unsigned char u8RecordOnOff;        /*0~7bit对应位为1 使能录像，为0非使能*/
    unsigned char arru8Resolution[8];   /*(0:QCIF;1:CIF;2:WCIF;3:D1;4:WD1;5:720P;6:1080P)*/
    unsigned char arru8FPS[8];          /*8~25之间,默认为15帧*/
    unsigned char arru8Quality[8];      /*0~5等级，0：最高，默认为0*/
    unsigned char u8OSDEnable;          /*0~7bit对应位为1 使能OSD，为0非使能*/
    unsigned char arru8ChName[8][16];   /*通道名称*/
}__attribute__ ((__packed__))MSG_0x01c0_S;

/**录像参数设置**/
typedef struct
{
    unsigned char u8RecordOnOff;        /*0~7bit对应位为1 使能录像，为0非使能*/
    unsigned char arru8Resolution[8];   /*(0:QCIF;1:CIF;2:WCIF;3:D1;4:WD1;5:720P;6:1080P)*/
    unsigned char arru8FPS[8];          /*8~25之间,默认为15帧*/
    unsigned char arru8Quality[8];      /*0~5等级，0：最高，默认为0*/
    unsigned char u8OSDEnable;          /*0~7bit对应位为1 使能OSD，为0非使能*/
    unsigned char arru8ChName[8][16];   /*通道名称*/
}__attribute__ ((__packed__))MSG_0x01c1_S;

/**QT设备检索应答中的一条信息**/
typedef struct
{
    unsigned short u8Channel;
    unsigned int arru32SysStartTimer;
    unsigned int arru32SysEndTimer;
}__attribute__ ((__packed__))ONE_REACH_ACK_S;

/**QT设备检索应答全部信息**/
typedef struct
{
    unsigned short u16TotalResources;         /*资源总数*/
    unsigned short u16CurResources;
    unsigned short u16StartSerNum;
    ONE_REACH_ACK_S data[1000];
}__attribute__ ((__packed__))IPCP_REACH_ACK_S;



#ifdef __cplusplus
}
#endif

#endif
