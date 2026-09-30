#ifndef BUILTINS_H
#define BUILTINS_H
#include "parser.h"
#include "shell.h"

//funciones a usar :

int es_builtin(const char *nombre); // devuelve 1 si lo es , 0 si no lo es 

int ejecutar_builtin(const t_comando *cmd, t_shell *shell); // ejecuta de cmd y devuelve 1

#endif