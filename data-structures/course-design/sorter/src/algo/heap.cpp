#include "algo.hpp"

namespace {

#if ENABLE_HEAP_SORT
class HeapSortAlgorithm : public Algo {
public:
    std::string id() const override { return "heap"; }
    std::string displayName() const override { return "Heap Sort"; }

    void sort(std::vector<long long> &data) const override {
        const int dataSize = static_cast<int>(data.size());
        for (int index = dataSize / 2 - 1; index >= 0; --index) {
            heapify(data, dataSize, index);
        }

        for (int index = dataSize - 1; index > 0; --index) {
            std::swap(data[0], data[index]);
            heapify(data, index, 0);
        }
    }

private:
    static void heapify(std::vector<long long> &data, int heapSize, int root) {
        int largest = root;
        const int left = 2 * root + 1;
        const int right = 2 * root + 2;

        if (left < heapSize && data[left] > data[largest]) {
            largest = left;
        }
        if (right < heapSize && data[right] > data[largest]) {
            largest = right;
        }

        if (largest != root) {
            std::swap(data[root], data[largest]);
            heapify(data, heapSize, largest);
        }
    }
};
#endif

} // namespace

namespace algo {

void registerHeap(AlgoRegistry &registry) {
#if ENABLE_HEAP_SORT
    registry.registerFactory("heap", []() { return std::make_unique<HeapSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
