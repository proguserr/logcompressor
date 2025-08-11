#include <lc/file_io.hpp>

#include "lc/pipeline.hpp"
#include "lc/format.hpp"
#include "lc/block.hpp"
#include "lc/file_io.hpp"
#include <thread>
#include <vector>
#include <string>
#include <iostream>

namespace lc {

int run_pipeline(const Options& opt) {
  if (opt.verify_only) {
    Header hdr{};
    std::vector<EncodedBlock> blocks;
    if (!lc::read_lcmp_blocks(opt.input, hdr, blocks)) {
      std::cerr << "verify: failed to read\n";
      return 2;
    }
    std::cerr << "verify: ok, blocks=" << blocks.size() << "\n";
    return 0;
  }

  if (!opt.decompress) {
    // Compress
    std::vector<uint8_t> data;
    if (!read_all(opt.input, data)) {
      std::cerr << "read input failed\n"; return 2;
    }
    Header hdr{};
    hdr.blk_sz = opt.block_size;
    hdr.flags = opt.codec;

    std::vector<EncodedBlock> blocks;
    for (size_t pos = 0; pos < data.size(); ) {
      size_t n = std::min<size_t>(hdr.blk_sz, data.size() - pos);
      EncodedBlock b;
      if (!encode_block(std::span<const uint8_t>(data.data()+pos, n), hdr.flags, b)) {
        std::cerr << "encode block failed\n"; return 3;
      }
      blocks.push_back(std::move(b));
      pos += n;
    }
    if (!lc::write_lcmp_file(opt.output, hdr, blocks)) {
      std::cerr << "write failed\n"; return 4;
    }
    return 0;
  } else {
    // Decompress
    Header hdr{};
    std::vector<EncodedBlock> blocks;
    if (!lc::read_lcmp_blocks(opt.input, hdr, blocks)) {
      std::cerr << "read lcmp failed\n"; return 2;
    }
    std::vector<uint8_t> out;
    out.reserve(hdr.blk_sz * blocks.size());
    for (auto& b : blocks) {
      std::vector<uint8_t> raw;
      if (!decode_block(b, raw)) { std::cerr << "decode failed\n"; return 3; }
      out.insert(out.end(), raw.begin(), raw.end());
    }
    if (!write_all(opt.output, out)) { std::cerr << "write out failed\n"; return 4; }
    return 0;
  }
}

} // namespace lc
