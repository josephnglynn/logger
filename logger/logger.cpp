//
// Created by joseph on 18/11/2021.
//

#include "logger.hpp"

#include <utility>

namespace logger
{


	Options::Options(std::vector<std::ostream*> streams, Colors colors) : colors(colors), output_streams(std::move(streams))
	{

	}


	Colors::Colors(const char* info,
		const char* warn,
		const char* success,
		const char* notify,
		const char* error,
		const char* starting_string,
		const char* reset_code)
		: info_color(info), warn_color(warn), success_color(success), notify_color(notify), error_color(error),
		  starting_string(starting_string), reset_code(reset_code)
	{

	}
	std::ostream& internal::operator<<(std::ostream& os, const internal::EndLine&)
	{
		os << std::endl;
		return os;
	}

	std::ostream& internal::operator<<(std::ostream& os, const internal::Ends& end_line)
	{
		os << std::ends;
		return os;
	}

	std::ostream& internal::operator<<(std::ostream& os, const internal::Flush& flush)
	{
		os << std::flush;
		return os;
	}
}