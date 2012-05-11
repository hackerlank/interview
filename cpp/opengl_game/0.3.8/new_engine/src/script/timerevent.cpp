#include "StdAfx.h"

#include "script.h"

#include "luathread.h"
#include "timerevent.h"

//////////////////////////////////////////////////////////////////////////

extern DWORD internal_time;

//////////////////////////////////////////////////////////////////////////

list <TimerEvent*> timerEvents;





// ������� � ������ ������ ������������� ���, ��� � ������ ��������� 
// ��, ��� �������� ������ �����. ����� �������, ���������� �������� ������ 
// ������ �������, ���� ��� �� ���������, �� ������ ���� ��� �� ���������.



// �������, ������� ������������ ��� ���������� �������
bool TimeGreater(TimerEvent* te1 , TimerEvent* te2)
{
	return te1->execTime > te2->execTime;
}

// ���������� ������� � ������ � ���������� �������
void AddTimerEvent(TimerEvent* te)
{
	// ���� �������, ������� �������� �����, ��� �����������.
	for(list<TimerEvent*>::iterator it = timerEvents.begin();
		it != timerEvents.end();
		it++)
	{
		if (te->execTime < (*it)->execTime)
		{
			// ��������� ����� ��������� ��������
			timerEvents.insert(it, te);
			return;
		}
	}
	// ������� ���������� ����� ����, ��������� � �����
	timerEvents.push_back(te);	
}

// ���������� �������������� �������
// dt - �����, ���� ������� ������� ���������
// action - ����� ������� � �������
// period - ������ ���������� ����� ������� �������������
// maxCalls - ���������� ������������
UINT AddTimerEvent(UINT dt, int action, UINT period, UINT maxCalls)
{
	TimerEvent* te = new TimerEvent();
	
	te->action = action;
	te->execTime = internal_time + dt;	

	te->SetPeriodic();
	te->period = period;
	te->maxCalls = maxCalls;			

	AddTimerEvent(te);

	return 1;
}

// ���������� �������
// dt - �����, ���� ������� ������� ���������
// action - ����� ������� � �������
UINT AddTimerEvent(UINT dt, int action)
{
	TimerEvent* te = new TimerEvent();

	te->action = action;
	te->execTime = internal_time + dt;

	AddTimerEvent(te);

	return 1;
}

// ��������� ������� �������������� ����������������� ������
void AddThreadTimerEvent(UINT dt, int action)
{
	TimerEvent* te = new TimerEvent();
	te->action = action;
	te->execTime = internal_time + dt;
	te->SetThreadResume();

	AddTimerEvent(te);
}

// ������� ��� ������� �� �������
void DeleteAllEvents()
{
	for(list <TimerEvent*>::iterator it = timerEvents.begin();
		it != timerEvents.end();
		it++)
	{
		DELETESINGLE(*it);
	}

	timerEvents.clear();
}


// ��������� ������� �������
// ���������� ������� ����� ������ TIMEREVENTTICK ����������
void ProcessTimerEvents()
{	
	TimerEvent* first;

	while (!timerEvents.empty())
	{
		first = *(timerEvents.begin());	// ��������� �� ��������� ������� � �������

		if (first->execTime > internal_time)	
			break;							// ����� ��� �� ������

		first->ExecEvent();
	
		if (first->IsPeriodic())
		{
			first->calls++;
			if (first->calls >= first->maxCalls - 1)			
			{
				// ������������� ����� �������������� �������
				// � ��������� ��� ��� ��� ��������� ��� ������������ � ��������
				first->ClearPeriodic();
			}

			first->execTime = internal_time + first->period;
			// ������� ������������� ������� � ����� ��������� ��� � ������
			timerEvents.pop_front();				
			AddTimerEvent(first);

		}
		else
		{
			// ������� ������������ �������
			timerEvents.pop_front();
			DELETESINGLE(first);
		}
	}

}


// ��������� ����������� �������.
void TimerEvent::ExecEvent()
{	
	if (this->IsThreadResume())
	{
		ProcessThread(this->action);
	}
	else
	{
		SCRIPT::ExecChunkFromReg(this->action);
	}
	
}