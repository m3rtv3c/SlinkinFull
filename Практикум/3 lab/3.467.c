#include <stdio.h>
#include <string.h>

void removeChars(char *str1, const char *str2) {
    int i, j, k;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    for (i = 0, k = 0; i < len1; i++) {
        int found = 0;
        for (j = 0; j < len2; j++) {
            if (str1[i] == str2[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            str1[k++] = str1[i];
        }
    }
    str1[k] = '\0';
}

int main() {
    char str1[100], str2[100];
    printf("Введите первую строку: ");
    fgets(str1, sizeof(str1), stdin);
    str1[strcspn(str1, "\n")] = 0;
    printf("Введите вторую строку (символы для удаления): ");
    fgets(str2, sizeof(str2), stdin);
    str2[strcspn(str2, "\n")] = 0;
    removeChars(str1, str2);
    printf("Результирующая строка: %s\n", str1);
    return 0;
}
