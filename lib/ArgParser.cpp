#include "ArgParser.h"

ArgumentParser::ArgParser::ArgParser(const char* value) {
    this->parser_name_ = value;
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv) {

}

ArgumentParser::IntArgument& ArgumentParser::ArgParser::AddIntArgument(char short_name, const char* full_name, const char* description) {
    int_arguments_.emplace_back(IntArgument(short_name, full_name, description));

    return *int_arguments_.end();
}

ArgumentParser::StringArgument& ArgumentParser::ArgParser::AddStringArgument(char short_name, const char* full_name, const char* description) {
    string_arguments_.emplace_back(StringArgument(short_name, full_name, description));

    return *string_arguments_.end();
}

ArgumentParser::FlagArgument& ArgumentParser::ArgParser::AddFlagArgument(char short_name, const char* full_name, const char* description) {
    flag_arguments_.emplace_back(FlagArgument(short_name, full_name, description));

    return *flag_arguments_.end();
}

ArgumentParser::Argument::Argument(char short_name, const char* full_name, const char* description)
    :
    short_name_(short_name),
    full_name_(full_name),
    description_(description)
{
}

ArgumentParser::Argument &ArgumentParser::Argument::Positional() {
    return *this;
}

ArgumentParser::Argument &ArgumentParser::Argument::MultiValue(std::size_t) {
    return *this;
}

ArgumentParser::IntArgument::IntArgument(char short_name, const char* full_name, const char* description)
    : Argument(short_name, full_name, description)
{
}

ArgumentParser::IntArgument& ArgumentParser::IntArgument::Default(int64_t value) {
    this->default_value_ = value;
    this->is_default_value_defined_ = true;

    return *this;
}

ArgumentParser::IntArgument &ArgumentParser::IntArgument::StoreValue() {
    return *this;
}

ArgumentParser::IntArgument &ArgumentParser::IntArgument::StoreValues() {
    return *this;
}

ArgumentParser::StringArgument::StringArgument(char short_name, const char* full_name, const char* description)
        : Argument(short_name, full_name, description)
{
}

ArgumentParser::StringArgument& ArgumentParser::StringArgument::Default(const char* value) {
    this->default_value_ = value;
    this->is_default_value_defined_ = true;

    return *this;
}

ArgumentParser::StringArgument &ArgumentParser::StringArgument::StoreValue() {
    return *this;
}

ArgumentParser::StringArgument &ArgumentParser::StringArgument::StoreValues() {
    return *this;
}

ArgumentParser::FlagArgument::FlagArgument(char short_name, const char* full_name, const char* description)
        : Argument(short_name, full_name, description)
{
}

ArgumentParser::FlagArgument& ArgumentParser::FlagArgument::Default(bool value) {
    this->default_value_ = value;
    this->is_default_value_defined_ = true;

    return *this;
}

ArgumentParser::FlagArgument &ArgumentParser::FlagArgument::StoreValue() {
    return *this;
}

ArgumentParser::FlagArgument &ArgumentParser::FlagArgument::StoreValues() {
    return *this;
}
