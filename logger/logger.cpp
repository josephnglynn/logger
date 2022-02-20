//
// Created by joseph on 18/11/2021.
//

#include "logger.hpp"

#include <utility>


namespace logger {

	Options::Options(std::vector<std::ostream*> streams) : output_streams(std::move(streams))
	{

	}

}