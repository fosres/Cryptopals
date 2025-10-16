#include "equihash/equihash.h"

#include <algorithm>
#include <array>
#include <iterator>
#include <limits>
#include <set>
#include <stdexcept>

#include "equihash/blake2b.h"

namespace equihash {
namespace {

std::vector<uint8_t> to_bytes(uint32_t value) {
  std::vector<uint8_t> bytes(4);
  for (std::size_t i = 0; i < 4; ++i) {
    bytes[3 - i] = static_cast<uint8_t>(value & 0xFF);
    value >>= 8;
  }
  return bytes;
}

bool all_zero(const std::vector<uint8_t> &data) {
  return std::all_of(data.begin(), data.end(), [](uint8_t b) { return b == 0; });
}

}  // namespace

EquihashSolver::EquihashSolver(std::size_t n, std::size_t k, std::vector<uint8_t> seed,
                               std::string personalization)
    : n_(n),
      k_(k),
      collisionBitLength_(0),
      hashBytes_((n + 7) / 8),
      unusedBits_(hashBytes_ * 8 - n),
      seed_(std::move(seed)),
      personalization_(std::move(personalization)) {
  if (n_ == 0 || k_ == 0) {
    throw std::invalid_argument("Equihash parameters must be positive");
  }
  if (n_ % (k_ + 1) != 0) {
    throw std::invalid_argument("Parameter n must be divisible by k + 1");
  }
  collisionBitLength_ = n_ / (k_ + 1);
}

std::vector<uint8_t> EquihashSolver::generate_hash(uint32_t index) const {
  std::vector<uint8_t> input;
  input.reserve(seed_.size() + personalization_.size() + sizeof(index));
  input.insert(input.end(), seed_.begin(), seed_.end());
  input.insert(input.end(), personalization_.begin(), personalization_.end());
  auto indexBytes = to_bytes(index);
  input.insert(input.end(), indexBytes.begin(), indexBytes.end());

  std::vector<uint8_t> fullHash = blake2b(input, hashBytes_);
  if (unusedBits_ != 0 && !fullHash.empty()) {
    const uint8_t mask = static_cast<uint8_t>(0xFFu << unusedBits_);
    fullHash.back() &= mask;
  }
  return fullHash;
}

bool EquihashSolver::are_disjoint(const Solution &a, const Solution &b) {
  auto itA = a.begin();
  auto itB = b.begin();
  while (itA != a.end() && itB != b.end()) {
    if (*itA == *itB) {
      return false;
    }
    if (*itA < *itB) {
      ++itA;
    } else {
      ++itB;
    }
  }
  return true;
}

EquihashSolver::Solution EquihashSolver::merge_indices(const Solution &a,
                                                        const Solution &b) const {
  Solution merged;
  merged.reserve(a.size() + b.size());
  std::merge(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(merged));
  return merged;
}

std::vector<uint8_t> EquihashSolver::xor_hashes(const std::vector<uint8_t> &a,
                                                const std::vector<uint8_t> &b) const {
  std::vector<uint8_t> result(hashBytes_, 0);
  for (std::size_t i = 0; i < hashBytes_; ++i) {
    const uint8_t lhs = i < a.size() ? a[i] : 0;
    const uint8_t rhs = i < b.size() ? b[i] : 0;
    result[i] = static_cast<uint8_t>(lhs ^ rhs);
  }
  if (unusedBits_ != 0 && !result.empty()) {
    const uint8_t mask = static_cast<uint8_t>(0xFFu << unusedBits_);
    result.back() &= mask;
  }
  return result;
}

uint64_t EquihashSolver::extract_bits(const std::vector<uint8_t> &hash, std::size_t bitOffset,
                                      std::size_t bitLength) const {
  if (bitLength == 0) {
    return 0;
  }
  if (bitLength > 64) {
    throw std::invalid_argument("extract_bits can extract at most 64 bits");
  }
  uint64_t value = 0;
  for (std::size_t i = 0; i < bitLength; ++i) {
    std::size_t bitIndex = bitOffset + i;
    std::size_t byteIndex = bitIndex / 8;
    std::size_t bitInByte = 7 - (bitIndex % 8);
    uint8_t bit = 0;
    if (byteIndex < hash.size()) {
      bit = static_cast<uint8_t>((hash[byteIndex] >> bitInByte) & 0x01u);
    }
    value = (value << 1) | bit;
  }
  return value;
}

std::vector<EquihashSolver::Solution> EquihashSolver::solve(std::size_t initialCount) const {
  struct Node {
    std::vector<uint8_t> hash;
    Solution indices;
  };

  if (initialCount == 0) {
    if (collisionBitLength_ + 1 >= sizeof(std::size_t) * 8) {
      throw std::invalid_argument("collisionBitLength is too large for default work size");
    }
    initialCount = std::size_t{1} << (collisionBitLength_ + 1);
  }
  if (initialCount == 0) {
    throw std::invalid_argument("initialCount must be non-zero");
  }
  if (initialCount > std::numeric_limits<uint32_t>::max()) {
    throw std::invalid_argument("initialCount exceeds supported index range");
  }

  std::vector<Node> current;
  current.reserve(initialCount);
  for (uint32_t i = 0; i < initialCount; ++i) {
    Node node;
    node.hash = generate_hash(i);
    node.indices = Solution{ i };
    current.push_back(std::move(node));
  }

  for (std::size_t round = 0; round < k_; ++round) {
    const std::size_t bitOffset = round * collisionBitLength_;
    std::sort(current.begin(), current.end(), [&](const Node &lhs, const Node &rhs) {
      return extract_bits(lhs.hash, bitOffset, collisionBitLength_) <
             extract_bits(rhs.hash, bitOffset, collisionBitLength_);
    });

    std::vector<Node> next;
    std::size_t groupStart = 0;
    while (groupStart < current.size()) {
      const uint64_t prefix =
          extract_bits(current[groupStart].hash, bitOffset, collisionBitLength_);
      std::size_t groupEnd = groupStart + 1;
      while (groupEnd < current.size() &&
             extract_bits(current[groupEnd].hash, bitOffset, collisionBitLength_) == prefix) {
        ++groupEnd;
      }
      for (std::size_t i = groupStart; i < groupEnd; ++i) {
        for (std::size_t j = i + 1; j < groupEnd; ++j) {
          const Node &a = current[i];
          const Node &b = current[j];
          if (!are_disjoint(a.indices, b.indices)) {
            continue;
          }
          Node combined;
          combined.indices = merge_indices(a.indices, b.indices);
          combined.hash = xor_hashes(a.hash, b.hash);
          next.push_back(std::move(combined));
        }
      }
      groupStart = groupEnd;
    }
    current = std::move(next);
    if (current.empty()) {
      break;
    }
  }

  std::set<Solution> uniqueSolutions;
  std::vector<Solution> solutions;
  for (const auto &node : current) {
    if (!all_zero(node.hash)) {
      continue;
    }
    if (node.indices.size() != indices_per_solution()) {
      continue;
    }
    if (uniqueSolutions.insert(node.indices).second) {
      solutions.push_back(node.indices);
    }
  }
  return solutions;
}

bool EquihashSolver::validate_solution(const Solution &solution) const {
  return verify_solution(solution).passed();
}

EquihashSolver::VerificationReport EquihashSolver::verify_solution(
    const Solution &solution) const {
  VerificationReport report;
  report.finalHashIsZero = false;
  report.sizeAndOrderValid = solution.size() == indices_per_solution() &&
                             std::is_sorted(solution.begin(), solution.end()) &&
                             std::adjacent_find(solution.begin(), solution.end()) ==
                                 solution.end();
  if (!report.sizeAndOrderValid) {
    return report;
  }

  struct Node {
    std::vector<uint8_t> hash;
    Solution indices;
  };

  std::vector<Node> current;
  current.reserve(solution.size());
  for (uint32_t index : solution) {
    Node node;
    node.hash = generate_hash(index);
    node.indices = Solution{ index };
    current.push_back(std::move(node));
  }

  for (std::size_t round = 0; round < k_; ++round) {
    VerificationStep step;
    step.round = round;
    step.collisionPrefixesMatch = true;
    step.indicesAreDisjoint = true;
    step.partialHashZero = true;

    const std::size_t bitOffset = round * collisionBitLength_;
    std::stable_sort(current.begin(), current.end(), [&](const Node &lhs, const Node &rhs) {
      return extract_bits(lhs.hash, bitOffset, collisionBitLength_) <
             extract_bits(rhs.hash, bitOffset, collisionBitLength_);
    });

    std::vector<Node> next;
    next.reserve(current.size() / 2);
    std::size_t groupStart = 0;
    while (groupStart < current.size()) {
      const uint64_t prefix =
          extract_bits(current[groupStart].hash, bitOffset, collisionBitLength_);
      std::size_t groupEnd = groupStart + 1;
      while (groupEnd < current.size() &&
             extract_bits(current[groupEnd].hash, bitOffset, collisionBitLength_) ==
                 prefix) {
        ++groupEnd;
      }
      if ((groupEnd - groupStart) % 2 != 0) {
        step.collisionPrefixesMatch = false;
        step.indicesAreDisjoint = false;
        step.partialHashZero = false;
        report.steps.push_back(step);
        return report;
      }
      for (std::size_t i = groupStart; i < groupEnd; i += 2) {
        const Node &a = current[i];
        const Node &b = current[i + 1];
        if (!are_disjoint(a.indices, b.indices)) {
          step.indicesAreDisjoint = false;
        }
        Node combined;
        combined.indices = merge_indices(a.indices, b.indices);
        combined.hash = xor_hashes(a.hash, b.hash);

        const std::size_t zeroPrefixBits = (round + 1) * collisionBitLength_;
        if (extract_bits(combined.hash, 0, zeroPrefixBits) != 0) {
          step.partialHashZero = false;
        }

        next.push_back(std::move(combined));
      }
      groupStart = groupEnd;
    }
    report.steps.push_back(step);
    if (!(step.collisionPrefixesMatch && step.indicesAreDisjoint &&
          step.partialHashZero)) {
      return report;
    }
    current = std::move(next);
  }

  if (current.size() == 1) {
    report.finalHashIsZero = all_zero(current.front().hash);
  }

  return report;
}

}  // namespace equihash
