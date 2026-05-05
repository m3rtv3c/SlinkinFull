#include <stdio.h>
#include <math.h>
int main() {
    int divMaxSum = 0;
    int divMaxNumber = 0;
    int m;
    int n;
    printf("Введите m\n");
    scanf("%d",&m);
    printf("Введите n\n");
    scanf("%d",&n);
    for (int i = m; i <= n; i++) {
        int divSum = 0;
        for (int j = 1; j <= sqrt(i); j++) {
            if (i % j == 0) {
                if(j!=i/j){
					divSum = divSum + j + (i/j);
					}
				else
					divSum +=j;
            }
        }
        if (divSum > divMaxSum) {
            divMaxSum = divSum;
            divMaxNumber = i;
        }
    }
    printf("Максимальное число: %d - Максимальная сумма: %d\n", divMaxNumber, divMaxSum);
    return 0;
}

