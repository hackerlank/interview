#ifndef __TIMEEVENT_H_
#define __TIMEEVENT_H_

class TimerEvent
{
public:
	UINT id;					// �������� ����� �������������� ��� ���������� �� �������.

	//char* action;				// ������ ������
	int action;					// ������ �� ������ ���

	UINT execTime;				// ����� ����������� �������

	UINT period;				// ������ �������������� �������
	UINT maxCalls;				// ���������� ����������� 

	UINT calls;					// ������� ���������� ������������

	UINT flags;	
	// 1 - threadResume				������ ������� - ������ ���������������� coroutine
	// 2 - periodic					������� �������������

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