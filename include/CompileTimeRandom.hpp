#pragma once

#include <array>
#include <iostream>
#include <utility>

class CCompileTimeRandom
{
public:
    // Default constructor using __TIME__ as a seed
    consteval CCompileTimeRandom() : m_uSeed(TimeStringToSeed(__TIME__)) {}

    // Constructor to initialize the class with a specific seed value
    consteval CCompileTimeRandom(unsigned int uSeed) : m_uSeed(uSeed) {}

    // Generate a random number by chaining multiple steps for complexity
    consteval unsigned int GenerateInt() const
    {
        unsigned int uPart1 = Generate(5);
        unsigned int uPart2 = GenerateFromSeed(uPart1, 7);
        unsigned int uPart3 = GenerateFromSeed(uPart2, 3);

        return Combine(uPart1, Combine(uPart2, uPart3));
    }

    // Generate a random integer within a specified range [min, max]
    consteval unsigned int GenerateIntInRange(unsigned int uMin, unsigned int uMax) const
    {
        return uMin + (GenerateInt() % (uMax - uMin + 1));
    }

    // Generate a random boolean value
    consteval bool GenerateBool() const
    {
        return (GenerateInt() % 2) == 0;
    }

    // Generate a random float in the range [min, max)
    consteval float GenerateFloat(float flMin, float flMax) const
    {
        float flScaledFloat = static_cast<float>(GenerateInt()) / static_cast<float>(0xffffffff);
        return flMin + flScaledFloat * (flMax - flMin);
    }

    // Generate a random double in the range [min, max)
    consteval double GenerateDouble(double dbMin, double dbMax) const
    {
        double dbScaledDouble = static_cast<double>(GenerateInt()) / static_cast<double>(0xffffffff);
        return dbMin + dbScaledDouble * (dbMax - dbMin);
    }

    // Generate a random character within a given range
    consteval char GenerateChar(char cMin = 'a', char cMax = 'z') const
    {
        return static_cast<char>(cMin + (GenerateInt() % (cMax - cMin + 1)));
    }

private:
    unsigned int m_uSeed;

    // Generate a pseudo-random number based on a number of iterations
    consteval unsigned int Generate(int nIterations) const
    {
        unsigned int uValue = m_uSeed;
        for (int i = 0; i < nIterations; ++i)
            uValue = Combine(uValue, HashStep(uValue));

        return uValue;
    }

    // Combine two values for more randomness
    consteval unsigned int Combine(unsigned int uA, unsigned int uB) const
    {
        return (uA ^ (uB << 7)) + (uB ^ (uA >> 3)) + 0x9E3779B9;
    }

    // Apply a hash step to add entropy
    consteval unsigned int HashStep(unsigned int uValue) const
    {
        return (uValue * 0x27d4eb2d) ^ (uValue >> 16);
    }

    // Generate a random-like value from a given seed
    consteval unsigned int GenerateFromSeed(unsigned int uNewSeed, int nIterations) const
    {
        unsigned int uValue = uNewSeed;
        for (int i = 0; i < nIterations; ++i)
            uValue = Combine(uValue, HashStep(uValue));

        return uValue;
    }

    // Convert __TIME__ macro string to a seed
    consteval unsigned int TimeStringToSeed(const char* szTime) const
    {
        unsigned int uHours = (szTime[0] - '0') * 10 + (szTime[1] - '0');
        unsigned int uMinutes = (szTime[3] - '0') * 10 + (szTime[4] - '0');
        unsigned int uSeconds = (szTime[6] - '0') * 10 + (szTime[7] - '0');

        return uHours * 3600 + uMinutes * 60 + uSeconds;
    }
};
