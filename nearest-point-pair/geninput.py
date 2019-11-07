import sys
from random import random


n = 100
s = 100.0
if len(sys.argv) >= 2:
    n = int(sys.argv[1])
if len(sys.argv) >= 3:
    s = float(sys.argv[2])


with open("input.txt", "w") as f:
    print(n, file=f)
    for i in range(n):
        print(random() * s, random() * s, file=f)
