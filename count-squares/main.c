#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int n, m, sum = 0, broken, prevSquare;
    scanf("%d%d", &n, &m);
    int *maxSquare = malloc(m * sizeof(int)),
        *maxLeft = malloc(m * sizeof(int)),
        *maxTop = malloc(m * sizeof(int));

    // Inception
    scanf("%d", &broken);
    maxSquare[0] = maxLeft[0] = maxTop[0] = !broken;
    sum += maxSquare[0];

    // First line
    for (int i = 1; i < m; i++) {
        scanf("%d", &broken);
        if (broken) {
            maxSquare[i] = maxLeft[i] = maxTop[i] = 0;
        } else {
            maxSquare[i] = maxTop[i] = 1;
            maxLeft[i] = maxLeft[i - 1] + 1;
        }
        sum += maxSquare[i];
    }

    // Subsequent lines - rolling update
    for (int i = 1; i < n; i++) {
        // First item
        prevSquare = maxSquare[0];
        scanf("%d", &broken);
        if (broken) {
            maxSquare[0] = maxLeft[0] = maxTop[0] = 0;
        } else {
            maxSquare[0] = maxLeft[0] = 1;
            maxTop[0] += 1;
        }
        sum += maxSquare[0];

        // Subsequent items
        for (int j = 1; j < m; j++) {
            int temp = maxSquare[j]; // Later moving to prevSquare
            scanf("%d", &broken);
            if (broken) {
                maxSquare[j] = maxLeft[j] = maxTop[j] = 0;
            } else {
                maxLeft[j] = maxLeft[j - 1] + 1;
                maxTop[j] += 1;

                int min = INT_MAX;
                if (min > prevSquare + 1) min = prevSquare + 1;
                if (min > maxLeft[j]) min = maxLeft[j];
                if (min > maxTop[j]) min = maxTop[j];
                maxSquare[j] = min;
            }
            prevSquare = temp;
            sum += maxSquare[j];
        }
    }
    printf("%d\n", sum);

    free(maxSquare);
    free(maxLeft);
    free(maxTop);
    return 0;
}
