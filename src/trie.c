// En src/trie.c

#include <stdlib.h>
#include <stdio.h>
#include "../incs/trie.h"

// Crea un solo nodo, inicializando todo en NULL
TrieNode *create_trie_node(void) {
    TrieNode *node = malloc(sizeof(TrieNode));
    if (!node) return NULL;
    node->hijoA = NULL;
    node->hijoC = NULL;
    node->hijoG = NULL;
    node->hijoT = NULL;
    node->positions = NULL; // <-- Se inicializa en NULL
    return node;
}

// Crea el árbol recursivamente
TrieNode *create_trie(int depth) {

    // --- ESTE ES EL CAMBIO CLAVE ---
    // Caso Base: (depth == 0). Este es un nodo HOJA.
    if (depth == 0) {
        TrieNode *leafNode = create_trie_node();
        if (!leafNode) return NULL;
        
        //printf("DEBUG: Creando lista en nodo hoja (depth=0).\n"); // DEBUG line
        leafNode->positions = create_list(); 

        if (!leafNode->positions) { 
            free(leafNode);
            return NULL;
        }
        return leafNode; // Devuelve la hoja
    }

    // Caso Recursivo: (depth > 0). Este es un nodo INTERNO.
    TrieNode *internalNode = create_trie_node();
    if (!internalNode) return NULL;

    // Los nodos internos no tienen lista, pero SÍ tienen hijos.
    internalNode->hijoA = create_trie(depth - 1);
    internalNode->hijoC = create_trie(depth - 1);
    internalNode->hijoG = create_trie(depth - 1);
    internalNode->hijoT = create_trie(depth - 1);

    return internalNode;
}

// Libera el árbol (Versión más robusta)
void free_trie(TrieNode *root, int depth) {
    // (El parámetro 'depth' ya no es necesario, pero lo dejamos por consistencia)
    if (!root) return;
    
    // Primero, libera a todos los hijos
    free_trie(root->hijoA, 0); // El '0' aquí no importa
    free_trie(root->hijoC, 0);
    free_trie(root->hijoG, 0);
    free_trie(root->hijoT, 0);
    
    // Si este nodo tiene una lista (es una hoja), libérala.
    if (root->positions) {
        free_list(root->positions);
    }
    
    // Finalmente, libérate a ti mismo
    free(root);
}

// Función auxiliar para navegar el árbol (Esta estaba bien)
TrieNode *get_leaf_node(TrieNode *root, const char *gene, int len) {
    TrieNode *current = root;

    for (int i = 0; i < len; i++) {
        if (current == NULL) return NULL; 

        switch (gene[i]) {
            case 'A': current = current->hijoA; break;
            case 'C': current = current->hijoC; break;
            case 'G': current = current->hijoG; break;
            case 'T': current = current->hijoT; break;
            default:
                return NULL; 
        }
    }
    return current;
}