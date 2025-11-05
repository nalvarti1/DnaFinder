#ifndef COMMANDS_H
#define COMMANDS_H

#include "bio.h"

void process_command(BioSystem *sys, const char *input); // Procesa el comando ingresado por el usuario: "start <m>", etc.
void cmd_exit(BioSystem *sys); // Implementa el comando "exit" para salir del programa
void cmd_help(void); // Comando "help" muestra comandos disponibles
void cmd_start(BioSystem *sys, int m); // Implementa el comando "start <m>" para crear un trie de altura m
void cmd_read(BioSystem *sys, const char *filename); // Implementa el comando "read <file>" para leer una secuencia desde un archivo
void cmd_search(BioSystem *sys, const char *gene); // Implementa el comando
void cmd_all(BioSystem *sys); // Comando "all" muestra todos los genes y sus posiciones
void cmd_max(BioSystem *sys); // Comando "max" muestra el/los gen(es) más frecuente(s)
void cmd_min(BioSystem *sys); // Comando "min" muestra el/los gen(es) menos frecuente(s)

#endif
