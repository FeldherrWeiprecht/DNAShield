#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    int mutate_count;
    int random_length;
    int compare_mode;
    char input_file[MAX_FILENAME_LENGTH];
    char csv_file[MAX_FILENAME_LENGTH];
    char compare_seq1[MAX_DNA_LENGTH];
    char compare_seq2[MAX_DNA_LENGTH];
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

char random_base(char exclude) {
    char bases[] = { 'A', 'C', 'G', 'T' };
    char new_base;

    do {
        new_base = bases[rand() % 4];
    } while (new_base == exclude);

    return new_base;
}

void mutate_sequence(char *sequence, int count) {
    int length = strlen(sequence);

    if (length == 0 || count <= 0) {
        return;
    }

    int mutated[MAX_DNA_LENGTH] = { 0 };
    int done = 0;

    while (done < count) {
        int pos = rand() % length;

        if (mutated[pos] == 0 && is_valid_base(sequence[pos])) {
            sequence[pos] = random_base(sequence[pos]);
            mutated[pos] = 1;
            done++;
        }
    }
}

void generate_random_sequence(char *sequence, int length) {
    char bases[] = { 'A', 'C', 'G', 'T' };
    int i = 0;

    while (i < length && i < MAX_DNA_LENGTH - 1) {
        sequence[i] = bases[rand() % 4];
        i++;
    }

    sequence[i] = '\0';
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

int count_differences(const char *s1, const char *s2) {
    int diff = 0;
    int i = 0;

    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            diff++;
        }
        i++;
    }

    while (s1[i] != '\0' || s2[i] != '\0') {
        diff++;
        i++;
    }

    return diff;
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
    config.mutate_count = 0;
    config.random_length = 0;
    config.compare_mode = 0;
    config.input_file[0] = '\0';
    config.csv_file[0] = '\0';
    config.compare_seq1[0] = '\0';
    config.compare_seq2[0] = '\0';

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
        } else if (strcmp(argv[i], "--mutate") == 0 && i + 1 < argc) {
            config.mutate_count = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--random") == 0 && i + 1 < argc) {
            config.random_length = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--compare") == 0 && i + 2 < argc) {
            strncpy(config.compare_seq1, argv[i + 1], MAX_DNA_LENGTH - 1);
            strncpy(config.compare_seq2, argv[i + 2], MAX_DNA_LENGTH - 1);
            config.compare_mode = 1;
            i += 2;
        }

        i++;
    }

    return config;
}

void process_sequence(char *sequence, options config) {
    clean_sequence(sequence);

    if (config.mutate_count > 0) {
        mutate_sequence(sequence, config.mutate_count);
    }

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

    printf("\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char sequence[MAX_DNA_LENGTH];
    options config = parse_args(argc, argv);

    if (config.compare_mode == 1) {
        clean_sequence(config.compare_seq1);
        clean_sequence(config.compare_seq2);
        int diff = count_differences(config.compare_seq1, config.compare_seq2);

        printf("Sequence 1: %s\n", config.compare_seq1);
        printf("Sequence 2: %s\n", config.compare_seq2);
        printf("Difference: %d base(s) differ\n\n", diff);

        if (config.show_json == 1) {
            print_json(config.compare_seq1);
            print_json(config.compare_seq2);
        }

        if (config.show_ascii == 1) {
            printf("ASCII for sequence 1:\n");
            print_sequence(config.compare_seq1);
            printf("ASCII for sequence 2:\n");
            print_sequence(config.compare_seq2);
        }

        if (config.show_stats == 1) {
            printf("Stats for sequence 1:\n");
            print_stats(config.compare_seq1);
            printf("Stats for sequence 2:\n");
            print_stats(config.compare_seq2);
        }

        if (config.show_summary == 1) {
            print_summary(config.compare_seq1);
            print_summary(config.compare_seq2);
        }

        return 0;
    }

    if (config.random_length > 0) {
        generate_random_sequence(sequence, config.random_length);
        process_sequence(sequence, config);
    } else if (config.file_mode == 1) {
        FILE *file = fopen(config.input_file, "r");

        if (file == NULL) {
            printf("Failed to open file: %s\n", config.input_file);
            return 1;
        }

        while (fgets(sequence, sizeof(sequence), file) != NULL) {
            sequence[strcspn(sequence, "\n")] = '\0';
            process_sequence(sequence, config);
        }

        fclose(file);
    } else {
        printf("Enter DNA sequence:\n");

        if (fgets(sequence, sizeof(sequence), stdin) == NULL) {
            printf("Input error.\n");
            return 1;
        }

        sequence[strcspn(sequence, "\n")] = '\0';
        process_sequence(sequence, config);
    }

    return 0;
}
