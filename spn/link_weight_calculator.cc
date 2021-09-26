#include <cmath>
#include <iostream>

#include "link_weight_calculator.h"


namespace spn {


InverseSquareRootMultiplyCalculator::InverseSquareRootMultiplyCalculator(
    const std::unordered_map<int, std::set<int>> &adj_ids)
  : adj_ids_(adj_ids) {

}


float InverseSquareRootMultiplyCalculator::calculate(int i, int j) {
  if (adj_ids_[i].size() == 0. || adj_ids_[j].size() == 0.) {
    return 0.;
  }
  return 1 / sqrt(adj_ids_[i].size() * adj_ids_[j].size());
}

  
} // namespace spn
