#include "ArgParser.h"

ArgumentParser::ArgParser::ArgParser(const char* value) {
    parser_name_ = value;
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv) {

    // Looks for help request
    for (int i = 0; i < argc; ++i) {
        if (!std::strcmp(argv[i], "-h") || !std::strcmp(argv[i], "--help")) {
            // call help function
        }
    }

    // Finds short parameters
    for (int i = 0; i < argc; ++i) {
        if (!std::strncmp(argv[i], "--", 2) || std::strncmp(argv[i], "-", 1) != 0) {
            continue;
        }

        for (int j = 1; j < std::strlen(argv[i]); ++j) {
            if (IsIntArgument(argv[i][j])) {
                AddIntArgumentValue(argv[i][j], std::strtoll(argv[i + 1], nullptr, 10));
            }
            if (IsStringArgument(argv[i][j])) {
                AddStringArgumentValue(argv[i][j], argv[i + 1]);
            }
            if (IsFlagArgument(argv[i][j])) {
                AddFlagArgumentValue(argv[i][j], true);
            }
        }
    }

    // Finds long parameters
    for (int i = 0; i < argc; ++i) {
        if (std::strncmp(argv[i], "--", 2) != 0) {
            continue;
        }

        const char* full_name = std::strtok(argv[i], "=");
        const char* value = std::strtok(nullptr, "=");

        if (IsIntArgument('\0', full_name)) {
            AddIntArgumentValue(full_name, std::strtoll(value, nullptr, 10));
        }
        if (IsStringArgument('\0', full_name)) {
            AddStringArgumentValue(full_name, value);
        }
        if (IsFlagArgument('\0', full_name)) {
            AddFlagArgumentValue(full_name, !std::strcmp("true", value));
        }
    }

    // Finds positional parameters
    for (int i = 1; i < argc; ++i) {
        if (!std::strncmp(argv[i], "--", 2) || !std::strncmp(argv[i], "-", 1)) {
            continue;
        }

        if (IsIntPositionalArgument()) {
            if (std::all_of(argv[i], argv[i] + std::strlen(argv[i]), ::isdigit)) {
                AddIntArgumentValue(GetPositionalIntArgumentName(), std::strtoll(argv[i], nullptr, 10));
            }
        } else {
            AddStringArgumentValue(GetPositionalStringArgumentName(), argv[i]);
        }
    }

    return CheckParserInput();
}

ArgumentParser::IntArgument& ArgumentParser::ArgParser::AddIntArgument(char short_name, const char* full_name, const char* description) {
    int_arguments_.emplace_back(IntArgument(short_name, full_name, description));

    return int_arguments_[int_arguments_.size() - 1];
}

ArgumentParser::StringArgument& ArgumentParser::ArgParser::AddStringArgument(char short_name, const char* full_name, const char* description) {
    string_arguments_.emplace_back(StringArgument(short_name, full_name, description));

    return string_arguments_[string_arguments_.size() - 1];
}

ArgumentParser::FlagArgument& ArgumentParser::ArgParser::AddFlagArgument(char short_name, const char* full_name, const char* description) {
    flag_arguments_.emplace_back(FlagArgument(short_name, full_name, description));

    return flag_arguments_[flag_arguments_.size() - 1];
}

bool ArgumentParser::ArgParser::IsIntArgument(char short_name, const char* full_name) {
    if (full_name == nullptr) {
        for (auto& i : int_arguments_) {
            if (i.short_name_ == short_name) {
                return true;
            }
        }
    } else {
        for (auto& i : int_arguments_) {
            if (i.full_name_ == full_name) {
                return true;
            }
        }
    }

    return false;
}

bool ArgumentParser::ArgParser::IsStringArgument(char short_name, const char* full_name) {
    if (full_name == nullptr) {
        for (auto& i : string_arguments_) {
            if (i.short_name_ == short_name) {
                return true;
            }
        }
    } else {
        for (auto& i : string_arguments_) {
            if (i.full_name_ == full_name) {
                return true;
            }
        }
    }

    return false;
}

bool ArgumentParser::ArgParser::IsFlagArgument(char short_name, const char* full_name) {
    if (full_name == nullptr) {
        for (auto& i : flag_arguments_) {
            if (i.short_name_ == short_name) {
                return true;
            }
        }
    } else {
        for (auto& i : flag_arguments_) {
            if (i.full_name_ == full_name) {
                return true;
            }
        }
    }

    return false;
}

void ArgumentParser::ArgParser::AddIntArgumentValue(char short_name, int64_t value) {
    for (auto& i : int_arguments_) {
        if (i.short_name_ == short_name) {
            if (i.is_multi_value_) {
                i.multi_value_storage_.emplace_back(value);
                if (i.is_multi_value_external_storage_) {
                    i.multi_value_external_storage_->emplace_back(value);
                }
            } else {
                i.value_ = value;
            }

            if (i.is_external_storage_) {
                *i.external_storage_ = value;
            }

            i.is_value_defined_ = true;
            ++i.args_counter_;
        }
    }
}

void ArgumentParser::ArgParser::AddStringArgumentValue(char short_name, const char* value) {
    for (auto& i : string_arguments_) {
        if (i.short_name_ == short_name) {
            if (i.is_multi_value_) {
                i.multi_value_storage_.emplace_back(value);
                if (i.is_multi_value_external_storage_) {
                    i.multi_value_external_storage_->emplace_back(value);
                }
            } else {
                i.value_ = value;
            }

            if (i.is_external_storage_) {
                *i.external_storage_ = value;
            }

            i.is_value_defined_ = true;
            ++i.args_counter_;
        }
    }
}

void ArgumentParser::ArgParser::AddFlagArgumentValue(char short_name, bool value) {
    for (auto& i : flag_arguments_) {
        if (i.short_name_ == short_name) {
            if (i.is_multi_value_) {
                i.multi_value_storage_.emplace_back(value);
                if (i.is_multi_value_external_storage_) {
                    i.multi_value_external_storage_->emplace_back(value);
                }
            } else {
                i.value_ = value;
            }

            if (i.is_external_storage_) {
                *i.external_storage_ = value;
            }

            i.is_value_defined_ = true;
            ++i.args_counter_;
        }
    }
}

void ArgumentParser::ArgParser::AddIntArgumentValue(const char* full_name, int64_t value) {
    for (auto& i : int_arguments_) {
        if (i.full_name_ == full_name) {
            if (i.is_multi_value_) {
                i.multi_value_storage_.emplace_back(value);
                if (i.is_multi_value_external_storage_) {
                    i.multi_value_external_storage_->emplace_back(value);
                }
            } else {
                i.value_ = value;
            }

            if (i.is_external_storage_) {
                *i.external_storage_ = value;
            }

            i.is_value_defined_ = true;
            ++i.args_counter_;
        }
    }
}

void ArgumentParser::ArgParser::AddStringArgumentValue(const char* full_name, const char* value) {
    for (auto& i : string_arguments_) {
        if (i.full_name_ == full_name) {
            if (i.is_multi_value_) {
                i.multi_value_storage_.emplace_back(value);
                if (i.is_multi_value_external_storage_) {
                    i.multi_value_external_storage_->emplace_back(value);
                }
            } else {
                i.value_ = value;
            }

            if (i.is_external_storage_) {
                *i.external_storage_ = value;
            }

            i.is_value_defined_ = true;
            ++i.args_counter_;
        }
    }
}

void ArgumentParser::ArgParser::AddFlagArgumentValue(const char* full_name, bool value) {
    for (auto& i : flag_arguments_) {
        if (i.full_name_ == full_name) {
            if (i.is_multi_value_) {
                i.multi_value_storage_.emplace_back(value);
                if (i.is_multi_value_external_storage_) {
                    i.multi_value_external_storage_->emplace_back(value);
                }
            } else {
                i.value_ = value;
            }

            if (i.is_external_storage_) {
                *i.external_storage_ = value;
            }

            i.is_value_defined_ = true;
            ++i.args_counter_;
        }
    }
}

const char *ArgumentParser::ArgParser::GetPositionalIntArgumentName() {
    for (auto& i : int_arguments_) {
        if (i.is_positional_) {
            return i.full_name_;
        }
    }

    return nullptr;
}

const char *ArgumentParser::ArgParser::GetPositionalStringArgumentName() {
    for (auto& i : string_arguments_) {
        if (i.is_positional_) {
            return i.full_name_;
        }
    }

    return nullptr;
}

bool ArgumentParser::ArgParser::IsIntPositionalArgument() {

    return std::ranges::any_of(
            int_arguments_.begin(),
            int_arguments_.end(),
            [](const IntArgument& i) {return i.is_positional_;});
}

bool ArgumentParser::ArgParser::CheckParserInput() {
    for (auto& i : int_arguments_) {
        if (!(i.is_value_defined_ || i.is_default_value_defined_)) {
            return false;
        }
        if (i.is_multi_value_ && i.min_args_count_) {
            if (i.args_counter_ < i.min_args_count_) {
                return false;
            }
        }
    }

    for (auto& i : string_arguments_) {
        if (!(i.is_value_defined_ || i.is_default_value_defined_)) {
            return false;
        }
        if (i.is_multi_value_ && i.min_args_count_) {
            if (i.args_counter_ < i.min_args_count_) {
                return false;
            }
        }
    }

    for (auto& i : flag_arguments_) {
        if (!(i.is_value_defined_ || i.is_default_value_defined_)) {
            return false;
        }
        if (i.is_multi_value_ && i.min_args_count_) {
            if (i.args_counter_ < i.min_args_count_) {
                return false;
            }
        }
    }

    return true;
}

int64_t ArgumentParser::ArgParser::GetIntValue(const char* full_name) const {
    for (auto& i : int_arguments_) {
        if (i.full_name_ == full_name) {
            return i.value_;
        }
    }

    return NULL;
}

const char* ArgumentParser::ArgParser::GetStringValue(const char* full_name) const {
    for (auto& i : string_arguments_) {
        if (i.full_name_ == full_name) {
            return i.value_;
        }
    }

    return nullptr;
}

bool ArgumentParser::ArgParser::GetFlagValue(const char* full_name) const {
    for (auto& i : flag_arguments_) {
        if (i.full_name_ == full_name) {
            return i.value_;
        }
    }

    return false;
}

const char *ArgumentParser::ArgParser::HelpDescription() {
    return nullptr;
}

ArgumentParser::Argument::Argument(char short_name, const char* full_name, const char* description)
    :
    short_name_(short_name),
    full_name_(full_name),
    description_(description)
{
}

ArgumentParser::Argument &ArgumentParser::Argument::MultiValue(std::size_t value) {
    min_args_count_ = value;
    is_multi_value_ = true;

    return *this;
}

ArgumentParser::Argument &ArgumentParser::Argument::Positional() {
    this->is_positional_ = true;

    return *this;
}

ArgumentParser::IntArgument::IntArgument(char short_name, const char* full_name, const char* description)
    : Argument(short_name, full_name, description)
{
}

ArgumentParser::IntArgument& ArgumentParser::IntArgument::Default(int64_t value) {
    default_value_ = value;
    is_default_value_defined_ = true;

    return *this;
}

ArgumentParser::IntArgument& ArgumentParser::IntArgument::StoreValue(int64_t& reference) {
    external_storage_ = &reference;
    is_external_storage_ = true;

    return *this;
}

ArgumentParser::IntArgument &ArgumentParser::IntArgument::StoreValues(std::vector<int64_t>& reference) {
    multi_value_external_storage_ = &reference;
    is_multi_value_external_storage_ = true;

    return *this;
}

ArgumentParser::StringArgument::StringArgument(char short_name, const char* full_name, const char* description)
        : Argument(short_name, full_name, description)
{
}

ArgumentParser::StringArgument& ArgumentParser::StringArgument::Default(const char* value) {
    default_value_ = value;
    is_default_value_defined_ = true;

    return *this;
}

ArgumentParser::StringArgument& ArgumentParser::StringArgument::StoreValue(const char*& reference) {
    external_storage_ = &reference;
    is_external_storage_ = true;

    return *this;
}

ArgumentParser::StringArgument& ArgumentParser::StringArgument::StoreValues(std::vector<const char*>& reference) {
    multi_value_external_storage_ = &reference;
    is_multi_value_external_storage_ = true;

    return *this;
}

ArgumentParser::FlagArgument::FlagArgument(char short_name, const char* full_name, const char* description)
        : Argument(short_name, full_name, description)
{
}

ArgumentParser::FlagArgument& ArgumentParser::FlagArgument::Default(bool value) {
    default_value_ = value;
    is_default_value_defined_ = true;

    return *this;
}

ArgumentParser::FlagArgument& ArgumentParser::FlagArgument::StoreValue(bool& reference) {
    external_storage_ = &reference;
    is_external_storage_ = true;

    return *this;
}

ArgumentParser::FlagArgument& ArgumentParser::FlagArgument::StoreValues(std::vector<bool>& reference) {
    multi_value_external_storage_ = &reference;
    is_multi_value_external_storage_ = true;

    return *this;
}
