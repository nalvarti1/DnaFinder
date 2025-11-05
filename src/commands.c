#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../incs/commands.h"
#include "../incs/trie.h"
#include "../incs/file_utils.h"

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
            printf("Error: El árbol no ha sido inicializado. Use 'start <m>'\n");
        } else {
            cmd_read(sys, arg);
        }
    }
    else if (sscanf(input, "search %127s", arg) == 1) {
        if (sys->root == NULL) {
            printf("Error: El árbol no ha sido inicializado. Use 'start <m>'\n");
        } else {
            cmd_search(sys, arg);
        }
    }
    else if (strncmp(input, "all", 3) == 0) {
        if (sys->root == NULL) {
            printf("Error: El árbol no ha sido inicializado. Use 'start <m>'\n");
        } else {
            cmd_all(sys);
        }
    }
    else if (strncmp(input, "max", 3) == 0) {
        if (sys->root == NULL) {
            printf("Error: El árbol no ha sido inicializado. Use 'start <m>'\n");
        } else {
            cmd_max(sys);
        }
    }
    else if (strncmp(input, "min", 3) == 0) {
        if (sys->root == NULL) {
            printf("Error: El árbol no ha sido inicializado. Use 'start <m>'\n");
        } else {
            cmd_min(sys);
        }
    } 
    // Si no coincide con nada
    else {
        printf("Comando desconocido. Escriba 'bio help' para ver los comandos disponibles.\n");
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
    printf("\033[0;32m\nAvailable commands:\n\033[0m");
    printf("  exit          -> Exit the program and clear memory.\n");
    printf("  start <m>     -> Create trie of height m\n");
    printf("  read <file>   -> Loads the sequence S from <file> (ej: adn.txt).\n");
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
        printf("Tree created with height %d\n", m);
    else
        printf("Error creating tree.\n");
}

//-------------------------------------------------------------------------------------------------------------------
// Leer una secuencia desde un archivo (Nicolas)
void cmd_read(BioSystem *sys, const char *filename) {
    // Leer el archivo
    char *sequence = read_sequence(filename); // De file_utils.c
    if (!sequence) {
        printf("Error: No se pudo leer el archivo '%s'\n", filename);
        return;
    }

    // Validar la secuencia
    if (!validate_sequence(sequence)) { // De file_utils.c
        printf("Error: La secuencia '%s' contiene caracteres invalidos.\n", filename);
        free(sequence);
        return;
    }

    // Iterar y poblar el árbol
    int n = strlen(sequence);
    int m = sys->gene_length;

    // Ajuste por si el archivo incluye un salto de línea al final
    if (n > 0 && sequence[n - 1] == '\n') {
        sequence[n - 1] = '\0'; // Lo convertimos en fin de string
        n--; // Reducimos la longitud
    }

    if (n < m) {
        printf("Error: La secuencia (largo %d) es mas corta que el gen (largo %d).\n", n, m);
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
            printf("Advertencia: No se pudo insertar el gen en la posicion %d.\n", i);
        }
    }

    // 4. Liberar memoria y confirmar
    free(sequence); // Ya no necesitamos el string gigante
    printf("Sequence S read from file %s\n", filename); // [cite: 97]
}

//-------------------------------------------------------------------------------------------------------------------
// Buscar un gen en la secuencia (Matias implementado)
void cmd_search(BioSystem *sys, const char *gene) {
    // Implementación pendiente
    printf("cmd_search not implemented yet.\n");
}
//-------------------------------------------------------------------------------------------------------------------
// Mostrar todos los genes encontrados (Sebastian no implementado aún)
void cmd_all(BioSystem *sys) {
    // Implementación pendiente
    printf("cmd_all not implemented yet.\n");
}

//-------------------------------------------------------------------------------------------------------------------
// Mostrar el gen más frecuente (Carlos no implementado aún)
void cmd_max(BioSystem *sys) {
    // Implementación pendiente
    printf("cmd_max not implemented yet.\n");
}

//-------------------------------------------------------------------------------------------------------------------  
// Mostrar el gen menos frecuente (Christoffer no implementado aún)
void cmd_min(BioSystem *sys) {
    // Implementación pendiente
    printf("cmd_min not implemented yet.\n");
}