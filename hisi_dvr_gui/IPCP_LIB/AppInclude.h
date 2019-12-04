#ifndef __APPINCLUDE_H__
#define __APPINCLUDE_H__
//#include "HstComFunLib.h"

const  unsigned int InitChar=(0x00);
const  int WISDOM_TRUE=0;
const  int WISDOM_FALSE= (-1);

typedef struct hst_THREAD_STATUS_S
{
    const char *ThreadName;  
    pthread_t pid;
    bool bThreadStart;
}THREAD_STATUS_S;


typedef THREAD_STATUS_S THREAD_STATUS; 	/*rename*/

typedef struct hst_THREAD_PARA_T
{
    void  *ClassPara;
    void  *ThreadPar;
}THREAD_PARA_S;
typedef THREAD_PARA_S ThreadPara;  			/*rename*/

typedef struct
{
	char year;
	char month;
	char date;
	char week;
	char hour;
	char minute;
	char second;
}SYSTEM_TIME_SET_S;


#endif

