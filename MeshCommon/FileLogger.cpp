#include "FileLogger.h"
#include <string.h>

namespace Log {
using namespace IO;

CFileLogger::CFileLogger(_LPCTSTR fname, long mode) : file(fname) {
	file.Open(mode);
}

void CFileLogger::Write(_LPCTSTR buf) {
	file.Write((void*)buf, strlen(buf));
	Flush();
}

void CFileLogger::Close() {
	file.Close();
}
}
