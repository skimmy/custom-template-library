// matrix.hpp

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

#include <utility>

#ifndef _CTL_DATA_STRUCTURE_
#define _CTL_DATA_STRUCTURE_

DEFAULT_NAMESPACE_BEGIN

template <typename _ContentT, typename _Alloc = std::allocator<_ContentT> >
struct _2D_matrix
{

  typedef _ContentT              content_type;
  typedef _ContentT*             content_pointer;
  typedef size_t                 size_type;
  typedef size_t                 index_type;
  
  // member variables
  content_pointer _mat;
  size_type _rows;
  size_type _cols;

  
  _2D_matrix(size_type _r, size_type _c)
    : _rows {_r}, _cols {_c}
  {
    _mat = new content_type[_rows*_cols];
  }

  ~_2D_matrix() {
    delete[] _mat;
  }

  content_type
  operator()(index_type _i, index_type _j) const {
    return _mat[_i*_cols + _j];
  }

  content_type&
  operator()(index_type _i, index_type _j) {
    return _mat[_i*_cols + _j];
  }

  std::pair<size_type,size_type>
  shape() const {
    return std::make_pair(_rows, _cols);
  }
  
};

using IntMatrix    = _2D_matrix<int>;
using LongMatrix   = _2D_matrix<long>;
using DoubleMatrix = _2D_matrix<double>;
using FloatMatrix  = _2D_matrix<float>;


DEFAULT_NAMESPACE_END

#endif
