#pragma once

#ifndef SORTER_INCLUDE_SORT_HPP_
#define SORTER_INCLUDE_SORT_HPP_

#include "algo.hpp"
#include "config.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

struct SortExecutionResult {
    std::string algorithmId;
    std::string algorithmName;
    std::string outputFile;
    bool sorted = false;
    std::optional<long long> durationMicroseconds;
    std::optional<std::uint64_t> memoryDeltaKb;
};

class Sorter {
public:
    explicit Sorter(const AlgoRegistry &registry);

    std::vector<std::unique_ptr<Algo>> selectAlgorithms(const std::vector<std::string> &requested) const;

    std::vector<SortExecutionResult> sortData(const std::vector<long long> &sourceData,
                                              const std::vector<std::unique_ptr<Algo>> &algorithms,
                                              const AppConfig &config, const RunArtifacts &artifacts) const;

private:
    const AlgoRegistry &registry_;
};

#endif // SORTER_INCLUDE_SORT_HPP_
