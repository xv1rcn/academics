#include "regex.hpp"

#include <expected>
#include <regex>
#include <string>
#include <vector>

namespace regex {

namespace {

std::regex_constants::syntax_option_type build_regex_flags(const RegexOptions &options) {
    auto flags = options.extended ? std::regex_constants::extended : std::regex_constants::ECMAScript;
    if (options.ignore_case) {
        flags |= std::regex_constants::icase;
    }
    return flags;
}

} // namespace

std::expected<std::regex, std::string> compile_regex(const std::string &pattern, const RegexOptions &options) {
    try {
        return std::regex(pattern, build_regex_flags(options));
    } catch (const std::regex_error &e) {
        return std::unexpected(std::string("finder: error: regex engine reported: ") + e.what());
    }
}

std::expected<common::MatchRanges, std::string> find_all_regex_ranges(const std::string &text,
                                                                      const std::regex &compiled_regex) {
    try {
        common::MatchRanges ranges;
        for (std::sregex_iterator it(text.begin(), text.end(), compiled_regex), end; it != end; ++it) {
            const auto &m = *it;
            ranges.push_back(common::MatchRange{.begin = static_cast<std::size_t>(m.position()),
                                                .end = static_cast<std::size_t>(m.position() + m.length())});
        }
        return ranges;
    } catch (const std::regex_error &e) {
        return std::unexpected(std::string("finder: error: regex engine reported: ") + e.what());
    }
}

std::expected<std::string, std::string> replace_all_regex(const std::string &text, const std::regex &compiled_regex,
                                                          const std::string &replacement) {
    try {
        return std::regex_replace(text, compiled_regex, replacement);
    } catch (const std::regex_error &e) {
        return std::unexpected(std::string("finder: error: regex engine reported: ") + e.what());
    }
}

std::expected<common::MatchRanges, std::string>
find_all_regex_ranges(const std::string &text, const std::string &pattern, const RegexOptions &options) {
    return compile_regex(pattern, options).and_then([&text](const std::regex &re) {
        return find_all_regex_ranges(text, re);
    });
}

std::expected<std::string, std::string> replace_all_regex(const std::string &text, const std::string &pattern,
                                                          const std::string &replacement, const RegexOptions &options) {
    return compile_regex(pattern, options).and_then([&text, &replacement](const std::regex &re) {
        return replace_all_regex(text, re, replacement);
    });
}

} // namespace regex
