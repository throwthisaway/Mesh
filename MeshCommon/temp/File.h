#pragma once
#include <stdio.h>
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif
#include "w32types.h"
#include <tchar.h>
#include <string>

namespace IO
{
#define FILE_READ 0x1L
#define FILE_REWRITE 0x2L
#define FILE_APPEND 0x4L
#define FILE_BIN 0x8L
#define FILE_TEXT 0x10L


typedef enum eSEEK_ORIGIN{SEEK_BEGIN, SEEK_BOTTOM, SEEK_POS}SEEK_ORIGIN;
class CFileExceptionBase
{
public:
	std::string msg;
};
class CFileNotOpenException : public CFileExceptionBase
{
public:
	CFileNotOpenException(LPCTSTR fname) { this->msg = _T("File not open: "); this->msg += fname; };
};
class CFileNotFoundException : public CFileExceptionBase
{
public:
	CFileNotFoundException(LPCTSTR fname) { this->msg = _T("File not found: "); this->msg += fname; };
};

class CFileSystem
{
public:
	static void GetCWD(LPTSTR buffer, int size)
	{
		#ifdef WIN32 
			_tgetcwd(buffer, size); 
		#else
			getcwd(buffer, size);
		#endif
	};
	static void ChDir(LPCTSTR buffer)
	{
	#ifdef WIN32  
		_tchdir(buffer);
	#else
		chdir(buffer); 
	#endif
	};
	static void MkDir(LPCTSTR  buffer)
    {
	#ifdef WIN32  
		_tmkdir(buffer);
	#else
		mkdir(buffer); 
	#endif
	};
};

class CFile
{
	CFile(void);
protected:
	FILE * f;
	LPCTSTR fname;
	CFile(FILE * f, LPCTSTR fname);
    ~CFile();
public:	
	void Close(void);
	BOOL IsOpened(void);
	void Seek(SEEK_ORIGIN origin, long offset);
	long Pos(void);
	long Size(void);
	static BOOL Exists(LPCTSTR fname);

};

}
