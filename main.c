#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define MAX_DNA_LENGTH 1024
#define MAX_FILENAME_LENGTH 256
#define MAX_TEXT_LENGTH 512
#define BAR_WIDTH 40
#define MAX_COMPRESSED_LENGTH (MAX_DNA_LENGTH * 3)

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
    int do_qrcode;
    int do_bars;
    int do_compress;
    int do_decompress;
    int do_export_stats;
    int mutate_count;
    int random_length;
    int compare_mode;
    int encrypt_mode;
    int decrypt_mode;
    int no_color;

    char input_file[MAX_FILENAME_LENGTH];
    char csv_file[MAX_FILENAME_LENGTH];
    char export_stats_file[MAX_FILENAME_LENGTH];
    char compare_seq1[MAX_DNA_LENGTH];
    char compare_seq2[MAX_DNA_LENGTH];
    char encrypt_text[MAX_TEXT_LENGTH];
    char decrypt_hex[MAX_TEXT_LENGTH];
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

    int count = 0;

    for (int i = 0; sequence[i] != '\0'; i++) {
        print_base(sequence[i]);
        count++;

        if (count % 32 == 0) {
            printf("\n");
        }
    }

    if (count % 32 != 0) {
        printf("\n");
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

    printf("Total Length: %d\n", total);
    printf("A: %d  C: %d  G: %d  T: %d\n", a, c, g, t);
    printf("GC Content: %.1f%%\n", total > 0 ? (100.0 * gc / total) : 0.0);
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

void export_stats_json(const char *filename, const char *sequence) {
    int a, c, g, t;

    count_bases(sequence, &a, &c, &g, &t);

    int total = a + c + g + t;
    int gc = c + g;

    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Failed to write JSON stats to file: %s\n", filename);
        return;
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"sequence\": \"%s\",\n", sequence);
    fprintf(file, "  \"length\": %d,\n", total);
    fprintf(file, "  \"A\": %d,\n", a);
    fprintf(file, "  \"C\": %d,\n", c);
    fprintf(file, "  \"G\": %d,\n", g);
    fprintf(file, "  \"T\": %d,\n", t);
    fprintf(file, "  \"gc_percent\": %.1f\n", total > 0 ? (100.0 * gc / total) : 0.0);
    fprintf(file, "}\n");

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

            i = i + 2;
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

void decrypt_hex_with_dna_key(const char *sequence, const char *hex_string) {
    unsigned char key[16];

    derive_key_bytes(sequence, key);

    printf("\n=== Decrypted Output ===\n\n");

    printf("Cipherhex : %s\n", hex_string);
    printf("Plaintext : ");

    int len = strlen(hex_string);

    for (int i = 0; i + 1 < len; i = i + 2) {
        char hex_byte[3];

        hex_byte[0] = hex_string[i];
        hex_byte[1] = hex_string[i + 1];
        hex_byte[2] = '\0';

        unsigned int value;
        sscanf(hex_byte, "%02X", &value);

        unsigned char decrypted = (unsigned char)value ^ key[(i / 2) % 16];
        printf("%c", decrypted);
    }

    printf("\n");
}

void print_qrcode(const char *sequence) {
    printf("\n=== QR Code ===\n\n");

    int len = strlen(sequence);
    int size = 21;
    int block = 0;

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (block >= len) {
                printf("[ ]");
            } else {
                char bit = sequence[block] % 2;

                if (bit == 1) {
                    printf("[#]");
                } else {
                    printf("[ ]");
                }

                block++;
            }
        }

        printf("\n");
    }
}

void print_bar(const char *label, int count, int max, const char *color, int no_color) {
    int bar_length = 0;

    if (max > 0) {
        bar_length = (count * BAR_WIDTH) / max;
    }

    if (!no_color) {
        printf("%s", color);
    }

    printf("%s: ", label);

    for (int i = 0; i < bar_length; i++) {
        printf("#");
    }

    for (int i = bar_length; i < BAR_WIDTH; i++) {
        printf(" ");
    }

    printf(" (%d)", count);

    if (!no_color) {
        printf("\033[0m");
    }

    printf("\n");
}

void print_bars(const char *sequence, int no_color) {
    int a;
    int c;
    int g;
    int t;

    count_bases(sequence, &a, &c, &g, &t);

    int max = a;

    if (c > max) {
        max = c;
    }

    if (g > max) {
        max = g;
    }

    if (t > max) {
        max = t;
    }

    printf("\n=== DNA Base Distribution Bars ===\n\n");

    print_bar("A", a, max, "\033[31m", no_color);
    print_bar("C", c, max, "\033[32m", no_color);
    print_bar("G", g, max, "\033[34m", no_color);
    print_bar("T", t, max, "\033[33m", no_color);
}

void compress_sequence(const char *input, char *output) {
    int i = 0;
    int out = 0;

    while (input[i] != '\0') {
        char base = input[i];
        int count = 1;

        while (input[i + count] != '\0' && input[i + count] == base) {
            count++;
        }

        output[out++] = base;

        int digits = 0;
        int temp = count;

        do {
            digits++;
            temp = temp / 10;
        } while (temp > 0);

        for (int d = digits - 1; d >= 0; d--) {
            int digit = (count / (int)pow(10, d)) % 10;
            output[out++] = '0' + digit;
        }

        i = i + count;
    }

    output[out] = '\0';
}

void decompress_sequence(const char *input, char *output) {
    int in = 0;
    int out = 0;

    while (input[in] != '\0') {
        char base = input[in];

        if (!is_valid_base(base)) {
            output[out] = '\0';
            return;
        }

        in++;

        int count = 0;

        while (isdigit(input[in])) {
            count = count * 10 + (input[in] - '0');
            in++;
        }

        if (count == 0) {
            output[out] = '\0';
            return;
        }

        for (int i = 0; i < count && out < MAX_DNA_LENGTH - 1; i++) {
            output[out++] = base;
        }
    }

    output[out] = '\0';
}

void print_compressed(const char *sequence) {
    char compressed[MAX_COMPRESSED_LENGTH];

    compress_sequence(sequence, compressed);

    printf("\n=== Compressed Sequence ===\n\n");
    printf("%s\n", compressed);
}

void print_decompressed(const char *sequence) {
    char decompressed[MAX_DNA_LENGTH];

    decompress_sequence(sequence, decompressed);

    printf("\n=== Decompressed Sequence ===\n\n");
    printf("%s\n", decompressed);
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
    config.do_qrcode = 0;
    config.do_bars = 0;
    config.do_compress = 0;
    config.do_decompress = 0;
    config.do_export_stats = 0;
    config.mutate_count = 0;
    config.random_length = 0;
    config.compare_mode = 0;
    config.encrypt_mode = 0;
    config.decrypt_mode = 0;
    config.no_color = 0;

    config.input_file[0] = '\0';
    config.csv_file[0] = '\0';
    config.export_stats_file[0] = '\0';
    config.compare_seq1[0] = '\0';
    config.compare_seq2[0] = '\0';
    config.encrypt_text[0] = '\0';
    config.decrypt_hex[0] = '\0';

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
        } else if (strcmp(argv[i], "--qrcode") == 0) {
            config.do_qrcode = 1;
        } else if (strcmp(argv[i], "--bars") == 0) {
            config.do_bars = 1;
        } else if (strcmp(argv[i], "--compress") == 0) {
            config.do_compress = 1;
        } else if (strcmp(argv[i], "--decompress") == 0) {
            config.do_decompress = 1;
        } else if (strcmp(argv[i], "--export-stats") == 0 && i + 1 < argc) {
            strncpy(config.export_stats_file, argv[++i], MAX_FILENAME_LENGTH - 1);
            config.do_export_stats = 1;
        } else if (strcmp(argv[i], "--no-color") == 0) {
            config.no_color = 1;
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
        } else if (strcmp(argv[i], "--decrypt") == 0 && i + 1 < argc) {
            strncpy(config.decrypt_hex, argv[++i], MAX_TEXT_LENGTH - 1);
            config.decrypt_mode = 1;
        }
    }

    return config;
}

void process_sequence(char *sequence, options config) {
    char work_seq[MAX_DNA_LENGTH];

    strcpy(work_seq, sequence);

    if (config.do_decompress == 1) {
        char decompressed[MAX_DNA_LENGTH];

        decompress_sequence(work_seq, decompressed);

        printf("\n=== Decompressed Sequence ===\n\n");
        printf("%s\n", decompressed);

        strcpy(work_seq, decompressed);
    } else {
        clean_sequence(work_seq);
    }

    if (config.mutate_count > 0) {
        mutate_sequence(work_seq, config.mutate_count);
    }

    if (config.do_complement == 1) {
        make_complement(work_seq);
    }

    if (config.do_reverse == 1) {
        reverse_sequence(work_seq);
    }

    if (config.show_json == 1) {
        print_json(work_seq);
    }

    if (config.do_binary == 1) {
        print_binary(work_seq);
    }

    if (config.do_hex == 1) {
        print_hex(work_seq);
    }

    if (config.do_key == 1) {
        derive_key(work_seq);
    }

    if (config.do_hash == 1) {
        derive_hash(work_seq);
    }

    if (config.encrypt_mode == 1) {
        encrypt_text_with_dna_key(work_seq, config.encrypt_text);
    }

    if (config.decrypt_mode == 1) {
        decrypt_hex_with_dna_key(work_seq, config.decrypt_hex);
    }

    if (config.do_qrcode == 1) {
        print_qrcode(work_seq);
    }

    if (config.do_bars == 1) {
        print_bars(work_seq, config.no_color);
    }

    if (config.do_compress == 1) {
        print_compressed(work_seq);
    }

    if (config.show_ascii == 1) {
        print_sequence(work_seq);
    }

    if (config.show_stats == 1) {
        print_stats(work_seq);
    }

    if (config.show_summary == 1) {
        print_summary(work_seq);
    }

    if (config.do_csv == 1) {
        export_csv(config.csv_file, work_seq);
    }

    if (config.do_export_stats == 1) {
        export_stats_json(config.export_stats_file, work_seq);
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

    process_sequence(config.compare_seq1, config);
    process_sequence(config.compare_seq2, config);
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
