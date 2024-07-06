#pragma once
#include <memory>

namespace Zero
{
		// Alias for std::shared_ptr
		template<typename T>
		using Ref = std::shared_ptr <T>;
		// Alias for std::make_shared
		template<typename T, typename ... Args>
		constexpr Ref<T> CreateRef(Args&& ... args)
		{
			return std::make_shared<T>(std::forward<Args>(args)...);
		}

		// Alias for std::shared_ptr
		template<typename T>
		using Scope = std::unique_ptr <T>;
		// Alias for std::make_shared
		template<typename T, typename ... Args>
		constexpr Scope<T> CreateScope(Args&& ... args)
		{
			return std::make_unique<T>(std::forward<Args>(args)...);
		}


}