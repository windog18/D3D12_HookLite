#pragma once
#include <Windows.h>
#include <stdint.h>
#include <dxgi.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <d3d11on12.h>
#include "MinHook/include/MinHook.h"


#include "UWP.hpp"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include <clocale>
#include <locale>
#include <sstream> 
#include <string>
#include <vector>
#include <fstream>


#define DECLARE_FUNCTIONPTR(DReturnType,DFunctionName,...) \
typedef DReturnType(__stdcall* DFunctionName)(__VA_ARGS__);\
DFunctionName o##DFunctionName = NULL; \
DReturnType __stdcall hk##DFunctionName(__VA_ARGS__) \



#define CREATE_HOOKPAIR(methodVirtualPtr,DFunctionName) \
{\
	MH_STATUS result = MH_OK; \
	do { \
		auto result = MH_CreateHook(methodVirtualPtr, hk##DFunctionName, (LPVOID*)&o##DFunctionName); \
	}while(result != MH_OK); \
}

inline void Log(const char *fmt, ...)
{
	if (!fmt)	return;

	char		text[4096];
	va_list		ap;
	va_start(ap, fmt);
	vsprintf_s(text, fmt, ap);
	va_end(ap);


	std::string outputStrs = "[2020] ";
	outputStrs = outputStrs + std::string(text);
	//printf(text);
	OutputDebugStringA(outputStrs.c_str());
// 	std::ofstream logfile(DumpPath, std::ios::app);
// 	if (logfile.is_open() && text)	logfile << text << std::endl;
// 	logfile.close();
}


#if 1
#define  LOG_ONCE(...) \
	
#else
#define LOG_ONCE(...)\
{\
	Log(__VA_ARGS__);\
}
#endif


