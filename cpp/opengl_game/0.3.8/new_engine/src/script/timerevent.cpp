#include "StdAfx.h"

#include "script.h"

#include "luathread.h"
#include "timerevent.h"

//////////////////////////////////////////////////////////////////////////

extern DWORD internal_time;

//////////////////////////////////////////////////////////////////////////

list <TimerEvent*> timerEvents;





// События в списке всегда отсортированы так, что в начале находятся 
// те, что наступят раньше всего. Таким образом, достаточно смотреть только 
// первое событие, если оно не наступило, то другие тоже еще не наступили.



// Условие, которое используется при сортировке эвентов
bool TimeGreater(TimerEvent* te1 , TimerEvent* te2)
{
	return te1->execTime > te2->execTime;
}

// Добавление события в массив и сортировка массива
void AddTimerEvent(TimerEvent* te)
{
	// Ищем событие, которое наступит позже, чем добавляемое.
	for(list<TimerEvent*>::iterator it = timerEvents.begin();
		it != timerEvents.end();
		it++)
	{
		if (te->execTime < (*it)->execTime)
		{
			// Вставляем перед найденным событием
			timerEvents.insert(it, te);
			return;
		}
	}
	// Событие произойдет позже всех, добавляем в конец
	timerEvents.push_back(te);	
}

// Добавление периодического события
// dt - время, черз которое событие сработает
// action - номер скрипта в реестре
// period - период повторения после первого срабатываения
// maxCalls - количество срабатываний
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

// Добавление события
// dt - время, черз которое событие сработает
// action - номер скрипта в реестре
UINT AddTimerEvent(UINT dt, int action)
{
	TimerEvent* te = new TimerEvent();

	te->action = action;
	te->execTime = internal_time + dt;

	AddTimerEvent(te);

	return 1;
}

// Добавлеят событие восстановления приостановленного потока
void AddThreadTimerEvent(UINT dt, int action)
{
	TimerEvent* te = new TimerEvent();
	te->action = action;
	te->execTime = internal_time + dt;
	te->SetThreadResume();

	AddTimerEvent(te);
}

// Удаляет все события из массива
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


// Обработка событий таймера
// Вызывается минимум через каждые TIMEREVENTTICK милисекунд
void ProcessTimerEvents()
{	
	TimerEvent* first;

	while (!timerEvents.empty())
	{
		first = *(timerEvents.begin());	// Указатель на последнее событие в массиве

		if (first->execTime > internal_time)	
			break;							// Время еще не пришло

		first->ExecEvent();
	
		if (first->IsPeriodic())
		{
			first->calls++;
			if (first->calls >= first->maxCalls - 1)			
			{
				// Предпоследний вызов периодического события
				// В следующий раз оно уже вызовется как обыкновенное и исчезнет
				first->ClearPeriodic();
			}

			first->execTime = internal_time + first->period;
			// Удаляем периодическое событие и опять добавляем его в массив
			timerEvents.pop_front();				
			AddTimerEvent(first);

		}
		else
		{
			// Удаляем отработавшее событие
			timerEvents.pop_front();
			DELETESINGLE(first);
		}
	}

}


// Исполняет настпуившее событие.
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