#ifndef _HstlibIpcpCommon_H
#define _HstlibIpcpCommon_H

#include "SysInclude.h"
#ifdef __cplusplus
extern "C" {
#endif

const unsigned char WSD_MAX_ADDR_NUM 			=(16);      //定定最大16个地址
const unsigned char PALATFOM_MAX_NUM      		=(8);       //连接平台个数
const unsigned char GPS_PLATFORM_ADDR     		=(0xa1); 
const unsigned char WSD_DAEMON_ADDR      	 	=(0xa2);
const unsigned char WSD_ROUTER_ADDR	    	 	=(0xa3);
const unsigned char WSD_GPSSERVICE_ADDR   		=(0xa4); 
const unsigned char WSD_MODEM_ADDR        		=(0xa5); 
const unsigned char WSD_PERIPHER_ADDR     		=(0xa6); 
const unsigned char WSD_CAPTURE_ADDR      		=(0xa7); 
const unsigned char WSD_OTHER_ADDR        		=(0xa8); 
const unsigned char WSD_GUI_ADDR        		=(0xa9);

const unsigned char WSD_MCU_ADDR          		=(0xb1); 
const unsigned char WSD_LCD_ADDR          		=(0xb2); 

const unsigned char GPS_PLATFORM2_ADDR   		=(0xa9);  
const unsigned char GPS_PLATFORM_UDP_ADDR    	=(0xaa);  

const unsigned char LBS_PLATFORM_ADDR     		=(0xab); 
const unsigned char WSD_LBSSERVICE_ADDR   		=(0xac); 

const unsigned char LBS_GAODEMAP_ADDR     		=(0xae); 


const unsigned char WSD_TRANSPORT_ADDR   		=(0xfd); 
const unsigned char WSD_MAINTAIN_ADDR   		=(0xfc);

const unsigned char WSD_BROADCAST_ADDR     		=(0xfe);  /*broadcast addr*/


const unsigned char WSD_JJQ_ADDR          =(0xc1); 
const unsigned char WSD_PJQ_ADDR          =(0xc2); 
const unsigned char WSD_LED_ADDR          =(0xc3); 
const unsigned char WSD_TTS_ADDR          =(0xc4); 
const unsigned char WSD_GPS_ADDR          =(0xc5); 

const unsigned short QUEUE_GUI_KEY            	    =(0x1233);
const unsigned short QUEUE_GPSSERVICE_KEY        	=(0x1234); 
const unsigned short QUEUE_ROUTER_KEY				=(0x1235); 
const unsigned short QUEUE_PERIPHER_KEY         	=(0x1236); 
const unsigned short QUEUE_CAPTURE_KEY           	=(0x1237); 
const unsigned short QUEUE_MODEM_KEY             	=(0x1238); 
const unsigned short QUEUE_TOTHER_KEY            	=(0x1239);

const unsigned short QUEUE_PLATFORM_KEY          	=(0x123a); 
const unsigned short QUEUE_DAEMON_KEY 				=(0x123b); 
const unsigned short QUEUE_GPSSERVICE2PF_KEY      	=(0x123c); 
const unsigned short QUEUE_PLATFORM2_KEY         	=(0x123d); 
const unsigned short QUEUE_PLATFORM_UDP_KEY      	=(0x123e); 
const unsigned short QUEUE_MCU2JJQ_KEY           	=(0x123f); 
const unsigned short QUEUE_HISI2MCU_KEY          	=(0x1240); 
const unsigned short QUEUE_PERI_IPC2JJQ_KEY      	=(0x1241); 
const unsigned short QUEUE_HISI2LCD_KEY          	=(0x1242); 
const unsigned short QUEUE_MCU2LED_KEY           	=(0x1243); 
const unsigned short QUEUE_PERI_IPC2LED_KEY      	=(0x1244); 
const unsigned short QUEUE_PERI_IPC2MCU_KEY      	=(0x1245); 
const unsigned short QUEUE_MCU2PJQ_KEY           	=(0x1247); 
const unsigned short QUEUE_PERI_IPC2PJQ_KEY      	=(0x1248); 
const unsigned short QUEUE_GPSSERVICE2PF2_KEY     	=(0x1249); 
const unsigned short QUEUE_PERI_IPC2JJQ2_KEY      	=(0x124a); 
const unsigned short QUEUE_LBS_PLATFORM_KEY       	=(0x124b); 
const unsigned short QUEUE_LBS_GPSSERVER_KEY      	=(0x124c); 
const unsigned short QUEUE_MCU2WISDOMPJQ_KEY      	=(0x124d); 

const unsigned short SLP_GPSSERVICE_KEY	        	=(0x124e);     //进程内发送到业务逻辑线程 也将采用消息队列
const unsigned short SLP_ROUTER_KEY			    	=(0x124f); 
const unsigned short SLP_PERIPHER_KEY	       		=(0x1250); 
const unsigned short SLP_CAPTURE_KEY	            =(0x1251); 
const unsigned short QUEUE_FEEDDOG_KEY		   		=(0x1252); 
const unsigned short SLP_MODEM_KEY	            	=(0x1253); 
const unsigned short SLP_DAEMON_KEY	            	=(0x1254); 
const unsigned short QUEUE_GAODEMAP_KEY 			=(0x1255); 
const unsigned short SLP_TOTHER_KEY	            	=(0x1256); 


const unsigned short QUEUE_TRANSPORT_KEY            =(0x1257);  /*物流业务IPCP       QUEUE      KEY*/
const unsigned short QUEUE_TRANSPORT2PF_KEY         =(0x1259);  /*物流业务与平台1     QUEUE     KEY*/
const unsigned short QUEUE_TRANSPORT2PF2_KEY        =(0x125a);  /*物流业务与平台2     QUEUE     KEY*/



const unsigned short QUEUE_MAINTAIN_KEY             =(0x125b);   /*维护业务IPCP      QUEUE KEY*/
const unsigned short SLP_MAINTAIN_KEY               =(0x125c);   /*维护业务IPCP      SLP   KEY*/



const unsigned short QUEUE_MSG_HEAD              	=(0xa5a5); 
const unsigned short QUEUE_MSG_END               	=(0x5a5a); 

const unsigned int MAX_CMDID_NUM 			   	 =(1024); 
/*通用IPCP指令*/
const unsigned int QUEUE_COMMACK_CMD       		 =(0x00fe);    //消息队列通用应答      (=(0xFE）
const unsigned int QUEUE_RESTART_CMD        	 =(0x00fd);    //重启模块进程          (=(0xFD)
const unsigned int QUEUE_DISABLE_CMD        	 =(0x00fc);    //禁用模块进程          (=(0xFC）
const unsigned int QUEUE_RESET_CMD          	 =(0x00fb);    //初始化模块进程         (=(0xFB）
const unsigned int QUEUE_TRANSFER_CMD       	 =(0x00fa);    //业务数据下行透传        (=(0xFA）
const unsigned int QUEUE_PLAT_MSG_TRAN_CMD		 =(0x10FA);    //业务数据上行透传(=(0x10FA）
const unsigned int QUEUE_OTHERTRANSFER_CMD  	 =(0x00f9);    //其它数据透传          (=(0xF9）
const unsigned int QUEUE_PWCONTROL_CMD      	 =(0x00f8);    //电源控制            (=(0xF8）
const unsigned int QUEUE_COM_SYSTEM_CMD     	 =(0x00f7);    //系统命令调用          (=(0xF7)
const unsigned int QUEUE_COM_SYSRST_CMD     	 =(0x00f6);    //系统命令重启          (=(0xF6)
/*ROUTER模块相关IPCP指令*/
const unsigned int QUEUE_REGPLATFORM_CMD         =(0x00CE);    //注册监控平台
const unsigned int QUEUE_RECONNECT_CMD     		 =(0x00CD);    //重连平台 
const unsigned int QUEUE_DESTORYPLATFORM_CMD     =(0x00CC);    //释放监控平台 
const unsigned int QUEUE_GETSTATUS_CMD           =(0x00CB);    //信息路由模块状态获取
const unsigned int QUEUE_REGPLATFORM_ACK         =(0x00CA);    //注册监控平台回复
const unsigned int QUEUE_DESTORY_ACK             =(0x00C9);    //释放平台应答(=(0xC9)
/*MODEM模块相关IPCP指令*/
const unsigned int QUEUE_DIAILCONTROL_CMD        =(0x00EE);    //拔号控制 （=(0xEE）
const unsigned int QUEUE_CALLOUT_CMD             =(0x00ED);    //呼出语音（=(0xED）
const unsigned int QUEUE_CALLSTA_CMD             =(0x00EC);    //呼出状态返回（=(0xEC）
const unsigned int QUEUE_CALLIN_CMD              =(0x00EB);    //被叫语音（=(0xEB）
const unsigned int QUEUE_CALLIN_ACK              =(0x00EA);    //被叫操作（=(0xEA）
const unsigned int QUEUE_VOLUME_CMD              =(0x00E9);    //调节音量操作（=(0xE9）
const unsigned int QUEUE_DTMF_CMD                =(0x00E8);    //DTMF操作（=(0xE8）
const unsigned int QUEUE_MSMIN_CMD               =(0x00E7);    //MSM IN
const unsigned int QUEUE_MSMSEND_CMD             =(0x00E6);    //MSM SEND
const unsigned int QUEUE_MSMDEL_CMD              =(0x00E5);    //MSM DEL
const unsigned int QUEUE_CALL_CUT                =(0x00E4);    //挂断
const unsigned int QUEUE_MODEM_SOFRST            =(0x00E3);    //软重启
const unsigned int QUEUE_MODEM_HARRST            =(0x00E2);    //硬件重启
const unsigned int QUEUE_MODEM_NET_STATE         =(0x00E1);    //网络状况汇报
const unsigned int MODEM_APN_SET_CMD             =(0x00E0);    //设置MODEM APN
const unsigned int QUEUE_MODEM_TTS_SEND          =(0x00DF);    //通信模块TTS
const unsigned int QUEUE_MODEM_TTS_END           =(0x00DE);    //通信模块停止当前TTS播报
const unsigned int QUEUE_MODEM_TTS_CFG           =(0x00DD);    //配置TTS 音量 速度 音调 风格
const unsigned int QUEUE_MODEM_INFO_REQ          =(0x00DC);    //请求模块信息 运营商 模块版本 ICCID
const unsigned int QUEUE_COMM_CTROL_ATCMD        =(0x00DB);    //AT通用指令 原0x0059



/*视频模块相关IPCP指令*/
const unsigned int QUEUE_MEDIA_SEARCH_CMD        		=(0x0050);//多媒体检索 								 (原0x0074)
const unsigned int QUEUE_MEDIA_SEARCH_ACK       		=(0x0051);//多媒体检索结果返回							 (原0x0075)
const unsigned int QUEUE_MEDIA_SEARCH_SOLO       		=(0x0052);//单条多媒体检索上传							
const unsigned int QUEUE_MEDIA_FORMAT_DISK       		=(0x0053);/*格式化外部存储器*/							
const unsigned int QUEUE_MEDIA_MEDIA_STAT       		=(0x0054);/*多媒体状态信息*/
const unsigned int QUEUE_MEDIA_ALL_ATTR_CMD        		=(0x0055);//音视频参数设置(JTT1078) 					（0x0055）
const unsigned int QUEUE_MEDIA_CHANNELLIST_CMD        	=(0x0056);//音视频通道列表设置(JTT1078)  				（0x0056）
const unsigned int QUEUE_MEDIA_ONE_ATTR_CMD        		=(0x0057);//单独视频通道参数设置(JTT1078)  				（0x0057）
const unsigned int QUEUE_MEDIA_ALARM_REC_CMD        	=(0x0058);//特殊报警录像参数设置(JTT1078)  				（0x0058）
const unsigned int QUEUE_MEDIA_ALARM_SHIELD_CMD         =(0x0059);//视频相关报警屏蔽字设置(JTT1078)  				（0x0059）
const unsigned int QUEUE_MEDIA_ALARM_ANALY_CMD        	=(0x005a);//图像分析报警参数设置(JTT1078)    				（0x005A）
const unsigned int QUEUE_MEDIA_AWAKEN_CMD        		=(0x005b);//终端休眠唤醒模式设置(JTT1078) 				（0x005B）
const unsigned int QUEUE_VEDIO_PLAY_CMD          		=(0x005c);//多功能屏视频回放							 (0x005c)
const unsigned int QUEUE_VEDIO_PLAY_ACK          		=(0x005d);//多功能屏视频回放结果返回						 (0x005d)
const unsigned int QUEUE_VEDIO_CTRL_CMD          		=(0x005e);//多功能屏视频控制							 (0x005e)
const unsigned int QUEUE_VEDIO_CTRL_ACK          		=(0x005f);//多功能屏视频控制结果返回						 (0x005f)
const unsigned int QUEUE_VEDIO_PREVIEW_CMD      		=(0x0060);//多功能屏实时视频预览							 (0x0060)
const unsigned int QUEUE_VEDIO_PREVIEW_ACK      		=(0x0061);//多功能屏实时视频预览结果返回						 (0x0061)
const unsigned int QUEUE_VEDIO_MARKER_CMD       		=(0x0062);//多功能屏视频标注							 (0x0062)

const unsigned int QUEUE_SERVICE_STATUS_CMD       		=(0x0063);//业务进程状态获取 （0x0063）
const unsigned int QUEUE_SERVICE_STATUS_ACK       		=(0x0064);//业务进程通用状态应答 （0x0064）
const unsigned int QUEUE_DEV_STATUS_CMD       			=(0x0065);//设备状态状态获取 （0x0065）
const unsigned int QUEUE_DEV_STATUS_ACK       			=(0x0066);//设备状态获取应答 （0x0066）
const unsigned int QUEUE_DEV_FORMAT_MSG      			=(0x0067);//通知视频进程将格式化存储设备
const unsigned int QUEUE_MEDIA_MEDIA_ALARM       		=(0x0068);/*多媒体报警信息*/
const unsigned int QUEUE_THRES_ALARM       				=(0x0069);/*音视频阈值报警信息*/
const unsigned int QUEUE_THRES_ALARM_ACK       			=(0x006A);/*音视频阈值报警回复操作*/


/*业务模块相关IPCP指令*/
const unsigned int QUEUE_PHOTO_SNAP_CMD   		 =(0x0070);
const unsigned int QUEUE_PHOTO_SNAP_ACK		  	 =(0x0071);   //回传已拍照的文件id
const unsigned int QUEUE_MEDIA_UPLOAD_ACK		 =(0x0072);   //回传待上传文件的绝对路径
const unsigned int QUEUE_MEDIA_AUDIO_CMD  		 =(0x0073);
const unsigned int QUEUE_BROAD_POSI     		 =(0x0074);   /* 广播位置信息 */
const unsigned int QUEUE_BROAD_SLEEP     		 =(0x0075);   /* 广播休眠唤醒事件下发 */
const unsigned int QUEUE_EXENVENT       		 =(0x0076);   /* 广播外部事件下发 */
const unsigned int QUEUE_EX_PARA       		 	 =(0x0077);   /* 外部参数下发 */
const unsigned int QUEUE_ADAS_PHOTO_SAVE       	 =(0x0078);   /* ADAS报警图片存储上传*/

/*运维模块相关IPCP指令*/
const unsigned int QUEUE_DEV_STAUS_CMD   		 =(0x00A0);   /* 终端状态查询 */
const unsigned int QUEUE_DEV_STAUS_ACK   		 =(0x00A1);   /* 终端状态查询应答 */
const unsigned int QUEUE_CHIP_SERIALNUM_CMD   	 =(0X00A2);   /* MCU芯片序列号查询 */
const unsigned int QUEUE_EMERGENCY_CMD   		 =(0X00A3);   /* 紧急事件上报 */
const unsigned int QUEUE_DEV_CONTROL_CMD   		 =(0x00A4);   /* 终端控制 */  
const unsigned int QUEUE_PARA_QUERY_CMD   		 =(0x00A5);   /* 参数查询 */
const unsigned int QUEUE_MCU_OSPF_CMD   		 =(0x00A6);   /* 透传MCU相关参数设置及查询 */
const unsigned int QUEUE_RECORD_CMD   		     =(0x00A7);   /* 录单设置及查询 */
const unsigned int QUEUE_PARA_SET_CMD   		 =(0x00A8);   /* 参数设置 */
const unsigned int QUEUE_PROFILE_SET_CMD   		 =(0x00A9);   /* 配置文件设置 */
const unsigned int QUEUE_UPDATE_FLAG_CMD   		 =(0x00AA);   /* 升级标志 */
const unsigned int QUEUE_UPLOAD_FILE_CMD   		 =(0x00AB);   /* 上传文件 */
const unsigned int QUEUE_UPLOAD_FILE_ACK   		 =(0x00AC);   /* 上传文件应答 */


/*其它模块IPCP指令*/
const unsigned int QUEUE_FACE_VERIFY_CMD         =(0x0091);   //人脸识别结果


/*QT界面IPCP指令*/
const unsigned int QUEUE_QTGUI_HEARTBEAT_CMD             =(0x0101);   //QT心跳
const unsigned int QUEUE_QTGUI_STATUS_REPORT_CMD         =(0x0110);   //设备状态汇报
const unsigned int QUEUE_QTGUI_PARA_SET_CMD              =(0x0102);   //QT参数设置
const unsigned int QUEUE_QTGUI_PARA_QUERY_CMD            =(0x0103);   //QT参数查询
const unsigned int QUEUE_QTGUI_PARA_QUERY_ACK_CMD        =(0x0130);   //设备参数查询应答
const unsigned int QUEUE_QTGUI_QUERY_CONTROL_CMD         =(0x0104);   //QT设备控制
const unsigned int QUEUE_QTGUI_QUERY_CONTROL_ACK_CMD     =(0x0140);   //QT设备控制应答
const unsigned int QUEUE_QTGUI_QUERY_VIDEO_PRE_CMD       =(0x0105);   //QT视频预览
const unsigned int QUEUE_QTGUI_QUERY_SEARCH_CMD          =(0x0106);   //QT设备检索
const unsigned int QUEUE_QTGUI_QUERY_SEARCH_ACK_CMD      =(0x0160);   //设备检索应答
const unsigned int QUEUE_QTGUI_QUERY_PLAYBACK_CMD        =(0x0107);   //QT设备回放
const unsigned int QUEUE_QTGUI_QUERY_DOWNLOAD_CMD        =(0x0108);   //QT文件导出
const unsigned int QUEUE_QTGUI_QUERY_DOWNLOAD_ACK_CMD    =(0x0180);   //QT文件导出应答
const unsigned int QUEUE_QTGUI_QUERY_PLAYBACK_CTL_CMD    =(0x0109);   //QT设备回放控制
const unsigned int QUEUE_QTGUI_QUERY_KEY_CMD             =(0x01a0);   //QT按键信息
const unsigned int QUEUE_QTGUI_QUERY_TRIGGER_PARA_CMD    =(0x010b);   //QT触发参数查询
const unsigned int QUEUE_QTGUI_TRIGGER_PARA_ACK_CMD      =(0x01b0);   //QT触发参数查询应答
const unsigned int QUEUE_QTGUI_TRIGGER_PARA_SET_CMD      =(0x01b1);   //QT触发参数查询设置
const unsigned int QUEUE_QTGUI_VIDEO_PARA_QUERY_CMD      =(0x010c);   //QT录像参数查询
const unsigned int QUEUE_QTGUI_VIDEO_PARA_QUERY_ACK_CMD  =(0x01c0);   //QT录像参数查询应答
const unsigned int QUEUE_QTGUI_VIDEO_PARA_SET_CMD        =(0x01c1);   //QT录像参数设置

/*对接高德IPCP指令*/
const unsigned int QUEUE_GPSSTATUS_GET_CMD       =(0x0090);   //位置信息获取
const unsigned int QUEUE_GPSSTATUS_ACK_CMD       =(0x0009);   //位置信息获取应答




/************************************************************************************/
/* | 2Byet	| Byet	 | Byet	   | 2Byet  | 2*Byet |2Byet    |N*Byet | Byet | 2Byet  |*/
/* | 0xa5a5 | 源地址 | 目标地址| 流水号 | 消息ID |消息长度 |消息体 | 校验 | 0x5a5a |*/
/* | 0  1   |   2    |    3    |  4  5  |  6  7  | 8  9    |10.... | 10+N | 10+N+1 |*/
/************************************************************************************/

const unsigned int 	MSG_HEAD_LEN 				=(10);
const unsigned int 	MSG_END_LEN  				=(3);
const unsigned int	MSG_ARCH_MSG_LEN  			=(13);

const unsigned int  MSG_SRC_ADDR 				=(2);
const unsigned int  MSG_TAR_ADDR 				=(3);
const unsigned int  MSG_SERIAL_NUM_ADDR 		=(4);
const unsigned int  MSG_CMID_ADDR 				=(6);
const unsigned int  MSG_MSG_LEN_ADDR 			=(8);
const unsigned int 	MSG_ACK_CMID_ADDR 			=(10);
const unsigned int 	MSG_ACK_SERIAL_NUM_ADDR 	=(12);
const unsigned int 	MSG_ACK_RES_ADDR 			=(14);
const unsigned int	MSG_CONTENT_OFFSET 	 		=(10);

struct QueueMsgPack
{
    unsigned char SrcAddr;
    unsigned char TargAddr;
    unsigned short Len;
    unsigned char *Data;
    unsigned short CmdId;
};

const unsigned int MAXSIZE =(16*4096);

struct ARCH_MSG_S{
    long int MsgType;
    unsigned char SomeText[MAXSIZE];
};

typedef ARCH_MSG_S  _Msg_St;

#ifdef __cplusplus
}
#endif

/**********************************************/

#endif
