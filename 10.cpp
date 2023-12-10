#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <cmath>
#include <algorithm>


using Coordinate = std::pair<int, int>;
const std::vector<Coordinate> moves{{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
using Connection = std::pair<Coordinate, Coordinate>;
const std::unordered_map<char, int> cmap{
    {'|', 1},
    {'-', 2},
    {'L', 3},
    {'J', 4},
    {'7', 5},
    {'F', 6},
    {'.', 7},
    {'S', 0},
};
const std::unordered_map<int, Connection> tiles{
    {1, {moves[1], moves[3]}},
    {2, {moves[0], moves[2]}},
    {3, {moves[1], moves[0]}},
    {4, {moves[1], moves[2]}},
    {5, {moves[3], moves[2]}},
    {6, {moves[3], moves[0]}},
};
// | is a vertical pipe connecting north and south.  13
// - is a horizontal pipe connecting east and west.  02
// L is a 90-degree bend connecting north and east.  10
// J is a 90-degree bend connecting north and west.  12
// 7 is a 90-degree bend connecting south and west.  32
// F is a 90-degree bend connecting south and east.  30
// . is ground; there is no pipe in this tile.
// S is the starting position of the animal; there is a pipe on this tile, but your sketch doesn't show what shape the pipe has.
using Map = std::vector<std::vector<int>>;
Map make_map(const std::vector<std::string>& lines) {
    Map map;
    for (const auto& line : lines) {
        map.push_back({});
        for (const auto& c : line) {
            map.back().push_back(cmap.at(c));
        }
    }
    return map;
}

Coordinate operator+(const Coordinate& left, const Coordinate& right) {
    return {left.first + right.first, left.second + right.second};
}


int get_tile(const Map& map, const Coordinate coordinate) {
    return map[coordinate.first][coordinate.second];
}

Coordinate get_next(const Map& map, const Coordinate& prev, const Coordinate& cur) {
    const auto& [conn1, conn2] = tiles.at(get_tile(map, cur));
    if (cur + conn1 != prev) return cur + conn1;
    return cur + conn2;
}

std::ostream& operator<<(std::ostream& os, const Coordinate& c) {
    os << "(" << c.first << "," << c.second << ")";
    return os;
}


int part1(const Map& map) {
    int m = map.size(), n = map[0].size();
    Coordinate start;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (map[i][j] == 0) {
                start = {i, j};
                break;
            }
        }
    }
    std::vector<Coordinate> seen;
    Coordinate cur;
    for (const auto& move : moves) {
        cur = start + move;
        const auto& [conn1, conn2] = tiles.at(get_tile(map, cur));
        if (cur + conn1 == start || cur + conn2 == start) {
            // seen.push_back(cur);
            break;
        }
    }
    auto prev = start;
    int cnt = 1;
    while (cur != start) {
        auto next = get_next(map, prev, cur);
        prev = cur;
        cur = next;
        ++cnt;
        // seen.push_back(cur);
        // for (auto x : seen) std::cout << x;
        // std::cout << std::endl;
    }
    return cnt / 2;
}

template<>
struct std::hash<std::pair<double, double>>
{
    std::size_t operator()(const std::pair<double, double>& p) const noexcept
    {
        std::size_t h1 = std::hash<double>{}(p.first);
        std::size_t h2 = std::hash<double>{}(p.second);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

double fix_numerical_issues(double x) {
    constexpr double eps = 1e-8;
    if (std::ceil(x) - x < eps) return std::ceil(x);
    if (x - std::floor(x) < eps) return std::floor(x);
    return x;
}

bool inside(const Coordinate& c, const std::vector<Coordinate>& polygon, const Map& map) {
    std::unordered_set<std::pair<double, double>> intersects;
    const double slope_i = static_cast<double>(c.second) / static_cast<double>(c.first);
    const double slope_j= static_cast<double>(c.first) / static_cast<double>(c.second);
    for (int i = 0; i < polygon.size() - 1; ++i) {
        auto a = polygon[i];
        auto b = polygon[i + 1];
        if (a.first == b.first) {
            // horizontal
            if (c.first < a.first) continue;
            if (b.second < a.second) std::swap(a, b);
            double intersect = fix_numerical_issues(a.first * slope_i);
            if (intersect >= a.second && intersect <= b.second) {
                // starting point is a 7...
                if (intersect == static_cast<double>(a.second) && (map[a.first][a.second] == 3 || map[a.first][a.second] == 5 || map[a.first][a.second] == 0)) continue;
                if (intersect == static_cast<double>(b.second) && (map[b.first][b.second] == 3 || map[b.first][b.second] == 5 || map[b.first][b.second] == 0)) continue;
                // std::cout << a << " " << b << " ";
                // std::cout << c << ":" << intersect << std::endl;
                intersects.insert({a.first, intersect});
            }
        } else {
            // vertical
            if (c.second < a.second) continue;
            if (b.first < a.first) std::swap(a, b);
            double intersect = fix_numerical_issues(a.second * slope_j);
            if (intersect >= a.first && intersect <= b.first) {
                if (intersect == static_cast<double>(a.first) && (map[a.first][a.second] == 3 || map[a.first][a.second] == 5 || map[a.first][a.second] == 0)) continue;
                if (intersect == static_cast<double>(b.first) && (map[b.first][b.second] == 3 || map[b.first][b.second] == 5 || map[b.first][b.second] == 0)) continue;
                // std::cout << a << " " << b << " ";
                // std::cout << c << ":" << intersect << std::endl;
                intersects.insert({intersect, a.second});
            }
        }
    }
    // std::vector<std::pair<double, double>> v(intersects.begin(), intersects.end());
    // std::sort(v.begin(), v.end(), [](std::pair<double, double> l, std::pair<double, double> r) {return l.first < r.first;});
    // std::cout << std::setprecision(100);
    // for (auto x : v) {
    // std::cout << "(" << x.first << "," << x.second << ")" << std::endl;
    // }
    // std::cout << intersects.size() << std::endl;
    return intersects.size() % 2;
}

template<>
struct std::hash<Coordinate>
{
    std::size_t operator()(const Coordinate& p) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(p.first);
        std::size_t h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};


int part2(const Map& map) {
    int m = map.size(), n = map[0].size();
    Coordinate start;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (map[i][j] == 0) {
                start = {i, j};
                break;
            }
        }
    }
    Coordinate cur;
    for (const auto& move : moves) {
        cur = start + move;
        if (cur.first < 0 || cur.first >= m || cur.second < 0 || cur.second >= n) continue;
        const auto& [conn1, conn2] = tiles.at(get_tile(map, cur));
        if (cur + conn1 == start || cur + conn2 == start) {
            break;
        }
    }
    auto prev = start;
    std::unordered_set<Coordinate> pipes{start};
    std::vector<Coordinate> polygon{start};
    while (cur != start) {
        pipes.insert(cur);
        auto tile = get_tile(map, cur);
        if (tile != 1 && tile != 2) polygon.push_back(cur);
        auto next = get_next(map, prev, cur);
        prev = cur;
        cur = next;
    }
    int ans = 0;
    polygon.push_back(start);
    // return inside({133, 42}, polygon, map);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == 0 || j == 0) {
                std::cout << 0;
                continue;
            }
            if (pipes.find({i, j}) != pipes.end()) {
                std::cout << 2;
                continue;
            }
            std::cout << inside({i, j}, polygon, map);
            // std::cout << i << "," << j << " " << inside({i, j}, polygon) << "  ";
            // if (i == 122 && j == 14) std::cout << "h" << inside({i, j}, polygon, map) << std::endl;
            ans += inside({i, j}, polygon, map);
        }
        std::cout << std::endl;
    }    
    return ans;
}


int main() {
    std::ifstream f("10.txt");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(f, line)) {
        lines.push_back(std::move(line));
    }
    auto map = make_map(lines);
    std::cout << part2(map) << std::endl;
}


