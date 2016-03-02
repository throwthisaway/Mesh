#ifndef __TIMER_H__
#define __TIMER_H__

#include <windows.h>

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
class CMMTimer
{
public:
	typedef DWORD interval_type;
	CMMTimer()
	{
		start();
	}
	~CMMTimer() {}

	inline void start()
	{
		m_start=timeGetTime();
	}
	inline void stop()
	{
		m_end=timeGetTime();
	}
	inline interval_type get_seconds()
	{
		return get_period()/(interval_type)1000;
	}
	inline interval_type get_miliseconds()
	{
		return get_period();
	}
	inline interval_type get_microseconds()
	{
		return get_period()*(interval_type)1000;
	}
protected:
	interval_type m_start,m_end;
	inline interval_type get_period()
	{
		return m_end-m_start;
	}
};
class CHighPerfTimer
{
public:
	typedef LONGLONG interval_type;
	CHighPerfTimer()
	{
		_frequency();
		start();
	}
	~CHighPerfTimer() {}

	inline void start()
	{
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_start));
	}
	inline void stop()
	{
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_end));
	}
	inline interval_type get_period()
	{
		return m_end-m_start;
	}
	inline interval_type get_seconds()
	{
		return get_period()/_frequency();	
	}
	inline interval_type get_miliseconds()
	{
		interval_type count=get_period();
		return (count<9223372036854775) ? count*(interval_type)1000/_frequency():count/_frequency()*(interval_type)1000;
	}
	inline interval_type get_microseconds()
	{
		interval_type count=get_period();
		return (count<9223372036854) ? count*(interval_type)1000000/_frequency():count/_frequency()*(interval_type)1000000;
	}
	inline static interval_type _frequency()
	{
		return _query_frequency();
	}
protected:
	interval_type m_start,m_end;
	inline static interval_type _query_frequency()
	{
		interval_type frequency;
		if (!::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency)))
			frequency=(interval_type)9223372036854775807;
		return frequency;
	}
};
class CTickTimer
{
public:
	typedef DWORD interval_type;
	CTickTimer()
	{
		start();
	}
	~CTickTimer() {}

	inline void start()
	{
		m_start=GetTickCount();
	}
	inline void stop()
	{
		m_end=GetTickCount();
	}
	inline interval_type get_seconds()
	{
		return get_period()/(interval_type)1000;
	}
	inline interval_type get_miliseconds()
	{
		return get_period();
	}
	inline interval_type get_microseconds()
	{
		return get_period()*(interval_type)1000;
	}
protected:
	interval_type m_start,m_end;
	inline interval_type get_period()
	{
		return m_end-m_start;
	}
};
#endif