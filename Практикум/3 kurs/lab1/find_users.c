#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

typedef struct {
    char *name;
    char **groups;
    int group_count;
} user_info;

int find_user_index(user_info *list, int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].name, name) == 0) return i;
    }
    return -1;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <имя_пользователя>\n", argv[0]);
        return 1;
    }

    struct passwd *pw = getpwnam(argv[1]);
    if (!pw) {
        fprintf(stderr, "Пользователь %s не найден\n", argv[1]);
        return 1;
    }

    user_info neighbors[1000]; 
    int total_neighbors = 0;

    struct group *grp;
    setgrent();

    while ((grp = getgrent()) != NULL) {
        int user_in_group = 0;

        //Проверяем, является ли группа общей (есть ли в ней целевой пользователь)
        if (pw->pw_gid == grp->gr_gid) {
            user_in_group = 1;
        } else {
			//перебор дополнительных участников
            for (int i = 0; grp->gr_mem[i] != NULL; i++) {
                if (strcmp(grp->gr_mem[i], pw->pw_name) == 0) {
                    user_in_group = 1;
                    break;
                }
            }
        }

		//Если на одном из этапов пользователь найден в одной из групп
        //Если группа общая, добавляем ВСЕХ остальных её участников в список
        if (user_in_group) {
            for (int i = 0; grp->gr_mem[i] != NULL; i++) {
                char *m_name = grp->gr_mem[i]; //имя пользователя 
            
				if (strcmp(m_name, pw->pw_name) == 0) continue;

                int idx = find_user_index(neighbors, total_neighbors, m_name); 
                if (idx == -1) {
                    //функция поиска по массиву не нашла юзера, значит добавляем его в массив
                    neighbors[total_neighbors].name = strdup(m_name);
                    neighbors[total_neighbors].groups = malloc(sizeof(char*) * 100);
                    neighbors[total_neighbors].groups[0] = strdup(grp->gr_name);
                    neighbors[total_neighbors].group_count = 1;
                    total_neighbors++;
                } else {
                    //Если пользователь уже встречался то просто добавляем
                    int current_count = neighbors[idx].group_count;  
					neighbors[idx].groups[current_count] = strdup(grp->gr_name); 
					neighbors[idx].group_count = current_count + 1;  
                }
            }
        }
    }
    endgrent();

    //Вывод результата
    printf("Пользователи, имеющие общие группы с %s:\n", pw->pw_name);
    for (int i = 0; i < total_neighbors; i++) {
        printf("%s | Группы: ", neighbors[i].name);
        for (int j = 0; j < neighbors[i].group_count; j++) {
            printf("%s%s", neighbors[i].groups[j], (j == neighbors[i].group_count - 1) ? "" : ", ");
        }
        printf("\n");
    }

    //Очистка
    for (int i = 0; i < total_neighbors; i++) {
        for (int j = 0; j < neighbors[i].group_count; j++) free(neighbors[i].groups[j]);
        free(neighbors[i].groups);
        free(neighbors[i].name);
    }

    return 0;
}
