#ifndef TRIE_H
#define TRIE_H

#include "list.h"

// Estructura de un nodo del trie
typedef struct TrieNode {
    struct TrieNode *hijoA;
    struct TrieNode *hijoC;
    struct TrieNode *hijoG;
    struct TrieNode *hijoT;
    List *positions; // se usará en las hojas
} TrieNode;

TrieNode *create_trie(int depth); // Crea un trie de profundidad dada
void free_trie(TrieNode *root, int depth); // Libera la memoria del trie
TrieNode *get_leaf_node(TrieNode *root, const char *gene, int len); // Obtiene el nodo hoja para un gen dado

#endif
