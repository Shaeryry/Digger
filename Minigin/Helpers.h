#pragma once

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
	}
}