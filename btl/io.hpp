// btl/io.hpp

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

#include "../btl.h"

#include <utility>
#include <string>
#include <fstream>

#ifndef _BTL_IO_
#define _BTL_IO_

BTL_DEFAULT_NAMESPACE_BEGIN

// Reads a fasta file
// Notes:
//   _StreamT must provide eof() method as in istream
//   _StreamT must have a getline() defined in std::getline
template <typename _StreamT>
std::pair<std::string, std::string>
read_fasta(_StreamT& is)
{
  std::string header { "" };
  std::string genome { "" };

  if (is) {
    std::getline(is, header);
  }

  while(is) {
    std::string tmp { "" };
    std::getline(is, tmp);
    genome.append(tmp);
  }

  return std::make_pair(std::move(header), std::move(genome));
}

template<>
std::pair<std::string, std::string>
read_fasta(std::string& path) {
  std::ifstream is(path);
  auto pp = read_fasta(is);
  is.close();
  return pp;
}

template <typename _StreamT, typename _ContT, typename _HeadT>
void
write_fasta(_StreamT& os, const _ContT& g, const 
	    _HeadT& h, size_t max_line=50) {
  os << h << "\n";
  auto b = g.begin();
  auto e = g.end();
  for (size_t i = 1; b != e; ++b, ++i) {
    os << *b;
    if ( (i % max_line) == 0) { std::cout << "\n"; }
  }
}

using HeaderGenomePair = std::pair<std::string, std::string>;

BTL_DEFAULT_NAMESPACE_END

#endif
