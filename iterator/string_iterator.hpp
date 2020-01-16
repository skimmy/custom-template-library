// iterator/string_iterator.hpp

// Copyright 2020 Michele Schimd

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

#include <cstdint>
#include <string>
#include <vector>


#ifndef _CTL_STRING_ITERATOR_H_
#define _CTL_STRING_ITERATOR_H_

CTL_DEFAULT_NAMESPACE_BEGIN

class SigmaNIterator {
private:
  std::vector<uint8_t> x;
  std::string alphabet;
  bool end;
  
public:
  SigmaNIterator(size_t n, std::string Sigma) :
    x(n, 0), alphabet {Sigma}, end {false}
    { }

  SigmaNIterator() : end {true} { }

  SigmaNIterator& operator++() {
    if (!end) {
      int j = x.size()-1;
      while(j>=0 and x[j] == (alphabet.size()-1)) {
	x[j] = 0;
	j--;
      }
      if (j>=0) {
	x[j]++;
      }
      else {
	end = true;	
      }
    }
    return *this;
  }

  std::string operator*() {
    std::string s {""};
    for (auto it = x.begin(); it != x.end(); ++it) {
      s += alphabet[*it];
    }
    return s;
  }

  bool operator==(const SigmaNIterator& other) {
    return (end == other.end);
  }

  bool operator!=(const SigmaNIterator& other) {
    return !(*this == other);
  }
  
};

CTL_DEFAULT_NAMESPACE_END

#endif
