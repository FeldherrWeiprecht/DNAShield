#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

#define MAX_DNA_LENGTH 1024
#define MAX_FILENAME_LENGTH 256
#define MAX_TEXT_LENGTH 512
#define BAR_WIDTH 40
#define BAR_HEIGHT 20
#define MAX_COMPRESSED_LENGTH (MAX_DNA_LENGTH * 3)
#define KEY_SIZE 16
#define MAX_MATCHES 128

#define PROGRAM_VERSION "1.0.0"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

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
    int do_histogram;
    int histogram_vertical;
    int do_compress;
    int do_decompress;
    int do_export_stats;
    int do_find;
    int do_complexity;
    int mutate_count;
    int errors_count;
    int random_length;
    int compare_mode;
    int encrypt_mode;
    int decrypt_mode;
    int encrypt_file_mode;
    int decrypt_file_mode;
    int stdin_mode;
    int no_color;
    int show_version;
    int do_benchmark;
    int do_translate;
    int rotate_n;

    char input_file[MAX_FILENAME_LENGTH];
    char output_file[MAX_FILENAME_LENGTH];
    char csv_file[MAX_FILENAME_LENGTH];
    char export_stats_file[MAX_FILENAME_LENGTH];
    char compare_seq1[MAX_DNA_LENGTH];
    char compare_seq2[MAX_DNA_LENGTH];
    char find_pattern[MAX_DNA_LENGTH];
    char encrypt_text[MAX_TEXT_LENGTH];
    char decrypt_hex[MAX_TEXT_LENGTH];
    char encrypt_file_input[MAX_FILENAME_LENGTH];
    char encrypt_file_output[MAX_FILENAME_LENGTH];
    char decrypt_file_input[MAX_FILENAME_LENGTH];
    char decrypt_file_output[MAX_FILENAME_LENGTH];
} options;

int is_valid_base(char base) 
{
    if (base == 'A') 
    {
        return 1;
    }

    if (base == 'C') 
    {
        return 1;
    }

    if (base == 'G') 
    {
        return 1;
    }

    if (base == 'T') 
    {
        return 1;
    }

    return 0;
}

char complement_base(char base) 
{
    if (base == 'A') 
    {
        return 'T';
    }

    if (base == 'T') 
    {
        return 'A';
    }

    if (base == 'C') 
    {
        return 'G';
    }

    if (base == 'G') 
    {
        return 'C';
    }

    return '?';
}

void reverse_sequence(char *sequence) 
{
    int length = strlen(sequence);

    for (int i = 0; i < length / 2; i++) 
    {
        char temp = sequence[i];
        sequence[i] = sequence[length - 1 - i];
        sequence[length - 1 - i] = temp;
    }
}

void make_complement(char *sequence) 
{
    for (int i = 0; sequence[i] != '\0'; i++) 
    {
        sequence[i] = complement_base(sequence[i]);
    }
}

char random_base(char exclude) 
{
    char bases[] = { 'A', 'C', 'G', 'T' };
    char new_base;

    do 
    {
        new_base = bases[rand() % 4];
    } 
    while (new_base == exclude);

    return new_base;
}

void mutate_sequence(char *sequence, int count) 
{
    int length = strlen(sequence);

    if (length == 0) 
    {
        return;
    }

    if (count <= 0) 
    {
        return;
    }

    int mutated[MAX_DNA_LENGTH] = { 0 };
    int done = 0;

    while (done < count) 
    {
        int pos = rand() % length;

        if (mutated[pos] == 0) 
        {
            if (is_valid_base(sequence[pos])) 
            {
                sequence[pos] = random_base(sequence[pos]);
                mutated[pos] = 1;
                done++;
            }
        }
    }
}

void inject_errors(char *sequence, int count) 
{
    int length = strlen(sequence);

    if (length == 0) 
    {
        return;
    }

    if (count <= 0) 
    {
        return;
    }

    int errored[MAX_DNA_LENGTH] = { 0 };
    int done = 0;

    while (done < count) 
    {
        int pos = rand() % length;

        if (errored[pos] == 0) 
        {
            if (is_valid_base(sequence[pos])) 
            {
                sequence[pos] = random_base(sequence[pos]);
                errored[pos] = 1;
                done++;
            }
        }
    }
}

void generate_random_sequence(char *sequence, int length) 
{
    char bases[] = { 'A', 'C', 'G', 'T' };

    for (int i = 0; i < length && i < MAX_DNA_LENGTH - 1; i++) 
    {
        sequence[i] = bases[rand() % 4];
    }

    if (length < MAX_DNA_LENGTH - 1) 
    {
        sequence[length] = '\0';
    } 
    else 
    {
        sequence[MAX_DNA_LENGTH - 1] = '\0';
    }
}

void clean_sequence(char *sequence) 
{
    int j = 0;

    for (int i = 0; sequence[i] != '\0'; i++) 
    {
        char base = toupper(sequence[i]);

        if (is_valid_base(base)) 
        {
            sequence[j] = base;
            j++;
        }
    }

    sequence[j] = '\0';
}

int count_differences(const char *s1, const char *s2) 
{
    int diff = 0;
    int i = 0;

    while (s1[i] != '\0' && s2[i] != '\0') 
    {
        if (s1[i] != s2[i]) 
        {
            diff++;
        }

        i++;
    }

    while (s1[i] != '\0' || s2[i] != '\0') 
    {
        diff++;
        i++;
    }

    return diff;
}

void count_bases(const char *sequence, int *a, int *c, int *g, int *t) 
{
    *a = 0;
    *c = 0;
    *g = 0;
    *t = 0;

    for (int i = 0; sequence[i] != '\0'; i++) 
    {
        if (sequence[i] == 'A') 
        {
            (*a)++;
        } 
        else if (sequence[i] == 'C') 
        {
            (*c)++;
        } 
        else if (sequence[i] == 'G') 
        {
            (*g)++;
        } 
        else if (sequence[i] == 'T') 
        {
            (*t)++;
        }
    }
}

void print_base(char base) 
{
    if (base == 'A') 
    {
        printf("[A]");
    } 
    else if (base == 'C') 
    {
        printf("[C]");
    } 
    else if (base == 'G') 
    {
        printf("[G]");
    } 
    else if (base == 'T') 
    {
        printf("[T]");
    } 
    else 
    {
        printf("[?]");
    }
}

void print_sequence(const char *sequence) 
{
    printf("\n=== ASCII View ===\n\n");

    int count = 0;

    for (int i = 0; sequence[i] != '\0'; i++) 
    {
        print_base(sequence[i]);
        count++;

        if (count % 32 == 0) 
        {
            printf("\n");
        }
    }

    if (count % 32 != 0) 
    {
        printf("\n");
    }
}

void print_match_marked(const char *seq, int seq_len, int start, int pat_len, int color) 
{
    for (int i = 0; i < seq_len; i++) 
    {
        if (i >= start && i < start + pat_len) 
        {
            if (!color) 
            {
                print_base(seq[i]);
            } 
            else 
            {
                printf("\033[42;30m");
                print_base(seq[i]);
                printf("\033[0m");
            }
        } 
        else 
        {
            print_base(seq[i]);
        }
    }

    printf("\n");
}

void find_pattern(const char *sequence, const char *pattern, int color) 
{
    int seq_len = strlen(sequence);
    int pat_len = strlen(pattern);
    int positions[MAX_MATCHES];
    int match_count = 0;

    if (pat_len == 0) 
    {
        printf("\n=== Pattern Search: \"%s\" ===\n\n", pattern);
        printf("No matches found.\n");
        return;
    }

    if (seq_len == 0) 
    {
        printf("\n=== Pattern Search: \"%s\" ===\n\n", pattern);
        printf("No matches found.\n");
        return;
    }

    if (pat_len > seq_len) 
    {
        printf("\n=== Pattern Search: \"%s\" ===\n\n", pattern);
        printf("No matches found.\n");
        return;
    }

    char upper_pattern[MAX_DNA_LENGTH];
    for (int i = 0; i < pat_len; i++) 
    {
        upper_pattern[i] = toupper(pattern[i]);
    }
    upper_pattern[pat_len] = '\0';

    printf("\n=== Pattern Search: \"%s\" ===\n\n", pattern);

    for (int i = 0; i <= seq_len - pat_len && match_count < MAX_MATCHES; i++) 
    {
        int match = 1;

        for (int j = 0; j < pat_len; j++) 
        {
            if (toupper(sequence[i + j]) != upper_pattern[j]) 
            {
                match = 0;
                break;
            }
        }

        if (match) 
        {
            positions[match_count] = i;
            match_count++;
        }
    }

    if (match_count == 0) 
    {
        printf("No matches found.\n");
        return;
    }

    printf("Found %d match(es):\n\n", match_count);

    for (int m = 0; m < match_count; m++) 
    {
        printf("%d. Position: %d\n", m + 1, positions[m] + 1);
        print_match_marked(sequence, seq_len, positions[m], pat_len, color ? 1 : 0);
        printf("\n");
    }
}

void print_stats(const char *sequence) 
{
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

void print_summary(const char *sequence) 
{
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

    if (total > 0) 
    {
        printf("GC Content: %.1f%%\n", (100.0 * gc / total));
    } 
    else 
    {
        printf("GC Content: 0.0%%\n");
    }
}

void print_json(const char *sequence) 
{
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

    if (total > 0) 
    {
        printf("  \"gc_percent\": %.1f\n", (100.0 * gc / total));
    } 
    else 
    {
        printf("  \"gc_percent\": 0.0\n");
    }

    printf("}\n");
}

void export_csv(const char *filename, const char *sequence) 
{
    int count_a;
    int count_c;
    int count_g;
    int count_t;

    count_bases(sequence, &count_a, &count_c, &count_g, &count_t);

    int total = count_a + count_c + count_g + count_t;
    int gc = count_g + count_c;

    FILE *file = fopen(filename, "a");

    if (file == NULL) 
    {
        printf("Failed to write to file: %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);

    if (size == 0) 
    {
        fprintf(file, "sequence,length,A,C,G,T,gc_percent\n");
    }

    fprintf(file, "%s,%d,%d,%d,%d,%d,", sequence, total, count_a, count_c, count_g, count_t);

    if (total > 0) 
    {
        fprintf(file, "%.1f\n", (100.0 * gc / total));
    } 
    else 
    {
        fprintf(file, "0.0\n");
    }

    fclose(file);
}

void export_stats_json(const char *filename, const char *sequence) 
{
    int a, c, g, t;

    count_bases(sequence, &a, &c, &g, &t);

    int total = a + c + g + t;
    int gc = c + g;

    FILE *file = fopen(filename, "w");

    if (file == NULL) 
    {
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

    if (total > 0) 
    {
        fprintf(file, "  \"gc_percent\": %.1f\n", (100.0 * gc / total));
    } 
    else 
    {
        fprintf(file, "  \"gc_percent\": 0.0\n");
    }

    fprintf(file, "}\n");

    fclose(file);
}

void print_binary(const char *sequence) 
{
    printf("\n=== Binary Output ===\n\n");

    for (int i = 0; sequence[i] != '\0'; i++) 
    {
        if (sequence[i] == 'A') 
        {
            printf("00");
        } 
        else if (sequence[i] == 'C') 
        {
            printf("01");
        } 
        else if (sequence[i] == 'G') 
        {
            printf("10");
        } 
        else if (sequence[i] == 'T') 
        {
            printf("11");
        } 
        else 
        {
            printf("??");
        }
    }

    printf("\n");
}

void print_hex(const char *sequence) 
{
    printf("\n=== Hex Output ===\n\n");

    int i = 0;

    while (sequence[i] != '\0') 
    {
        char code1 = 0;
        char code2 = 0;

        if (sequence[i] == 'A') 
        {
            code1 = 0;
        } 
        else if (sequence[i] == 'C') 
        {
            code1 = 1;
        } 
        else if (sequence[i] == 'G') 
        {
            code1 = 2;
        } 
        else if (sequence[i] == 'T') 
        {
            code1 = 3;
        }

        if (sequence[i + 1] != '\0') 
        {
            if (sequence[i + 1] == 'A') 
            {
                code2 = 0;
            } 
            else if (sequence[i + 1] == 'C') 
            {
                code2 = 1;
            } 
            else if (sequence[i + 1] == 'G') 
            {
                code2 = 2;
            } 
            else if (sequence[i + 1] == 'T') 
            {
                code2 = 3;
            }

            unsigned char combined = (code1 << 2) | code2;
            printf("%X", combined);

            i = i + 2;
        } 
        else 
        {
            printf("%X", code1 << 2);
            break;
        }
    }

    printf("\n");
}

void derive_key_bytes(const char *sequence, unsigned char *key_out) 
{
    for (int i = 0; i < KEY_SIZE; i++) 
    {
        key_out[i] = 0;
    }

    for (int i = 0; sequence[i] != '\0'; i++) 
    {
        unsigned char value = 0;

        if (sequence[i] == 'A') 
        {
            value = 0x1;
        } 
        else if (sequence[i] == 'C') 
        {
            value = 0x3;
        } 
        else if (sequence[i] == 'G') 
        {
            value = 0x7;
        } 
        else if (sequence[i] == 'T') 
        {
            value = 0xD;
        }

        key_out[i % KEY_SIZE] ^= value ^ (i * 31);
    }
}

void derive_key(const char *sequence) 
{
    unsigned char key[KEY_SIZE];

    derive_key_bytes(sequence, key);

    printf("\n=== Derived Key ===\n\n");

    printf("DNA Key (32 chars): ");

    for (int i = 0; i < KEY_SIZE; i++) 
    {
        printf("%02X", key[i]);
    }

    printf("\n");
}

void derive_hash(const char *sequence) 
{
    unsigned char hash[32] = { 0 };

    for (int i = 0; sequence[i] != '\0'; i++) 
    {
        unsigned char value = 0;

        if (sequence[i] == 'A') 
        {
            value = 0xA3;
        } 
        else if (sequence[i] == 'C') 
        {
            value = 0xC5;
        } 
        else if (sequence[i] == 'G') 
        {
            value = 0xF1;
        } 
        else if (sequence[i] == 'T') 
        {
            value = 0x9E;
        }

        hash[i % 32] ^= value ^ (i * 17);
        hash[(i * 3) % 32] ^= (value << (i % 5)) | (value >> ((8 - (i % 5)) % 8));
    }

    printf("\n=== Hash ===\n\n");

    printf("DNA Hash (64 chars): ");

    for (int i = 0; i < 32; i++) 
    {
        printf("%02X", hash[i]);
    }

    printf("\n");
}

void encrypt_text_with_dna_key(const char *sequence, const char *text) 
{
    unsigned char key[KEY_SIZE];

    derive_key_bytes(sequence, key);

    printf("\n=== Encrypted Output ===\n\n");

    printf("Plaintext : %s\n", text);
    printf("Cipherhex : ");

    for (int i = 0; text[i] != '\0'; i++) 
    {
        unsigned char encrypted = text[i] ^ key[i % KEY_SIZE];
        printf("%02X", encrypted);
    }

    printf("\n");
}

void decrypt_hex_with_dna_key(const char *sequence, const char *hex_string) 
{
    unsigned char key[KEY_SIZE];

    derive_key_bytes(sequence, key);

    printf("\n=== Decrypted Output ===\n\n");

    printf("Cipherhex : %s\n", hex_string);
    printf("Plaintext : ");

    int len = strlen(hex_string);

    for (int i = 0; i + 1 < len; i = i + 2) 
    {
        char hex_byte[3];

        hex_byte[0] = hex_string[i];
        hex_byte[1] = hex_string[i + 1];
        hex_byte[2] = '\0';

        unsigned int value;
        sscanf(hex_byte, "%02X", &value);

        unsigned char decrypted = (unsigned char)value ^ key[(i / 2) % KEY_SIZE];
        printf("%c", decrypted);
    }

    printf("\n");
}

void print_qrcode(const char *sequence) 
{
    printf("\n=== QR Code ===\n\n");

    int len = strlen(sequence);
    int size = 21;
    int block = 0;

    for (int y = 0; y < size; y++) 
    {
        for (int x = 0; x < size; x++) 
        {
            if (block >= len) 
            {
                printf("[ ]");
            } 
            else 
            {
                char bit = sequence[block] % 2;

                if (bit == 1) 
                {
                    printf("[#]");
                } 
                else 
                {
                    printf("[ ]");
                }

                block++;
            }
        }

        printf("\n");
    }
}

void print_histogram_horizontal(const char *sequence, int no_color) 
{
    int a;
    int c;
    int g;
    int t;

    count_bases(sequence, &a, &c, &g, &t);

    int max = a;

    if (c > max) 
    {
        max = c;
    }

    if (g > max) 
    {
        max = g;
    }

    if (t > max) 
    {
        max = t;
    }

    int scaled_a;
    int scaled_c;
    int scaled_g;
    int scaled_t;

    if (max <= BAR_WIDTH) 
    {
        scaled_a = a;
        scaled_c = c;
        scaled_g = g;
        scaled_t = t;
    } 
    else 
    {
        scaled_a = (a * BAR_WIDTH) / max;
        scaled_c = (c * BAR_WIDTH) / max;
        scaled_g = (g * BAR_WIDTH) / max;
        scaled_t = (t * BAR_WIDTH) / max;
    }

    if (!no_color) 
    {
        printf("\033[31m");
    }

    printf("A: ");

    for (int i = 0; i < scaled_a; i++) 
    {
        printf("#");
    }

    for (int i = scaled_a; i < BAR_WIDTH; i++) 
    {
        printf(" ");
    }

    if (!no_color) 
    {
        printf("\033[0m");
    }

    printf(" (%d)\n", a);

    if (!no_color) 
    {
        printf("\033[32m");
    }

    printf("C: ");

    for (int i = 0; i < scaled_c; i++) 
    {
        printf("#");
    }

    for (int i = scaled_c; i < BAR_WIDTH; i++) 
    {
        printf(" ");
    }

    if (!no_color) 
    {
        printf("\033[0m");
    }

    printf(" (%d)\n", c);

    if (!no_color) 
    {
        printf("\033[34m");
    }

    printf("G: ");

    for (int i = 0; i < scaled_g; i++) 
    {
        printf("#");
    }

    for (int i = scaled_g; i < BAR_WIDTH; i++) 
    {
        printf(" ");
    }

    if (!no_color) 
    {
        printf("\033[0m");
    }

    printf(" (%d)\n", g);

    if (!no_color) 
    {
        printf("\033[33m");
    }

    printf("T: ");

    for (int i = 0; i < scaled_t; i++) 
    {
        printf("#");
    }

    for (int i = scaled_t; i < BAR_WIDTH; i++) 
    {
        printf(" ");
    }

    if (!no_color) 
    {
        printf("\033[0m");
    }

    printf(" (%d)\n", t);
}

void print_histogram_vertical(const char *sequence, int no_color)
{
    int a, c, g, t;
    count_bases(sequence, &a, &c, &g, &t);

    int max = a;

    if (c > max) 
    {
        max = c;
    }

    if (g > max) 
    {
        max = g;
    }

    if (t > max) 
    {
        max = t;
    }

    int height = BAR_HEIGHT;

    if (max < BAR_HEIGHT && max > 0)
    {
        height = max;
    }

    printf("\n=== DNA Base Distribution Histogram (vertical) ===\n\n");

    for (int row = height; row > 0; row--) 
    {
        if (!no_color) 
        {
            printf("\033[31m");
        }

        printf(" ");

        if (a >= row) 
        {
            printf("#");
        } 
        else 
        {
            printf(" ");
        }

        if (!no_color) 
        {
            printf("\033[0m");
        }

        if (!no_color) 
        {
            printf("\033[32m");
        }

        printf(" ");

        if (c >= row) 
        {
            printf("#");
        } 
        else 
        {
            printf(" ");
        }

        if (!no_color) 
        {
            printf("\033[0m");
        }

        if (!no_color) 
        {
            printf("\033[34m");
        }

        printf(" ");

        if (g >= row) 
        {
            printf("#");
        } 
        else 
        {
            printf(" ");
        }

        if (!no_color) 
        {
            printf("\033[0m");
        }

        if (!no_color) 
        {
            printf("\033[33m");
        }

        printf(" ");

        if (t >= row) 
        {
            printf("#");
        } 
        else 
        {
            printf(" ");
        }

        if (!no_color) 
        {
            printf("\033[0m");
        }

        printf("\n");
    }

    printf(" A C G T\n");

    printf("(%d %d %d %d)\n", a, c, g, t);
}

void compress_sequence(const char *input, char *output) 
{
    int i = 0;
    int out = 0;

    while (input[i] != '\0') 
    {
        char base = input[i];
        int count = 1;

        while (input[i + count] != '\0' && input[i + count] == base) 
        {
            count++;
        }

        output[out++] = base;

        int digits = 0;
        int temp = count;

        do 
        {
            digits++;
            temp = temp / 10;
        } 
        while (temp > 0);

        for (int d = digits - 1; d >= 0; d--) 
        {
            int digit = (count / (int)pow(10, d)) % 10;
            output[out++] = '0' + digit;
        }

        i = i + count;
    }

    output[out] = '\0';
}

void decompress_sequence(const char *input, char *output) 
{
    int in = 0;
    int out = 0;

    while (input[in] != '\0') 
    {
        char base = input[in];

        if (!is_valid_base(base)) 
        {
            output[out] = '\0';
            return;
        }

        in++;

        int count = 0;

        while (isdigit(input[in])) 
        {
            count = count * 10 + (input[in] - '0');
            in++;
        }

        if (count == 0) 
        {
            output[out] = '\0';
            return;
        }

        for (int i = 0; i < count && out < MAX_DNA_LENGTH - 1; i++) 
        {
            output[out++] = base;
        }
    }

    output[out] = '\0';
}

void print_compressed(const char *sequence) 
{
    char compressed[MAX_COMPRESSED_LENGTH];

    compress_sequence(sequence, compressed);

    printf("\n=== Compressed Sequence ===\n\n");
    printf("%s\n", compressed);
}

void print_decompressed(const char *sequence) 
{
    char decompressed[MAX_DNA_LENGTH];

    decompress_sequence(sequence, decompressed);

    printf("\n=== Decompressed Sequence ===\n\n");
    printf("%s\n", decompressed);
}

void encrypt_file(const char *dna_sequence, const char *input_filename, const char *output_filename) 
{
    unsigned char key[KEY_SIZE];

    derive_key_bytes(dna_sequence, key);

    FILE *fin = fopen(input_filename, "rb");

    if (fin == NULL) 
    {
        printf("Error: Could not open input file '%s'\n", input_filename);
        return;
    }

    FILE *fout = fopen(output_filename, "wb");

    if (fout == NULL) 
    {
        fclose(fin);
        printf("Error: Could not open output file '%s'\n", output_filename);
        return;
    }

    printf("\n=== File Encryption ===\n\n");
    printf("Input : %s\n", input_filename);
    printf("Output: %s\n", output_filename);

    int i = 0;
    int ch;

    while ((ch = fgetc(fin)) != EOF) 
    {
        unsigned char encrypted = ((unsigned char)ch) ^ key[i % KEY_SIZE];
        fputc(encrypted, fout);
        i++;
    }

    fclose(fin);
    fclose(fout);

    printf("File encrypted successfully.\n");
}

void decrypt_file(const char *dna_sequence, const char *input_filename, const char *output_filename) 
{
    unsigned char key[KEY_SIZE];

    derive_key_bytes(dna_sequence, key);

    FILE *fin = fopen(input_filename, "rb");

    if (fin == NULL) 
    {
        printf("Error: Could not open input file '%s'\n", input_filename);
        return;
    }

    FILE *fout = fopen(output_filename, "wb");

    if (fout == NULL) 
    {
        fclose(fin);
        printf("Error: Could not open output file '%s'\n", output_filename);
        return;
    }

    printf("\n=== File Decryption ===\n\n");
    printf("Input : %s\n", input_filename);
    printf("Output: %s\n", output_filename);

    int i = 0;
    int ch;

    while ((ch = fgetc(fin)) != EOF) 
    {
        unsigned char decrypted = ((unsigned char)ch) ^ key[i % KEY_SIZE];
        fputc(decrypted, fout);
        i++;
    }

    fclose(fin);
    fclose(fout);

    printf("File decrypted successfully.\n");
}

void print_complexity(const char *sequence) 
{
    int a, c, g, t;
    count_bases(sequence, &a, &c, &g, &t);
    int total = a + c + g + t;

    if (total == 0) 
    {
        printf("\n=== Sequence Complexity ===\n\n");
        printf("Sequence is empty.\n");
        return;
    }

    double pa = (double)a / total;
    double pc = (double)c / total;
    double pg = (double)g / total;
    double pt = (double)t / total;

    double entropy = 0.0;

    if (pa > 0) 
    {
        entropy -= pa * log2(pa);
    }

    if (pc > 0) 
    {
        entropy -= pc * log2(pc);
    }

    if (pg > 0) 
    {
        entropy -= pg * log2(pg);
    }

    if (pt > 0) 
    {
        entropy -= pt * log2(pt);
    }

    printf("\n=== Sequence Complexity ===\n\n");
    printf("Shannon Entropy: %.4f bits/base (max: 2.0000)\n", entropy);
}

void print_version(const char *progname) 
{
    printf("\n%s version %s\n", progname, PROGRAM_VERSION);
    printf("Build date: %s %s\n\n", BUILD_DATE, BUILD_TIME);
}

void print_help(const char *progname)
{
    printf("\n%s - DNA Sequence Analysis Tool\n\n", progname);

    printf("Usage:\n");
    printf("  %s [options]\n\n", progname);

    printf("Options:\n");
    printf("  --ascii                 Show ASCII representation of DNA sequence (default on)\n");
    printf("  --stats                 Show base statistics (default on)\n");
    printf("  --summary               Show summary of sequence statistics\n");
    printf("  --json                  Output analysis in JSON format (disables ASCII, stats, summary)\n");
    printf("  --reverse               Reverse the DNA sequence before analysis\n");
    printf("  --complement            Use complement of DNA sequence before analysis\n");
    printf("  --binary                Output DNA sequence as binary code\n");
    printf("  --hex                   Output DNA sequence as hexadecimal code\n");
    printf("  --key                   Derive a 128-bit key from DNA sequence\n");
    printf("  --hash                  Output a 256-bit hash of the DNA sequence\n");
    printf("  --qrcode                Visualize DNA sequence as a symbolic QR code\n");
    printf("  --histogram             Show base distribution histogram (horizontal by default)\n");
    printf("  --histogram-vertical    Show base distribution histogram vertically\n");
    printf("  --histogram-horizontal  Show base distribution histogram horizontally\n");
    printf("  --compress              Compress the DNA sequence (run-length encoding)\n");
    printf("  --decompress            Decompress a run-length encoded DNA sequence\n");
    printf("  --export-stats <file>   Export statistics as JSON to file\n");
    printf("  --errors <N>            Introduce N random errors in the sequence\n");
    printf("  --no-color              Disable colored output\n");
    printf("  --csv <file>            Export sequence data to CSV file\n");
    printf("  --file <file>           Read sequences from a file (one per line)\n");
    printf("  --mutate <N>            Introduce N random point mutations\n");
    printf("  --random <length>       Generate a random DNA sequence of given length\n");
    printf("  --compare <seq1> <seq2> Compare two DNA sequences and count differences\n");
    printf("  --find <pattern>        Search for a subsequence pattern in DNA\n");
    printf("  --encrypt <text>        Encrypt text with DNA-derived key\n");
    printf("  --decrypt <hex>         Decrypt hex string with DNA-derived key\n");
    printf("  --encrypt-file <in> <out>  Encrypt a file using DNA key\n");
    printf("  --decrypt-file <in> <out>  Decrypt a file using DNA key\n");
    printf("  --stdin                 Read sequences from standard input\n");
    printf("  --complexity            Calculate sequence complexity (Shannon entropy)\n");
    printf("  --translate             Translate DNA sequence to amino acid sequence\n");
    printf("  --benchmark             Measure and display analysis time\n");
    printf("  --rotate <N>            Cyclically rotate DNA sequence by N bases\n");
    printf("  --version, -v           Show program version and build info\n");
    printf("  --help, -h              Show this help message\n\n");
}

options parse_args(int argc, char *argv[]) 
{
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
    config.do_histogram = 0;
    config.histogram_vertical = 0;
    config.do_compress = 0;
    config.do_decompress = 0;
    config.do_export_stats = 0;
    config.do_find = 0;
    config.do_complexity = 0;
    config.mutate_count = 0;
    config.errors_count = 0;
    config.random_length = 0;
    config.compare_mode = 0;
    config.encrypt_mode = 0;
    config.decrypt_mode = 0;
    config.encrypt_file_mode = 0;
    config.decrypt_file_mode = 0;
    config.stdin_mode = 0;
    config.no_color = 0;
    config.show_version = 0;
    config.do_benchmark = 0;
    config.do_translate = 0;
    config.rotate_n = 0;

    config.input_file[0] = '\0';
    config.output_file[0] = '\0';
    config.csv_file[0] = '\0';
    config.export_stats_file[0] = '\0';
    config.compare_seq1[0] = '\0';
    config.compare_seq2[0] = '\0';
    config.find_pattern[0] = '\0';
    config.encrypt_text[0] = '\0';
    config.decrypt_hex[0] = '\0';
    config.encrypt_file_input[0] = '\0';
    config.encrypt_file_output[0] = '\0';
    config.decrypt_file_input[0] = '\0';
    config.decrypt_file_output[0] = '\0';

    for (int i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "--ascii") == 0) 
        {
            config.show_ascii = 1;
        } 
        else if (strcmp(argv[i], "--stats") == 0) 
        {
            config.show_stats = 1;
        } 
        else if (strcmp(argv[i], "--summary") == 0) 
        {
            config.show_summary = 1;
        } 
        else if (strcmp(argv[i], "--json") == 0) 
        {
            config.show_json = 1;
            config.show_ascii = 0;
            config.show_stats = 0;
            config.show_summary = 0;
        } 
        else if (strcmp(argv[i], "--reverse") == 0) 
        {
            config.do_reverse = 1;
        } 
        else if (strcmp(argv[i], "--complement") == 0) 
        {
            config.do_complement = 1;
        } 
        else if (strcmp(argv[i], "--binary") == 0) 
        {
            config.do_binary = 1;
        } 
        else if (strcmp(argv[i], "--hex") == 0) 
        {
            config.do_hex = 1;
        } 
        else if (strcmp(argv[i], "--key") == 0) 
        {
            config.do_key = 1;
        } 
        else if (strcmp(argv[i], "--hash") == 0) 
        {
            config.do_hash = 1;
        } 
        else if (strcmp(argv[i], "--qrcode") == 0) 
        {
            config.do_qrcode = 1;
        } 
        else if (strcmp(argv[i], "--histogram") == 0) 
        {
            config.do_histogram = 1;
        } 
        else if (strcmp(argv[i], "--histogram-vertical") == 0) 
        {
            config.do_histogram = 1;
            config.histogram_vertical = 1;
        } 
        else if (strcmp(argv[i], "--histogram-horizontal") == 0) 
        {
            config.do_histogram = 1;
            config.histogram_vertical = 0;
        } 
        else if (strcmp(argv[i], "--compress") == 0) 
        {
            config.do_compress = 1;
        } 
        else if (strcmp(argv[i], "--decompress") == 0) 
        {
            config.do_decompress = 1;
        } 
        else if (strcmp(argv[i], "--export-stats") == 0 && i + 1 < argc) 
        {
            strncpy(config.export_stats_file, argv[++i], MAX_FILENAME_LENGTH - 1);
            config.do_export_stats = 1;
        } 
        else if (strcmp(argv[i], "--errors") == 0 && i + 1 < argc) 
        {
            config.errors_count = atoi(argv[++i]);
        } 
        else if (strcmp(argv[i], "--no-color") == 0) 
        {
            config.no_color = 1;
        } 
        else if (strcmp(argv[i], "--csv") == 0 && i + 1 < argc) 
        {
            strncpy(config.csv_file, argv[++i], MAX_FILENAME_LENGTH - 1);
            config.do_csv = 1;
        } 
        else if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) 
        {
            strncpy(config.input_file, argv[++i], MAX_FILENAME_LENGTH - 1);
            config.file_mode = 1;
        } 
        else if (strcmp(argv[i], "--mutate") == 0 && i + 1 < argc) 
        {
            config.mutate_count = atoi(argv[++i]);
        } 
        else if (strcmp(argv[i], "--random") == 0 && i + 1 < argc) 
        {
            config.random_length = atoi(argv[++i]);
        } 
        else if (strcmp(argv[i], "--compare") == 0 && i + 2 < argc) 
        {
            strncpy(config.compare_seq1, argv[++i], MAX_DNA_LENGTH - 1);
            strncpy(config.compare_seq2, argv[++i], MAX_DNA_LENGTH - 1);
            config.compare_mode = 1;
        } 
        else if (strcmp(argv[i], "--find") == 0 && i + 1 < argc) 
        {
            strncpy(config.find_pattern, argv[++i], MAX_DNA_LENGTH - 1);
            config.do_find = 1;
        } 
        else if (strcmp(argv[i], "--encrypt") == 0 && i + 1 < argc) 
        {
            strncpy(config.encrypt_text, argv[++i], MAX_TEXT_LENGTH - 1);
            config.encrypt_mode = 1;
        } 
        else if (strcmp(argv[i], "--decrypt") == 0 && i + 1 < argc) 
        {
            strncpy(config.decrypt_hex, argv[++i], MAX_TEXT_LENGTH - 1);
            config.decrypt_mode = 1;
        } 
        else if (strcmp(argv[i], "--encrypt-file") == 0 && i + 2 < argc) 
        {
            strncpy(config.encrypt_file_input, argv[++i], MAX_FILENAME_LENGTH - 1);
            strncpy(config.encrypt_file_output, argv[++i], MAX_FILENAME_LENGTH - 1);
            config.encrypt_file_mode = 1;
        } 
        else if (strcmp(argv[i], "--decrypt-file") == 0 && i + 2 < argc) 
        {
            strncpy(config.decrypt_file_input, argv[++i], MAX_FILENAME_LENGTH - 1);
            strncpy(config.decrypt_file_output, argv[++i], MAX_FILENAME_LENGTH - 1);
            config.decrypt_file_mode = 1;
        } 
        else if (strcmp(argv[i], "--stdin") == 0) 
        {
            config.stdin_mode = 1;
        } 
        else if (strcmp(argv[i], "--complexity") == 0) 
        {
            config.do_complexity = 1;
        } 
        else if (strcmp(argv[i], "--translate") == 0) 
        {
            config.do_translate = 1;
        }
        else if (strcmp(argv[i], "--rotate") == 0 && i + 1 < argc)
        {
            config.rotate_n = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) 
        {
            config.show_version = 1;
        }
        else if (strcmp(argv[i], "--benchmark") == 0)
        {
            config.do_benchmark = 1;
        }
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) 
        {
            print_help(argv[0]);
            exit(0);
        }
    }

    if (config.show_json == 0)
    {
        if (config.show_ascii == 0 && config.show_stats == 0 && config.show_summary == 0)
        {
            config.show_ascii = 1;
            config.show_stats = 1;
            config.show_summary = 1;
        }
        else
        {
            if (config.show_ascii == 0)
            {
                config.show_ascii = 1;
            }
            if (config.show_stats == 0)
            {
                config.show_stats = 1;
            }
            if (config.show_summary == 0)
            {
                config.show_summary = 1;
            }
        }
    }

    return config;
}

char translate_codon(const char *codon)
{
    char c1 = codon[0];
    char c2 = codon[1];
    char c3 = codon[2];

    if (c1 == 'T') 
    {
        if (c2 == 'T') 
        {
            if (c3 == 'T' || c3 == 'C') 
            {
                return 'F';
            }
            else if (c3 == 'A' || c3 == 'G') 
            {
                return 'L';
            }
        }
        else if (c2 == 'C') 
        {
            return 'S';
        }
        else if (c2 == 'A') 
        {
            if (c3 == 'T' || c3 == 'C') 
            {
                return 'Y';
            }
            else if (c3 == 'A' || c3 == 'G') 
            {
                return '*';
            }
        }
        else if (c2 == 'G') 
        {
            if (c3 == 'T' || c3 == 'C') 
            {
                return 'C';
            }
            else if (c3 == 'A') 
            {
                return '*';
            }
            else if (c3 == 'G') 
            {
                return 'W';
            }
        }
    }
    else if (c1 == 'C') 
    {
        if (c2 == 'T') 
        {
            return 'L';
        }
        else if (c2 == 'C') 
        {
            return 'P';
        }
        else if (c2 == 'A') 
        {
            if (c3 == 'T' || c3 == 'C') 
            {
                return 'H';
            }
            else if (c3 == 'A' || c3 == 'G') 
            {
                return 'Q';
            }
        }
        else if (c2 == 'G') 
        {
            return 'R';
        }
    }
    else if (c1 == 'A') 
    {
        if (c2 == 'T') 
        {
            if (c3 == 'T' || c3 == 'C' || c3 == 'A') 
            {
                return 'I';
            }
            else if (c3 == 'G') 
            {
                return 'M';
            }
        }
        else if (c2 == 'C') 
        {
            return 'T';
        }
        else if (c2 == 'A') 
        {
            if (c3 == 'T' || c3 == 'C') 
            {
                return 'N';
            }
            else if (c3 == 'A' || c3 == 'G') 
            {
                return 'K';
            }
        }
        else if (c2 == 'G') 
        {
            if (c3 == 'T' || c3 == 'C') 
            {
                return 'S';
            }
            else if (c3 == 'A' || c3 == 'G') 
            {
                return 'R';
            }
        }
    }
    else if (c1 == 'G') 
    {
        if (c2 == 'T') 
        {
            return 'V';
        }
        else if (c2 == 'C') 
        {
            return 'A';
        }
        else if (c2 == 'A') 
        {
            if (c3 == 'T' || c3 == 'C') 
            {
                return 'D';
            }
            else if (c3 == 'A' || c3 == 'G') 
            {
                return 'E';
            }
        }
        else if (c2 == 'G') 
        {
            return 'G';
        }
    }

    return 'X';
}

void translate_sequence(const char *sequence) 
{
    printf("\n=== Translation to Amino Acids ===\n\n");

    int len = strlen(sequence);
    int start_index = -1;
    int i;

    for (i = 0; i + 2 < len; i++) 
    {
        if (sequence[i] == 'A' && sequence[i + 1] == 'T' && sequence[i + 2] == 'G') 
        {
            start_index = i;
            break;
        }
    }

    if (start_index == -1) 
    {
        printf("No start codon found.\n");
        printf("\n");
        return;
    }

    for (i = start_index; i + 2 < len; i += 3) 
    {
        char codon[4];

        codon[0] = sequence[i];
        codon[1] = sequence[i + 1];
        codon[2] = sequence[i + 2];
        codon[3] = '\0';

        char aa = translate_codon(codon);

        if (aa == '*') 
        {
            break;
        }

        printf("%c", aa);
    }

    printf("\n");
}

void rotate_sequence(char *sequence, int n)
{
    int length = strlen(sequence);

    if (length == 0) 
    {
        return;
    }

    if (n < 0) 
    {
        n = length + (n % length);
    } 
    else 
    {
        n = n % length;
    }

    if (n == 0) 
    {
        return;
    }

    char temp[MAX_DNA_LENGTH];
    int i;

    for (i = 0; i < length; i++) 
    {
        temp[(i + n) % length] = sequence[i];
    }

    for (i = 0; i < length; i++) 
    {
        sequence[i] = temp[i];
    }

    sequence[length] = '\0';
}

void process_sequence(char *sequence, options config) 
{
    char work_seq[MAX_DNA_LENGTH];

    strcpy(work_seq, sequence);

    if (config.do_decompress == 1) 
    {
        char decompressed[MAX_DNA_LENGTH];

        decompress_sequence(work_seq, decompressed);

        printf("\n=== Decompressed Sequence ===\n\n");
        printf("%s\n", decompressed);

        strcpy(work_seq, decompressed);
    } 
    else 
    {
        clean_sequence(work_seq);
    }

    if (config.mutate_count > 0) 
    {
        mutate_sequence(work_seq, config.mutate_count);
    }

    if (config.errors_count > 0) 
    {
        inject_errors(work_seq, config.errors_count);
    }

    if (config.do_complement == 1) 
    {
        make_complement(work_seq);
    }

    if (config.do_reverse == 1) 
    {
        reverse_sequence(work_seq);
    }

    if (config.rotate_n != 0) 
    {
        rotate_sequence(work_seq, config.rotate_n);
    }

    if (config.do_find == 1 && config.find_pattern[0] != '\0') 
    {
        find_pattern(work_seq, config.find_pattern, config.no_color ? 0 : 1);
    }

    if (config.show_json == 1) 
    {
        print_json(work_seq);
    }

    if (config.do_binary == 1) 
    {
        print_binary(work_seq);
    }

    if (config.do_hex == 1) 
    {
        print_hex(work_seq);
    }

    if (config.do_key == 1) 
    {
        derive_key(work_seq);
    }

    if (config.do_hash == 1) 
    {
        derive_hash(work_seq);
    }

    if (config.encrypt_mode == 1) 
    {
        encrypt_text_with_dna_key(work_seq, config.encrypt_text);
    }

    if (config.decrypt_mode == 1) 
    {
        decrypt_hex_with_dna_key(work_seq, config.decrypt_hex);
    }

    if (config.do_qrcode == 1) 
    {
        print_qrcode(work_seq);
    }

    if (config.do_histogram == 1) 
    {
        if (config.histogram_vertical) 
        {
            print_histogram_vertical(work_seq, config.no_color);
        } 
        else 
        {
            print_histogram_horizontal(work_seq, config.no_color);
        }
    }

    if (config.do_compress == 1) 
    {
        print_compressed(work_seq);
    }

    if (config.do_complexity == 1) 
    {
        print_complexity(work_seq);
    }

    if (config.do_translate == 1) 
    {
        translate_sequence(work_seq);
    }

    if (config.show_ascii == 1) 
    {
        print_sequence(work_seq);
    }

    if (config.show_stats == 1) 
    {
        print_stats(work_seq);
    }

    if (config.show_summary == 1) 
    {
        print_summary(work_seq);
    }

    if (config.do_csv == 1) 
    {
        export_csv(config.csv_file, work_seq);
    }

    if (config.do_export_stats == 1) 
    {
        export_stats_json(config.export_stats_file, work_seq);
    }

    printf("\n");
}

void run_compare_mode(options config) 
{
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

int main(int argc, char *argv[]) 
{
    srand(time(NULL));

    char sequence[MAX_DNA_LENGTH];

    options config = parse_args(argc, argv);

    if (config.show_version == 1) 
    {
        print_version(argv[0]);
        return 0;
    }

    if (config.compare_mode == 1) 
    {
        run_compare_mode(config);
        return 0;
    }

    if (config.random_length > 0) 
    {
        generate_random_sequence(sequence, config.random_length);
        process_sequence(sequence, config);
        return 0;
    }

    if (config.encrypt_file_mode == 1) 
    {
        printf("\nPlease enter a DNA sequence (for encryption):\n\n");

        if (fgets(sequence, sizeof(sequence), stdin) == NULL) 
        {
            printf("Error: Failed to read input.\n");
            return 1;
        }

        sequence[strcspn(sequence, "\n")] = '\0';
        clean_sequence(sequence);
        encrypt_file(sequence, config.encrypt_file_input, config.encrypt_file_output);
        return 0;
    }

    if (config.decrypt_file_mode == 1) 
    {
        printf("\nPlease enter a DNA sequence (for decryption):\n\n");

        if (fgets(sequence, sizeof(sequence), stdin) == NULL) 
        {
            printf("Error: Failed to read input.\n");
            return 1;
        }

        sequence[strcspn(sequence, "\n")] = '\0';
        clean_sequence(sequence);
        decrypt_file(sequence, config.decrypt_file_input, config.decrypt_file_output);
        return 0;
    }

    clock_t start_time;
    clock_t end_time;

    if (config.do_benchmark == 1) 
    {
        start_time = clock();
    }

    if (config.file_mode == 1) 
    {
        FILE *file = fopen(config.input_file, "r");

        if (file == NULL) 
        {
            printf("Error: Could not open file '%s'\n", config.input_file);
            return 1;
        }

        if (config.do_csv == 1) 
        {
            FILE *csv = fopen(config.csv_file, "w");

            if (csv != NULL) 
            {
                fprintf(csv, "sequence,length,A,C,G,T,gc_percent\n");
                fclose(csv);
            }
        }

        while (fgets(sequence, sizeof(sequence), file) != NULL) 
        {
            sequence[strcspn(sequence, "\n")] = '\0';
            process_sequence(sequence, config);
        }

        fclose(file);
    } 
    else 
    {
        int use_stdin = 0;

        if (config.stdin_mode == 1) 
        {
            use_stdin = 1;
        } 
        else 
        {
#ifdef _WIN32
            if (!isatty(_fileno(stdin))) 
            {
                use_stdin = 1;
            }
#else
            if (!isatty(fileno(stdin))) 
            {
                use_stdin = 1;
            }
#endif
        }

        if (use_stdin) 
        {
            while (fgets(sequence, sizeof(sequence), stdin) != NULL) 
            {
                sequence[strcspn(sequence, "\n")] = '\0';
                process_sequence(sequence, config);
            }
        } 
        else 
        {
            printf("\nPlease enter a DNA sequence:\n\n");

            if (fgets(sequence, sizeof(sequence), stdin) == NULL) 
            {
                printf("Error: Failed to read input.\n");
                return 1;
            }

            sequence[strcspn(sequence, "\n")] = '\0';

            process_sequence(sequence, config);
        }
    }

    if (config.do_benchmark == 1) 
    {
        end_time = clock();

        double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        printf("\n=== Benchmark ===\n\n");
        printf("Analysis took %.6f seconds.\n\n", elapsed);
    }

    return 0;
}
