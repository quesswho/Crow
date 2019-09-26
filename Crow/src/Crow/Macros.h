#pragma once

typedef unsigned char uchar; // 0 - 255
typedef unsigned short int ushort; // 0 - 65535
typedef unsigned int uint;   // 0 - 4 294 967 295
typedef unsigned long ulong;

#ifdef CR_x64
	typedef long long int int64;
	typedef unsigned long long int uint64; // 0 - 18 446 744 073 709 551 615
#endif


#ifdef CR_DEBUG
	#define CR_CORE_INFO(...) Crow::Log::getCoreLogger()->info(__VA_ARGS__)
	#define CR_CORE_WARNING(...) Crow::Log::getCoreLogger()->warn(__VA_ARGS__)
	#define CR_CORE_ERROR(...) Crow::Log::getCoreLogger()->error(__VA_ARGS__)
	#define CR_CORE_FATAL(...) Crow::Log::getCoreLogger()->error(__VA_ARGS__); __debugbreak;

	#define CR_GAME_INFO(...) Crow::Log::getGameLogger()->info(__VA_ARGS__)
	#define CR_GAME_WARNING(...) Crow::Log::getGameLogger()->warn(__VA_ARGS__)
	#define CR_GAME_ERROR(...) Crow::Log::getGameLogger()->error(__VA_ARGS__)
	#define CR_GAME_FATAL(...) Crow::Log::getGameLogger()->error(__VA_ARGS__); __debugbreak;
#else
	#define CR_CORE_INFO(...)
	#define CR_CORE_WARNING(...)
	#define CR_CORE_ERROR(...)
	#define CR_CORE_FATAL(...)

	#define CR_GAME_INFO(...)
	#define CR_GAME_WARNING(...)
	#define CR_GAME_ERROR(...)
	#define CR_GAME_FATAL(...)
#endif



#if !defined(CR_PLATFORM_WINDOWS)
	#if defined(CR_DEBUG)
		#define CR_WINDOWSERROR() CR_CORE_FATAL("Cannot use a Windows application on a non Windows OS!");	// Will print error if OS is not windows
    #else
		#define CR_WINDOWSERROR()
	#endif
#else
	#define WIN32_LEAN_AND_MEAN		// Make sure Windows.h exludes unwanted includes
	#define CR_WINDOWSERROR()	// Will print error if OS is not windows
	
	#pragma comment(lib, "d3d12.lib")
	#pragma comment(lib, "dxgi.lib")
	#pragma comment(lib, "d3dcompiler.lib")
	#pragma comment(lib, "dxguid.lib")
#endif