#ifndef FILEWRITER_H_
#define FILEWRITER_H_
#include "File.h"

namespace IO
{


class CFileCannotOpenException : public CFileExceptionBase
{
public:
	CFileCannotOpenException(LPCTSTR fname) { this->msg = _T("File cannot open: "); this->msg += fname; };
};

class CFileWriter : public CFile
{
protected:
	CFileWriter(FILE * f, LPCTSTR fname) : CFile(f, fname) {};
public:
    ~CFileWriter();
	static CFileWriter * Open(const char * fname, long mode);
	long Write(void * data, long count);
	void Flush(void);
};

}

#endif /*FILEWRITER_H_*/
