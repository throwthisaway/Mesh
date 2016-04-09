#pragma once
#include "File.h"
namespace IO
{

class CFileReader : public CFile
{
protected:
	CFileReader(FILE * f, LPCTSTR fname) : CFile(f, fname) {};
public:
	static BOOL Exist(LPCTSTR fname);
	long Read(void * data, long count);	
	BOOL GetLine(LPTSTR data, long count);
	static CFileReader * Open(LPCTSTR fname, long mode = FILE_READ | FILE_BIN);
	BOOL Eof(void);
};
}

