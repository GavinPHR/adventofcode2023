#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cstdio>
#include <deque>

using Row = std::vector<int>;
using Map = std::vector<Row>;
struct C {
    int k = -1;
    int i = -1;
    int j = -1;
};
inline bool in_bound(int m, int n, const C& c, int max_step=2) {
    if (c.k > max_step) return false;
    if (c.i < 0 || c.i >= m) return false;
    if (c.j < 0 || c.j >= n) return false;
    return true;
}
std::uint64_t hash(const C& cur, const C& prev) {
    // 10bits per loc
    std::uint64_t h = 0;
    h = (h << 10) + cur.k; h = (h << 10) + cur.i; h = (h << 10) + cur.j;
    h = (h << 10) + prev.k; h = (h << 10) + prev.i; h = (h << 10) + prev.j;
    return h;
}
std::pair<C, C> unhash(std::uint64_t h) {
    std::uint64_t mask = (1 << 10) - 1;
    C cur{
        static_cast<int>((h >> 50) & mask), static_cast<int>((h >> 40) & mask), static_cast<int>((h >> 30) & mask)
    };
    C prev{
        static_cast<int>((h >> 20) & mask), static_cast<int>((h >> 10) & mask), static_cast<int>((h) & mask)
    };
    return {cur, prev};
}

int part1(const Map& map) {
    int m = map.size(), n = map[0].size();
    std::unordered_map<std::uint64_t, int> cost_map;
    std::deque<std::pair<C, C>> queue{
        {{0, 0, 0}, {0, 0, -1}},
        {{0, 0, 0}, {0, -1, 0}}
    };
    std::deque<int> costs{0, 0};
    int ans = INT_MAX;
    C ans_c;
    while (!queue.empty()) {
        const auto [cur, prev] = queue.front();
        const auto cost = costs.front();
        const auto h = hash(cur, prev);
        costs.pop_front();
        queue.pop_front();
        if (cost_map.find(h) != cost_map.end()) {
            if (cost >= cost_map[h]) continue;
        }
        cost_map[h] = cost;
        std::vector<C> nexts;
        if (cur.i != prev.i) {
            // up/down
            nexts = {
                {cur.k + 1, cur.i + (cur.i - prev.i), cur.j},
                {0, cur.i, cur.j + 1},
                {0, cur.i, cur.j - 1},
            };
        } else {
            // left/right
            nexts = {
                {cur.k + 1, cur.i, cur.j + (cur.j - prev.j)},
                {0, cur.i + 1, cur.j},
                {0, cur.i - 1, cur.j},
            };
        }
        for (auto& next : nexts) {
            if (!in_bound(m, n, next)) continue;
            int new_cost = cost + map[next.i][next.j];
            if (next.i == m - 1 && next.j == n - 1) {
                ans = std::min(ans, new_cost);
                ans_c = next;
                continue;
            }
            queue.push_back({next, cur});
            costs.push_back(new_cost);
        }
    }
    return ans;
}


int part2(const Map& map) {
    int m = map.size(), n = map[0].size();
    std::unordered_map<std::uint64_t, int> cost_map;
    std::deque<std::pair<C, C>> queue{
        {{0, 0, 0}, {0, 0, -1}},
        {{0, 0, 0}, {0, -1, 0}}
    };
    std::deque<int> costs{0, 0};
    int ans = INT_MAX;
    C ans_c;
    while (!queue.empty()) {
        const auto [cur, prev] = queue.front();
        const auto cost = costs.front();
        const auto h = hash(cur, prev);
        costs.pop_front();
        queue.pop_front();
        if (cost_map.find(h) != cost_map.end()) {
            if (cost >= cost_map[h]) continue;
        }
        cost_map[h] = cost;
        std::vector<C> nexts;
        std::vector<int> next_costs;
        if (cur.i != prev.i) {
            // up/down
            int acc_cost = 0;
            for (int step = 1; step <= 10; ++step) {
                if (cur.j + step >= n) break;
                acc_cost += map[cur.i][cur.j + step];
                if (step >= 4) {
                    nexts.push_back({0, cur.i, cur.j + step});
                    next_costs.push_back(acc_cost);
                }
            }
            acc_cost = 0;
            for (int step = 1; step <= 10; ++step) {
                if (cur.j - step < 0) break;
                acc_cost += map[cur.i][cur.j - step];
                if (step >= 4) {
                    nexts.push_back({0, cur.i, cur.j - step});
                    next_costs.push_back(acc_cost);
                }
            }
        } else {
            // left/right
            int acc_cost = 0;
            for (int step = 1; step <= 10; ++step) {
                if (cur.i + step >= m) break;
                acc_cost += map[cur.i + step][cur.j];
                if (step >= 4) {
                    nexts.push_back({0, cur.i + step, cur.j});
                    next_costs.push_back(acc_cost);
                }
            }
            acc_cost = 0;
            for (int step = 1; step <= 10; ++step) {
                if (cur.i - step < 0) break;
                acc_cost += map[cur.i - step][cur.j];
                if (step >= 4) {
                    nexts.push_back({0, cur.i - step, cur.j});
                    next_costs.push_back(acc_cost);
                }
            }
        }
        for (int i = 0; i < nexts.size(); ++i) {
            auto& next = nexts[i];
            auto& next_cost = next_costs[i];
            if (!in_bound(m, n, next, INT_MAX)) continue;
            int new_cost = cost + next_cost;
            if (next.i == m - 1 && next.j == n - 1) {
                ans = std::min(ans, new_cost);
                ans_c = next;
                continue;
            }
            queue.push_back({next, cur});
            costs.push_back(new_cost);
        }
    }
    return ans;
}


int main() {
    std::ifstream f("17.txt");
    std::string line;
    Map map;
    while (std::getline(f, line)) {
        map.push_back({});
        for (const auto& c : line) map.back().push_back(c - '0');
    }
    std::cout << part2(map) << std::endl;
}


