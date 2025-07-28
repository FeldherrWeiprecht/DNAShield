#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_DNA_LENGTH 1024
#define MAX_FILENAME_LENGTH 256
#define MAX_TEXT_LENGTH 512

typedef struct {
    int show_ascii;
    int show_stats;
    int show_summary;
    int show_json;
    int do_reverse;
    int do_complement;
    int file_mode;
    int do_csv;
    int do_binary;
    int do_hex;
    int do_key;
    int do_hash;
    int mutate_count;
    int random_length;
    int compare_mode;
    int encrypt_mode;

    char input_file[MAX_FILENAME_LENGTH];
    char csv_file[MAX_FILENAME_LENGTH];
    char compare_seq1[MAX_DNA_LENGTH];
    char compare_seq2[MAX_DNA_LENGTH];
    char encrypt_text[MAX_TEXT_LENGTH];
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

    for (int i = 0; i < length / 2; i++) {
        char temp = sequence[i];
        sequence[i] = sequence[length - 1 - i];
        sequence[length - 1 - i] = temp;
    }
}

void make_complement(char *sequence) {
    for (int i = 0; sequence[i] != '\0'; i++) {
        sequence[i] = complement_base(sequence[i]);
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

    if (length == 0) {
        return;
    }

    if (count <= 0) {
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

    for (int i = 0; i < length && i < MAX_DNA_LENGTH - 1; i++) {
        sequence[i] = bases[rand() % 4];
    }

    sequence[length < MAX_DNA_LENGTH - 1 ? length : MAX_DNA_LENGTH - 1] = '\0';
}

void clean_sequence(char *sequence) {
    int j = 0;

    for (int i = 0; sequence[i] != '\0'; i++) {
        char base = toupper(sequence[i]);

        if (is_valid_base(base)) {
            sequence[j] = base;
            j++;
        }
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
    printf("\n=== ASCII View ===\n\n");

    for (int i = 0; sequence[i] != '\0'; i++) {
        print_base(sequence[i]);
    }

    printf("\n");
}

void count_bases(const char *sequence, int *a, int *c, int *g, int *t) {
    *a = 0;
    *c = 0;
    *g = 0;
    *t = 0;

    for (int i = 0; sequence[i] != '\0'; i++) {
        if (sequence[i] == 'A') {
            (*a)++;
        } else if (sequence[i] == 'C') {
            (*c)++;
        } else if (sequence[i] == 'G') {
            (*g)++;
        } else if (sequence[i] == 'T') {
            (*t)++;
        }
    }
}

void print_stats(const char *sequence) {
    int a;
    int c;
    int g;
    int t;

    count_bases(sequence, &a, &c, &g, &t);

    printf("\n=== Base Statistics ===\n\n");
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

    printf("\n=== Summary ===\n\n");
    printf("Total Length : %d\n", total);
    printf("A: %d  C: %d  G: %d  T: %d\n", a, c, g, t);
    printf("GC Content   : %.1f%%\n", total > 0 ? (100.0 * gc / total) : 0.0);
}

void print_json(const char *sequence) {
    int a;
    int c;
    int g;
    int t;

    count_bases(sequence, &a, &c, &g, &t);

    int total = a + c + g + t;
    int gc = c + g;

    printf("\n=== JSON Output ===\n\n");
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
    int count_a;
    int count_c;
    int count_g;
    int count_t;

    count_bases(sequence, &count_a, &count_c, &count_g, &count_t);

    int total = count_a + count_c + count_g + count_t;
    int gc = count_g + count_c;

    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        printf("Failed to write to file: %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == 0) {
        fprintf(file, "sequence,length,A,C,G,T,gc_percent\n");
    }

    fprintf(file, "%s,%d,%d,%d,%d,%d,%.1f\n",
        sequence,
        total,
        count_a,
        count_c,
        count_g,
        count_t,
        total > 0 ? (100.0 * gc / total) : 0.0
    );

    fclose(file);
}

void print_binary(const char *sequence) {
    printf("\n=== Binary Output ===\n\n");

    for (int i = 0; sequence[i] != '\0'; i++) {
        if (sequence[i] == 'A') {
            printf("00");
        } else if (sequence[i] == 'C') {
            printf("01");
        } else if (sequence[i] == 'G') {
            printf("10");
        } else if (sequence[i] == 'T') {
            printf("11");
        } else {
            printf("??");
        }
    }

    printf("\n");
}

void print_hex(const char *sequence) {
    printf("\n=== Hex Output ===\n\n");

    int i = 0;

    while (sequence[i] != '\0') {
        char code1 = 0;
        char code2 = 0;

        if (sequence[i] == 'A') {
            code1 = 0;
        } else if (sequence[i] == 'C') {
            code1 = 1;
        } else if (sequence[i] == 'G') {
            code1 = 2;
        } else if (sequence[i] == 'T') {
            code1 = 3;
        }

        if (sequence[i + 1] != '\0') {
            if (sequence[i + 1] == 'A') {
                code2 = 0;
            } else if (sequence[i + 1] == 'C') {
                code2 = 1;
            } else if (sequence[i + 1] == 'G') {
                code2 = 2;
            } else if (sequence[i + 1] == 'T') {
                code2 = 3;
            }

            unsigned char combined = (code1 << 2) | code2;
            printf("%X", combined);

            i += 2;
        } else {
            printf("%X", code1 << 2);
            break;
        }
    }

    printf("\n");
}

void derive_key_bytes(const char *sequence, unsigned char *key_out) {
    for (int i = 0; i < 16; i++) {
        key_out[i] = 0;
    }

    for (int i = 0; sequence[i] != '\0'; i++) {
        unsigned char value = 0;

        if (sequence[i] == 'A') {
            value = 0x1;
        } else if (sequence[i] == 'C') {
            value = 0x3;
        } else if (sequence[i] == 'G') {
            value = 0x7;
        } else if (sequence[i] == 'T') {
            value = 0xD;
        }

        key_out[i % 16] ^= value ^ (i * 31);
    }
}

void derive_key(const char *sequence) {
    unsigned char key[16];

    derive_key_bytes(sequence, key);

    printf("\n=== Derived Key ===\n\n");
    printf("DNA Key (32 chars): ");

    for (int i = 0; i < 16; i++) {
        printf("%02X", key[i]);
    }

    printf("\n");
}

void derive_hash(const char *sequence) {
    unsigned char hash[32] = { 0 };

    for (int i = 0; sequence[i] != '\0'; i++) {
        unsigned char value = 0;

        if (sequence[i] == 'A') {
            value = 0xA3;
        } else if (sequence[i] == 'C') {
            value = 0xC5;
        } else if (sequence[i] == 'G') {
            value = 0xF1;
        } else if (sequence[i] == 'T') {
            value = 0x9E;
        }

        hash[i % 32] ^= value ^ (i * 17);
        hash[(i * 3) % 32] ^= (value << (i % 5)) | (value >> ((8 - (i % 5)) % 8));
    }

    printf("\n=== Hash ===\n\n");
    printf("DNA Hash (64 chars): ");

    for (int i = 0; i < 32; i++) {
        printf("%02X", hash[i]);
    }

    printf("\n");
}

void encrypt_text_with_dna_key(const char *sequence, const char *text) {
    unsigned char key[16];

    derive_key_bytes(sequence, key);

    printf("\n=== Encrypted Output ===\n\n");
    printf("Plaintext : %s\n", text);
    printf("Cipherhex : ");

    for (int i = 0; text[i] != '\0'; i++) {
        unsigned char encrypted = text[i] ^ key[i % 16];
        printf("%02X", encrypted);
    }

    printf("\n");
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
    config.do_binary = 0;
    config.do_hex = 0;
    config.do_key = 0;
    config.do_hash = 0;
    config.mutate_count = 0;
    config.random_length = 0;
    config.compare_mode = 0;
    config.encrypt_mode = 0;
    config.input_file[0] = '\0';
    config.csv_file[0] = '\0';
    config.compare_seq1[0] = '\0';
    config.compare_seq2[0] = '\0';
    config.encrypt_text[0] = '\0';

    for (int i = 1; i < argc; i++) {
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
        } else if (strcmp(argv[i], "--binary") == 0) {
            config.do_binary = 1;
        } else if (strcmp(argv[i], "--hex") == 0) {
            config.do_hex = 1;
        } else if (strcmp(argv[i], "--key") == 0) {
            config.do_key = 1;
        } else if (strcmp(argv[i], "--hash") == 0) {
            config.do_hash = 1;
        } else if (strcmp(argv[i], "--csv") == 0 && i + 1 < argc) {
            strncpy(config.csv_file, argv[++i], MAX_FILENAME_LENGTH - 1);
            config.do_csv = 1;
        } else if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
            strncpy(config.input_file, argv[++i], MAX_FILENAME_LENGTH - 1);
            config.file_mode = 1;
        } else if (strcmp(argv[i], "--mutate") == 0 && i + 1 < argc) {
            config.mutate_count = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--random") == 0 && i + 1 < argc) {
            config.random_length = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--compare") == 0 && i + 2 < argc) {
            strncpy(config.compare_seq1, argv[++i], MAX_DNA_LENGTH - 1);
            strncpy(config.compare_seq2, argv[++i], MAX_DNA_LENGTH - 1);
            config.compare_mode = 1;
        } else if (strcmp(argv[i], "--encrypt") == 0 && i + 1 < argc) {
            strncpy(config.encrypt_text, argv[++i], MAX_TEXT_LENGTH - 1);
            config.encrypt_mode = 1;
        }
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

    if (config.do_binary == 1) {
        print_binary(sequence);
    }

    if (config.do_hex == 1) {
        print_hex(sequence);
    }

    if (config.do_key == 1) {
        derive_key(sequence);
    }

    if (config.do_hash == 1) {
        derive_hash(sequence);
    }

    if (config.encrypt_mode == 1) {
        encrypt_text_with_dna_key(sequence, config.encrypt_text);
    }

    if (config.show_ascii == 1) {
        print_sequence(sequence);
    }

    if (config.show_stats == 1) {
        print_stats(sequence);
    }

    if (config.show_summary == 1) {
        print_summary(sequence);
    }

    if (config.do_csv == 1) {
        export_csv(config.csv_file, sequence);
    }

    printf("\n");
}

void run_compare_mode(options config) {
    clean_sequence(config.compare_seq1);
    clean_sequence(config.compare_seq2);

    int diff = count_differences(config.compare_seq1, config.compare_seq2);

    printf("\n=== Comparing Sequences ===\n\n");
    printf("Sequence 1: %s\n", config.compare_seq1);
    printf("Sequence 2: %s\n", config.compare_seq2);
    printf("Differences: %d base(s)\n", diff);

    if (config.show_json == 1) {
        print_json(config.compare_seq1);
        print_json(config.compare_seq2);
    }

    if (config.do_binary == 1) {
        print_binary(config.compare_seq1);
        print_binary(config.compare_seq2);
    }

    if (config.do_hex == 1) {
        print_hex(config.compare_seq1);
        print_hex(config.compare_seq2);
    }

    if (config.do_key == 1) {
        derive_key(config.compare_seq1);
        derive_key(config.compare_seq2);
    }

    if (config.do_hash == 1) {
        derive_hash(config.compare_seq1);
        derive_hash(config.compare_seq2);
    }

    if (config.encrypt_mode == 1) {
        encrypt_text_with_dna_key(config.compare_seq1, config.encrypt_text);
        encrypt_text_with_dna_key(config.compare_seq2, config.encrypt_text);
    }

    if (config.show_ascii == 1) {
        printf("\n--- ASCII Sequence 1 ---\n");
        print_sequence(config.compare_seq1);

        printf("\n--- ASCII Sequence 2 ---\n");
        print_sequence(config.compare_seq2);
    }

    if (config.show_stats == 1) {
        printf("\n--- Statistics Sequence 1 ---\n");
        print_stats(config.compare_seq1);

        printf("\n--- Statistics Sequence 2 ---\n");
        print_stats(config.compare_seq2);
    }

    if (config.show_summary == 1) {
        print_summary(config.compare_seq1);
        print_summary(config.compare_seq2);
    }

    if (config.do_csv == 1) {
        export_csv(config.csv_file, config.compare_seq1);
        export_csv(config.csv_file, config.compare_seq2);
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char sequence[MAX_DNA_LENGTH];
    options config = parse_args(argc, argv);

    if (config.compare_mode == 1) {
        run_compare_mode(config);
        return 0;
    }

    if (config.random_length > 0) {
        generate_random_sequence(sequence, config.random_length);
        process_sequence(sequence, config);
        return 0;
    }

    if (config.file_mode == 1) {
        FILE *file = fopen(config.input_file, "r");

        if (file == NULL) {
            printf("Error: Could not open file '%s'\n", config.input_file);
            return 1;
        }

        if (config.do_csv == 1) {
            FILE *csv = fopen(config.csv_file, "w");
            if (csv != NULL) {
                fprintf(csv, "sequence,length,A,C,G,T,gc_percent\n");
                fclose(csv);
            }
        }

        while (fgets(sequence, sizeof(sequence), file) != NULL) {
            sequence[strcspn(sequence, "\n")] = '\0';
            process_sequence(sequence, config);
        }

        fclose(file);
        return 0;
    }

    printf("\nPlease enter a DNA sequence:\n\n");

    if (fgets(sequence, sizeof(sequence), stdin) == NULL) {
        printf("Error: Failed to read input.\n");
        return 1;
    }

    sequence[strcspn(sequence, "\n")] = '\0';

    process_sequence(sequence, config);

    return 0;
}
