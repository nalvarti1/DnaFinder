#ifndef FILE_UTILS_H
#define FILE_UTILS_H

char *read_sequence(const char *filename); // Lee una secuencia genética desde un archivo
int validate_sequence(const char *seq); // Valida que la secuencia genética contenga solo caracteres A, C, G, T

#endif
