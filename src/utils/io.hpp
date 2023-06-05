#pragma once

#include <string>

namespace utils
{
	namespace io
	{
		bool remove_file(const std::string& file);
		bool move_file(const std::string& src, const std::string& target);
		bool file_exists(const std::string& file);
		bool write_file(const std::string& file, const std::string& data, bool append = false);
		bool read_file(const std::string& file, std::string* data);
		std::string read_file(const std::string& file);
		std::size_t file_size(const std::string& file);
	}
}
