#include "app.hpp"

#include "cli.hpp"
#include "common_types.hpp"
#include "kmp.hpp"
#include "regex.hpp"

#if FINDER_HAVE_RANG
#include <rang.hpp>
#endif

#include <cstdio>
#include <cstdlib>
#include <expected>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

namespace {

constexpr int kExitSuccess = 0;
constexpr int kExitNoMatch = 1;
constexpr int kExitError = 2;

enum class ColorTag { red, green, blue, magenta, white };

#if FINDER_HAVE_RANG

rang::fgB to_rang_color(ColorTag color) {
    switch (color) {
    case ColorTag::red:
        return rang::fgB::red;
    case ColorTag::green:
        return rang::fgB::green;
    case ColorTag::blue:
        return rang::fgB::blue;
    case ColorTag::magenta:
        return rang::fgB::magenta;
    case ColorTag::white:
        return rang::fgB::gray;
    }
    return rang::fgB::magenta;
}

#endif

std::string colorize_segment(std::string_view text, ColorTag color) {
#if FINDER_HAVE_RANG
    std::ostringstream out;
    out << to_rang_color(color) << text << rang::style::reset;
    return out.str();
#else
    (void)color;
    return std::string(text);
#endif
}

std::string format_line_number(std::size_t line_no) {
    const std::string digits = std::to_string(line_no);
    std::string grouped;
    grouped.reserve(digits.size() + digits.size() / 3);

    int count = 0;
    for (std::size_t i = digits.size(); i > 0; --i) {
        if (count == 3) {
            grouped.insert(grouped.begin(), ' ');
            count = 0;
        }
        grouped.insert(grouped.begin(), digits[i - 1]);
        ++count;
    }

    constexpr std::size_t kMinWidth = 3;
    if (grouped.size() < kMinWidth) {
        grouped.insert(grouped.begin(), kMinWidth - grouped.size(), ' ');
    }

    return grouped;
}

bool is_stdout_tty() {
#if defined(_WIN32)
    return ::_isatty(::_fileno(stdout)) == 1;
#else
    return ::isatty(::fileno(stdout)) == 1;
#endif
}

bool env_var_is_nonempty(const char *name) {
    const char *value = std::getenv(name);
    return value != nullptr && *value != '\0';
}

bool is_dumb_terminal() {
    const char *term = std::getenv("TERM");
    return term != nullptr && std::string_view(term) == "dumb";
}

void apply_color_mode(const std::string &mode) {
#if FINDER_HAVE_RANG
    if (mode == "always") {
        rang::setControlMode(rang::control::Force);
        return;
    }
    if (mode == "never") {
        rang::setControlMode(rang::control::Off);
        return;
    }
    if (env_var_is_nonempty("NO_COLOR") || is_dumb_terminal()) {
        rang::setControlMode(rang::control::Off);
        return;
    }
    const bool stdout_is_tty = is_stdout_tty();
    rang::setControlMode(stdout_is_tty ? rang::control::Force : rang::control::Off);
#else
    (void)mode;
#endif
}

int fail(std::string_view message) {
    std::cerr << message << '\n';
    return kExitError;
}

common::MatchRanges full_line_range_if_changed(bool matched, const std::string &before, const std::string &after) {
    if (matched && after != before) {
        return {common::MatchRange{.begin = 0, .end = after.size()}};
    }
    return {};
}

bool file_has_trailing_newline(const std::string &path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        return false;
    }

    input.seekg(0, std::ios::end);
    const auto size = input.tellg();
    if (size <= 0) {
        return false;
    }

    input.seekg(-1, std::ios::end);
    char ch = '\0';
    input.get(ch);
    return ch == '\n';
}

std::expected<void, std::string> write_lines_atomically(const std::string &path, const std::vector<std::string> &lines,
                                                        bool keep_trailing_newline) {
    namespace fs = std::filesystem;
    std::error_code ec;

    const fs::path target(path);
    fs::path tmp = target;
    tmp += ".tmp";

    {
        std::ofstream out(tmp, std::ios::trunc);
        if (!out) {
            return std::unexpected("unable to open temporary output file '" + tmp.string() + "'");
        }

        for (std::size_t i = 0; i < lines.size(); ++i) {
            out << lines[i];
            if (i + 1 < lines.size() || keep_trailing_newline) {
                out << '\n';
            }
        }

        out.flush();
        if (!out) {
            out.close();
            fs::remove(tmp, ec);
            return std::unexpected("failed while writing temporary output file '" + tmp.string() + "'");
        }
    }

#if defined(_WIN32)
    if (!::MoveFileExW(tmp.wstring().c_str(), target.wstring().c_str(),
                       MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        fs::remove(tmp, ec);
        return std::unexpected("failed to atomically replace target file '" + target.string() + "'");
    }
#else
    fs::rename(tmp, target, ec);
    if (ec) {
        fs::remove(tmp, ec);
        return std::unexpected("failed to atomically replace target file '" + target.string() + "'");
    }
#endif

    return {};
}

std::string colorize_ranges(std::string_view text, const common::MatchRanges &ranges, ColorTag color) {
    std::ostringstream out;

    std::size_t cursor = 0;
    for (const auto &range : ranges) {
        if (range.begin > text.size() || range.end > text.size() || range.begin >= range.end) {
            continue;
        }
        if (range.begin < cursor) {
            continue;
        }

        out << text.substr(cursor, range.begin - cursor);
        out << colorize_segment(text.substr(range.begin, range.end - range.begin), color);
        cursor = range.end;
    }
    out << text.substr(cursor);
    return out.str();
}

std::string build_kmp_inline_diff(std::string_view text, const common::MatchRanges &ranges,
                                  const std::string &replacement) {
    std::ostringstream out;
    std::size_t cursor = 0;

    for (const auto &range : ranges) {
        if (range.begin > text.size() || range.end > text.size() || range.begin >= range.end) {
            continue;
        }
        if (range.begin < cursor) {
            continue;
        }

        out << text.substr(cursor, range.begin - cursor);
        out << colorize_segment(text.substr(range.begin, range.end - range.begin), ColorTag::red);
        out << colorize_segment(replacement, ColorTag::green);
        cursor = range.end;
    }

    out << text.substr(cursor);
    return out.str();
}

std::string build_regex_inline_diff(const std::string &text, const std::regex &compiled_regex,
                                    const std::string &replacement) {
    std::ostringstream out;
    std::size_t cursor = 0;

    for (std::sregex_iterator it(text.begin(), text.end(), compiled_regex), end; it != end; ++it) {
        const auto &match = *it;
        const std::size_t begin = static_cast<std::size_t>(match.position());
        const std::size_t finish = begin + static_cast<std::size_t>(match.length());
        if (begin < cursor) {
            continue;
        }

        out << std::string_view(text).substr(cursor, begin - cursor);
        out << colorize_segment(std::string_view(text).substr(begin, finish - begin), ColorTag::red);
        out << colorize_segment(match.format(replacement), ColorTag::green);
        cursor = finish;
    }

    out << std::string_view(text).substr(cursor);
    return out.str();
}

void print_line(std::size_t line_no, bool with_line_number, const std::string &text, std::string_view tag = "") {
    if (!tag.empty()) {
        std::cout << colorize_segment(tag, ColorTag::magenta);
    }
    if (with_line_number) {
        std::cout << colorize_segment(format_line_number(line_no), ColorTag::white) << ' '
                  << colorize_segment("|", ColorTag::blue) << ' ';
    }
    std::cout << text << '\n';
}

} // namespace

namespace app {

int run(int argc, char *argv[]) {
    CliOptions options;
    const int parse_exit_code = parse_cli(argc, argv, options);
    if (parse_exit_code != 0) {
        return parse_exit_code;
    }

    apply_color_mode(options.color_mode);

    const bool regex_mode = !options.regexp.empty();
    const bool replace_mode = !options.replacement.empty();
    const std::string &plain_pattern = options.plain_pattern;

    std::optional<std::regex> compiled;
    if (regex_mode) {
        regex::RegexOptions regex_options{.ignore_case = options.ignore_case, .extended = options.extended};
        const auto compiled_regex = regex::compile_regex(options.regexp, regex_options);
        if (!compiled_regex.has_value()) {
            return fail("finder: error: failed to compile regular expression pattern '" + options.regexp +
                        "' with the selected dialect/options: " + compiled_regex.error());
        }
        compiled = *compiled_regex;
    } else if (plain_pattern.empty()) {
        return fail("finder: error: literal mode requires a positional PATTERN when --regexp is not provided.");
    }

    std::ifstream input_file_stream;
    std::istream *input = &std::cin;
    bool keep_trailing_newline = false;
    if (!options.input_file.empty()) {
        input_file_stream.open(options.input_file);
        if (!input_file_stream) {
            return fail("finder: error: cannot open input file '" + options.input_file + "' for reading.");
        }
        keep_trailing_newline = file_has_trailing_newline(options.input_file);
        input = &input_file_stream;
    }

    std::size_t line_no = 0;
    std::size_t selected_count = 0;
    bool any_selected = false;
    std::vector<std::string> in_place_lines;

    std::string line;
    while (std::getline(*input, line)) {
        ++line_no;

        const auto ranges_result = regex_mode ? regex::find_all_regex_ranges(line, *compiled)
                                              : kmp::find_all_kmp_ranges(line, plain_pattern, options.ignore_case);
        if (!ranges_result.has_value()) {
            return fail("finder: error: match evaluation failed while processing the input stream: " +
                        ranges_result.error());
        }
        const auto &ranges = *ranges_result;

        const bool matched = !ranges.empty();

        if (replace_mode) {
            const auto replaced_result =
                regex_mode ? regex::replace_all_regex(line, *compiled, options.replacement)
                           : kmp::replace_all_kmp(line, plain_pattern, options.replacement, options.ignore_case);
            if (!replaced_result.has_value()) {
                return fail("finder: error: replacement operation failed while processing the input stream: " +
                            replaced_result.error());
            }
            const std::string &replaced = *replaced_result;

            if (options.in_place) {
                in_place_lines.push_back(replaced);
            }

            if (options.diff) {
                const std::string rendered = regex_mode ? build_regex_inline_diff(line, *compiled, options.replacement)
                                                        : build_kmp_inline_diff(line, ranges, options.replacement);
                print_line(line_no, options.line_number, rendered);
            } else if (!options.quiet) {
                const auto replaced_ranges = full_line_range_if_changed(matched, line, replaced);
                const std::string rendered = colorize_ranges(replaced, replaced_ranges, ColorTag::green);
                print_line(line_no, options.line_number, rendered);
            }

            if (matched) {
                any_selected = true;
            }
            continue;
        }

        const bool selected = options.invert_match ? !matched : matched;
        if (selected) {
            any_selected = true;
            ++selected_count;
        }

        if (options.quiet && selected) {
            return kExitSuccess;
        }

        if (options.count || !selected) {
            continue;
        }

        if (options.only_matching) {
            for (const auto &range : ranges) {
                const std::string piece = line.substr(range.begin, range.end - range.begin);
                const common::MatchRanges piece_ranges{common::MatchRange{.begin = 0, .end = piece.size()}};
                print_line(line_no, options.line_number, colorize_ranges(piece, piece_ranges, ColorTag::red));
            }
        } else {
            print_line(line_no, options.line_number, colorize_ranges(line, ranges, ColorTag::red));
        }
    }

    if (!input->eof() && input->fail()) {
        return fail("finder: error: failed while reading from the input stream.");
    }

    if (options.count) {
        std::cout << selected_count << '\n';
    }

    if (options.in_place) {
        if (input_file_stream.is_open()) {
            input_file_stream.close();
        }
        const auto write_result = write_lines_atomically(options.input_file, in_place_lines, keep_trailing_newline);
        if (!write_result.has_value()) {
            return fail("finder: error: cannot safely write replacement output back to file '" + options.input_file +
                        "': " + write_result.error());
        }
    }

    return any_selected ? kExitSuccess : kExitNoMatch;
}

} // namespace app
