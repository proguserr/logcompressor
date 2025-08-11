
#pragma once
#include <cstdint>
#include <vector>
#include <span>

namespace lc {

inline uint64_t zigzag_encode(int64_t v) {
  return (v << 1) ^ (v >> 63);
}
inline int64_t zigzag_decode(uint64_t v) {
  return (v >> 1) ^ -static_cast<int64_t>(v & 1);
}

std::vector<uint8_t> varint_encode_u64(uint64_t v);
uint64_t varint_decode_u64(std::span<const uint8_t> buf, size_t& consumed);

} // namespace lc
