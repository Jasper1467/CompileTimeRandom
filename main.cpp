#include <iostream>

#include "include/CompileTimeRandom.hpp"

int main()
{
    constexpr CCompileTimeRandom rng;

    std::cout << "Random Int: " << rng.GenerateInt() << "\n";
    std::cout << "Random Int [1, 10]: " << rng.GenerateIntInRange(1, 10) << "\n";
    std::cout << "Random Bool: " << (rng.GenerateBool() ? "true" : "false") << "\n";
    std::cout << "Random Float [0.0, 1.0): " << rng.GenerateFloat(0.0f, 1.0f) << "\n";

    return 0;
}