from functools import cache

rows = []
groups_list = []
with open("12.txt", "r") as f:
    for line in f:
        row, g = line.split(" ")
        groups = list(map(int, g.split(",")))
        rows.append(row)
        groups_list.append(groups)


def run(multiple=1):
    ans = 0
    for i, row in enumerate(rows):
        groups_ = groups_list[i]
        row = "?".join([row] * multiple)
        groups = []
        for _ in range(multiple):
            groups += groups_
        @cache
        def rec(p=0, i=0):
            nonlocal row, groups
            def valid(p, g):
                if p + g > len(row):
                    return False
                b1 = "." not in row[p:p + g]
                b2 = (p + g == len(row)) or (row[p + g] != "#")
                return b1 and b2
            if i == len(groups):
                return row[p:].count("#") == 0
            if p >= len(row):
                return 0
            if row[p] == "#":
                if valid(p, groups[i]):
                    return rec(p + groups[i] + 1, i + 1)
                else:
                    return 0
            if valid(p, groups[i]):
                return rec(p + groups[i] + 1, i + 1) + rec(p + 1, i)
            return rec(p + 1, i)
        # print(row, rec())
        ans += rec()
    return ans

print(run(5))
