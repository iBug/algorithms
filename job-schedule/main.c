#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int n, k, *t;
int *x, *a, min_time = INT_MAX, *min_a;

void dispatch(int d) {
    if (d >= n) {
        int time = INT_MIN;
        for (int i = 0; i < k; i++)
            if (x[i] > time)
                time = x[i];
        if (time < min_time) {
            min_time = time;
            for (int i = 0; i < n; i++)
                min_a[i] = a[i];
        }
        return;
    }

    int max_k = (d + 1) < k ? d + 1 : k;
    for (int i = 0; i < max_k; i++) {
        a[d] = i;
        x[i] += t[d];
        if (x[i] < min_time)
            dispatch(d + 1);
        x[i] -= t[d];
    }
}

int main() {
    scanf("%d%d", &n, &k);
    t = malloc(n * sizeof *t);
    x = malloc(k * sizeof *x);
    for (int i = 0; i < n; i++)
        scanf("%d", t + i);
    for (int i = 0; i < k; i++)
        x[i] = 0;

    a = malloc(2 * n * sizeof *a);
    min_a = a + n;

    dispatch(0);
    printf("%d\n", min_time);
    printf("%d", min_a[0]);
    for (int i = 1; i < n; i++)
        printf(" %d", min_a[i]);
    puts("");

    free(t);
    free(x);
    free(a);
    return 0;
}
