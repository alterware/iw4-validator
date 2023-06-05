#include <std_include.hpp>
#include "string.hpp"

namespace utils::string
{
	std::vector<std::string> split(const std::string& s, const char delim)
	{
		std::stringstream ss(s);
		std::string item;
		std::vector<std::string> elems;

		while (std::getline(ss, item, delim))
		{
			elems.push_back(item); // elems.push_back(std::move(item)); // if C++11
		}

		return elems;
	}

	std::string to_lower(const std::string& text)
	{
		std::string result;
		std::ranges::transform(text, std::back_inserter(result), [](const unsigned char input)
		{
			return static_cast<char>(std::tolower(input));
		});

		return result;
	}

	std::string to_upper(const std::string& text)
	{
		std::string result;
		std::ranges::transform(text, std::back_inserter(result), [](const unsigned char input)
		{
			return static_cast<char>(std::toupper(input));
		});

		return result;
	}
}
