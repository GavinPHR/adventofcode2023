import dataclasses
@dataclasses.dataclass
class V:
    x: int
    y: int
    z: int
    def __add__(self, other):
        return V(
            self.x + other.x,
            self.y + other.y,
            self.z + other.z
        )
    def __sub__(self, other):
        return V(
            self.x - other.x,
            self.y - other.y,
            self.z - other.z
        )

s2V = lambda s: V(*(int(x) for x in s.split(", ")))

lines = []
with open("24.txt", "r") as f:
    for line in f:
        l, r = line.split(" @ ")
        lines.append((s2V(l), s2V(r)))

N = len(lines)
# min_area = 200000000000000
# max_area = 400000000000000
# ans = 0
# for i in range(N - 1):
#     for j in range(i + 1, N):
#         x1, v1 = lines[i]
#         x2, v2 = lines[j]
#         a, b = x1.x, x1.y
#         c, d = v1.x, v1.y
#         e, f = x2.x, x2.y
#         g, h = v2.x, v2.y
#         r = g / h
#         denom = (c - r * d)
#         if denom == 0:
#             print(x1, x2, "parallel")
#             continue
#         t1 = (e - r * f - a + r * b) / denom
#         t2 = (a + t1 * c - e) / g
#         if t1 < 0 or t2 < 0:
#             print(x1, x2, "time in past")
#             continue
#         intersect_x = a + t1 * c
#         intersect_y = b + t1 * d
#         if min_area <= intersect_x <= max_area and min_area <= intersect_y <= max_area:
#             print(x1, x2, (intersect_x, intersect_y))
#             ans += 1
#         else:
#             print(x1, x2, "outside")
        
# print(ans)
import z3
c1, c2, c3 = z3.Reals("c1 c2 c3")
d1, d2, d3 = z3.Reals("d1 d2 d3")
# x + t_i * v = c + t * d
equations = []
for i in range(N):
    x, v = lines[i]
    ti = z3.Real(f"t{i}")
    equations.extend([
        x.x + ti * v.x == c1 + ti * d1,
        x.y + ti * v.y == c2 + ti * d2,
        x.z + ti * v.z == c3 + ti * d3
    ])

z3.set_option(max_args=10000000, max_lines=1000000, max_depth=10000000, max_visited=1000000)
z3.solve(equations)
