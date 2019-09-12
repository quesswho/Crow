#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include "Common.h"

namespace Crow {

	class FileUtils {
	public:
		static std::string ReadFile(const char* path)
		{
			std::ifstream file(path);
			if (file.fail())
			{
				CR_CORE_ERROR("Failed to read {0}", path, "!");
				return std::string("-->ERROR<--");
			}
			const auto size = std::filesystem::file_size(path);

			std::string result(size, ' ');
			file.read(result.data(), size);

			return result;
		}
	};
}