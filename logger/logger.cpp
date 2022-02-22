//
// Created by joseph on 18/11/2021.
//

#include "logger.hpp"

namespace logger
{

	OutputSettings::OutputSettings(Color info,
		Color warn,
		Color success,
		Color notify,
		Color error,
		Color starting_string_color,
		const char* starting_string,
		const char* reset_code
	) : info_color(info), warn_color(warn), success_color(success), notify_color(notify), error_color(error),
		starting_string_color(starting_string_color), starting_string(starting_string), reset_code(reset_code) {}

	OutputEntry::OutputEntry(std::ostream& ostream, bool colored_output) : colored_output(colored_output), ostream(ostream) {}

	namespace internal
	{
		Logger::Logger(std::vector<OutputEntry> output_entries) : output_streams(std::move(output_entries)) {}
	}
}