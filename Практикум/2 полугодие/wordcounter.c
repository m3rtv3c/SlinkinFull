#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int count_chars = 0, count_words = 0, count_lines = 0;
    int option_char = 0, option_word = 0, option_line = 0;

    
    int opt;
    while ((opt = getopt(argc, argv, "cwl")) != -1) {
        switch (opt) {
            case 'c':
                option_char = 1;
                break;
            case 'w':
                option_word = 1;
                break;
            case 'l':
                option_line = 1;
                break;
            default:
                fprintf(stderr, "Использование: %s [-cwl] [файл]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    FILE *file = NULL;
    if (optind < argc) {
        file = fopen(argv[optind], "r");
        if (!file) {
            perror("Ошибка открытия файла");
            exit(EXIT_FAILURE);
        }
    } else {
        file = stdin;
    }

    int c;
    int in_word = 0;

    while ((c = fgetc(file)) != EOF) {
        if (option_char || (!option_char && !option_word && !option_line)) {
            count_chars++;
        }
        if (option_line || (!option_char && !option_word && !option_line)) {
            if (c == '\n') {
                count_lines++;
            }
        }
        if (c >= 33) {
            if (!in_word) {
                in_word = 1;
                count_words++;
            }
        } else {
            in_word = 0;
        }    
    }
    if (file != stdin) {
        fclose(file);
    }
    
    if (option_line || (!option_char && !option_word && !option_line))
        printf("%d ", count_lines);
    if (option_word || (!option_char && !option_word && !option_line))
        printf("%d ", count_words);
    if (option_char || (!option_char && !option_word && !option_line))
        printf("%d ", count_chars);
    printf("\n");

    return 0;
}
