# logger
This is a cpp logger for all my projects

## Usage

When called without any value for `OuputSettings` or with it as `DebugOnly`, there will only be output in debug builds when the `DEBUG` macro is defined.

NOTE: `Release` means `RELEASE_AND_DEBUG`

```c++

int state = logger::DebugOnly; //0


//NOTE: without optional logger::debug or logger::release info and warn default to debug only and error and success default to release and debug

//OUTPUT: Whoa state: 0
logger::info("Whoa", "state: ", state); //Blue color

//OUTPUT: Whoa state: 0
logger::success("Whoa", "state: ", state); //Green color

//OUTPUT: Whoa state: 0
logger::warn("Whoa", "state: ", state); //Yellow color

//OUTPUT: Whoa state: 0
logger::error("Whoa", "state: ", state); //Red color


//Release And Debug Output
logger::info<logger::Release>("This is being called in release");

```