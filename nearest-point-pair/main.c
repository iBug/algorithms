#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _Point {
    double x;
    double y;
    int xi;  // Index when sorted by X
    int yi;  // Index when sorted by Y
} Point;

typedef struct _DistInfo {
    double value;
    int a, b;
} DistInfo;

// Raw array, sorted by X, sorted by Y
Point *p, **px, **py;
int n;

void sort_p(int l, int r, int x_y) {
    if (l + 1 >= r)
        return;

    int i = l, j = r - 1, flag = 0;
    Point *t;
    while (i < j) {
        if (x_y == 0) {  // Sort by X
            if (px[i]->x > px[j]->x) {
                t = px[i];
                px[i] = px[j];
                px[j] = t;
                flag = !flag;
            }
        } else {  // Sort by Y
            if (py[i]->y > py[j]->y) {
                t = py[i];
                py[i] = py[j];
                py[j] = t;
                flag = !flag;
            }
        }
        flag ? i++ : j--;
    }

    sort_p(l, i, x_y);
    sort_p(i + 1, r, x_y);
}

double distance(Point *a, Point *b) {
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

void find(DistInfo* result, int l, int r) {
    if (l + 1 >= r) {
        result->value = INFINITY;
        return;
    }
    if (l + 2 == r) {
        result->value = distance(px[l], px[l + 1]);
        result->a = px[l] - p;
        result->b = px[l + 1] - p;
    }

    DistInfo lresult, rresult;
    int mid = (l + r) / 2;
    find(&lresult, l, mid);
    find(&rresult, mid, r);

    if (lresult.value < rresult.value) {
        result->value = lresult.value;
        result->a = lresult.a;
        result->b = lresult.b;
    } else {
        result->value = rresult.value;
        result->a = rresult.a;
        result->b = rresult.b;
    }
    double delta = result->value;

    // Find the cross-region minimum distance
    // Enumerate every point where (mid.x-d <= p.x <= mid.x)
    for (int i = mid - 1; i >= l; i--) {
        if (px[i]->x < px[mid]->x - result->value)
            // This one's done
            break;
        int c, j;

        // Search both up and down for up to 4 points each
        for (c = 4, j = px[i]->yi; c > 0; j++) {
            if (j >= n || py[j]->y > px[i]->y + delta)  // Gone too far
                break;
            if (py[j]->xi < mid || py[j]->xi >= r)  // Index out of range
                continue;
            if (py[j]->x > px[mid]->x + delta)  // Not in the area we want
                continue;
            c--;
            double d = distance(px[i], py[j]);
            if (d < result->value) {
                result->value = d;
                result->a = i;
                result->b = py[j]->xi;
            }
        }

        for (c = 4, j = px[i]->yi; c > 0; j--) {
            if (j < 0 || py[j]->y < px[i]->y - delta)  // Gone too far
                break;
            if (py[j]->xi < mid || py[j]->xi >= r)  // Index out of range
                continue;
            if (py[j]->x > px[mid]->x + delta)  // Not in the area we want
                continue;
            c--;
            double d = distance(px[i], py[j]);
            if (d < result->value) {
                result->value = d;
                result->a = i;
                result->b = py[j]->xi;
            }
        }
    }
}

int main() {
    scanf("%d", &n);
    p = malloc(n * sizeof *p);
    px = malloc(2 * n * sizeof *px);
    py = px + n;
    for (int i = 0; i < n; i++) {
        scanf("%lf%lf", &p[i].x, &p[i].y);
        px[i] = py[i] = p + i;
    }

    // Sort by X and Y
    sort_p(0, n, 0);
    sort_p(0, n, 1);
    for (int i = 0; i < n; i++)
        px[i]->xi = py[i]->yi = i;

    DistInfo result;
    find(&result, 0, n);
    printf("%lf\n", result.value);
    printf("(%lf, %lf) <=> (%lf, %lf)\n",
           px[result.a]->x, px[result.a]->y,
           px[result.b]->x, px[result.b]->y);

    free(p);
    free(px);
    return 0;
}
