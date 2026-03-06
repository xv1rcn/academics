#include "algo.hpp"

#include <algorithm>
#include <cmath>

namespace {

#if ENABLE_INTROSORT
class IntroSortAlgorithm : public Algo {
public:
    std::string id() const override { return "introsort"; }
    std::string displayName() const override { return "Introsort"; }

    void sort(std::vector<long long> &data) const override {
        if (data.empty()) {
            return;
        }

        const int depthLimit = static_cast<int>(2 * std::floor(std::log2(data.size())));
        introsort(data, 0, static_cast<int>(data.size()), depthLimit);
        insertionSort(data, 0, static_cast<int>(data.size()));
    }

private:
    static constexpr int kInsertionSortThreshold = 16;

    static void introsort(std::vector<long long> &data, int begin, int end, int depthLimit) {
        while (end - begin > kInsertionSortThreshold) {
            if (depthLimit == 0) {
                heapSortRange(data, begin, end);
                return;
            }
            --depthLimit;

            const int pivot = partition(data, begin, end);
            introsort(data, pivot, end, depthLimit);
            end = pivot;
        }
    }

    static int partition(std::vector<long long> &data, int begin, int end) {
        const int middle = begin + (end - begin) / 2;
        const int last = end - 1;

        if (data[middle] < data[begin]) {
            std::swap(data[middle], data[begin]);
        }
        if (data[last] < data[middle]) {
            std::swap(data[last], data[middle]);
        }
        if (data[middle] < data[begin]) {
            std::swap(data[middle], data[begin]);
        }

        const long long pivot = data[middle];
        int left = begin;
        int right = end - 1;

        while (true) {
            while (data[left] < pivot) {
                ++left;
            }
            while (pivot < data[right]) {
                --right;
            }
            if (left >= right) {
                return right + 1;
            }
            std::swap(data[left], data[right]);
            ++left;
            --right;
        }
    }

    static void insertionSort(std::vector<long long> &data, int begin, int end) {
        for (int index = begin + 1; index < end; ++index) {
            const long long value = data[index];
            int position = index;
            while (position > begin && value < data[position - 1]) {
                data[position] = data[position - 1];
                --position;
            }
            data[position] = value;
        }
    }

    static void heapSortRange(std::vector<long long> &data, int begin, int end) {
        auto first = data.begin() + begin;
        auto last = data.begin() + end;
        std::make_heap(first, last);
        std::sort_heap(first, last);
    }
};
#endif

} // namespace

namespace algo {

void registerIntrosort(AlgoRegistry &registry) {
#if ENABLE_INTROSORT
    registry.registerFactory("introsort", []() { return std::make_unique<IntroSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
