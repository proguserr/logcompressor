
#include "lc/checksum.hpp"
#include <xxhash.h>

namespace lc {

uint32_t xxh32(std::span<const uint8_t> data, uint32_t seed) {
  return XXH32(data.data(), data.size(), seed);
}

} // namespace lc
