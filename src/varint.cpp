
#include "lc/varint.hpp"

namespace lc {

std::vector<uint8_t> varint_encode_u64(uint64_t v) {
  std::vector<uint8_t> out;
  while (v >= 0x80) {
    out.push_back(static_cast<uint8_t>(v | 0x80));
    v >>= 7;
  }
  out.push_back(static_cast<uint8_t>(v));
  return out;
}

uint64_t varint_decode_u64(std::span<const uint8_t> buf, size_t& consumed) {
  uint64_t result = 0;
  int shift = 0;
  consumed = 0;
  for (uint8_t b : buf) {
    result |= static_cast<uint64_t>(b & 0x7F) << shift;
    consumed++;
    if ((b & 0x80) == 0) break;
    shift += 7;
  }
  return result;
}

} // namespace lc
