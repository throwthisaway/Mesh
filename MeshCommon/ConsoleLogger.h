#ifndef CONSOLELOGGER_H_
#define CONSOLELOGGER_H_
#include "Logger.h"

namespace Log
{

class CConsoleLogger : CLogger
{
public:
	CConsoleLogger();
	virtual ~CConsoleLogger();
};

}

#endif /*CONSOLELOGGER_H_*/
