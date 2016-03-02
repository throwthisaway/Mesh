#ifndef FILELOGGER_H_
#define FILELOGGER_H_
#include <stdio.h>
#include "FileWriter.h"
#include "Logger.h"
using namespace IO;

namespace Log
{

class CFileLogger : public CLogger
{	
	long mode;
	const char * fname;
	CFileWriter *file;
public:	
	CFileLogger(LPCTSTR fname, long mode = FILE_REWRITE);
	void SetFileName(LPCTSTR);
	void SetFileMode(long);
	virtual void Write(LPCTSTR);
	virtual void Close();
	void Flush(void) const;
	virtual ~CFileLogger();
};

}

#endif /*FILELOGGER_H_*/
