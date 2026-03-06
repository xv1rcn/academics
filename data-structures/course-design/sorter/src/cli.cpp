#include "cli.hpp"

#include "algo.hpp"
#include "common.hpp"

#include <CLI/CLI.hpp>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {

std::string joinAlgorithmIds(const std::vector<std::string> &algorithmIds) {
    std::string joined;
    for (std::size_t i = 0; i < algorithmIds.size(); ++i) {
        if (i > 0) {
            joined += ",";
        }
        joined += algorithmIds[i];
    }
    return joined;
}

std::vector<std::string> orderedAlgorithmIds(const AlgoRegistry &registry) {
    const std::vector<std::string> preferredOrder = {"quick",    "merge", "heap",      "introsort", "stdsort",
                                                     "stable",   "shell", "insertion", "selection", "bubble",
                                                     "cocktail", "comb",  "memhog"};

    const auto available = registry.availableAlgorithms();
    std::unordered_set<std::string> availableSet(available.begin(), available.end());

    std::vector<std::string> ordered;
    ordered.reserve(available.size());

    for (const auto &id : preferredOrder) {
        if (availableSet.find(id) != availableSet.end()) {
            ordered.push_back(id);
        }
    }

    for (const auto &id : available) {
        if (std::find(ordered.begin(), ordered.end(), id) == ordered.end()) {
            ordered.push_back(id);
        }
    }

    return ordered;
}

void printAvailableAlgorithms(const AlgoRegistry &registry) {
    const auto available = orderedAlgorithmIds(registry);
    std::cout << "Available algorithms:" << std::endl;
    for (const auto &id : available) {
        std::cout << "  - " << id << std::endl;
    }
    std::cout << "  - all" << std::endl;
}

void applyOutputMappings(const std::vector<std::string> &rawMappings, const AlgoRegistry &registry,
                         std::unordered_map<std::string, std::string> &output) {
    for (const std::string &mapping : rawMappings) {
        const std::size_t delimiter = mapping.find('=');
        if (delimiter == std::string::npos || delimiter == 0 || delimiter == mapping.size() - 1) {
            throw CLI::ValidationError("--output-map", "Expected <algorithm>=<path>.");
        }
        const std::string id = utils::toLower(mapping.substr(0, delimiter));
        const std::string path = mapping.substr(delimiter + 1);
        if (!registry.has(id)) {
            throw CLI::ValidationError("--output-map", "Unknown algorithm id in mapping: " + id);
        }
        output[id] = path;
    }
}

void normalizeAlgorithms(std::vector<std::string> &algorithmIds) {
    for (std::string &id : algorithmIds) {
        id = utils::toLower(id);
    }
}

void normalizeAndValidateAlgorithms(std::vector<std::string> &algorithmIds, const AlgoRegistry &registry) {
    normalizeAlgorithms(algorithmIds);

    if (algorithmIds.empty()) {
        return;
    }

    bool hasAll = false;
    for (const auto &id : algorithmIds) {
        if (id == "all") {
            hasAll = true;
            break;
        }
    }

    if (hasAll) {
        if (algorithmIds.size() > 1) {
            std::cerr << "[warn] --algo includes 'all'; other algorithm ids are ignored." << std::endl;
        }
        algorithmIds = {"all"};
        return;
    }

    std::unordered_set<std::string> seen;
    std::vector<std::string> deduplicated;
    deduplicated.reserve(algorithmIds.size());

    for (const auto &id : algorithmIds) {
        if (!registry.has(id)) {
            throw CLI::ValidationError("--algo", "Unknown algorithm id: " + id);
        }
        if (seen.insert(id).second) {
            deduplicated.push_back(id);
        }
    }

    algorithmIds.swap(deduplicated);
}

} // namespace

AppConfig Cli::parse(int argc, char *argv[]) {
    AppConfig config;
    const AlgoRegistry registry;
    const auto availableAlgorithmIds = orderedAlgorithmIds(registry);
    auto validAlgorithmIds = availableAlgorithmIds;
    validAlgorithmIds.push_back("all");

    CLI::App app{"sorter"};
    app.description("Sort and benchmark long long integer datasets.");
    app.option_defaults()->always_capture_default();

    auto *runGroup = app.add_option_group("Run control");
    auto *dataGroup = app.add_option_group("Data source");
    auto *outputGroup = app.add_option_group("Output");
    auto *benchGroup = app.add_option_group("Benchmark");

    bool useRandomFlag = false;
    bool noRandomFlag = false;
    bool listAlgorithmsFlag = false;

    std::string modeString = "sort";
    runGroup->add_option("-m,--mode", modeString, "Run mode: sort, verify, both.")
        ->check(CLI::IsMember({"sort", "verify", "both"}, CLI::ignore_case));

    auto *algorithmsOption =
        runGroup
            ->add_option("-a,--algo,--algorithms,--algos", config.selectedAlgorithms,
                         ("Comma-separated algorithms: " + joinAlgorithmIds(availableAlgorithmIds) + ",all").c_str())
            ->delimiter(',')
            ->check(CLI::IsMember(validAlgorithmIds, CLI::ignore_case));

    runGroup->add_flag("-l,--list-algorithms,--list-algos", listAlgorithmsFlag, "List available algorithms and exit.");

    auto *randomSizeOption = dataGroup
                                 ->add_option("-n,--size,--random-size,--rand-size", config.randomSize,
                                              "Number of random values to generate.")
                                 ->check(CLI::PositiveNumber);

    auto *inputFileOption = dataGroup->add_option("-i,--input,--input-file,--in", config.inputFile,
                                                  "Input file path (required with --no-random).");

    auto *randomFileOption =
        dataGroup->add_option("-r,--random-file,--rand-file", config.randomFile,
                              "Path to write generated random input. Default: <runDir>/data_input.txt");

    auto *useRandomOption =
        dataGroup->add_flag("-R,--random,--use-random,--generate", useRandomFlag, "Force random input generation.");
    auto *noRandomOption = dataGroup->add_flag("-N,--no-random,--input-only", noRandomFlag,
                                               "Disable random generation and require --input.");
    useRandomOption->excludes(noRandomOption);
    noRandomOption->excludes(useRandomOption);
    noRandomOption->needs(inputFileOption);
    randomFileOption->excludes(noRandomOption);
    randomSizeOption->excludes(noRandomOption);

    std::string outputFile;
    auto *outputFileOption = outputGroup->add_option("-o,--output,--output-file,--out", outputFile,
                                                     "Output file path; for multi-algo supports {algo} placeholder.");

    std::vector<std::string> outputMappings;
    auto *outputMapOption = outputGroup->add_option("-O,--output-map,--out-map", outputMappings,
                                                    "Per-algorithm output override, repeatable: <algorithm>=<path>");

    auto *benchmarkTimeOption = benchGroup->add_flag("-t,--time,--benchmark-time,--bench-time",
                                                     config.benchmark.enableTime, "Enable timing benchmark.");
    auto *benchmarkMemoryOption = benchGroup->add_flag("-M,--memory,--benchmark-memory,--bench-memory,--bench-mem",
                                                       config.benchmark.enableMemory, "Enable RSS memory benchmark.");
    auto *saveBenchOption = benchGroup->add_flag("-s,--save-benchmark,--save-bench", config.benchmark.save,
                                                 "Save benchmark report to CSV.");
    auto *benchFileOption = benchGroup->add_option("-b,--benchmark-file,--bench-file", config.benchmark.outputFile,
                                                   "Benchmark output CSV path.");
    benchFileOption->needs(saveBenchOption);

    try {
        app.parse(argc, argv);

        if (listAlgorithmsFlag) {
            printAvailableAlgorithms(registry);
            std::exit(0);
        }

        normalizeAndValidateAlgorithms(config.selectedAlgorithms, registry);
        applyOutputMappings(outputMappings, registry, config.perAlgorithmOutput);
    } catch (const CLI::ParseError &error) {
        std::exit(app.exit(error));
    }

    if (useRandomFlag || noRandomFlag) {
        config.useRandomInput = useRandomFlag && !noRandomFlag;
    }

    modeString = utils::toLower(modeString);
    config.mode = modeString == "verify" ? RunMode::Verify : (modeString == "both" ? RunMode::Both : RunMode::Sort);

    if (!outputFile.empty()) {
        config.outputFile = outputFile;
    }

    if (outputFileOption->count() > 0 && outputMapOption->count() > 0) {
        std::cerr << "[warn] --output-map overrides --output for mapped algorithms." << std::endl;
    }

    if (config.useRandomInput && inputFileOption->count() > 0) {
        std::cerr << "[warn] --input is ignored when random input is enabled." << std::endl;
    }

    if (config.mode == RunMode::Verify) {
        if (algorithmsOption->count() > 0) {
            std::cerr << "[warn] --algorithms is ignored in verify mode." << std::endl;
        }
        if (outputFileOption->count() > 0 || outputMapOption->count() > 0) {
            std::cerr << "[warn] output options are ignored in verify mode." << std::endl;
        }
        if (benchmarkTimeOption->count() > 0 || benchmarkMemoryOption->count() > 0 || saveBenchOption->count() > 0 ||
            benchFileOption->count() > 0) {
            std::cerr << "[warn] benchmark options are ignored in verify mode." << std::endl;
        }
    }

    return config;
}
