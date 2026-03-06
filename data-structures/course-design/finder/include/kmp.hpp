#pragma once

#ifndef FINDER_INCLUDE_KMP_HPP_
#define FINDER_INCLUDE_KMP_HPP_

#include "common_types.hpp"

#include <expected>
#include <string>
#include <vector>

namespace kmp {

std::expected<common::MatchRanges, std::string> find_all_kmp_ranges(const std::string &text, const std::string &pattern,
                                                                    bool ignore_case);

std::expected<std::string, std::string> replace_all_kmp(const std::string &text, const std::string &pattern,
                                                        const std::string &replacement, bool ignore_case);

} // namespace kmp

#endif // FINDER_INCLUDE_KMP_HPP_
