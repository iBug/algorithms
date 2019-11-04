#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifndef SIZE
#define SIZE 1024
#endif

int n, c[SIZE][SIZE];
int x[SIZE], min_cost = INT_MAX;

void work(int p, int cost) {
    if (p >= n) {
        if (cost < min_cost)
            min_cost = cost;
        return;
    } else if (cost > min_cost) {
        return;
    }
    for (int i = 0; i < n; i++) {
        if (x[i] == 0) {
            x[i] = 1;
            work(p + 1, cost + c[p][i]);
            x[i] = 0;
        }
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &c[i][j]);
    work(0, 0);
    printf("%d\n", min_cost);
    return 0;
}
