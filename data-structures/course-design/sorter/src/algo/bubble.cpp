#include "algo.hpp"

namespace {

#if ENABLE_BUBBLE_SORT
class BubbleSortAlgorithm : public Algo {
public:
    std::string id() const override { return "bubble"; }
    std::string displayName() const override { return "Bubble Sort"; }

    void sort(std::vector<long long> &data) const override {
        const int dataSize = static_cast<int>(data.size());
        for (int pass = 0; pass < dataSize - 1; ++pass) {
            bool swapped = false;
            for (int index = 0; index < dataSize - pass - 1; ++index) {
                if (data[index] > data[index + 1]) {
                    std::swap(data[index], data[index + 1]);
                    swapped = true;
                }
            }
            if (!swapped) {
                break;
            }
        }
    }
};
#endif

} // namespace

namespace algo {

void registerBubble(AlgoRegistry &registry) {
#if ENABLE_BUBBLE_SORT
    registry.registerFactory("bubble", []() { return std::make_unique<BubbleSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
