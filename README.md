# ArgParser

This C++ library provides a flexible framework for creating custom argument parsers, enabling seamless integration of command-line argument handling into your applications. This library streamlines the process of parsing command-line inputs with intuitive classes and methods.

## Features

* Flexible Argument Handling: Define and customize command-line arguments to suit your application's needs
  
* Support for Various Data Types: Easily handle integer values, strings, and boolean flags as command-line arguments
  
* Multi-Valued Arguments: Handle multiple values for an argument effortlessly
  
* Positional Arguments: Define positional arguments for greater versatility in command-line usage
  
* Error Handling: Ensure robustness with built-in checks for valid input.

## Usage 

```cpp
#include "ArgumentParser.h"

int main(int argc, char** argv) {
    ArgumentParser::ArgParser parser("MyParser");

    parser.AddIntArgument('i', "integer", "Description for integer argument").Default(0);
    parser.AddStringArgument('s', "string", "Description for string argument").Default("default_string");
    parser.AddFlagArgument('f', "flag", "Description for flag argument").Default(false);

    if (parser.Parse(argc, argv)) {
        int64_t intValue = parser.GetIntValue("integer");
        const char* stringValue = parser.GetStringValue("string");
        bool flagValue = parser.GetFlagValue("flag");
        // Handle parsed arguments
    } else {
        std::cout << "Invalid input. Use the following syntax:\n";
        std::cout << parser.HelpDescription() << std::endl;
    }

    return 0;
}

```
