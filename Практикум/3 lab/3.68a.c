#include <stdio.h>

#define IS_PALINDROME(n) (((n) / 1000) == ((n) % 10) && (((n) / 100) % 10) == (((n) / 10) % 10))


int is_palindrome(int n) {
    int first = n / 1000;
    int second = (n / 100) % 10;
    int third = (n / 10) % 10;
    int fourth = n % 10;
    return (first == fourth) && (second == third);
}

int main() {
    int n;
    printf("Введите четырехзначное число: ");
    scanf("%d", &n);
    printf("-------------------------\n");
	printf("Проверка решения с помощью макроса:\n");
    if (IS_PALINDROME(n)) {
        printf("Число %d является палиндромом.\n", n);
    } else {
        printf("Число %d не палиндром\n", n);
    }
    printf("-------------------------\n");
    printf("Проверка с помощью функции:\n");
    if(is_palindrome(n)){
		printf("Число %d является палиндромом\n", n);
	} else {
		printf("Число %d не является палиндромом\n", n);
	}
    

    return 0;
}
