
#pragma once
#include <vector>
#include <span>
#include <cstdint>

namespace lc {

struct CodecIf {
  virtual ~CodecIf() = default;
  virtual bool compress(std::span<const uint8_t> in, std::vector<uint8_t>& out) = 0;
  virtual bool decompress(std::span<const uint8_t> in, std::vector<uint8_t>& out, size_t raw_len_hint) = 0;
};

CodecIf* make_lz4();
CodecIf* make_raw();

} // namespace lc
