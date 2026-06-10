#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/wait.h>

#define MAX_SERVERS 100
#define MAX_NAME 128

typedef struct
{
    char name[MAX_NAME];
    int count;
} Server;


//линейный поиск имени сервера в массиве 
int find_server(Server arr[], int n, char *name)
{
    for(int i=0; i<n; i++)
    {
        if(strcmp(arr[i].name,name) == 0)
            return i;
    }
    return -1;
}

void process_file(char *csv_file, char *result_file)
{
	//открываем файл с логами
    FILE *f = fopen(csv_file, "r");

    if(f == NULL)
    {
        perror(csv_file);
        exit(1);
    }

	
    Server local[MAX_SERVERS];
    int local_count = 0;

	//буфер для чтения строки
    char line[4096];

    //Пропускаем заголовок
    fgets(line, sizeof(line), f);

    while(fgets(line, sizeof(line), f))
    {
        char *token;
        int col = 0;

		//разбиваем строку на токены по разделителю
        token = strtok(line, ";");

		//Начинаем подсчет серверов 
        while(token != NULL)
        {
			//Берем 5 колонку
            if(col == 4)   
            {
                char server[MAX_NAME];

				//Берем имя сервера
                strcpy(server, token);

				//Убираем кавыки, сдвигаем слово влево
                if(server[0]=='"')
                    memmove(server, server + 1, strlen(server) + 1);

				//получаем новую длину
                int len = strlen(server);

				//если последний символ ковычки, то меняем на пустой символ
                if(server[len-1]=='"')
                    server[len-1]='\0';

				//ищем,не встречался ли сервер 
                int pos = find_server(local,local_count,server);
                 
                //Видим сервер впервый раз               
                if(pos == -1)
                {
                    strcpy(local[local_count].name, server);
					//Количество упоминаний именно ЭТОГО сервера 
                    local[local_count].count = 1;
                    local_count++;
                }
                else
                {
					//Увеличиваем 	
                    local[pos].count++;
                }
                break;
            }
            token = strtok(NULL, ";");
            col++;
        }
    }

    fclose(f);

	//поток для результирующего файла + выдача прав
    int fd = open(result_file, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror(result_file);
        exit(1);
    }

    flock(fd, LOCK_EX);

    FILE *res = fdopen(fd, "r+");
    if (res == NULL) {
        perror("fdopen");
        flock(fd, LOCK_UN);
        close(fd);
        exit(1);
    }

    Server global[MAX_SERVERS];
    int global_count = 0;

    char name[MAX_NAME];
    int count;

    // Убеждаемся, что мы в самом начале файла, чтобы считать актуальные данные
    rewind(res);

    // Читаем глобальные данные, которые успели записать предыдущие форки
    while(fscanf(res, "%s %d", name, &count) == 2)
    {
        if (global_count < MAX_SERVERS) {
            strcpy(global[global_count].name, name); 
            global[global_count].count = count;
            global_count++;
        }
    }

    // Слияние данных 
    for(int i = 0; i < local_count; i++)
    {
        int pos = find_server(global, global_count, local[i].name);

        if(pos == -1)
        {
            if (global_count < MAX_SERVERS) {
                strcpy(global[global_count].name, local[i].name);
                global[global_count].count = local[i].count;
                global_count++;
            }
        }
        else
        {
            global[pos].count += local[i].count;
        }
    }

    rewind(res);

    // Записываем обновленные данные
    for(int i = 0; i < global_count; i++)
    {
        fprintf(res, "%s %d\n", global[i].name, global[i].count);
    }

    fflush(res);

    ftruncate(fileno(res), ftell(res));

    flock(fileno(res), LOCK_UN);
    fclose(res);
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage: %s result.txt logs...\n",
               argv[0]);
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

	//родитель ждет завершения всех процессов
    while(wait(NULL) > 0);

    printf("Aggregation completed\n");

    return 0;
}
