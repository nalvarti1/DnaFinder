#include <stdlib.h>
#include "../incs/list.h"

// Crea una nueva lista enlazada vacía
List *create_list(void) {
    List *list = malloc(sizeof(List));
    if (!list) return NULL;
    list->head = NULL;
    list->count = 0;
    return list;
}

// Agrega una posición a la lista enlazada
void add_position(List *list, int pos) {
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) return;
    new_node->pos = pos;
    new_node->next = list->head;
    list->head = new_node;
    list->count++;
}

// Libera la memoria utilizada por la lista enlazada
void free_list(List *list) {
    Node *current = list->head;
    while (current) {
        Node *tmp = current;
        current = current->next;
        free(tmp);
    }
    free(list);
}
