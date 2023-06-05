#include <std_include.hpp>

#include "component/console.hpp"

#include "cg_client_side_effects_mp.hpp"
#include "q_shared.hpp"

#define MAX_CLIENT_ENT_SOUNDS 1024

#define SKIP_LINE(buffer, text) \
	if (match_line_starting_with((buffer), (text))) \
	{ \
		(buffer) = skip_line_starting_with((buffer), (text)); \
	}

#define SKIP_TEXT(buffer, text) \
	(buffer) = skip_text((buffer), (text)); \
	if (!(buffer)) \
	{ \
		return false; \
	}

#define SKIP_TEXT_PTR(buffer, text) \
	(buffer) = skip_text((buffer), (text)); \
	if (!(buffer)) \
	{ \
		return nullptr; \
	}

#define SKIP_WHITE_SPACE(buffer) \
	(buffer) = skip_white_space((buffer)); \
	if (!(buffer)) \
	{ \
		return false; \
	}

#define SKIP_WHITE_SPACE_PTR(buffer) \
	(buffer) = skip_white_space((buffer)); \
	if (!(buffer)) \
	{ \
		return nullptr; \
	}

#define PARSE_STRING_PTR(buffer, out) \
	(buffer) = parse_string_finish((buffer), (out), sizeof(out)); \
	if (!(buffer)) \
	{ \
		return nullptr; \
	}

#define PARSE_VEC3_PTR(buffer, out) \
	(buffer) = parse_vec3_finish((buffer), (out)); \
	if (!(buffer)) \
	{ \
		return nullptr; \
	}

namespace game
{
	auto client_ent_sound_count = 0;

	// Just report back to the user if there are too many
	void add_client_ent_sound(const float* origin, const char* sound_alias)
	{
		if (client_ent_sound_count == MAX_CLIENT_ENT_SOUNDS)
		{
			console::error("Unable to add %s at [%.2f, %.2f, %.2f]-> Too many client ent sounds. Reduce sounds or increase MAX_CLIENT_ENT_SOUNDS (%d).\n",
				sound_alias, origin[0], origin[1], origin[2], MAX_CLIENT_ENT_SOUNDS);
			return;
		}

		++client_ent_sound_count;
	}

	const char* skip_text(const char* line, const char* skip_text)
	{
		char error_text[128]{};

		if (std::strncmp(skip_text, line, std::strlen(skip_text)) != 0)
		{
			I_strncpyz(error_text, line, sizeof(error_text));
			console::error("Unexpected text '%s' when trying to find '%s' in map's effect file\n", error_text, skip_text);
			return nullptr;
		}

		return &line[std::strlen(skip_text)];
	}

	const char* skip_white_space(const char* line)
	{
		while (std::isspace(static_cast<unsigned char>(*line)))
		{
			++line;
		}

		return line;
	}

	const char* skip_line_starting_with(const char* line, const char* skip_line)
	{
		const auto* result = skip_text(line, skip_line);
		if (!result)
		{
			return nullptr;
		}

		for (auto i = *result; i != '\n'; i = *++result)
		{
			if (i == '\r' || i == '\0')
			{
				break;
			}
		}

		return skip_white_space(result);
	}

	bool match_line_starting_with(const char* line, const char* start_line)
	{
		return std::strncmp(start_line, line, std::strlen(start_line)) == 0;
	}

	const char* parse_string(const char* line, char* text, unsigned int buffer_size)
	{
		char error_text[128]{};

		if (*line != '"')
		{
			I_strncpyz(error_text, line, sizeof(error_text));
			console::error("Expected a quoted string instead of '%s'\n");
			return nullptr;
		}

		auto c = line[1];
		unsigned int i;

		for (i = 0; ((c != '\"' && (c != '\0')) && (i < buffer_size)); ++i)
		{
			text[i] = c;
			c = line[i + 2];
		}

		if (i == buffer_size)
		{
			I_strncpyz(error_text, line, sizeof(error_text));
			console::error("String was longer than expected '%s'\n", error_text);
			return nullptr;
		}

		text[i] = '\0';
		return &line[i + 2];
	}

	const char* parse_string_finish(const char* line, char* text, unsigned int buffer_size)
	{
		line = parse_string(line, text, buffer_size);
		if (!line)
		{
			return nullptr;
		}

		for (auto i = *line; i != '\n'; i = *++line)
		{
			if (i == '\r' || i == '\0')
			{
				break;
			}
		}

		return skip_white_space(line);
	}

	const char* parse_float_finish(const char* line, float* value)
	{
		char error_text[128]{};

		if (std::sscanf(line, "%f", value) != 1)
		{
			I_strncpyz(error_text, line, sizeof(error_text));
			console::error("Expected a float instead of '%s'\n", error_text);
			return nullptr;
		}

		for (auto i = *line; i != '\n'; i = *++line)
		{
			if (i == '\r' || i == '\0')
			{
				break;
			}
		}

		return skip_white_space(line);
	}

	const char* parse_vec3_finish(const char* line, float* origin)
	{
		char error_text[128]{};

		if (std::sscanf(line, "%f, %f, %f", origin, origin + 1, origin + 2) != 3)
		{
			I_strncpyz(error_text, line, sizeof(error_text));
			console::error("Expected 3 floats instead of '%s'\n", error_text);
			return nullptr;
		}

		for (auto i = *line; i != '\n'; i = *++line)
		{
			if (i == '\r' || i == '\0')
			{
				break;
			}
		}

		return skip_white_space(line);
	}

	const char* parse_sound(const char* line)
	{
		float origin[3], angles[3];
		char sound_alias[256]{};

		SKIP_TEXT_PTR(line, "ent = createLoopSound();");

		SKIP_WHITE_SPACE_PTR(line);

		SKIP_TEXT_PTR(line, "ent.v[ \"origin\" ] = (");
		PARSE_VEC3_PTR(line, origin);

		SKIP_TEXT_PTR(line, "ent.v[ \"angles\" ] = (");
		PARSE_VEC3_PTR(line, angles);

		SKIP_TEXT_PTR(line, "ent.v[ \"soundalias\" ] = ");
		PARSE_STRING_PTR(line, sound_alias);

		add_client_ent_sound(origin, sound_alias);
		return line;
	}

	const char* parse_effect(const char* line)
	{
		float delay;
		float origin[3], angles[3];
		char sound_alias[256]{};
		char fx_def_name[256]{};

		SKIP_TEXT_PTR(line, "ent = createOneshotEffect( ");
		PARSE_STRING_PTR(line, fx_def_name);

		SKIP_TEXT_PTR(line, "ent.v[ \"origin\" ] = (");
		PARSE_VEC3_PTR(line, origin);

		SKIP_TEXT_PTR(line, "ent.v[ \"angles\" ] = (");
		PARSE_VEC3_PTR(line, angles);

		SKIP_TEXT_PTR(line, "ent.v[ \"fxid\" ] = ");
		PARSE_STRING_PTR(line, fx_def_name);

		SKIP_TEXT_PTR(line, "ent.v[ \"delay\" ] = ");
		line = parse_float_finish(line, &delay);
		if (!line)
		{
			return nullptr;
		}

		if (match_line_starting_with(line, "ent.v[ \"soundalias\" ] = "))
		{
			SKIP_TEXT_PTR(line, "ent.v[ \"soundalias\" ] = ");
			PARSE_STRING_PTR(line, sound_alias);

			add_client_ent_sound(origin, sound_alias);
		}

		// Skip FX registration code

		return line;
	}

	bool parse_client_effects(const char* buffer)
	{
		char error_text[128]{};

		// Header section

		SKIP_TEXT(buffer, "//_createfx generated. Do not touch!!");
		SKIP_WHITE_SPACE(buffer);
		SKIP_TEXT(buffer, "#include common_scripts\\utility;");
		SKIP_WHITE_SPACE(buffer);
		SKIP_TEXT(buffer, "#include common_scripts\\_createfx;");
		SKIP_WHITE_SPACE(buffer);

		// Fake GSC section

		SKIP_TEXT(buffer, "main()");
		SKIP_WHITE_SPACE(buffer);
		SKIP_TEXT(buffer, "{");
		SKIP_WHITE_SPACE(buffer);

		SKIP_LINE(buffer, "//");

		if (buffer)
		{
			while (*buffer != '}' && *buffer)
			{
				if (match_line_starting_with(buffer, "ent = createLoopSound();"))
				{
					buffer = parse_sound(buffer);
					if (!buffer)
					{
						return false;
					}
				}
				else if (match_line_starting_with(buffer, "ent = createOneshotEffect( "))
				{
					buffer = parse_effect(buffer);
					if (!buffer)
					{
						return false;
					}
				}
				else
				{
					I_strncpyz(error_text, buffer, sizeof(error_text));
					console::error("Expected 'ent = createLoopSound();' or 'ent = createOneshotEffect' instead of '%s' in map's effect file\n", error_text);
					return false;
				}
			}
		}

		buffer = skip_line_starting_with(buffer, "}");
		if (buffer && *buffer)
		{
			I_strncpyz(error_text, buffer, sizeof(error_text));
			console::error("Unexpected data after parsing '%s' map's effect file\n", error_text);
			return false;
		}

		return true;
	}
}
