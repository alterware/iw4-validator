#include <std_include.hpp>

#include "q_shared.hpp"

namespace game
{
	void I_strncpyz(char* dest, const char* src, std::size_t dest_size)
	{
		assert(src);
		assert(dest);

		if (!dest)
		{
			return;
		}

		if (!src)
		{
			*dest = '\0';
		}
		else
		{
			auto* p = reinterpret_cast<const unsigned char*>(src - 1);
			auto* q = reinterpret_cast<unsigned char*>(dest - 1);
			auto n = dest_size + 1;
			do
			{
				if (!--n)
				{
					break;
				}

				*++q = *++p;
			} while (*q);

			dest[dest_size - 1] = '\0';
		}
	}
}
