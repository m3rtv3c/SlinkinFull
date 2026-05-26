#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

void make_empty_file(const char *path, mode_t mode) {
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        perror("Ошибка создания файла");
        return;
    }
    fclose(f);
    chmod(path, mode & 07777);
}

void make_symlink(const char *src, const char *dst) {
    char target[PATH_MAX];
    int len = readlink(src, target, sizeof(target) - 1);
    if (len < 0) {
        perror("Ошибка чтения ссылки");
        return;
    }
    target[len] = '\0'; 

	//перенаправляем символьную ссылку
    unlink(dst);
    symlink(target, dst);
}

void copy_dir(const char *src_dir, const char *dst_dir) {
    DIR *dir = opendir(src_dir);
    if (!dir) return;

    struct dirent *entry;
    char src_path[PATH_MAX];
    char dst_path[PATH_MAX];
    struct stat st;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst_dir, entry->d_name);

        if (lstat(src_path, &st) == -1) continue;

		//Если директория, то создаем новую и рекурсивно идем до конца
        if (S_ISDIR(st.st_mode)) {
            mkdir(dst_path, st.st_mode & 07777);
            copy_dir(src_path, dst_path);
        } 
        //Если символьная ссылка, то создаем ее
        else if (S_ISLNK(st.st_mode)) {
            make_symlink(src_path, dst_path);
        } 
        else {
            //Все остальное делаем пустыми файлами
            make_empty_file(dst_path, st.st_mode);
        }
    }
    closedir(dir);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Использование: %s <откуда> <куда>\n", argv[0]);
        return 1;
    }

    struct stat st;
    if (lstat(argv[1], &st) == -1) {
        perror("Исходный каталог не найден");
        return 1;
    }

    mkdir(argv[2], st.st_mode & 07777);

    copy_dir(argv[1], argv[2]);

    printf("Готово!\n");
    return 0;
}
