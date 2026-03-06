#include "algo.hpp"

namespace {

#if ENABLE_SELECTION_SORT
class SelectionSortAlgorithm : public Algo {
public:
    std::string id() const override { return "selection"; }
    std::string displayName() const override { return "Selection Sort"; }

    void sort(std::vector<long long> &data) const override {
        const int dataSize = static_cast<int>(data.size());
        for (int index = 0; index < dataSize - 1; ++index) {
            int minimumIndex = index;
            for (int candidate = index + 1; candidate < dataSize; ++candidate) {
                if (data[candidate] < data[minimumIndex]) {
                    minimumIndex = candidate;
                }
            }
            if (minimumIndex != index) {
                std::swap(data[index], data[minimumIndex]);
            }
        }
    }
};
#endif

} // namespace

namespace algo {

void registerSelection(AlgoRegistry &registry) {
#if ENABLE_SELECTION_SORT
    registry.registerFactory("selection", []() { return std::make_unique<SelectionSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
