#pragma once
#include <string>
#include <unordered_map>
#include <vector>


namespace spn {

class Dictionary {
  public:
    typedef std::vector<std::string>::iterator iterator;
    typedef std::vector<std::string>::const_iterator const_iterator;

    Dictionary() {}

    void add(const std::string &item);

    bool contains(const std::string &item) const;

    int get_index(const std::string &item) const;

    const std::string &get_item(int index) const;

    size_t size() const { return items_.size(); }

    iterator begin() { return items_.begin(); }

    const_iterator begin() const { return items_.begin(); }

    iterator end() { return items_.end(); }

    const_iterator end() const { return items_.end(); }

  private:
    std::vector<std::string> items_;
    std::unordered_map<std::string, int> index_;
};
  
} // namespace spn
