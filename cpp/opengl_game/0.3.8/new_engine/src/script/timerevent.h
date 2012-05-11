#ifndef __TIMEEVENT_H_
#define __TIMEEVENT_H_

class TimerEvent
{
public:
	UINT id;					// Возможно будет использоваться для управления из скрипта.

	//char* action;				// Строка команд
	int action;					// Ссылка на реестр луа

	UINT execTime;				// Время наступления события

	UINT period;				// Период периодического события
	UINT maxCalls;				// Количество срабатыванй 

	UINT calls;					// Текущее количество срабатываний

	UINT flags;	
	// 1 - threadResume				данное событие - запуск приостановленной coroutine
	// 2 - periodic					событие переодическое

	__INLINE BYTE IsThreadResume()		{ return flags & 1; }
	__INLINE void SetThreadResume()		{ flags |= 1; }
	__INLINE void ClearThreadResume()	{ flags &= ~1; }

	__INLINE BYTE IsPeriodic()		{ return flags & 2; }
	__INLINE void SetPeriodic()		{ flags |= 2; }
	__INLINE void ClearPeriodic()	{ flags &= ~2; }

	void ExecEvent();

	TimerEvent()
	{
		id = 0;
		action = 0;
		execTime = 0;
		flags = 0;
		period = 0;
		maxCalls = 0;
		calls = 0;
	}

	~TimerEvent()
	{
		//DELETEARRAY(action);
		SCRIPT::RemoveFromRegistry(action);
	}

};


UINT AddTimerEvent(UINT dt, int action, UINT period, UINT maxCalls);
UINT AddTimerEvent(UINT dt, int action);
void AddThreadTimerEvent(UINT dt, int action);
void DeleteAllEvents();

void ProcessTimerEvents();

#define TIMEREVENTTICK 10


#endif // __TIMEEVENT_H_