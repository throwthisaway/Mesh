#pragma once
#include <stdio.h>
#include "FileWriter.h"
#include "Logger.h"

namespace Log {

class CFileLogger : public CLogger {
	IO::CFileWriter file;
public:	
	CFileLogger(_LPCTSTR fname, long mode = FILE_REWRITE);
	virtual void Write(_LPCTSTR);
	virtual void Close();
};

}
