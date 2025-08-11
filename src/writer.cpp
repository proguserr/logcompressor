#include "lc/file_io.hpp"
#include "lc/block.hpp"
#include "lc/format.hpp"
#include <span>
#include <algorithm>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace lc {

bool build_and_write(const std::string& in, const std::string& out, const Header& hdr){
  std::vector<uint8_t> data;
  if (!read_all(in, data)) {
    std::cerr << "read input failed\n"; return false;
  }
  std::vector<EncodedBlock> blocks;
  blocks.reserve((data.size()+hdr.blk_sz-1)/hdr.blk_sz);
  for (std::size_t pos = 0; pos < data.size(); ) {
    std::size_t n = std::min<std::size_t>(hdr.blk_sz, data.size() - pos);
    EncodedBlock b;
    if (!encode_block(std::span<const uint8_t>(data.data()+pos, n), hdr.flags, b)) {
      std::cerr << "encode block failed\n"; return false;
    }
    blocks.push_back(std::move(b));
    pos += n;
  }
  if (!write_lcmp_file(out, hdr, blocks)) {
    std::cerr << "write failed\n"; return false;
  }
  return true;
}

} // namespace lc
