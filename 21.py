from collections import deque

with open("21.txt", "r") as f:
    lines = f.read().split("\n")
    map = [list(line) for line in lines]
m = len(map)
n = len(map[0])

directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]
start_i = None
start_j = None
for i in range(m):
    for j in range(n):
        if map[i][j] == "S":
            start_i = i 
            start_j = j

q = deque([(0, start_i, start_j)])
seen = set()
ans = set()
steps = 64
while q:
    cnt, i, j = q.popleft()
    if (i, j) in seen:
        continue
    seen.add((i, j))
    if cnt % 2 == 0:
        ans.add((i, j))
    if cnt == steps:
        continue
    for di, dj in directions:
        ii, jj = i + di, j + dj
        if 0 <= ii < m and 0 <= jj < n and map[ii][jj] != "#" and (ii, jj) not in seen:
        # if map[ii % m][jj] != "#" and (ii, jj) not in seen:
            q.append((cnt + 1 ,ii, jj))

for i, j in ans:
    map[i][j] = "O"
for row in map:
    print("".join(row))
print(len(ans))

def f(steps):
    q = deque([(0, start_i, start_j)])
    seen = set()
    ans = set()
    while q:
        cnt, i, j = q.popleft()
        if (i, j) in seen:
            continue
        seen.add((i, j))
        if cnt % 2 == steps % 2:
            ans.add((i, j))
        if cnt == steps:
            continue
        for di, dj in directions:
            ii, jj = i + di, j + dj
            if map[ii % m][jj % n] != "#" and (ii, jj) not in seen:
                q.append((cnt + 1 ,ii, jj))
    return len(ans)

print()
print(f(65 + 0 * 131))
print(f(65 + 1 * 131))
print(f(65 + 2 * 131))
print(f(65 + 3 * 131))

x = 26501365 // 131
print(3691 + 14694 * x + 14590 * x ** 2)
