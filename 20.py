from collections import deque, defaultdict

class FlipFlop:
    def __init__(self):
        self.state = 0

    def input(self, fname, pulse):
        if pulse == 1:
            return True
        if self.state == 0:
            self.state = 1
        else:
            self.state = 0
    
    def output(self):
        return self.state
        
class Conjunction:
    def __init__(self):
        self.states = {}
    
    def register(self, name):
        self.states[name] = 0

    def input(self, fname, pulse):
        self.states[fname] = pulse
    
    def output(self):
        if all(v == 1 for v in self.states.values()):
            return 0
        return 1
    
class Broadcaster:
    def __init__(self):
        self.state = 0

    def input(self, fname, pulse):
        self.state = pulse

    def output(self):
        return self.state
        
G = {}
M = {}
flipflops = set()
conjunctions = set()
with open("20.txt", "r") as f:
    for line in f:
        line = line.rstrip()
        l, r = line.split(" -> ")
        if l[0] == "%":
            M[l[1:]] = FlipFlop()
            G[l[1:]] = r.split(", ")
            flipflops.add(l[1:])
        elif l[0] == "&":
            M[l[1:]] = Conjunction()
            G[l[1:]] = r.split(", ")
            conjunctions.add(l[1:])
        elif l == "broadcaster":
            M["broadcaster"] = Broadcaster()
            G[l] = r.split(", ")

print(G)
import graphviz
dot = graphviz.Digraph() 
for x, ys in G.items():
    for y in ys:
        dot.edge(x, y)
print(dot.source)  
print(M)
for node, neighs in G.items():
    for neigh in neighs:
        if neigh in conjunctions:
            M[neigh].register(node)
for c in conjunctions:
    print(c, M[c].states)

low = 0
high = 0
cycles = defaultdict(list)
for cnt in range(1, 10_000 + 1):
    # q = deque([("button", 0, "broadcaster")])
    # q = deque([("broadcaster", 0, "ml")])
    # q = deque([("broadcaster", 0, "jn")])
    # q = deque([("broadcaster", 0, "xs")])
    q = deque([("broadcaster", 0, "kl")])
    while q:
        fname, pulse, tname = q.popleft()
        if fname in ["sv", "ch", "th", "gh"] and pulse == 1:
            print("found", cnt)
        if pulse == 0:
            low += 1
        else:
            high += 1
        if tname not in M:
            continue
        module = M[tname]
        flag = module.input(fname, pulse)
        if flag:
            continue
        out = module.output()
        if out is not None:
            for neigh in G[tname]:
                q.append((tname, out, neigh))

print(low * high)
print(3917 * 4001 * 3947 * 3943)
