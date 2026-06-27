// Day 04 — Solutions

#include <iostream>
#include <vector>
#include <climits>

int main() {
    // Exercise 1
    std::cout << "Exercise 1:\n";
    std::cout << "Enter N: ";
    int N; std::cin >> N;
    long long sum = 0;
    for (int i = 1; i <= N; ++i) sum += i;
    std::cout << "Sum 1.." << N << " = " << sum << "\n";
    std::cout << "(Formula check: N*(N+1)/2 = " << (long long)N*(N+1)/2 << ")\n";

    // Exercise 2
    std::cout << "\nExercise 2 - FizzBuzz:\n";
    for (int i = 1; i <= 100; ++i) {
        if      (i % 15 == 0) std::cout << "FizzBuzz\n";
        else if (i % 3  == 0) std::cout << "Fizz\n";
        else if (i % 5  == 0) std::cout << "Buzz\n";
        else                  std::cout << i << "\n";
    }

    // Exercise 3
    std::cout << "\nExercise 3:\n";
    std::cout << "Enter a positive integer: ";
    int num; std::cin >> num;
    int tmp = num, digitSum = 0;
    while (tmp > 0) { digitSum += tmp % 10; tmp /= 10; }
    std::cout << "Digit sum of " << num << " = " << digitSum << "\n";

    // Exercise 4
    std::cout << "\nExercise 4:\n";
    std::cout << "Height: ";
    int H; std::cin >> H;
    for (int row = 1; row <= H; ++row) {
        for (int col = 0; col < row; ++col) std::cout << "*";
        std::cout << "\n";
    }

    // Exercise 5
    std::cout << "\nExercise 5:\n";
    std::vector<double> vals;
    std::cout << "Enter 5 doubles:\n";
    for (int i = 0; i < 5; ++i) {
        double d; std::cin >> d;
        vals.push_back(d);
    }
    double minV = vals[0], maxV = vals[0], total = 0;
    for (double v : vals) {
        if (v < minV) minV = v;
        if (v > maxV) maxV = v;
        total += v;
    }
    std::cout << "Min = " << minV << ", Max = " << maxV
              << ", Avg = " << total / vals.size() << "\n";

    return 0;
}
