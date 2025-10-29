#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../incs/file_utils.h"

// Lee una secuencia genética desde un archivo
char *read_sequence(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    // Obtener tamaño del archivo
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    // Leer contenido del archivo
    char *seq = malloc(size + 1);
    if (!seq) {
        fclose(f);
        return NULL;
    }

    fread(seq, 1, size, f); // Leer el archivo completo
    seq[size] = '\0'; // Null-terminar la cadena
    fclose(f); // Cerrar el archivo
    return seq;
}

// Valida que la secuencia genética contenga solo caracteres A, C, G, T
int validate_sequence(const char *seq) {
    for (int i = 0; seq[i]; i++) {
        if (seq[i] != 'A' && seq[i] != 'C' && seq[i] != 'G' && seq[i] != 'T' && seq[i] != '\n')
            return 0;
    }
    return 1;
}
