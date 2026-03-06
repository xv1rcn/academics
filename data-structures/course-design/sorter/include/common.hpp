#pragma once

#ifndef SORTER_INCLUDE_COMMON_HPP_
#define SORTER_INCLUDE_COMMON_HPP_

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>
#include <vector>

namespace utils {

inline std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return value;
}

} // namespace utils

class Check {
public:
    static bool isNonDecreasing(const std::vector<long long> &data) {
        for (std::size_t index = 1; index < data.size(); ++index) {
            if (data[index] < data[index - 1]) {
                return false;
            }
        }
        return true;
    }
};

#endif // SORTER_INCLUDE_COMMON_HPP_
