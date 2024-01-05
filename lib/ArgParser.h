/*
 * Author: Artem Khudyakov // GitHub: deadnotxaa
 */
#pragma once

#include <vector>
#include <cinttypes>
#include <cstring>
#include <cctype>
#include <algorithm>

namespace ArgumentParser {

class ArgParser;

// Base class for arguments
class Argument {
    friend ArgParser;

public:
    Argument& MultiValue(std::size_t = 0);
    Argument& Positional();

protected:
    Argument(char, const char*, const char*);

    char short_name_{};
    const char* full_name_{};
    const char* description_{};

    std::size_t min_args_count_ = 0;
    std::size_t args_counter_ = 0;

    bool is_value_defined_ = false;
    bool is_default_value_defined_ = false;
    bool is_multi_value_ = false;
    bool is_positional_ = false;
    bool is_external_storage_ = false;
    bool is_multi_value_external_storage_ = false;
};


class IntArgument : public Argument {
    friend ArgParser;

public:
    IntArgument& Default(int64_t);
    IntArgument& StoreValue(int64_t&);
    IntArgument& StoreValues(std::vector<int64_t>&);

private:
    IntArgument(char, const char*, const char*);

    int64_t value_{};
    int64_t default_value_{};
    int64_t* external_storage_{};
    std::vector<int64_t> multi_value_storage_{};
    std::vector<int64_t>* multi_value_external_storage_{};
};


class StringArgument : public Argument {
    friend ArgParser;

public:
    StringArgument& Default(const char*);
    StringArgument& StoreValue(const char*&);
    StringArgument& StoreValues(std::vector<const char*>&);

private:
    StringArgument(char, const char*, const char*);

    const char* value_{};
    const char* default_value_{};

    const char** external_storage_{};
    std::vector<const char*> multi_value_storage_{};
    std::vector<const char*>* multi_value_external_storage_{};
};


class FlagArgument : public Argument {
    friend ArgParser;

public:
    FlagArgument& Default(bool);
    FlagArgument& StoreValue(bool&);
    FlagArgument& StoreValues(std::vector<bool>&);

private:
    FlagArgument(char, const char*, const char*);

    bool value_{};
    bool default_value_{};

    bool* external_storage_{};
    std::vector<bool> multi_value_storage_{};
    std::vector<bool>* multi_value_external_storage_{};
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

    bool IsIntArgument(char = '\0', const char* = nullptr);
    bool IsStringArgument(char = '\0', const char* = nullptr);
    bool IsFlagArgument(char = '\0', const char* = nullptr);

    void AddIntArgumentValue(char, int64_t);
    void AddStringArgumentValue(char, const char*);
    void AddFlagArgumentValue(char, bool);

    void AddIntArgumentValue(const char*, int64_t);
    void AddStringArgumentValue(const char*, const char*);
    void AddFlagArgumentValue(const char*, bool);

    const char* GetPositionalIntArgumentName();
    const char* GetPositionalStringArgumentName();

    bool IsIntPositionalArgument();
    bool CheckParserInput();

private:
    const char* parser_name_{};

    std::vector<IntArgument> int_arguments_;
    std::vector<StringArgument> string_arguments_;
    std::vector<FlagArgument> flag_arguments_;
};

} // namespace ArgumentParser