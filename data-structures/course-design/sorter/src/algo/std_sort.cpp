#include "algo.hpp"

#include <algorithm>

namespace {

#if ENABLE_STD_SORT
class StdSortAlgorithm : public Algo {
public:
    std::string id() const override { return "stdsort"; }
    std::string displayName() const override { return "Std Sort"; }

    void sort(std::vector<long long> &data) const override { std::sort(data.begin(), data.end()); }
};
#endif

} // namespace

namespace algo {

void registerStdSort(AlgoRegistry &registry) {
#if ENABLE_STD_SORT
    registry.registerFactory("stdsort", []() { return std::make_unique<StdSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
