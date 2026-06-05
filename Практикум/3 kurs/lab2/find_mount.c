#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <limits.h>

// Структура файловой системы (только нужные поля)
typedef struct {
    fsid_t fsid;     //уникальный id
    long type_code;  //магическое число          
    char mnt_path[PATH_MAX]; //путь монтирования
    
    double total_gb;         //гигабайты
    double free_gb;  
             
    int found_in_mtab;       //нашли ли в файловой системе
    int is_inside;           //внутри каталога, или нет
} FoundFS;

struct statfs sfs;

FoundFS saved_fs[100];
int fs_count = 0;
unsigned long start_dev_id = 0; 


//просто словарь для типов файловых систем
const char* get_fs_type_name(long type) {
    switch (type) {
        case 0xEF53:     return "ext2/ext3/ext4";
        case 0x01021994: return "tmpfs";
        case 0x4d44:     return "fat/vfat";
        case 0x5346544e: return "ntfs";
        case 0x9123683E: return "btrfs";
        case 0x58465342: return "xfs";
        default:         return "Другие типы ФС";
    }
}

void find_mount_point(FoundFS *fs) {
    FILE *fp = fopen("/etc/mtab", "r");
    if (!fp) return;

    char line[1024];
    char dev_name[256];
    char mnt_dir[PATH_MAX];
	struct statfs sfs;
    // Читаем mtab построчно
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%255s %255s", dev_name, mnt_dir) == 2) {
           
            //Проверяем statfs для этой точки монтирования
            if (statfs(mnt_dir, &sfs) == 0) {
                //Если FSID совпал с нашей найденной системой
                if (sfs.f_fsid.__val[0] == fs->fsid.__val[0] &&
                    sfs.f_fsid.__val[1] == fs->fsid.__val[1]) {
                    
                    strcpy(fs->mnt_path, mnt_dir);
                    fs->found_in_mtab = 1;
                    break; 
                }
            }
        }
    }
    fclose(fp);
}

int check_if_inside(const char *base, const char *path) {
    size_t len = strlen(base);
    if (strncmp(base, path, len) == 0) {
        if (path[len] == '\0' || path[len] == '/') {
            return 1;
        }
    }
    return 0;
}

void scan_dir(const char *current_path, const char *start_path) {
    DIR *dir = opendir(current_path);
    if (!dir) return;
    if (statfs(current_path, &sfs) == 0) {
        int is_unique = 1;

        for (int i = 0; i < fs_count; i++) {
            if (saved_fs[i].fsid.__val[0] == sfs.f_fsid.__val[0] &&
                saved_fs[i].fsid.__val[1] == sfs.f_fsid.__val[1]) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique && fs_count < 100) {
            fs_count++;
            int id = fs_count - 1;
            
            saved_fs[id].fsid = sfs.f_fsid;
            saved_fs[id].type_code = sfs.f_type;
            saved_fs[id].total_gb = ((double)sfs.f_blocks * sfs.f_bsize) / (1024 * 1024 * 1024);
            saved_fs[id].free_gb = ((double)sfs.f_bfree * sfs.f_bsize) / (1024 * 1024 * 1024);
            saved_fs[id].found_in_mtab = 0;
            saved_fs[id].is_inside = 0;

            find_mount_point(&saved_fs[id]);

            if (saved_fs[id].found_in_mtab) {
                saved_fs[id].is_inside = check_if_inside(start_path, saved_fs[id].mnt_path);
            }
        }
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

		if (entry->d_type == DT_DIR) {
			char next_path[PATH_MAX];
			snprintf(next_path, sizeof(next_path), "%s/%s", current_path, entry->d_name);

			scan_dir(next_path, start_path);
		}
    }
    closedir(dir);
}

void print_results() {
    printf("\n==================================================\n");
    printf("               РЕЗУЛЬТАТЫ СКАНИРОВАНИЯ            \n");
    printf("==================================================\n");

    long target_types[] = {0xEF53, 0x01021994, 0x4d44, 0x5346544e, 0x9123683E, 0x58465342};
    int num_types = sizeof(target_types) / sizeof(target_types[0]);

    for (int t = 0; t <= num_types; t++) {
        int section_header_printed = 0;

        for (int i = 0; i < fs_count; i++) {
            int is_match = 0;

            if (t < num_types) {
                if (saved_fs[i].type_code == target_types[t]) {
                    is_match = 1;
                }
            } else {
                is_match = 1;
                for (int k = 0; k < num_types; k++) {
                    if (saved_fs[i].type_code == target_types[k]) {
                        is_match = 0;
                    }
                }
            }

            if (is_match) {
                if (!section_header_printed) {
                    if (t < num_types) {
                        printf("\n[ТИП ФС: %s]\n", get_fs_type_name(target_types[t]));
                    } else {
                        printf("\n[ТИП ФС: Другие/Неизвестные]\n");
                    }
                    printf("--------------------------------------------------\n");
                    section_header_printed = 1;
                }

                printf("  FSID              : [%X:%X]\n", saved_fs[i].fsid.__val[0], saved_fs[i].fsid.__val[1]);
                printf("  Общий объем       : %.2f ГБ\n", saved_fs[i].total_gb);
                printf("  Свободно          : %.2f ГБ\n", saved_fs[i].free_gb);
                
                if (saved_fs[i].found_in_mtab) {
                    if (saved_fs[i].is_inside) {
                        printf("  Точка монтирования : %s (ВНУТРИ каталога)\n", saved_fs[i].mnt_path);
                    } else {
                        printf("  Точка монтирования : %s (СНАРУЖИ каталога)\n", saved_fs[i].mnt_path);
                        printf("  Ни одна точка монтирования не лежит внутри заданного каталога.\n");
                    }
                } else {
                    printf("  Точка монтирования : Не найдена в /etc/mtab\n");
                }
                printf("\n");
            }
        }
    }
    printf("==================================================\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Использование: %s <путь_к_каталогу>\n", argv[0]);
        return 1;
    }

    struct stat st;
    if (stat(argv[1], &st) != 0) {
        perror("Ошибка открытия стартового каталога");
        return 1;
    }

    start_dev_id = st.st_dev;

    printf("Сканирование с каталога: %s\n", argv[1]);
    scan_dir(argv[1], argv[1]);
    
    print_results();

    return 0;
}
