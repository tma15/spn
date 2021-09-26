#include "argparser.h"
#include "network_builder.h"
#include "string_util.h"
#include "wordnet_reader.h"


int main(int argc, char **argv) {
  spn::ArgumentParser argparser;
  argparser.add_argument("--alpha", "1e+3");
  argparser.add_argument("--beta", "1.");
  argparser.add_argument("--max_epoch", "100");
  argparser.add_argument("--out_file", "out");
  argparser.add_argument("--positive", "良い,素晴らしい", "Seed positive words (separate words by ,)");
  argparser.add_argument("--negative", "悪い,ひどい", "Seed negative words (separate words by ,)");
  argparser.add_argument("--wordnet", "", "Path to wnjpn.db");
  argparser.parse_args(argc, argv);

  std::string db_file = argparser.get<std::string>("wordnet");

  spn::WordNetReader reader(db_file);

  float alpha = argparser.get<float>("alpha");
  float beta = argparser.get<float>("beta");
  spn::LexiconNetworkBuilder builder(reader, alpha, beta);

  int max_epoch = argparser.get<int>("max_epoch");
  std::string out_file = argparser.get<std::string>("out_file");

  std::string pos = argparser.get<std::string>("positive");
  std::vector<std::string> positive;
  spn::split(pos, ',', &positive);

  std::string neg = argparser.get<std::string>("negative");
  std::vector<std::string> negative;
  spn::split(neg, ',', &negative);

  builder.build(max_epoch, out_file, positive, negative);
  
  return 0;
}
