#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

char NumberToSymbol(size_t Num)
{
    switch (Num)
    {
    case 0:
        return 'A';
    case 1:
        return 'C';
    case 2:
        return 'G';
    case 3:
        return 'T';
    default:
        return ' '; // shouldn't happen
    }
}

size_t SymbolToNumber(char symbol)
{
    switch (symbol)
    {
    case 'A':
        return 0;
    case 'C':
        return 1;
    case 'G':
        return 2;
    case 'T':
        return 3;
    default:
        return -1; // shouldn't happen
    }
}


size_t PatternToNumber(const std::string& pattern)
{
    if (pattern.empty()) return 0;

    size_t result = 0;
    size_t base = 1;

    for (int i = pattern.size() - 1; i >= 0; --i)
    {
        result += SymbolToNumber(pattern[i]) * base;
        base *= 4;
    }
    return result;
}


std::string NumberToPattern(size_t index, size_t k)
{
    if (k == 1) return std::string{ NumberToSymbol(index) };
    std::string pattern;
    pattern.reserve(k);

    while (index != 0)
    {
        auto rem = index % 4;
        pattern.insert(pattern.cbegin(), NumberToSymbol(rem));
        index /= 4;
    }

    auto paddingLen = k - pattern.size();

    while (paddingLen-- > 0)
    {
        pattern.insert(pattern.cbegin(), 'A');
    }
    return pattern;
}
std::vector<std::size_t> ComputeFrequencies(const std::string& text, size_t k)
{
    std::vector<std::size_t> FreqArray(size_t(std::pow(4, k)), 0);

    const size_t searchLen = text.size() - k + 1;
    for (size_t i = 0; i < searchLen; ++i)
    {
        std::string pattern = text.substr(i, k);
        const size_t j = PatternToNumber(pattern);
        ++FreqArray[j];
    }
    return FreqArray;
}


std::set<std::string> ClumpFinding(const std::string& genome, size_t k, size_t L, size_t t)
{
    const auto SearchLen = size_t(std::pow(4, k));
    std::vector<size_t> Clump(SearchLen, 0);
    std::set<std::string> FrequentPatterns;

    const auto searchLen = genome.size() - L + 1;
    std::string Text = genome.substr(0, L);
    auto FreqArray = ComputeFrequencies(Text, k);
    for (size_t i = 0; i < SearchLen; ++i)
    {
        if (FreqArray[i] >= t)
        {
            Clump[i] = 1;
        }
    }
    for (size_t i = 1; i < searchLen; ++i)
    {
        std::string FirstPattern = genome.substr(i - 1, k);
        auto idx = PatternToNumber(FirstPattern);
        --FreqArray[idx];

        std::string LastPattern = genome.substr(i + L - k, k);
        idx = PatternToNumber(LastPattern);
        ++FreqArray[idx];
        if (FreqArray[idx] >= t)
        {
            Clump[idx] = 1;
        }
    }

    for (size_t i = 0; i < SearchLen; ++i)
    {
        if (Clump[i] == 1)
        {
            FrequentPatterns.insert(NumberToPattern(i, k));
        }
    }
    return FrequentPatterns;
}


size_t PatternCount(const std::string& text, const std::string& pattern)
{
    size_t count = 0;
    std::string::size_type pos = 0;
    while ((pos = text.find(pattern, pos)) != std::string::npos)
    {
        ++count;
        ++pos;
    }
    return count;
}



std::set<std::string> FrequentWords(const std::string& text, std::size_t k)
{
    std::set<std::string> FrequentPatterns;


    const auto FreqArray = ComputeFrequencies(text, k);
    const size_t maxCount = *std::max_element(FreqArray.begin(), FreqArray.end());
    const auto searchLen = std::pow(4, k);

    for (size_t i = 0; i < searchLen; ++i)
    {
        if (FreqArray[i] == maxCount)
            FrequentPatterns.insert(NumberToPattern(i, k));
    }

    return FrequentPatterns;
}



std::string ReverseComplement(const std::string& text)
{
    std::string result;
    result.reserve(text.size());

    for (int i = text.size() - 1; i >= 0; --i)
    {
        switch (text[i])
        {
        case 'A':
            result.push_back('T');
            break;
        case 'C':
            result.push_back('G');
            break;
        case 'G':
            result.push_back('C');
            break;
        case 'T':
            result.push_back('A');
            break;
        }
    }
    return result;
}

std::vector<size_t> PatternMatching(const std::string& text, const std::string& pattern)
{
    std::vector<size_t> startPositions;
    std::string::size_type pos = 0;
    while ((pos = text.find(pattern, pos)) != std::string::npos)
    {
        startPositions.emplace_back(pos);
        ++pos;
    }
    return startPositions;
}


int main()
{
    auto file = std::ifstream("E_coli.txt");
    std::string genome;
    std::getline(file, genome);
    file.close();

    auto Clumps = ClumpFinding(genome, 9, 500, 3);

    for (const auto& c : Clumps)
    {
        printf("%s ", c.c_str());
    }
    printf("\nClumps count: %u\n", Clumps.size());
    getchar();
}

