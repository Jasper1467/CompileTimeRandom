# CompileTimeRandom

**CompileTimeRandom** is a modern C++ random number generator designed to generate random numbers entirely at compile time using advanced meta-programming techniques. This enables deterministic random number generation that can be evaluated and optimized during compilation, making it highly efficient for scenarios where runtime randomness is not required.

## Features

- Compile-time random number generation using `consteval`.
- Default seeding based on the compilation time (`__TIME__` macro).
- Custom seed support for deterministic randomness.
- Entirely header-only, no dependencies.
- Leverages C++20/23 features for meta-programming and compile-time execution.

## Requirements

- **C++ Standard**: Requires C++20 or later.
- **Compiler**: A compiler that supports `consteval` and other modern C++20/23 features (e.g., GCC 10+, Clang 10+, MSVC 19.29+).

## Usage
```cpp
constexpr CCompileTimeRandom rng;

    std::cout << "Random Int: " << rng.GenerateInt() << "\n";
    std::cout << "Random Int [1, 10]: " << rng.GenerateIntInRange(1, 10) << "\n";
    std::cout << "Random Bool: " << (rng.GenerateBool() ? "true" : "false") << "\n";
    std::cout << "Random Float [0.0, 1.0): " << rng.GenerateFloat(0.0f, 1.0f) << "\n";
```