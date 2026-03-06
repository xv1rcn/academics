#include "io.hpp"

#include "common.hpp"

#include <array>
#include <cctype>
#include <charconv>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

#if defined(_WIN32)
#include <process.h>
#endif

#if defined(__linux__)
#include <unistd.h>
#endif

namespace {

constexpr std::size_t kIoBufferFlushThreshold = 1 << 20;

long long randomLongLong() {
    static thread_local std::mt19937_64 generator(std::random_device{}());
    static thread_local std::uniform_int_distribution<long long> distribution(std::numeric_limits<long long>::min(),
                                                                              std::numeric_limits<long long>::max());
    return distribution(generator);
}

std::string currentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime{};

#if defined(_WIN32)
    localtime_s(&localTime, &currentTime);
#elif defined(__linux__) || defined(__APPLE__)
    localtime_r(&currentTime, &localTime);
#else
    localTime = *std::localtime(&currentTime);
#endif

    std::ostringstream builder;
    builder << std::put_time(&localTime, "%Y%m%d_%H%M%S");
    return builder.str();
}

std::string shortHash() {
    std::ostringstream seedBuilder;
    seedBuilder << std::chrono::high_resolution_clock::now().time_since_epoch().count();
#if defined(__linux__)
    seedBuilder << '_' << static_cast<long long>(::getpid());
#elif defined(_WIN32)
    seedBuilder << '_' << static_cast<long long>(_getpid());
#endif
    std::random_device randomDevice;
    seedBuilder << '_' << randomDevice();

    const std::size_t seedHash = std::hash<std::string>{}(seedBuilder.str());

    std::ostringstream hashStream;
    hashStream << std::hex << std::setw(6) << std::setfill('0') << (seedHash & 0xFFFFFF);
    return hashStream.str();
}

void ensureParentDirectory(const std::filesystem::path &filePath) {
    const std::filesystem::path parent = filePath.parent_path();
    if (parent.empty()) {
        return;
    }

    std::error_code error;
    std::filesystem::create_directories(parent, error);
    if (error) {
        throw std::runtime_error("Failed to create parent directory: " + parent.string());
    }
}

void flushBufferIfNeeded(std::ofstream &out, std::string &buffer) {
    if (buffer.size() < kIoBufferFlushThreshold) {
        return;
    }
    out.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    if (!out.good()) {
        throw std::runtime_error("Failed while writing output data.");
    }
    buffer.clear();
}

bool isWhitespace(char ch) {
    return std::isspace(static_cast<unsigned char>(ch)) != 0;
}

long long parseLongLongToken(std::string_view token) {
    long long value = 0;
    const char *begin = token.data();
    const char *end = token.data() + token.size();
    const auto [ptr, error] = std::from_chars(begin, end, value);
    if (error != std::errc() || ptr != end) {
        throw std::runtime_error("Invalid numeric token: " + std::string(token));
    }
    return value;
}

std::string resolveMultiAlgorithmPath(const std::string &templatePath, const std::string &algorithmId) {
    const std::string placeholder = "{algo}";
    const std::size_t placeholderPos = templatePath.find(placeholder);
    if (placeholderPos != std::string::npos) {
        std::string resolved = templatePath;
        resolved.replace(placeholderPos, placeholder.size(), algorithmId);
        return resolved;
    }

    std::filesystem::path path(templatePath);
    if (path.has_extension()) {
        const std::string stem = path.stem().string();
        const std::string extension = path.extension().string();
        const std::filesystem::path parent = path.parent_path();
        return (parent / (stem + "_" + algorithmId + extension)).string();
    }

    return (path / ("data_sorted_" + algorithmId + ".txt")).string();
}

} // namespace

void DataIo::generateRandomDataFile(const std::string &filename, std::size_t count) {
    ensureParentDirectory(filename);

    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to create random data file: " + filename);
    }

    std::string buffer;
    buffer.reserve(kIoBufferFlushThreshold);

    for (std::size_t index = 0; index < count; ++index) {
        if (index > 0) {
            buffer.push_back('\n');
        }
        buffer += std::to_string(randomLongLong());
        flushBufferIfNeeded(out, buffer);
    }

    if (!buffer.empty()) {
        out.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        if (!out.good()) {
            throw std::runtime_error("Failed while writing random data file: " + filename);
        }
    }
}

std::vector<long long> DataIo::readDataFile(const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        throw std::runtime_error("Failed to open input file: " + filename);
    }

    std::vector<long long> data;

    constexpr std::size_t kChunkSize = 1 << 20;

#if defined(_MSC_VER)
    std::vector<char> heapBuffer(kChunkSize);
    char *bufferPtr = heapBuffer.data();
#else
    std::array<char, kChunkSize> buffer{};
    char *bufferPtr = buffer.data();
#endif

    std::string pending;
    pending.reserve(kChunkSize * 2);

    auto consumeCompleteTokens = [&]() {
        std::size_t index = 0;
        const std::size_t size = pending.size();

        while (index < size) {
            while (index < size && isWhitespace(pending[index])) {
                ++index;
            }
            if (index >= size) {
                pending.clear();
                return;
            }

            const std::size_t tokenBegin = index;
            while (index < size && !isWhitespace(pending[index])) {
                ++index;
            }

            if (index == size) {
                pending.erase(0, tokenBegin);
                return;
            }

            data.push_back(parseLongLongToken(std::string_view(pending.data() + tokenBegin, index - tokenBegin)));
        }

        pending.clear();
    };

    while (in) {
        in.read(bufferPtr, static_cast<std::streamsize>(kChunkSize));
        const std::streamsize readCount = in.gcount();
        if (readCount <= 0) {
            break;
        }
        pending.append(bufferPtr, static_cast<std::size_t>(readCount));
        consumeCompleteTokens();
    }

    if (!pending.empty()) {
        std::size_t begin = 0;
        while (begin < pending.size() && isWhitespace(pending[begin])) {
            ++begin;
        }
        if (begin < pending.size()) {
            data.push_back(parseLongLongToken(std::string_view(pending.data() + begin, pending.size() - begin)));
        }
    }

    return data;
}

void DataIo::writeDataFile(const std::string &filename, const std::vector<long long> &data) {
    ensureParentDirectory(filename);

    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to create output file: " + filename);
    }

    std::string buffer;
    buffer.reserve(kIoBufferFlushThreshold);

    for (std::size_t index = 0; index < data.size(); ++index) {
        if (index > 0) {
            buffer.push_back('\n');
        }
        buffer += std::to_string(data[index]);
        flushBufferIfNeeded(out, buffer);
    }

    if (!buffer.empty()) {
        out.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        if (!out.good()) {
            throw std::runtime_error("Failed while writing output file: " + filename);
        }
    }
}

void DataIo::writeBenchmarkReport(const std::string &filename, const std::vector<SortExecutionResult> &results,
                                  bool hasTime, bool hasMemory) {
    ensureParentDirectory(filename);

    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to create benchmark report file: " + filename);
    }

    out << "algorithm_id,algorithm_name,output_file,sorted";
    if (hasTime) {
        out << ",duration_us";
    }
    if (hasMemory) {
        out << ",memory_delta_kb";
    }
    out << '\n';

    for (const auto &result : results) {
        out << result.algorithmId << ',' << result.algorithmName << ',' << result.outputFile << ','
            << (result.sorted ? "true" : "false");
        if (hasTime) {
            out << ','
                << (result.durationMicroseconds.has_value() ? std::to_string(result.durationMicroseconds.value()) : "");
        }
        if (hasMemory) {
            out << ',' << (result.memoryDeltaKb.has_value() ? std::to_string(result.memoryDeltaKb.value()) : "");
        }
        out << '\n';
    }
}

std::string Paths::resolveInputFilePath(const AppConfig &config, const RunArtifacts &artifacts) {
    if (config.useRandomInput) {
        if (!config.randomFile.empty()) {
            return config.randomFile;
        }
        return artifacts.defaultInputFile;
    }

    if (config.inputFile.empty()) {
        throw std::runtime_error("--input-file is required when --no-random is used.");
    }
    return config.inputFile;
}

std::string Paths::resolveBenchmarkFilePath(const AppConfig &config, const RunArtifacts &artifacts) {
    if (!config.benchmark.outputFile.empty()) {
        return config.benchmark.outputFile;
    }
    return artifacts.defaultBenchmarkFile;
}

std::string Paths::resolveOutputFilePath(const AppConfig &config, const RunArtifacts &artifacts,
                                         const std::string &algorithmId, std::size_t algorithmCount) {
    const std::string normalizedId = utils::toLower(algorithmId);

    const auto mapIt = config.perAlgorithmOutput.find(normalizedId);
    if (mapIt != config.perAlgorithmOutput.end()) {
        return mapIt->second;
    }

    if (config.outputFile.has_value()) {
        if (algorithmCount == 1) {
            return config.outputFile.value();
        }
        return resolveMultiAlgorithmPath(config.outputFile.value(), normalizedId);
    }

    return (artifacts.runDirectory / ("data_sorted_" + normalizedId + ".txt")).string();
}

std::filesystem::path RunDir::createRunDirectory(const std::filesystem::path &baseDirectory) {
    std::error_code error;
    std::filesystem::create_directories(baseDirectory, error);
    if (error) {
        throw std::runtime_error("Failed to create base directory: " + baseDirectory.string());
    }

    const std::filesystem::path runDirectory = baseDirectory / ("run_" + currentTimestamp() + "_" + shortHash());
    std::filesystem::create_directories(runDirectory, error);
    if (error) {
        throw std::runtime_error("Failed to create run directory: " + runDirectory.string());
    }

    return runDirectory;
}
