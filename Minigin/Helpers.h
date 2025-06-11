#pragma once
#include <sstream>
#include <iomanip>

namespace Rinigin {
	namespace Helpers {
		inline unsigned int constexpr sdbm_hash(const char* str)
		{
			unsigned int hash = 0;
			int c;
			while ((c = *str++)) {
				hash = c + (hash << 6) + (hash << 16) - hash;
			}
			return hash;
		}

		inline std::string GetFormattedScore(int score)
		{
			std::ostringstream oss;
			oss << std::setfill('0') << std::setw(5) << score;
			std::string scoreStr = oss.str();

			return scoreStr;
		}
	}
}