// stream_map.hpp

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

/// \file stream_map.hpp \brief This file contains the functions to
/// convert streams to map of key value pairs. It also contains
/// factory function allowing working directly with file

#include "../ctl.h"

#include <map>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#ifndef _CTL_STREAM_MAP_
#define _CTL_STREAM_MAP_


CTL_DEFAULT_NAMESPACE_BEGIN

/// \fn _T from_string(std::string str)
/// \brief converts a string to type _T using operator >>.
template <typename _T>
_T from_string(std::string str) {
  std::stringstream _stream{ str };
  _T t;
  _stream >> t;
  return t;
}

template<>
inline std::string
from_string<std::string>(std::string str) {
  // hopefully compiler will optimize
  return  str;
}

// Tokenization: assumes 'Key [sep] Val' other occurrences of [sep]
// are considered part of Val
template <typename _KeyT, typename _ValT>
std::map<_KeyT,_ValT>
stream_to_map(std::istream& _is, char delimiter='=', char comment='#') {
  std::map<_KeyT, _ValT> _map;
  while(!_is.eof()) {
    std::string line {};
    std::getline(_is, line);
    // strip comment setting end to the first occurence of the comment char
    auto end = std::find(line.begin(), line.end(), comment);
    auto _sep_iter = std::find(line.begin(), end, delimiter);
    if (_sep_iter != end) {
      _KeyT _key = from_string<_KeyT>(std::string {line.begin(), _sep_iter});
      _ValT _val = from_string<_ValT>(std::string { ++_sep_iter, end});
      _map[_key] = _val;
    }
  }
  return _map;
}

using StringStringMap = typename std::map<std::string, std::string>;

/// \brief Creates a map from string to string starting from the file
/// at the given path.
template <typename _PathT>
std::map<std::string, std::string>
make_string_map_from_file(const _PathT& path, char delimiter='=', char comment='#') {
  std::ifstream ifs {path};
  return stream_to_map<std::string, std::string>(ifs, delimiter, comment);
}

/// \brief Creates a map from string to string starting from the given stream
template <typename _StreamT>
std::map<std::string, std::string>
make_string_map_from_stream(_StreamT& ifs, char delimiter='=', char comment='#') {
  return stream_to_map<std::string, std::string>(ifs, delimiter, comment);
}


CTL_DEFAULT_NAMESPACE_END


#endif
