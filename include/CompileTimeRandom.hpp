#pragma once

#include <array>
#include <iostream>
#include <utility>

class CCompileTimeRandom
{
public:
    // Default constructor using __TIME__ as a seed
    consteval CCompileTimeRandom() : m_uSeed(TimeStringToSeed(__TIME__))
    {
    }

    // Constructor to initialize the class with a specific seed value
    consteval CCompileTimeRandom(unsigned int uSeed) : m_uSeed(uSeed)
    {
    }

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

    // Generate a compile-time array of random integers
    template <std::size_t N>
    consteval std::array<unsigned int, N> GenerateIntArray(unsigned int uMin, unsigned int uMax) const
    {
        std::array<unsigned int, N> arr{};
        unsigned int uCurrentSeed = m_uSeed; // Use the seed as a starting point for array generation
        for (std::size_t i = 0; i < N; ++i)
        {
            // Generate a new random number based on the current seed
            CCompileTimeRandom tempRNG(uCurrentSeed);
            arr[i] = tempRNG.GenerateIntInRange(uMin, uMax);
            // Update the seed with the newly generated random value to change the sequence
            uCurrentSeed = tempRNG.GenerateInt();
        }
        return arr;
    }

    // Compile-time shuffling of an array
    template <std::size_t N>
    consteval std::array<unsigned int, N> ShuffleArray(const std::array<unsigned int, N> &arr) const
    {
        std::array<unsigned int, N> shuffledArray = arr;
        unsigned int uCurrentSeed = m_uSeed; // Seed for the shuffle
        for (std::size_t i = 0; i < N; ++i)
        {
            // Generate a new index based on the current seed for shuffling
            CCompileTimeRandom tempRNG(uCurrentSeed);
            unsigned int j = tempRNG.GenerateIntInRange(0, N - 1); // Random index
            std::swap(shuffledArray[i], shuffledArray[j]);
            uCurrentSeed = tempRNG.GenerateInt(); // Update the seed to prevent repetitive shuffles
        }
        return shuffledArray;
    }

    template <typename T1, typename T2>
    consteval std::pair<T1, T2> GenerateRandomPair(T1 min1, T1 max1, T2 min2, T2 max2) const
    {
        return std::make_pair(GenerateIntInRange(min1, max1), GenerateFloat(min2, max2));
    }

    template <std::size_t N>
    consteval std::array<unsigned int, N> ShuffleArrayNoRepetition(const std::array<unsigned int, N> &arr) const
    {
        std::array<unsigned int, N> shuffledArray = arr;
        unsigned int uCurrentSeed = m_uSeed; // Seed for the shuffle
        for (std::size_t i = 0; i < N; ++i)
        {
            CCompileTimeRandom tempRNG(uCurrentSeed);
            unsigned int j;
            do
            {
                j = tempRNG.GenerateIntInRange(0, N - 1);
            } while (i == j); // Ensure no element stays in its original position
            std::swap(shuffledArray[i], shuffledArray[j]);
            uCurrentSeed = tempRNG.GenerateInt();
        }
        return shuffledArray;
    }

    template <std::size_t N>
    consteval std::array<char, N> GenerateRandomString(char cMin = 'a', char cMax = 'z') const
    {
        std::array<char, N> randomString{};
        unsigned int uCurrentSeed = m_uSeed;
        for (std::size_t i = 0; i < N - 1; ++i) // N - 1 to keep space for null terminator
        {
            CCompileTimeRandom tempRNG(uCurrentSeed);
            randomString[i] = tempRNG.GenerateChar(cMin, cMax);
            uCurrentSeed = tempRNG.GenerateInt();
        }
        randomString[N - 1] = '\0'; // Null-terminate the string
        return randomString;
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
    consteval unsigned int TimeStringToSeed(const char *szTime) const
    {
        unsigned int uHours = (szTime[0] - '0') * 10 + (szTime[1] - '0');
        unsigned int uMinutes = (szTime[3] - '0') * 10 + (szTime[4] - '0');
        unsigned int uSeconds = (szTime[6] - '0') * 10 + (szTime[7] - '0');

        return uHours * 3600 + uMinutes * 60 + uSeconds;
    }
};
