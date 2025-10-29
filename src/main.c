#include <stdio.h>
#include <string.h>
#include "../incs/commands.h"

int main(void) {
    BioSystem sys;
    init_bio_system(&sys);

    char input[128];
    printf("=== BioFinder ===\n");
    // Instrucciones para el usuario, cuando se implementen más comandos se agregaran aquí
    printf("Commands available:\n");
    printf("  >bio start <m>   -> Create trie of height m\n");
    printf("  >bio exit        -> Exit program\n\n");

    // Bucle principal para leer y procesar comandos
    while (1) {
        printf("\033[0;32m>bio \033[0m");
        if (!fgets(input, sizeof(input), stdin))
            break;

        // Comando para salir del programa
        if (strncmp(input, "exit", 4) == 0)
            break;

        // Llamada a la función para procesar el comando
        process_command(&sys, input);
    }

    // Liberar recursos antes de salir
    free_bio_system(&sys);
    printf("Clearing cache and exiting...\n");
    return 0;
}
