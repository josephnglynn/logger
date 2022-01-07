//
// Created by joseph on 18/11/2021.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#define Red "\u001b[31m"
#define Green "\u001b[32m"
#define Yellow "\u001b[33m"
#define Blue "\u001b[34m"
#define RESET "\u001b[0m"
#define STARTING_STRING "\u001b[37m==> "

#include <iostream>

namespace logger
{

	enum OutputSettings
	{
		Debug, //DEBUG ONLY
		RAD //RELEASE AND DEBUG
	};

	template<typename ...T>
	static inline constexpr void _output(const std::string& color, T... data)
	{
		std::cout << STARTING_STRING << color;
		((std::cout << std::forward<T>(data) << " "), ...);
		std::cout << RESET << std::endl;
	}

	template<OutputSettings O = Debug, typename ...T>
	static inline constexpr void _checkForDebug(const std::string& color ,T... data)
	{
		if constexpr(O == RAD)
		{
			_output(color, (std::forward<T>(data), ...));
		}
		else
		{

#ifdef DEBUG
			_output(color, (std::forward<T>(data), ...));
#endif
		}
	}

	template<OutputSettings O = Debug, typename ...T>
	constexpr void info(T... data)
	{
		_checkForDebug<O>(Blue, (std::forward<T>(data), ...));
	}

	template<OutputSettings O = Debug, typename ...T>
	constexpr void warn(T... data)
	{
		_checkForDebug<O>(Yellow, (std::forward<T>(data), ...));
	}

	template<OutputSettings O = Debug, typename ...T>
	constexpr void error(T... data)
	{
		_checkForDebug<O>(Red, (std::forward<T>(data), ...));
	}

	template<OutputSettings O = Debug, typename ...T>
	constexpr void success(T... data)
	{
		_checkForDebug<O>(Green, (std::forward<T>(data), ...));
	}




}

#endif //LOGGER_HPP
