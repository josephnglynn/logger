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

	struct TerminalCode
	{
		TerminalCode() = default;
		constexpr TerminalCode(const char* color) : value(color) {}

		inline friend std::ostream& operator<<(std::ostream& ostream, const TerminalCode& tc) {
			ostream << tc.value;
			return ostream;
		}

		const char* value;
	};

	namespace values
	{
		constexpr TerminalCode blue = "\u001b[34m";
		constexpr TerminalCode yellow = "\u001b[33m";
		constexpr TerminalCode green = "\u001b[32m";
		constexpr TerminalCode purple = "\u001b[35m";
		constexpr TerminalCode red = "\u001b[31m";
		constexpr TerminalCode grey = "\u001b[37m";

		constexpr const char* starting_string = "==> ";
		constexpr TerminalCode reset_code = "\u001b[0m";
		constexpr const char* default_interval = "    "; // Note, kept for later
	}

	struct OutputSettings
	{
		OutputSettings(
			TerminalCode info = values::blue,
			TerminalCode warn = values::yellow,

			TerminalCode success = values::green,
			TerminalCode notify = values::purple,
			TerminalCode error = values::red,

			TerminalCode starting_string_color = values::grey,

			const char* starting_string = values::starting_string,
			TerminalCode reset_code = values::reset_code
		) : info_color(info), warn_color(warn), success_color(success), notify_color(notify), error_color(error),
			starting_string_color(starting_string_color), starting_string(starting_string), reset_code(reset_code)
		{
		}

		TerminalCode info_color;
		TerminalCode warn_color;

		TerminalCode success_color;
		TerminalCode notify_color;
		TerminalCode error_color;

		TerminalCode starting_string_color;

		const char* starting_string;
		TerminalCode reset_code;
	};

	struct OutputEntry
	{
		OutputEntry(std::ostream* ostream = &std::cout, bool colored_output = true)
			: colored_output(colored_output), ostream(ostream)
		{
		}

		OutputEntry(std::ostream& ostream = std::cout, bool colored_output = true)
			: colored_output(colored_output), ostream(&ostream)
		{
		} // Kept for legacy reasons

		inline friend constexpr bool operator==(const OutputEntry& oe1, const OutputEntry& oe2)
		{
			return (oe1.ostream
				== oe2.ostream); // If it is the same stream, then it shouldn't matter whether colored_output or not
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
			Logger(std::vector<OutputEntry> output_entries) : m_output_streams(std::move(output_entries))
			{
			}

			inline void add_stream(OutputEntry output_entry)
			{
				m_output_streams.emplace_back(output_entry);
			}

			inline void remove_stream(OutputEntry output_entry)
			{
				const auto result = std::find(m_output_streams.begin(), m_output_streams.end(), output_entry);
				if (result != std::end(m_output_streams))
				{
					m_output_streams.erase(result);
				}
			}

			template<typename T>
			inline constexpr void output(const OutputEntry& oe, const T& data) const
			{
				if constexpr(std::is_same<T, TerminalCode>::value)
				{
					if (!oe.colored_output) return;
				}
				*oe.ostream << data;
			}

			template<typename T>
			inline friend const Logger& operator<<(const Logger& logger, const T& data)
			{
				for (const auto& oe: logger.m_output_streams)
				{
					logger.output<T>(oe, data);
				}
				return logger;
			}

			template<typename T>
			inline void output_to_a_stream(const OutputEntry entry, const T& data) const
			{
				output<T>(entry, data);
			}

			template<typename T>
			inline void output_to_select_streams(const std::vector<OutputEntry>& entries, const T& data) const
			{
				for (const auto& oe: entries)
				{
					output<T>(oe, data);
				}
			}

			[[nodiscard]] inline const OutputSettings& get_output_setting() const
			{
				return m_output_settings;
			}

		private:
			OutputSettings m_output_settings;
			std::vector<OutputEntry> m_output_streams;
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
		LOGGER_INLINE void output(const OutputSettings& os, TerminalCode color, T... data)
		{
			*logger_instance << os.starting_string_color << os.starting_string << color;
			((*logger_instance << data << " "), ...);
			*logger_instance << os.reset_code << logger::endl;
		}

		template<OutputFunction OF>
		inline constexpr TerminalCode get_color_from_output_function(const OutputSettings& os)
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

		inline OutputEntry make_output_entry_with_cout()
		{
			return { std::cout, true };
		}
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

	class scoped_stream
	{

	public:
		explicit scoped_stream(OutputEntry oe) : m_output_entries(oe)
		{
		}

		explicit scoped_stream(std::ostream* ostream, bool coloured_output = true) : m_output_entries({ ostream,coloured_output })
		{
			add_stream(m_output_entries);
		}

		explicit scoped_stream(std::ostream& ostream, bool coloured_output = true) : m_output_entries({ ostream,coloured_output })
		{
			add_stream(m_output_entries);
		}

		~scoped_stream()
		{
			remove_stream(m_output_entries);
		}

		template<typename T>
		inline friend const scoped_stream& operator<<(const scoped_stream& temp_stream, const T& data)
		{
			internal::logger_instance->output_to_a_stream(temp_stream.m_output_entries, data);
			return temp_stream;
		}

	private:
		const OutputEntry m_output_entries;
	};

	class scoped_streams
	{

	public:
		explicit scoped_streams(std::vector<OutputEntry> output_entries) : m_output_entries(std::move(output_entries))
		{
			for (const auto& oe: m_output_entries)
			{
				add_stream(oe);
			}
		}

		~scoped_streams()
		{
			for (const auto& oe: m_output_entries)
			{
				remove_stream(oe);
			}
		}

		template<typename T>
		inline friend const scoped_streams& operator<<(const scoped_streams& temp_stream, const T& data)
		{
			internal::logger_instance->output_to_select_streams(temp_stream.m_output_entries, data);
			return temp_stream;
		}

	private:
		const std::vector<OutputEntry> m_output_entries;
	};

	typedef scoped_stream use_a_temporary_stream;
	typedef scoped_streams use_temporary_streams;

	// Init Functions

	inline void init(const bool use_std_out = true)
	{
		std::vector<OutputEntry> entries =
			use_std_out ? std::vector<OutputEntry>({ internal::make_output_entry_with_cout() }) : std::vector<
				OutputEntry>();
		internal::logger_instance = std::make_unique<internal::Logger>(entries);
	}

	inline scoped_stream init(const OutputEntry& output_entry, const bool use_std_out)
	{
		init(use_std_out);
		return scoped_stream(output_entry);
	}

	inline scoped_streams init(std::vector<OutputEntry> output_entries, const bool use_std_out)
	{
		init(use_std_out);
		return scoped_streams(std::move(output_entries));
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
