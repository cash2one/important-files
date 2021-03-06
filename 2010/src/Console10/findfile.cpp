#include "stdafx.h"	//--i want remove it
#include "findfile.h"
#include <windows.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FindFile::FindFile()
: _limit(def_limit)
, _dirs(0)
{

}

FindFile::~FindFile()
{

}

//////////////////////////////////////////////////////////////////////
string FindFile::_uppercase_(string s)
{
	const char aazz = 'z' - 'Z';
	string rs;
	for (string::iterator it = s.begin();
		s.end() != it; it++)
	{
		if ('a' <= *it && *it <= 'z') rs.append(1, *it - aazz);
		else rs.append(1, *it);
	}
	return rs;
}
//////////////////////////////////////////////////////////////////////

int FindFile::match(string fext_, string file_)
{
	string fext = _uppercase_(fext_);
	string file = _uppercase_(file_);

	int pos = file.find_last_of('.');
	if (string::npos != pos) file = file.substr(pos);

	return (string::npos != fext.find(file));
}

void FindFile::limit(int limit_)
{
	if (limit_ < 1) _limit = def_limit;
	if (limit_ < max_limit) _limit = limit_;
}

int FindFile::dir(string s)
{
	return (FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(s.c_str()));
}

int FindFile::find(string path_, string fext_)
{
	_co_file.clear();
	_co_dir.clear();
	_co.clear();

	string path = path_;
	if (path.size() > 0 && c_pathSeparator != path[path.size() - 1])
		path.append(s_pathSeparator);

	_co_dir.push_back( path );

	string fext = fext_;
	if (0 == fext.compare("*")
		|| 0 == fext.compare("*.*")
		) fext = "";

	//string file = fext;
	string file = "*";

	string s = path + file;

	WIN32_FIND_DATA fileinfo = {0};
	HANDLE handle = FindFirstFile( s.c_str(), &fileinfo );

	if (NULL != handle && INVALID_HANDLE_VALUE != handle)
	{

		do {

			if (_limit_()) break;	//--limit test

			if ('.' != fileinfo.cFileName[0])	//--skip./..
				if ((FILE_ATTRIBUTE_DIRECTORY & fileinfo.dwFileAttributes)
					== FILE_ATTRIBUTE_DIRECTORY)	//--目录
				{
					if (!_limit_()	//--limit test
						//&& 0 == fext.size()
						) _co_dir.push_back( path + fileinfo.cFileName + s_pathSeparator );

					if (0 != dirs())
					{
						FindFile o;
						o.dirs(dirs());
						o.find( path + fileinfo.cFileName, fext);
						//--dir
						typeT o_dir = o.co_dir();
						for (typeT::iterator it_dir = o_dir.begin();
							o_dir.end() != it_dir && !_limit_();	//--limit test
							it_dir ++) _co_dir.push_back(*it_dir);
						//--file
						typeT o_file = o.co_file();
						for (typeT::iterator it_file = o_file.begin();
							o_file.end() != it_file && !_limit_();	//--limit test
							it_file ++) _co_file.push_back(*it_file);
					}
				}
				else
				{
					if (!_limit_()	//--limit test
						&& (0 == fext.size() || match(fext, fileinfo.cFileName))
						) _co_file.push_back( path + fileinfo.cFileName );
				}

		} while (FindNextFile( handle, &fileinfo ));

		FindClose(handle);
	}

	//--dir
	for (typeT::iterator it_dir = _co_dir.begin();
		_co_dir.end() != it_dir;
		it_dir ++) _co.push_back(*it_dir);
	//--file
	for (typeT::iterator it_file = _co_file.begin();
		_co_file.end() != it_file;
		it_file ++) _co.push_back(*it_file);

	return count();
}

//--example
void FindFile::test()
{
	char win32sys_path[MAX_PATH] = {0};
	::GetSystemDirectory(win32sys_path, MAX_PATH);

	string path = win32sys_path;
	string fext = "*.dll; *.exe; *.ini; *.sys";
	//string fext;

	FindFile o;
	//o.dirs(1);	//--查找子目录
	o.limit(100);	//--最多查找100个
	int count = o.find(path, fext);
	//int count = o.find(path);
	int count_dir = o.co_dir().size();
	int count_file = o.co_file().size();

	//--first
	::MessageBox(NULL, o.get().data(), "FindFile.test.path", MB_OK);

	FindFile::typeT coo;
	FindFile::typeT::iterator it;
	string s;
	stringstream ss;

	//--all
	coo = o.co();
	s.erase();
	ss.str("");
	ss << coo.size();
	s.append("list = " + ss.str() + "\r\n");
	for (it = coo.begin(); coo.end() != it; it ++)
	{
		s.append(*it);
		s.append("\r\n");
	}
	::MessageBox(NULL, s.c_str(), "FindFile.test.all", MB_OK);

	//--dir
	coo = o.co_dir();
	s.erase();
	ss.str("");
	ss << coo.size();
	s.append("list = " + ss.str() + "\r\n");
	for (it = coo.begin(); coo.end() != it; it ++)
	{
		s.append(*it);
		s.append("\r\n");
	}
	::MessageBox(NULL, s.c_str(), "FindFile.test.dir", MB_OK);

	//--file
	coo = o.co_file();
	s.erase();
	ss.str("");
	ss << coo.size();
	s.append("list = " + ss.str() + "\r\n");
	for (it = coo.begin(); coo.end() != it; it ++)
	{
		s.append(*it);
		s.append("\r\n");
	}
	::MessageBox(NULL, s.c_str(), "FindFile.test.file", MB_OK);
}

