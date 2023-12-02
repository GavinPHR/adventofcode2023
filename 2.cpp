#include <iostream>
#include <fstream>
#include <algorithm>

struct Set {
    int red = 0;
    int green = 0;
    int blue = 0;

    void clear() {
        red = 0;
        green = 0;
        blue = 0;
    }

    int product() {
        return red * green * blue;
    }

    void update_max(const Set& s) {
        red = std::max(red, s.red);
        green = std::max(green, s.green);
        blue = std::max(blue, s.blue);
    }
};

bool operator<=(const Set& s1, const Set& s2) {
    return (s1.red <= s2.red) && (s1.green <= s2.green) && (s1.blue <= s2.blue);
}

std::ostream& operator<<(std::ostream& stream, const Set& s) {
    std::cout << "(" << s.red << "," << s.green << "," << s.blue << ")";
    return stream;
 }


void process_group(Set& s1, const std::string& line, int l, int r) {
    int n = std::stoi(line.substr(l, r - l));
    while (line[l] != ' ') ++l;
    ++l;
    std::string colour = line.substr(l, r - l);
    if (colour == "red") s1.red = n;
    else if (colour == "green") s1.green = n;
    else if (colour == "blue") s1.blue = n;
    else throw;
}


int part1(const std::string& line) {
    Set s1;
    const static Set s2{12, 13, 14};
    int i = 0;
    while (line[i] != ' ') ++i;
    int game = 0;
    int j = i + 1;
    while (line[j] != ':') {
        game = game * 10 + (line[j] - '0');
        ++j;
    }
    i = j;
    std::cout << game << " ";

    while (i < line.size()) {
        s1.clear();
        j = i + 1;
        while (j < line.size() && line[j] != ';') ++j;
        for (int k = i + 1; k <= j; ++k) {
            if (k == j || line[k] == ',' || line[k] == ';') {
                process_group(s1, line, i + 2, k);
                i = k;
            }
        }
        std::cout << s1 << " ";
        if (!(s1 <= s2)) {
            std::cout << std::endl;
            return 0;
        }
    }
    std::cout << std::endl;
    return game;
}


int part2(const std::string& line) {
    Set s1;
    Set s2;
    int i = 0;
    while (line[i] != ' ') ++i;
    int game = 0;
    int j = i + 1;
    while (line[j] != ':') {
        game = game * 10 + (line[j] - '0');
        ++j;
    }
    i = j;

    while (i < line.size()) {
        s1.clear();
        j = i + 1;
        while (j < line.size() && line[j] != ';') ++j;
        for (int k = i + 1; k <= j; ++k) {
            if (k == j || line[k] == ',' || line[k] == ';') {
                process_group(s1, line, i + 2, k);
                i = k;
            }
        }
        s2.update_max(s1);
    }
    return s2.product();
}

int main() {
    std::ifstream f("2.txt");
    std::string line;
    std::uint64_t ans = 0;
    while (std::getline(f, line)) {
        ans += part2(line);
    }
    std::cout << ans << std::endl;
}