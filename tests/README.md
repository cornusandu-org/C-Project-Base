# Tests for stopping.hpp

## Overview

Comprehensive unit tests for `stopping.hpp` focusing on recent changes:

1. **Line 16**: Debug exits now call `exit(errcode)` directly instead of `goto _finish`
2. **Lines 66-67**: Direct boolean access instead of atomic `.load()`/`.store()` operations

## Building and Running

### Prerequisites
- CMake 3.14+
- C++14-compatible compiler
- Internet connection (to fetch GoogleTest)

### Build Instructions

```bash
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

## Test Coverage

### Core Tests
- Exit code validation (12 codes)
- Signal number validation (11 signals)
- Atomic boolean operations
- Thread safety

### Change-Specific Tests
- Debug exit direct call behavior
- Atomic boolean direct access patterns
- Race condition detection
- Memory ordering implications

### Edge Cases
- Exit code uniqueness
- Valid range validation
- Concurrent handler invocation
- Reentrancy protection

### Stress Tests
- Multi-threaded atomic operations (10,000 iterations × 4 threads)
- Concurrent signal handler calls (100 threads)
- Memory ordering verification

## Test Results

The test suite includes 40+ tests covering:
- Constants and definitions
- Atomic operations (old vs new approach)
- Concurrency and thread safety
- Regression tests for specific changes
- Edge cases and boundary conditions

## Notes

- Uses GoogleTest framework
- Some tests may show non-deterministic behavior due to thread scheduling
- Race condition tests document potential issues with direct atomic access