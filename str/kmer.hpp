// str/kmer.hpp

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

#ifndef _CTL_STR_KMER_
#define _CTL_STR_KMER_

CTL_DEFAULT_NAMESPACE_BEGIN

template <typename SeqT_ = std::string, typename MapT_>
void
kmer_statistics(const SeqT_& seq, size_t k, MapT_& map_, bool overlap = true)
{
  auto begin = seq.begin();
  auto end   = seq.end();
  auto iter  = seq.begin();
  size_t n = std::distance(begin, end);
  if (k < 1 || k > n) {
    return;
  }
  std::advance(iter, k);
  while(iter != end) {
    auto key = SeqT_(begin , iter);
    map_[key]++;
    ++begin;
    ++iter;
  }
}

CTL_DEFAULT_NAMESPACE_END

#endif
