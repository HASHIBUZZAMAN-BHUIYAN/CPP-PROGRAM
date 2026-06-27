// Day 07 Mini Project — Text Statistics Analyzer

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

int main() {
    std::cout << "=== Text Statistics Analyzer ===\n";
    std::cout << "Enter text (end with a blank line):\n";

    std::string text, line;
    while (std::getline(std::cin, line) && !line.empty())
        text += line + " ";

    if (text.empty()) { std::cout << "No input.\n"; return 0; }

    // Character count
    int charCount = static_cast<int>(text.size()) - 1;  // -1 for trailing space

    // Word count and frequency
    std::istringstream iss(text);
    std::string word;
    std::vector<std::string> words;
    std::map<std::string, int> freq;
    while (iss >> word) {
        // strip punctuation
        std::string clean;
        for (char c : word) if (std::isalpha(c)) clean += std::tolower(c);
        if (!clean.empty()) {
            words.push_back(clean);
            freq[clean]++;
        }
    }

    // Sentence count (rough: count . ! ?)
    int sentences = 0;
    for (char c : text) if (c == '.' || c == '!' || c == '?') ++sentences;

    // Most common word
    auto maxIt = std::max_element(freq.begin(), freq.end(),
        [](const auto& a, const auto& b){ return a.second < b.second; });

    std::cout << "\n--- Statistics ---\n";
    std::cout << "Characters:    " << charCount << "\n";
    std::cout << "Words:         " << words.size() << "\n";
    std::cout << "Sentences (~): " << sentences << "\n";
    std::cout << "Unique words:  " << freq.size() << "\n";
    if (maxIt != freq.end())
        std::cout << "Most common:   \"" << maxIt->first << "\" (" << maxIt->second << " times)\n";

    std::cout << "\nTop 5 most frequent:\n";
    std::vector<std::pair<std::string,int>> sorted(freq.begin(), freq.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b){ return a.second > b.second; });
    int shown = 0;
    for (const auto& p : sorted) {
        if (shown++ >= 5) break;
        std::cout << "  \"" << p.first << "\": " << p.second << "\n";
    }

    return 0;
}
