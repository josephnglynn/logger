//
// Created by joseph on 18/11/2021.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <ostream>
#include <vector>

namespace logger
{

	enum OutputSettings
	{
		DebugOnly, // DEBUG ONLY
		Release // RELEASE AND DEBUG
	};

	struct Options
	{
		std::vector<std::ostream*> output_streams;
	};

	namespace internal
	{
		const auto starting_string = "\u001b[37m==> ";
		const auto interval = "    "; // Note, kept for later
		const auto reset = "\u001b[0m";
		const auto red = "\u001b[31m";
		const auto green = "\u001b[32m";
		const auto yellow = "\u001b[33m";
		const auto blue = "\u001b[34m";
		const auto purple = "\u001b[35m";

#ifdef DEBUG
		const bool is_debug_build = true;
#else
		const bool is_debug_build = false;
#endif

		Options* options;

		inline Options* get_default_options()
		{
			Options* def_options = new Options();

			def_options->output_streams = { &std::cout };

			return def_options;
		}


		template<typename ...T>
		static inline void output(const char* color, T... data)
		{
			for (const auto stream: options->output_streams)
			{
				*stream << starting_string << color;
				((*stream << data << " "), ...)
				*stream << reset << std::endl;
			}
		}



		template<OutputSettings O, typename ...T>
		static inline constexpr void checkForDebug(const char* color, T... data)
		{
			if constexpr(O == Release || internal::is_debug_build)
			{
				_output(color, std::forward<T>(data)...);
			}
		}
	}

	constexpr void init(Options* opt = internal::get_default_options())
	{
		internal::options = opt;
	}

	template<OutputSettings O = DebugOnly, typename ...T>
	constexpr void info(T... data)
	{
		internal::checkForDebug<O>(internal::blue, std::forward<T>(data)...);
	}

	template<OutputSettings O = DebugOnly, typename ...T>
	constexpr void warn(T... data)
	{
		internal::checkForDebug<O>(internal::yellow, std::forward<T>(data)...);
	}

	template<OutputSettings O = Release, typename ...T>
	constexpr void error(T... data)
	{
		internal::checkForDebug<O>(internal::red, std::forward<T>(data)...);
	}

	template<OutputSettings O = Release, typename ...T>
	constexpr void success(T... data)
	{
		internal::checkForDebug<O>(internal::green, std::forward<T>(data)...);
	}

	template<OutputSettings O = Release, typename ...T>
	constexpr void notify(T... data)
	{
		internal::checkForDebug<O>(internal::purple, std::forward<T>(data)...);
	}

}

#endif //LOGGER_HPP
