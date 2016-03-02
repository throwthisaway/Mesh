#ifndef __TIMER_H__
#define __TIMER_H__
#include <GL/glut.h>

class CHighPerfTimer
{
public:
	typedef long long interval_type;
	CHighPerfTimer()
	{
		_frequency();
		start();
	}
	~CHighPerfTimer() {}

	inline void start()
	{
		//TODO::
		//::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_start));
	}
	inline void stop()
	{
		//TODO::
		//::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_end));
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
protected:
	interval_type m_start,m_end;
	inline static interval_type _query_frequency()
	{
		interval_type frequency = 1;
		//TODO::
		//if (!::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency)))
		//	frequency=(interval_type)9223372036854775807;
		return frequency;
	}
	inline static interval_type _frequency()
	{
		static interval_type s_frequency=_query_frequency();
		return s_frequency;
	}
};

class CGLUTTimer
{
	int m_start, m_end;
public:
	CGLUTTimer()
	{
		start();
	}
	inline void start()
	{
		m_start = glutGet(GLUT_ELAPSED_TIME);
	}
	inline void stop()
	{
		m_end = glutGet(GLUT_ELAPSED_TIME);
	}
	inline int get_miliseconds()
	{
		return m_end - m_start;
	}
	inline int get_microseconds()
	{
		return (m_end - m_start)*1000;
	}
};
#endif