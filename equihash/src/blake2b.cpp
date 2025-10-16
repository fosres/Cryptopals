#include "equihash/blake2b.h"

#include <array>
#include <cstring>
#include <stdexcept>

namespace equihash {
namespace {

constexpr std::size_t BLAKE2B_BLOCKBYTES = 128;
constexpr std::size_t BLAKE2B_KEYBYTES = 64;
constexpr std::size_t BLAKE2B_OUTBYTES_INTERNAL = 64;

struct blake2b_state {
  std::array<uint64_t, 8> h;
  std::array<uint64_t, 2> t;
  std::array<uint64_t, 2> f;
  std::array<uint8_t, BLAKE2B_BLOCKBYTES> buf;
  std::size_t buflen;
  std::size_t outlen;
};

constexpr uint64_t IV[8] = {0x6A09E667F3BCC908ULL, 0xBB67AE8584CAA73BULL,
                            0x3C6EF372FE94F82BULL, 0xA54FF53A5F1D36F1ULL,
                            0x510E527FADE682D1ULL, 0x9B05688C2B3E6C1FULL,
                            0x1F83D9ABFB41BD6BULL, 0x5BE0CD19137E2179ULL};

inline uint64_t rotr64(uint64_t w, unsigned c) {
  return (w >> c) | (w << (64 - c));
}

inline void g(uint64_t &a, uint64_t &b, uint64_t &c, uint64_t &d, uint64_t x, uint64_t y) {
  a = a + b + x;
  d = rotr64(d ^ a, 32);
  c = c + d;
  b = rotr64(b ^ c, 24);
  a = a + b + y;
  d = rotr64(d ^ a, 16);
  c = c + d;
  b = rotr64(b ^ c, 63);
}

inline void round(std::array<uint64_t, 16> &v, const std::array<uint64_t, 16> &m,
                  std::size_t r) {
  static constexpr uint8_t sigma[12][16] = {
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
      {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
      {11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4},
      {7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8},
      {9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13},
      {2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9},
      {12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11},
      {13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10},
      {6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5},
      {10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0},
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
      {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3}};

  const uint8_t *s = sigma[r];
  g(v[0], v[4], v[8], v[12], m[s[0]], m[s[1]]);
  g(v[1], v[5], v[9], v[13], m[s[2]], m[s[3]]);
  g(v[2], v[6], v[10], v[14], m[s[4]], m[s[5]]);
  g(v[3], v[7], v[11], v[15], m[s[6]], m[s[7]]);
  g(v[0], v[5], v[10], v[15], m[s[8]], m[s[9]]);
  g(v[1], v[6], v[11], v[12], m[s[10]], m[s[11]]);
  g(v[2], v[7], v[8], v[13], m[s[12]], m[s[13]]);
  g(v[3], v[4], v[9], v[14], m[s[14]], m[s[15]]);
}

void blake2b_compress(blake2b_state &S, const uint8_t block[BLAKE2B_BLOCKBYTES]) {
  std::array<uint64_t, 16> m{};
  std::array<uint64_t, 16> v{};

  for (std::size_t i = 0; i < 16; ++i) {
    m[i] = 0;
    for (std::size_t j = 0; j < 8; ++j) {
      m[i] |= static_cast<uint64_t>(block[i * 8 + j]) << (8 * j);
    }
  }

  for (std::size_t i = 0; i < 8; ++i) {
    v[i] = S.h[i];
    v[i + 8] = IV[i];
  }

  v[12] ^= S.t[0];
  v[13] ^= S.t[1];
  v[14] ^= S.f[0];
  v[15] ^= S.f[1];

  for (std::size_t r = 0; r < 12; ++r) {
    round(v, m, r);
  }

  for (std::size_t i = 0; i < 8; ++i) {
    S.h[i] ^= v[i] ^ v[i + 8];
  }
}

void blake2b_init(blake2b_state &S, std::size_t outlen) {
  if (outlen == 0 || outlen > BLAKE2B_OUTBYTES_INTERNAL) {
    throw std::invalid_argument("Invalid blake2b outlen");
  }
  S.h.fill(0);
  S.t.fill(0);
  S.f.fill(0);
  S.buf.fill(0);
  S.buflen = 0;
  S.outlen = outlen;

  for (std::size_t i = 0; i < 8; ++i) {
    S.h[i] = IV[i];
  }
  S.h[0] ^= 0x01010000 ^ static_cast<uint64_t>(outlen);
}

void blake2b_init_key(blake2b_state &S, std::size_t outlen, const uint8_t *key,
                       std::size_t keylen) {
  if (keylen > BLAKE2B_KEYBYTES) {
    throw std::invalid_argument("Invalid blake2b key length");
  }
  blake2b_init(S, outlen);
  std::array<uint8_t, BLAKE2B_BLOCKBYTES> block{};
  std::memcpy(block.data(), key, keylen);
  S.buflen = BLAKE2B_BLOCKBYTES;
  blake2b_compress(S, block.data());
  S.buflen = 0;
  std::memset(block.data(), 0, block.size());
}

void blake2b_update(blake2b_state &S, const uint8_t *in, std::size_t inlen) {
  std::size_t offset = 0;
  while (inlen > 0) {
    const std::size_t space = BLAKE2B_BLOCKBYTES - S.buflen;
    const std::size_t take = std::min(inlen, space);
    std::memcpy(S.buf.data() + S.buflen, in + offset, take);
    S.buflen += take;
    offset += take;
    inlen -= take;

    if (S.buflen == BLAKE2B_BLOCKBYTES) {
      S.t[0] += BLAKE2B_BLOCKBYTES;
      if (S.t[0] < BLAKE2B_BLOCKBYTES) {
        S.t[1] += 1;
      }
      blake2b_compress(S, S.buf.data());
      S.buflen = 0;
    }
  }
}

void blake2b_final(blake2b_state &S, uint8_t *out, std::size_t outlen) {
  if (outlen != S.outlen) {
    throw std::invalid_argument("Output length mismatch in blake2b_final");
  }
  S.t[0] += S.buflen;
  if (S.t[0] < S.buflen) {
    S.t[1] += 1;
  }
  S.f[0] = ~0ULL;
  if (S.buflen < BLAKE2B_BLOCKBYTES) {
    std::memset(S.buf.data() + S.buflen, 0, BLAKE2B_BLOCKBYTES - S.buflen);
  }
  blake2b_compress(S, S.buf.data());

  for (std::size_t i = 0; i < 8; ++i) {
    uint64_t word = S.h[i];
    for (std::size_t j = 0; j < 8 && (i * 8 + j) < outlen; ++j) {
      out[i * 8 + j] = static_cast<uint8_t>(word & 0xFFu);
      word >>= 8;
    }
  }
}

}  // namespace

int blake2b(uint8_t *out, std::size_t outlen, const uint8_t *in, std::size_t inlen,
            const uint8_t *key, std::size_t keylen) {
  blake2b_state S{};
  if (keylen > 0) {
    blake2b_init_key(S, outlen, key, keylen);
  } else {
    blake2b_init(S, outlen);
  }
  blake2b_update(S, in, inlen);
  blake2b_final(S, out, outlen);
  return 0;
}

std::vector<uint8_t> blake2b(const std::vector<uint8_t> &data, std::size_t outlen) {
  std::vector<uint8_t> out(outlen);
  blake2b(out.data(), outlen, data.data(), data.size(), nullptr, 0);
  return out;
}

}  // namespace equihash
