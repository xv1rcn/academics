#include "algo.hpp"

namespace {

#if ENABLE_SHELL_SORT
class ShellSortAlgorithm : public Algo {
public:
    std::string id() const override { return "shell"; }
    std::string displayName() const override { return "Shell Sort"; }

    void sort(std::vector<long long> &data) const override {
        const int dataSize = static_cast<int>(data.size());
        for (int gap = dataSize / 2; gap > 0; gap /= 2) {
            for (int index = gap; index < dataSize; ++index) {
                const long long currentValue = data[index];
                int position = index;
                while (position >= gap && data[position - gap] > currentValue) {
                    data[position] = data[position - gap];
                    position -= gap;
                }
                data[position] = currentValue;
            }
        }
    }
};
#endif

} // namespace

namespace algo {

void registerShell(AlgoRegistry &registry) {
#if ENABLE_SHELL_SORT
    registry.registerFactory("shell", []() { return std::make_unique<ShellSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
