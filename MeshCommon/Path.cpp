#include "Path.h"

namespace IO
{
	const TCHAR CPath::_separator = '\\';
	const TCHAR CPath::_altSeparator = '/';
	const TCHAR CPath::_dot[] = { '.', '\0' };
    CPath::CPath(const string& path) :
        _path(Conv(path))
    {
    }
	string CPath::Conv(const string& path)
	{
		string tempPath(path);
		for(string::iterator it = tempPath.begin(); it != tempPath.end(); it++)
		{
			if (*it == _altSeparator)
				*it = _separator;
		}
		return tempPath;
	}
	string CPath::GetPath(const string& fullPath)
	{
		string tempFullPath = Conv(fullPath);
		string::size_type pos = fullPath.rfind(_separator);
		if (pos == string::npos)
			return _T("");
		return tempFullPath.substr(0, pos + 1);
	}
    void CPath::Combine(const string& path)
    {
        string::iterator it1 = _path.end();
        it1--;
        string convertedPath = Conv(path);
        string::iterator it2= convertedPath.begin();
        if (*it1 != _separator && *it2 != _separator)
            _path += _separator;
        _path += path;
    }
    string CPath::GetFileName(void)
    {
        return GetFileName(_path);
    }
    string CPath::GetFileName(string& path)
    {
        // start might be './'
        const string::size_type pos2 = path.find_last_of(_dot[0]);
        string::size_type pos1 =  path.rfind(_separator, pos2);
        pos1++;
        return path.substr(pos1, pos2 - pos1);
    }
    string CPath::GetFileNameWExt(void)
    {
        string::size_type pos1 =  _path.rfind(_separator);
        pos1++;
        return _path.substr(pos1);
    }
    void CPath::SetExt(const string& ext)
    {
        _path = SetExt(_path, ext);
    }
    string CPath::SetExt(const string& fullPath, const string& ext)
    {
        //// start might be './'
        //string::size_type pos2 = fullPath.find_last_of(_dot[0]);
        //string tmp_path(fullPath, 0, pos2);
        ////if (pos2 != string::npos)
        ////    tmp_path.append(1, _dot[0]);
        ////else 
        ////    ++pos2;
        //tmp_path.replace(pos2, string::npos, ext);
        //string::size_type pos1 =  fullPath.rfind(_separator, pos2);
        //pos1++;
        //return fullPath.substr(pos1, pos2 - pos1);
        return fullPath + _dot[0] + ext;
    }
    CPath CPath::operator+(const CPath& path)
    {
        CPath res(*this);
        res.Combine(path);
        return res;
    }
}
