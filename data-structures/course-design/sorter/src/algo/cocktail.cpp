#include "algo.hpp"

namespace {

#if ENABLE_COCKTAIL_SORT
class CocktailSortAlgorithm : public Algo {
public:
    std::string id() const override { return "cocktail"; }
    std::string displayName() const override { return "Cocktail Sort"; }

    void sort(std::vector<long long> &data) const override {
        if (data.empty()) {
            return;
        }

        int left = 0;
        int right = static_cast<int>(data.size()) - 1;
        bool swapped = true;

        while (swapped && left < right) {
            swapped = false;
            for (int index = left; index < right; ++index) {
                if (data[index] > data[index + 1]) {
                    std::swap(data[index], data[index + 1]);
                    swapped = true;
                }
            }
            --right;

            if (!swapped) {
                break;
            }

            swapped = false;
            for (int index = right; index > left; --index) {
                if (data[index - 1] > data[index]) {
                    std::swap(data[index - 1], data[index]);
                    swapped = true;
                }
            }
            ++left;
        }
    }
};
#endif

} // namespace

namespace algo {

void registerCocktail(AlgoRegistry &registry) {
#if ENABLE_COCKTAIL_SORT
    registry.registerFactory("cocktail", []() { return std::make_unique<CocktailSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
