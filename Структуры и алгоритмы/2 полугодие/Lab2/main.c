#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "listunit_l1.h"



// Функция для listActionL1: вывод строки в верхнем регистре
int printUpper(char *s) {
    if (!s) return 0;
    for (int i = 0; s[i]; i++)
        putchar(toupper((unsigned char)s[i]));
    printf(" | ");
    return 1; 
}

int main() {
    pnodeL1 head = NULL; 

    printf("--- Создание и добавление узлов ---\n");
    addFirstNodeL1(&head, createNodeL1("Node 1")); 
    addLastNodeL1(&head, createNodeL1("Node 2"));  
    insertAfterNodeL1(head, createNodeL1("Node 3"));
    listOutL1(head);

    printf("\n--- Проверка listCountL1 ---\n");
    printf("Количество элементов: %d\n", listCountL1(head));

    printf("\n--- Проверка listSumStr ---\n");
    char buffer[5];
    if (listSumStr(buffer, sizeof(buffer), head, " +-* "))
        printf("Объединенные строки: %s\n", buffer);

    printf("\n--- Проверка listActionL1 (печать в верхнем регистре) ---\n");
    listActionL1(head, printUpper);
    printf("\n");

    printf("\n--- Проверка deleteAfterNodeL1 ---\n");
    pnodeL1 deleted = deleteAfterNodeL1(head);
    if (deleted) {
        printf("Удаленный узел: %s\n", deleted->data);
        disposeNodeL1(&deleted);
    }
    listOutL1(head);

    printf("\n--- Проверка disposeAfterNodeL1 ---\n");
    disposeAfterNodeL1(head); 
    listOutL1(head);

    printf("\n--- Проверка disposeNodeL1 ---\n");
    disposeNodeL1(&head); 
    listOutL1(head); 

    printf("\n--- Создание нового списка для disposeListL1 ---\n");
    addFirstNodeL1(&head, createNodeL1("A"));
    addLastNodeL1(&head, createNodeL1("B"));
    addLastNodeL1(&head, createNodeL1("C"));
    listOutL1(head);

    printf("\n--- Проверка disposeListL1 ---	\n");
    disposeListL1(&head);
    listOutL1(head); 

    return 0;
}
