// iterator/circular_iterator.hpp

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

#include <iterator>

#ifndef _CTL_CIRCULAR_ITERATOR_H_
#define _CTL_CIRCULAR_ITERATOR_H_

CTL_DEFAULT_NAMESPACE_BEGIN

template <typename _IterT, typename IntType>
class circular_iterator
  : public std::iterator<std::random_access_iterator_tag,
			 typename std::iterator_traits<_IterT>::value_type,
			 typename std::iterator_traits<_IterT>::difference_type,
			 typename std::iterator_traits<_IterT>::pointer,
			 typename std::iterator_traits<_IterT>::reference>
{
private:
  _IterT  _b;
  IntType _n;
  IntType _curr;
public:
  circular_iterator(_IterT b, IntType n)
    : _b { b }, _n { n }, _curr { 0 } {}

  circular_iterator operator+(IntType k)
  {
    circular_iterator tmp(_b, _n);
    tmp._curr += (_curr + k) % _n;
    return tmp;
  }

  circular_iterator operator++()
    { _curr = (_curr + 1) % _n; return *this; }

  typename std::iterator_traits<_IterT>::value_type& operator*()
    { return *(_b + _curr); }
};

template <typename _IterT, typename IntType>
circular_iterator<_IterT, IntType>
make_circular_iterator(_IterT it, IntType n)
{
  return circular_iterator<_IterT, IntType>(it, n);
}

// TODO: Implement make_* for container

CTL_DEFAULT_NAMESPACE_END

#endif
