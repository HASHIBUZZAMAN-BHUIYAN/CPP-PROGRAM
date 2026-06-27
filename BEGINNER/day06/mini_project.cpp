// Day 06 Mini Project — Grade Book
// Store student scores in a vector and compute statistics.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>

int main() {
    std::cout << "=== Grade Book ===\n";
    std::cout << "How many students? ";
    int count; std::cin >> count;
    std::cin.ignore();

    std::vector<std::string> names;
    std::vector<double> scores;

    for (int i = 0; i < count; ++i) {
        std::cout << "Student " << (i+1) << " name: ";
        std::string name; std::getline(std::cin, name);
        std::cout << "Score: ";
        double score; std::cin >> score;
        std::cin.ignore();
        names.push_back(name);
        scores.push_back(score);
    }

    // Stats
    double total = std::accumulate(scores.begin(), scores.end(), 0.0);
    double avg = total / count;
    double hi  = *std::max_element(scores.begin(), scores.end());
    double lo  = *std::min_element(scores.begin(), scores.end());

    std::cout << "\n=== Results ===\n";
    std::cout << std::fixed << std::setprecision(1);
    for (int i = 0; i < count; ++i) {
        char grade;
        if      (scores[i] >= 90) grade = 'A';
        else if (scores[i] >= 80) grade = 'B';
        else if (scores[i] >= 70) grade = 'C';
        else if (scores[i] >= 60) grade = 'D';
        else                       grade = 'F';
        std::cout << std::left << std::setw(20) << names[i]
                  << " | " << std::setw(5) << scores[i]
                  << " | " << grade << "\n";
    }
    std::cout << "\nAverage: " << avg << "  High: " << hi << "  Low: " << lo << "\n";

    return 0;
}
