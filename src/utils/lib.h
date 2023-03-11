#ifndef NANODB_UTILS_LIB_H_
#define NANODB_UTILS_LIB_H_

#include <cassert>
#include <iostream>

namespace nanodb {
void Assert(const bool expression, const char* message) {
  if (!expression) {
    std::cerr << message << std::endl;
    assert(false);
  }
}

void Panic(const char* message) { Assert(false, message); }
}  // namespace nanodb

#endif