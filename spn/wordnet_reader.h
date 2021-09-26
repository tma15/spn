#pragma once
#include <string>
#include <vector>

#include <sqlite3.h>


namespace spn {

class WordNetReader {
  public:
    WordNetReader() {}

    WordNetReader(const std::string &db_file, const std::string lang="jpn");

    void lang(const std::string lang) { lang_ = lang; }

    int get_word_idx(const std::string &word);

    void get_words(std::vector<std::string> *words);

    void get_synset_ids(
        int word_idx,
        std::vector<std::string> *synset_ids);

    void get_definitions(
        const std::string &word,
        std::vector<std::string> *definitions);

  private:
    std::string db_file_;
    std::string lang_;
    sqlite3 *db_;
    sqlite3_stmt *stmt_;
};
 
} // namespace spn
