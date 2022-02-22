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
		constexpr const char* grey = "\u001b[37m";
		constexpr const char* starting_string = "==> ";
		constexpr const char* reset_code = "\u001b[0m";

		constexpr const char* default_interval = "    "; // Note, kept for later
	}

	enum BuildSettings
	{
		Debug, // DEBUG ONLY
		Release, // RELEASE ONLY,
		All,
	};

	struct OutputSettings
	{
		explicit OutputSettings(
			const char* info = values::blue,
			const char* warn = values::yellow,

			const char* success = values::green,
			const char* notify = values::purple,
			const char* error = values::red,

			const char* starting_string_color = values::grey,

			const char* starting_string = values::starting_string,
			const char* reset_code = values::reset_code
		);

		const char* info_color;
		const char* warn_color;

		const char* success_color;
		const char* notify_color;
		const char* error_color;

		const char* starting_string_color;

		const char* starting_string;
		const char* reset_code;
	};

	struct OptionEntry
	{
		// Don't mark as explicit so that { std::cout, logger::OutputSettings() } still works.
		OptionEntry(std::ostream& ostream = std::cout, OutputSettings colors = OutputSettings());

		std::ostream& ostream;
		OutputSettings output_settings;
	};

	typedef std::vector<OptionEntry> Options;

	namespace internal
	{

		enum OutputFunction
		{
			Info,
			Warn,

			Success,
			Notify,
			Error
		};

#ifdef DEBUG
		const bool is_debug_build = true;
#else
		const bool is_debug_build = false;
#endif

		std::unique_ptr<Options> options;

		template<typename ...T>
		LOGGER_INLINE void output(const OptionEntry& entry, const char* color, T... data)
		{
			entry.ostream << entry.output_settings.starting_string_color << entry.output_settings.starting_string
						  << color;
			((entry.ostream << data << " "), ...);
			entry.ostream << entry.output_settings.reset_code << std::endl;
		}

		template<BuildSettings O, OutputFunction type, typename ...T>
		inline constexpr void output_wrapper(T... data)
		{
			if constexpr(O == All || (internal::is_debug_build && O == Debug)
				|| (!internal::is_debug_build && O == Release))
			{
				for (const auto& entry: *options)
				{
					if constexpr(type == OutputFunction::Info) output(entry, entry.output_settings.info_color, data...);
					if constexpr(type == OutputFunction::Warn) output(entry, entry.output_settings.warn_color, data...);
					if constexpr(type == OutputFunction::Success)
						output(entry,
							entry.output_settings.success_color,
							data...);
					if constexpr(type == OutputFunction::Notify)
						output(entry,
							entry.output_settings.notify_color,
							data...);
					if constexpr(type == OutputFunction::Error)
						output(entry,
							entry.output_settings.error_color,
							data...);
				}
			}
		}
	}

	// Helpers

	inline std::ostream& endl(std::ostream& os)
	{
		os << std::endl;
		return os;
	}

	inline std::ostream& ends(std::ostream& os)
	{
		os << std::ends;
		return os;
	}

	inline std::ostream& flush(std::ostream& os)
	{
		os << std::flush;
		return os;
	}

	inline std::unique_ptr<Options> make_options(std::vector<OptionEntry> entries = { OptionEntry() })
	{
		return std::make_unique<Options>(Options(std::move(entries)));
	}


	// Init Functions

	inline void init(std::unique_ptr<Options> opt = make_options())
	{
		internal::options = std::move(opt);
	}

	// Output functions

	template<BuildSettings O = Debug, typename ...T>
	inline constexpr void info(T... data)
	{
		internal::output_wrapper<O, internal::OutputFunction::Info>(data...);
	}

	template<BuildSettings O = Debug, typename ...T>
	inline constexpr void warn(T... data)
	{
		internal::output_wrapper<O, internal::OutputFunction::Warn>(data...);
	}

	template<BuildSettings O = All, typename ...T>
	inline constexpr void success(T... data)
	{
		internal::output_wrapper<O, internal::OutputFunction::Success>(data...);
	}

	template<BuildSettings O = All, typename ...T>
	inline constexpr void notify(T... data)
	{
		internal::output_wrapper<O, internal::OutputFunction::Notify>(data...);
	}

	template<BuildSettings O = All, typename ...T>
	inline constexpr void error(T... data)
	{
		internal::output_wrapper<O, internal::OutputFunction::Error>(data...);
	}

}

#endif //LOGGER_HPP
