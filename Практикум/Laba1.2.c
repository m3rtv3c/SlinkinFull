#include <stdio.h>
#include <limits.h>

void perform_operations_signed(signed char X, signed char Y) {
    unsigned int stepsAdd = 0, stepsSub = 0;
    signed char X_signed = X;

    while (1) {
        signed char temp = X_signed + Y;
        if ((Y > 0 && temp < X_signed) || (Y < 0 && temp > X_signed)) {
            stepsAdd++;
            break;
        }
        X_signed = temp;
        stepsAdd++;
    }

    X_signed = X;

    while (1) {
        signed char temp = X_signed - Y;
        if ((Y > 0 && temp > X_signed) || (Y < 0 && temp < X_signed)) {
            stepsSub++;
            break;
        }
        X_signed = temp;
        stepsSub++;
    }

    printf("Для signed char (%hhd, %hhd) требуется %u операций для сложения\n", X, Y, stepsAdd);
    printf("Для signed char (%hhd, %hhd) требуется %u операций для вычитания\n", X, Y, stepsSub);
}

void perform_operations_unsigned(unsigned char X, unsigned char Y) {
    unsigned int stepsAdd = 0, stepsSub = 0;
    unsigned char X_unsigned = X;

    while (1) {
        unsigned char temp = X_unsigned + Y;
        if (temp < X_unsigned) {
            stepsAdd++;
            break;
        }
        X_unsigned = temp;
        stepsAdd++;
    }

    X_unsigned = X;

    while (1) {
        unsigned char temp = X_unsigned - Y;
        if (temp > X_unsigned) {
            stepsSub++;
            break;
        }
        X_unsigned = temp;
        stepsSub++;
    }

    printf("Для unsigned char (%hhu, %hhu) требуется %u операций для сложения\n", X, Y, stepsAdd);
    printf("Для unsigned char (%hhu, %hhu) требуется %u операций для вычитания\n", X, Y, stepsSub);
}

int main() {
    signed char X_signed, Y_signed;
    unsigned char X_unsigned, Y_unsigned;

    printf("Введите значение X (signed char): ");
    scanf("%hhd", &X_signed);
    printf("Введите значение Y (signed char): ");
    scanf("%hhd", &Y_signed);
    perform_operations_signed(X_signed, Y_signed);

    printf("Введите значение X (unsigned char): ");
    scanf("%hhu", &X_unsigned);
    printf("Введите значение Y (unsigned char): ");
    scanf("%hhu", &Y_unsigned);
    perform_operations_unsigned(X_unsigned, Y_unsigned);

    return 0;
}
