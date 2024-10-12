#pragma once

#include <array>
#include <iostream>
#include <utility>

class CCompileTimeRandom
{
public:
    // Default constructor using __TIME__ as the seed
    consteval CCompileTimeRandom() : m_uSeed(TimeStringToSeed(__TIME__)) {}

    // Constructor to initialize the class with a specific seed value
    consteval CCompileTimeRandom(unsigned int uSeed) : m_uSeed(uSeed) {}

    // Generate a random integer by chaining multiple steps for complexity
    consteval unsigned int GenerateInt() const
    {
        unsigned int part1 = Generate(5);
        unsigned int part2 = GenerateFromSeed(part1, 7);
        unsigned int part3 = GenerateFromSeed(part2, 3);

        return Combine(part1, Combine(part2, part3));
    }

    // Generate a random integer within a range [min, max]
    consteval unsigned int GenerateIntInRange(unsigned int uMin, unsigned int uMax) const
    {
        return uMin + (GenerateInt() % (uMax - uMin + 1));
    }

    // Generate a random boolean value
    consteval bool GenerateBool() const
    {
        return GenerateInt() % 2 == 0;
    }

    // Generate a random float in the range [min, max)
    consteval float GenerateFloat(float flMin, float flMax) const
    {
        float scaled = static_cast<float>(GenerateInt()) / static_cast<float>(0xffffffff);
        return flMin + scaled * (flMax - flMin);
    }

    // Generate a random double in the range [min, max)
    consteval double GenerateDouble(double dbMin, double dbMax) const
    {
        double scaled = static_cast<double>(GenerateInt()) / static_cast<double>(0xffffffff);
        return dbMin + scaled * (dbMax - dbMin);
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
        unsigned int seed = m_uSeed;
        for (std::size_t i = 0; i < N; ++i)
        {
            CCompileTimeRandom tempRNG(seed);
            arr[i] = tempRNG.GenerateIntInRange(uMin, uMax);
            seed = tempRNG.GenerateInt();
        }
        return arr;
    }

    // Shuffle an array at compile time
    template <std::size_t N>
    consteval std::array<unsigned int, N> ShuffleArray(const std::array<unsigned int, N> &arr) const
    {
        std::array<unsigned int, N> shuffledArray = arr;
        unsigned int seed = m_uSeed;
        for (std::size_t i = 0; i < N; ++i)
        {
            CCompileTimeRandom tempRNG(seed);
            unsigned int j = tempRNG.GenerateIntInRange(0, N - 1);
            std::swap(shuffledArray[i], shuffledArray[j]);
            seed = tempRNG.GenerateInt();
        }
        return shuffledArray;
    }

    // Generate a random pair of values
    template <typename T1, typename T2>
    consteval std::pair<T1, T2> GenerateRandomPair(T1 min1, T1 max1, T2 min2, T2 max2) const
    {
        return {GenerateIntInRange(min1, max1), GenerateFloat(min2, max2)};
    }

    // Shuffle an array ensuring no element stays in its original position
    template <std::size_t N>
    consteval std::array<unsigned int, N> ShuffleArrayNoRepetition(const std::array<unsigned int, N> &arr) const
    {
        std::array<unsigned int, N> shuffledArray = arr;
        unsigned int seed = m_uSeed;
        for (std::size_t i = 0; i < N; ++i)
        {
            CCompileTimeRandom tempRNG(seed);
            unsigned int j;
            do
            {
                j = tempRNG.GenerateIntInRange(0, N - 1);
            } while (i == j);
            std::swap(shuffledArray[i], shuffledArray[j]);
            seed = tempRNG.GenerateInt();
        }
        return shuffledArray;
    }

    // Generate a compile-time random string
    template <std::size_t N>
    consteval std::array<char, N> GenerateRandomString(char cMin = 'a', char cMax = 'z') const
    {
        std::array<char, N> randomString{};
        unsigned int seed = m_uSeed;
        for (std::size_t i = 0; i < N - 1; ++i) // N - 1 to reserve space for null terminator
        {
            CCompileTimeRandom tempRNG(seed);
            randomString[i] = tempRNG.GenerateChar(cMin, cMax);
            seed = tempRNG.GenerateInt();
        }
        randomString[N - 1] = '\0'; // Null-terminate the string
        return randomString;
    }

private:
    unsigned int m_uSeed;

    // Pseudo-random number generator using xorshift
    consteval unsigned int XorShift(unsigned int seed) const
    {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }

    // Generate a pseudo-random number based on iterations
    consteval unsigned int Generate(int nIterations) const
    {
        unsigned int value = m_uSeed;
        for (int i = 0; i < nIterations; ++i)
        {
            value = XorShift(value);
        }
        return value;
    }

    // Combine two values for more randomness
    consteval unsigned int Combine(unsigned int a, unsigned int b) const
    {
        return (a ^ (b << 7)) + (b ^ (a >> 3)) + 0x9E3779B9;
    }

    // Hash step to add entropy
    consteval unsigned int HashStep(unsigned int value) const
    {
        return (value * 0x27d4eb2d) ^ (value >> 16);
    }

    // Generate a random-like value from a given seed
    consteval unsigned int GenerateFromSeed(unsigned int seed, int iterations) const
    {
        unsigned int value = seed;
        for (int i = 0; i < iterations; ++i)
        {
            value = Combine(value, HashStep(value));
        }
        return value;
    }

    // Convert __TIME__ macro to seed
    consteval unsigned int TimeStringToSeed(const char *time) const
    {
        unsigned int hours = (time[0] - '0') * 10 + (time[1] - '0');
        unsigned int minutes = (time[3] - '0') * 10 + (time[4] - '0');
        unsigned int seconds = (time[6] - '0') * 10 + (time[7] - '0');
        return hours * 3600 + minutes * 60 + seconds;
    }
};
