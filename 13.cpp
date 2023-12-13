#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>


using Pattern = std::vector<std::vector<bool>>;
std::ostream& operator<<(std::ostream& os, const Pattern& p) {
    for (const auto& row : p) {
        for (const auto& x : row) os << x;
        os << std::endl;
    }
    return os;
}
std::vector<bool> slice_j(const Pattern& p, int j) {
    std::vector<bool> ret;
    for (const auto& row : p) ret.push_back(row[j]);
    return ret;
}

int part1_(const Pattern& pattern) {
    int m = pattern.size(), n = pattern[0].size();
    for (int i = 1; i < m; ++i) {
        bool found = true;
        int l = i - 1, r = i;
        while (l >= 0 && r < m) {
            if (pattern[l] != pattern[r]) {
                found = false;
                break;
            }
            --l; ++r;
        }
        if (found) return i * 100;
    }
    for (int j = 1; j < n; ++j) {
        bool found = true;
        int l = j - 1, r = j;
        while (l >= 0 && r < n) {
            if (slice_j(pattern, l) != slice_j(pattern, r)) {
                found = false;
                break;
            }
            --l; ++r;
        }
        if (found) return j;
    }
    return -1;
}

int part1(const std::vector<Pattern>& patterns) {
    int ans = 0;
    for (const auto& pattern : patterns) {
        ans += part1_(pattern);
    }
    return ans;
}

bool one_bit_diff(int a, int b) {
    int c = a ^ b;
    return !(c & (c - 1));
}

int part2_(const Pattern& pattern) {
    int m = pattern.size(), n = pattern[0].size();
    std::vector<int> xs;
    for (const auto& row : pattern) {
        int x = 0;
        for (const auto& v : row) x = (x + v) << 1;
        xs.push_back(x);
    }
    for (int i = 1; i < m; ++i) {
        bool found = true;
        int l = i - 1, r = i;
        bool used = false;
        while (l >= 0 && r < m) {
            if (xs[l] != xs[r]) {
                if (used || !one_bit_diff(xs[l], xs[r])) {
                    found = false;
                    break;
                }
                used = true;
            }
            --l; ++r;
        }
        if (used && found) return i * 100;
    }

    xs.clear();
    for (int j = 0; j < n; ++j) {
        int x = 0;
        for (const auto& v : slice_j(pattern, j)) x = (x + v) << 1;
        xs.push_back(x);
    }
    for (int j = 1; j < n; ++j) {
        bool found = true;
        int l = j - 1, r = j;
        bool used = false;
        while (l >= 0 && r < n) {
            if (xs[l] != xs[r]) {
                if (used || !one_bit_diff(xs[l], xs[r])) {
                    found = false;
                    break;
                }
                used = true;
            }
            --l; ++r;
        }
        if (used && found) return j;
    }
    return -1;
}

int part2(const std::vector<Pattern>& patterns) {
    // int maxm = 0, maxn = 0;
    // for (const auto& pattern : patterns) {
    //     maxm = std::max(maxm, static_cast<int>(pattern.size()));
    //     maxn = std::max(maxn, static_cast<int>(pattern[0].size()));
    // }
    // std::cout << maxm << " " << maxn << std::endl;
    // 17 17
    int ans = 0;
    for (const auto& pattern : patterns) {
        if (part2_(pattern) == -1) std::cout << pattern << std::endl;
        ans += part2_(pattern);
    }
    return ans;
}


int main() {
    std::ifstream f("13.txt");
    std::string line;
    std::vector<Pattern> patterns(1);
    while (std::getline(f, line)) {
        if (line.size() == 0) {
            patterns.push_back({});
            continue;
        }
        auto& pattern = patterns.back();
        pattern.push_back({});
        for (const auto& c : line) {
            if (c == '.') pattern.back().push_back(0);
            else pattern.back().push_back(1);
        }
    }
    std::cout << part2(patterns) << std::endl;
}


