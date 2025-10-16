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

  EquihashSolver::VerificationReport firstReport;
  bool firstReportCaptured = false;
  for (const auto &solution : solutions) {
    auto report = solver.verify_solution(solution);
    if (!report.passed()) {
      std::cerr << "Invalid Equihash solution" << std::endl;
      std::cerr << "  Size and order valid: "
                << (report.sizeAndOrderValid ? "yes" : "no") << '\n';
      for (const auto &step : report.steps) {
        std::cerr << "  Round " << step.round << ": prefixes "
                  << (step.collisionPrefixesMatch ? "match" : "mismatch")
                  << ", indices "
                  << (step.indicesAreDisjoint ? "disjoint" : "overlap")
                  << ", prefix xor "
                  << (step.partialHashZero ? "zero" : "non-zero") << '\n';
      }
      std::cerr << "  Final hash zero: "
                << (report.finalHashIsZero ? "yes" : "no") << std::endl;
      return 1;
    }
    if (!firstReportCaptured) {
      firstReport = report;
      firstReportCaptured = true;
    }
  }

  const auto &first = solutions.front();
  std::cout << "Found " << solutions.size() << " solutions\n";
  std::cout << "Solution size: " << first.size() << "\n";
  std::cout << "Indices:";
  for (uint32_t value : first) {
    std::cout << ' ' << value;
  }
  std::cout << "\nVerification steps:\n";
  std::cout << "  Size and order valid: "
            << (firstReport.sizeAndOrderValid ? "yes" : "no") << '\n';
  for (const auto &step : firstReport.steps) {
    std::cout << "  Round " << step.round << ": prefixes "
              << (step.collisionPrefixesMatch ? "match" : "mismatch")
              << ", indices "
              << (step.indicesAreDisjoint ? "disjoint" : "overlap")
              << ", prefix xor "
              << (step.partialHashZero ? "zero" : "non-zero") << '\n';
  }
  std::cout << "  Final hash zero: "
            << (firstReport.finalHashIsZero ? "yes" : "no") << '\n';
  std::cout << "Overall verification: "
            << (firstReport.passed() ? "passed" : "failed") << std::endl;
  return 0;
}
