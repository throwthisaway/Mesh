#ifndef WIN32
#include "intel_compatibility.h"
#endif
#include "FileReader.h"
#include <sys/stat.h>
namespace IO
{
	
BOOL CFileReader::Exist(LPCTSTR fname)
{
	struct _stat64i32 buffer;
	return !_tstat(fname, &buffer);
}

CFileReader * CFileReader::Open(LPCTSTR fname, long mode)
{
	FILE * f;	
	unsigned long sm;	
	if (mode & FILE_READ)
		sm = 'r';	
	else
		return NULL;
	if (mode & FILE_BIN)
		sm |= 'b'<<8;	
	else if (mode & FILE_TEXT)
		sm |= 't'<<8;	
	else 
		sm |= 'b'<<8;	 
	if (!(f = _tfopen(fname, (LPCTSTR)&sm)))
		throw CFileNotFoundException(fname);
		//return NULL;	
	return new CFileReader(f, fname);
}

long CFileReader::Read(void * data, long count)
{
	if (!IsOpened())
		throw new CFileNotOpenException(fname);
	return (long)fread(data, count, 1, f);
}

BOOL CFileReader::GetLine(LPTSTR data, long count)
{
	if (count <= 0)
		return FALSE;
	if (!IsOpened())
		throw new CFileNotOpenException(fname);
	if (_fgetts(data, count, f))
		return TRUE;
	return FALSE;
}
BOOL CFileReader::Eof(void)
{
	return feof(f);
}

}
