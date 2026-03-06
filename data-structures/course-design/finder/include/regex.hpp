#pragma once

#ifndef FINDER_INCLUDE_REGEX_HPP_
#define FINDER_INCLUDE_REGEX_HPP_

#include "common_types.hpp"

#include <expected>
#include <regex>
#include <string>
#include <vector>

namespace regex {

struct RegexOptions {
    bool ignore_case = false;
    bool extended = false;
};

std::expected<std::regex, std::string> compile_regex(const std::string &pattern, const RegexOptions &options = {});

std::expected<common::MatchRanges, std::string> find_all_regex_ranges(const std::string &text,
                                                                      const std::regex &compiled_regex);

std::expected<std::string, std::string> replace_all_regex(const std::string &text, const std::regex &compiled_regex,
                                                          const std::string &replacement);

std::expected<common::MatchRanges, std::string>
find_all_regex_ranges(const std::string &text, const std::string &pattern, const RegexOptions &options = {});

std::expected<std::string, std::string> replace_all_regex(const std::string &text, const std::string &pattern,
                                                          const std::string &replacement,
                                                          const RegexOptions &options = {});

} // namespace regex

#endif // FINDER_INCLUDE_REGEX_HPP_
