#include <std_include.hpp>
#include "io.hpp"

namespace utils
{
	namespace io
	{
		bool remove_file(const std::string& file)
		{
			return std::remove(file.c_str()) == 0;
		}

		bool move_file(const std::string& src, const std::string& target)
		{
			return std::rename(src.c_str(), target.c_str()) == 0;
		}

		bool file_exists(const std::string& file)
		{
			return std::ifstream(file).good();
		}

		bool write_file(const std::string& file, const std::string& data, const bool append)
		{
			auto mode = std::ios::binary | std::ofstream::out;
			if (append)
			{
				mode |= std::ofstream::app;
			}

			std::ofstream stream(file, mode);

			if (stream.is_open())
			{
				stream.write(data.data(), static_cast<std::streamsize>(data.size()));
				stream.close();
				return true;
			}

			return false;
		}

		std::string read_file(const std::string& file)
		{
			std::string data;
			read_file(file, &data);
			return data;
		}

		bool read_file(const std::string& file, std::string* data)
		{
			if (!data) return false;
			data->clear();

			if (file_exists(file))
			{
				std::ifstream stream(file, std::ios::binary);
				if (!stream.is_open()) return false;

				stream.seekg(0, std::ios::end);
				const std::streamsize size = stream.tellg();
				stream.seekg(0, std::ios::beg);

				if (size > -1)
				{
					data->resize(static_cast<std::uint32_t>(size));
					stream.read(const_cast<char*>(data->data()), size);
					stream.close();
					return true;
				}
			}

			return false;
		}

		std::size_t file_size(const std::string& file)
		{
			if (file_exists(file))
			{
				std::ifstream stream(file, std::ios::binary);

				if (stream.good())
				{
					stream.seekg(0, std::ios::end);
					return static_cast<std::size_t>(stream.tellg());
				}
			}

			return 0;
		}
	}
}
