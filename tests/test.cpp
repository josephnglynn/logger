//
// Created by joseph on 20/02/2022.
//
#include <logger/logger.hpp>
#include <fstream>

inline void line_break()
{
	std::cout << std::endl << "###########" << std::endl << std::endl;
}

int main()
{
	logger::init();

	logger::info("INFO TEST");
	logger::warn("WARN TEST");

	logger::success("SUCCESS TEST");
	logger::notify("NOTIFY TEST");
	logger::error("ERROR TEST");

	line_break();

	std::ofstream file("output");
	logger::init(logger::make_options({ &std::cout, &file }, logger::Colors()));
	logger::info("OUTPUT WITH FILE OUTPUT TOO");

	line_break();

	logger::init()
}