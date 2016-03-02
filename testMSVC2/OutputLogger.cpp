#include "StdAfx.h"
#include "OutputLogger.h"
namespace Log
{

COutputLogger::COutputLogger(CLoggable* log)
{
	_log = log;
}

COutputLogger::~COutputLogger(void)
{
}

void COutputLogger::Write(LPCTSTR msg)
{
	_log->AddString(msg);	
}

void COutputLogger::Close()
{
	
}

void COutputLogger::Flush(void) const
{
}

}
