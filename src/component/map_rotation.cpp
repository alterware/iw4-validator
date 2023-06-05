#include <std_include.hpp>

#include <utils/string.hpp>

#include "map_rotation.hpp"

using namespace std::literals;

namespace map_rotation
{
	rotation_data::rotation_data()
		: index_(0)
	{
	}

	void rotation_data::randomize()
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		std::ranges::shuffle(this->rotation_entries_, gen);
	}

	void rotation_data::add_entry(const std::string& key, const std::string& value)
	{
		this->rotation_entries_.emplace_back(std::make_pair(key, value));
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

	void rotation_data::parse(const std::string& data)
	{
		const auto tokens = utils::string::split(data, ' ');
		for (std::size_t i = 0; !tokens.empty() && i < (tokens.size() - 1); i += 2)
		{
			const auto& key = tokens[i];
			const auto& value = tokens[i + 1];

			if (key == "map"s || key == "gametype"s)
			{
				this->add_entry(key, value);
			}
			else
			{
				throw map_rotation_parse_error();
			}
		}
	}
}
