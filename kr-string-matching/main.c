#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BASE 128L
#define MOD 1000000007LL
#define SIZE (1U << 16)

void match(const char *pattern, const char *str) {
    const size_t pLen = strlen(pattern),
                 sLen = strlen(str);
    if (pLen == 0 || pLen > sLen)  // Pardon?
        return;

    int64_t pHash = 0,
            sHash = 0,
            hardMod = 1;
    for (size_t i = 0; i < pLen - 1; i++) {
        hardMod = (hardMod * BASE) % MOD;
    }

    for (size_t i = 0; i < pLen; i++) {
        pHash = (pHash * BASE + pattern[i]) % MOD;
        sHash = (sHash * BASE + str[i]) % MOD;
    }

    // Try matching the hash first
    for (size_t i = 0; i <= sLen - pLen; i++) {
        if (sHash == pHash) {
            // Hash match, now examine whole string
            int match = 1;
            for (size_t j = 0; j < pLen; j++) {
                if (pattern[j] != str[i + j]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                printf("Found match at position %ld\n", i);
            }
        }

        if (i < sLen - pLen) {
            sHash = (sHash - str[i] * hardMod) * BASE + str[i + pLen];
            sHash %= MOD;
            if (sHash < 0)
                sHash += MOD;
        }
    }
}

int main() {
    char *pattern = malloc(SIZE),
         *str = malloc(SIZE);
    fgets(pattern, SIZE, stdin);
    fgets(str, SIZE, stdin);
    if (pattern[strlen(pattern) - 1] == '\n')
        pattern[strlen(pattern) - 1] = 0;
    if (str[strlen(str) - 1] == '\n')
        str[strlen(str) - 1] = 0;
    match(pattern, str);
    return 0;
}
