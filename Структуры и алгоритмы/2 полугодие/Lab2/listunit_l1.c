#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listunit_l1.h"

 // создает узел по копии переданной строки и возвращает его
pnodeL1 createNodeL1(char *data) {
    if (!data) return NULL;
    pnodeL1 p = (pnodeL1)malloc(sizeof(tnodeL1));
    if (!p) return NULL;
    p->data = (char*)malloc(strlen(data) + 1);
    if (!p->data) 
    { 
		free(p); return NULL; 
	}
    strcpy(p->data, data);
    p->pnext = NULL;
    return p;
}

 // вставляет узел в начало списка и возвращает вставленный узел 
pnodeL1 addFirstNodeL1(pnodeL1 *ph, pnodeL1 p) {
    if (!p || !ph) return NULL;
    p->pnext = *ph;
    *ph = p;
    return p;
}
 // вставляет узел в конец списка и возвращает вставленный узел 
pnodeL1 addLastNodeL1(pnodeL1 *ph, pnodeL1 p) {
    if (!p || !ph) return NULL;
    if (*ph == NULL) return addFirstNodeL1(ph, p);
    pnodeL1 q = *ph;
    while (q->pnext) 
    q = q->pnext;
    q->pnext = p;
    p->pnext = NULL;
    return p;
}

 // вставляет узел в список после переданного узла и возвращает вставленный узел 
pnodeL1 insertAfterNodeL1(pnodeL1 pn, pnodeL1 p) {
    if (!p) return NULL;
    if (!pn) return p; 
    p->pnext = pn->pnext;
    pn->pnext = p;
    return p;
}

 // уничтожает узел и устанавливает его в NULL
void disposeNodeL1(pnodeL1 *pn) {
    if (!pn || !*pn) return;
    free((*pn)->data);
    free(*pn);
    *pn = NULL;
}

 // удаляет узел из списка после переданного узла и ВОЗВРАЩАЕТ удаленный узел
pnodeL1 deleteAfterNodeL1(pnodeL1 pn) {
    if (!pn || !pn->pnext) return NULL;
    pnodeL1 temp = pn->pnext;
    pn->pnext = temp->pnext;
    temp->pnext = NULL;
    return temp;
}

 // удаляет узел из списка после переданного узла и УНИЧТОЖАЕТ удаленный узел
void disposeAfterNodeL1(pnodeL1 pn) {
    pnodeL1 temp = deleteAfterNodeL1(pn);
    if (temp) disposeNodeL1(&temp);
}

 // уничтожает список и устанавливает его в NULL
void disposeListL1(pnodeL1 *ph) {
    if (!ph) return;
    pnodeL1 cur = *ph;
    while (cur) {
        pnodeL1 next = cur->pnext;
        disposeNodeL1(&cur);
        cur = next;
    }
    *ph = NULL;
}

 // выполняет функцию func над каждым узлом списка
 // если func возвращает 0, listAction завершает перебор узлов списка  
void listActionL1(pnodeL1 ph, listfunc func) {
    if (!func) return;
    for (pnodeL1 p = ph; p; p = p->pnext) {
        if (!func(p->data)) break;
    }
}

 // выводит список в стандартный поток вывода
void listOutL1(pnodeL1 ph) {
    for (pnodeL1 p = ph; p; p = p->pnext) {
        printf("%s -> ", p->data);
    }
    printf("NULL\n");
}

 // возвращает количество элементов в списке
int listCountL1(pnodeL1 ph) {
    int count = 0;
    for (pnodeL1 p = ph; p; p = p->pnext) count++;
    return count;
}

 // Объединяет данные элементов списка в строку с использованием разделителя
 // delimiter между отдельными данными.
 // Полученную строку заносит в dest и возвращает в качестве результата функции.
 // Длина результирующей строки не должна превышать maxsize.
 // Если maxsize==0 или dest==NULL, функция возвращает NULL
char *listSumStr(char *dest, int maxsize, pnodeL1 ph, char *delimiter) {
    if (!dest || maxsize <= 0) return NULL;
    dest[0] = '\0';

    int delim_len = (delimiter ? strlen(delimiter) : 0);
    int used = 0;

    for (pnodeL1 p = ph; p; p = p->pnext) {
        char *data = p->data;
        int len = strlen(data);
        //вычисляем оставшееся место
        int space_left = maxsize - used - 1;
        if (space_left <= 0) break;
        //Вычисляем сколько символов нужно
        int to_copy = (len < space_left) ? len : space_left;
        strncat(dest, data, to_copy);
        used += to_copy;
        
        if (to_copy < len) break;
        
        //добавляем разделитель только если есть следующий элемент и указан разделитель
        if (p->pnext && delimiter) {
            space_left = maxsize - used - 1;
            if (space_left <= 0) break;

            int delim_copy = (delim_len < space_left) ? delim_len : space_left;
            strncat(dest, delimiter, delim_copy);
            used += delim_copy;

            if (delim_copy < delim_len) break;
        }
    }
    dest[used] = '\0';
    return dest;
}
