#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

char a[SIZE], b[SIZE], q[SIZE];
char *pa = a - 1, *pb = b - 1;
int na, nb, s[SIZE][SIZE];
int Max(int a, int b) {
    return a > b ? a : b;
}

int LCS(){
	int i,j;
	char *a = pa, *b = pb; // To screen the global arrays
	for (i = 0; i < SIZE; i++)
        s[0][i] = s[i][0] = 0;
	for (i = 1; i <= na; i++) {
		for (j = 1; j <= nb; j++) {
			if (a[i] == b[j]) {
                s[i][j] = s[i - 1][j - 1] + 1;
            } else {
                s[i][j] = Max(s[i - 1][j], s[i][j - 1]);
            }
		}
	}
	i = na;
	j = nb;
	int n = 0;
	while (i > 0 && j > 0) {
		if ((i == 1 || j == 1) && s[i][j] > 0) {
			q[n] = a[i];
			n++;
			break;
		} else {
			if (a[i] == b[j]) {
				q[n] = a[i];
				n++; i--; j--;
			} else {
				if (s[i - 1][j] >= s[i][j - 1])
                    i--;
				else
                    j--;
			}
		}
	}
	return n;
}

int main(){
	scanf("%d%d", &na, &nb);
	if(na > SIZE || nb > SIZE)
        return 1;
	scanf(" %s",a);
	scanf(" %s",b);
	int i = LCS();
	printf("%d\n", i);
	for (i--; i >= 0; i--)
        putchar(q[i]);
	putchar('\n');
	return 0;
}

