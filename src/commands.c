#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../incs/commands.h"
#include "../incs/trie.h"
#include "../incs/file_utils.h"

// Estructura auxiliar para almacenar informacion de cada gen encontrado
typedef struct GeneInfo {
    char *gene;           // String del gen (ej: "AA", "ACGT")
    int frequency;        // Cuantas veces aparece en la secuencia
    List *positions;      // Puntero a la lista de posiciones
} GeneInfo;

// Procesa el comando ingresado por el usuario: "start <m>", etc.
// En src/commands.c

void process_command(BioSystem *sys, const char *input) {

    char arg[128]; // Argumento para 'read' y 'search'
    int m;

    // 
    if (sscanf(input, "start %d", &m) == 1) {
        cmd_start(sys, m);
    }

    // Comando 'exit'
    else if (strncmp(input, "exit", 4) == 0) {
        cmd_exit(sys);
    }
    // Comando 'help'
    else if (strncmp(input, "help", 4) == 0) {
        cmd_help();
    }
    // --- Comandos que SÍ necesitan que 'start' se haya ejecutado ---
    else if (sscanf(input, "read %127s", arg) == 1) {
        if (sys->root == NULL) {
            printf("Error: Tree has not been initialized. Use 'start <m>'\n");
        } else {
            cmd_read(sys, arg);
        }
    }
    else if (sscanf(input, "search %127s", arg) == 1) {
        if (sys->root == NULL) {
            printf("Error: Tree has not been initialized. Use 'start <m>'\n");
        } else {
            cmd_search(sys, arg);
        }
    }
    else if (strncmp(input, "all", 3) == 0) {
        if (sys->root == NULL) {
            printf("Error: Tree has not been initialized. Use 'start <m>'\n");
        } else {
            cmd_all(sys);
        }
    }
    else if (strncmp(input, "max", 3) == 0) {
        if (sys->root == NULL) {
            printf("Error: Tree has not been initialized. Use 'start <m>'\n");
        } else {
            cmd_max(sys);
        }
    }
    else if (strncmp(input, "min", 3) == 0) {
        if (sys->root == NULL) {
            printf("Error: Tree has not been initialized. Use 'start <m>'\n");
        } else {
            cmd_min(sys);
        }
    } 
    // Si no coincide con nada
    else {
        printf("Unknown command. Type 'bio help' to see available commands.\n");
    }
}

//-------------------------------------------------------------------------------------------------------------------
// Implementa el comando "exit" para liberar memoria y salir del programa
void cmd_exit(BioSystem *sys) {
    free_bio_system(sys);
    printf("Clearing cache and exiting...\n");
    exit(0);
}

//-------------------------------------------------------------------------------------------------------------------
// Muestra la ayuda con los comandos disponibles
void cmd_help(void) {
    printf("\033[0;32m\nAvailable Commands:\n\033[0m");
    printf("  exit          -> Exit the program and clear memory.\n");
    printf("  start <m>     -> Create trie of height m\n");
    printf("  read <file>   -> Loads the sequence S from <file> (ej: adn.txt or data/adn.txt).\n");
    printf("  search <gen>  -> Searches for <gen> in the sequence S and prints its positions.\n");
    printf("  all           -> Displays all genes found and their positions.\n");
    printf("  max           -> Displays the most frequent gene(s) and their positions.\n");
    printf("  min           -> Displays the least frequent gene(s) and their positions.\n");
}

//-------------------------------------------------------------------------------------------------------------------
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
        printf("Tree created with height: %d\n", m);
    else
        printf("Error creating tree.\n");
}

//-------------------------------------------------------------------------------------------------------------------
// Leer una secuencia desde un archivo (Nicolas)
void cmd_read(BioSystem *sys, const char *filename) {
    // Leer el archivo
    char *sequence = read_sequence(filename); // De file_utils.c
    if (!sequence) {
        printf("Error: Could not read file '%s'\n", filename);
        return;
    }

    // Validar la secuencia
    if (!validate_sequence(sequence)) { // De file_utils.c
        printf("Error: Sequence '%s' contains invalid characters.\n", filename);
        free(sequence);
        return;
    }

    // Iterar y poblar el árbol con genes y sus posiciones 
    int n = strlen(sequence);
    int m = sys->gene_length;

    // Ajuste por si el archivo incluye un salto de línea al final 
    if (n > 0 && sequence[n - 1] == '\n') {
        sequence[n - 1] = '\0'; // Lo convertimos en fin de string
        n--; // Reducimos la longitud
    }

    if (n < m) {
        printf("Error: Sequence (length %d) is shorter than gene length (%d).\n", n, m);
        free(sequence);
        return;
    }

    // El PDF dice iterar para los n-m genes [cite: 57]
    for (int i = 0; i <= n - m; i++) {
        // Obtenemos un puntero al inicio del gen i-ésimo
        char *gen_segment = &sequence[i]; 

        // Descendemos en el árbol para encontrar la hoja
        TrieNode *leaf = get_leaf_node(sys->root, gen_segment, m);

        // Si la hoja existe y tiene una lista...
        if (leaf && leaf->positions) {
            // Guardamos la posición 'i' en la lista
            add_position(leaf->positions, i);
        } else {
            printf("Warning: Could not insert gene at position %d.\n", i);
        }
    }

    // 4. Liberar memoria y confirmar
    free(sequence); // Ya no necesitamos el string gigante
    printf("Sequence S read from file %s\n", filename); // [cite: 97]
}

//-------------------------------------------------------------------------------------------------------------------
// Buscar un gen en la secuencia (Matias implementado)
void cmd_search(BioSystem *sys, const char *gene) {

    // Validar que el gen tenga la longitud correcta
    if (strlen(gene) != (size_t)sys->gene_length) { 
        printf("Error: Gene must be of length %d. Gene '%s' has length %zu.\n", 
               sys->gene_length, gene, strlen(gene));
        return;
    }

    // Descendemos en el árbol para encontrar la hoja del gen
    TrieNode *leaf = get_leaf_node(sys->root, gene, sys->gene_length);

    // Si la hoja no existe o no tiene posiciones, imprimimos -1
    if (leaf == NULL || leaf->positions == NULL || leaf->positions->count == 0) {
        printf("-1\n");

    } else {
        // Recorremos la lista de posiciones e imprimimos
        Node* current_pos = leaf->positions->head;
        
        // Iteramos sobre la lista enlazada de posiciones
        while (current_pos != NULL) {
            // Imprime la posición (ej. 4)
            printf("%d", current_pos->pos);
            
            // Imprime un espacio si no es la última posición
            if (current_pos->next != NULL) {
                printf(" ");
            }
            
            // Avanzamos al siguiente nodo
            current_pos = current_pos->next;
        }
        
        // Imprime un salto de línea al final
        printf("\n");
    }
}

//-------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------
// Funcion auxiliar recursiva para recorrer el arbol y recolectar todos los genes (Carlos)
//-------------------------------------------------------------------------------------------------------------------
void collect_genes_recursive(
    TrieNode *node,           // Nodo actual en el recorrido
    int depth,                // Profundidad total del arbol (tamaño del gen)
    int current_depth,        // Profundidad actual en el recorrido
    char *gene_buffer,        // Buffer para construir el string del gen
    GeneInfo **genes_array,   // Array dinamico para almacenar genes encontrados
    int *count,               // Contador de genes encontrados
    int *capacity             // Capacidad actual del array
) {
    // CASO BASE: Llegamos a una hoja (profundidad completa)
    if (current_depth == depth) {
        // Verificar si esta hoja tiene posiciones (el gen existe en la secuencia)
        if (node->positions && node->positions->count > 0) {
            // Terminar el string del gen
            gene_buffer[current_depth] = '\0';
            
            // Expandir el array si esta lleno
            if (*count >= *capacity) {
                *capacity *= 2;  // Duplicar capacidad
                *genes_array = realloc(*genes_array, (*capacity) * sizeof(GeneInfo));
                if (!(*genes_array)) {
                    printf("Error: Memory reallocation failed.\n");
                    return;
                }
            }
            
            // Guardar la informacion del gen en el array
            (*genes_array)[*count].gene = malloc(depth + 1);
            if (!(*genes_array)[*count].gene) {
                printf("Error: Memory allocation failed for gene string.\n");
                return;
            }
            strcpy((*genes_array)[*count].gene, gene_buffer);
            (*genes_array)[*count].frequency = node->positions->count;
            (*genes_array)[*count].positions = node->positions;
            (*count)++;
        }
        return;  // Salir de la recursion
    }
    
    // CASO RECURSIVO: Explorar los 4 posibles hijos
    
    // Explorar hijo A
    if (node->hijoA) {
        gene_buffer[current_depth] = 'A';
        collect_genes_recursive(node->hijoA, depth, current_depth + 1, gene_buffer, genes_array, count, capacity);
    }
    
    // Explorar hijo C
    if (node->hijoC) {
        gene_buffer[current_depth] = 'C';
        collect_genes_recursive(node->hijoC, depth, current_depth + 1, gene_buffer, genes_array, count, capacity);
    }
    
    // Explorar hijo G
    if (node->hijoG) {
        gene_buffer[current_depth] = 'G';
        collect_genes_recursive(node->hijoG, depth, current_depth + 1, gene_buffer, genes_array, count, capacity);
    }
    
    // Explorar hijo T
    if (node->hijoT) {
        gene_buffer[current_depth] = 'T';
        collect_genes_recursive(node->hijoT, depth, current_depth + 1, gene_buffer, genes_array, count, capacity);
    }
}

// Mostrar todos los genes encontrados (Sebastian)
void cmd_all(BioSystem *sys) {
    // Verifica que el sistema este inicializado
    if (sys->root == NULL) {
        printf("Error: Tree has not been initialized. Use 'start <m>'\n");
        return;
    }
    
    // Prepara estructuras para recolectar informacion
    int capacity = 16;     // Capacidad inicial
    int count = 0;         // Contador de genes encontrados
    
    GeneInfo *genes_array = malloc(capacity * sizeof(GeneInfo));
    if (!genes_array) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    char *gene_buffer = malloc(sys->gene_length + 1);
    if (!gene_buffer) {
        free(genes_array);
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    // Llena el array con todos los genes
    collect_genes_recursive(sys->root, sys->gene_length, 0, gene_buffer, &genes_array, &count, &capacity);
    
    // Verifica si se encontraron genes
    if (count == 0) {
        printf("No genes found in sequence.\n");
        free(gene_buffer);
        free(genes_array);
        return;
    }
    
    for (int i = 0; i < count; i++) {
        // Imprime el nombre del gen
        printf("%s", genes_array[i].gene); 
            
        // Prepara arreglo temporal para ordenar posiciones
        int *positions_array = malloc(genes_array[i].frequency * sizeof(int));
        if (!positions_array) {
            printf("\nError: Memory allocation failed for positions.\n");
            continue; 
        }
            
        // Copia posiciones de la lista al array
        Node *current = genes_array[i].positions->head;
        int pos_count = 0;
        while (current != NULL) {
            positions_array[pos_count++] = current->pos;
            current = current->next;
        }
            
        // Ordena las posiciones usando bubble sort
        for (int j = 0; j < pos_count - 1; j++) {
            for (int k = 0; k < pos_count - j - 1; k++) {
                if (positions_array[k] > positions_array[k + 1]) {
                    int temp = positions_array[k];
                    positions_array[k] = positions_array[k + 1];
                    positions_array[k + 1] = temp;
                }
            }
        }
            
        // Imprime las posiciones ordenadas (ej: " 4 7")
        for (int j = 0; j < pos_count; j++) {
            printf(" %d", positions_array[j]);
        }
        printf("\n");
            
        // Libera array temporal de posiciones
        free(positions_array);
    }
    
    // Libera toda la memoria asignada
    for (int i = 0; i < count; i++) {
        free(genes_array[i].gene);
    }
    free(genes_array);
    free(gene_buffer);
}



//-------------------------------------------------------------------------------------------------------------------
// Mostrar el gen mas frecuente (Carlos)
//-------------------------------------------------------------------------------------------------------------------
void cmd_max(BioSystem *sys) {
    // Verificar que el sistema este inicializado
    if (sys->root == NULL) {
        printf("Error: Tree has not been initialized. Use 'start <m>'\n");
        return;
    }
    
    // Preparar estructuras para recolectar informacion
    int capacity = 16;     // Capacidad inicial (se expandira si es necesario)
    int count = 0;         // Contador de genes encontrados
    
    // Crear array dinamico para almacenar informacion de genes
    GeneInfo *genes_array = malloc(capacity * sizeof(GeneInfo));
    if (!genes_array) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    // Crear buffer para construir strings de genes durante el recorrido
    char *gene_buffer = malloc(sys->gene_length + 1);
    if (!gene_buffer) {
        free(genes_array);
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    // Recorrer el arbol y recolectar todos los genes
    collect_genes_recursive(sys->root, sys->gene_length, 0, gene_buffer, &genes_array, &count, &capacity);
    
    // Verificar si se encontraron genes
    if (count == 0) {
        printf("No genes found in sequence.\n");
        free(gene_buffer);
        free(genes_array);
        return;
    }
    
    // Encontrar la frecuencia maxima
    int max_frequency = 0;
    for (int i = 0; i < count; i++) {
        if (genes_array[i].frequency > max_frequency) {
            max_frequency = genes_array[i].frequency;
        }
    }
    
    // Imprimir todos los genes con frecuencia maxima
    for (int i = 0; i < count; i++) {
        if (genes_array[i].frequency == max_frequency) {
            printf("%s", genes_array[i].gene); // Imprimir el nombre del gen
            
            // Preparar array temporal para ordenar posiciones
            // (Las posiciones están en orden inverso en la lista enlazada)
            int *positions_array = malloc(genes_array[i].frequency * sizeof(int));
            if (!positions_array) {
                printf("\nError: Memory allocation failed for positions.\n");
                continue;  // Saltar este gen
            }
            
            // Copiar posiciones de la lista al array
            Node *current = genes_array[i].positions->head;
            int pos_count = 0;
            while (current != NULL) {
                positions_array[pos_count++] = current->pos;
                current = current->next;
            }
            
            // Ordenar las posiciones usando bubble sort (mismo código de ordenamiento que usó Carlos)
            for (int j = 0; j < pos_count - 1; j++) {
                for (int k = 0; k < pos_count - j - 1; k++) {
                    if (positions_array[k] > positions_array[k + 1]) {
                        int temp = positions_array[k];
                        positions_array[k] = positions_array[k + 1];
                        positions_array[k + 1] = temp;
                    }
                }
            }
            
            // Imprimir las posiciones ordenadas
            for (int j = 0; j < pos_count; j++) {
                printf(" %d", positions_array[j]);
            }
            printf("\n");
            
            // Liberar array temporal de posiciones
            free(positions_array);
        }
    }
    
    // Liberar toda la memoria asignada
    for (int i = 0; i < count; i++) {
        free(genes_array[i].gene);
    }
    free(genes_array);
    free(gene_buffer);
}

//-------------------------------------------------------------------------------------------------------------------  
// Mostrar el gen menos frecuente (Christoffer)
void cmd_min(BioSystem *sys) {
    // Verificar que el sistema este inicializado
    if (sys->root == NULL) {
        printf("Error: Tree has not been initialized. Use 'start <m>'\n");
        return;
    }
    
    // Preparar estructuras para recolectar informacion
    int capacity = 16;     // Capacidad inicial (se expandira si es necesario)
    int count = 0;         // Contador de genes encontrados
    
    // Crear array dinamico para almacenar informacion de genes
    GeneInfo *genes_array = malloc(capacity * sizeof(GeneInfo));
    if (!genes_array) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    // Crear buffer para construir strings de genes durante el recorrido
    char *gene_buffer = malloc(sys->gene_length + 1);
    if (!gene_buffer) {
        free(genes_array);
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    // Recorrer el arbol y recolectar todos los genes
    collect_genes_recursive(sys->root, sys->gene_length, 0, gene_buffer, &genes_array, &count, &capacity);
    
    // Verificar si se encontraron genes
    if (count == 0) {
        printf("No genes found in sequence.\n");
        free(gene_buffer);
        free(genes_array);
        return;
    }
    
    // ------------------- INICIO DEL CAMBIO -------------------
    // Encontrar la frecuencia minima
    // (Se garantiza que count > 0 gracias al 'if' anterior)
    
    int min_frequency = genes_array[0].frequency;
    
    for (int i = 1; i < count; i++) { // Empezar en 1, ya que usamos el 0 para inicializar
        if (genes_array[i].frequency < min_frequency) {
            min_frequency = genes_array[i].frequency;
        }
    }
    
    // Imprimir todos los genes con frecuencia minima
    for (int i = 0; i < count; i++) {
        // Este es el segundo cambio: '==' min_frequency
        if (genes_array[i].frequency == min_frequency) {
            // ------------------- FIN DEL CAMBIO -------------------

            printf("%s", genes_array[i].gene); // Imprimir el nombre del gen
            
            // Preparar array temporal para ordenar posiciones
            // (Las posiciones están en orden inverso en la lista enlazada)
            int *positions_array = malloc(genes_array[i].frequency * sizeof(int));
            if (!positions_array) {
                printf("\nError: Memory allocation failed for positions.\n");
                continue;  // Saltar este gen
            }
            
            // Copiar posiciones de la lista al array
            Node *current = genes_array[i].positions->head;
            int pos_count = 0;
            while (current != NULL) {
                positions_array[pos_count++] = current->pos;
                current = current->next;
            }
            
            // Ordenar las posiciones usando bubble sort
            // (Este es el mismo código de ordenamiento que usó Carlos)
            for (int j = 0; j < pos_count - 1; j++) {
                for (int k = 0; k < pos_count - j - 1; k++) {
                    if (positions_array[k] > positions_array[k + 1]) {
                        int temp = positions_array[k];
                        positions_array[k] = positions_array[k + 1];
                        positions_array[k + 1] = temp;
                    }
                }
            }
            
            // Imprimir las posiciones ordenadas
            for (int j = 0; j < pos_count; j++) {
                printf(" %d", positions_array[j]);
            }
            printf("\n");
            
            // Liberar array temporal de posiciones
            free(positions_array);
        }
    }
    
    // Liberar toda la memoria asignada
    for (int i = 0; i < count; i++) {
        free(genes_array[i].gene);
    }
    free(genes_array);
    free(gene_buffer);
}
