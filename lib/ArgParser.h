/*
 * Author: Artem Khudyakov // GitHub: deadnotxaa
 */
#pragma once

#include <vector>
#include <cinttypes>

namespace ArgumentParser {

class ArgParser;

class Argument {
    friend ArgParser;

public:
    Argument& MultiValue(std::size_t = 0);
    Argument& Positional();

protected:
    Argument(char, const char*, const char*);

    char short_name_;
    const char* full_name_ = nullptr;
    const char* description_ = nullptr;

    bool is_default_value_defined_ = false;
    bool is_multi_value_ = false;
};


class IntArgument : public Argument {
    friend ArgParser;

public:
    IntArgument& Default(int64_t);
    IntArgument& StoreValue();
    IntArgument& StoreValues();

private:
    IntArgument(char, const char*, const char*);

    int64_t value_;
    int64_t default_value_;
};


class StringArgument : public Argument {
    friend ArgParser;

public:
    StringArgument& Default(const char*);
    StringArgument& StoreValue();
    StringArgument& StoreValues();

private:
    StringArgument(char, const char*, const char*);

    const char* value_;
    const char* default_value_;
};


class FlagArgument : public Argument {
    friend ArgParser;

public:
    FlagArgument& Default(bool);
    FlagArgument& StoreValue();
    FlagArgument& StoreValues();

private:
    FlagArgument(char, const char*, const char*);

    bool value_;
    bool default_value_;
};


class ArgParser {
public:
    explicit ArgParser(const char*);
    ArgParser(ArgParser& other) = delete;
    ArgParser(ArgParser&& other) = delete;

    bool Parse(int, char**);

    IntArgument& AddIntArgument(char, const char*, const char*);
    StringArgument& AddStringArgument(char, const char*, const char*);
    FlagArgument& AddFlagArgument(char, const char*, const char*);

private:
    [[maybe_unused]] const char* parser_name_ = nullptr;

    std::vector<IntArgument> int_arguments_;
    std::vector<StringArgument> string_arguments_;
    std::vector<FlagArgument> flag_arguments_;
};

} // namespace ArgumentParser