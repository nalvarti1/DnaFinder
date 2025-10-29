#ifndef COMMANDS_H
#define COMMANDS_H

#include "bio.h"

void process_command(BioSystem *sys, const char *input); // Procesa el comando ingresado por el usuario: "start <m>", etc.
void cmd_start(BioSystem *sys, int m); // Implementa el comando "start <m>" para crear un trie de altura m
// Se pueden agregar más declaraciones de funciones para otros comandos aquí
// read, search, max, min, all.

#endif
