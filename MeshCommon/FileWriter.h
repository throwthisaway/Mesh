#pragma once
#include "File.h"
#include <memory>

namespace IO {

class CFileOpenException : public CFileExceptionBase {
public:
	CFileOpenException(std::string fname) { msg = __T("File cannot open: "); msg += fname; };
};

class CFileWriter : public CFile {
public:
	CFileWriter(_LPCTSTR fname) : CFile(nullptr, fname) {};
	~CFileWriter();
	void Open(long mode);
	size_t Write(void * data, size_t count);
	void Flush(void);
};
}
