#include <stdio.h>
#include "listunit_l2c.c"

/* функция для проверки listActionL2C */
int printNode(double x) {
    printf("%.1lf ", x);
    return 1;
}

int main(void) {
    pnodeL2C head = NULL;
    pnodeL2C p;

    printf("========== ТЕСТ 1: addFirst ==========\n");
    addFirstNodeL2C(&head, createNodeL2C(1));
    addFirstNodeL2C(&head, createNodeL2C(2));
    addFirstNodeL2C(&head, createNodeL2C(3));

    // Вывод в обратном порядке
    printf("Backward: ");
    listOutL2C(head, 0); // Вывод задом наперед

    // Вывод по порядку
    printf("Forward:  ");
    listOutL2C(head, 1); // Вывод по порядку

    printf("\n========== ТЕСТ 2: addLast ==========\n");
    addLastNodeL2C(&head, createNodeL2C(10));
    addLastNodeL2C(&head, createNodeL2C(20));

    // Вывод в обратном порядке
    printf("Backward: ");
    listOutL2C(head, 0); // Вывод задом наперед

    // Вывод по порядку
    printf("Forward:  ");
    listOutL2C(head, 1); // Вывод по порядку

    printf("\n========== ТЕСТ 3: insertAfter ==========\n");
    insertAfterNodeL2C(head, createNodeL2C(99));

    // Вывод в обратном порядке
    printf("Backward: ");
    listOutL2C(head, 0); // Вывод задом наперед

    // Вывод по порядку
    printf("Forward:  ");
    listOutL2C(head, 1); // Вывод по порядку

    printf("\n========== ТЕСТ 4: insertBefore ==========\n");
    pnodeL2C n = createNodeL2C(77);
    insertBeforeNodeL2C(head, n);
    //head = n;                     

    // Вывод в обратном порядке
    printf("Backward: ");
    listOutL2C(head, 0);   // Вывод задом наперед

    // Вывод по порядку
    printf("Forward:  ");
    listOutL2C(head, 1);   // Вывод по порядку

    printf("\n========== ТЕСТ 5: listAction ==========\n");

    // Вывод в обратном порядке
    printf("Backward: ");
    listActionL2C(head, 0, printNode);  // Вывод задом наперед
    printf("\n");

    // Вывод по порядку
    printf("Forward:  ");
    listActionL2C(head, 1, printNode);  // Вывод по порядку
    printf("\n");

    printf("\n========== ТЕСТ 6: listCount ==========\n");
    printf("Count = %d\n", listCountL2C(head));

    printf("\n========== ТЕСТ 7: min / max ==========\n");
    printf("Min = %.1lf\n", minmaxL2C(head, 1));
    printf("Max = %.1lf\n", minmaxL2C(head, 0));

    printf("\n========== ТЕСТ 8: abNode ==========\n");
    p = abNodeL2C(head, 1, 1, 5);   // первый > 5
    if (p)
        printf("First > 5: %.1lf\n", p->data);

    p = abNodeL2C(head, 0, 0, 50);  // последний < 50
    if (p)
        printf("Last < 50: %.1lf\n", p->data);

    printf("\n========== ТЕСТ 9: delete head ==========\n");
    p = deleteNodeL2C(&head, head);
    disposeNodeL2C(&p);

    // Вывод в обратном порядке
    printf("Backward: ");
    listOutL2C(head, 0); // Вывод задом наперед

    // Вывод по порядку
    printf("Forward:  ");
    listOutL2C(head, 1); // Вывод по порядку

    printf("\n========== ТЕСТ 10: delete middle ==========\n");
    p = deleteNodeL2C(&head, head->pnext);
    disposeNodeL2C(&p);

    // Вывод в обратном порядке
    printf("Backward: ");
    listOutL2C(head, 0); // Вывод задом наперед

    // Вывод по порядку
    printf("Forward:  ");
    listOutL2C(head, 1); // Вывод по порядку

    printf("\n========== ТЕСТ 11: single element ==========\n");
    disposeListL2C(&head);
    addFirstNodeL2C(&head, createNodeL2C(42));

    // Вывод в обратном порядке
    printf("Backward: ");
    listOutL2C(head, 0); // Вывод задом наперед

    // Вывод по порядку
    printf("Forward:  ");
    listOutL2C(head, 1); // Вывод по порядку

    p = deleteNodeL2C(&head, head);
    disposeNodeL2C(&p);
    printf("после удаления, head = %p\n", (void*)head);

    printf("\n========== ТЕСТ 12: empty list ==========\n");

    // Вывод в обратном порядке
    printf("Backward: ");
    listOutL2C(head, 0); // Вывод задом наперед

    // Вывод по порядку
    printf("Forward:  ");
    listOutL2C(head, 1); // Вывод по порядку
    printf("Count = %d\n", listCountL2C(head));

    printf("\n========== ТЕСТ 13: disposeList ==========\n");
    disposeListL2C(&head);
    if (!head)
        printf("List disposed successfully\n");

    return 0;
}
