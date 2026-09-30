#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "builtins.h"
#include "shell.h"

#define MAX_ENTRADA 1024
#define TAM_RUTA 1024

// funcion q usaremos 
void quitar_salto_de_linea(char *s);
void mostrar_prompt(void);


int main(){
    
    char entrada[MAX_ENTRADA];
    t_shell shell = { .ejecutando = 1, .ultimo_estado = 0 };  // Inicializa cada campo por su nombre

    while (shell.ejecutando) {
        mostrar_prompt();

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            printf("\n");
            break;  // EOF: Ctrl+Z y Enter en Windows
        }

        quitar_salto_de_linea(entrada);

        t_comando *cmd = parsear_linea(entrada);
        if (cmd == NULL) {
            shell.ultimo_estado = 2;  // Error de sintaxis (el parser ya lo informó)
            continue;
        }

        if (cmd->argc == 0) {
            liberar_comando(cmd);
            continue;
        }

        if (es_builtin(cmd->argv[0])) {
            shell.ultimo_estado = ejecutar_builtin(cmd, &shell);
        } else {
            fprintf(stderr, "%s: comando no encontrado\n", cmd->argv[0]);
            shell.ultimo_estado = 127;
        }

        liberar_comando(cmd);
    }

    return shell.ultimo_estado;
}

//cuerpos de las funciones :

void quitar_salto_de_linea(char *s){

    s[ strcspn(s,"\r\n") ] = '\0';

}

void mostar_prompt(void){
    char ruta[TAM_RUTA];

    if(getcwd(ruta,sizeof(ruta)) != NULL){
        printf("miShell %s> ", ruta);
    }else{
        printf("miShell> ");
    }

    fflush(stdout);
}
