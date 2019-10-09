#include <stdio.h>
#include <stdlib.h>

#ifndef MOD
#define MOD 1000013
#endif

int merge(int* a, int l, int m, int r) {
    int *b = malloc((r - l) * sizeof(int));
    int i = 0, j = l, k = m, s = 0;
    while (j < m && k < r) {
        if (a[j] <= a[k]) {
            b[i] = a[j];
            i++;
            j++;
        } else {
            s += m - j;
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
    return s;
}

int merge_sort(int* a, int l, int r) {
    if (l + 1 >= r)
        return 0;
    int s = 0, m = (l + r) / 2;
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
    int s = merge_sort(a, 0, n);
    free(a);
    printf("%d\n", s);
    return 0;
}
