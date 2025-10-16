#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace equihash {

class EquihashSolver {
 public:
  using Solution = std::vector<uint32_t>;

  struct VerificationStep {
    std::size_t round = 0;
    bool collisionPrefixesMatch = false;
    bool indicesAreDisjoint = false;
  };

  struct VerificationReport {
    bool sizeAndOrderValid = false;
    bool finalHashIsZero = false;
    std::vector<VerificationStep> steps;

    bool rounds_passed() const {
      for (const auto &step : steps) {
        if (!(step.collisionPrefixesMatch && step.indicesAreDisjoint)) {
          return false;
        }
      }
      return true;
    }

    bool passed() const {
      return sizeAndOrderValid && rounds_passed() && finalHashIsZero;
    }
  };

  EquihashSolver(std::size_t n, std::size_t k, std::vector<uint8_t> seed,
                 std::string personalization = {});

  std::vector<Solution> solve(std::size_t initialCount = 0) const;

  bool validate_solution(const Solution &solution) const;

  VerificationReport verify_solution(const Solution &solution) const;

  std::size_t indices_per_solution() const { return 1u << k_; }

 private:
  std::size_t n_;
  std::size_t k_;
  std::size_t collisionBitLength_;
  std::size_t hashBytes_;
  std::size_t unusedBits_;
  std::vector<uint8_t> seed_;
  std::string personalization_;

  std::vector<uint8_t> generate_hash(uint32_t index) const;
  static bool are_disjoint(const Solution &a, const Solution &b);
  Solution merge_indices(const Solution &a, const Solution &b) const;
  std::vector<uint8_t> xor_hashes(const std::vector<uint8_t> &a,
                                  const std::vector<uint8_t> &b) const;
  uint64_t extract_bits(const std::vector<uint8_t> &hash, std::size_t bitOffset,
                        std::size_t bitLength) const;
};

}  // namespace equihash
