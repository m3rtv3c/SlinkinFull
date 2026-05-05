#include <stdio.h>
#include <stdbool.h>

int main() {
    long int i, j, n;
    bool fl;
    for (n = 1; n <= 1000000; n++) {
        fl = false;
        for (i = 1; i < n; i++) {
            for (j = i+1; j <= n; j++) {
                if (i * i * i + j * j * j == n) {
                    if (fl) {
                        printf("%ld = %ld^3 + %ld^3\n", n, i, j);
                        return 0;
                    } else {
                        printf("%ld = %ld^3 + %ld^3\n", n, i, j);
                        fl=true;
                    }
                }
            }
        }
    }
    return 0;
}

