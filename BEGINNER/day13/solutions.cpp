// Day 13 — Solutions

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include <iomanip>

struct Person { std::string name; int age; };

int main() {
    // Exercise 1
    std::cout << "Exercise 1:\n";
    std::cout << "Enter a sentence: ";
    std::string line; std::getline(std::cin, line);
    std::istringstream iss(line);
    std::map<std::string, int> freq;
    std::string word;
    while (iss >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        ++freq[word];
    }
    for (const auto& [w, c] : freq)
        std::cout << std::setw(15) << w << ": " << c << "\n";

    // Exercise 2
    std::cout << "\nExercise 2:\n";
    std::set<int> A = {1,2,3,4,5}, B = {3,4,5,6,7};
    std::vector<int> u, inter, diff;
    std::set_union(A.begin(),A.end(),B.begin(),B.end(), std::back_inserter(u));
    std::set_intersection(A.begin(),A.end(),B.begin(),B.end(), std::back_inserter(inter));
    std::set_difference(A.begin(),A.end(),B.begin(),B.end(), std::back_inserter(diff));
    std::cout << "Union: "; for(int x:u) std::cout<<x<<" "; std::cout<<"\n";
    std::cout << "Inter: "; for(int x:inter) std::cout<<x<<" "; std::cout<<"\n";
    std::cout << "Diff:  "; for(int x:diff) std::cout<<x<<" "; std::cout<<"\n";

    // Exercise 3
    std::cout << "\nExercise 3:\n";
    std::vector<int> nums(20);
    std::iota(nums.begin(), nums.end(), 1);
    std::vector<int> div3;
    std::copy_if(nums.begin(), nums.end(), std::back_inserter(div3),
                 [](int x){ return x % 3 == 0; });
    std::cout << "Div by 3: "; for(int x:div3) std::cout<<x<<" "; std::cout<<"\n";
    std::transform(div3.begin(), div3.end(), div3.begin(), [](int x){return x*x;});
    std::cout << "Squared:  "; for(int x:div3) std::cout<<x<<" "; std::cout<<"\n";
    int sum = std::accumulate(div3.begin(), div3.end(), 0);
    std::cout << "Sum = " << sum << "\n";

    // Exercise 4
    std::cout << "\nExercise 4:\n";
    std::priority_queue<int> pq;
    for (int x : {3,1,4,1,5,9,2,6,5}) pq.push(x);
    while (!pq.empty()) { std::cout << pq.top() << " "; pq.pop(); }
    std::cout << "\n";

    // Exercise 5
    std::cout << "\nExercise 5:\n";
    std::vector<Person> people = {{"Bob",30},{"Alice",25},{"Carol",25},{"Dave",30}};
    std::sort(people.begin(), people.end(), [](const Person& a, const Person& b){
        if (a.age != b.age) return a.age < b.age;
        return a.name < b.name;
    });
    for (const auto& p : people)
        std::cout << p.name << " (" << p.age << ")\n";

    return 0;
}
