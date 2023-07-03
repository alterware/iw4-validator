#pragma once

namespace map_rotation
{
#define DEFAULT_ERROR_MSG "Map Rotation Parse Error"

	class map_rotation_parse_error : public std::runtime_error
	{
		static std::string fmt(const std::string& message)
		{
			std::string error = DEFAULT_ERROR_MSG;

			if (!message.empty())
			{
				error.append(": ");
				error.append(message);
			}

			return error;
		}

	public:
		map_rotation_parse_error(const std::string& message)
			: std::runtime_error(fmt(message))
		{
		}

		map_rotation_parse_error()
			: std::runtime_error(DEFAULT_ERROR_MSG)
		{
		}
	};

	class rotation_data
	{
	public:
		using rotation_entry = std::pair<std::string, std::string>;

		rotation_data();

		void add_entry(const std::string& key, const std::string& value);

		[[nodiscard]] std::size_t get_entries_size() const noexcept;

		[[nodiscard]] rotation_entry& get_next_entry();

		[[nodiscard]] bool contains(const std::string& key, const std::string& value) const;
		[[nodiscard]] bool empty() const noexcept;

		void parse(const std::string& data);

	private:
		std::vector<rotation_entry> rotation_entries_;
		std::size_t index_;
	};
}
