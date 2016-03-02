#include "stdafx.h"
#include "DumpData.h"

namespace Scene
{

CDumpData::CDumpData(LPCSTR fName) :
    _fw(IO::CFileWriter::Open(fName, FILE_REWRITE|FILE_BIN))
{
}


CDumpData::~CDumpData(void)
{
}

 int CDumpData::ExportData(const void* data, const size_t byteLen, BOOL writeLen)
 {
     if (writeLen)
     {
         if (_fw->Write((void*)&byteLen, sizeof(byteLen))<1)
             return -1;
     }
     return _fw->Write(const_cast<void*>(data), byteLen) < 1;
 }
}