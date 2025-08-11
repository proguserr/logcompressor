
#pragma once
#include <cstdint>
#include <span>

namespace lc {
uint32_t xxh32(std::span<const uint8_t> data, uint32_t seed = 0);
} // namespace lc
