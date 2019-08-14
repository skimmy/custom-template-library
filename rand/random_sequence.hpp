// random_sequence.hpp

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

#include <cstdlib>
#include <random>

#ifndef _CTL_RANDOM_SEQUENCE_
#define _CTL_RANDOM_SEQUENCE_

DEFAULT_NAMESPACE_BEGIN

template<typename _SampleT>
class sequence_distribution
{
public:
  typedef _SampleT sample_type;

  // constructors
  template<typename _IterT>
  sequence_distribution(_IterT begin, _IterT end)
    : omega(begin,end)
  {
    std::vector<int> v(omega.size(), 1);
    dd = std::discrete_distribution<int>(v.begin(), v.end());
  }

  // TODO Constructor with distribution
    
  // generate operations
  template<typename _RandD>
  sample_type
  operator()(_RandD& dev)
  { return omega[dd(dev)]; }
  
  template<typename _InsIt, typename _RandD>
  void
  operator()(size_t n, _InsIt it, _RandD& dev)
  {
    while(n>0) {
      it = omega[dd(dev)];
      n--;
    }
  }

  
// TODO

// Convenience function to produce sequence directly
// _ContT sequence(size_t n)
  

private:
  std::vector<sample_type> omega;
  std::discrete_distribution<int> dd;

  
};

// factory functions
template<typename _SampleT, typename _IterT>
sequence_distribution<_SampleT>
make_uniform_sequence_distribution(_IterT begin, _IterT end) {
  return sequence_distribution<_SampleT>(begin, end);
}

template<typename _SampleT, typename _ContT>
sequence_distribution<_SampleT>
make_uniform_sequence_distribution(_ContT cont) {
  return sequence_distribution<_SampleT>(cont.begin(), cont.end());
}


DEFAULT_NAMESPACE_END

#endif
