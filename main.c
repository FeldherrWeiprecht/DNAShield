#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DNA_LENGTH 1024
#define MAX_FILENAME_LENGTH 256

typedef struct {
    int show_ascii;
    int show_stats;
    int show_summary;
    int show_json;
    int do_reverse;
    int do_complement;
    int file_mode;
    int do_csv;
    char input_file[MAX_FILENAME_LENGTH];
    char csv_file[MAX_FILENAME_LENGTH];
} options;

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

char complement_base(char base) {
    if (base == 'A') {
        return 'T';
    }

    if (base == 'T') {
        return 'A';
    }

    if (base == 'C') {
        return 'G';
    }

    if (base == 'G') {
        return 'C';
    }

    return '?';
}

void reverse_sequence(char *sequence) {
    int length = strlen(sequence);
    int i = 0;

    while (i < length / 2) {
        char temp = sequence[i];
        sequence[i] = sequence[length - 1 - i];
        sequence[length - 1 - i] = temp;
        i++;
    }
}

void make_complement(char *sequence) {
    int i = 0;

    while (sequence[i] != '\0') {
        sequence[i] = complement_base(sequence[i]);
        i++;
    }
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
    int i = 0;

    while (sequence[i] != '\0') {
        print_base(sequence[i]);
        i++;
    }

    printf("\n");
}

void count_bases(const char *sequence, int *a, int *c, int *g, int *t) {
    *a = 0;
    *c = 0;
    *g = 0;
    *t = 0;

    int i = 0;

    while (sequence[i] != '\0') {
        if (sequence[i] == 'A') {
            (*a)++;
        } else if (sequence[i] == 'C') {
            (*c)++;
        } else if (sequence[i] == 'G') {
            (*g)++;
        } else if (sequence[i] == 'T') {
            (*t)++;
        }

        i++;
    }
}

void print_stats(const char *sequence) {
    int a;
    int c;
    int g;
    int t;

    count_bases(sequence, &a, &c, &g, &t);

    printf("Base statistics:\n");
    printf("A: %d\n", a);
    printf("C: %d\n", c);
    printf("G: %d\n", g);
    printf("T: %d\n", t);
}

void print_summary(const char *sequence) {
    int a;
    int c;
    int g;
    int t;

    count_bases(sequence, &a, &c, &g, &t);

    int total = a + c + g + t;
    int gc = c + g;

    printf("\nSummary: Length=%d A=%d C=%d G=%d T=%d GC%%=%.1f%%\n",
           total, a, c, g, t, total > 0 ? (100.0 * gc / total) : 0.0);
}

void print_json(const char *sequence) {
    int a;
    int c;
    int g;
    int t;

    count_bases(sequence, &a, &c, &g, &t);

    int total = a + c + g + t;
    int gc = c + g;

    printf("{\n");
    printf("  \"length\": %d,\n", total);
    printf("  \"sequence\": \"%s\",\n", sequence);
    printf("  \"counts\": {\n");
    printf("    \"A\": %d,\n", a);
    printf("    \"C\": %d,\n", c);
    printf("    \"G\": %d,\n", g);
    printf("    \"T\": %d\n", t);
    printf("  },\n");
    printf("  \"gc_percent\": %.1f\n", total > 0 ? (100.0 * gc / total) : 0.0);
    printf("}\n");
}

void export_csv(const char *filename, const char *sequence) {
    int a;
    int c;
    int g;
    int t;

    count_bases(sequence, &a, &c, &g, &t);

    int total = a + c + g + t;
    int gc = c + g;

    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Failed to open CSV file: %s\n", filename);
        return;
    }

    fprintf(file, "sequence,length,A,C,G,T,gc_percent\n");
    fprintf(file, "%s,%d,%d,%d,%d,%d,%.1f\n",
            sequence, total, a, c, g, t,
            total > 0 ? (100.0 * gc / total) : 0.0);

    fclose(file);
}

void clean_sequence(char *sequence) {
    int i = 0;
    int j = 0;

    while (sequence[i] != '\0') {
        char base = toupper(sequence[i]);

        if (is_valid_base(base)) {
            sequence[j] = base;
            j++;
        }

        i++;
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

options parse_args(int argc, char *argv[]) {
    options config;
    config.show_ascii = 1;
    config.show_stats = 1;
    config.show_summary = 0;
    config.show_json = 0;
    config.do_reverse = 0;
    config.do_complement = 0;
    config.file_mode = 0;
    config.do_csv = 0;
    config.input_file[0] = '\0';
    config.csv_file[0] = '\0';

    int i = 1;

    while (i < argc) {
        if (strcmp(argv[i], "--ascii") == 0) {
            config.show_ascii = 1;
        } else if (strcmp(argv[i], "--stats") == 0) {
            config.show_stats = 1;
        } else if (strcmp(argv[i], "--summary") == 0) {
            config.show_summary = 1;
        } else if (strcmp(argv[i], "--json") == 0) {
            config.show_json = 1;
            config.show_ascii = 0;
            config.show_stats = 0;
            config.show_summary = 0;
        } else if (strcmp(argv[i], "--reverse") == 0) {
            config.do_reverse = 1;
        } else if (strcmp(argv[i], "--complement") == 0) {
            config.do_complement = 1;
        } else if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
            strncpy(config.input_file, argv[i + 1], MAX_FILENAME_LENGTH - 1);
            config.file_mode = 1;
            i++;
        } else if (strcmp(argv[i], "--csv") == 0 && i + 1 < argc) {
            strncpy(config.csv_file, argv[i + 1], MAX_FILENAME_LENGTH - 1);
            config.do_csv = 1;
            i++;
        }

        i++;
    }

    return config;
}

int main(int argc, char *argv[]) {
    char sequence[MAX_DNA_LENGTH];
    options config = parse_args(argc, argv);

    if (config.file_mode == 1) {
        int ok = load_from_file(config.input_file, sequence, MAX_DNA_LENGTH);

        if (ok == 0) {
            printf("Failed to load file: %s\n", config.input_file);
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

    if (config.do_complement == 1) {
        make_complement(sequence);
    }

    if (config.do_reverse == 1) {
        reverse_sequence(sequence);
    }

    if (config.show_json == 1) {
        print_json(sequence);
    }

    if (config.show_ascii == 1) {
        printf("Valid DNA sequence:\n");
        print_sequence(sequence);
    }

    if (config.show_stats == 1) {
        printf("\n");
        print_stats(sequence);
    }

    if (config.show_summary == 1) {
        print_summary(sequence);
    }

    if (config.do_csv == 1) {
        export_csv(config.csv_file, sequence);
    }

    return 0;
}
