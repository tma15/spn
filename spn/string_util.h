#pragma once
#include <string>
#include <vector>


namespace spn {

inline void split(std::string str, char delimiter, std::vector<std::string> *result) {
  int first = 0;
  int last = str.find_first_of(delimiter);

  while (first < str.size()) {
    std::string subStr(str, first, last - first);

    result->push_back(subStr);

    first = last + 1;
    last = str.find_first_of(delimiter, first);

    if (last == std::string::npos) {
      last = str.size();
    }
  }
}
  
} // namespace spn
