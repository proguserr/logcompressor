
#include "lc/format.hpp"
#include "lc/block.hpp"
#include <cassert>
#include <vector>
#include <string>

int main() {
  std::string s(1024, 'A');
  for (int i = 0; i < (int)s.size(); ++i) s[i] = 'A' + (i % 26);
  lc::EncodedBlock eb;
  bool ok = lc::encode_block(std::span<const uint8_t>((const uint8_t*)s.data(), s.size()), 1, eb);
  assert(ok);
  std::vector<uint8_t> out;
  ok = lc::decode_block(eb, out);
  assert(ok);
  assert(out.size() == s.size());
  return 0;
}
