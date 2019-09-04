// btl/generators.hpp

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

#include "../rand/random_sequence.hpp"

#include <iterator>

#ifndef _BTL_GENERATOR_
#define _BTL_GENERATOR_

BTL_DEFAULT_NAMESPACE_BEGIN

template <typename _ContT, typename _DistT, typename _RandDev>
_ContT
random_genome(size_t G, _DistT dist, _RandDev& rdev) {
  _ContT genome;
  std::string v {"ACGT"};
  auto gen = ctl::make_sequence_distribution<char, std::string, _DistT>(v, dist);
  gen(G, std::inserter(genome, genome.end()), rdev);
  return genome;
}

template <typename DistT_, typename RandDev_>
std::string
random_genome_string(size_t G, DistT_ dist, RandDev_& rdev) {
  return random_genome<std::string, DistT_, RandDev_>(G, dist, rdev);
}

BTL_DEFAULT_NAMESPACE_END

#endif
