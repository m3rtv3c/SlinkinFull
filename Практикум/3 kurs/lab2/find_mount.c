#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/wait.h>

typedef struct
{
    char *name;  // Динамическая строка для имени любой длины
    int count;
} Server;

// Линейный поиск имени сервера в динамическом массиве 
int find_server(Server arr[], int n, const char *name)
{
    if (arr == NULL) return -1;
    for(int i = 0; i < n; i++)
    {
        if(strcmp(arr[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Вспомогательная функция для безопасного добавления/обновления серверов в динамическом массиве
Server* add_or_update_server(Server **arr, int *count, int *capacity, const char *name, int value)
{
    int pos = find_server(*arr, *count, name);
    
    // Если сервер уже есть в массиве, просто суммируем счетчик
    if (pos != -1) 
    {
        (*arr)[pos].count += value;
        return *arr;
    }
    
    // Если массив заполнился, увеличиваем его емкость в 2 раза
    if (*count >= *capacity) 
    {
        *capacity = (*capacity == 0) ? 16 : (*capacity * 2);
        Server *temp = realloc(*arr, (*capacity) * sizeof(Server));
        if (temp == NULL) 
        {
            perror("Ошибка выделения памяти под массив");
            exit(1);
        }
        *arr = temp;
    }
    
    // strdup выделяет память ровно под длину имени (+1 для '\0') и копирует строку
    (*arr)[*count].name = strdup(name);
    if ((*arr)[*count].name == NULL)
    {
        perror("Ошибка выделения памяти под имя сервера");
        exit(1);
    }
    (*arr)[*count].count = value;
    (*count)++;
    
    return *arr;
}

void process_file(char *csv_file, char *result_file)
{
    // Открываем файл с логами
    FILE *f = fopen(csv_file, "r");
    if(f == NULL)
    {
        perror(csv_file);
        exit(1);
    }

    // Инициализация локального динамического массива
    Server *local = NULL;
    int local_count = 0;
    int local_capacity = 0;

    // Буфер для чтения строки из CSV (4096 байт обычно достаточно для одной строки лога)
    char line[4096];

    // Пропускаем заголовок
    if (!fgets(line, sizeof(line), f)) 
    {
        fclose(f);
        exit(0); // Файл пустой, завершаем работу форка
    }

    while(fgets(line, sizeof(line), f))
    {
        char *token;
        int col = 0;

        // Разбиваем строку на токены по разделителю
        token = strtok(line, ";");

        // Начинаем подсчет серверов 
        while(token != NULL)
        {
            // Берем 5 колонку
            if(col == 4)   
            {
                // Выделяем память под имя динамически, чтобы не ограничивать длину в 128 символов
                char *server = strdup(token);
                if (server == NULL) 
                {
                    perror("Ошибка strdup");
                    exit(1);
                }

                // Убираем кавычки, сдвигаем слово влево (исправлен баг со strlen)
                if(server[0] == '"')
                    memmove(server, server + 1, strlen(server) + 1);

                // Получаем новую длину
                int len = strlen(server);

                // Если последний символ кавычка, то меняем на пустой символ
                if(len > 0 && server[len - 1] == '"')
                    server[len - 1] = '\0';

                // Добавляем или обновляем в локальном динамическом массиве
                local = add_or_update_server(&local, &local_count, &local_capacity, server, 1);
                
                free(server); // Освобождаем временную копию
                break;
            }
            token = strtok(NULL, ";");
            col++;
        }
    }
    fclose(f);

    // Поток для результирующего файла + выдача прав
    int fd = open(result_file, O_RDWR | O_CREAT, 0666);
    if (fd < 0) 
    {
        perror(result_file);
        exit(1);
    }

    // Локаем, чтобы форки не перезаписывали данные одновременно
    flock(fd, LOCK_EX);

    // Открываем файл
    FILE *res = fdopen(fd, "r+");

    // Инициализация глобального динамического массива
    Server *global = NULL;
    int global_count = 0;
    int global_capacity = 0;

    // Динамический буфер для fscanf, защищающий от переполнения при чтении длинных имен
    char *name_buf = malloc(4096);
    int count;

    // Ставим указатель на начало
    rewind(res);

    // Берем имя и счетчик, переносим их в массив global через функцию слияния
    // Ограничиваем fscanf чтением максимум 4095 символов во избежание переполнения буфера
    while(fscanf(res, "%4095s %d", name_buf, &count) == 2)
    {
        global = add_or_update_server(&global, &global_count, &global_capacity, name_buf, count);
    }
    free(name_buf); // Буфер для чтения больше не нужен

    // Начинаем слияние локальных данных с глобальными
    for(int i = 0; i < local_count; i++)
    {
        global = add_or_update_server(&global, &global_count, &global_capacity, local[i].name, local[i].count);
    }

    // Идем снова в начало, чтобы перезаписать
    rewind(res);
    // Обрезаем файл до 0 байт
    ftruncate(fd, 0);

    // Проходимся по массиву и записываем все в файл
    for(int i = 0; i < global_count; i++)
    {
        fprintf(res, "%s %d\n", global[i].name, global[i].count);
    }

    // Сбрасываем данные из памяти в файл
    fflush(res);

    // Разблокируем файл
    flock(fd, LOCK_UN);
    fclose(res);

    // Полное освобождение динамической памяти перед завершением процесса
    for(int i = 0; i < local_count; i++) free(local[i].name);
    free(local);

    for(int i = 0; i < global_count; i++) free(global[i].name);
    free(global);
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage: %s result.txt logs...\n", argv[0]);
        return 1;
    }

    char *result_file = argv[1];

    for(int i = 2; i < argc; i++)
    {
        pid_t pid = fork();

        if(pid == 0)
        {
            process_file(argv[i], result_file);
            exit(0);
        }
    }

    // Родитель ждет завершения всех процессов
    while(wait(NULL) > 0);

    printf("Aggregation completed\n");

    return 0;
}
