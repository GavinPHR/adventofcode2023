import dataclasses
import numpy as np

@dataclasses.dataclass
class C:
    i: int
    j: int
    k: int

    def __sub__(self, other):
        return C(
            self.i - other.i, 
            self.j - other.j, 
            self.k - other.k
        )
    def __add__(self, other):
        return C(
            self.i + other.i, 
            self.j + other.j, 
            self.k + other.k
        )

cubes = []
with open("22.txt", "r") as f:
    for line in f:
        l, r = line.split("~")
        l = C(*(int(x) for x in l.split(",")))
        r = C(*(int(x) for x in r.split(",")))
        cubes.append((l, r))

cubes.sort(key=lambda c: min(c[0].k, c[1].k))
max_i = max(max(c[0].i, c[1].i) for c in cubes)
max_j = max(max(c[0].j, c[1].j) for c in cubes)
max_k = max(max(c[0].k, c[1].k) for c in cubes)
S = np.zeros((max_k + 1, max_i + 1, max_j + 1))

one_level = C(0, 0, 1)
def get_diff(l, r):
    diff = r - l
    if diff.i > 0:
        diff.i = 1
    if diff.j > 0:
        diff.j = 1
    if diff.k > 0:
        diff.k = 1
    return diff

def fill(l, r, n):
    diff = get_diff(l, r)
    l_ = l + C(0, 0, 0)
    while True:
        S[l_.k][l_.i][l_.j] = n
        if l_ == r:
            break
        l_ += diff


def drop1(l, r, fill_flag=True):
    diff = get_diff(l, r)
    l = l + C(0, 0, 0)
    r = r + C(0, 0, 0)
    while min(l.k, r.k) > 1:
        valid = True
        l_ = l - one_level
        r_ = r - one_level
        while True:
            if S[l_.k][l_.i][l_.j] != 0:
                valid = False
                break
            if l_ == r_:
                break
            l_ += diff
        if valid:
            l = l - one_level
            r = r_
        else:
            break
    if fill_flag:
        fill(l, r, 1)
    return l, r

def simulate():
    for i, (l, r) in enumerate(cubes):
        l, r = drop1(l, r)
        cubes[i] = (l, r)

simulate()
# can_remove = []
# for i, (l, r) in enumerate(cubes):
#     fill(l, r, 0)
#     flag = True
#     for j, (l_, r_) in enumerate(cubes):
#         if i == j:
#             continue
#         fill(l_, r_, 0)
#         l__, r__ = drop1(l_, r_, fill_flag=False)
#         fill(l_, r_, 1)
#         if l_ != l__ or r_ != r__:
#             flag = False
#             break
#     if flag:
#         can_remove.append((l, r))
#     fill(l, r, 1)
# print(len(can_remove))

ans = 0
for i, (l, r) in enumerate(cubes):
    fill(l, r, 0)
    cnt = 0
    fill_back = []
    for j, (l_, r_) in enumerate(cubes):
        if i >= j:
            continue
        fill(l_, r_, 0)
        l__, r__ = drop1(l_, r_, fill_flag=False)
        if l_ != l__ or r_ != r__:
            cnt += 1
            fill_back.append((l_, r_))
        else:
            fill(l_, r_, 1)
    for l_, r_ in fill_back:
        fill(l_, r_, 1)
    fill(l, r, 1)
    ans += cnt

print(ans)
