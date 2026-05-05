// заголовочный файл функций поиска
#ifndef FINDUNIT
#define FINDUNIT

/*
 общее количество итераций поиска,
 обнуляется и заполняется каждой функцией поиска
*/  
int find_count;

/*
 Тип тест-функции
 Для линейного поиска должна возвращать 0 или 1
 (0 - переданный элемент НЕ соответствует критериям поиска, 1 - соответствует)
 Для бинарного поиска должна возвращать 0, 1 или -1
 (0 - переданный элемент соответствует критериям поиска; 
  1,-1 - больше/меньше искомого элемента или наоборот в зависимости от решения программиста)
*/

 typedef int (*testfunc)(int);

/*
 Линейный поиск одного элемента в целочисленном массиве
 возвращает индекс найденного значения или -1
 Параметры:
 src - исходный массив
 src_size - кол-во элементов в src
 func - тест-функция
*/
int line_find_one(const int src[], int src_size, testfunc func) {
    find_count = 0; 
    for (int i = 0; i < src_size; i++) {
        find_count++;
        if (func(src[i]) == 1) {
            return i;
        }
    }
    return -1;
}

/*
 Линейный поиск всех элементов в целочисленном массиве
 возвращает количество найденных элементов или 0
 Параметры:
 src - исходный массив
 src_size - кол-во элементов в src
 func - тест-функция
 result - массив найденных индексов
 result_maxsize - максимальное кол-во элементов в result
*/  
int line_find_all(const int src[], int src_size, testfunc func, int result[], int result_maxsize) {
    find_count = 0; 
    int found = 0;
    for (int i = 0; i < src_size && found != result_maxsize; i++) {
        find_count++;
        if (func(src[i]) == 1) {
            result[found++] = i;
        }
    }
    return found; 
}

/*
 Бинарный поиск одного элемента в целочисленном массиве
 возвращает индекс найденного значения или -1
 Параметры:
 src - исходный массив
 src_size - кол-во элементов в src
 func - тест-функция
*/  
int bin_find_one(const int src[], int src_size, testfunc func) {
    find_count = 0;
    int left = 0, right = src_size - 1;
    while (left <= right) {
        find_count++;
        int mid = left + (right - left) / 2;
        int cmp = func(src[mid]);
        if (cmp == 0)
            return mid;
        if (cmp < 0)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return -1;
}

/*
 Бинарный поиск всех элементов в целочисленном массиве
 возвращает количество найденных элементов или 0
 Параметры:
 src - исходный массив
 src_size - кол-во элементов в src
 func - тест-функция
 res_beg - адрес переменной, куда будет сохранен индекс первого найденного элемента
 res_end - адрес переменной, куда будет сохранен индекс последнего найденного элемента
*/  
int bin_find_all(const int src[], int src_size, testfunc func, int *res_beg, int *res_end) {
    find_count = 0;
    int first = -1, last = -1;
    int left = 0, right = src_size - 1;
    while (left <= right) {
        find_count++;
        int mid = left + (right - left) / 2;
        int cmp = func(src[mid]);
        if (cmp == 0) {
            first = mid - 1;
            while(first >= 0 && func(src[first]) == 0){
				first--;
				}
			first++;
			last = mid+1;
			while(last < src_size && func(src[last]) == 0){
				last++;
				}
			last--;
			*res_beg=first;
			*res_end=last;
			return (last-first)+1;
        } else if (cmp < 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
        *res_beg = *res_end = -1;
        return 0;
}

#endif // FINDUNIT
