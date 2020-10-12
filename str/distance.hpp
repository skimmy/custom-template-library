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

#include <vector>
#include <cstdlib>

#ifndef _CTL_STR_DISTANCE_
#define _CTL_STR_DISTANCE_

CTL_DEFAULT_NAMESPACE_BEGIN


/// \brief Computes Hamming distance between ranges
template<typename _IterT1, typename _IterT2, typename _IntT = size_t>
_IntT
hamming_distance(_IterT1 b1, _IterT1 e1, _IterT2 b2) {
  _IntT c {0};
  while(b1 != e1) {
    c += (*b1 != *b2) ? 1 : 0;
    ++b1;
    ++b2;
  }
  return c;
}


constexpr size_t iS_ = 0;
constexpr size_t iD_ = 1;
constexpr size_t iI_ = 2;

// Basic dynamic programming edit distance

/// \brief This class represents the standard Wagner and Fischer edit
/// distance dynamic programming algorithm.
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

  EditDistanceWF(size_t n, size_t m, CostVector costs)
    : dp_struct{n+1, m+1}, costs_vector {costs}
  {
    init();
  }

  EditDistanceWF(const EditDistanceWF& wf) 
    : dp_struct{wf._dp_struct}, costs_vector {wf.costs_vector}
    { }

  EditDistanceWF(EditDistanceWF&& wf) noexcept
    : dp_struct(std::move(wf.dp_struct)), costs_vector{std::move(wf.costs_vector)}
    { }

  EditDistanceWF& operator=(const EditDistanceWF& _wf)
  {
    dp_struct = _wf.dp_struct;
    costs_vector = _wf.costs_vector;
  }

  EditDistanceWF& operator=(EditDistanceWF&& _wf)
  {
    dp_struct = std::move(_wf.dp_struct);
    costs_vector = std::move(_wf.costs_vector);
  }    

  
  void
  init() {
    dp_struct(0, 0) = 0;
    for (size_t i = 1; i < dp_struct._rows; ++i) {
      dp_struct(i, 0)
	= dp_struct(i-1, 0) + costs_vector[iD_];
    }
    for (size_t j = 1; j < dp_struct._cols; ++j) {
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

  template <typename ListT> // e.g., std::list<std::pair<size_t,size_t>>
  ListT
  backtrack()
  {
    ListT l;
    size_t i = dp_struct.shape().first; 
    size_t j = dp_struct.shape().second;
  
    while(i>0 and j>0)
    {
      l.push_front(std::make_pair(i,j));
      CostType a = dp_struct(i-1,j-1);
      CostType b = dp_struct(i-1,j);
      CostType c = dp_struct(i,j-1);
      CostType d = dp_struct(i,j);
    

      // Match
      if ( (a == d) && ( a <= b) && (a <= c)) {
	i--; j--;
	continue;
      }    
      // Substitution
      if ( (a < b) && (a < c) ) {      
      i--; j--;
      continue;
      }

      // Deletion
      if ( (b < a) && (b < c) ) {      
	i--;
	continue;
      }

      // Insertion
      if ( (c < a) && (c < b) ) {
	j--;
	continue;
      }
        
      // Tie between all operations
      if ( (a == b) && (b == c) ) {
	if (i < j) {
	  j--;
	  continue;
	}
	if (i == j) {	
	  i--; j--;
	  continue;
	}
	if (i > j) {
	  i--;
	  continue;
	}
      }

      // Tie between sub and del
      if (a == b) {
	if (i > j) {
	  i--;
	} else {
	  i--; j--;
	}
	continue;	
      }
      // Tie between sub and ins
      if (a == c) {
	if (i < j) {
	  j--;
	} else {
	  i--; j--;
	}
	continue;
      }
      
      // Tie between del and ins
      if (b == c) {
	if (j <= i) {
	  i--;
	} else {
	  j--;
	}
	continue;

      }
    }
  
    // backtrack the left edge (if needed)
    while(i > 0) {
      l.push_front(std::make_pair(i,j));
      i--;
    }

    // bacltrack the top edge (if needed)
    while(j > 0) {
      l.push_front(std::make_pair(i,j));
      j--;
    }
      
    return l;
  }
  
  
  
}; // EditDistanceWF


template <typename T = size_t>
EditDistanceWF<T> make_wf_alg(size_t n, size_t m)
{
  return EditDistanceWF<T>(n, m, {1, 1, 1});
}

template<typename CostType = size_t>
class EditDistanceBandApproxLinSpace {
public:
    typedef std::vector <CostType> CostVector;

private:
    _2D_array<CostType> dp_struct;
    CostVector costs_vector;
    size_t bandwidth;
    size_t n;
    size_t m;
    const CostType Inf;

public:
    EditDistanceBandApproxLinSpace(size_t n_, size_t m_,
                                   size_t T, const CostVector &costV)
            : dp_struct{2, std::max(n_+1, m_+1)}, costs_vector{costV}, bandwidth{T},
              n{n_}, m{m_},
              Inf{(*std::max_element(costs_vector.begin(), costs_vector.end())) * 2 * (n_ + m_ + 1)} {}

    void init() {
        dp_struct(0, 0) = 0;
        for (size_t j = 1; j <= bandwidth; ++j) {
            dp_struct(0, j) = dp_struct(0, j - 1) + costs_vector[iI_];
        }
        dp_struct(1, 0) = costs_vector[iD_];
    }

    template<typename IterT>
    CostType
    operator()(IterT b1, IterT e1, IterT b2, IterT e2) {
        size_t n = std::distance(b1, e1);
        size_t m = std::distance(b2, e2);

        // initialization is done here rather than in the constructor
        // because needed at each calculation (i.e., vectors will contain
        // values from older calculations if any)
        init();

        for (size_t i = 1; i <= n; ++i) {
            // initialization of 'border' and 'diagonals'
            dp_struct(1, 0) = (i <= bandwidth) ?
                              dp_struct(0, 0) + costs_vector[iD_] : Inf;
            if (i <= m - bandwidth) {
                dp_struct(0, i + bandwidth) = Inf;
            }
            if (i >= bandwidth + 1) {
                dp_struct(1, i - (bandwidth + 1)) = Inf;
            }

            // casts are necessary to cope with negative
            size_t j_min = std::max<int>(1, static_cast<int>(i - bandwidth));
            size_t j_max = std::min<int>(m, i + bandwidth);
            for (size_t j = j_min; j <= j_max; ++j) {
                //CostType delta{(s1[i - 1] == s2[j - 1]) ? 0 : costs_vector[iS_]};
                CostType delta = (*(b1 + i - 1) == *(b2 + j - 1)) ? 0 : costs_vector[iS_];
                dp_struct(1, j)
                        = std::min<CostType>(delta + dp_struct(0, j - 1),
                                             std::min(dp_struct(0, j) + costs_vector[iD_],
                                                      dp_struct(1, j - 1) + costs_vector[iI_]));
            }
            // swap the two vectors
           dp_struct.swap_rows(0, 1);
        }
        return dp_struct(0, m);
    }

//    template<typename IndexedType>
//    CostType operator()(const IndexedType &s1, const IndexedType &s2,
//                       size_t band) {
//        size_t old_band = bandwidth;
//        change_bandwidth(band);
//        CostType result = calculate(s1, s2);
//        change_bandwidth(old_band);
//        return result;
//    }

    /**
       \brief Changes the bandwidth \e without reallocating the internal
       structure. It the new bandwidth cannot be accomodated the old
       value is kept and \c false is returned.

       \param new_band The new bandwidth
       \return \c true if value is changed \c false otherwise

       \note This is supplied for performance reasons, that is it can be
       used to perform the algorithm for any fitting value of the
       bandwidth without the necessity of more instances or without
       resizing.
     */
    bool
    change_bandwidth(size_t new_band) {
        if (new_band > std::min(dp_struct._rows, dp_struct._cols)) {
            return false;
        }
        bandwidth = new_band;
        return true;
    }


    void print_dp_matrix() const {
        dp_struct.print_matrix();
    }

}; // EditDistanceBandApproxLinSpace


template<typename CostT = size_t>
EditDistanceBandApproxLinSpace<CostT> make_band_linear_alg(size_t n, size_t m, size_t T) {
    return EditDistanceBandApproxLinSpace<CostT>(n, m, T, {1, 1, 1});
}


CTL_DEFAULT_NAMESPACE_END

#endif
