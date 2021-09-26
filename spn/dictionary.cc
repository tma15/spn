#include <iostream>
#include "dictionary.h"


namespace spn {

void Dictionary::add(const std::string &item) {
  if (!contains(item)) {
    int idx = items_.size();
    items_.push_back(item);
    index_.insert(std::make_pair(item, idx));
  }
}


bool Dictionary::contains(const std::string &item) const {
  return index_.find(item) != index_.end();
}


int Dictionary::get_index(const std::string &item) const {
  if (!contains(item)) {
    std::cerr << "Key " << item << " does not exist" << std::endl;
  }

  auto it = index_.find(item);
  return it->second;
}


const std::string &Dictionary::get_item(int index) const {
  if (index >= items_.size()) {
    std::cerr << "Index " << index << " is larger than dictionary size"
              << " " << size() << std::endl;
  }
  return items_[index];
}
  
} // namespace spn
