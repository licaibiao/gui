#ifndef __GPS_DEBUG_H__
#define __GPS_DEBUG_H__

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <sys/statfs.h> 

#define MAX_LOGFILE_SIZE 50000
#define LOGFLAG 0

enum UserDefineLogNum
{
    GENERAL_LOG_NUM=1,
    DAEMON_LOG_NUM,
    ROUTER_LOG_NUM,
    MODEM_LOG_NUM,
    PERIPHER_LOG_NUM,
    GPSSERVICE_LOG_NUM,
    CAPTURE_LOG_NUM,
    OTHER_LOG_NUM,
};

#if LOGFLAG > 0
#define TRACE_TRACE_GENERAL     Trace(GENERAL_LOG_NUM, __LINE__) 
#define TRACE_DAEMON            Trace(DAEMON_LOG_NUM,  __LINE__)  
#define TRACE_ROUTER            Trace(ROUTER_LOG_NUM, __LINE__)           
#define TRACE_MODEM             Trace(MODEM_LOG_NUM, __LINE__)           
#define TRACE_PERIPHER          Trace(PERIPHER_LOG_NUM, __LINE__)         
#define TRACE_GPSSERVICE        Trace(GPSSERVICE_LOG_NUM, __LINE__)            
#define TRACE_CAPTURE           Trace(CAPTURE_LOG_NUM, __LINE__)     
#define TRACE_OTHER             Trace(OTHER_LOG_NUM, __LINE__)      
#else
#define TRACE printf
#define TRACE_GENERAL printf
#define TRACE_DAEMON printf
#define TRACE_GPS printf
#define TRACE_CAPTURE printf
#define TRACE_PPP printf
#define TRACE_COMM printf
#define TRACE_UPDATE printf
#define TRACE_OTHER  printf

#endif
#define SYSLOGDIR "/mnt/log/"
#define SYSLOGDISK "/mnt/"
#define MIX_AVAILABLE_DISK  (1024+512)
#define TRACE_HST (printf("%s(%d)-<%s>: ",__FILE__, __LINE__, __FUNCTION__), printf)
class Trace
{
public:
	Trace(UserDefineLogNum LogNum, int nLineNo ):defineLogNum(LogNum),m_nLineNo(nLineNo)
	{
    }
	inline void operator()(const char *pszFmt, ...) const
	{
		va_list ptr;
		va_start(ptr, pszFmt);
		TraceV(m_nLineNo,pszFmt,ptr);
		va_end(ptr);
	}

private:
    unsigned int availableDisk(const char * pcDir) const
    {
    	if(NULL == pcDir)
    		return -1;
    	struct statfs diskInfo;
    	unsigned int blocksize = 0;       //每个block里包含的字节数  
    	unsigned int availableDisk  = 0;  //可用空间大小
    	int iRet = -1;
    	iRet = statfs(pcDir, &diskInfo);
    	if(0 == iRet)
    	{
    		blocksize = diskInfo.f_bsize; 
    		availableDisk = diskInfo.f_bavail * blocksize/1024;//(K)
    	}
    	return availableDisk;
    }
    void TraceV(int nLine,const char *pszFmt, va_list args) const
    {
        time_t tNow;
        time(&tNow);
        struct tm *time;
        time=localtime(&tNow);
        char log_path[64] ={0};
        char log_dir[64] ={0};
        if(time->tm_year-100 < 16 && time->tm_year-100 > 30)
        {
            time->tm_year =116;
            time->tm_mon =0;
        }
        if(MIX_AVAILABLE_DISK>=availableDisk(SYSLOGDISK))
        {
            system("rm -rf /mnt/log/*"); //统一删除不做覆盖
            return;
        }
        sprintf(log_dir,"%sl-%02d%02d%02d",SYSLOGDIR,(time->tm_year-100),(1+time->tm_mon),(time->tm_mday));
        if ( 0 != access(log_dir, F_OK) )
        {
            if( mkdir(log_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==0 )
            {
                printf("[%s:%d] create dir %s ok!\n", __func__, __LINE__,log_dir);
            }
            else
            {
                printf("[%s:%d] create dir %s error!\n", __func__, __LINE__,log_dir);
                perror("mkdir");
        		return;
            }
        }
        sprintf(log_path,"%s/1000000%d.log",log_dir,defineLogNum);
        FILE* pFile=fopen(log_path,"a+");
        if (pFile==NULL)
        {
            vprintf(pszFmt,args);
            return;
        }
        fprintf(pFile,"%02d-%02d %02d:%02d:%02d :  ",time->tm_mon+1,time->tm_mday,time->tm_hour,time->tm_min,time->tm_sec);
        vfprintf(pFile,pszFmt,args);
        fclose(pFile);
        return;
    }
private:
    UserDefineLogNum defineLogNum;
    const int m_nLineNo;
};

#endif


