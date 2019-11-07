import sys
from random import randint


n = 10
k = 3
if len(sys.argv) >= 2:
    n = int(sys.argv[1])
if len(sys.argv) >= 3:
    k = int(sys.argv[2])


with open("input.txt", "w") as f:
    print(n, k, file=f)
    print(" ".join([str(randint(1, n * k)) for x in range(n)]), file=f)
