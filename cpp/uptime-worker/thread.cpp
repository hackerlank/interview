#include "thread.h"

//typedef DWORD (*LPTHREAD_METHOD)(LPVOID pParam);

// Статическая функция, которая запустит метод.
//static  DWORD StartFunc (LPSTARTUP_PARAM pStartup);

// Структура параметров для статической функции.



DWORD StartFunc(LPSTARTUP_PARAM pStartup)
{
    // Распаковываем данные в новом потоке.
    LPTHREAD_METHOD pMethod = pStartup->pMethod;
    LPVOID pParam            = pStartup->pParam;

#if(MSV_PROC_STAT)
_msvps_thread_cnt++;
_msvps_thread_is++;
#endif
    // Запускаем метод класса в новом потоке.
    DWORD dwResult = (*pMethod)(pParam);
#if(MSV_PROC_STAT)
_msvps_thread_is--;
#endif

    // Удаляем временные данные и возвращаем код возврата из нового потока.
    delete pStartup;
    return dwResult;
}


HANDLE StartThread(LPTHREAD_METHOD pMethod, LPVOID pParam, LPDWORD pdwThreadID, const LPSECURITY_ATTRIBUTES pSecurity, DWORD dwStackSize, DWORD dwFlags){
    // Создаем структуру и упаковываем данные для статической функции.
    LPSTARTUP_PARAM pStartup = new STARTUP_PARAM;
    pStartup->pMethod    = pMethod;
    pStartup->pParam    = pParam;

    // Создаем новый поток.
#ifdef WIN32
    return CreateThread(pSecurity, dwStackSize, (LPTHREAD_START_ROUTINE)StartFunc, pStartup, dwFlags, pdwThreadID);
#else
pthread_t restrict;
if(pthread_create(&restrict, pSecurity, (void* (*)(void*))StartFunc, pStartup)) return pdwThreadID;
return 0;
#endif
}
