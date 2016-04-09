#ifndef FILEWRITER_H_
#define FILEWRITER_H_
#include "File.h"

namespace IO
{


class CFileCannotOpenException : public CFileExceptionBase
{
public:
	CFileCannotOpenException(_LPCTSTR fname) { this->msg = __T("File cannot open: "); this->msg += fname; };
};

class CFileWriter : public CFile
{
protected:
	CFileWriter(FILE * f, _LPCTSTR fname) : CFile(f, fname) {};
public:
    ~CFileWriter();
	static CFileWriter * Open(const char * fname, long mode);
	size_t Write(void * data, size_t count);
	void Flush(void);
};

}

#endif /*FILEWRITER_H_*/
