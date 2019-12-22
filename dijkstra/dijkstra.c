#include <stdio.h>
#include <limits.h>
#define LEFT(x) (2 * (x) + 1)
#define RIGHT(x) (2 * (x) + 2)
#define PARENT(x) (((x) - 1) / 2)

typedef struct _Edge {
    int u, v, w;
} Edge;

int N, M, S, T;
Edge E[1000000];
int D[100000], Ei[100001]; // Ei = Edge index

// Heap
int H[100000], Hi[100000], Hn;

void sort_E(int l, int r) {
    if (l + 1 >= r)
        return;
    int i = l, j = r - 1, f = 0;
    while (i < j) {
        if (E[i].u > E[j].u) {
            Edge t = E[i];
            E[i] = E[j];
            E[j] = t;
            f = !f;
        }
        f ? j-- : i++;
    }
    sort_E(l, i);
    sort_E(i + 1, r);
}

int extract_min() {
    int heap_min = H[0];
    Hi[heap_min] = -1;
    int key = H[0] = H[--Hn];
    int n = 0, smaller;
    while (n < Hn) {
        smaller = LEFT(n);
        if (RIGHT(n) < Hn && H[RIGHT(n)] < H[LEFT(n)])
            smaller = RIGHT(n);
        if (smaller >= Hn)
            break;
        H[n] = H[smaller];
        Hi[H[n]] = n;
        n = smaller;
    }
    H[n] = key;
    Hi[key] = n;
    return heap_min;
}

void update_D(int node, int value) {
    if ((D[node] <= value && D[node] > 0) || node == S)
        return; // No need to update
    D[node] = value;
    int n = Hi[node];
    if (n > 0 || (n == 0 && H[0] == node)) {
        // Already in heap
    } else {
        // Append to heap
        H[Hn] = INT_MAX;
        n = Hn++;
    }
    // Decrease key
    while (n > 0) {
        int p = PARENT(n);
        if (H[p] < H[n])
            break;
        H[n] = H[p];
        Hi[H[n]] = n;
        n = p;
    }
    Hi[node] = n;
    H[n] = node;
}

int main() {
    scanf("%d%d%d%d", &N, &M, &S, &T);
    if (S-- == T--) {
        printf("%d\n", 0);
        return 0;
    }
    for (int i = 0; i < M; i++) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        E[i].u = u - 1;
        E[i].v = v - 1;
        E[i].w = w;
    }
    sort_E(0, M);
    Ei[0] = 0;
    Ei[N] = M;
    int current = 0;
    for (int i = 0; i < M; i++)
        while (current < E[i].u)
            Ei[++current] = i;

    // Dijkstra
    H[0] = S;
    Hi[S] = 0;
    Hn = 1;
    D[S] = 0;

    while (Hn > 0) {
        int node = extract_min();
        for (int i = Ei[node]; i < Ei[node + 1]; i++) {
            update_D(E[i].v, D[node] + E[i].w);
        }
    }

    if (D[T] == 0) {
        printf("%d\n", -1);
    } else {
        printf("%d\n", D[T]);
    }
    return 0;
}
