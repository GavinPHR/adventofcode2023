#include <iostream>
#include <fstream>
#include <utility>
#include <cmath>
#include <vector>

std::vector<int> read_ints(const std::string& s) {
    std::vector<int> ret;
    int cur = 0;
    for (const auto& c : s) {
        if (std::isdigit(c)) {
            cur = cur * 10 + (c - '0');
        } else {
            if (cur) ret.push_back(cur);
            cur = 0;
        }
    }
    ret.push_back(cur);
    return ret;
}

std::uint64_t read_int(const std::string& s) {
    std::uint64_t cur = 0;
    for (const auto& c : s) {
        if (std::isdigit(c)) {
            cur = cur * 10 + (c - '0');
        }
    }
    return cur;
}

int part1(const std::vector<int>& time, const std::vector<int>& distances) {
    int ans = 1;
    for (int i = 0; i < time.size(); ++i) {
        auto T = time[i];
        auto D = distances[i];
        int cnt = 0;
        for (int t = 0; t <= T; ++t) {
            if (t * (T - t) > D) ++cnt;
        }
        ans *= cnt;
    }
    return ans;
}


// t * T - t * t - D == 0, 2 solutions, everything in between
// (T +- sqrt(T * T - 4 * D)) / 2
std::uint64_t part2(double T, double D) {
    auto left = (T - std::sqrt(T * T - 4 * D)) / 2;
    auto right = (T + std::sqrt(T * T - 4 * D)) / 2;
    // std::cout << std::fixed << std::setprecision(5);
    // std::cout << left << " " << right << std::endl;
    return static_cast<std::uint64_t>(right) - static_cast<std::uint64_t>(left);
}


int main() {
    std::ifstream f("6.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(f, line)) {
        lines.push_back(std::move(line));
    }
    auto time = read_ints(lines[0]);
    auto distances = read_ints(lines[1]);

    std::cout << part2(read_int(lines[0]), read_int(lines[1])) << std::endl;
}