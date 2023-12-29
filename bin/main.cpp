#include "lib/ArgParser.h"
#include <iostream>

int main(int argc, char** argv) {
    ArgumentParser::ArgParser parser("cucumber");

    parser.AddStringArgument('a', "append", "description").Default("a");
    parser.AddIntArgument('b', "extract", "description");
    parser.AddFlagArgument('c', "merge", "description");
    parser.Parse(argc, argv);

    std::cout << "complete" << std::endl;
    return 0;
}
