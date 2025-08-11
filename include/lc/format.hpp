
#pragma once
#include <cstdint>
#include <cstddef>
#include <string>

namespace lc {

constexpr uint32_t MAGIC = 0x4C434D50; // 'LCMP'
constexpr uint8_t  VERSION = 0x01;

enum Codec : uint8_t { CODEC_RAW = 0, CODEC_LZ4 = 1 };

struct Header {
  uint32_t magic{MAGIC};
  uint8_t  version{VERSION};
  uint8_t  flags{0};        // codec bits etc.
  uint32_t blk_sz{512 * 1024};
  uint16_t rsvd{0};
};

struct BlockMeta {
  uint32_t raw_len{0};
  uint32_t comp_len{0};
  uint32_t dict_id{0};
  uint32_t checksum{0};
  uint64_t file_offset{0};
};

} // namespace lc
