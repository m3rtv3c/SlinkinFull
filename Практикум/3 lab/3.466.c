#include <stdio.h>
#include <string.h>
#define SWAP


void swap(char *str, int startPos) {
    int length = strlen(str);
    if (startPos < 0 || startPos >= length) {
        printf("Неверная начальная позиция.\n");
        return;
    }
    for (int i = startPos; i < length; i++) {
        if (str[i] == '0') {
            str[i] = '1';
        } else if (str[i] == '1') {
            str[i] = '0';
        }
    }
}

int main() {
    char str[100];
    int startPos;
    printf("Введите строку (состоящую из 0 и 1): ");
    scanf("%s", str);
    printf("Введите начальную позицию: ");
    scanf("%d", &startPos);
    swap(str, startPos);
    printf("Измененная строка: %s\n", str);
    
    return 0;
}
