from collections import defaultdict, Counter
import heapq
import random

G = defaultdict(set)
with open("25.txt", "r") as f:
    for line in f:
        l, rs = line.strip().split(": ")
        for r in rs.split(" "):
            G[l].add(r)
            G[r].add(l)
V = list(G.keys())

def dijkstra(s, t):
    dist = {v: float("inf") for v in V}
    dist[s] = 0
    prev = {}
    q = [(0, s)]
    while q:
        d, u = heapq.heappop(q)
        if u == t:
            break
        if d > dist[u]:
            continue
        for v in G[u]:
            alt = dist[u] + 1
            if alt < dist[v]:
                heapq.heappush(q, (alt, v))
                dist[v] = alt
                prev[v] = u
    assert dist[t] != float("inf")
    edges = []
    cur = t
    while cur != s:
        edges.append((prev[cur], cur))
        cur = prev[cur]
    return edges


def monte_carlo(n=1_000):
    cnt = Counter()
    for _ in range(n):
        i = random.randint(0, len(V) - 1)
        j = random.randint(0, len(V) - 1)
        s = V[i]
        t = V[j]
        edges = dijkstra(s, t)
        for u, v in edges:
            if (u, v) in cnt or (v, u) in cnt:
                if (u, v) in cnt:
                    cnt[(u, v)] += 1
                else:
                    cnt[(v, u)] += 1
            else:
                cnt[(u, v)] += 1
    return [t[0] for t in cnt.most_common(3)]

top3 = monte_carlo()
for u, v in top3:
    G[u].remove(v)
    G[v].remove(u)

seen = set()
stack = [u]
while stack:
    u = stack.pop()
    if u in seen:
        continue
    seen.add(u)
    for v in G[u]:
        stack.append(v)
print(len(seen) * (len(G) - len(seen)))
