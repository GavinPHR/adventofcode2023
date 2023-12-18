ins = []
with open("18.txt", "r") as f:
    for line in f:
        ins.append(line.split())

D = {
    "R": (0, 1),
    "D": (1, 0),
    "L": (0, -1),
    "U": (-1, 0),
}
for i in range(len(ins)):
    _, _, c = ins[i]
    d = list(D.keys())[int(c[-2])]
    n = int(c[2:-2], 16)
    ins[i] = (d, n, c)

vertices = [(0, 0)]
for d, n, c in ins:
    i, j = vertices[-1]
    di, dj = D[d]
    n = int(n)
    vertices.append((i + n * di, j + n * dj))

def poly_area(vertices):
    area = 0
    for i in range(len(vertices) - 1):
        y1, x1 = vertices[i]
        y2, x2 = vertices[(i + 1) % len(vertices)]
        area += (x1 + x2) * (y2 - y1) / 2
    return area

def inv_picks(A, b):
    return A + 1 - b / 2

A = poly_area(vertices)
b = sum(int(x[1]) for x in ins)
i = inv_picks(A, b)
# print(vertices)
print(b + i)
