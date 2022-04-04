//
// Created by joseph on 20/02/2022.
//
#include <logger/logger.hpp>
#include <fstream>
#include <functional>
#include <thread>

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
	std::ofstream file("log.txt");
	logger::scoped_stream temp_logger(file);

	logger::info("OUTPUT WITH FILE OUTPUT TOO");
	logger::warn(
		"But be careful, as currently it also writes m_output_settings as well, however this should not be a problem if you intend to cat the file in your terminal");

	uint32_t data = 123456789;
	logger::notify("Some important data: ", data);
	logger::warn(
		"trying 'cat log.txt' and you should see the m_output_settings work, now try editing it, you should see that its a bit messy",
		logger::endl);

	logger::info("The following line will only be outputted to the file");
	temp_logger << "Temporary output";
}

inline void multiple_streams()
{

	{
		std::vector<logger::OutputEntry> files = {
			{ new std::ofstream("ms1.txt") },
			{ new std::ofstream("ms2.txt") },
			{ new std::ofstream("ms3.txt") }
		};

		logger::scoped_streams temp_logger(files);
		std::ofstream non_temp("non_temp.txt");
		logger::add_stream(non_temp);

		logger::notify(
			"You should see this output in the files, ms1.txt, ms2.txt, ms3.txt, non_temp.txt and standard output, as we are calling a function");
		logger::warn("You should only see the next output in ms1.txt, ms2.txt and ms3.txt");
		temp_logger << "You should see this output only in the ms*.txt files";

		// Cleanup
		for (auto f: files) delete f.ostream;
		logger::remove_stream(non_temp);
	}

}

int main()
{
	// Init
	std::ofstream my_log_file("log.txt");
	logger::init(my_log_file);
	logger::init(false); // Should not overwrite


	std::tuple<std::function<void()>, const char*> tests[] = {
		{ basic_color_test, "Basic color test" },
		{ file_test, "File test" },
		{ multiple_streams, "Multiple streams test" },
	};

	for (const auto& test: tests)
	{
		start_test(std::get<1>(test));
		std::get<0>(test)();
		end_test(std::get<1>(test));
	}
}