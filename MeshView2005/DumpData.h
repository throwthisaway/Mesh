#pragma once

#include "FileWriter.h"
#include <memory>
namespace Scene
{

class CDumpData
{
    std::auto_ptr<IO::CFileWriter> _fw;
public:
    CDumpData(LPCSTR fName);
    ~CDumpData(void);
     int CDumpData::ExportData(const void* data, const size_t byteLen, BOOL writeLen = FALSE);
};
}

