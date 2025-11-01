#include <functional>
#include <sstream>

#include "utils.h"

std::string vecToStr(const std::vector<char> &v) {
    std::ostringstream oss;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i)
            oss << ' ';
        oss << v[i];
    }
    return oss.str();
}

std::vector<char> parseCharTokens(const std::string &s) {
    std::vector<char> tokens;
    if (s.find(' ') != std::string::npos) {
        std::istringstream iss(s);
        std::string t;
        while (iss >> t)
            tokens.push_back(t[0]);
    } else {
        for (char c : s)
            tokens.push_back(c);
    }
    return tokens;
}

bool isValidPreorder(const std::vector<char> &tokens, char nullChar) {
    size_t n = tokens.size();
    size_t idx = 0;
    std::function<bool()> rec = [&]() -> bool {
        if (idx >= n)
            return false;
        char cur = tokens[idx++];
        if (cur == nullChar)
            return true;
        if (!rec())
            return false;
        if (!rec())
            return false;
        return true;
    };
    bool ok = rec();
    return ok && idx == n;
}
