#pragma once

#ifndef SORTER_INCLUDE_CONFIG_HPP_
#define SORTER_INCLUDE_CONFIG_HPP_

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

enum class RunMode { Sort, Verify, Both };

struct BenchmarkOptions {
    bool enableTime = false;
    bool enableMemory = false;
    bool save = false;
    std::string outputFile;
};

struct AppConfig {
    bool useRandomInput = true;
    std::string inputFile;
    std::string randomFile;
    std::size_t randomSize = 10000;

    RunMode mode = RunMode::Sort;
    std::vector<std::string> selectedAlgorithms;

    std::optional<std::string> outputFile;
    std::unordered_map<std::string, std::string> perAlgorithmOutput;

    BenchmarkOptions benchmark;
};

struct RunArtifacts {
    std::filesystem::path runDirectory;
    std::string defaultInputFile;
    std::string defaultBenchmarkFile;
};

#endif // SORTER_INCLUDE_CONFIG_HPP_
