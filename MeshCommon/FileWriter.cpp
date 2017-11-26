#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include "FileWriter.h"

namespace IO {

void CFileWriter::Open(long mode) {
	unsigned long sm = 0;
	if (mode & FILE_REWRITE)
		sm = 'w';
	else if (mode & FILE_APPEND)
		sm = 'a';
	else 
		return;
	if (mode & FILE_BIN)
		sm |= 'b'<<8;
	else if (mode & FILE_TEXT)
		sm |= 't'<<8;
	else 
		sm |= 'b'<<8;
	f = fopen(fname,(const char*)&sm);
	if (!f) throw CFileOpenException(fname);
}

size_t CFileWriter::Write(void * data, size_t count) {
	if (!IsOpened())
		throw new CFileOpenException(fname);
	return fwrite(data, count, 1, f);
}
void CFileWriter::Flush() {
	if (!IsOpened())
		throw new CFileOpenException(fname);
	fflush(f);
	
}
CFileWriter::~CFileWriter() {
	if (f) fflush(f);
}
}
