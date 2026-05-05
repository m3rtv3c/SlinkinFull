#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>



void list_dir(const char *base_path)
{
	DIR *dir = opendir(base_path);
	
	if(!dir){
		perror("Ошибка открытия");
		return;
		}
		
	struct dirent *entry;
	char path[PATH_MAX];
	
	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		}
		
		
	
	
}










int main(int argc, char **argv)
{
	if (argc < 2) {
        fprintf(stderr, "Использование: %s <путь_к_каталогу>\n", argv[0]);
        return 1;
    }
	return 0;
}

