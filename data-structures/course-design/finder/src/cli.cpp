#include "cli.hpp"

#include <CLI/CLI.hpp>

#include <iostream>
#include <vector>

namespace app {

int parse_cli(int argc, char *argv[], CliOptions &options) {
    CLI::App cli{"finder - stream-oriented pattern search and replacement utility"};
    std::vector<std::string> positional_args;

    auto *opt_regexp = cli.add_option(
        "-e,--regexp", options.regexp,
        "Use PATTERN as a regular expression (regex mode). Mutually exclusive with --literal and positional PATTERN.");
    auto *opt_literal =
        cli.add_flag("-l,--literal", options.literal_mode,
                     "Treat PATTERN as a fixed literal string (KMP mode), similar to grep --fixed-strings.");
    auto *opt_ignore_case =
        cli.add_flag("-i,--ignore-case", options.ignore_case,
                     "Ignore ASCII case distinctions while matching (equivalent to case-insensitive regex matching).");
    auto *opt_line_number = cli.add_flag("-n,--line-number", options.line_number,
                                         "Prefix each output line with its 1-based line number in the input stream.");
    auto *opt_count = cli.add_flag("-c,--count", options.count,
                                   "Print only the number of selected input lines; suppress normal line output.");
    auto *opt_invert_match =
        cli.add_flag("-v,--invert-match", options.invert_match, "Select non-matching lines instead of matching lines.");
    auto *opt_only_matching =
        cli.add_flag("-o,--only-matching", options.only_matching,
                     "Print only the matching part of selected lines, one match per output line.");
    auto *opt_quiet = cli.add_flag(
        "-q,--quiet", options.quiet,
        "Suppress normal output; exit immediately with status 0 on first selected line, 1 if none are selected.");
    auto *opt_replace = cli.add_option("-r,--replace", options.replacement,
                                       "Enable replacement mode and substitute each match with REPLACEMENT (supports "
                                       "regex replacement syntax such as $1).");
    auto *opt_in_place = cli.add_flag(
        "-p,--in-place", options.in_place,
        "Write replacement results back to the input file in place; requires --replace and a FILE argument.");
    auto *opt_diff = cli.add_flag(
        "-d,--diff", options.diff,
        "In replacement mode, print both original and replaced forms for each processed line in a diff-like format.");
    auto *opt_extended = cli.add_flag("-x,--extended", options.extended,
                                      "Use POSIX Extended Regular Expression (ERE) syntax for --regexp patterns.");
    auto *opt_color =
        cli.add_option("--color", options.color_mode, "Control colorized output: auto (default), always, or never.");
    opt_color->check(CLI::IsMember({"auto", "always", "never"}));
    auto *opt_positional = cli.add_option(
        "args", positional_args, "Positional arguments: [PATTERN [FILE]] in literal mode, or [FILE] in --regexp mode.");
    opt_positional->expected(0, 2);

    opt_literal->excludes(opt_regexp);
    opt_regexp->excludes(opt_literal);
    opt_extended->needs(opt_regexp);
    opt_only_matching->excludes(opt_invert_match);
    opt_replace->excludes(opt_count);
    opt_replace->excludes(opt_only_matching);
    opt_replace->excludes(opt_quiet);
    opt_replace->excludes(opt_invert_match);
    opt_in_place->needs(opt_replace);
    opt_diff->needs(opt_replace);

    (void)opt_ignore_case;
    (void)opt_line_number;

    try {
        cli.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        (void)cli.exit(e);
        return 2;
    }

    if (!options.regexp.empty()) {
        if (positional_args.size() > 1) {
            std::cerr << "finder: error: in --regexp mode, at most one positional FILE is allowed.\n";
            return 2;
        }
        if (!positional_args.empty()) {
            options.input_file = positional_args[0];
        }
    } else {
        if (positional_args.empty()) {
            std::cerr << "finder: error: literal mode requires a positional PATTERN when --regexp is not provided.\n";
            return 2;
        }
        options.plain_pattern = positional_args[0];
        if (positional_args.size() > 1) {
            options.input_file = positional_args[1];
        }
    }

    if (options.in_place && options.input_file.empty()) {
        std::cerr << "finder: error: --in-place requires an input FILE and cannot operate on standard input.\n";
        return 2;
    }

    return 0;
}

} // namespace app
