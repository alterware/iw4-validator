#pragma once

namespace utils::string
{
	std::vector<std::string> split(const std::string& s, char delim);

	std::string to_lower(const std::string& text);
	std::string to_upper(const std::string& text);
}
