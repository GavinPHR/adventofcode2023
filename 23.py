with open("23.txt", "r") as f:
    lines = f.read().split("\n")
    map = [list(line) for line in lines]

m, n = len(map), len(map[0])
for j in range(n):
    if map[0][j] == ".":
        start_j = j
        break
for j in range(n):
    if map[m - 1][j] == ".":
        end_j = j
        break


D = {
    ">": (0, 1), 
    "<": (0, -1),
    "v": (1, 0),
    "^": (-1, 0)
}

import sys
sys.setrecursionlimit(10_000)

# cands = []
# def dfs(i, j, seen=set([(0, start_j)])):
#     if i == m - 1 and j == end_j:
#         cands.append(len(seen) - 1)
#         return
#     if map[i][j] == ".":
#         d = list(D.values())
#     else:
#         d = [D[map[i][j]]]
#     for di, dj in d:
#         ii, jj = i + di, j + dj
#         if 0 <= ii < m and 0 <= jj < n and map[ii][jj] != "#" and (ii, jj) not in seen:
#             seen.add((ii, jj))
#             dfs(ii, jj)
#             seen.remove((ii, jj))

# dfs(0, start_j)
# print(max(cands))
from collections import defaultdict
G = defaultdict(set)
intersections = set()
for i in range(m):
    for j in range(n):
        if map[i][j] == "#":
            continue
        poss = []
        for di, dj in list(D.values()):
            ii, jj = i + di, j + dj
            if 0 <= ii < m and 0 <= jj < n and map[ii][jj] != "#":
                poss.append((di, dj))
        if len(poss) == 2:
            continue
        intersections.add((i, j))

for i, j in intersections:
    if map[i][j] == "#":
        continue
    for di, dj in list(D.values()):
        ii, jj = i + di, j + dj
        if 0 <= ii < m and 0 <= jj < n and map[ii][jj] != "#":
            cnt = 1
            curi, curj, previ, prevj = ii, jj, i, j
            while True:
                if (curi, curj) in intersections:
                    break
                for dcuri, dcurj in list(D.values()):
                    newi, newj = curi + dcuri, curj + dcurj
                    if newi == previ and newj == prevj:
                        continue
                    if 0 <= newi < m and 0 <= newj < n and map[newi][newj] != "#":
                        cnt += 1
                        previ, prevj = curi, curj
                        curi, curj = newi, newj
                        break
            G[(i, j)].add((curi, curj, cnt))

# for k, v in G.items():
#     print(k, v)
cands = []
weights = []
def dfs(i, j, seen=set([(0, start_j)])):
    if i == m - 1 and j == end_j:
        cands.append(sum(weights))
        return
    for ii, jj, w in G[(i, j)]:
        if (ii, jj) not in seen:
            seen.add((ii, jj))
            # seen.append((ii, jj))
            weights.append(w)
            dfs(ii, jj)
            weights.pop()
            # seen.pop()
            seen.remove((ii, jj))
dfs(0, start_j)
print(max(cands))
