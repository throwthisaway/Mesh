#ifndef LOGGER_H_
#define LOGGER_H_
#include <tchar.h>

namespace Log
{

class CLogger
{
public:
	CLogger();
	virtual void Write(const TCHAR* buf) = 0;
	virtual void Close(void) = 0;
	virtual void Flush(void) const = 0;
	virtual ~CLogger();
};

}

#endif /*LOGGER_H_*/
