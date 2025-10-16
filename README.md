# Cryptopals
This repository hosts my solutions for CryptoPals in C, Golang, or Rust

## Building the Equihash solver

The Equihash implementation lives under `equihash/` and is built with CMake. To
configure, build, and run the accompanying tests:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The build produces a static library target named `equihash` and a
`test_equihash` binary that exercises the solver.
