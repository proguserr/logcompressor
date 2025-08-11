
#include "lc/pipeline.hpp"
#include <iostream>
#include <string>
#include <cstring>

using namespace lc;

static void help() {
  std::cout << "logc - C++ Log Compressor\n"
               "Usage:\n"
               "  logc -i <in> -o <out> [--codec lz4|raw] [--block 512k] [--threads N]\n"
               "  logc -d -i <in.lc> -o <out>\n"
               "  logc --verify -i <in.lc>\n";
}

static uint32_t parse_block(const std::string& s) {
  // accepts K/M suffix
  uint64_t v = 0;
  char suf = 0;
  if (sscanf(s.c_str(), "%llu%c", &v, &suf) >= 1) {
    if (suf == 'k' || suf == 'K') v *= 1024;
    else if (suf == 'm' || suf == 'M') v *= 1024*1024;
    return (uint32_t)v;
  }
  return 512*1024;
}

int main(int argc, char** argv) {
  if (argc == 1) { help(); return 0; }
  Options opt;
  for (int i = 1; i < argc; ++i) {
    std::string a = argv[i];
    if (a == "-i" && i+1 < argc) opt.input = argv[++i];
    else if (a == "-o" && i+1 < argc) opt.output = argv[++i];
    else if (a == "-d") opt.decompress = true;
    else if (a == "--verify") opt.verify_only = true;
    else if (a == "--codec" && i+1 < argc) {
      std::string c = argv[++i];
      if (c == "lz4") opt.codec = 1;
      else if (c == "raw") opt.codec = 0;
    } else if (a == "--block" && i+1 < argc) {
      opt.block_size = parse_block(argv[++i]);
    } else if (a == "--threads" && i+1 < argc) {
      opt.threads = std::stoi(argv[++i]);
    } else if (a == "--help" || a == "-h") {
      help(); return 0;
    }
  }
  if (opt.input.empty()) { std::cerr << "missing -i\n"; return 2; }
  if (!opt.verify_only && opt.output.empty()) { std::cerr << "missing -o\n"; return 2; }
  return run_pipeline(opt);
}
