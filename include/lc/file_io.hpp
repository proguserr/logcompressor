#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "lc/format.hpp"
#include "lc/block.hpp"

namespace lc {
// raw file IO
bool read_all(const std::string& path, std::vector<uint8_t>& out);
bool write_all(const std::string& path, const std::vector<uint8_t>& data);

// LCMP container IO (simple framing, little-endian):
// magic "LCMP", version=1, blk_sz (u32), flags (u32), count (u32)
// for each block: raw_len (u32), comp_len (u32), checksum (u32), comp_bytes...
bool read_lcmp_blocks(const std::string& path, Header& hdr, std::vector<EncodedBlock>& blocks);
bool write_lcmp_file(const std::string& path, const Header& hdr, const std::vector<EncodedBlock>& blocks);
} // namespace lc
