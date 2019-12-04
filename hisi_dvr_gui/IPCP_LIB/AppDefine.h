#ifndef __APP_DEFINE__
#define __APP_DEFINE__

#define CONS_SYSTEM_NAME	  "wsd"		        //系统名称wisdom 
#define CONS_DAEMON_KEY       "daemon"
#define CONS_MODEM_KEY        "modem"
#define CONS_ROUTER_KEY	      "router"
#define CONS_TAXISEVICE_KEY   "taxisevice"
#define CONS_PERIPHER_KEY     "peripher"
#define CONS_CAPTURE_KEY      "capture"
#define CONS_OTHER_KEY        "other"
#define CONS_TRANSPORT_KEY    "transport"
#define CONS_MAINTAIN_KEY     "maintain"

const unsigned int CONS_SYSCMDBUF_MAX_LEN					=128;
const unsigned int CONS_DEFAULT_HEART_BEAT_INTER_S         	=10;		//Default进程心跳间隔
const unsigned int CONS_PPP_HEART_BEAT_INTER_S             	=60;		//PPP进程心跳间隔
const unsigned int CONS_FILE_NAME_MAX_LEN	    			=64;	                    //文件名最大字符数
const unsigned int CONS_PATH_FILE_MAX_LEN	    			=128;	                    //路径文件最大字符数
const unsigned int CONS_SERVER_NAME_MAX_LEN					=24;	                //服务进程名最大字符数

/*用于比较系统时间*/
const unsigned int  THE_COMPILE_TIME	=(1523721600);//2018/4/15 00:00:00
const unsigned int  THE_DIFF_TIME		=(1681488000);//2023/4/15 00:00:00

#endif

