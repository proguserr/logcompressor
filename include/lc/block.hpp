#pragma once
#include <cstdint>
#include <vector>
#include <span>
#include "lc/format.hpp"

namespace lc {

struct BlockMeta {
  uint32_t raw_len{};    // raw (uncompressed) bytes
  uint32_t comp_len{};   // compressed size (0 => payload is raw)
  uint32_t checksum{};   // xxhash32 of RAW data
  uint32_t dict_id{};    // unused (0)
};

struct EncodedBlock {
  BlockMeta meta;
  std::vector<uint8_t> payload; // compressed bytes or raw if comp_len==0
};

bool encode_block(std::span<const uint8_t> raw, uint32_t codec, EncodedBlock& out);
bool decode_block(const EncodedBlock& in, std::vector<uint8_t>& out);

} // namespace lc
