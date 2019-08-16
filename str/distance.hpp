// str/distance.hpp

// Copyright 2019 Michele Schimd

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "../ctl.h"
#include "../data_structure/matrix.hpp"

#include <cstdlib>

#ifndef _CTL_STR_DISTANCE_
#define _CTL_STR_DISTANCE_

CTL_DEFAULT_NAMESPACE_BEGIN

constexpr size_t iS_ = 0;
constexpr size_t iD_ = 1;
constexpr size_t iI_ = 2;

// Basic dynamic programming edit distance
/**
 * \brief This class represents the standard Wagner and Fischer edit
 * distance dynamic programming algorithm.
 */
template<typename CostType = size_t>
class EditDistanceWF {
public:
    typedef std::vector<CostType> CostVector;

private:
  _2D_matrix<CostType> dp_struct;
  // costs are in vector [W_S, W_D, W_I]
  // (i.e., [0] -> Sub, [1] -> Del, [2] -> Ins).
  // We should consider a better design for costs possible alternatives are
  // - a custom struct with fields for the csos
  // - a caller that is invoked when costs are needed (should be constexpr)
  CostVector costs_vector;
  
public:  

  EditDistanceWF(size_t n, size_t m, const CostVector& costs)
    : dp_struct{n+1, m+1}, costs_vector {costs}
  {
    init();
  }

  
  void
  init() {
    dp_struct(0, 0) = 0;
    for (size_t i = 1; i <= dp_struct._rows; ++i) {
      dp_struct(i, 0)
	= dp_struct(i-1, 0) + costs_vector[iD_];
    }
    for (size_t j = 1; j <= dp_struct._cols; ++j) {
      dp_struct(0, j)
	= dp_struct(0, j-1) + costs_vector[iI_];
    }
  }

  // Notes: IterT must be random iterator
  template <typename IterT>
  CostType
  operator()(IterT b1, IterT e1, IterT b2, IterT e2)
  {
    size_t n = std::distance(b1, e1);
    size_t m = std::distance(b2, e2);
    for (size_t i = 1; i <= n; ++i) {
      for (size_t j = 1; j <= m; ++j) {
	CostType delta = (*(b1+i-1) == *(b2+j-1)) ? 0 : costs_vector[iS_];
	CostType A_ = dp_struct(i-1, j-1) + delta; 
	CostType B_ = dp_struct(i-1, j)   + costs_vector[iD_];
	CostType C_ = dp_struct(i, j-1)   + costs_vector[iI_];
	dp_struct(i, j) = std::min(A_,std::min(B_, C_ ));
      }
    }
    return dp_struct(n, m);
  }

  // Notes: IndexedType must have size() method and [] indexing
  template <typename IndexedType>
  CostType
  operator()(const IndexedType& s1, const IndexedType& s2)
  {
    size_t n = s1.size();
    size_t m = s2.size();
    for (size_t i = 1; i <= n; ++i) {
      for(size_t j = 1; j <= m; ++j) {
	CostType delta = ( s1[i-1] == s2[j-1] ) ? 0 : costs_vector[iS_];
	CostType A_ = dp_struct(i-1, j-1) + delta; 
	CostType B_ = dp_struct(i-1, j)   + costs_vector[iD_];
	CostType C_ = dp_struct(i, j-1)   + costs_vector[iI_];
	dp_struct(i, j) = std::min(A_,std::min(B_, C_ ));
	
      }
    }
    return dp_struct(n, m);
  }

  // EditDistanceInfo
  // backtrack() {
  //   EditDistanceInfo info;
  //   // following uses an 'old' functions that accepts size_t** as DP matrix
  //   // it should be changed to accept a dp_struct
  //   lbio::sim::edit::closest_to_diagonal_backtrack(dp_struct.n, dp_struct.m,
  // 						   dp_struct.dp_matrix, info);
  //   return info;
  // }

  // void
  // print_dp_matrix() {
  //   dp_struct.print_matrix();
  // }
}; // EditDistanceWF


template <typename T = size_t>
EditDistanceWF<T> make_wf_alg(size_t n, size_t m)
{
  std::vector<T> costs = {1, 1, 1};
  return EditDistanceWF<T>(n, m, costs);
}


CTL_DEFAULT_NAMESPACE_END

#endif