#include <stdio.h>
#include <string.h>
#include "parser.h"

#define MAX_ENTRADA 1024

// funcion q usaremos 
void quitar_salto_de_linea(char *s);

int main(){
    
    char entrada[MAX_ENTRADA];

    while(1){

        
        printf("miShell> ");
        fflush(stdout); // fuerza a mostrar el promt

        if(fgets(entrada,sizeof(entrada),stdin) == NULL){
            printf("\n");
            break; //seria el EOF : ctrl+z y enter
        }

        quitar_salto_de_linea(entrada);
        t_comando *cmd = parsear_linea(entrada);

        //error de sintactico
        if(cmd == NULL) 
            continue;

        //entrada vacia
        if(cmd->argc == 0){
            libera_comando(cmd);
            continue;
        };

        if(strcmp(cmd->argv[0], "exit") == 0){
            libera_comando(cmd);
            break;
        };

        imprimir_comando(cmd);
        libera_comando(cmd);

    };

    return 0;

}

//cuerpito de la funcion
void quitar_salto_de_linea(char *s){

    s[ strcspn(s,"\r\n") ] = '\0';

}