#include <stdlib.h>
#include <stdio.h>
#include "../incs/bio.h"

// Inicializa el sistema biológico
void init_bio_system(BioSystem *sys) {
    sys->root = NULL; // Puntero raíz inicializado a NULL
    sys->gene_length = 0; // Longitud del gen inicializada a 0
}

// Libera los recursos del sistema biológico
void free_bio_system(BioSystem *sys) {
    if (sys->root) {
        // Liberar el trie
        free_trie(sys->root, sys->gene_length);
        // Establecer el puntero raíz a NULL
        sys->root = NULL;
    }
}
