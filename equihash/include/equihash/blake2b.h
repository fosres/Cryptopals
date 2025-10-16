#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace equihash {

constexpr std::size_t BLAKE2B_OUTBYTES = 64;

int blake2b(uint8_t *out, std::size_t outlen, const uint8_t *in, std::size_t inlen,
            const uint8_t *key, std::size_t keylen);

std::vector<uint8_t> blake2b(const std::vector<uint8_t> &data, std::size_t outlen);

}  // namespace equihash
