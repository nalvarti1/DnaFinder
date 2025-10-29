#ifndef BIO_H
#define BIO_H

#include "trie.h"

// Estructura del sistema biológico
typedef struct BioSystem {
    TrieNode *root;
    int gene_length;
} BioSystem;

void init_bio_system(BioSystem *sys); // Inicializa el sistema biológico
void free_bio_system(BioSystem *sys); // Libera los recursos del sistema biológico

#endif
