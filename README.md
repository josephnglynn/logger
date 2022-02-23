# logger

This is a cpp logger for all my projects

## Usage

Before starting using any functions, call `logger::init()` which has the following signatures.

```c++
inline void init(const bool use_std_out = true); // Where use_std_out means init logger with one ostream output of std::cout
inline scoped_stream init(const OutputEntry& output_entry, const bool use_std_out) // Where use_std_out is same as before, and OutputEntry is an extra output stream to add to logger
inline scoped_streams init(std::vector<OutputEntry> output_entries, const bool use_std_out) // Where use_std_out is same as before, and there is a vector of OutputEntries
```

### Note

A `OutputEntry` with only members is an ostream with a boolean for whether color should be outputted.

```c++
struct OutputEntry
{
	// Constructors, Operators...
	bool colored_output;
	std::ostream* ostream;
};
```

## What is a `scoped_stream || scoped_streams`

`scoped_stream` or `scoped_streams` can be used instead of needing to manually add and remove a `OutputEntry` from the logger. When the object is first constructed it adds the stream, and when destroyed, removes the stream. Take the example below.
```c++
void some_function()
{
	// Note, it is optional whether you want to construct an OutputEntry, as just passing ostream and / or the bool, will construct it for you anyway.
	std::ofstream log("log.txt");
	logger::scoped_stream scoped_logger (file); // When called here, the ofstream is added to the logger
	
	// Now when we call any of the functions from logger::* which output, it will write to the file as well
	logger::notify("Writing to log.txt now!!!");
	
	// We can also use the scoped_logger to output
	scoped_logger << "This will only be outputted to the log.txt file";
} // The file is removed from the logger here


void basic_example_with_scoped_streams() 
{
	std::vector<logger::OutputEntry> files = {
		{ new std::ofstream("log_1.txt") },
		{ new std::ofstream("log_2.txt") },
		{ new std::ofstream("log_3.txt") }
	}; // Note how here we must create OutputEntry objects, but the coloured output field is still optional
	
	logger::scoped_streams scoped_logger(files);
	
	// Again calls to any functions in logger::* will output with this file as well
	logger::notify("Outputting to 3 extra log files 👌");
	
	// And calling operator<< will only output to files
	scoped_logger << "Whoa only the 3 logged files";
}
```

## Functions

```c++
logger::init(std::unique_ptr<logger::Options> = logger::make_options()) // Init Library

logger::info(Args...);  // Default Color = Blue, Default Run = Debug Only
logger::warn(Args...);  // Default Color = Yellow, Default Run = Debug Only

logger::success(Args...);  // Default Color = Green, Default Run = Release And Debug ( All )
logger::notify(Args...);  // Default Color = Purple, Default Run = Release And Debug ( All )
logger::error(Args...);  // Default Color = Red, Default Run = Release And Debug ( All )
```

## Example

### Note:

This is a simple example, for more look at `tests/test.cpp`

```c++
#include <iostream> // Note logger.hpp does include <iostream>
#include <fstream>
#include <logger/logger.hpp>

std::ofstream my_log_file("log.txt");
logger::init(logger::make_options({ &std::cout, &file }));

logger::info("This will only work in debug builds, when DEBUG macro is defined");
logger::info<logger::Release>("This will work in only release builds");
logger::info<logger::All>("This function will work in any build")

logger::success("This function will work in any build");
logger::success<logger::Debug>("This function will only work in Debug");
logger::success<logger::Release>("This function will only work in release");
```

## Quirks

Due to the templated nature of this library, `std::endl` and its friends `std::flush` and `std::ends`, do not want to
work, this library provides the following, `logger::endl`, `logger::flush`, `logger::ends` as a drop in replacement,
when using functions in this library. E.G: `logger::notify("Whoa, lets do an std::endl", std::endl)`
becomes `logger::notify("Whoa, lets do an std::endl", logger::endl)` 
