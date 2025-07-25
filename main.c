#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DNA_LENGTH 1024

int is_valid_base(char base) {
    if (base == 'A') {
        return 1;
    }

    if (base == 'C') {
        return 1;
    }

    if (base == 'G') {
        return 1;
    }

    if (base == 'T') {
        return 1;
    }

    return 0;
}

void print_base(char base) {
    if (base == 'A') {
        printf("[A]");
    } else if (base == 'C') {
        printf("[C]");
    } else if (base == 'G') {
        printf("[G]");
    } else if (base == 'T') {
        printf("[T]");
    } else {
        printf("[?]");
    }
}

void print_sequence(const char *sequence) {
    for (int i = 0; sequence[i] != '\0'; i++) {
        print_base(sequence[i]);
    }

    printf("\n");
}

void clean_sequence(char *sequence) {
    int i = 0;
    int j = 0;

    while (sequence[i] != '\0') {
        char base = sequence[i];

        if (base >= 'a' && base <= 'z') {
            base = base - ('a' - 'A');
        }

        if (is_valid_base(base)) {
            sequence[j] = base;
            j++;
        }

        i++;
    }

    sequence[j] = '\0';
}

int read_sequence_stdin(char *buffer, int max_length) {
    printf("Enter DNA sequence:\n");

    if (fgets(buffer, max_length, stdin) == NULL) {
        return 0;
    }

    int length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }

    return 1;
}

int read_sequence_file(const char *filename, char *buffer, int max_length) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return 0;
    }

    if (fgets(buffer, max_length, file) == NULL) {
        fclose(file);
        return 0;
    }

    int length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }

    fclose(file);
    return 1;
}

int main(int argc, char *argv[]) {
    char sequence[MAX_DNA_LENGTH];
    int loaded = 0;

    if (argc == 3 && strcmp(argv[1], "--file") == 0) {
        loaded = read_sequence_file(argv[2], sequence, MAX_DNA_LENGTH);

        if (!loaded) {
            printf("Failed to read from file: %s\n", argv[2]);
            return 1;
        }
    } else {
        loaded = read_sequence_stdin(sequence, MAX_DNA_LENGTH);

        if (!loaded) {
            printf("Failed to read input.\n");
            return 1;
        }
    }

    clean_sequence(sequence);

    if (strlen(sequence) == 0) {
        printf("No valid DNA bases found.\n");
        return 1;
    }

    printf("Valid DNA sequence:\n");
    print_sequence(sequence);

    return 0;
}
