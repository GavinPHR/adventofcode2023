#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <cmath>
#include <algorithm>


using Map = std::vector<std::vector<bool>>;
std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& c) {
    os << "(" << c.first << "," << c.second << ")";
    return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for (const auto& x : v) os << x << " ";
    return os;
}

std::uint64_t part1(const Map& map) {
    int m = map.size(), n = map[0].size();
    Map map1(m);
    for (int j = 0; j < n; ++j) {
        bool flag = true;
        for (int i = 0; i < m; ++i) {
            map1[i].push_back(map[i][j]);
            if (map[i][j]) flag = false;
        }
        if (flag) {
            for (int i = 0; i < m; ++i) map1[i].push_back(map[i][j]);
        }
    }
    Map map2;
    for (int i = 0; i < m; ++i) {
        map2.push_back(map1[i]);
        bool flag = true;
        for (const auto& b : map1[i]) {
            if (b) {
                flag = false;
                break;
            }
        }
        if (flag) map2.push_back(map1[i]);
    }
    m = map2.size();
    n = map2[0].size();
    std::vector<std::pair<int, int>> co;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (map2[i][j]) co.push_back({i, j});
        }
    }
    std::uint64_t ans = 0;
    for (int i = 0; i < co.size() - 1; ++i) {
        for (int j = i + 1; j < co.size(); ++j) {
            auto diff_i = std::abs(co[i].first - co[j].first);
            auto diff_j = std::abs(co[i].second - co[j].second);
            ans += diff_i + diff_j;
        }
    }
    return ans;
}

std::uint64_t count(int l, int r, const std::vector<int>& v) {
    if (r < l) std::swap(l, r);
    auto bin = [&v](int x) {
        int i = 0, j = v.size() - 1;
        while (i < j) {
            auto mid = i + (j - i) / 2;
            if (v[mid] < x) i = mid + 1;
            else j = mid;
        }
        if (x > v[j]) return j + 1;
        return j;
    };
    return bin(r) - bin(l);
}

std::uint64_t part2(const Map& map) {
    constexpr std::uint64_t factor = 1'000'000;
    int m = map.size(), n = map[0].size();
    std::vector<int> is, js;
    for (int i = 0; i < m; ++i) {
        bool flag = true;
        for (int j = 0; j < n; ++j) {
            if (map[i][j]) {
                flag = false;
                break;
            }
        }
        if (flag) is.push_back(i);
    }
    for (int j = 0; j < n; ++j) {
        bool flag = true;
        for (int i = 0; i < n; ++i) {
            if (map[i][j]) {
                flag = false;
                break;
            }
        }
        if (flag) js.push_back(j);
    }
    std::vector<std::pair<int, int>> co;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (map[i][j]) co.push_back({i, j});
        }
    }
    // std::cout << is << std::endl;
    // std::cout << js << std::endl;
    // std::cout << co << std::endl;
    std::uint64_t ans = 0;
    for (int i = 0; i < co.size() - 1; ++i) {
        for (int j = i + 1; j < co.size(); ++j) {
            // std::cout << co[i] << " " << co[j] << std::endl;
            auto cnt_i = count(co[i].first, co[j].first, is);
            auto cnt_j = count(co[i].second, co[j].second, js);
            // std::cout << count(co[i].first, co[j].first, is) << std::endl;
            // std::cout << count(co[i].second, co[j].second, js) << std::endl;
            auto diff_i = std::abs(co[i].first - co[j].first);
            auto diff_j = std::abs(co[i].second - co[j].second);
            ans += diff_i + diff_j - cnt_i - cnt_j + factor * cnt_i + factor * cnt_j;
        }
    }
    return ans;
}


int main() {
    std::ifstream f("11.txt");
    std::string line;
    Map map;
    while (std::getline(f, line)) {
        map.push_back({});
        for (const auto& c : line) {
            if (c == '.') map.back().push_back(0);
            else map.back().push_back(1);
        }
    }
    std::cout << part2(map) << std::endl;
}


