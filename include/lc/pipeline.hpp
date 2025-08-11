
#pragma once
#include <string>
#include <cstdint>

namespace lc {

struct Options {
  std::string input;
  std::string output;
  bool decompress{false};
  bool verify_only{false};
  uint8_t codec{1}; // 1=LZ4, 0=RAW
  uint32_t block_size{512 * 1024};
  int threads{0}; // 0 = auto
};

int run_pipeline(const Options& opt);

} // namespace lc
