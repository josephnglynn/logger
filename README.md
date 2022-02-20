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

logger::success(Args...);  // Default Color = Green, Default Run = Release And Debug
logger::notify(Args...);  // Default Color = Purple, Default Run = Release And Debug
logger::error(Args...);  // Default Color = Red, Default Run = Release And Debug
```


## Example

### Note:
This is a simple example, for more look at `tests/test.cpp`
```c++
#include <iostream> // Note logger.hpp does include <iostream>
#include <fstream>



std::ifstream my_log_file("log.txt");
logger::init(logger::make_options({ &std::cout, &file }));

logger::info("This will only work in debug builds, when DEBUG macro is defined");
logger::info<Release>("This will work in any build");

logger::success("This function will work in any build");
logger::success<DebugOnly>("This function will only work in Debug")
```