#include <iostream>

// Class to encapsulate compile-time random number generation
class CCompileTimeRandom
{
public:
    // Default constructor using __TIME__ as a seed
    consteval CCompileTimeRandom() : m_dwSeed(TimeStringToSeed(__TIME__)) {}

    // Constructor to initialize the class with a seed value
    consteval CCompileTimeRandom(unsigned int dwSeed) : m_dwSeed(dwSeed) {}

    // Generate a pseudo-random number based on iterations
    consteval unsigned int Generate(int nIterations) const
    {
        unsigned int dwValue = m_dwSeed;
        for (int i = 0; i < nIterations; ++i)
        {
            dwValue = Combine(dwValue, HashStep(dwValue));
        }
        return dwValue;
    }

    // Generate a more complex random number by chaining random steps
    consteval unsigned int GenerateMoreRandom() const
    {
        unsigned int dwPart1 = Generate(5);   // First pass with 5 iterations
        unsigned int dwPart2 = GenerateFromSeed(dwPart1, 7);  // Use dwPart1 as new seed, 7 iterations
        unsigned int dwPart3 = GenerateFromSeed(dwPart2, 3);  // Again with 3 iterations
        return Combine(dwPart1, Combine(dwPart2, dwPart3));  // Combine all parts for final random number
    }

private:
    unsigned int m_dwSeed;  // Member variable to hold the seed

    // Combine function to mix two values for better randomness
    consteval unsigned int Combine(unsigned int dwA, unsigned int dwB) const
    {
        return (dwA ^ (dwB << 7)) + (dwB ^ (dwA >> 3)) + 0x9E3779B9;
    }

    // Hash-like function to create more entropy from the value
    consteval unsigned int HashStep(unsigned int dwValue) const
    {
        return (dwValue * 0x27d4eb2d) ^ (dwValue >> 16);
    }

    // Generate random-like value from a specific seed
    consteval unsigned int GenerateFromSeed(unsigned int dwNewSeed, int nIterations) const
    {
        unsigned int dwValue = dwNewSeed;
        for (int i = 0; i < nIterations; ++i)
        {
            dwValue = Combine(dwValue, HashStep(dwValue));
        }
        return dwValue;
    }

    // Utility to convert __TIME__ macro string to an unsigned integer seed
    consteval unsigned int TimeStringToSeed(const char* szTime) const
    {
        // Time is in the format HH:MM:SS
        unsigned int uHours = (szTime[0] - '0') * 10 + (szTime[1] - '0'); // Convert HH
        unsigned int uMinutes = (szTime[3] - '0') * 10 + (szTime[4] - '0'); // Convert MM
        unsigned int uSeconds = (szTime[6] - '0') * 10 + (szTime[7] - '0'); // Convert SS

        // Combine hours, minutes, and seconds into a single seed
        return uHours * 3600 + uMinutes * 60 + uSeconds;
    }
};

int main()
{
    // Example 1: Use default constructor with __TIME__ as the seed
    constexpr CCompileTimeRandom randGen;

    // Generate a random number at compile time
    constexpr unsigned int dwRandomNumber = randGen.GenerateMoreRandom();
    std::cout << "Random number at compile time (using __TIME__): " << dwRandomNumber << std::endl;

    // Example 2: Use custom seed
    constexpr CCompileTimeRandom customRandGen(98765);
    constexpr unsigned int customRandomNumber = customRandGen.GenerateMoreRandom();
    std::cout << "Random number with custom seed: " << customRandomNumber << std::endl;

    return 0;
}