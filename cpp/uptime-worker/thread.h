#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED
#ifdef WIN32
#include <windows.h>
#endif
typedef unsigned long DWORD;
typedef DWORD* LPDWORD;
typedef void* LPVOID;
typedef void* HANDLE;
typedef DWORD (*LPTHREAD_METHOD)(LPVOID pParam);

//typedef SECURITY_ATTRIBUTES* LPSECURITY_ATTRIBUTES;
#ifndef WIN32
#include "pthread.h"
#define LPSECURITY_ATTRIBUTES pthread_attr_t*
#endif



// ����������� �������, ������� �������� �����.
//static  DWORD StartFunc (LPSTARTUP_PARAM pStartup);

// ��������� ���������� ��� ����������� �������.
typedef struct STARTUP_PARAM
{
    LPTHREAD_METHOD    pMethod;
    LPVOID            pParam;
} *LPSTARTUP_PARAM;

DWORD StartFunc(LPSTARTUP_PARAM pStartup);
HANDLE StartThread(LPTHREAD_METHOD pMethod, LPVOID pParam=0, LPDWORD pdwThreadID=0, const LPSECURITY_ATTRIBUTES pSecurity=0, DWORD dwStackSize=0, DWORD dwFlags=0);




#endif // THREAD_H_INCLUDED
