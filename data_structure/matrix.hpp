// data_structure/matrix.hpp

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
#include <cstring>
#include <vector>

#ifndef _CTL_DATA_STRUCTURE_
#define _CTL_DATA_STRUCTURE_

CTL_DEFAULT_NAMESPACE_BEGIN

// List of TODOs:
//  - Make all structures as compliant as possible with the stl

template<typename T_>
T_ **
alloc_matrix(size_t n, size_t m) {
    T_ **p_ = new T_ *[n];
    for (size_t i = 0; i < n; ++i) {
        p_[i] = new T_[m];
    }
    return p_;
}

template<typename T_>
void
free_matrix(T_ **p_, size_t n, size_t m) {
    for (size_t i = 0; i < n; ++i) {
        delete[] p_[i];
    }
    delete[] p_;
}

// TODO: Improve by substituting double pointer with a single pointer
// array. Then use the same structure as _2D_matrix making the access
// through the array of pointers. This could increase efficiency since
// the items are stored in a one dimension array.
template<typename _ContentT, typename _Alloc = std::allocator<_ContentT>>
class _2D_array {
public:

    typedef _ContentT content_type;
    typedef _ContentT *content_pointer;
    typedef size_t size_type;
    typedef size_t index_type;

    // member variables
    content_type **_mat;
    size_type _rows;
    size_type _cols;

    explicit _2D_array(size_type _r, size_type _c)
            : _rows{_r}, _cols{_c} {
        _mat = alloc_matrix<content_type>(_rows, _cols);
    }


    // move constructor
    _2D_array(_2D_array &&_m)
            : _mat{std::move(_m._mat)}, _rows{_m._rows}, _cols{_m._cols} {
        { _m._mat = nullptr; }
    }

    ~_2D_array() {
        if (_mat) {
            free_matrix<content_type>(_mat, _rows, _cols);
        }
        _mat = nullptr;
    }

    content_type
    operator()(index_type _i, index_type _j) const {
//        assert(_i < _rows && _j < _cols);
        return _mat[_i][_j];
    }

    content_type &
    operator()(index_type _i, index_type _j) {
//        assert(_i < _rows && _j < _cols);
        return _mat[_i][_j];
    }

    std::pair<size_type, size_type>
    shape() const {
        return std::make_pair(_rows, _cols);
    }

    void
    swap_rows(size_t i, size_t j) {
        std::swap(_mat[i], _mat[j]);
    }

};

using Int2DArray = _2D_array<int>;
using Long2DArray = _2D_array<long>;
using Double2DArray = _2D_array<double>;
using Float2DArray = _2D_array<float>;

template<typename _ContentT, typename _Alloc = std::allocator<_ContentT>>
class _2D_matrix {
public:

    typedef _ContentT content_type;
    typedef _ContentT *content_pointer;
    typedef size_t size_type;
    typedef size_t index_type;

    // member variables
    content_pointer _mat;
    size_type _rows;
    size_type _cols;


    explicit _2D_matrix(size_type _r, size_type _c)
            : _rows{_r}, _cols{_c} {
        _mat = new content_type[_rows * _cols];
    }

    // copy constructor
    _2D_matrix(const _2D_matrix &_m)
            : _rows{_m._rows}, _cols{_m.cols} {
        _mat = new content_type[_rows * _cols];
        std::memcpy(_mat, _m._mat, _rows * _cols * sizeof(content_type));
    }

    // move constructor
    _2D_matrix(_2D_matrix &&_m)
            : _mat{std::move(_m._mat)}, _rows{_m._rows}, _cols{_m._cols} { _m._mat = nullptr; }

    ~_2D_matrix() {
        if (_mat) {
            delete[] _mat;
        }
        _mat = nullptr;
    }

    content_type
    operator()(index_type _i, index_type _j) const {
        return _mat[_i * _cols + _j];
    }

    content_type &
    operator()(index_type _i, index_type _j) {
        return _mat[_i * _cols + _j];
    }

    // copy assignment
    _2D_matrix &operator=(const _2D_matrix &_m) {
        _rows = _m._rows;
        _cols = _m._cols;
        _mat = new content_type[_rows * _cols];
        std::memcpy(_mat, _m._mat, _rows * _cols * sizeof(content_type));
    }

    // move assignment
    _2D_matrix &operator=(_2D_matrix &&_m) {
        _rows = _m._rows;
        _cols = _m._cols;
        std::swap(_mat, _m._mat);
        _m.mat = nullptr;
    }

    std::pair<size_type, size_type>
    shape() const {
        return std::make_pair(_rows, _cols);
    }

};

using IntMatrix = _2D_matrix<int>;
using LongMatrix = _2D_matrix<long>;
using DoubleMatrix = _2D_matrix<double>;
using FloatMatrix = _2D_matrix<float>;


CTL_DEFAULT_NAMESPACE_END

#endif
