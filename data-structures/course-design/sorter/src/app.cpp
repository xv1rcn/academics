#include "app.hpp"

#include "cli.hpp"
#include "common.hpp"
#include "io.hpp"
#include "sort.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

void printBenchmarkToConsole(const std::vector<SortExecutionResult> &results, bool hasTime, bool hasMemory) {
    std::cout << "Benchmark summary:" << std::endl;
    for (const auto &result : results) {
        std::cout << "  - " << result.algorithmName;
        if (hasTime && result.durationMicroseconds.has_value()) {
            std::cout << " | time(us): " << result.durationMicroseconds.value();
        }
        if (hasMemory && result.memoryDeltaKb.has_value()) {
            std::cout << " | memory_delta_kb: " << result.memoryDeltaKb.value();
        }
        std::cout << std::endl;
    }
}

} // namespace

int App::run(int argc, char *argv[]) {
    const AppConfig config = Cli::parse(argc, argv);

    RunArtifacts artifacts;
    artifacts.runDirectory = RunDir::createRunDirectory("runs");
    artifacts.defaultInputFile = (artifacts.runDirectory / "data_input.txt").string();
    artifacts.defaultBenchmarkFile = (artifacts.runDirectory / "data_benchmark.csv").string();

    const std::string inputFilePath = Paths::resolveInputFilePath(config, artifacts);

    if (config.useRandomInput) {
        DataIo::generateRandomDataFile(inputFilePath, config.randomSize);
        std::cout << "Generated random input file: " << inputFilePath << std::endl;
        std::cout << "Random data size: " << config.randomSize << std::endl;
    }

    const std::vector<long long> inputData = DataIo::readDataFile(inputFilePath);
    if (inputData.empty()) {
        throw std::runtime_error("Input dataset is empty.");
    }

    const bool doSort = config.mode == RunMode::Sort || config.mode == RunMode::Both;
    const bool doVerify = config.mode == RunMode::Verify || config.mode == RunMode::Both;

    std::cout << "Run directory: " << artifacts.runDirectory.string() << std::endl;
    std::cout << "Input file: " << inputFilePath << std::endl;
    std::cout << "Input count: " << inputData.size() << std::endl;

    std::vector<SortExecutionResult> sortResults;
    if (doSort) {
        const AlgoRegistry registry;
        const Sorter engine(registry);
        const auto selectedAlgorithms = engine.selectAlgorithms(config.selectedAlgorithms);

        std::cout << "Sorting with algorithm count: " << selectedAlgorithms.size() << std::endl;
        sortResults = engine.sortData(inputData, selectedAlgorithms, config, artifacts);

        for (const auto &result : sortResults) {
            std::cout << "Sorted by " << result.algorithmName << " -> output: " << result.outputFile << std::endl;
        }
    }

    if (doVerify) {
        if (doSort) {
            for (const auto &result : sortResults) {
                std::cout << "Verify output [" << result.algorithmId << "]: " << (result.sorted ? "PASS" : "FAIL")
                          << std::endl;
            }
        } else {
            const bool sorted = Check::isNonDecreasing(inputData);
            std::cout << "Verify input order: " << (sorted ? "PASS" : "FAIL") << std::endl;
        }
    }

    if (config.benchmark.enableTime || config.benchmark.enableMemory) {
        if (!doSort) {
            std::cout << "Benchmark options are ignored because sorting is not enabled." << std::endl;
        } else {
            printBenchmarkToConsole(sortResults, config.benchmark.enableTime, config.benchmark.enableMemory);

            if (config.benchmark.save) {
                const std::string benchmarkPath = Paths::resolveBenchmarkFilePath(config, artifacts);
                DataIo::writeBenchmarkReport(benchmarkPath, sortResults, config.benchmark.enableTime,
                                             config.benchmark.enableMemory);
                std::cout << "Benchmark report saved: " << benchmarkPath << std::endl;
            }
        }
    } else if (config.benchmark.save) {
        std::cout << "Benchmark save is ignored because no benchmark is enabled." << std::endl;
    }

    return 0;
}
