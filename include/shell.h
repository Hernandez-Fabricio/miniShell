#ifndef SHELL_H
#define SHELL_H

// los estados de la shell -> en ejec :1 pa seguir y 0 pa terminar -> en ult_est : salida de comando ( 0 pa exito )

typedef struct{
    int ejecutando;
    int ultimo_estado;
}t_shell;


#endif