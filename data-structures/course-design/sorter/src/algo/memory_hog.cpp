#include "algo.hpp"

namespace {

#if ENABLE_MEMORY_HOG_SORT
class MemoryHogSortAlgorithm : public Algo {
public:
    std::string id() const override { return "memhog"; }
    std::string displayName() const override { return "Memory Hog Sort"; }

    void sort(std::vector<long long> &data) const override {
        constexpr std::size_t kRetentionFactor = 8;
        constexpr std::size_t kMaxRetainedElements = 8'000'000;

        static std::vector<long long> retainedBuffer;

        std::size_t requested = data.size() * kRetentionFactor;
        if (requested / kRetentionFactor != data.size()) {
            requested = kMaxRetainedElements;
        }
        if (requested > kMaxRetainedElements) {
            requested = kMaxRetainedElements;
        }

        if (retainedBuffer.size() < requested) {
            retainedBuffer.resize(requested);
        }
        for (std::size_t index = 0; index < retainedBuffer.size(); ++index) {
            retainedBuffer[index] = static_cast<long long>(index);
        }

        std::sort(data.begin(), data.end());
    }
};
#endif

} // namespace

namespace algo {

void registerMemoryHog(AlgoRegistry &registry) {
#if ENABLE_MEMORY_HOG_SORT
    registry.registerFactory("memhog", []() { return std::make_unique<MemoryHogSortAlgorithm>(); });
#else
    (void)registry;
#endif
}

} // namespace algo
