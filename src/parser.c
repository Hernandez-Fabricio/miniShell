#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define CAPACIDAD_INICIAL 8

// funciones axiliares -> definidas aca para proteger el estado interno y evitar choques de los nombres

static t_comando *crear_comando(void);
static int agregar_token(t_comando *cmd, const char *inicio, size_t largo);
static int es_separador(char c);

// su cuerpo :

//==========================================================================================//
static t_comando *crear_comando(void){ //inicializo el comando

    t_comando *cmd = malloc(sizeof(t_comando));

    if(cmd == NULL)
        return NULL;

    cmd -> argv = malloc(CAPACIDAD_INICIAL * sizeof(char *));  // Reservo el arreglo de punteros

    if(cmd -> argv == NULL){
        free(cmd);            
        return NULL;
    }

    cmd -> argc = 0;
    cmd -> capacidad = CAPACIDAD_INICIAL;
    cmd -> argv[0] = NULL;

    return cmd;

}
//==========================================================================================//
static int agregar_token(t_comando *cmd, const char *inicio, size_t largo){

    char *copia = malloc(largo + 1);
    
    if((cmd -> argc + 2) > (cmd -> capacidad )){
        int nueva_capacidad = cmd -> capacidad * 2;
        char **tmp = realloc(cmd -> argv , nueva_capacidad * sizeof(char*));
        
        if(tmp == NULL)
            return -1;
        
        cmd -> argv = tmp;
        cmd -> capacidad = nueva_capacidad;
    };

    if( copia == NULL )
        return -1;

    memcpy(copia, inicio, largo);
    copia[largo] = '\0';

    cmd -> argv[cmd -> argc] = copia; // agregamos el token
    cmd -> argc++;                    
    cmd -> argv[cmd -> argc] = NULL;  // el null siempre al final

    return 0;

}
//==========================================================================================//
static int es_separador(char c){
    return (c == ' ' || c == '\t');
}
//==========================================================================================//

// cuerpo de las funciones de parser.h :

//==========================================================================================//
t_comando *parsear_linea(const char *linea){

    t_comando *cmd = crear_comando();
    char *buffer = malloc(strlen(linea) + 1); // buffer temporal donde se arma cada token antes de copiarlo.
    const char *p = linea; // puntero que recorre la linea caracter a caracter 

    if ( linea == NULL )
        return NULL;

    if( cmd == NULL ){
        fprintf(stderr, "miniShell : sin memoria\n");
        return NULL;
    };

    if( buffer == NULL){
        fprintf(stderr, "miniShell : sin memoria\n");
        libera_comando(cmd);
        return NULL;
    };

    while(1){
        size_t largo = 0;
        int en_comillas = 0;

        //paso 1 : saltear los separadores 
        while(es_separador(*p)){
            p++;
        };

        //paso 2 : si llegamos al final -> no hay tokens 
        if(*p == '\0')
            break;

        //paso 3 : armar el token en el buffer 
        while((*p != '\0') && (en_comillas || !es_separador(*p))){

            if(*p == '"'){
                en_comillas = !en_comillas;
            }else{
                buffer[largo] = *p;
                largo++;
            };
            p++;
        };

        if(en_comillas){
            fprintf(stderr, "miniShell : error de sintaxis : comillas sin cerrar\n");
            free(buffer);
            libera_comando(cmd);
            return NULL;
        };

        //paso 4 : guardar uuna copia del token
        if(agregar_token(cmd,buffer,largo) != 0){
            fprintf(stderr, "miniShell : sin memoria\n");
            free(buffer);
            libera_comando(cmd);
            return NULL;
        };

    };

    free(buffer);
    return cmd;

}
//==========================================================================================//
void libera_comando(t_comando *cmd){

    if( cmd == NULL)
        return;

    for(int i = 0 ; i < (cmd->argc) ; i++){
        free(cmd->argv[i]);
    };

    free(cmd->argv);
    free(cmd);
    
}
//==========================================================================================//
void imprimir_comando(const t_comando *cmd){

    if(cmd == NULL)
        return;

    printf("argc = %d\n", cmd-> argc);

    for(int i = 0 ; i < (cmd -> argc); i++){
        printf(" argv[%d] = [%s]\n", i , cmd->argv[i] );
    }

    printf(" argv[%d] = NULL\n", cmd->argc );

}
//==========================================================================================//
