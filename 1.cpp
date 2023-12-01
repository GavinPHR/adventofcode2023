#include <iostream>
#include <fstream>
#include <vector>

int part1(const std::string& line) {
    const size_t n = line.size();
    int first, last;
    for (size_t i = 0; i < n; ++i) {
        if (std::isdigit(line[i])) {
            first = line[i] - '0';
            break;
        }
    }
    for (size_t i = n - 1; i >= 0; --i) {
        if (std::isdigit(line[i])) {
            last = line[i] - '0';
            break;
        }
    }
    return first * 10 + last;
}

const std::string digits[]{"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

inline bool string_match(const std::string& line, const std::string& pat, int idx) {
    for (int i = 0; i < pat.size(); ++i) {
        if (idx + i >= line.size()) return false;
        if (line[idx + i] != pat[i]) return false;
    }
    return true;
}

int part2(const std::string& line) {
    std::vector<int> candidates;
    int idx = 0;
    while (idx < line.size()) {
        if (std::isdigit(line[idx])) {
            candidates.push_back(line[idx++] - '0');
        } else {
            for (int i = 1; i < 10; ++i) {
                if (string_match(line, digits[i], idx)) {
                    candidates.push_back(i);
                    break;
                }
            }
            ++idx;
        }
    }
    return candidates[0] * 10 + candidates.back();
}

int main() {
    std::ifstream f("1.txt");
    std::string line;
    std::uint64_t ans = 0;
    while (std::getline(f, line)) {
        // ans += part1(line);
        ans += part2(line);
    }
    std::cout << ans << std::endl;
}