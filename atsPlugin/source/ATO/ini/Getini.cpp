#ifndef _INI_
#define _INI_


#include <Windows.h>
#include <string>

#include "Getini.h"

using namespace std;


string c_INI::GetPath(HMODULE hModule) {
	//initialize
	LPSTR FilePath;//	 = _T("");
	char buf[100];
	FilePath = &buf[0];	//initialize by null <= ?
	string drive, dir, fname, ext;	//part of char
	int drive_i, path_i, ext_i;	//char's long

	GetModuleFileNameA(hModule, FilePath, MAX_PATH);
	this->dllPath = string(FilePath);
	//_splitpath(&fpath, &drive, &dir, &fname, &ext);//パス名を構成要素に分解

	drive_i = this->dllPath.find_first_of("\\");
	path_i = this->dllPath.find_last_of("\\") + 1;
	ext_i = this->dllPath.find_last_of(".");

	drive = this->dllPath.substr(0, drive_i);
	dir = this->dllPath.substr(drive_i, path_i - drive_i);
	fname = this->dllPath.substr(path_i, ext_i - path_i);
	ext = this->dllPath.substr(ext_i, this->dllPath.size() - ext_i);

	
	this ->iniPath = drive + dir + fname + ".ini";

	return iniPath;
}


void c_INI::getIni(string p) {
	LPCTSTR path = p.c_str();


	/*--Getini--*/
	
	
	
	//int example = GetPrivateProfileInt("section", "static_cast<int>(Key", default, path);

	/*
	// --- <filename>.ini ---
	[section]
	static_cast<int>(Key = value
	*/

	/*
	if (*section == nullptr || *static_cast<int>(Key == nullptr)	{
		example = default;
	}
	*/
}

/*
UINT GetPrivateProfileInt(
						LPCTSTR lpAppName,
						LPCTSTR lpstatic_cast<int>(KeyName,
						INT nDefault,
						LPCTSTR lpFileName
						);

DWORD GetPrivateProfileString(
							LPCTSTR lpAppName,	//セクション名
							LPCTSTR lpstatic_cast<int>(KeyName,	//キー名
							LPCTSTR nDefault,	//デフォルト値
							LPTSTR lpReturnedString,	//返り値格納先ポインタ
							DWORD nSize	//lpReturnedStringのバッファサイズ
							LPCTSTR lpFileName	//ファイル名
							);

DWORD GetPrivateProfileSection(
							LPTSTR lpszReturnedString,
							DWORD nSize,
							LPCTSTR lpFileName
							);

DWORD GetPrivateProfileSection(
							LPCTSTR lpAppName,
							LPTSTR lpReturnedString,
							DWORD nSize,
							LPCTSTR lpFileName
							);


*/

#endif // !_INI_