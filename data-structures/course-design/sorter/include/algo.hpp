#pragma once

#ifndef SORTER_INCLUDE_ALGO_HPP_
#define SORTER_INCLUDE_ALGO_HPP_

#include "common.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Algo {
public:
    virtual ~Algo() = default;
    virtual std::string id() const = 0;
    virtual std::string displayName() const = 0;
    virtual void sort(std::vector<long long> &data) const = 0;
};

class AlgoRegistry {
public:
    using Factory = std::function<std::unique_ptr<Algo>()>;

    AlgoRegistry();

    void registerFactory(const std::string &id, Factory factory);
    bool has(const std::string &id) const;
    std::unique_ptr<Algo> create(const std::string &id) const;
    std::vector<std::string> availableAlgorithms() const;
    std::string firstRegisteredAlgorithmId() const;

private:
    std::unordered_map<std::string, Factory> factories_;
    std::vector<std::string> registrationOrder_;
};

#ifndef ENABLE_SHELL_SORT
#define ENABLE_SHELL_SORT 1
#endif
#ifndef ENABLE_QUICK_SORT
#define ENABLE_QUICK_SORT 1
#endif
#ifndef ENABLE_HEAP_SORT
#define ENABLE_HEAP_SORT 1
#endif
#ifndef ENABLE_MERGE_SORT
#define ENABLE_MERGE_SORT 1
#endif
#ifndef ENABLE_INTROSORT
#define ENABLE_INTROSORT 1
#endif
#ifndef ENABLE_INSERTION_SORT
#define ENABLE_INSERTION_SORT 1
#endif
#ifndef ENABLE_SELECTION_SORT
#define ENABLE_SELECTION_SORT 1
#endif
#ifndef ENABLE_BUBBLE_SORT
#define ENABLE_BUBBLE_SORT 1
#endif
#ifndef ENABLE_MEMORY_HOG_SORT
#define ENABLE_MEMORY_HOG_SORT 1
#endif
#ifndef ENABLE_STD_SORT
#define ENABLE_STD_SORT 1
#endif
#ifndef ENABLE_STABLE_SORT
#define ENABLE_STABLE_SORT 1
#endif
#ifndef ENABLE_COCKTAIL_SORT
#define ENABLE_COCKTAIL_SORT 1
#endif
#ifndef ENABLE_COMB_SORT
#define ENABLE_COMB_SORT 1
#endif

namespace algo {

void registerShell(AlgoRegistry &registry);
void registerQuick(AlgoRegistry &registry);
void registerHeap(AlgoRegistry &registry);
void registerMerge(AlgoRegistry &registry);
void registerIntrosort(AlgoRegistry &registry);
void registerInsertion(AlgoRegistry &registry);
void registerSelection(AlgoRegistry &registry);
void registerBubble(AlgoRegistry &registry);
void registerMemoryHog(AlgoRegistry &registry);
void registerStdSort(AlgoRegistry &registry);
void registerStableSort(AlgoRegistry &registry);
void registerCocktail(AlgoRegistry &registry);
void registerComb(AlgoRegistry &registry);

} // namespace algo

inline AlgoRegistry::AlgoRegistry() {
    algo::registerShell(*this);
    algo::registerQuick(*this);
    algo::registerHeap(*this);
    algo::registerMerge(*this);
    algo::registerIntrosort(*this);
    algo::registerInsertion(*this);
    algo::registerSelection(*this);
    algo::registerBubble(*this);
    algo::registerMemoryHog(*this);
    algo::registerStdSort(*this);
    algo::registerStableSort(*this);
    algo::registerCocktail(*this);
    algo::registerComb(*this);
}

inline void AlgoRegistry::registerFactory(const std::string &id, Factory factory) {
    const std::string normalizedId = utils::toLower(id);
    const bool isNew = factories_.find(normalizedId) == factories_.end();
    factories_[normalizedId] = std::move(factory);
    if (isNew) {
        registrationOrder_.push_back(normalizedId);
    }
}

inline bool AlgoRegistry::has(const std::string &id) const {
    const std::string normalizedId = utils::toLower(id);
    return factories_.find(normalizedId) != factories_.end();
}

inline std::unique_ptr<Algo> AlgoRegistry::create(const std::string &id) const {
    const std::string normalizedId = utils::toLower(id);
    const auto it = factories_.find(normalizedId);
    if (it == factories_.end()) {
        throw std::runtime_error("Algorithm not available: " + id);
    }
    return it->second();
}

inline std::vector<std::string> AlgoRegistry::availableAlgorithms() const {
    std::vector<std::string> ids;
    ids.reserve(factories_.size());
    for (const auto &entry : factories_) {
        ids.push_back(entry.first);
    }
    std::sort(ids.begin(), ids.end());
    return ids;
}

inline std::string AlgoRegistry::firstRegisteredAlgorithmId() const {
    if (registrationOrder_.empty()) {
        throw std::runtime_error("No algorithm is available in this build.");
    }
    return registrationOrder_.front();
}

#endif // SORTER_INCLUDE_ALGO_HPP_
