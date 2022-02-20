//
// Created by joseph on 18/11/2021.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <ostream>
#include <utility>
#include <vector>
#include <memory>

#define LOGGER_INLINE inline
#ifdef LOGGER_DISABLE_INLINE
#define LOGGER_INLINE
#endif

namespace logger
{

	namespace values
	{
		constexpr const char* blue = "\u001b[34m";
		constexpr const char* yellow = "\u001b[33m";
		constexpr const char* green = "\u001b[32m";
		constexpr const char* purple = "\u001b[35m";
		constexpr const char* red = "\u001b[31m";
		constexpr const char* starting_string = "\u001b[37m==> ";
		constexpr const char* reset_code = "\u001b[0m";

		constexpr const char* default_interval = "    "; // Note, kept for later
	}

	enum OutputSettings
	{
		Debug, // DEBUG ONLY
		Release, // RELEASE ONLY,
		All,
	};

	struct Colors
	{
		explicit Colors(
			const char* info = values::blue,
			const char* warn = values::yellow,

			const char* success = values::green,
			const char* notify = values::purple,
			const char* error = values::red,

			const char* starting_string = values::starting_string,
			const char* reset_code = values::reset_code
		);

		const char* info_color;
		const char* warn_color;

		const char* success_color;
		const char* notify_color;
		const char* error_color;

		const char* starting_string;
		const char* reset_code;
	};

	struct Options
	{
		explicit Options(std::vector<std::ostream*> streams = { &std::cout }, Colors colors = Colors());

		Colors colors;
		std::vector<std::ostream*> output_streams;
	};

	namespace internal
	{

#ifdef DEBUG
		const bool is_debug_build = true;
#else
		const bool is_debug_build = false;
#endif

		std::unique_ptr<Options> options;

		template<typename ...T>
		LOGGER_INLINE void output(const char* color, T... data)
		{
			for (const auto stream: options->output_streams)
			{
				*stream << options->colors.starting_string << color;
				((*stream << data << " "), ...);
				*stream << options->colors.reset_code << std::endl;
			}
		}

		template<OutputSettings O, typename ...T>
		inline constexpr void output_wrapper(const char* color, T... data)
		{
			if constexpr(O == All || (internal::is_debug_build && O == Debug) || (!internal::is_debug_build && O == Release))
			{
				output(color, std::forward<T>(data)...);
			}
		}
	}

	inline std::unique_ptr<Options> make_options(std::vector<std::ostream*> streams = { &std::cout },
		Colors colors = Colors())
	{
		return std::make_unique<Options>(Options(std::move(streams), colors));
	}

	inline void init(std::unique_ptr<Options> opt = make_options())
	{
		internal::options = std::move(opt);
	}



	template<OutputSettings O = Debug, typename ...T>
	inline constexpr void info(T... data)
	{
		internal::output_wrapper<O>(internal::options->colors.info_color, std::forward<T>(data)...);
	}

	template<OutputSettings O = Debug, typename ...T>
	inline constexpr void warn(T... data)
	{
		internal::output_wrapper<O>(internal::options->colors.warn_color, std::forward<T>(data)...);
	}

	template<OutputSettings O = All, typename ...T>
	inline constexpr void success(T... data)
	{
		internal::output_wrapper<O>(internal::options->colors.success_color, std::forward<T>(data)...);
	}

	template<OutputSettings O = All, typename ...T>
	inline constexpr void notify(T... data)
	{
		internal::output_wrapper<O>(internal::options->colors.notify_color, std::forward<T>(data)...);
	}

	template<OutputSettings O = All, typename ...T>
	inline constexpr void error(T... data)
	{
		internal::output_wrapper<O>(internal::options->colors.error_color, std::forward<T>(data)...);
	}

}

#endif //LOGGER_HPP
