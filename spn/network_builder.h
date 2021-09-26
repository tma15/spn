#pragma once
#include <set>
#include <unordered_map>

#include <mecab.h>

#include "dictionary.h"
#include "link_weight_calculator.h"
#include "wordnet_reader.h"


namespace spn {


class LexiconNetworkBuilder {
  public:
    LexiconNetworkBuilder() {}

    LexiconNetworkBuilder(
        const WordNetReader &reader,
        float alpha=1e+3,
        float beta=1.);

    void build(
        int max_epoch,
        const std::string &out_file,
        const std::vector<std::string> &positive={},
        const std::vector<std::string> &negative={});

    void preprocess(
        const std::vector<std::string> &positive={},
        const std::vector<std::string> &negative={});

    void train(int max_epoch, const std::string &out_file);

  private:
    void extract_tokens(
        MeCab::Tagger *tagger,
        const std::string &text,
        const std::vector<std::string> &positive,
        const std::vector<std::string> &negative,
        std::vector<std::string> *tokens);

    Dictionary dictionary_;
    WordNetReader reader_;

    std::unordered_map<std::string, std::set<std::string>> adj_words_;
    std::unordered_map<int, std::set<int>> adj_ids_;
    std::unique_ptr<LinkWeightCalculator> link_weight_calculator_ = nullptr;
    std::set<int> positive_idx_;
    std::set<int> negative_idx_;
    float alpha_;
    float beta_;
    float *spin_;
};
  
} // namespace spn
