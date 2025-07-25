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

    if (argc == 3 && strcmp(argv[1], "--file") == 0) {
        if (!load_from_file(argv[2], sequence, MAX_DNA_LENGTH)) {
            printf("Failed to load file: %s\n", argv[2]);
            return 1;
        }
    } else {
        printf("Enter DNA sequence:\n");
        if (fgets(sequence, sizeof(sequence), stdin) == NULL) {
            printf("Input error.\n");
            return 1;
        }

        sequence[strcspn(sequence, "\n")] = '\0';
    }

    clean_sequence(sequence);

    printf("Valid DNA sequence:\n");
    print_sequence(sequence);

    int count_a, count_c, count_g, count_t;
    count_bases(sequence, &count_a, &count_c, &count_g, &count_t);

    printf("\nBase statistics:\n");
    printf("A: %d\n", count_a);
    printf("C: %d\n", count_c);
    printf("G: %d\n", count_g);
    printf("T: %d\n", count_t);

    return 0;
}
