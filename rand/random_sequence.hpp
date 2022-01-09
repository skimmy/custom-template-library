// rand/random_sequence.hpp

// Copyright 2022 Michele Schimd

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

CTL_DEFAULT_NAMESPACE_BEGIN

template<typename _SampleT>
class sequence_distribution
{
public:
  typedef _SampleT sample_type;

  // constructors
  template<typename _IterT>
  sequence_distribution(const _IterT& begin, const _IterT& end)
    : omega(begin,end)
  {
    std::vector<int> v(omega.size(), 1);
    dd = std::discrete_distribution<int>(v.begin(), v.end());
  }

  // TODO Constructor with distribution
  template<typename _IterT, typename _DistT>
  sequence_distribution(const _IterT& begin, const _IterT& end,
			const _DistT& dbegin, const _DistT& dend)
    : omega(begin, end), dd(dbegin, dend)
  {
  }
    
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
      *it = omega[dd(dev)];
      ++it;
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
make_uniform_sequence_distribution(const _IterT& begin, const _IterT& end) {
  return sequence_distribution<_SampleT>(begin, end);
}

template<typename _SampleT, typename _ContT>
sequence_distribution<_SampleT>
make_uniform_sequence_distribution(const _ContT& cont) {
  return sequence_distribution<_SampleT>(cont.begin(), cont.end());
}

template<typename _SampleT, typename _IterT, typename _DistT>
sequence_distribution<_SampleT>
make_sequence_distribution(const _IterT& begin, const _IterT& end,
			   const _DistT& dbegin, const _DistT& dend) {
  return sequence_distribution<_SampleT>(begin, end, dbegin, dend);
}

template<typename _SampleT, typename _ContT, typename _DistT>
sequence_distribution<_SampleT>
make_sequence_distribution(const _ContT& cont, _DistT dist) {
  return sequence_distribution<_SampleT>(cont.begin(), cont.end(),
					 dist.begin(), dist.end());
}

template<typename _SampleT>
class integer_distribution
{
public:
  typedef _SampleT sample_type;

  // constructors
  integer_distribution(sample_type a, sample_type b)
    : low(a), high(b), dd(a, b)
  {
  }

    
  // generate operations
  template<typename _RandD>
  sample_type
  operator()(_RandD& dev)
  { return dd(dev); }
  
  template<typename _InsIt, typename _RandD>
  void
  operator()(size_t n, _InsIt it, _RandD& dev)
  {
    while(n>0) {
      *it = dd(dev);
      ++it;
      n--;
    }
  }


private:
  sample_type low;
  sample_type high;
  std::uniform_int_distribution<sample_type> dd;
  
};

template<typename _SampleT>
integer_distribution<_SampleT>
make_uniform_integer_distribution(_SampleT low, _SampleT high) {
    return integer_distribution<_SampleT>(low, high);
}


CTL_DEFAULT_NAMESPACE_END

#endif
