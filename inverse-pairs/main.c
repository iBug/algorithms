#include <stdio.h>
#include <stdlib.h>

#ifndef MOD
#define MOD 10000019UL
#endif

unsigned long merge(int* a, int l, int m, int r) {
    int *b = malloc((r - l) * sizeof(int));
    int i = 0, j = l, k = m;
    unsigned long s = 0;
    while (j < m && k < r) {
        if (a[j] <= a[k]) {
            b[i] = a[j];
            i++;
            j++;
        } else {
            s = (s + m - j) % MOD;
            b[i] = a[k];
            i++;
            k++;
        }
    }
    while (j < m)
        b[i++] = a[j++];
    while (k < r)
        b[i++] = a[k++];
    for (i = 0; i < r - l; i++)
        a[l + i] = b[i];
    free(b);
    return s;
}

unsigned long merge_sort(int* a, int l, int r) {
    if (l + 1 >= r)
        return 0;
    int m = (l + r) / 2;
    unsigned long s = 0;
    s = (s + merge_sort(a, l, m)) % MOD;
    s = (s + merge_sort(a, m, r)) % MOD;
    s = (s + merge(a, l, m, r)) % MOD;
    return s;
}

int main() {
    int n, *a;
    scanf("%d", &n);
    a = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);
    unsigned long s = merge_sort(a, 0, n);
    free(a);
    printf("%lu\n", s);
    return 0;
}
