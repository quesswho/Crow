#pragma once

typedef unsigned char uchar; // 0 - 255
typedef unsigned short int ushort; // 0 - 65535
typedef unsigned int uint;   // 0 - 4 294 967 295

#ifdef CR_x64
	typedef long long int int64;
	typedef unsigned long long int uint64; // 0 - 18 446 744 073 709 551 615
#endif


// CR_ASSERT is disabled because of issues
#ifdef CR_DEBUG
	#define CR_CORE_INFO(...) Crow::Log::getCoreLogger()->info(__VA_ARGS__)
	#define CR_CORE_WARNING(...) Crow::Log::getCoreLogger()->warn(__VA_ARGS__)
	#define CR_CORE_ERROR(...) Crow::Log::getCoreLogger()->error(__VA_ARGS__)
	//#define CR_CORE_ASSERT(x, ...) if(!x) Crow::Log::getCoreLogger()->error(__VA_ARGS__) // CR_CORE_ASSERT(object, char* | std::string)

	#define CR_GAME_INFO(...) Crow::Log::getGameLogger()->info(__VA_ARGS__)
	#define CR_GAME_WARNING(...) Crow::Log::getGameLogger()->warn(__VA_ARGS__)
	#define CR_GAME_ERROR(...) Crow::Log::getGameLogger()->error(__VA_ARGS__)
	//#define CR_GAME_ASSERT(x, ...) if(!x) Crow::Log::getGameLogger()->error(__VA_ARGS__) // CR_GAME_ASSERT(object, char* | std::string)
#else
	#define CR_CORE_INFO(...)
	#define CR_CORE_WARNING(...)
	#define CR_CORE_ERROR(...)
	//#define CR_CORE_ASSERT(x, ...) ;

	#define CR_GAME_INFO(...)
	#define CR_GAME_WARNING(...)
	#define CR_GAME_ERROR(...)
	//#define CR_GAME_ASSERT(x, ...) ;
#endif