#include "kmp.hpp"

#include <cctype>
#include <string>
#include <vector>

namespace kmp {
namespace {

char fold_ascii(char c) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

bool equal_char(char lhs, char rhs, bool ignore_case) {
    if (!ignore_case) {
        return lhs == rhs;
    }
    return fold_ascii(lhs) == fold_ascii(rhs);
}

std::vector<std::size_t> build_lps(const std::string &pattern, bool ignore_case) {
    std::vector<std::size_t> lps(pattern.size(), 0);
    std::size_t len = 0;

    for (std::size_t i = 1; i < pattern.size();) {
        if (equal_char(pattern[i], pattern[len], ignore_case)) {
            lps[i++] = ++len;
        } else if (len != 0) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }

    return lps;
}

} // namespace

std::expected<common::MatchRanges, std::string> find_all_kmp_ranges(const std::string &text, const std::string &pattern,
                                                                    bool ignore_case) {
    if (pattern.empty()) {
        return std::unexpected("finder: error: literal PATTERN must not be empty in fixed-string mode");
    }

    common::MatchRanges ranges;
    const auto lps = build_lps(pattern, ignore_case);

    std::size_t i = 0;
    std::size_t j = 0;
    while (i < text.size()) {
        if (equal_char(text[i], pattern[j], ignore_case)) {
            ++i;
            ++j;
            if (j == pattern.size()) {
                const std::size_t start = i - j;
                ranges.push_back(common::MatchRange{.begin = start, .end = start + pattern.size()});
                j = lps[j - 1];
            }
        } else if (j != 0) {
            j = lps[j - 1];
        } else {
            ++i;
        }
    }

    return ranges;
}

std::expected<std::string, std::string> replace_all_kmp(const std::string &text, const std::string &pattern,
                                                        const std::string &replacement, bool ignore_case) {
    const auto ranges_result = find_all_kmp_ranges(text, pattern, ignore_case);
    if (!ranges_result.has_value()) {
        return std::unexpected(ranges_result.error());
    }

    const auto &ranges = *ranges_result;
    if (ranges.empty()) {
        return text;
    }

    std::string result;
    result.reserve(text.size());

    std::size_t cursor = 0;
    for (const auto &range : ranges) {
        result.append(text.substr(cursor, range.begin - cursor));
        result.append(replacement);
        cursor = range.end;
    }
    result.append(text.substr(cursor));

    return result;
}

} // namespace kmp
