#pragma once
#include <vector>
#include <string>

std::vector<std::string> split_string(std::string& str, std::string& crit) {
    std::string split = "";
    std::vector<std::string> splits;
    unsigned int start = str.find_first_of(" ");
    start++;
    for (; start < str.length(); start++) {
        if (str.substr(start, 1) != crit) {
            split += str[start]; 
        } else if (str.substr(start, 1) == crit || start == (unsigned int)(str.length() - 1)) {
            splits.push_back(split);
            split = "";
        }
    }
    splits.push_back(split); // Hack: push the last string here because I can't tell end of line in the loop
    return splits;
}