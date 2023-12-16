#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cstdio>

using Map = std::vector<std::string>;

struct C {
    int i;
    int j;
};

inline C operator-(const C& lhs, const C& rhs) {
    return {lhs.i - rhs.i, lhs.j - rhs.j};
}
inline C operator+(const C& lhs, const C& rhs) {
    return {lhs.i + rhs.i, lhs.j + rhs.j};
}
inline char idx(const Map& map, const C& c) {
    return map[c.i][c.j];
}
std::pair<C, C> next(const Map& map, const C& cur, const C& prev) {
    constexpr C placeholder{-1, -1};
    auto x = idx(map, cur);
    auto diff = cur - prev;
    if (x == '.') {
        return {cur + diff, placeholder};
    }
    if (x == '-') {
        if (diff.i == 0) return {cur + diff, placeholder};
        return {cur + C{0, -1}, cur + C{0, 1}};
    }
    if (x == '|') {
        if (diff.j == 0) return {cur + diff, placeholder};
        return {cur + C{-1, 0}, cur + C{1, 0}};
    }
    if (x == '/') {
        if (diff.i == 0) {
            if (diff.j > 0) return {cur + C{-1, 0}, placeholder};
            return {cur + C{1, 0}, placeholder};
        } else {
            if (diff.i > 0) return {cur + C{0, -1}, placeholder};
            return {cur + C{0, 1}, placeholder};
        }
    }
    if (x == '\\') {
        if (diff.i == 0) {
            if (diff.j > 0) return {cur + C{1, 0}, placeholder};
            return {cur + C{-1, 0}, placeholder};
        } else {
            if (diff.i > 0) return {cur + C{0, 1}, placeholder};
            return {cur + C{0, -1}, placeholder};
        }
    }
    throw;
}

unsigned int hash(const C& cur, const C& prev) {
    return (cur.i << 24) + (cur.j << 16) + (prev.i << 8) + prev.j;
}

int part1(const Map& map, const std::pair<C, C>& start={{0, 0}, {0, -1}}) {
    int m = map.size(), n = map[0].size();
    std::vector<std::pair<C, C>> stack;
    stack.push_back(start);
    Map marker(m, std::string(n, '.'));
    std::unordered_set<unsigned int> seen;
    while (!stack.empty()) {
        const auto [cur, prev] = std::move(stack.back());
        stack.pop_back();
        auto h = hash(cur, prev);
        if (seen.find(h) != seen.end()) continue;
        seen.insert(h);
        marker[cur.i][cur.j] = '#';
        const auto [next1, next2] = next(map, cur, prev);
        if (next1.i >= 0 && next1.i < m && next1.j >= 0 && next1.j < n) stack.push_back({next1, cur});
        if (next2.i >= 0 && next2.i < m && next2.j >= 0 && next2.j < n) stack.push_back({next2, cur});
    }
    // for (auto row : marker) printf("%s\n", row.c_str());
    int cnt = 0;
    for (const auto& row : marker) {
        for (const auto& c : row) {
            if (c == '#') ++cnt;
        }
    }
    return cnt;
}

int part2(const Map& map) {
    int m = map.size(), n = map[0].size();
    int ans = 0;
    for (int i = 0; i < m; ++i) {
        ans = std::max(ans, part1(map, {{i, 0}, {i, -1}}));
        ans = std::max(ans, part1(map, {{i, n - 1}, {i, n}}));
    }
    for (int j = 0; j < n; ++j) {
        ans = std::max(ans, part1(map, {{0, j}, {-1, j}}));
        ans = std::max(ans, part1(map, {{m - 1, j}, {m, j}}));
    }
    return ans;
}

int main() {
    std::ifstream f("16.txt");
    std::string line;
    Map map;
    while (std::getline(f, line)) {
        map.push_back(std::move(line));
    }
    std::cout << part2(map) << std::endl;
}


