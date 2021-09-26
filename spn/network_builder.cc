#include <iomanip>
#include <iostream>
#include <fstream>
#include <memory>

#include "wordnet_reader.h"
#include "network_builder.h"
#include "string_util.h"


namespace spn {

LexiconNetworkBuilder::LexiconNetworkBuilder(
    const WordNetReader &reader,
    float alpha,
    float beta)
  : reader_(reader),
    alpha_(alpha),
    beta_(beta) {

}


void LexiconNetworkBuilder::build(
    int max_epoch,
    const std::string &out_file,
    const std::vector<std::string> &positive,
    const std::vector<std::string> &negative) {
  preprocess(positive, negative);
  train(max_epoch, out_file);
}


void LexiconNetworkBuilder::extract_tokens(
    MeCab::Tagger *tagger,
    const std::string &text,
    const std::vector<std::string> &positive,
    const std::vector<std::string> &negative,
    std::vector<std::string> *tokens) {

  const MeCab::Node *node = tagger->parseToNode(text.c_str());
  int offset = 0;
  for (; node; node = node->next) {
    if (node->stat == MECAB_BOS_NODE) {
      continue;
    }
    if (node->stat == MECAB_EOS_NODE) {
      break;
    }
    std::vector<std::string> features;
    split(node->feature, ',', &features);
    std::string pos1 = features[0];
    std::string pos2 = features[1];
    std::string lemma = features[6];
    if (lemma == "*") {
      lemma = text.substr(offset, node->length);
    }

    if ((pos1 == "名詞" && pos2 != "非自立")
        || (pos1 == "動詞" && pos2 != "非自立")
        || pos1 == "形容詞"
        || pos1 == "形容動詞"
    ) {
      dictionary_.add(lemma);
      tokens->push_back(lemma);

      if (std::find(positive.begin(), positive.end(), lemma) != positive.end()) {
        positive_idx_.insert(dictionary_.get_index(lemma));
      }
      if (std::find(negative.begin(), negative.end(), lemma) != negative.end()) {
        negative_idx_.insert(dictionary_.get_index(lemma));
      }

    }

    offset += node->length;
  }
}


void LexiconNetworkBuilder::preprocess(
    const std::vector<std::string> &positive,
    const std::vector<std::string> &negative) {

  MeCab::Tagger *tagger = MeCab::createTagger("");

  std::vector<std::string> wordnet_words;
  reader_.get_words(&wordnet_words);

  // Build adjancent tokens for each token
  for (auto &wordnet_word : wordnet_words) {
    dictionary_.add(wordnet_word);

    std::vector<std::string> definitions;
    reader_.get_definitions(wordnet_word, &definitions);

    std::vector<std::string> tokens;
    extract_tokens(tagger, wordnet_word, positive, negative, &tokens);

    for (auto &def : definitions) {
      std::vector<std::string> def_tokens;
      extract_tokens(tagger, def, positive, negative, &def_tokens);

      for (auto &tok : tokens) {
        for (auto &def_token : def_tokens) {
          adj_words_[tok].insert(def_token);
        }
      }
    }
  }

  // Convert adjacent words into corresponding ids
  for (auto it = adj_words_.begin(); it != adj_words_.end(); ++it) {
    int idx_i = dictionary_.get_index(it->first);

    for (auto it_set = it->second.begin();
         it_set != it->second.end();
         it_set++) {

      int idx_j = dictionary_.get_index(*it_set);
      adj_ids_[idx_i].insert(idx_j);
      adj_ids_[idx_j].insert(idx_i);
    }
  }

  link_weight_calculator_ = std::unique_ptr<LinkWeightCalculator>(new
        InverseSquareRootMultiplyCalculator(adj_ids_));
}


void LexiconNetworkBuilder::train(
    int max_epoch,
    const std::string &out_file) {

  spin_ = new float[dictionary_.size()];
  for (int i = 0; i < dictionary_.size(); ++i) {
    spin_[i] = 0.;
  }
  for (auto it = positive_idx_.begin(); it != positive_idx_.end(); it++) {
    spin_[*it] = 1.;
  }
  for (auto it = negative_idx_.begin(); it != negative_idx_.end(); it++) {
    spin_[*it] = -1.;
  }

  std::vector<std::pair<float, std::string>> final_data(dictionary_.size());
  for (int epoch = 1; epoch <= max_epoch; ++epoch) {
    float diff_sum = 0.;
    std::vector<std::pair<float, std::string>> data(dictionary_.size());

    for (int i = 0; i < dictionary_.size(); ++i) {
      std::string wi = dictionary_.get_item(i);

      float sum = 0.;
      for (auto it = adj_ids_[i].begin(); it != adj_ids_[i].end(); it++) {
        std::string wj = dictionary_.get_item(*it);
        float w_ij = link_weight_calculator_->calculate(i, *it);
        sum += w_ij * spin_[*it];
//        std::cout << wi << " " << wj << " " << w_ij << std::endl;
      }

      float denom = 0.;
      float molec = 0.;

      if (positive_idx_.find(i) != positive_idx_.end()) {
        molec += 1. * exp(beta_ * 1. * sum);
        molec += -1. * exp(beta_ * -1. * sum - alpha_ * 4);

        denom += exp(beta_ * 1. * sum);
        denom += exp(beta_ * -1. * sum - alpha_ * 4);
      } else if (negative_idx_.find(i) != negative_idx_.end()) {
        molec += 1. * exp(beta_ * 1. * sum - alpha_ * 4);
        molec += -1. * exp(beta_ * -1. * sum);

        denom += exp(beta_ * 1. * sum - alpha_ * 4);
        denom += exp(beta_ * -1. * sum);
      } else {
        molec += 1. * exp(beta_ * 1. * sum);
        molec += -1. * exp(beta_ * -1. * sum);

        denom += exp(beta_ * 1. * sum);
        denom += exp(beta_ * -1. * sum);
      }
      float new_spin = molec / denom;
      float diff = abs(spin_[i] - new_spin);
      diff_sum += diff;

      spin_[i] = new_spin;
      data.push_back(std::make_pair(spin_[i], wi));
    }
    sort(data.begin(), data.end());
//    for (auto x : data) {
//      if (abs(x.first) > 0.01) {
//        std::cout << x.first << " " << x.second << std::endl;
//      }
//    }
    final_data = data;
    std::cerr << "epoch: " << epoch << "/" << max_epoch << " " << diff_sum / dictionary_.size() << std::endl;
  }

  std::ofstream ofs(out_file);
  for (auto x : final_data) {
    ofs << x.first << " " << x.second << std::endl;
  }
}

  
} // namespace spn
