#include <stdio.h>
#include <stdlib.h>

#define NV 4039
#define NE 88234

int *A, *IA, *JA;  // For compressed sparse row
size_t count;
const char *filename = "facebook_combined.txt";

int main() {
    FILE *fp = fopen(filename, "r");
    // No edge weight, so no need for A
    //A = malloc(NE * sizeof(int));
    A = NULL;
    IA = malloc((1 + NV) * sizeof(int));
    JA = malloc(NE * sizeof(int));

    // Taking advantage of sorted input
    int thisV = 0;
    IA[0] = 0;
    for (int i = 0; i < NE; i++) {
        int a, b;
        fscanf(fp, "%d %d", &a, &b);
        while (a != thisV) {
            thisV++;
            IA[thisV] = i;
        }
        JA[i] = b;
    }

    fclose(fp);
    return 0;
}
