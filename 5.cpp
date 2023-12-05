#include <iostream>
#include <fstream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>

using Interval = std::pair<std::uint64_t, std::uint64_t>;
using Map = std::vector<std::vector<std::uint64_t>>;

void print_intervals(const std::vector<Interval>& v) {
    for (const auto& i : v) {
        std::cout << "(" << i.first << "," << i.second << ")";
    }
    std::cout << std::endl;
}

template<>
struct std::hash<Interval> {
    std::size_t operator()(const Interval& interval) const noexcept {
        std::size_t h1 = std::hash<std::uint64_t>{}(interval.first);
        std::size_t h2 = std::hash<std::uint64_t>{}(interval.second);
        return 3 * h1 + h2; 
    }
};

std::vector<Map> parse_input(const std::vector<std::string>& lines) {
    std::vector<Map> maps;
    int i = 1;
    while (i < lines.size()) {
        auto& line = lines[i];
        if (line.size() == 0) {
            i += 2;
            maps.push_back(Map());
            continue;
        }
        std::uint64_t d, s, l;
        d = std::stoll(line);
        s = std::stoll(line.substr(line.find(' ') + 1));
        l = std::stoll(line.substr(line.rfind(' ') + 1));
        maps.back().push_back({d, s, s + l});
        ++i;
    }
    for (auto& m : maps) {
        std::sort(m.begin(), m.end(), [](
            const std::vector<std::uint64_t>& v1, 
            const std::vector<std::uint64_t>& v2) {
                return v1[1] < v2[1];
            });
    }
    // for (auto map : maps) {
    //     for (auto v : map) {
    //         std::cout << v[0] << " " << v[1]<< " " << v[2]<<  std::endl;
    //     }
    //     std::cout << std::endl;
    // }
    return maps;
}

inline bool in_interval(std::uint64_t n, const std::vector<std::uint64_t>& v) {
    if (n >= v[1] && n < v[2]) return true;
    return false;
}

std::uint64_t part1(const std::vector<std::uint64_t>& seeds, const std::vector<Map>& maps) {
    std::uint64_t ret = std::numeric_limits<std::uint64_t>::max();
    for (auto n : seeds) {
        // std::cout << "seed " << n << std::endl;
        for (const auto& map : maps) {
            for (const auto& v : map) {
                if (in_interval(n, v)) {
                    n = v[0] + (n - v[1]);
                    break;
                }
            }
            // std::cout << n << std::endl;
        }
        ret = std::min(ret, n);
    }
    return ret;
}


inline bool is_overlap(const Interval& interval, const std::vector<std::uint64_t>& v) {
    if (interval.first < v[2] && interval.second > v[1]) return true;
    return false;
}

std::uint64_t part2(const std::vector<std::uint64_t>& seeds, const std::vector<Map>& maps) {
    std::uint64_t ret = std::numeric_limits<std::uint64_t>::max();
    for (int i = 0; i < seeds.size(); i += 2) {
        std::vector<Interval> intervals{{seeds[i], seeds[i] + seeds[i + 1]}};
        std::vector<Interval> new_intervals;
        for (const auto& map : maps) {
            // print_intervals(intervals);
            while (!intervals.empty()) {
                auto& interval = intervals.back();
                intervals.pop_back();
                for (const auto& v : map) {
                    if (is_overlap(interval, v)) {
                        if (interval.first < v[1]) new_intervals.push_back({interval.first, v[1]});
                        auto l = std::max(interval.first, v[1]);
                        auto r = std::min(interval.second, v[2]);
                        new_intervals.push_back({v[0] + l - v[1], v[0] + r - v[1]});
                        interval.first = r;
                    }
                }
                if (interval.first != interval.second) new_intervals.push_back({interval.first, interval.second});
            }
            std::swap(intervals, new_intervals);
            // print_intervals(intervals);
        }
        for (const auto& interval : intervals) {
            ret = std::min(ret, interval.first);
        }
    }
    return ret;
}

int main() {
    std::ifstream f("5.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(f, line)) {
        lines.push_back(std::move(line));
    }
    std::vector<std::uint64_t> seeds;
    int i = lines[0].find(' ');
    while (i != -1) {
        seeds.push_back(std::stoll(lines[0].substr(i + 1)));
        i = lines[0].find(' ', i + 1);
    }
    // for (auto s : seeds) std::cout << s << " ";
    // std::cout << std::endl;
    auto maps = parse_input(lines);
    std::cout << part2(seeds, maps) << std::endl;
}