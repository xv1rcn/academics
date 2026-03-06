#include "algo.hpp"

namespace {

#if ENABLE_QUICK_SORT
class QuickSortAlgorithm : public Algo {
public:
    std::string id() const override { return "quick"; }
    std::string displayName() const override { return "Quick Sort"; }

    void sort(std::vector<long long> &data) const override {
        if (data.empty()) {
            return;
        }
        quickSort(data, 0, static_cast<int>(data.size()) - 1);
    }

private:
    static int partition(std::vector<long long> &data, int low, int high) {
        const long long pivot = data[high];
        int current = low - 1;
        for (int index = low; index < high; ++index) {
            if (data[index] <= pivot) {
                ++current;
                std::swap(data[current], data[index]);
            }
        }
        std::swap(data[current + 1], data[high]);
        return current + 1;
    }

    static void quickSort(std::vector<long long> &data, int low, int high) {
        if (low < high) {
            const int pivotIndex = partition(data, low, high);
            quickSort(data, low, pivotIndex - 1);
            quickSort(data, pivotIndex + 1, high);
        }
    }
};
#endif

} // namespace

namespace algo {

void registerQuick(AlgoRegistry &registry) {
#if ENABLE_QUICK_SORT
    registry.registerFactory("quick", []() { return std::make_unique<QuickSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
