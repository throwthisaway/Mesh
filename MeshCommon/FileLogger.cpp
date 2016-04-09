#include "FileLogger.h"
#include <string.h>

namespace Log
{
	
CFileLogger::CFileLogger(_LPCTSTR fname, long mode)
{
	try
	{
		file = CFileWriter::Open(this->fname  = fname, this->mode = mode);
	}catch (CFileCannotOpenException e)
	{
		//TODO: do something...
	}

}

void CFileLogger::Write(_LPCTSTR buf)
{
	if (!file)
		throw CFileCannotOpenException(fname);
	file->Write((void*)buf, strlen(buf));	//VS2005 safe calls should be here	
	Flush();
}

void CFileLogger::SetFileName(_LPCTSTR fname)
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
