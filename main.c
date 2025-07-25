#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DNA_LENGTH 1024

int is_valid_base(char base) {
    switch (base) {
        case 'A':
        case 'C':
        case 'G':
        case 'T':
            return 1;
        default:
            return 0;
    }
}

char complement_base(char base) {
    switch (base) {
        case 'A':
            return 'T';
        case 'T':
            return 'A';
        case 'C':
            return 'G';
        case 'G':
            return 'C';
        default:
            return '?';
    }
}

void reverse_sequence(char *sequence) {
    int length = strlen(sequence);
    for (int i = 0; i < length / 2; i++) {
        char temp = sequence[i];
        sequence[i] = sequence[length - 1 - i];
        sequence[length - 1 - i] = temp;
    }
}

void make_complement(char *sequence) {
    int length = strlen(sequence);
    for (int i = 0; i < length; i++) {
        sequence[i] = complement_base(sequence[i]);
    }
}

void print_base(char base) {
    switch (base) {
        case 'A':
            printf("[A]");
            break;
        case 'C':
            printf("[C]");
            break;
        case 'G':
            printf("[G]");
            break;
        case 'T':
            printf("[T]");
            break;
        default:
            printf("[?]");
            break;
    }
}

void print_sequence(const char *sequence) {
    int i = 0;
    while (sequence[i] != '\0') {
        print_base(sequence[i]);
        i++;
    }
    printf("\n");
}

void count_bases(const char *sequence, int *count_a, int *count_c, int *count_g, int *count_t) {
    *count_a = 0;
    *count_c = 0;
    *count_g = 0;
    *count_t = 0;

    for (int i = 0; sequence[i] != '\0'; i++) {
        switch (sequence[i]) {
            case 'A':
                (*count_a)++;
                break;
            case 'C':
                (*count_c)++;
                break;
            case 'G':
                (*count_g)++;
                break;
            case 'T':
                (*count_t)++;
                break;
        }
    }
}

void print_stats(const char *sequence) {
    int count_a, count_c, count_g, count_t;
    count_bases(sequence, &count_a, &count_c, &count_g, &count_t);
    printf("Base statistics:\n");
    printf("A: %d\n", count_a);
    printf("C: %d\n", count_c);
    printf("G: %d\n", count_g);
    printf("T: %d\n", count_t);
}

void clean_sequence(char *sequence) {
    int length = strlen(sequence);
    int j = 0;

    for (int i = 0; i < length; i++) {
        char base = toupper(sequence[i]);
        if (is_valid_base(base)) {
            sequence[j] = base;
            j++;
        }
    }

    sequence[j] = '\0';
}

int load_from_file(const char *filename, char *buffer, int max_len) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return 0;
    }

    if (fgets(buffer, max_len, file) == NULL) {
        fclose(file);
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    fclose(file);
    return 1;
}

int main(int argc, char *argv[]) {
    char sequence[MAX_DNA_LENGTH];
    int show_ascii = 1;
    int show_stats = 1;
    int file_mode = 0;
    int do_reverse = 0;
    int do_complement = 0;

    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--ascii") == 0) {
                show_ascii = 1;
            } else if (strcmp(argv[i], "--stats") == 0) {
                show_stats = 1;
            } else if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
                if (!load_from_file(argv[i + 1], sequence, MAX_DNA_LENGTH)) {
                    printf("Failed to load file: %s\n", argv[i + 1]);
                    return 1;
                }
                file_mode = 1;
                i++;
            } else if (strcmp(argv[i], "--reverse") == 0) {
                do_reverse = 1;
            } else if (strcmp(argv[i], "--complement") == 0) {
                do_complement = 1;
            }
        }
    }

    if (!file_mode) {
        printf("Enter DNA sequence:\n");
        if (fgets(sequence, sizeof(sequence), stdin) == NULL) {
            printf("Input error.\n");
            return 1;
        }
        sequence[strcspn(sequence, "\n")] = '\0';
    }

    clean_sequence(sequence);

    if (do_complement) {
        make_complement(sequence);
    }

    if (do_reverse) {
        reverse_sequence(sequence);
    }

    if (show_ascii) {
        printf("Valid DNA sequence:\n");
        print_sequence(sequence);
    }

    if (show_stats) {
        printf("\n");
        print_stats(sequence);
    }

    return 0;
}
