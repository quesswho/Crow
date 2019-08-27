#pragma once
#include <filesystem>
#include <fstream>
#include <string>

namespace Crow {

	class FileUtils {
		static std::string ReadFile(const char* path)
		{
			std::ifstream file(path);
			const auto size = std::filesystem::file_size(path);

			std::string result(size, ' ');
			file.read(result.data(), size);

			return result;
		}
	};
}