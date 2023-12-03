#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <unordered_map>

const std::vector<std::pair<int, int>> moves{
    {1, 0}, {1, 1}, {0, 1}, {-1, 1},
    {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
};


template<class T>
void print_matrix(const std::vector<T>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& e : row) std::cout << e;
        std::cout << std::endl;
    }
}

int part1(const std::vector<std::string>& G) {   
    int m = G.size();
    int n = G[0].size();

    std::vector<std::vector<bool>> adj(m, std::vector<bool>(n, false));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (G[i][j] == '.' || std::isdigit(G[i][j])) continue;
            for (const auto& [di, dj] : moves) {
                int p = i + di;
                int q = j + dj;
                if (p < 0 || p >= m || q < 0 || q >= n) continue;
                adj[p][q] = true;
            }
        }
    }

    int cur = 0;
    int ret = 0;
    for (int i = 0; i < m; ++i) {
        bool is_adj = false;
        for (int j = 0; j <= n; ++j) {
            if (j == n || !std::isdigit(G[i][j])) {
                if (is_adj) ret += cur;
                cur = 0;
                is_adj = false;
            } else {
                is_adj |= adj[i][j];
                cur = cur * 10 + (G[i][j] - '0');
            }
        }
    }
    return ret;
}


int part2(const std::vector<std::string>& G) {   
    int m = G.size();
    int n = G[0].size();

    std::unordered_map<int, std::vector<int>> map;

    int cur = 0;
    for (int i = 0; i < m; ++i) {
        std::vector<int> adj;
        bool found = false;
        for (int j = 0; j <= n; ++j) {
            if (j == n || !std::isdigit(G[i][j])) {
                for (const int id : adj) {
                    map[id].push_back(cur);
                }
                cur = 0;
                adj.clear();
                found = false;
            } else {
                cur = cur * 10 + (G[i][j] - '0');
                for (const auto& [di, dj] : moves) {
                    int p = i + di;
                    int q = j + dj;
                    if (p < 0 || p >= m || q < 0 || q >= n) continue;
                    if (!found && G[p][q] == '*') {
                        adj.push_back(p * n + q);
                        found = true;
                    }
                }
            }
        }
    }

    int ret = 0;
    for (const auto& [k, v] : map) {
        if (v.size() == 2) {
            ret += v[0] * v[1];
        }
    }
    return ret;
}


int main() {
    std::ifstream f("3.txt");
    std::vector<std::string> G;
    std::string line;
    std::uint64_t ans = 0;
    while (std::getline(f, line)) {
        G.push_back(std::move(line));
    }
    std::cout << part2(G) << std::endl;
}