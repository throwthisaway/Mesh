#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include "FileWriter.h"

namespace IO
{

CFileWriter * CFileWriter::Open(const char * fname, long mode)
{
	FILE * f;
	unsigned long sm = 0;	
	if (mode & FILE_REWRITE)
		sm = 'w';	
	else if (mode & FILE_APPEND)
		sm = 'a';	
	else 
		return NULL;
	if (mode & FILE_BIN)
		sm |= 'b'<<8;	
	else if (mode & FILE_TEXT)
		sm |= 't'<<8;	
	else 
		sm |= 'b'<<8;	
	f = fopen(fname,(const char*)&sm);
	return new CFileWriter(f, fname);
}

long CFileWriter::Write(void * data, long count)
{
	if (!IsOpened())
		throw new CFileCannotOpenException(fname);
	return (long)fwrite(data, count, 1, f);
}
void CFileWriter::Flush()
{	
	if (!IsOpened())
		throw new CFileCannotOpenException(fname);
	fflush(f);
	
}
CFileWriter::~CFileWriter()
{
    Flush();
}
}
