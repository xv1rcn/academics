#include "algo.hpp"

#include <algorithm>

namespace {

#if ENABLE_STABLE_SORT
class StableSortAlgorithm : public Algo {
public:
    std::string id() const override { return "stable"; }
    std::string displayName() const override { return "Stable Sort"; }

    void sort(std::vector<long long> &data) const override { std::stable_sort(data.begin(), data.end()); }
};
#endif

} // namespace

namespace algo {

void registerStableSort(AlgoRegistry &registry) {
#if ENABLE_STABLE_SORT
    registry.registerFactory("stable", []() { return std::make_unique<StableSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
