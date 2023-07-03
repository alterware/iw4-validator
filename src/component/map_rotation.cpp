#include <std_include.hpp>

#include <utils/string.hpp>

#include "map_rotation.hpp"

using namespace std::literals;

namespace map_rotation
{
	namespace
	{
		bool is_valid_key(const std::string& key)
		{
			static std::array<const char*, 3> keys =
			{
				"map",
				"gametype",
				"exec",
			};

			return std::ranges::find(keys, key) != std::end(keys);
		}
	}

	rotation_data::rotation_data()
		: index_(0)
	{
	}

	void rotation_data::add_entry(const std::string& key, const std::string& value)
	{
		this->rotation_entries_.emplace_back(key, value);
	}

	std::size_t rotation_data::get_entries_size() const noexcept
	{
		return this->rotation_entries_.size();
	}

	rotation_data::rotation_entry& rotation_data::get_next_entry()
	{
		const auto index = this->index_;
		++this->index_ %= this->rotation_entries_.size();
		return this->rotation_entries_.at(index);
	}

	bool rotation_data::contains(const std::string& key, const std::string& value) const
	{
		return std::ranges::any_of(this->rotation_entries_, [&](const auto& entry)
		{
			return entry.first == key && entry.second == value;
		});
	}

	bool rotation_data::empty() const noexcept
	{
		return this->rotation_entries_.empty();
	}

	void rotation_data::parse(const std::string& data)
	{
		const auto tokens = utils::string::split(data, ' ');
		for (std::size_t i = 0; !tokens.empty() && i < (tokens.size() - 1); i += 2)
		{
			const auto& key = tokens[i];
			const auto& value = tokens[i + 1];

			if (is_valid_key(key))
			{
				this->add_entry(key, value);
			}
			else
			{
				throw map_rotation_parse_error(utils::string::va("Invalid key '%s'", key.data()));
			}
		}
	}
}
