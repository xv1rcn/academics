#pragma once

#ifndef SORTER_INCLUDE_IO_HPP_
#define SORTER_INCLUDE_IO_HPP_

#include "config.hpp"
#include "sort.hpp"

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

class DataIo {
public:
    static void generateRandomDataFile(const std::string &filename, std::size_t count);
    static std::vector<long long> readDataFile(const std::string &filename);
    static void writeDataFile(const std::string &filename, const std::vector<long long> &data);
    static void writeBenchmarkReport(const std::string &filename, const std::vector<SortExecutionResult> &results,
                                     bool hasTime, bool hasMemory);
};

class Paths {
public:
    static std::string resolveInputFilePath(const AppConfig &config, const RunArtifacts &artifacts);
    static std::string resolveBenchmarkFilePath(const AppConfig &config, const RunArtifacts &artifacts);
    static std::string resolveOutputFilePath(const AppConfig &config, const RunArtifacts &artifacts,
                                             const std::string &algorithmId, std::size_t algorithmCount);
};

class RunDir {
public:
    static std::filesystem::path createRunDirectory(const std::filesystem::path &baseDirectory);
};

#endif // SORTER_INCLUDE_IO_HPP_
