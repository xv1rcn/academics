#pragma once

#ifndef FINDER_INCLUDE_COMMON_TYPES_HPP_
#define FINDER_INCLUDE_COMMON_TYPES_HPP_

#include <cstddef>
#include <vector>

namespace common {

struct MatchRange {
    std::size_t begin = 0;
    std::size_t end = 0;
};

using MatchRanges = std::vector<MatchRange>;

} // namespace common

#endif // FINDER_INCLUDE_COMMON_TYPES_HPP_
