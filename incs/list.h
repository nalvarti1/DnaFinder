#ifndef LIST_H
#define LIST_H

// Estructura de un nodo de la lista enlazada
typedef struct Node {
    int pos;
    struct Node *next;
} Node;

// Estructura de la lista enlazada
typedef struct List {
    Node *head;
    int count;
} List;

List *create_list(void); // Crea una nueva lista enlazada vacía
void add_position(List *list, int pos); // Agrega una posición a la lista enlazada
void free_list(List *list); // Libera la memoria utilizada por la lista enlazada

#endif
