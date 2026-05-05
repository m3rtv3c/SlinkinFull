// заголовочный файл функций поиска

#ifndef FINDUNIT
#define FINDUNIT

/*
 общее количество итераций поиска,
 обнуляется и заполняется каждой функцией поиска
*/  
int find_count = 0;

/*
 Тип тест-функции
 Для линейного поиска должна возвращать 0 или 1
 (0 - переданный элемент НЕ соответствует критериям поиска, 1 - соответствует)

 Для бинарного поиска должна возвращать 0, 1 или -1
 (0 - переданный элемент соответствует критериям поиска; 
  -1 - переданный элемент МЕНЬШЕ искомого критерия/элемента;
   1 - переданный элемент БОЛЬШЕ искомого критерия/элемента)
*/
typedef int (*testfunc)(int);

/*
 Линейный поиск одного элемента в целочисленном массиве
 возвращает индекс найденного значения или -1
 Параметры:
 src - исходный массив
 src_size - кол-во элементов в src
 func - тест-функция (возвращает 1 для соответствия, 0 для несоответствия)
*/
int line_find_one(const int src[], int src_size, testfunc func) {
    find_count = 0; 
    for (int i = 0; i < src_size; i++) {
        find_count++;
        // Проверка соответствия только через testfunc
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
 func - тест-функция (возвращает 1 для соответствия, 0 для несоответствия)
 result - массив найденных индексов
 result_maxsize - максимальное кол-во элементов в result
*/  
int line_find_all(const int src[], int src_size, testfunc func, int result[], int result_maxsize) {
    find_count = 0; 
    int found = 0;
    for (int i = 0; i < src_size && found < result_maxsize; i++) {
        find_count++;
        if (func(src[i]) == 1) {
            result[found++] = i; 
        }
    }
    return found; 
}

/*
 * Вспомогательная функция бинарного поиска для нахождения первого вхождения
 * элемента, соответствующего критерию testfunc() == 0.
 * Возвращает индекс или -1.
 */
int bin_find_first_occurrence(const int src[], int src_size, testfunc func) {
    int left = 0, right = src_size - 1;
    int first_occurrence = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        find_count++; // Учитываем сравнение в бинарном поиске
        int comparison_result = func(src[mid]);

        if (comparison_result == 0) {
            // Найден элемент, соответствующий критерию.
            // Это может быть первое вхождение, но нужно проверить слева.
            first_occurrence = mid;
            right = mid - 1; // Продолжаем поиск первого вхождения в левой половине
        } else if (comparison_result < 0) { // src[mid] < target
            // Целевой элемент находится правее
            left = mid + 1;
        } else { // comparison_result > 0, src[mid] > target
            // Целевой элемент находится левее
            right = mid - 1;
        }
    }
    return first_occurrence;
}

/*
 * Вспомогательная функция бинарного поиска для нахождения последнего вхождения
 * элемента, соответствующего критерию testfunc() == 0.
 * Возвращает индекс или -1.
 */
int bin_find_last_occurrence(const int src[], int src_size, testfunc func) {
    int left = 0, right = src_size - 1;
    int last_occurrence = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        find_count++; // Учитываем сравнение в бинарном поиске
        int comparison_result = func(src[mid]);

        if (comparison_result == 0) {
            // Найден элемент, соответствующий критерию.
            // Это может быть последнее вхождение, но нужно проверить справа.
            last_occurrence = mid;
            left = mid + 1; // Продолжаем поиск последнего вхождения в правой половине
        } else if (comparison_result < 0) { // src[mid] < target
            // Целевой элемент находится правее
            left = mid + 1;
        } else { // comparison_result > 0, src[mid] > target
            // Целевой элемент находится левее
            right = mid - 1;
        }
    }
    return last_occurrence;
}

/*
 Бинарный поиск одного элемента в целочисленном массиве
 возвращает индекс найденного значения или -1
 Параметры:
 src - исходный массив (должен быть отсортирован!)
 src_size - кол-во элементов в src
 func - тест-функция (возвращает -1, 0, 1 для сравнения с критерием)
*/  
int bin_find_one(const int src[], int src_size, testfunc func) {
    int left = 0, right = src_size - 1;
    find_count = 0; // Сброс счетчика для этой функции

    while (left <= right) {
        int mid = left + (right - left) / 2;
        find_count++; // Учитываем сравнение
        int comparison_result = func(src[mid]);

        if (comparison_result == 0) {
            // Найден элемент, соответствующий критерию
            return mid;
        } else if (comparison_result < 0) { // src[mid] < target
            // Искомый элемент в правой половине
            left = mid + 1;
        } else { // comparison_result > 0, src[mid] > target
            // Искомый элемент в левой половине
            right = mid - 1;
        }
    }
    return -1; // Элемент не найден
}

/*
 Бинарный поиск всех элементов в целочисленном массиве
 возвращает количество найденных элементов или 0
 Параметры:
 src - исходный массив (должен быть отсортирован!)
 src_size - кол-во элементов в src
 func - тест-функция (возвращает -1, 0, 1 для сравнения с критерием)
 res_beg - адрес переменной, куда будет сохранен индекс первого найденного элемента
 res_end - адрес переменной, куда будет сохранен индекс последнего найденного элемента
*/  
int bin_find_all(const int src[], int src_size, testfunc func, int *res_beg, int *res_end) {
    // Сброс счетчика для этой функции
    find_count = 0; 
    
    // Находим индекс первого вхождения элемента, соответствующего критерию
    int first = bin_find_first_occurrence(src, src_size, func);

    if (first == -1) {
        // Элемент не найден вообще
        *res_beg = -1;
        *res_end = -1;
        return 0;
    }
    
    // Находим индекс последнего вхождения элемента, соответствующего критерию
    int last = bin_find_last_occurrence(src, src_size, func);

    // find_count уже был увеличен в bin_find_first_occurrence и bin_find_last_occurrence

    *res_beg = first;
    *res_end = last;

    // Количество найденных элементов равно (индекс_последнего - индекс_первого + 1)
    return last - first + 1;
}

#endif // FINDUNIT
