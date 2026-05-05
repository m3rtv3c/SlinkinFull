#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>

void print_permissions(mode_t mode, int shift, const char* label, int is_dir){
	//Изолируем 3 бита с прав(7=111)
	int perms = (mode >> shift) & 7;
	printf("%s: ", label);
	if(perms == 0) printf("Прав нет");
	else
	{
		if (perms & 4){
			 printf("%s, ", is_dir ? "просмотр" : "чтение");
			}
        if 
        (perms & 2){
			 printf("%s ", is_dir ? "редактирование" : "исполнение");
			} 
        if 
        (perms & 1){
			printf("%s ", is_dir ? "вход в директорию" : "запись");
			} 
		
	}
		printf("\n");
}
	
	
//Стандартные макросы sys/stat, переводим в человекопонимаемый вид
const char* get_type(mode_t mode){
	if (S_ISDIR(mode))  return "каталог";
    if (S_ISFIFO(mode)) return "fifo-канал";
    if (S_ISLNK(mode))  return "ссылка";
    if (S_ISCHR(mode))  return "символьное устройство";
    if (S_ISBLK(mode))  return "блочное устройство";
    if (S_ISSOCK(mode)) return "сокет";
    return "обычный файл";
	}
	
void list_dir(const char *base_path) {
	//Открываем поток чтения
    DIR *dir = opendir(base_path);
    if (!dir) {
		perror("Ошибка открытия");
		return;
	}


    struct dirent *entry;
    char path[PATH_MAX];
    struct stat st;

	//Идем до конца директорииы
    while ((entry = readdir(dir)) != NULL) {
		//Пропускаем текущую и родительскую директория, иначе будет вечная рекурсия
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

		//Склейка
		//base_path - путь к текущей папке, entry->d_name - файл который читаем. 
		//например: home/student и там есть файл example.txt, итоговой путь = home/student/example.txt
        snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);

        if (lstat(path, &st) == -1) continue;
        
        int is_dir = S_ISDIR(st.st_mode);

        struct passwd *pw = getpwuid(st.st_uid);
        struct group  *gr = getgrgid(st.st_gid);

		//Вывод
		printf("файл: %s\n", path);
		printf("тип: %s\n", get_type(st.st_mode));
		printf("хозяин: %s (%d)\n", pw ? pw->pw_name : "unknown", st.st_uid);
		printf("группа: %s (%d)\n", gr ? gr->gr_name : "unknown", st.st_gid);

		//Передаём is_dir в print_permissions
		print_permissions(st.st_mode, 6, "права хозяина", is_dir);
		print_permissions(st.st_mode, 3, "права группы", is_dir);
		print_permissions(st.st_mode, 0, "права остальных", is_dir);

		//спецправа
        if (st.st_mode & (S_ISUID | S_ISGID | S_ISVTX)) {
            printf("спецправа: ");
            if (st.st_mode & S_ISUID) {
				printf("%s \n", is_dir ? "SUID(в каталогах - игнорируется)" : "SUID(запуск от имени хозяина)" );
				}
            
            if (st.st_mode & S_ISGID){
				printf("%s \n", is_dir ? "SGID(новые подфайлы и поддиректории будут иметь такую же группу)" : "SGID(запускается от имени группы которая владеет файлом)" );
				}
      
            if (st.st_mode & S_ISVTX){
				printf("%s \n", is_dir ? "Sticky(файлы в директории могут менять только хозяины этих файлов)" : "Sticky(удалять или изменять файл может только хозяин)" );
				} 
            printf("\n");
        }
        printf("---------------------------\n");

		//Если каталог - рекурсия
        if (S_ISDIR(st.st_mode)) {
            list_dir(path);
        }
    }
    //Закрываем поток
    closedir(dir);
}
int main(int argc, char **argv)
{
	if (argc < 2) {
        fprintf(stderr, "Использование: %s <путь_к_каталогу>\n", argv[0]);
        return 1;
    }
    list_dir(argv[1]);
    return 0;
}

