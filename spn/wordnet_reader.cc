#include <iostream>
#include <sstream>
#include "error.h"
#include "wordnet_reader.h"


namespace spn {

WordNetReader::WordNetReader(
    const std::string &db_file,
    const std::string lang) {

  int ret;
  ret = sqlite3_open_v2(db_file.c_str(),
                        &db_,
                        SQLITE_OPEN_READONLY,
                        nullptr);

  SPN_ABORT_IF_NOT_EQ(ret, SQLITE_OK, sqlite3_errmsg(db_));

  if (lang != "jpn") {
    SPN_RUNTIME_ERROR("Only lang=jpn is supported!");
  }
  lang_ = lang;
}


int WordNetReader::get_word_idx(const std::string &word) {
  std::stringstream ss;
  ss << "SELECT * FROM word WHERE lemma='" << word
     << "' and lang ='" << lang_ << "'";
  std::string q = ss.str();

  int ret = sqlite3_prepare_v2(
      db_,
      q.c_str(),
      -1,
      &stmt_,
      nullptr);
  SPN_ABORT_IF_NOT_EQ(ret, SQLITE_OK, sqlite3_errmsg(db_));

  int idx;
  while(sqlite3_step(stmt_) == SQLITE_ROW) {
    idx = sqlite3_column_int64(stmt_, 0);
    break;
  }
  SPN_ABORT_IF_NOT_EQ(sqlite3_reset(stmt_), SQLITE_OK, sqlite3_errmsg(db_));
  SPN_ABORT_IF_NOT_EQ(sqlite3_clear_bindings(stmt_), SQLITE_OK, sqlite3_errmsg(db_));
  return idx;
}


void WordNetReader::get_words(std::vector<std::string> *words) {
  std::stringstream ss;
  ss << "SELECT * FROM word WHERE lang ='" << lang_ << "'";
  std::string q = ss.str();

  int ret = sqlite3_prepare_v2(
      db_,
      q.c_str(),
      -1,
      &stmt_,
      nullptr);
  SPN_ABORT_IF_NOT_EQ(ret, SQLITE_OK, sqlite3_errmsg(db_));

  int idx;
  while(sqlite3_step(stmt_) == SQLITE_ROW) {
    const unsigned char *lemma = sqlite3_column_text(stmt_, 2);
    const unsigned char *pos = sqlite3_column_text(stmt_, 4);
    std::string s(reinterpret_cast<const char *>(lemma));
    words->push_back(s);
  }
  SPN_ABORT_IF_NOT_EQ(sqlite3_reset(stmt_), SQLITE_OK, sqlite3_errmsg(db_));
  SPN_ABORT_IF_NOT_EQ(sqlite3_clear_bindings(stmt_), SQLITE_OK, sqlite3_errmsg(db_));
}

 
void WordNetReader::get_synset_ids(
    int word_idx,
    std::vector<std::string> *synset_ids) {

  std::stringstream ss;
  ss << "SELECT * FROM sense WHERE wordid=" << word_idx;
  std::string q = ss.str();

  int ret = sqlite3_prepare_v2(
      db_,
      q.c_str(),
      -1,
      &stmt_,
      nullptr);
  SPN_ABORT_IF_NOT_EQ(ret, SQLITE_OK, sqlite3_errmsg(db_));

  while(sqlite3_step(stmt_) == SQLITE_ROW) {
    const unsigned char *syn = sqlite3_column_text(stmt_, 0);
    std::string s(reinterpret_cast<const char *>(syn));
    synset_ids->push_back(s);
  }
  SPN_ABORT_IF_NOT_EQ(sqlite3_reset(stmt_), SQLITE_OK, sqlite3_errmsg(db_));
  SPN_ABORT_IF_NOT_EQ(sqlite3_clear_bindings(stmt_), SQLITE_OK, sqlite3_errmsg(db_));
}


void WordNetReader::get_definitions(
    const std::string &word,
    std::vector<std::string> *definitions) {

  int word_idx = get_word_idx(word);
  std::vector<std::string> synset_ids;
  get_synset_ids(word_idx, &synset_ids);

  for (auto &syn_idx : synset_ids) {
    std::stringstream ss;
    ss << "SELECT * FROM synset_def WHERE synset='" << syn_idx << "'"
       << " and lang = '" << lang_ << "'";
    std::string q = ss.str();

    int ret = sqlite3_prepare_v2(
        db_,
        q.c_str(),
        -1,
        &stmt_,
        nullptr);
    SPN_ABORT_IF_NOT_EQ(ret, SQLITE_OK, sqlite3_errmsg(db_));

    while(sqlite3_step(stmt_) == SQLITE_ROW) {
      const unsigned char *def = sqlite3_column_text(stmt_, 2);
      std::string s(reinterpret_cast<const char *>(def));
      definitions->push_back(s);
    }
    SPN_ABORT_IF_NOT_EQ(sqlite3_reset(stmt_), SQLITE_OK, sqlite3_errmsg(db_));
    SPN_ABORT_IF_NOT_EQ(sqlite3_clear_bindings(stmt_), SQLITE_OK, sqlite3_errmsg(db_));
  }
}
  
} // namespace spn
