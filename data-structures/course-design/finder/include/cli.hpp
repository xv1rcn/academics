#pragma once

#ifndef FINDER_INCLUDE_CLI_HPP_
#define FINDER_INCLUDE_CLI_HPP_

#include <string>

namespace app {

struct CliOptions {
    std::string regexp;
    std::string plain_pattern;
    std::string replacement;
    std::string color_mode = "auto";
    bool ignore_case = false;
    bool extended = false;
    bool literal_mode = false;
    bool line_number = false;
    bool count = false;
    bool invert_match = false;
    bool only_matching = false;
    bool quiet = false;
    bool in_place = false;
    bool diff = false;
    std::string input_file;
};

int parse_cli(int argc, char *argv[], CliOptions &options);

} // namespace app

#endif // FINDER_INCLUDE_CLI_HPP_
