#ifndef __SYSTEM_DEFINE__
#define __SYSTEM_DEFINE__

const unsigned int  SYS__gettid=224;
const unsigned int SYS__readahead=225;

#define hst_sleep(seconds)	usleep(seconds*1000*1000)
#define hst_msleep(milliseconds) usleep(milliseconds*1000)

#define STRUCTOFFSET( struc, e ) (size_t)&(((struc*)0)->e)
#define STRUCTSIZEOF(struc, e) sizeof(((struc *)0)->e) 
#define ARRAY_SIZE(_A) (sizeof(_A) / sizeof((_A)[0]))
#define ASSERT(exp) \
		{if (!(exp)) printf("[WSD]Assert failed.file=%s,line=%d,exp=%s err!!!\n",__FILE__,__LINE__,#exp);}

#define HST_PRINTF(fmt...)   \
    do {\
        printf("[%s:%d]: ", __FUNCTION__, __LINE__);\
        printf(fmt);\
       }while(0)

#endif

