#pragma once

#include <iostream>
#include <array>
#include <utility>

class CCompileTimeRandom
{
public:
    // Default constructor using __TIME__ as a seed
    consteval CCompileTimeRandom() 
        : m_uSeed(TimeStringToSeed(__TIME__)) {}

    // Constructor to initialize the class with a seed value
    consteval CCompileTimeRandom(unsigned int uSeed) 
        : m_uSeed(uSeed) {}

    // Generate a more complex random number by chaining random steps
    consteval unsigned int GenerateInt() const
    {
        unsigned int uPart1 = Generate(5);  // First pass with 5 iterations
        unsigned int uPart2 = GenerateFromSeed(uPart1, 7); // Use uPart1 as new seed
        unsigned int uPart3 = GenerateFromSeed(uPart2, 3); // Again with 3 iterations
        
        return Combine(uPart1, Combine(uPart2, uPart3)); // Combine all parts
    }

    // Generate a random integer within a specified range [min, max]
    consteval unsigned int GenerateIntInRange(unsigned int uMin, unsigned int uMax) const
    {
        unsigned int uRandomInt = GenerateInt();
        return uMin + (uRandomInt % (uMax - uMin + 1));
    }

    // Generate a random boolean value
    consteval bool GenerateBool() const
    {
        return (GenerateInt() % 2) == 0; // Even number signifies true
    }

    // Generate a random float in the range [min, max)
    consteval float GenerateFloat(float fMin, float fMax) const
    {
        unsigned int uRandomInt = GenerateInt(); // Generate a random integer
        float fScaledFloat = static_cast<float>(uRandomInt) / static_cast<float>(0xffffffff); // Scale to [0, 1)
        return fMin + fScaledFloat * (fMax - fMin); // Map to [min, max)
    }

    // Generate a random double in the range [min, max)
    consteval double GenerateDouble(double fMin, double fMax) const
    {
        unsigned int uRandomInt = GenerateInt(); // Generate a random integer
        double fScaledDouble = static_cast<double>(uRandomInt) / static_cast<double>(0xffffffff); // Scale to [0, 1)
        return fMin + fScaledDouble * (fMax - fMin); // Map to [min, max)
    }

    // Generate a random character from a given range [min, max]
    consteval char GenerateChar(char cMin = 'a', char cMax = 'z') const
    {
        unsigned int uRandomInt = GenerateInt(); // Generate a random integer
        return static_cast<char>(cMin + (uRandomInt % (cMax - cMin + 1))); // Map to character range
    }

private:
    unsigned int m_uSeed; // Member variable to hold the seed

    // Generate a pseudo-random number based on iterations
    consteval unsigned int Generate(int nIterations) const
    {
        unsigned int uValue = m_uSeed;
        for (int i = 0; i < nIterations; ++i)
            uValue = Combine(uValue, HashStep(uValue));

        return uValue;
    }

    // Combine two values for better randomness
    consteval unsigned int Combine(unsigned int uA, unsigned int uB) const
    {
        return (uA ^ (uB << 7)) + (uB ^ (uA >> 3)) + 0x9E3779B9;
    }

    // Create more entropy from the value
    consteval unsigned int HashStep(unsigned int uValue) const
    {
        return (uValue * 0x27d4eb2d) ^ (uValue >> 16);
    }

    // Generate a random-like value from a specific seed
    consteval unsigned int GenerateFromSeed(unsigned int uNewSeed, int nIterations) const
    {
        unsigned int uValue = uNewSeed;
        for (int i = 0; i < nIterations; ++i)
            uValue = Combine(uValue, HashStep(uValue));

        return uValue;
    }

    // Convert __TIME__ macro string to an unsigned integer seed
    consteval unsigned int TimeStringToSeed(const char* szTime) const
    {
        // Time is in the format HH:MM:SS
        unsigned int uHours   = (szTime[0] - '0') * 10 + (szTime[1] - '0'); // Convert HH
        unsigned int uMinutes = (szTime[3] - '0') * 10 + (szTime[4] - '0'); // Convert MM
        unsigned int uSeconds = (szTime[6] - '0') * 10 + (szTime[7] - '0'); // Convert SS

        // Combine hours, minutes, and seconds into a single seed
        return uHours * 3600 + uMinutes * 60 + uSeconds;
    }
};
