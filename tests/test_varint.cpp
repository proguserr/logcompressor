
#include "lc/varint.hpp"
#include <cassert>
#include <vector>
#include <iostream>

int main() {
  std::vector<uint64_t> vals = {0,1,127,128,300,1234567890123ULL};
  for (auto v : vals) {
    auto enc = lc::varint_encode_u64(v);
    size_t used = 0;
    auto dec = lc::varint_decode_u64(std::span<const uint8_t>(enc.data(), enc.size()), used);
    assert(dec == v);
  }
  std::cout << "varint ok\n";
  return 0;
}
