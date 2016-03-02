#pragma once
#include "Logger.h"

namespace Log
{
	class CLoggable
	{
	public:
		virtual void AddString(LPCTSTR) = 0;
	};
	class COutputLogger :
		public CLogger
	{
		CLoggable * _log;
	public:
		virtual void Write(LPCTSTR msg);
		virtual void Close();
		void Flush(void) const;
		COutputLogger(CLoggable* log);
		virtual ~COutputLogger(void);
	};

}
