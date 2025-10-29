#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../incs/commands.h"

// Procesa el comando ingresado por el usuario: "start <m>", etc.
void process_command(BioSystem *sys, const char *input) {
    char cmd[16];
    int m;

    if (sscanf(input, "start %d", &m) == 1) {
        cmd_start(sys, m);
    } else {
        printf("Unknown command.\n");
    }
}

// Implementa el comando "start <m>" para crear un trie de altura m
void cmd_start(BioSystem *sys, int m) {
    if (m <= 0) {
        printf("Error: gene length must be greater than 0.\n");
        return;
    }

    if (sys->root != NULL) {
        free_trie(sys->root, sys->gene_length);
        sys->root = NULL;
    }

    sys->root = create_trie(m);
    sys->gene_length = m;

    if (sys->root)
        printf("Tree created with height %d\n", m);
    else
        printf("Error creating tree.\n");
}
