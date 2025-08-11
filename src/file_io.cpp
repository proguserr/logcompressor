#include "lc/file_io.hpp"
#include <fstream>
#include <cstring>

namespace lc {

static inline void put_u32(std::ofstream& o, uint32_t v){
  unsigned char b[4] = { (unsigned char)(v      &0xFF),
                         (unsigned char)((v>>8) &0xFF),
                         (unsigned char)((v>>16)&0xFF),
                         (unsigned char)((v>>24)&0xFF) };
  o.write((const char*)b, 4);
}
static inline bool get_u32(std::ifstream& i, uint32_t& v){
  unsigned char b[4];
  if (!i.read((char*)b, 4)) return false;
  v = (uint32_t)b[0] | ((uint32_t)b[1]<<8) | ((uint32_t)b[2]<<16) | ((uint32_t)b[3]<<24);
  return true;
}

bool read_all(const std::string& path, std::vector<uint8_t>& out){
  std::ifstream f(path, std::ios::binary);
  if(!f) return false;
  f.seekg(0, std::ios::end);
  std::streamsize sz = f.tellg();
  if (sz < 0) sz = 0;
  f.seekg(0, std::ios::beg);
  out.resize((size_t)sz);
  if(sz>0) f.read((char*)out.data(), sz);
  return (bool)f || sz==0;
}

bool write_all(const std::string& path, const std::vector<uint8_t>& data){
  std::ofstream f(path, std::ios::binary|std::ios::trunc);
  if(!f) return false;
  if(!data.empty()) f.write((const char*)data.data(), (std::streamsize)data.size());
  return (bool)f;
}

bool write_lcmp_file(const std::string& path, const Header& hdr, const std::vector<EncodedBlock>& blocks){
  std::ofstream o(path, std::ios::binary|std::ios::trunc);
  if(!o) return false;

  // header
  const char magic[4] = {'L','C','M','P'};
  o.write(magic, 4);
  put_u32(o, 1);                 // version
  put_u32(o, (uint32_t)hdr.blk_sz);
  put_u32(o, (uint32_t)hdr.flags);
  put_u32(o, (uint32_t)blocks.size());

  // blocks
  for(const auto& b : blocks){
    uint32_t raw_len = (uint32_t)b.raw_len;
    uint32_t comp_len = (uint32_t)b.comp.size();
    uint32_t cks = (uint32_t)b.checksum;
    put_u32(o, raw_len);
    put_u32(o, comp_len);
    put_u32(o, cks);
    if (comp_len) o.write((const char*)b.comp.data(), comp_len);
  }
  return (bool)o;
}

bool read_lcmp_blocks(const std::string& path, Header& hdr, std::vector<EncodedBlock>& blocks){
  std::ifstream i(path, std::ios::binary);
  if(!i) return false;

  char mg[4];
  if (!i.read(mg, 4)) return false;
  if (std::memcmp(mg, "LCMP", 4)!=0) return false;

  uint32_t ver=0, blk_sz=0, flags=0, cnt=0;
  if (!get_u32(i, ver)) return false;
  if (ver != 1) return false;
  if (!get_u32(i, blk_sz)) return false;
  if (!get_u32(i, flags)) return false;
  if (!get_u32(i, cnt)) return false;

  hdr.blk_sz = blk_sz;
  hdr.flags  = flags;

  blocks.clear();
  blocks.reserve(cnt);
  for (uint32_t k=0;k<cnt;k++){
    uint32_t raw_len=0, comp_len=0, cks=0;
    if (!get_u32(i, raw_len)) return false;
    if (!get_u32(i, comp_len)) return false;
    if (!get_u32(i, cks)) return false;

    EncodedBlock b;
    b.raw_len  = raw_len;
    b.checksum = cks;
    b.comp.resize(comp_len);
    if (comp_len){
      if (!i.read((char*)b.comp.data(), comp_len)) return false;
    }
    blocks.push_back(std::move(b));
  }
  return true;
}

} // namespace lc
