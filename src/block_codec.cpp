
#include "lc/block.hpp"
#include "lc/codec.hpp"
#include "lc/checksum.hpp"
#include <memory>

namespace lc {

static std::unique_ptr<CodecIf> codec_for(uint8_t c) {
  if (c == 1) return std::unique_ptr<CodecIf>(make_lz4());
  return std::unique_ptr<CodecIf>(make_raw());
}

bool encode_block(std::span<const uint8_t> raw, uint8_t codec, EncodedBlock& out) {
  out.meta.raw_len = static_cast<uint32_t>(raw.size());
  out.meta.dict_id = 0;
  out.meta.checksum = xxh32(raw, 0);
  auto c = codec_for(codec);
  std::vector<uint8_t> tmp;
  if (!c->compress(raw, tmp)) return false;
  if (tmp.size() >= raw.size()) {
    // store raw
    out.payload.assign(raw.begin(), raw.end());
    out.meta.comp_len = static_cast<uint32_t>(out.payload.size());
    return true;
  }
  out.payload.swap(tmp);
  out.meta.comp_len = static_cast<uint32_t>(out.payload.size());
  return true;
}

bool decode_block(const EncodedBlock& in, std::vector<uint8_t>& raw_out) {
  // caller must know codec; in this simplified version we try LZ4 first then raw
  std::vector<uint8_t> out;
  // Try to guess by length mismatches is unreliable; assume LZ4 then fallback
  auto lz = std::unique_ptr<CodecIf>(make_lz4());
  if (lz->decompress(in.payload, out, in.meta.raw_len)) {
    raw_out.swap(out);
    return true;
  }
  auto rawc = std::unique_ptr<CodecIf>(make_raw());
  if (rawc->decompress(in.payload, out, in.meta.raw_len)) {
    raw_out.swap(out);
    return true;
  }
  return false;
}

} // namespace lc
