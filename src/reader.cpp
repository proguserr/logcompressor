#include "lc/file_io.hpp"
#include "lc/block.hpp"
#include "lc/format.hpp"
#include <vector>
#include <cstdint>
#include <iostream>

namespace lc {

bool read_and_write_raw(const std::string& in, const std::string& out){
  Header hdr{};
  std::vector<EncodedBlock> blocks;
  if (!read_lcmp_blocks(in, hdr, blocks)) {
    std::cerr << "read lcmp failed\n"; return false;
  }
  std::vector<uint8_t> outbuf;
  outbuf.reserve(static_cast<std::size_t>(hdr.blk_sz) * blocks.size());
  for (auto& b : blocks) {
    std::vector<uint8_t> raw;
    if (!decode_block(b, raw)) { std::cerr << "decode failed\n"; return false; }
    outbuf.insert(outbuf.end(), raw.begin(), raw.end());
  }
  if (!write_all(out, outbuf)) { std::cerr << "write out failed\n"; return false; }
  return true;
}

} // namespace lc
