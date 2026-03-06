#include "sort.hpp"

#include "common.hpp"
#include "io.hpp"

#include <chrono>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#endif

namespace {

#if defined(__linux__)
std::uint64_t readStatusValueKb(const char *keyPrefix) {
    std::ifstream in("/proc/self/status");
    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind(keyPrefix, 0) == 0) {
            std::istringstream parser(line.substr(std::char_traits<char>::length(keyPrefix)));
            std::uint64_t kb = 0;
            parser >> kb;
            return kb;
        }
    }
    return 0;
}
#endif

long long elapsedMicrosecondsSince(const std::chrono::high_resolution_clock::time_point &start) {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start)
        .count();
}

std::uint64_t currentRssKb() {
#if defined(__linux__)
    return readStatusValueKb("VmRSS:");
#elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS_EX counters{};
    if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS *>(&counters),
                             sizeof(counters)) != 0) {
        return static_cast<std::uint64_t>(counters.WorkingSetSize / 1024);
    }
#endif
    return 0;
}

} // namespace

Sorter::Sorter(const AlgoRegistry &registry) : registry_(registry) {}

std::vector<std::unique_ptr<Algo>> Sorter::selectAlgorithms(const std::vector<std::string> &requested) const {
    std::vector<std::unique_ptr<Algo>> selected;

    if (requested.empty()) {
        selected.push_back(registry_.create(registry_.firstRegisteredAlgorithmId()));
        return selected;
    }

    for (const std::string &idRaw : requested) {
        if (utils::toLower(idRaw) == "all") {
            const auto allIds = registry_.availableAlgorithms();
            for (const std::string &id : allIds) {
                selected.push_back(registry_.create(id));
            }
            return selected;
        }
    }

    if (requested.size() == 1) {
        const std::string id = utils::toLower(requested.front());
        if (!registry_.has(id)) {
            throw std::runtime_error("Requested algorithm is not available: " + id);
        }
        selected.push_back(registry_.create(id));
        return selected;
    }

    std::unordered_set<std::string> seen;
    for (const std::string &idRaw : requested) {
        const std::string id = utils::toLower(idRaw);
        if (!seen.insert(id).second) {
            continue;
        }

        if (!registry_.has(id)) {
            throw std::runtime_error("Requested algorithm is not available: " + id);
        }
        selected.push_back(registry_.create(id));
    }

    if (selected.empty()) {
        selected.push_back(registry_.create(registry_.firstRegisteredAlgorithmId()));
    }

    return selected;
}

std::vector<SortExecutionResult> Sorter::sortData(const std::vector<long long> &sourceData,
                                                  const std::vector<std::unique_ptr<Algo>> &algorithms,
                                                  const AppConfig &config, const RunArtifacts &artifacts) const {
    std::vector<SortExecutionResult> results;
    results.reserve(algorithms.size());

    for (const auto &algorithm : algorithms) {
        std::vector<long long> data = sourceData;

        const std::uint64_t rssBefore = config.benchmark.enableMemory ? currentRssKb() : 0;
        const auto startTime = std::chrono::high_resolution_clock::now();
        algorithm->sort(data);

        const std::uint64_t rssAfter = config.benchmark.enableMemory ? currentRssKb() : 0;

        const std::string outputPath =
            Paths::resolveOutputFilePath(config, artifacts, algorithm->id(), algorithms.size());
        DataIo::writeDataFile(outputPath, data);

        SortExecutionResult result;
        result.algorithmId = algorithm->id();
        result.algorithmName = algorithm->displayName();
        result.outputFile = outputPath;
        result.sorted = Check::isNonDecreasing(data);

        if (config.benchmark.enableTime) {
            result.durationMicroseconds = elapsedMicrosecondsSince(startTime);
        }

        if (config.benchmark.enableMemory) {
            result.memoryDeltaKb = rssAfter >= rssBefore ? (rssAfter - rssBefore) : 0;
        }

        results.push_back(result);
    }

    return results;
}
