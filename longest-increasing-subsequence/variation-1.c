// File: main.c
// Author: iBug

#include <stdio.h>
#include <stdlib.h>

typedef struct _Point {
    int x;
    int y;
} Point;

Point p[10000];
int n, m, min[10000];

void sort_p(int l, int r) {
    if (l + 1 >= r)
        return;
    int i = l, j = r - 1, f = 0;
    while (i < j) {
        if (p[i].x > p[j].x || (p[i].x == p[j].x && p[i].y > p[j].y)) {
            Point t = p[i];
            p[i] = p[j];
            p[j] = t;
            f = !f;
        }
        f ? i++ : j--;
    }
    sort_p(l, i);
    sort_p(i + 1, r);
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d%d", &p[i].x, &p[i].y);
    sort_p(0, n);

    m = 1;
    min[0] = p[0].y;
    for (int i = 1; i < n; i++) {
        if (p[i].y < min[0]) {
            min[0] = p[i].y;
            continue;
        }
        // Binary search
        int l = 0, r = m;
        while (l + 1 < r) {
            int mid = (l + r) / 2;
            if (min[mid] >= p[i].y) {
                r = mid;
            } else {
                l = mid;
            }
        }
        min[l + 1] = p[i].y;
        if (l == m - 1)
            m++;
    }
    printf("%d\n", m);
    return 0;
}
