
#include "lc/codec.hpp"
#include <lz4.h>
#include <memory>

namespace lc {

struct LZ4Codec : CodecIf {
  bool compress(std::span<const uint8_t> in, std::vector<uint8_t>& out) override {
    int max_dst = LZ4_compressBound(static_cast<int>(in.size()));
    out.resize(max_dst);
    int n = LZ4_compress_default(
      reinterpret_cast<const char*>(in.data()),
      reinterpret_cast<char*>(out.data()),
      static_cast<int>(in.size()),
      max_dst);
    if (n <= 0) return false;
    out.resize(n);
    return true;
  }
  bool decompress(std::span<const uint8_t> in, std::vector<uint8_t>& out, size_t raw_len_hint) override {
    out.resize(raw_len_hint);
    int n = LZ4_decompress_safe(
      reinterpret_cast<const char*>(in.data()),
      reinterpret_cast<char*>(out.data()),
      static_cast<int>(in.size()),
      static_cast<int>(out.size()));
    return n >= 0;
  }
};

struct RawCodec : CodecIf {
  bool compress(std::span<const uint8_t> in, std::vector<uint8_t>& out) override {
    out.assign(in.begin(), in.end());
    return true;
  }
  bool decompress(std::span<const uint8_t> in, std::vector<uint8_t>& out, size_t) override {
    out.assign(in.begin(), in.end());
    return true;
  }
};

CodecIf* make_lz4() { return new LZ4Codec(); }
CodecIf* make_raw() { return new RawCodec(); }

} // namespace lc
