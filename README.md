# logger

This is a cpp logger for all my projects

## Usage

Before starting using any functions, call `logger::init()` which has the following signatures.

```c++
void init(const bool use_std_out = true); // Where use_std_out means init logger with one ostream output of std::cout
void init(const OutputEntry& output_entry, const bool use_std_out = true); // Where use_std_out is same as before, and OutputEntry is an extra output stream to add to logger
void init(std::vector<OutputEntry> output_entries, const bool use_std_out = true); // Where use_std_out is same as before, and there is a vector of OutputEntries
```

### Note

A `OutputEntry` with only members is an ostream with a boolean for whether color should be outputted.

## What is a `scoped_stream || scoped_streams`

`scoped_stream` or `scoped_streams` can be used instead of needing to manually add and remove a `OutputEntry` from the
logger. When the object is first constructed it adds the stream, and when destroyed, removes the stream. Take the
example below.

```c++
void some_function()
{
	std::ofstream log("log.txt");
        
        // The ofstream is added to the logger
	logger::scoped_stream scoped_logger (file);
	
	// Now when we call any of the output functions from logger::*, it will write to the file as well as other streams
	logger::notify("Writing to log.txt and other streams now!!!");
	
	// We can also use the scoped_logger to output
	scoped_logger << "This will only be outputted to the log.txt file";
} // The file is removed from the logger here


void basic_example_with_scoped_streams() 
{
	std::vector<logger::OutputEntry> files = {
		{ new std::ofstream("log_1.txt") },
		{ new std::ofstream("log_2.txt") },
		{ new std::ofstream("log_3.txt") }
	}; 
        // Note how here we must create OutputEntry objects, as the constructor doesn't have optional params.
	
	logger::scoped_streams scoped_logger(files);
	
	// Again calls to any functions in logger::* will output with this file as well
	logger::notify("Outputting to 3 extra log files 👌");
	
	// And calling operator<< will only output to files
	scoped_logger << "Whoa only the 3 logged files";
}
```

### Definition

#### scoped_stream
```c++
class scoped_stream
{
public:
	// Adds stream to logger
	scoped_stream(OutputEntry oe) ;
	scoped_stream(std::ostream* ostream, bool coloured_output = true);
	scoped_stream(std::ostream& ostream, bool coloured_output = true);
	
	~scoped_stream(); // Removes stream from logger

	// What allows you to do << on the scoped_stream object
	template<typename T>
	friend const scoped_stream& operator<<(const scoped_stream& temp_stream, const T& data);

	private:
		const OutputEntry m_output_entries;
	};

```
#### scoped_streams

```c++

class scoped_streams
{

public:
    // Adds stream to logger
    scoped_streams(std::vector<OutputEntry> output_entries);
    
    // Removes stream to logger
    ~scoped_streams();
    
    // Adds << operator
    template<typename T>
    friend const scoped_streams& operator<<(const scoped_streams& temp_stream, const T& data);
    
private:
    const std::vector<OutputEntry> m_output_entries;
};
```


## Output Functions

```c++
logger::info(Args...);  // Default Color = Blue, Default Run = Debug Only
logger::warn(Args...);  // Default Color = Yellow, Default Run = Debug Only

logger::success(Args...);  // Default Color = Green, Default Run = Release And Debug ( All )
logger::notify(Args...);  // Default Color = Purple, Default Run = Release And Debug ( All )
logger::error(Args...);  // Default Color = Red, Default Run = Release And Debug ( All )
```

## Other Functions

```c++
// Adds streams to logger
void add_stream(OutputEntry output_entry);
void add_stream(std::ostream& ostream, bool colored_output = true);

// Remove streams from logger
void remove_stream(OutputEntry output_entry);
void remove_stream(std::ostream& ostream, bool colored_output = true);
```

## Example

### Note:

This is a simple example, for more look at `tests/test.cpp`

```c++
#include <fstream>
#include <logger/logger.hpp>

int main()
{
    std::ofstream my_log_file("log.txt")
    const auto log = logger::init( my_log_file );
	
    logger::info("This will only work in debug builds, when DEBUG macro is defined");
    logger::info<logger::Release>("This will work in only release builds");
    logger::info<logger::All>("This function will work in any build")
    
    logger::success("This function will work in any build");
    logger::success<logger::Debug>("This function will only work in Debug");
    logger::success<logger::Release>("This function will only work in release");	
	
    logger::notify("Exiting now..."); // Called in any build
}
```

## Quirks

Due to the templated nature of this library, `std::endl` and its friends `std::flush` and `std::ends`, do not want to
work, this library provides the following, `logger::endl`, `logger::flush`, `logger::ends` as a drop in replacement,
when using functions in this library. E.G: `logger::notify("Whoa, lets do an std::endl", std::endl)`
becomes `logger::notify("Whoa, lets do an std::endl", logger::endl)`

## Types

Note, the following types have their implementation and constexpr and inline qualifiers removed

### Terminal Code

This object specifies the color code

```c++
struct TerminalCode
{
	TerminalCode() = default;
	constexpr TerminalCode(const char* color) : value(color) {}

	friend std::ostream& operator<<(std::ostream& ostream, const TerminalCode& tc);

	const char* value;
};
```


### Output Settings

This object specifies the colours to use with the logger
```c++
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
    );
    
    TerminalCode info_color;
    TerminalCode warn_color;
    
    TerminalCode success_color;
    TerminalCode notify_color;
    TerminalCode error_color;
    
    TerminalCode starting_string_color;
    
    const char* starting_string;
    TerminalCode reset_code;
};

```

### Output Entry

This object describes which ostream to add to the logger and whether colors should be outputted.

```c++
struct OutputEntry
{
    OutputEntry(std::ostream* ostream = &std::cout, bool colored_output = true);
    OutputEntry(std::ostream& ostream = std::cout, bool colored_output = true);
    
    friend bool operator==(const OutputEntry& oe1, const OutputEntry& oe2);
    
    bool colored_output;
    std::ostream* ostream;
};
```

### BuildSettings 

Specifies when to acctually output

```c++
enum BuildSettings
{
	Debug,   // DEBUG ONLY
	Release, // RELEASE ONLY,
	All,     // ANY BUILD
};
```

## Values

The `values` namespace in `logger`, contains the default colors for ansi terminals.
```c++

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
    constexpr const char* default_interval = "    "; 
}

```
