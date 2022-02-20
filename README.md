# logger
This is a cpp logger for all my projects

## Usage

Before starting using any functions, call `logger::init()`, which takes an optional parameter of type `std::unique_ptr<logger::Options>`
 
### Note
Although you can construct  `std::unique_ptr<logger::Options>`  with the  `std::make_unique<logger::Options>()`, it becomes a long line due to the optional call to the `logger::Options` constructor. Therefore, I recommend using the provided `logger::make_options(streams, colors)` function

Now just call any of the following functions below

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

Due to the templated nature of this library, `std::endl` and its friends `std::flush` and `std::ends`, do not want to work, this library provides the following, `logger::endl`, `logger::flush`, `logger::ends` as a drop in replacement, when using functions in this library.
E.G: `logger::notify("Whoa, lets do an std::endl", std::endl)` becomes `logger::notify("Whoa, lets do an std::endl", logger::endl)` 
