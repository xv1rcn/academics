#include "algo.hpp"

namespace {

#if ENABLE_MERGE_SORT
class MergeSortAlgorithm : public Algo {
public:
    std::string id() const override { return "merge"; }
    std::string displayName() const override { return "Merge Sort"; }

    void sort(std::vector<long long> &data) const override {
        if (data.empty()) {
            return;
        }
        mergeSort(data, 0, static_cast<int>(data.size()) - 1);
    }

private:
    static void merge(std::vector<long long> &data, int left, int middle, int right) {
        const int leftSize = middle - left + 1;
        const int rightSize = right - middle;

        std::vector<long long> leftPart(leftSize);
        std::vector<long long> rightPart(rightSize);

        for (int index = 0; index < leftSize; ++index) {
            leftPart[index] = data[left + index];
        }
        for (int index = 0; index < rightSize; ++index) {
            rightPart[index] = data[middle + 1 + index];
        }

        int leftIndex = 0;
        int rightIndex = 0;
        int mergedIndex = left;

        while (leftIndex < leftSize && rightIndex < rightSize) {
            if (leftPart[leftIndex] <= rightPart[rightIndex]) {
                data[mergedIndex++] = leftPart[leftIndex++];
            } else {
                data[mergedIndex++] = rightPart[rightIndex++];
            }
        }

        while (leftIndex < leftSize) {
            data[mergedIndex++] = leftPart[leftIndex++];
        }
        while (rightIndex < rightSize) {
            data[mergedIndex++] = rightPart[rightIndex++];
        }
    }

    static void mergeSort(std::vector<long long> &data, int left, int right) {
        if (left < right) {
            const int middle = left + (right - left) / 2;
            mergeSort(data, left, middle);
            mergeSort(data, middle + 1, right);
            merge(data, left, middle, right);
        }
    }
};
#endif

} // namespace

namespace algo {

void registerMerge(AlgoRegistry &registry) {
#if ENABLE_MERGE_SORT
    registry.registerFactory("merge", []() { return std::make_unique<MergeSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
