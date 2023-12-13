#include "std_include.hpp"

#include "component/console.hpp"
#include "component/map_rotation.hpp"

#include "game/cg_client_side_effects_mp.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

namespace
{
	void load_client_effects(const std::string& filename)
	{
		assert(!filename.empty());
		if (filename.empty())
		{
			throw std::runtime_error("filename parameter is empty");
		}

		std::string data;
		if (!utils::io::read_file(filename, &data) || data.empty())
		{
			throw std::runtime_error(utils::string::va("'%s' is empty", filename.data()));
		}

		if (game::parse_client_effects(data.data()))
		{
			console::info("Successfully parsed '%s'\n", filename.data());
		}
	}

	void load_map_rotation(const std::string& filename)
	{
		assert(!filename.empty());
		if (filename.empty())
		{
			throw std::runtime_error("filename parameter is empty");
		}

		std::string data;
		if (!utils::io::read_file(filename, &data) || data.empty())
		{
			throw std::runtime_error(utils::string::va("'%s' is empty", filename.data()));
		}

		try
		{
			map_rotation::rotation_data rotation_data;
			rotation_data.parse(data);

			console::info("Successfully parsed map rotation\n");
		}
		catch (const std::exception& ex)
		{
			console::error(utils::string::va("%s. '%s' contains invalid data!\n", ex.what(), filename.data()));
		}
	}

	void unsafe_main(const std::span<char*> s)
	{
		auto p = [&](const std::string& o, const std::function <void(const std::string&)>& c) -> void
		{
			auto r(s | std::views::transform([](char* v) -> std::string
			{
				return { v };
			}));

			const auto i(std::ranges::find(r, o));
			const auto e(r.end());

			if (auto n(i != e ? std::ranges::next(i, 1, e) : e); i != e && n != e)
			{
				c({ *n });
			}
		};

		p("-createfx", load_client_effects);
		p("-map-rotation", load_map_rotation);
	}
}

int main(int argc, char* argv[])
{
	console::set_title("AlterWare IW4x-validator");
	console::log("Starting AlterWare iw4-validator");

	try
	{
		unsafe_main(std::span(argv, argc));
		return EXIT_SUCCESS;
	}
	catch (const std::exception& ex)
	{
		console::error("Fatal error: %s\n", ex.what());
		return EXIT_FAILURE;
	}
}
