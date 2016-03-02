#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include "FileLogger.h"
#include <string.h>

namespace Log
{
	
CFileLogger::CFileLogger(LPCTSTR fname, long mode)
{
	try
	{
		file = CFileWriter::Open(this->fname  = fname, this->mode = mode);
	}catch (CFileCannotOpenException e)
	{
		//TODO: do something...
	}

}

void CFileLogger::Write(LPCTSTR buf)
{
	if (!file)
		throw CFileCannotOpenException(fname);
	file->Write((void*)buf, _tcslen(buf));	//VS2005 safe calls should be here	
	Flush();
}

void CFileLogger::SetFileName(LPCTSTR fname)
{
	if (file)
		delete file;
	file = CFileWriter::Open(fname, mode);
}

void CFileLogger::SetFileMode(long mode)
{
	this->mode = mode;
	// TODO: ReOpen if rewrite ?
}
void CFileLogger::Close()
{
	if (file)
		file->Close();
}
CFileLogger::~CFileLogger()
{
	if (file)
		delete file; 
}

void CFileLogger::Flush(void) const
{
	file->Flush();
}
}
