#include <iostream>

#include "equihash/equihash.h"

int main() {
  using equihash::EquihashSolver;
  std::vector<uint8_t> seed = {0x45, 0x71, 0x48, 0x01, 0x10};
  std::string personalization = "EquihashTest";
  EquihashSolver solver(32, 3, seed, personalization);

  auto solutions = solver.solve();
  if (solutions.empty()) {
    std::cerr << "No Equihash solutions found" << std::endl;
    return 1;
  }

  for (const auto &solution : solutions) {
    if (!solver.validate_solution(solution)) {
      std::cerr << "Invalid Equihash solution" << std::endl;
      return 1;
    }
  }

  const auto &first = solutions.front();
  std::cout << "Found " << solutions.size() << " solutions\n";
  std::cout << "Solution size: " << first.size() << "\n";
  std::cout << "Indices:";
  for (uint32_t value : first) {
    std::cout << ' ' << value;
  }
  std::cout << std::endl;
  return 0;
}
