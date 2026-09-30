#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "builtins.h"

#define TAM_RUTA 1024


// firma comun entre los builtinss -> puntero a funcion es good
typedef int (*t_funcion_builtin)(const t_comando *cmd, t_shell *shell);


// mi tabla de despacho es : 
typedef struct{
    const char *nombre; // lo q escribe el usuario
    t_funcion_builtin funcion; // quien lo resuelve
    const char *descripcion; // lo q mostrar el help
}t_builtin;


//funciones a usar :
static const t_builtin *buscar_builtin(const char *nombre);
static int builtin_exit(const t_comando *cmd, t_shell *shell);
static int builtin_cd(const t_comando *cmd, t_shell *shell);
static int builtin_pwd(const t_comando *cmd, t_shell *shell);
static int builtin_echo(const t_comando *cmd, t_shell *shell);
static int builtin_help(const t_comando *cmd, t_shell *shell);


//la tabla de despacho sera :
static const t_builtin tabla_builtins[] =  {{ "exit" , builtin_exit , "Termina la shell : exit [codigo]" },
                                            { "cd"   , builtin_cd   , "cambia el directorio actual: cd [directorio]"},
                                            { "pwd"  , builtin_pwd  , "Muestra el directorio actual"},
                                            { "echo" , builtin_echo , "Imprime sus argumentos: echo[texto...]"},
                                            { "help" , builtin_help , "Muestra esta ayuda"},
                                            { NULL   , NULL         , NULL }
                                            };


//cuerpo de dichas funciones :
//==========================================================================================//
int es_builtin(const char *nombre){
    return buscar_builtin(nombre) != NULL ;
}
//==========================================================================================//
int ejecutar_builtin(const t_comando *cmd, t_shell *shell){
    const t_builtin *builtin = buscar_builtin(cmd->argv[0]);

    if(builtin == NULL)
        return 1;

    return builtin -> funcion(cmd, shell); 
}
//==========================================================================================//
static const t_builtin *buscar_builtin(const char *nombre){
    if(nombre == NULL)
        return NULL;

    for(int i=0 ; tabla_builtins[i].nombre != NULL ; i++){
        if(strcmp(tabla_builtins[i].nombre, nombre) == 0)
            return &tabla_builtins[i];
    }

    return NULL;
}
//==========================================================================================//
static int builtin_exit(const t_comando *cmd, t_shell *shell){
    int codigo = shell -> ultimo_estado;

    if((cmd -> argc) > 2){
        fprintf(stderr, "exit: demasiados argumentos\n");
        return 1;
    }

    if((cmd -> argc) == 2){
        char *fin;
        errno = 0;
        long valor = strtol(cmd->argv[1],&fin,10);

        if( (fin == (cmd -> argv[1])) || (*fin !='\0') || (errno == ERANGE) ){
            fprintf(stderr, "exit : '%s' no es un numero valido \n", cmd -> argv[1]);
            return 1;
        } 

        codigo = (int) valor; // le cambio el tipo
    }

    shell -> ejecutando = 0; // le avisamo al main q corte el loop
    return codigo;
}
//==========================================================================================//
static int builtin_cd(const t_comando *cmd, t_shell *shell){
    (void) shell; // no lo usaremos -> para evitar el warning nomas

    const char *destino;

    if((cmd -> argc) > 2){
        fprintf(stderr, "cd: demasiados argumentos\n");
        return 1;
    }

    if((cmd -> argc) == 1){
        destino = getenv("USERPROFILE");

        if(destino == NULL)
            destino = getenv("HOME");

        if(destino == NULL){
            fprintf(stderr, "cd: no pudo determinar la carpeta personal\n");
            return 1;
        }
    }else{
        destino = cmd -> argv[1];
    }

    if(chdir(destino) != 0){
        fprintf(stderr, "cd: %s: %s\n", destino, strerror(errno));
        return 1;
    }

    return 0;
}
//==========================================================================================//
static int builtin_pwd(const t_comando *cmd, t_shell *shell){
    (void) cmd;
    (void) shell;

    char ruta[TAM_RUTA];

    if(getcwd(ruta, sizeof(ruta)) == NULL){
        fprintf(stderr, "pwd: %s\n", strerror(errno));
        return 1;
    }

    printf("%s\n", ruta);
    return 0;
}
//==========================================================================================//
static int builtin_echo(const t_comando *cmd, t_shell *shell){
    (void) shell;

    for(int i = 1 ; i < (cmd -> argc) ; i++){
        if(i > 1)
            printf(" ");

        printf("%s", cmd -> argv[i]);
    }

    printf("\n");
    return 0;
}
//==========================================================================================//
static int builtin_help(const t_comando *cmd, t_shell *shell){
    (void) cmd;
    (void) shell;

    printf("Comandos internos de miShell:\n");

    for(int i = 0 ; tabla_builtins[i].nombre != NULL ; i++){
        printf("  %-6s %s\n", tabla_builtins[i].nombre, tabla_builtins[i].descripcion);
    }

    return 0;
}
//==========================================================================================//
