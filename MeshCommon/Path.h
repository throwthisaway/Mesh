#pragma once
#include <string>
#include <tchar.h>
namespace IO
{
	using namespace std;
	class CPath
	{
        string _path;
		static const TCHAR _separator, _altSeparator;
        static const TCHAR _dot[];
		static string Conv(const string& path);
	public:
        CPath(const string& path);
        static inline const TCHAR GetCurrentSeparator(void) { return _separator; }
		static string GetPath(const string& fullPath);
        void Combine(const string& path);
        operator const string&() const { return _path; } 
        operator const TCHAR *() const { return _path.c_str(); }
        CPath operator+(const   CPath& path);
        string GetFileName(void);
        static string GetFileName(string& path);
        string GetFileNameWExt(void);
        void SetExt(const string& ext);
        static string SetExt(const string& fullPath, const string& ext);
	};
}

