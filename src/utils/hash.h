#ifndef NANODB_UTILS_HASH_H_
#define NANODB_UTILS_HASH_H_

#include <cstdint>

namespace nanodb {
namespace hash {

const uint32_t c1 = 0xcc9e2d51;
const uint32_t c2 = 0x1b873593;
const uint32_t c3 = 0xe6546b64;
const uint32_t c4 = 0x85ebca6b;
const uint32_t c5 = 0xc2b2ae35;

// Murmur3 hash algorithm
uint32_t MurmurHash3_32(const uint8_t* source, const size_t bytes,
                        uint32_t seed = 0x0d000721) {
  const uint32_t* source_32 = reinterpret_cast<const uint32_t*>(source);
  for (size_t i = bytes >> 2; i >= 1; --i) {
    uint32_t v = *source_32++;
    v *= c1;
    v = (v << 15) | (v >> 17);
    v *= c2;
    seed ^= v;
    seed = (seed << 13) | (seed >> 19);
    seed = seed * 5 + c3;
  }
  source = reinterpret_cast<const uint8_t*>(source_32);
  switch (uint32_t v = 0; bytes & 3) {
    case 3:
      v ^= source[2] << 16;
    case 2:
      v ^= source[1] << 8;
    case 1:
      v ^= source[0];
      v *= c1;
      v = (v << 15) | (v >> 17);
      v *= c2;
      seed ^= v;
      ;
  }
  seed ^= bytes;
  seed ^= seed >> 16;
  seed *= c4;
  seed ^= seed >> 13;
  seed *= c5;
  seed ^= seed >> 16;
  return seed;
}

uint32_t MurmurHash3_32(const std::string& str,
                        const uint32_t seed = 0x0d000721) {
  return MurmurHash3_32(reinterpret_cast<const uint8_t*>(str.c_str()),
                        str.length(), seed);
}

template <typename KeyType>
class HashTrait {
 public:
  uint64_t operator()(const KeyType& x) const {
    return static_cast<uint64_t>(x);
  }
};

template <typename ValueType>
class EqualTrait {
 public:
  bool operator()(const ValueType& a, const ValueType& b) const {
    return a == b;
  }
};

// MurmurHash3 algorithm for std::string
template <>
class HashTrait<std::string> {
  uint64_t operator()(const std::string& x) const {
    return MurmurHash3_32(reinterpret_cast<const uint8_t*>(x.c_str()),
                          x.length());
  }
};

}  // namespace hash
}  // namespace nanodb

#endif