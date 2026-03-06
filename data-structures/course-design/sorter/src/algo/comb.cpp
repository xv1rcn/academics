#include "algo.hpp"

namespace {

#if ENABLE_COMB_SORT
class CombSortAlgorithm : public Algo {
public:
    std::string id() const override { return "comb"; }
    std::string displayName() const override { return "Comb Sort"; }

    void sort(std::vector<long long> &data) const override {
        const int dataSize = static_cast<int>(data.size());
        if (dataSize <= 1) {
            return;
        }

        int gap = dataSize;
        bool swapped = true;

        while (gap > 1 || swapped) {
            gap = (gap * 10) / 13;
            if (gap < 1) {
                gap = 1;
            }

            swapped = false;
            for (int index = 0; index + gap < dataSize; ++index) {
                if (data[index] > data[index + gap]) {
                    std::swap(data[index], data[index + gap]);
                    swapped = true;
                }
            }
        }
    }
};
#endif

} // namespace

namespace algo {

void registerComb(AlgoRegistry &registry) {
#if ENABLE_COMB_SORT
    registry.registerFactory("comb", []() { return std::make_unique<CombSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
