#ifndef NANODB_UTILS_SLICE_H_
#define NANODB_UTILS_SLICE_H_

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include "hash.h"

namespace nanodb {
class Slice {
 private:
  const char* data_;

  size_t length_;

 public:
  Slice() {}

  Slice(const char* data) : data_(data), length_(strlen(data)) {}

  Slice(const char* data, const size_t length) : data_(data), length_(length) {}

  Slice(const std::string& data) : data_(data.data()), length_(data.size()) {}

  Slice(const Slice&) = default;

  Slice& operator=(const Slice&) = default;

  ~Slice() {}

  const char* data() const noexcept { return data_; }

  size_t length() const noexcept { return length_; }

  char operator[](const size_t pos) const {
    assert(pos < length_);
    return data_[pos];
  }

  // Return the slice of [l, r).
  Slice SubSlice(const size_t l, const size_t r) {
    assert(l <= r);
    assert(r <= length_);
    return Slice(data_ + l, r - l);
  }

  bool Empty() const noexcept { return length_ == 0; }

  void Clear() {
    data_ = "";
    length_ = 0;
  }

  void RemovePrefix(const size_t length) {
    assert(length <= length_);
    data_ += length;
    length_ -= length;
  }

  bool StartsWith(const Slice& x) const {
    return length_ >= x.length_ && memcmp(data_, x.data_, x.length_) == 0;
  }

  int Compare(const Slice& x) const {
    int result = memcmp(data_, x.data_, std::min(length_, x.length_));
    if (result == 0) {
      if (length_ < x.length_) {
        result = -1;
      } else {
        result = 1;
      }
    }
    return result;
  }

  std::string ToString() const { return std::string(data_, length_); }

  bool operator==(const Slice& x) const { return Compare(x) == 0; }

  bool operator!=(const Slice& x) const { return Compare(x) != 0; }
};

template <>
class hash::HashTrait<Slice> {
  uint64_t operator()(const Slice& x) const {
    return hash::MurmurHash3_32(reinterpret_cast<const uint8_t*>(x.data()),
                                x.length());
  }
};
}  // namespace nanodb

#endif