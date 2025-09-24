import matplotlib.pyplot as plt
import os
import re
from collections import defaultdict
from labellines import labelLines

filename = "profile.txt"

profile_data = []
with open(filename) as file:
    profile_data.extend(line.strip() for line in file.readlines())

profile_result = {}  # profile_result[n][container][operation]
temp, n, n_flag = None, None, True
for line in profile_data:
    if n_flag:
        if line.startswith("Profiling with"):
            n_flag = False
            temp = defaultdict(dict)
            n = int(line.split()[-1])
        continue
    if line.startswith("Finished profiling for"):
        n_flag = True
        profile_result[n] = temp
        continue
    m = re.match(r"(\d+)(\w+) (\w+): ([\deE.-]+)s", line)
    if m:
        length, signature, operation, cost = m.groups()
        temp[signature[: int(length)]][operation] = float(cost)

extract = lambda x: next(iter(x))
ns = profile_result.keys()
containers = extract(profile_result.values()).keys()
operations = extract(extract(profile_result.values()).values()).keys()

os.makedirs("img", exist_ok=True)
for operation in operations:
    print(f"Plotting operation: {operation}...")
    plt.figure(figsize=(12, 6))
    for container in containers:
        costs = [profile_result[n][container][operation] for n in ns]
        plt.plot(ns, costs, label=container)
    labelLines(plt.gca().get_lines(), align=False, xvals=80_000)
    plt.xlabel("n")
    plt.xticks(rotation=30)
    plt.ylabel("Time (s)")
    plt.title(f"Profile Time Costs for Operation {operation}")
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"img/profile_{operation}.png")
    # plt.show()
