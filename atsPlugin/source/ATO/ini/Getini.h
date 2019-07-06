#pragma once
#include "../header/define.h"
#include "Getini.h"

using namespace std;

class c_INI {
public:
	//variant
	bool isATC;
	bool isATO;
	bool isTASC;

	//function
	void getIni(string);
	string GetPath(HMODULE);

private:
	string dllPath;	//.dllのpath
	string iniPath;	//.iniのpath
};
