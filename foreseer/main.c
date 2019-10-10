#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    int *a = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    int max = INT_MIN, ttl = 0;
    for (int i = 0; i < k - 1; i++) {
        if (a[i] >= max) {
            max = a[i];
            ttl = k;
        }
        ttl--;
    }
    for (int i = k - 1; i < n; i++) {
        if (a[i] >= max) {
            max = a[i];
            ttl = k;
        }
        ttl--;
        if (ttl == 0) {
            max = INT_MIN;
            for (int j = i - k + 1; j <= i; j++) {
                if (a[j] >= max) {
                    max = a[j];
                    ttl = j;
                }
            }
            ttl = ttl + k - i;
        }
        printf("%d ", max);
    }
    printf("\n");

    free(a);
    return 0;
}
