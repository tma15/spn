#pragma once
#include <set>
#include <string>
#include <unordered_map>
#include "dictionary.h"


namespace spn {


class LinkWeightCalculator {
  public:
    virtual ~LinkWeightCalculator() {}
    virtual float calculate(int i, int j)=0;
};


class InverseSquareRootMultiplyCalculator : public LinkWeightCalculator {
  public:
    InverseSquareRootMultiplyCalculator(
        const std::unordered_map<int, std::set<int>> &adj_ids);


    float calculate(int i, int j);

  private:
    std::unordered_map<int, std::set<int>> adj_ids_;
};

  
} // namespace spn
