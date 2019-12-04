#ifndef __SYSINCLUDE_H__
#define __SYSINCLUDE_H__

#include <unistd.h> 
#include <dirent.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <dlfcn.h> 
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "sys/ioctl.h"
#include <sys/types.h>
#include <sys/io.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/sem.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <bitset>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <string>

#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>  
#include <sys/ipc.h>  

using namespace std;

#include "SysDefine.h"
#include "SysDebug.h"
#include "AppDefine.h"
#include "AppInclude.h"


typedef unsigned char byte;
typedef signed char	INT8S;
typedef signed int	INT32S;
typedef unsigned char    BYTE;
typedef unsigned short   WORD;
typedef unsigned int     DWORD;
typedef unsigned char    INT8U;
typedef unsigned short	 INT16U;
typedef unsigned int     INT32U;
#define U8 unsigned char
#define U16 unsigned short
#define U32 unsigned int

#define PID_T           pid_t
#define SOCKET          int
#define PACKED __attribute__ ((__packed__));//按照实际占用字节数进行对齐

#endif

