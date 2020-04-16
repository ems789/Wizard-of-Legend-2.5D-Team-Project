#pragma once
class CDirectoryMgr
{
private:
	CDirectoryMgr();
	~CDirectoryMgr();

public:
	static CString ConvertRelativePath(const TCHAR* pFullPath);
	static void ExtractPathInfo(const TCHAR* pFullPath, list<PATH_INFO*>& rPathInfoList);
	static int GetFileCount(const TCHAR* pFullPath);
};

