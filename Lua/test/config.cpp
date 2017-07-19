#include "config.h"

static std::string rootPath = "..\\..\\Data\\";
static std::wstring rootPathW = L"..\\..\\Data\\";

std::string getAccuratePath(const char* file)
{
	return rootPath + file;
}

std::wstring getAccuratePathW(const wchar_t* file)
{
	return rootPathW + file;
}

