#include <stdlib.h>
#include <stdio.h>
#include "../incs/trie.h"

// Crea un nuevo nodo del trie
TrieNode *create_trie_node(void) {
    TrieNode *node = malloc(sizeof(TrieNode));
    if (!node) return NULL;
    node->hijoA = node->hijoC = node->hijoG = node->hijoT = NULL;
    node->positions = NULL;
    return node;
}

// Crea un trie de profundidad dada
TrieNode *create_trie(int depth) {
    if (depth <= 0) return NULL;
    TrieNode *node = create_trie_node();
    if (depth == 1) return node;

    node->hijoA = create_trie(depth - 1);
    node->hijoC = create_trie(depth - 1);
    node->hijoG = create_trie(depth - 1);
    node->hijoT = create_trie(depth - 1);
    return node;
}

void free_trie(TrieNode *root, int depth) {
    if (!root) return;
    if (depth > 1) {
        free_trie(root->hijoA, depth - 1);
        free_trie(root->hijoC, depth - 1);
        free_trie(root->hijoG, depth - 1);
        free_trie(root->hijoT, depth - 1);
    }
    if (root->positions) free_list(root->positions);
    free(root);
}
