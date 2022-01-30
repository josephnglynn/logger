//
// Created by joseph on 18/11/2021.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#define LOGGER_RED "\u001b[31m"
#define LOGGER_GREEN "\u001b[32m"
#define LOGGER_YELLOW "\u001b[33m"
#define LOGGER_BLUE "\u001b[34m"
#define LOGGER_PURPLE "\u001b[35m"
#define RESET "\u001b[0m"
#define STARTING_STRING "\u001b[37m==> "
#define LOGGER_INTERVAL = "    "

#ifndef LOGGER_STREAM
#define LOGGER_STREAM std::cout
#endif

#ifndef LOGGER_ALWAYS_OUTPUT
	#ifdef DEBUG
	#define LOGGER_ALWAYS_OUTPUT
	#endif
#endif

#include <iostream>

namespace logger
{

	enum OutputSettings
	{
		DebugOnly, //DEBUG ONLY
		Release //RELEASE AND DEBUG
	};

	template<typename ...T>
	static inline constexpr void _output(const std::string& color, T... data)
	{
		LOGGER_STREAM << STARTING_STRING << color;
		((LOGGER_STREAM << std::forward<T>(data) << " "), ...);
		LOGGER_STREAM << RESET << std::endl;
	}

	template<OutputSettings O, typename ...T>
	static inline constexpr void _checkForDebug(const std::string& color ,T... data)
	{
		if constexpr(O == Release)
		{
			_output(color, std::forward<T>(data)...);
		}
		else
		{

#ifdef LOGGER_ALWAYS_OUTPUT
			_output(color, std::forward<T>(data)...);
#endif
		}
	}

	template<OutputSettings O = DebugOnly, typename ...T>
	constexpr void info(T... data)
	{
		_checkForDebug<O>(LOGGER_LOGGER_BLUE, std::forward<T>(data)...);
	}

	template<OutputSettings O = DebugOnly, typename ...T>
	constexpr void warn(T... data)
	{
		_checkForDebug<O>(LOGGER_YELLOW, std::forward<T>(data)...);
	}

	template<OutputSettings O = Release, typename ...T>
	constexpr void error(T... data)
	{
		_checkForDebug<O>(LOGGER_RED, std::forward<T>(data)...);
	}

	template<OutputSettings O = Release, typename ...T>
	constexpr void success(T... data)
	{
		_checkForDebug<O>(LOGGER_LOGGER_GREEN, std::forward<T>(data)...);
	}

	template<OutputSettings O = Release, typename ...T>
	constexpr void notify(T... data)
	{
		_checkForDebug<O>(LOGGER_LOGGER_PURPLE, std::forward<T>(data)...);
	}



}

#endif //LOGGER_HPP
