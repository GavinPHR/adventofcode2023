#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>


using Node = std::string;
using Map = std::unordered_map<Node, std::pair<Node, Node>>;


int part1(const std::string& inst, const Map& map) {
    const static std::string start("AAA");
    const static std::string end("ZZZ");
    auto cur = start;
    int ans = 0;
    while (true) {
        bool found = false;
        for (const auto& c : inst) {
            if (c == 'L') cur = map.at(cur).first;
            else cur = map.at(cur).second;
            ++ans;
            if (cur == end) {
                found = true;
                break;
            }
        }
        if (found) break;
    }
    return ans;
}

std::uint64_t gcd(std::uint64_t a, std::uint64_t b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

std::uint64_t part2(const std::string& inst, const Map& map) {
    std::vector<std::string> nodes;
    for (const auto& [k, _] : map) {
        if (k[2] == 'A') nodes.push_back(k);
    }
    std::uint64_t ans = 1;
    for (auto cur : nodes) {
        int cnt = 0;
        while (true) {
            bool found = false;
            for (const auto& c : inst) {
                if (c == 'L') cur = map.at(cur).first;
                else cur = map.at(cur).second;
                ++cnt;
                if (cur[2] == 'Z') {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        ans  = (ans * cnt) / gcd(ans, cnt);
    }
    return ans;
}

// AAA = (BBB, CCC)
// 0123456789012345
Map make_map(const std::vector<std::string>& lines) {
    Map map;
    for (int i = 2; i < lines.size(); ++i) {
        auto a = lines[i].substr(0, 3);
        auto b = lines[i].substr(7, 3);
        auto c = lines[i].substr(12, 3);
        map[a] = {b, c};
    }
    return map;
}

template<>
struct std::hash<std::pair<std::string, char>>
{
    std::size_t operator()(const std::pair<std::string, char>& p) const noexcept
    {
        std::size_t h1 = std::hash<std::string>{}(p.first);
        std::size_t h2 = std::hash<char>{}(p.second);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};
 

int main() {
    std::ifstream f("8.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(f, line)) {
        lines.push_back(std::move(line));
    }
    auto inst = lines[0];
    auto map = make_map(lines);
    std::cout << part2(inst, map) << std::endl;
}