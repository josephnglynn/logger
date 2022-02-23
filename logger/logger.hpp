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
#include <algorithm>
#include <functional>

#define LOGGER_INLINE inline
#ifdef LOGGER_DISABLE_INLINE
#define LOGGER_INLINE
#endif

namespace logger
{
	typedef const char* Color;

	namespace values
	{
		constexpr Color blue = "\u001b[34m";
		constexpr Color yellow = "\u001b[33m";
		constexpr Color green = "\u001b[32m";
		constexpr Color purple = "\u001b[35m";
		constexpr Color red = "\u001b[31m";
		constexpr Color grey = "\u001b[37m";
		constexpr Color starting_string = "==> ";
		constexpr Color reset_code = "\u001b[0m";

		constexpr Color default_interval = "    "; // Note, kept for later
	}

	struct OutputSettings
	{
		OutputSettings(
			Color info = values::blue,
			Color warn = values::yellow,

			Color success = values::green,
			Color notify = values::purple,
			Color error = values::red,

			Color starting_string_color = values::grey,

			Color starting_string = values::starting_string,
			Color reset_code = values::reset_code
		);

		Color info_color;
		Color warn_color;

		Color success_color;
		Color notify_color;
		Color error_color;

		Color starting_string_color;

		const char* starting_string;
		const char* reset_code;
	};

	struct OutputEntry
	{
		OutputEntry(std::ostream* ostream = &std::cout, bool colored_output = true);
		OutputEntry(std::ostream& ostream = std::cout, bool colored_output = true); // Kept for legacy reasons

		inline friend constexpr bool operator==(const OutputEntry& oe1, const OutputEntry& oe2)
		{
			return (oe1.ostream == oe2.ostream); // If it is the same stream, then it shouldn't matter whether colored_output or not
		}

		bool colored_output;
		std::ostream* ostream;
	};

	namespace internal
	{
		class Logger
		{
		public:

			Logger() = default;
			Logger(std::vector<OutputEntry> output_entries);

			inline void add_stream(OutputEntry output_entry)
			{
				output_streams.emplace_back(output_entry);
			}

			inline void remove_stream(OutputEntry output_entry)
			{
				const auto result = std::find(output_streams.begin(), output_streams.end(), output_entry);
				if (result != std::end(output_streams))
				{
					output_streams.erase(result);
				}
			}

			template<typename T>
			friend const Logger& operator<<(const Logger& logger, const T& data)
			{

				for (const auto& output_entry: logger.output_streams)
				{
					if constexpr(std::is_same<T, Color>::value)
					{
						if (output_entry.colored_output) continue;
					}
					*output_entry.ostream << data << " ";
				}
				return logger;
			}

			[[nodiscard]] inline const OutputSettings& get_output_setting() const
			{
				return output_settings;
			}

		private:
			OutputSettings output_settings;
			std::vector<OutputEntry> output_streams;
		};

		std::unique_ptr<Logger> logger_instance;
	}

	enum BuildSettings
	{
		Debug, // DEBUG ONLY
		Release, // RELEASE ONLY,
		All,
	};


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

		template<typename ...T>
		LOGGER_INLINE void output(const OutputSettings& os, Color color, T... data)
		{
			*logger_instance << os.starting_string_color << os.starting_string << color;
			((*logger_instance << data << " "), ...);
			*logger_instance << os.reset_code << logger::endl;
		}

		template<OutputFunction OF>
		inline constexpr Color get_color_from_output_function(const OutputSettings& os)
		{
			if constexpr(OF == Info) return os.info_color;
			if constexpr(OF == Warn) return os.warn_color;

			if constexpr(OF == Success) return os.success_color;
			if constexpr(OF == Notify) return os.notify_color;
			if constexpr(OF == Error) return os.error_color;
		}

		template<BuildSettings O, OutputFunction OF, typename ...T>
		inline constexpr void output_wrapper(T... data)
		{
			if constexpr(O == All || (internal::is_debug_build && O == Debug)
				|| (!internal::is_debug_build && O == Release))
			{
				const OutputSettings& os = logger_instance->get_output_setting();
				output(os, get_color_from_output_function<OF>(os), data...);
			}
		}
	}

	// Init Functions

	inline void init(std::vector<OutputEntry> output_entries = { OutputEntry(std::cout, true) })
	{
		internal::logger_instance = std::make_unique<internal::Logger>(std::move(output_entries));
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

	// Streams

	inline void add_stream(OutputEntry output_entry)
	{
		internal::logger_instance->add_stream(output_entry);
	}

	inline void add_stream(std::ostream& ostream, bool colored_output = true)
	{
		add_stream(OutputEntry(ostream, colored_output));
	}

	inline void remove_stream(OutputEntry output_entry)
	{
		internal::logger_instance->remove_stream(output_entry);
	}

	inline void remove_stream(std::ostream& ostream, bool colored_output = true)
	{
		remove_stream(OutputEntry(ostream, colored_output));
	}



}

#endif //LOGGER_HPP
