#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdio>


int part1(const std::string& line) {
    int ans = 0;
    int cur = 0;
    for (const auto& c : line) {
        if (c == ',') {
            ans += cur;
            cur = 0;
            continue;
        }
        cur = ((cur + c) * 17) % 256;
    }
    return ans + cur;
}

std::vector<std::pair<std::string_view, int>> preproc(std::string_view line) {
    int i = 0, j = 0;
    std::vector<std::pair<std::string_view, int>> ret;
    while (i < line.size()) {
        int v = -1;
        int split = -1;
        while (j < line.size() && line[j] != ',') {
            if (line[j] == '=') {
                split = j;
                v = line[j + 1] - '0';
            } else if (line[j] == '-') {
                split = j;
            } 
            ++j;
        }
        ret.push_back({line.substr(i, split - i), v});
        ++j;
        i = j;
    }
    return ret;
}

int hash(std::string_view line) {
    int cur = 0;
    for (const auto& c : line) {
        cur = ((cur + c) * 17) % 256;
    }
    return cur;
}

int part2(std::string_view line) {
    std::vector<std::vector<std::pair<std::string_view, int>>> boxes(256);
    std::vector<std::pair<std::string_view, int>> ret = preproc(line);
    // for (auto [k, v] : ret) printf("%s %d\n", std::string(k).c_str(), v); 
    for (const auto& [k, v] : ret) {
        int b = hash(k);
        bool found = false;
        for (auto it = boxes[b].begin(); it != boxes[b].end(); ++it) {
            auto& k_ = it->first;
            if (k == k_) {
                found = true;
                if (v == -1){
                    boxes[b].erase(it);
                    break;
                } else {
                    it->second = v;
                }
            }
        }
        if (!found && v != -1) boxes[b].push_back({k, v});
        
    }
    int ans = 0;
    for (int i = 0; i < 256; ++i) {
        const auto& box = boxes[i];
        for (int j = 0; j < box.size(); ++j) {
            ans += (i + 1) * (j + 1) * box[j].second;
        }
    }
    return ans;
}

int main() {
    std::ifstream f("15.txt");
    std::string line;
    std::vector<std::string> lines;
    std::getline(f, line);
    std::cout << part2(line) << std::endl;
}


