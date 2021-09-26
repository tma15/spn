#pragma once
#include <iostream>
#include <set>
#include <sstream>
#include <map>


namespace spn {


class ArgumentParser {
  public:
    ArgumentParser() {}
    void add_argument(const std::string &option);
    void add_argument(const std::string &option, const std::string &defval);
    void add_argument(const std::string &option, const std::string &defval, const std::string &help);
    void parse_args(int argc, char** argv);
    void print_help();

    template<typename T> 
    T get(const std::string &option);

  private:
    std::string get_key(const std::string &option);
    std::map<std::string, std::string> args_;
    std::map<std::string, std::string> helps_;
    std::string name_;
};


void ArgumentParser::print_help() {
  std::cout << "Usage of " << name_ << ":" << std::endl << std::endl;
  for (auto it = helps_.begin(); it != helps_.end(); ++it) {
    std::cout << " " << it->first << "\t" << it->second << std::endl;
  }
  exit(1);
}


void ArgumentParser::add_argument(const std::string &option) {
  args_[get_key(option)] = "";
}


void ArgumentParser::add_argument(
    const std::string &option,
    const std::string &defval) {
  args_[get_key(option)] = defval;
  helps_[option] = "";
}


void ArgumentParser::add_argument(
    const std::string &option,
    const std::string &defval,
    const std::string &help) {
  args_[get_key(option)] = defval;
  helps_[option] = help;
}


void ArgumentParser::parse_args(int argc, char** argv) {

  name_ = std::string(argv[0]);
  for (unsigned i = 1; i < argc; ++i) {
    std::string a = argv[i];
    if (a == "-h" || a == "--help") {
      print_help();
    }

    std::string key = get_key(argv[i]);

    if (args_.find(key) != args_.end()) {
      std::string value = std::string(argv[i + 1]);
      args_[key] = value;
      i++;
    }
  }
}

template<typename T>
T ArgumentParser::get(const std::string &option) {
  T ret;

  std::string value = args_[option];
  std::stringstream ss(value);
  ss >> ret;
  return ret;
}


std::string ArgumentParser::get_key(const std::string &option) {
  std::string key;

  if (option[0] == '-' && option[1] == '-') {
    key = std::string(option.begin() + 2, option.end());
  } else if (option[0] == '-' && option[1] != '-') {
    key = std::string(option.begin() + 1, option.end());
  }
  return key;
}

} // namespace spn
