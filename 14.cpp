#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdio>


int part1(const std::vector<std::string>& lines) {
    int m = lines.size(), n = lines[0].size();
    int ans = 0;
    for (int j = 0; j < n; ++j) {
        int obs = -1;
        int cnt = 0;
        for (int i = 0; i < m; ++i) {
            if (lines[i][j] == '.') continue;
            if (lines[i][j] == '#') {
                obs = i;
                cnt = 0;
                continue;
            }
            ans += m - (obs + cnt) - 1;
            // printf("%d %d %d\n", i, j, m - (obs + cnt) - 1);
            ++cnt;
        }
    }
    return ans;
}


void simulate(std::vector<std::string>& lines, std::string direction) {
    int m = lines.size(), n = lines[0].size();
    if (direction == "north") {
        for (int j = 0; j < n; ++j) {
            int obs = -1;
            int cnt = 0;
            for (int i = 0; i < m; ++i) {
                if (lines[i][j] == '.') continue;
                if (lines[i][j] == '#') {
                    obs = i;
                    cnt = 0;
                    continue;
                }
                lines[i][j] = '.';
                lines[obs + cnt + 1][j] = 'O';
                ++cnt;
            }
        }
    } else if (direction == "west") {
        for (int i = 0; i < m; ++i) {
            int obs = -1;
            int cnt = 0;
            for (int j = 0; j < n; ++j) {
                if (lines[i][j] == '.') continue;
                if (lines[i][j] == '#') {
                    obs = j;
                    cnt = 0;
                    continue;
                }
                lines[i][j] = '.';
                lines[i][obs + cnt + 1] = 'O';
                ++cnt;
            }
        }
    } else if (direction == "south") {
        for (int j = 0; j < n; ++j) {
            int obs = m;
            int cnt = 0;
            for (int i = m - 1; i >= 0; --i) {
                if (lines[i][j] == '.') continue;
                if (lines[i][j] == '#') {
                    obs = i;
                    cnt = 0;
                    continue;
                }
                lines[i][j] = '.';
                lines[obs - cnt - 1][j] = 'O';
                ++cnt;
            }
        }
    } else {
        for (int i = 0; i < m; ++i) {
            int obs = n;
            int cnt = 0;
            for (int j = n - 1; j >= 0; --j) {
                if (lines[i][j] == '.') continue;
                if (lines[i][j] == '#') {
                    obs = j;
                    cnt = 0;
                    continue;
                }
                lines[i][j] = '.';
                lines[i][obs - cnt - 1] = 'O';
                ++cnt;
            }
        }
    }
}

void print(const std::vector<std::string>& lines) {
    for (const auto& s : lines) std::cout << s << std::endl;
    std::cout<< std::endl;
}

int part2(const std::vector<std::string>& lines) {
    const static std::vector<std::string> directions{"north", "west", "south", "east"};
    constexpr int cycles = 1000000000;
    std::vector<std::vector<std::string>> seen{lines};
    int found_i = -1;
    int iters = 0;
    while (true) {
        auto last = seen.back();
        for (const auto& d : directions) {
            simulate(last, d);
            // printf("%s\n", d.c_str());
            // print(last);
        }
        ++iters;
        for (int i = 0; i < seen.size(); ++i) {
            if (seen[i] == last) {
                found_i = i;
                break;
            }
        }
        if (found_i != -1) break;
        seen.push_back(std::move(last));
    }
    // printf("%d %d %d\n", found_i, iters, seen.size());
    // print(seen[found_i + (cycles - found_i) % (iters - found_i)]);
    const auto& target = seen[found_i + (cycles - found_i) % (iters - found_i)];
    int ans = 0;
    for (int i = 0; i < target.size(); ++i) {
        for (int j = 0; j < target[0].size(); ++j) {
            if (target[i][j] == 'O') {
                ans += target.size() - i;
            }
        }
    }
    return ans; 
}

int main() {
    std::ifstream f("14.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(f, line)) {
        lines.push_back(std::move(line));
    }
    std::cout << part2(lines) << std::endl;
}


