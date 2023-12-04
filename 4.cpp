#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <utility>
#include <numeric>


using WinningNums = std::unordered_set<int>;
using MyNums = std::unordered_set<int>;
using Card = std::pair<WinningNums, MyNums>;

void print_card(const Card& c) {
    for (const auto& x : c.first) std::cout << x << " ";
    std::cout << "| ";
    for (const auto& x : c.second) std::cout << x << " ";
    std::cout << std::endl;
}


std::vector<Card> parse_input(const std::vector<std::string>& lines) {
    std::vector<Card> cards;
    for (const auto& line : lines) {
        WinningNums wn;
        MyNums mn;
        bool is_winning = true;
        int i = 10;
        int cur = 0;
        while (i < line.size()) {
            if (line[i] == '|') is_winning = false;
            if (line[i] == ' ') {
                if (cur != 0) {
                    if (is_winning) wn.insert(cur);
                    else mn.insert(cur);
                }
                cur = 0;
            } else if (std::isdigit(line[i])) {
                cur = cur * 10 + (line[i] - '0');
            }
            ++i;
        }
        mn.insert(cur);
        cur = 0;
        cards.push_back(std::make_pair(std::move(wn), std::move(mn)));
    }
    return cards;
}


int part1(const std::vector<Card>& cards) {
    std::uint64_t ans = 0;
    for (const auto& card : cards) {
        int cnt = 0;
        for (const auto& n : card.second) {
            if (card.first.find(n) != card.first.end()) {
                ++cnt;
            }
        }
        ans += cnt > 0 ? 1 << (cnt - 1) : 0;
    }
    return ans;
}


int part2(const std::vector<Card>& cards) {
    std::uint64_t ans = 0;
    std::vector<std::uint64_t> copies(cards.size(), 1);
    for (int i = 0; i < cards.size(); ++i) {
        const auto& card = cards[i];
        int cnt = 0;
        for (const auto& n : card.second) {
            if (card.first.find(n) != card.first.end()) {
                ++cnt;
            }
        }
        for (int j = i + 1; j < i + cnt + 1; ++j) copies[j] += copies[i];
    }
    return std::reduce(copies.begin(), copies.end());
}


int main() {
    std::ifstream f("4.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(f, line)) {
        lines.push_back(std::move(line));
    }
    auto cards = parse_input(lines);
    std::cout << part2(cards) << std::endl;
}