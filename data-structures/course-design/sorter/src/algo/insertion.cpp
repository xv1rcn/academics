#include "algo.hpp"

namespace {

#if ENABLE_INSERTION_SORT
class InsertionSortAlgorithm : public Algo {
public:
    std::string id() const override { return "insertion"; }
    std::string displayName() const override { return "Insertion Sort"; }

    void sort(std::vector<long long> &data) const override {
        const int dataSize = static_cast<int>(data.size());
        for (int index = 1; index < dataSize; ++index) {
            const long long key = data[index];
            int position = index - 1;

            while (position >= 0 && data[position] > key) {
                data[position + 1] = data[position];
                --position;
            }
            data[position + 1] = key;
        }
    }
};
#endif

} // namespace

namespace algo {

void registerInsertion(AlgoRegistry &registry) {
#if ENABLE_INSERTION_SORT
    registry.registerFactory("insertion", []() { return std::make_unique<InsertionSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
