#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** createMatrix(int n) {
    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }
    return matrix;
}

void printMatrix(int **m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", m[i][j]);
        }
        printf("\n");
    }
}

void zeroMatrix(int **m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m[i][j] = 0;
        }
    }
}


void fillRandom(int **m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m[i][j] = rand() % 100;
        }
    }
}

void changeElement(int **m, int n) {
    int row, col, val;
    printf("Введите строку и столбец (начиная с 1): ");
    scanf("%d %d", &row, &col);
    if (row < 1 || row > n || col < 1 || col > n) {
        printf("Неверные координаты\n");
        return;
    }
    printf("Введите новое значение: ");
    scanf("%d", &val);
    m[row-1][col-1] = val;
}

void rotateClockwise(int **m, int n) {
    int temp[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[j][n - 1 - i] = m[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m[i][j] = temp[i][j];
        }
        //free(temp[i]);
    }
    //free(temp);
}

void rotateCounterClockwise(int **m, int n) {
    int **temp = createMatrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[n - 1 - j][i] = m[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m[i][j] = temp[i][j];
        }
        free(temp[i]);
    }
    free(temp);
}

void rotateMatrix(int **m, int n) {
    int dir;
    printf("Выберите направление (1 - по часовой, 2 - против): ");
    scanf("%d", &dir);
    if (dir == 1) {
        rotateClockwise(m, n);
    } else if (dir == 2) {
        rotateCounterClockwise(m, n);
    } else {
        printf("Ошибка выбора\n");
    }
}

void transposeMatrix(int **m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            int temp = m[i][j];
            m[i][j] = m[j][i];
            m[j][i] = temp;
        }
    }
}

int main() {
    int N;
    do {
        printf("Введите размер матрицы N: ");
        scanf("%d", &N);
    } while (N <= 0);

    int **matrix = createMatrix(N);
    zeroMatrix(matrix, N);
    srand(time(NULL));

    int choice;
    do {
        printf("\nМеню:\n");
        printf("1. Вывести матрицу\n");
        printf("2. Обнулить матрицу\n");
        printf("3. Заполнить случайными числами\n");
        printf("4. Изменить элемент\n");
        printf("5. Повернуть матрицу\n");
        printf("6. Транспонировать матрицу\n");
        printf("7. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printMatrix(matrix, N);
                break;
            case 2:
                zeroMatrix(matrix, N);
                break;
            case 3:
                fillRandom(matrix, N);
                break;
            case 4:
                changeElement(matrix, N);
                break;
            case 5:
                rotateMatrix(matrix, N);
                break;
            case 6:
                transposeMatrix(matrix, N);
                break;
            case 7:
                printf("Завершение работы\n");
                break;
            default:
                printf("Неверный выбор\n");
        }
    } while (choice != 7);
    
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
