#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <unordered_set>
#include <unordered_map>


enum class Type {
    HighCard, OnePair, TwoPair, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind
};

const std::unordered_map<char, int> values{
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'T', 10},
    {'J', 11},
    {'Q', 12},
    {'K', 13},
    {'A', 14},
};

Type determine_type(const std::string& hand) {
    std::unordered_map<char, int> cnt;
    for (const auto& c : hand) ++cnt[c];
    if (cnt.size() == 1) return Type::FiveOfAKind;
    if (cnt.size() == 2) {
        auto test = cnt.cbegin()->second;
        if (test == 1 || test == 4) return Type::FourOfAKind;
        return Type::FullHouse;
    }
    if (cnt.size() == 3) {
        for (const auto& [_, test] : cnt) {
            if (test == 3) return Type::ThreeOfAKind;
        }
        return Type::TwoPair;
    }
    if (cnt.size() == 4) return Type::OnePair;
    return Type::HighCard;
}


bool comp(const std::string& left, const std::string& right) {
    auto t1 = determine_type(left);
    auto t2 = determine_type(right);
    if (t1 != t2) return t1 < t2;
    int i = 0;
    while (i < 5 && (left[i] == right[i])) ++i;
    if (i == 5) return false;
    return values.at(left[i]) < values.at(right[i]);
}



std::uint64_t part1(std::vector<std::pair<std::string, std::uint64_t>>& hands) {
    std::sort(hands.begin(), hands.end(), [](const std::pair<std::string, std::uint64_t>& left, std::pair<std::string, std::uint64_t>& right) {return comp(left.first, right.first);});
    for (auto [k, v] : hands) std::cout << k << " " << v << std::endl;
    std::uint64_t ans = 0;
    for (int i = 0; i < hands.size(); ++i) ans += (i + 1) * hands[i].second;
    return ans;
}


const std::unordered_map<char, int> values2{
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'T', 10},
    {'J', 1},
    {'Q', 12},
    {'K', 13},
    {'A', 14},
};

Type determine_type2(const std::string& hand) {
    std::unordered_map<char, int> cnt;
    for (const auto& c : hand) ++cnt[c];
    if (cnt.find('J') != cnt.end()) {
        auto J_cnt = cnt['J'];
        if (J_cnt == 5) return Type::FiveOfAKind;
        cnt.erase('J');
        int max_count = 0;
        char elem = ' ';
        for (const auto& [k, v] : cnt) {
            if (v > max_count) {
                max_count = v;
                elem = k;
            }
        }
        cnt[elem] += J_cnt;
    }
    if (cnt.size() == 1) return Type::FiveOfAKind;
    if (cnt.size() == 2) {
        auto test = cnt.cbegin()->second;
        if (test == 1 || test == 4) return Type::FourOfAKind;
        return Type::FullHouse;
    }
    if (cnt.size() == 3) {
        for (const auto& [_, test] : cnt) {
            if (test == 3) return Type::ThreeOfAKind;
        }
        return Type::TwoPair;
    }
    if (cnt.size() == 4) return Type::OnePair;
    return Type::HighCard;
}


bool comp2(const std::string& left, const std::string& right) {
    auto t1 = determine_type2(left);
    auto t2 = determine_type2(right);
    if (t1 != t2) return t1 < t2;
    int i = 0;
    while (i < 5 && (left[i] == right[i])) ++i;
    if (i == 5) return false;
    return values2.at(left[i]) < values2.at(right[i]);
}



std::uint64_t part2(std::vector<std::pair<std::string, std::uint64_t>>& hands) {
    std::sort(hands.begin(), hands.end(), [](const std::pair<std::string, std::uint64_t>& left, std::pair<std::string, std::uint64_t>& right) {return comp2(left.first, right.first);});
    // for (auto [k, v] : hands) std::cout << k << " " << v << std::endl;
    std::uint64_t ans = 0;
    for (int i = 0; i < hands.size(); ++i) ans += (i + 1) * hands[i].second;
    return ans;
}


std::pair<std::string, std::uint64_t> read_line(const std::string& line) {
    auto space = line.find(' ');
    return {line.substr(0, space), std::stoll(line.substr(space))};
}

int main() {
    std::ifstream f("7.txt");
    std::string line;
    std::vector<std::pair<std::string, std::uint64_t>> hands;
    while (std::getline(f, line)) {
        hands.push_back(read_line(line));
    }
    std::cout << part2(hands) << std::endl;
}