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
### Without custom seed
```cpp
    constexpr CCompileTimeRandom randGen;
    constexpr unsigned int dwRandomNumber = randGen.GenerateMoreRandom();
```
### With custom seed
```cpp
    constexpr CCompileTimeRandom customRandGen(98765);
    constexpr unsigned int customRandomNumber = customRandGen.GenerateMoreRandom();
```
