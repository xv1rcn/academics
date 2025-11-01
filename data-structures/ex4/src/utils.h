#pragma once

#include <string>
#include <vector>

std::string vecToStr(const std::vector<char>& v);
std::vector<char> parseCharTokens(const std::string& s);
bool isValidPreorder(const std::vector<char>& tokens, char nullChar);
