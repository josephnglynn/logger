//
// Created by joseph on 20/02/2022.
//
#include <logger/logger.hpp>
#include <fstream>
#include <functional>

inline void start_test(const char* test_name)
{
	std::cout << std::endl << "########### Starting " << test_name << " ###########" << std::endl << std::endl;
}

inline void end_test(const char* test_name)
{
	std::cout << std::endl << "########### Ending " << test_name << " ###########" << std::endl << std::endl;
}

inline void basic_color_test()
{
	logger::info("INFO TEST");
	logger::warn("WARN TEST");

	logger::success("SUCCESS TEST");
	logger::notify("NOTIFY TEST");
	logger::error("ERROR TEST");
}

inline void file_test()
{
	std::fstream file("log.txt", std::ios_base::in | std::ios_base::out | std::ios_base::trunc);

	logger::info("OUTPUT WITH FILE OUTPUT TOO");
	logger::warn(
		"But be careful, as currently it also writes output_settings as well, however this should not be a problem if you intend to cat the file in your terminal");

	uint32_t data = 123456789;
	logger::notify("Some important data: ", data);
	logger::warn(
		"trying 'cat log.txt' and you should see the output_settings work, now try editing it, you should see that its a bit messy",
		logger::endl);
}

int main()
{
	// Init
	logger::init();

	std::tuple<std::function<void()>, const char*> tests[] = {
		{ basic_color_test, "Basic color test" },
		{ file_test, "File test" },
	};

	for (const auto& test: tests)
	{
		start_test(std::get<1>(test));
		std::get<0>(test)();
		end_test(std::get<1>(test));
	}
}