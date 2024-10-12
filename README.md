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
std::cout << "Random Double [0.0, 1.0): " << rng.GenerateDouble(0.0, 1.0) << "\n";
std::cout << "Random Char [a, z]: " << rng.GenerateChar('a', 'z') << "\n";

// Generate a compile-time array of random integers
constexpr auto randomArray = rng.GenerateIntArray<5>(1, 100);
std::cout << "Random Array [1, 100]: ";
for (const auto& val : randomArray)
    std::cout << val << " ";

std::cout << "\n";

// Shuffle the array at compile time
constexpr auto shuffledArray = rng.ShuffleArray(randomArray);
std::cout << "Shuffled Array: ";
for (const auto& val : shuffledArray)
      std::cout << val << " ";

std::cout << "\n";

// Generate a random string at compile time
std::cout << "Random String: ";

constexpr auto randomString = rng.GenerateRandomString<10>();
for (char c : randomString)
    std::cout << c;

std::cout << "\n";
```
**Output:**
```
Random Int: 2049598935
Random Int [1, 10]: 6
Random Bool: false
Random Float [0.0, 1.0): 0.477209
Random Double [0.0, 1.0): 0.477209
Random Char [a, z]: h
Random Array [1, 100]: 36 80 36 23 36
Shuffled Array: 80 36 23 36 36
Random String: hdxonbtza
```
