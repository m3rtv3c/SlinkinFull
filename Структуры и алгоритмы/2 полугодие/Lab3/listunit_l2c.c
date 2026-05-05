#include <stdio.h>
#include <stdlib.h>
#include "listunit_l2c.h"

int g_count;
double g_minmax;
int g_min;


// создает узел и возвращает его
 pnodeL2C createNodeL2C(double data){
	pnodeL2C p = (pnodeL2C)malloc(sizeof(tnodeL2C));
	if(!p) return NULL;
	p->data = data; // просто записываем значение в узел
	p->pprev = NULL; // при инициализации - пусто
	p->pnext = NULL;
	return p;
		
}

// вставляет узел в начало списка и возвращает вставленный узел 
pnodeL2C addFirstNodeL2C(pnodeL2C *ph, pnodeL2C p) {
    if (!p) return NULL;
    if (!*ph) {
        p->pprev = p->pnext = p;
        *ph = p; // объявляем голову
    } else {
        pnodeL2C tail = (*ph)->pprev; //берем хвост
        p->pnext = *ph; // следующий элемент вставляемого узла указывает на голову
        p->pprev = tail; // предыдущий элемент вставляемого узла указывает на хвост, ведь список закольцован
        tail->pnext = p; // указатель на следующий элемент хвоста становится наш вставляемый узел
        (*ph)->pprev = p; // указатель на предыдущий элемент текущей головы становится вставляемый узел
        *ph = p; // меняем голову на вставляемый узел
    }
    return p;
}

// вставляет узел в конец списка и возвращает вставленный узел 
 pnodeL2C addLastNodeL2C(pnodeL2C *ph, pnodeL2C p){
	addFirstNodeL2C(ph, p); // вставка в начало
    *ph = (*ph)->pnext; //пододвигаем на 1 элемент, а значит вставленный узел, становится в конец
    return p;
	}
	
// вставляет узел p в список после узла pn и возвращает вставленный узел 
pnodeL2C insertAfterNodeL2C(pnodeL2C pn, pnodeL2C p){
    if(!p || !pn) return NULL;
    p->pnext = pn->pnext; // указатель на следующий элемент становится таким же как у элемента после которого мы вставляем
    p->pprev = pn; // указатель на предыдущий элемент вставляемого узла становится равен узлу после коготорого мы вставляем
    pn->pnext->pprev = p;  // меняем указатель на преыдущий элемент следующего узла на вставляемый
    pn->pnext = p; // меняем следующий элемент pn на p
    return p;
}
// вставляет узел p в список перед узлом pn и возвращает вставленный узел 
pnodeL2C insertBeforeNodeL2C(pnodeL2C pn, pnodeL2C p)
{
    if (!pn || !p) return NULL;
    p->pnext = pn; // следующий элемент вставляемого узла становится узел перед которым вставляем
    p->pprev = pn->pprev; //предыдущий элемент вставляемого узла становится таким же что и у узла перед которым вставляем
    pn->pprev->pnext = p; // предыдущим элементом следующего узла становится вставляемый узел
    pn->pprev = p; //предыдущим элементом узла перед которым вставляем указываем вставляемый узел
	
    return p;
}

// удаляет узел из списка и возвращает удаленный узел
 pnodeL2C deleteNodeL2C(pnodeL2C *ph, pnodeL2C pn){
	if(!ph || !*ph || !pn) return NULL;
    // проверка на единственный элемент(следующий элемент узла равен самому узлу, ведь список закольцован)
    if(pn->pnext == pn){
        *ph = NULL;
        pn->pnext = pn->pprev = NULL;
        return pn;
    }
    //делаем из pn.prev - p - p.next == pn.prev -><-pn.next
    pn->pprev->pnext = pn->pnext;
    pn->pnext->pprev = pn->pprev;
    //Если удаляем голову, то новой головой становится следующий элемент
    if(*ph == pn)
        *ph = pn->pnext;
        //Удаляем связи узла
    pn->pnext = pn->pprev = NULL;
    return pn;
	 }
// уничтожает узел и устанавливает его в NULL. 
 void disposeNodeL2C(pnodeL2C *pn){
    if (!pn || !*pn) return;
    free(*pn); //просто освобождаем узел
    *pn = NULL;
}
// уничтожает список и устанавливает его в NULL
void disposeListL2C(pnodeL2C *ph)
{
    if (!ph || !*ph) return;
    //Начинаем обход со следующего от головы элемента
    pnodeL2C p = (*ph)->pnext;
    while (p != *ph) {
        pnodeL2C tmp = p;
        p = p->pnext;
        free(tmp);
    }
    free(*ph);
    *ph = NULL;
}


// выполняет функцию func над каждым узлом списка
// если fwd!=0, перебор узлов осуществляется в прямом направлении, иначе - в обратном
// если func возвращает 0, listAction завершает перебор узлов
 typedef int (*listfunc)(double);
void listActionL2C(pnodeL2C ph, int fwd, listfunc func) {
    if (!ph || !func) return;
    pnodeL2C p = ph;
    do {
        if (!func(p->data)) break;
        p = fwd ? p->pnext : p->pprev;
    } while (p != ph);
}




// последующие функции должны использовать listActionL2C
// выводит список в стандартный поток вывода в прямом (fwd!=0) или обратном (fwd==0) порядке
int printDouble(double data) {
    printf("%.2lf ", data);
    return 1;
}
void listOutL2C(pnodeL2C ph, int fwd) {
    if (!ph) {
        printf("(empty)\n");
        return;
    }

    if (fwd) {
        // Прямой порядок: от головы вперед
        listActionL2C(ph, 1, printDouble);
    } else {
        // Обратный порядок: от ХВОСТА назад
        listActionL2C(ph->pprev, 0, printDouble);
    }
    printf("\n");
}



// возвращает количество элементов в списке
int countFunc(double data) {
    (void)data;
    g_count++;
    return 1;
}

int listCountL2C(pnodeL2C ph){
    if (!ph) return 0;
    g_count = 0;
    listActionL2C(ph, 1, countFunc);
    return g_count;
}
// возвращает минимальный (min!=0) или максимальный (min==0) элемент в списке
int minmaxFunc(double data){
    if (g_min) {
        if (data < g_minmax) g_minmax = data;
    } else {
        if (data > g_minmax) g_minmax = data;
    }
    //Возвращаем 1 чтобы listFunc продолжал перебор
    return 1;
}
double minmaxL2C(pnodeL2C ph, int min){
    if (!ph) return 0.0;

    g_min = min;
    g_minmax = ph->data;

    listActionL2C(ph->pnext, 1, minmaxFunc);
    return g_minmax;
}
// возвращает первый (first!=0) или последний (first==0) элемент в списке, больший (above!=0)
// или меньший (above==0) data
pnodeL2C abNodeL2C(pnodeL2C ph, int first, int above, double data) {
    if (!ph) return NULL;

    pnodeL2C p; 
    pnodeL2C res = NULL;

    if (first) { // first !=0
        // Идём вперёд, ищем первый подходящий, начинаем с головы
        p = ph; // Мы ведь уже указали голову, просто while работать не будет!
		do {
            if ((above && p->data > data) || (!above && p->data < data)) { //если above !=0, то ищем больше. above == 0, то ищем элементы меньше
                return p; 
            }
            p = p->pnext; // следующий узел
        } while (p!=ph); // идем пока не достигнем головы
    } else { //first == 0
        // Идём вперёд, ищем последний подходящий, начинаем также с головы
        p = ph;
        do {
            if ((above && p->data > data) || (!above && p->data < data)) { 
                res = p; // запоминаем каждый подходящий
            }
            p = p->pnext;
        } while (p != ph);
        return res; // возвращаем последний найденный
    }

    return NULL; // если ничего не найдено
}

