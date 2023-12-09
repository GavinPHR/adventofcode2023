#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


std::int64_t rec(const std::vector<std::int64_t>& seq) {
    bool all_zeros = true;
    for (const auto& x : seq) {
        if (x != 0) {
            all_zeros = false;
            break;
        }
    }
    if (all_zeros) return 0;
    std::vector<std::int64_t> new_seq;
    for (int i = 1; i < seq.size(); ++i) {
        new_seq.push_back(seq[i] - seq[i - 1]);
    }
    return seq.back() + rec(new_seq);
}

std::int64_t part1(const std::vector<std::vector<std::int64_t>>& seqs) {
    std::int64_t ans = 0;
    for (const auto& seq : seqs) {
        ans += rec(seq);
    }
    return ans;
}


std::int64_t part2(const std::vector<std::vector<std::int64_t>>& seqs) {
    std::int64_t ans = 0;
    for (auto seq : seqs) {
        std::reverse(seq.begin(), seq.end());
        ans += rec(seq);
    }
    return ans;
}


int main() {
    std::ifstream f("9.txt");
    std::string line;
    std::vector<std::vector<std::int64_t>> seqs;
    while (std::getline(f, line)) {
        std::int64_t sign = 1;
        std::int64_t cur = 0;
        seqs.push_back({});
        for (const auto& c : line) {
            if (std::isdigit(c)) {
                cur = cur * 10 + (c - '0');
            } else if (c == '-') {
                sign = -1;
            } else {
                seqs.back().push_back(sign * cur);
                cur = 0;
                sign = 1;
            }
        }
        seqs.back().push_back(sign * cur);
    }
    std::cout << part2(seqs) << std::endl;
}