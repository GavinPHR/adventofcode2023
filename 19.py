import operator
from copy import deepcopy

workflows = {}
parts = []
with open("19.txt", "r") as f:
    is_workflow = True
    for line in f:
        line = line.rstrip()
        if not line:
            is_workflow = False
            continue
        if is_workflow:
            i = line.find("{")
            name = line[:i]
            workflows[name] = []
            for tuple in line[i + 1:-1].split(","):
                j = tuple.find("<")
                if j == -1:
                    j = tuple.find(">")
                if j == -1:
                    workflows[name].append(tuple)
                    continue
                k = tuple.find(":")
                var = tuple[:j]
                op = operator.lt if tuple[j] == "<" else operator.gt
                integer = int(tuple[j + 1:k])
                jump = tuple[k + 1:]
                workflows[name].append((var, op, integer, jump))
        else:
            line = line[1:-1]
            d = {}
            for ab in line.split(","):
                a, b = ab.split("=")
                d[a] = int(b)
            parts.append(d)

ans = 0
for part in parts:
    cur = "in"
    while True:
        if cur == "A" or cur == "R":
            break
        workflow = workflows[cur]
        for task in workflow[:-1]:
            var, op, integer, jump = task
            if op(part[var], integer):
                cur = jump
                break
        else:
            cur = workflow[-1]
    if cur == "A":
        ans += sum(part.values())

print(ans)

stack = [({
    "x": (1, 4000),
    "m": (1, 4000),
    "a": (1, 4000),
    "s": (1, 4000)
}, "in")]

ans = 0
while stack:
    d, cur = stack.pop()
    # print((d, cur))
    if cur == "A":
        b = 1
        for l, r in d.values():
            b *= r - l + 1
        ans += b
        continue
    if cur == "R":
        continue
    workflow = workflows[cur]
    for task in workflow[:-1]:
        var, op, integer, jump = task
        l, r = d[var]
        if l <= integer <= r:
            d_ = deepcopy(d)
            if op == operator.lt:
                d_[var] = (l, integer - 1)
                d[var] = (integer, r)
                stack.append((d_, jump))
            else:
                d_[var] = (integer + 1, r)
                d[var] = (l, integer)
                stack.append((d_, jump))
        elif integer < l and op == operator.gt:
            stack.append((deepcopy(d), jump))
            break
        elif integer > r and op == operator.lt:
            stack.append((deepcopy(d), jump))
            break
    stack.append((deepcopy(d), workflow[-1]))

print(ans)