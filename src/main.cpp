#include "std_include.hpp"

#include "component/console.hpp"
#include "component/map_rotation.hpp"

#include "game/cg_client_side_effects_mp.hpp"

#include <utils/io.hpp>

namespace
{
	bool load_client_effects(const std::string& filename)
	{
		if (filename.empty())
		{
			console::error("filename parameter is empty\n");
			return false;
		}

		const auto data = utils::io::read_file(filename);
		if (data.empty())
		{
			console::error("'%s' is empty\n", filename.data());
			return false;
		}

		return game::parse_client_effects(data.data());
	}

	bool load_map_rotation(const std::string& filename)
	{
		if (filename.empty())
		{
			console::error("filename parameter is empty\n");
			return false;
		}

		const auto data = utils::io::read_file(filename);
		if (data.empty())
		{
			console::error("'%s' is empty\n", filename.data());
			return false;
		}

		try
		{
			map_rotation::rotation_data rotation_data;
			rotation_data.parse(data);
		}
		catch (const std::exception& ex)
		{
			console::error("%s: '%s' contains invalid data!\n", ex.what(), filename.data());
			return false;
		}

		console::info("Successfully parsed map rotation\n");
		return true;
	}
}

int unsafe_main(std::string&& prog, std::vector<std::string>&& args)
{
	// Parse command-line flags (only increment i for matching flags)
	for (auto i = args.begin(); i != args.end();)
	{
		if (*i == "-createfx")
		{
			++i;
			const auto filename = i != args.end() ? *i++ : std::string();
			console::info("Parsing createfx '%s'\n", filename.data());

			if (!load_client_effects(filename))
			{
				return EXIT_FAILURE;
			}
		}
		else if (*i == "-map-rotation")
		{
			++i;
			const auto filename = i != args.end() ? *i++ : std::string();
			console::info("Parsing map rotation '%s'\n", filename.data());

			if (!load_map_rotation(filename))
			{
				return EXIT_FAILURE;
			}

		}
		else
		{
			console::info("X Labs IW4x validator tool\n"
				"Usage: %s OPTIONS\n"
				"  -createfx <filename>\n"
				"  -fx <filename>\n"
				"  -map-rotation <filename>\n",
				prog.data()
			);

			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	console::set_title("X Labs IW4x-validator");
	console::log("Starting X Labs IW4x-validator");

	try
	{
		std::string prog(argv[0]);
		std::vector<std::string> args;

		args.reserve(argc - 1);
		args.assign(argv + 1, argv + argc);
		return unsafe_main(std::move(prog), std::move(args));
	}
	catch (const std::exception& ex)
	{
		console::error("Fatal error: %s\n", ex.what());
		return EXIT_FAILURE;
	}
}
