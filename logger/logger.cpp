//
// Created by joseph on 18/11/2021.
//

#include "logger.hpp"

#include <utility>

namespace logger
{

	OutputSettings::OutputSettings(const char* info,
		const char* warn,
		const char* success,
		const char* notify,
		const char* error,
		const char* starting_string_color,
		const char* starting_string,
		const char* reset_code)
		: info_color(info), warn_color(warn), success_color(success), notify_color(notify), error_color(error),
		  starting_string_color(starting_string_color), starting_string(starting_string), reset_code(reset_code)
	{
	}

	OptionEntry::OptionEntry(std::ostream& ostream, OutputSettings colors) : ostream(ostream), output_settings(colors)
	{
	}


}